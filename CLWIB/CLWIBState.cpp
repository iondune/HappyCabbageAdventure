#include "CLWIBState.h"
/* These are here because someone doesn't use extern, or put prototypes in their header files */
#include "draw.h"
#include "texture.h"
#include <cmath>

#ifdef _WIN32
static inline double round(double val)
{    
    return floor(val + 0.5);
}

#define M_PI 3.14159f
#endif

//Boolean integers for keypressing
int aDown = 0, dDown = 0, spaceDown = 0, wDown = 0, sDown = 0, gDown = 0, fDown = 0;
int backwardsView = 0, overView = 0;

freetype::font_data our_font;

int WindowWidth, WindowHeight;


float pointToWorldY(float why, float dist);
float pointToWorldX(float ex, float dist);
float x2w(int oldX);
float yp2w(int oldY);
void Load3DS();
void LoadShaders();
void LoadTextures();
void PrepMeshes();
void EngineInit();
void PrepPreviewBlock();

float previewBlockMouseX, previewBlockMouseY; 
float lastBlockPlacedLocationX, lastBlockPlacedLocationY;
using namespace Cabbage::Collider;
CActor *Player, *Derp;
CObject *Floor, *Block;
CMeshRenderable *PreviewBlock;

void BlockMesh() {
   cubeMesh = CMeshLoader::createCubeMesh();
   //cubeMesh->linearizeIndices();
   cubeMesh->calculateNormalsPerFace();
}

CLWIBState::CLWIBState()
: Application (CApplication::get())
{}

void CLWIBState::BlocksInit( void ) {
   PrepGrass(-25, -1, 50, 1);
   PrepSky();

   SRect2 area;
}

void initBlockMap();

//Initalizer fxn
void CLWIBState::begin()
{
   initBlockMap();
   blockWidth = 1;
   blockHeight = 1;
   SPosition2 size = Application.getWindowSize();
   WindowWidth = size.X;
   WindowHeight = size.Y; 
   glClearColor(0.4,0.8,1.0,0);

   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);
   //glEnable(GL_CULL_FACE);
   //glCullFace(GL_BACK);

   SDL_WM_SetCaption("Happy Cabbage Adventure", NULL);

   //Initialize Font
   our_font.init("WIFFLES_.TTF", 30);

   eye = SVector3(0,0,10);
   look = SVector3(0,0,-1);
   Camera = new CCamera((float)WindowWidth/(float)WindowHeight, 0.01f, 100.f, 60.f);
   Camera->setPosition(eye);
   Camera->setLookDirection(look - eye);
   Camera->recalculateViewMatrix();

   Application.getSceneManager().setActiveCamera(Camera);

   LoadShaders();

   Load3DS();
   LoadTextures();
   BlockMesh();

   //Load the meshes into VBOs
   PrepMeshes();

   srand(time(NULL));

   //Initialize Fxns
   BlocksInit();
   PrepPreviewBlock();

   printf("END OF BEGIN\n");
}


//Runs at very start of display
void CLWIBState::OnRenderStart(float const Elapsed)
{
   glViewport(0, 0, WindowWidth, WindowHeight);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   Camera->setPosition(eye);
   //look.Z = eye.Z;
   Camera->setLookDirection(look - eye);
   Camera->setProjection(60.f, (float)WindowWidth/(float)WindowHeight, 0.01f, 100.f);
   Camera->recalculateViewMatrix();

   stepCamera(Application.getElapsedTime());
   float x=round(eye.X + previewBlockMouseX),y= round(eye.Y + previewBlockMouseY);
   PreviewBlock->setTranslation(SVector3(x+(float)blockWidth/2,y+(float)blockHeight/2, 0));
   //PreviewBlock->setTranslation(SVector3((x+(x+w))/2, (y+(y+h))/2, 0));
   //PreviewBlock->setTranslation(SVector3(x,y, 0));

   Application.getSceneManager().drawAll();

   //Draw Text
   freetype::print(our_font, 10, WindowHeight-40.f, "Elapsed Time: %0.0f ", Application.getRunTime());
   freetype::print(our_font, (float)WindowWidth/2, (float)WindowHeight/2, "+");
      

   drawSubWindow();
   Application.getSceneManager().drawAll();

   SDL_GL_SwapBuffers();
}

void CLWIBState::drawSubWindow() {
   glViewport(WindowWidth-400, WindowHeight-200, 400, 200);
   glScissor(WindowWidth-400, WindowHeight-200, 400, 200);

   glEnable(GL_SCISSOR_TEST);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glDisable(GL_SCISSOR_TEST);

   Camera->setPosition(SVector3(eye.X, eye.Y + 5, 20));
   Camera->setLookDirection(SVector3(0,0,-1));
   Camera->recalculateViewMatrix();
   Camera->setProjection(60.f, 2, 0.01f, 100.f);

   Application.getSceneManager().drawAll();
}


