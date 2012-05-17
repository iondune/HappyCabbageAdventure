#include "CElementPlayer.h"
#include "CGameLevel.h"
#include "CPlayerView.h"
#include "CElementPlayer.h"
#include "CPlayerAbility.h"

CElementPlayer::CElementPlayer(SRect2f nArea, bool useCamera)
: CGameplayElement((CCollideable *&)PhysicsEngineObject, (ISceneObject *&)SceneObject, nArea), Direction(Right), Action(Standing), Recovering(0.0f), Shaking(0.0f), ShakeFactor(SVector3f(0.0f)),
  ISquishable(2.0f, 2.0f), AllowMovement(true), PlayJump(true), Victory(false), VictoryTime(0.0f), ShakeFactorFactor(1000.0f), MoveKeyDelay(0.0f), UseCamera(useCamera), Godmode(false) {
   setupSoundEffects();
}

#include "CEventManager.h"
#include "CApplication.h"
#include "CPlayerAbilityDash.h"

void CElementPlayer::setCanUseAbility(Abilities::EAbilityType t) {
   Stats.AvailableAbilities.insert(t);
}

void CElementPlayer::setStats(Cabbage::PlayerInformation st) {
   Stats = st;
}

void CElementPlayer::doGodmode() {
   if(CApplication::get().getEventManager().IsKeyDown[SDLK_a] || CApplication::get().getEventManager().IsKeyDown[SDLK_LEFT]) {
      if(MoveKeyDelay > 0.0f) {
         if(!used(Abilities::DASH)) {
            Abilities.push_back(new CPlayerAbilityDash(*this, true));
         }
      }
      else
         MoveKeyDelay = 0.3f;
      if(MoveKeyDelay < 0.0f) {
         MoveKeyDelay = 0.0f;
      }
      Direction = Left;
      Action = Walking;
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);
   }
   else if(CApplication::get().getEventManager().IsKeyDown[SDLK_d] || CApplication::get().getEventManager().IsKeyDown[SDLK_RIGHT]) {
      if(MoveKeyDelay > 0.0f) {
         if(!used(Abilities::DASH)) {
            Abilities.push_back(new CPlayerAbilityDash(*this, true));
         }
      }
      else
         MoveKeyDelay = 0.3f;
      if(MoveKeyDelay < 0.0f) {
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
   if(CApplication::get().getEventManager().IsKeyDown[SDLK_SPACE] || CApplication::get().getEventManager().IsKeyDown[SDLK_w]) {
      Action = Jumping;
      PhysicsEngineObject->setVelocity(SVector2f((float) PhysicsEngineObject->getVelocity().X, 10.0f));
   }
   else if(CApplication::get().getEventManager().IsKeyDown[SDLK_s]) {
      PhysicsEngineObject->setVelocity(SVector2f((float) PhysicsEngineObject->getVelocity().X, -10.0f));
   }
   else {
   }
}

bool hWasDown = false;

void CElementPlayer::updatePlayerAction() {
   if(Victory)
      return;
   if(!hWasDown && CApplication::get().getEventManager().IsKeyDown[SDLK_h]) {
      Godmode = !Godmode;
      printf("Here\n");
      if(Godmode) {
         PhysicsEngineObject->CollideableLevel = 0;//INTERACTOR_NULL_BLOCK;
         PhysicsEngineObject->CanCollideWith = INTERACTOR_NULL_BLOCK;
         PhysicsEngineObject->setControlFall(false);
         PhysicsEngineObject->setFallAcceleration(0.0f);
         PhysicsEngineObject->setJumping(false);
         PhysicsEngineObject->setVelocity(SVector2(PhysicsEngineObject->getVelocity().X, 0.0f));
         hWasDown = CApplication::get().getEventManager().IsKeyDown[SDLK_h];
         doGodmode();
         return;
      }
      else {
         PhysicsEngineObject->CollideableLevel = INTERACTOR_SUPERACTORS;
         PhysicsEngineObject->CanCollideWith = INTERACTOR_SUPERACTORS | INTERACTOR_ITEMS | INTERACTOR_ACTORS | INTERACTOR_BLOCKS;
         PhysicsEngineObject->CanCollideWith &= ~INTERACTOR_SUPERNONCOLLIDERS;
         PhysicsEngineObject->setControlFall(true);
         PhysicsEngineObject->setFallAcceleration(10.0f);
         PhysicsEngineObject->setJumping(false);
         PhysicsEngineObject->setVelocity(SVector2(PhysicsEngineObject->getVelocity().X, 0.0f));
      }
   }
   hWasDown = CApplication::get().getEventManager().IsKeyDown[SDLK_h];
   if(Godmode) {
      PhysicsEngineObject->setVelocity(SVector2(PhysicsEngineObject->getVelocity().X, 0.0f));
      doGodmode();
   }

   if(!AllowMovement) {
      Action = Standing;
      PhysicsEngineObject->setJumping(false);
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::None);
      return;
   }
   if(CApplication::get().getEventManager().IsKeyDown[SDLK_a] || CApplication::get().getEventManager().IsKeyDown[SDLK_LEFT]) {
      if(Stats.canUseAbility(Abilities::DASH)) {
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
      }
      Direction = Left;
      Action = Walking;
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);
   }
   else if(CApplication::get().getEventManager().IsKeyDown[SDLK_d] || CApplication::get().getEventManager().IsKeyDown[SDLK_RIGHT]) {
      if(Stats.canUseAbility(Abilities::DASH)) {
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
   if(Stats.canUseAbility(Abilities::LASER)) {
      if(!used(Abilities::LASER)) {
         if(CApplication::get().getEventManager().IsKeyDown[SDLK_l]) {
            Abilities.push_back(new CPlayerAbilityLaser(*this));
         }
      }
      else {
         getAbility(Abilities::LASER)->checkKey(CApplication::get().getEventManager().IsKeyDown[SDLK_l]);
      }
   }
   /* Shield */
   if(Stats.canUseAbility(Abilities::SHIELD) && Stats.Energy > 0) {
      if(!used(Abilities::SHIELD)) {
         if(CApplication::get().getEventManager().IsKeyDown[SDLK_k]) {
            Abilities.push_back(new CPlayerAbilityShield(*this));
         }
      }
      else {
         getAbility(Abilities::SHIELD)->checkKey(CApplication::get().getEventManager().IsKeyDown[SDLK_k]);
      }
   }
   /* Dash */
   if(Godmode || used(Abilities::DASH) || Stats.canUseAbility(Abilities::DASH)) {
      if(!used(Abilities::DASH)) {
      }
      else {
         getAbility(Abilities::DASH)->checkKey(CApplication::get().getEventManager().IsKeyDown[SDLK_LEFT] || CApplication::get().getEventManager().IsKeyDown[SDLK_RIGHT] || CApplication::get().getEventManager().IsKeyDown[SDLK_a] || CApplication::get().getEventManager().IsKeyDown[SDLK_d]);
      }
   }
   if(Stats.canUseAbility(Abilities::BLINK)) {
      if(!used(Abilities::BLINK) && CApplication::get().getEventManager().IsKeyDown[SDLK_e]) {
         Abilities.push_back(new CPlayerAbilityBlink(*this));
      }
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
         ShakeFactor = SVector3f((float)rand()/(float)RAND_MAX * 0.3f - 0.15f, (float)rand()/(float)RAND_MAX * 0.3f - 0.15f, 0) / ShakeFactorFactor;
      if(Shaking <= 0.0f) {
         ShakeFactor = SVector3f(0.0f);
         ShakeFactorFactor = 1000.0f;
      }
   }

   SVector2f leftOfPlayer = SVector2f(Area.Position.X, Area.getCenter().Y);
   SVector2f rightOfPlayer = SVector2f(Area.Position.X + Area.Size.X, Area.getCenter().Y);
   CCollisionObject *l, *r;
   l = Level.getPhysicsEngine().getObjectBelow(leftOfPlayer);
   r = Level.getPhysicsEngine().getObjectBelow(rightOfPlayer);

   View->setShadowHeights(Level.getPhysicsEngine().getHeightBelow(leftOfPlayer), Level.getPhysicsEngine().getHeightBelow(rightOfPlayer));
   // If the cabbage is hanging over an edge, there might not be an object below it, so this check is necessary
   View->setCutoffPoint(l?l->getArea():SRect2f(-50.0f, -50.0f, 0.0f, 0.0f),r?r->getArea():SRect2f(-50.0f, -50.0f, 0.0f, 0.0f));

   updateAbilities(time);
   if (Victory) {
      playLevelVictory(time);
   }
   else {
      View->updateView(time);
   }

   Scale = ISquishable::Squish(PhysicsEngineObject->getVelocity());
   View->setCabbageScale(SVector3f(Scale.X, Scale.X, Scale.Y));
}

Cabbage::PlayerInformation & CElementPlayer::getStats() {
   return Stats;
}

void CElementPlayer::OnCollision(const SCollisionEvent& Event) {
   for(unsigned int i = 0; i < Abilities.size(); i++) {
      Abilities[i]->inOnCollision(Event);
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
   PhysicsEngineObject->setTypeId(PhysicsEngineObject->getTypeId() | INTERACTOR_SUPERACTORS);
   PhysicsEngineObject->setCollisionMask(PhysicsEngineObject->getCollisionMask() | INTERACTOR_SUPERACTORS | INTERACTOR_ITEMS);
}

void CElementPlayer::setupSceneObject() {
   SceneObject = new ISceneObject();
   View = new CPlayerView(SceneObject, Direction, Action, Stats.Health, Area, ShakeFactor, PhysicsEngineObject, UseCamera);
}

bool CElementPlayer::decrementHealth() {
   if(used(Abilities::SHIELD) || Godmode)
         return false;

   if(Recovering == 0.0f) {
      Stats.Health--;

      if(Stats.Health <= 0) {
         Stats.Health = 0;
         decrementLives();
         return false;
      }

      View->removeLeaf();
      View->setHurt(true);
      Recovering = 1.0f;

      Mix_PlayChannel(-1, takeDmg, 0);

      return true;
   }

   return false; // is this right? this control path didn't return before
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

bool CElementPlayer::subtractHealth(int amount) {
   if(used(Abilities::SHIELD) || Godmode)
      return false;
   if(Recovering > 0.0f)
      return false;
   std::max(0, Stats.Health -= amount);

   if(Stats.Health <= 0) {
      Stats.Health = 0;
      decrementLives();
      return false;
   }
   View->removeLeaf();
   View->setHurt(true);
   Recovering = 1.0f;
   Mix_PlayChannel(-1, takeDmg, 0);
   return true;
}
   return false;

void CElementPlayer::incrementLives() {
   Stats.Lives++;
}

#include "COverworldState.h"
#include "../../StateGame/CGameState.h"
void CElementPlayer::decrementLives() {
   Mix_PlayMusic(deathMusic, 1);

   //Reset stage
   if (Stats.Lives > 0) {
      Stats.Lives--;
      Stats.Health = Stats.MaxHealth;
      Stats.Energy = Stats.MaxEnergy;

      CGameState::get().Stats = Stats;
      CApplication::get().getStateManager().setState(new CFadeOutState(& CGameState::get()));
   }

   //Reset to overworld and set stats back to max values.
   else {
      COverworldState::get().levelCompleted = false;

      Cabbage::PlayerInformation newStats = COverworldState::get().Stats;
      newStats.Health = Stats.MaxHealth;
      newStats.Energy = Stats.MaxEnergy;
      newStats.Lives = 3; //default number of lives

      CApplication::get().getStateManager().setState(new CFadeOutState(& COverworldState::get()));
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

   temp = MusicDirectory + "takeDmg.wav";
   takeDmg = Mix_LoadWAV(temp.c_str());

   temp = MusicDirectory + "chargeLaser1.wav";
   chargeLaser1 = Mix_LoadWAV(temp.c_str());

   temp = MusicDirectory + "chargeLaser2.wav";
   chargeLaser2 = Mix_LoadWAV(temp.c_str());

   temp = MusicDirectory + "fireLaser3.wav";
   fireLaser = Mix_LoadWAV(temp.c_str());

   temp = MusicDirectory + "Soundtracks/Win.wav";
   victoryMusic = Mix_LoadMUS(temp.c_str());

   temp = MusicDirectory + "Soundtracks/Death.wav";
   deathMusic = Mix_LoadMUS(temp.c_str());

   if (!victoryMusic) {
      printf("Mix_LoadWAV: %s\n", Mix_GetError());
      exit(1);
   }
}

void CElementPlayer::setVictoryFlag(bool value) {
   Victory = value;
}

void CElementPlayer::setAllowMovement(bool value) {
   AllowMovement = value;
}

CElementPlayer::EAction CElementPlayer::getAction() {
   return Action;
}

void CElementPlayer::playLevelVictory(float time) {
   SVector3f curRotation = View->getCabbageSceneObject().getRotation();
   SVector2f curLocation = SVector2f (Area.getCenter().X - .5f, Area.getCenter().Y - .5f);

   //Start Victory Music
   if (VictoryTime == 0.0f) {
      Mix_PlayMusic(victoryMusic, 1);

      if (!WinParticle1)
         WinParticle1 = new CParticleEngine(SVector3f(curLocation.X, curLocation.Y, .5f), 40, 2.f, HURT_PARTICLE);
      if (!WinParticle2)
         WinParticle2 = new CParticleEngine(SVector3f(curLocation.X + 4.f, curLocation.Y, .5f), 40, 2.f, HURT_PARTICLE);
      if (!WinParticle3)
         WinParticle3 = new CParticleEngine(SVector3f(curLocation.X + 8.f, curLocation.Y, .5f), 40, 2.f, HURT_PARTICLE);
   }


   if (WinParticle1) {
      WinParticle1->step(time);
   }
   if (WinParticle2) {
      WinParticle2->step(time);
   }
   if (WinParticle3) {
      WinParticle3->step(time);
   }


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

   else if (VictoryTime > 1.f && VictoryTime < 1.2f) {
      Action = Walking;
      PhysicsEngineObject->setJumping(false);
   }
   else if (VictoryTime > 1.5f && VictoryTime < 2.5f) {
      Action = Jumping;
      PhysicsEngineObject->setJumping(true);
      View->getCabbageSceneObject().setRotation(SVector3f(curRotation.X, curRotation.Y + 720.f*time, 80.0f));
   }
   else if (VictoryTime > 2.5f && VictoryTime < 2.9f) {
      Action = Standing;
      PhysicsEngineObject->setJumping(false);
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::None);
   }

   else if (VictoryTime > 2.9f && VictoryTime < 3.9f) {
      Action = Jumping;
      PhysicsEngineObject->setJumping(true);

      View->getCabbageSceneObject().setRotation(SVector3f(curRotation.X, curRotation.Y - 360.f*time, 80.f));
      PhysicsEngineObject->setArea(SRect2f(Area.Position.X - 3.f*time, Area.Position.Y, 1.f, 1.f));
   }

   else if (VictoryTime > 3.8f && VictoryTime < 3.9f) {
      Action = Standing;
      PhysicsEngineObject->setJumping(false);
   }
   else if (VictoryTime > 3.9f && VictoryTime < 4.9f) {
      View->getCabbageSceneObject().setRotation(SVector3f(curRotation.X, 0.f, curRotation.Z - 765.f*time));
      PhysicsEngineObject->setArea(SRect2f(Area.Position.X - 3.f*time, Area.Position.Y, 1.f, 1.f));
   }
   else if (VictoryTime > 4.9f && VictoryTime < 6.4f) {
      if (!glow) {
         glow = new CParticleEngine(SVector3f(Area.getCenter().X, Area.getCenter().Y, 0), 400, 2.f, LASER_CHARGING_PARTICLE);
      }
      Action = Standing;
      PhysicsEngineObject->setJumping(false);

      Scale.Y -= 0.4f*time;
      glow->step(time);
   }

   if (VictoryTime > 6.9f && VictoryTime < 7.3f) {
      Scale.Y += 1.2f*time;
      Scale.X -= 1.8f*time;

      PhysicsEngineObject->addImpulse(SVector2f(0.f, 30.f));

   }

   else if(VictoryTime >= 7.3f)
   {
      COverworldState::get().levelCompleted = true;
      COverworldState::get().Stats = Stats;
      CApplication::get().getStateManager().setState(new CFadeOutState(& COverworldState::get()));
   }

   //Translate the camera unless we're launching off.
   else {
      CApplication::get().getSceneManager().getActiveCamera()->setPosition(SVector3f(Area.getCenter().X, Area.getCenter().Y + 1.3f, 10) + ShakeFactor);
   }

   View->getCabbageSceneObject().setTranslation(SVector3f(Area.getCenter().X, Area.getCenter().Y, 0));
   View->updateShadow();

   VictoryTime += time;

   printf("End of victory\n");
}
