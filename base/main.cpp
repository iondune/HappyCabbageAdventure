#include <iostream>

#ifdef __unix__
#include <GL/gl.h>
#include <SDL/SDL.h>
#endif

#include "header.h"

#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"

#define TREE_Y_OFFSET 2.1
#define ANGLE(j,k) (j==2?3:(j?2:(k?1:0)))
#define NEXT(j) (j==2?0:(j?2:1))

//Boolean integers for keypressing
int aDown = 0, dDown = 0, spaceDown = 0, wDown = 0, sDown = 0;
int backwardsView = 0, overView = 0;


freetype::font_data our_font;

//Variables need to create VBOs of meshes, textures, and shaders
CShader *Shader, *Diffuse, *DiffuseTexture;  //Use Diffuse for trees (doesn't need texture)
CImage *grassImg, *skyImg, *dirtImg;
CTexture *grassTxt, *skyTxt, *dirtTxt;
CMesh *basicTreeMesh, *christmasTreeMesh;
CMeshRenderable *renderChristmasTree, *renderBasicTree;

int WindowWidth, WindowHeight;

void Load3DS();
void LoadTextures();
void PrepMeshes();

using namespace Cabbage::Collider;
CEngine *Engine;
CActor *Player, *Derp;
CObject *Floor, *Block;
CPlayerView *PlayerView;

CGameplayManager * GameplayManager;

std::vector<CMeshRenderable*> blocks;

void ViewInit( void ) {
   PlayerView = new CPlayerView();
   CMeshRenderable *playerRenderable = new CMeshRenderable();
   playerRenderable->setMesh(christmasTreeMesh);
   playerRenderable->setShader(Diffuse);
   playerRenderable->setScale(SVector3(2));
   CApplication::get().getSceneManager().addRenderable(playerRenderable);
   PlayerView->setRenderable(playerRenderable);
}

CMesh *cubeMesh;
void BlockMesh() {
   cubeMesh = CMeshLoader::createCubeMesh();
}

std::vector<CMeshRenderable*> enemies;

class CGameState : public CState<CGameState>
{
   CApplication & Application;

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
   PrepBlock(-25, -1, 50, 1);

   SRect2 area;

   GameplayManager = new CGameplayManager(Player, Engine);
   GameplayManager->addEnemy(SVector2(-10, 4));
   GameplayManager->addEnemy(SVector2(-5, 40));
   GameplayManager->addEnemy(SVector2(0, 40));
   GameplayManager->addEnemy(SVector2(5, 40));
   GameplayManager->addEnemy(SVector2(10, 40));
   GameplayManager->addEnemy(SVector2(15, 40));
   GameplayManager->addEnemy(SVector2(20, 40));
   GameplayManager->addEnemy(SVector2(25, 40));
   GameplayManager->addEnemy(SVector2(25, 45));
   GameplayManager->addEnemy(SVector2(25, 50));
   PrepEnemy(-10, 4);
   PrepEnemy(-5, 40);
   PrepEnemy(0, 40);
   PrepEnemy(5, 40);
   PrepEnemy(10, 40);
   PrepEnemy(15, 40);
   PrepEnemy(20, 40);
   PrepEnemy(25, 40);
   PrepEnemy(25, 45);
   PrepEnemy(25, 50);


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
         glClearColor(0.6f, 0.3f, 0.9f, 1.0f);

         glEnable(GL_DEPTH_TEST);
         glDepthFunc(GL_LEQUAL);
         //glEnable(GL_CULL_FACE);
         //glCullFace(GL_BACK);
         
         SDL_WM_SetCaption("Happy Cabbage Adventure", NULL);

         //Initialize Font
         our_font.init("WIFFLES_.TTF", 30);

         Camera = new CCamera((float)WindowWidth/(float)WindowHeight, 0.01f, 100.f, 60.f);
         CApplication::get().getSceneManager().setActiveCamera(Camera);

//This code should all be working.  Commented out so can make sure have the basics working first
         //Load shader and attributes
         Diffuse = CShaderLoader::loadShader("Shaders/Diffuse");
         DiffuseTexture = CShaderLoader::loadShader("Shaders/DiffuseTexture");

         //DemoLight();
         setupSoundtrack();
         startSoundtrack();

         //Set up camera  I DON'T KNOW WHAT TO DO HERE!
         //float AspectRatio = (float)WindowWidth/(float)Windowheight;
         //Camera = new CCamera(AspectRatio, ...

         Load3DS();
         LoadTextures();
         BlockMesh();

