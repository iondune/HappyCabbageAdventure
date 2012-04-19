#ifndef __CCABBAGE_H____
#define __CCABBAGE_H____

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
class CCabbage : public CPlaceable
{
   CShader* shader;
   public:
   int num;
   CCabbage(float nx, float ny, int width, int height);
   void writeXML(xmlwriter *);
   void moveTo(float,float);
   void setShader(CShader*);
   CMeshSceneObject * setupItem(CShader * shader, CShader * dShader, CCollisionEngine *Engine, CGameplayManager *GameplayManager);
};

#endif
