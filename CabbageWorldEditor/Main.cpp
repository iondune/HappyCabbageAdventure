

//#pragma comment(lib, "../lib/CabbageScene.lib")
#pragma comment(lib, "OpenGL32.lib")

#pragma comment(lib, "glu32.lib")


#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "../lib/CabbageFramework.lib")
#pragma comment(lib, "../lib/CabbageScene.lib")

#include <GL/glew.h>
#include "../CabbageFramework/CApplication.h"
#include <string>
#include "levelReader.h"
#include "xmlToCode.cpp"
#include "../CabbageCore/CabbageCore.h"

#include "CLevelManager.h"


void loadLevel(std::string const & level) 
{
    std::vector<plat> list;
    std::vector<obj> character;
    xmlLevReader(list,character, level);

    CSceneManager & SceneManager = CApplication::get().getSceneManager();

    CLevelManager * thing = new CLevelManager(& SceneManager);

    for (unsigned int i = 0; i < list.size(); i++)
    {
        thing->addBlockObject(SVector2((float)list[i].topX, (float)list[i].topY), SVector2((float)list[i].height, (float)list[i].width), 0);
        //do stuff
    }
    for (unsigned int i = 0; i < character.size(); i++)
    {
        //do stuff
    }

}

class SampleState : public CState<SampleState>
{

    CApplication & Application;

public:

    SampleState()
        : Application(CApplication::get())
    {}

    void begin()
    {
        glClearColor(0.6f, 0.3f, 0.9f, 1.0f);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);


        loadLevel("level2.xml");
    
        GLfloat win[3];
        GLdouble obj[3];
        GLdouble obj2[3];
        GLdouble ray[3];
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
        win[1] = (float)viewpoint[3] - win[1];
        win[2] = 0.f;
        glReadPixels((GLint)win[0], (GLint)win[1], 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win[2]);

        gluUnProject(win[0], win[1], win[2], modelview, pro, viewpoint, & obj[0], & obj[1], & obj[2]);
        win[2] = 1.f;
        gluUnProject(win[0], win[1], win[2], modelview, pro, viewpoint, & obj2[0], & obj2[1], & obj2[2]);

        for (int i = 0; i < 3; i++)
            ray[i] = obj2[i] - obj[i];

        Application.getSceneManager().getActiveCamera()->setPosition(SVector3(0, 0, -100.f));
        Application.getSceneManager().getActiveCamera()->setProjection(80.f, float(800)/float(600), 0.1f, 1000.f);
        Application.getSceneManager().getActiveCamera()->setLookDirection(SVector3(0, 0, 1.f));
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Application.getSceneManager().drawAll();

        

        SDL_GL_SwapBuffers();
    }

    void OnRenderEnd(float const Elapsed)
    {
    }

};


int main(int argc, char * argv[]) 
{
    // Setup application
    CApplication & Application = CApplication::get();
    Application.init(SPosition2(800, 600));

    CStateManager & StateManager = Application.getStateManager();
    StateManager.setState(& SampleState::get());


    

    // Setup gui


    // run program
    Application.run();

    return 0;
}
