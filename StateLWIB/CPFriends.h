#ifndef __CPFRIENS_H____
#define __CPFRIENDS_H____

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
class CPFriends : public CPlaceable
{
   public:
   Cabbage::Collider::CElevator* elevator;
   int Speed, Range;
   CPFriends(float nx, float ny, int type);
   int z, t;
   void writeXML(xmlwriter *);
   void moveTo(float,float);
   CMeshSceneObject * setupItem(CShader * shader, CShader * dShader, Cabbage::Collider::CEngine *Engine, CGameplayManager *GameplayManager);
};

#endif
