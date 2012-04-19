#ifndef ____CELEMENTENEMY_HEADER_
#define ____CELEMENTENEMY_HEADER_
#include "../CGameplayElement.h"

class CElementEnemy : public CGameplayElement {
   int Type;

   virtual void update(float f);
   virtual void setupObjects();
   virtual void OnCollision(CCollideable *Object, CCollideable *With);
   virtual void writeXML(xmlwriter *l);

   CElementEnemy(SRect2 nArea, int type);
};

class CEnemyLoader {
   static CElementEnemy *LoadEnemy(SRect2 nArea, int type, int texture); 
};
#endif
