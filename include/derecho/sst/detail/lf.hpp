#ifndef LF_HPP
#define LF_HPP

/**
 * @file lf.hpp
 * Contains declarations needed for working with RDMA using LibFabric libraries,
 * including the Resources class and global setup functions.
 */
#include <derecho/config.h>
#include <derecho/core/derecho_type_definitions.hpp>
#include <derecho/core/detail/connection_manager.hpp>
#include <derecho/utils/logger.hpp>

#include <iostream>
#include <map>
#include <rdma/fabric.h>
#include <rdma/fi_errno.h>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <map>
#include <tuple>
#include <queue>

#ifndef LF_VERSION
#define LF_VERSION FI_VERSION(1, 5)
#endif

namespace sst {

using memory_attribute_t    =   derecho::memory_attribute_t;
using ip_addr_t             =   derecho::ip_addr_t;
using node_id_t             =   derecho::node_id_t;

class lf_completion_entry_ctxt {
private:
    uint32_t    _ce_idx;     // index into the comepletion entry vector. - 0xFFFFFFFF for invalid
    uint32_t    _remote_id;  // thread id of the sender
    /**
     * @brief Managed flag
     * There two kinds of completion contexts:
     * - managed: the caller of libfabric APIs will manage the context by themselves, therefore the completion polling
     *   thread does not need to release the context memory.
     * - unmanaged: the caller of libfabric APIs will not manage the context. It means that the caller allocated the
     *   memory for the context using `malloc()`, `calloc()`, `new`, and etc.. Therefore, the completion polling thread
     *   is responsible of free the memory.
     */
    bool        _managed;

public:
    // constructor
    lf_completion_entry_ctxt(bool managed = true):
        _ce_idx(0xffffffff),
        _remote_id(0xffffffff),
        _managed(managed) {}

    // getters and setters
    inline uint32_t ce_idx() { return _ce_idx; }
    inline uint32_t remote_id() { return _remote_id; }
    inline void set_ce_idx(const uint32_t& idx) { _ce_idx = idx; }
    inline void set_remote_id(const uint32_t& rid) { _remote_id = rid; }
    inline bool is_managed() { return _managed; }
};

/**
 * Represents the set of RDMA resources needed to maintain a two-way connection
 * to a single remote node.
 */
class _resources {
private:
    /** Connect the queue pair
     *
     * @param is_lf_server This parameter decide local role in connection.
     *     If is_lf_server is true, it waits on PEP for connection from remote
     *     side. Otherwise, it initiate a connection to remote side.
     */
    void connect_endpoint(bool is_lf_server);
    /** Initialize resource endpoint using fi_info
     *
     * @param fi The fi_info object
     * @return 0 for success.
     */
    int init_endpoint(struct fi_info* fi);

protected:
    /** Pointer to the SST-module logger, which lives in a global static registry */
    std::shared_ptr<spdlog::logger> sst_logger;
    std::atomic<bool> remote_failed;
    /**
     * post read/write request
     *
     * @param ctxt      pointer to the sender context, caller should maintain the
     *     ownership of this context until completion.
     * @param offset    The offset within the remote buffer to read/write
     * @param size      The number of bytes to read/write
     * @param op        0 for read and 1 for write
     * @param completion
     *
     * @return the return code for operation.
     *
     */
    int post_remote_send(lf_completion_entry_ctxt* ctxt, const long long int offset, const long long int size,
                         const int op, const bool completion);

public:
    /** ID of the remote node. */
    int remote_id;
    /** tx/rx completion queue */
    // struct fid_cq *txcq, *rxcq; - moved to g_ctxt
    /** Handle for the LibFabric endpoint. */
    struct fid_ep* ep;
    /** memory region for remote writer */
    struct fid_mr* write_mr;
    /** memory region for remote writer */
    struct fid_mr* read_mr;
    /** Pointer to the memory buffer used for local writes.*/
    uint8_t* write_buf;
    /** Pointer to the memory buffer used for the results of RDMA remote reads. */
    uint8_t* read_buf;
    /** key for local read buffer */
    uint64_t mr_lrkey;
    /** key for local write buffer */
    uint64_t mr_lwkey;
    /** key for remote write buffer */
    uint64_t mr_rwkey;
    /** remote write memory address */
    fi_addr_t remote_fi_addr;
    /** the event queue */
    struct fid_eq* eq;

