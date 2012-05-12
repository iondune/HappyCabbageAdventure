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

float previewBlockMouseX, previewBlockMouseY; 
float lastBlockPlacedLocationX, lastBlockPlacedLocationY;


CLWIBState::CLWIBState()
: Application (CApplication::get())
{}

void CLWIBState::BlocksInit( void ) {
  // PrepGrass(-25, -1, 1, 1);
   PrepSky();

   SRect2 area;
}

struct quickndirty {
   CGameplayElement *r;
   bool o;
   int mapX,mapY;
} typedef qd;

void initBlockMap();

//Minblockvalue = -25
//Heightoffset = 0.5
qd blockMap[500][100]; //225, 100

void initBlockMap() {
   int i,j;
   for(i=0; i<500; i++)
      for(j=0; j<100; j++) {
         blockMap[i][j].o = false;
         blockMap[i][j].r = NULL;
         blockMap[i][j].mapX = -1;
         blockMap[i][j].mapY = -1;
      }
}
//Initalizer fxn
int pastX; int pastY;
void CLWIBState::begin()
{
    pastX = 0;
    pastY = 0;
   blockFlag = 0; 
   clickDown = 0;
   uniType = 0;
   flagFlag = 0;
   cabbageFlag = 0; // for cabbage
   xCabbage = 0; // for cabbage
   yCabbage = 0;// for cabbage
   friendType = 0;
   dayNight = 0;
   env = 0;
   change = 0; //this determines
   aDown = dDown = spaceDown = wDown = sDown = gDown = fDown = tDown = eDown = mDown = oneDown = twoDown = threeDown = cDown = 0;

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

   eye = SVector3(-19.f,0.f,10.f);
   look = SVector3(-19.f,0.f,-1.f);
   Camera = new CPerspectiveCamera((float)WindowWidth/(float)WindowHeight, 0.01f, 100.f, 60.f);
   Camera->setPosition(eye);
   Camera->setLookDirection(look - eye);
   Camera->recalculateViewMatrix();
   Application.getSceneManager().setActiveCamera(Camera);

   float const LightBrightness = 1.0f;
   Application.getSceneManager().Lights.push_back(new CLight()); 
   Application.getSceneManager().Lights.back()->Color = SVector3(LightBrightness);
   Application.getSceneManager().Lights.back()->Position = SVector3(-5.f, 200.f, 500.f);
   //Load the meshes into VBOs

   srand((unsigned int) time(0));
   PreviewBlock = new CElementBlock(SRect2(1,1,1,1),1,1);
   PreviewBlock->setupObjects();
   //Initialize Fxns
   BlocksInit();
   
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
   PrepPreviews(x,y, Elapsed);
    //PreviewBlock->

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
           if (uniType == 0) {
               block2->setText("Placing grass block");
               //PreviewBlock = new CElementBlock(SRect2(x,y,blockWidth,blockHeight),blockDepth,uniType);
           }
           if (uniType == 1) {
               block2->setText("Placing dirt block");
               //PreviewBlock = new CElementBlock(SRect2(x,y,blockWidth,blockHeight),blockDepth,uniType);
           }
           if (uniType == 2) {
               block2->setText("Placing rock block");
               //PreviewBlock = new CElementBlock(SRect2(x,y,blockWidth,blockHeight),blockDepth,uniType);
           }

           if (uniType == -5) {
               block2->setText("Placing ground block");
               //PreviewBlock = new CElementBlock(SRect2(x,y,blockWidth,blockHeight),blockDepth,uniType);
           }
           if (uniType >= 3)
               uniType = 0;
       }
       if (cDown == 1) {
           //block3->setVisible(true);
           block2->setText("changing block width\n");
       }
       if (cDown == 2) {
           //block3->setVisible(true);
           block2->setText("changing block height\n");
       }
       if (cDown == 3) {
           //block3->setVisible(true);
           block2->setText("changing block depth\n");
       }
       if (cDown == 4) {
          block2->setText("adding breakableBlocks");
          //PreviewBlock = new CElementBlockBreakable(SRect2(x,y,blockWidth,blockHeight));
       }
       if (cDown == 5){
          block2->setText("adding deathBlocks");
          //PreviewBlock = new CElementBlockDeath(SRect2(x,y,blockWidth,blockHeight),blockDepth,uniType, 1.0f,1.0f);
       }
   }
   if (twoDown && !showHelp && !tDown && !oneDown && !threeDown&& !fourDown) {
       block3->setVisible(false);
       block2->setVisible(true);
       block1->setText("Placing enemy");
       if (uniType == 0) {
           block2->setText("Placing Apple\n");
       }
       if (uniType == 1) {
           block2->setText("Placing Orange\n");
           //PreviewEnemy->setScale(SVector3(1,1,1));
       }
       if (uniType == 2) {
           block2->setText("Placing Kiwi\n");
       }

       if (uniType == 3) {
           block2->setText("Placing Grape\n");
       }

       if (uniType == 4) {
           block2->setText("Placing Flame\n");
       }

       if (uniType == 5) {
           block2->setText("Placing Blade\n");
       }
       
       if (uniType == 6) {
           block2->setText("Placing Giant Apple\n");
       }
       
       if (uniType == 7) {
           block2->setText("Placing Pear\n");
       }
       
       if (uniType == 8) {
           block2->setText("Placing Banana\n");
       }
       
       if (uniType == 9) {
           block2->setText("Placing cherry\n");
       }
       if (uniType == 10) {
          block2->setText("Placing pineapple\n");
       }
       if (uniType == 11) {
          block2->setText("Placing pomegranite\n");
       }
       if (uniType == 12) {
          block2->setText("Placing strawberry\n");
       }
       
       if (uniType >= 13)
           uniType = 0;
   }
   if (oneDown && !showHelp && !tDown && !twoDown && !threeDown&& !fourDown) {
       block3->setVisible(false);
       block2->setVisible(false);
       if (friendType == 0) {
           block1->setText("Insert Cabbage");
       } else if (friendType == 1) {
           block1->setText("Insert derp");
       } else 
           printf("error friend not here");

   }
   if (threeDown && !showHelp && !tDown && !twoDown && !oneDown&& !fourDown) {
       block3->setVisible(false);
       block2->setVisible(true);
       block1->setText("Insert flag");
       if (uniType == 0)
           block2->setText("regular flag");
       if (uniType == 1)
           block2->setText("secret flag");
       if (uniType >= 2)
           uniType = 0;
   }
   if (!threeDown && !showHelp && !tDown && !twoDown && !oneDown && fourDown) {
        block3->setVisible(false);
        block2->setVisible(true);
        block1->setText("Insert item");
        if (uniType == 0)  {// health
            block2->setText("Adding Health");
        }
        else if (uniType == 1) { // energy
            block2->setText("Adding Energy");
        }
        else if (uniType == 2) {// life
            block2->setText("Adding life");
        }
        else if (uniType == 3) { // powerup 
            block2->setText("Adding Seeds");
        }
        else if (uniType >=4)
            uniType = 0;
   }
   if (sixDown) {
        block3->setVisible(true);
        block2->setVisible(true);
        block1->setText("Set enviroment");
        if (dayNight == 0)  {
            block2->setText("Day");
        }
        else if (dayNight == 1) {
            block2->setText("Night");
        }
        if (env == 0) {
            block3->setText("Green Plains");
        }
        else if (env == 1) { 
            block3->setText("Desert");
        }
        else if (env == 2) { 
            block3->setText("mountains");
        }
        else if (env == 3) { 
            block3->setText("Space");
        }
   }
   if (tDown && !showHelp ){
       block1->setText("Remove mode");
       block2->setText("");
       block3->setText("");
   }
   pickInsert();
   changeTiles();
   //PreviewBlock->setupObjects();
   for (int i = 0 ; i < placeables.size(); i++)
       placeables[i]->update(Elapsed);

   Application.getSceneManager().drawAll();

   Application.getGUIEngine().drawAll(); 
   Application.getSceneManager().endDraw();
   SDL_GL_SwapBuffers();
}

