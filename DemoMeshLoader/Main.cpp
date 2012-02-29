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
#pragma comment(lib, "CabbageScene.lib")
#pragma comment(lib, "CabbageFramework.lib")
#pragma comment(lib, "CabbageGUI.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#ifdef _DEBUG
#pragma comment(lib, "freetype.lib")
#else
#pragma comment(lib, "freetype.lib")
#endif

#include <GL/glew.h>
#include <SDL/SDL.h>
#endif

// Utility classes for loading shaders/meshes
#include <CabbageScene.h>
#include <CabbageFramework.h>
#include <CabbageGUI.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr

//#include "FreeType.h"

class CMainState : public CState<CMainState>
{

    CApplication & Application;
	CSceneManager & SceneManager;
	CGUIEngine & GUIEngine;

public:

    //freetype::font_data Font;
	//gltext::Font Font;

    CMainState()
        : Application(CApplication::get()), WindowWidth(1440), WindowHeight(900), Scale(1), Animate(false), Mode(0), ShowHelp(false),
		SceneManager(CApplication::get().getSceneManager()), GUIEngine(CApplication::get().getGUIEngine())
    {}

	CMeshSceneObject * WingMan;

    void begin()
    {
        // OpenGL init
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Setup camera
        float AspectRatio = (float)WindowWidth / (float)WindowHeight;
        Camera = new CCameraControl();//new CCamera(AspectRatio, 0.01f, 100.f, 60.f);
        Camera->setPosition(SVector3(0, 0, 2));
        Camera->setLookDirection(SVector3(0, 0, -1));
        CApplication::get().getSceneManager().setActiveCamera(Camera);

        // Setup scene
        CSceneManager & SceneManager = CApplication::get().getSceneManager();
        SceneManager.Lights.push_back(new CLight());
        SceneManager.Lights.back()->Color = SVector3(0.5f, 0.2f, 0.2f);
        SceneManager.Lights.back()->Position = SVector3(1.f, 2.f, 3.f);

        SceneManager.Lights.push_back(new CLight());
        SceneManager.Lights.back()->Color = SVector3(0.2f, 0.5f, 0.2f);
        SceneManager.Lights.back()->Position = SVector3(-1.f, -2.f, -3.f);

        SceneManager.Lights.push_back(new CLight());
        SceneManager.Lights.back()->Color = SVector3(0.2f, 0.2f, 0.5f);
        SceneManager.Lights.back()->Position = SVector3(-3.f, 0.f, 0.f);


        // Attempt to load shader and attributes
        Shader = CShaderLoader::loadShader("Flat");
        if (! Shader)
        {
            std::cerr << "Unable to open or compile necessary shader." << std::endl;
            waitForUser();

            exit(1);
        }

        // Attempt to load mesh
        MeshFace = CMeshLoader::load3dsMesh("../Base/Models/crappycabbage2.3ds", false);
        if (MeshFace)
        {
            MeshFace->linearizeIndices();
            MeshFace->resizeMesh(SVector3(1.5));
            MeshFace->centerMeshByExtents(SVector3(0));
            MeshFace->calculateNormalsPerFace();
        }

        MeshVertex = CMeshLoader::load3dsMesh("../Base/Models/crappycabbage2.3ds", false);
        if (MeshVertex)
        {
            MeshVertex->resizeMesh(SVector3(1.5));
            MeshVertex->centerMeshByExtents(SVector3(0));
            MeshVertex->calculateNormalsPerVertex(true);
        }


        Renderable = new CMeshSceneObject();
        Renderable->setMesh(MeshFace);
        Renderable->setShader(Shader);
        setMaterial(3);

		WingMan = new CMeshSceneObject();
		WingMan->setMesh(MeshVertex);
		WingMan->setShader("DiffuseTexture");
		WingMan->setTexture("../Base/Textures/GrassyGrass.bmp", 2);
		WingMan->setTexture("../Base/Textures/DirtyDirt.bmp", 3);
		ISceneObject * Dummy = new ISceneObject();
		Dummy->setTranslation(SVector3(1.5f,0,0));
		WingMan->setParent(Dummy);
		Dummy->setParent(Renderable);

		CMesh * Cube = CMeshLoader::createCubeMesh();
		CMeshSceneObject * SkyBox = SceneManager.addMeshSceneObject(Cube, CShaderLoader::loadShader("DiffuseTexture"));
		SkyBox->setScale(SVector3(20.f));
		SkyBox->setTexture("../DemoMeshLoader/stars.bmp");
		SkyBox->setCullingEnabled(false);

		CApplication::get().getSceneManager().addSceneObject(Renderable);
		CGUIFontWidget * Font = new CGUIFontWidget("Fonts/DejaVuSansMono.ttf", 14.f);
		Font->setPosition(SVector2(0, 250.f));
		Font->setText("Hello, world!");
		GUIEngine.addWidget(Font);
        //Font.init("Fonts/DejaVuSansMono.ttf", 14);
		
		Timer = 0.f;
    }

