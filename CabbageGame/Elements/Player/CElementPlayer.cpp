#include "CElementPlayer.h"
#include "CGameLevel.h"
#include "CPlayerView.h"
#include "CElementPlayer.h"

CElementPlayer::CElementPlayer(SRect2 nArea)
: CGameplayElement((CCollideable *&)PhysicsEngineObject, (ISceneObject *&)SceneObject, nArea), Direction(Right), Action(Standing), Recovering(0.0f), Shaking(0.0f), ShakeFactor(SVector3(0.0f)),
  ISquishable(2.0f, 2.0f), AllowMovement(true), PlayJump(true), Victory(false), VictoryTime(0.0f), ShakeFactorFactor(1000.0f), MoveKeyDelay(0.0f) {
   setupSoundEffects();
}

#include "CEventManager.h"
#include "CApplication.h"
#include "CPlayerAbilityDash.h"

void CElementPlayer::updatePlayerAction() {
   if(Victory)
      return;
   if(!AllowMovement) {
      Action = Standing;
      PhysicsEngineObject->setJumping(false);
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::None);
      return;
   }
   if(CApplication::get().getEventManager().IsKeyDown[SDLK_a]) {
      if(Action == Standing) {
         if(MoveKeyDelay > 0.0f) {
            if(!used(Abilities::DASH)) {
               Abilities.push_back(new CPlayerAbilityDash(*this));
            }
         }
         else
            MoveKeyDelay = 0.3f;
      }
      else if(MoveKeyDelay <= 0.0f) {
         MoveKeyDelay = 0.0f;
      }
      Direction = Left;
      Action = Walking;
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);
   }
   else if(CApplication::get().getEventManager().IsKeyDown[SDLK_d]) {
      if(Action == Standing) {
         if(MoveKeyDelay > 0.0f) {
            if(!used(Abilities::DASH)) {
               Abilities.push_back(new CPlayerAbilityDash(*this));
            }
         }
         else
            MoveKeyDelay = 0.3f;
      }
      else if(MoveKeyDelay <= 0.0f) {
         MoveKeyDelay = 0.0f;
      }
      Direction = Right;
      Action = Walking;
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveRight);
   }
   else {
      Action = Standing;
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::None);
   }
   if(CApplication::get().getEventManager().IsKeyDown[SDLK_SPACE]) {
      Action = Jumping;
      PhysicsEngineObject->setJumping(true);

      //Check if jumping should be enabled.
      if (PhysicsEngineObject->getVelocity().Y == 0.f)
               PlayJump = true;
      if (PlayJump) {
         Mix_PlayChannel(-1, jump, 0);
         PlayJump = false;
      }
   }
   else {
      PhysicsEngineObject->setJumping(false);
   }
}

#include "CPlayerAbilityShield.h"
#include "CPlayerAbilityBlink.h"
#include "CPlayerAbilityLaser.h"

bool CElementPlayer::used(Abilities::EAbilityType a) {
   return (usedAbility.find(a) != usedAbility.end());
}

//Only works for single-use abilities
CPlayerAbility *CElementPlayer::getAbility(Abilities::EAbilityType a) {
   if(usedAbility.find(a) == usedAbility.end())
      return NULL;
   else return Abilities[(*usedAbility.find(a)).second];
}

void CElementPlayer::checkAbilityKeypress() {
   if (!Victory)
      AllowMovement = true;
   usedAbility.clear();
   for(unsigned int i = 0; i < Abilities.size(); i++) {
      usedAbility[Abilities[i]->getType()] = i;
   }
   /* Laser */
   if(!used(Abilities::LASER)) {
      if(CApplication::get().getEventManager().IsKeyDown[SDLK_l]) {
         Abilities.push_back(new CPlayerAbilityLaser(*this));
      }
   }
   else {
      getAbility(Abilities::LASER)->checkKey(CApplication::get().getEventManager().IsKeyDown[SDLK_l]);
   }
   if(Stats.Energy <= 0)
      return;
   /* Shield */
   if(!used(Abilities::SHIELD)) {
      if(CApplication::get().getEventManager().IsKeyDown[SDLK_k]) {
         Abilities.push_back(new CPlayerAbilityShield(*this));
      }
   }
   else {
      getAbility(Abilities::SHIELD)->checkKey(CApplication::get().getEventManager().IsKeyDown[SDLK_k]);
   }
   /* Dash */
   if(!used(Abilities::DASH)) {
   }
   else {
      getAbility(Abilities::DASH)->checkKey(CApplication::get().getEventManager().IsKeyDown[SDLK_a] || CApplication::get().getEventManager().IsKeyDown[SDLK_d]);
   }
   if(!used(Abilities::BLINK) && CApplication::get().getEventManager().IsKeyDown[SDLK_e]) {
      Abilities.push_back(new CPlayerAbilityBlink(*this));
   }
}

