#include "CElementPlayer.h"
#include "CGameLevel.h"
#include "CPlayerView.h"

CElementPlayer::CElementPlayer(SRect2 nArea)
: CGameplayElement((CCollideable *&)PhysicsEngineObject, (ISceneObject *&)SceneObject, nArea), Direction(Right), Action(Standing), Recovering(0.0f), Shaking(0.0f), ShakeFactor(SVector3(0.0f)),
  ISquishable(2.0f, 2.0f){

}

#include "CEventManager.h"
#include "CApplication.h"

void CElementPlayer::updatePlayerAction() {
   if(CApplication::get().getEventManager().IsKeyDown[SDLK_a]) {
      Direction = Left;
      Action = Walking;
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);
   }
   else if(CApplication::get().getEventManager().IsKeyDown[SDLK_d]) {
      Direction = Right;
      Action = Walking;
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveRight);
   }
   else {
      Action = Walking;
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::None);
   }
   if(CApplication::get().getEventManager().IsKeyDown[SDLK_SPACE]) {
      Action = Jumping;
      PhysicsEngineObject->setJumping(true);
   }
   else {
      PhysicsEngineObject->setJumping(false);
   }
}

#include "CPlayerAbilityShield.h"
#include "CPlayerAbilityBlink.h"

void CElementPlayer::checkAbilityKeypress() {
   //Probably a more OO way to set this up, but meh
   usedLeafShield = usedBlink = false;
   for(int i = 0; i < Abilities.size(); i++) {
      if(Abilities[i]->getType() == Abilities::SHIELD)
         usedLeafShield = true;
      if(Abilities[i]->getType() == Abilities::BLINK)
         usedBlink = true;
   }
   if(!usedLeafShield && CApplication::get().getEventManager().IsKeyDown[SDLK_r]) {
      Abilities.push_back(new CPlayerAbilityShield(*this));
   }
   if(!usedBlink && CApplication::get().getEventManager().IsKeyDown[SDLK_e]) {
      Abilities.push_back(new CPlayerAbilityBlink(*this));
   }
}

void CElementPlayer::updateAbilities(float time) {
   std::vector<CPlayerAbility *> AbilityKillList;
   for(int i = 0; i < Abilities.size(); i++) {
      Abilities[i]->updateTime(time);
      Abilities[i]->inUpdatePhysicsEngineObject(time);
      Abilities[i]->inUpdateSceneObject(time);
      if(Abilities[i]->isDead()) {
         printf("dead\n"); 
         AbilityKillList.push_back(Abilities[i]);
      }
   }
   for(int i = 0; i < AbilityKillList.size(); i++) {
      Abilities.erase(std::remove(Abilities.begin(), Abilities.end(), AbilityKillList[i]), Abilities.end());
      delete AbilityKillList[i];
   }
}

void CElementPlayer::updatePhysicsEngineObject(float time) {
   updatePlayerAction();
   checkAbilityKeypress();
}

void CElementPlayer::updateSceneObject(float time) {
   if(Recovering > 0.0f) {
      Recovering -= time;
      if(Recovering <= 0.0f) {
         View->setVisible(true);
         View->setHurt(false);
         Recovering = 0.0f;
      }
      else {
         View->setVisible((int)(ElapsedTime * 50.0f) % 2 != 0);
      }
   }
   if(Shaking > 0.0f) {
      Shaking -= time;
      if((int)((ElapsedTime - (int)ElapsedTime)*20.0f) % 2 == 0)
         ShakeFactor = SVector3((float)rand()/(float)RAND_MAX * 0.3f - 0.15f, (float)rand()/(float)RAND_MAX * 0.3f - 0.15f, 0) / ShakeFactorFactor;
      if(Shaking <= 0.0f) {
         ShakeFactor = SVector3(0.0f);
      }
   }
   updateAbilities(time);
   View->updateView(time);

   Scale = ISquishable::Squish(PhysicsEngineObject->getVelocity());

   SceneObject->setScale(SVector3(Scale.X, Scale.X, Scale.Y));

}

Cabbage::PlayerInformation & CElementPlayer::getStats() {
   return Stats;
}

void CElementPlayer::OnCollision(CCollideable *Object) {
   for(int i = 0; i < Abilities.size(); i++) {
      Abilities[i]->inOnCollision(Object);
   }
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
   PhysicsEngineObject = Level.getPhysicsEngine().addActor();
   PhysicsEngineObject->setArea(Area);
   PhysicsEngineObject->getAttributes().MaxWalk = 7.5f;
   PhysicsEngineObject->CollideableType = COLLIDEABLE_TYPE_PLAYER;
   PhysicsEngineObject->CollideableLevel |= INTERACTOR_SUPERACTORS;
   PhysicsEngineObject->CanCollideWith |= INTERACTOR_SUPERACTORS | INTERACTOR_ITEMS;
}

void CElementPlayer::setupSceneObject() {
   SceneObject = new ISceneObject();
   View = new CPlayerView(SceneObject, Direction, Action, Stats.Health, Area, ShakeFactor, PhysicsEngineObject);
}

bool CElementPlayer::decrementHealth() {
   if(Stats.Health <= 0) {
      return false; //Lose state
   }
   if(Recovering == 0.0f) {
      Stats.Health--; 
      View->removeLeaf();
      View->setHurt(true);
      Recovering = 1.0f;
      return true;
   }
   return false;
   //Else lose state
}
void CElementPlayer::incrementHealth() {
   if(Stats.Health < 5) {
      Stats.Health++; 
      View->addLeaf();
   }
}

void CElementPlayer::setShaking(float time, float factor) {
   Shaking = time;
   ShakeFactorFactor = factor;
}

void CElementPlayer::incrementSeeds() {
   Stats.Seeds++;
   if(Stats.Seeds == 100) {
      Stats.Seeds = 0;
      Stats.Lives++;
   }
}

bool CElementPlayer::isLeafShieldOn() {
   return usedLeafShield;
}

bool CElementPlayer::isBlinkOn() {
   return usedBlink;
}
