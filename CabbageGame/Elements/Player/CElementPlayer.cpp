#include "CElementPlayer.h"
#include "CGameLevel.h"
#include "CPlayerView.h"
#include "CElementPlayer.h"
#include "CPlayerAbility.h"

#define JUMP_SOUND "61847__simon-rue__boink-v3.wav"
//#define JUMP_SOUND "172205__fins__jumping.wav"
#define DAMAGE_TAKEN_SOUND "135460__joelaudio__lettuce-crunch-quick-001.wav"
#define WIN_MUSIC "Soundtracks/531842_Victory-Dance.ogg"
#define DEATH_MUSIC "Soundtracks/Death.ogg"

CElementPlayer::CElementPlayer(SRect2f nArea, bool useCamera)
: CGameplayElement((CCollideable *&)PhysicsEngineObject, (ISceneObject *&)SceneObject, nArea), Direction(Right), Action(Standing), Recovering(0.0f), Shaking(0.0f), ShakeFactor(SVector3f(0.0f)),
  ISquishable(2.0f, 2.0f), AllowMovement(true), PlayJump(true), Victory(false), VictoryTime(0.0f), ShakeFactorFactor(1000.0f), MoveKeyDelay(0.0f), UseCamera(useCamera), Godmode(false),
WinParticle1(NULL), WinParticle2(NULL), WinParticle3(NULL), glow(NULL), hWasDown(false), jWasDown(false), nWasDown(false), oldGrav(0.0f) {
   setupSoundEffects();
}

