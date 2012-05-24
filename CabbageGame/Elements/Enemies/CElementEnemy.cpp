#include "CElementEnemy.h"
#include "CGameLevel.h"

#include "CElementEnemyApple.h"
#include "CElementEnemyBlade.h"
#include "CElementEnemyOrange.h"
#include "CElementEnemyGrape.h"
#include "CElementEnemyKiwi.h"
#include "CElementEnemyProjectileKiwi.h"
#include "CElementEnemyFlame.h"
#include "CElementEnemyProjectileGrape.h"
#include "CElementEnemyGiantApple.h"
#include "CElementEnemyPear.h"
#include "CElementEnemyCherry.h"
#include "CElementEnemyProjectileCherry.h"
#include "CElementEnemyBanana.h"
#include "CElementEnemyProjectileBanana.h"
#include "CElementEnemyStrawberry.h"
#include "CElementEnemyPineapple.h"
#include "CElementEnemyPomegranate.h"
#include "CElementEnemyLemon.h"

#include "CElementItemSeed.h"
#include "CElementItemLife.h"
#include "CElementItemEnergy.h"

//Generic enemy, for usage in the LWIB, I guess.
CElementEnemy::CElementEnemy(SRect2f nArea, Enemies::EEnemyType type)
: CGameplayElement((CCollideable *&)PhysicsEngineObject, (ISceneObject *&)SceneObject, nArea), PhysicsEngineObject(NULL),
  SceneObject(NULL), Type(type), MaxHealth(1), CurHealth(MaxHealth), TimeToDeath(-1.0f), OldRot(SVector3f(0.0f)), InvincibilityTime(0.0f) {
}

//Enemy created by factory
CElementEnemy *CEnemyLoader::LoadEnemy(SRect2f nArea, Enemies::EEnemyType type) {
   switch(type) {
   case Enemies::APPLE:
      return new CElementEnemyApple(nArea);
      break;
   case Enemies::ORANGE:
      return new CElementEnemyOrange(nArea);
      break;
   case Enemies::KIWI:
      return new CElementEnemyKiwi(nArea, 0);
      break;
   case Enemies::GRAPE:
      return new CElementEnemyGrape(nArea);
      break;
   case Enemies::FLAME:
      return new CElementEnemyFlame(nArea);
      break;
   case Enemies::BLADE:
      return new CElementEnemyBlade(nArea);
      break;
   case Enemies::KIWI_PROJECTILE:
      return new CElementEnemyProjectileKiwi(nArea);
      break;
   case Enemies::GRAPE_PROJECTILE:
      return new CElementEnemyProjectileGrape(nArea);
      break;
   case Enemies::GIANT_APPLE:
      return new CElementEnemyGiantApple(nArea);
      break;
   case Enemies::PEAR:
      return new CElementEnemyPear(nArea);
      break;
   case Enemies::CHERRY:
      return new CElementEnemyCherry(nArea);
      break;
   case Enemies::CHERRY_PROJECTILE:
      return new CElementEnemyProjectileCherry(nArea, 0.0f);
      break;
   case Enemies::BANANA:
      return new CElementEnemyBanana(nArea);
      break;
   case Enemies::BANANA_PROJECTILE:
      return new CElementEnemyProjectileBanana(nArea);
      break;
   case Enemies::PINEAPPLE:
      return new CElementEnemyPineapple(nArea);
      break;
   case Enemies::POMEGRANATE:
      return new CElementEnemyPomegranate(nArea);
      break;
   case Enemies::STRAWBERRY:
      return new CElementEnemyStrawberry(nArea);
      break;
   case Enemies::LEMON:
      return new CElementEnemyLemon(nArea);
      break;
   default:
      fprintf(stderr, "Error: unknown enemy type %d\n", type);
      exit(1);
      break;
   }
}

void CElementEnemy::setupPhysicsEngineObject() {
   PhysicsEngineObject->setArea(SRect2f(Area.Position.X, Area.Position.Y, Area.Size.X - .11f, Area.Size.Y - .11f));

   if (Level.getEnv() == Env::WATER) {
      PhysicsEngineObject->getActorAttributes().MaxWalk *= 0.5f;
      PhysicsEngineObject->getActorAttributes().WalkAccel *= 0.25f;
      PhysicsEngineObject->getActorAttributes().JumpAccel *= 0.75f;
      PhysicsEngineObject->getActorAttributes().JumpLength *= 2.0f;
      PhysicsEngineObject->getActorAttributes().AirControl *= 8.0f;
      PhysicsEngineObject->getActorAttributes().AirSpeedFactor *= 0.5f;
   }
}

