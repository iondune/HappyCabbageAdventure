#include <iostream>

#ifdef __unix__
#include <GL/gl.h>
#include <SDL/SDL.h>
#endif

#include "header.h"

#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "CGameEventManager.h"
#include "CGameEventReceiver.h"

#define TREE_Y_OFFSET 2.1
#define ANGLE(j,k) (j==2?3:(j?2:(k?1:0)))
#define NEXT(j) (j==2?0:(j?2:1))

//Boolean integers for keypressing
int aDown = 0, dDown = 0, spaceDown = 0, wDown = 0, sDown = 0;
int backwardsView = 0, overView = 0;


freetype::font_data our_font;

//Variables need to create VBOs of meshes, textures, and shaders
CShader *Shader, *Flat, *Diffuse, *DiffuseTexture, *normalColor;  //Use Diffuse for trees (doesn't need texture)

CImage *grassImg, *skyImg, *dirtImg, *blueFlwrImg, *pinkFlwrImg, *ficusImg,
       *poinImg;

CTexture *grassTxt, *skyTxt, *dirtTxt, *blueFlwrTxt, *pinkFlwrTxt, *ficusTxt,
         *poinTxt;

CMesh *basicTreeMesh, *cabbageMesh, *christmasTreeMesh, *cubeMesh, *discMesh,
      *blueFlwrMesh, *pinkFlwrMesh, *ficusMesh, *poinMesh;

CMeshRenderable *renderShadow, *playerRenderable, *renderChristmasTree, 
  *renderBasicTree, *renderBlueFlwr, *renderPinkFlwr, *renderFicus, *renderPoin;

std::vector<CMeshRenderable*> blocks, enemies;

int WindowWidth, WindowHeight;

void Load3DS();
void LoadShaders();
void LoadTextures();
void PrepMeshes();
void EngineInit();

using namespace Cabbage::Collider;
CEngine *Engine;
CActor *Player, *Derp;
CObject *Floor, *Block;
CPlayerView *PlayerView;

CGameplayManager *GameplayManager;

void ViewInit( void ) {
   PlayerView = new CPlayerView();
   PlayerView->setRenderable(playerRenderable, renderShadow);
}

void BlockMesh() {
   cubeMesh = CMeshLoader::createCubeMesh();
   //cubeMesh->linearizeIndices();
   cubeMesh->calculateNormalsPerFace();
}

void DiscMesh() {
   discMesh = CMeshLoader::createDiscMesh();
   discMesh->linearizeIndices();
   discMesh->calculateNormalsPerFace();
}

class CGameState : public CState<CGameState>
{
   CApplication & Application;
   CGameEventManager * GameEventManager;
   CGameEventReceiver GameEventReceiver;

   public:
   CGameState()
      : Application (CApplication::get())
   {}

