#include <stdio.h>

#include "CGameEventReceiver.h"

int numKilled;

CGameEventReceiver::CGameEventReceiver()
{
   numKilled = 0;
   DeadEnemies.clear();
   playerDamagedEngine = NULL;
   Player = NULL;
}

void CGameEventReceiver::OnPlayerDamaged(SPlayerDamagedEvent const & Event) {
   if(playerDamagedEngine) {
      playerDamagedEngine->deconstruct();
      delete playerDamagedEngine;
      playerDamagedEngine = NULL;
   }
   playerDamagedEngine = new CParticleEngine(SVector3(Event.Damagee->getArea().getCenter(), 0.0f), 6, 0.8f, HURT_PARTICLE);

   Player = Event.Damagee;
}

void CGameEventReceiver::OnEnemyDeath(SEnemyDeathEvent const & Event) {
   //fprintf(stderr, "Removing enemy %d\n", Event.Enemy);
   int randSeeds = rand()%7;

   SDeadEnemy DeadEnemy;
   if (Event.Enemy.Actor->CollideableType == COLLIDEABLE_TYPE_PKIWI) {
      DeadEnemy.DeathTimer = .0f;
   }
   else {
      DeadEnemy.DeathTimer = 4.f;
      if(rand()%2 == 0) {
    	  if (Event.PlayerHealthLeft == 1 || rand()%2 == 0)
    		  CItem::makeItem(Event.Enemy.Actor->getArea().Position.X + Event.Enemy.Actor->getArea().Size.X / 2.0f - 0.3f
    				  , Event.Enemy.Actor->getArea().getCenter().Y, 1.0f, 1.0f, CItem::health, Event.Manager);
    	  else
    		  CItem::makeItem(Event.Enemy.Actor->getArea().Position.X + Event.Enemy.Actor->getArea().Size.X / 2.0f - 0.3f
    				  , Event.Enemy.Actor->getArea().getCenter().Y, 1.0f, 1.0f, CItem::energy, Event.Manager);
      }

      if (randSeeds > 0 && randSeeds < 6) {
         for (int n = 1; n <= randSeeds; n++) {
            CItem::makeItem(Event.Enemy.Actor->getArea().Position.X + Event.Enemy.Actor->getArea().Size.X / 2.0f - 0.3f + (float)n,
               Event.Enemy.Actor->getArea().getCenter().Y, 1.0f, 1.0f, CItem::seed, Event.Manager);
         }
      }
   }
      DeadEnemy.Renderable = Event.Enemy.Renderable;

      if(Event.Enemy.KillMethod == 0 && DeadEnemy.Renderable != 0) {
         DeadEnemy.Renderable->setScale(SVector3(1.f, 1.0f, 0.4f));
         DeadEnemy.Renderable->setTranslation(DeadEnemy.Renderable->getTranslation() - SVector3(0.f, 0.5f, 0.f));
      }
      else {
         DeadEnemy.Renderable->setScale(SVector3(0.01f, 0.01f, 0.01f));

      }

      DeadEnemy.ParticleE = new CParticleEngine(DeadEnemy.Renderable->getTranslation(), 20, 4, BURST_PARTICLE);
      DeadEnemy.ParticleE->UsePhysics(Event.Manager->getEngine());

      numKilled++;

   DeadEnemies.push_back(DeadEnemy);

   //CApplication::get().getSceneManager().removeSceneObject(Event.Renderable);

}

void CGameEventReceiver::OnGameTickStart(float const Elapsed)
{
   int i = 0;
   for (std::vector<SDeadEnemy>::iterator it = DeadEnemies.begin(); it != DeadEnemies.end();)
   {
      it->DeathTimer -= Elapsed;
      if(it->ParticleE)
         it->ParticleE->step(Elapsed);
      if (it->DeathTimer < 3.5f)
         CApplication::get().getSceneManager().removeSceneObject(it->Renderable);
      if (it->DeathTimer < 0.f)
      {
         it->ParticleE->deconstruct();
         delete it->ParticleE;
         it->ParticleE = NULL;
         it = DeadEnemies.erase(it);
      }
      else
         ++ it;
   }

   if(playerDamagedEngine && !playerDamagedEngine->dead) {
      //playerDamagedEngine->setCenterPos(SVector3(Player->getArea().getCenter(), 0.0f));
      playerDamagedEngine->step(Elapsed);
   }
   else if(playerDamagedEngine && playerDamagedEngine->dead) {
      playerDamagedEngine->deconstruct();
      delete playerDamagedEngine;
      playerDamagedEngine = NULL;
      Player = NULL;
   }
}