void CElementEnemy::writeXML(xmlwriter *l) {
    std::stringstream xValue, yValue, widthValue, heightValue, tagValue, eType;
    xValue << Area.Position.X;
    yValue << Area.Position.Y;
    widthValue << Area.Size.X;
    heightValue << Area.Size.Y;
    tagValue << "CEnemy";
    eType << Type;
    //put code for type
    // 0: apple 1: orange
    l->AddAtributes("type ", eType.str());
    l->AddAtributes("width ", widthValue.str());
    l->AddAtributes("height ", heightValue.str());
    l->AddAtributes("Y ", yValue.str());
    l->AddAtributes("X ", xValue.str());
    l->Createtag(tagValue.str());
    l->CloseLasttag();
}

void CElementEnemy::dieWithSeeds() {
   if(Dead)
      return;
   Area.Position.Y += 0.3f;
   for(int i = 0; i < rand()%7 + 3; i++) {
      CElementItem *seed;
      Level.addItem(seed = CItemLoader::LoadItem(Area, Items::SEED));

      float rand1 = (float)rand()/(float)RAND_MAX;
      float rand2 = (float)rand()/(float)RAND_MAX;

      ((CCollisionActor *)seed->getPhysicsEngineObject())->addImpulse(SVector2f(rand1*8.f - 4.f, rand2*4.5f + 1.0f));
   }
   if (rand()%3 == 0)
      dropItem();
   Area.Position.Y -= 0.3f;
   TempTime = 0.0f;
   removeFromPhysicsEngine();
   ParticleEngine = new CParticleEngine(SceneObject->getTranslation(), 20, 4, BURST_PARTICLE, Level.isNight());
   ParticleEngine->UsePhysics(&Level.getPhysicsEngine(), Level.getEnv());
   die();
}

void CElementEnemy::dropItem() {
   int random = rand() % 5;
   CElementItem *item;

   //Drop health
   if (random < 3) {
      Level.addItem(item = CItemLoader::LoadItem(Area, Items::HEALTH));
   }

   //Drop energy
   else
      Level.addItem(item = CItemLoader::LoadItem(Area, Items::ENERGY));

   float rand1 = (float)rand()/(float)RAND_MAX;
   float rand2 = (float)rand()/(float)RAND_MAX;

   ((CCollisionActor *)item->getPhysicsEngineObject())->addImpulse(SVector2f(rand1*8.f - 4.f, rand2*4.5f + 1.0f));
}

void CElementEnemy::OnCollision(const SCollisionEvent& Event) {
   if(!Dead && Event.Other == Level.getPlayer().getPhysicsEngineObject()) {
      if(TimeToDeath > 0.0f)
         return;
      CCollisionActor * PlayerActor = (CCollisionActor *)Level.getPlayer().getPhysicsEngineObject();

      //Check if jumped on top of enemy.
      if(Level.getPlayer().getArea().Position.Y > PhysicsEngineObject->getArea().otherCorner().Y - 0.05f) {
    	  if (InvincibilityTime <= 0.0f) {
    		  takeDamage(1);
    		  InvincibilityTime = .2f;
    	  }
      }

      //Did the player run into them?
      else {
         if(Level.getPlayer().decrementHealth()) {
            if(PlayerActor->getArea().getCenter().X > Area.getCenter().X)
               PlayerActor->addImpulse(SVector2f(7.f, 2.8f));
            else
               PlayerActor->addImpulse(SVector2f(-7.f, 2.8f));
            Level.getPlayer().setShaking(1.0f, 3.0f);
         }
      }
   }
}

void CElementEnemy::updatePhysicsEngineObject(float time) {
	InvincibilityTime -= time;

	if(TimeToDeath > 0.0f) {
		PhysicsEngineObject->setAction(CCollisionActor::EActionType::None);
		TimeToDeath -= time;
		if(TimeToDeath <= 0.0f) {
			dieWithSeeds();
		}
	}
}

