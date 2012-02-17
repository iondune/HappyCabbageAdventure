#include "CBadGuy.h"
#include "EApple.h"

void CBadGuy::setShader(CShader* shade) {
   CBadGuy::Shader = shade;
}

void CBadGuy::makeBadGuy(float x, float y, float w, float h, EnemyType enemy, CGameplayManager* manager) {
   EApple* temp;

   if (enemy == apple) {
      temp = new EApple(x, y, w, h, manager);
   }

   else {
      printf("Invalid enemy type requested.\n");
      return;
   }

   temp->Manager->Test.push_back(*temp);
}
