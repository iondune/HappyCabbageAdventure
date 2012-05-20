#ifndef _CABBAGECOLLIDER_CENGINE_H_INCLUDED_
#define _CABBAGECOLLIDER_CENGINE_H_INCLUDED_

#include <vector>
#include <set>
#include <limits>

#include "CCollideable.h"
#include "CCollisionObject.h"
#include "CCollisionActor.h"
#include "CCollisionElevator.h"

class CCollisionEngine
{

public:
	
	typedef std::set<CCollideable *> CollideableList;
	typedef std::set<CCollisionObject *> ObjectList;
	typedef std::set<CCollisionActor *> ActorList;

private:
   CollisionReal ImpulseMultiplier;

	ObjectList Objects;
	ActorList Actors;

	ActorList ActorAddList;
	ObjectList ObjectAddList;
	CollideableList RemoveList;

	void performTick(float const TickTime);

	float Timer;

	CollisionReal Gravity;

	void runLists();

public:

	CCollisionEngine();
	~CCollisionEngine();

	void removeAll();
	void removeCollideable(CCollideable * Collideable);
	void remove(CCollideable * Collideable);

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

	std::vector<CCollideable *> const getAllInBound(SRect2f const Bound) const;

	void setGravity(CollisionReal gravity);
	CollisionReal const getGravity() const;

   void setImpulseMultiplier(CollisionReal p);
   CollisionReal getImpulseMultiplier();
};

#endif
