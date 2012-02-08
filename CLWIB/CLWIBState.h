#ifndef __CGAMESTATE_HEADER_
#define __CGAMESTATE_HEADER_

#include <iostream>

#include "header.h"


#define TREE_Y_OFFSET 2.1
#define ANGLE(j,k) (j==2?3:(j?2:(k?1:0)))
#define NEXT(j) (j==2?0:(j?2:1))

class CLWIBState : public CState<CLWIBState>
{
   CApplication & Application;

   public:
   CLWIBState();
   void begin();
   CCamera *Camera;
   SVector3 eye, look;

   void OnRenderStart(float const Elapsed);
   void OnKeyboardEvent(SKeyboardEvent const & Event);
   void OnMouseEvent(SMouseEvent const & Event);
   void end();

   void PrepBlock(float x, float y, float w, float h); 
   void PrepGrass(float x, float y, float w, float h);

   void PrepSky();

   void drawSubWindow();

   void BlocksInit( void ); 

   void stepCamera(float delta); 
   CMeshRenderable* PrepEnemy(float x, float y);
};
#endif
