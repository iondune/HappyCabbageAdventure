#include "COverworldState.h"
/* These are here because someone doesn't use extern, or put prototypes in their header files */
#include "overworldDraw.h"

//Boolean integers for keypressing
int aDown = 0, dDown = 0, spaceDown = 0, wDown = 0, sDown = 0;
int backwardsView = 0, overView = 0;

int WindowWidth, WindowHeight;

std::vector<Node> nodes;

void Load3DS();
void LoadShaders();
void LoadTextures();
void PrepMeshes();

using namespace Cabbage::Collider;
CEngine *Engine;
CActor *Player;
SLight * PlayerLight;
CPlayerView *PlayerView;

CGameplayManager *GameplayManager;

void ViewInit( void ) {
   PlayerView = new CPlayerView();
   PlayerView->setRenderable(playerRenderable, renderShadow);
}

COverworldState::COverworldState()
: Application (CApplication::get())
{}

//Initalizer fxn
void COverworldState::begin()
{
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
   our_font.init("WIFFLES_.TTF", 30);

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

   Load3DS();
   LoadTextures();
   BlockMesh();

   DiscMesh();

   //Load the meshes into VBOs
   PrepMeshes();

   Application.getSceneManager().addRenderable(playerRenderable);

   //Initialize Fxns
   ViewInit();
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

   PlayerLight->PositionUniform->Value = playerRenderable->getTranslation();

   Application.getSceneManager().drawAll();

   //Calculate FPS
   timeTotal += Application.getElapsedTime();
   numFrames++;
   if(timeTotal >= 0.1) {
      fps = numFrames / timeTotal;
      timeTotal = 0;
      numFrames = 0;
   }

   freetype::print(our_font, 10, WindowHeight-40.f, "Overworld\n");

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
         if(musicOn) {
            musicOn = false;
            Mix_HaltMusic();
         }
         else {
            musicOn = true;
            Mix_PlayMusic(music, -1);
         }
      }
      if(Event.Key == SDLK_SPACE) {
         spaceDown = 1;

      }
      if(Event.Key == SDLK_ESCAPE) {
         //TODO: Replace with an event/signal to end the game world 
         Application.getStateManager().setState(& CMainMenuState::get());
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
   stopSoundtrack();
   Mix_CloseAudio();
   our_font.clean();

   Application.getSceneManager().removeAllRenderables();
}

CMeshRenderable* COverworldState::PrepEnemy(float x, float y) {
   CMeshRenderable *tempEnemy;
   enemies.push_back(tempEnemy = new CMeshRenderable());
   tempEnemy->setMesh(enemyMesh);
   //tempEnemy->setTexture(dirtTxt);
   tempEnemy->getMaterial().Shader = Flat;
   tempEnemy->setRotation(SVector3(-90, 0, 0));
   Application.getSceneManager().addRenderable(tempEnemy);
   return tempEnemy;
}

void LoadShaders() {
   Flat = CShaderLoader::loadShader("Diffuse");
   Diffuse = CShaderLoader::loadShader("Diffuse");
   DiffuseTexture = CShaderLoader::loadShader("DiffuseTexture");
   //normalColor = CShaderLoader::loadShader("NormalColor");
}


void Load3DS()
{
   enemyMesh = CMeshLoader::load3dsMesh("Models/appleEnemy.3ds");
   if(enemyMesh) {
      enemyMesh->resizeMesh(SVector3(1));
      enemyMesh->centerMeshByExtents(SVector3(0));
      enemyMesh->calculateNormalsPerFace();
   }

   else {
      fprintf(stderr, "Failed to load the enemy mesh\n");
   }

   basicTreeMesh = CMeshLoader::load3dsMesh("Models/tree2.3ds");
   if (basicTreeMesh) {
      basicTreeMesh->resizeMesh(SVector3(0.5));
      basicTreeMesh->centerMeshByExtents(SVector3(0));
      basicTreeMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load the basic tree mesh\n");
   }

   christmasTreeMesh = CMeshLoader::load3dsMesh("Models/christmasTree3.3ds");
   if (christmasTreeMesh) {
      christmasTreeMesh->resizeMesh(SVector3(0.5));
      christmasTreeMesh->centerMeshByExtents(SVector3(0));
      christmasTreeMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load the christmas tree mesh\n");
   }



   cabbageMesh = CMeshLoader::load3dsMesh("Models/crappycabbage.3ds");
   if (cabbageMesh) {
      cabbageMesh->resizeMesh(SVector3(0.5));
      cabbageMesh->centerMeshByExtents(SVector3(0));
      cabbageMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load the cababge mesh\n");
   }
}

void LoadTextures()
{
   grassImg = CImageLoader::loadImage("Textures/grass.bmp");
   skyImg = CImageLoader::loadImage("Textures/sky.bmp");
   dirtImg = CImageLoader::loadImage("Textures/dirt.bmp");

   grassTxt = new CTexture(grassImg);
   skyTxt = new CTexture(skyImg);
   dirtTxt = new CTexture(dirtImg);
}

void PrepMeshes()
{
   renderBasicTree = new CMeshRenderable();
   renderBasicTree->setMesh(basicTreeMesh);
   renderBasicTree->getMaterial().Shader = Flat;

   renderChristmasTree = new CMeshRenderable();
   renderChristmasTree->setMesh(cabbageMesh);
   renderChristmasTree->getMaterial().Shader = Flat;

   playerRenderable = new CMeshRenderable();
   playerRenderable->setMesh(cabbageMesh);
   playerRenderable->getMaterial().Shader = Flat;
   playerRenderable->setScale(SVector3(.75));

   renderMap = new CMeshRenderable();
   renderMap->setMesh(mapMesh);
   renderMap->getMaterial().Shader = Flat;

   renderNode = new CMeshRenderable();
   renderNode->setMesh(discMesh);
   renderNode->getMaterial().Shader = Flat;
}
