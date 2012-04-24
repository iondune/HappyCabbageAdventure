#include "CGameplayManager.h"
#include "CGameEventManager.h"
#include "EKiwi.h"

int CGameplayManager::PlayerLives;
int CGameplayManager::SeedCount;

CGameplayManager::CGameplayManager(CCollisionActor * playerActor, CCollisionEngine * engine)
: PlayerActor(playerActor), PlayerRecovering(false), PlayerHealth(5), Engine(engine), PlayerEnergy(3)
{
   Engine->setCollisionResponder(this);
   GameEventManager = new CGameEventManager();
   won = 0;
   GodMode = 0;
   GodModeTime = 0;
   dead = 0;
   gameOver = 0;
   PlayerHealth = 5; PlayerRecovering = false; PlayerActor = playerActor; Engine = engine; PlayerEnergy = 3;
   ShootingLaser = 0;
   JustKilled = 0;

}

void CGameplayManager::setLives(int num) {
   PlayerLives = num;
}

void CGameplayManager::UseAbility(int energyCost) {
   PlayerEnergy -= energyCost;
}

void CGameplayManager::setGodMode(float time) {
   GodModeTime = time;
}

bool CGameplayManager::OnCollision(CCollideable * Object, CCollideable * With)
{
   CCollideable * Other = 0;
   CCollideable * PlayerCollideable = 0;
   CCollideable * Flag = 0;
   CCollideable * Projectile = 0;

   //Read that a projectile collided with something
   if (Object->CollideableType == COLLIDEABLE_TYPE_PKIWI) {
      Projectile = Object;
   }

   if (With->CollideableType == COLLIDEABLE_TYPE_PKIWI) {
      Projectile = With;
   }

   if (Object == PlayerActor) {
      PlayerCollideable = PlayerActor;
      Other = With;
   }
   if (With == PlayerActor) {
      PlayerCollideable = PlayerActor;
      Other = Object;
   }
   if (Object == VictoryFlag)
      Flag = Object;
   if (Object == SecretVictoryFlag)
       Flag = Object;
   if (With == VictoryFlag)
      Flag = With;
   if (With == SecretVictoryFlag)
       Flag = With;
   if(Flag)
      printf("Flag collision\n");

   int birdCollision = 0;

   if(PlayerCollideable && Other->CollideableType == COLLIDEABLE_TYPE_ITEM) {
      ItemList ToKill;
      for(ItemList::iterator it = Items.begin(); it != Items.end(); ++ it) {
         if (Other == (*it)->Actor) {
            if((*it)->Type == CItem::health) {
               if (getPlayerHealth() < 5) {
                  PlayerHealth++;
               }
               KillItemList.push_back(*it);
            }
            else if ((*it)->Type == CItem::energy) {
               if (getPlayerEnergy() < 3)
                  PlayerEnergy++;
               KillItemList.push_back(*it);
            }
            else if ((*it)->Type == CItem::energy) {
               if (getPlayerEnergy() < 3)
                  PlayerEnergy++;
               KillItemList.push_back(*it);
            }
            else if ((*it)->Type == CItem::seed) {
               SeedCount++;
               if (SeedCount == 100) {
                  PlayerLives++;
                  SeedCount = 0;
               }
               KillItemList.push_back(*it);
            }
            else if ((*it)->Type == CItem::life) {
               PlayerLives++; 
               KillItemList.push_back(*it);
            }
            ToKill.push_back(*it);
         }
      }
      for(ItemList::iterator it = ToKill.begin(); it != ToKill.end(); ++ it) {
         Items.erase(std::remove(Items.begin(), Items.end(), *it), Items.end());
      }
      return false;
   }
   //return;

   if((With->CollideableType == COLLIDEABLE_TYPE_ACTOR || With == PlayerActor || With->CollideableType == COLLIDEABLE_TYPE_BLOCK)
         && Object->CollideableType == COLLIDEABLE_TYPE_KIWI) {

      if(With->CollideableType == COLLIDEABLE_TYPE_ACTOR) {
         ((CCollisionActor*)With)->setImpulse(SVector2(0.00f, -0.3f), 0.2f);
      }
      birdCollision = 1;

      if(With == PlayerActor) {
         ((CCollisionActor*)Object)->setImpulse(SVector2(0.03f, 0.3f)*7, 0.2f);
      }
      else {
         Object->CollideableLevel = INTERACTOR_SUPERNONCOLLIDERS;
         Object->CanCollideWith = 0;//INTERACTOR_NULL_BLOCK;//INTERACTOR_ACTORS; 

         for (EnemyList::iterator it = Enemies.begin(); it != Enemies.end(); ++ it)
         {
            //Remove projectile from scene
            if (Object == (*it)->Actor) {
               CCollisionActor* actor = (*it)->Actor;
               ((EKiwi*)(*it))->inZ = 1;
               ((EKiwi*)(*it))->lastX = Object->getArea().getCenter().X;
               ((EKiwi*)(*it))->Depth = With->getDepth();

               SVector2f newPos = actor->LastPosition + actor->Movement;
               SVector2f size = actor->getArea().Size;
               actor->setArea(SRect2(newPos, size));
               return true;
            }
         }
      }
   }

   if((GodMode) && birdCollision == 1 && PlayerCollideable) {
      PlayerActor->setImpulse(SVector2(0.f, -0.4f) * 7, 0.5f);
   }

   if ((!Other && !Projectile) || GodMode || ShootingLaser)
      return true;

   float const HitThreshold = 0.2f;

   if(PlayerCollideable && Flag)
      won = 1;
   if(won) {
      return true;
   }

   for (EnemyList::iterator it = Enemies.begin(); it != Enemies.end(); ++ it)
   {
      //Remove projectile from scene
      if (Projectile == (*it)->Actor) {
         KillList.push_back(*it);
         (*it)->KilledBy = 2;
      }

      if (Other == (*it)->Actor)
      {

         if (PlayerActor->getArea().Position.Y > Other->getArea().otherCorner().Y - HitThreshold &&
               PlayerActor->getVelocity().Y <= 0 &&
               (*it)->Actor->CollideableType != COLLIDEABLE_TYPE_FLAME && (*it)->Actor->CollideableType != COLLIDEABLE_TYPE_PKIWI)
         {
            Mix_PlayChannel(-1, killEnemy, 0);
            KillList.push_back(* it);
            (* it)->KilledBy = 0;
            //fprintf(stderr, "Enemy detected as dead! %d\n", it->Renderable);


            //Chris look here for the player impulse!
            PlayerActor->setImpulse(SVector2(0.f, 10.0f), 0.05f);
            PlayerActor->setVelocity(SVector2(PlayerActor->getVelocity().X, 0.0f));

            Enemies.erase(std::remove(Enemies.begin(), Enemies.end(), *it), Enemies.end());
            printf("In kill code: %d\n", Other);
            return true;
         }
         //Need to rewrite so works without SEnemy
         else
         {
            if (GodModeTime > 0) {
               if((*it)->Actor->CollideableType != COLLIDEABLE_TYPE_FLAME) {
                  KillList.push_back(* it);
                  if((*it)->Actor->CollideableType == COLLIDEABLE_TYPE_PKIWI)
                     (* it)->KilledBy = 2;
                  else {
                     (* it)->KilledBy = 1;
                     Mix_PlayChannel(-1, killEnemy, 0);
                     Enemies.erase(it);
                  }
               }
               continue;
            }
            if (isPlayerAlive() && PlayerRecovering <= 0.f)
            {
               printf("In getting damaged code: %d\n", Other);
               SPlayerDamagedEvent Event;
               SEnemy enemy;
               enemy.Actor = (*it)->Actor;
               enemy.Renderable = (*it)->Renderable;

               Event.DamagedBy = enemy;
               Event.Damagee = PlayerActor;
               if(PlayerHealth > 1)
                  GameEventManager->OnPlayerDamaged(Event);

               //Set hurt face


               //Chris Code.  Damage Sound plays here
               if(playTakeDmg) {
                  Mix_PlayChannel(-1, takeDmg, 0);
               }

               -- PlayerHealth;

               if (PlayerHealth <= 0)
               {
                  SPlayerDeathEvent Event;
                  Event.KilledBy = enemy;
                  GameEventManager->OnPlayerDeath(Event);
               }

               float const KnockbackSpeed = 7.f;
               float const KnockbackDuration = 0.1f;

               PlayerRecovering = 1.0f;
               if((*it)->Actor->CollideableType != COLLIDEABLE_TYPE_FLAME) {

                  if (PlayerActor->getArea().getCenter().X > Other->getArea().getCenter().X)
                     PlayerActor->setImpulse(SVector2(1.f, 0.4f) * KnockbackSpeed, KnockbackDuration);
                  else
                     PlayerActor->setImpulse(SVector2(-1.f, 0.4f) * KnockbackSpeed, KnockbackDuration);
                  if (PlayerActor->getArea().getCenter().Y <= Other->getArea().getCenter().Y)
                     PlayerActor->addImpulse(SVector2(0.f, -0.75f) * KnockbackSpeed);
                  else
                     PlayerActor->addImpulse(SVector2(0.f, -0.75f) * KnockbackSpeed);
               }

            }
         }
         return true;
      }
   }
   return true;
}

