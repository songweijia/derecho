#pragma once

#include "../subgroup_caller.hpp"

namespace derecho {

template <typename T>
SubgroupCaller<T>::SubgroupCaller(uint32_t type_id, node_id_t nid, subgroup_id_t subgroup_id, shard_t shard_num,
                                  rpc::RPCManager& group_rpc_manager)
        : node_id(nid),
          subgroup_id(subgroup_id),
          group_rpc_manager(group_rpc_manager),
          wrapped_this(rpc::make_remote_invoker<T>(nid, type_id, subgroup_id,
                                                                T::register_functions(), *group_rpc_manager.receivers)) {}

template <typename T>
template <rpc::FunctionTag tag, typename... Args>
auto SubgroupCaller<T>::in_subgroup_ordered_send(Args&&... args) {
    if(is_valid()) {
        size_t payload_size_for_multicast_send = wrapped_this->template get_size_for_ordered_send<tag>(std::forward<Args>(args)...);

        using Ret = typename std::remove_pointer<decltype(wrapped_this->template getReturnType<tag>(
                std::forward<Args>(args)...))>::type;
        rpc::QueryResults<Ret>* results_ptr;
        rpc::PendingResults<Ret>* pending_ptr;
        auto serializer = [&](char* buffer) {
            //By the time this lambda runs, the current thread will be holding a read lock on view_mutex
            const std::size_t max_payload_size = group_rpc_manager.view_manager.get_max_payload_sizes().at(subgroup_id);
            auto send_return_struct = wrapped_this->template send<tag>(
                    [&buffer, &max_payload_size](size_t size) -> char* {
                        if(size <= max_payload_size) {
                            return buffer;
                        } else {
                            throw derecho_exception("The size of serialized args exceeds the maximum message size.");
                        }
                    },
                    std::forward<Args>(args)...);
            results_ptr = new rpc::QueryResults<Ret>(std::move(send_return_struct.results));
            pending_ptr = &send_return_struct.pending;
        };

        std::shared_lock<std::shared_timed_mutex> view_read_lock(group_rpc_manager.view_manager.view_mutex);
        group_rpc_manager.view_manager.view_change_cv.wait(view_read_lock, [&]() {
            return group_rpc_manager.view_manager.curr_view
                    ->multicast_group->send(subgroup_id, payload_size_for_multicast_send, serializer, true);
        });
        group_rpc_manager.finish_rpc_send(subgroup_id, *pending_ptr);
        return std::move(*results_ptr);
    } else {
        throw empty_reference_exception{"Attempted to use an empty Replicated<T>"};
    }
}


template <typename T>
template <rpc::FunctionTag tag, typename... Args>
auto SubgroupCaller<T>::ordered_send(node_id_t dest_node, Args&&... args) {
    if(is_valid()) {
        if(group_rpc_manager.view_manager.get_current_view().get().rank_of(dest_node) == -1) {
            throw invalid_node_exception("Cannot send a p2p request to node "
                    + std::to_string(dest_node) + ": it is not a member of the Group.");
        }
        auto return_pair = wrapped_this->template send<tag>(
                [this, &dest_node](size_t size) -> char* {
                    const std::size_t max_p2p_request_payload_size = getConfUInt64(CONF_DERECHO_MAX_P2P_REQUEST_PAYLOAD_SIZE);
                    if(size <= max_p2p_request_payload_size) {
                        return (char*)group_rpc_manager.get_sendbuffer_ptr(dest_node,
                                                                           sst::REQUEST_TYPE::P2P_REQUEST);
                    } else {
                        throw derecho_exception("The size of serialized args exceeds the maximum message size.");
                    }
                },
                std::forward<Args>(args)...);
        group_rpc_manager.finish_p2p_send(dest_node, subgroup_id, return_pair.pending);
        return std::move(return_pair.results);
    } else {
        throw empty_reference_exception{"Attempted to use an empty Replicated<T>"};
    }
}

template <typename T>
template <rpc::FunctionTag tag, typename... Args>
auto SubgroupCaller<T>::ordered_send(shard_t sn, Args&&... args) {
    if (sn == node_id) {
        return in_subgroup_ordered_send<tag>(std::forward<Args>(args)...);
    } else {
        // TODO: relayed ordered send
    }
}

}   //  namespace derecho