//Sends event every time key pressed (also when held)
void CLWIBState::OnKeyboardEvent(SKeyboardEvent const & Event)
{
    if(Event.Pressed){
        if (Event.Key == SDLK_q)
        {
            printf("there is %d amount of placeables in the array\n", placeables.size());
        }
        if(Event.Key == SDLK_1){
            change = 1;
        }
        if(Event.Key == SDLK_3){
            change = 3;
        }
        if (Event.Key == SDLK_4) {
            change = 4;
        }
        if (Event.Key == SDLK_6) {
            change = 6;
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
        }
        if(Event.Key == SDLK_k){
            loadWorld();
        }
        if(Event.Key == SDLK_j){
            //printf("Angle: %d\n", ANGLE(overView, backwardsView));
        }
        if(Event.Key == SDLK_u) {
            removeObject();
        }
        if(Event.Key == SDLK_r) {
            undoObjects();
        }
        if(Event.Key == SDLK_t){
            change = 5;
        }
        if(Event.Key == SDLK_m){
            mDown = 1; //move
        }
        if(Event.Key == SDLK_c){
            if (!oneDown && !twoDown && !threeDown && !tDown) {
                if(cDown <= 5)
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
            if (oneDown) {
                if (friendType < 1)
                    friendType++;
                else
                    friendType = 0;
            } else if (twoDown) {
                if (uniType < 12) //temp constraint
                    uniType++;
                else
                    uniType = 0;
            } else if (threeDown){ 
                if (uniType < 1)
                    uniType++;
                else
                    uniType = 0;
            } else if (fourDown) {
                if (uniType < 3) 
                    uniType++;
                else
                    uniType = 0;
            } else if (!oneDown && !threeDown && !tDown && !fourDown){
                if (cDown == 0) { 
                    if (uniType < 2  && uniType >= 0 && uniType != 2)
                        uniType++;
                    else if (uniType == 2) {
                        uniType = -5;
                        blockWidth = 5;
                        blockHeight = 2;
                        blockDepth = 5;
                    }
                    else if (uniType == -5) {
                        uniType = 0;
                        blockWidth = 1;
                        blockHeight = 1;
                        blockDepth = 1;
                    }
                }
            
                if (cDown == 1&& uniType != 2) {
                    if(blockWidth < 10 && uniType != -5)
                        blockWidth++;
                }
                if (cDown == 2&& uniType != 2) {
                    if(blockHeight < 10 && uniType != -5)
                        blockHeight++;
                }
                if (cDown == 3&& uniType != 2) {
                    if(blockDepth < 6 && uniType != -5)
                        blockDepth++;
                }
            }
        } 
        if (Event.Key == SDLK_z) { // subtracting generally
            if (oneDown) {
                 if (friendType != 0)
                    friendType--;
                else
                    friendType = 1;
            } else if (twoDown) {
                if (uniType != 0) 
                    uniType--;
                else
                    uniType = 12;
            } else if (threeDown) {
                if (uniType == 1)
                    uniType--;
                else
                    uniType = 1;
            } else if (fourDown) {
                if (uniType != 0)
                    uniType--;
                else
                    uniType = 5;
            } else if (!oneDown && !threeDown && !tDown){
                if (cDown == 0) {
                    if (uniType != 0 && uniType > 0)   
                        uniType--;
                    else if (uniType == 0) {
                        uniType = -5;
                        blockWidth = 5;
                        blockHeight = 5;
                        blockDepth = 5;
                    }
                    else if (uniType == -5) {
                        uniType = 2;
                        blockWidth = 1;
                        blockHeight = 1;
                        blockDepth = 1;
                    }
                }
                if(cDown == 1&& uniType != 2) {
                    if(blockWidth > 1 && uniType != -5) 
                        blockWidth--;
                }
                if(cDown == 2&& uniType != 2) {
                    if(blockHeight > 1 && uniType != -5)
                        blockHeight--;
                }
                if(cDown == 3&& uniType != 2) {
                    if(blockDepth > 1 && uniType != -5)
                        blockDepth--;
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
    while (placeables.size() > 0)    
    {
        printf("the size of placeables is %d\n", placeables.size());
        CGameplayElement *m_block = placeables.back();
        int i,j;
        for(i = 0; i <m_block->getArea().Size.X; i++) {
            for(j = 0; j <m_block->getArea().Size.Y; j++) {
                blockMap[(int)m_block->getArea().Position.X+25+i][(int)(m_block->getArea().Position.Y-0.5+25)+j].o = false;
                blockMap[(int)m_block->getArea().Position.X+25+i][(int)(m_block->getArea().Position.Y-0.5+25)+j].r = NULL;
                blockMap[(int)m_block->getArea().Position.X+25+i][(int)(m_block->getArea().Position.Y-0.5+25)+j].mapX = m_block->getArea().Position.X+25;
                blockMap[(int)m_block->getArea().Position.X+25+i][(int)(m_block->getArea().Position.Y-0.5+25)+j].mapY = m_block->getArea().Position.Y+25;
            }
        }   
        placeables.back()->removeFromSceneManager();
        placeables.pop_back();
    }
    initBlockMap();

    /*placeables.clear();
    Application.getSceneManager().removeAllSceneObjects();

    BlocksInit();
    PreviewBlock = new CElementBlock(SRect2(1,1,1,1),1,1);
    PreviewBlock->setupObjects();*/

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
                t = xml->getAttributeValueAsInt(4);
                PrepFlag((float)x,(float)y,t);
            }
            if (!strcmp("CPItem", xml->getNodeName()))
            {
                x = xml->getAttributeValueAsInt(0);
                y = xml->getAttributeValueAsInt(1);
                t = xml->getAttributeValueAsInt(2);
                PrepItem((float)x, (float)y, t);
            }
            if (!strcmp("CPFriends", xml->getNodeName())) {
                x = xml->getAttributeValueAsInt(0);
                y = xml->getAttributeValueAsInt(1);
                t = xml->getAttributeValueAsInt(2);
                //PrepFriends(x,y,t);
            }
            if (!strcmp("CBreakable", xml->getNodeName())) {
                x = xml->getAttributeValueAsInt(0);
                y = xml->getAttributeValueAsInt(1);
                h = xml->getAttributeValueAsInt(2);
                w = xml->getAttributeValueAsInt(3);
                cDown = 4;
                printf("cDown is %d", cDown);
                PrepBlock((float)x,(float)y,w,h,1,1,1);
                cDown = 0;
            }
            if(!strcmp("DeathBlock",xml->getNodeName()))
            {
                x = xml->getAttributeValueAsInt(0);
                y = xml->getAttributeValueAsInt(1);
                h = xml->getAttributeValueAsInt(2);
                w = xml->getAttributeValueAsInt(3);
                d = xml->getAttributeValueAsInt(4);
                t = xml->getAttributeValueAsInt(5);
                cDown = 5;
                printf("deathblock x,y,h,w,d,t = %d, %d, %d, %d, %d, %d\n",x,y,h,w,d,t);
                PrepBlock((float)x,(float)y,w,h,t,1,1);
                cDown = 0;
            }
            if (!strcmp("envVar", xml->getNodeName()))
            {
                env= xml->getAttributeValueAsInt(0);
                dayNight = xml->getAttributeValueAsInt(1);
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
    std::stringstream DN, area, tag;
    //for day or night
    DN << dayNight;
    area << env;
    tag << "envVar";
    worldlist->AddAtributes("dayNight", DN.str());
    worldlist->AddAtributes("env", area.str());
    worldlist->Createtag(tag.str());
    worldlist->CloseLasttag();

    std::vector<CGameplayElement*>::iterator it;
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
   Application.getSceneManager().removeAllSceneObjects();
}

void CLWIBState::PrepPreviews(int x , int y, float t) {

    PreviewBlock->setArea(SRect2(x,y,1,1));
    PreviewBlock->update(t);

}


void CLWIBState::PrepItem(float x, float y, int item) {

   if(x < -25 || y < -25 || x >= 500 || y >= 75)
      return;
   if(blockMap[(int)x+25][(int)(y-0.5+25)].o) {
      printf("Blockmap space occupied. Did not place Item\n");
      return;
   }

   printf("Placed item starting at %0.2f, %0.2f\n", x, y);
   CElementItem *tempPlaceable;
   if (item == 0)
       tempPlaceable = CItemLoader::LoadItem(SRect2(x, y, 1, 1), (Items::EItemType)item);
   if (item == 1)
       tempPlaceable = CItemLoader::LoadItem(SRect2(x, y, 1, 1),(Items::EItemType) item);
   if (item == 2)
       tempPlaceable = CItemLoader::LoadItem(SRect2(x, y, 1, 1),(Items::EItemType) item);
   if (item == 3)
       tempPlaceable = CItemLoader::LoadItem(SRect2(x, y, 1, 1),(Items::EItemType) item);
   placeables.push_back(tempPlaceable);
   tempPlaceable->setupObjects();
 
   blockMap[(int)x+25][(int)(y-0.5+25)].o = true;
   blockMap[(int)x+25][(int)(y-0.5+25)].r = tempPlaceable;
   blockMap[(int)x+25][(int)(y-0.5+25)].mapX = (int)x+25;
   blockMap[(int)x+25][(int)(y-0.5+25)].mapY = (int)(y-0.5+25);
   redoPlaceables.clear();

}

void CLWIBState::PrepFlag(float x, float y, int t) {

   if(x < -25 || y < -25 || x >= 500 || y >= 75)
      return;
   if(blockMap[(int)x+25][(int)(y-0.5+25)].o) {
      printf("Blockmap space occupied. Did not place Cabbage\n");
      return;
   }

   printf("Placed flag starting at %0.2f, %0.2f\n", x, y);
   CGameplayElement *tempPlaceable;
   placeables.push_back(tempPlaceable = new CElementBlockFlag(SRect2(x, y, 1, 1),t));//add flag
   tempPlaceable->setupObjects();

   blockMap[(int)x+25][(int)(y-0.5+25)].o = true;
   blockMap[(int)x+25][(int)(y-0.5+25)].r = tempPlaceable;
   blockMap[(int)x+25][(int)(y-0.5+25)].mapX = (int)x+25;
   blockMap[(int)x+25][(int)(y-0.5+25)].mapY = (int)(y-0.5+25);
   redoPlaceables.clear();

}

void CLWIBState::PrepEnemy(float x, float y, int type) {
    int h = 1, w = 1;
    if(x < -25 || y < -25 || x >= 500 || y >= 75)
      return;
   if(blockMap[(int)x+25][(int)(y-0.5+25)].o) {
      printf("Blockmap space occupied. Did not place enemy\n");
      return;
   }
   printf("Placed enemy starting at %0.2f, %0.2f\n", x, y);
   CGameplayElement *tempPlaceable;
   if (type == 6){
        type = 8;
         h = 3;
        w = 3;
   }
   else if(type == 7)
        type = 9;
   else if(type == 8) 
        type = 10;
   else if (type == 9)
        type = 12;
   else if (type == 10)
        type = 14;
   else if (type == 11)
        type = 15;
   else if (type == 12)
        type = 16;
   placeables.push_back(tempPlaceable = CEnemyLoader::LoadEnemy(SRect2(x, y, (float)w, (float)h),(Enemies::EEnemyType) type));
   tempPlaceable->setupObjects(); 
   tempPlaceable->printInformation();

   blockMap[(int)x+25][(int)(y-0.5+25)].o = true;
   blockMap[(int)x+25][(int)(y-0.5+25)].r = tempPlaceable;
   blockMap[(int)x+25][(int)(y-0.5+25)].mapX = (int)x+25;
   blockMap[(int)x+25][(int)(y-0.5+25)].mapY = (int)(y-0.5+25);
   //redoPlaceables.clear();
}

/*void CLWIBState::PrepFriends(int x, int y, int t) {
    if(x < -25 || y < -25 || x >= 500 || y >= 75)
        return;
    if(blockMap[(int)x+25][(int)(y-0.5+25)].o) {
        printf("Blockmap space occupied. Did not place friend\n");
        return;
    }
    printf("Placed friend starting at %0.2f, %0.2f\n", x, y);
    CMeshSceneObject *tempFriends;
    CGameplayElement *tempPlaceable;
    blocks.push_back(tempFriends = new CMeshSceneObject());
    placeables.push_back(tempPlaceable = new CPFriends(x, y, t));
    if (t == 0)
        tempFriends->setMesh(appleMesh);

    tempFriends->setShader(ERP_DEFAULT, Diffuse);
    tempFriends->setShader(ERP_DEFERRED_OBJECTS, DeferredDiffuse);
    tempFriends->setTranslation(SVector3((x+(x+1))/2, (y+(y+1))/2, 0));
    tempFriends->setRotation(SVector3(-90, 0, 0));
    tempFriends->setScale(SVector3(1, 1, 1));
    blockMap[(int)x+25][(int)(y-0.5+25)].o = true;
    blockMap[(int)x+25][(int)(y-0.5+25)].r = tempFriends;
    blockMap[(int)x+25][(int)(y-0.5+25)].mapX = (int)x+25;
    blockMap[(int)x+25][(int)(y-0.5+25)].mapY = (int)(y-0.5+25);
    //Application.getSceneManager().addSceneObject(tempFriends);
    redo.clear();
    redoPlaceables.clear();
}*/


qd lastCabbage = blockMap[0][0];

void CLWIBState::PrepCabbage(float x, float y) {

    if(x < -25 || y < -25 || x >= 500 || y >= 75)
        return;
    if(blockMap[(int)x+25][(int)(y-0.5+25)].o) {
        printf("Blockmap space occupied. Did not place Cabbage\n");
        return;
    }

    if (cabbageFlag == 1) {
        blockMap[(int)xCabbage+25][(int)(yCabbage-0.5+25)].o = false;
        blockMap[(int)xCabbage+25][(int)(yCabbage-0.5+25)].r = NULL;
        blockMap[(int)xCabbage+25][(int)(yCabbage-0.5+25)].mapX = -1;
        blockMap[(int)xCabbage+25][(int)(yCabbage-0.5+25)].mapY = -1;
        printf("tempx = %d, tempy =  %d\n",xCabbage ,yCabbage);
        placeables.erase(std::remove(placeables.begin(), placeables.end(), lastCabbage.r), placeables.end());
        lastCabbage.r->removeFromGame();
    }

    printf("Placed cabbage starting at %0.2f, %0.2f\n", x, y);
    CGameplayElement *tempPlaceable;
    placeables.push_back(tempPlaceable = new CElementPlayer(SRect2(x, y, 1, 1), false));
    tempPlaceable->setupObjects();  

    blockMap[(int)x+25][(int)(y-0.5f+25)].o = true;
    blockMap[(int)x+25][(int)(y-0.5f+25)].r = tempPlaceable;
    blockMap[(int)x+25][(int)(y-0.5f+25)].mapX = (int)x+25;
    blockMap[(int)x+25][(int)(y-0.5f+25)].mapY = (int)(y-0.5f+25);
    xCabbage = (int) x, yCabbage = (int) y;

    lastCabbage = blockMap[(int)x+25][(int)(y-0.5+25)];
    cabbageFlag = 1;
}

void CLWIBState::PrepBlock(float x, float y, int w, int h, int d, int t, int moving) {
   if(x < -25 || y < -25 || x >= 500 || y >= 75)
      return;
   //if(t == -5 && (int)y != -5)
   //    return;
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

   printf("Placed block starting at %0.2f, %0.2f, %d, %d, %d\n", x, y, w,h,d);
   
   CGameplayElement *tempPlaceable;
   printf("cDown before placing it %d\n",cDown);
   if (cDown == 4)
   {
       placeables.push_back(tempPlaceable = new CElementBlockBreakable(SRect2(x,y,w,h)));
       printf("making breakable blocks\n");
   }
   else if (cDown == 5)
       placeables.push_back(tempPlaceable = new CElementBlockDeath(SRect2(x,y,w,h),d,t,1.0f,1.0f));
   else
       placeables.push_back(tempPlaceable = new CElementBlock(SRect2(x,y,w,h),d,t));

   for(i = 0; i < w; i++) {
      for(j = 0; j < h; j++) {
         blockMap[(int)x+25+i][(int)(y-0.5+25)+j].o = true;
         blockMap[(int)x+25+i][(int)(y-0.5+25)+j].r = tempPlaceable;
         blockMap[(int)x+25+i][(int)(y-0.5+25)+j].mapX = (int)x+25;
         blockMap[(int)x+25+i][(int)(y-0.5+25)+j].mapY = (int)(y-0.5+25);
      }
   }
   tempPlaceable->setupObjects();

}

void CLWIBState::PrepSky() {

   CMeshSceneObject *tempBlock;

   CMesh* quad = CMeshLoader::load3dsMesh("Base/Quad.3ds");
   quad->centerMeshByExtents(SVector3(0.0f));
   quad->linearizeIndices();
   quad->calculateNormalsPerFace();

   tempBlock = new CMeshSceneObject();
   tempBlock->setMesh(quad);

   tempBlock->setTexture(CImageLoader::loadTexture("Base/sky.bmp", true));

   tempBlock->setShader(ERP_DEFAULT, "DiffuseTextureBright");
   tempBlock->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Textured");
   tempBlock->setTranslation(SVector3(85/*75*/, 13, -5.0));
   tempBlock->setScale(SVector3(250, 1, 50));
   tempBlock->setRotation(SVector3(90.0f, 0.0f, 0.0f));

   CApplication::get().getSceneManager().addSceneObject(tempBlock);

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
            PrepEnemy(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY),uniType);
         }
         if(!tDown && !twoDown && !oneDown && threeDown&& !fourDown) {
            PrepFlag(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY), uniType);
         }
         if(!tDown && oneDown && !threeDown && !twoDown&& !fourDown) {
             if (friendType == 0)
                PrepCabbage(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY));
             /*else if (friendType == 1)
                PrepFriends(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY), friendType);*/
         }
         else if (!tDown && !oneDown && !threeDown &&!twoDown && !fourDown) {
            PrepBlock(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY), blockWidth, blockHeight, blockDepth,uniType,mDown);
         }
         else if (!tDown && !oneDown && !threeDown &&!twoDown && fourDown) {
            PrepItem(round(eye.X + previewBlockMouseX),round(eye.Y + previewBlockMouseY),uniType);
         }
         //else { //Delete item
         if (tDown && !oneDown && !threeDown && !twoDown && !fourDown) {
             if(lastMouseOveredBlock.o) {
                 printf("clicked on removing\n");
                 lastMouseOveredBlock.r->removeFromGame();
                 placeables.erase(std::remove(placeables.begin(), placeables.end(), lastMouseOveredBlock.r), placeables.end());
                 //blocks.erase(std::remove(blocks.begin(), blocks.end(), lastMouseOveredBlock.r), blocks.end());
                 //redoPlaceables.push_back(lastMouseOveredBlock.p);
                 //redo.push_back(lastMouseOveredBlock.r);
                 int x = lastMouseOveredBlock.mapX;
                 int y = lastMouseOveredBlock.mapY;

                 int i,j;
                 for(i = 0; i < lastMouseOveredBlock.r->getArea().Size.X; i++) {
                     for(j = 0; j < lastMouseOveredBlock.r->getArea().Size.Y; j++) {
                         blockMap[x+i][y+j].o = false;
                         blockMap[x+i][y+j].r = NULL;
                         blockMap[x+i][y+j].mapX = -1;
                         blockMap[x+i][y+j].mapY = -1;
                         printf("removed object at %d , %d\n",x,y);
                     }
                 }
                 lastMouseOveredBlock = blockMap[x][y];
             }
            printf("Here7\n");
         }
         //}

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
            /*if(m_qd.o && m_qd.r != lastMouseOveredBlock.r) {

                tempShader = m_qd.r->getShader(); 
                m_qd.r->setShader(ERP_DEFAULT, DiffuseTextureBright);
            }
            if(lastMouseOveredBlock.o && m_qd.r != lastMouseOveredBlock.r) {
                lastMouseOveredBlock.r->setShader(ERP_DEFAULT, tempShader);
            }*/
         }
         if(!threeDown && !tDown && !twoDown && !oneDown && !fourDown && mouseDown) {
            PrepBlock(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY), blockWidth, blockHeight, blockDepth, uniType,mDown);
         }
         else if(twoDown && mouseDown) {
            PrepEnemy(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY),uniType);
         }
         else if(threeDown && mouseDown) {
            PrepFlag(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY), uniType);
         }
         else if (mouseDown && fourDown) {
             PrepItem(round(eye.X + previewBlockMouseX),round(eye.Y + previewBlockMouseY),uniType);
         }
         lastMouseOveredBlock = m_qd;
      }
   }
}

