////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <string.h>

#include "GamerCamp/GameSpecific/Platforms/CGCObjGroupAOEParalysis.h"

#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GamerCamp/Core/GCTypes.h"
#include "GamerCamp/GCObject/GCObject.h"
#include "GamerCamp/GameSpecific/Platforms/GCObjAOEParalysis.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"



//////////////////////////////////////////////////////////////////////////
// using
using namespace cocos2d;



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CGCObjGroupAOEParalysis::CGCObjGroupAOEParalysis()
{
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
// virtual
CGCObjGroupAOEParalysis::~CGCObjGroupAOEParalysis()
{}

void CGCObjGroupAOEParalysis::VOnGroupResourceAcquire(void)
{

	CGCObjectGroup::VOnGroupResourceAcquire();
}


//////////////////////////////////////////////////////////////////////////
// only handle platforms
//////////////////////////////////////////////////////////////////////////
//virtual 
bool CGCObjGroupAOEParalysis::VHandlesThisTypeId( GCTypeID idQueryType )
{
	return( GetGCTypeIDOf(CGCObjGroupAOEParalysis) == idQueryType );
}



//////////////////////////////////////////////////////////////////////////
// must return the typeid of the CGCObjectGroup derived class
//////////////////////////////////////////////////////////////////////////
//virtual 
GCTypeID CGCObjGroupAOEParalysis::VGetTypeId( void )
{
	return GetGCTypeIDOf( CGCObjGroupAOEParalysis );
}

struct SArrayOfAOEParalysis
{
	u32 uCount;
	CGCObjGroupAOEParalysis* apAOEParalysis[CGCObjectGroup::EMaxGCObjects];
};


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjGroupAOEParalysis::VOnGroupResourceRelease( void )
{
	// n.b. this must happen first, as will fail if objects destroyed before 
	CGCObjectGroup::VOnGroupResourceRelease(); 
	DestroyPlatforms();
}




//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CGCObjGroupAOEParalysis::DestroyPlatforms( void )
{
	// this iterates the array of registered CGCObjects 
	// calling the supplied functor then deleting them
	DestroyObjectsReverseOrder( [&]( CGCObject* pObject )
	{
		GCASSERT( GetGCTypeIDOf(CGCObjGroupAOEParalysis) == pObject->GetGCTypeID(), "wrong type!" );
		CGCObjSprite* pProjectileAsSprite = static_cast< CGCObjSprite* >( pObject );
		pProjectileAsSprite->DestroySprite();
	});
}

