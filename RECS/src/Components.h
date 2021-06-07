#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <typeindex>

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
	static auto GetTypeID() -> size_t { return typeid(T).hash_code(); }
}; // Class Component

}
#endif // !COMPONENTS_H
