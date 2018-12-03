/**
 * @file group_admin.h
 *
 * @date Nov 27, 2018
 * @author edward
 */

#pragma once

#include <map>
#include <memory>
#include <string>

#include "derecho_exception.h"
#include "register_rpc_functions.h"
#include "container_template_functions.h"
#include "subgroup_functions.h"

#include "persistent/Persistent.hpp"
#include <mutils-serialization/SerializationSupport.hpp>

namespace derecho {

class View;
class SubView;
class ViewManager;

/**
 * An exception that indicates that a subgroup membership function was unable
 * to finish executing because its enclosing Group was not in a valid state.
 * Usually this means that the Group did not have enough members to completely
 * provision the subgroup, or specific nodes that the subgroup needed were not
 * available.
 */
class subgroup_provisioning_exception : public derecho_exception {
public:
    subgroup_provisioning_exception(const std::string& message = "") : derecho_exception(message) {}
};

/** The type to use in the SubgroupInfo maps for a subgroup
 * that doesn't implement a Replicated Object */
struct RawObject {};

/**
 * The data structure used to store a subgroups-and-shards layout for a single
 * subgroup type (i.e. Replicated Object type). The outer vector represents
 * subgroups of the same type, and the inner vector represents shards of each
 * subgroup, so the vectors map subgroup index -> shard index -> sub-view of
 * that shard.
 */
using subgroup_shard_layout_t = std::vector<std::vector<SubView>>;

class GroupAdmin : public mutils::ByteRepresentable {
    //ViewManager gets access to update next_unassigned_rank when the View changes
    friend class ViewManager;
protected:
    SubgroupAllocationPolicy default_allocator_policy;
    std::vector<std::unique_ptr<subgroup_shard_layout_t>> previous_assignment;
    int next_unassigned_rank;
    /** This field can't be serialized, but all nodes already know it in ViewManager,
     * so ViewManager must set it after receiving a GroupAdmin object. */
    std::vector<std::type_index> subgroup_initialization_order;

    bool assign_subgroup(const std::type_index& subgroup_type, const View& curr_view, int& next_unassigned_rank, const ShardAllocationPolicy& subgroup_policy);

public:
    GroupAdmin(const SubgroupAllocationPolicy& allocation_policy)
            : default_allocator_policy(allocation_policy),
              next_unassigned_rank(0) {}
    GroupAdmin(const SubgroupAllocationPolicy& allocation_policy,
               const std::vector<std::unique_ptr<subgroup_shard_layout_t>>& previous_assignment,
               const int next_unassigned_rank)
        : default_allocator_policy(allocation_policy),
          previous_assignment(previous_assignment),
          next_unassigned_rank(next_unassigned_rank) {}

    void set_subgroup_initialization_order(const std::vector<std::type_index>& initialization_order) {
        subgroup_initialization_order = initialization_order;
    }
    virtual ~GroupAdmin() = default;

//    void force_new_epoch();
    /**
     * A local (non-RPC) function that generates a subgroups-and-shards layout
     * for a specific subgroup type, given the type_index of the subgroup's type
     * and the current View. The GroupAdmin base class implements this to
     * implement Derecho's default subgroup allocation function, but clients
     * can extend GroupAdmin to override this function.
     * @param subgroup_type The type of the subgroup to compute subgroup and
     * shard membership for.
     * @param curr_view The View in which to compute membership; the membership
     * should be represented as SubViews of this View.
     * @return A subgroup_shard_layout_t for this type of subgroup.
     */
    virtual subgroup_shard_layout_t compute_subgroup_membership(const std::type_index& subgroup_type, const View& curr_view);

    /**
     * A local (non-RPC) function called by ViewManager when the subgroup
     * membership function needs to reset its state and "forget" the last
     * assignment it computed. This occurs when ViewManager encounters a failure
     * during the creation of the initial View, so if this function is called,
     * it means that the subgroup membership computed by the subgroup membership
     * function was never installed and was based on a faulty View.
     */
    virtual void reset_subgroup_membership_state();

    DEFAULT_SERIALIZATION_SUPPORT(GroupAdmin, default_allocator_policy, previous_assignment, next_unassigned_rank);
//    REGISTER_RPC_FUNCTIONS(GroupAdmin, force_new_epoch);
};

} //namespace derecho

