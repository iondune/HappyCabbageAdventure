#include "CCollisionInteractor.h"

#include "CCollisionEngine.h"

#include <algorithm>

void CCollisionInteractor::updateVectors(CollisionReal const TickTime)
{

	// Friction
	if (Standing)
		Velocity.X *= Attributes.GroundStandingFriction; // Slowdown factor
	else
		Velocity.X *= Attributes.AirStandingFriction; // Air Slowdown factor

	////////////////////
	// Impulse Update //
	////////////////////

	// Perform impulse
	for (std::vector<std::pair<SVec2, float> >::iterator it = Impulses.begin(); it != Impulses.end();)
	{
		if (it->second > 0)
        {
			Velocity += it->first;
            it->second -= (float) TickTime;
        }

        if (it->second <= 0)
            it = Impulses.erase(it);
	}


	////////////////////
	// Gravity Update //
	////////////////////

	// Cease falling if gravity is off
	if (GravityEnabled)
		FallAcceleration -= CollisionEngine->getGravity() * Attributes.GravityMultiplier * TickTime;
	else
		FallAcceleration = 0;

	// Add velocity from gravity
	Velocity.Y += FallAcceleration * TickTime;


	//////////////////
	// Housekeeping //
	//////////////////

	// Keep track of whether actor is standing on a surface
	Standing = 0;

	// Keep track of last position for movement reversal
	LastPosition = Area.Position;

	// Perform test movement
	Movement = Velocity * TickTime;
	Area.Position += Movement;

	// Update phase list
	PhaseList.swap(NewPhaseList);
	NewPhaseList.clear();
}


bool CCollisionInteractor::updateCollision(CCollideable * Object, float const TickTime)
{
	// Determine what sort of collision occurred
	int CollisionType = checkCollision(Object, TickTime);

	// Determine events to emit
	if (CollisionType)
	{
		SCollisionEvent Event;
		Event.This = this;
		Event.Other = Object;
		Event.Direction = (ECollisionType::Domain) CollisionType;
		Event.Receiver = false;

		if (CollisionType & ECollisionType::Responded)
		{
			OnCollision.emit(Event);

			// Coupled event emission
			SCollisionEvent Event;
			Event.This = Object;
			Event.Other = this;
			Event.Direction = (ECollisionType::Domain) CollisionType;
			Event.Receiver = true;

			Object->OnCollision.emit(Event);
		}
		else
		{
			NewPhaseList.insert(Object);
			if (PhaseList.find(Object) == PhaseList.end())
			{
				OnPhaseBegin.emit(Event);

				// Coupled event emission
				SCollisionEvent Event;
				Event.This = this;
				Event.Other = Object;
				Event.Direction = (ECollisionType::Domain) CollisionType;
				Event.Receiver = true;

				Object->OnPhaseBegin.emit(Event);
			}
		}
	}

	// Elasticity for vertical collisions
	if (CollisionType & ECollisionType::Up && CollisionType & ECollisionType::Responded)
	{
		Velocity.Y *= -Object->getMaterial().Elasticity;
	}

	// Bounce for vertical collisions
	if (CollisionType & ECollisionType::Down && CollisionType & ECollisionType::Responded)
	{
		/*if(Attributes.Bounce > 0.01f)
		{
			Velocity.Y = Attributes.Bounce;
			Attributes.Bounce *= 0.5f;
		}*/
		//Jumping = false;
	}

	// Wtf is this
	//if (Attributes.Reacts && (CollisionType & ECollisionType::Left || CollisionType & ECollisionType::Right))
	//	Velocity.X *= -Object->getMaterial().Elasticity;

	// Return true if this object landed on something
	return (CollisionType & ECollisionType::Down) && (CollisionType & ECollisionType::Responded);
}


