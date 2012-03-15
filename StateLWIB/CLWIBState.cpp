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
CMeshSceneObject *PreviewBlock, *PreviewEnemy, *PreviewCabbage, *PreviewGround, *PreviewFlag, *PreviewItem;

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
   //printf("asdf\n");
   clickDown = 0;
   cabbageFlag = 0; // for cabbage
   xCabbage = 0; // for cabbage
   yCabbage = 0;// for cabbage
   textureType = 0;
   enemyType = 0;
   itemType = 0;
   change = 0; //this determines
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

   DeferredDiffuse = CShaderLoader::loadShader("Deferred\Diffuse");
   DeferredTexture = CShaderLoader::loadShader("Deferred\Textured");
   
   float const LightBrightness = 1.0f;
   Application.getSceneManager().Lights.push_back(new CLight()); 
   Application.getSceneManager().Lights.back()->Color = SVector3(LightBrightness);
   Application.getSceneManager().Lights.back()->Position = SVector3(-5.f, 200.f, 500.f);
   //Load the meshes into VBOs

   srand((unsigned int) time(0));

   //Initialize Fxns
   BlocksInit();
   PrepPreviews();
   prepText();
   prepHud();
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
   PreviewFlag->setTranslation(SVector3(x+0.5f,y+0.5f, 0));
   PreviewItem->setTranslation(SVector3(x+0.5f,y+0.5f, 0));
   if(clickDown) {
      PreviewItem->setVisible(false);
      PreviewFlag->setVisible(false);
      PreviewCabbage->setVisible(false); 
      PreviewBlock->setVisible(false);
      PreviewEnemy->setVisible(false);
   }
   else if(tDown) {
      PreviewItem->setVisible(false);
      PreviewFlag->setVisible(false);
      PreviewCabbage->setVisible(false); 
      PreviewBlock->setVisible(false);
      PreviewEnemy->setVisible(false);
   }
   else if(twoDown) {
      PreviewItem->setVisible(false);
      PreviewFlag->setVisible(false);
      PreviewCabbage->setVisible(false); 
      PreviewEnemy->setVisible(true);
      PreviewBlock->setVisible(false);
   }
   else if(oneDown) {
   
      PreviewItem->setVisible(false);
      PreviewFlag->setVisible(false);
      PreviewCabbage->setVisible(true); 
      PreviewEnemy->setVisible(false);
      PreviewBlock->setVisible(true);
   }
   else if(threeDown) {
      PreviewItem->setVisible(false);
      PreviewFlag->setVisible(true);
      PreviewCabbage->setVisible(false); 
      PreviewEnemy->setVisible(false);
      PreviewBlock->setVisible(false);
    
   }
   else if(fourDown) {
      PreviewItem->setVisible(true);
      PreviewFlag->setVisible(false);
      PreviewCabbage->setVisible(false); 
      PreviewBlock->setVisible(false);
      PreviewEnemy->setVisible(false);
   }
   else {
      PreviewItem->setVisible(false);
      PreviewFlag->setVisible(false);
      PreviewCabbage->setVisible(false); 
      PreviewBlock->setVisible(true);
      PreviewEnemy->setVisible(false);
   }


   //Application.getSceneManager().drawAll();

   //Draw Text
   if (!showHelp)
       //freetype::print(our_font, (float)WindowWidth/2, (float)WindowHeight/2, "+");

   if (showHelp) {
       block3->setVisible(false);
       block2->setVisible(false);
       block1->setText("You add blocks by defualt\n"\
           "press Z/X change block type\n"\
           "Press E to place enemies press Z/X add different enemies\n"\
           "Press F/G to make blocks shorter/longer\n"\
           "Press H/J to make blocks skinny/wider\n"\
           "press R to Redo action\n");
   }
   else
       help->setText("Press F1 For Help");
   if (!twoDown && !showHelp && !tDown && !oneDown && !threeDown && !fourDown) {
       block2->setVisible(true);
       block1->setText("Placing Block");
       if (cDown == 0) {
           block3->setVisible(false);
           if (textureType == 0) {
               block2->setText("Placing grass block");
           }
           if (textureType == 1) {
               block2->setText("Placing dirt block");
           }
           if (textureType == 2) {
               block2->setText("Placing rock block");
           }

           if (textureType == -5) {
               block2->setText("Placing ground block");
           }
       }
       if (cDown == 1) {
           block3->setVisible(true);
           block2->setText("changing block width\n");
       }
       if (cDown == 2) {
           block3->setVisible(true);
           block2->setText("changing block height\n");
       }
       if (cDown == 3) {
           block3->setVisible(true);
           block2->setText("changing block depth\n");
       }
   }
   if (twoDown && !showHelp && !tDown && !oneDown && !threeDown&& !fourDown) {
       block3->setVisible(false);
       block2->setVisible(true);
       block1->setText("Placing enemy");
       if (enemyType == 0) {
           block2->setText("Placing Apple\n");
           PreviewEnemy->setMesh(appleMesh);
       }
       if (enemyType == 1) {
           block2->setText("Placing Orange\n");
           PreviewEnemy->setMesh(orangeMesh);
       }
       if (enemyType == 2) {
           block2->setText("Placing Kiwi\n");
           PreviewEnemy->setMesh(kiwiMesh);
       }

       if (enemyType == 3) {
           block2->setText("Placing Grape\n");
           PreviewEnemy->setMesh(cubeMesh);
       }

       if (enemyType == 4) {
           block2->setText("Placing Flame\n");
           PreviewEnemy->setMesh(cubeMesh);
       }

       if (enemyType == 5) {
           block2->setText("Placing Blade\n");
           PreviewEnemy->setMesh(bladeMesh);
       }
   }
   if (oneDown && !showHelp && !tDown && !twoDown && !threeDown&& !fourDown) {
       block3->setVisible(false);
       block2->setVisible(false);
       block1->setText("Insert Cabbage");
       PreviewCabbage->setMesh(cabbageMesh);
   }
   if (threeDown && !showHelp && !tDown && !twoDown && !oneDown&& !fourDown) {
       block3->setVisible(false);
       block2->setVisible(false);
       block1->setText("Insert flag");
       PreviewCabbage->setMesh(flagMesh);
   }
   if (!threeDown && !showHelp && !tDown && !twoDown && !oneDown && fourDown) {
        block3->setVisible(false);
        block2->setVisible(true);
        block1->setText("Insert item");
        if (itemType == 0)  {// health
            block2->setText("Adding Health");
            PreviewItem->setMesh(health);
        }
        else if (itemType == 1) { // energy
            block2->setText("Adding Energy");
            PreviewItem->setMesh(energy);
        }
        else if (itemType == 2) {// life
            block2->setText("Adding life");
        }
        else if (itemType == 3) // powerup
            block2->setText("Adding powerup");
   }
   if (tDown && !showHelp ){
       block1->setText("Remove mode");
   }
   //drawSubWindow();
   pickInsert();
   Application.getSceneManager().drawAll();

   Application.getGUIEngine().drawAll(); 
   Application.getSceneManager().endDraw();
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
            change = 1;
            /*if (oneDown == 1)
                oneDown = 0;
            else {
                oneDown = 1;
                threeDown = 0; 
                tDown = 0;
                twoDown = 0;
                fourDown = 0;
                blockWidth = 1;
                blockHeight = 1;
                blockDepth = 1;
                PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
            }*/
        }
        if(Event.Key == SDLK_3){
            change = 3;
            /*if (threeDown == 1)
                threeDown = 0;
            else {
                threeDown = 1;
                oneDown = 0;
                tDown = 0;
                twoDown = 0;
                fourDown = 0;
                blockWidth = 1;
                blockHeight = 1;
                blockDepth = 1;
                PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
            }*/
        }
        if (Event.Key == SDLK_4) {
            change = 4;
            /*if (fourDown == 1)
                fourDown = 0;
            else {
                fourDown = 1;
                threeDown = 0;
                oneDown = 0;
                tDown = 0;
                twoDown = 0;
                blockWidth = 1;
                blockHeight = 1;
                blockDepth = 1;
                PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
            }*/
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
            change = 2;
            /*if (twoDown == 1 ) {
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
                threeDown = 0; 
                tDown = 0;
                oneDown = 0;
                fourDown = 0;
                blockWidth = 1;
                blockHeight = 1;
                blockDepth = 1;
                PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
            }*/
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
                threeDown = 0; 
                fourDown = 0;
            }
        }
        if(Event.Key == SDLK_m){
            mDown = 1; //move
        }
        if(Event.Key == SDLK_c){
            if (!oneDown && !twoDown && !threeDown && !tDown) {
                if(cDown <= 3)
                    cDown++;
                else
                    cDown = 0;
            }
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
            } else if (fourDown) {
                if (itemType < 2) 
                    itemType++;
                else
                    itemType = 0;
            } else if (!oneDown && !threeDown && !tDown && !fourDown){
                if (cDown == 0) { 
                    if (textureType < 2  && textureType >= 0 && textureType != 2)
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
            
                if (cDown == 1&& textureType != 2) {
                    if(blockWidth < 10 && textureType != -5)
                        blockWidth++;
                    PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
                }
                if (cDown == 2&& textureType != 2) {
                    if(blockHeight < 10 && textureType != -5)
                        blockHeight++;
                    PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
                }
                if (cDown == 3&& textureType != 2) {
                    if(blockDepth < 6 && textureType != -5)
                        blockDepth++;
                    PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
                }
            }
        } 
        if (Event.Key == SDLK_z) { // subtracting generally
            if (twoDown) {
                if (enemyType != 0) 
                    enemyType--;
                else
                    enemyType = 5;
            } else if (fourDown) {
                if (itemType != 0)
                    itemType--;
                else
                    itemType = 1;
            } else if (!oneDown && !threeDown && !tDown){
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
                if(cDown == 1&& textureType != 2) {
                    if(blockWidth > 1 && textureType != -5) 
                        blockWidth--;
                    PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
                }
                if(cDown == 2&& textureType != 2) {
                    if(blockHeight > 1 && textureType != -5)
                        blockHeight--;
                    PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
                }
                if(cDown == 3&& textureType != 2) {
                    if(blockDepth > 1 && textureType != -5)
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
    while(blocks.size() > 3 && placeables.size() > 0 ) {
        Application.getSceneManager().removeSceneObject(blocks.back());
        blocks.pop_back();
        CPlaceable *m_block = placeables.back();
        placeables.pop_back();

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
    }
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
                PrepBlock((float)x,(float)y,w,h,d,t,moving);
                printf("texture is %d\n", t);
           }
           if(!strcmp("CEnemy", xml->getNodeName()))
           {
              x = xml->getAttributeValueAsInt(0);
              y = xml->getAttributeValueAsInt(1);
              t = xml->getAttributeValueAsInt(4);
              PrepEnemy((float)x,(float)y,t);
           }
           if(!strcmp("CCabbage", xml->getNodeName()))
           {
              x = xml->getAttributeValueAsInt(0);
              y = xml->getAttributeValueAsInt(1);
              PrepCabbage((float)x,(float)y);
           }
           if(!strcmp("CFlag", xml->getNodeName()))
           {
              x = xml->getAttributeValueAsInt(0);
              y = xml->getAttributeValueAsInt(1);
              PrepFlag((float)x,(float)y);
           }
           if (!strcmp("CPItem", xml->getNodeName()))
           {
                x = xml->getAttributeValueAsInt(0);
                y = xml->getAttributeValueAsInt(1);
                t = xml->getAttributeValueAsInt(2);
                PrepItem((float)x, (float)y, t);
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
   Application.getGUIEngine().removeAllWidgets(); 
   //our_font.clean();
   placeables.clear();
   redoPlaceables.clear();
   blocks.clear();
   redo.clear();
   Application.getSceneManager().removeAllSceneObjects();
}

void CLWIBState::PrepPreviews() {
   blocks.push_back(PreviewBlock = new CMeshSceneObject());
   PreviewBlock->setMesh(cubeMesh);
   PreviewBlock->setTexture("Colors/white.bmp");
   PreviewBlock->setShader(ERP_DEFAULT, DiffuseTexture);
   PreviewBlock->setShader(ERP_DEFERRED_OBJECTS, DeferredTexture);
   PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));

   blocks.push_back(PreviewFlag = new CMeshSceneObject());
   blocks.push_back(PreviewCabbage = new CMeshSceneObject());
   blocks.push_back(PreviewItem = new CMeshSceneObject());
   blocks.push_back(PreviewEnemy = new CMeshSceneObject());
   appleMesh = CMeshLoader::load3dsMesh("Base/appleEnemy.3ds");
   orangeMesh = CMeshLoader::load3dsMesh("Base/orange.3ds");
   kiwiMesh = CMeshLoader::load3dsMesh("Base/killerkiwi.3ds");
   cabbageMesh = CMeshLoader::load3dsMesh("Base/crappycabbage.3ds");
   bladeMesh = CMeshLoader::load3dsMesh("Base/trap1.3ds");
   flagMesh = CMeshLoader::load3dsMesh("Base/flag.3ds");
   health = CMeshLoader::load3dsMesh("Base/healthboost.3ds");
   energy = CMeshLoader::load3dsMesh("Base/energyboost.3ds");
   //PreviewCabbage->setMesh(appleMesh);
   
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
   if (cabbageMesh) {
      cabbageMesh->resizeMesh(SVector3(1));
      cabbageMesh->centerMeshByExtents(SVector3(0));
      cabbageMesh->calculateNormalsPerVertex();
   }
   if (flagMesh) {
      flagMesh->resizeMesh(SVector3(1));
      flagMesh->centerMeshByExtents(SVector3(0));
      flagMesh->calculateNormalsPerVertex();
   }
   if (health) {
   health->resizeMesh(SVector3(1));
      health->centerMeshByExtents(SVector3(0));
      health->calculateNormalsPerFace();
   }
   if (energy) {
   energy->resizeMesh(SVector3(1));
      energy->centerMeshByExtents(SVector3(0));
      energy->calculateNormalsPerFace();
   }
   PreviewFlag->setMesh(flagMesh);
   PreviewFlag->setShader(ERP_DEFAULT, Diffuse);
   PreviewFlag->setShader(ERP_DEFERRED_OBJECTS, DeferredDiffuse);
   PreviewFlag->setRotation(SVector3(-90, 0, 0));
   PreviewFlag->setScale(SVector3(0.5,0.5, 0.5));
   //
   PreviewCabbage->setMesh(cabbageMesh);
   PreviewCabbage->setShader(ERP_DEFAULT, Diffuse);
   PreviewCabbage->setShader(ERP_DEFERRED_OBJECTS, DeferredDiffuse);
   PreviewCabbage->setRotation(SVector3(90, 0, 0));
   PreviewCabbage->setScale(SVector3(.0150f, .00025f,.0016f));
   //
   
   PreviewEnemy->setShader(ERP_DEFAULT, Diffuse);
   PreviewEnemy->setShader(ERP_DEFERRED_OBJECTS, DeferredDiffuse);
   PreviewEnemy->setRotation(SVector3(-90, 0, 0));
   PreviewBlock->setScale(SVector3(1, 1, 1));
   //
   PreviewItem->setMesh(health);
   PreviewItem->setShader(ERP_DEFAULT, Diffuse);
   PreviewItem->setShader(ERP_DEFERRED_OBJECTS, DeferredDiffuse);
   PreviewItem->setRotation(SVector3(-90, 0, 0));
   PreviewItem->setScale(SVector3(1, 1, 1));

   CApplication::get().getSceneManager().addSceneObject(PreviewItem);
   CApplication::get().getSceneManager().addSceneObject(PreviewBlock);
   CApplication::get().getSceneManager().addSceneObject(PreviewFlag);
   CApplication::get().getSceneManager().addSceneObject(PreviewEnemy);
   CApplication::get().getSceneManager().addSceneObject(PreviewCabbage);
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

void CLWIBState::PrepItem(float x, float y, int item) {

   if(x < -25 || y < -25 || x >= 200 || y >= 75)
      return;
   if(blockMap[(int)x+25][(int)(y-0.5+25)].o) {
      printf("Blockmap space occupied. Did not place Cabbage\n");
      return;
   }

   printf("Placed item starting at %0.2f, %0.2f\n", x, y);
   CMeshSceneObject *tempItem;
   CPItem *tempPlaceable;
   blocks.push_back(tempItem = new CMeshSceneObject());
   placeables.push_back(tempPlaceable = new CPItem(x, y, item));
   if (item == 0)
       tempItem->setMesh(health);
   if (item == 1)
       tempItem->setMesh(energy);
   tempItem->setShader(ERP_DEFAULT, Diffuse);
   tempItem->setShader(ERP_DEFERRED_OBJECTS, DeferredDiffuse);
   tempItem->setTranslation(SVector3((x+(x + 1))/2, (y+(y + 1))/2, 0));
   tempItem->setRotation(SVector3(-90, 0, 0));
   tempItem->setScale(SVector3(0.5, 0.5, 0.5));
   blockMap[(int)x+25][(int)(y-0.5+25)].o = true;
   blockMap[(int)x+25][(int)(y-0.5+25)].r = tempItem;
   blockMap[(int)x+25][(int)(y-0.5+25)].p = tempPlaceable;
   blockMap[(int)x+25][(int)(y-0.5+25)].mapX = (int)x+25;
   blockMap[(int)x+25][(int)(y-0.5+25)].mapY = (int)(y-0.5+25);
   Application.getSceneManager().addSceneObject(tempItem);
   redo.clear();
   redoPlaceables.clear();

}

void CLWIBState::PrepFlag(float x, float y) {

   if(x < -25 || y < -25 || x >= 200 || y >= 75)
      return;
   if(blockMap[(int)x+25][(int)(y-0.5+25)].o) {
      printf("Blockmap space occupied. Did not place Cabbage\n");
      return;
   }

   printf("Placed flag starting at %0.2f, %0.2f\n", x, y);
   CMeshSceneObject *tempFlag;
   CFlag *tempPlaceable;
   blocks.push_back(tempFlag = new CMeshSceneObject());
   placeables.push_back(tempPlaceable = new CFlag(x, y, 1, 1));
   tempFlag->setMesh(flagMesh);
   tempFlag->setShader(ERP_DEFAULT, Diffuse);
   tempFlag->setShader(ERP_DEFERRED_OBJECTS, DeferredDiffuse);
   tempFlag->setTranslation(SVector3((x+(x + 1))/2, (y+(y + 1))/2, 0));
   tempFlag->setRotation(SVector3(-90, 0, 0));
   tempFlag->setScale(SVector3(0.5, 0.5, 0.5));
   blockMap[(int)x+25][(int)(y-0.5+25)].o = true;
   blockMap[(int)x+25][(int)(y-0.5+25)].r = tempFlag;
   blockMap[(int)x+25][(int)(y-0.5+25)].p = tempPlaceable;
   blockMap[(int)x+25][(int)(y-0.5+25)].mapX = (int)x+25;
   blockMap[(int)x+25][(int)(y-0.5+25)].mapY = (int)(y-0.5+25);
   Application.getSceneManager().addSceneObject(tempFlag);
   redo.clear();
   redoPlaceables.clear();

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
   tempEnemy->setShader(ERP_DEFAULT, Diffuse);
   tempEnemy->setShader(ERP_DEFERRED_OBJECTS, DeferredDiffuse);
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

qd lastCabbage = blockMap[0][0];

void CLWIBState::PrepCabbage(float x, float y) {

    if(x < -25 || y < -25 || x >= 200 || y >= 75)
        return;
    if(blockMap[(int)x+25][(int)(y-0.5+25)].o) {
        printf("Blockmap space occupied. Did not place Cabbage\n");
        return;
    }

    if (cabbageFlag == 1) {
        blockMap[(int)xCabbage+25][(int)(yCabbage-0.5+25)].o = false;
        blockMap[(int)xCabbage+25][(int)(yCabbage-0.5+25)].r = NULL;
        blockMap[(int)xCabbage+25][(int)(yCabbage-0.5+25)].p = NULL;
        blockMap[(int)xCabbage+25][(int)(yCabbage-0.5+25)].mapX = -1;
        blockMap[(int)xCabbage+25][(int)(yCabbage-0.5+25)].mapY = -1;
        printf("tempx = %d, tempy =  %d\n",xCabbage ,yCabbage);
        placeables.erase(std::remove(placeables.begin(), placeables.end(), lastCabbage.p), placeables.end());
        blocks.erase(std::remove(blocks.begin(), blocks.end(), lastCabbage.r), blocks.end());
        Application.getSceneManager().removeSceneObject(lastCabbage.r);
    } 

    printf("Placed cabbage starting at %0.2f, %0.2f\n", x, y);
    CMeshSceneObject *tempCabbage;
    CCabbage *tempPlaceable;
    blocks.push_back(tempCabbage = new CMeshSceneObject());
    placeables.push_back(tempPlaceable = new CCabbage(x, y, 1, 1));
    tempCabbage->setMesh(cabbageMesh);
    tempCabbage->setShader(ERP_DEFAULT, Diffuse);
    tempCabbage->setShader(ERP_DEFERRED_OBJECTS, DeferredDiffuse);
    tempCabbage->setTranslation(SVector3((x+(x + 1))/2, (y+(y + 1))/2, 0));
    tempCabbage->setRotation(SVector3(-90, 0, 0));
    tempCabbage->setScale(SVector3(0.5, 0.5, 0.5));
    blockMap[(int)x+25][(int)(y-0.5f+25)].o = true;
    blockMap[(int)x+25][(int)(y-0.5f+25)].r = tempCabbage;
    blockMap[(int)x+25][(int)(y-0.5f+25)].p = tempPlaceable;
    blockMap[(int)x+25][(int)(y-0.5f+25)].mapX = (int)x+25;
    blockMap[(int)x+25][(int)(y-0.5f+25)].mapY = (int)(y-0.5f+25);
    xCabbage = (int) x, yCabbage = (int) y;

    lastCabbage = blockMap[(int)x+25][(int)(y-0.5+25)];
    Application.getSceneManager().addSceneObject(tempCabbage);
    cabbageFlag = 1;
    redo.clear();
    redoPlaceables.clear();
}

void CLWIBState::PrepBlock(float x, float y, int w, int h, int d, int t, int moving) {
   if(x < -25 || y < -25 || x >= 200 || y >= 75)
      return;
   if(t == -5 && (int)y != -5)
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
   placeables.push_back(tempPlaceable = new CBlock(x, y, w, h, d, t, moving));
   tempBlock->setMesh(cubeMesh);
   if (t == 0)
        tempBlock->setTexture("Base/grass.bmp");
   else if (t == 1)
        tempBlock->setTexture("Base/dirt.bmp");
   else if (t == 2)
        tempBlock->setTexture("Base/rock.bmp");
   else if (t == -5)
        tempBlock->setTexture("Base/GrassyGrass.bmp");
   else
       printf("texture doesn't exist\n");
   tempBlock->setShader(ERP_DEFAULT, DiffuseTexture);
   tempBlock->setShader(ERP_DEFERRED_OBJECTS, DeferredTexture);
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
   if(moving) {
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

   tempBlock->setTexture("Base/sky.bmp");
   tempBlock->setShader(ERP_DEFAULT, DiffuseTexture);
   tempBlock->setShader(ERP_DEFERRED_OBJECTS, DeferredTexture);
   tempBlock->setTranslation(SVector3((x+(x+w))/2, (y+(y+h))/2, 0));
   tempBlock->setScale(SVector3(w, h, .5));
   Application.getSceneManager().addSceneObject(tempBlock);
   CMeshSceneObject *tempyBlock;
   blocks.push_back(tempyBlock = new CMeshSceneObject());
   tempyBlock->setMesh(cubeMesh);

   tempyBlock->setTexture("Base/sky.bmp");
   tempyBlock->setShader(ERP_DEFAULT, DiffuseTexture);
   tempyBlock->setShader(ERP_DEFERRED_OBJECTS, DeferredTexture);
   tempyBlock->setTranslation(SVector3(172, (y+(y+h))/2, 0));
   tempyBlock->setScale(SVector3(w, h, .5));
   Application.getSceneManager().addSceneObject(tempyBlock);

}

void CLWIBState::PrepSky() {

   CMeshSceneObject *tempBlock;
   blocks.push_back(tempBlock = new CMeshSceneObject());
   tempBlock->setMesh(cubeMesh);
   tempBlock->setTexture("Base/sky.bmp");
   tempBlock->setShader(ERP_DEFAULT, DiffuseTexture);
   tempBlock->setShader(ERP_DEFERRED_OBJECTS, DeferredTexture);
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
    if(clickDown)
        return;
   if(Event.Button.Value == SMouseEvent::EButton::Left) {
      if(Event.Pressed && Event.Type.Value == SMouseEvent::EType::Click) {
         mouseDown = 1;
         if(!tDown && twoDown && !oneDown && !threeDown && !fourDown) {
            PrepEnemy(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY),enemyType);
         }
         if(!tDown && !twoDown && !oneDown && threeDown&& !fourDown) {
            PrepFlag(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY));
         }
         if(!tDown && oneDown && !threeDown && !twoDown&& !fourDown) {
             PrepCabbage(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY));
         }
         else if (!tDown && !oneDown && !threeDown &&!twoDown && !fourDown) {
            PrepBlock(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY), blockWidth, blockHeight, blockDepth,textureType,mDown);
         }
         else if (!tDown && !oneDown && !threeDown &&!twoDown && fourDown) {
            PrepItem(round(eye.X + previewBlockMouseX),round(eye.Y + previewBlockMouseY),itemType);
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
         CShader *tempShader;
         m_qd = blockMap[(int)x+25][(int)(y-0.5+25)];
         tempShader = CShaderLoader::loadShader("DiffuseTexture");
         if(tDown) {
            if(m_qd.o && m_qd.r != lastMouseOveredBlock.r) {

                tempShader = m_qd.r->getShader(); 
                m_qd.r->setShader(ERP_DEFAULT, DiffuseTextureBright);
            }
            if(lastMouseOveredBlock.o && m_qd.r != lastMouseOveredBlock.r) {
                lastMouseOveredBlock.r->setShader(ERP_DEFAULT, tempShader);
            }
         }
         if(!threeDown && !tDown && !twoDown && mouseDown) {
            PrepBlock(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY), blockWidth, blockHeight, blockDepth, textureType,mDown);
         }
         lastMouseOveredBlock = m_qd;
      }
   }
}

