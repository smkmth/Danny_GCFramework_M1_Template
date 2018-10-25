#ifndef _GCOBJENEMY_H_
#define _GCOBJENEMY_H_

#ifndef _GCOBJSPRITEPHYSICS_H_
	#include "../../GCCocosInterface/GCObjSpritePhysics.h"
#endif

class CGCObjEnemy 
: public CGCObjSpritePhysics
{
private:

	// Sprite size and position variables
	float m_fSpriteWidth;
	float m_fSpriteHeight;
	float m_fCurrentXPos;
	float m_fCurrentYPos;

	// Patrolling variables
		// Patrol movement vector, sets the force at which the enemy moves
		b2Vec2 m_v2PatrolMovement;
		// Patrol enum, decides what direction the enemy patrols in (left&right or up&down)
		enum m_ePatrollingDirectionEnum { LeftAndRight, UpAndDown };
		m_ePatrollingDirectionEnum m_ePatrolDirection = UpAndDown;
		// Float patrol points, decides the minimum and maximum points that the enemy will move to
		float m_fPatrolMinimumPoint;
		float m_fPatrolMaximumPoint;
		// Float patrolling speed
		float m_fPatrolSpeed;
		float m_fPatrolSpeedMax;
		// Bool to flip the patrol speed, sends the enemy the other way
		bool m_bPatrolSpeedFlipped;


public:

	// Constructor
	CGCObjEnemy();

	//////////////////////////////////////////////////////////////////////////
	// We need a virtual destructor since delete will be called on pointers of 
	// this class to delete derived types.
	virtual ~CGCObjEnemy()
	{}

	//////////////////////////////////////////////////////////////////////////
	// Overridden virtuals from the game object interface
	virtual void VOnResourceAcquire	( void ) override;

	virtual void VOnResurrected		( void ) override;

	virtual void VOnUpdate(f32 fTimeStep) override;

	// Overridden virtuals from the game object interface
	//////////////////////////////////////////////////////////////////////////


	// Getters and Setters
		// Setters
	void setPatrolMinimumPoint(float _input);
	void setPatrolMaximumPoint(float _input);
	void setPatrolDirection(int _input);
};
#endif // #ifndef _GCOBJEnemy_H_