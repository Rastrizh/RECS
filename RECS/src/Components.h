#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <unordered_map>
#include <typeinfo>
#include "Entities.h"

namespace RECS {
	class IComponent
	{
	public:
		virtual ~IComponent() = default;
	}; // Class IComponent

	template<class T>
	class Component : public IComponent
	{
	public:
		~Component() override = default;
	private:
		static const size_t STATIC_COMONENT_TYPE_ID;
	public:
		static auto GetTypeID() ->const size_t { return STATIC_COMONENT_TYPE_ID; }
	}; // Class Component

	template<class T>
	const size_t Component<T>::STATIC_COMONENT_TYPE_ID = typeid(T).hash_code();

	class ComponentContainer
	{
	public:
		std::unordered_map<size_t, std::unordered_map<RECS::EntityID, IComponent*>> container;
	public:
		static auto instance() ->ComponentContainer&
		{
			static auto m_instance = new ComponentContainer();
			return *m_instance;
		}
		~ComponentContainer()
		{
			for (auto &p : container)
			{
				for (auto &c : p.second)
				{
					delete c.second;
					c.second = nullptr;
				}
			}
		}
	private:
		ComponentContainer() = default;
	public:
		template<typename T, typename ... P>
		void AddComponent(EntityID ownerId, P&&... params)
		{
			IComponent *component = new T(std::forward<P>(params) ...);
			container[T::GetTypeID()][ownerId] = component;
		}
		
		template<typename T>
		void DeleteComponent(EntityID ownerId)
		{
			container[T::GetTypeID()].erase(ownerId);
		}

		template<typename T>
		auto GetComponent(EntityID ownerId) ->T*
		{
			return dynamic_cast<T*>(container[T::GetTypeID()][ownerId]);
		}

		template<typename T>
		auto HasComponent(EntityID id) ->bool
		{
			for (const auto& e : container[T::GetTypeID()])
			{
				if (e.first == id)
					return true;
			}
			return false;
		}
	}; // Class ComponentContainer
}
#endif // !COMPONENTS_H
