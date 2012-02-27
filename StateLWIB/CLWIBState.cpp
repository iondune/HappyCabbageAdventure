#include "CLWIBState.h"
/* These are here because someone doesn't use extern, or put prototypes in their header files */
#include <cmath>

#ifdef _WIN32
static inline float round(float r)
{
    return (r > 0.f) ? floor(r + 0.5f) : ceil(r - 0.5f);

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
CMeshSceneObject *PreviewBlock, *PreviewEnemy, *PreviewCabbage, *PreviewGround;

CLWIBState::CLWIBState()
: Application (CApplication::get())
{}

void CLWIBState::BlocksInit( void ) {
   PrepGrass(-25, -1, 1, 1);
   PrepSky();

   SRect2 area;
}

struct quickndirty {
   CMeshSceneObject *r;
   CPlaceable *p;
   bool o;
   int mapX,mapY;
} typedef qd;

void initBlockMap();

//Minblockvalue = -25
//Heightoffset = 0.5
qd blockMap[225][100];


//Initalizer fxn
void CLWIBState::begin()
{
   textureType = 0;
   enemyType = 0;
   aDown = dDown = spaceDown = wDown = sDown = gDown = fDown = tDown = eDown = mDown = oneDown = twoDown = threeDown = cDown = 0;
   cubeMesh = CMeshLoader::createCubeMesh();
   cubeMesh->calculateNormalsPerFace();

   showHelp = false;
   blockCycle = 0;
   initBlockMap();
   blockWidth = 1;
   blockHeight = 1;
   blockDepth = 1;
   SPosition2 size = Application.getWindowSize();
   WindowWidth = size.X;
   WindowHeight = size.Y; 
   glClearColor(0.4f,0.8f,1.0f,0);

   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);
   //glEnable(GL_CULL_FACE);
   //glCullFace(GL_BACK);

   SDL_WM_SetCaption("Happy Cabbage Adventure", NULL);

   //Initialize Font
   our_font.init("WIFFLES_.TTF", 30);

   eye = SVector3(0,0,10);
   look = SVector3(0,0,-1);
   Camera = new CPerspectiveCamera((float)WindowWidth/(float)WindowHeight, 0.01f, 100.f, 60.f);
   Camera->setPosition(eye);
   Camera->setLookDirection(look - eye);
   Camera->recalculateViewMatrix();
   Application.getSceneManager().setActiveCamera(Camera);

   Diffuse = CShaderLoader::loadShader("Diffuse");
   DiffuseTexture = CShaderLoader::loadShader("DiffuseTexture");
   DiffuseTextureBright = CShaderLoader::loadShader("DiffuseTextureBright");


   //Load the meshes into VBOs

   srand((unsigned int) time(0));

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
   PreviewEnemy->setTranslation(SVector3(x+0.5f,y+0.5f, 0));
   PreviewCabbage->setTranslation(SVector3(x+0.5f,y+0.5f, 0));

   if(tDown) {
      PreviewCabbage->setVisible(false); 
      PreviewBlock->setVisible(false);
      PreviewEnemy->setVisible(false);
   }
   else if(twoDown) {
      PreviewCabbage->setVisible(false); 
      PreviewEnemy->setVisible(true);
      PreviewBlock->setVisible(false);
   }
   else if(oneDown) {
   
      PreviewCabbage->setVisible(true); 
      PreviewEnemy->setVisible(false);
      PreviewBlock->setVisible(false);
   }
   else {
      PreviewCabbage->setVisible(false); 
      PreviewBlock->setVisible(true);
      PreviewEnemy->setVisible(false);
   }


   Application.getSceneManager().drawAll();

   //Draw Text
   //freetype::print(our_font, 10, WindowHeight-40.f, "Elapsed Time: %0.0f ", Application.getRunTime());
   if (!showHelp)
       freetype::print(our_font, (float)WindowWidth/2, (float)WindowHeight/2, "+");
      
    if (showHelp) {
        freetype::print(our_font, 15, WindowHeight - 230.f, \
            "WASD to control camera\n"\
            "You add blocks by defualt\n"\
            "press Z/X change block type\n"\
            "Press E to place enemies press Z/X add different enemies\n"\
            "Press F/G to make blocks shorter/longer\n"\
            "Press H/J to make blocks skinny/wider\n"\
            "press R to Redo action\n");
    }
    else
        freetype::print(our_font, 15, WindowHeight - 50.f, "Press F1 For Help");
    if (!twoDown && !showHelp && !tDown && !oneDown) {
        freetype::print(our_font, 20, WindowHeight - 100.f, "Placing block\n\n");
        if (cDown == 0) {
            if (textureType == 0) {
                freetype::print(our_font, 20, WindowHeight - 150.f, "Placing grass block\n");
                PreviewEnemy->setMesh(appleMesh);
            }
            if (textureType == 1) {
                freetype::print(our_font, 20, WindowHeight - 150.f, "Placing dirt block\n");
                PreviewEnemy->setMesh(orangeMesh);
            }
            if (textureType == 2) {
                freetype::print(our_font, 20, WindowHeight - 150.f, "Placing rock block \n");
                PreviewEnemy->setMesh(kiwiMesh);
            }

            if (textureType == -5) {
                freetype::print(our_font, 20, WindowHeight - 150.f, "ground block\n");
                PreviewEnemy->setMesh(cubeMesh);
            }
        }
        if (cDown == 1) {
           freetype::print(our_font, 20, WindowHeight - 150.f, "changing block width\n");
           freetype::print(our_font, 20, WindowHeight - 200.f, "block width is %d\n", blockWidth);
        }
        if (cDown == 2) {
           freetype::print(our_font, 20, WindowHeight - 150.f, "changing block height\n");
           freetype::print(our_font, 20, WindowHeight - 200.f, "block height is %d\n", blockHeight);
        }
        if (cDown == 3) {
           freetype::print(our_font, 20, WindowHeight - 150.f, "changing block depth\n");
           freetype::print(our_font, 20, WindowHeight - 200.f, "block depth is %d\n", blockDepth);
        }
    }
    if (twoDown && !showHelp && !tDown && !oneDown) {
        freetype::print(our_font, 20, WindowHeight - 100.f, "Placing enemy\n");
        if (enemyType == 0) {
            freetype::print(our_font, 20, WindowHeight - 150.f, "Placing Apple\n");
           PreviewEnemy->setMesh(appleMesh);
        }
        if (enemyType == 1) {
            freetype::print(our_font, 20, WindowHeight - 150.f, "Placing Orange\n");
            PreviewEnemy->setMesh(orangeMesh);
        }
        if (enemyType == 2) {
            freetype::print(our_font, 20, WindowHeight - 150.f, "Placing Kiwi\n");
            PreviewEnemy->setMesh(kiwiMesh);
        }

        if (enemyType == 3) {
            freetype::print(our_font, 20, WindowHeight - 150.f, "Placing Grape\n");
            PreviewEnemy->setMesh(cubeMesh);
        }

        if (enemyType == 4) {
           freetype::print(our_font, 20, WindowHeight - 150.f, "Placing Flame\n");
           PreviewEnemy->setMesh(cubeMesh);
        }

        if (enemyType == 5) {
           freetype::print(our_font, 20, WindowHeight - 150.f, "Placing Blade\n");
           PreviewEnemy->setMesh(cubeMesh);
        }
    }
    if (oneDown && !showHelp && !tDown && !twoDown) {
        freetype::print(our_font, 20, WindowHeight - 100.f, "Insert Cabbage\n\n");
    }
    if (tDown && !showHelp )
        freetype::print(our_font, 20, WindowHeight - 100.f, "Remove mode\n\n");
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
        if(Event.Key == SDLK_1){
            if (oneDown == 1)
                oneDown = 0;
            else {
                oneDown = 1;
                tDown = 0;
                twoDown = 0;
            }
        }
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
        if(Event.Key == SDLK_d){
            dDown = 1;
        }
        if(Event.Key == SDLK_2){
            if (twoDown == 1 ) {
                twoDown = 0;
                if(textureType == -5) {
                    blockWidth = 5;
                    blockHeight = 5;
                    blockDepth = 5;
                    PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
                }
            }
            else {
                twoDown = 1; //enemy
                tDown = 0;
                oneDown = 0;
                blockWidth = 1;
                blockHeight = 1;
                blockDepth = 1;
                PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
            }
        }
        if(Event.Key == SDLK_k){
            loadWorld();
        }
        if(Event.Key == SDLK_j){
            //printf("Angle: %d\n", ANGLE(overView, backwardsView));
        }
        if(Event.Key == SDLK_u) {
            //3 is the number of static blocks created before the user can add in new blocks (ie unremovable)
            if(blocks.size() > 3 && placeables.size() > 0) {
                Application.getSceneManager().removeSceneObject(blocks.back());
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
                Application.getSceneManager().addSceneObject(redo.back());
                CPlaceable *m_block = redoPlaceables.back();
                CMeshSceneObject *m_r = redo.back();
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
            if (tDown == 1)
                tDown = 0;
            else {
                tDown = 1; //remove
                twoDown = 0;
                oneDown = 0;
            }
        }
        if(Event.Key == SDLK_m){
            mDown = 1; //move
        }
        if(Event.Key == SDLK_c){
            if(cDown <= 3)
                cDown++;
            else
                cDown = 0;
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
        if(Event.Key == SDLK_F1) {
            showHelp = Event.Pressed;
        }
        if(Event.Key == SDLK_x ) { // adding generally
            if (twoDown) {
                if (enemyType < 6) //temp constraint
                    enemyType++;
                else
                    enemyType = 0;
            } else {
                if (cDown == 0) { 
                    if (textureType < 3 && textureType >= 0 && textureType != 2)
                        textureType++;
                    else if (textureType == 2) {
                        textureType = -5;
                        blockWidth = 5;
                        blockHeight = 5;
                        blockDepth = 5;
                        PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
                    }
                    else if (textureType == -5) {
                        textureType = 0;
                        blockWidth = 1;
                        blockHeight = 1;
                        blockDepth = 1;
                        PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
                    }
                }
            }
            if (cDown == 1) {
                if(blockWidth < 10 && textureType != 2)
                    blockWidth++;
                PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
            }
            if (cDown == 2) {
                if(blockHeight < 10 && textureType != 2)
                    blockHeight++;
                PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
            }
            if (cDown == 3) {
                if(blockDepth < 6 && textureType != 2)
                    blockDepth++;
                PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
            }
        } 
        if (Event.Key == SDLK_z) { // subtracting generally
            if (twoDown) {
                if (enemyType != 0) 
                    enemyType--;
                else
                    enemyType = 0;
            } else {
                if (cDown == 0) {
                    if (textureType != 0 && textureType > 0)   
                        textureType--;
                    else if (textureType == 0) {
                        textureType = -5;
                        blockWidth = 5;
                        blockHeight = 5;
                        blockDepth = 5;
                        PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
                    }
                    else if (textureType == -5) {
                        textureType = 2;
                        blockWidth = 1;
                        blockHeight = 1;
                        blockDepth = 1;
                        PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
                    }
                }
                if(cDown == 1) {
                    if(blockWidth > 1 && textureType != 2) 
                        blockWidth--;
                    PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
                }
                if(cDown == 2) {
                    if(blockWidth > 1 && textureType != 2)
                        blockWidth--;
                    PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
                }
                if(cDown == 3) {
                    if(blockDepth > 1 && textureType != 2)
                        blockDepth--;
                    PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
                }
            }

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
      /*if(Event.Key == SDLK_e){
         twoDown = 0;
      }*/
      if(Event.Key == SDLK_f){
         fDown = 0;
      }
      if(Event.Key == SDLK_g){
         gDown = 0;
      }
     /* if(Event.Key == SDLK_t){
         tDown = 0;
      }*/
      if(Event.Key == SDLK_k){
      }
      if(Event.Key == SDLK_j){
      }
      if(Event.Key == SDLK_m){
         mDown = 0; //move
      }
      if(Event.Key == SDLK_SPACE){
         spaceDown = 0;
      }
      if(Event.Key == SDLK_F1) {
         showHelp = false;
      }
   }
}

void CLWIBState::loadWorld() {
    int x,y,h,w,d,t;
    int moving, range, speed;
    std::string name;
    //float spd, rng;
     
    cout << "Enter the name of the file you want to load: ";
    cin >> name;
    irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(name.c_str());
	while (xml && xml->read())
	{
        switch(xml->getNodeType())
        {
        case irr::io::EXN_TEXT:
             break;
        case irr::io::EXN_ELEMENT:
           if(!strcmp("CBlock", xml->getNodeName()))
           {
                // id, X, Y, height, width / from 0,1,2 so on
                x = xml->getAttributeValueAsInt(0);
                y = xml->getAttributeValueAsInt(1);
                h = xml->getAttributeValueAsInt(2);
                w = xml->getAttributeValueAsInt(3);
                d = xml->getAttributeValueAsInt(4);
                t = xml->getAttributeValueAsInt(5);
                moving = xml->getAttributeValueAsInt(6);
                range = (int) xml->getAttributeValueAsFloat(7); //Range
                speed = (int) xml->getAttributeValueAsFloat(8); //Speed
                PrepBlock((float)x,(float)y,w,h,d,t);
                printf("texture is %d\n", t);
           }
           if(!strcmp("CEnemy", xml->getNodeName()))
           {
              x = xml->getAttributeValueAsInt(0);
              y = xml->getAttributeValueAsInt(1);
              h = xml->getAttributeValueAsInt(2);
              w = xml->getAttributeValueAsInt(3);
              t = xml->getAttributeValueAsInt(4);
              PrepEnemy((float)x,(float)y,t);
           }
         break;
        
        }
    }
}

void CLWIBState::printXML() {
    std::string name;
    cout << "Enter the name of the file you want to save: ";
    cin >> name;

    cout << name;

    xmlwriter *worldlist = new xmlwriter(name);
    
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
   Application.getSceneManager().removeAllSceneObjects();
}

void CLWIBState::PrepPreviews() {
   blocks.push_back(PreviewBlock = new CMeshSceneObject());
   PreviewBlock->setMesh(cubeMesh);

   PreviewBlock->setTexture("Textures/white.bmp");
  
   PreviewBlock->setShader(DiffuseTexture);
   PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
   CApplication::get().getSceneManager().addSceneObject(PreviewBlock);

   blocks.push_back(PreviewEnemy = new CMeshSceneObject());
   appleMesh = CMeshLoader::load3dsMesh("Models/appleEnemy.3ds");
   orangeMesh = CMeshLoader::load3dsMesh("Models/orange.3ds");
   kiwiMesh = CMeshLoader::load3dsMesh("Models/killerkiwi.3ds");
   cabbageMesh = CMeshLoader::load3dsMesh("Models/crappycabbage.3ds");
   bladeMesh = CMeshLoader::load3dsMesh("Models/trap1.3ds");

   blocks.push_back(PreviewCabbage = new CMeshSceneObject());
   //PreviewCabbage->setMesh(cabbageMesh
   
   if(appleMesh) {
      appleMesh->resizeMesh(SVector3(1));
      appleMesh->centerMeshByExtents(SVector3(0));
      appleMesh->calculateNormalsPerFace();
   }

   if(orangeMesh) {
         orangeMesh->resizeMesh(SVector3(1));
         orangeMesh->centerMeshByExtents(SVector3(0));
         orangeMesh->calculateNormalsPerFace();
    }

   if(kiwiMesh) {
         kiwiMesh->resizeMesh(SVector3(1));
         kiwiMesh->centerMeshByExtents(SVector3(0));
         kiwiMesh->calculateNormalsPerFace();
    }
   if(bladeMesh) {
      bladeMesh->resizeMesh(SVector3(1));
      bladeMesh->centerMeshByExtents(SVector3(0));
      bladeMesh->calculateNormalsPerFace();
   }
   /*PreviewCabbage->setShader(Diffuse);
   PreviewCabbage->setRotation(SVector3(-90, 0, 0));
   PreviewCabbage->setScale(SVector3(0.5,0.5, 0.5));*/
   PreviewEnemy->setShader(Diffuse);
   PreviewEnemy->setRotation(SVector3(-90, 0, 0));
   PreviewBlock->setScale(SVector3(1, 1, 1));

   CApplication::get().getSceneManager().addSceneObject(PreviewEnemy);
   PreviewEnemy->setVisible(false);
}

void initBlockMap() {
   int i,j;
   for(i=0; i<225; i++)
      for(j=0; j<100; j++) {
         blockMap[i][j].o = false;
         blockMap[i][j].p = NULL;
         blockMap[i][j].r = NULL;
         blockMap[i][j].mapX = -1;
         blockMap[i][j].mapY = -1;
      }
}

void CLWIBState::PrepEnemy(float x, float y, int type) {
   if(x < -25 || y < -25 || x >= 200 || y >= 75)
      return;
   if(blockMap[(int)x+25][(int)(y-0.5+25)].o) {
      printf("Blockmap space occupied. Did not place enemy\n");
      return;
   }

   printf("Placed enemy starting at %0.2f, %0.2f\n", x, y);
   CMeshSceneObject *tempEnemy;
   CEnemy *tempPlaceable;
   blocks.push_back(tempEnemy = new CMeshSceneObject());
   placeables.push_back(tempPlaceable = new CEnemy(x, y, 1, 1, type));
   if (type == 0)
        tempEnemy->setMesh(appleMesh);
   if (type == 1)
        tempEnemy->setMesh(orangeMesh);
   if (type == 2)
        tempEnemy->setMesh(kiwiMesh);
   if (type == 3)
        tempEnemy->setMesh(appleMesh);
   if (type == 4)
        tempEnemy->setMesh(appleMesh);
   if (type == 5)
        tempEnemy->setMesh(bladeMesh);
   tempEnemy->setShader(Diffuse);
   tempEnemy->setTranslation(SVector3((x+(x+1))/2, (y+(y+1))/2, 0));
   tempEnemy->setRotation(SVector3(-90, 0, 0));
   tempEnemy->setScale(SVector3(1, 1, 1));
   blockMap[(int)x+25][(int)(y-0.5+25)].o = true;
   blockMap[(int)x+25][(int)(y-0.5+25)].r = tempEnemy;
   blockMap[(int)x+25][(int)(y-0.5+25)].p = tempPlaceable;
   blockMap[(int)x+25][(int)(y-0.5+25)].mapX = (int)x+25;
   blockMap[(int)x+25][(int)(y-0.5+25)].mapY = (int)(y-0.5+25);
   Application.getSceneManager().addSceneObject(tempEnemy);
   redo.clear();
   redoPlaceables.clear();
}

void CLWIBState::PrepCabbage(float x, float y) {

   if(x < -25 || y < -25 || x >= 200 || y >= 75)
      return;
   if(blockMap[(int)x+25][(int)(y-0.5+25)].o) {
      printf("Blockmap space occupied. Did not place Cabbage\n");
      return;
   }

   printf("Placed cabbage starting at %0.2f, %0.2f\n", x, y);
   CMeshSceneObject *tempCabbage;
   CCabbage *tempPlaceable;
   blocks.push_back(tempCabbage = new CMeshSceneObject());
   placeables.push_back(tempPlaceable = new CCabbage(x, y, 1, 1));
   tempCabbage->setMesh(cabbageMesh);
   tempCabbage->setShader(Diffuse);
   tempCabbage->setTranslation(SVector3((x+(x+1.5f))/2, (y+(y))/2, 0));
   tempCabbage->setRotation(SVector3(-90, 0, 0));
   tempCabbage->setScale(SVector3(0.5, 0.5, 0.5));
   blockMap[(int)x+25][(int)(y-0.5+25)].o = true;
   blockMap[(int)x+25][(int)(y-0.5+25)].r = tempCabbage;
   blockMap[(int)x+25][(int)(y-0.5+25)].p = tempPlaceable;
   blockMap[(int)x+25][(int)(y-0.5+25)].mapX = (int)x+25;
   blockMap[(int)x+25][(int)(y-0.5+25)].mapY = (int)(y-0.5+25);
   Application.getSceneManager().addSceneObject(tempCabbage);
   redo.clear();
   redoPlaceables.clear();
}

void CLWIBState::PrepBlock(float x, float y, int w, int h, int d, int t) {
   if(x < -25 || y < -25 || x >= 200 || y >= 75)
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
   CMeshSceneObject *tempBlock;
   CBlock *tempPlaceable;
   blocks.push_back(tempBlock = new CMeshSceneObject());
   placeables.push_back(tempPlaceable = new CBlock(x, y, w, h, d, t));
   tempBlock->setMesh(cubeMesh);
   if (textureType == -5) {
       tempBlock->setTexture("Textures/GrassyGrass.bmp");
        printf("loaded groundBlockmesh\n");
   }
   if (t == 0)
        tempBlock->setTexture("Textures/grass.bmp");
   else if (t == 1)
        tempBlock->setTexture("Textures/dirt.bmp");
   else if (t == 2)
        tempBlock->setTexture("Textures/rock.bmp");
   else
       printf("texture doesn't exist\n");
   tempBlock->setShader(DiffuseTexture);
   tempBlock->setTranslation(SVector3((x+(x+w))/2, (y+(y+h))/2, 0));
   tempBlock->setScale(SVector3((float) w, (float) h, (float) d));
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
   Application.getSceneManager().addSceneObject(tempBlock);
   redo.clear();
   redoPlaceables.clear();
   if(mDown) {
      tempPlaceable->isMovingPlatform = 1;
      tempPlaceable->Range = 2;
      tempPlaceable->Speed = 1;
   }
}

void CLWIBState::PrepGrass(float x, float y, float w, float h) {
   CMeshSceneObject *tempBlock;
   // block
   blocks.push_back(tempBlock = new CMeshSceneObject());
   tempBlock->setMesh(cubeMesh);

   tempBlock->setTexture("Textures/sky.bmp");
   tempBlock->setShader(DiffuseTexture);
   tempBlock->setTranslation(SVector3((x+(x+w))/2, (y+(y+h))/2, 0));
   tempBlock->setScale(SVector3(w, h, .5));
   Application.getSceneManager().addSceneObject(tempBlock);
   CMeshSceneObject *tempyBlock;
   blocks.push_back(tempyBlock = new CMeshSceneObject());
   tempyBlock->setMesh(cubeMesh);

   tempyBlock->setTexture("Textures/sky.bmp");
   tempyBlock->setShader(DiffuseTexture);
   tempyBlock->setTranslation(SVector3(172, (y+(y+h))/2, 0));
   tempyBlock->setScale(SVector3(w, h, .5));
   Application.getSceneManager().addSceneObject(tempyBlock);

}

void CLWIBState::PrepSky() {

   CMeshSceneObject *tempBlock;
   blocks.push_back(tempBlock = new CMeshSceneObject());
   tempBlock->setMesh(cubeMesh);
   tempBlock->setTexture("Textures/sky.bmp");
   tempBlock->setShader(DiffuseTexture);
   tempBlock->setTranslation(SVector3(0, 24, -2.5));
   tempBlock->setScale(SVector3(400, 50, 1));
   tempBlock->setCullingEnabled(false);
   Application.getSceneManager().addSceneObject(tempBlock);
}


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
         PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, 1));
         return;
      }
      if(Event.Pressed && Event.Type.Value == SMouseEvent::EType::Click && gDown) {
         if(blockHeight < 10)
            blockHeight++;
         PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, 1));
         return;
      }
      if(Event.Pressed && Event.Type.Value == SMouseEvent::EType::Click) {
         mouseDown = 1;
         if(!tDown && twoDown && !oneDown) {
            printf("Here\n");
            PrepEnemy(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY),enemyType);
         }
         if(!tDown && oneDown && !twoDown) {
             PrepCabbage(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY));
         }
         else if (!tDown && !oneDown && !twoDown) {
            PrepBlock(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY), blockWidth, blockHeight, blockDepth,textureType);
         }
         else {
            if(lastMouseOveredBlock.o) {
               Application.getSceneManager().removeSceneObject(lastMouseOveredBlock.r);
               placeables.erase(std::remove(placeables.begin(), placeables.end(), lastMouseOveredBlock.p), placeables.end());
               blocks.erase(std::remove(blocks.begin(), blocks.end(), lastMouseOveredBlock.r), blocks.end());
               //redoPlaceables.push_back(lastMouseOveredBlock.p);
               //redo.push_back(lastMouseOveredBlock.r);
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
         if(x < -25 || y < -25 || x >= 200 || y >= 75)
            return;
         if(oldx == x && oldy == y)
            return;
         qd m_qd;
         m_qd = blockMap[(int)x+25][(int)(y-0.5+25)];
         if(tDown) {
            if(m_qd.o && m_qd.r != lastMouseOveredBlock.r) {
               m_qd.r->setShader(DiffuseTextureBright);
            }
         }
         if(lastMouseOveredBlock.o && m_qd.r != lastMouseOveredBlock.r) {
            lastMouseOveredBlock.r->setShader(DiffuseTexture);
         }
         if(!tDown && !twoDown && mouseDown) {
            PrepBlock(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY), blockWidth, blockHeight, blockDepth, textureType);
         }
         lastMouseOveredBlock = m_qd;
      }
   }
   /*else if(Event.Button.Value == SMouseEvent::EButton::Right) {
      if(Event.Pressed && fDown) {
         if(blockWidth > 1)
            blockWidth--;
         PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, 1));
         return;
      }
      if(Event.Pressed && gDown) {
         if(blockHeight > 1)
            blockHeight--;
         PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, 1));
         return;
      }
   }*/
}

  

void CLWIBState::stepCamera(float delta) {
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
   if(!tDown && !twoDown && mouseDown) {
      PrepBlock(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY), blockWidth, blockHeight, blockDepth,textureType);
   }
}

