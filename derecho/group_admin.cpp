
#include "group_admin.h"
#include "view.h"

namespace derecho {

/**
 * Allocates members to a single subgroup, using that subgroup's
 * ShardAllocationPolicy, and pushes the resulting vector of SubViews onto the
 * back of previous_assignment's outer vector. This should be called
 * num_subgroups times, in order of subgroup number.
 * @param curr_view A reference to the same curr_view passed to operator()
 * @param next_unassigned_rank A reference to the same next_unassigned_rank
 * "cursor" passed to operator()
 * @param subgroup_policy The ShardAllocationPolicy to use for this subgroup
 * @return True if the subgroup was assigned successfully, false if the View
 * ran out of nodes and the subgroup could not be fully populated.
 */
bool GroupAdmin::assign_subgroup(const std::type_index& subgroup_type,
                                 const View& curr_view, int& next_unassigned_rank,
                                 const ShardAllocationPolicy& subgroup_policy) {
    if(subgroup_policy.even_shards) {
        if(static_cast<int>(curr_view.members.size()) - next_unassigned_rank
           < subgroup_policy.num_shards * subgroup_policy.nodes_per_shard) {
            return false;
        }
    }
    std::size_t subgroup_type_position = std::distance(subgroup_initialization_order.begin(),
                                                       std::find(subgroup_initialization_order.begin(),
                                                                 subgroup_initialization_order.end(),
                                                                 subgroup_type));
    subgroup_shard_layout_t& previous_subgroup_assignment = previous_assignment[subgroup_type_position];
    previous_subgroup_assignment.emplace_back(std::vector<SubView>());
    for(int shard_num = 0; shard_num < subgroup_policy.num_shards; ++shard_num) {
        if(!subgroup_policy.even_shards && next_unassigned_rank + subgroup_policy.num_nodes_by_shard[shard_num] >= (int)curr_view.members.size()) {
            return false;
        }
        int nodes_needed = subgroup_policy.even_shards ? subgroup_policy.nodes_per_shard : subgroup_policy.num_nodes_by_shard[shard_num];
        std::vector<node_id_t> desired_nodes(&curr_view.members[next_unassigned_rank],
                                             &curr_view.members[next_unassigned_rank + nodes_needed]);
        next_unassigned_rank += nodes_needed;
        Mode delivery_mode = subgroup_policy.even_shards ? subgroup_policy.shards_mode : subgroup_policy.modes_by_shard[shard_num];
        previous_subgroup_assignment.back().emplace_back(curr_view.make_subview(desired_nodes, delivery_mode));
    }
    return true;
}

subgroup_shard_layout_t GroupAdmin::compute_subgroup_membership(const std::type_index& subgroup_type, const View& curr_view) {
    std::size_t subgroup_type_position = std::distance(subgroup_initialization_order.begin(),
                                                       std::find(subgroup_initialization_order.begin(),
                                                                 subgroup_initialization_order.end(),
                                                                 subgroup_type));
    if(previous_assignment[subgroup_type_position]) {
        //Assume the new assignment will be the same as the previous except for a few changes
        auto next_assignment = std::make_unique<subgroup_shard_layout_t>(*previous_assignment[subgroup_type_position]);
        for(int subgroup_num = 0; subgroup_num < default_allocator_policy.num_subgroups; ++subgroup_num) {
            int num_shards_in_subgroup;
            if(default_allocator_policy.identical_subgroups) {
                num_shards_in_subgroup = default_allocator_policy.shard_policy_by_subgroup[0].num_shards;
            } else {
                num_shards_in_subgroup = default_allocator_policy.shard_policy_by_subgroup[subgroup_num].num_shards;
            }
            for(int shard_num = 0; shard_num < num_shards_in_subgroup; ++shard_num) {
                //Check each member of the shard in the previous assignment
                for(std::size_t shard_rank = 0;
                    shard_rank < (*previous_assignment[subgroup_type_position])[subgroup_num][shard_num].members.size();
                    ++shard_rank) {
                    if(curr_view.rank_of((*previous_assignment[subgroup_type_position])[subgroup_num][shard_num].members[shard_rank]) == -1) {
                        //This node is not in the current view, so take the next available one
                        if(next_unassigned_rank >= static_cast<int>(curr_view.members.size())) {
                            throw subgroup_provisioning_exception();
                        }
                        (*next_assignment)[subgroup_num][shard_num].members[shard_rank] = curr_view.members[next_unassigned_rank];
                        (*next_assignment)[subgroup_num][shard_num].member_ips_and_ports[shard_rank] = curr_view.member_ips_and_ports[next_unassigned_rank];
                        next_unassigned_rank++;
                    }
                }
                //These will be initialized from scratch by the calling ViewManager
                (*next_assignment)[subgroup_num][shard_num].joined.clear();
                (*next_assignment)[subgroup_num][shard_num].departed.clear();
            }
        }
        //If we reached this point without throwing an exception, the new assignment is adequate and can be saved
        previous_assignment[subgroup_type_position] = std::move(next_assignment);
    } else {
        previous_assignment[subgroup_type_position] = std::make_unique<subgroup_shard_layout_t>();
        for(int subgroup_num = 0; subgroup_num < default_allocator_policy.num_subgroups; ++subgroup_num) {
            bool assignment_success;
            if(default_allocator_policy.identical_subgroups) {
                assignment_success = assign_subgroup(subgroup_type, curr_view, next_unassigned_rank, default_allocator_policy.shard_policy_by_subgroup[0]);
            } else {
                assignment_success = assign_subgroup(subgroup_type, curr_view, next_unassigned_rank, default_allocator_policy.shard_policy_by_subgroup[subgroup_num]);
            }
            if(!assignment_success) {
                //Delete the partially-created "previous assignment," since it won't be installed
                previous_assignment[subgroup_type_position].reset();
                throw subgroup_provisioning_exception();
            }
        }
    }
    return *previous_assignment[subgroup_type_position];
}

void GroupAdmin::reset_subgroup_membership_state() {
    next_unassigned_rank = 0;
    for(int i = 0; i < previous_assignment.size(); ++i) {
        previous_assignment[i].release();
    }
}

}


