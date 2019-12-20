#include <iostream>
#include <vector>
#include <memory>

#include <derecho/core/derecho.hpp>
#include <derecho/cascade/cascade.hpp>
#include <derecho/cascade/object.hpp>

using namespace derecho::cascade;

#define PERSISTENT_CASCADE

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
#ifdef PERSISTENT_CASCADE
            {std::type_index(typeid(PersistentCascadeStore<uint64_t,Object,&Object::IK,&Object::IV,ST_FILE>)),
             derecho::one_subgroup_policy(derecho::flexible_even_shards("PERSISTENT_CASCADE_STORE"))}
#else
            {std::type_index(typeid(VolatileCascadeStore<uint64_t,Object,&Object::IK,&Object::IV>)),
             derecho::one_subgroup_policy(derecho::flexible_even_shards("VOLATILE_CASCADE_STORE"))}
#endif//PERSISTENT_CASCADE
        })
    };

    // The factory
#ifdef PERSISTENT_CASCADE
    auto persistent_cascade_store_factory = [](persistent::PersistentRegistry* pr,derecho::subgroup_id_t sid){
        return std::make_unique<PersistentCascadeStore<uint64_t,Object,&Object::IK,&Object::IV,ST_FILE>>(
            sid,
            pr,
            [](derecho::subgroup_id_t sid,
               const uint32_t shard_num,
               const uint64_t& key,
               const Object& value){
                std::cout << "Persistent CascadeWatcher get:" << std::endl;
                std::cout << "\tsid = " << sid << std::endl;
                std::cout << "\tshard_num = " << shard_num << std::endl;
                std::cout << "\tkey = " << key << std::endl;
                std::cout << "\tvalue = " << value << std::endl;

            });
    };
#else
    auto volatile_cascade_store_factory = [](persistent::PersistentRegistry* pr,derecho::subgroup_id_t sid){
        return std::make_unique<VolatileCascadeStore<uint64_t,Object,&Object::IK,&Object::IV>>(
            sid,
            [](derecho::subgroup_id_t sid,
               const uint32_t shard_num,
               const uint64_t& key,
               const Object& value){
                std::cout << "Volatile CascadeWatcher get:" << std::endl;
                std::cout << "\tsid = " << sid << std::endl;
                std::cout << "\tshard_num = " << shard_num << std::endl;
                std::cout << "\tkey = " << key << std::endl;
                std::cout << "\tvalue = " << value << std::endl;

            });
    };
#endif//PERSISTENT_CASCADE
    
    // The derecho group
#ifdef PERSISTENT_CASCADE
    derecho::Group<PersistentCascadeStore<uint64_t,Object,&Object::IK,&Object::IV,ST_FILE>>
#else
    derecho::Group<VolatileCascadeStore<uint64_t,Object,&Object::IK,&Object::IV>>
#endif//PERSISTENT_CASCADE
        group(callback_set,si,nullptr,//TODO: use deserialization manager here.
              std::vector<derecho::view_upcall_t>{},
#ifdef PERSISTENT_CASCADE
              persistent_cascade_store_factory
#else
              volatile_cascade_store_factory
#endif//PERSISTENT_CASCADE
        );
    std::cout << "Finished constructing Derecho group." << std::endl;

    std::string odata("cascade test message - ");
    struct timespec now;
    clock_gettime(CLOCK_REALTIME,&now);
    srandom(now.tv_sec);
    int my_rank = group.get_my_rank();
    odata += random();
    Object o(my_rank,odata.c_str(), odata.length() + 1);

#ifdef PERSISTENT_CASCADE
    auto& handle = group.template get_subgroup<PersistentCascadeStore<uint64_t,Object,&Object::IK,&Object::IV,ST_FILE>>(0);
#else
    auto& handle = group.template get_subgroup<VolatileCascadeStore<uint64_t,Object,&Object::IK,&Object::IV>>(0);
#endif

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
