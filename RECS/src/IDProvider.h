#ifndef ID_PROVIDER_H
#define ID_PROVIDER_H

#include "RECSTypes.h"
#include <list>

namespace RECS {

template<class T>
struct IDProvider
{
public:
	static IDType Get()
	{
		if (freeID.empty())
		{
			auto ret = counter;
			counter++;
			return ret;
		}
		else
		{
			IDType ID = freeID.back();
			freeID.pop_back();
			return ID;
		}
	}

	static void Remove(const IDType& id)
	{
		freeID.push_back(id);
	}

	static bool isDeletedID(const IDType& id) { return std::find(freeID.begin(), freeID.end(), id) != freeID.end() ? true : false; }
public:
	static IDType counter;
	static std::list<IDType> freeID;
};

template<class T>
IDType typename RECS::IDProvider<T>::counter;

template<class T>
std::list<RECS::IDType> typename RECS::IDProvider<T>::freeID;
}
#endif // !ID_PROVIDER_H
