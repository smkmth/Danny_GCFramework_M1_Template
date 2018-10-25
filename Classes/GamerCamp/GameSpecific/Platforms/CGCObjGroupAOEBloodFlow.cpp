#include <string.h>
#include "GamerCamp/GameSpecific/Platforms/CGCObjGroupAOEBloodFlow.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GamerCamp/Core/GCTypes.h"
#include "GamerCamp/GCObject/GCObject.h"
#include "GamerCamp/GameSpecific/Platforms/GCObjAOEBloodFlow.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"

// Using cocos namespace, helps to shorten the use of some built-in functionality
USING_NS_CC;


// Constructor
CGCObjGroupAOEBloodFlow::CGCObjGroupAOEBloodFlow()
{
}


// Virtual 
// Destructor
CGCObjGroupAOEBloodFlow::~CGCObjGroupAOEBloodFlow()
{
}


// Virtual 
// Set ID to only handle the type of 'Platform' 
bool CGCObjGroupAOEBloodFlow::VHandlesThisTypeId( GCTypeID idQueryType )
{
	return(GetGCTypeIDOf( CGCObjPlatform ) == idQueryType);
}


// Virtual 
// Must return the typeid of the CGCObjectGroup derived class 
GCTypeID CGCObjGroupAOEBloodFlow::VGetTypeId( void )
{
	return GetGCTypeIDOf( CGCObjGroupAOEBloodFlow );
}


// Virtual
// Release resources for created enemies, call 'DestroyPlatforms'
void CGCObjGroupAOEBloodFlow::VOnGroupResourceRelease( void )
{
	CGCObjectGroup::VOnGroupResourceRelease();
	DestroyPlatforms();
}


// Remove platforms from scene
void CGCObjGroupAOEBloodFlow::DestroyPlatforms( void )
{
	// this iterates the array of registered CGCObjects 
	// calling the supplied functor then deleting them
	DestroyObjectsReverseOrder( [&]( CGCObject* pObject )
	{
		GCASSERT( GetGCTypeIDOf( CGCObjGroupAOEBloodFlow ) == pObject->GetGCTypeID(), "wrong type!" );
		CGCObjSprite* pProjectileAsSprite = static_cast<CGCObjSprite*>(pObject);
		pProjectileAsSprite->DestroySprite();
	} );
}