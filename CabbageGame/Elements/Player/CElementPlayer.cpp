#include "CElementPlayer.h"
#include "CGameLevel.h"
#include "CPlayerView.h"

CElementPlayer::CElementPlayer(SRect2 nArea)
: CGameplayElement(PhysicsEngineObject, SceneObject, nArea), Direction(Right), Action(Standing) {

}

#include "CEventManager.h"
#include "CApplication.h"

void CElementPlayer::updatePlayerAction() {
   if(CApplication::get().getEventManager().IsKeyDown[SDLK_a]) {
      Direction = Left;
      Action = Walking;
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);
   }
   if(CApplication::get().getEventManager().IsKeyDown[SDLK_d]) {
      Direction = Right;
      Action = Walking;
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveRight);
   }
   if(CApplication::get().getEventManager().IsKeyDown[SDLK_SPACE]) {
      Action = Jumping;
      PhysicsEngineObject->setJumping(true);
   }
}

void CElementPlayer::updatePhysicsEngineObject(float time) {
   updatePlayerAction();
}

void CElementPlayer::updateSceneObject(float time) {
   View->updateView(time);
}

void CElementPlayer::OnCollision(CCollideable *Object, CCollideable *With) {
   return;
}

void CElementPlayer::writeXML(xmlwriter *l) {
   std::stringstream xValue, yValue, widthValue, heightValue, tagValue;
   xValue << Area.Position.X;
   yValue << Area.Position.Y;
   widthValue << Area.Size.X;
   heightValue << Area.Size.Y;

   tagValue << "CCabbage";

   l->AddAtributes("width ", widthValue.str());
   l->AddAtributes("height ", heightValue.str());
   l->AddAtributes("Y ", yValue.str());
   l->AddAtributes("X ", xValue.str());
   l->Createtag(tagValue.str());
   l->CloseLasttag();
}

void CElementPlayer::setupPhysicsEngineObject() {
   //TODO: Fix this once we have a way to refer to the physics engine
   PhysicsEngineObject = (new CCollisionEngine())->addActor();
   PhysicsEngineObject->setArea(Area);
   PhysicsEngineObject->getAttributes().MaxWalk = 3.5f;
   PhysicsEngineObject->CollideableType = COLLIDEABLE_TYPE_PLAYER;
   PhysicsEngineObject->CollideableLevel |= INTERACTOR_SUPERACTORS;
   PhysicsEngineObject->CanCollideWith |= INTERACTOR_SUPERACTORS | INTERACTOR_ITEMS;
}

void CElementPlayer::setupSceneObject() {
   SceneObject = new ISceneObject();
   View = new CPlayerView(SceneObject, Direction, Action, Stats.Health, Area);
}
