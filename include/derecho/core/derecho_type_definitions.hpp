#pragma once
/** Type alias for IP addresses, currently stored as strings. */
using ip_addr_t = std::string;
/** Type alias for Node IDs in a Derecho group. */
using node_id_t = uint32_t;
#define INVALID_NODE_ID (0xffffffff)

class NodeId {
private:
    uint32_t node_id;
public:
    NodeId(uint32_t nid) : node_id(nid) {};
    uint32_t get() {return node_id;};
};

class ShardNum {
private:
    uint32_t shard_num;
public:
    ShardNum(uint32_t sn) : shard_num(sn) {};
    uint32_t get() {return shard_num;};
};