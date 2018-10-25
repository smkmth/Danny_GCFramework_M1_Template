#ifndef _GCOBJDOOR_H_
#include "GCObjDoor.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "../../GCCocosInterface/GB2ShapeCache-x.h"		
#include "GCObjDoor.h"


USING_NS_CC;


//virtual 
void CGCObjDoor::VOnResourceAcquire()
{

	//GREEN_KEY (0)
//RED_KEY (1)
//BLUE_KEY (2) 


	char* pszPlist;
	switch (m_iDoorNumber) {
	case 0:
		pszPlist = "TexturePacker/Sprites/DNADoor/Blue/DNADoorBlue.plist";
		break;
		
	case 1:
		pszPlist = "TexturePacker/Sprites/DNADoor/Green/DNADoorGreen.plist";
		break;
	
	case 2:
		pszPlist = "TexturePacker/Sprites/DNADoor/Red/DNADoorRed.plist";
		break;

	default:
		pszPlist = "TexturePacker/Sprites/Platform/Platform.plist";
		m_iDoorNumber = 1;
		break;
	}
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(pszPlist);
	AcquireResources(pszPlist, "Door_Blue", b2_staticBody, true);
	SetParent(IGCGameLayer::ActiveInstance());
///*
	/*
	switch (m_iDoorNumber) {
	case 0:
		const char* pszPlist = "TexturePacker/Sprites/Door/Green/Green.plist";
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(pszPlist);
		AcquireResources(pszPlist, "platform", b2_staticBody, true);
		SetParent(IGCGameLayer::ActiveInstance());
		break;

	case 1:

		const char* pszPlist = "TexturePacker/Sprites/Door/Red/Red.plist";
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(pszPlist);
		AcquireResources(pszPlist, "platform", b2_staticBody, true);
		SetParent(IGCGameLayer::ActiveInstance());
		break;

	case 2:
		const char* pszPlist = "TexturePacker/Sprites/Door/Blue/Blue.plist";
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(pszPlist);
		AcquireResources(pszPlist, "platform", b2_staticBody, true);
		SetParent(IGCGameLayer::ActiveInstance());
		break;

	default:
		const char* pszPlist = "TexturePacker/Sprites/Platform/Platform.plist";
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(pszPlist);
		AcquireResources(pszPlist, "platform", b2_staticBody, true);
		SetParent(IGCGameLayer::ActiveInstance());
		m_iDoorNumber = 1;
		break;

	}*/
	

	

	
	

}

void CGCObjDoor::VOnResourceRelease()
{
	DestroySprite();
}




int CGCObjDoor::GetDoorNumber()
{
	return m_iDoorNumber;

	
}

void CGCObjDoor::SetDoorNumber(int doorNumber)
{
	m_iDoorNumber = doorNumber;
}
