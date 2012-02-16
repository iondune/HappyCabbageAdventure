#ifndef CBADGUY_H_
#define CBADGUY_H_

#include "CGameplayManager.h"
#include "../CabbageScene/CabbageScene.h"
#include "../CabbageFramework/CabbageFramework.h"

enum EnemyType {
   apple,
   orange
};

class CBadGuy {
public:
   float x, y, w, h;
   EnemyType Enemy;
   CGameplayManager* Manager;
   Cabbage::Collider::CActor* Actor;
   CRenderable* Renderable;
   CShader* Shader;

   void setShader(CShader* shader);
   static void makeBadGuy(float x, float y, float w, float h, EnemyType enemy, CGameplayManager* manager);

   //Methods that must be implemented in subclasses
   virtual void update() =0;
   virtual void loadMesh() =0;
   virtual void loadActor() =0;
};

#endif /* CBADGUY_H_ */
