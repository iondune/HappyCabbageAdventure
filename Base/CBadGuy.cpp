#include "CBadGuy.h"

void CBadGuy::setShader(CShader* shade) {
   Shader = shade;
}

CBadGuy* CBadGuy::makeBadGuy(float x, float y, float w, float h, EnemyType enemy, CGameplayManager* manager) {
   CBadGuy temp = 0;

   if (enemy == EnemyType::apple) {
      temp = new EApple(x, y, w, h, manager);
   }

   else {
      printf("Invalid enemy type requested.\n");
   }

   return temp;
}

CBadGuy::~CBadGuy() {
   // TODO Auto-generated destructor stub
}

