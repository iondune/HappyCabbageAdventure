#include "CEventManager.h"


SMouseEvent::EButton::EButton()
    : Value(Left)
{}

SMouseEvent::EButton::EButton(Domain const value)
    : Value(value)
{}

SMouseEvent::EType::EType()
    : Value(Move)
{}

SMouseEvent::EType::EType(Domain const value)
    : Value(value)
{}

CEventManager::CEventManager()
    : IsKeyDown(KeyStates), MouseLocation(MousePositionState), IsMouseDown(MouseStates)
{
    for (unsigned int i = 0; i < SDLK_LAST; ++ i)
        KeyStates[i] = false;

    for (unsigned int i = 0; i < SMouseEvent::EButton::Count; ++ i)
        MouseStates[i] = false;
}