CElementPlayer::EDirection CElementPlayer::getDirection() {
   return Direction;
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
      //if(MoveKeyDelay > 0.0f) {
      //   if(!used(Abilities::DASH)) {
      //      Abilities.push_back(new CPlayerAbilityDash(*this, true));
      //   }
      //}
      //else
      //   MoveKeyDelay = 0.3f;
      //if(MoveKeyDelay < 0.0f) {
      //   MoveKeyDelay = 0.0f;
      //}
      Direction = Left;
      Action = Walking;
      PhysicsEngineObject->setAction(CCollisionActor::EActionType::MoveLeft);
   }
   else if(CApplication::get().getEventManager().IsKeyDown[SDLK_d] || CApplication::get().getEventManager().IsKeyDown[SDLK_RIGHT]) {
      //if(MoveKeyDelay > 0.0f) {
      //   if(!used(Abilities::DASH)) {
      //      Abilities.push_back(new CPlayerAbilityDash(*this, true));
      //   }
      //}
      //else
      //   MoveKeyDelay = 0.3f;
      //if(MoveKeyDelay < 0.0f) {
      //   MoveKeyDelay = 0.0f;
      //}
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

#include "CSceneEffectManager.h"

float oldMaxWalk = 0;
void CElementPlayer::updatePlayerAction() {
   if(Victory)
      return;
   if(!nWasDown && CApplication::get().getEventManager().IsKeyDown[SDLK_n]) {
      bool enabledBloom = CApplication::get().getSceneManager().getEffectManager()->isEffectEnabled(ESE_BLOOM);
      CApplication::get().getSceneManager().getEffectManager()->setEffectEnabled(ESE_BLOOM, !enabledBloom);
      this->printInformation();
   }
   nWasDown = CApplication::get().getEventManager().IsKeyDown[SDLK_n];
   if(!hWasDown && CApplication::get().getEventManager().IsKeyDown[SDLK_h]) {
      Godmode = !Godmode;
      printf("Godmode toggled.\n");
      if(Godmode) {
         oldMaxWalk = PhysicsEngineObject->getActorAttributes().MaxWalk;
         PhysicsEngineObject->getActorAttributes().MaxWalk  = 50;
         PhysicsEngineObject->setTypeId(0);
         PhysicsEngineObject->setCollisionMask(0);
         PhysicsEngineObject->setDetectionMask(0);
         PhysicsEngineObject->setGravityEnabled(false);
         PhysicsEngineObject->setJumping(false);
         PhysicsEngineObject->setVelocity(SVec2(PhysicsEngineObject->getVelocity().X, 0.0));
         hWasDown = CApplication::get().getEventManager().IsKeyDown[SDLK_h];
         doGodmode();
         return;
      }
      else {
         PhysicsEngineObject->getActorAttributes().MaxWalk  = oldMaxWalk;
         PhysicsEngineObject->setTypeId(INTERACTOR_ACTORS | INTERACTOR_SUPERACTORS);
         PhysicsEngineObject->setCollisionMask((INTERACTOR_SUPERACTORS | INTERACTOR_ACTORS | INTERACTOR_BLOCKS) & ~INTERACTOR_SUPERNONCOLLIDERS);
         PhysicsEngineObject->setDetectionMask(INTERACTOR_ITEMS);
         PhysicsEngineObject->setGravityEnabled(true);
         PhysicsEngineObject->setJumping(false);
         PhysicsEngineObject->setVelocity(SVec2(PhysicsEngineObject->getVelocity().X, 0.0));
      }
   }
   hWasDown = CApplication::get().getEventManager().IsKeyDown[SDLK_h];
   if(Godmode) {
      PhysicsEngineObject->setVelocity(SVec2(PhysicsEngineObject->getVelocity().X, 0.0));
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
   if(CApplication::get().getEventManager().IsKeyDown[SDLK_SPACE] || CApplication::get().getEventManager().IsKeyDown[SDLK_w]) {
      Action = Jumping;
      PhysicsEngineObject->setJumping(true);

      //Check if jumping should be enabled.
      if (PhysicsEngineObject->getVelocity().Y == 0.f && !PhysicsEngineObject->isJumping())
    	  PlayJump = true;
      if (PlayJump) {
         

         CApplication::get().getSoundManager().registerAndPlaySound(JUMP_SOUND);
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
#include "CPlayerAbilityHeadbutt.h"

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
   /* Headbutt */
   if(Stats.canUseAbility(Abilities::HEADBUTT)) {
      if(!used(Abilities::HEADBUTT)) {
         if(CApplication::get().getEventManager().IsKeyDown[SDLK_p]) {
            Abilities.push_back(new CPlayerAbilityHeadbutt(*this));
         }
      }
      else {
         getAbility(Abilities::HEADBUTT)->checkKey(CApplication::get().getEventManager().IsKeyDown[SDLK_p]);
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
   if(!jWasDown && CApplication::get().getEventManager().IsKeyDown[SDLK_j]) {
      int newSubView = View->getSubView();
      newSubView++;
      if(newSubView > 5)
         newSubView = 0;
      View->useSubView(newSubView);
   }
   jWasDown = CApplication::get().getEventManager().IsKeyDown[SDLK_j];

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

   if(Level.isLoaded())
      Area.Size *= .999f;
      PhysicsEngineObject->setArea(Area);
      oldMaxWalk = PhysicsEngineObject->getActorAttributes().MaxWalk = 5.0f;
      PhysicsEngineObject->getActorAttributes().AirControl *= 4.0f;
      PhysicsEngineObject->getActorAttributes().AirSpeedFactor  = 1.3f;
      PhysicsEngineObject->getActorAttributes().JumpAccel *= 2.0f;
      PhysicsEngineObject->getActorAttributes().JumpLength *= 0.3f;
   //printf("HERE!!! LOOK HEREs %d\n", PhysicsEngineObject->getTypeId());
   PhysicsEngineObject->setTypeId(PhysicsEngineObject->getTypeId() | INTERACTOR_SUPERACTORS);
   PhysicsEngineObject->setCollisionMask(PhysicsEngineObject->getCollisionMask() | INTERACTOR_SUPERACTORS);
   PhysicsEngineObject->setDetectionMask(INTERACTOR_ITEMS);
   if(PhysicsEngineObject->getCollisionMask() & INTERACTOR_ITEMS) {
      printf("Hate my life\n");
      assert(false);
   }
   else {
      printf("Ian is a cant-do-nothin\n");
   }

   if (Level.getEnv() == Env::WATER) {
      oldMaxWalk = PhysicsEngineObject->getActorAttributes().MaxWalk *= 0.75f;
      PhysicsEngineObject->getActorAttributes().AirControl *= 0.75f;
      PhysicsEngineObject->getActorAttributes().AirSpeedFactor *= 0.75f;
      PhysicsEngineObject->getActorAttributes().JumpAccel *= 0.5f;
      PhysicsEngineObject->getActorAttributes().JumpLength *= 0.75f;
      ////PhysicsEngineObject->getActorAttributes().WalkAccel *= 0.25f;
      //PhysicsEngineObject->getActorAttributes().JumpAccel *= 0.75f;
      //PhysicsEngineObject->getActorAttributes().JumpLength *= 0.8f;
      //PhysicsEngineObject->getActorAttributes().AirControl *= 2.0f;
      //PhysicsEngineObject->getActorAttributes().AirSpeedFactor *= 0.5f;
   }
}

void CElementPlayer::setupSceneObject() {
   SceneObject = new ISceneObject();
   View = new CPlayerView(SceneObject, Direction, Action, Stats.Health, Area, ShakeFactor, PhysicsEngineObject, UseCamera, Level);
}

bool CElementPlayer::decrementHealth() {
   if(used(Abilities::SHIELD) || Godmode)
      return false;

   if(Recovering > 0.0f) {
      return false;
   }
   Stats.Health--;

   if(Stats.Health <= 0) {
      Stats.Health = 0;
      decrementLives();
      return false;
   }

   View->removeLeaf();
   View->setHurt(true);
   Recovering = 1.0f;

   
   CApplication::get().getSoundManager().playSound(DAMAGE_TAKEN_SOUND);

   return true;
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
   CApplication::get().getSoundManager().playSound(DAMAGE_TAKEN_SOUND);
   return true;
}

void CElementPlayer::incrementLives() {
   Stats.Lives++;
}

#include "COverworldState.h"
#include "../../StateGame/CGameState.h"
void CElementPlayer::decrementLives() {
   CApplication::get().getSoundManager().swapTrack(DEATH_MUSIC);

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
   CApplication::get().getSoundManager().registerSound(JUMP_SOUND);
   CApplication::get().getSoundManager().registerSound(DAMAGE_TAKEN_SOUND);
   CApplication::get().getSoundManager().registerTrack(WIN_MUSIC);
   CApplication::get().getSoundManager().registerTrack(DEATH_MUSIC);
}

#include "CElementEnemy.h"
void CElementPlayer::setVictoryFlag(bool value) {
   Victory = value;
   for(unsigned int i = 0; i < Level.getEnemies().size(); i++) {
      Level.getEnemies()[i]->setTimeToDeath(0.3f);
   }
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
      CApplication::get().getSoundManager().swapTrack(WIN_MUSIC);

      if (!WinParticle1)
         WinParticle1 = new CParticleEngine(SVector3f(curLocation.X, curLocation.Y, .5f), 40, 2.f, HURT_PARTICLE, Level.isNight());
      if (!WinParticle2)
         WinParticle2 = new CParticleEngine(SVector3f(curLocation.X + 4.f, curLocation.Y, .5f), 40, 2.f, HURT_PARTICLE, Level.isNight());
      if (!WinParticle3)
         WinParticle3 = new CParticleEngine(SVector3f(curLocation.X + 8.f, curLocation.Y, .5f), 40, 2.f, HURT_PARTICLE, Level.isNight());
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
         glow = new CParticleEngine(SVector3f(Area.getCenter().X, Area.getCenter().Y, 0), 400, 2.f, LASER_CHARGING_PARTICLE, Level.isNight());
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

void CElementPlayer::printInformation() {
   printf("CElementPlayer; Area: [[%0.1f, %0.1f],[%0.0f, %0.0f]]; Stats: [[Health: %d, Energy: %d, Seeds: %d, Lives: %d]]\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y, Stats.Health, Stats.Energy, Stats.Seeds, Stats.Lives);
}
