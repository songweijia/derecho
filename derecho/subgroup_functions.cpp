/**
 * @file subgroup_functions.cpp
 *
 * @date Feb 28, 2017
 */

#include <vector>

#include "derecho_internal.h"
#include "derecho_modes.h"
#include "subgroup_functions.h"
#include "view.h"

namespace derecho {

subgroup_shard_layout_t one_subgroup_entire_view(const View& curr_view, int& next_unassigned_rank) {
    subgroup_shard_layout_t subgroup_vector(1);
    subgroup_vector[0].emplace_back(curr_view.make_subview(curr_view.members));
    next_unassigned_rank = curr_view.members.size();
    return subgroup_vector;
}
subgroup_shard_layout_t one_subgroup_entire_view_raw(const View& curr_view, int& next_unassigned_rank) {
    subgroup_shard_layout_t subgroup_vector(1);
    subgroup_vector[0].emplace_back(curr_view.make_subview(curr_view.members, Mode::UNORDERED));
    next_unassigned_rank = curr_view.members.size();
    return subgroup_vector;
}

ShardAllocationPolicy even_sharding_policy(int num_shards, int nodes_per_shard) {
    return ShardAllocationPolicy{num_shards, true, nodes_per_shard, Mode::ORDERED, {}, {}};
}

ShardAllocationPolicy raw_even_sharding_policy(int num_shards, int nodes_per_shard) {
    return ShardAllocationPolicy{num_shards, true, nodes_per_shard, Mode::UNORDERED, {}, {}};
}

ShardAllocationPolicy custom_shards_policy(const std::vector<int>& num_nodes_by_shard,
                                           const std::vector<Mode>& delivery_modes_by_shard) {
    return ShardAllocationPolicy{static_cast<int>(num_nodes_by_shard.size()), false, -1, Mode::ORDERED,
                                 num_nodes_by_shard, delivery_modes_by_shard};
}

SubgroupAllocationPolicy one_subgroup_policy(const ShardAllocationPolicy& policy) {
    return SubgroupAllocationPolicy{1, true, {policy}};
}

SubgroupAllocationPolicy identical_subgroups_policy(int num_subgroups, const ShardAllocationPolicy& subgroup_policy) {
    return SubgroupAllocationPolicy{num_subgroups, true, {subgroup_policy}};
}

subgroup_shard_layout_t CrossProductAllocator::operator()(const View& curr_view,
                                                          int& next_unassigned_rank) {
    /* Ignore next_unassigned_rank, because this subgroup's assignment is based entirely
     * on the source and target subgroups, and doesn't provision any new nodes. */
    subgroup_id_t source_subgroup_id = curr_view.subgroup_ids_by_type.at(policy.source_subgroup.first)
                                               .at(policy.source_subgroup.second);
    subgroup_id_t target_subgroup_id = curr_view.subgroup_ids_by_type.at(policy.target_subgroup.first)
                                               .at(policy.target_subgroup.second);
    int num_source_members = 0;
    for(const auto& shard_view : curr_view.subgroup_shard_views[source_subgroup_id]) {
        num_source_members += shard_view.members.size();
    }
    int num_target_shards = curr_view.subgroup_shard_views[target_subgroup_id].size();
    //Each subgroup will have only one shard, since they'll all overlap, so there are source * target subgroups
    subgroup_shard_layout_t assignment(num_source_members * num_target_shards);
    //I want a list of all members of the source subgroup, "flattened" out of shards, but we don't have that
    //Instead, iterate through the source's shards in order and keep a consistent index
    int source_member_index = 0;
    for(std::size_t source_shard = 0; source_shard < curr_view.subgroup_shard_views[source_subgroup_id].size(); ++source_shard) {
        for(const auto& source_node : curr_view.subgroup_shard_views[source_subgroup_id][source_shard].members) {
            for(int target_shard = 0; target_shard < num_target_shards; ++target_shard) {
                const SubView& target_shard_view = curr_view.subgroup_shard_views[target_subgroup_id][target_shard];
                std::vector<node_id_t> desired_nodes(target_shard_view.members.size() + 1);
                desired_nodes[0] = source_node;
                std::copy(target_shard_view.members.begin(),
                          target_shard_view.members.end(),
                          desired_nodes.begin() + 1);
                std::vector<int> sender_flags(desired_nodes.size(), false);
                sender_flags[0] = true;
                //The vector at this subgroup's index will be default initialized, so push_back a single shard
                assignment[source_member_index * num_target_shards + target_shard].push_back(
                        curr_view.make_subview(desired_nodes, Mode::ORDERED, sender_flags));
                //Now, to send from source_member_index to target_shard, we can use the subgroup at
                //source_member_index * num_target_shards + target_shard
            }
            source_member_index++;
        }
    }
    return assignment;
}
}  // namespace derecho
