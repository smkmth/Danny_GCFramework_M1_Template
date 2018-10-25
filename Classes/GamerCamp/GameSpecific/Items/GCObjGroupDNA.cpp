#include <string.h>
#include "GamerCamp/GameSpecific/Items/GCObjGroupDNA.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GamerCamp/Core/GCTypes.h"
#include "GamerCamp/GCObject/GCObject.h"
#include "GamerCamp/GameSpecific/Items/GCObjDNA.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"


// Using cocos namespace, helps to shorten the use of some built-in functionality
USING_NS_CC;


// Constructor
CGCObjGroupDNA::CGCObjGroupDNA()
{
}


// Virtual
// Destructor
CGCObjGroupDNA::~CGCObjGroupDNA()
{}


// Virtual
// Set ID to only handle the type of 'Item'
bool CGCObjGroupDNA::VHandlesThisTypeId( GCTypeID idQueryType )
{
	return( GetGCTypeIDOf( CGCObjItem ) == idQueryType );
}


// Virtual 
// Must return the typeid of the CGCObjectGroup derived class
GCTypeID CGCObjGroupDNA::VGetTypeId( void )
{
	return GetGCTypeIDOf( CGCObjGroupDNA );
}


// Virtual 
// Initialisation post item creation
void CGCObjGroupDNA::VOnGroupResourceAcquire_PostObject( void )
{
	// Parent class version
	CGCObjectGroup::VOnGroupResourceAcquire_PostObject();

	// Set up animations for all items
	//const char* pszPlist_Coin	        = "TexturePacker/Sprites/Coin/Coin.plist";
	//const char* pszAnim_Coin_Rotate		= "Rotate";

	// Make an animation
	//ValueMap&	rdicPList	= GCCocosHelpers::CreateDictionaryFromPlist( pszPlist_Coin );
	//Animation*	pAnimation	= GCCocosHelpers::CreateAnimation( rdicPList, pszAnim_Coin_Rotate );

	// This is a workaround for the fact that Marmalade's version of GCC for ARM doesn't support lambdas
	SGCObjectGatherer sMyGatherer;
	ForEachObject( sMyGatherer );

	// Create an animation action and set it for each item
	for( u32 uIndex = 0; uIndex < sMyGatherer.uCount; ++uIndex )
	{
		CGCObject* pcItemAsObject = sMyGatherer.apObjects[ uIndex ];
		CCAssert(	( GetGCTypeIDOf( CGCObjItem ) == pcItemAsObject->GetGCTypeID() ), 
			"CGCObject derived type mismatch!" );

		CGCObjSprite* pItemSprite = (CGCObjSprite*) pcItemAsObject;
		//pItemSprite->RunAction( GCCocosHelpers::CreateAnimationActionLoop( pAnimation ) );
	}
}


// Virtual 
// Release resources for created items, call 'DestroyItems'
void CGCObjGroupDNA::VOnGroupResourceRelease( void )
{
	// n.b. this must happen first, as will fail if objects destroyed before 
	CGCObjectGroup::VOnGroupResourceRelease(); 
	DestroyItems();
}


// Destroy managed items, removing them from the scene
void CGCObjGroupDNA::DestroyItems( void )
{
	// This iterates the array of registered CGCObjects 
	// calling the supplied functor then deleting them
	DestroyObjectsReverseOrder( [&]( CGCObject* pObject )
	{
		GCASSERT( GetGCTypeIDOf( CGCObjItem ) == pObject->GetGCTypeID(), "wrong type!" );
		CGCObjSprite* pProjectileAsSprite = static_cast< CGCObjSprite* >( pObject );
		pProjectileAsSprite->DestroySprite();
	});
}