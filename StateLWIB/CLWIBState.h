#ifndef __CLWIBSTATE_HEADER_
#define __CLWIBSTATE_HEADER_

#include <iostream>

#include "header.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "../StateMainMenu/CMainMenuState.h"
#include "../CabbageGame/Elements/Enemies/CElementEnemy.h"
#include "../CabbageGame/Elements/Blocks/CElementBlock.h"
#include "../CabbageGame/Elements/Player/CElementPlayer.h"
#include "../CabbageGame/Elements/Blocks/CElementBlockBreakable.h"
#include "../CabbageGame/Elements/Blocks/CElementBlockDeath.h"
#include "../CabbageGame/Elements/Items/CElementItem.h"
#include "../CabbageGame/Elements/CGameplayElement.h"
#include "../CabbageGame/irrxml-1.2/src/irrXML.h"
#include "../CabbageGame/wmlwriter_src/xmlwriter.h"
#include "../CabbageGame/CGameLevel.h"
#include "../CabbageGame/Elements/Blocks/CElementBlockFlag.h"
#include "../CabbageGame/CGameplayManager.h"
#include "../CabbageGUI/CabbageGUI.h"

#define TREE_Y_OFFSET 2.1
#define ANGLE(j,k) (j==2?3:(j?2:(k?1:0)))
#define NEXT(j) (j==2?0:(j?2:1))

class CLWIBState : public CState<CLWIBState>
{
   CApplication & Application;
   private:

   //Boolean integers for keypressing
   CGameplayManager *worldDriver;
   CGameLevel *gameWorld;
   int aDown , dDown , spaceDown , wDown , sDown , gDown , fDown , tDown, eDown, mDown, cDown;
   int oneDown, twoDown, threeDown, fourDown, sixDown, clickDown;
   int cabbageFlag;
   int flagFlag;
   int secretFlag;
   int xCabbage;
   int yCabbage;
   int blockFlag;
   int blockCycle;
   int change;
   int tileLoop;
   int dayNight, env; 
   bool showHelp, loadedFromXml;
   int enemyType, textureType, itemType, friendType;
   int uniType;
   int WindowWidth, WindowHeight;
   //stuff for textures
   CTexture *grass,*dirt,*rock,*ground,*cabbageImage,*apple,*orange,*kiwi,*fire,*blade, *alien;
   CTexture *leaf, *heart, *derp;
   CTexture *blockRight, *blockLeft, *blockUp, *blockDown, *blockIn, *blockOut, *flagImg;
   CTexture *grape, *banana, *pear, *cherry, *pineapple, *strawberry, *pomegrante, *circleArrow;
   // GUIImageWidget
   CGUIFontWidget *block1, *block2, *block3;
   CGUIFontWidget *help;

   // GUIImagewidgets
   CGUIImageWidget *leftArrow, *rightArrow, *cabbage, *flag;
   CGUIImageWidget *tileOne, *tileTwo, *tileThree, *tileFour;
   CGUIImageWidget *tileFive, *tileSix, *tileSeven, *tileEight, *tileNine, *tileTen;
   CGUIImageWidget *save, *load, *undoTile, *redoTile;
    //CMESH stuff

   float xp2w(int oldX);
   float yp2w(int oldY);
   CGameplayElement *PreviewBlock;

   public:
   std::vector<CGUIImageWidget*> tileArray;
   std::vector<CGameplayElement*> placeables, redoPlaceables;//, PreviewBlock;
   CLWIBState();
   void begin();
   CPerspectiveCamera *Camera;
   SVector3f eye, look;

   int blockWidth, blockHeight,blockDepth;
   void OnRenderStart(float const Elapsed);
   void OnKeyboardEvent(SKeyboardEvent const & Event);
   void OnMouseEvent(SMouseEvent const & Event);
   void end();

   void pickInsert();
   void PrepBlock(float x, float y, int w, int h, int d, int t, int moving); 
   void PrepEnemy(float x, float y, int type); 
   void PrepCabbage(float x, float y);
   void PrepFlag(float X ,float y, int t);
   void PrepItem(float x, float y, int type);
  //void PrepFriends(int x, int y, int t);
   void PrepPreviews(int x, int y, int w ,int h , float t);

   void PrepSky();
   void loadWorld();
   void printXML();
   void drawSubWindow();
   void BlocksInit( void ); 
   void prepText();
   void prepHud();
   void stepCamera(float delta);
   void changeTiles();
   void removeCollideable();
   void undoObjects();
   void OnWidgetClick(CGUIWidget * Widget);
   void OnWidgetHover(CGUIWidget * Widget);
   void OnWidgetUnHover(CGUIWidget * Widget);

};
#endif
