#ifndef _GCOBJDNA_H_
#include "GCObjDNA.h"
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
void CGCObjDNA::VOnResourceAcquire()
{
	char* pszPlist;
	switch (m_iDNAColour) {

	case 0:

		pszPlist = "TexturePacker/Sprites/DNAKey/Blue/BlueKey.plist";
		break;

	case 1:
		pszPlist = "TexturePacker/Sprites/DNAKey/Green/GreenKey.plist";
		break;
		
	case 2:
		pszPlist = "TexturePacker/Sprites/DNAKey/Red/RedKey.plist";
		break;
		
		
	default:
		pszPlist = "TexturePacker/Sprites/Coin/Coin.plist"; // If key shows as a coin, something has gone wrong with this switch
		m_iDNAColour = 1;
		break;
	}
	//char* pszPlist = "TexturePacker/Sprites/Coin/Coin.plist";
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile( pszPlist );
	AcquireResources( pszPlist, "BlueKey", b2_dynamicBody, true );
	SetParent( IGCGameLayer::ActiveInstance() );
}


// Pick up item, takes in player and gives them the key number they have collected
void CGCObjDNA::PickUpItem( CGCObjPlayer* player )
{
	player->SetDNA();
}