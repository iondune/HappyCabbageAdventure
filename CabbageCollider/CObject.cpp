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

    SVector2 CElevator::performMovement(float const TickTime)
	{
        SVector2 Movement;
        //Movement.Y = TickTime * 0.5f;
        Movement.X = TickTime * 0.1f;
        Area.Position += Movement;
		return Movement;
	}

}
}
