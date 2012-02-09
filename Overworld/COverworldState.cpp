#include "COverworldState.h"
/* These are here because someone doesn't use extern, or put prototypes in their header files */

COverworldState::COverworldState()
: Application (CApplication::get())
{}

//Initalizer fxn
void COverworldState::begin()
{
   aDown = 0; dDown = 0; spaceDown = 0; wDown = 0; sDown = 0;
   camRotValue = 0;
   rot = SVector3(-90, 0, -90);
   SPosition2 size = Application.getWindowSize();
   WindowWidth = size.X;
   WindowHeight = size.Y; 
   glClearColor(0.4f,0.8,1,0);

   mouseDown = 0;


   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);
   //glEnable(GL_CULL_FACE);
   //glCullFace(GL_BACK);

   SDL_WM_SetCaption("Happy Cabbage Adventure", NULL);

   //Initialize Font

   eye = SVector3(1.47, 0.33, 0);
   look = SVector3(0.57, -0.08, 0.19);
   Camera = new CCamera((float)WindowWidth/(float)WindowHeight, 0.01f, 100.f, 60.f);
   Application.getSceneManager().setActiveCamera(Camera);
   float const LightBrightness = 0.6f;
   Camera->setPosition(eye);
   Camera->setLookDirection(look - eye);

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

void COverworldState::step(float delta) {
   //rot.X += 10*delta;
   camRotValue += 0.5*delta;
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
   eyeRot.X += 0.2*cos(camRotValue);
   eyeRot.Z += 0.2*(sin(camRotValue) + 1);
   eyeRot.Y += 0.1*(sin(camRotValue) + 1);
   Camera->setPosition(eyeRot);
   Camera->setLookDirection(look - eyeRot);

   renderMap->setRotation(rot);

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
         printf("Eye coords: %0.2f %0.2f %0.2f\n", eye.X, eye.Y, eye.Z);
         printf("Look coords: %0.2f %0.2f %0.2f\n", look.X, look.Y, look.Z);
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
   if(mapMesh) {
      mapMesh->centerMeshByExtents(SVector3(0));
      mapMesh->calculateNormalsPerFace();
   }
   renderMap = new CMeshRenderable();
   renderMap->setMesh(mapMesh);
   renderMap->getMaterial().Texture = CImageLoader::loadTexture("Models/world.bmp");
   renderMap->getMaterial().Shader = DiffuseTexture;
   CApplication::get().getSceneManager().addRenderable(renderMap);

   CShader* discShader = DiffuseTexture;

   CMesh *discMesh = CMeshLoader::loadAsciiMesh("Disc");
   discMesh->linearizeIndices();
   discMesh->calculateNormalsPerFace();

   discRender = new CMeshRenderable();
   discRender->setMesh(discMesh);
   discRender->getMaterial().Texture = CImageLoader::loadTexture("Models/disc_red.bmp");
   discRender->getMaterial().Shader = discShader;
   discRender->setTranslation(SVector3(0.5, -0.13, 0.1));
   discRender->setScale(SVector3(0.2));
   CApplication::get().getSceneManager().addRenderable(discRender);

   discRender = new CMeshRenderable();
   discRender->setMesh(discMesh);
   discRender->getMaterial().Texture = CImageLoader::loadTexture("Models/disc_orange.bmp");
   discRender->getMaterial().Shader = discShader;
   discRender->setTranslation(SVector3(0.9, -0.12999999, 0.3));
   discRender->setScale(SVector3(0.19));
   CApplication::get().getSceneManager().addRenderable(discRender);

   discRender = new CMeshRenderable();
   discRender->setMesh(discMesh);
   discRender->getMaterial().Texture = CImageLoader::loadTexture("Models/disc_red.bmp");
   discRender->getMaterial().Shader = discShader;
   discRender->setTranslation(SVector3(0.9, -0.13, 0.3));
   discRender->setScale(SVector3(0.2));
   CApplication::get().getSceneManager().addRenderable(discRender);
   
   CMesh *playerMesh;
   playerMesh = CMeshLoader::load3dsMesh("Models/crappycabbage.3ds");
   if (playerMesh) {
      playerMesh->resizeMesh(SVector3(0.1));
      playerMesh->centerMeshByExtents(SVector3(0));
      playerMesh->calculateNormalsPerFace();
   }
   else {
      fprintf(stderr, "Failed to load the cababge mesh\n");
   }

   playerRender = new CMeshRenderable();
   playerRender->setMesh(playerMesh);
   playerRender->getMaterial().Shader = Flat;
   playerRender->setTranslation(SVector3(0.9, 0, 0.3));
   playerRender->setScale(SVector3(1));

   CApplication::get().getSceneManager().addRenderable(playerRender);
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
