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

			SAttributes();
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

			EActionType();
			EActionType(Domain const value);

			bool const operator == (Domain const value);

		};

	protected:

		friend class CEngine;

		float FallAcceleration;
		SVector2 Velocity;
		bool Standing;

		SVector2 LastPosition, Movement;

		SAttributes Attributes;
		EActionType Action;
		bool Jump;

		CActor();

		// Update functions, used by engine
		void updateVectors(float const TickTime);
		
		bool updateCollision(CCollideable * Object, float const TickTime);
		int checkCollision(CCollideable * Object, float const TickTime);
		void onStanding();

		void pushIfCollided(CObject * Object, SVector2 const & Movement);

	public:
		
		~CActor();

		bool collidesWith(CObject * Object) const;
		bool isAbove(CObject * Object, float & height) const;

		void setAcceleration(SVector2 const & accel);
		SVector2 const & getAcceleration() const;

		bool const isStanding() const;

		void setVelocity(SVector2 const & vel);
		SVector2 const & getVelocity() const;

		SAttributes const & getAttributes() const;

		// Action control
		void setAction(EActionType const & action);
		void jump();

		virtual void draw();

	};
}
}

#endif