	int currentMat;

    void setMaterial(int const i)
    {
		currentMat = i;
		CMaterial mat;
		mat.Texture = CImageLoader::loadTexture("spaceshiptexture.bmp");
        switch (i)
        {
        default:
        case 1:
            mat.AmbientColor = SVector3(0.2f);
            mat.DiffuseColor = SVector3(0.9f);
            mat.Shininess = 10.f;
			Renderable->setMaterial(mat);
            break;
        case 2:
            mat.AmbientColor = SVector3(0.2f);
            mat.DiffuseColor = SVector3(1.2f);
            mat.Shininess = 200.f;
			Renderable->setMaterial(mat);
            break;
        case 3:
            mat.AmbientColor = SVector3(0.2f);
            mat.DiffuseColor = SVector3(1.4f);
            mat.Shininess = 3000.f;
			Renderable->setMaterial(mat);
            break;
        case 4:
            mat.AmbientColor = SVector3(0.2f);
            mat.DiffuseColor = SVector3(1.4f);
            mat.Shininess = 0.1f;
			Renderable->setMaterial(mat);
            break;
		case 5:
            mat.AmbientColor = SVector3(0.2f);
            mat.DiffuseColor = SVector3(1.4f);
            mat.Shininess = 0.01f;
			Renderable->setMaterial(mat);
            break;
        }

        CApplication::get().getSceneManager().SceneChanged = true;
    }

	float Timer;

    void OnRenderStart(float const Elapsed)
    {
		Timer += Elapsed;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Camera->update(Elapsed);

        Renderable->setTranslation(Translation);
        Renderable->setScale(Scale);
        Renderable->setRotation(Rotation);

		WingMan->setTranslation(SVector3(0, 0, 0.25f*sin(0.8f*Timer)));

        CApplication::get().getSceneManager().drawAll();

        /*if (ShowHelp)
            freetype::print(Font, 0, (float)Application.getWindowSize().Y - 15.f, "WASD to control camera\nRight click and hold to pan\n\n"\
                "E to choose Scale tool\nR for Rotate\nT for Translate\nLeft click applies tool\n\n"\
                "F to use flat shading\nV to use smooth shading\n\n"\
                "Z to use vertex-lighting shader\nX to use per-pixel lighting shader without specular\nC to use per-pixel lighting shader with specular\n\n"\
                "1, 2, and 3 to change materials\n\n");
        else
            freetype::print(Font, 0, (float)Application.getWindowSize().Y - 15.f, "Press F1 to view commands");*/
		GUIEngine.drawAll();

        SDL_GL_SwapBuffers();
    }

    bool ShowHelp;

    void OnKeyboardEvent(SKeyboardEvent const & Event)
    {
        switch (Event.Key)
        {

        case SDLK_n:

			if (! Event.Pressed)
			{
				if (Renderable->isDebugDataEnabled(EDebugData::Normals))
					Renderable->disableDebugData(EDebugData::Normals);
				else
					Renderable->enableDebugData(EDebugData::Normals);
			}

            break;

        case SDLK_m:

            if (! Event.Pressed)
			{
				if (Renderable->isDebugDataEnabled(EDebugData::NormalColors))
					Renderable->disableDebugData(EDebugData::NormalColors);
				else
					Renderable->enableDebugData(EDebugData::NormalColors);
			}

            break;

        case SDLK_f:

            if (! Event.Pressed)
			{
                Renderable->setMesh(MeshFace);
				setMaterial(currentMat);
			}

            break;

        case SDLK_v:

            if (! Event.Pressed)
			{
                Renderable->setMesh(MeshVertex);
				setMaterial(currentMat);
			}

            break;

        case SDLK_z:
            if (! Event.Pressed)
                Renderable->setShader(CShaderLoader::loadShader("DiffuseTexture_2"));
            break;

        case SDLK_x:
            if (! Event.Pressed)
                Renderable->setShader(CShaderLoader::loadShader("DiffuseTexture"));
            break;

        case SDLK_c:
            if (! Event.Pressed)
                Renderable->setShader(CShaderLoader::loadShader("Specular"));
            break;

        case SDLK_j:
            if (! Event.Pressed)
                Animate = ! Animate;
            break;

        case SDLK_r:
            Mode = 0;
            break;

        case SDLK_t:
            Mode = 1;
            break;

        case SDLK_e:
            Mode = 2;
            break;

        case SDLK_1:
			setMaterial(1);
            break;

        case SDLK_2:
			setMaterial(2);
            break;

        case SDLK_3:
			setMaterial(3);
            break;

        case SDLK_4:
			setMaterial(4);
            break;

		case SDLK_5:
			setMaterial(5);
            break;

        case SDLK_F1:
            ShowHelp = Event.Pressed;
            break;

        }
    }

