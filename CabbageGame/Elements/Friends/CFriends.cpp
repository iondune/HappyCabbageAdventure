#include "CFriends.h"

#include "CFDerp.h"

void CFriends::setShader(CShader* shade) {
   CFriends::Shader = shade;
}

void CFriends::makeFriend(float x, float y, float w, float h, FriendType item, CGameplayManager* manager) {
   CFriends* temp;

   if (item == Derp) {
      temp = new CFDerp(x, y, w, h, manager);
      temp->Type = item;
   }

   else {
      printf("Invalid item type requested or not implemented.\n");
      return;
   }

   temp->Manager->Friends.push_back(temp);
}

void CFriends::doRenderable() {
   Renderable->setTranslation(SVector3(Actor->getArea().getCenter().X,Actor->getArea().getCenter().Y, 0));
}
