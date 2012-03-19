#include "CGameState.h"
/* These are here because someone doesn't use extern, or put prototypes in their header files */
#include "draw.h"

//Boolean integers for keypressing
int aDown = 0, dDown = 0, spaceDown = 0, wDown = 0, sDown = 0, lDown = 0;
int backwardsView = 0, overView = 0;

int WindowWidth, WindowHeight;

void Load3DS();
void LoadShaders();
void LoadTextures();
void PrepMeshes();
void EngineInit();

using namespace Cabbage::Collider;
CEngine *Engine;
CActor *Player, *Derp, *WinPlayer;
CLight * PlayerLight;
CObject *Floor, *Block, *victoryBlock;
CPlayerView *PlayerView;
std::vector<CElevator*> elevators;
int Charged = 0;
int prevHealth = 0;
int numLives = -3;
int NoClipMode = 0;

CGameplayManager *GameplayManager;

void ViewInit( void ) {
   PlayerView = new CPlayerView();
   CMeshSceneObject *renderLeftShadow, *renderRightShadow;

   renderLeftShadow = new CMeshSceneObject();
   renderLeftShadow->setMesh(discMesh);
   renderLeftShadow->setShader(ERP_DEFAULT, BlackShader);
   renderLeftShadow->setShader(ERP_DEFERRED_OBJECTS, BlackShader);
   renderLeftShadow->addUniform("cutoff", & PlayerView->LeftShadowCutoff);
   renderLeftShadow->addUniform("start", & PlayerView->LeftShadowStart);

   renderRightShadow = new CMeshSceneObject();
   renderRightShadow->setMesh(discMesh);
   renderRightShadow->setShader(ERP_DEFAULT, BlackShader);
   renderRightShadow->setShader(ERP_DEFERRED_OBJECTS, BlackShader);
   renderRightShadow->addUniform("cutoff", & PlayerView->RightShadowCutoff);
   renderRightShadow->addUniform("start", & PlayerView->RightShadowStart);

   CApplication::get().getSceneManager().addSceneObject(renderLeftShadow);
   CApplication::get().getSceneManager().addSceneObject(renderRightShadow);

   PlayerView->setRenderable(playerRenderable, renderLeftShadow, renderRightShadow);
   PlayerView->setSize(SVector2(1.0f, 1.0f));
}

void BlockMesh() {
   cubeMesh = CMeshLoader::createCubeMesh();
   cubeMesh->linearizeIndices();
   cubeMesh->calculateNormalsPerFace();
}

void DiscMesh() {
   discMesh = CMeshLoader::createDiscMesh();
   //discMesh->linearizeIndices();
   //discMesh->calculateNormalsPerFace();
}

CGameState::CGameState()
: Application (CApplication::get())
{}

void CGameState::loadWorld(std::vector<CPlaceable*> *list)
{
   NumTreeTypes = 2;
   NumFlowerTypes = 2;
   numBlocks = 0;
   blocksY.clear();
   blocksX.clear();
   blocksFinal.clear();

    int x,y,w,d,h,t, moving;
    //float spd, rng;

    irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(levelName);
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
            moving = xml->getAttributeValueAsInt(6);
            if (t == -5 && !lowDef)
               GeneratePlants((float) x, (float) y, (float) w, (float) h, (float) d);

            list->push_back(ptr = new CBlock((float)x,(float)y,w,h,d,t,moving));
            if(xml->getAttributeValueAsInt(6)) {
               ptr->isMovingPlatform = 1;
               ptr->Range = (int) xml->getAttributeValueAsFloat(7); //Range
               ptr->Speed = (int) xml->getAttributeValueAsFloat(8); //Speed
            }
            else {
               ptr->isMovingPlatform = 0;
               numBlocks++;
               int curW = w;
               for(; curW > 0; curW--)
                  blocksY.push_back(new CBiggerBlock((float)x + (w - curW), (float)y, 1.0f, (float)h));
            }
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
            cen->setShader(Toon);
            cen->isMovingPlatform = 0;
         }
         if(!strcmp("CCabbage",xml->getNodeName())) {
            x = xml->getAttributeValueAsInt(0);
            y = xml->getAttributeValueAsInt(1);
            h = xml->getAttributeValueAsInt(2);
            w = xml->getAttributeValueAsInt(3);
            t = xml->getAttributeValueAsInt(4);
            Player->setArea(SRect2((float)x, (float)y, (float)h, (float)w));
         }
         if(!strcmp("CFlag", xml->getNodeName())) {
            x = xml->getAttributeValueAsInt(0);
            y = xml->getAttributeValueAsInt(1);
            h = xml->getAttributeValueAsInt(2);
            w = xml->getAttributeValueAsInt(3);
            t = xml->getAttributeValueAsInt(4);
            victoryBlock = Engine->addObject();
            victoryBlock->setArea(SRect2((float)x,(float)y, 1, 5));
            //PrepBlock(x,y+1,1,1);
            flagLogo->setTranslation(SVector3((float)x, (float) y+.9f, 1.0f));
            renderFlag->setTranslation(SVector3((float)x,(float) y+.5f, 1.0f));
            GameplayManager->setVictoryFlag(victoryBlock);
            if (t == 1){
                //add victory flag
            }
         }
         if(!strcmp("CPItem",xml->getNodeName())) {
            CPItem * stuff;
            x = xml->getAttributeValueAsInt(0);
            y = xml->getAttributeValueAsInt(1);
            t = xml->getAttributeValueAsInt(2);
            list->push_back(stuff = new CPItem((float)x,(float)y,t));
            stuff->isMovingPlatform = 0;
         }
         break;


      }
   }
}

void CGameState::consolidateAndAddBlocks() {
   printf("Size of blocksY: %d\n", blocksY.size());

   sort(blocksY.begin(), blocksY.end(), sortXY);

   CBiggerBlock *curBlock = blocksY[0];
   for(unsigned int i = 1; i < blocksY.size(); i++) {
      CBiggerBlock * newBlock = consolidateY(curBlock, blocksY[i]);
      /*
      printf("Consolidation of [%0.0f,%0.0f,%0.0f,%0.0f] and [%0.0f,%0.0f,%0.0f,%0.0f]? %s\n", 
            curBlock->x, curBlock->y, curBlock->w, curBlock->h,
            blocksY[i]->x, blocksY[i]->y, blocksY[i]->w, blocksY[i]->h,
            newBlock==NULL?"NO":"YES");
      */
      // There was nothing to consolidate, which means this CBiggerBlock is done (in the Y direction)
      if(newBlock == NULL) {
         blocksX.push_back(curBlock);
         curBlock = blocksY[i];
      }
      else {
         delete curBlock;
         delete blocksY[i];
         blocksY[i] = NULL;
         curBlock = newBlock;
      }
   }
   blocksX.push_back(curBlock);

   sort(blocksX.begin(), blocksX.end(), sortYX);
   printf("Size of blocksX: %d\n", blocksX.size());
   curBlock = blocksX[0];
   for(unsigned int i = 1; i < blocksX.size(); i++) {
      CBiggerBlock * newBlock = consolidateX(curBlock, blocksX[i]);
      /*
      printf("Consolidation of [%0.0f,%0.0f,%0.0f,%0.0f] and [%0.0f,%0.0f,%0.0f,%0.0f]? %s\n", 
            curBlock->x, curBlock->y, curBlock->w, curBlock->h,
            blocksX[i]->x, blocksX[i]->y, blocksX[i]->w, blocksX[i]->h,
            newBlock==NULL?"NO":"YES");
      */

      // There was nothing to consolidate, which means this CBiggerBlock is done (in the X direction)
      if(newBlock == NULL) {
         blocksFinal.push_back(curBlock);
         curBlock = blocksX[i];
      }
      else {
         delete curBlock;
         delete blocksX[i];
         blocksX[i] = NULL;
         curBlock = newBlock;
      }
   }
   blocksFinal.push_back(curBlock);

   printf("Size of blocksFinal: %d\n", blocksFinal.size());
   for(unsigned int i = 0; i < blocksFinal.size(); i++) {
      /*
      printf("Block %d: [%0.0f,%0.0f,%0.0f,%0.0f]\n", i,
            blocksFinal[i]->x, blocksFinal[i]->y, blocksFinal[i]->w, blocksFinal[i]->h);
      */
      blocksFinal[i]->addToEngine(Engine);
      delete blocksFinal[i];
      blocksFinal[i] = NULL;
   }
   printf("Total blocks saved: %d\n", numBlocks - blocksFinal.size());
   numBlocks = blocksFinal.size();
   blocksFinal.clear();
   blocksX.clear();
   blocksY.clear();
}

void CGameState::EngineInit( void ) {
   elevators.clear();
   if(Engine) {
      Engine->removeAll();
   }
   Engine = new CEngine();
   Player = Engine->addActor();
   Player->setArea(SRect2(-24.5f, 3, 1, 1));
   Player->getAttributes().MaxWalk = 3.5f;
   Player->CollideableType = COLLIDEABLE_TYPE_PLAYER;
   Player->CollideableLevel |= INTERACTOR_SUPERACTORS;
   Player->CanCollideWith |= INTERACTOR_SUPERACTORS | INTERACTOR_ITEMS;
   //Player->CanCollideWith |= INTERACTOR_NULL_BLOCK; // Block for procing the physics engine

   WinPlayer = Engine->addActor();
   WinPlayer->setArea(SRect2(-0.f, 10., 1, 1));
   WinPlayer->getAttributes().MaxWalk = 3.5f;
   WinPlayer->CollideableLevel = 0;
   WinPlayer->CanCollideWith = 0;
   WinPlayer->CollideableType = COLLIDEABLE_TYPE_PLAYER;
   //WinPlayer->CanCollideWith |= INTERACTOR_NULL_BLOCK; // Block for procing the physics engine

   Derp = Engine->addActor();
   Derp->setArea(SRect2(-20, 3, 1, 1));

   PrepSky();

   SRect2 area;

   GameplayManager = new CGameplayManager(Player, Engine);
   if(numLives != -3)
      GameplayManager->setLives(numLives);
   numLives = GameplayManager->getPlayerLives();

   GameEventManager = & GameplayManager->getGameEventManager();
   GameEventManager->OnEnemyDeath.connect(& GameEventReceiver, &CGameEventReceiver::OnEnemyDeath);
   GameEventManager->OnPlayerDamaged.connect(& GameEventReceiver, &CGameEventReceiver::OnPlayerDamaged);
   Application.getEventManager().OnGameTickStart.connect(& GameEventReceiver, & CGameEventReceiver::OnGameTickStart);

   std::vector<CPlaceable*> list;
   CObject *lastOne = NULL;
   loadWorld(&list);

   std::vector<CPlaceable*>::iterator it;
   for(it=list.begin();it<list.end();it++) {
      (*it)->setupItem(ToonTexture, DeferredToonTexture, Engine, GameplayManager);

      if((*it)->isMovingPlatform) {
         elevators.push_back(((CBlock*)(*it))->elevator);
         lastOne = ((CBlock*)(*it))->elevator;
      }
   }

   consolidateAndAddBlocks();
}

