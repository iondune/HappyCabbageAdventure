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
CLight * PlayerLight;
CObject *Floor, *Block;
CPlayerView *PlayerView;
std::vector<CElevator*> elevators;

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

void CGameState::loadWorld(std::vector<CPlaceable*> *list)
{
    int x,y,w,d,h,t;
    //float spd, rng;

    irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader("test.xml");
	while (xml && xml->read())
	{
      switch(xml->getNodeType())
      {
      case irr::io::EXN_TEXT:
         break;
      case irr::io::EXN_ELEMENT:
         if(!strcmp("CBlock", xml->getNodeName()))
         {
            CBlock * ptr;
            // id, X, Y, height, width / from 0,1,2 so on
            x = xml->getAttributeValueAsInt(0);
            y = xml->getAttributeValueAsInt(1);
            h = xml->getAttributeValueAsInt(2);
            w = xml->getAttributeValueAsInt(3);
            d = xml->getAttributeValueAsInt(4);
            t = xml->getAttributeValueAsInt(5);
            list->push_back(ptr = new CBlock((float)x,(float)y,w,h,d,t));
            if(xml->getAttributeValueAsInt(6)) {
               ptr->isMovingPlatform = 1;
               ptr->Range = (int) xml->getAttributeValueAsFloat(7); //Range
               ptr->Speed = (int) xml->getAttributeValueAsFloat(8); //Speed
            }
            else
               ptr->isMovingPlatform = 0;
         }
         if(!strcmp("CEnemy", xml->getNodeName()))
         {
            CEnemy *cen;
            x = xml->getAttributeValueAsInt(0);
            y = xml->getAttributeValueAsInt(1);
            h = xml->getAttributeValueAsInt(2);
            w = xml->getAttributeValueAsInt(3);
            t = xml->getAttributeValueAsInt(4);
            list->push_back(cen = new CEnemy((float)x,(float)y,w,h,t));
            cen->setShader(Diffuse);
            cen->isMovingPlatform = 0;
         }
         break;
      }
   }
}

void CGameState::EngineInit( void ) {
   Engine = new CEngine();
   Player = Engine->addActor();
   Player->setArea(SRect2(-24.5f, 3, 1, 1));

   Derp = Engine->addActor();
   Derp->setArea(SRect2(-17, 0, 1, 1));

   Floor = Engine->addObject();
   Floor->setArea(SRect2(-25, -1, 200, 1));
   PrepGrass(-25, -1, 200, 1);
   PrepSky();

   SRect2 area;

   GameplayManager = new CGameplayManager(Player, Engine);

   GameEventManager = & GameplayManager->getGameEventManager();
   GameEventManager->OnEnemyDeath.connect(& GameEventReceiver, &CGameEventReceiver::OnEnemyDeath);
   Application.getEventManager().OnGameTickStart.connect(& GameEventReceiver, & CGameEventReceiver::OnGameTickStart);

   float i = 0;
   float j = 0;

   std::vector<CPlaceable*> list;
   CObject *lastOne = NULL;
   loadWorld(&list);

   std::vector<CPlaceable*>::iterator it;
   for(it=list.begin();it<list.end();it++) {
      (*it)->setupItem(DiffuseTexture, Engine, GameplayManager);
      printf(" %d\n", (*it)->isMovingPlatform);
      if((*it)->isMovingPlatform) {
         elevators.push_back(((CBlock*)(*it))->elevator);
         lastOne = ((CBlock*)(*it))->elevator;
         printf("this is a moving platform\n");
      }
      else
         printf("this is NOT a moving platform\n");
   }

   Block = Engine->addObject();
   Block->setArea(SRect2(172.f,0.f, 4.f, 10.f));
   PrepBlock(172,0,4,10);
   if(lastOne) {
      GameplayManager->setVictoryFlag(Block);
   }


   /*
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
   */
}

#define PARTICLE
#ifdef PARTICLE
CParticleEngine * particleLeafEngine;
CParticleEngine * particleCubeEngine;
#endif

