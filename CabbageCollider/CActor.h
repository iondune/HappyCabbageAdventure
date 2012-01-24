#ifndef _CABBAGECOLLIDER_CACTOR_H_INCLUDED_
#define _CABBAGECOLLIDER_CACTOR_H_INCLUDED_

#include "../CabbageCore/SRect2.h"

#include "CObject.h"

namespace Cabbage
{
namespace Collider
{

	class ECollisionType
	{

	public:

		enum Value
		{
			None = 0,
			Up = 1,
			Down = 2,
			Left = 4,
			Right = 8
		};

	};

	class CActor : public CCollideable
	{

	public:

		struct SAttributes
		{
			float MoveAccel;
			float JumpSpeed;
			float AirMod;
			float AirCap;

			SAttributes()
				: MoveAccel(50.8f), JumpSpeed(8.f), AirMod(0.3f), AirCap(0.97f)
			{}
		};

		class EActionType
		{

		public:

			enum Domain
			{
				Standing,
				MoveLeft,
				MoveRight
			};

		private:

			Domain Value;

		public:

			EActionType()
				: Value(Standing)
			{}

			EActionType(Domain const value)
				: Value(value)
			{}

			bool const operator == (Domain const value)
			{
				return Value == value;
			}
		};

	protected:

		friend class CEngine;

		SVector2 Acceleration;
		SVector2 Velocity;
		bool Standing;

		SVector2 LastPosition, Movement;

		SAttributes Attributes;
		EActionType Action;
		bool Jump;

		CActor();

	public:
		
		~CActor();

		bool collidesWith(CObject * Object);

		int checkCollision(CCollideable * Object, float const TickTime);
		void onStanding();

		void updateVectors(float const TickTime);

		void pushIfCollided(CObject * Object, SVector2 const & Movement);

		void setAcceleration(SVector2 const & accel)
		{
			Acceleration = accel;
		}

		SVector2 const & getAcceleration() const
		{
			return Acceleration;
		}

		bool const isStanding() const
		{
			return Standing;
		}

		void setVelocity(SVector2 const & vel)
		{
			Velocity = vel;
		}

		SVector2 const & getVelocity() const
		{
			return Velocity;
		}

		SAttributes const & getAttributes() const
		{
			return Attributes;
		}

		void setAction(EActionType const & action)
		{
			Action = action;
		}

		void jump()
		{
			Jump = true;
		}

		bool updateCollision(CCollideable * Object, float const TickTime);

		void draw();

	};
}
}

#endif
