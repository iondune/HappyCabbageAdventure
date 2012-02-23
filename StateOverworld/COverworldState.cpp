#include "COverworldState.h"
#include "../CabbageSound/sound.h"

COverworldState::COverworldState()
: Application (CApplication::get())
{}

//Initalizer fxn
void COverworldState::begin()
{
   curNode = 0;
   aDown = 0; dDown = 0; spaceDown = 0; wDown = 0; sDown = 0;
   transitionTimer = 0.0;

   camRotValue = 0;
   rot = SVector3(-90, 0, -90);
   SPosition2 size = Application.getWindowSize();
   WindowWidth = size.X;
   WindowHeight = size.Y; 
   glClearColor(0.4f,0.8f,1,0);

   mouseDown = 0;

   loadLevels();
   setCameraTrans();


   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);

   SDL_WM_SetCaption("Happy Cabbage Adventure", NULL);

   changex = 0.87f;
   changey = 0.58f;
   changez = 1.27;

   //eye = SVector3(1.47f, 0.33f, 0);
   //look = SVector3(0.57f, -0.08f, 0.19f);
   eye = cameraPos[0];
   look = levels[0].loc;
   Camera = new CCamera((float)WindowWidth/(float)WindowHeight, 0.01f, 100.f, 60.f);
   Application.getSceneManager().setActiveCamera(Camera);
   float const LightBrightness = 0.6f;
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

   superInterpolator(look, lookShift, lookTarget, delta);
   superInterpolator(eye, eyeShift, eyeTarget, delta);

   transitionTimer = transitionTimer < 0.0f ? 0.0f : transitionTimer - delta;

   if(transitionTimer == 0.0f)
   {
     camRotValue += 0.5f*delta;
   }
   /*

   if(!(look == lookTarget))
   {
     printf("That's a paddlin: %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f\n",
         look.X, look.Y, look.Z, lookTarget.X, lookTarget.Y, lookTarget.Z);
   }
   */
}

//Runs at very start of display
void COverworldState::OnRenderStart(float const Elapsed)
{
  //printf("Elapsed %f",Elapsed);
  //printf("Elapsed %f",Application.getElapsedTime());
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


   SDL_GL_SwapBuffers();
}


//Sends event every time key pressed (also when held)
void COverworldState::OnKeyboardEvent(SKeyboardEvent const & Event)
{
  bool diskChanger = false;
  bool eyeChanger = false;

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
      if(Event.Key == SDLK_SPACE) {
        CGameState::get().levelName = levels[curNode].name;
         Application.getStateManager().setState(& CGameState::get());
         spaceDown = 1;
         changeSoundtrack("SMW.wav");
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
      if(Event.Key == SDLK_i){
        changey += 0.01f;
        eyeChanger = true;
      }
      if(Event.Key == SDLK_k){
        changey -= 0.01f;
        eyeChanger = true;
      }
      if(Event.Key == SDLK_u){
        changex += 0.01f;
        eyeChanger = true;
      }
      if(Event.Key == SDLK_j){
        changex -= 0.01f;
        eyeChanger = true;
      }
      if(Event.Key == SDLK_o){
        changez += 0.01f;
        eyeChanger = true;
      }
      if(Event.Key == SDLK_l){
        changez -= 0.01f;
        eyeChanger = true;
      }
      if(Event.Key == SDLK_p){
         printf("disk coords: %0.2f %0.2f %0.2f\n", changex,changey, changez);
      }
      if(Event.Key == SDLK_n){
        look = levels[0].loc;
        eye = SVector3(0.74, 0.48, 1.50);
      }
      if(Event.Key == SDLK_SPACE){
         spaceDown = 0;
      }
   }

   if(diskChanger)
        discRender->setTranslation(SVector3(changex,changey, changez));

   if(eyeChanger)
     eye = SVector3(changex, changey, changez);
}

void COverworldState::end()
{
   Application.getSceneManager().removeAllSceneObjects();
}

void COverworldState::loadLevels()
{
  //If you're adding more levels be sure to update COverworldState's NUM_LEVELS
  levels[0].name = "test.xml";
  levels[0].loc = SVector3(0.33, -0.17, 1.05); //bit of green near sole orange hill
  levels[1].name = "level1.xml";
  levels[1].loc = SVector3(0.83, 0.00, 0.65); //Green hill
  levels[2].name = "level2.xml";
  levels[2].loc = SVector3(0.9f, -0.12999999f, 0.3f);
  levels[3].name = "level3.xml";
  levels[3].loc = SVector3(0.5f, -0.13f, 0.1f);
  levels[4].name = "test.xml";
  levels[4].loc = SVector3(0.98, -0.27, -0.19); //Green beach
  levels[5].name = "test.xml";
  levels[5].loc = SVector3(0.94, -0.25, -0.38); //Yellow beach
}