   void EngineInit( void ) {
      Engine = new CEngine();
      Player = Engine->addActor();
      Player->setArea(SRect2(-24.5, 3, 1, 1));

      Derp = Engine->addActor();
      Derp->setArea(SRect2(-17, 0, 1, 1));

      Floor = Engine->addObject();
      Floor->setArea(SRect2(-25, -1, 50, 1));
      PrepGrass(-25, -1, 50, 1);
      PrepSky();

      SRect2 area;

      GameplayManager = new CGameplayManager(Player, Engine);
      GameplayManager->addEnemy(SVector2(-20, 4), PrepEnemy(-20,4));
      GameplayManager->addEnemy(SVector2(-18, 4), PrepEnemy(-18,4));
      GameplayManager->addEnemy(SVector2(-16, 4), PrepEnemy(-16,4));
      GameplayManager->addEnemy(SVector2(-14, 4), PrepEnemy(-14,4));
      GameplayManager->addEnemy(SVector2(-12, 4), PrepEnemy(-12,4));
      GameplayManager->addEnemy(SVector2(-10, 4), PrepEnemy(-10,4));
      GameplayManager->addEnemy(SVector2(-5, 40), PrepEnemy(-5, 40));
      GameplayManager->addEnemy(SVector2(0, 40), PrepEnemy(0, 40));
      GameplayManager->addEnemy(SVector2(5, 40), PrepEnemy(5, 40));
      GameplayManager->addEnemy(SVector2(10, 40), PrepEnemy(10, 40));
      GameplayManager->addEnemy(SVector2(15, 40), PrepEnemy(15, 40));
      GameplayManager->addEnemy(SVector2(20, 40), PrepEnemy(20, 40));
      GameplayManager->addEnemy(SVector2(25, 40), PrepEnemy(25, 40));
      GameplayManager->addEnemy(SVector2(25, 45), PrepEnemy(25, 45));
      GameplayManager->addEnemy(SVector2(25, 50), PrepEnemy(25, 50));

      GameEventManager = & GameplayManager->getGameEventManager();
      GameEventManager->OnEnemyDeath.connect(& GameEventReceiver, &CGameEventReceiver::OnEnemyDeath);

      float i = 0;
      float j = 0;

      for(j = 0; j < 10; j+=2.5) {
         Block = Engine->addObject();
         area = SRect2(-15 + j, 1.5 + j, 2, 1);
         PrepBlock(-15 + j, 1.5 + j, 2, 1);
         Block->setArea(area);
      }

      Block = Engine->addObject();
      area = SRect2(-22, 7, 6, 0.2);
      PrepBlock(-22, 7, 6, 0.2);
      Block->setArea(area);

      Block = Engine->addObject();
      area = SRect2(-22, 7, 0.2, 3);
      PrepBlock(-22, 7, 0.2, 3);
      Block->setArea(area);

      for(; i < 12; i++) {
         Block = Engine->addObject();
         area = SRect2(-i + 5, 0, 1, i+1);
         PrepBlock(-i + 5, 0, 1, i+1);
         Block->setArea(area);
      }

      for(i=0; i < 7; i++) {
         Block = Engine->addObject();
         area = SRect2(i + 15, 0, 1, i+1);
         PrepBlock(i + 15, 0, 1, i+1);
         Block->setArea(area);
      }

      for(i=0; i < 2; i++) {
         Block = Engine->addObject();
         area = SRect2(24, 2+i*4, 2, 1);
         PrepBlock(24, 2+i*4, 2, 1);
         Block->setArea(area);
      }

      Block = Engine->addObject();
      area = SRect2(27.5, 4, 2, 1);
      PrepBlock(27.5, 4, 2, 1);
      Block->setArea(area);
   }

   //Initalizer fxn
   void begin()
   {
      glClearColor(0,0,0,0);

      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);
      //glEnable(GL_CULL_FACE);
      //glCullFace(GL_BACK);

      SDL_WM_SetCaption("Happy Cabbage Adventure", NULL);

      //Initialize Font
      our_font.init("WIFFLES_.TTF", 30);

      Camera = new CCamera((float)WindowWidth/(float)WindowHeight, 0.01f, 100.f, 60.f);
      Application.getSceneManager().setActiveCamera(Camera);


      LoadShaders();

      soundInit();
      setupSoundtrack();
      startSoundtrack();

      Load3DS();
      LoadTextures();
      BlockMesh();

      DiscMesh();

      //Load the meshes into VBOs
      PrepMeshes();

      PrepShadow();

      Application.getSceneManager().addRenderable(renderBasicTree);
      Application.getSceneManager().addRenderable(renderChristmasTree);
      Application.getSceneManager().addRenderable(playerRenderable);

      Application.getSceneManager().addRenderable(renderBlueFlwr);
      Application.getSceneManager().addRenderable(renderPinkFlwr);
      Application.getSceneManager().addRenderable(renderFicus);
      Application.getSceneManager().addRenderable(renderPoin);

      //Initialize Fxns
      EngineInit();
      ViewInit();
      fps = timeTotal = 0;
      numFrames = 0;

