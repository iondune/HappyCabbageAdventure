#include "COverworldState.h"
#include "../CabbageSound/sound.h"

COverworldState::COverworldState()
: Application (CApplication::get())
{}

//Initalizer fxn
void COverworldState::begin()
{
   aDown = 0; dDown = 0; spaceDown = 0; wDown = 0; sDown = 0;
   transitionTimer = 0.0;

   camRotValue = stepValue = 0;
   rot = SVector3(-90, 0, -90);
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

   }
   else if(!levelCompleted)
   {
     printf("Level failed\n");

   }

   //testFun();

   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);

   SDL_WM_SetCaption("Happy Cabbage Adventure", NULL);

   changex = 0.87f;
   changey = 0.58f;
   changez = 1.27f;

   //eye = SVector3(1.47f, 0.33f, 0);
   //look = SVector3(0.57f, -0.08f, 0.19f);
   eye = cameraPos[curCamera];
   look = levels[curNode].loc;
   Camera = new CPerspectiveCamera((float)WindowWidth/(float)WindowHeight, 0.01f, 100.f, 60.f);
   Application.getSceneManager().setActiveCamera(Camera);
   float const LightBrightness = 0.0f;
   Camera->setPosition(eye);
   Camera->setLookDirection(look - eye);

    CSceneManager & SceneManager = Application.getSceneManager();
    SceneManager.Lights.push_back(new CLight());
    SceneManager.Lights.back()->Color = SVector3(LightBrightness);
    SceneManager.Lights.back()->Position = SVector3(30.f, 2.f, 3.f);

    SceneManager.Lights.push_back(new CLight());
    SceneManager.Lights.back()->Color = SVector3(LightBrightness);
    SceneManager.Lights.back()->Position = SVector3(-1.f, -2.f, 30.f);

    SceneManager.Lights.push_back(new CLight());
    SceneManager.Lights.back()->Color = SVector3(LightBrightness);
    SceneManager.Lights.back()->Position = SVector3(-30.f, 0.f, 0.f);

    SceneManager.Lights.push_back(new CLight());
    SceneManager.Lights.back()->Color = SVector3(LightBrightness);
    PlayerLight = SceneManager.Lights.back();


   LoadShaders();

   //Load the meshes into VBOs
   PrepMeshes();


   //Initialize Fxns
   fps = timeTotal = 0;
   numFrames = 0;

}

