#include "CLWIBState.h"
/* These are here because someone doesn't use extern, or put prototypes in their header files */
#include <cmath>

#ifdef _WIN32
static inline double round(double val)
{
    return floor(val + 0.5);
}
#define M_PI 3.14159f
#endif


float x2w(int oldX);
float yp2w(int oldY);
void EngineInit();
void PrepPreviews();

float previewBlockMouseX, previewBlockMouseY; 
float lastBlockPlacedLocationX, lastBlockPlacedLocationY;
using namespace Cabbage::Collider;
CMeshRenderable *PreviewBlock, *PreviewEnemy;

CLWIBState::CLWIBState()
: Application (CApplication::get())
{}

void CLWIBState::BlocksInit( void ) {
   PrepGrass(-25, -1, 50, 1);
   PrepSky();

   SRect2 area;
}

struct quickndirty {
   CMeshRenderable *r;
   CPlaceable *p;
   bool o;
   int mapX,mapY;
} typedef qd;

void initBlockMap();

//Minblockvalue = -25
//Heightoffset = 0.5
qd blockMap[100][100];


//Initalizer fxn
void CLWIBState::begin()
{
   aDown = dDown = spaceDown = wDown = sDown = gDown = fDown = tDown = eDown = 0;
   cubeMesh = CMeshLoader::createCubeMesh();
   cubeMesh->calculateNormalsPerFace();

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

   DiffuseTexture = CShaderLoader::loadShader("DiffuseTexture");
   Flat = CShaderLoader::loadShader("Flat");
   DiffuseTextureBright = CShaderLoader::loadShader("DiffuseTextureBright");


   //Load the meshes into VBOs

   srand(time(NULL));

   //Initialize Fxns
   BlocksInit();
   PrepPreviews();

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
   if(tDown) {
      PreviewBlock->setVisible(false);
      PreviewEnemy->setVisible(false);
   }
   else if(eDown) {
      PreviewEnemy->setVisible(true);
   }
   else {
      PreviewBlock->setVisible(true);
   }
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
      if(Event.Key == SDLK_p){
         printXML();
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
      if(Event.Key == SDLK_e){
         eDown = 1;
      }
      if(Event.Key == SDLK_k){
      }
      if(Event.Key == SDLK_j){
         //printf("Angle: %d\n", ANGLE(overView, backwardsView));
      }
      if(Event.Key == SDLK_u) {
         //3 is the number of static blocks created before the user can add in new blocks (ie unremovable)
         if(blocks.size() > 3 && placeables.size() > 0) {
            Application.getSceneManager().removeRenderable(blocks.back());
            CPlaceable *m_block = placeables.back();
            redo.push_back(blocks.back());
            redoPlaceables.push_back(placeables.back());

            int i,j;
            for(i = 0; i < m_block->w; i++) {
               for(j = 0; j < m_block->h; j++) {
                  blockMap[(int)m_block->x+25+i][(int)(m_block->y-0.5+25)+j].o = false;
                  blockMap[(int)m_block->x+25+i][(int)(m_block->y-0.5+25)+j].r = NULL;
                  blockMap[(int)m_block->x+25+i][(int)(m_block->y-0.5+25)+j].p = NULL;
                  blockMap[(int)m_block->x+25+i][(int)(m_block->y-0.5+25)+j].mapX = -1;
                  blockMap[(int)m_block->x+25+i][(int)(m_block->y-0.5+25)+j].mapY = -1;
               }
            }

            placeables.pop_back();
            blocks.pop_back();
         }
      }
      if(Event.Key == SDLK_r) {
         if(redo.size() > 0 && redoPlaceables.size() > 0) {
            Application.getSceneManager().addRenderable(redo.back());
            CPlaceable *m_block = redoPlaceables.back();
            CMeshRenderable *m_r = redo.back();
            blocks.push_back(redo.back());
            placeables.push_back(redoPlaceables.back());

            int i,j;
            for(i = 0; i < m_block->w; i++) {
               for(j = 0; j < m_block->h; j++) {
                  blockMap[(int)m_block->x+25+i][(int)(m_block->y-0.5+25)+j].o = true;
                  blockMap[(int)m_block->x+25+i][(int)(m_block->y-0.5+25)+j].p = m_block;
                  blockMap[(int)m_block->x+25+i][(int)(m_block->y-0.5+25)+j].r = m_r;

                  blockMap[(int)m_block->x+25+i][(int)(m_block->y-0.5+25)+j].mapX = (int)m_block->x+25;
                  blockMap[(int)m_block->x+25+i][(int)(m_block->y-0.5+25)+j].mapY = (int)(m_block->y-0.5+25);
               }
            }

            redo.pop_back();
            redoPlaceables.pop_back();
         }
      }
      if(Event.Key == SDLK_t){
         tDown = 1;
      }
      if(Event.Key == SDLK_m){
         
      }
      if(Event.Key == SDLK_SPACE) {
         spaceDown = 1;

      }
      if(Event.Key == SDLK_ESCAPE) {
         //TODO: Replace with an event/signal to end the game world 
         //finished = true;
         glViewport(0, 0, WindowWidth, WindowHeight);
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
      if(Event.Key == SDLK_e){
         eDown = 0;
      }
      if(Event.Key == SDLK_f){
         fDown = 0;
      }
      if(Event.Key == SDLK_g){
         gDown = 0;
      }
      if(Event.Key == SDLK_t){
         tDown = 0;
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

void CLWIBState::printXML() {
    xmlwriter *worldlist = new xmlwriter("test.xml");
    
    std::vector<CPlaceable*>::iterator it;
    for(it=placeables.begin();it<placeables.end();it++) {
       (*it)->writeXML(worldlist);
    }
    worldlist->CloseAlltags();
    delete worldlist;
}

//Runs at program close (currently not implemented)
void CLWIBState::end()
{
   our_font.clean();
   placeables.clear();
   redoPlaceables.clear();
   blocks.clear();
   redo.clear();
   Application.getSceneManager().removeAllRenderables();
}

void CLWIBState::PrepPreviews() {
   blocks.push_back(PreviewBlock = new CMeshRenderable());
   PreviewBlock->setMesh(cubeMesh);

   PreviewBlock->getMaterial().Texture = CImageLoader::loadTexture("Textures/grass.bmp");
   PreviewBlock->getMaterial().Shader = DiffuseTexture;
   //PreviewBlock->setTranslation(SVector3((x+(x+w))/2, (y+(y+h))/2, 0));
   PreviewBlock->setScale(SVector3(blockWidth, blockHeight, 1));
   CApplication::get().getSceneManager().addRenderable(PreviewBlock);

   blocks.push_back(PreviewEnemy = new CMeshRenderable());
   appleMesh = CMeshLoader::load3dsMesh("Models/appleEnemy.3ds");
   appleMesh->calculateNormalsPerFace();
   PreviewEnemy->setMesh(appleMesh);

   PreviewEnemy->getMaterial().Shader = Flat;
   PreviewBlock->setScale(SVector3(1, 1, 1));

   CApplication::get().getSceneManager().addRenderable(PreviewEnemy);
   PreviewEnemy->setVisible(false);
}

void initBlockMap() {
   int i,j;
   for(i=0; i<100; i++)
      for(j=0; j<100; j++) {
         blockMap[i][j].o = false;
         blockMap[i][j].p = NULL;
         blockMap[i][j].r = NULL;
         blockMap[i][j].mapX = -1;
         blockMap[i][j].mapY = -1;
      }
}

void CLWIBState::PrepBlock(float x, float y, int w, int h) {
   if(x < -25 || y < -25)
      return;
   int i,j, ret=0;
   for(i=0;i<w;i++) {
      for(j=0;j<h;j++) {
         if(!blockMap[(int)x+25+i][(int)(y-0.5+25)+j].o) {
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
   CBlock *tempPlaceable;
   blocks.push_back(tempBlock = new CMeshRenderable());
   placeables.push_back(tempPlaceable = new CBlock(x, y, w, h));
   tempBlock->setMesh(cubeMesh);
   tempBlock->getMaterial().Texture = CImageLoader::loadTexture("Textures/dirt.bmp");;
   tempBlock->getMaterial().Shader = DiffuseTexture;
   tempBlock->setTranslation(SVector3((x+(x+w))/2, (y+(y+h))/2, 0));
   //tempBlock->setTranslation(SVector3(x, y, 0));
   tempBlock->setScale(SVector3(w, h, 1));
   for(i = 0; i < w; i++) {
      for(j = 0; j < h; j++) {
         blockMap[(int)x+25+i][(int)(y-0.5+25)+j].o = true;
         blockMap[(int)x+25+i][(int)(y-0.5+25)+j].r = tempBlock;
         blockMap[(int)x+25+i][(int)(y-0.5+25)+j].p = tempPlaceable;
         blockMap[(int)x+25+i][(int)(y-0.5+25)+j].mapX = (int)x+25;
         blockMap[(int)x+25+i][(int)(y-0.5+25)+j].mapY = (int)(y-0.5+25);
      }
   }
   tempBlock->setRotation(SVector3(0, 0, 0));
   Application.getSceneManager().addRenderable(tempBlock);
   redo.clear();
   redoPlaceables.clear();
}

void CLWIBState::PrepGrass(float x, float y, float w, float h) {
   CMeshRenderable *tempBlock;
   blocks.push_back(tempBlock = new CMeshRenderable());
   tempBlock->setMesh(cubeMesh);

   tempBlock->getMaterial().Texture = CImageLoader::loadTexture("Textures/grass.bmp");
   tempBlock->getMaterial().Shader = DiffuseTexture;
   tempBlock->setTranslation(SVector3((x+(x+w))/2, (y+(y+h))/2, 0));
   tempBlock->setScale(SVector3(w, h, 5));
   Application.getSceneManager().addRenderable(tempBlock);

}

void CLWIBState::PrepSky() {
   CMeshRenderable *tempBlock;
   blocks.push_back(tempBlock = new CMeshRenderable());
   tempBlock->setMesh(cubeMesh);
   tempBlock->getMaterial().Texture = CImageLoader::loadTexture("Textures/sky.bmp");
   tempBlock->getMaterial().Shader = DiffuseTexture;
   tempBlock->setTranslation(SVector3(0, 24, -2.5));
   tempBlock->setScale(SVector3(100, 50, 1));
   Application.getSceneManager().addRenderable(tempBlock);
}

/*
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
*/

/*
void LoadTextures()
{
   grassImg = CImageLoader::loadImage("Textures/grass.bmp");
   skyImg = CImageLoader::loadImage("Textures/sky.bmp");
   dirtImg = CImageLoader::loadImage("Textures/dirt.bmp");
   blueFlwrImg = CImageLoader::loadImage("Textures/blueFlower.bmp");
   pinkFlwrImg = CImageLoader::loadImage("Textures/pinkFlower.bmp");
   poinImg = CImageLoader::loadImage("Textures/poin.bmp");
}
*/


float CLWIBState::xp2w(int oldX)
{
   return (float)2/WindowWidth * oldX - 1; 
}

float CLWIBState::yp2w(int oldY)
{
   return (float)-2/WindowHeight * oldY + 1; 
}
qd lastMouseOveredBlock;
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
         if(!tDown)
            PrepBlock(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY), blockWidth, blockHeight);
         else {
            if(lastMouseOveredBlock.o) {
               Application.getSceneManager().removeRenderable(lastMouseOveredBlock.r);
               placeables.erase(std::remove(placeables.begin(), placeables.end(), lastMouseOveredBlock.p), placeables.end());
               blocks.erase(std::remove(blocks.begin(), blocks.end(), lastMouseOveredBlock.r), blocks.end());
               redoPlaceables.push_back(lastMouseOveredBlock.p);
               redo.push_back(lastMouseOveredBlock.r);
               int x = lastMouseOveredBlock.mapX;
               int y = lastMouseOveredBlock.mapY;

               int i,j;
               for(i = 0; i < lastMouseOveredBlock.p->w; i++) {
                  for(j = 0; j < lastMouseOveredBlock.p->h; j++) {
                     blockMap[x+i][y+j].o = false;
                     blockMap[x+i][y+j].r = NULL;
                     blockMap[x+i][y+j].p = NULL;
                     blockMap[x+i][y+j].mapX = -1;
                     blockMap[x+i][y+j].mapY = -1;
                  }
               }
               lastMouseOveredBlock = blockMap[x][y];
            }
         }

         /* Snap camera to where the block was placed
         eye.X = look.X =  eye.X + 6*tan(30*M_PI/180)*xp2w(Event.Location.X);
         eye.Y = look.Y = eye.Y + 6*tan(30*M_PI/180)*yp2w(Event.Location.Y);
         */
      }
      else if(!Event.Pressed && Event.Type.Value == SMouseEvent::EType::Click) {
         mouseDown = 0;
      }
      if(Event.Type.Value == SMouseEvent::EType::Move) {
         float oldPBMX = previewBlockMouseX, oldPBMY = previewBlockMouseY;
         previewBlockMouseX = 10*((float)WindowWidth/WindowHeight)*tan(30*M_PI/180)*xp2w(Event.Location.X);
         previewBlockMouseY = 10*tan(30*M_PI/180)*yp2w(Event.Location.Y);
            float x = round(eye.X + previewBlockMouseX);
            float y = round(eye.Y + previewBlockMouseY);
            float oldx = round(eye.X + oldPBMX);
            float oldy = round(eye.Y + oldPBMY);
            if(x < -25 || y < -25)
               return;
            if(oldx == x && oldy == y)
               return;
            qd m_qd;
            m_qd = blockMap[(int)x+25][(int)(y-0.5+25)];
            if(tDown) {
               if(m_qd.o && m_qd.r != lastMouseOveredBlock.r) {
                  m_qd.r->getMaterial().Shader = DiffuseTextureBright;
               }
            }
            if(lastMouseOveredBlock.o && m_qd.r != lastMouseOveredBlock.r) {
               lastMouseOveredBlock.r->getMaterial().Shader = DiffuseTexture;
            }
            if(!tDown && mouseDown) {
               PrepBlock(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY), blockWidth, blockHeight);
            }
            lastMouseOveredBlock = m_qd;
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
   if(!tDown && mouseDown) {
      PrepBlock(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY), blockWidth, blockHeight);
   }
}

