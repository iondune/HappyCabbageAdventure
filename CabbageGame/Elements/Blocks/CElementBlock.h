#ifndef __CBLOCK_H____
#define __CBLOCK_H____

#include "../CGameplayElement.h"


class CElementBlock : public CGameplayElement {
   protected:
      CCollisionObject * PhysicsEngineObject; //Override CGameplayElement's but with type CObject
      CMeshSceneObject * SceneObject; //Override CGameplayElement's but with type CMeshSceneObject
      int Depth, Texture;
   public:
      virtual void OnCollision(CCollideable *Object);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void update(float);
      virtual void updatePhysicsEngineObject(float);
      virtual void updateSceneObject(float);
      virtual void reactToAbility(Abilities::EAbilityType ability); 

      virtual void writeXML(xmlwriter *);
      CElementBlock(SRect2 nArea, int depth, int texture);

      virtual void printInformation();
};

#endif