         //Load the meshes into VBOs
         PrepMeshes();
         Application.getSceneManager().addRenderable(renderBasicTree);
         Application.getSceneManager().addRenderable(renderChristmasTree);
         //Initialize Fxns
         EngineInit();
         ViewInit();

      }

      CCamera *Camera;

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

         Engine->updateAll(Application.getElapsedTime()); //Might be an issue (since updateAll requires float and delta is a UInt32)
         PlayerView->step(Application.getElapsedTime()*1000);

         GameplayManager->run(Application.getElapsedTime());

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
   renderBasicTree->setScale(SVector3(0.5));
   renderBasicTree->setRotation(SVector3(-90, 0, -90));

   //ENEMY DISPLAY
   int i = 0;
   for (CGameplayManager::EnemyList::iterator it = GameplayManager->Enemies.begin(); it != GameplayManager->Enemies.end(); ++ it)
   {
      enemies[i]->setTranslation(SVector3(it->Actor->getArea().getCenter().X, it->Actor->getArea().getCenter().Y, 0));
      i++;
   }



   // ...and by spinning it around
   static float const RotationSpeed = 50.f;
   //Rotation.X += RotationSpeed*Delta;
   //Rotation.Y += RotationSpeed*Delta*2;

   glLoadIdentity();

   freetype::print(our_font, 10, WindowHeight-40, "Elapsed Time: %f\n"
         "Health: %d\nspaceDown: %d\nwDown: %d\nsDown: %d\noverView: %d ", Application.getElapsedTime(), GameplayManager->getPlayerHealth(), spaceDown, wDown, sDown, overView);

   if (! GameplayManager->isPlayerAlive()) {
      //Chris Code.  Play Death Sound
      if (playDead) {
         Mix_HaltMusic();
         Mix_PlayChannel(-1, die, 0); //Only play once
         playDead = false;
      }
      freetype::print(our_font, 50, WindowHeight - 240, "GAME OVER! YOU ARE DEAD");
   }
}

      //Runs at very start of display
      void OnRenderStart(float const Elapsed)
      {
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         glMatrixMode(GL_MODELVIEW);
         glLoadIdentity();

         oldDisplay();

         Application.getSceneManager().drawAll();

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

      void PrepBlock(float x, float y, float w, float h) {
         CMeshRenderable *tempBlock;
         blocks.push_back(tempBlock = new CMeshRenderable());
         tempBlock->setMesh(cubeMesh);
         //tempBlock->setTexture(dirtTxt);
         tempBlock->setShader(Diffuse);
         tempBlock->setTranslation(SVector3((x+(x+w))/2, (y+(y+h))/2, 0));
         tempBlock->setScale(SVector3(w, h, 1));
         tempBlock->setRotation(SVector3(0, 0, 0));
         Application.getSceneManager().addRenderable(tempBlock);
      }

      void PrepEnemy(float x, float y) {
         CMeshRenderable *tempEnemy;
         enemies.push_back(tempEnemy = new CMeshRenderable());
         tempEnemy->setMesh(basicTreeMesh);
         //tempEnemy->setTexture(dirtTxt);
         tempEnemy->setShader(Diffuse);
         Application.getSceneManager().addRenderable(tempEnemy);
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

   christmasTreeMesh = CMeshLoader::load3dsMesh("Models/christmasTree.3ds");
   if (christmasTreeMesh) {
      christmasTreeMesh->resizeMesh(SVector3(0.5));
      christmasTreeMesh->centerMeshByExtents(SVector3(0));
      christmasTreeMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load the christmas tree mesh\n");
   }

   /* Load cabbage mesh */
   /* Load enemy mesh */
}

void LoadTextures()
{
   CImage *grassImg = CImageLoader::loadImage("Textures/grass.bmp");
   CImage *skyImg = CImageLoader::loadImage("Textures/sky.bmp");
   CImage *dirtImg = CImageLoader::loadImage("Textures/dirt.bmp");

   if(!grassImg)
      printf("ERROR!!!!\n");
   grassTxt = new CTexture(grassImg);
   skyTxt = new CTexture(skyImg);
   dirtTxt = new CTexture(dirtImg);
}

void PrepMeshes()
{
   renderBasicTree = new CMeshRenderable();
   renderBasicTree->setMesh(basicTreeMesh);
   renderBasicTree->setShader(Diffuse);

   renderChristmasTree = new CMeshRenderable();
   renderChristmasTree->setMesh(christmasTreeMesh);
   renderChristmasTree->setShader(Diffuse);
}
