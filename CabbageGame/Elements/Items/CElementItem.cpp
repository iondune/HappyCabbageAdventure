#include "CElementItem.h"
#include "CGameLevel.h"

#include "CElementItemHealth.h"
#include "CElementItemEnergy.h"
#include "CElementItemLife.h"
#include "CElementItemSeed.h"

//Generic item, for usage in the LWIB, I guess.
CElementItem::CElementItem(SRect2 nArea, Items::EItemType type)
: CGameplayElement((CCollideable *&)PhysicsEngineObject, (ISceneObject *&)SceneObject, nArea), Type(type) {
}

//Item created by factory
CElementItem *CItemLoader::LoadItem(SRect2 nArea, Items::EItemType type) {
   switch(type) {
   case Items::HEALTH:
      return new CElementItemHealth(nArea);
      break;
   case Items::ENERGY:
      return new CElementItemEnergy(nArea);
      break;
   case Items::LIFE:
      return new CElementItemLife(nArea);
      break;
   case Items::SEED:
      return new CElementItemSeed(nArea);
      break;
   default:
      fprintf(stderr, "Error: unknown item type %d\n", type);
      exit(1);
      break;
   }
   return NULL;
}

void CElementItem::writeXML(xmlwriter *l) {
    std::stringstream xValue, yValue, isMovingValue, itemType, tagValue;
    xValue << Area.Position.X;
    yValue << Area.Position.Y;
    itemType << Type;
    tagValue << "CPItem";

    l->AddAtributes("type ", itemType.str());
    l->AddAtributes("Y ", yValue.str());
    l->AddAtributes("X ", xValue.str());
    l->Createtag(tagValue.str());
    l->CloseLasttag();
}

void CElementItem::OnCollision(CCollideable *Object) {
   fprintf(stderr, "Error: collision on generic item type %d (perhaps the CElementItem::OnCollision function wasn't overridden?).\n", Type);
   exit(1);
}

void CElementItem::updatePhysicsEngineObject(float time) {
   fprintf(stderr, "Error: updatePhysicsEngineObject on generic item type %d (perhaps the CElementItem::updatePhysicsEngineObject function wasn't overridden?).\n", Type);
   exit(1);
}

void CElementItem::updateSceneObject(float time) {
   return;
}

Items::EItemType CElementItem::getItemType() {
   return Type;
}

void CElementItem::printInformation() {
   printf("CElementItem; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]; Type: %d\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y, Type);
}

void CElementItem::reactToAbility(Abilities::EAbilityType Ability) {
   SVector2 PlayerVelocity = ((CCollisionActor*)Level.getPlayer().getPhysicsEngineObject())->getVelocity();
   switch(Ability) {
      case Abilities::SHIELD:
         ((CCollisionActor*)PhysicsEngineObject)->setImpulse((PlayerVelocity + SVector2(0.0f, 2.5f)) * 3.0f, 0.01f);
         //dieWithSeeds();
         break;
      default:
         break;
   }
}
