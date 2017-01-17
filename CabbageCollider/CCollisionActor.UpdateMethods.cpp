#include "CCollisionActor.h"

#include "CCollisionEngine.h"

#include <iomanip>
#include <algorithm>


void CCollisionActor::updateVectors(CollisionReal const TickTime)
{
	/////////////////////////////////////
	// Walking/Running movement update //
	/////////////////////////////////////

	// Determine maximum walking velocity
	CollisionReal MaxVelocity = Attributes.MaxWalk * (Standing ? 1.f : Attributes.AirSpeedFactor);
   //std::cout << "CurrentVel: " << std::setiosflags(std::ios::fixed) << std::setprecision(2) << Velocity.X << "; MaxVel: " << MaxVelocity << "; Standing: " << Standing << "; MaxWalk: " << Attributes.MaxWalk << std::endl;

	bool Moving = false;
	if (Action == EActionType::MoveLeft)
	{
		if (Velocity.X > -MaxVelocity)
		{
			Velocity.X -= Attributes.WalkAccel * TickTime * (Standing ? 1.f : Attributes.AirControl);
			if (Velocity.X < -MaxVelocity)
				Velocity.X = -MaxVelocity;
		}
      else if(Velocity.X < 0) {
         Velocity.X = -MaxVelocity;
      }
		Moving = true;
	}
	else if (Action == EActionType::MoveRight)
	{
		if (Velocity.X < MaxVelocity)
		{
			Velocity.X += Attributes.WalkAccel * TickTime * (Standing ? 1.f : Attributes.AirControl);
			if (Velocity.X > MaxVelocity)
				Velocity.X = MaxVelocity;
		}
      else if(Velocity.X > 0) {
         Velocity.X = MaxVelocity;
      }

		Moving = true;
	}
	else if (Action == EActionType::None)
	{
	}

	// Friction when not moving
	if (! Moving)
	{
		if (Standing)
			Velocity.X *= Attributes.GroundStandingFriction; // Slowdown factor
		else
			Velocity.X *= Attributes.AirStandingFriction; // Air Slowdown factor
	}


	///////////////////////////
	// Jumping action update //
	///////////////////////////

	// Manage jumping action
	if (Jumping || Standing) // Can't start jumping unless we are standing
	{
		if (! Jumping && WantsToJump)
			JumpTimer = Attributes.JumpLength; // Start jumping

		Jumping = WantsToJump;
	}

	// Perform jump
	if (Jumping)
	{
		if (JumpTimer > 0)
		{
			Velocity.Y = Attributes.JumpAccel;
			JumpTimer -= TickTime;
		}

		if (JumpTimer <= 0)
			Jumping = false;
	}


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
	if (GravityEnabled) {
		FallAcceleration -= CollisionEngine->getGravity() * Attributes.GravityMultiplier * TickTime;

	   // Add velocity from gravity
	   Velocity.Y += FallAcceleration * TickTime;
   }
	else
		FallAcceleration = 0;

   if(Velocity.Y < GLOBAL_TERMINAL_VELOCITY)
      Velocity.Y = GLOBAL_TERMINAL_VELOCITY;


	//////////////////
	// Housekeeping //
	//////////////////

	// Keep track of whether a collision-movement was allowed
	AllowedMovement = false;
	// Keep track of whether actor is standing on a surface
	Standing = 0;

	// Keep track of last position for movement reversal
	LastPosition = Area.Position;

	// Perform test movement
	Movement = Velocity * TickTime;
	Area.Position += Movement;

	// Update phase list

	/*if (PhaseList.size() || NewPhaseList.size())
	{
		std::cout << "Before: ";
		std::cout << "Regular: " << PhaseList.size();
		std::cout << " Post: " << NewPhaseList.size();
		std::cout << std::endl;
	}*/
	PhaseList.swap(NewPhaseList);
	NewPhaseList.clear();
	/*if (PhaseList.size() || NewPhaseList.size())
	{
		std::cout << "After: ";
		std::cout << "Regular: " << PhaseList.size();
		std::cout << " Post: " << NewPhaseList.size();
		std::cout << std::endl;
	}*/
}


bool CCollisionActor::updateCollision(CCollideable * Object, float const TickTime)
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
		Jumping = false;
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

	//if (CollisionType & ECollisionType::Left || CollisionType & ECollisionType::Right)
	//	Velocity.X *= -Object->getMaterial().Elasticity;

	// Return true if this object landed on something
	return (CollisionType & ECollisionType::Down) && (CollisionType & ECollisionType::Responded);
}


int CCollisionActor::checkCollision(CCollideable * Object, CollisionReal const TickTime)
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

			bool AllowedMovementForThisObject = false;

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

				// If still collided... (persistent collision)
				if (Area.intersects(Object->getInternalArea())) // If still collision after revert...
				{
					// Determine minimal intersection
					CollisionReal Intersections[2];
					Intersections[0] = Area.getIntersection(Object->getInternalArea()).getArea(); // 0 - old position

					Area.Position[i] = LastPosition[i] + OriginalMovement;
					Intersections[1] = Area.getIntersection(Object->getInternalArea()).getArea(); // 1 - new position

					// If movement causes a reduced intersection...
					if (Intersections[1] < Intersections[0] && ! equals(Intersections[0], Intersections[1]))
					{
						// If new position is better or equal

						//std::cout << "Allowed collision movement (" << (i ? 'y' : 'x') << "): (" << Intersections[0] << " -> " << Intersections[1] << ")" << std::endl;
						AllowedMovement = true;
						AllowedMovementForThisObject = true;
						Movement[i] = OriginalMovement;
					}
					else
					{
						Area.Position[i] = LastPosition[i];
					}
				}
				
				// Also, check whether it could have been a step up
				if (! AllowedMovementForThisObject && i == 0)
				{
					if (Area.Position.Y - Object->getArea().otherCorner().Y > - Attributes.MaxStep)
					{
						//std::cout << "Allowing step!" << std::endl;
						AllowedMovement = true;
						AllowedMovementForThisObject = true;
						Movement[i] = OriginalMovement;
					}
				}
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


void CCollisionActor::onStanding(CCollideable * Object)
{
	// Keep reference to object being stood on
	Standing = Object;

	// Cancel gravity acceleration
	FallAcceleration = 0;

	// Stop downwards movement (from impulse, etc.)
	Velocity.Y = std::max(Velocity.Y, (CollisionReal) 0);


	// Adjust to surface below
	static CollisionReal const MaxStandingAdjustThreshold = 2 * Attributes.MaxStep;

	if (abs(Area.Position.Y - Object->getArea().otherCorner().Y) < MaxStandingAdjustThreshold)
		Area.Position.Y = Object->getArea().otherCorner().Y;
}


void CCollisionActor::pushIfCollided(CCollisionObject * Object, SVec2 const Movement)
{
	if (! collidesWith(Object) && Object != Standing)
		return;
	
	Area.Position += Movement;
}


void CCollisionActor::updatePhaseList()
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
