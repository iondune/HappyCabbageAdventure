#ifndef __CBLOCK_H____
#define __CBLOCK_H____

#include <stdio.h>
#include "CPlaceable.h"
#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "../Base/CGameplayManager.h"
#include "../CabbageCollider/CEngine.h"
#include "../CabbageCollider/CObject.h"
#include "../CabbageCollider/CActor.h"
#include <sstream>
#include <string>
class CBlock : public CPlaceable
{
   public:
   Cabbage::Collider::CElevator* elevator;
   int Speed, Range;
   CBlock(float nx, float ny, int width, int height, int depth, int texture, int moving);
   int z, t;
   void writeXML(xmlwriter *);
   void moveTo(float,float);
   CMeshSceneObject * setupItem(CShader * shader, Cabbage::Collider::CEngine *Engine, CGameplayManager *GameplayManager);
};

#endif
