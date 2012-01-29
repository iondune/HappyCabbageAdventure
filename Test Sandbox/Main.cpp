#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "../lib/CabbageScene.lib")
#pragma comment(lib, "../lib/CabbageFramework.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")


#include "../CabbageFramework/CApplication.h"


class SampleState : public CState<SampleState>
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

int main(int argc, char * argv[])
{
	CApplication & Application = CApplication::get();
	Application.init(SPosition2(200, 200));

	CStateManager & StateManager = Application.getStateManager();
	StateManager.setState<SampleState>();

	Application.run();

	return 0;
}
