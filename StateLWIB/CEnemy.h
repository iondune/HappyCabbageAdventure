#ifndef __CENEMY_H____
#define __CENEMY_H____

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
class CEnemy : public CPlaceable
{
   CShader* shader;
   public:
   int num, env;
   CEnemy(float nx, float ny, int width, int height, int type, int env);
   void writeXML(xmlwriter *);
   void moveTo(float,float);
   void setShader(CShader*);
   CMeshSceneObject * setupItem(CShader * shader, CShader * dShader, CCollisionEngine *Engine, CGameplayManager *GameplayManager);
};

#endif
