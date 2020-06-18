#pragma once

// #include <tuple>
// #include <functional>

#include "detail/rpc_manager.hpp"
#include "detail/rpc_utils.hpp"

namespace derecho {

template <typename T>
class SubgroupCaller {
private:
    /** The ID of this node */
    const node_id_t node_id;
    /** The internally-generated subgroup ID of the subgroup that this SubgroupCaller will contact. */
    subgroup_id_t subgroup_id;
    /** Reference to the RPCManager for the Group this SubgroupCaller is in */
    rpc::RPCManager& group_rpc_manager;
    /** The actual implementation of SubgroupCaller, which has lots of ugly template parameters */
    std::unique_ptr<rpc::RemoteInvokerFor<T>> wrapped_this;

    template <rpc::FunctionTag tag, typename... Args>
    auto in_subgroup_ordered_send(Args&&... args);

public:
    SubgroupCaller(uint32_t type_id, node_id_t nid, subgroup_id_t subgroup_id, shard_t shard_num, rpc::RPCManager& group_rpc_manager);

    SubgroupCaller(SubgroupCaller&&) = default;
    SubgroupCaller(const SubgroupCaller&) = delete;

    /**
     * Sends a peer-to-peer message to a single member of the subgroup that
     * this SubgroupCaller<T> connects to, invoking the RPC function identified
     * by the FunctionTag template parameter.
     * @param dest_node The ID of the node that the P2P message should be sent to
     * @param args The arguments to the RPC function being invoked
     * @return An instance of rpc::QueryResults<Ret>, where Ret is the return type
     * of the RPC function being invoked
     */
    template <rpc::FunctionTag tag, typename... Args>
    auto ordered_send(node_id_t dest_node, Args&&... args);

    /**
     * Sends a multicast to the entire subgroup that replicates this Replicated<T>,
     * invoking the RPC function identified by the FunctionTag template parameter.
     * The caller must keep the returned QueryResults object in scope in order to
     * receive replies.
     * @param args The arguments to the RPC function
     * @return An instance of rpc::QueryResults<Ret>, where Ret is the return type
     * of the RPC function being invoked.
     */
    template <rpc::FunctionTag tag, typename... Args>
    auto ordered_send(shard_t sn, Args&&... args);
    
    bool is_valid() const { return true; } // TODO: return *user_object_ptr && true; ???
};


}  // namespace derecho

#include "detail/subgroup_caller_impl.hpp"