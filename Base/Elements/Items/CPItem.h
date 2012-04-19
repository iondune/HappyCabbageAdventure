#ifndef __CITEM_H____
#define __CITEM_H____

#include <stdio.h>
#include "CPlaceable.h"
#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "../Base/CGameplayManager.h"
#include "../CabbageCollider/CCollisionEngine.h"
#include "../CabbageCollider/CCollisionObject.h"

#include "../CabbageCollider/CCollisionActor.h"
#include <sstream>
#include <string>
class CPItem : public CPlaceable
{
   public:
   CElevator* elevator;
   int Speed, Range;
   CPItem(float nx, float ny, int type);
   int t;
   void writeXML(xmlwriter *);
   void moveTo(float,float);
   
   CMeshSceneObject * setupItem(CShader * shader, CShader * dShader, CCollisionEngine *Engine, CGameplayManager *GameplayManager);
};

#endif
