#ifndef ____CELEMENTENEMY_HEADER_
#define ____CELEMENTENEMY_HEADER_
#include "../CGameplayElement.h"

namespace Enemies {
   enum EEnemyType {
      APPLE					= 0,
      ORANGE				= 1,
      KIWI					= 2,
      GRAPE					= 3,
      FLAME					= 4,
      BLADE					= 5,
      KIWI_PROJECTILE	= 6,
      GRAPE_PROJECTILE	= 7
   };
}

class CElementEnemy : public CGameplayElement {
   public:

   protected:
      CCollisionActor * PhysicsEngineObject; //Override CGameplayElement's but with type CCollisionActor
      CMeshSceneObject * SceneObject; //Override CGameplayElement's but with type CMeshSceneObject

      Enemies::EEnemyType Type;

   public:
      virtual void OnCollision(CCollideable *Object, CCollideable *With);
      virtual void writeXML(xmlwriter *l);

      virtual void setupPhysicsEngineObject()=0;
      virtual void setupSceneObject()=0;

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);

      Enemies::EEnemyType getEnemyType();

      CElementEnemy(SRect2 nArea, Enemies::EEnemyType type);

      virtual void printInformation();
};

class CEnemyLoader {
   public:
      static CElementEnemy *LoadEnemy(SRect2 nArea, Enemies::EEnemyType type); 
};
#endif
