#ifndef ENTITIES_H
#define ENTITIES_H

#include <set>
#include <map>
#include <vector>

namespace RECS {
	using EntityID = unsigned short;

	class IEntity
	{
	public:
		EntityID entityID;
	private:
		static EntityID IDCounetr;
	private:
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
		std::map<EntityID, IEntity*> m_entityContainer;
	public:
		static auto instance() ->EntityContainer&
		{
			static auto instance = new EntityContainer();
			return *instance;
		}
		~EntityContainer();
	private:
		EntityContainer() = default;
	public:
		auto CreateEntity() ->IEntity*;
		auto GetGroupOfEntities(std::vector<EntityID> targetIDs) ->std::vector<IEntity*>;
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
	inline auto EntityContainer::GetGroupOfEntities(std::vector<EntityID> targetIDs) ->std::vector<IEntity*>
	{
		std::vector<IEntity*> vec;
		for (auto &i : targetIDs)
		{
			vec.push_back(m_entityContainer[i]);
		}
		return vec;
	}
}
#endif // !ENTITIES_H
