#include "CItem.h"

#include "CIHealth.h"
#include "CIEnergy.h"

void CItem::setShader(CShader* shade) {
   CItem::Shader = shade;
}

void CItem::makeItem(float x, float y, float w, float h, ItemType item, CGameplayManager* manager) {
   CItem* temp;

   if (item == health) {
      temp = new CIHealth(x, y, w*0.4f, h*0.4f, manager);
      temp->Type = item;
   }

   else if (item == energy) {
	   temp = new CIEnergy(x, y, w*.4f, h * .4f, manager);
	   temp->Type = item;
   }
   else {
      printf("Invalid item type requested or not implemented.\n");
      return;
   }

   temp->Manager->Items.push_back(temp);
}

void CItem::doRenderable() {
   Renderable->setTranslation(SVector3(Actor->getArea().getCenter().X,Actor->getArea().getCenter().Y, 0));
}
