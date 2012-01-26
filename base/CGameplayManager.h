#ifndef _CGAMEPLAYEMANAGER_H_INCLUDED_
#define _CGAMEPLAYEMANAGER_H_INCLUDED_

#include "../CabbageCollider/CEngine.h"

class CGameplayManager : public Cabbage::Collider::ICollisionResponder
{

public:

	struct SEnemy
	{
		Cabbage::Collider::CActor * Actor;
	};

	typedef std::vector<SEnemy> EnemyList;
	EnemyList Enemies;
	EnemyList KillList;

private:

	Cabbage::Collider::CActor * PlayerActor;
	int PlayerHealth;
	float PlayerRecovering;

	Cabbage::Collider::CEngine * Engine;

public:

	CGameplayManager(Cabbage::Collider::CActor * playerActor, Cabbage::Collider::CEngine * engine)
		: PlayerActor(playerActor), PlayerRecovering(false), PlayerHealth(5), Engine(engine)
	{
		Engine->setCollisionResponder(this);
	}

	void OnCollision(Cabbage::Collider::CCollideable * Object, Cabbage::Collider::CCollideable * With)
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
						-- PlayerHealth;
						PlayerRecovering = 1.5f;

						float const KnockbackSpeed = 7.f;
						float const KnockbackDuration = 0.2f;

						if (PlayerActor->getArea().getCenter().X > Other->getArea().getCenter().X)
							PlayerActor->setImpulse(SVector2(1.f, 1.f) * KnockbackSpeed, KnockbackDuration);
						else
							PlayerActor->setImpulse(SVector2(-1.f, 1.f) * KnockbackSpeed, KnockbackDuration);
					}
				}
				break;
			}
	}

	bool const isPlayerAlive() const
	{
		return PlayerHealth > 0;
	}
	
	int const getPlayerHealth() const
	{
		return PlayerHealth;
	}

	void run(float const TickTime)
	{
		if (PlayerRecovering > 0.f)
			PlayerRecovering -= TickTime;

		for (EnemyList::iterator it = KillList.begin(); it != KillList.end(); ++ it)
		{
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

	void addEnemy(SVector2 const & Position)
	{
		SEnemy enemy;
		enemy.Actor = Engine->addActor();
		enemy.Actor->setArea(SRect2(Position, 1));
		enemy.Actor->getAttributes().MaxWalk = 1.2f;
		Enemies.push_back(enemy);
	}

};

#endif
