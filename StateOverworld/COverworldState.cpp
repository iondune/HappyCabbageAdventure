#include "COverworldState.h"
#include "../CabbageSound/sound.h"
#include "CGameState.h"
#include "CMainMenuState.h"


COverworldState::COverworldState()
: Application (CApplication::get())
{}

//Initalizer fxn
void COverworldState::begin()
{
   isDay = 1;
   aDown = 0; dDown = 0; spaceDown = 0; wDown = 0; sDown = 0, yDown = 0, sineValue = 0;
   transitionTimer = 0.0;
   transitionWorld = 0;

   camRotValue = stepValue = 0;
   rot = SVector3f(-90, 0, -90);
   SPosition2 size = Application.getWindowSize();
   WindowWidth = size.X;
   WindowHeight = size.Y; 
   glClearColor(0.4f,0.8f,1,0);

   mouseDown = 0;

   if(newGame)
   {
      printf("Starting new game\n");
      newGame = false;
      curNode = 0;
      curCamera = 0;
      levelsUnlocked = true; //NOTE: this should be switched to false upon release
      loadLevels();
      setCameraTrans();
   }
   else if(levelCompleted)
   {
      printf("Completed level %d\n", curNode);
      levels[curNode].completed = true;
      Stats.Health = Stats.MaxHealth;
      Stats.Energy = Stats.MaxEnergy;
   }
   else if(!levelCompleted)
   {
      printf("Level failed\n");
      if(NewStats.Lives <= 0) {
         printf("Game over\n");
      }
      else {
         Stats.Lives = NewStats.Lives;
         Stats.Seeds = NewStats.Seeds;
      }
   }

   //testFun();

   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);

   SDL_WM_SetCaption("Happy Cabbage Adventure", NULL);

   changex = 0.94f;
   changey = -0.25f;
   changez = -0.38f;

   //eye = SVector3f(1.47f, 0.33f, 0);
   //look = SVector3f(0.57f, -0.08f, 0.19f);
   eye = cameraPos[curCamera];
   look = levels[curNode].loc;
   Camera = new CPerspectiveCamera((float)WindowWidth/(float)WindowHeight, 0.01f, 100.f, 60.f);
   Application.getSceneManager().setActiveCamera(Camera);
   float const LightBrightness = 2.1f;
   Camera->setPosition(eye);
   Camera->setLookDirection(look - eye);

   CSceneManager & SceneManager = Application.getSceneManager();

   SceneManager.Lights.push_back(new CLight());
   SceneManager.Lights.back()->Color = SVector3f(LightBrightness);
   SceneManager.Lights.back()->Position = SVector3f(30.f, 2.f, 3.f);

   SceneManager.Lights.push_back(new CLight());
   SceneManager.Lights.back()->Color = SVector3f(LightBrightness);
   SceneManager.Lights.back()->Position = SVector3f(-1.f, -2.f, 30.f);

   SceneManager.Lights.push_back(new CLight());
   SceneManager.Lights.back()->Color = SVector3f(LightBrightness);
   SceneManager.Lights.back()->Position = SVector3f(-30.f, 0.f, 0.f);

   SceneManager.Lights.push_back(new CLight());
   SceneManager.Lights.back()->Color = SVector3f(LightBrightness);
   PlayerLight = SceneManager.Lights.back();


   LoadShaders();

   //Load the meshes into VBOs
   PrepMeshes();
   PrepSkySphere();


   //Initialize Fxns
   fps = timeTotal = 0;
   numFrames = 0;

}