      printf("END OF BEGIN\n");
   }

   CCamera *Camera;
   float fps, timeTotal;
   int numFrames;

   void oldDisplay() {
      float curXVelocity = Player->getVelocity().X;
      PlayerView->setVelocity(Player->getVelocity());

      if (GameplayManager->isPlayerAlive())
      {
         if(!overView) {
            if(dDown && aDown) {
               Player->setAction(CActor::EActionType::None);
               PlayerView->setState(CPlayerView::State::Standing);
            }
            else if((aDown && !backwardsView) || (dDown && backwardsView)) {
               Player->setAction(CActor::EActionType::MoveLeft);
               PlayerView->setState(CPlayerView::State::MovingLeft);
            }
            else if((dDown && !backwardsView) || (aDown && backwardsView)) {
               Player->setAction(CActor::EActionType::MoveRight);
               PlayerView->setState(CPlayerView::State::MovingRight);
            }
            else {
               Player->setAction(CActor::EActionType::None);
               PlayerView->setState(CPlayerView::State::Standing);
            }
         }
         else {
            if(sDown && wDown) {
               Player->setAction(CActor::EActionType::None);
               PlayerView->setState(CPlayerView::State::Standing);
            }
            else if(((wDown && overView == 1)) || (sDown && overView == 2)) {
               Player->setAction(CActor::EActionType::MoveRight);
               PlayerView->setState(CPlayerView::State::MovingRight);
            }
            else if((sDown && overView == 1) || (wDown && overView == 2)) {
               Player->setAction(CActor::EActionType::MoveLeft);
               PlayerView->setState(CPlayerView::State::MovingLeft);
            }
            else {
               Player->setAction(CActor::EActionType::None);
               PlayerView->setState(CPlayerView::State::Standing);
            }
         }

         if (Player->isStanding() && spaceDown != 0) {
            playJump = true;
         }

         Player->setJumping(spaceDown != 0);

         if (playJump) {
            Mix_PlayChannel(-1, jump, 0);
            playJump = false;
         }

      }
      else
      {
         Player->setAction(CActor::EActionType::None);
         PlayerView->setState(CPlayerView::State::Standing);
         Player->setJumping(false);
      }
      PlayerView->setRecovering(GameplayManager->getRecovering());

      Derp->setJumping(true);

      Engine->updateAll(Application.getElapsedTime());
      GameplayManager->run(Application.getElapsedTime());
      PlayerView->step(Application.getElapsedTime()*1000);

      SVector2 middleOfPlayer = Player->getArea().getCenter();
      //printf("%0.2f, %0.2f\n", Player->getArea().getCenter().X, Player->getArea().getCenter().Y);
      PlayerView->setMiddle(middleOfPlayer);
      PlayerView->setGround(Engine->getHeightBelow(Player));

      PlayerView->establishCamera(Camera, ANGLE(overView, backwardsView));

      //draw the ground plane
      //drawPlane();
      //drawSky(backwardsView);
      //drawDirt(backwardsView);

      /*
      //Chris Code, draw Trees
      addTrees(NUM_TREES, basicTree);

      glPushMatrix();

      glTranslatef(-12.5, TREE_Y_OFFSET - .1, -2);
      glScalef(1.5, 1.5, 1.5);

      drawTree(christmasTree, DARK_GREEN_SHINY);

      glPopMatrix();

      glPushMatrix();

      glTranslatef(10, TREE_Y_OFFSET - .7, 2.0);

      drawTree(christmasTree, DARK_GREEN_SHINY);

      glTranslatef(7, 0, 0);
      drawTree(christmasTree, DARK_GREEN_SHINY);

      glPopMatrix();
       */

      PlayerView->draw();

      //Draw derp (enemy)
      renderBasicTree->setTranslation(SVector3(Derp->getArea().getCenter().X, Derp->getArea().getCenter().Y, 0));
      renderBasicTree->setScale(SVector3(1.5));
      renderBasicTree->setRotation(SVector3(-90, 0, -90));

      //ENEMY DISPLAY
      int i = 0;
      for (CGameplayManager::EnemyList::iterator it = GameplayManager->Enemies.begin(); it != GameplayManager->Enemies.end(); ++ it)
      {
         ((CMeshRenderable*)(it->Renderable))->setTranslation(SVector3(it->Actor->getArea().getCenter().X, it->Actor->getArea().getCenter().Y, 0));
         i++;
      }



      // ...and by spinning it around
      static float const RotationSpeed = 50.f;
      //Rotation.X += RotationSpeed*Delta;
      //Rotation.Y += RotationSpeed*Delta*2;

      glLoadIdentity();
   }

   //Runs at very start of display
   void OnRenderStart(float const Elapsed)
   {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      oldDisplay();

      Application.getSceneManager().drawAll();

      if (! GameplayManager->isPlayerAlive()) {
         //Chris Code.  Play Death Sound
         if (playDead) {
            Mix_HaltMusic();
            Mix_PlayChannel(-1, die, 0); //Only play once
            playDead = false;
         }
         freetype::print(our_font, 50, WindowHeight - 240, "GAME OVER! YOU ARE DEAD");
      }

      //Calculate FPS
      timeTotal += Application.getElapsedTime();
      numFrames++;
      if(timeTotal >= 0.1) {
         fps = numFrames / timeTotal;
         timeTotal = 0;
         numFrames = 0;
      }

 

      //Draw Text
      freetype::print(our_font, 10, WindowHeight-40, "Elapsed Time: %0.0f\n"
            "Health: %d\nnumKilled: %d\nFPS: %0.2f ", Application.getRunTime(), GameplayManager->getPlayerHealth(), numKilled, fps);


      SDL_GL_SwapBuffers();
   }


   //Sends event every time key pressed (also when held)
   void OnKeyboardEvent(SKeyboardEvent const & Event)
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
         if(Event.Key == SDLK_k){
            backwardsView = !backwardsView;
         }
         if(Event.Key == SDLK_j){
            overView = NEXT(overView);
            //printf("Angle: %d\n", ANGLE(overView, backwardsView));
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
            //finished = true;
            exit(1);
         }
      }
      //Check if key let go, Not sure if this will work in here.
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

   //Runs at program close (currently not implemented)
   void end()
   {
      stopSoundtrack();
      Mix_CloseAudio();
      our_font.clean();
   }

   void PrepShadow() {
      renderShadow = new CMeshRenderable();
      renderShadow->setMesh(discMesh);
      renderShadow->setShader(DiffuseTexture);

      Application.getSceneManager().addRenderable(renderShadow);
   }

   void PrepBlock(float x, float y, float w, float h) {
      CMeshRenderable *tempBlock;
      blocks.push_back(tempBlock = new CMeshRenderable());
      tempBlock->setMesh(cubeMesh);
      tempBlock->setTexture(grassTxt);
      tempBlock->setShader(DiffuseTexture);
      tempBlock->setTranslation(SVector3((x+(x+w))/2, (y+(y+h))/2, 0));
      tempBlock->setScale(SVector3(w, h, 1));
      tempBlock->setRotation(SVector3(0, 0, 0));
      Application.getSceneManager().addRenderable(tempBlock);
   }

   void PrepGrass(float x, float y, float w, float h) {
      CMeshRenderable *tempBlock;
      blocks.push_back(tempBlock = new CMeshRenderable());
      tempBlock->setMesh(cubeMesh);
      tempBlock->setTexture(dirtTxt);
      tempBlock->setShader(DiffuseTexture);
      tempBlock->setTranslation(SVector3((x+(x+w))/2, (y+(y+h))/2, 0));
      tempBlock->setScale(SVector3(w, h, 5));
      Application.getSceneManager().addRenderable(tempBlock);

   }

   void PrepSky() {
      CMeshRenderable *tempBlock;
      blocks.push_back(tempBlock = new CMeshRenderable());
      tempBlock->setMesh(cubeMesh);
      tempBlock->setTexture(skyTxt);
      tempBlock->setShader(DiffuseTexture);
      tempBlock->setTranslation(SVector3(0, 24, -2.5));
      tempBlock->setScale(SVector3(100, 50, 1));
      Application.getSceneManager().addRenderable(tempBlock);

   }

   /*
      void PrepElement(float x, float y, float scale, CMesh* model) { 
      CMeshRenderable *tempElement;
      elements.push_back(tempElement = new CMeshRenderable());
      tempElement->setMesh(model);
//tempElement->setTexture(dirtTxt);
tempElement->setShader(Flat);
tempElement->setTranslation(SVector3(x, y, z));
tempElement->setScale(SVector3(scale));
//TODO: make the rotation an argument?
tempElement->setRotation(SVector3(-90, 0, 0));
Application.getSceneManager().addRenderable(tempElement);
}
    */


