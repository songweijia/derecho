#pragma once

#include <map>
#include <memory>
#include <string>
#include <time.h>
#include <iostream>
#include <tuple>
#include <optional>

#include <derecho/core/derecho.hpp>
#include <derecho/mutils-serialization/SerializationSupport.hpp>
#include <derecho/persistent/Persistent.hpp>

namespace derecho {
namespace cascade {
    /**
     * The cascade store interface.
     * @tparam KT The type of the key
     * @tparam VT The type of the value must
     *            - includes a public and mutable field 'ver' of type std::tuple<version_t,uint64_t> for its version and
     *              timestamp.
     *            - includes a public field 'key' of type KT for the key
     *            - TODO: enforce this with 'concepts' in C++ 20?
     * @tparam IK A pointer to an invalid key (generally a static member of class KT)
     * @tparam IV A pointer to an invalid value (generally a static member of class VT)
     */
    template <typename KT, typename VT, KT* IK, VT* IV>
    class ICascadeStore {
        /**
         * put a value (with key)
         * @param value
         * @return a tuple including version number (version_t) and a timestamp in microseconds.
         */
        virtual std::tuple<persistent::version_t,uint64_t> put(const VT& value) = 0;
        /**
         * remove a value with key
         * @param key
         * @return a tuple including version number (version_t) and a timestamp in microseconds.
         */
        virtual std::tuple<persistent::version_t,uint64_t> remove(const KT& key) = 0;
        /**
         * get value by version
         * @param key
         * @param ver - Version, if version == INVALID_VERSION, get the latest value.
         * @return a value
         */
        virtual const VT get(const KT& key, const persistent::version_t& ver) = 0;
        /**
         * get value by timestamp
         * @param key
         * @param ts_us - timestamp in microsecond
         * @return a value
         */
        virtual const VT get_by_time(const KT& key, const uint64_t& ts_us) = 0;
        /**
         * ordered_put
         * @param value
         * @return a tuple including version number (version_t) and a timestamp in microseconds.
         */
        virtual std::tuple<persistent::version_t,uint64_t> ordered_put(const VT& value) = 0;
        /**
         * ordered_remove
         * @param key
         * @return a tuple including version number (version_t) and a timestamp in microseconds.
         */
        virtual std::tuple<persistent::version_t,uint64_t> ordered_remove(const KT& key) = 0;
        /**
         * ordered_get
         * @param key
         * @return a value
         */
        virtual const VT ordered_get(const KT& key) = 0;
    };

    /**
     * template volatile cascade stores.
     * 
     * VolatileCascadeStore is highly efficient by manage all the data only in the memory without implementing the heavy
     * log mechanism. Reading by version or time will always return invlaid value.
     */
    template <typename KT, typename VT, KT* IK, VT* IV>
    class VolatileCascadeStore : public ICascadeStore<KT, VT, IK, IV>,
                                 public mutils::ByteRepresentable,
                                 public derecho::GroupReference {
    public:
        /* group reference */
        using derecho::GroupReference::group;
        /* subgroup id */
        subgroup_id_t subgroup_id;
        /* volatile cascade store in memory */
        std::map<KT,VT> kv_map;
        /* watcher */
        // TODO: const CascadeWatcher cascade_watcher;
        
        REGISTER_RPC_FUNCTIONS(VolatileCascadeStore,
                               put,
                               remove,
                               get,
                               get_by_time,
                               ordered_put,
                               ordered_remove,
                               ordered_get);
        virtual std::tuple<persistent::version_t,uint64_t> put(const VT& value) override;
        virtual std::tuple<persistent::version_t,uint64_t> remove(const KT& key) override;
        virtual const VT get(const KT& key, const persistent::version_t& ver) override;
        virtual const VT get_by_time(const KT& key, const uint64_t& ts_us) override;
        virtual std::tuple<persistent::version_t,uint64_t> ordered_put(const VT& value) override;
        virtual std::tuple<persistent::version_t,uint64_t> ordered_remove(const KT& key) override;
        virtual const VT ordered_get(const KT& key) override;


        // serialization support
        DEFAULT_SERIALIZE(subgroup_id,kv_map);

        static std::unique_ptr<VolatileCascadeStore> from_bytes(mutils::DeserializationManager* dsm, char const* buf);

        DEFAULT_DESERIALIZE_NOALLOC(VolatileCascadeStore);

        void ensure_registered(mutils::DeserializationManager&) {}

        /* constructors */
        VolatileCascadeStore(subgroup_id_t sid);
        VolatileCascadeStore(subgroup_id_t sid,const std::map<KT,VT>& _kvm); // copy kv_map
        VolatileCascadeStore(subgroup_id_t sid,std::map<KT,VT>&& _kvm); // move kv_map
    };

    /**
     * template for persistent cascade stores.
     * 
     * PersistentCascadeStore is full-fledged implementation with log mechansim. Data can be stored in different
     * persistent devices including file system(persistent::ST_FILE) or SPDK(persistent::ST_SPDK). Please note that the
     * data is cached in memory too.
     */
    template <typename KT, typename VT, KT* IK, VT* IV, persistent::StorageType ST=persistent::ST_FILE>
    class PersistentCascadeStore : public ICascadeStore<KT, VT, IK, IV>,
                                   public mutils::ByteRepresentable,
                                   public derecho::GroupReference {
    public:
        using derecho::GroupReference::group;
        std::map<KT,VT> kv_map;
        // TODO: const CascadeWatcher cascade_watcher;
        
        REGISTER_RPC_FUNCTIONS(PersistentCascadeStore,
                               put,
                               remove,
                               get,
                               get_by_time,
                               ordered_put,
                               ordered_remove,
                               ordered_get);
        virtual std::tuple<persistent::version_t,uint64_t> put(const VT& value) override;
        virtual std::tuple<persistent::version_t,uint64_t> remove(const KT& key) override;
        virtual const VT get(const KT& key, const persistent::version_t& ver) override;
        virtual const VT get_by_time(const KT& key, const uint64_t& ts_us) override;
        virtual std::tuple<persistent::version_t,uint64_t> ordered_put(const VT& value) override;
        virtual std::tuple<persistent::version_t,uint64_t> ordered_remove(const KT& key) override;
        virtual const VT ordered_get(const KT& key) override;
    };
} // namespace cascade
} // namespace derecho
#include "detail/cascade_impl.hpp"
