#ifndef __CLWIBSTATE_HEADER_
#define __CLWIBSTATE_HEADER_

#include <iostream>

#include "header.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "../StateMainMenu/CMainMenuState.h"
#include "CPlaceable.h"
#include "CBlock.h"
#include "wmlwriter_src/xmlwriter.h"

#define TREE_Y_OFFSET 2.1
#define ANGLE(j,k) (j==2?3:(j?2:(k?1:0)))
#define NEXT(j) (j==2?0:(j?2:1))

class CLWIBState : public CState<CLWIBState>
{
   CApplication & Application;
   private:

   //Boolean integers for keypressing
   int aDown , dDown , spaceDown , wDown , sDown , gDown , fDown , tDown, eDown, mDown;
   bool showHelp;
   freetype::font_data our_font;

   int WindowWidth, WindowHeight;


   CMesh *cubeMesh, *appleMesh;

   float xp2w(int oldX);
   float yp2w(int oldY);


   public:
   std::vector<CPlaceable*> placeables, redoPlaceables;
   std::vector<CMeshRenderable*> blocks, redo;
   CLWIBState();
   void begin();
   CCamera *Camera;
   SVector3 eye, look;

   CShader *DiffuseTexture, *DiffuseTextureBright, *Diffuse;



   int blockWidth, blockHeight;
   void OnRenderStart(float const Elapsed);
   void OnKeyboardEvent(SKeyboardEvent const & Event);
   void OnMouseEvent(SMouseEvent const & Event);
   void end();

   void PrepBlock(float x, float y, int w, int h); 
   void PrepEnemy(float x, float y); 
   void PrepGrass(float x, float y, float w, float h);
   void PrepPreviews();

   void PrepSky();

   void printXML();

   void drawSubWindow();

   void BlocksInit( void ); 

   void stepCamera(float delta); 
};
#endif
