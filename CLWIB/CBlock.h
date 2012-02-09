#ifndef __CBLOCK_H____
#define __CBLOCK_H____

#include <stdio.h>
#include "CPlaceable.h"
#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "../base/CGameplayManager.h"
#include "../CabbageCollider/CEngine.h"
#include "../CabbageCollider/CObject.h"
#include "../CabbageCollider/CActor.h"
#include <sstream>
#include <string>
class CBlock : public CPlaceable
{
   public:

   CBlock(float nx, float ny, int width, int height);
   void printXML();
   void moveTo(float,float);
   CMeshRenderable * setupItem(CShader * shader, Cabbage::Collider::CEngine *Engine, CGameplayManager *GameplayManager);
};

#endif