int CCollisionInteractor::checkCollision(CCollideable * Object, CollisionReal const TickTime)
{
	int Out = ECollisionType::None;

	if (canCollideWith(Object))
	{
		// Revert position
		Area.Position = LastPosition;

		// Side in each direction separately
		for (int i = 0; i < 2; ++ i)
		{
			CollisionReal OriginalMovement = Movement[i];

			// No movement - continue
			if (equals(Movement[i], (CollisionReal) 0.0))
				continue;

			// Try this movement
			Area.Position[i] = LastPosition[i] + Movement[i];

			// If collision caused...
			if (Area.intersects(Object->getInternalArea()))
			{
				// Determine direction
				if (Movement[i] > (CollisionReal) 0.0)
					Out |= (i ? ECollisionType::Up : ECollisionType::Right);
				else if (Movement[i] < (CollisionReal) 0.0)
					Out |= (i ? ECollisionType::Down : ECollisionType::Left);
				else
					std::cout << "Null movement, undefined behavior." << std::endl;

				// Remove movement
				Movement[i] = (CollisionReal) 0.0;
				Area.Position[i] = LastPosition[i];
			}
		}

		if (Out)
			Out |= ECollisionType::Responded;
	}
	else
	{
		for (int i = 0; i < 2; ++ i)
		{
			if (Area.intersects(Object->getInternalArea()))
			{
				if (Movement[i] > (CollisionReal) 0.0)
					Out |= (i ? ECollisionType::Up : ECollisionType::Right);
				else if (Movement[i] < (CollisionReal) 0.0)
					Out |= (i ? ECollisionType::Down : ECollisionType::Left);
			}
		}
	}

	return Out;
}


void CCollisionInteractor::onStanding(CCollideable * Object)
{
	// Keep reference to object being stood on
	Standing = Object;

	// Cancel gravity acceleration
	FallAcceleration = 0;

	// Stop downwards movement (from impulse, etc.)
	Velocity.Y = std::max(Velocity.Y, (CollisionReal) 0);
}


void CCollisionInteractor::pushIfCollided(CCollisionObject * Object, SVec2 const Movement)
{
	if (! collidesWith(Object) && Object != Standing)
		return;
	
	Area.Position += Movement;
}


void CCollisionInteractor::updatePhaseList()
{
	// Emit phase leaving events
	for (std::set<CCollideable *>::iterator it = PhaseList.begin(); it != PhaseList.end(); ++ it)
	{
		if (NewPhaseList.find(* it) == NewPhaseList.end())
		{
			SCollisionEvent Event;
			Event.This = this;
			Event.Other = * it;
			Event.Direction = (ECollisionType::Domain) ECollisionType::UnPhase;
			Event.Receiver = false;
			OnPhaseEnd.emit(Event);

			Event.This = * it;
			Event.Other = this;
			Event.Direction = (ECollisionType::Domain) ECollisionType::UnPhase;
			Event.Receiver = true;
			(* it)->OnPhaseEnd.emit(Event);
		}
	}
}

#include "CCollisionInteractor.h"

CCollisionInteractor::SAttributes::SAttributes()
	: Bounce(0.0f),
	GravityMultiplier(1.0f),
	MaxStep(0.1),
	AirStandingFriction(0.99f),
	GroundStandingFriction(0.95f)
{}

#include "CCollisionInteractor.h"

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


CCollisionInteractor::CCollisionInteractor(CCollisionEngine * collisionEngine)
	: Standing(0), FallAcceleration(0.f),
	GravityEnabled(true), 
	CCollideable(collisionEngine)
{
	TypeId = INTERACTOR_ACTORS;
	CollisionMask = INTERACTOR_BLOCKS | INTERACTOR_ACTORS;
}

CCollisionInteractor::~CCollisionInteractor()
{}

bool CCollisionInteractor::isAbove(CCollisionObject * Object, float & height) const
{
	if (Area.getCenter().Y < Object->getArea().otherCorner().Y)
		return false;

	if (Area.getCenter().X < Object->getArea().Position.X || Area.getCenter().X > Object->getArea().otherCorner().X)
		return false;

	height = (float) Object->getArea().otherCorner().Y;

	return true;
}

void CCollisionInteractor::setGravityEnabled(bool const gravityEnabled)
{
	GravityEnabled = gravityEnabled;
}

bool const CCollisionInteractor::isGravityEnabled() const
{
	return GravityEnabled;
}

bool const CCollisionInteractor::isStanding() const
{
	return Standing != 0;
}

void CCollisionInteractor::setVelocity(SVec2 const & vel)
{
	Velocity = vel;
}

SVec2 const & CCollisionInteractor::getVelocity() const
{
	return Velocity;
}

CCollisionInteractor::SAttributes const & CCollisionInteractor::getAttributes() const
{
	return Attributes;
}

CCollisionInteractor::SAttributes & CCollisionInteractor::getAttributes()
{
	return Attributes;
}

void CCollisionInteractor::draw()
{
	glColor3f(Standing ? 1.f : 0.f, 0.f, 0.f);

	CCollideable::draw();

	glColor3f(1, 1, 1);
}

void CCollisionInteractor::addImpulse(SVec2 const & velocity, float const Duration)
{
	Impulses.push_back(std::pair<SVec2, float>(velocity * CollisionEngine->getImpulseMultiplier(), Duration)); 
}
