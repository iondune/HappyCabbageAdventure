#include "CElementBlockElevator.h"
#include "CGameLevel.h"

CElementBlockElevator::CElementBlockElevator(SRect2f nArea, int D, int T, float R, float S, int style)
: CElementBlock(nArea, D, T), Range(R), Speed(S), Style(style), OriginalArea(nArea.Position, nArea.Size) {
}

void CElementBlockElevator::OnCollision(const SCollisionEvent& Event) {
   return;
}

void CElementBlockElevator::update(float time) {
   ElapsedTime += time;
   if(!Dead) {
      Area = PhysicsEngineObject->getArea();
      if(Level.shouldRender()) {
         updateSceneObject(time);
      }
   }
}

void CElementBlockElevator::writeXML(xmlwriter *l) {
   std::stringstream xValue, yValue, widthValue, heightValue, tagValue, rangeValue, speedValue, depthValue, textureType, eleType;
   xValue << OriginalArea.Position.X;
   yValue << OriginalArea.Position.Y;
   widthValue << OriginalArea.Size.X;
   heightValue << OriginalArea.Size.Y;
   depthValue << Depth;
   textureType << Texture;
   speedValue << Speed;
   rangeValue << Range;
   eleType << Style;

   tagValue << "CBlock";
   l->AddAtributes("Style", eleType.str());
   l->AddAtributes("speed ", rangeValue.str());
   l->AddAtributes("range ", rangeValue.str());
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
   
   Ele->Style = (EElevatorType)Style; // 0 horizontal, 1 vertical, 2 circular  
   Ele->Range = Range;
   Ele->Speed = Speed;
}

void CElementBlockElevator::updateSceneObject(float time) {
   SceneObject->setTranslation(SVector3f((Area.Position.X+(Area.Position.X+Area.Size.X))/2, (Area.Position.Y+(Area.Position.Y+Area.Size.Y))/2, 0));
}

void CElementBlockElevator::setupSceneObject() {
   if(!Level.isLoaded())
      setupPhysicsEngineObject();
   CElementBlock::setupSceneObject();
   CApplication::get().getSceneManager().removeSceneObject(SceneObject);
   CApplication::get().getSceneManager().addSceneObject(SceneObject);
}

void CElementBlockElevator::printInformation() {
   printf("CElementBlockElevator; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y);
}