void CElementEnemy::updateSceneObject(float time) {
   if(ParticleEngine) {
      ParticleEngine->step(time);
      ParticleEngine->setCenterPos(SVector3f(Area.getCenter(), 0.0f));
   }
   else if(TimeToDeath > 0.0f) {
      SceneObject->setRotation(OldRot + SVector3f(0.0f, 720.0f*time, 0.0f));
      OldRot = SceneObject->getRotation();
      //printf("Rot is %0.2f %0.2f %0.2f\n", OldRot.X, OldRot.Y, OldRot.Z);
   }
   return;
}

Enemies::EEnemyType CElementEnemy::getEnemyType() {
   return Type;
}

void CElementEnemy::printInformation() {
   printf("CElementEnemy; Area: [[%0.0f, %0.0f],[%0.0f, %0.0f]]; Type: %d\n", Area.Position.X, Area.Position.Y, Area.Size.X, Area.Size.Y, Type);
}

void CElementEnemy::reactToAbility(Abilities::EAbilityType Ability) {
   if (InvincibilityTime <= 0.0f) {
      InvincibilityTime = .1f;
   }
   SVector2f PlayerVelocity = ((CCollisionActor*)Level.getPlayer().getPhysicsEngineObject())->getVelocity();
   SVector2f endImpulse = (SVector2f(PlayerVelocity.X > 0.0f ? 6.0f : -6.0f, 2.0f) * 3.0f);
   switch(Ability) {
      case Abilities::SHIELD:
         ((CCollisionActor*)PhysicsEngineObject)->addImpulse((PlayerVelocity + SVector2f(0.0f, 2.5f)) * 3.0f);
         //dieWithSeeds();
         break;
      case Abilities::HEADBUTT:
         if(TimeToDeath == -1.0f) {
            printf("Headbutt w/ end impulse: %0.2f %0.2f\n", endImpulse.X, endImpulse.Y); 
            Level.getPlayer().setShaking(0.25f, 0.7f);
            ((CCollisionActor*)PhysicsEngineObject)->addImpulse(endImpulse);
            ((CCollisionActor*)PhysicsEngineObject)->getAttributes().AirStandingFriction = 0.99f;
            ((CCollisionActor*)PhysicsEngineObject)->setGravityEnabled(false);
            ((CCollisionActor*)PhysicsEngineObject)->setTypeId(INTERACTOR_NONCOLLIDERS);
            ((CCollisionActor*)PhysicsEngineObject)->setCollisionMask(INTERACTOR_BLOCKS);
            ((CCollisionActor*)PhysicsEngineObject)->setDetectionMask(INTERACTOR_SUPERACTORS);
            ((CCollisionActor*)PhysicsEngineObject)->OnPhaseBegin.connect(this, &CElementEnemy::OnCollision);
            OldRot = SceneObject->getRotation();
            TimeToDeath = 0.5f;
         }
         break;
      case Abilities::LASER:
         if(!Dead)
            takeDamage(3);
         break;
      default:
         break;
   }
}

int CElementEnemy::getHealth() {
   return CurHealth;
}

int CElementEnemy::heal(int amount) {

   CurHealth = std::min(MaxHealth, CurHealth + amount);

   return CurHealth;
}

void CElementEnemy::die() {

}

int CElementEnemy::takeDamage(int amount) {
	printf("CurHealth: %d, amount: %d\n", CurHealth, amount);
   CurHealth = std::max(0, CurHealth - amount);

   CCollisionActor * PlayerActor = (CCollisionActor *)Level.getPlayer().getPhysicsEngineObject();

   PlayerActor->addImpulse(SVector2f(0.0f, 9.0f));

   Mix_PlayChannel(-1, Level.dmgEnemy, 0);
   Level.getPlayer().setShaking(0.4f, 3.0f);

   //The enemy was killed.
   if (CurHealth <= 0) {
      dieWithSeeds();
      PlayerActor->addImpulse(SVector2f(0.0f, 3.0f));
   }

   return CurHealth;
}

void CElementEnemy::setTimeToDeath(float t) {
   TimeToDeath = t;
}
