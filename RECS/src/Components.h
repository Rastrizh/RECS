#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "Entities.h"
#include <unordered_map>

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
			std::unordered_map<EntityID, ComponentTypeName> container;
		};
		// componentTypeName
		// TO DO: make mechanism to provide component type name
		// TO DO: create Entity ID in entity class | checked
		// TO DO: create AddComponent and DeleteComponent methods in entity class
		virtual void AddComponent(EntityID EntityID)
		{

		}
		virtual void DeleteComponent(EntityID EntityID)
		{

		}
	};
}

#endif // !COMPONENTS_H