    glm::vec3 LastVec;

    glm::vec3 makeSphereVec(int x, int y)
    {
        float ix = 2*(x / (float) CApplication::get().getWindowSize().X) - 1.f;
        float iy = 1.f - 2*(y / (float) CApplication::get().getWindowSize().X);

        float length = sqrt(ix*ix + iy*iy);

        if (length > 1.f)
        {
            return glm::vec3(ix / length, iy / length, 0);
        }
        else
        {
            return glm::vec3(ix, iy, sqrt(1 - ix*ix - iy*iy));
        }
    }

    int Mode;

    void OnMouseEvent(SMouseEvent const & Event)
    {
        switch (Event.Type.Value)
        {

        case SMouseEvent::EType::Click:

            LastVec = makeSphereVec(Event.Location.X, Event.Location.Y);

            break;

        case SMouseEvent::EType::Move:
            {
                float difX = (float) Event.Movement.X;
                float difY = (float) Event.Movement.Y;
                glm::vec3 NewVec = makeSphereVec(Event.Location.X, Event.Location.Y);
                glm::mat4 ViewMatrix = CApplication::get().getSceneManager().getActiveCamera()->getViewMatrix();

                if (CApplication::get().getEventManager().IsMouseDown[Event.Button.Left])
                {
	                if (Mode == 0)
	                {
		                glm::vec3 cross = glm::cross(LastVec, NewVec);
		                glm::vec4 axis(cross.x, cross.y, cross.z, 0);
		                float dot = 180.f / 3.1415f * acos(glm::dot(LastVec, NewVec));

		                if (cross.x == cross.x && cross.y == cross.y && cross.z == cross.z && dot == dot && (cross.x != 0 || cross.y != 0 || cross.z != 0))
		                {
			
                            glm::mat4 inverse = glm::inverse(ViewMatrix);
			                axis = inverse * axis;

			                glm::vec3 rotAxis(axis.x, axis.y, axis.z);
			                Rotation = glm::rotate(glm::mat4(1.f), dot, rotAxis)*Rotation;
		                }
		                else
		                {
			                printf("NaN! Skipping\n");
		                }
	                }
	                else if (Mode == 1)
	                {
		                float const moveSpeed = 0.01f;
		                glm::vec4 trans(difX*moveSpeed, difY*moveSpeed, 0, 0);
		                trans = ViewMatrix * trans;
		                Translation.X += trans.x;
		                Translation.Y -= trans.y;
		                Translation.Z -= trans.z;
	                }
	                else if (Mode == 2)
	                {
		                float const scaleSpeed = 0.01f;
                        Scale = SVector3(std::max(std::min(Scale.X + difX*scaleSpeed, 2.f), 0.1f));
	                }
                }

                if (CApplication::get().getEventManager().IsMouseDown[Event.Button.Right])
                {
                    /*Phi -= difY*0.01f;
                    Theta += difX*0.01f;

                    if (Phi > 1.55)
                        Phi = 1.55;
                    if (Phi < -1.55)
                        Phi = -1.55;*/
                }

                LastVec = NewVec;

                break;
            }
        }
    }

    // Shader and Mesh utility classes
    CShader * Shader;
    CMesh * MeshFace, * MeshVertex;
    CMeshSceneObject * Renderable;
    CTexture * Texture;
    CCameraControl * Camera;
    CScene * Scene;

    // Information about mesh
    SVector3 Translation, Scale;
    glm::mat4 Rotation;
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
    Application.init(SPosition2(1440, 900));

    Application.getStateManager().setState(& CMainState::get());

    Application.run();

    return 0;
}
