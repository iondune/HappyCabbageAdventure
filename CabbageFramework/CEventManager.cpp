#include "CEventManager.h"

#include "CApplication.h"


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


CApplicationEventReceiver::CApplicationEventReceiver()
	: Application(& CApplication::get())
{
	Application->getEventManager().OnGameTickStart.connect(this, & CApplicationEventReceiver::OnGameTickStart);
	Application->getEventManager().OnGameTickEnd.connect(this, & CApplicationEventReceiver::OnGameTickEnd);
	Application->getEventManager().OnRenderStart.connect(this, & CApplicationEventReceiver::OnRenderStart);
	Application->getEventManager().OnRenderEnd.connect(this, & CApplicationEventReceiver::OnRenderEnd);
	Application->getEventManager().OnKeyboardEvent.connect(this, & CApplicationEventReceiver::OnKeyboardEvent);
	Application->getEventManager().OnMouseEvent.connect(this, & CApplicationEventReceiver::OnMouseEvent);
	Application->getEventManager().OnApplicationExit.connect(this, & CApplicationEventReceiver::OnApplicationExit);
}

void CApplicationEventReceiver::OnKeyboardEvent(SKeyboardEvent const & Event)
{}

void CApplicationEventReceiver::OnMouseEvent(SMouseEvent const & Event)
{}

void CApplicationEventReceiver::OnGameTickStart(float const Elapsed)
{}

void CApplicationEventReceiver::OnGameTickEnd(float const Elapsed)
{}

void CApplicationEventReceiver::OnRenderStart(float const Elapsed)
{}

void CApplicationEventReceiver::OnRenderEnd(float const Elapsed)
{}

void CApplicationEventReceiver::OnApplicationExit()
{}


CEventManager::CEventManager()
    : IsKeyDown(KeyStates), MouseLocation(MousePositionState), IsMouseDown(MouseStates)
{
    for (unsigned int i = 0; i < SDLK_LAST; ++ i)
        KeyStates[i] = false;

    for (unsigned int i = 0; i < SMouseEvent::EButton::Count; ++ i)
        MouseStates[i] = false;
}
