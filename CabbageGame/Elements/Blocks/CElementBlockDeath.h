#ifndef __CELEMENTBLOCKDEATH_H____
#define __CELEMENTBLOCKDEATH_H____

#include "../CGameplayElement.h"
#include "CElementBlock.h"


class CElementBlockDeath : public CElementBlock {
   public:
   private:
      float Range, Speed;

   public:
      virtual void OnCollision(const SCollisionEvent& Event);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void writeXML(xmlwriter *);
      virtual void update(float);

      CElementBlockDeath(SRect2f nArea, int depth, int texture, float r, float s);

      virtual void printInformation();
};

#endif
