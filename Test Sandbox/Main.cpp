#include "../CabbageFramework/CApplication.h"


class SampleState : public IState<SampleState>
{

public:

	void begin()
	{
	}

	void end()
	{
	}


	void OnGameTickStart(float const Elapsed)
	{
	}

	void OnGameTickEnd(float const Elapsed)
	{
	}


	void OnRenderStart(float const Elapsed)
	{
	}

	void OnRenderEnd(float const Elapsed)
	{
	}

};

int main()
{
	CApplication & Application = CApplication::get();
	Application.init(SPosition2(200, 200));

	Application.getStateManager().
}
