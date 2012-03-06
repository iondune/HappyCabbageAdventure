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
   NextState = State;
}

void CStateManager::doStateChange()
{
	if(!NextState)
		return;
	static float const Fadetime = 0.3f;
	CApplication::get().getSceneManager().blurSceneOut(Fadetime, CApplication::get().getRunTime());

	if (CurrentState)
		CurrentState->end();

	CurrentState = NextState;
	NextState = NULL;

	CurrentState->begin();

	CApplication::get().getSceneManager().blurSceneIn(Fadetime, CApplication::get().getRunTime());
}

void CStateManager::shutDown()
{
	if (CurrentState)
		CurrentState->end();

	CurrentState = 0;
}
