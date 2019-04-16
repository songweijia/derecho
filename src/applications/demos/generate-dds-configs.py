#!/usr/bin/python
import sys
import random
import os
import ConfigParser

def generate_topics(num_topics):
    array_topics = [];
    for i in range(num_topics):
        hash = random.getrandbits(128)
        array_topics.append('%032x' % hash)
    return array_topics

def pick_topics(topics,num):
    if num > len(topics):
        raise Exception('There are only %d topics, not enough for %d producers.' % ( len(topics), num) )
    bitmap = bytearray(len(topics));
    picked_topics = []
    for i in range(num):
        idx = random.randint(0,len(topics)-1);
        while bitmap[idx] != 0:
            idx = (idx + 1)%len(topics)
        bitmap[idx] = 1;
        picked_topics.append(topics[idx]);
    return picked_topics;

def topic_string(topics):
    topic_str = None
    for topic in topics:
        if topic_str == None:
            topic_str = topic
        else:
            topic_str = topic_str + "," + topic
    return topic_str

def generate_client_cfg(topics,num_producers_per_client,prefix,f):
    f.write(prefix)
    f.write('[DDS_DEMO/producer]\n')
    f.write('topics = %s\n' % topic_string(pick_topics(topics,num_producers_per_client)) )
    f.write('message_freq_hz = 1.0\n')

def generate_replica_cfg(topics,num_consumers_per_replica,prefix,f):
    f.write(prefix)
    f.write('[DDS_DEMO/consumer]\n')
    f.write('topics = %s\n' % topic_string(pick_topics(topics,num_consumers_per_replica)) )

if __name__ == '__main__':
    cfg_in_file = 'dds-cfg.in'
    if len(sys.argv) >= 2:
        cfg_in_file = sys.argv[1]

    # 0 - check if the configuration file exists.
    if not os.path.isfile(cfg_in_file):
        raise Exception('cannot open configuration input file: %s' % cfg_in_file);
    # 0.1 - make the path
    if not os.path.isdir('generated-dds-cfg'):
        os.mkdir('generated-dds-cfg')
    # 0.2 - get the parameters
    cfgin = ConfigParser.RawConfigParser()
    cfgin.read(cfg_in_file)
    nodes_ips_str = cfgin.get("derecho","nodes_ips")
    leader_ip = cfgin.get("derecho","leader_ip")
    rdma_provider = cfgin.get("derecho","rdma_provider")
    rdma_domain = cfgin.get("derecho","rdma_domain")
    min_rep_factor = cfgin.getint("objectstore","min_rep_factor")
    num_replicas = cfgin.getint("objectstore","num_replicas")
    num_topics = cfgin.getint("dds","num_topics")
    num_consumers_per_replica = cfgin.getint("dds","num_consumers_per_replica")
    num_producers_per_client = cfgin.getint("dds","num_producers_per_client")
    

    # 1 - generate the dpods configurations
    dpods_cfg_str = ""
    with open("dpods-cfg.in",'r') as cfg_in_file:
        dpods_cfg_str = cfg_in_file.read()
    dpods_cfg_str = dpods_cfg_str.replace("@LEADER_IP@",leader_ip)
    dpods_cfg_str = dpods_cfg_str.replace("@PROVIDER@",rdma_provider)
    dpods_cfg_str = dpods_cfg_str.replace("@DOMAIN@",rdma_domain)
    dpods_cfg_str = dpods_cfg_str.replace("@MIN_REP_FACTOR@",str(min_rep_factor))
    dpods_cfg_str = dpods_cfg_str.replace("@REPLICAS@",str(num_replicas-1))

    # 2 - generate the configuration files
    topics = generate_topics(num_topics)
    tokens = nodes_ips_str.split(",")
    if len(tokens) < num_replicas:
        raise Exception("We have %d nodes, not enough for %d replicas." % (len(tokens),num_replicas));
    cnt = 1
    for local_ip in tokens:
        node_cfg = dpods_cfg_str.replace("@LOCAL_IP@",local_ip)
        local_id = cnt
        if local_ip == leader_ip:
            local_id = 0
        else:
            cnt = cnt + 1
        node_cfg = node_cfg.replace("@LOCAL_ID@",str(local_id))
        if local_id < num_replicas:
            # replicas
            f = open('generated-dds-cfg/replica.%d' % local_id, 'w')
            generate_replica_cfg(topics,num_consumers_per_replica,node_cfg,f)
            f.close()
        else:
            # clients
            f = open('generated-dds-cfg/client.%d' % local_id, 'w')
            generate_client_cfg(topics,num_producers_per_client,node_cfg,f)
            f.close()