void CGameplayManager::runDeathSequence(float elapsedTime) {
   if(dead) {
      if(playerDeathParticleEngine && !playerDeathParticleEngine->dead) {
         playerDeathParticleEngine->setCenterPos(SVector3(PlayerActor->getArea().getCenter(), 0));
         playerDeathParticleEngine->step(elapsedTime);
      }
      else {
         delete playerDeathParticleEngine;
         playerDeathParticleEngine = NULL;
      }
      return;
   }
   dead = !isPlayerAlive();
   if(dead) {
      playerDeathParticleEngine = new CParticleEngine(SVector3(PlayerActor->getArea().getCenter(), 0), 400, 5, DEATH_PARTICLE);
      Engine->removeActor(PlayerActor);
      playerView->removeFromScene();

      if(PlayerLives > 0)
         PlayerLives--;
      else {
         gameOver = 1;
      }
   }
}

SVector2f CGameplayManager::getPlayerLocation() {
   return SVector2(PlayerActor->getArea().getCenter().X, PlayerActor->getArea().getCenter().Y);
}

void CGameplayManager::setVictoryFlag(CCollisionObject * f) {
   VictoryFlag = f;
}

void CGameplayManager::setSecretVictoryFlag(CCollisionObject * f) {
   SecretVictoryFlag = f;
}

