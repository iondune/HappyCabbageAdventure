#ifndef __CBREAKABLE_H____
#define __CBREAKABLE_H____

#include "../CGameplayElement.h"
#include "CElementBlock.h"


class CElementBlockBreakable : public CElementBlock {
   private:
      void explode();

   public:
      virtual void OnCollision(CCollideable *Object);
      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();
      virtual void writeXML(xmlwriter *);

      CElementBlockBreakable(SRect2 nArea); //So we don't need a block loader
      virtual void reactToAbility(Abilities::EAbilityType ability); 

      virtual void printInformation();
};

#endif
