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
        time = 0.f;
    }

    SVector2 CElevator::performMovement(float const TickTime)
	{
        SVector2 Movement;
        time += TickTime * 0.5f;
        Movement.Y = TickTime * sin(time) * 0.5f;
        //Movement.X = TickTime * cos(time) * 0.5f;
        Area.Position += Movement;
		return Movement;
	}

}
}
