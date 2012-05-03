#ifndef ____CELEMENTENEMY_HEADER_
#define ____CELEMENTENEMY_HEADER_
#include "../CGameplayElement.h"
#include "CElementPlayer.h"
#include "sound.h"

namespace Enemies {
   enum EEnemyType {
      APPLE					 = 0,
      ORANGE				 = 1,
      KIWI					 = 2,
      GRAPE					 = 3,
      FLAME					 = 4,
      BLADE					 = 5,
      KIWI_PROJECTILE	 = 6,
      GRAPE_PROJECTILE	 = 7,
      GIANT_APPLE        = 8,
      PEAR               = 9,
      BANNANA            = 10,
      BANNANA_PROJECTILE = 11,
      CHERRY             = 12,
      CHERRY_PROJECTILE  = 13
   };
}

class CElementEnemy : public CGameplayElement {
   public:

   protected:
      CCollisionActor * PhysicsEngineObject; //Override CGameplayElement's but with type CCollisionActor
      CMeshSceneObject * SceneObject; //Override CGameplayElement's but with type CMeshSceneObject

      Enemies::EEnemyType Type;
      void dieWithSeeds();
      void deathAnimation();

      float TempTime;

      bool AnimatingDeath;

      int MaxHealth, CurHealth;

   public:
      virtual void OnCollision(CCollideable *Object);
      virtual void writeXML(xmlwriter *l);

      virtual void setupPhysicsEngineObject()=0;
      virtual void setupSceneObject()=0;

      virtual void updatePhysicsEngineObject(float time);
      virtual void updateSceneObject(float time);
      virtual void reactToAbility(Abilities::EAbilityType);

      Enemies::EEnemyType getEnemyType();

      CElementEnemy(SRect2 nArea, Enemies::EEnemyType type);

      virtual int getHealth();
      virtual int takeDamage(int amount);
      virtual int heal(int amount);

      virtual void printInformation();
};

class CEnemyLoader {
   public:
      static CElementEnemy *LoadEnemy(SRect2 nArea, Enemies::EEnemyType type); 
};
#endif
