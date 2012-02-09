#include "CObject.h"

namespace Cabbage
{
namespace Collider
{

	CObject::CObject()
	{}

	CObject::~CObject()
	{}

	SVector2 CObject::performMovement(float const TickTime)
	{
		return SVector2();
	}

    CElevator::CElevator()
    {
        Timer = 0.f;
        Speed = 0.5f;
        Range = 0.5f;
    }

    SVector2 CElevator::performMovement(float const TickTime)
	{
        SVector2 Movement;
        Timer += TickTime * Speed;
        Movement.X = TickTime * sin(Timer) * Range;
        Area.Position += Movement;
		return Movement;
	}

}
}
