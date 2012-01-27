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

void CStateManager::OnRenderEnd(float const Elapsed)
{
	if (CurrentState)
		CurrentState->OnRenderEnd(Elapsed);
}
