#include "COverworldState.h"
/* These are here because someone doesn't use extern, or put prototypes in their header files */

COverworldState::COverworldState()
: Application (CApplication::get())
{}

//Initalizer fxn
void COverworldState::begin()
{
   aDown = 0; dDown = 0; spaceDown = 0; wDown = 0; sDown = 0;
   SPosition2 size = Application.getWindowSize();
   WindowWidth = size.X;
   WindowHeight = size.Y; 
   glClearColor(0.4f,0.8f,1.f,0);

   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);
   //glEnable(GL_CULL_FACE);
   //glCullFace(GL_BACK);

   SDL_WM_SetCaption("Happy Cabbage Adventure", NULL);

   //Initialize Font

   Camera = new CCamera((float)WindowWidth/(float)WindowHeight, 0.01f, 100.f, 60.f);
   Application.getSceneManager().setActiveCamera(Camera);
   float const LightBrightness = 0.4f;

    CSceneManager & SceneManager = Application.getSceneManager();
    SceneManager.Lights.push_back(SLight());
    SceneManager.Lights.back().ColorUniform->Value = SVector3(LightBrightness);
    SceneManager.Lights.back().PositionUniform->Value = SVector3(30.f, 2.f, 3.f);

    SceneManager.Lights.push_back(SLight());
    SceneManager.Lights.back().ColorUniform->Value = SVector3(LightBrightness);
    SceneManager.Lights.back().PositionUniform->Value = SVector3(-1.f, -2.f, 30.f);

    SceneManager.Lights.push_back(SLight());
    SceneManager.Lights.back().ColorUniform->Value = SVector3(LightBrightness);
    SceneManager.Lights.back().PositionUniform->Value = SVector3(-30.f, 0.f, 0.f);

    SceneManager.Lights.push_back(SLight());
    SceneManager.Lights.back().ColorUniform->Value = SVector3(LightBrightness);
    PlayerLight = & SceneManager.Lights.back();


   LoadShaders();

   /*soundInit();
   setupSoundtrack();
   startSoundtrack();*/

   //DiscMesh();

   //Load the meshes into VBOs
   PrepMeshes();


   //Initialize Fxns
   fps = timeTotal = 0;
   numFrames = 0;

   printf("END OF BEGIN\n");
}

//Runs at very start of display
void COverworldState::OnRenderStart(float const Elapsed)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();


   Application.getSceneManager().drawAll();

   //Calculate FPS
   timeTotal += Application.getElapsedTime();
   numFrames++;
   if(timeTotal >= 0.1) {
      fps = numFrames / timeTotal;
      timeTotal = 0;
      numFrames = 0;
   }


   SDL_GL_SwapBuffers();
}


//Sends event every time key pressed (also when held)
void COverworldState::OnKeyboardEvent(SKeyboardEvent const & Event)
{
   if(Event.Pressed){
      if(Event.Key == SDLK_w){
         wDown = 1;
      }
      if(Event.Key == SDLK_s){
         sDown = 1;
      }
      if(Event.Key == SDLK_a){
         aDown = 1;
      }
      if(Event.Key == SDLK_d){
         dDown = 1;
      }
      if(Event.Key == SDLK_m){
      }
      if(Event.Key == SDLK_SPACE) {
         spaceDown = 1;

      }
      if(Event.Key == SDLK_ESCAPE) {
         //TODO: Replace with an event/signal to end the game world 
         //Application.getStateManager().setState(& CMainMenuState::get());
      }
   }
   else  {
      if(Event.Key == SDLK_w){
         wDown = 0;
      }
      if(Event.Key == SDLK_s){
         sDown = 0;
      }
      if(Event.Key == SDLK_a){
         aDown = 0;
      }
      if(Event.Key == SDLK_d){
         dDown = 0;
      }
      if(Event.Key == SDLK_k){
      }
      if(Event.Key == SDLK_j){
      }
      if(Event.Key == SDLK_SPACE){
         spaceDown = 0;
      }
   }
}

void COverworldState::end()
{   
   Application.getSceneManager().removeAllRenderables();
}


void COverworldState::LoadShaders() {
   Flat = CShaderLoader::loadShader("Diffuse");
   Diffuse = CShaderLoader::loadShader("Diffuse");
   DiffuseTexture = CShaderLoader::loadShader("DiffuseTexture");
   //normalColor = CShaderLoader::loadShader("NormalColor");
}

void COverworldState::PrepMeshes()
{
   CMesh *mapMesh = CMeshLoader::load3dsMesh("Models/world.3ds");
   renderMap = new CMeshRenderable();
   renderMap->setMesh(mapMesh);
   renderMap->getMaterial().Texture = CImageLoader::loadTexture("Models/world.bmp");
   renderMap->getMaterial().Shader = DiffuseTexture;
   CApplication::get().getSceneManager().addRenderable(renderMap);

   /*
   renderNode = new CMeshRenderable();
   renderNode->setMesh(discMesh);
   renderNode->getMaterial().Shader = Flat;
   */
}
