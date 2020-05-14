#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <unordered_map>
#include <typeinfo>

namespace RECS {
	using ComponentType = size_t;

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
		static auto GetTypeID() -> size_t { return STATIC_COMONENT_TYPE_ID; }
	}; // Class Component

	template<class T>
	const size_t Component<T>::STATIC_COMONENT_TYPE_ID = typeid(T).hash_code();

}
#endif // !COMPONENTS_H