void COverworldState::setCameraTrans()
{
  cameraPos[0] = SVector3(0.21f, 0.36f, 1.76f);
  cameraPos[1] = SVector3(1.47f, 0.33f, 0.0f);

}

void COverworldState::LoadShaders() {
   Flat = CShaderLoader::loadShader("Diffuse");
   Diffuse = CShaderLoader::loadShader("Diffuse");
   DiffuseTexture = CShaderLoader::loadShader("DiffuseTexture");
   //normalColor = CShaderLoader::loadShader("NormalColor");
}

void COverworldState::PrepMeshes()
{
   //Set up world map renderable
   CMesh *mapMesh = CMeshLoader::load3dsMesh("Models/world.3ds");
   if(mapMesh) {
      mapMesh->centerMeshByExtents(SVector3(0));
      mapMesh->calculateNormalsPerFace();
   }
   
   CMaterial mat;

   mat.Texture = CImageLoader::loadTexture("Models/world.bmp");
   renderMap = CApplication::get().getSceneManager().addMeshSceneObject(mapMesh, DiffuseTexture);
   renderMap->setMaterial(mat);

   //Set up player renderable
   CMesh *playerMesh = CMeshLoader::load3dsMesh("Models/crappycabbage.3ds");
   if (playerMesh) {
      playerMesh->resizeMesh(SVector3(0.5f));
      playerMesh->centerMeshByExtents(SVector3(0));
      playerMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load the cababge mesh\n");
   }

   playerRender = CApplication::get().getSceneManager().addMeshSceneObject(playerMesh, Flat);
   playerVector = levels[0].loc;
   playerVector.Y += 0.05;
   playerRender->setTranslation(playerVector);
   playerRender->setRotation(SVector3(-90, 0, 180));
   playerRender->setScale(SVector3(0.18f));

   (playerRender);

   CMesh *discMesh = CMeshLoader::loadAsciiMesh("Disc");
   discMesh->linearizeIndices();
   discMesh->calculateNormalsPerFace();

   for(int i = 0; i < NUM_LEVELS; i++)
   {
     levelIcons(levels[i].loc, discMesh, 1);
   }

   levelIcons(SVector3(0.6f, 0.06f, 0.6f), discMesh, 1);
/*
   changex = 0.6f;
   changey = 0.06f;
   changez = 0.6f;
   */
/*
   discRender = new CMeshSceneObject();
   discRender->setMesh(discMesh);
   discRender->getMaterial().Texture = CImageLoader::loadTexture("Models/disc_red.bmp");
   discRender->getMaterial().Shader = DiffuseTexture;
   discRender->setTranslation(SVector3(0.5f, -0.13f, 0.1f));
   discRender->setScale(SVector3(0.1f));
   CApplication::get().getSceneManager().addSceneObject(discRender);

   CMeshSceneObject *orangeDisc;
   orangeDisc = discRender = new CMeshSceneObject();
   discRender->setMesh(discMesh);
   discRender->getMaterial().Texture = CImageLoader::loadTexture("Models/disc_orange.bmp");
   discRender->getMaterial().Shader = DiffuseTexture;
   discRender->setTranslation(SVector3(0.9f, -0.12999999f, 0.3f));
   discRender->setScale(SVector3(0.09f));
   CApplication::get().getSceneManager().addSceneObject(discRender);

   discRender = new CMeshSceneObject();
   discRender->setMesh(discMesh);
   discRender->getMaterial().Texture = CImageLoader::loadTexture("Models/disc_red.bmp");
   discRender->getMaterial().Shader = DiffuseTexture;
   discRender->setTranslation(SVector3(0.9f, -0.13f, 0.3f));
   discRender->setScale(SVector3(0.1f));
   //CApplication::get().getSceneManager().addSceneObject(discRender);
   discRender = orangeDisc;
  */ 
}