void CElementPlayer::updateAbilities(float time) {
   if(MoveKeyDelay > 0.0f)
      MoveKeyDelay -= time;
   std::vector<CPlayerAbility *> AbilityKillList;
   for(unsigned int i = 0; i < Abilities.size(); i++) {
      Abilities[i]->updateTime(time);
      Abilities[i]->inUpdatePhysicsEngineObject(time);
      Abilities[i]->inUpdateSceneObject(time);
      if(Abilities[i]->isDead()) {
         AbilityKillList.push_back(Abilities[i]);
      }
   }
   for(unsigned int i = 0; i < AbilityKillList.size(); i++) {
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
         ShakeFactorFactor = 1000.0f;
      }
   }

   SVector2 leftOfPlayer = SVector2(Area.Position.X, Area.getCenter().Y);
   SVector2 rightOfPlayer = SVector2(Area.Position.X + Area.Size.X, Area.getCenter().Y);
   CCollisionObject *l, *r;
   l = Level.getPhysicsEngine().getObjectBelow(leftOfPlayer);
   r = Level.getPhysicsEngine().getObjectBelow(rightOfPlayer);

   View->setShadowHeights(Level.getPhysicsEngine().getHeightBelow(leftOfPlayer), Level.getPhysicsEngine().getHeightBelow(rightOfPlayer));
   // If the cabbage is hanging over an edge, there might not be an object below it, so this check is necessary
   View->setCutoffPoint(l?l->getArea():SRect2(-50.0f, -50.0f, 0.0f, 0.0f),r?r->getArea():SRect2(-50.0f, -50.0f, 0.0f, 0.0f));

   updateAbilities(time);
   if (Victory)
      playLevelVictory(time);
   else {
      View->updateView(time);
      Scale = ISquishable::Squish(PhysicsEngineObject->getVelocity());
      View->setCabbageScale(SVector3(Scale.X, Scale.X, Scale.Y));
   }

}

Cabbage::PlayerInformation & CElementPlayer::getStats() {
   return Stats;
}

