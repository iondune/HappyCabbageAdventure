#include "CGameState.h"
/* These are here because someone doesn't use extern, or put prototypes in their header files */
#include "draw.h"


//Boolean integers for keypressing
int aDown = 0, dDown = 0, spaceDown = 0, wDown = 0, sDown = 0;
int backwardsView = 0, overView = 0;

int WindowWidth, WindowHeight;

void Load3DS();
void LoadShaders();
void LoadTextures();
void PrepMeshes();
void EngineInit();

using namespace Cabbage::Collider;
CEngine *Engine;
CActor *Player, *Derp;
SLight * PlayerLight;
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

CGameState::CGameState()
: Application (CApplication::get())
{}

void CGameState::EngineInit( void ) {
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

   CBlock *m_block = new CBlock(0, 0, 3, 5);
   m_block->setupItem(DiffuseTexture, Engine, GameplayManager);

   for(j = 0; j < 10; j+=2.5f) {
      Block = Engine->addObject();
      area = SRect2(-15.f + j, 1.5f + j, 2.f, 1);
      PrepBlock(-15.f + j, 1.5f + j, 2.f, 1);
      Block->setArea(area);
   }

   Block = Engine->addObject();
   area = SRect2(-22.f, 7.f, 6.f, 0.2f);
   PrepBlock(-22.f, 7.f, 6.f, 0.2f);
   Block->setArea(area);

   Block = Engine->addObject();
   area = SRect2(-22.f, 7.f, 0.2f, 3.f);
   PrepBlock(-22.f, 7.f, 0.2f, 3.f);
   Block->setArea(area);

   for(; i < 12; i++) {
      Block = Engine->addObject();
      area = SRect2(-i + 5.f, 0.f, 1.f, i+1.f);
      PrepBlock(-i + 5.f, 0.f, 1.f, i+1.f);
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
void CGameState::begin()
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
   float const LightBrightness = 0.2f;

    CSceneManager & SceneManager = Application.getSceneManager();
    SceneManager.Lights.push_back(SLight());
    SceneManager.Lights.back().ColorUniform->Value = SVector3(LightBrightness);
    SceneManager.Lights.back().PositionUniform->Value = SVector3(15.f, 2.f, 3.f);

    SceneManager.Lights.push_back(SLight());
    SceneManager.Lights.back().ColorUniform->Value = SVector3(LightBrightness);
    SceneManager.Lights.back().PositionUniform->Value = SVector3(-1.f, -2.f, -3.f);

    SceneManager.Lights.push_back(SLight());
    SceneManager.Lights.back().ColorUniform->Value = SVector3(LightBrightness);
    SceneManager.Lights.back().PositionUniform->Value = SVector3(-15.f, 0.f, 0.f);

    SceneManager.Lights.push_back(SLight());
    SceneManager.Lights.back().ColorUniform->Value = SVector3(LightBrightness);
    PlayerLight = & SceneManager.Lights.back();


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
   Application.getSceneManager().addRenderable(playerRenderable);

   srand((unsigned int) time(NULL));

   int random;

   for (int n = 0; n < 24; n++) {
      random = rand() % 8;

      if (n % 2 == 0)
         if (random < 3 ) {
            drawBlueFlwr(-22.f + n * 2, -1, 2, .6f, Application);
         }
         else if (random < 6) {
            drawPinkFlwr(-22.f + n * 2, -1, 2, .6f, Application);
         }

         else {
            drawPoin(-22.f + n * 2, -1, 2, 1.f, Application);
         }
      else
         if (random < 3) {
            drawBlueFlwr(-22.f + n * 2, -1, -2, .7f, Application);
         }
         else if (random < 6) {
            drawPinkFlwr(-22.f + n * 2, -1, -2, .7f, Application);
         }
         else {
            drawPoin(-22.f + n * 2, .2f, -2, 1.f, Application);
         }
   }

   for (int n = 0; n < 12; n++) {
      random = rand() % 3;

      if (n % 2 == 0)
         if (random < 2) {
            drawBasicTree(-20.4f + n * 4, 2.0f, 2, 8.0f, Application);
         }
         else if (random == 2) {
            drawChristmasTree(-20.4f + n * 4, 1.5, 2, 6.0f, Application);
         }
         else {
            drawFicus(-21.f + n * 4, -1, 2, 1.0f, Application);
         }
      else {

         if (random < 2) {
            drawBasicTree(-22.4f + n * 4, 2.0f, -2, 8.0f, Application);
         }
         else if (random == 2) {
            drawChristmasTree(-22.4f + n * 4, 1.4f, -2, 6.0f, Application);
         }
         else {
            drawFicus(-21.f + n * 4, -1, -2, 5.0f, Application);
         }
      }
   }
   //drawFicus(-15, -1, -2, 2.0, Application);
   //drawBasicTree(-20, -1, -2, 2.0, Application);
   //drawChristmasTree(-14, 1, 2, 5.0, Application);

   //Initialize Fxns
   EngineInit();
   ViewInit();
   fps = timeTotal = 0;
   numFrames = 0;

   printf("END OF BEGIN\n");
}

void CGameState::oldDisplay() {
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
void CGameState::OnRenderStart(float const Elapsed)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   PlayerLight->PositionUniform->Value = playerRenderable->getTranslation();

   oldDisplay();

   Application.getSceneManager().drawAll();

   if (! GameplayManager->isPlayerAlive()) {
      //Chris Code.  Play Death Sound
      if (playDead) {
         Mix_HaltMusic();
         Mix_PlayChannel(-1, die, 0); //Only play once
         playDead = false;
      }
      freetype::print(our_font, 50, WindowHeight - 240.f, "GAME OVER! YOU ARE DEAD");
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
   freetype::print(our_font, 10, WindowHeight-40.f, "Elapsed Time: %0.0f\n"
         "Health: %d\nnumKilled: %d\nFPS: %0.2f ", Application.getRunTime(), GameplayManager->getPlayerHealth(), numKilled, fps);


   SDL_GL_SwapBuffers();
}


//Sends event every time key pressed (also when held)
void CGameState::OnKeyboardEvent(SKeyboardEvent const & Event)
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
         //exit(1);
         Application.getStateManager().setState(& CMainMenuState::get());
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
void CGameState::end()
{
   stopSoundtrack();
   Mix_CloseAudio();
   our_font.clean();
}

void CGameState::PrepShadow() {
   renderShadow = new CMeshRenderable();
   renderShadow->setMesh(discMesh);
   renderShadow->getMaterial().Shader = DiffuseTexture;

   Application.getSceneManager().addRenderable(renderShadow);
}

void CGameState::PrepBlock(float x, float y, float w, float h) {
   CMeshRenderable *tempBlock;
   blocks.push_back(tempBlock = new CMeshRenderable());
   tempBlock->setMesh(cubeMesh);
   tempBlock->getMaterial().Texture = dirtTxt;
   tempBlock->getMaterial().Shader = DiffuseTexture;
   tempBlock->setTranslation(SVector3((x+(x+w))/2, (y+(y+h))/2, 0));
   tempBlock->setScale(SVector3(w, h, 1));
   tempBlock->setRotation(SVector3(0, 0, 0));
   Application.getSceneManager().addRenderable(tempBlock);
}

void CGameState::PrepGrass(float x, float y, float w, float h) {
   CMeshRenderable *tempBlock;
   blocks.push_back(tempBlock = new CMeshRenderable());
   tempBlock->setMesh(cubeMesh);
   tempBlock->getMaterial().Texture = grassTxt;
   tempBlock->getMaterial().Shader = DiffuseTexture;
   tempBlock->setTranslation(SVector3((x+(x+w))/2, (y+(y+h))/2, 0));
   tempBlock->setScale(SVector3(w, h, 5));
   Application.getSceneManager().addRenderable(tempBlock);

}

void CGameState::PrepSky() {
   CMeshRenderable *tempBlock;
   blocks.push_back(tempBlock = new CMeshRenderable());
   tempBlock->setMesh(cubeMesh);
   tempBlock->getMaterial().Texture = skyTxt;
   tempBlock->getMaterial().Shader = DiffuseTexture;
   tempBlock->setTranslation(SVector3(0, 24, -2.5));
   tempBlock->setScale(SVector3(100, 50, 1));
   Application.getSceneManager().addRenderable(tempBlock);

}

CMeshRenderable* CGameState::PrepEnemy(float x, float y) {
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
   enemyMesh = CMeshLoader::load3dsMesh("Models/rabbit.3DS");
   if(enemyMesh) {
      enemyMesh->resizeMesh(SVector3(0.5));
      enemyMesh->centerMeshByExtents(SVector3(0));
      enemyMesh->calculateNormalsPerFace();
   }

   else {
      fprintf(stderr, "Failed to load the enemy mesh\n");
   }

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



   cabbageMesh = CMeshLoader::load3dsMesh("Models/crappycabbage.3ds");
   if (cabbageMesh) {
      cabbageMesh->resizeMesh(SVector3(0.5));
      cabbageMesh->centerMeshByExtents(SVector3(0));
      cabbageMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load the cababge mesh\n");
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
   renderBasicTree->getMaterial().Shader = Flat;

   renderChristmasTree = new CMeshRenderable();
   renderChristmasTree->setMesh(cabbageMesh);
   renderChristmasTree->getMaterial().Shader = Flat;

   playerRenderable = new CMeshRenderable();
   playerRenderable->setMesh(cabbageMesh);
   playerRenderable->getMaterial().Shader = Flat;
   playerRenderable->setScale(SVector3(2));

   renderBlueFlwr = new CMeshRenderable();
   renderBlueFlwr->setMesh(blueFlwrMesh);
   renderBlueFlwr->getMaterial().Texture = blueFlwrTxt;
   renderBlueFlwr->getMaterial().Shader = DiffuseTexture;
   renderBlueFlwr->setTranslation(SVector3(-23.f, .18f, 2));
   renderBlueFlwr->setScale(SVector3(.36f));
   renderBlueFlwr->setRotation(SVector3(-90, 0, 0));

   renderPinkFlwr = new CMeshRenderable();
   renderPinkFlwr->setMesh(pinkFlwrMesh);
   renderPinkFlwr->getMaterial().Texture = pinkFlwrTxt;
   renderPinkFlwr->setTranslation(SVector3(-20, .2f, 2));
   renderPinkFlwr->setScale(SVector3(.36f));
   renderPinkFlwr->setRotation(SVector3(-90, 0, 0));
   renderPinkFlwr->getMaterial().Shader = DiffuseTexture;

   renderFicus = new CMeshRenderable();
   renderFicus->setMesh(ficusMesh);
   renderFicus->setTranslation(SVector3(-21, .5f, 2));
   renderFicus->setScale(SVector3(1.0));
   renderFicus->setRotation(SVector3(-90, 0, 0));
   renderFicus->getMaterial().Texture = blueFlwrTxt;
   renderFicus->getMaterial().Shader = DiffuseTexture;

   renderPoin = new CMeshRenderable();
   renderPoin->setMesh(poinMesh);
   renderPoin->setTranslation(SVector3(-19, .5f, 2));
   renderPoin->setScale(SVector3(.75));
   renderPoin->setRotation(SVector3(-90, 0, 0));
   renderPoin->getMaterial().Texture = (poinTxt);
   renderPoin->getMaterial().Shader = DiffuseTexture;
}