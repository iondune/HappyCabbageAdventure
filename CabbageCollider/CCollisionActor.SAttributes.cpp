#include "CCollisionActor.h"

CCollisionActor::SAttributes::SAttributes()
	: MaxWalk(4.5f),
	WalkAccel(60.f),
	JumpAccel(6.4f),
	JumpLength(0.6f),
	AirControl(0.75f),
	AirSpeedFactor(0.7f),
	AirStandingFriction(0.99f),
	GroundStandingFriction(0.95f),
	Bounce(0.0f),
	GravityMultiplier(1.0f)
{}