bool const CGameplayManager::isWon() const
{
   return won != 0; 
}

bool const CGameplayManager::isGameOver() const
{
   return gameOver != 0; 
}

bool const CGameplayManager::isPlayerAlive() const
{
   return (PlayerHealth > 0) && (-5.0 < PlayerActor->getArea().getCenter().Y);
}

int const CGameplayManager::getPlayerHealth() const
{
   return PlayerHealth;
}

int const CGameplayManager::getPlayerEnergy() const
{
   return PlayerEnergy;
}

int const CGameplayManager::getPlayerLives()
{
   return PlayerLives;
}

int const CGameplayManager::getNumLives()
{
   return PlayerLives;
}

int const CGameplayManager::getSeedCount()
{
   return SeedCount;
}

bool const CGameplayManager::isJumping() const {
   return PlayerActor->isJumping();
}

void CGameplayManager::run(float const TickTime)
{
   if (PlayerRecovering > 0.f)
      PlayerRecovering -= TickTime;
   if (GodModeTime > 0)
      GodModeTime -= TickTime;
   if (JustKilled > 0)
      JustKilled -= TickTime;
   if (JustKilled < 0.f)
      JustKilled = 0;

   EnemyList toKill;

   if(ShootingLaser) {
      for (EnemyList::iterator it = Enemies.begin(); it != Enemies.end(); ++ it)
      {
         if ((*it)->Actor->CollideableType != COLLIDEABLE_TYPE_FLAME) {
            if(LaserBox.intersects((*it)->Actor->getArea())) {
               Mix_PlayChannel(-1, killEnemy, 0);
               toKill.push_back(* it);
               KillList.push_back(* it);
               (* it)->KilledBy = 1;
            }
         }
      }
      for (EnemyList::iterator it = toKill.begin(); it != toKill.end(); ++ it)
      {
         Enemies.erase(std::remove(Enemies.begin(), Enemies.end(), *it), Enemies.end());
      }
   }

   for (ItemList::iterator it = KillItemList.begin(); it != KillItemList.end(); ++it) {
      Engine->removeActor((*it)->Actor);
      CApplication::get().getSceneManager().removeSceneObject((*it)->Renderable);
   }

   KillItemList.clear();


   for (EnemyList::iterator it = KillList.begin(); it != KillList.end(); ++ it)
   {
      SEnemyDeathEvent Event;
      SEnemy enemy;
      enemy.Actor = (*it)->Actor;
      enemy.Renderable = (*it)->Renderable;
      Event.Enemy = enemy;
      Event.Enemy.KillMethod = (*it)->KilledBy;
      Event.PlayerHealthLeft = PlayerHealth;
      Event.Manager = this;

      GameEventManager->OnEnemyDeath(Event);

      Engine->removeActor((*it)->Actor);
      if((*it)->KilledBy == 0)
         JustKilled = 0.3f;
   }

   KillList.clear();


   float cabbageCenterX = PlayerActor->getArea().getCenter().X;
   float cabbageCenterY = PlayerActor->getArea().getCenter().Y;

   for (unsigned int i = 0; i < Enemies.size(); i++) {
      float enemyCenterX = Enemies[i]->Actor->getArea().getCenter().X;
      float enemyCenterY = Enemies[i]->Actor->getArea().getCenter().Y;

      if ((enemyCenterX < cabbageCenterX + 9 && enemyCenterX > cabbageCenterX - 9) && !isWon()) {
         if(Enemies[i]->Actor->CollideableType == COLLIDEABLE_TYPE_KIWI) {
            EKiwi *kPtr = (EKiwi*)Enemies[i];
            if(kPtr->inZ && (
                     (kPtr->Actor->getArea().getCenter().X - kPtr->lastX > 0.4f) ||
                     (kPtr->Actor->getArea().getCenter().X - kPtr->lastX < -0.4f))
              ) {
               kPtr->inZ = 0;
               kPtr->Actor->CollideableLevel = INTERACTOR_SUPERNONCOLLIDERS;
               kPtr->Actor->CanCollideWith = INTERACTOR_BLOCKS | INTERACTOR_SUPERACTORS;
            }
         }
         Enemies[i]->update(TickTime);
      }

      //Kill only the visible enemies to ensure we don't cause massive lag.
      else if (isWon() && (Enemies[i]->Actor->CollideableType != COLLIDEABLE_TYPE_FLAME) && (enemyCenterX < cabbageCenterX + 12 && enemyCenterX > cabbageCenterX - 12)) {
         KillList.push_back(Enemies[i]);
      }
   }

   if (isWon()) {
      Enemies.clear();
   }

   for (ItemList::iterator it = Items.begin(); it != Items.end(); ++ it) {
      float itemCenterX = (*it)->Actor->getArea().getCenter().X;
      float itemCenterY = (*it)->Actor->getArea().getCenter().Y;

      if ((itemCenterX < cabbageCenterX + 9 && itemCenterX > cabbageCenterX - 9))// && (itemCenterY < cabbageCenterY + 9 && itemCenterY > cabbageCenterY - 9))
         (*it)->update(TickTime);
   }
   runDeathSequence(TickTime);
}

CCollisionEngine* CGameplayManager::getEngine() {
   return Engine;
}


CGameEventManager & CGameplayManager::getGameEventManager()
{
   return * GameEventManager;
}
