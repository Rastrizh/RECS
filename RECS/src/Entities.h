#ifndef ENTITIES_H
#define ENTITIES_H

#include <set>
#include <map>
#include <unordered_map>
#include <vector>

namespace RECS {
	class EntityContainer;

	using EntityID = unsigned long long;

	class IEntity
	{
	public:
		EntityID entityID;
	private:
		static EntityID IDCounetr;
		static std::set<EntityID> freeIDs;
	public:
		IEntity();
		~IEntity();

		void DeleteComponent();

		template<typename T, class ... P>
		void AddComponent(P&&... params)
		{
			ComponentContainer::instance().AddComponent<T>(entityID, std::forward<P>(params) ...);
			EntityContainer::instance().m_ComponentLists[this].push_back(T::GetTypeID());
		}

		template<typename T>
		auto GetComponent() ->T*
		{
			return ComponentContainer::instance().GetComponent<T>(entityID);
		}

		template<typename T>
		auto HasComponent() ->bool
		{
			auto comp = EntityContainer::instance().m_ComponentLists.find(this);
			auto it = std::find(comp->second.begin(), comp->second.end(), T::GetTypeID());

			if (it != comp->second.end())
				return true;

			return false;
		}
	};

	class EntityContainer
	{
	public:
		std::unordered_map<EntityID, IEntity*> m_entityContainer;

		std::map<IEntity*, std::list<size_t>> m_ComponentLists;
	public:
		static auto instance()->EntityContainer&;
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
			if (e.second == componentTypeIDs)
				targets.push_back(e.first);
		}
		return targets;
	}
}
#endif // !ENTITIES_H
