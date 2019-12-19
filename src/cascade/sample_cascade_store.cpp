#include <iostream>
#include <vector>
#include <memory>

#include <derecho/core/derecho.hpp>
#include <derecho/cascade/cascade.hpp>
#include <derecho/cascade/object.hpp>

using namespace derecho::cascade;

int main(int argc, char** argv) {
    /** initialize the parameters
     */
    derecho::Conf::initialize(argc, argv);

    derecho::CallbackSet callback_set{
        nullptr,    // delivery callback
        nullptr,    // local persistence callback
        nullptr     // global persistence callback
    };

    // The subgroup info structure
    derecho::SubgroupInfo si {
        derecho::DefaultSubgroupAllocator({
            {std::type_index(typeid(VolatileCascadeStore<uint64_t,Object,&Object::IK,&Object::IV>)),
             derecho::one_subgroup_policy(derecho::flexible_even_shards("VOLATILE_CASCADE_STORE"))}
        })
    };

    // The factory
    auto volatile_cascade_store_factory = [](persistent::PersistentRegistry*,derecho::subgroup_id_t sid){
        return std::make_unique<VolatileCascadeStore<uint64_t,Object,&Object::IK,&Object::IV>>(
            sid,
            [](derecho::subgroup_id_t sid,
               const uint32_t shard_num,
               const uint64_t& key,
               const Object& value){
                std::cout << "CascadeWatcher get:" << std::endl;
                std::cout << "\tsid = " << sid << std::endl;
                std::cout << "\tshard_num = " << shard_num << std::endl;
                std::cout << "\tkey = " << key << std::endl;
                std::cout << "\tvalue = " << value << std::endl;

            });
//            CascadeWatcher<uint64_t,Object,&Object::IK,&Object::IV>());
    };
    
    // The derecho group
    derecho::Group<VolatileCascadeStore<uint64_t,Object,&Object::IK,&Object::IV>> 
        group(callback_set,si,nullptr,
              std::vector<derecho::view_upcall_t>{},
              volatile_cascade_store_factory);
    std::cout << "Finished constructing Derecho group." << std::endl;

    // TODO: do something here.
    std::string odata("cascade test message - ");
    struct timespec now;
    clock_gettime(CLOCK_REALTIME,&now);
    srandom(now.tv_sec);
    int my_rank = group.get_my_rank();
    odata += random();
    Object o(my_rank,odata.c_str(), odata.length() + 1);

    auto& handle = group.template get_subgroup<VolatileCascadeStore<uint64_t,Object,&Object::IK,&Object::IV>>(0);

    derecho::rpc::QueryResults<std::tuple<persistent::version_t,uint64_t>> results = handle.template p2p_send<RPC_NAME(put)>(group.get_my_id(),o);
    decltype(results)::ReplyMap& replies = results.get();
    std::tuple<version_t,uint64_t> put_result = replies.begin()->second.get();
    std::cout << "put returned with version=" << std::get<0>(put_result) 
              << ", timestamp=" << std::get<1>(put_result) << std::endl;

    // Quit
    std::cout << "Press ENTER to stop." << std::endl;
    std::cin.get();
    group.barrier_sync();
    group.leave();
    return 0;
}