//Sends event every time key pressed (also when held)
void CLWIBState::OnKeyboardEvent(SKeyboardEvent const & Event)
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
      if(Event.Key == SDLK_f){
         fDown = 1;
      }
      if(Event.Key == SDLK_g){
         gDown = 1;
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
      if(Event.Key == SDLK_u) {
         //3 is the number of static blocks created before the user can add in new blocks (ie unremovable)
         if(blocks.size() > 3) {
            Application.getSceneManager().removeRenderable(blocks.back());
            redo.push_back(blocks.back());
            blocks.pop_back();
         }
      }
      if(Event.Key == SDLK_r) {
         if(redo.size() > 0) {
            Application.getSceneManager().addRenderable(redo.back());
            blocks.push_back(redo.back());
            redo.pop_back();
         }
      }
      if(Event.Key == SDLK_m){
         
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
      if(Event.Key == SDLK_f){
         fDown = 0;
      }
      if(Event.Key == SDLK_g){
         gDown = 0;
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
void CLWIBState::end()
{
   our_font.clean();
}

void CLWIBState::PrepPreviewBlock() {
   blocks.push_back(PreviewBlock = new CMeshRenderable());
   PreviewBlock->setMesh(cubeMesh);
   PreviewBlock->getMaterial().Texture = grassTxt;
   PreviewBlock->getMaterial().Shader = DiffuseTexture;
   //PreviewBlock->setTranslation(SVector3((x+(x+w))/2, (y+(y+h))/2, 0));
   PreviewBlock->setScale(SVector3(blockWidth, blockHeight, 1));
   CApplication::get().getSceneManager().addRenderable(PreviewBlock);
}

//Minblockvalue = -25
//Heightoffset = 0.5
bool blockMap[100][100];

void initBlockMap() {
   int i,j;
   for(i=0; i<100; i++)
      for(j=0; j<100; j++)
         blockMap[i][j]=0;
}

void CLWIBState::PrepBlock(float x, float y, int w, int h) {
   if(x < -25 || y < -25)
      return;
   int i,j, ret=0;
   for(i=0;i<w;i++) {
      for(j=0;j<h;j++) { 
         if(!blockMap[(int)x+25+i][(int)(y-0.5+25)+j]) {
            ret = 1;
            break;
         }
      }
      if(ret)
         break;
   }
   if(!ret)
      return;

   printf("Placed block starting at %0.2f, %0.2f\n", x, y);
   CMeshRenderable *tempBlock;
   blocks.push_back(tempBlock = new CMeshRenderable());
   tempBlock->setMesh(cubeMesh);
   tempBlock->getMaterial().Texture = dirtTxt;
   tempBlock->getMaterial().Shader = DiffuseTexture;
   tempBlock->setTranslation(SVector3((x+(x+w))/2, (y+(y+h))/2, 0));
   //tempBlock->setTranslation(SVector3(x, y, 0));
   tempBlock->setScale(SVector3(w, h, 1));
   for(i = 0; i < w; i++) {
      for(j = 0; j < h; j++) {
         blockMap[(int)x+25+i][(int)(y-0.5+25)+j] = true;
      }
   }
   tempBlock->setRotation(SVector3(0, 0, 0));
   Application.getSceneManager().addRenderable(tempBlock);
   redo.clear();
}

void CLWIBState::PrepGrass(float x, float y, float w, float h) {
   CMeshRenderable *tempBlock;
   blocks.push_back(tempBlock = new CMeshRenderable());
   tempBlock->setMesh(cubeMesh);
   tempBlock->getMaterial().Texture = grassTxt;
   tempBlock->getMaterial().Shader = DiffuseTexture;
   tempBlock->setTranslation(SVector3((x+(x+w))/2, (y+(y+h))/2, 0));
   tempBlock->setScale(SVector3(w, h, 5));
   Application.getSceneManager().addRenderable(tempBlock);

}

void CLWIBState::PrepSky() {
   CMeshRenderable *tempBlock;
   blocks.push_back(tempBlock = new CMeshRenderable());
   tempBlock->setMesh(cubeMesh);
   tempBlock->getMaterial().Texture = skyTxt;
   tempBlock->getMaterial().Shader = DiffuseTexture;
   tempBlock->setTranslation(SVector3(0, 24, -2.5));
   tempBlock->setScale(SVector3(100, 50, 1));
   Application.getSceneManager().addRenderable(tempBlock);

}

void LoadShaders() {
   Flat = CShaderLoader::loadShader("Flat");
   Diffuse = CShaderLoader::loadShader("Diffuse");
   DiffuseTexture = CShaderLoader::loadShader("DiffuseTexture");
   //normalColor = CShaderLoader::loadShader("NormalColor");
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

float pointToWorldX(float ex, float dist)
{
   return 0.f;//tan(30*M_PI/180)
}
float pointToWorldY(float why, float dist)
{
    return 0.f;
}

float xp2w(int oldX)
{
   return (float)2/WindowWidth * oldX - 1; 
}

float yp2w(int oldY)
{
   return (float)-2/WindowHeight * oldY + 1; 
}

int startx, starty;
float pitchphi, yawtheta;
int mouseDown;
void CLWIBState::OnMouseEvent(SMouseEvent const & Event) {
   if(Event.Button.Value == SMouseEvent::EButton::Left) {
      if(Event.Pressed && Event.Type.Value == SMouseEvent::EType::Click && fDown) {
         if(blockWidth < 10)
            blockWidth++;
         PreviewBlock->setScale(SVector3(blockWidth, blockHeight, 1));
         return;
      }
      if(Event.Pressed && Event.Type.Value == SMouseEvent::EType::Click && gDown) {
         if(blockHeight < 10)
            blockHeight++;
         PreviewBlock->setScale(SVector3(blockWidth, blockHeight, 1));
         return;
      }
      if(Event.Pressed && Event.Type.Value == SMouseEvent::EType::Click) {
         mouseDown = 1;
         PrepBlock(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY), blockWidth, blockHeight);
         /* Snap camera to where the block was placed
         eye.X = look.X =  eye.X + 6*tan(30*M_PI/180)*xp2w(Event.Location.X);
         eye.Y = look.Y = eye.Y + 6*tan(30*M_PI/180)*yp2w(Event.Location.Y);
         */
      }
      else if(!Event.Pressed && Event.Type.Value == SMouseEvent::EType::Click) {
         mouseDown = 0;
      }
      if(Event.Type.Value == SMouseEvent::EType::Move) {
         previewBlockMouseX = 10*((float)WindowWidth/WindowHeight)*tan(30*M_PI/180)*xp2w(Event.Location.X);
         previewBlockMouseY = 10*tan(30*M_PI/180)*yp2w(Event.Location.Y);
         if(mouseDown) {
            PrepBlock(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY), blockWidth, blockHeight);
         }
      }
   }
   else if(Event.Button.Value == SMouseEvent::EButton::Right) {
      if(Event.Pressed && fDown) {
         if(blockWidth > 1)
            blockWidth--;
         PreviewBlock->setScale(SVector3(blockWidth, blockHeight, 1));
         return;
      }
      if(Event.Pressed && gDown) {
         if(blockHeight > 1)
            blockHeight--;
         PreviewBlock->setScale(SVector3(blockWidth, blockHeight, 1));
         return;
      }
   }
}

  
/*
   int startx, starty;
   float pitchphi, yawtheta;
   int mouseDown;
   void OnMouseEvent(SMouseEvent const & Event) {
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

                  //the Y and Z components are switched in our view
                  look = SVector3(cos(pitchphi)*cos(yawtheta) + eye.X, 
                     -cos(pitchphi)*cos(M_PI/2 - yawtheta) + eye.Y, 
                     sin(pitchphi) + eye.Z);
               }
               startx = x;
               starty = newy;
            }
         }
      }
   }
   */

void CLWIBState::stepCamera(float delta) {
   /*
   SVector3 zoom = look - eye;
   zoom *= (5*delta);
   SVector3 strafe = SVector3(0,0,1).crossProduct(zoom);

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
      zoom.Z = 0;
      look += zoom;
      eye += zoom;
   }
   //S
   if(sDown) {
      zoom.Z = 0;
      look -= zoom;
      eye -= zoom;
   }
   */
   float factor = 6;
   //D
   if(dDown) {
      eye.X += delta*factor;
      look.X += delta*factor;
   }
   //A
   if(aDown) {
      eye.X -= delta*factor;
      look.X -= delta*factor;
   }
   //W
   if(wDown) {
      eye.Y += delta*factor;
      look.Y += delta*factor;
   }
   //S
   if(sDown) {
      eye.Y -= delta*factor;
      look.Y -= delta*factor;
   }
   if(mouseDown) {
      PrepBlock(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY), blockWidth, blockHeight);
   }
}

