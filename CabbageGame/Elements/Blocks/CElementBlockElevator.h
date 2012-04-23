#ifndef __CELEVATOR_H____
#define __CELEVATOR_H____

#include "../CGameplayElement.h"
#include "CElementBlock.h"


class CElementBlockElevator : public CElementBlock {
   public:
   private:
      float Range, Speed;

   public:
      virtual void OnCollision(CCollideable *Object);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void writeXML(xmlwriter *);
      virtual void update(float);
      virtual void updateSceneObject(float);

      CElementBlockElevator(SRect2 nArea, int depth, int texture, float r, float s);

      virtual void printInformation();
};

#endif
