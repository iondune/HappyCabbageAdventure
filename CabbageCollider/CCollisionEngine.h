#ifndef _CABBAGECOLLIDER_CENGINE_H_INCLUDED_
#define _CABBAGECOLLIDER_CENGINE_H_INCLUDED_

#include <vector>
#include <limits>

#include "CCollisionObject.h"
#include "CCollisionActor.h"

/*
#define INTERACTOR_
#define INTERACTOR_
#define INTERACTOR_
*/
#define INTERACTOR_NON 128

typedef std::vector<CCollisionObject *> ObjectList;
typedef std::vector<CCollisionActor *> ActorList;

class ICollisionResponder
{

public:

	virtual void OnCollision(CCollideable * Object, CCollideable * With) =0;

};

class CCollisionEngine
{

public:

private:

	ObjectList Objects;
	ActorList Actors;

	ICollisionResponder * CollisionResponder;

	bool CanCollide(CCollideable *a, CCollideable *b);

	void performTick(float const TickTime);

	float Timer;

public:

	void removeAll();

	void removeObject(CCollisionObject * Object);


	CCollisionEngine();

	~CCollisionEngine();


	void setCollisionResponder(ICollisionResponder * collisionResponder);

	void removeActor(CCollisionActor * Actor);

	void update(float const Elapsed);
	void updateAll(float const Elapsed);

	CCollisionObject* const getObjectBelow(SVector2 pos);

	float const getHeightBelow(SVector2 pos);

	float const getHeightBelow(CCollisionActor * Actor);

	void addNullBlock();

	CCollisionObject * addObject();

	CElevator * addElevator();

	CCollisionActor * addActor();

	ObjectList const & getObjects() const;

	ActorList const & getActors() const;

};

#endif
