#ifndef ENTITIES_H
#define ENTITIES_H

#include <set>
#include <map>
#include <unordered_map>
#include <vector>

namespace RECS {
	using EntityID = unsigned long long;

	class IEntity
	{
	public:
		EntityID entityID;
	private:
		static EntityID IDCounetr;
		static std::set<EntityID> freeIDs;
	public:
		IEntity()
		{
			if (freeIDs.empty())
			{
				entityID = ++IDCounetr;
			}
			else
			{
				entityID = *freeIDs.begin();
				freeIDs.erase(freeIDs.begin());
			}
		}
		virtual ~IEntity()
		{
			freeIDs.insert(this->entityID);
		}

		template<typename T, class ... P>
		void AddComponent(P&&... params)
		{
			ComponentContainer::instance().AddComponent<T>(entityID, std::forward<P>(params) ...);
			EntityContainer::instance().m_ComponentLists[this].push_back(T::GetTypeID());
		}

		template<typename T>
		void DeleteComponent()
		{
			ComponentContainer::instance().DeleteComponent<T>(entityID);
		}

		template<typename T>
		auto GetComponent() ->T*
		{
			return dynamic_cast<T*>(ComponentContainer::instance().GetComponent<T>(entityID));
		}

		template<typename T>
		auto HasComponent() ->bool
		{
			return ComponentContainer::instance().HasComponent<T>(entityID);
		}
	};

	EntityID IEntity::IDCounetr = 0;
	std::set<EntityID> IEntity::freeIDs;

	class EntityContainer
	{
	public:
		std::unordered_map<EntityID, IEntity*> m_entityContainer;

		std::map<IEntity*, std::list<size_t>> m_ComponentLists;
	public:
		static auto instance() ->EntityContainer&
		{
			static EntityContainer instance;
			return instance;
		}
		~EntityContainer();
	private:
		EntityContainer() = default;
	public:
		auto CreateEntity() ->IEntity*;
		auto GetGroupOfEntities(std::list<size_t> componentTypeIDs) ->std::vector<IEntity*>;
	};

	inline EntityContainer::~EntityContainer()
	{
		for (auto &p : m_entityContainer)
		{
			delete p.second;
			p.second = nullptr;
		}
	}

	inline auto EntityContainer::CreateEntity() ->IEntity*
	{
		auto entity = new IEntity();
		m_entityContainer[entity->entityID] = entity;
		return entity;
	}
	inline auto EntityContainer::GetGroupOfEntities(std::list<size_t> componentTypeIDs) ->std::vector<IEntity*>
	{
		std::vector<IEntity*> targets;
		for (auto &e : m_ComponentLists)
		{
			if(e.second == componentTypeIDs)
				targets.push_back(e.first);
		}
		return targets;
	}
}
#endif // !ENTITIES_H