    /**
     * Out-of-Band memory and send management
     */
private:
    struct oob_mr_t {
        void*           addr;
        size_t          size;
        struct  fid_mr* mr;
    };
    static std::shared_mutex  oob_mrs_mutex;
    static std::map<uint64_t,struct oob_mr_t> oob_mrs;

    /**
     * get the descriptor of the corresponding oob memory region
     * Important: it assumes shared lock on oob_mrs_mutex.
     * If iov does not fall into an oob memory region, it fails with nullptr.
     *
     * @param iov
     *
     * @return the descriptor of type void*, or nullptr in case of failure.
     */
    static void* get_oob_mr_desc(const struct iovec& iov);

public:
    /**
     * get the descriptor of the corresponding oob memory region
     * Important: it assumes shared lock on oob_mrs_mutex.
     * If iov does not fall into an oob memory region, it fails with nullptr.
     *
     * @param addr
     *
     * @return the descriptor of type void*, or nullptr in case of failure.
     * @throw   derecho::derecho_exception if not found.
     */
    static void* get_oob_mr_desc(void* addr);

    /**
     * Get the key of the corresponding oob memory region for remote access.
     *
     * @param addr      The address of registered oob memory
     *
     * @return  the remote access key,
     * @throw   derecho::derecho_exception if not found.
     */
    static uint64_t get_oob_mr_key(void* addr);

    /**
     * Register oob memory
     * @param addr  the address of the OOB memory
     * @param size  the size of the OOB memory
     * @param attr  the memory attribute
     *
     * @throws derecho_exception on failure.
     */
    static void register_oob_memory_ex(void* addr, size_t size, const memory_attribute_t& attr);

    /**
     * Deregister oob memory
     * @param addr the address of OOB memory
     *
     * @throws derecho_exception on failure.
     */
    static void deregister_oob_memory(void* addr);

    /**
     * Wait for a completion entries
     * @param num_entries   The number of entries to wait for
     * @param timeout_us    The number of microseconds to wait before throwing timeout
     *
     * @throws derecho_exception on failure.
     */
    void wait_for_thread_local_completion_entries(size_t num_entries, uint64_t timeout_us);
private:

#define OOB_OP_READ     0x0
#define OOB_OP_WRITE    0x1
#define OOB_OP_SEND     0x2
#define OOB_OP_RECV     0x3
    /*
     * oob operation
     * IMPORTANT: please read the remark for the blocking argument.
     *
     * @param op                The operation, current we support OOB_OP_READ and OOB_OP_WRITE.
     * @param iov               The gather memory vector, the total size of the source should not go beyond 'size'.
     * @param iovcnt            The length of the vector.
     * @param remote_dest_addr  The remote address for receiving this message
     * @param rkey              The access key for the remote memory.
     * @param size              The size of the remote buffer
     *
     * @throws derecho_exception at failure.
     */
    void oob_remote_op(uint32_t op, const struct iovec* iov, int iovcnt,
                       void* remote_dest_addr, uint64_t rkey, size_t size);

public:
    /*
     * oob write
     * @param iov               The gather memory vector, the total size of the source should not go beyond 'size'.
     * @param iovcnt            The length of the vector.
     * @param remote_dest_addr  The remote address for receiving this message
     * @param rkey              The access key for the remote memory.
     * @param size              The size of the remote buffer
     *
     * @throws derecho_exception at failure.
     */
    void oob_remote_write(const struct iovec* iov, int iovcnt,
                          void* remote_dest_addr, uint64_t rkey, size_t size);

