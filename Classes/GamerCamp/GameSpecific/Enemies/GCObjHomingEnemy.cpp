#include <memory.h>
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GamerCamp/GameSpecific/Enemies/GCObjHomingEnemy.h"


// Using cocos namespace, helps to shorten the use of some built-in functionality
USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
// GetGCTypeIDOf uses the template in GCTypeID to generate a unique ID for 
// this type - need this to construct our base type
CGCObjHomingEnemy::CGCObjHomingEnemy()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CGCObjEnemy ) )
{
}

// Virtual
// Initialisation for the enemies sprite
void CGCObjHomingEnemy::VOnResourceAcquire()
{
	const char* pszPlist = "TexturePacker/Sprites/PlaceHolderEnemies/Virus1Placeholder/Virus1.plist";
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile( pszPlist );
	AcquireResources( pszPlist, "Idle00", b2_dynamicBody, true );
	SetParent( IGCGameLayer::ActiveInstance() );
}


// Virtual
// Resurrect inherited function, respawns enemy
void CGCObjHomingEnemy::VOnResurrected( void )
{
	CGCObjSpritePhysics::VOnResurrected();
	GetPhysicsBody()->SetGravityScale( 0.0f );
}


// Virtual
// Master update for homing enemy
void CGCObjHomingEnemy::VOnUpdate( f32 fTimeStep )
{
	GetPhysicsBody()->ApplyForceToCenter( m_v2LastPlayerPos, true );
}


// Getters and Setters
	// Setters
void CGCObjHomingEnemy::SetPlayerDirection( b2Vec2 _target )
{
	m_v2LastPlayerPos = _target;
}