

//#pragma comment(lib, "../lib/CabbageScene.lib")
#pragma comment(lib, "OpenGL32.lib")

#pragma comment(lib, "glu32.lib")


#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "../lib/CabbageFramework.lib")

#include <GL/glew.h>
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
    // Setup application
    CApplication & Application = CApplication::get();
    Application.init(SPosition2(200, 200));

    CStateManager & StateManager = Application.getStateManager();
    StateManager.setState<SampleState>();

    GLfloat win[3];
    GLdouble obj[3];
    GLint viewpoint[4];
    GLdouble modelview[16];
    GLdouble pro[16];

    SPosition2 point;

    glGetDoublev (GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev (GL_PROJECTION_MATRIX, pro);
    glGetIntegerv (GL_VIEWPORT, viewpoint);

    point = Application.getEventManager().MouseLocation;

    win[0] = (float)point.X;
    win[1] = (float)point.Y;
    win[2] = (float)viewpoint[3] - win[1];

    glReadPixels((GLint)win[0], (GLint)win[1], 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win[2]);

    gluUnProject(win[0], win[1], win[2], modelview, pro, viewpoint, & obj[0], & obj[1], & obj[2]);
    // Setup gui


    // run program
    Application.run();

    return 0;
}