void CLWIBState::prepText() {
    
    SColor FontColor(1.0f, 1.0f, 1.0f);
    help = new CGUIFontWidget("WIFFLES_.TTF", 30.f);
    help->setText("Press F1 for help\n");
    help->setPosition(SVector2(.05f, .95f));
    help->setColor(FontColor);
    help->setVisible(true);

    block1 = new CGUIFontWidget("WIFFLES_.TTF", 30.f);
    block1->setText("Inserting block\n");
    block1->setPosition(SVector2(.05f, .9f));
    block1->setColor(FontColor);
    block1->setVisible(true);
    
    block2 = new CGUIFontWidget("WIFFLES_.TTF", 30.f);
    block2->setText("grassTexture\n");
    block2->setPosition(SVector2(.05f, .85f));
    block2->setColor(FontColor);
    block2->setVisible(true);

    block3 = new CGUIFontWidget("WIFFLES_.TTF", 30.f);
    block3->setText("block width is\n");
    block3->setPosition(SVector2(.05f, .80f));
    block3->setColor(FontColor);
    block3->setVisible(true);

    Application.getGUIEngine().addWidget(block1);
    Application.getGUIEngine().addWidget(block2);
    Application.getGUIEngine().addWidget(block3);
    Application.getGUIEngine().addWidget(help);
 
}

