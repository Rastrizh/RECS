#ifndef ENTITIES_H
#define ENTITIES_H

#include <set>

using EntityID = unsigned short;

class IEntity
{
public:
	EntityID entityID;
	static EntityID IDCounetr;
private:
	static std::set<EntityID> freeIDs;
public:
	IEntity()
	{
		if(freeIDs.empty())
			entityID = ++IDCounetr;
		else
		{
			entityID = *freeIDs.begin();
			freeIDs.erase(freeIDs.begin());
		}
	}
	virtual ~IEntity()
	{
		freeIDs.insert(this->entityID);
	}
};

EntityID IEntity::IDCounetr = 0;

#endif // !ENTITIES_H