//Initalizer fxn
void CGameState::begin()
{
#ifdef PARTICLE
   particleLeafEngine = particleCubeEngine = 0;
#endif
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

    SceneManager.Lights.clear();

    SceneManager.Lights.push_back(new CLight());
    SceneManager.Lights.back()->Color = SVector3(LightBrightness);
    SceneManager.Lights.back()->Position = SVector3(200.f, 2.f, 15.f);

    SceneManager.Lights.push_back(new CLight());
    SceneManager.Lights.back()->Color = SVector3(LightBrightness);
    SceneManager.Lights.back()->Position = SVector3(70.f, -2.f, 30.f);

    SceneManager.Lights.push_back(new CLight());
    SceneManager.Lights.back()->Color = SVector3(LightBrightness);
    SceneManager.Lights.back()->Position = SVector3(-30.f, 0.f, 15.f);

    SceneManager.Lights.push_back(new CLight());
    SceneManager.Lights.back()->Color = SVector3(LightBrightness);
    PlayerLight = SceneManager.Lights.back();


   LoadShaders();

   Load3DS();
   LoadTextures();
   BlockMesh();

   DiscMesh();

   //Load the meshes into VBOs
   PrepMeshes();

   PrepShadow();

   Application.getSceneManager().addSceneObject(playerRenderable);
   Application.getSceneManager().addSceneObject(renderFlag);
   Application.getSceneManager().addSceneObject(flagLogo);

   srand((unsigned int) time(NULL));

   int random;

   if(!lowDef) {
      for (int n = 0; n < 100; n++) {
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

      for (int n = 0; n < 50; n++) {
         random = rand() % 3;

         if (n % 2 == 0)
            drawBasicTree(-20.4f + n * 4, 2.0f, 2, 8.0f, Application);
         else {

            if (random < 2) {
               drawBasicTree(-22.4f + n * 4, 2.0f, -2, 8.0f, Application);
            }
            else if (random == 2) {
               drawChristmasTree(-22.4f + n * 4, 1.4f, -2, 6.0f, Application);
            }
         }
      }
   }

   //Initialize Fxns
   EngineInit();
   ViewInit();
   fps = timeTotal = 0;
   numFrames = 0;

   printf("CGameState:  Begin Function Complete\n");


   Application.skipElapsedTime();
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

         else if (GameplayManager->isWon()) {

            Player->setAction(CActor::EActionType::None);

            PlayerView->setState(CPlayerView::State::Standing);

            Player->setJumping(true);

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
   PlayerView->setMiddle(middleOfPlayer);
   PlayerView->setGround(Engine->getHeightBelow(Player));

   PlayerView->establishCamera(Camera, ANGLE(overView, backwardsView));

#ifdef PARTICLE
   if(particleLeafEngine && !particleLeafEngine->dead) {
      particleLeafEngine->setCenterPos(SVector3(Player->getArea().getCenter().X, Player->getArea().getCenter().Y, 0));
      particleLeafEngine->step(Application.getElapsedTime());
   }
   if(particleCubeEngine && !particleCubeEngine->dead) {
      particleCubeEngine->setLookRight(PlayerView->getLookRight());
      particleCubeEngine->setCenterPos(SVector3(Player->getArea().getCenter().X, Player->getArea().getCenter().Y, 0));
      particleCubeEngine->step(Application.getElapsedTime());
   }
#endif

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
   for (std::vector<CBadGuy*>::iterator it = GameplayManager->Enemies.begin(); it != GameplayManager->Enemies.end(); ++ it)
   {
      (*it)->doRenderable();
//      ((CMeshSceneObject*)((*it)->Renderable))->setTranslation(SVector3((*it)->Actor->getArea().getCenter().X, (*it)->Actor->getArea().getCenter().Y, 0));
//      if((*it)->Actor->getVelocity().X < -0.01f)
//         ((CMeshSceneObject*)((*it)->Renderable))->setScale(SVector3(-1,1,1));
//      else if((*it)->Actor->getVelocity().X > 0.01f)
//         ((CMeshSceneObject*)((*it)->Renderable))->setScale(SVector3(1,1,1));
      i++;
   }

   std::vector<CElevator*>::iterator it;
   for(it=elevators.begin();it<elevators.end();it++) {
      CElevator * ptr = (*it);
      SVector2 pos = ptr->getArea().Position;
      SVector2 size = ptr->getArea().Size;
      ptr->getRenderable()->setTranslation(SVector3(pos.X + (float)size.X/2, pos.Y + (float)size.Y/2, 0));
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

   PlayerLight->Position = playerRenderable->getTranslation();

   oldDisplay();

   Application.getSceneManager().drawAll();

   if (! GameplayManager->isPlayerAlive() || GameplayManager->isWon()) {
      if(GameplayManager->isWon()) {
         if (playVictory) {
            Mix_HaltMusic();
            Mix_PlayChannel(-1, victory,0);
            playVictory = false;
         }
         freetype::print(our_font, 50, WindowHeight - 240.f, "CONGRATULATIONS! YOU HAVE WON!");
      }

      else {
         //Chris Code.  Play Death Sound
         if (playDead) {
            Mix_HaltMusic();
            Mix_PlayChannel(-1, die, 0); //Only play once
            playDead = false;
         }

         freetype::print(our_font, 50, WindowHeight - 240.f, "GAME OVER! YOU ARE DEAD.");
      }
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
#ifdef PARTICLE
      if(Event.Key == SDLK_e) {
         if(!particleCubeEngine || (particleCubeEngine && particleCubeEngine->dead))
            particleCubeEngine = new CParticleEngine(SVector3(0, 1, 0), 80, 6, CUBE_PARTICLE);
      }
      if(Event.Key == SDLK_r) {
         if(!particleLeafEngine || (particleLeafEngine && particleLeafEngine->dead))
            particleLeafEngine = new CParticleEngine(SVector3(0, 1, 0), 30, 6, LEAF_PARTICLE);
      }
#endif
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

void CGameState::end()
{   
   stopSoundtrack();
   Mix_CloseAudio();
   our_font.clean();

   Application.getSceneManager().removeAllSceneObjects();
}

void CGameState::PrepShadow() {
   renderShadow = new CMeshSceneObject();
   renderShadow->setMesh(discMesh);
   renderShadow->setShader(DiffuseTexture);

   Application.getSceneManager().addSceneObject(renderShadow);
}

void CGameState::PrepBlock(float x, float y, float w, float h) {
   CMeshSceneObject *tempBlock;
   blocks.push_back(tempBlock = new CMeshSceneObject());
   tempBlock->setMesh(cubeMesh);
   tempBlock->setTexture(dirtTxt);
   tempBlock->setShader(DiffuseTexture);
   tempBlock->setTranslation(SVector3((x+(x+w))/2, (y+(y+h))/2, 0));
   tempBlock->setScale(SVector3(w, h, 1));
   tempBlock->setRotation(SVector3(0, 0, 0));
   Application.getSceneManager().addSceneObject(tempBlock);
}

void CGameState::PrepGrass(float x, float y, float w, float h) {
   CMeshSceneObject *tempBlock;
   blocks.push_back(tempBlock = new CMeshSceneObject());
   tempBlock->setMesh(cubeMesh);
   tempBlock->setTexture(grassTxt);
   tempBlock->setShader(DiffuseTexture);
   tempBlock->setTranslation(SVector3((x+(x+w))/2, (y+(y+h))/2, 0));
   tempBlock->setScale(SVector3(w, h, 5));
   Application.getSceneManager().addSceneObject(tempBlock);

}

void CGameState::PrepSky() {
   CMeshSceneObject *tempBlock;
   blocks.push_back(tempBlock = new CMeshSceneObject());
   tempBlock->setMesh(cubeMesh);
   tempBlock->setTexture(skyTxt);
   tempBlock->setShader(DiffuseTexture);
   tempBlock->setTranslation(SVector3(0, 24, -2.5));
   tempBlock->setScale(SVector3(400, 50, 1));
   Application.getSceneManager().addSceneObject(tempBlock);

}

CMeshSceneObject* CGameState::PrepEnemy(float x, float y) {
   CMeshSceneObject *tempEnemy;
   enemies.push_back(tempEnemy = new CMeshSceneObject());
   tempEnemy->setMesh(enemyMesh);
   //tempEnemy->setTexture(dirtTxt);
   tempEnemy->setShader(Flat);
   tempEnemy->setRotation(SVector3(-90, 0, 0));
   Application.getSceneManager().addSceneObject(tempEnemy);
   return tempEnemy;
}

void LoadShaders() {
   Flat = CShaderLoader::loadShader("Diffuse");
   Diffuse = CShaderLoader::loadShader("Diffuse");
   DiffuseTexture = CShaderLoader::loadShader("DiffuseTexture");
   normalColor = CShaderLoader::loadShader("Simple");
   Toon = CShaderLoader::loadShader("Toon");
   //Toon = Diffuse;
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

   basicTreeMesh = CMeshLoader::load3dsMesh("Models/tree4.3ds");
   if (basicTreeMesh) {
      basicTreeMesh->resizeMesh(SVector3(0.5));
      basicTreeMesh->centerMeshByExtents(SVector3(0));
      basicTreeMesh->calculateNormalsPerVertex();
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

   flagMesh = CMeshLoader::load3dsMesh("Models/flag2.3ds");
   if (flagMesh) {
      flagMesh->centerMeshByExtents(SVector3(0));
      flagMesh->calculateNormalsPerFace();
   }

   else {
      fprintf(stderr, "Failed to load flag mesh.\n");
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
   flagImg = CImageLoader::loadImage("Textures/white.bmp");

   grassTxt = new CTexture(grassImg);
   skyTxt = new CTexture(skyImg);
   dirtTxt = new CTexture(dirtImg);
   blueFlwrTxt = new CTexture(blueFlwrImg);
   pinkFlwrTxt = new CTexture(pinkFlwrImg);
   poinTxt = new CTexture(poinImg);
   flagTxt = new CTexture(flagImg);
}

void PrepMeshes()
{
   renderBasicTree = new CMeshSceneObject();
   renderBasicTree->setMesh(basicTreeMesh);
   renderBasicTree->setShader(Toon);

   renderChristmasTree = new CMeshSceneObject();
   renderChristmasTree->setMesh(cabbageMesh);
   renderChristmasTree->setShader(Flat);

   playerRenderable = new CMeshSceneObject();
   playerRenderable->setMesh(cabbageMesh);
   playerRenderable->setShader(Toon);
   playerRenderable->setScale(SVector3(2));

   renderBlueFlwr = new CMeshSceneObject();
   renderBlueFlwr->setMesh(blueFlwrMesh);
   renderBlueFlwr->setTexture(blueFlwrTxt);
   renderBlueFlwr->setShader(DiffuseTexture);
   renderBlueFlwr->setTranslation(SVector3(-23.f, .18f, 2));
   renderBlueFlwr->setScale(SVector3(.36f));
   renderBlueFlwr->setRotation(SVector3(-90, 0, 0));

   renderPinkFlwr = new CMeshSceneObject();
   renderPinkFlwr->setMesh(pinkFlwrMesh);
   renderPinkFlwr->setTexture(pinkFlwrTxt);
   renderPinkFlwr->setTranslation(SVector3(-20, .2f, 2));
   renderPinkFlwr->setScale(SVector3(.36f));
   renderPinkFlwr->setRotation(SVector3(-90, 0, 0));
   renderPinkFlwr->setShader(DiffuseTexture);

   renderFicus = new CMeshSceneObject();
   renderFicus->setMesh(ficusMesh);
   renderFicus->setTranslation(SVector3(-21, .5f, 2));
   renderFicus->setScale(SVector3(1.0));
   renderFicus->setRotation(SVector3(-90, 0, 0));
   renderFicus->setTexture(blueFlwrTxt);
   renderFicus->setShader(DiffuseTexture);

   renderPoin = new CMeshSceneObject();
   renderPoin->setMesh(poinMesh);
   renderPoin->setTranslation(SVector3(-19, .5f, 2));
   renderPoin->setScale(SVector3(.75));
   renderPoin->setRotation(SVector3(-90, 0, 0));
   renderPoin->setTexture(poinTxt);
   renderPoin->setShader(DiffuseTexture);

   renderFlag = new CMeshSceneObject();
   renderFlag->setMesh(flagMesh);
   renderFlag->setTranslation(SVector3(170, .5f, 1.0f));
   renderFlag->setRotation(SVector3(-90,0,0));
   renderFlag->setScale(SVector3(.0150f, .00025f,.0016f));
   renderFlag->setTexture(flagTxt);
   renderFlag->setShader(DiffuseTexture);

   flagLogo = new CMeshSceneObject();
   flagLogo->setMesh(cabbageMesh);
   flagLogo->setTranslation(SVector3(170, .9f, 1.0f));
   flagLogo->setRotation(SVector3(-90,0,0));
   flagLogo->setScale(SVector3(.75f));
   flagLogo->setShader(Flat);
}
