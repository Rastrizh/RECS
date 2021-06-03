#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <unordered_map>
#include <typeinfo>
#include "RECSTypes.h"

namespace RECS {

	class IComponent
	{};

	template<class T>
	class Component : public IComponent
	{
	public:
		static auto GetTypeID() -> size_t { return typeid(T).hash_code(); }
	}; // Class Component
}
#endif // !COMPONENTS_H