CMeshRenderable* PrepEnemy(float x, float y) {
   CMeshRenderable *tempEnemy;
   enemies.push_back(tempEnemy = new CMeshRenderable());
   tempEnemy->setMesh(basicTreeMesh);
   //tempEnemy->setTexture(dirtTxt);
   tempEnemy->setShader(Flat);
   Application.getSceneManager().addRenderable(tempEnemy);
   return tempEnemy;
}

};

int main (int argc, char *argv[])
{
   CApplication & Application = Application.get();
   Application.init(SPosition2(WindowWidth = 1024, WindowHeight = 768));

   Application.getStateManager().setState<CGameState>();

   Application.run();

   return 0;
}

void LoadShaders() {
   Flat = CShaderLoader::loadShader("Shaders/Flat");
   Diffuse = CShaderLoader::loadShader("Shaders/Diffuse");
   DiffuseTexture = CShaderLoader::loadShader("Shaders/DiffuseTexture");
    normalColor = CShaderLoader::loadShader("Shaders/normalColor");
}


void Load3DS()
{
   basicTreeMesh = CMeshLoader::load3dsMesh("Models/tree.3ds");
   if (basicTreeMesh) {
      basicTreeMesh->resizeMesh(SVector3(0.5));
      basicTreeMesh->centerMeshByExtents(SVector3(0));
      basicTreeMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load the basic tree mesh\n");
   }



   cabbageMesh = CMeshLoader::load3dsMesh("Models/crappycabbage.3ds");
   if (cabbageMesh) {
      cabbageMesh->resizeMesh(SVector3(0.5));
      cabbageMesh->centerMeshByExtents(SVector3(0));
      cabbageMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load the christmas tree mesh\n");
   }



   blueFlwrMesh = CMeshLoader::load3dsMesh("Models/blueFlower.3ds");
   if (blueFlwrMesh) {
      blueFlwrMesh->centerMeshByExtents(SVector3(0));
      blueFlwrMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load blue flower mesh.\n");
   }



   pinkFlwrMesh = CMeshLoader::load3dsMesh("Models/pinkFlower.3ds");
   if (pinkFlwrMesh) {
      pinkFlwrMesh->centerMeshByExtents(SVector3(0));
      pinkFlwrMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load pink flower mesh.\n");
   }

   ficusMesh = CMeshLoader::load3dsMesh("Models/ficus.3ds");
   if (ficusMesh) {
      ficusMesh->centerMeshByExtents(SVector3(0));
      ficusMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load ficus mesh.\n");
   }

   poinMesh = CMeshLoader::load3dsMesh("Models/poin.3ds");
   if (poinMesh) {
      poinMesh->centerMeshByExtents(SVector3(0));
      poinMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load poinsetta mesh.\n");
   }



   /* Load enemy mesh */
}

