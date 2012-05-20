#include "CBadGuy.h"
#include "EApple.h"
#include "EOrange.h"
#include "EKiwi.h"
#include "EFlame.h"
#include "EBlade.h"
#include "PKiwi.h"
#include "EGrape.h"
#include "PGrape.h"

void CBadGuy::setShader(CShader* shade) {
   CBadGuy::Shader = shade;
}

void CBadGuy::makeBadGuy(float x, float y, float w, float h, EnemyType enemy, CGameplayManager* manager, int env) {
   CBadGuy* temp;

   if (enemy == apple) {
      temp = new EApple(x, y, w, h, manager, env);
   }

   else if (enemy == orange) {
      temp = new EOrange(x, y, w, h, manager, env);
   }

   else if (enemy == kiwi) {
      int direction = 0;
      temp = new EKiwi(x, y, w, h, manager, direction);
   }

   else if (enemy == flame) {
      temp = new EFlame(x, y, w, h, manager);
   }

   else if (enemy == blade) {
      temp = new EBlade(x, y, w, h, manager);
   }

   else if (enemy == pKiwi) {
      temp = new PKiwi(x, y, w, h, manager);
   }

   else if (enemy == grape) {
      temp = new EGrape(x, y, w, h, manager);
   }

   else if (enemy == pGrape) {
      temp = new PGrape(x, y, w, h, manager);
   }

   else {
      printf("Invalid enemy type requested.\n");
      return;
   }

   temp->Manager->Enemies.push_back(temp);
}

void CBadGuy::doRenderable() {

   Renderable->setTranslation(SVector3f(Actor->getArea().getCenter().X,Actor->getArea().getCenter().Y, 0));

   if(Actor->getVelocity().X < -0.01f)
      Renderable->setScale(SVector3f(-1,1,1));
   else if(Actor->getVelocity().X > 0.01f)
      Renderable->setScale(SVector3f(1,1,1));

}
