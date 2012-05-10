#include "CElementBlockDeath.h"
#include "CGameLevel.h"
#include "CElementPlayer.h"

CElementBlockDeath::CElementBlockDeath(SRect2f nArea, int D, int T, float R, float S)
: CElementBlock(nArea, D, T), Range(R), Speed(S) {
}

void CElementBlockDeath::OnCollision(const SCollisionEvent& Event) {
   Level.getPlayer().setHealth(0);
   return;
}

void CElementBlockDeath::update(float t) {
   CGameplayElement::update(t);
}

void CElementBlockDeath::writeXML(xmlwriter *l) {
   std::stringstream xValue, yValue, widthValue, heightValue, tagValue, rangeValue, speedValue, depthValue, textureType;
   xValue << Area.Position.X;
   yValue << Area.Position.Y;
   widthValue << Area.Size.X;
   heightValue << Area.Size.Y;
   depthValue << Depth;
   textureType << Texture;
   speedValue << 1;


   tagValue << "DeathBlock";

   l->AddAtributes("isMoving ", speedValue.str());
   l->AddAtributes("texture ", textureType.str());
   l->AddAtributes("depth ", depthValue.str());
   l->AddAtributes("width ", widthValue.str());
   l->AddAtributes("height ", heightValue.str());
   l->AddAtributes("Y ", yValue.str());
   l->AddAtributes("X ", xValue.str());
   l->Createtag(tagValue.str());
   l->CloseLasttag();
}

void CElementBlockDeath::setupSceneObject() {
}

void CElementBlockDeath::setupPhysicsEngineObject() {
   PhysicsEngineObject = Level.getPhysicsEngine().addObject();
   PhysicsEngineObject->setArea(Area);
}

void CElementBlockDeath::printInformation() {
   printf("CElementBlockDeath; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
