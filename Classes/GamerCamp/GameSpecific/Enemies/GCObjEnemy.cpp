#include <memory.h>
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GCObjEnemy.h"

// Using cocos namespace, helps to shorten the use of some built-in functionality
USING_NS_CC;


//////////////////////////////////////////////////////////////////////////
// GetGCTypeIDOf uses the template in GCTypeID to generate a unique ID for 
// this type - need this to construct our base type
CGCObjEnemy::CGCObjEnemy()
: CGCObjSpritePhysics( GetGCTypeIDOf( CGCObjEnemy ) )
{	
}


// Virtual 
// Initialisation for the enemies sprite
void CGCObjEnemy::VOnResourceAcquire()
{
	//Initialising
	const char* pszPlist = "TexturePacker/Sprites/Bacteria/BacteriaBrain.plist";
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile( pszPlist );
	AcquireResources( pszPlist, "BacteriaBrain", b2_dynamicBody, true );
	SetParent( IGCGameLayer::ActiveInstance() );
	m_fSpriteWidth = GetBoundingBox().size.width;
	m_fSpriteHeight = GetBoundingBox().size.height;
	m_fCurrentXPos = GetSpritePosition().x;
	m_fCurrentYPos = GetSpritePosition().y;

		//Patrolling
	m_fPatrolSpeedMax = 20.0f;
	m_fPatrolSpeed = m_fPatrolSpeedMax;
	m_bPatrolSpeedFlipped = false;
}


// Virtual 
// Resurrect inherited function, respawns enemy
void CGCObjEnemy::VOnResurrected( void )
{
	CGCObjSpritePhysics::VOnResurrected();
	GetPhysicsBody()->SetGravityScale( 0.0f );
}


// Virtual 
// Master update for enemy
void CGCObjEnemy::VOnUpdate(f32 fTimeStep)
{
	// Update sprite position variables
	m_fCurrentXPos = GetSpritePosition().x;
	m_fCurrentYPos = GetSpritePosition().y;

	// Enemy movement switch, chooses the direction of the enemy's patrol based on 'm_PatrolDirection'
	switch (m_ePatrolDirection)
	{
	// LEFT AND RIGHT PATROL
	case CGCObjEnemy::LeftAndRight:
		// If enemy passes their minimum or maximum patrol point
		if (GetSpritePosition().x <= m_fPatrolMinimumPoint || GetSpritePosition().x >= m_fPatrolMaximumPoint)
		{
			if (m_bPatrolSpeedFlipped == false)
			{
				// Flip movement and sprite x pos
				m_fPatrolSpeed = m_fPatrolSpeed * -1;
				m_v2PatrolMovement.Set(m_fPatrolSpeed, 0.0f);
				if (GetSpritePosition().x <= m_fPatrolMaximumPoint)
				{
					SetFlippedX(true);
				}
				if (GetSpritePosition().x >= m_fPatrolMaximumPoint)
				{
					SetFlippedX(false);
				}
				m_bPatrolSpeedFlipped = true;
				// Slow the enemy, so that they can change their direction
				GetPhysicsBody()->SetLinearVelocity(b2Vec2_zero);
			}
		}
		// If enemy is still within their minimum and maximum patrol points
		else if (GetSpritePosition().x > m_fPatrolMinimumPoint && GetSpritePosition().x < m_fPatrolMaximumPoint)
		{
			m_bPatrolSpeedFlipped = false;
			m_v2PatrolMovement.Set(m_fPatrolSpeed, 0.0f);
		}
		break;

	// UP AND DOWN PATROL
	case CGCObjEnemy::UpAndDown:
		// If enemy passes their minimum or maximum patrol point
		if (GetSpritePosition().y <= m_fPatrolMinimumPoint || GetSpritePosition().y >= m_fPatrolMaximumPoint)
		{
			if (m_bPatrolSpeedFlipped == false)
			{
				// Flip movement and sprite x pos
				m_fPatrolSpeed = m_fPatrolSpeed * -1;
				m_v2PatrolMovement.Set(0.0f, m_fPatrolSpeed);
				m_bPatrolSpeedFlipped = true;
				// Slow the enemy, so that they can change their direction
				GetPhysicsBody()->SetLinearVelocity(b2Vec2_zero);
			}
		}
		// If enemy is still within their minimum and maximum patrol points
		else if (GetSpritePosition().y > m_fPatrolMinimumPoint && GetSpritePosition().y < m_fPatrolMaximumPoint)
		{
			m_bPatrolSpeedFlipped = false;
			m_v2PatrolMovement.Set(0.0f, m_fPatrolSpeed);
		}
		break;
	}
	
	//Apply movement to the enemy
	GetPhysicsBody()->ApplyForceToCenter(m_v2PatrolMovement, true);
}


// Getters and Setters
	// Setters
void CGCObjEnemy::setPatrolMinimumPoint(float _input)
{
	m_fPatrolMinimumPoint = _input;
}
void CGCObjEnemy::setPatrolMaximumPoint(float _input)
{
	m_fPatrolMaximumPoint = _input;
}
void CGCObjEnemy::setPatrolDirection(int _input)
{
	switch (_input)
	{
	case 0:
		m_ePatrolDirection = LeftAndRight;
		break;
	case 1:
		m_ePatrolDirection = UpAndDown;
		break;
	default:
		m_ePatrolDirection = LeftAndRight;
		break;
	}
}