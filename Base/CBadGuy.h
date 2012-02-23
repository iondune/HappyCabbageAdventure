#ifndef CBADGUY_H_
#define CBADGUY_H_

#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"
#include "../CabbageCollider/CEngine.h"
#include "../CabbageSound/sound.h"

class CGameplayManager;

class CBadGuy {
public:

   enum EnemyType {
      apple,
      orange,
      kiwi,
      grape,
      flame
   };

   float x, y, w, h;
   EnemyType Enemy;
   CGameplayManager* Manager;
   Cabbage::Collider::CActor* Actor;
   CMeshSceneObject* Renderable;
   //Currently the Shader stuff appears to be acting up.
   CShader* Shader;

   void setShader(CShader* shader);
   static void makeBadGuy(float x, float y, float w, float h, EnemyType enemy, CGameplayManager* manager);

   //Methods that must be implemented in subclasses
   virtual void update(float const TickTime) =0;
   virtual void loadMesh() =0;
   virtual void loadActor() =0;
   virtual void doRenderable();
};

#endif /* CBADGUY_H_ */
