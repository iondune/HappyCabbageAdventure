#ifndef CBADGUY_H_
#define CBADGUY_H_

enum EnemyType {
   apple,
   none
};

class CBadGuy {
public:
   float x, y, w, h;
   EnemyType Enemy;
   CGameplayManager* Manager;
   Cabbage::Collider::CActor* Actor;
   CRenderable* Renderable;
   static CShader* Shader;

   static void setShader(CShader* shader);
   static CBadGuy* makeBadGuy(float x, float y, float w, float h, EnemyType enemy, CGameplayManager* manager);

   //Methods that must be implemented in subclasses
   virtual void update();
   virtual void loadMesh();
   virtual void loadActor();
};

#endif /* CBADGUY_H_ */