void CLWIBState::removeObject() {
    printf("placeable.size() is %d\n", placeables.size());
    if (placeables.size() > 0) {
        CGameplayElement *m_block = placeables.back();
        printf("placeable x is %f placeable y is %f\n",m_block->getArea().Size.X,m_block->getArea().Size.Y);
        int i,j;
        for(i = 0; i <m_block->getArea().Size.X; i++) {
            for(j = 0; j <m_block->getArea().Size.Y; j++) {
                blockMap[(int)m_block->getArea().Position.X+25+i][(int)(m_block->getArea().Position.Y-0.5+25)+j].o = false;
                blockMap[(int)m_block->getArea().Position.X+25+i][(int)(m_block->getArea().Position.Y-0.5+25)+j].r = NULL;
                blockMap[(int)m_block->getArea().Position.X+25+i][(int)(m_block->getArea().Position.Y-0.5+25)+j].mapX = -1;
                blockMap[(int)m_block->getArea().Position.X+25+i][(int)(m_block->getArea().Position.Y-0.5+25)+j].mapY = -1;
                printf("here\n");
            }
        }
        redoPlaceables.push_back(m_block);
        placeables.back()->removeFromGame();
        placeables.pop_back();
    }
}

void CLWIBState::undoObjects() {
    if(redoPlaceables.size() > 0) {
        CGameplayElement *m_block = redoPlaceables.back();
        //printf("placeable x is %f placeable y is %f\n",m_block->getArea().Size.X,m_block->getArea().Size.Y);
        int i,j;
        for(i = 0; i <m_block->getArea().Size.X; i++) {
            for(j = 0; j <m_block->getArea().Size.Y; j++) {
                blockMap[(int)m_block->getArea().Position.X+25+i][(int)(m_block->getArea().Position.Y-0.5+25)+j].o = true;
                blockMap[(int)m_block->getArea().Position.X+25+i][(int)(m_block->getArea().Position.Y-0.5+25)+j].r = m_block;
                blockMap[(int)m_block->getArea().Position.X+25+i][(int)(m_block->getArea().Position.Y-0.5+25)+j].mapX = m_block->getArea().Position.X+25;
                blockMap[(int)m_block->getArea().Position.X+25+i][(int)(m_block->getArea().Position.Y-0.5+25)+j].mapY = m_block->getArea().Position.Y+25;
                printf("here\n");
            }
        }
        placeables.push_back(m_block);
        redoPlaceables.pop_back();
        m_block->setupObjects();
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

    block1->addDropShadow();
    block2->addDropShadow();
    block3->addDropShadow();
    help->addDropShadow();

    Application.getGUIEngine().addWidget(block1);
    Application.getGUIEngine().addWidget(block2);
    Application.getGUIEngine().addWidget(block3);
    Application.getGUIEngine().addWidget(help);

}

void CLWIBState::OnWidgetHover(CGUIWidget *widget) {
    if(widget) 
        clickDown = 1;
}
void CLWIBState::OnWidgetUnHover(CGUIWidget *widget) {
    if (widget)
        clickDown = 0;
}

void CLWIBState::changeTiles() {

    if (change == 0) { // blocksi
        tileOne->setImage(grass);
        tileTwo->setImage(dirt);
        tileThree->setImage(rock);
        tileFour->setImage(ground);
        tileFive->setImage(blockLeft);
        tileSix->setImage(blockRight);
        tileSeven->setImage(blockDown);
        tileEight->setImage(blockUp);
        tileNine->setImage(blockIn);
        tileTen->setImage(blockOut);

        for (int i = 0; i < 10; i++)
        {
            if (!Application.getGUIEngine().isWidgetIn(tileArray[i]))
                Application.getGUIEngine().addWidget(tileArray[i]);
        }

    }
    if (change == 1) { // cabbage

        tileOne->setImage(cabbageImage);
        tileTwo->setImage(derp);
        for (int i = 0; i < 2; i++)
        {
            if (!Application.getGUIEngine().isWidgetIn(tileArray[i]))
                Application.getGUIEngine().addWidget(tileArray[i]);
        }
        for (int i = 2; i < 10; i++)
        {
            if (Application.getGUIEngine().isWidgetIn(tileArray[i]))
                Application.getGUIEngine().removeWidget(tileArray[i]);
        }
    } 
    if (change == 2) { // enemies

        tileOne->setImage(apple);
        tileTwo->setImage(orange);
        tileThree->setImage(kiwi);
        tileFour->setImage(grape);
        tileFive->setImage(fire);
        tileSix->setImage(blade);
        tileSeven->setImage(apple);
        tileEight->setImage(pear);
        tileNine->setImage(banana);
        tileTen->setImage(cherry);
        for (int i = 0; i < 10; i++)
        {
            if (!Application.getGUIEngine().isWidgetIn(tileArray[i]))
                Application.getGUIEngine().addWidget(tileArray[i]);
        }
    }
    if (change == 3) { // flag

        tileOne->setImage(flagImg);
        tileTwo->setImage(flagImg);
        for (int i = 0; i < 2; i++)
        {
            if (!Application.getGUIEngine().isWidgetIn(tileArray[i]))
                Application.getGUIEngine().addWidget(tileArray[i]);
        }
        for (int i = 2; i < 10; i++)
        {
            if (Application.getGUIEngine().isWidgetIn(tileArray[i]))
                Application.getGUIEngine().removeWidget(tileArray[i]);
        }
    }
    if (change == 4) { //items
        tileOne->setImage(leaf);
        tileTwo->setImage(heart);
        tileThree->setImage(heart);
        tileFour->setImage(heart);
        for (int i = 0; i < 4; i++)
        {
            if (!Application.getGUIEngine().isWidgetIn(tileArray[i]))
                Application.getGUIEngine().addWidget(tileArray[i]);
        }
        for (int i = 4; i < 10; i++)
        {
            if (Application.getGUIEngine().isWidgetIn(tileArray[i]))
                Application.getGUIEngine().removeWidget(tileArray[i]);
        }

    }
    if (change == 5) {
        for (int i = 0; i < 10; i++)
        {
            if (Application.getGUIEngine().isWidgetIn(tileArray[i]))
                Application.getGUIEngine().removeWidget(tileArray[i]);
        }
    }
    if (change == 6) {
        for (int i = 0; i < 6; i++)
        {
            if (!Application.getGUIEngine().isWidgetIn(tileArray[i]))
                Application.getGUIEngine().addWidget(tileArray[i]);
        }
        for (int i = 6; i < 10; i++)
        {
            if (Application.getGUIEngine().isWidgetIn(tileArray[i]))
                Application.getGUIEngine().removeWidget(tileArray[i]);
        }
    }

}

void CLWIBState::OnWidgetClick(CGUIWidget *widget) {
    
    if (widget == leftArrow) {
        if (change == 0)
            change = 6;
        else
            change--;
        if (change == 0)
            cDown = 0;
    }
    if (widget == rightArrow){
        if (change == 6)
            change = 0;
        else
            change++;
        if (change == 0)
            cDown = 0;
    }
    if (widget == tileOne) {
        if (change == 0) {
            if (uniType == -5) {
                blockWidth = 1;
                blockHeight = 1;
                blockDepth = 1;
            }
            uniType = 0;
            cDown = 0;
        }
        if (change == 1)
        {
            friendType = 0;
        }
        if (change == 2) {
            uniType = 0;
        }
        if (change == 3) {
            uniType = 0;
        }
        if (change == 4) {
            uniType = 0;
        }
        if (change == 6)
            dayNight = 0;
    }
    if (widget == tileTwo) {
        if (change == 0) {
            if (uniType == -5) {
                blockWidth = 1;
                blockHeight = 1;
                blockDepth = 1;
            }
            uniType = 1;
            cDown = 0;
        }
        if (change == 1){
            friendType = 1;
        }
        if (change == 2) {
            uniType = 1;
        }
        if (change == 3) {
            uniType = 1;
        }
        if (change == 4) {
            uniType =1; 
        }
        if (change == 6)
            dayNight = 1;
    }
    if (widget == tileThree) {
        if (change == 0) {
            if (uniType == -5) {
                blockWidth = 1;
                blockHeight = 1;
                blockDepth = 1;
            }
            uniType = 2;
            cDown = 0;
        }
        if (change == 2) {
            uniType = 2;
        }
        if (change == 4)
            uniType = 2;
        if (change == 6)
            env = 0;
    }
    if (widget == tileFour) {
        if (change == 0) {
            uniType = -5;
            blockWidth = 5;
            blockHeight = 2;
            blockDepth = 5;
            cDown = 0;
        }
        if (change == 2) {
            uniType = 3;
        }
        if (change == 4)
            uniType = 3;
        if (change == 6)
            env = 1;
    }
    if (widget == tileFive) {
        if (change == 0) {
            if (blockWidth > 1 && uniType != -5)
                blockWidth--;
            cDown = 1;
        }
        if (change == 2) {
            uniType = 4;
        }
        if (change == 6)
            env = 2;
    }
    if (widget == tileSix) {
        if (change == 0) {
            if (blockWidth < 10 && uniType != -5)
                blockWidth++;
            cDown = 1;

        }
        if (change == 2) {
            uniType = 5;
        }
        if (change == 6)
            env = 3;
    }
    if (widget == tileSeven) {
        if (change == 0) {
            if (blockHeight > 1 && uniType != -5)
                blockHeight--;
            cDown = 2;
        }
        if (change == 2) {
            uniType = 6;
        }
    }
    if (widget == tileEight) {
        if (change == 0) {
            if (blockHeight < 10 && uniType != -5)
                blockHeight++;
            cDown = 2;
        }
        if (change == 1) {
        }
        if (change == 2)
            uniType = 7;
    }
    if (widget == tileNine) {
        if (change == 0) {
            if (blockDepth > 1 && uniType != -5)
                blockDepth--;
            cDown = 3;
        }
        if (change == 1) {
        }
        if (change == 2)
            uniType = 8;
    }
    if (widget == tileTen) {
        if (tileLoop < 2)
            tileLoop++;
        else
            tileLoop = 0;
        /*if (change == 0) {
            if (blockDepth < 5 && uniType != -5)
                blockDepth++;
            cDown = 3;
        }
        if (change == 1) {
        }
        if (change == 2)
            uniType = 9;*/
    }
    if (widget == save ) {
        printXML();
    }
    if (widget == load ) {
        loadWorld();
        cDown = 0;
    }
    if (widget == undoTile) {
        if( placeables.size() > 0) {
            undoObjects();
        }
    }
    if (widget == redoTile) {
        /*if(redoPlaceables.size() > 0) {
            //Application.getSceneManager().addSceneObject(redo.back());
            CGameplayElement *m_block = redoPlaceables.back();
            CMeshSceneObject *m_r = redo.back();
            placeables.push_back(redoPlaceables.back());

            int i,j;
            for(i = 0; i < m_block->getArea().Position.X; i++) {
                for(j = 0; j < m_block->getArea().Size.Y; j++) {
                    blockMap[(int)m_block->getArea().Position.X+25+i][(int)(m_block->getArea().Position.Y-0.5+25)+j].o = true;
                 //   blockMap[(int)m_block->getArea().Position.X+25+i][(int)(m_block->getArea().Position.Y-0.5+25)+j].r = m_r;

                    blockMap[(int)m_block->getArea().Position.X+25+i][(int)(m_block->getArea().Position.Y-0.5+25)+j].mapX = (int)m_block->getArea().Position.X+25;
                    blockMap[(int)m_block->getArea().Position.X+25+i][(int)(m_block->getArea().Position.Y-0.5+25)+j].mapY = (int)(m_block->getArea().Position.Y-0.5+25);
                }
            }

            redoPlaceables.pop_back();
        }*/

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
        sixDown = 0;

    }
    if (change == 1) {
        oneDown = 1;
        threeDown = 0; 
        tDown = 0;
        twoDown = 0;
        fourDown = 0;
        sixDown = 0;
        blockWidth = 1;
        blockHeight = 1;
        blockDepth = 1;
    }
    if (change == 2) {
        twoDown = 1; //enemy
        threeDown = 0; 
        tDown = 0;
        oneDown = 0;
        sixDown = 0;
        fourDown = 0;
        blockWidth = 1;
        blockHeight = 1;
        blockDepth = 1;
    }
    if (change == 3) {
        threeDown = 1;
        oneDown = 0;
        tDown = 0;
        twoDown = 0;
        fourDown = 0;
        sixDown = 0;
        blockWidth = 1;
        blockHeight = 1;
        blockDepth = 1;
    }
    if (change == 4) {
        fourDown = 1;
        threeDown = 0;
        oneDown = 0;
        tDown = 0;
        sixDown = 0;
        twoDown = 0;
        blockWidth = 1;
        blockHeight = 1;
        blockDepth = 1;
    }
    if (change == 5) {
        fourDown = 0;
        threeDown = 0;
        oneDown = 0;
        tDown = 1;
        twoDown = 0;
        sixDown = 0;
        blockWidth = 1;
        blockHeight = 1;
        blockDepth = 1;
    }
    if (change == 6) {
        fourDown = 0;;
        threeDown = 0;
        oneDown = 0;
        sixDown = 1;
        tDown = 0;
        twoDown = 0;
        blockWidth = 1;
        blockHeight = 1;
        blockDepth = 1;
    }
    
}

void CLWIBState::prepHud() {
    // prepping hud wwidgest
    SVector2 norm = SVector2(.1f, .1f);
    CTexture *imgLeft = new CTexture(CImageLoader::loadImage("ModelImages/leftArrowPic.bmp"));
    CTexture *imgright = new CTexture(CImageLoader::loadImage("ModelImages/rightArrowPic.bmp"));
    CTexture *saveImg = new CTexture(CImageLoader::loadImage("ModelImages/save.bmp"));
    CTexture *loadImg = new CTexture(CImageLoader::loadImage("ModelImages/folder.bmp"));
    CTexture *undoImg = new CTexture(CImageLoader::loadImage("ModelImages/undo.bmp"));
    CTexture *redoImg = new CTexture(CImageLoader::loadImage("ModelImages/redo.bmp"));

    //prep more tiles

    blockRight = new CTexture(CImageLoader::loadImage("ModelImages/blockLength2.bmp"));
    blockLeft = new CTexture(CImageLoader::loadImage("ModelImages/blockLength.bmp"));
    blockUp = new CTexture(CImageLoader::loadImage("ModelImages/blockHeight.bmp"));
    blockDown = new CTexture(CImageLoader::loadImage("ModelImages/blockHeight2.bmp"));
    blockIn = new CTexture(CImageLoader::loadImage("ModelImages/blockDepth.bmp"));
    blockOut = new CTexture(CImageLoader::loadImage("ModelImages/blockDepth2.bmp"));

    derp = new CTexture(CImageLoader::loadImage("ModelImages/derp_gray.bmp"));
    grape = new CTexture(CImageLoader::loadImage("ModelImages/grapes_gray.bmp"));
    banana = new CTexture(CImageLoader::loadImage("ModelImages/banana_gray.bmp"));
    pear = new CTexture(CImageLoader::loadImage("ModelImages/pear_gray.bmp"));
    cherry = new CTexture(CImageLoader::loadImage("ModelImages/cherries_gray.bmp"));
    grass = new CTexture(CImageLoader::loadImage("Base/grass.bmp"));
    dirt = new CTexture(CImageLoader::loadImage("Base/dirt.bmp"));
    rock = new CTexture(CImageLoader::loadImage("Base/rock.bmp"));
    ground = new CTexture(CImageLoader::loadImage("Base/GrassyGrass.bmp"));
    apple = new CTexture(CImageLoader::loadImage("ModelImages/apple_gray.bmp"));
    orange = new CTexture(CImageLoader::loadImage("ModelImages/orange_gray.bmp"));
    kiwi = new CTexture(CImageLoader::loadImage("ModelImages/kiwi_gray.bmp"));
    blade = new CTexture(CImageLoader::loadImage("ModelImages/trap1_gray.bmp"));
    cabbageImage = new CTexture(CImageLoader::loadImage("ModelImages/cabbage_gray.bmp"));
    fire = new CTexture(CImageLoader::loadImage("ModelImages/flame.bmp"));
    grape = new CTexture(CImageLoader::loadImage("ModelImages/grapes_gray.bmp"));
    leaf = new CTexture(CImageLoader::loadImage("ModelImages/leaf_gray.bmp"));
    heart = new CTexture(CImageLoader::loadImage("ModelImages/water_energy_gray.bmp"));
    flagImg = new CTexture(CImageLoader::loadImage("ModelImages/flag_gray.bmp"));
    //arrows to cycle though the blocks, enemies, cabbage, flag etc.
    leftArrow = new CGUIImageWidget(imgLeft, norm);
    leftArrow->setPosition(SVector2(1.05f, .85f));

    rightArrow = new CGUIImageWidget(imgright, norm);
    rightArrow->setPosition(SVector2(1.20f, .85f));
    // cabbage button
    //cabbage = new CGUIImageWidget();

    save = new CGUIImageWidget(saveImg, norm);
    save->setPosition(SVector2(.75f, .85f));
    load = new CGUIImageWidget(loadImg, norm);
    load->setPosition(SVector2(.90f, .85f));
    //flag = new CGUIImageWidget();
    
    undoTile = new CGUIImageWidget(undoImg, norm);
    undoTile->setPosition(SVector2(1.05f, .0f));
    redoTile = new CGUIImageWidget(redoImg, norm);
    redoTile->setPosition(SVector2(1.20f, .0f));
    //blocks buttons

    tileOne = new CGUIImageWidget(grass,norm);
    tileOne->setPosition(SVector2(1.05f, .73f));
    tileArray.push_back(tileOne);

    tileTwo = new CGUIImageWidget(dirt,norm);
    tileTwo->setPosition(SVector2(1.20f, .73f));
    tileArray.push_back(tileTwo);
    
    tileThree = new CGUIImageWidget(rock,norm);
    tileThree->setPosition(SVector2(1.05f, .58f));
    tileArray.push_back(tileThree);
    
    tileFour = new CGUIImageWidget(ground,norm);
    tileFour->setPosition(SVector2(1.20f, .58f));
    tileArray.push_back(tileFour);
    
    tileFive = new CGUIImageWidget(ground,norm);
    tileFive->setPosition(SVector2(1.05f, .43f));
    tileArray.push_back(tileFive);
   
    tileSix = new CGUIImageWidget(ground,norm);
    tileSix->setPosition(SVector2(1.20f, .43f));
    tileArray.push_back(tileSix);
    
    tileSeven = new CGUIImageWidget(ground,norm);
    tileSeven->setPosition(SVector2(1.05f, .28f));
    tileArray.push_back(tileSeven);
    
    tileEight = new CGUIImageWidget(ground,norm);
    tileEight->setPosition(SVector2(1.20f, .28f));
    tileArray.push_back(tileEight);
    
    tileNine = new CGUIImageWidget(ground,norm);
    tileNine->setPosition(SVector2(1.05f, .13f));
    tileArray.push_back(tileNine);
    
    tileTen = new CGUIImageWidget(ground,norm);
    tileTen->setPosition(SVector2(1.20f, .13f));
    tileArray.push_back(tileTen);
    //adding widgets to game 

    Application.getGUIEngine().addWidget(undoTile);
    Application.getGUIEngine().addWidget(redoTile);
    Application.getGUIEngine().addWidget(save);
    Application.getGUIEngine().addWidget(load);
    Application.getGUIEngine().addWidget(tileOne);
    Application.getGUIEngine().addWidget(tileTwo);
    Application.getGUIEngine().addWidget(tileThree);
    Application.getGUIEngine().addWidget(tileFour);
    //Application.getGUIEngine().addWidget(tileFive);
    //Application.getGUIEngine().addWidget(tileSix);
    //Application.getGUIEngine().addWidget(tileSeven);
    //Application.getGUIEngine().addWidget(tileEight);
    //Application.getGUIEngine().addWidget(tileNine);
    Application.getGUIEngine().addWidget(leftArrow);
    Application.getGUIEngine().addWidget(rightArrow);
}

void CLWIBState::stepCamera(float delta) {
   float factor = 6;
   //D
   if(dDown) {
       if (spaceDown == 1){
           eye.X += delta*factor*6;
           look.X += delta*factor*6;
       }
       eye.X += delta*factor;
       look.X += delta*factor;
   }
   //A
   if(aDown) {
       if (spaceDown == 1){
           eye.X -= delta*factor*6;
           look.X -= delta*factor*6;
       }
      eye.X -= delta*factor;
      look.X -= delta*factor;
   }
   //W
   if(wDown) {
       if (spaceDown == 1){
           eye.Y += delta*factor*6;
           look.Y += delta*factor*6;
       }
      eye.Y += delta*factor;
      look.Y += delta*factor;
   }
   //S
   if(sDown) {
       if (spaceDown == 1){
           eye.Y -= delta*factor*6;
           look.Y -= delta*factor*6;
       }
      eye.Y -= delta*factor;
      look.Y -= delta*factor;
   }
   if(!tDown && !twoDown && !oneDown && !twoDown && !threeDown && !fourDown && mouseDown) {
      PrepBlock(round(eye.X + previewBlockMouseX), round(eye.Y + previewBlockMouseY), blockWidth, blockHeight, blockDepth,uniType,mDown);
   }
}

