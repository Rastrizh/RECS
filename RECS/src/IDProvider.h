#ifndef ID_PROVIDER_H
#define ID_PROVIDER_H

#include "RECSTypes.h"
#include <set>

namespace RECS{
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
			IDType ID = *freeID.begin();
			freeID.erase(freeID.begin());
			return ID;
		}
	}
	static void Remove(const IDType& id)
	{
		freeID.insert(id);
	}
private:
	static IDType counter;
	static std::set<IDType> freeID;
};

template<class T>
IDType RECS::IDProvider<T>::counter;

template<class T>
std::set<RECS::IDType> RECS::IDProvider<T>::freeID;
}
#endif // !ID_PROVIDER_H
