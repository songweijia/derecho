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
        return std::make_unique<VolatileCascadeStore<uint64_t,Object,&Object::IK,&Object::IV>>(sid);
    };
    
    // The derecho group
    derecho::Group<VolatileCascadeStore<uint64_t,Object,&Object::IK,&Object::IV>> 
        group(callback_set,si,nullptr,
              std::vector<derecho::view_upcall_t>{},
              volatile_cascade_store_factory);
    std::cout << "Finished constructing Derecho group." << std::endl;

    // TODO: do something here.

    // Quit
    std::cout << "Press ENTER to stop." << std::endl;
    std::cin.get();
    group.barrier_sync();
    group.leave();
    return 0;
}