void CElementPlayer::OnCollision(CCollideable *Object) {
   for(unsigned int i = 0; i < Abilities.size(); i++) {
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
   PhysicsEngineObject->getAttributes().MaxWalk = 3.5f;
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
      //decrementLives();
      return false; //Lose state
   }
   if(Recovering == 0.0f) {
      Stats.Health--; 
      View->removeLeaf();
      View->setHurt(true);
      Recovering = 1.0f;
      Mix_PlayChannel(-1, takeDmg, 0);
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

void CElementPlayer::setHealth(int amount) {
   amount = std::min(Stats.MaxHealth, amount);
   Stats.Health = std::max(0, amount);
}

void CElementPlayer::incrementLives() {
   Stats.Lives++;
}

void CElementPlayer::decrementLives() {
   if (Stats.Lives > 1) {
      Stats.Lives--;
   }

   else {
      /*Death code here*/
   }
}

void CElementPlayer::changeEnergy(int amount) {
   Stats.Energy = std::min(Stats.Energy + amount, Stats.MaxEnergy);
}

void CElementPlayer::setShaking(float time, float factor) {
   if(time > Shaking)
      Shaking = time;
   if(factor < ShakeFactorFactor)
      ShakeFactorFactor = factor;
}

void CElementPlayer::incrementSeeds() {
   Stats.Seeds++;
   if(Stats.Seeds == 100) {
      Stats.Seeds = 0;
      Stats.Lives++;
   }
}

std::map<Abilities::EAbilityType, int> &CElementPlayer::getAbilityStatus() {
   return usedAbility;
}

void CElementPlayer::setupSoundEffects() {

   string MusicDirectory = "../Media/Music/";
   string temp;

   if(Mix_OpenAudio(22050, AUDIO_S16, 2, 2048))
      fprintf(stderr, "Could not open audio!\n");

   temp = MusicDirectory + "jump.wav";
   jump = Mix_LoadWAV(temp.c_str());

   if (!jump) {
      printf("Mix_LoadWAV: %s\n", Mix_GetError());
      exit(1);
   }

   temp = MusicDirectory + "takeDmg.wav";
   takeDmg = Mix_LoadWAV(temp.c_str());

   temp = MusicDirectory + "chargeLaser1.wav";
   chargeLaser1 = Mix_LoadWAV(temp.c_str());

   temp = MusicDirectory + "chargeLaser2.wav";
   chargeLaser2 = Mix_LoadWAV(temp.c_str());

   temp = MusicDirectory + "fireLaser3.wav";
   fireLaser = Mix_LoadWAV(temp.c_str());
}

void CElementPlayer::setVictoryFlag(bool value) {
	Victory = value;
}

void CElementPlayer::setAllowMovement(bool value) {
	AllowMovement = value;
}

#include "COverworldState.h"
void CElementPlayer::playLevelVictory(float time) {
   SVector3 curRotation = View->getCabbageSceneObject().getRotation();
   SVector2 curLocation = SVector2 (Area.getCenter().X - .5f, Area.getCenter().Y - .5f);

   if (VictoryTime > .00f && VictoryTime < .07f) {
      Action = Standing;
      PhysicsEngineObject->setJumping(false);
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::None);
   }

   if (VictoryTime > .05f && VictoryTime < .07f) {
      Direction = Right;
      Action = Walking;
      PhysicsEngineObject->setJumping(true);
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveRight);
   }
   else if (VictoryTime > .07f && VictoryTime < .12f) {
      PhysicsEngineObject->setJumping(false);
   }
   else if (VictoryTime > .72f && VictoryTime < .76f) {
      Action = Jumping;
      PhysicsEngineObject->setJumping(true);
   }
   //Fireworks.  Ignoring for now. I can do that.  : D
   /*else if (VictoryTime > .76f && VictoryTime < 1.f) {
      if (!f1) {
         SVector3 flagPosition = renderFlag->getTranslation();

         f1 = new CParticleEngine(SVector3(flagPosition.X, flagPosition.Y, flagPosition.Z + .5f), 40, 2.f, HURT_PARTICLE);  //Would like to change these later so not leaves. Fine for now
         f2 = new CParticleEngine(SVector3(flagPosition.X + 4.f, flagPosition.Y, flagPosition.Z + .5f), 40, 2.f, HURT_PARTICLE);
         f3 = new CParticleEngine(SVector3(flagPosition.X + 8.f, flagPosition.Y, flagPosition.Z + .5f), 40, 2.f, HURT_PARTICLE);
      }
   }*/
   else if (VictoryTime > 1.f && VictoryTime < 1.2f) {
      Action = Walking;
      PhysicsEngineObject->setJumping(false);
   }
   else if (VictoryTime > 1.5f && VictoryTime < 2.5f) {
      Action = Jumping;
      PhysicsEngineObject->setJumping(true);
      View->getCabbageSceneObject().setRotation(SVector3(curRotation.X, curRotation.Y + 720.f*time, 80.0f));
   }
   else if (VictoryTime > 2.5f && VictoryTime < 2.9f) {
      Action = Standing;
      PhysicsEngineObject->setJumping(false);
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::None);
   }

   else if (VictoryTime > 2.9f && VictoryTime < 3.9f) {
      Action = Jumping;
      PhysicsEngineObject->setJumping(true);

      View->getCabbageSceneObject().setRotation(SVector3(curRotation.X, curRotation.Y - 360.f*time, 80.f));
      PhysicsEngineObject->setArea(SRect2(Area.Position.X - 3.f*time, Area.Position.Y, 1.f, 1.f));
   }

   else if (VictoryTime > 3.8f && VictoryTime < 3.9f) {
      Action = Standing;
      PhysicsEngineObject->setJumping(false);
   }
   else if (VictoryTime > 3.9f && VictoryTime < 4.9f) {
      View->getCabbageSceneObject().setRotation(SVector3(curRotation.X, 0.f, curRotation.Z - 765.f*time));
      PhysicsEngineObject->setArea(SRect2(Area.Position.X - 3.f*time, Area.Position.Y, 1.f, 1.f));
   }
   else if (VictoryTime > 4.9f && VictoryTime < 6.4f) {
      /*if (!glow) {
         glow = new CParticleEngine(SVector3(Area.Position.X + 0.5f, Area.Position.Y - 0.25f, 0), 400, 2.f, LASER_CHARGING_PARTICLE);
      }*/
      Action = Standing;
      PhysicsEngineObject->setJumping(false);

      Scale.Y -= 0.4f*time;
   }

   if (VictoryTime > 6.9f && VictoryTime < 7.3f) {
      Scale.Y += 1.2f*time;
      Scale.X -= 1.8f*time;

      PhysicsEngineObject->setImpulse(SVector2(0.f, 30.f), 0.01f);

   }

   else if(VictoryTime >= 7.3f)
   {
      COverworldState::get().levelCompleted = true;
      CApplication::get().getStateManager().setState(new CFadeOutState(& COverworldState::get()));
   }

   //Translate the camera unless we're launching off.
   else {
      CApplication::get().getSceneManager().getActiveCamera()->setPosition(SVector3(Area.getCenter().X, Area.getCenter().Y + 1.3f, 10) + ShakeFactor);
   }

   View->getCabbageSceneObject().setTranslation(SVector3(Area.getCenter().X, Area.getCenter().Y, 0));
   View->updateShadow();

   VictoryTime += time;
}
