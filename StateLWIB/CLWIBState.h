#ifndef __CLWIBSTATE_HEADER_
#define __CLWIBSTATE_HEADER_

#include <iostream>

#include "header.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "../StateMainMenu/CMainMenuState.h"
#include "CPlaceable.h"
#include "CBlock.h"
#include "CCabbage.h"
#include "CFlag.h"
#include "CPItem.h"
#include "../Base/irrxml-1.2/src/irrXML.h"
#include "wmlwriter_src/xmlwriter.h"
#include "../CabbageGUI/CabbageGUI.h"

#define TREE_Y_OFFSET 2.1
#define ANGLE(j,k) (j==2?3:(j?2:(k?1:0)))
#define NEXT(j) (j==2?0:(j?2:1))

class CLWIBState : public CState<CLWIBState>
{
   CApplication & Application;
   private:

   //Boolean integers for keypressing
   int aDown , dDown , spaceDown , wDown , sDown , gDown , fDown , tDown, eDown, mDown, cDown;
   int oneDown, twoDown, threeDown, fourDown, clickDown;
   int cabbageFlag;
   int xCabbage;
   int yCabbage;
   int blockCycle;
   int change;
   bool showHelp;
   int enemyType, textureType, itemType;
   int WindowWidth, WindowHeight;
   //stuff for textures
   CTexture *grass,*dirt,*rock,*ground,*cabbageImage,*apple,*orange,*kiwi,*fire,*blade;
   // GUIImageWidget
   CGUIFontWidget *block1, *block2, *block3;
   CGUIFontWidget *help;

   // GUIImagewidgets
   CGUIImageWidget *leftArrow, *rightArrow, *cabbage, *flag;
   CGUIImageWidget *tileOne, *tileTwo, *tileThree, *tileFour;
   CGUIImageWidget *tileFive, *tileSix, *tileSeven, *tileEight, *tileNine;
   CGUIImageWidget *save, *load;
    //CMESH stuff
   CMesh *cubeMesh, *appleMesh, *orangeMesh, *kiwiMesh, *cabbageMesh, *groundMesh, *bladeMesh, *flagMesh;
   CMesh *health, *energy;
   float xp2w(int oldX);
   float yp2w(int oldY);
   

   public:
   std::vector<CPlaceable*> placeables, redoPlaceables;
   std::vector<CMeshSceneObject*> blocks, redo;
   CLWIBState();
   void begin();
   CPerspectiveCamera *Camera;
   SVector3 eye, look;

   CShader *DiffuseTexture, *DiffuseTextureBright, *Diffuse;



   int blockWidth, blockHeight,blockDepth;
   void OnRenderStart(float const Elapsed);
   void OnKeyboardEvent(SKeyboardEvent const & Event);
   void OnMouseEvent(SMouseEvent const & Event);
   void end();

   void pickInsert();
   void PrepBlock(float x, float y, int w, int h, int d, int t, int moving); 
   void PrepEnemy(float x, float y, int type); 
   void PrepCabbage(float x, float y);
   void PrepFlag(float X ,float y);
   void PrepItem(float x, float y, int type);
   void PrepGrass(float x, float y, float w, float h);
   void PrepPreviews();

   void PrepSky();
   void loadWorld();
   void printXML();
   void drawSubWindow();
   void BlocksInit( void ); 
   void prepText();
   void prepHud();
   void stepCamera(float delta);
   void changeTiles();
   void OnWidgetClick(CGUIWidget * Widget);
   void OnWidgetHover(CGUIWidget * Widget);
   void OnWidgetUnHover(CGUIWidget * Widget);

};
#endif
