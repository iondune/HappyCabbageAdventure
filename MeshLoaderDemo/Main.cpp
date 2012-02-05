#include <iostream>

#ifdef __unix__
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#endif

#ifdef _WIN32
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "../lib/CabbageScene.lib")
#pragma comment(lib, "../lib/CabbageFramework.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")

#include <GL/glew.h>
#include <SDL/SDL.h>
#endif

// Utility classes for loading shaders/meshes
#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"

class CMainState : public CState<CMainState>
{

public:

    CMainState()
        : WindowWidth(1440), WindowHeight(900), Scale(1), Animate(false)
    {}

    void begin()
    {
        // OpenGL init
        glClearColor(0.6f, 0.3f, 0.9f, 1.0f);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Setup camera
        float AspectRatio = (float)WindowWidth / (float)WindowHeight;
        Camera = new CCamera(AspectRatio, 0.01f, 100.f, 60.f);
        CApplication::get().getSceneManager().setActiveCamera(Camera);

        // Attempt to load shader and attributes
        Shader = CShaderLoader::loadShader("Shaders/DiffuseTexture");
        if (! Shader)
        {
            std::cerr << "Unable to open or compile necessary shader." << std::endl;
            waitForUser();
            exit(1);
        }


        // Attempt to load mesh
        MeshFace = CMeshLoader::createCubeMesh();//("spaceship.3ds");
        if (MeshFace)
        {
            MeshFace->linearizeIndices();
            MeshFace->resizeMesh(SVector3(1.5));
            MeshFace->centerMeshByExtents(SVector3(0));
            MeshFace->calculateNormalsPerFace();
        }

        MeshVertex = CMeshLoader::createCubeMesh();//load3dsMesh("spaceship.3ds");
        if (MeshVertex)
        {
            MeshVertex->resizeMesh(SVector3(1.5));
            MeshVertex->centerMeshByExtents(SVector3(0));
            MeshVertex->calculateNormalsPerVertex();
        }


        CImage * Image = CImageLoader::loadImage("spaceshiptexture.bmp");
        if (! Image)
        {
            std::cerr << "Unable to load necessary texture." << std::endl;
            waitForUser();
            exit(1);
        }
        Texture = new CTexture(Image);


        // Now load our mesh into a VBO, retrieving the number of triangles and the handles to each VBO
        Renderable = new CMeshRenderable();
        Renderable->setMesh(MeshFace);
        Renderable->setTexture(Texture);
        Renderable->setShader(Shader);
    }

    // Manages time independant movement and draws the VBO
    void OnRenderStart(float const Elapsed)
    {
        // Determine time since last draw
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        Camera->setPosition(SVector3(0, 0, 2));
        Camera->setLookDirection(SVector3(0, 0, -1));
        Camera->recalculateViewMatrix();

        
        // Animates the loaded model by modulating it's size
        static float const ScaleSpeed = 1.f;
        static float const ScaleThreshold = 0.4f;
        if (Animate)
            ScaleTimer += Elapsed * ScaleSpeed;
        Scale = SVector3(1) + ScaleThreshold * cos(ScaleTimer);

        // ...and by spinning it around
        static float const RotationSpeed = 50.f;
        if (Animate)
        {
            Rotation.X += RotationSpeed*Elapsed;
            Rotation.Y += RotationSpeed*Elapsed*2;
        }
        

        Renderable->setTranslation(Translation);
        Renderable->setScale(Scale);
        Renderable->setRotation(Rotation);

        Renderable->draw(* Camera);

        SDL_GL_SwapBuffers();
    }

    void OnKeyboardEvent(SKeyboardEvent const & Event)
    {
        switch (Event.Key)
        {

        case SDLK_n:

            if (Event.Pressed)
                Renderable->enableDebugData(EDebugData::Normals);
            else
                Renderable->disableDebugData(EDebugData::Normals);

            break;

        case SDLK_m:

            if (Event.Pressed)
                Renderable->enableDebugData(EDebugData::NormalColors);
            else
                Renderable->disableDebugData(EDebugData::NormalColors);

            break;

        case SDLK_f:

            if (! Event.Pressed)
                Renderable->setMesh(MeshFace);

            break;

        case SDLK_v:

            if (! Event.Pressed)
                Renderable->setMesh(MeshVertex);

            break;

        case SDLK_g:

            if (! Event.Pressed)
                Renderable->setShader(CShaderLoader::loadShader("Shaders/Flat"));

            break;

        case SDLK_b:

            if (! Event.Pressed)
                Renderable->setShader(CShaderLoader::loadShader("Shaders/DiffuseTexture"));

            break;

        case SDLK_j:

            if (! Event.Pressed)
                Animate = ! Animate;

            break;

        }
    }

    // Shader and Mesh utility classes
    CShader * Shader;
    CMesh * MeshFace, * MeshVertex;
    CMeshRenderable * Renderable;
    CTexture * Texture;
    CCamera * Camera;

    // Information about mesh
    SVector3 Translation, Rotation, Scale;
    int TriangleCount;

    // Window information
    int WindowWidth, WindowHeight;

    // Time-independant movement variables
    float ScaleTimer;
    bool Animate;

};


int main(int argc, char * argv[])
{
    CApplication & Application = CApplication::get();
    Application.init(SPosition2(800, 600));

    Application.getStateManager().setState<CMainState>();

    Application.run();

    return 0;
}
