#ifndef __CBLOCK_H____
#define __CBLOCK_H____

#include <stdio.h>
#include "CPlaceable.h"
#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "../Base/CGameplayManager.h"
#include "../CabbageCollider/CCollisionEngine.h"
#include "../CabbageCollider/CCollisionObject.h"
#include "../CabbageCollider/CActor.h"
#include <sstream>
#include <string>
class CBlock : public CPlaceable
{
   public:
   CElevator* elevator;
   int Speed, Range;
   CBlock(float nx, float ny, int width, int height, int depth, int texture, int moving, int env);
   int z, t, env;
   void writeXML(xmlwriter *);
   void moveTo(float,float);
   CMeshSceneObject * setupItem(CShader * shader, CShader * dShader, CCollisionEngine *Engine, CGameplayManager *GameplayManager);
};

#endif
