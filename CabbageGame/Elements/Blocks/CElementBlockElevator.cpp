#include "CElementBlockElevator.h"
#include "CGameLevel.h"

CElementBlockElevator::CElementBlockElevator(SRect2f nArea, int D, int T, float R, float S)
: CElementBlock(nArea, D, T), Range(R), Speed(S) {
}

void CElementBlockElevator::OnCollision(const SCollisionEvent& Event) {
   return;
}

void CElementBlockElevator::update(float t) {
   CGameplayElement::update(t);
}

void CElementBlockElevator::writeXML(xmlwriter *l) {
   std::stringstream xValue, yValue, widthValue, heightValue, tagValue, rangeValue, speedValue, depthValue, textureType;
   xValue << Area.Position.X;
   yValue << Area.Position.Y;
   widthValue << Area.Size.X;
   heightValue << Area.Size.Y;
   depthValue << Depth;
   textureType << Texture;
   speedValue << 1;


   tagValue << "CBlock";

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

void CElementBlockElevator::setupPhysicsEngineObject() {
   CCollisionElevator *Ele;
   PhysicsEngineObject = Ele = Level.getPhysicsEngine().addElevator();
   PhysicsEngineObject->setArea(Area);

   Ele->Range = Range;
   Ele->Speed = Speed;
}

void CElementBlockElevator::updateSceneObject(float time) {
   SceneObject->setTranslation(SVector3f((Area.Position.X+(Area.Position.X+Area.Size.X))/2, (Area.Position.Y+(Area.Position.Y+Area.Size.Y))/2, 0));
}

void CElementBlockElevator::setupSceneObject() {
   CElementBlock::setupSceneObject();
   CApplication::get().getSceneManager().removeSceneObject(SceneObject);
   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

void CElementBlockElevator::printInformation() {
   printf("CElementBlockElevator; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
