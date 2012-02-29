#include <stdio.h>

#include "CGameEventReceiver.h"

int numKilled;

CGameEventReceiver::CGameEventReceiver()
{
   numKilled = 0;
   printf("Here!!!\n");
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
   playerDamagedEngine = new CParticleEngine(SVector3(Event.Damagee->getArea().getCenter(), 0.0f), 6, 0.8, HURT_PARTICLE);
   Player = Event.Damagee;
}

void CGameEventReceiver::OnEnemyDeath(SEnemyDeathEvent const & Event) {
   fprintf(stderr, "Removing enemy %d\n", Event.Enemy);

   SDeadEnemy DeadEnemy;
   if (Event.Enemy.Actor->CollideableType == COLLIDEABLE_TYPE_PKIWI)
      DeadEnemy.DeathTimer = .0f;
   else
      DeadEnemy.DeathTimer = 6.f;
   DeadEnemy.Renderable = Event.Enemy.Renderable;

      DeadEnemy.Renderable->setScale(SVector3(1.f, 1.0f, 0.4f));
      DeadEnemy.Renderable->setTranslation(DeadEnemy.Renderable->getTranslation() - SVector3(0.f, 0.5f, 0.f));

      DeadEnemy.ParticleE = new CParticleEngine(DeadEnemy.Renderable->getTranslation(), 100, 6, DEATH_PARTICLE);

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
      if (it->DeathTimer < 0.f)
      {
         CApplication::get().getSceneManager().removeSceneObject(it->Renderable);
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
