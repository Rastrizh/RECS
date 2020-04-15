#ifndef COMPONENTS_H
#define COMPONENTS_H

namespace RECS {
	class IComponent
	{
	private:
		template<typename ComponentTypeName>
		class ComponentContainer
		{
		public:
			/* TO DO: make mechanism to assotioate newely created component class
			 * inherited from IComponent with ComponentTypeName and initialize
			 * container in constructor
			 */
			ComponentContainer()
			{
				// TO DO: initialize container with ComponentTypeName
			}
			std::unordered_map</*EntityID*/int, ComponentTypeName> container;
		};
		// componentTypeName
		// TO DO: make mechanism to provide component type name
		// TO DO: create Entity ID in entity class
		// TO DO: create AddComponent and DeleteComponent methods in entity class
		virtual void AddComponent(/*EntityID*/int EntityID)
		{

		}
		virtual void DeleteComponent(/*EntityID*/int EntityID)
		{

		}
	};
}

#endif // !COMPONENTS_H
