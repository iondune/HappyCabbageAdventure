#ifndef _CABBAGECOLLIDER_CENGINE_H_INCLUDED_
#define _CABBAGECOLLIDER_CENGINE_H_INCLUDED_

#include <vector>
#include <limits>

#include "CCollisionObject.h"
#include "CCollisionActor.h"
#include "CCollisionElevator.h"


#define INTERACTOR_NON 128



class CCollisionEngine
{

public:

	typedef std::vector<CCollisionObject *> ObjectList;
	typedef std::vector<CCollisionActor *> ActorList;

private:

	ObjectList Objects;
	ActorList Actors;

	void performTick(float const TickTime);

	float Timer;

public:

	CCollisionEngine();
	~CCollisionEngine();

	void removeAll();
	void removeObject(CCollisionObject * Object);
	void removeActor(CCollisionActor * Actor);

	void updateAll(float const Elapsed);

	CCollisionObject* const getObjectBelow(SVector2f pos);
	float const getHeightBelow(SVector2f pos);
	float const getHeightBelow(CCollisionActor * Actor);

	CCollisionObject * addObject();
	CCollisionElevator * addElevator();
	CCollisionActor * addActor();

	ObjectList const & getObjects() const;
	ActorList const & getActors() const;

};

#endif
