#include "CElementItemPowerup.h"
#include "CGameLevel.h"

CElementItemPowerup::CElementItemPowerup(SRect2f nArea, int t) :
   CElementItem(nArea, Items::POWERUP), Type((Abilities::EAbilityType) t) {
}

void CElementItemPowerup::setupPhysicsEngineObject() {
   /* Set up the actor (not actually an actor, since this one doesn't move its position) */
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();
   PhysicsEngineObject->setArea(Area);

   //Set actor attributes
   PhysicsEngineObject->CollideableType = COLLIDEABLE_TYPE_ITEM;
   PhysicsEngineObject->CollideableLevel = INTERACTOR_ITEMS;
   PhysicsEngineObject->CanCollideWith = INTERACTOR_BLOCKS | INTERACTOR_SUPERACTORS;
}

void CElementItemPowerup::setupSceneObject() {
   SceneObject = new CMeshSceneObject();
   SceneObject->setCullingEnabled(false);

   /* Set up container */
   Container = new CMeshSceneObject();
   CMesh *mesh = CMeshLoader::load3dsMesh("Base/container.3ds");
   if(mesh) {
      mesh->resizeMesh(SVector3f(1));
      mesh->centerMeshByExtents(SVector3f(0));
      mesh->calculateNormalsPerFace();
   }
   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   Container->setMesh(mesh);
   Container->setShader(ERP_DEFAULT, "Toon");
   Container->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   SceneObject->addChild(Container);

   InnerObject = new CMeshSceneObject();
   mesh = CMeshLoader::load3dsMesh("crappycabbageDos.3ds");
   if(mesh) {
      mesh->resizeMesh(SVector3f(1));
      mesh->centerMeshByExtents(SVector3f(0));
      mesh->calculateNormalsPerFace();
   }
   else
      printf("ERROR.  MESH DID NOT LOAD PROPERLY.\n");

   InnerObject->setMesh(mesh);
   InnerObject->setShader(ERP_DEFAULT, "Toon");
   InnerObject->setShader(ERP_DEFERRED_OBJECTS, "Deferred/Toon");
   InnerObject->setScale(SVector3f(0.3f));
   SceneObject->addChild(InnerObject);

   //SceneObject.addChild(Container);
   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

void CElementItemPowerup::writeXML(xmlwriter *l) {
    std::stringstream xValue, yValue, isMovingValue, itemType, tagValue;
    xValue << Area.Position.X;
    yValue << Area.Position.Y;
    itemType << Type;
    tagValue << "CPItemPowerup";

    l->AddAtributes("type ", itemType.str());
    l->AddAtributes("Y ", yValue.str());
    l->AddAtributes("X ", xValue.str());
    l->Createtag(tagValue.str());
    l->CloseLasttag();
}

void CElementItemPowerup::OnCollision(CCollideable *Object) {
   if(!Dead && Object == Level.getPlayer().getPhysicsEngineObject()) {
      Level.getPlayer().setCanUseAbility(Type);
      removeFromGame();
      Dead = true;
   }
}
                                                            
//CGameplayElement has an attribute called ElapsedTime, which is updated by CGameplayElement's update function.
void CElementItemPowerup::updatePhysicsEngineObject(float time) {
   return; //It just falls if it's in the air, usually
}

//This is where the renderable would be updated for the more complex enemies
void CElementItemPowerup::updateSceneObject(float time) {
   Container->setTranslation(SVector3f(PhysicsEngineObject->getArea().getCenter(), 0));
   Container->setRotation(SVector3f(-90, 0, 90 + 40*ElapsedTime));

   InnerObject->setTranslation(SVector3f(PhysicsEngineObject->getArea().getCenter(), 0));
   InnerObject->setRotation(SVector3f(-90, 0, 90 - 140*ElapsedTime));
}

void CElementItemPowerup::printInformation() {
   printf("CElementItemPowerup; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]; Type: %d\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y, (int)Type);
}
