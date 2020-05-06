#ifndef SYSTEMS_H
#define SYSTEMS_H

#include <vector>
#include <algorithm>
#include "Components.h"
#include "Entities.h"

#define EXPAND(...) __VA_ARGS__
#define FUNC_1(MODIFIER, X) MODIFIER(X)
#define FUNC_2(MODIFIER, X, ...) MODIFIER(X), FUNC_1(MODIFIER, __VA_ARGS__)
#define FUNC_3(MODIFIER, X, ...) MODIFIER(X), FUNC_2(MODIFIER, __VA_ARGS__)
#define FUNC_4(MODIFIER, X, ...) MODIFIER(X), FUNC_3(MODIFIER, __VA_ARGS__)
#define FUNC_5(MODIFIER, X, ...) MODIFIER(X), FUNC_4(MODIFIER, __VA_ARGS__)
#define FUNC_6(MODIFIER, X, ...) MODIFIER(X), FUNC_5(MODIFIER, __VA_ARGS__)
#define GET_MACRO(_1, _2, _3, _4, _5, _6, NAME, ...) NAME
#define FOR_EACH(MODIFIER, ...) EXPAND(GET_MACRO(__VA_ARGS__, FUNC_6, FUNC_5, FUNC_4, FUNC_3, FUNC_2, FUNC_1)(MODIFIER, __VA_ARGS__))

#define COMPONENT_GET_TYPE_ID(COMPONENT_CLASS) COMPONENT_CLASS::GetTypeID()
#define CREATE_TARGET_ENTITY_ID_LIST(...) (std::vector<RECS::EntityID>)(RECS::ISystem::GetTargetEntityIDs(std::vector<size_t>({ FOR_EACH(COMPONENT_GET_TYPE_ID, __VA_ARGS__) })))

namespace RECS {
	class ISystem
	{
	protected:
		std::vector<IEntity*> m_targets;
	public:
		virtual ~ISystem()
		{
		}
		static auto GetTargetEntityIDs(std::vector<size_t> targetComponentTypeIDs) -> std::vector<EntityID>;
		virtual void CreatePool() = 0;
		virtual void Update() = 0;
	};

	auto ISystem::GetTargetEntityIDs(std::vector<size_t> targetComponentTypeIDs) -> std::vector<EntityID>
	{
		std::vector<EntityID> targetEntityIDs;
		for (auto &componentTypeID : targetComponentTypeIDs)
		{
			for (auto &targets : ComponentContainer::instance().container[componentTypeID])
				targetEntityIDs.push_back(targets.first);
		}

		std::sort(targetEntityIDs.begin(), targetEntityIDs.end());
		for (auto it_targetID = targetEntityIDs.begin(); it_targetID!=targetEntityIDs.end()-1; ++it_targetID)
		{
			if (*it_targetID != *(std::next(it_targetID,1))) {
				if (it_targetID == targetEntityIDs.begin())
				{
					targetEntityIDs.erase(targetEntityIDs.begin());
					it_targetID = targetEntityIDs.begin();
					continue;
				}
				targetEntityIDs.erase(std::next(it_targetID));
			}
		}
		auto last = std::unique(targetEntityIDs.begin(), targetEntityIDs.end());
		targetEntityIDs.erase(last, targetEntityIDs.end());

		return targetEntityIDs;
	}
}

#endif // !SYSTEMS_H
