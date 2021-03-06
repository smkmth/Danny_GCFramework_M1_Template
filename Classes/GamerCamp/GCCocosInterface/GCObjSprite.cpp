////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "GCObjSprite.h"

#ifndef _GCCOCOSHELPERS_H_
	#include "../GCCocosInterface/GCCocosHelpers.h"
#endif

#ifndef _IGCGAMELAYER_H_
	#include "IGCGameLayer.h"
#endif

#ifndef __COCOS2D_H__
	#include "cocos2d.h"
#endif


//////////////////////////////////////////////////////////////////////////
// save ourselves some typing later
using namespace cocos2d;



//////////////////////////////////////////////////////////////////////////
// protected version to pass through id to GCObject
//////////////////////////////////////////////////////////////////////////
CGCObjSprite::CGCObjSprite( GCTypeID idDerivedType )
: CGCObject		( idDerivedType )
, m_pcSprite	( NULL )
{
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CGCObjSprite::CGCObjSprite( void )
: CGCObject				( GetGCTypeIDOf( CGCObjSprite ) )
, m_pcSprite			( NULL )
, m_v2InitialPosition	( b2Vec2( 0.0f, 0.0f ) )
{
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
CGCObjSprite::~CGCObjSprite( void )
{
	CCAssert( NULL == m_pcSprite,	"CGCObjSprite::~CGCObjSprite -it appears that DestroySprite() "
									"was not called on this instance of CGCObjSprite" );
}


//////////////////////////////////////////////////////////////////////////
//
// N.B. GCCocosHelpers::CreateSpriteFromPlist() returns an autoreleased 
// CCsprite, so we must retain() it to prevent it from being cleaned up
//////////////////////////////////////////////////////////////////////////
void CGCObjSprite::CreateSprite( const char* pszPlist )
{
	m_pcSprite = GCCocosHelpers::CreateSpriteFromPlist( pszPlist );
	CCAssert( m_pcSprite, "m_pcSprite was not created - is pszPlist correct path" );
	m_pcSprite->retain();
}




//////////////////////////////////////////////////////////////////////////
// This is faster because the user must load a __Dictionary from a plist
// before calling it - will only be faster if creating many sprites.
// 
// See Sprite_LoadTextureAndFramesToCachesAndGetDictionary()
// 
// N.B. GCCocosHelpers::CreateSpriteFromPlist() returns an autoreleased 
// CCsprite, so we must retain() it to prevent it from being cleaned up
//////////////////////////////////////////////////////////////////////////
void CGCObjSprite::CreateSpriteFast( ValueMap dicSpriteInfo )
{
	m_pcSprite = GCCocosHelpers::Sprite_CreateSpriteFrom1stFrame( dicSpriteInfo );
	CCAssert( m_pcSprite, "m_pcSprite was not created - is pszPlist correct path" );
	m_pcSprite->retain();
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CGCObjSprite::DestroySprite( void )
{
	// this macro calls release on m_pcSprite then sets it to NULL
	CC_SAFE_RELEASE_NULL( m_pcSprite );
}



////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
void CGCObjSprite::SetParent( cocos2d::Node* pNewParent )
{
	CCAssert( m_pcSprite, "m_pcSprite is NULL. Have you called CreateSprite?" );
	CCAssert( pNewParent, "pNewParent is NULL" );
	// n.b. this does nothing if the CCNode derived type has no parent...
	if( m_pcSprite->getParent() )
	{
		m_pcSprite->removeFromParentAndCleanup( false );
	}
	pNewParent->addChild( m_pcSprite );
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CGCObjSprite::SetParent( cocos2d::Node* pNewParent, int zOrder )
{
	CCAssert( m_pcSprite, "m_pcSprite is NULL. Have you called CreateSprite?" );
	CCAssert( pNewParent, "pNewParent is NULL" );
	// n.b. this does nothing if the CCNode derived type has no parent...
	if (m_pcSprite->getParent())
	{
		m_pcSprite->removeFromParentAndCleanup( false );
	}
	pNewParent->addChild( m_pcSprite, zOrder );
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CGCObjSprite::EActionState CGCObjSprite::RunAction( Action* pAction )
{
	CCAssert( m_pcSprite, "m_pcSprite is NULL. Have you called CreateSprite?" );
	CCAssert( pAction, "pAction is NULL" );
	m_pcSprite->runAction( pAction );
	return EAS_Running;
}

bool CGCObjSprite::IsActionStarted(Action* pAction)
{
	UpdateActionStatus(pAction);
	unsigned int i = pAction->getFlags();

	if (i == 1) {
		return true;
	}
	else {
		return false;
	}
}
void CGCObjSprite::UpdateActionStatus(cocos2d::Action* pAction)
{
	CCAssert(m_pcSprite, "m_pcSprite is NULL. Have you called CreateSprite?");
	CCAssert(pAction, "pAction is NULL");
	bool b = pAction->isDone();
	if (b) {
		pAction->setFlags(0);
	}
}
CGCObjSprite::EActionState CGCObjSprite::StopAction(Action* pAction)
{
	CCAssert(m_pcSprite, "m_pcSprite is NULL. Have you called CreateSprite?");
	CCAssert(pAction, "pAction is NULL");
	m_pcSprite->stopAction(pAction);
	pAction->setFlags(0);
	return EAS_Finished;
}


//////////////////////////////////////////////////////////////////////////
// default behaviour is to add the managed sprite to the game layer 
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjSprite::VOnResourceAcquire( void )
{}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjSprite::VOnReset( void )
{
	SetSpriteRotation( 0.0f );
	SetSpritePosition( m_v2InitialPosition );
	SetVisible( false ); // N.B. set to true in VOnResurrect()

	// if the physics world is being rendered in debug mode we need to render sprites semi-trans to make the rendering visible
	if( IGCGameLayer::ActiveInstance()->B2dIsDebugDrawing() )
	{
		m_pcSprite->setOpacity( 128 ); 
	}
}


//////////////////////////////////////////////////////////////////////////
// don't need to do anything - shutting down the parent will reduce the 
// refcount on m_pcSprite 
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjSprite::VOnResourceRelease( void )
{
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjSprite::VOnKilled( void )
{
	SetVisible( false );
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjSprite::VOnResurrected( void )
{
	SetVisible( true );
}