void COverworldState::step(float delta) {
   //rot.X += 10*delta;

  superInterpolator(playerVector, playerVecShift, delta, TRANSITION_PERIOD);
   superInterpolator(look, lookShift, delta, TRANSITION_PERIOD);
   superInterpolator(eye, eyeShift, delta, TRANSITION_PERIOD);

   if(transitionTimer == 0.0f)
   {
     camRotValue += 0.5f*delta;
   }
   else if(transitionTimer < 0.0f )
   { 
     transitionTimer =  0.0f;
     playerRender->setRotation(SVector3(-90, 0, angleMaker(levels[curNode].loc, cameraPos[curCamera])));

     /*
     if(curNode != NUM_LEVELS -1)
     {
arrowRender2->setTranslation(SVector3(0,0.2,0) + playerVector);
   arrowRender2->setRotation(SVector3(90.0f, 0.0f, angleMaker(playerVector, levels[curNode + 1].loc)));
     }
     */
   }
   else
   {
     transitionTimer -= delta;
   }


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
   SVector3 eyeRot = eye;
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
      if(Event.Key == SDLK_a){
         aDown = 1;
         movePlayer();
      }
      if(Event.Key == SDLK_d){
         dDown = 1;
         movePlayer();
      }
      if(Event.Key == SDLK_m){
         printf("Eye coords: %0.2f %0.2f %0.2f\n", eye.X, eye.Y, eye.Z);
         printf("Look coords: %0.2f %0.2f %0.2f\n", look.X, look.Y, look.Z);
      }
      if(Event.Key == SDLK_SPACE && transitionTimer == 0.0f) {
        CGameState::get().levelName = levels[curNode].name;
         Application.getStateManager().setState(new CFadeOutState(& CGameState::get()));
         spaceDown = 1;
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
      if(Event.Key == SDLK_a){
         aDown = 0;
      }
      if(Event.Key == SDLK_d){
         dDown = 0;
      }
      if(Event.Key == SDLK_v){
         levelsUnlocked = !levelsUnlocked;
      }
      if(Event.Key == SDLK_i){
        changey += 0.01f;
        arrowChanger = true;
      }
      if(Event.Key == SDLK_k){
        changey -= 0.01f;
        arrowChanger = true;
      }
      if(Event.Key == SDLK_u){
        changex += 0.01f;
        arrowChanger = true;
      }
      if(Event.Key == SDLK_j){
        changex -= 0.01f;
        arrowChanger = true;
      }
      if(Event.Key == SDLK_o){
        changez += 5.00f;
        arrowChanger = true;
      }
      if(Event.Key == SDLK_l){
        changez -= 5.00f;
        arrowChanger = true;
      }
      if(Event.Key == SDLK_p){
         printf("disk coords: %0.2f %0.2f %0.2f\n", changex,changey, changez);
      }
      if(Event.Key == SDLK_n){
        look = levels[0].loc;
        eye = SVector3(0.74f, 0.48f, 1.50f);
      }
      if(Event.Key == SDLK_SPACE){
         spaceDown = 0;
      }
   }

   if(diskChanger)
        discRender->setTranslation(SVector3(changex,changey, changez));

   if(eyeChanger)
     eye = SVector3(changex, changey, changez);

   if(arrowChanger)
     arrowRender2->setRotation(SVector3(90.0f, 0.0f, changez ));

}

void COverworldState::end()
{
   Application.getSceneManager().removeAllSceneObjects();
   Application.getSceneManager().Lights.clear();
}

void COverworldState::loadLevels()
{
  //If you're adding more levels be sure to update COverworldState's NUM_LEVELS
  levels[0].name = "jorge1.xml";
  levels[0].loc = SVector3(0.33f, -0.17f, 1.05f); //bit of green near sole orange hill
  levels[1].name = "jorge2.xml";
  levels[1].loc = SVector3(0.83f, 0.00f, 0.65f); //Green hill
  levels[2].name = "jorge3.xml";
  levels[2].loc = SVector3(0.9f, -0.12999999f, 0.3f);
  levels[3].name = "chris.xml";
  levels[3].loc = SVector3(0.5f, -0.13f, 0.1f);
  levels[4].name = "test2.xml";
  levels[4].loc = SVector3(0.98f, -0.27f, -0.19f); //Green beach
  levels[5].name = "test.xml";
  levels[5].loc = SVector3(0.94f, -0.25f, -0.38f); //Yellow beach

  for(int i = 0; i < NUM_LEVELS; i++)
  {
    levels[i].completed = false;
  }
}

