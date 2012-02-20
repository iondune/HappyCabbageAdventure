#include "CBadGuy.h"
#include "EApple.h"
#include "EOrange.h"
#include "EKiwi.h"

void CBadGuy::setShader(CShader* shade) {
   CBadGuy::Shader = shade;
}

void CBadGuy::makeBadGuy(float x, float y, float w, float h, EnemyType enemy, CGameplayManager* manager) {
   CBadGuy* temp;

   if (enemy == apple) {
      temp = new EApple(x, y, w, h, manager);
   }

   else if (enemy == orange) {
      temp = new EOrange(x, y, w, h, manager);
   }

   else if (enemy == kiwi) {
      temp = new EKiwi(x, y, w, h, manager);
   }

   else {
      printf("Invalid enemy type requested.\n");
      return;
   }

   temp->Manager->Enemies.push_back(temp);
}