void CLWIBState::OnWidgetHover(CGUIWidget *widget) {
    //std::cout << "Widget hovered! " << widget << std::endl;
    if(widget) 
      clickDown = 1;
}
void CLWIBState::OnWidgetUnHover(CGUIWidget *widget) {
    //std::cout << "Widget unhovered! " << Widget << std::endl;
    if (widget)
        clickDown = 0;
}


void CLWIBState::OnWidgetClick(CGUIWidget *widget) {
    if (widget == leftArrow) {
        if (change == -1)
            change = 4;
        else
            change--;
    }
    if (widget == rightArrow){
        if (change == 4)
            change = 0;
        else
            change++;
    }
}


void CLWIBState::pickInsert()
{
    if (change == 0) {
        oneDown = 0;
        threeDown = 0; 
        tDown = 0;
        twoDown = 0;
        fourDown = 0;
       /* blockWidth = 1;
        blockHeight = 1;
        blockDepth = 1;*/
        PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
    }
    if (change == 1) {
        oneDown = 1;
        threeDown = 0; 
        tDown = 0;
        twoDown = 0;
        fourDown = 0;
        blockWidth = 1;
        blockHeight = 1;
        blockDepth = 1;
        PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
    }
    if (change == 2) {
      twoDown = 1; //enemy
                threeDown = 0; 
                tDown = 0;
                oneDown = 0;
                fourDown = 0;
                blockWidth = 1;
                blockHeight = 1;
                blockDepth = 1;
                PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
    }
    if (change == 3) {
        threeDown = 1;
        oneDown = 0;
        tDown = 0;
        twoDown = 0;
        fourDown = 0;
        blockWidth = 1;
        blockHeight = 1;
        blockDepth = 1;
        PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
    }
    if (change == 4) {
        fourDown = 1;
        threeDown = 0;
        oneDown = 0;
        tDown = 0;
        twoDown = 0;
        blockWidth = 1;
        blockHeight = 1;
        blockDepth = 1;
        PreviewBlock->setScale(SVector3((float) blockWidth, (float) blockHeight, (float) blockDepth));
    }
    
}
void CLWIBState::prepHud() {
    // prepping hud wwidgest
    SVector2 norm = SVector2(.1f, .1f);
    CTexture *imgLeft = new CTexture(CImageLoader::loadImage("ModelImages/leftArrowPic.bmp"));
    CTexture *imgright = new CTexture(CImageLoader::loadImage("ModelImages/rightArrowPic.bmp"));
    CTexture *grass = new CTexture(CImageLoader::loadImage("Base/grass.bmp"));
    CTexture *dirt = new CTexture(CImageLoader::loadImage("Base/dirt.bmp"));
    CTexture *rock = new CTexture(CImageLoader::loadImage("Base/rock.bmp"));
    //CTexture *imgright = new CTexture(CImageLoader::loadImage("ModelImages/rightArrowPic.bmp"));
    //arrows to cycle though the blocks, enemies, cabbage, flag etc.
    leftArrow = new CGUIImageWidget(imgLeft, norm);
    leftArrow->setPosition(SVector2(1.05f, .85f));

    rightArrow = new CGUIImageWidget(imgright, norm);
    rightArrow->setPosition(SVector2(1.20f, .85f));
    // cabbage button
    //cabbage = new CGUIImageWidget();

    //flag = new CGUIImageWidget();
    //blocks buttons
    grassBlock = new CGUIImageWidget(grass,norm);
    grassBlock->setPosition(SVector2(1.05f, .70f));
    dirtBlock = new CGUIImageWidget(dirt,norm);
    dirtBlock->setPosition(SVector2(1.20f, .70f));
    rockBlock = new CGUIImageWidget(rock,norm);
    rockBlock->setPosition(SVector2(1.05f, .55f));
    //groundBlock = new CGUIImageWidget(test2,norm);
    //grassBlock = new CGUIImageWidget(test2,norm);

   // grassBlock->setPosition(SVector2(1.15f, 1.0f));

    //rockBlock = new CGUIImageWidget(test,SVector2(.1f, .1f));

   // grassBlock->setPosition(SVector2(1.15f, .5f));

    //groundBlock = new CGUIImageWidget(test,SVector2(.5f, .5f));

    //dirtBlock = new CGUIImageWidget(test,SVector2(.5f, .5f));

    /*appleE =new CGUIImageWidget() ;

    orangeE =new CGUIImageWidget() ;

    kiwiE = new CGUIImageWidget();

    fireE = new CGUIImageWidget();

    bladeE = new CGUIImageWidget();

    cycleLeft = new CGUIImageWidget();

    cycleRight = new CGUIImageWidget();*/
    //adding widgets to game 
    Application.getGUIEngine().addWidget(grassBlock);
    Application.getGUIEngine().addWidget(dirtBlock);
    Application.getGUIEngine().addWidget(rockBlock);
    Application.getGUIEngine().addWidget(leftArrow);
    Application.getGUIEngine().addWidget(rightArrow);
    //Application.getGUIEngine().addWidget(grassBlock);
    //Application.getGUIEngine().addWidget(rockBlock);
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
      PrepBlock(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY), blockWidth, blockHeight, blockDepth,textureType,mDown);
   }
}

