#ifndef __CPLAYER_H____
#define __CPLAYER_H____

#include "../CGameplayElement.h"
#include "CEventManager.h"

#define NUM_CABBAGE_MESH_STATES 5

namespace Cabbage {
   //Information kept between levels
   class PlayerInformation {
      public:
         int Health;
         int Energy;
         int Lives;
         int Seeds;

         PlayerInformation() : Health(5), Energy(3), Lives(3), Seeds(0) {

         }
   };

   //Information kept in the Overworld
   class LevelInformation {
      public:
         int LevelsCompleted;
         int LastLevelEntered;

         LevelInformation() : LevelsCompleted(0), LastLevelEntered(0) {

         }
   };
}

class CElementPlayer : public CGameplayElement, public CApplicationEventReceiver {
   private:
      CCollisionActor * PhysicsEngineObject; //Override CGameplayElement's but with type CObject
      ISceneObject * SceneObject; //Override CGameplayElement's. This will contain the cabbage and its shadow 

      ISceneObject HurtCabbageArr[NUM_CABBAGE_MESH_STATES];
      ISceneObject NormalCabbageArr[NUM_CABBAGE_MESH_STATES];

      Cabbage::PlayerInformation Stats;

   public:
      CElementPlayer(SRect2 nArea);

      //CGameplayElement functions
      virtual void OnCollision(CCollideable *Object, CCollideable *With);

      virtual void setupPhysicsEngineObject();
      virtual void setupSceneObject();

      virtual void updatePhysicsEngineObject(float);
      virtual void updateSceneObject(float);

      virtual void writeXML(xmlwriter *);

      //Keyboard event functions
      //void OnKeyboardEvent(SKeyboardEvent const & Event);
};

#endif