    /*
     * oob read
     * @param iov               The scatter memory vector, the total size of the source should not go beyond 'size'.
     * @param iovcnt            The length of the vector.
     * @param remote_src_addr   The remote address for receiving this message
     * @param rkey              The access key for the remote memory.
     * @param size              The size of the remote buffer
     *
     * @throws derecho_exception at failure.
     */
    void oob_remote_read(const struct iovec* iov, int iovcnt,
                         void* remote_src_addr, uint64_t rkey, size_t size);

    /*
     * oob send
     * @param iov               The gather memory vector.
     * @param iovcnt            The length of the vector.
     *
     * @throws derecho_exception at failure.
     */
    void oob_send(const struct iovec* iov, int iovcnt);

    /*
     * oob recv
     * @param iov               The gather memory vector.
     * @param iovcnt            The length of the vector.
     *
     * @throws derecho_exception at failure.
     */
    void oob_recv(const struct iovec* iov, int iovcnt);

    /*
     * Public callable wrapper around wait_for_thread_local_completion_entries()
     * @param op                The OOB operations, one of the following:
     *                          - OOB_OP_READ
     *                          - OOB_OP_WRITE
     *                          - OOB_OP_SEND
     *                          - OOB_OP_RECV
     *                          For most of the cases, we wait for only one completion. To allow an operation like
     *                          "exchange", which is to be implemented, we might need to write for two completions.
     * @param timeout_us        Timeout settings in microseconds.
     */
    void wait_for_oob_op(uint32_t op, uint64_t timeout_us);

    /*
     * release singleton resources
     */
    static void global_release();

    /**
     * Constructor
     * Initializes the resources. Registers write_addr and read_addr as the read
     * and write buffers and connects a queue pair with the specified remote node.
     *
     * @param r_id The node id of the remote node to connect to.
     * @param write_addr A pointer to the memory to use as the write buffer. This
     * is where data should be written locally in order to send it in an RDMA write
     * to the remote node.
     * @param read_addr A pointer to the memory to use as the read buffer. This is
     * where the results of RDMA reads from the remote node will arrive.
     * @param size_w The size of the write buffer (in bytes).
     * @param size_r The size of the read buffer (in bytes).
     * @param is_lf_server Is local node a libfabric server or client. A libfabric
     *         client initiates connection to the passive endpoint of the remote
     *         node, while a libfabric server waiting for the conneciton using its
     *         local passive endpoint.
     */
    _resources(int r_id, uint8_t* write_addr, uint8_t* read_addr, int size_w,
               int size_r, int is_lf_server);
    /** Destroys the resources. */
    virtual ~_resources();
};

/**
 * A public-facing version of the internal _resources class that extends it
 * with more convenient functions.
 */
class resources : public _resources {
public:
    /** Constructor: simply forwards to _resources::_resources */
    resources(int r_id, uint8_t* write_addr, uint8_t* read_addr, int size_w,
              int size_r, int is_lf_server) : _resources(r_id, write_addr, read_addr, size_w, size_r, is_lf_server) {
    }
    /**
     * Report that the remote node this object is connected to has failed.
     * This will cause all future remote operations to be no-ops.
     */
    void report_failure();
    /*
      wrapper functions that make up the user interface
      all call post_remote_send with different parameters
    */

