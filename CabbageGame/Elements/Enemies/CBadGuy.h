#ifndef CBADGUY_H_
#define CBADGUY_H_

#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "../CabbageCollider/CCollisionEngine.h"
#include "../CabbageSound/sound.h"

class CGameplayManager;

class CBadGuy {
public:

   enum EnemyType {
      apple,
      orange,
      kiwi,
      grape,
      flame,
      blade,
      pKiwi,
      pGrape
   };

   float x, y, w, h;
   EnemyType Enemy;
   CGameplayManager* Manager;
   CCollisionActor* Actor;
   CMeshSceneObject* Renderable;
   int KilledBy;
   //Currently the Shader stuff appears to be acting up.
   CShader* Shader;

   void setShader(CShader* shader);
   static void makeBadGuy(float x, float y, float w, float h, EnemyType enemy, CGameplayManager* manager, int env);

   //Methods that must be implemented in subclasses
   virtual void update(float const TickTime) =0;
   virtual void loadMesh() =0;
   virtual void loadActor() =0;
   virtual void doRenderable();
};

#endif /* CBADGUY_H_ */
