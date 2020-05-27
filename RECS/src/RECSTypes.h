#ifndef RECSTYPES_H
#define RECSTYPES_H

namespace RECS {
	using EntityID = unsigned long long;
	using ComponentType = size_t;
	using ComponentTypeIDList = std::list<ComponentType>;
}

#endif // !RECSTYPES_H