    /** Post an RDMA read at the beginning address of remote memory. */
    void post_remote_read(const long long int size);
    /** Post an RDMA read at an offset into remote memory. */
    void post_remote_read(const long long int offset, const long long int size);
    /** Post an RDMA write at the beginning address of remote memory. */
    void post_remote_write(const long long int size);
    /** Post an RDMA write at an offset into remote memory. */
    void post_remote_write(const long long int offset, long long int size);
    void post_remote_write_with_completion(lf_completion_entry_ctxt* ctxt, const long long int size);
    /** Post an RDMA write at an offset into remote memory. */
    void post_remote_write_with_completion(lf_completion_entry_ctxt* ctxt, const long long int offset, const long long int size);

};

/**
 * A public-facing version of the internal _resources class that extends it
 * with functions that support two-sided sends and receives.
 */
class resources_two_sided : public _resources {
    int post_receive(lf_completion_entry_ctxt* ctxt, const long long int offset, const long long int size);

public:
    /** constructor: simply forwards to _resources::_resources */
    resources_two_sided(int r_id, uint8_t* write_addr, uint8_t* read_addr, int size_w,
                        int size_r, int is_lf_server) : _resources(r_id, write_addr, read_addr, size_w, size_r, is_lf_server) {
    }
    /**
     * Report that the remote node this object is connected to has failed.
     * This will cause all future remote operations to be no-ops.
     */
    void report_failure();
    void post_two_sided_send(const long long int size);
    /** Post an RDMA write at an offset into remote memory. */
    void post_two_sided_send(const long long int offset, long long int size);
    void post_two_sided_send_with_completion(lf_completion_entry_ctxt* ctxt, const long long int size);
    /** Post an RDMA write at an offset into remote memory. */
    void post_two_sided_send_with_completion(lf_completion_entry_ctxt* ctxt, const long long int offset, const long long int size);
    void post_two_sided_receive(lf_completion_entry_ctxt* ctxt, const long long int size);
    void post_two_sided_receive(lf_completion_entry_ctxt* ctxt, const long long int offset, const long long int size);
};

/**
 * Adds a new node to the SST TCP connections set.
 */
bool add_node(uint32_t new_id, const std::pair<ip_addr_t, uint16_t>& new_ip_addr_and_port);
/**
 * Adds a new node to external client connections set.
 */
bool add_external_node(uint32_t new_id, const std::pair<ip_addr_t, uint16_t>& new_ip_addr_and_port);
/**
 * Removes a node from the SST TCP connections set
 */
bool remove_node(uint32_t node_id);
/**
 * Blocks the current thread until both this node and a remote node reach this
 * function, which exchanges some trivial data over a TCP connection.
 * @param r_id - ID of the node to exchange data with.
 * @return true/false
 */
bool sync(uint32_t r_id);
/**
 * Compares the set of external client connections to a list of known live nodes and
 * removes any connections to nodes not in that list. This is used to
 * filter out connections to nodes that were removed from the view.
 * @param live_nodes_list A list of node IDs whose connections should be
 * retained; all other connections will be deleted.
 */
void filter_external_to(const std::vector<node_id_t>& live_nodes_list);
/**
 * Initializes the global libfabric resources. Must be called before creating
 * or using any SST instance.
 *
 * @param internal_ip_addrs_and_ports A map from id to (IP address, port) pairs for internal group members
 * @param external_ip_addrs_and_ports A map from id to (IP address, port) pairs for external connections
 * @param node_id id of this node.
 */
void lf_initialize(const std::map<uint32_t, std::pair<ip_addr_t, uint16_t>>& internal_ip_addrs_and_ports,
                   const std::map<uint32_t, std::pair<ip_addr_t, uint16_t>>& external_ip_addrs_and_ports,
                   uint32_t node_id);
/** Polls for completion of a single posted remote write.
 * @return a pair: <completion_entry_index,<remote_id,result(1/0)>>
 */
std::pair<uint32_t, std::pair<int32_t, int32_t>> lf_poll_completion();
/** Shutdown the polling thread. */
void shutdown_polling_thread();
/** Destroys the global libfabric resources. */
void lf_destroy();

/* -------- Error-handling tools -------- */

/**
 * Internal-only enum describing what action error-handling functions should
 * take if a LibFabric function fails.
 */
enum NextOnFailure {
    REPORT_ON_FAILURE = 0,  //!< REPORT_ON_FAILURE Print an error message, but continue
    CRASH_ON_FAILURE = 1    //!< CRASH_ON_FAILURE Print an error message, then exit the entire program
};

/**
 * Calls a LibFabrics function with any number of arguments forwarded via perfect
 * forwarding. If the function returns the FI_EAGAIN error code, keeps calling it
 * again with the same arguments. If the function returns any other error code,
 * prints an error message containing the "description" argument, then either
 * crashes the program or returns the error code depending on the value of the
 * "failure_mode" argument.
 *
 * @param description A description of the LibFabrics operation being performed,
 * for debugging purposes. Will be printed in the error message if this
 * operation fails.
 * @param failure_mode An enum value representing what to do if the LibFabric
 * function returns a failure other than EAGAIN
 * @param lf_function A pointer to the LibFabrics function to call
 * @param lf_args The arguments to call the LibFabrics function with
 * @return The same return value returned by the LibFabrics function
 */
template <typename FuncType, typename... ArgTypes>
inline int64_t fail_if_nonzero_retry_on_eagain(const std::string& description, const NextOnFailure& failure_mode,
                                               FuncType lf_function, ArgTypes&&... lf_args) {
    //Some lf functions return int, others return ssize_t, but both will fit in an int64_t
    int64_t return_code;
    do {
        return_code = (*lf_function)(std::forward<ArgTypes>(lf_args)...);
    } while(return_code == -FI_EAGAIN);
    if(return_code != 0) {
        dbg_default_error("LibFabric error! Return code = {}. Operation description: {}", return_code, description);
        std::cerr << "LibFabric error! Ret=" << return_code << ", desc=" << description << std::endl;
        if(failure_mode == CRASH_ON_FAILURE) {
            dbg_default_flush();
            exit(-1);
        }
    }
    return return_code;
}

/**
 * Calls a LibFabrics function with any number of arguments forwarded via perfect
 * forwarding. If the function returns the FI_EAGAIN error code, keeps calling it
 * again with the same arguments, unless the provided "abort predicate" returns
 * true. If the function returns a non-EAGAIN error code, prints an error message
 * containing the "description" argument and returns the error code. If the abort
 * predicate is true, returns the function's error code even if it is EAGAIN.
 *
 * @param description A description of the LibFabrics operation being performed,
 * for debugging purposes. Will be printed in the error message if this
 * operation fails.
 * @param abort_predicate A std::function that will return true if we should give
 * up retrying the LibFabrics function
 * @param lf_function A pointer to the LibFabrics function to call
 * @param lf_args The arguments to call the LibFabrics function with
 * @return The same return value returned by the LibFabrics function
 */
template <typename FuncType, typename... ArgTypes>
inline int64_t retry_on_eagain_unless(const std::string& description,
                                      const std::function<bool(void)>& abort_predicate,
                                      FuncType lf_function, ArgTypes&&... lf_args) {
    //Some lf functions return int, others return ssize_t, but both will fit in an int64_t
    int64_t return_code;
    do {
        return_code = (*lf_function)(std::forward<ArgTypes>(lf_args)...);
    } while(return_code == -FI_EAGAIN && !abort_predicate());
    //If the abort predicate is true, don't report the error, since we are giving up anyway
    if(return_code != 0 && !abort_predicate()) {
        dbg_default_error("LibFabric error! Return code = {}. Operation description: {}", return_code, description);
        std::cerr << "LibFabric error! Ret=" << return_code << ", desc=" << description << std::endl;
    }
    return return_code;
}

/**
 * Calls a C function that may return either a pointer or NULL, forwarding
 * all of its arguments by copy (because C functions don't understand
 * references). If the function succeeds, returns the pointer; if the function
 * returns NULL, prints an error message and crashes.
 *
 * @param description A description of the operation being performed, which
 * will be printed with the error message if it fails.
 * @param c_func A pointer to the C function to call
 * @param args The arguments to call the C function with
 * @return The pointer that the C function returns
 */
template <typename FuncType, typename... ArgTypes>
inline std::invoke_result_t<FuncType, ArgTypes...> crash_if_nullptr(const std::string& description,
                                                                    FuncType c_func, ArgTypes... args) {
    std::invoke_result_t<FuncType, ArgTypes...> return_val = (*c_func)(args...);
    if(return_val == nullptr) {
        dbg_default_error("Null pointer error in lf.cpp! Description: {}", description);
        std::cerr << "Null pointer error in lf.cpp! Description: " << description << std::endl;
        dbg_default_flush();
        exit(-1);
    }
    return return_val;
}

}  // namespace sst

#endif  // LF_HPP
