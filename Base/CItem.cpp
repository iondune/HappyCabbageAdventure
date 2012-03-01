#include "CItem.h"

#include "CIHealth.h"

void CItem::setShader(CShader* shade) {
   CItem::Shader = shade;
}

void CItem::makeItem(float x, float y, float w, float h, ItemType item, CGameplayManager* manager) {
   CItem* temp;

   if (item == health) {
      temp = new CIHealth(x, y, w, h, manager);
   }
   else {
      printf("Invalid item type requested or not implemented.\n");
      return;
   }

   temp->Manager->Items.push_back(temp);
}

void CItem::doRenderable() {
   Renderable->setTranslation(SVector3(Actor->getArea().getCenter().X,Actor->getArea().getCenter().Y, 0));

   if(Actor->getVelocity().X < -0.01f)
      Renderable->setScale(SVector3(-1,1,1));
   else if(Actor->getVelocity().X > 0.01f)
      Renderable->setScale(SVector3(1,1,1));

}