void COverworldState::levelIcons(SVector3 loc, CMesh *levelIcon, int iconColor)
{

   discRender = CApplication::get().getSceneManager().addMeshSceneObject(levelIcon, DiffuseTexture);

   CMaterial mat;
   switch(iconColor)
   {
     case 1:
       mat.Texture = CImageLoader::loadTexture("Models/disc_red.bmp");
       break;

     case 2:
       mat.Texture = CImageLoader::loadTexture("Models/disc_orange.bmp");
       break;
       
     default:
       mat.Texture = CImageLoader::loadTexture("Models/disc_red.bmp");
       break;
   }
   discRender->setMaterial(mat);

   discRender->setTranslation(loc);
   discRender->setScale(SVector3(0.1f));
}

void COverworldState::bouncePlayer() {
   SVector3 m_base = playerVector;
   m_base.Y += 0.03f*(sin(camRotValue*15)+1);
   playerRender->setTranslation(m_base);
}


void COverworldState::movePlayer() {
  bool moved = false;
  if(transitionTimer > 0)
    return;

  if(aDown && curNode > 0)
  {
    curNode--;
    moved = true;
  }
  else if(dDown && curNode < NUM_LEVELS - 1)
  {
    curNode++;
    moved = true;
  }

  if(moved)
  {
    playerVector = levels[curNode].loc;
    playerVector.Y += 0.05;

    transitionTimer = TRANSITION_PERIOD;

    lookTarget = levels[curNode].loc;
    shiftSetter(look, lookShift, lookTarget);

    if(curNode == 1 && aDown)
    {
      eyeTarget = cameraPos[0];
      shiftSetter(eye, eyeShift, eyeTarget);
    }
    else if(curNode == 2 && dDown)
    {
      eyeTarget = cameraPos[1];
      shiftSetter(eye, eyeShift, eyeTarget);
    }



  }
  /*
   if(curNode == 0) {
      playerVector = SVector3(0.5f, -0.08f, 0.1f);
      //discRender->setTranslation(SVector3(0.5f, -0.12999999f, 0.1f));
      curNode = 1;
   }
   else {
      playerVector = SVector3(0.9f, -0.08f, 0.3f);
      //discRender->setTranslation(SVector3(0.9f, -0.12999999f, 0.3f));
      curNode = 0;
   }
   */
}

void COverworldState::superInterpolator(SVector3 & curr, SVector3 & change, SVector3 & target, float delta)
{
  SVector3 zeroV = SVector3(0.0f, 0.0f, 0.0f);

  if(transitionTimer <= 0)
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
    curr = curr + (change * delta/TRANSITION_PERIOD);
  }
}

void COverworldState::shiftSetter(SVector3 & curr, SVector3 & change, SVector3 & target)
{
  change = target - curr;
}
/*
void COverworldState::OnMouseEvent(SMouseEvent const & Event) {
   if(Event.Button.Value == SMouseEvent::EButton::Left) {
      if(Event.Pressed && Event.Type.Value == SMouseEvent::EType::Click) {
         mouseDown = 1;
      }
      else if(!Event.Pressed && Event.Type.Value == SMouseEvent::EType::Click) {
         mouseDown = 0;
      }
      if(mouseDown && Event.Type.Value == SMouseEvent::EType::Move) {
         int x = Event.Location.X;
         int y = Event.Location.Y;
         startx = Event.Location.X - Event.Movement.X;
         starty = WindowHeight - (Event.Location.Y - Event.Movement.Y) - 1;
         if(x <= WindowWidth && x >= 0 && y >= 0 && y <= WindowHeight) {
            int newy = WindowHeight-y-1;

            if((pitchphi < 50*M_PI/180 || (float)(newy-starty)<0) && 
                  (pitchphi > -50*M_PI/180 || (float)(newy-starty)>0)) {
               yawtheta += (float)(x-startx)/WindowWidth*M_PI;
               pitchphi += (float)(newy-starty)/WindowHeight*M_PI;

               look = SVector3(cos(pitchphi)*cos(yawtheta) + eye.X,
                     sin(pitchphi) + eye.Y,
                     cos(pitchphi)*cos(M_PI/2 - yawtheta) + eye.Z);
            }
            startx = x;
            starty = newy;
         }
      }
   }
}

void COverworldState::stepCamera(float delta) {
   SVector3 zoom = look - eye;
   zoom *= (1*delta);
   SVector3 strafe = SVector3(0,1,0).crossProduct(zoom);

   //D
   if(dDown) {
      look -= strafe;
      eye -= strafe;
   }
   //A
   if(aDown) {
      look += strafe;
      eye += strafe;
   }
   //W
   if(wDown) {
      look += zoom;
      eye += zoom;
   }
   //S
   if(sDown) {
      look -= zoom;
      eye -= zoom;
   }
}
*/
