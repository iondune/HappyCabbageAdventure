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
	: Standing(0), JumpTimer(0.f), FallAcceleration(0.f),
	GravityEnabled(true), WantsToJump(false), Jumping(false),
	CCollideable(collisionEngine)
{
	TypeId = INTERACTOR_ACTORS;
	CollisionMask = INTERACTOR_BLOCKS | INTERACTOR_ACTORS;

	AllowedMovement = false;
}

CCollisionActor::~CCollisionActor()
{}

void CCollisionActor::onStanding(CCollideable * Object)
{
	// Keep reference to object being stood on
	Standing = Object;

	// Cancel gravity acceleration
	FallAcceleration = 0;

	// Stop downwards movement (from impulse, etc.)
	Velocity.Y = std::max(Velocity.Y, (CollisionReal) 0);


	// Adjust to surface below
	static float const MaxStandingAdjustThreshold = 0.1f;

	if (abs(Area.Position.Y - Object->getArea().otherCorner().Y) < MaxStandingAdjustThreshold)
		Area.Position.Y = Object->getArea().otherCorner().Y;
}

bool CCollisionActor::isAbove(CCollisionObject * Object, float & height) const
{
	if (Area.getCenter().Y < Object->getArea().otherCorner().Y)
		return false;

	if (Area.getCenter().X < Object->getArea().Position.X || Area.getCenter().X > Object->getArea().otherCorner().X)
		return false;

	height = (float) Object->getArea().otherCorner().Y;

	return true;
}

void CCollisionActor::setGravityEnabled(bool const gravityEnabled)
{
	GravityEnabled = gravityEnabled;
}

bool const CCollisionActor::isGravityEnabled() const
{
	return GravityEnabled;
}

void CCollisionActor::pushIfCollided(CCollisionObject * Object, SVec2 const Movement)
{
	if (! collidesWith(Object) && Object != Standing)
		return;
	
	Area.Position += Movement;
}

bool const CCollisionActor::isStanding() const
{
	return Standing != 0;
}

void CCollisionActor::setVelocity(SVec2 const & vel)
{
	Velocity = vel;
}

SVec2 const & CCollisionActor::getVelocity() const
{
	return Velocity;
}

CCollisionActor::SAttributes const & CCollisionActor::getAttributes() const
{
	return Attributes;
}

CCollisionActor::SAttributes & CCollisionActor::getAttributes()
{
	return Attributes;
}

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

void CCollisionActor::addImpulse(SVec2 const & velocity, float const Duration)
{
	Impulses.push_back(std::pair<SVec2, float>(velocity * CollisionEngine->getImpulseMultiplier(), Duration)); 
}
