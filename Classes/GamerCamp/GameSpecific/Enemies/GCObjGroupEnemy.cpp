#include <string.h>
#include "GamerCamp/GameSpecific/Enemies/GCObjGroupEnemy.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GamerCamp/Core/GCTypes.h"
#include "GamerCamp/GCObject/GCObject.h"
#include "GamerCamp/GameSpecific/Enemies/GCObjEnemy.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"


// Using cocos namespace, helps to shorten the use of some built-in functionality
USING_NS_CC;


// Constructor
CGCObjGroupEnemy::CGCObjGroupEnemy()
{
}


// Virtual
// Destructor
CGCObjGroupEnemy::~CGCObjGroupEnemy()
{}


// Virtual 
// Set ID to only handle the type of 'Enemy'
bool CGCObjGroupEnemy::VHandlesThisTypeId( GCTypeID idQueryType )
{
	return(GetGCTypeIDOf( CGCObjEnemy ) == idQueryType);
}


// Virtual 
// Must return the typeid of the CGCObjectGroup derived class
GCTypeID CGCObjGroupEnemy::VGetTypeId( void )
{
	return GetGCTypeIDOf( CGCObjGroupEnemy );
}


// Virtual
// Initialisation
void CGCObjGroupEnemy::VOnGroupResourceAcquire( void )
{
	CreateEnemies();
	CGCObjectGroup::VOnGroupResourceAcquire();
}


// Virtual
// Initialisation post enemy creation
void CGCObjGroupEnemy::VOnGroupResourceAcquire_PostObject( void )
{
	// Parent class version
	CGCObjectGroup::VOnGroupResourceAcquire_PostObject();

	// Set up animations for all items
	/*const char* pszPlist = "TexturePacker/Sprites/PlaceHolderEnemies/Virus1Placeholder/Virus1.plist";
	const char* pszAnim_Fly = "Frames/Idle";

	// Make an animation
	ValueMap&	rdictPList = GCCocosHelpers::CreateDictionaryFromPlist( pszPlist );
	Animation*	pAnimation = GCCocosHelpers::CreateAnimation( rdictPList, pszAnim_Fly );

	// This is a workaround for the fact that Marmalade's version of GCC for ARM doesn't support lambdas
	SGCObjectGatherer sMyGatherer;
	ForEachObject( sMyGatherer );

	//Create an animation action and set it for each Enemy
	for (u32 uIndex = 0; uIndex < sMyGatherer.uCount; ++uIndex)
	{
		CGCObject* pcItemAsObject = sMyGatherer.apObjects[uIndex];
		CCAssert( (GetGCTypeIDOf( CGCObjEnemy ) == pcItemAsObject->GetGCTypeID()),
			"CGCObject derived type mismatch!" );

		CGCObjSprite* pItemSprite = (CGCObjSprite*)pcItemAsObject;
		pItemSprite->RunAction( GCCocosHelpers::CreateAnimationActionLoop( pAnimation ) );
	}*/
}


// Virtual
// Release resources for created enemies, call 'DestroyEnemies'
void CGCObjGroupEnemy::VOnGroupResourceRelease( void )
{
	CGCObjectGroup::VOnGroupResourceRelease();
	DestroyEnemies();
}


// Enemy creation
void CGCObjGroupEnemy::CreateEnemies( void )
{
	for (u32 uLoop = 0; uLoop < k_uNumEnemies; ++uLoop)
	{
		CGCObjEnemy* pEnemy = new CGCObjEnemy();
		pEnemy->SetResetPosition( m_v2EnemyPositionArray[uLoop] );
		pEnemy->setPatrolMinimumPoint( m_v2EnemyPatrolMinimumArray[uLoop] );
		pEnemy->setPatrolMaximumPoint( m_v2EnemyPatrolMaximumArray[uLoop] );
		pEnemy->setPatrolDirection( m_v2EnemyPatrolDirectionArray[uLoop] );
	}
}


// Remove enemies from scene
void CGCObjGroupEnemy::DestroyEnemies( void )
{
	// this iterates the array of registered CGCObjects 
	// calling the supplied functor then deleting them
	DestroyObjectsReverseOrder( [&]( CGCObject* pObject )
	{
		GCASSERT( GetGCTypeIDOf( CGCObjEnemy ) == pObject->GetGCTypeID(), "wrong type!" );
		CGCObjSprite* pProjectileAsSprite = static_cast<CGCObjSprite*>(pObject);
		pProjectileAsSprite->DestroySprite();
	} );
}


// Getters and Setters
	// Getters
int CGCObjGroupEnemy::GetNumberOfEnemies()
{
	return k_uNumEnemies;
}
	// Setters
void CGCObjGroupEnemy::SetEnemyPosition( int _iEnemyNumber, b2Vec2 _v2EnemyPosition )
{
	m_v2EnemyPositionArray[_iEnemyNumber - 1] = _v2EnemyPosition;
}
void CGCObjGroupEnemy::SetEnemyPatrol( int _iEnemyNumber, float _fpatrolMinimumPoint, float _fpatrolMaximumPoint, int _iPatrolDirection )
{
	m_v2EnemyPatrolMinimumArray[_iEnemyNumber - 1] = _fpatrolMinimumPoint;
	m_v2EnemyPatrolMaximumArray[_iEnemyNumber - 1] = _fpatrolMaximumPoint;
	m_v2EnemyPatrolDirectionArray[_iEnemyNumber - 1] = _iPatrolDirection;
}