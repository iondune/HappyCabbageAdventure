#ifndef CELEMENTITEMPOWERUP_H_
#define CELEMENTITEMPOWERUP_H_

#include "CElementItem.h"

#include "CPlayerAbility.h"

class CElementItemPowerup : public CElementItem
{
   private:
      Abilities::EAbilityType Type;
      CMeshSceneObject *InnerObject;
      CMeshSceneObject *Container;

   public:
      CElementItemPowerup(SRect2f nArea, int t);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      virtual void OnCollision(const SCollisionEvent& Event);
      void writeXML(xmlwriter *l);

      virtual void printInformation();
};

#endif
