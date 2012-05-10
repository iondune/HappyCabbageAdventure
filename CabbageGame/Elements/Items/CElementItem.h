#ifndef ____CELEMENTITEM_HEADER_
#define ____CELEMENTITEM_HEADER_
#include "../CGameplayElement.h"
#include "CElementPlayer.h"

namespace Items {
   enum EItemType {
      HEALTH			   = 0,
      ENERGY				= 1,
      LIFE					= 2,
      SEED					= 3,
      POWERUP				= 4
   };
}

class CElementItem : public CGameplayElement {
   public:

   protected:
      CCollisionActor * PhysicsEngineObject; //Override CGameplayElement's but with type CCollisionActor
      CMeshSceneObject * SceneObject; //Override CGameplayElement's but with type CMeshSceneObject

      Items::EItemType Type;

   public:
      virtual void OnCollision(CCollideable *Object);
      virtual void writeXML(xmlwriter *l);

      virtual void setupPhysicsEngineObject()=0;
      virtual void setupSceneObject()=0;

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);
      virtual void reactToAbility(Abilities::EAbilityType);

      Items::EItemType getItemType();

      CElementItem(SRect2f nArea, Items::EItemType type);

      virtual void printInformation();
};

class CItemLoader {
   public:
      static CElementItem *LoadItem(SRect2f nArea, Items::EItemType type); 
};
#endif
