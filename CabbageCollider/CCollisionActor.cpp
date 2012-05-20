#include "CCollisionActor.h"

#include <algorithm>
#include <Utils.h>

#include "CCollisionObject.h"
#include "CCollisionEngine.h"

#ifdef __unix__
#include <GL/gl.h>
#endif

#ifdef _WIN32
#include <GL/glew.h>
#endif


CCollisionActor::CCollisionActor(CCollisionEngine * collisionEngine)
	: JumpTimer(0.f), 
	WantsToJump(false), Jumping(false),
	CCollisionInteractor(collisionEngine)
{
	TypeId = INTERACTOR_ACTORS;
	CollisionMask = INTERACTOR_BLOCKS | INTERACTOR_ACTORS;

	AllowedMovement = false;
}

CCollisionActor::~CCollisionActor()
{}

void CCollisionActor::setAction(EActionType const & action)
{
	Action = action;
}

bool const CCollisionActor::isJumping() const
{
	return Jumping;
}

CCollisionActor::EActionType const CCollisionActor::getAction() const
{
	return Action;
}

void CCollisionActor::setJumping(bool const jumping)
{
	WantsToJump = jumping;
}

void CCollisionActor::draw()
{
	if (! Standing && ! Jumping && ! AllowedMovement)
		glColor3f(1, 1, 1);
	else
		glColor3f(Standing ? 1.f : 0.f, AllowedMovement ? 1.f : 0.f, Jumping ? 1.f : 0.f);

	CCollideable::draw();

	glColor3f(1, 1, 1);
}
