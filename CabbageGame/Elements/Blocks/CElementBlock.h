#ifndef __CBLOCK_H____
#define __CBLOCK_H____

#include "../CGameplayElement.h"

class CElementBlock : public CGameplayElement {
   private:
      CCollisionObject * PhysicsEngineObject; //Override CGameplayElement's but with type CObject
      CMeshSceneObject * SceneObject; //Override CGameplayElement's but with type CMeshSceneObject
      int Depth, Texture;
   public:
      virtual void update(float);
      virtual void setupObjects();
      virtual void OnCollision(CCollideable *Object, CCollideable *With);

      virtual void writeXML(xmlwriter *);
      CElementBlock(SRect2 nArea, int depth, int texture);
};

#endif
