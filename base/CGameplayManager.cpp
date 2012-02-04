#include "CGameplayManager.h"
#include "CGameEventManager.h"

CGameplayManager::CGameplayManager(Cabbage::Collider::CActor * playerActor, Cabbage::Collider::CEngine * engine)
    : PlayerActor(playerActor), PlayerRecovering(false), PlayerHealth(5), Engine(engine)
{
    Engine->setCollisionResponder(this);
    GameEventManager = new CGameEventManager();
}

void CGameplayManager::OnCollision(Cabbage::Collider::CCollideable * Object, Cabbage::Collider::CCollideable * With)
{
    Cabbage::Collider::CCollideable * Other = 0;
    if (Object == PlayerActor)
        Other = With;
    if (With == PlayerActor)
        Other = Object;

    if (! Other)
        return;

    float const HitThreshold = 0.05f;

    for (EnemyList::iterator it = Enemies.begin(); it != Enemies.end(); ++ it)
    if (Other == it->Actor)
    {
        if (PlayerActor->getArea().Position.Y > Other->getArea().otherCorner().Y - HitThreshold)
        {
            KillList.push_back(* it);
            Enemies.erase(it);
        }
        else
        {
            if (isPlayerAlive() && PlayerRecovering <= 0.f)
            {
                SPlayerDamagedEvent Event;
                Event.DamagedBy = & * it;
                GameEventManager->OnPlayerDamaged(Event);

                -- PlayerHealth;

                if (PlayerHealth <= 0)
                {
                    SPlayerDeathEvent Event;
                    Event.KilledBy = & * it;
                    GameEventManager->OnPlayerDeath(Event);
                }

                //Chris Code.  Damage Sound plays here
                if(playTakeDmg) {
                    Mix_PlayChannel(-1, takeDmg, 0);
                }

                float const KnockbackSpeed = 7.f;
                float const KnockbackDuration = 0.2f;

                PlayerRecovering = KnockbackDuration*3;

                if (PlayerActor->getArea().getCenter().X > Other->getArea().getCenter().X)
                    PlayerActor->setImpulse(SVector2(1.f, 0.5f) * KnockbackSpeed, KnockbackDuration);
                else
                    PlayerActor->setImpulse(SVector2(-1.f, 0.5f) * KnockbackSpeed, KnockbackDuration);
            }
        }
        break;
    }
}

bool const CGameplayManager::isPlayerAlive() const
{
    return PlayerHealth > 0;
}
    
int const CGameplayManager::getPlayerHealth() const
{
    return PlayerHealth;
}

void CGameplayManager::run(float const TickTime)
{
    if (PlayerRecovering > 0.f)
        PlayerRecovering -= TickTime;

    for (EnemyList::iterator it = KillList.begin(); it != KillList.end(); ++ it)
    {
        SEnemyDeathEvent Event;
        Event.Enemy = & * it;
        fprintf(stderr, "%d\n", &GameEventManager->OnEnemyDeath);
        GameEventManager->OnEnemyDeath(Event);

        Mix_PlayChannel(-1, killEnemy, 0);

        Engine->removeActor(it->Actor);
    }

    KillList.clear();

    for (EnemyList::iterator it = Enemies.begin(); it != Enemies.end(); ++ it)
    {
        if (isPlayerAlive())
        {
            if (PlayerActor->getArea().getCenter().X < it->Actor->getArea().getCenter().X)
                it->Actor->setAction(Cabbage::Collider::CActor::EActionType::MoveLeft);
            else
                it->Actor->setAction(Cabbage::Collider::CActor::EActionType::MoveRight);
        }
        else
        {
            it->Actor->setAction(Cabbage::Collider::CActor::EActionType::None);
        }
        //if (rand()%1000 == 1)
        //	it->Actor->setJumping(true);
    }
}

void CGameplayManager::addEnemy(SVector2 const & Position, void* renderable)
{
    SEnemy enemy;
    enemy.Actor = Engine->addActor();
    enemy.Actor->setArea(SRect2(Position, 1));
    enemy.Actor->getAttributes().MaxWalk = 1.2f;
    enemy.Renderable = renderable;
    Enemies.push_back(enemy);
}

CGameEventManager & CGameplayManager::getGameEventManager()
{
    return * GameEventManager;
}
