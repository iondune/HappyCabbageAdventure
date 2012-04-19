#ifndef __CFlag_H____
#define __CFlag_H____

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
class CFlag : public CPlaceable
{
   CShader* shader;
   public:
   int num;
   int isSecret;
   CFlag(float nx, float ny, int width, int height, int secret);
   void writeXML(xmlwriter *);
   void moveTo(float,float);
   void setShader(CShader*);
   CMeshSceneObject * setupItem(CShader * shader, CShader * dShader, CCollisionEngine *Engine, CGameplayManager *GameplayManager);
};

#endif