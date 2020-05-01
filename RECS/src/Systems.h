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
		static std::vector<EntityID> GetTargetEntityIDs(std::vector<size_t>);
		//std::vector<IEntity*> GetTargetEntities(std::vector<EntityID>);
		//void GetTargets(std::initializer_list<EntityID>) { }
	};

	std::vector<EntityID> ISystem::GetTargetEntityIDs(std::vector<size_t> iList)
	{
		std::vector<EntityID> t_iList;
		for (auto &p : iList)
		{
			for (auto &b : ComponentContainer::instance().container[p])
				t_iList.push_back(b.first);
		}

		std::sort(t_iList.begin(), t_iList.end());
		for (auto p = t_iList.begin(); p!=t_iList.end()-1; ++p)
		{
			if (*p != *(std::next(p,1))) {
				if (p == t_iList.begin())
				{
					t_iList.erase(t_iList.begin());
					p = t_iList.begin();
					continue;
				}
				t_iList.erase(std::next(p));
			}
		}
		auto last = std::unique(t_iList.begin(), t_iList.end());
		t_iList.erase(last, t_iList.end());

		return t_iList;
	}
}

#endif // !SYSTEMS_H
