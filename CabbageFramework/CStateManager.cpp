#include "CStateManager.h"

CStateManager::CStateManager()
    : CurrentState(0)
{}


void CStateManager::OnGameTickStart(float const Elapsed)
{
    if (CurrentState)
        CurrentState->OnGameTickStart(Elapsed);
}

void CStateManager::OnGameTickEnd(float const Elapsed)
{
    if (CurrentState)
        CurrentState->OnGameTickEnd(Elapsed);
}


void CStateManager::OnRenderStart(float const Elapsed)
{
    if (CurrentState)
        CurrentState->OnRenderStart(Elapsed);
}

#include "CApplication.h"
void CStateManager::OnRenderEnd(float const Elapsed)
{
    if (CurrentState)
        CurrentState->OnRenderEnd(Elapsed);
}

void CStateManager::OnMouseEvent(SMouseEvent const & Event)
{
    if (CurrentState)
        CurrentState->OnMouseEvent(Event);
}

void CStateManager::OnKeyboardEvent(SKeyboardEvent const & Event)
{
    if (CurrentState)
        CurrentState->OnKeyboardEvent(Event);
}

void CStateManager::setState(IState * State)
{
    CApplication::get().getSceneManager().blurSceneOut(0.3f);
    if (CurrentState) {
        CurrentState->end();
    }

    CurrentState = State;

    CurrentState->begin();
    CApplication::get().getSceneManager().blurSceneIn(0.3f);
}

void CStateManager::shutDown()
{
	if (CurrentState)
		CurrentState->end();

	CurrentState = 0;
}