#define PARTICLE
#ifdef PARTICLE
CParticleEngine *particleLeafEngine;
CParticleEngine *particleCubeEngine;
CParticleEngine *particleLaserEngine;
CParticleEngine *particleLaserFireEngine;
CParticleEngine *particleDustEngine;
#endif

void CGameState::LoadHUD() {
	//Prepare GUI
   CabbageFace =  new CGUIImageWidget(CImageLoader::loadTGAImage("Base/cabbage.tga"), SVector2(.3f, .15f));
   CabbageFace->setPosition(SVector2(-.07f, .85f));

   CabbageHurtFace =  new CGUIImageWidget(CImageLoader::loadTGAImage("Base/cabbageouch.tga"), SVector2(.3f, .15f));
   CabbageHurtFace->setPosition(SVector2(-.081f, .861f));
   CabbageHurtFace->setVisible(false);

	Health5 = new CGUIImageWidget(CImageLoader::loadTGAImage("Base/HealthCabbage5.tga"), SVector2(.1f, .1f));
	Health5->setPosition(SVector2(.36f, .86f));

	Health4 = new CGUIImageWidget(CImageLoader::loadTGAImage("Base/HealthCabbage4.tga"), SVector2(.1f, .1f));
	Health4->setPosition(SVector2(.31f, .86f));

	Health3 = new CGUIImageWidget(CImageLoader::loadTGAImage("Base/HealthCabbage3.tga"), SVector2(.1f, .1f));
	Health3->setPosition(SVector2(.26f, .86f));

	Health2 = new CGUIImageWidget(CImageLoader::loadTGAImage("Base/HealthCabbage2.tga"), SVector2(.1f, .1f));
	Health2->setPosition(SVector2(.21f, .86f));

	Health1 = new CGUIImageWidget(CImageLoader::loadTGAImage("Base/HealthCabbage1.tga"), SVector2(.1f, .1f));
	Health1->setPosition(SVector2(.16f, .86f));

	CabbageEnergyBar = new CGUIImageWidget(CImageLoader::loadTGAImage("Base/EnergyBarTop.tga"), SVector2(.47f, .1f));
	CabbageEnergyBar->setPosition(SVector2(.02f, .78f));

	CabbageMeter = new CGUIImageWidget(CImageLoader::loadTGAImage("Base/EnergyBarBottom.tga"), SVector2(.47f, .1f));
	CabbageMeter->setPosition(SVector2(.02f, .78f));

	//Application.getGUIEngine().addWidget(CabbageFace);
	Application.getGUIEngine().addWidget(Health1);
	Application.getGUIEngine().addWidget(Health2);
	Application.getGUIEngine().addWidget(Health3);
	Application.getGUIEngine().addWidget(Health4);
	Application.getGUIEngine().addWidget(Health5);
	Application.getGUIEngine().addWidget(CabbageMeter);
	Application.getGUIEngine().addWidget(CabbageEnergyBar);
	Application.getGUIEngine().addWidget(CabbageFace);
	Application.getGUIEngine().addWidget(CabbageHurtFace);

}

void CGameState::Initialize() {
   Charged = 0; aDown = 0; dDown = 0; spaceDown = 0; wDown = 0; sDown = 0; lDown = 0;
   backwardsView = 0; overView = 0; energyStatus = 3.f; prevEnergy = 3.f;
   prevHealth = 0;
   StartWin = 0.0f;
   curScaleX = curScaleY = 1.f;
   launch = true;

   GameEventReceiver = CGameEventReceiver();
   oldFern = false;

   CApplication::get().getSceneManager().setCullingEnabled(true);

   f1 = f2 = f3 = glow = 0;
#ifdef PARTICLE
   particleLeafEngine = particleCubeEngine = particleLaserEngine = particleLaserFireEngine = particleDustEngine = 0;
#endif
   SPosition2 size = Application.getWindowSize();
   WindowWidth = size.X;
   WindowHeight = size.Y; 
   

   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);
   //glEnable(GL_CULL_FACE);
   //glCullFace(GL_BACK);

   SDL_WM_SetCaption("Happy Cabbage Adventure", NULL);

   //Initialize Font
   //our_font.init("WIFFLES_.TTF", 30);
   SColor FontColor(1.0f, 1.0f, 1.0f);
   GameWinText = new CGUIFontWidget("WIFFLES_.TTF", 30.f);
   GameWinText->setText("CONGRATULATIONS! YOU HAVE WON!");
   GameWinText->setVisible(false);
   GameWinText->setPosition(SVector2(0.3f, 0.75f));
   GameWinText->setColor(FontColor);

   GameOverText = new CGUIFontWidget("WIFFLES_.TTF", 30.f);
   GameOverText->setText("GAME OVER! YOU ARE DEAD. PRESS SPACE");
   GameOverText->setVisible(false);
   GameOverText->setPosition(SVector2(0.35f, 0.75f));
   GameOverText->setColor(FontColor);

   RestartGameText = new CGUIFontWidget("WIFFLES_.TTF", 30.f);
   RestartGameText->setText("YOU'VE DIED. PRESS SPACE TO CONTINUE!");
   RestartGameText->setVisible(false);
   RestartGameText->setPosition(SVector2(0.25f, 0.75f));
   RestartGameText->setColor(FontColor);

   LivesText = new CGUIFontWidget("WIFFLES_.TTF", 30.f);
   LivesText->setText("Lives: ");
   LivesText->setVisible(true);
   LivesText->setPosition(SVector2(0.14f, 0.87f));
   LivesText->setColor(SColor(0.0f, 0.80f, 0.0f));

   LivesText2 = new CGUIFontWidget("WIFFLES_.TTF", 31.f);
   LivesText2->setText("Lives: ");
   LivesText2->setVisible(true);
   LivesText2->setPosition(SVector2(0.1385f, 0.872f));
   LivesText2->setColor(SColor(0.0f, 0.0f, 0.0f));

   fpsText = new CGUIFontWidget("WIFFLES_.TTF", 15.f);
   fpsText->setText("FPS: ");
   fpsText->setVisible(true);
   fpsText->setPosition(SVector2((float)WindowWidth/(float)WindowHeight - 0.1f, 0.98f));
   fpsText->setColor(SColor(1.0f, 1.0f, 1.0f));

   fpsText2 = new CGUIFontWidget("WIFFLES_.TTF", 15.f);
   fpsText2->setText("FPS: ");
   fpsText2->setVisible(true);
   fpsText2->setPosition(SVector2((float)WindowWidth/(float)WindowHeight - 0.1f, 0.978f));
   fpsText2->setColor(SColor(0.0f, 0.0f, 0.0f));

   Application.getGUIEngine().addWidget(GameWinText);
   Application.getGUIEngine().addWidget(GameOverText);
   Application.getGUIEngine().addWidget(RestartGameText);
   Application.getGUIEngine().addWidget(LivesText2);
   Application.getGUIEngine().addWidget(LivesText);
   Application.getGUIEngine().addWidget(fpsText2);
   Application.getGUIEngine().addWidget(fpsText);

   Camera = new CPerspectiveCamera((float)WindowWidth/(float)WindowHeight, 0.01f, 100.f, 60.f);
   Application.getSceneManager().setActiveCamera(Camera);
   float const LightBrightness = 0.4f;

    CSceneManager & SceneManager = Application.getSceneManager();

    SceneManager.Lights.clear();

    SceneManager.Lights.push_back(new CLight());
    SceneManager.Lights.back()->Color = SVector3(LightBrightness);
    SceneManager.Lights.back()->Position = SVector3(-5.f, 200.f, 500.f);

   LoadShaders();

   Load3DS();
   LoadTextures();
   BlockMesh();

   DiscMesh();

   //Load the meshes into VBOs
   PrepMeshes();

   Application.getSceneManager().addSceneObject(renderDerp);
   Application.getSceneManager().addSceneObject(playerRenderable);
   Application.getSceneManager().addSceneObject(renderFlag);
   Application.getSceneManager().addSceneObject(flagLogo);
   Application.getSceneManager().addSceneObject(renderWinCabbage);

   srand((unsigned int) time(NULL));

   //Initialize Fxns
   EngineInit();
   ViewInit();
   GameplayManager->playerView = PlayerView;
   LoadHUD();
   fps = timeTotal = 0;
   numFrames = 0;

   //Start Level Music
   changeSoundtrack("SMW.wav");
   startSoundtrack();

   printf("CGameState:  Begin Function Complete\n");
}

//Initalizer fxn
void CGameState::begin()
{
	std::cout << "Start gamestate init..." << std::endl;
	GameplayManager = NULL;
	numLives = -3;
	Initialize();
	CApplication::get().getSceneManager().load();
	Application.skipElapsedTime();
	std::cout << "Finished gamestate init..." << std::endl;
}

