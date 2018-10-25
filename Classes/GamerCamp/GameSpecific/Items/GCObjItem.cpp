#ifndef _GCOBJITEMS_H_
#include "GCObjItem.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "../../GCCocosInterface/GB2ShapeCache-x.h"
#include "GamerCamp/GameSpecific/Player/GCObjPlayer.h"

// Using cocos namespace, helps to shorten the use of some built-in functionality
USING_NS_CC;


// Virtual
// Initialisation for the key sprites
void CGCObjItem::VOnResourceAcquire()
{
	char* pszPlist;
	switch (m_iKeyNumber) {

	case 0:
		pszPlist = "TexturePacker/Sprites/PickUp/BlueVirus/BlueVirus.plist";
		break;
	case 1:
		pszPlist = "TexturePacker/Sprites/PickUp/GreenVirus/GreenVirus.plist";
		break;
	case 2:
		pszPlist = "TexturePacker/Sprites/PickUp/RedVirus/RedVirus.plist";
		break;
	default:
		pszPlist = "TexturePacker/Sprites/Coin/Coin.plist"; // If key shows as a coin, something has gone wrong with this switch
		m_iKeyNumber = 1;
	}
	//char* pszPlist = "TexturePacker/Sprites/Coin/Coin.plist";
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile( pszPlist );
	AcquireResources( pszPlist, "BlueKey", b2_staticBody, true );
	SetParent( IGCGameLayer::ActiveInstance() );
}


// Pick up item, takes in player and gives them the key number they have collected
void CGCObjItem::PickUpItem( CGCObjPlayer* player )
{
	player->GotNewKey( m_iKeyNumber );
}