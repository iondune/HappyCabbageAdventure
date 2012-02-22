#ifndef __CCABBAGE_H____
#define __CCABBAGE_H____

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
class CCabbage : public CPlaceable
{
   CShader* shader;
   public:
   int num;
   CCabbage(float nx, float ny, int width, int height);
   void writeXML(xmlwriter *);
   void moveTo(float,float);
   void setShader(CShader*);
   CMeshSceneObject * setupItem(CShader * shader, Cabbage::Collider::CEngine *Engine, CGameplayManager *GameplayManager, Cabbage::Collider::CActor *Player);
};

#endif
