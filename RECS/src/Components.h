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
	{ }

	ComponentHandle(T* component)
		: component(component)
	{ }

	inline T* operator->() const {	return component; }

	inline operator bool() const {	return isValid(); }

	inline T&  get() { return *component; }

	inline bool isValid() const { return component != nullptr;	}

private:
	T* component;
};

}
#endif // !COMPONENTS_H
