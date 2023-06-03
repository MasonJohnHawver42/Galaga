#pragma once

#include <typeindex>
#include <typeinfo>
#include <memory>
#include <map>

namespace res 
{

    using id = std::size_t;
    using cache_id = std::type_index;

	template<typename T>
	struct Manager
	{
        using res_t = std::shared_ptr<T>;

        template<typename... Args>
		res_t load(Args &&...args) const;
        void save(res_t res) const;
        void unload(res_t res) const;
	};

    class BaseCache { public: virtual ~BaseCache() = default; };

	template<typename T, typename = Manager<T>>
	class Cache; 

	template<typename T, typename M>
    class Cache : public BaseCache 
    {
    public:
        using Manager = M;
        using Resource = std::shared_ptr<T>;
        using Map = std::map<id, Resource>;

        Cache() : m_Map(), m_Manager() {}
        ~Cache() {}


        bool valid(id res_id) { return m_Map.count(res_id) > 0; }

        Resource operator[](id res_id) 
        {
            Resource res{};
            if (this->valid(res_id)) { res = m_Map[res_id]; }
            // else { res = s_default; }
            return res;
        }
		
        template<typename... Args>
        void load(id res_id, Args &&...args) 
        {
            if (!this->valid(res_id)) 
            {
                Resource res = m_Manager.load(args ...);
                m_Map.insert(std::make_pair(res_id, res));
            }
        }
        
        template<typename... Args>
        void reload(id res_id, Args &&...args) 
        {
            if (this->valid(res_id)) { m_Map[res_id] = m_Manager.load(args ...); }
        }

        void save(id res_id) 
        {
            if (this->valid(res_id)) 
            {
                Resource res = m_Map[res_id];
                m_Manager.save(res);
            }
        }
        void unload(id res_id) 
        {
            if (this->valid(res_id)) 
            {
                Resource res = m_Map[res_id];
                m_Manager.unload(res);
            }
        }

        // static cache_id id() { return std::type_index(typeid(Cache<T, M>)); }

        // static void set_default(id res_id) 
        // {
        //     if (this->valid(res_id)) 
        //     {
        //         s_default_id
        //     }
        // }

    private:
        Map     m_Map;
        Manager m_Manager;

        // static id s_default_id;
        // static Resource s_default;
    };

    class Resources 
    {
    public:
        using Map = std::map<std::type_index, BaseCache*>;

        Resources() : m_Caches() {}

        template<typename T, typename M>
        Cache<T, M>& add() 
        {
            auto id = std::type_index(typeid(Cache<T, M>));
            Cache<T, M>* cache = new Cache<T, M>();
            m_Caches.emplace(id, cache); 
            return *cache;
        }

        template<typename T, typename M>
        Cache<T, M>& get() 
        {
            auto id = std::type_index(typeid(Cache<T, M>));
            return *dynamic_cast<Cache<T, M>*>(m_Caches[id]);;
        }

    private:
        Map m_Caches;
    };

}