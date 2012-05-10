#ifndef _CABBAGECOLLIDER_CENGINE_H_INCLUDED_
#define _CABBAGECOLLIDER_CENGINE_H_INCLUDED_

#include <vector>
#include <set>
#include <limits>

#include "CCollisionObject.h"
#include "CCollisionActor.h"
#include "CCollisionElevator.h"


#define INTERACTOR_NON 128



class CCollisionEngine
{

public:
	
	typedef std::set<CCollideable *> CollideableList;
	typedef std::set<CCollisionObject *> ObjectList;
	typedef std::set<CCollisionActor *> ActorList;

private:

	ObjectList Objects;
	ActorList Actors;

	ActorList ActorAddList;
	ObjectList ObjectAddList;
	CollideableList RemoveList;

	void performTick(float const TickTime);

	float Timer;

	void runLists();

public:

	CCollisionEngine();
	~CCollisionEngine();

	void removeAll();
	void removeCollideable(CCollideable * Collideable);

	void updateAll(float const Elapsed);
	void update(float const Elapsed);

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