void COverworldState::PrepSkySphere() {
   printf("Loading sky sphere mesh\n");
   CMesh *splitSphereMesh = CMeshLoader::load3dsMesh("Base/SphereSplit.3ds");
   if(splitSphereMesh) {
      splitSphereMesh->resizeMesh(SVector3f(10.0f));
      splitSphereMesh->centerMeshByExtents(SVector3f(0));
      splitSphereMesh->linearizeIndices();
      splitSphereMesh->calculateNormalsPerVertex();
   }
   CMeshSceneObject *temp = new CMeshSceneObject();

   temp->setMesh(splitSphereMesh);
   temp->setScale(SVector3f(9.0f));
   temp->setTranslation(SVector3f(0.0f, -24.0f, 0.0f));
   /*
   temp->setTexture(CImageLoader::loadTexture("Colors/LightBlue.bmp", 1), 0);
   temp->setTexture(CImageLoader::loadTexture("Colors/DarkBlue.bmp", 1), 1);
   */
   temp->setTexture(CImageLoader::loadTexture("Base/DayOverworld.bmp", 1), 0);
   temp->setTexture(CImageLoader::loadTexture("Base/NightOverworld.bmp", 1), 1);
   temp->setShader(ERP_DEFAULT, CShaderLoader::loadShader("DiffuseTexture"));
   temp->setShader(ERP_DEFERRED_OBJECTS, CShaderLoader::loadShader("Deferred/Textured"));
   temp->setCullingEnabled(false);


   Application.getSceneManager().addSceneObject(SkySphere = temp);
   printf("Done\n");
}

void COverworldState::step(float delta) {
   //rot.X += 10*delta;

   superInterpolator(playerVector, playerVecShift, delta, TRANSITION_PERIOD);
   superInterpolator(look, lookShift, delta, TRANSITION_PERIOD);
   superInterpolator(eye, eyeShift, delta, TRANSITION_PERIOD);

   if(transitionTimer == 0.0f)
   {
      //camRotValue += 0.5f*delta;
   }
   else if(transitionTimer < 0.0f )
   { 
      transitionTimer =  0.0f;
      playerRender->setRotation(SVector3f(-90, 0, angleMaker(levels[curNode].loc, cameraPos[curCamera])));
      transitionWorld = 0;

      /*
         if(curNode != NUM_LEVELS -1)
         {
         arrowRender2->setTranslation(SVector3f(0,0.2,0) + playerVector);
         arrowRender2->setRotation(SVector3f(90.0f, 0.0f, angleMaker(playerVector, levels[curNode + 1].loc)));
         }
         */
   }
   else
   {
      transitionTimer -= delta;
   }

   if(transitionTimer && transitionWorld) {
      //Needs to travel 180 degrees in TRANSITION_PERIOD time

      sineValue += 180.0f/TRANSITION_PERIOD*delta;
   }
   //SkySphere->setScale(SVector3f(0.0f + sineValue/20.0f));
   //printf("Total: %0.0f\n", 0.0f + sineValue/20.0f);
   SkySphere->setRotation(SVector3f(90, 180 + sineValue, 0));


   stepValue += 0.5f*delta;
}

//Runs at very start of display
void COverworldState::OnRenderStart(float const Elapsed)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   step(Application.getElapsedTime());
   //stepCamera(Application.getElapsedTime());
   SVector3f eyeRot = eye;
   eyeRot.X += 0.2f*cos(camRotValue);
   eyeRot.Z += 0.2f*(sin(camRotValue) + 1);
   eyeRot.Y += 0.1f*(sin(camRotValue) + 1);
   bouncePlayer();
   Camera->setPosition(eyeRot);
   Camera->setLookDirection(look - eyeRot);

   renderMap->setRotation(rot);

   Application.getSceneManager().drawAll();

   //Calculate FPS
   timeTotal += Application.getElapsedTime();
   numFrames++;
   if(timeTotal >= 0.1f) {
      fps = numFrames / timeTotal;
      timeTotal = 0;
      numFrames = 0;
   }


   Application.getSceneManager().endDraw();
   SDL_GL_SwapBuffers();
}


