#ifndef __CBLOCK_H____
#define __CBLOCK_H____

#include "../CGameplayElement.h"


class CElementBlock : public CGameplayElement {
   protected:
      CCollisionObject * PhysicsEngineObject; //Override CGameplayElement's but with type CObject
      CMeshSceneObject * SceneObject; //Override CGameplayElement's but with type CMeshSceneObject
      int Depth, Texture;
   public:
      virtual void OnCollision(const SCollisionEvent& Event);

      virtual void setupSceneObject();
      virtual void setupPhysicsEngineObject();

      virtual void update(float);
      virtual void updatePhysicsEngineObject(float);
      virtual void updateSceneObject(float);
      virtual void reactToAbility(Abilities::EAbilityType ability); 

      virtual void writeXML(xmlwriter *);
      CElementBlock(SRect2f nArea, int depth, int texture);

      virtual void printInformation();
};

#endif
