#include "CGameplayManager.h"
#include "CGameEventManager.h"

CGameplayManager::CGameplayManager(Cabbage::Collider::CActor * playerActor, Cabbage::Collider::CEngine * engine)
    : PlayerActor(playerActor), PlayerRecovering(false), PlayerHealth(5), Engine(engine)
{
    Engine->setCollisionResponder(this);
    GameEventManager = new CGameEventManager();
    won = 0;
    GodMode = 0;
}

void CGameplayManager::OnCollision(Cabbage::Collider::CCollideable * Object, Cabbage::Collider::CCollideable * With)
{
    Cabbage::Collider::CCollideable * Other = 0;
    Cabbage::Collider::CCollideable * PlayerCollideable = 0;
    Cabbage::Collider::CCollideable * Flag = 0;
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
    if (With == VictoryFlag)
       Flag = With;

    int birdCollision = 0;
    //printf("With's type: %d, Object's type: %d\n", With->CollideableType, Object->CollideableType);
    if((With->CollideableType == COLLIDEABLE_TYPE_ACTOR || With == PlayerActor || With->CollideableType == COLLIDEABLE_TYPE_BLOCK)
          && Object->CollideableType == COLLIDEABLE_TYPE_KIWI) {
       //printf("Hey 15\n");
       //SRect2 area = ((Cabbage::Collider::CActor*)Object)->getArea();
       //((Cabbage::Collider::CActor*)Object)->setArea(SRect2(area.Position.X + 1.5, area.Position.Y + 0.2, area.Size.X, area.Size.Y));
       ((Cabbage::Collider::CActor*)Object)->setImpulse(SVector2(0.03f, 0.3f)*7, 0.2);
       if(With->CollideableType == COLLIDEABLE_TYPE_ACTOR) {
          ((Cabbage::Collider::CActor*)With)->setImpulse(SVector2(0.00f, -0.3f), 0.2);
       }
       birdCollision = 1;
    }

    if (Object == PlayerActor && With->CollideableType == COLLIDEABLE_TYPE_FLAME) {
    }

    if(GodMode && birdCollision == 1 && PlayerCollideable) {
        PlayerActor->setImpulse(SVector2(0.f, -0.4f) * 7, 0.5);
    }

    if (! Other || GodMode)
        return;
   
    float const HitThreshold = 0.05f;

    if(PlayerCollideable && Flag)
       won = 1;
    if(won) {
       return;
    }


    for (EnemyList::iterator it = Enemies.begin(); it != Enemies.end(); ++ it)
    {
        if (Other == (*it)->Actor)
        {

            if (PlayerActor->getArea().Position.Y > Other->getArea().otherCorner().Y - HitThreshold && (*it)->Actor->CollideableType != COLLIDEABLE_TYPE_FLAME)
            {
                Mix_PlayChannel(-1, killEnemy, 0);
                KillList.push_back(* it);
                //fprintf(stderr, "Enemy detected as dead! %d\n", it->Renderable);

                Enemies.erase(it);
            }
            //Need to rewrite so works without SEnemy
            else
            {
                if (isPlayerAlive() && PlayerRecovering <= 0.f)
                {
                    SPlayerDamagedEvent Event;
                    SEnemy enemy;
                    enemy.Actor = (*it)->Actor;
                    enemy.Renderable = (*it)->Renderable;

                    Event.DamagedBy = enemy;
                    GameEventManager->OnPlayerDamaged(Event);

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
                    float const KnockbackDuration = 0.2f;

                    PlayerRecovering = KnockbackDuration*3;

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
            break;
        }
    }
}

SVector2 CGameplayManager::getPlayerLocation() {
   return SVector2(PlayerActor->getArea().getCenter().X, PlayerActor->getArea().getCenter().Y);
}

void CGameplayManager::setVictoryFlag(Cabbage::Collider::CObject * f) {
   VictoryFlag = f;
}


bool const CGameplayManager::isWon() const
{
    return won != 0; 
}

bool const CGameplayManager::isPlayerAlive() const
{
    return (PlayerHealth > 0) && (-5.0 < PlayerActor->getArea().getCenter().Y);
}
    
int const CGameplayManager::getPlayerHealth() const
{
    return PlayerHealth;
}

bool const CGameplayManager::isJumping() const {
   return PlayerActor->isJumping();
}

void CGameplayManager::run(float const TickTime)
{
    if (PlayerRecovering > 0.f)
        PlayerRecovering -= TickTime;

    for (EnemyList::iterator it = KillList.begin(); it != KillList.end(); ++ it)
    {
       SEnemyDeathEvent Event;
       SEnemy enemy;
       enemy.Actor = (*it)->Actor;
       enemy.Renderable = (*it)->Renderable;
       Event.Enemy = enemy;

       GameEventManager->OnEnemyDeath(Event);

       Engine->removeActor((*it)->Actor);
    }

    KillList.clear();


    float cabbageCenterX = PlayerActor->getArea().getCenter().X;
    float cabbageCenterY = PlayerActor->getArea().getCenter().Y;

    for (EnemyList::iterator it = Enemies.begin(); it != Enemies.end(); ++ it) {
       float enemyCenterX = (*it)->Actor->getArea().getCenter().X;
       float enemyCenterY = (*it)->Actor->getArea().getCenter().Y;

       if ((enemyCenterX < cabbageCenterX + 7 && enemyCenterX > cabbageCenterX - 7) && (enemyCenterY < cabbageCenterY + 7 && enemyCenterY > cabbageCenterY - 7))
          (*it)->update(TickTime);
    }
}

Cabbage::Collider::CEngine* CGameplayManager::getEngine() {
   return Engine;
}


CGameEventManager & CGameplayManager::getGameEventManager()
{
    return * GameEventManager;
}