void COverworldState::setCameraTrans()
{
  cameraPos[0] = SVector3(0.21f, 0.36f, 1.76f);
  cameraPos[1] = SVector3(1.47f, 0.33f, 0.0f);

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
      mapMesh->centerMeshByExtents(SVector3(0));
      mapMesh->calculateNormalsPerFace();
   }
   
   CMaterial mat;

   mat.Texture = CImageLoader::loadTexture("../Models/Base/world.bmp");
   renderMap = CApplication::get().getSceneManager().addMeshSceneObject(mapMesh, ToonTexture, DeferredTexture);
   renderMap->setMaterial(mat);
   renderMap->setCullingEnabled(false);

   //Set up player renderable
   CMesh *playerMesh = CMeshLoader::load3dsMesh("Base/cabbage/cabbage_5.3ds");

   if (playerMesh) {
      playerMesh->resizeMesh(SVector3(0.45f));
      playerMesh->centerMeshByExtents(SVector3(0));
      playerMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load the cababge mesh\n");
   }

   playerRender = CApplication::get().getSceneManager().addMeshSceneObject(playerMesh, Flat, DeferredFlat);
   playerVector = levels[curNode].loc;
   playerVector.Y += 0.05f;
   playerRender->setTranslation(playerVector);
   playerRender->setRotation(SVector3(-90.0f, 0.0f, 45.0f));
   playerRender->setScale(SVector3(0.18f));

   (playerRender);

   CMesh *arrowMesh = CMeshLoader::load3dsMesh("Base/arrow.3ds");
   if (arrowMesh) {
     arrowMesh->resizeMesh(SVector3(0.5f));
     arrowMesh->centerMeshByExtents(SVector3(0));
     arrowMesh->calculateNormalsPerFace();
   }
   else {
     fprintf(stderr, "Failed to load the cababge mesh\n");
   }
   arrowRender1 = CApplication::get().getSceneManager().addMeshSceneObject(arrowMesh, Flat, DeferredFlat);
   arrowRender1->setTranslation(SVector3(0, -0.2f, 0.0) + playerVector);
   arrowRender1->setRotation(SVector3(-90.0f, 0.0f, 45.0f));
   arrowRender1->setScale(SVector3(0.18f));
                  
   arrowRender2 = CApplication::get().getSceneManager().addMeshSceneObject(arrowMesh, Flat, DeferredFlat);
   arrowRender2->setTranslation(SVector3(0,-0.2f,0) + playerVector);
   arrowRender2->setRotation(SVector3(90.0f, 0.0f, angleMaker(playerVector, levels[curNode + 1].loc) + 90.0f ));
   arrowRender2->setScale(SVector3(0.18f));



   CMesh *discMesh = CMeshLoader::createDiscMesh();
   discMesh->linearizeIndices();
   discMesh->calculateNormalsPerFace();

   CMesh *flagMesh = CMeshLoader::load3dsMesh("Base/flag2.3ds");
   
   if (flagMesh) {
     flagMesh->centerMeshByExtents(SVector3(0));
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

   //levelIcons(SVector3(0.6f, 0.06f, 0.6f), discMesh, 1);
/*
   changex = 0.6f;
   changey = 0.06f;
   changez = 0.6f;
   */
}

void COverworldState::levelIcons(SVector3 loc, CMesh *levelIcon, int iconColor)
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
   discRender->setScale(SVector3(0.1f));
}

void COverworldState::addMeshes(SVector3 loc, CMesh *newMesh, CTexture *texture)
{
  CMeshSceneObject *renderFlag = new CMeshSceneObject();
  renderFlag->setMesh(newMesh);
  renderFlag->setTranslation(loc + SVector3(-0.05f, 0.05f, 0.05f));
  renderFlag->setRotation(SVector3(-90,0,0));
  renderFlag->setScale(SVector3(.00150f, .000025f,.00016f));
  renderFlag->setTexture(texture);
  renderFlag->setShader(ERP_DEFAULT, ToonTexture);
  renderFlag->setShader(ERP_DEFERRED_OBJECTS, DeferredTexture);

  Application.getSceneManager().addSceneObject(renderFlag);



}

void COverworldState::bouncePlayer() {
   SVector3 m_base = playerVector;
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
    moved = true;
  }
  else if(dDown && curNode < NUM_LEVELS - 1 && (levels[curNode].completed || levelsUnlocked))
  {
    curNode++;
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
  }
}

void COverworldState::superInterpolator(SVector3 & curr, SVector3 & change,
    float delta, float transLength)
{
  SVector3 zeroV = SVector3(0.0f, 0.0f, 0.0f);

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

void COverworldState::shiftSetter(SVector3 & curr, SVector3 & change, SVector3 & target)
{
  change = target - curr;
}

float COverworldState::angleMaker(SVector3 start, SVector3 toPoint)
{
    //float asinxz, asinzx, acosxz, acoszx;
	float lookat = 0.f;
     SVector3 temp = toPoint - start;

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
  SVector3 temp = cameraPos[1] -levels[2].loc;
  temp.normalize();

  printf("TEST %f\n", (atan2(temp.X,temp.Z) * RAD_TO_DEG));

}
