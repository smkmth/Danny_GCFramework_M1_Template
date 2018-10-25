////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CGCObjAOESlow.h"


#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "../../GCCocosInterface/GB2ShapeCache-x.h"	



USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjAOESlow::VOnResourceAcquire()
{
	const char* pszPlist = "TexturePacker/Sprites/SlowCube/SlowCube.plist";
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(pszPlist);
	AcquireResources(pszPlist, "SlowCube", b2_staticBody, true);
	SetParent(IGCGameLayer::ActiveInstance());

}