SRect2 oldMiddle;

void CGameState::oldDisplay() {
   float curXVelocity = Player->getVelocity().X;
   PlayerView->setVelocity(Player->getVelocity());

   if (GameplayManager->isPlayerAlive() && !lDown)
   {
      if(!overView) {
         if(dDown && aDown) {
            Player->setAction(CActor::EActionType::None);
            PlayerView->setState(CPlayerView::State::Standing);
         }
         //I disabled backwards view for far view so we don't need backwards keypresses anymore
         else if((aDown)){// && !backwardsView) || (dDown && backwardsView)) {
            Player->setAction(CActor::EActionType::MoveLeft);
            PlayerView->setState(CPlayerView::State::MovingLeft);
         }
         else if((dDown)){// && !backwardsView) || (aDown && backwardsView)) {
            Player->setAction(CActor::EActionType::MoveRight);
            PlayerView->setState(CPlayerView::State::MovingRight);
         }

         else if (GameplayManager->isWon()) {

            Player->setAction(CActor::EActionType::None);

            PlayerView->setState(CPlayerView::State::Standing);

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

      if (!playChargeLaser) {
         Mix_HaltChannel(aChannel);
         playChargeLaser = true;
      }
   }
   else
   {
      if(!GameplayManager->isPlayerAlive() && spaceDown) {
         if(GameplayManager->getPlayerLives() > 0) {
            numLives = GameplayManager->getPlayerLives();
            end();
            Initialize();
            playDead = true;
            return;
         }
         else {
           COverworldState::get().levelCompleted = false;
           Application.getStateManager().setState(new CFadeOutState(& COverworldState::get()));
         }
      }
      if(!GameplayManager->isPlayerAlive() && !playDead) {
         if(GameplayManager->getPlayerLives() <= 0) {
            GameOverText->setVisible(true);
         }
         else {
            RestartGameText->setVisible(true);
         }
      }
      Player->setAction(CActor::EActionType::None);
      PlayerView->setState(CPlayerView::State::Standing);
      Player->setJumping(false);
   }
   PlayerView->setRecovering(GameplayManager->getRecovering());

   Derp->setJumping(true);

   char buf[30];
   numLives = GameplayManager->getPlayerLives();
   sprintf(buf, "%d", numLives);
   LivesText2->setText(buf);
   LivesText->setText(buf);

   Engine->updateAll(Application.getElapsedTime());
   GameplayManager->run(Application.getElapsedTime());

   PlayerView->step(Application.getElapsedTime());

   SVector2 middleOfPlayer = Player->getArea().getCenter();
   PlayerView->setMiddle(middleOfPlayer);
   SVector2 leftOfPlayer = SVector2(Player->getArea().Position.X, Player->getArea().getCenter().Y);
   SVector2 rightOfPlayer = SVector2(Player->getArea().Position.X + Player->getArea().Size.X, Player->getArea().getCenter().Y);
   PlayerView->setLeftGround(Engine->getHeightBelow(leftOfPlayer));
   PlayerView->setRightGround(Engine->getHeightBelow(rightOfPlayer));

   // Split the shadow into two
   CObject *l, *r;
   l = Engine->getObjectBelow(leftOfPlayer);
   r = Engine->getObjectBelow(rightOfPlayer);
   // If the cabbage is hanging over an edge, there might not be an object below it, so this check is necessary
   PlayerView->setCutoffPoint(l?l->getArea():SRect2(-50.0f, -50.0f, 0.0f, 0.0f),r?r->getArea():SRect2(-50.0f, -50.0f, 0.0f, 0.0f));

   PlayerView->establishCamera(Camera, ANGLE(overView, backwardsView), 
         (!!particleLaserFireEngine ? 1 : 0) + ((GameplayManager->getRecovering() > 0 || GameplayManager->JustKilled) ? 2 : 0));

#ifdef PARTICLE
   if(particleDustEngine && !particleDustEngine->dead) {
      particleDustEngine->setLookRight(PlayerView->getLookRight());
      particleDustEngine->setCenterPos(SVector3(Player->getArea().getCenter().X, Player->getArea().getCenter().Y, 0));
      particleDustEngine->step(Application.getElapsedTime());
   }
   if(particleLeafEngine && !particleLeafEngine->dead) {
      particleLeafEngine->setCenterPos(SVector3(Player->getArea().getCenter().X, Player->getArea().getCenter().Y, 0));
      particleLeafEngine->step(Application.getElapsedTime());
   }
   if(particleCubeEngine && !particleCubeEngine->dead) {
      particleCubeEngine->setLookRight(PlayerView->getLookRight());
      particleCubeEngine->setCenterPos(SVector3(Player->getArea().getCenter().X, Player->getArea().getCenter().Y, 0));
      particleCubeEngine->step(Application.getElapsedTime());
   }
   if(particleLaserEngine && !particleLaserEngine->dead) {
      particleLaserEngine->setLookRight(PlayerView->getLookRight());
      particleLaserEngine->setCenterPos(SVector3(Player->getArea().getCenter().X, Player->getArea().getCenter().Y, 0));
      if(GameplayManager->getRecovering() > 0) {  //If hit while using the laser, stop?
         lDown = 0;
         PlayerView->setShader(Toon, DeferredToon);
         particleLaserEngine->deconstruct();
         delete particleLaserEngine;
         particleLaserEngine = NULL;

         Mix_HaltChannel(aChannel);
      }
      else {  //Keep char
         particleLaserEngine->step(Application.getElapsedTime());

         /*else if (channelTime >= 1500) {
            aChannel = Mix_PlayChannel(-1, chargeLaser2, 0);
            Mix_ExpireChannel(aChannel, 1500);
            channelTime += Application.getElapsedTime();
         }*/
      }
   }
   if(particleLaserEngine && particleLaserEngine->dead && GameplayManager->getPlayerEnergy() > 0) {
      if (!playChargeLaser) {
         playChargeLaser = true;
         Mix_HaltChannel(aChannel);
      }
      particleLaserEngine->deconstruct();
      delete particleLaserEngine;
      particleLaserEngine = NULL;

      Charged = 1;
      GameplayManager->UseAbility(1);
      lDown = 0;
   }
   if(particleLaserFireEngine && !particleLaserFireEngine->dead) {  //Fire ze laser
      Player->setArea(oldMiddle);
      Player->setFallAcceleration(0.0f); //for the screen shaking effect
      particleLaserFireEngine->step(Application.getElapsedTime());
      lDown = 1;
      PlayerView->setShader(ToonBright, DeferredToonBright);
   }
   if(particleLaserFireEngine && particleLaserFireEngine->dead) {
      Player->setFallAcceleration(0.0f);
	  PlayerView->setShader(Toon, DeferredToon);
      lDown = 0;
      Charged = 0;
      GameplayManager->ShootingLaser = 0;
      particleLaserFireEngine->deconstruct();
      delete particleLaserFireEngine;
      particleLaserFireEngine = NULL;
      Player->setImpulse(SVector2((PlayerView->getLookRight()?-1:1)*15.0f, 0.0f), 0.1f);
      playFireLaser = true;
   }
   PlayerView->Charging = lDown;
#endif

   PlayerView->draw();

   //Draw derp (enemy)
   renderDerp->setTranslation(SVector3(Derp->getArea().getCenter().X, Derp->getArea().getCenter().Y + .2f, 0));
   renderDerp->setScale(SVector3(1.5f));
   renderDerp->setRotation(SVector3(-90, 0, -90));

   //ENEMY DISPLAY
   for (std::vector<CBadGuy*>::iterator it = GameplayManager->Enemies.begin(); it != GameplayManager->Enemies.end(); ++ it)
   {
      (*it)->doRenderable();
   }

   //ITEMS DISPLAY
   for (std::vector<CItem*>::iterator it = GameplayManager->Items.begin(); it != GameplayManager->Items.end(); ++ it)
   {
      (*it)->doRenderable();
   }

   std::vector<CElevator*>::iterator it;
   for(it=elevators.begin();it<elevators.end();it++) {
      CElevator * ptr = (*it);
      SVector2 pos = ptr->getArea().Position;
      SVector2 size = ptr->getArea().Size;
      ptr->getRenderable()->setTranslation(SVector3(pos.X + (float)size.X/2, pos.Y + (float)size.Y/2, 0));
   }

   glLoadIdentity();
}

//Runs at very start of display
void CGameState::OnRenderStart(float const Elapsed)
{
	glClearColor(0.4f,0.8f,1.f,0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   oldDisplay();

   Application.getSceneManager().drawAll();
   //printf("Objs culled: %d/%d (called %d times)\n", Application.getSceneManager().getNumCulled(), Application.getSceneManager().getNumObjects(), Application.getSceneManager().getTimesCalled());
   Application.getGUIEngine().drawAll();

   //Check if we have beat the level.  If we have, play the victory theme, perform the win animation
   if (! GameplayManager->isPlayerAlive() || GameplayManager->isWon()) {
      if(GameplayManager->isWon()) {
         if (playVictory) {
            Mix_HaltMusic();
            Mix_PlayChannel(-1, victory,0);
            playVictory = false;
         }

         RunVictorySequence(Elapsed);
         Engine->removeObject(victoryBlock);
		   GameWinText->setVisible(true);
      }

      //Check if we have died.  If we have, play the death theme, perform the death animation, and halt the game.
      else {
         //Chris Code.  Play Death Sound
         if (playDead) {
            Mix_HaltMusic();
            Mix_PlayChannel(-1, die, 0);
            playDead = false;
            spaceDown = 0;
         }
         Charged = 0;
      }
   }

   //Calculate FPS
   timeTotal += Application.getElapsedTime();
   numFrames++;
   if(timeTotal >= 0.1) {
      fps = numFrames / timeTotal;
      char buf[15];
      sprintf(buf, "FPS: %0.0f\n", fps);
      fpsText->setText(buf);
      fpsText2->setText(buf);
      timeTotal = 0;
      numFrames = 0;
   }

   //Update the HUD
   if(StartWin == 0.0f)
      UpdateLeaves();
   UpdateEnergy(Elapsed);

   //Draw Text
   /*freetype::print(our_font, 30, WindowHeight-40.f, "Elapsed Time: %0.0f\n"
         "Energy: %d\nnumKilled: %d\nFPS: %0.2f ", Application.getRunTime(), GameplayManager->getPlayerEnergy(), numKilled, fps);*/


   if(moveDown > 0.0f) {
      moveDown -= Application.getElapsedTime();
   }
   Application.getSceneManager().endDraw();
   SDL_GL_SwapBuffers();
}

//Sends event every time key pressed (also when held)
void CGameState::OnKeyboardEvent(SKeyboardEvent const & Event)
{
	if (Event.Key == SDLK_c && !GameplayManager->isWon())
		CApplication::get().getSceneManager().setCullingEnabled(! Event.Pressed);

	if (Event.Key == SDLK_x && Event.Pressed) {
      CApplication::get().getSceneManager().toggleUseHierarchy();
      printf("UseHierarchy: %d\n", CApplication::get().getSceneManager().getUseHierarchy());
   }

	if (Event.Key == SDLK_f && Event.Pressed) {
      if(fpsText->isVisible()) {
         fpsText->setVisible(false);
         fpsText2->setVisible(false);
      }
      else {
         fpsText->setVisible(true);
         fpsText2->setVisible(true);
      }
   }

	if (! Event.Pressed)
	{
		if (Event.Key == SDLK_n && !GameplayManager->isWon())
		{
			SceneManager.getEffectManager()->setEffectEnabled(ESE_SSAO, ! SceneManager.getEffectManager()->isEffectEnabled(ESE_SSAO));
			//CApplication::get().getSceneManager().DoSSAO = ! CApplication::get().getSceneManager().DoSSAO;
		}
		if (Event.Key == SDLK_v && !GameplayManager->isWon())
		{
			static bool Deferred = false;
			SceneManager.setDeferred(Deferred = ! Deferred);
		}
		if (Event.Key == SDLK_b && !GameplayManager->isWon())
		{
			SceneManager.getEffectManager()->setEffectEnabled(ESE_BLOOM, ! SceneManager.getEffectManager()->isEffectEnabled(ESE_BLOOM));
			//CApplication::get().getSceneManager().DoBloom = ! CApplication::get().getSceneManager().DoBloom;
		}
		if (Event.Key == SDLK_COMMA && !GameplayManager->isWon())
		{
			//CApplication::get().getSceneManager().OnlySSAO = ! CApplication::get().getSceneManager().OnlySSAO;
		}
		if (Event.Key == SDLK_SLASH && !GameplayManager->isWon())
		{
			SceneManager.getEffectManager()->setEffectEnabled(ESE_SSAO_BLUR, ! SceneManager.getEffectManager()->isEffectEnabled(ESE_SSAO_BLUR));
			//CApplication::get().getSceneManager().DoBlur = ! CApplication::get().getSceneManager().DoBlur;
		}
		if (Event.Key == SDLK_PERIOD && !GameplayManager->isWon())
		{
			//CApplication::get().getSceneManager().OnlyNormals = ! CApplication::get().getSceneManager().OnlyNormals;
		}
	}

   if(Event.Pressed){
      if(Event.Key == SDLK_w && !GameplayManager->isWon()){
         wDown = 1;
      }
      if(Event.Key == SDLK_s && !GameplayManager->isWon()){
         sDown = 1;
      }


      if((Event.Key == SDLK_a || Event.Key == SDLK_LEFT) && !GameplayManager->isWon()){
         if(moveDown > 0.0f) {
            if(particleDustEngine) {
               particleDustEngine->deconstruct();
               delete particleDustEngine;
               particleDustEngine = NULL;
            }
            particleDustEngine = new CParticleEngine(SVector3(0, 1, 0), 70, -1.0f, DUST_PARTICLE);
            particleDustEngine->UsePhysics(Engine);

            Player->getAttributes().MaxWalk = NoClipMode?34.5f:5.5f;
            moveDown = 0.0f;
         }
         else {
            moveDown = 0.3f;
         }
         aDown = 1;
      }
      if((Event.Key == SDLK_d || Event.Key == SDLK_RIGHT) && !GameplayManager->isWon()){
         if(moveDown > 0.0f) {
            if(particleDustEngine) {
               particleDustEngine->deconstruct();
               delete particleDustEngine;
               particleDustEngine = NULL;
            }
            particleDustEngine = new CParticleEngine(SVector3(0, 1, 0), 70, -1.0f, DUST_PARTICLE);
            particleDustEngine->UsePhysics(Engine);

            Player->getAttributes().MaxWalk = NoClipMode?34.5f:5.5f;
            moveDown = 0.0f;
         }
         else {
            moveDown = 0.3f;
         }
         dDown = 1;
      }
#ifdef PARTICLE
      if(Event.Key == SDLK_l && !GameplayManager->isWon()){
         //GameplayManager->setChargingLaser
         if(GameplayManager->getPlayerEnergy() > 0) {
            if(!particleLaserFireEngine && (!particleLaserEngine || (particleLaserEngine && particleLaserEngine->dead))) {
               particleLaserEngine = new CParticleEngine(SVector3(0, 1, 0), 400, 2.3f, LASER_CHARGING_PARTICLE);
               if (playChargeLaser) { //If making multiple sounds, do here
                  playChargeLaser = false;
                  aChannel = Mix_PlayChannel(-1, chargeLaser2, 0);
                  //Mix_ExpireChannel(aChannel, 1500); //Halt the channel in 1.5 seconds
                  channelTime += Application.getElapsedTime();
               }
            }
            PlayerView->setShader(ToonBright, DeferredToonBright);
            lDown = 1;
         }
      }
      if(Event.Key == SDLK_e && !GameplayManager->isWon()) {
         if(!particleCubeEngine || (particleCubeEngine && particleCubeEngine->dead))
            particleCubeEngine = new CParticleEngine(SVector3(0, 1, 0), 100, 10, CUBE_PARTICLE);
      }
      if(Event.Key == SDLK_r && !GameplayManager->isWon()) {
         if((!particleLeafEngine || (particleLeafEngine && particleLeafEngine->dead)) && GameplayManager->getPlayerEnergy() > 0) {
            if(!particleLeafEngine || (particleLeafEngine && particleLeafEngine->dead))
               particleLeafEngine = new CParticleEngine(SVector3(0, 1, 0), 150, 6, LEAF_PARTICLE);
            PlayerView->setGodMode(6.0f);
            //GameplayManager->setRecovering(6.0);
            GameplayManager->UseAbility(1);
               GameplayManager->setGodMode(6.0f);
         }
      }
#endif
      if(Event.Key == SDLK_k && !GameplayManager->isWon()) {
         backwardsView = !backwardsView;
      }
      if(Event.Key == SDLK_j && !GameplayManager->isWon()){
         overView = NEXT(overView);
         //printf("Angle: %d\n", ANGLE(overView, backwardsView));
      }
      if(Event.Key == SDLK_g && !GameplayManager->isWon()){
         GameplayManager->GodMode = !GameplayManager->GodMode;
         printf("Godmode: %d\n", GameplayManager->GodMode);
      }
      if(Event.Key == SDLK_m && !GameplayManager->isWon()){
         if(musicOn) {
            musicOn = false;
            Mix_HaltMusic();
         }
         else {
            musicOn = true;
            Mix_PlayMusic(music, -1);
         }
      }
      if(Event.Key == SDLK_SPACE && !GameplayManager->isWon()) {
         spaceDown = 1;
         if(NoClipMode) {
            Player->setVelocity(SVector2(Player->getVelocity().X, 10.0f));
         }
      }
      if(Event.Key == SDLK_h && !GameplayManager->isWon()) {
         NoClipMode = ~NoClipMode;
         if(NoClipMode) {
            Player->CollideableLevel = INTERACTOR_NULL_BLOCK;
            Player->CanCollideWith = INTERACTOR_NULL_BLOCK;
            Player->setControlFall(false);
            Player->setFallAcceleration(0.0f);
            Player->setJumping(false);
            Player->setVelocity(SVector2(Player->getVelocity().X, 0.0f));
            //Player->Gravity = 0.0f;
         }
         else {
            Player->CollideableLevel = INTERACTOR_SUPERACTORS;
            Player->CanCollideWith = INTERACTOR_SUPERACTORS | INTERACTOR_ITEMS | INTERACTOR_ACTORS | INTERACTOR_BLOCKS;
            Player->CanCollideWith &= ~INTERACTOR_SUPERNONCOLLIDERS;
            Player->setControlFall(true);
            //Player->Gravity = 100.0f;
         }
      }
      if(Event.Key == SDLK_ESCAPE) {
         //TODO: Replace with an event/signal to end the game world 
        COverworldState::get().levelCompleted = false;
         Application.getStateManager().setState(new CFadeOutState(& COverworldState::get()));
         //Application.getStateManager().setState(new CFadeOutState(& CMainMenuState::get()));
      }
   }
   //Check if key let go, Not sure if this will work in here.
   else  {
      if(Event.Key == SDLK_h && !GameplayManager->isWon()) {
      }
      if(Event.Key == SDLK_w && !GameplayManager->isWon()){
         wDown = 0;
      }
      if(Event.Key == SDLK_s && !GameplayManager->isWon()){
         sDown = 0;
      }
      if((Event.Key == SDLK_a || Event.Key == SDLK_LEFT) && !GameplayManager->isWon()){
         if(particleDustEngine) {
            particleDustEngine->deconstruct();
            delete particleDustEngine;
            particleDustEngine = NULL;
         }

         Player->getAttributes().MaxWalk = 3.5f;
         aDown = 0;
      }
      if((Event.Key == SDLK_d || Event.Key == SDLK_RIGHT) && !GameplayManager->isWon()){
         if(particleDustEngine) {
            particleDustEngine->deconstruct();
            delete particleDustEngine;
            particleDustEngine = NULL;
         }

         Player->getAttributes().MaxWalk = 3.5f;
         dDown = 0;
      }
      if(Event.Key == SDLK_l && !GameplayManager->isWon()){
         if(particleLaserEngine) {
            particleLaserEngine->deconstruct();
            delete particleLaserEngine;
            particleLaserEngine = NULL;
         }
         if(Charged) {
            if (playFireLaser) {
               playFireLaser = false;
               Mix_PlayChannel(-1, fireLaser, 0);
            }
            Charged = 0;
            oldMiddle = Player->getArea();
            Player->setVelocity(SVector2(0.0f));
            GameplayManager->ShootingLaser = 1;
            if(PlayerView->getLookRight()) {
               GameplayManager->LaserBox = SRect2(Player->getArea().getCenter() - SVector2(0.5f, 0.0f), Player->getArea().Size + SVector2(5.0f, 0.0f));
            }
            else {
               GameplayManager->LaserBox = SRect2(Player->getArea().getCenter() - SVector2(5.5f, 0.0f), Player->getArea().Size + SVector2(5.0f, 0.0f));
            }

            particleLaserFireEngine = new CParticleEngine(SVector3(0, 1, 0), 1500, 1.2f, LASER_FIRING_PARTICLE);
            particleLaserFireEngine->setCenterPos(SVector3(Player->getArea().getCenter().X, Player->getArea().getCenter().Y, 0));
            particleLaserFireEngine->setLookRight(PlayerView->getLookRight());
         }
         lDown = 0;
         PlayerView->setShader(Toon, DeferredToon);
      }
      if(Event.Key == SDLK_k){
      }
      if(Event.Key == SDLK_j){
      }
      if(Event.Key == SDLK_SPACE){
         spaceDown = 0;
         if(NoClipMode) {
            Player->setVelocity(SVector2(Player->getVelocity().X, 0.0f));
         }
      }
   }
}

void CGameState::end()
{
	
			SceneManager.setDeferred(false);
   Application.getSceneManager().Lights.clear();


   stopSoundtrack();

   if(particleLeafEngine) {
      particleLeafEngine->deconstruct();
      delete particleLeafEngine;
   }
   if(particleCubeEngine) {
      particleCubeEngine->deconstruct();
      delete particleCubeEngine;
   }
   if(particleLaserEngine) {
      particleLaserEngine->deconstruct();
      delete particleLaserEngine;
   }
   if(particleLaserFireEngine) {
      particleLaserFireEngine->deconstruct();
      delete particleLaserFireEngine;
   }
   if(particleDustEngine) {
      particleDustEngine->deconstruct();
      delete particleDustEngine;
   }
   particleLeafEngine = particleCubeEngine = particleLaserEngine = particleLaserFireEngine = particleDustEngine = NULL;

   GameEventManager->OnEnemyDeath.disconnect(& GameEventReceiver);
   GameEventManager->OnPlayerDamaged.disconnect(& GameEventReceiver);
   Application.getEventManager().OnGameTickStart.disconnect(& GameEventReceiver);

   Application.getSceneManager().Lights.clear();
   Application.getSceneManager().removeAllSceneObjects();
   Application.getGUIEngine().removeAllWidgets();

   playVictory = true;
}

void CGameState::UpdateLeaves() {
	int curHealth = GameplayManager->getPlayerHealth();

	if (curHealth == 5) {
		if (prevHealth == 4) {
			Health5->setVisible(true);
         cabbage5->setVisible(true);
         cabbage4->setVisible(false);
      }
	}
	else if (curHealth == 4) {
		if (prevHealth == 5) {
			Health5->setVisible(false);
         cabbage5->setVisible(false);
         cabbage4->setVisible(true);
      }
		else if (prevHealth == 3) {
			Health4->setVisible(true);
         cabbage4->setVisible(true);
         cabbage3->setVisible(false);
		}
	}

	else if (curHealth == 3) {
		if (prevHealth == 4) {
			Health4->setVisible(false);
         cabbage4->setVisible(false);
         cabbage3->setVisible(true);
      }
		else if (prevHealth == 2) {
			Health3->setVisible(true);
         cabbage3->setVisible(true);
         cabbage2->setVisible(false);
      }
	}

	else if (curHealth == 2) {
		if (prevHealth == 3) {
			Health3->setVisible(false);
         cabbage3->setVisible(false);
         cabbage2->setVisible(true);
      }
		else if (prevHealth == 1) {
			Health2->setVisible(true);
         cabbage2->setVisible(true);
         cabbage1->setVisible(false);
      }
	}

	else if (curHealth == 1) {
		if (prevHealth == 2) {
			Health2->setVisible(false);
         cabbage2->setVisible(false);
         cabbage1->setVisible(true);
      }
		else if (prevHealth == 0) {
			Health1->setVisible(true);
         cabbage1->setVisible(false);
         cabbage0->setVisible(true);
      }
	}

	else if (curHealth == 0) {
		if (prevHealth == 1) {
			Health1->setVisible(false);
         cabbage1->setVisible(false);
         cabbage0->setVisible(true);
      }
		else if (prevHealth == 2) {
			Health1->setVisible(false);
			Health2->setVisible(false);
         cabbage1->setVisible(false);
         cabbage0->setVisible(true);
		}
		else if (prevHealth == 3) {
			Health1->setVisible(false);
			Health2->setVisible(false);
			Health3->setVisible(false);
         cabbage3->setVisible(false);
         cabbage0->setVisible(true);
		}
		else if (prevHealth == 4) {
			Health1->setVisible(false);
			Health2->setVisible(false);
			Health3->setVisible(false);
			Health4->setVisible(false);
         cabbage4->setVisible(false);
         cabbage0->setVisible(true);
		}
		else if (prevHealth == 5 || prevHealth == 0) {
			Health1->setVisible(false);
			Health2->setVisible(false);
			Health3->setVisible(false);
			Health4->setVisible(false);
			Health5->setVisible(false);
         cabbage5->setVisible(false);
         cabbage0->setVisible(true);
		}
	}

   if (GameplayManager->getRecovering() > 0.f) {
      CabbageHurtFace->setVisible(true);
      CabbageFace->setVisible(false);

      if (curHealth == 5) {
         cabbageHurt5->setVisible(true);
         cabbage5->setVisible(false);
      }
      else if (curHealth == 4) {
         cabbageHurt4->setVisible(true);
         cabbage4->setVisible(false);
      }
      else if (curHealth == 3) {
         cabbageHurt3->setVisible(true);
         cabbage3->setVisible(false);
      }
      else if (curHealth == 2) {
         cabbageHurt2->setVisible(true);
         cabbage2->setVisible(false);
      }
      else if (curHealth == 1) {
         cabbageHurt1->setVisible(true);
         cabbage1->setVisible(false);
      }
      else if (curHealth == 0) {
         cabbageHurt0->setVisible(true);
         cabbage0->setVisible(false);
      }
   }

   else if ((cabbageHurt5->isVisible() || cabbageHurt4->isVisible() ||
	         cabbageHurt3->isVisible() || cabbageHurt2->isVisible() ||
	         cabbageHurt1->isVisible() || cabbageHurt0->isVisible()) && curHealth > 0) {

      CabbageHurtFace->setVisible(false);
      CabbageFace->setVisible(true);

      if (curHealth == 5)
         cabbage5->setVisible(true);
      else if (curHealth == 4)
         cabbage4->setVisible(true);
      else if (curHealth == 3)
         cabbage3->setVisible(true);
      else if (curHealth == 2)
         cabbage2->setVisible(true);
      else if (curHealth == 1)
         cabbage1->setVisible(true);
      else if (curHealth == 0)
         cabbage0->setVisible(true);

      cabbageHurt5->setVisible(false);
      cabbageHurt4->setVisible(false);
      cabbageHurt3->setVisible(false);
      cabbageHurt2->setVisible(false);
      cabbageHurt1->setVisible(false);
      cabbageHurt0->setVisible(false);
   }

	prevHealth = curHealth;
}

void CGameState::UpdateEnergy(float const Elapsed) {
	float curEnergy = (float)GameplayManager->getPlayerEnergy();

	//printf("curEnergy is %f, energyStatus is %f\n", curEnergy, energyStatus);

	//Check if we're at the bottom of the energy bar.  If we are, make it invisible
	//to counteract the bar glitch
	if (energyStatus < 0.1f) {
	   CabbageMeter->setVisible(false);
	}
	else
	   CabbageMeter->setVisible(true);

	if (energyStatus >= curEnergy - .01f && energyStatus <= curEnergy + .01f)
	   energyStatus = curEnergy;

	if (energyStatus > curEnergy) {
		//printf("Enter\n");
		energyStatus -= .7f*Elapsed;
		CabbageMeter->setSize(SVector2(.47f*energyStatus/3.f, .1f));

	}

	else if (energyStatus < curEnergy) {
		energyStatus +=.7f*Elapsed;
		CabbageMeter->setSize(SVector2(.47f*energyStatus/3.f, .1f));
	}
}

void CGameState::RunVictorySequence(float Elapsed) {
   cabbageHurt5->setVisible(false);
   cabbageHurt4->setVisible(false);
   cabbageHurt3->setVisible(false);
   cabbageHurt2->setVisible(false);
   cabbageHurt1->setVisible(false);
   cabbageHurt0->setVisible(false);

   cabbage5->setVisible(true);
   cabbage4->setVisible(false);
   cabbage3->setVisible(false);
   cabbage2->setVisible(false);
   cabbage1->setVisible(false);
   cabbage0->setVisible(false);

   StartWin += Elapsed;

//Fireworks
   if (f1) {
      f1->step(Elapsed);
      f2->step(Elapsed);
      f3->step(Elapsed);
   }

   if (glow)
      glow->step(Elapsed);

//perform cabbage sequence
   SVector3 curRotation = cabbage5->getRotation();
   SVector2 curLocation = SVector2 (Player->getArea().getCenter().X - .5f, Player->getArea().getCenter().Y - .5f);

   if (StartWin > .00f && StartWin < .07f) {
      WinPlayer->CollideableLevel |= INTERACTOR_SUPERACTORS;
      WinPlayer->CanCollideWith |= INTERACTOR_SUPERACTORS | INTERACTOR_ITEMS;

      dDown = 0;
      aDown = 0;
   }
   if (StartWin > .05f && StartWin < .07f) {
      spaceDown = 1;
      dDown = 1;
   }
   else if (StartWin > .07f && StartWin < .12f)
      spaceDown = 0;
   else if (StartWin > .72f && StartWin < .76f)
      spaceDown = 1;
   else if (StartWin > .76f && StartWin < 1.f) {
      if (!f1) {
         SVector3 flagPosition = renderFlag->getTranslation();

         f1 = new CParticleEngine(SVector3(flagPosition.X, flagPosition.Y, flagPosition.Z + .5f), 40, 2.f, HURT_PARTICLE);  //Would like to change these later so not leaves. Fine for now
         f2 = new CParticleEngine(SVector3(flagPosition.X + 4.f, flagPosition.Y, flagPosition.Z + .5f), 40, 2.f, HURT_PARTICLE);
         f3 = new CParticleEngine(SVector3(flagPosition.X + 8.f, flagPosition.Y, flagPosition.Z + .5f), 40, 2.f, HURT_PARTICLE);
      }
   }
   else if (StartWin > 1.f && StartWin < 1.2f) {
      spaceDown = 0;
   }
   else if (StartWin > 1.5f && StartWin < 2.5f) {
      spaceDown = 1;
      cabbage5->setRotation(SVector3(curRotation.X + 720.f*Elapsed, 0.f, 0.f));
   }
   else if (StartWin > 2.5f && StartWin < 2.9f) {
      spaceDown = 0;
      dDown = 0;
   }

   else if (StartWin > 2.9f && StartWin < 3.9f) {
      spaceDown = 1;
      cabbage5->setRotation(SVector3(curRotation.X - 360.f*Elapsed, 0.f, 0.f));
      Player->setArea(SRect2(curLocation.X - 3.f*Elapsed, curLocation.Y, 1.f, 1.f));
   }

   else if (StartWin > 3.8f && StartWin < 3.9f)
      spaceDown = 0;
   else if (StartWin > 3.9f && StartWin < 4.9f) {
      cabbage5->setRotation(SVector3(curRotation.X, 0.f, curRotation.Z - 765.f*Elapsed));
      Player->setArea(SRect2(curLocation.X - 3.f*Elapsed, curLocation.Y, 1.f, 1.f));
   }
   else if (StartWin > 4.9f && StartWin < 6.4f) {
      if (!glow) {
         glow = new CParticleEngine(SVector3(curLocation.X + 0.5f, curLocation.Y - 0.25f, 0), 400, 2.f, LASER_CHARGING_PARTICLE);
      }
      spaceDown = 0;
      curScaleY -= 0.4f*Elapsed;
      cabbage5->setScale(SVector3(1.f, 1.f, curScaleY));
      //renderWinCabbage->setScale(SVector3(1.f, 1.f, curScaleY));
   }

   else if (StartWin > 6.9f && StartWin < 7.3f) {
      curScaleY += 1.2f*Elapsed;
      curScaleX -= 1.8f*Elapsed;


      renderWinCabbage->setTranslation(SVector3(WinPlayer->getArea().getCenter().X,WinPlayer->getArea().getCenter().Y, 0));
      WinPlayer->setImpulse(SVector2(0.f, 30.f), 0.01f);

      if (launch) {
         Engine->removeActor(Player);
         PlayerView->removeFromScene();

         renderWinCabbage->setVisible(true);
         renderWinCabbage->setCullingEnabled(false);
         renderWinCabbage->setScale(SVector3(1.75f, 1.75f, 1.75f));
         renderWinCabbage->setRotation(SVector3(-90.f, 0.f, 45.f));
         WinPlayer->setArea(SRect2(curLocation.X, curLocation.Y, 1, 1));

         cabbage5->setVisible(false);
         cabbage5->setCullingEnabled(true);

         launch = false;
      }//Damien mark 1
   }
   else if(StartWin >= 7.3f)
   {
      COverworldState::get().levelCompleted = true;
      Application.getStateManager().setState(new CFadeOutState(& COverworldState::get()));
   }
}

void CGameState::PrepShadow() {
}

void CGameState::PrepBlock(float x, float y, float w, float h) {
   CMeshSceneObject *tempBlock;
   blocks.push_back(tempBlock = new CMeshSceneObject());
   tempBlock->setMesh(cubeMesh);
   tempBlock->setTexture(dirtTxt);
   tempBlock->setShader(ERP_DEFAULT, ToonTexture);
   tempBlock->setShader(ERP_DEFERRED_OBJECTS, DeferredToonTexture);
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
   tempBlock->setShader(ERP_DEFAULT, ToonTexture);
   tempBlock->setShader(ERP_DEFERRED_OBJECTS, DeferredToonTexture);
   tempBlock->setTranslation(SVector3((x+(x+w))/2, (y+(y+h))/2, 0));
   tempBlock->setScale(SVector3(0, 0, 0));
   Application.getSceneManager().addSceneObject(tempBlock);

}

void CGameState::PrepSky() {
   CMeshSceneObject *tempBlock;
   blocks.push_back(tempBlock = new CMeshSceneObject());
   tempBlock->setMesh(cubeMesh);
   tempBlock->setTexture(skyTxt);
   tempBlock->setShader(ERP_DEFAULT, DiffuseTexture);
   tempBlock->setShader(ERP_DEFERRED_OBJECTS, DeferredTexture);
   tempBlock->setTranslation(SVector3(85/*75*/, 13, -5.0));
   tempBlock->setScale(SVector3(250, -50, 1));
   //tempBlock->setCullingEnabled(false);
   Application.getSceneManager().addSceneObject(tempBlock);

}

CMeshSceneObject* CGameState::PrepEnemy(float x, float y) {
   CMeshSceneObject *tempEnemy;
   enemies.push_back(tempEnemy = new CMeshSceneObject());
   tempEnemy->setMesh(enemyMesh);
   //tempEnemy->setTexture(dirtTxt);
   tempEnemy->setShader(ERP_DEFERRED_OBJECTS, DeferredFlat);
   tempEnemy->setShader(ERP_DEFAULT, Flat);
   tempEnemy->setRotation(SVector3(-90, 0, 0));
   Application.getSceneManager().addSceneObject(tempEnemy);
   return tempEnemy;
}

void CGameState::GeneratePlants(float x, float y, float w, float h, float d) {
   int numForeground, numBackground;
   int random;
   float randScale, randDepth;
   float div;

   if (w > 0.5f && w < 1.5f)  //If block size roughly 1, don't draw any trees
      numForeground = numBackground = 0;
   else {
      numForeground = (int) w / 2;
      numBackground = (int) w / 2;
   }

   div =  w/(float)numBackground;

   //Check how many tree-type objects we should draw in the background
   for (int n = 0; n < numBackground; n++) {
      random = rand()%2;

      if (random == 0)
         drawBasicTree(x + (n)*div + div/2.0f, -0.2f, -d/2.0f + .4f, 8.0f, Application);
      else if (random == 1)
         drawChristmasTree(x + (n)*div + div/2.0f, 1.4f, -d/2.0f + .4f, 6.0f, Application);
   }

   //Draw flower-type plants in background
   for (int n = 0; n < w; n++) {
      random = rand()%6;
      float subrand = (float) (rand()%5);
      randScale = (float) (rand()%20);
      randScale = randScale * .025f;
      randDepth = (float) (rand()%2);
      randDepth = (float) randDepth*.25f;

      if (random == 0 || random == 1)
         drawWhiteFlwr(x + n + .5f, -1.f, -d/2.0f + 1.6f + randDepth, .7f, Application);
      else if (random == 2 || random == 3 || random == 4) {
    	  if (subrand == 0)
    		  drawYellowFlwr(x + n + .5f, -1.0f, -d/2.0f + 1.5f + randDepth, .7f + randScale, Application);
    	  else if (subrand == 1)
    		  drawBlueFlwr(x + n + .5f, -1.0f, -d/2.0f + 1.5f + randDepth, .7f + randScale, Application);
    	  else if (subrand == 2)
    		  drawWhiteSunflwr(x + n + .5f, -1.0f, -d/2.0f + 1.5f + randDepth, .7f + randScale, Application);
    	  else if (subrand == 3)
    		  drawTealFlwr(x + n + .5f, -1.0f, -d/2.0f + 1.5f + randDepth, .7f + randScale, Application);
    	  else if (subrand == 4)
    		  drawPurpleFlwr(x + n + .5f, -1.0f, -d/2.0f + 1.5f + randDepth, .7f + randScale, Application);
      }
      else if (random == 5 && !oldFern) {
         drawFern(x + n + .5f, .2f, -d/2.0f + 1.5f + randDepth, 1.f + randScale, Application);
         oldFern = true;
      }
      else if (random == 5 && oldFern)
     	 n--;

      if (random != 5)
    	  oldFern = false;
   }

   //Draw flower-type plants in foreground
      for (int n = 0; n < w; n++) {
         random = rand()%6;
         float subrand = (float) (rand()%5);
         randScale = (float) (rand()%10);
         randScale = randScale * 0.025f;
         randDepth = (float) (rand()%2);
         randDepth = randDepth*0.25f;

         if (random == 0 || random == 1)
            drawWhiteFlwr(x + n + 0.5f, -1.0f, d/2.0f - 0.6f, 0.4f + randScale, Application);
         else if (random == 2 || random == 3 || random == 4) {
        	 if (subrand == 0)
        		 drawBlueFlwr(x + n + 0.5f, -1.0f, d/2.0f - 0.6f, 0.4f + randScale, Application);
        	 else if (subrand == 1)
        		 drawTealFlwr(x + n + 0.5f, -1.0f, d/2.0f - 0.6f, 0.4f + randScale, Application);
        	 else if (subrand == 2)
        		 drawPurpleFlwr(x + n + 0.5f, -1.0f, d/2.0f - 0.6f, 0.4f + randScale, Application);
        	 else if (subrand == 3)
        		 drawYellowFlwr(x + n + 0.5f, -1.0f, d/2.0f - 0.6f, 0.4f + randScale, Application);
        	 else if (subrand == 3)
        		 drawWhiteSunflwr(x + n + 0.5f, -1.0f, d/2.0f - 0.6f, 0.4f + randScale, Application);
         }
         else if (random == 5 && !oldFern) {
            drawFern(x + n + 0.5f, 0.2f, d/2.0f - 0.6f, 0.7f, Application);
            oldFern = true;
         }
         else if (random == 5 && oldFern)
        	 n--;

         if (random != 5)
       	  oldFern = false;
      }
}


void LoadShaders() {
   DeferredFlat = CShaderLoader::loadShader("Deferred/Diffuse");
   DeferredDiffuse = CShaderLoader::loadShader("Deferred/Diffuse");
   DeferredToonTexture = CShaderLoader::loadShader("Deferred/Textured");
   DeferredTexture = CShaderLoader::loadShader("Deferred/Textured");
   DeferredToon = CShaderLoader::loadShader("Deferred/Diffuse");
   DeferredToonBright = CShaderLoader::loadShader("Deferred/Diffuse");

   Flat = CShaderLoader::loadShader("Diffuse");
   Diffuse = CShaderLoader::loadShader("Diffuse");
   ToonTexture = CShaderLoader::loadShader("ToonTexture");
   DiffuseTexture = CShaderLoader::loadShader("DiffuseTextureBright");
   normalColor = CShaderLoader::loadShader("Simple");
   Toon = CShaderLoader::loadShader("Toon");
   ToonBright = CShaderLoader::loadShader("ToonBright");
   BlackShader = CShaderLoader::loadShader("SpecialDiscThing");
   //Toon = Diffuse;
}


void Load3DS()
{
   mCab5 = CMeshLoader::load3dsMesh("Base/cabbage/cabbage_5.3ds");
   if (mCab5) {
      mCab5->centerMeshByExtents(SVector3(0));
      mCab5->calculateNormalsPerFace();
      mCab5->resizeMesh(SVector3(0.45f));
   }

   mCab4 = CMeshLoader::load3dsMesh("Base/cabbage/cabbage_4.3ds");
   if (mCab4) {
      mCab4->centerMeshByExtents(SVector3(0));
      mCab4->calculateNormalsPerFace();
      mCab4->resizeMesh(SVector3(0.45f));
   }

   mCab3 = CMeshLoader::load3dsMesh("Base/cabbage/cabbage_3.3ds");
   if (mCab3) {
      mCab3->centerMeshByExtents(SVector3(0));
      mCab3->calculateNormalsPerFace();
      mCab3->resizeMesh(SVector3(0.45f));
   }

   mCab2 = CMeshLoader::load3dsMesh("Base/cabbage/cabbage_2.3ds");
   if (mCab2) {
      mCab2->centerMeshByExtents(SVector3(0));
      mCab2->calculateNormalsPerFace();
      mCab2->resizeMesh(SVector3(0.45f));
   }

   mCab1 = CMeshLoader::load3dsMesh("Base/cabbage/cabbage_1.3ds");
   if (mCab1) {
      mCab1->centerMeshByExtents(SVector3(0));
      mCab1->calculateNormalsPerFace();
      mCab1->resizeMesh(SVector3(0.45f));
   }

   mCab0 = CMeshLoader::load3dsMesh("Base/cabbage/cabbage_0.3ds");
   if (mCab0) {
      mCab0->centerMeshByExtents(SVector3(0));
      mCab0->calculateNormalsPerFace();
      mCab0->resizeMesh(SVector3(0.45f));
   }

   mCabOw5 = CMeshLoader::load3dsMesh("Base/cabbage/cabbage_ouch_5.3ds");
   if (mCabOw5) {
      mCabOw5->centerMeshByExtents(SVector3(0));
      mCabOw5->calculateNormalsPerFace();
      mCabOw5->resizeMesh(SVector3(0.45f));
   }

   mCabOw4 = CMeshLoader::load3dsMesh("Base/cabbage/cabbage_ouch_4.3ds");
   if (mCabOw4) {
      mCabOw4->centerMeshByExtents(SVector3(0));
      mCabOw4->calculateNormalsPerFace();
      mCabOw4->resizeMesh(SVector3(0.85f));
   }

   mCabOw3 = CMeshLoader::load3dsMesh("Base/cabbage/cabbage_ouch_3.3ds");
   if (mCabOw3) {
      mCabOw3->centerMeshByExtents(SVector3(0));
      mCabOw3->calculateNormalsPerFace();
      mCabOw3->resizeMesh(SVector3(0.85f));
   }

   mCabOw2 = CMeshLoader::load3dsMesh("Base/cabbage/cabbage_ouch_2.3ds");
   if (mCabOw2) {
      mCabOw2->centerMeshByExtents(SVector3(0));
      mCabOw2->calculateNormalsPerFace();
      mCabOw2->resizeMesh(SVector3(0.85f));
   }

   mCabOw1 = CMeshLoader::load3dsMesh("Base/cabbage/cabbage_ouch_1.3ds");
   if (mCabOw1) {
      mCabOw1->centerMeshByExtents(SVector3(0));
      mCabOw1->calculateNormalsPerFace();
      mCabOw1->resizeMesh(SVector3(0.85f));
   }

   mCabOw0 = CMeshLoader::load3dsMesh("Base/cabbage/cabbage_ouch_0.3ds");
   if (mCabOw0) {
      mCabOw0->centerMeshByExtents(SVector3(0));
      mCabOw0->calculateNormalsPerFace();
      mCabOw0->resizeMesh(SVector3(0.45f));
   }

   enemyMesh = CMeshLoader::load3dsMesh("Base/appleEnemy.3ds");
   if(enemyMesh) {
      enemyMesh->resizeMesh(SVector3(1));
      enemyMesh->centerMeshByExtents(SVector3(0));
      enemyMesh->calculateNormalsPerFace();
   }

   else {
      fprintf(stderr, "Failed to load the enemy mesh\n");
   }

   derpMesh = CMeshLoader::load3dsMesh("Base/derp.3ds");

   if(derpMesh) {
      derpMesh->resizeMesh(SVector3(1.0));
      derpMesh->centerMeshByExtents(SVector3(0));
      derpMesh->calculateNormalsPerVertex();
   }

   else {
      fprintf(stderr, "Failed to load derp mesh\n");
   }

   basicTreeMesh = CMeshLoader::load3dsMesh("Base/tree4.3ds");
   if (basicTreeMesh) {
      basicTreeMesh->resizeMesh(SVector3(0.5));
      basicTreeMesh->centerMeshByExtents(SVector3(0));
      basicTreeMesh->calculateNormalsPerVertex();
   }
   else {
      fprintf(stderr, "Failed to load the basic tree mesh\n");
   }

   christmasTreeMesh = CMeshLoader::load3dsMesh("Base/christmasTree3.3ds");
   if (christmasTreeMesh) {
      christmasTreeMesh->resizeMesh(SVector3(0.5));
      christmasTreeMesh->centerMeshByExtents(SVector3(0));
      christmasTreeMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load the christmas tree mesh\n");
   }

   cabbageMesh = CMeshLoader::load3dsMesh("Base/crappycabbage2.3ds");
   if (cabbageMesh) {
      cabbageMesh->resizeMesh(SVector3(0.5));
      cabbageMesh->centerMeshByExtents(SVector3(0));
      cabbageMesh->calculateNormalsPerVertex();
   }
   else {
      fprintf(stderr, "Failed to load the cabbage mesh\n");
   }

   cabbageDamage = CMeshLoader::load3dsMesh("Base/cabbageouch2.3ds");
   if (cabbageDamage) {
      cabbageDamage->resizeMesh(SVector3(0.5));
      cabbageDamage->centerMeshByExtents(SVector3(0));
      cabbageDamage->calculateNormalsPerVertex();
   }

   blueFlwrMesh = CMeshLoader::load3dsMesh("Base/simpleflower1.3ds");
   if (blueFlwrMesh) {
      blueFlwrMesh->centerMeshByExtents(SVector3(0));
      blueFlwrMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load blue flower mesh.\n");
   }

   whiteFlwrMesh = CMeshLoader::load3dsMesh("Base/simpleflower2.3ds");
   if (whiteFlwrMesh) {
      whiteFlwrMesh->centerMeshByExtents(SVector3(0));
      whiteFlwrMesh->resizeMesh(SVector3(.8f));
      whiteFlwrMesh->calculateNormalsPerFace();
   }

   whiteSunflwrMesh = CMeshLoader::load3dsMesh("Base/sunflowerwhite.3ds");
   if (whiteSunflwrMesh) {
	   whiteSunflwrMesh->centerMeshByExtents(SVector3(0));
	   whiteSunflwrMesh->resizeMesh(SVector3(.8f));
	   whiteSunflwrMesh->calculateNormalsPerFace();
   }

   yellowFlwrMesh = CMeshLoader::load3dsMesh("Base/sunfloweryellow.3ds");
   if (yellowFlwrMesh) {
	   yellowFlwrMesh->centerMeshByExtents(SVector3(0));
	   yellowFlwrMesh->resizeMesh(SVector3(.8f));
	   yellowFlwrMesh->calculateNormalsPerFace();
   }

   purpleFlwrMesh = CMeshLoader::load3dsMesh("Base/sunflowerpurple.3ds");
   if (purpleFlwrMesh) {
	   purpleFlwrMesh->centerMeshByExtents(SVector3(0));
	   purpleFlwrMesh->resizeMesh(SVector3(.8f));
	   purpleFlwrMesh->calculateNormalsPerFace();
   }

   tealFlwrMesh = CMeshLoader::load3dsMesh("Base/sunflowerteal.3ds");
   if (tealFlwrMesh) {
	   tealFlwrMesh->centerMeshByExtents(SVector3(0));
	   tealFlwrMesh->resizeMesh(SVector3(.8f));
   	   tealFlwrMesh->calculateNormalsPerFace();
   }

   else {
      fprintf(stderr, "Failed to load pink flower mesh.\n");
   }

   ficusMesh = CMeshLoader::load3dsMesh("Base/ficus.3ds");
   if (ficusMesh) {
      ficusMesh->centerMeshByExtents(SVector3(0));
      ficusMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load ficus mesh.\n");
   }

   fernMesh = CMeshLoader::load3dsMesh("Base/fern.3ds");
   if (fernMesh) {
      fernMesh->centerMeshByExtents(SVector3(0));
      fernMesh->resizeMesh(SVector3(2.f));
      fernMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load fern mesh.\n");
   }

   flagMesh = CMeshLoader::load3dsMesh("Base/flag2.3ds");
   if (flagMesh) {
      flagMesh->centerMeshByExtents(SVector3(0));
      flagMesh->calculateNormalsPerFace();
   }

   else {
      fprintf(stderr, "Failed to load flag mesh.\n");
   }
}

void LoadTextures()
{
   grassImg = CImageLoader::loadImage("Base/grass.bmp");
   skyImg = CImageLoader::loadImage("Base/sky.bmp");
   dirtImg = CImageLoader::loadImage("Base/dirt.bmp");
   blueFlwrImg = CImageLoader::loadImage("Base/blueFlower.bmp");
   whiteFlwrImg = CImageLoader::loadImage("Base/pinkFlower.bmp");
   flagImg = CImageLoader::loadImage("Base/white.bmp");

   grassTxt = new CTexture(grassImg);
   skyTxt = new CTexture(skyImg);
   dirtTxt = new CTexture(dirtImg);
   blueFlwrTxt = new CTexture(blueFlwrImg);
   whiteFlwrTxt = new CTexture(whiteFlwrImg);
   flagTxt = new CTexture(flagImg);
}

void PrepMeshes()
{
   renderDerp = new CMeshSceneObject();
   renderDerp->setMesh(derpMesh);
   renderDerp->setShader(ERP_DEFAULT, Toon);
   renderDerp->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);

  renderWinCabbage = new CMeshSceneObject();
  renderWinCabbage->setMesh(cabbageMesh);
  renderWinCabbage->setShader(ERP_DEFAULT, Toon);
  renderWinCabbage->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
  renderWinCabbage->setVisible(false);
  renderWinCabbage->setCullingEnabled(true);


   cabbage5 = new CMeshSceneObject();
   cabbage5->setMesh(mCab5);
  cabbage5->setShader(ERP_DEFAULT, Toon);
  cabbage5->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   cabbage5->setCullingEnabled(false);

   playerLight2 = new CPointLightSceneObject(10.f);
   CApplication::get().getSceneManager().addSceneObject(playerLight2);

   for (int x = -5; x < 5; ++ x)
	   for (int y = -5; y < 5; ++ y)
	   {
		   CPointLightSceneObject * point = new CPointLightSceneObject();
		   point->setRadius(1.5f);
		   point->setTranslation(SVector3(x * 3.f, y * 3.f, 0.f));
		   CApplication::get().getSceneManager().addSceneObject(point);
	   }

   cabbage4 = new CMeshSceneObject();
   cabbage4->setMesh(mCab4);
  cabbage4->setShader(ERP_DEFAULT, Toon);
  cabbage4->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   cabbage4->setCullingEnabled(false);
   cabbage4->setVisible(false);

   cabbage3 = new CMeshSceneObject();
   cabbage3->setMesh(mCab3);
  cabbage3->setShader(ERP_DEFAULT, Toon);
  cabbage3->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   cabbage3->setCullingEnabled(false);
   cabbage3->setVisible(false);

   cabbage2 = new CMeshSceneObject();
   cabbage2->setMesh(mCab2);
  cabbage2->setShader(ERP_DEFAULT, Toon);
  cabbage2->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   cabbage2->setCullingEnabled(false);
   cabbage2->setVisible(false);

   cabbage1 = new CMeshSceneObject();
   cabbage1->setMesh(mCab1);
  cabbage1->setShader(ERP_DEFAULT, Toon);
  cabbage1->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   cabbage1->setCullingEnabled(false);
   cabbage1->setVisible(false);

   cabbage0 = new CMeshSceneObject();
   cabbage0->setMesh(mCab0);
  cabbage0->setShader(ERP_DEFAULT, Toon);
  cabbage0->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   cabbage0->setCullingEnabled(false);
   cabbage0->setVisible(false);

   cabbageHurt5 = new CMeshSceneObject();
   cabbageHurt5->setMesh(mCabOw5);
  cabbageHurt5->setShader(ERP_DEFAULT, Toon);
  cabbageHurt5->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   cabbageHurt5->setCullingEnabled(false);
   cabbageHurt5->setTranslation(SVector3(0.f, 0.f, .2f));

   cabbageHurt4 = new CMeshSceneObject();
   cabbageHurt4->setMesh(mCabOw4);
  cabbageHurt4->setShader(ERP_DEFAULT, Toon);
  cabbageHurt4->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   cabbageHurt4->setCullingEnabled(false);
   cabbageHurt4->setVisible(false);
   cabbageHurt4->setTranslation(SVector3(0.f, 0.f, .2f));

   cabbageHurt3 = new CMeshSceneObject();
   cabbageHurt3->setMesh(mCabOw3);
  cabbageHurt3->setShader(ERP_DEFAULT, Toon);
  cabbageHurt3->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   cabbageHurt3->setCullingEnabled(false);
   cabbageHurt3->setVisible(false);
   cabbageHurt3->setTranslation(SVector3(0.f, 0.f, .2f));

   cabbageHurt2 = new CMeshSceneObject();
   cabbageHurt2->setMesh(mCabOw2);
  cabbageHurt2->setShader(ERP_DEFAULT, Toon);
  cabbageHurt2->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   cabbageHurt2->setCullingEnabled(false);
   cabbageHurt2->setVisible(false);
   cabbageHurt2->setTranslation(SVector3(0.f, 0.f, .2f));

   cabbageHurt1 = new CMeshSceneObject();
   cabbageHurt1->setMesh(mCabOw1);
  cabbageHurt1->setShader(ERP_DEFAULT, Toon);
  cabbageHurt1->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   cabbageHurt1->setCullingEnabled(false);
   cabbageHurt1->setVisible(false);
   cabbageHurt1->setTranslation(SVector3(0.f, 0.f, .2f));

   cabbageHurt0 = new CMeshSceneObject();
   cabbageHurt0->setMesh(mCabOw0);
  cabbageHurt0->setShader(ERP_DEFAULT, Toon);
  cabbageHurt0->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   cabbageHurt0->setCullingEnabled(false);
   cabbageHurt0->setVisible(false);
   cabbageHurt0->setTranslation(SVector3(0.f, 0.f, .2f));

   normalCabbage = new CMeshSceneObject();
   normalCabbage->setMesh(mCab5);
  normalCabbage->setShader(ERP_DEFAULT, Toon);
  normalCabbage->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   normalCabbage->setCullingEnabled(false);
   normalCabbage->setVisible(false);
   

   damageCabbage = new CMeshSceneObject();
   damageCabbage->setMesh(cabbageDamage);
  damageCabbage->setShader(ERP_DEFAULT, Toon);
  damageCabbage->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   damageCabbage->setVisible(false);
   damageCabbage->setScale(SVector3(1.5f));
   damageCabbage->setRotation(SVector3(0.f, 0.f, 45.f));
   damageCabbage->setTranslation(SVector3(0.f, 0.f, .15f));
   damageCabbage->setCullingEnabled(false);



   playerRenderable = new CMeshSceneObject();
   playerRenderable->addChild(normalCabbage);
   playerRenderable->addChild(damageCabbage);
   playerRenderable->setVisible(false);
   playerRenderable->addChild(cabbage5);
   playerRenderable->addChild(cabbage4);
   playerRenderable->addChild(cabbage3);
   playerRenderable->addChild(cabbage2);
   playerRenderable->addChild(cabbage1);
   playerRenderable->addChild(cabbage0);
   playerRenderable->addChild(cabbageHurt5);
   playerRenderable->addChild(cabbageHurt4);
   playerRenderable->addChild(cabbageHurt3);
   playerRenderable->addChild(cabbageHurt2);
   playerRenderable->addChild(cabbageHurt1);
   playerRenderable->addChild(cabbageHurt0);

   renderFlag = new CMeshSceneObject();
   renderFlag->setMesh(flagMesh);
   renderFlag->setTranslation(SVector3(170, 100.f, 1.0f));
   renderFlag->setRotation(SVector3(-90,0,0));
   renderFlag->setScale(SVector3(.0150f, .00025f,.0016f));
   renderFlag->setTexture(flagTxt);
   renderFlag->setShader(ERP_DEFAULT, ToonTexture);
   renderFlag->setShader(ERP_DEFERRED_OBJECTS, DeferredToonTexture);

   flagLogo = new CMeshSceneObject();
   flagLogo->setMesh(cabbageMesh);
   flagLogo->setTranslation(SVector3(170, 100.f, 1.0f));
   flagLogo->setRotation(SVector3(-90,0,0));
   flagLogo->setScale(SVector3(.75f));
  flagLogo->setShader(ERP_DEFAULT, Toon);
  flagLogo->setShader(ERP_DEFERRED_OBJECTS, DeferredToon);
   flagLogo->setShader(ERP_DEFERRED_OBJECTS, DeferredFlat);
}
