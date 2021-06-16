#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <typeindex>
#include "IDProvider.h"

namespace RECS {

class IComponent
{};

template<class T>
class Component : public IComponent
{
public:
	entityID ownerID;
	componentID ID;
public:
	~Component()
	{
		IDProvider<T>::Remove(this->ID);
	}
	static auto GetTypeID() -> size_t { return typeid(T).hash_code(); }
}; // Class Component

template<typename T>
class ComponentHandle
{
public:
	ComponentHandle()
		: component(nullptr)
	{
	}

	ComponentHandle(T* component)
		: component(component)
	{
	}

	T* operator->() const
	{
		return component;
	}

	operator bool() const
	{
		return isValid();
	}

	T& get()
	{
		return *component;
	}

	bool isValid() const
	{
		return component != nullptr;
	}

private:
	T* component;
};

}
#endif // !COMPONENTS_H
