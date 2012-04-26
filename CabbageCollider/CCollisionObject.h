#ifndef _CABBAGECOLLIDER_COBJECT_H_INCLUDED_
#define _CABBAGECOLLIDER_COBJECT_H_INCLUDED_

#include "CCollideable.h"

class CCollisionObject : public CCollideable
{

protected:

	friend class CCollisionEngine;

	CCollisionObject();

public:

	~CCollisionObject();

	virtual SVec2 performMovement(CollisionReal const TickTime);

};

#endif