void LoadTextures()
{
   grassImg = CImageLoader::loadImage("Textures/grass.bmp");
   skyImg = CImageLoader::loadImage("Textures/sky.bmp");
   dirtImg = CImageLoader::loadImage("Textures/dirt.bmp");
   blueFlwrImg = CImageLoader::loadImage("Textures/blueFlower.bmp");
   pinkFlwrImg = CImageLoader::loadImage("Textures/pinkFlower.bmp");
   poinImg = CImageLoader::loadImage("Textures/poin.bmp");

   grassTxt = new CTexture(grassImg);
   skyTxt = new CTexture(skyImg);
   dirtTxt = new CTexture(dirtImg);
   blueFlwrTxt = new CTexture(blueFlwrImg);
   pinkFlwrTxt = new CTexture(pinkFlwrImg);
   poinTxt = new CTexture(poinImg);
}

void PrepMeshes()
{
   renderBasicTree = new CMeshRenderable();
   renderBasicTree->setMesh(basicTreeMesh);
   renderBasicTree->setShader(Flat);

   renderChristmasTree = new CMeshRenderable();
   renderChristmasTree->setMesh(cabbageMesh);
   renderChristmasTree->setShader(Flat);

   playerRenderable = new CMeshRenderable();
   playerRenderable->setMesh(cabbageMesh);
   playerRenderable->setShader(Flat);
   playerRenderable->setScale(SVector3(2));

   renderBlueFlwr = new CMeshRenderable();
   renderBlueFlwr->setMesh(blueFlwrMesh);
   renderBlueFlwr->setTexture(blueFlwrTxt);
   renderBlueFlwr->setShader(DiffuseTexture);
   renderBlueFlwr->setTranslation(SVector3(-23, .18, 2));
   renderBlueFlwr->setScale(SVector3(.36));
   renderBlueFlwr->setRotation(SVector3(-90, 0, 0));

   renderPinkFlwr = new CMeshRenderable();
   renderPinkFlwr->setMesh(pinkFlwrMesh);
   renderPinkFlwr->setTexture(pinkFlwrTxt);
   renderPinkFlwr->setTranslation(SVector3(-20, .2, 2));
   renderPinkFlwr->setScale(SVector3(.36));
   renderPinkFlwr->setRotation(SVector3(-90, 0, 0));
   renderPinkFlwr->setShader(DiffuseTexture);

   renderFicus = new CMeshRenderable();
   renderFicus->setMesh(ficusMesh);
   renderFicus->setTranslation(SVector3(-21, .5, 2));
   renderFicus->setScale(SVector3(1.0));
   renderFicus->setRotation(SVector3(-90, 0, 0));
   renderFicus->setTexture(blueFlwrTxt);
   renderFicus->setShader(DiffuseTexture);

   renderPoin = new CMeshRenderable();
   renderPoin->setMesh(poinMesh);
   renderPoin->setTranslation(SVector3(-19, .5, 2));
   renderPoin->setScale(SVector3(.75));
   renderPoin->setRotation(SVector3(-90, 0, 0));
   renderPoin->setTexture(poinTxt);
   renderPoin->setShader(DiffuseTexture);
}
