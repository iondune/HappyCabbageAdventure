#include "CCollisionActor.h"


CCollisionActor::EActionType::EActionType()
	: Value(None)
{}

CCollisionActor::EActionType::EActionType(Domain const value)
	: Value(value)
{}

bool const CCollisionActor::EActionType::operator == (Domain const value) const
{
	return Value == value;
}
z