//Sends event every time key pressed (also when held)
void COverworldState::OnKeyboardEvent(SKeyboardEvent const & Event)
{
   bool diskChanger = false;
   bool eyeChanger = false;
   bool arrowChanger = false;

   if(Event.Pressed ){
      if(Event.Key == SDLK_w){
         wDown = 1;
      }
      if(Event.Key == SDLK_s){
         sDown = 1;
      }
      if(Event.Key == SDLK_y) {
         yDown = 1;
      }
      if(Event.Key == SDLK_h) {
         Stats.toggleGodmode();
         printf("Godmode %s.\n", (Stats.isGodmodeEnabled()?"enabled; all abilities now available":"disabled; all normal abilities returned"));
      }
      if(Event.Key == SDLK_a || Event.Key == SDLK_LEFT){
         aDown = 1;
         movePlayer();
      }
      if(Event.Key == SDLK_d || Event.Key == SDLK_RIGHT){
         dDown = 1;
         movePlayer();
      }
      if(Event.Key == SDLK_m){
         printf("Eye coords: %0.2f %0.2f %0.2f\n", eye.X, eye.Y, eye.Z);
         printf("Look coords: %0.2f %0.2f %0.2f\n", look.X, look.Y, look.Z);
      }
      if(Event.Key == SDLK_SPACE && transitionTimer == 0.0f) {
         CGameState::get().LevelName = levels[curNode].name;
         CGameState::get().Stats = Stats; 
         Stats.Health = Stats.MaxHealth;
         Stats.Energy = Stats.MaxEnergy;
         spaceDown = 1;

         Application.getStateManager().setState(new CFadeOutState(& CGameState::get()));
      }
      if(Event.Key == SDLK_ESCAPE) {
         Application.getStateManager().setState(new CFadeOutState(& CMainMenuState::get()));
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
      if(Event.Key == SDLK_a || Event.Key == SDLK_LEFT){
         aDown = 0;
      }
      if(Event.Key == SDLK_d || Event.Key == SDLK_RIGHT){
         dDown = 0;
      }
      if(Event.Key == SDLK_y) {
         yDown = 0;
      }
      if(Event.Key == SDLK_v){
         levelsUnlocked = !levelsUnlocked;
      }
      if(Event.Key == SDLK_i){
         changey += 0.01f;
         diskChanger = true;
      }
      if(Event.Key == SDLK_k){
         changey -= 0.01f;
         diskChanger = true;
      }
      if(Event.Key == SDLK_u){
         changex += 0.01f;
         diskChanger = true;
      }
      if(Event.Key == SDLK_j){
         changex -= 0.01f;
         diskChanger = true;
      }
      if(Event.Key == SDLK_o){
         changez += 0.01f;
         diskChanger = true;
      }
      if(Event.Key == SDLK_l){
         changez -= 0.01f;
         diskChanger = true;
      }
      if(Event.Key == SDLK_p){
         printf("disk coords: %0.2f %0.2f %0.2f\n", changex,changey, changez);
      }
      if(Event.Key == SDLK_n){
         look = levels[0].loc;
         eye = SVector3f(0.74f, 0.48f, 1.50f);
      }
      if(Event.Key == SDLK_SPACE){
         spaceDown = 0;
      }
   }

   if(diskChanger)
      discRender->setTranslation(SVector3f(changex,changey, changez));

   if(eyeChanger)
      eye = SVector3f(changex, changey, changez);

   if(arrowChanger)
      arrowRender2->setRotation(SVector3f(90.0f, 0.0f, changez ));

}

void COverworldState::end()
{
   CApplication::get().getSceneManager().setDeferred(false);
   CApplication::get().getSceneManager().removeAllSceneObjects();
   CApplication::get().getSceneManager().Lights.clear();
   CGameLevelLoader::resetLevel();
}

void COverworldState::loadLevels()
{
   //If you're adding more levels be sure to update COverworldState's NUM_LEVELS

	//Grass Levels
	levels[0].name = "jorge1.xml";//"jorge1.xml";
	levels[0].loc = SVector3f(0.33f, -0.17f, 1.05f); //bit of green near sole orange hill
	levels[1].name = "jorge2.xml";
	levels[1].loc = SVector3f(0.56, -0.29, 1.11);

	//Single water level
	levels[2].name = "water1.xml";
	levels[2].loc = SVector3f(0.84, -0.33, 1.19);

	//Moar Grass Levels!
	levels[3].name = "jorge3.xml";
	levels[3].loc = SVector3f(0.68, -0.21, 1.01);
	levels[4].name = "jorge5.xml";
	levels[4].loc = SVector3f(0.47, -0.16, 0.80);
	levels[5].name = "jorge6-night.xml";
	levels[5].loc = SVector3f(0.30, -0.12, 0.60);
	levels[6].name = "jorge7.xml";
	levels[6].loc = SVector3f(0.83f, 0.00f, 0.65f); //Green hill
	levels[7].name = "jorge8.xml";
	levels[7].loc = SVector3f(0.9f, -0.12999999f, 0.3f);
	levels[8].name = "chris-night.xml";
	levels[8].loc = SVector3f(0.5f, -0.13f, 0.1f);
	levels[9].name = "jorge9.xml";
	levels[9].loc = SVector3f(0.86, -0.16, -0.02);

	//Desert Levels
   levels[10].name = "chrisDesert.xml";
   levels[10].loc = SVector3f(0.94f, -0.25f, -0.38f); //Yellow beach
   levels[11].name = "chrisDesert1-night.xml";
   levels[11].loc = SVector3f(0.77f, -0.20f, -0.66f); //Yellow hills
   levels[12].name = "desertexhibition.xml";
   levels[12].loc = SVector3f(0.65, -0.20, -0.51);

   //Water Levels
   //levels[13].name = "water1.xml";
   //levels[13].loc = SVector3f(1.27f, -.21f, -1.0f);

   for(int i = 0; i < NUM_LEVELS; i++)
   {
      levels[i].completed = false;
   }
}

void COverworldState::setCameraTrans()
{
   cameraPos[0] = SVector3f(0.21f, 0.36f, 1.76f);
   cameraPos[1] = SVector3f(1.47f, 0.33f, 0.0f);
   cameraPos[2] = SVector3f(1.11f, 0.45f, -2.16f);

}

void COverworldState::LoadShaders() {
   Flat = CShaderLoader::loadShader("Toon");
   Diffuse = CShaderLoader::loadShader("Toon");
   ToonTexture = CShaderLoader::loadShader("ToonTexture");

   DeferredFlat = CShaderLoader::loadShader("Deferred/Diffuse");
   DeferredDiffuse = CShaderLoader::loadShader("Deferred/Diffuse");
   DeferredTexture = CShaderLoader::loadShader("Deferred/Textured");
   //normalColor = CShaderLoader::loadShader("NormalColor");
}

void COverworldState::PrepMeshes()
{
   //Set up world map renderable
   CMesh *mapMesh = CMeshLoader::load3dsMesh("Base/world.3ds");
   if(mapMesh) {
      mapMesh->centerMeshByExtents(SVector3f(0));
      mapMesh->calculateNormalsPerFace();
   }

   CMaterial mat;

   mat.Texture = CImageLoader::loadTexture("../Models/Base/world.bmp");
   renderMap = CApplication::get().getSceneManager().addMeshSceneObject(mapMesh, ToonTexture, DeferredTexture);
   renderMap->setMaterial(mat);
   renderMap->setCullingEnabled(false);
   //renderMap->setVisible(false);

   //Set up player renderable
   CMesh *playerMesh = CMeshLoader::load3dsMesh("Base/cabbage/cabbage_5.3ds");
   // CMesh *playerMesh = CMeshLoader::load3dsMesh("Base/crappycabbage2.3ds");

   if (playerMesh) {
      playerMesh->resizeMesh(SVector3f(0.45f));
      playerMesh->centerMeshByExtents(SVector3f(0));
      playerMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load the cababge mesh\n");
   }

   playerRender = CApplication::get().getSceneManager().addMeshSceneObject(playerMesh, Flat, DeferredFlat);
   playerVector = levels[curNode].loc;
   playerVector.Y += 0.05f;
   playerRender->setTranslation(playerVector);
   playerRender->setRotation(SVector3f(-90.0f, 0.0f, 45.0f));
   playerRender->setScale(SVector3f(0.18f));

   (playerRender);

   CMesh *arrowMesh = CMeshLoader::load3dsMesh("Base/arrow.3ds");
   if (arrowMesh) {
      arrowMesh->resizeMesh(SVector3f(0.5f));
      arrowMesh->centerMeshByExtents(SVector3f(0));
      arrowMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load the cababge mesh\n");
   }
   arrowRender1 = CApplication::get().getSceneManager().addMeshSceneObject(arrowMesh, Flat, DeferredFlat);
   arrowRender1->setTranslation(SVector3f(0, -0.2f, 0.0) + playerVector);
   arrowRender1->setRotation(SVector3f(-90.0f, 0.0f, 45.0f));
   arrowRender1->setScale(SVector3f(0.18f));

   arrowRender2 = CApplication::get().getSceneManager().addMeshSceneObject(arrowMesh, Flat, DeferredFlat);
   arrowRender2->setTranslation(SVector3f(0,-0.2f,0) + playerVector);
   arrowRender2->setRotation(SVector3f(90.0f, 0.0f, angleMaker(playerVector, levels[curNode + 1].loc) + 90.0f ));
   arrowRender2->setScale(SVector3f(0.18f));



   CMesh *discMesh = CMeshLoader::createDiscMesh();
   discMesh->linearizeIndices();
   discMesh->calculateNormalsPerFace();

   CMesh *flagMesh = CMeshLoader::load3dsMesh("Base/flag2.3ds");

   if (flagMesh) {
      flagMesh->centerMeshByExtents(SVector3f(0));
      flagMesh->calculateNormalsPerFace();
   }

   else {
      fprintf(stderr, "Failed to load flag mesh.\n");
   }

   CTexture *flagTxt = new CTexture(CImageLoader::loadImage("Base/white.bmp"));


   for(int i = 0; i < NUM_LEVELS; i++)
   {
      if(levels[i].completed)
      {
         levelIcons(levels[i].loc, discMesh, 1);
         addMeshes(levels[i].loc, flagMesh, flagTxt);
      }
      else
      {
         levelIcons(levels[i].loc, discMesh, 2);
      }

   }

}

void COverworldState::levelIcons(SVector3f loc, CMesh *levelIcon, int iconColor)
{

   discRender = CApplication::get().getSceneManager().addMeshSceneObject(levelIcon, ToonTexture, DeferredTexture);

   CMaterial mat;
   switch(iconColor)
   {
   case 1:
      mat.Texture = CImageLoader::loadTexture("Base/GrassyGrass.bmp");
      break;

   case 2:
      mat.Texture = CImageLoader::loadTexture("Base/dirt.bmp");
      break;

   default:
      mat.Texture = CImageLoader::loadTexture("../Models/disc_red.bmp");
      break;
   }
   discRender->setMaterial(mat);

   discRender->setTranslation(loc);
   discRender->setScale(SVector3f(0.1f));
}

void COverworldState::addMeshes(SVector3f loc, CMesh *newMesh, CTexture *texture)
{
   CMeshSceneObject *renderFlag = new CMeshSceneObject();
   renderFlag->setMesh(newMesh);
   renderFlag->setTranslation(loc + SVector3f(-0.05f, 0.05f, 0.05f));
   renderFlag->setRotation(SVector3f(-90,0,0));
   renderFlag->setScale(SVector3f(.00150f, .000025f,.00016f));
   renderFlag->setTexture(texture);
   renderFlag->setShader(ERP_DEFAULT, ToonTexture);
   renderFlag->setShader(ERP_DEFERRED_OBJECTS, DeferredTexture);

   Application.getSceneManager().addSceneObject(renderFlag);



}

void COverworldState::bouncePlayer() {
   SVector3f m_base = playerVector;
   m_base.Y += 0.03f*(sin(stepValue*15)+1);
   playerRender->setTranslation(m_base);
}


void COverworldState::movePlayer() {
   bool moved = false;
   if(transitionTimer != 0.0f)
      return;

   if(aDown && curNode > 0 )
   {
      curNode--;
      if(strstr(levels[curNode].name, "night") == NULL) {
         //Day
         if(!isDay) {
            transitionWorld = 1;
            isDay = 1;
         }
      }
      else {
         //Night
         if(isDay) {
            transitionWorld = 1;
            isDay = 0;
         }
      }
      moved = true;
   }
   else if(dDown && curNode < NUM_LEVELS - 1 && (levels[curNode].completed || levelsUnlocked))
   {
      curNode++;
      if(strstr(levels[curNode].name, "night") == NULL) {
         //Day
         if(!isDay) {
            transitionWorld = 1;
            isDay = 1;
         }
      }
      else {
         //Night
         if(isDay) {
            transitionWorld = 1;
            isDay = 0;
         }
      }
      moved = true;
   }

   if(moved)
   {
      transitionTimer = TRANSITION_PERIOD;

      playerVecTarget = levels[curNode].loc;
      playerVecTarget.Y += 0.05f;
      shiftSetter(playerVector, playerVecShift, playerVecTarget);

      lookTarget = levels[curNode].loc;
      shiftSetter(look, lookShift, lookTarget);

      if(curNode == 1 && aDown)
      {
         eyeTarget = cameraPos[0];
         curCamera = 0;
         shiftSetter(eye, eyeShift, eyeTarget);
      }
      else if(curNode == 2 && dDown)
      {
         eyeTarget = cameraPos[1];
         curCamera = 1;
         shiftSetter(eye, eyeShift, eyeTarget);
      }
      else if(curNode == 5 && aDown)
      {
         eyeTarget = cameraPos[1];
         curCamera = 1;
         shiftSetter(eye, eyeShift, eyeTarget);
      }
      else if(curNode == 6 && dDown)
      {
         eyeTarget = cameraPos[2];
         curCamera = 2;
         shiftSetter(eye, eyeShift, eyeTarget);
      }
   }
}

void COverworldState::superInterpolator(SVector3f & curr, SVector3f & change,
      float delta, float transLength)
{
   SVector3f zeroV = SVector3f(0.0f, 0.0f, 0.0f);

   if(transitionTimer <= 0.0f)
   {
      change = zeroV;
      return;
   }
   else if(change == zeroV)
   {
      return;
   }
   else
   {
      curr = curr + (change * delta/transLength);
   }
}

void COverworldState::shiftSetter(SVector3f & curr, SVector3f & change, SVector3f & target)
{
   change = target - curr;
}

float COverworldState::angleMaker(SVector3f start, SVector3f toPoint)
{
   //float asinxz, asinzx, acosxz, acoszx;
   float lookat = 0.f;
   SVector3f temp = toPoint - start;

   temp.normalize();
   /*

      lookat = atan2(temp.X,temp.Z) * RAD_TO_DEG;
      asinxz = asinf(temp.X/temp.Z) * RAD_TO_DEG;
      asinzx = asinf(temp.Z/temp.X) * RAD_TO_DEG;
      acosxz = acosf(temp.X/temp.Z) * RAD_TO_DEG;
      acoszx = acosf(temp.Z/temp.X) * RAD_TO_DEG;

      printf("asin %f %f\n acos %f %f\natan2xz %f\n\n",
      asinxz, asinzx, acosxz, acoszx,lookat);
      */
   lookat = lookat < 0 ? lookat + 55 : lookat;
   return lookat;
}
void COverworldState::testFun()
{
   SVector3f temp = cameraPos[1] -levels[2].loc;
   temp.normalize();

   printf("TEST %f\n", (atan2(temp.X,temp.Z) * RAD_TO_DEG));

}
