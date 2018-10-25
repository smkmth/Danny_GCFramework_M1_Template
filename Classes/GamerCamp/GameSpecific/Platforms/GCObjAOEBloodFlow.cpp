#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "../../GCCocosInterface/GB2ShapeCache-x.h"		
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GCObjAOEBloodFlow.h"

// Using cocos namespace, helps to shorten the use of some built-in functionality
USING_NS_CC;


// Virtual 
// Initialisation for the enemies sprite
void CGCObjAOEBloodFlow::VOnResourceAcquire(void)
{
	const char* pszPlist = "TexturePacker/Sprites/BloodFlow/BloodFlow.plist";
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(pszPlist);
	AcquireResources(pszPlist, "BloodFlow", b2_staticBody, true);
	SetParent(IGCGameLayer::ActiveInstance());
}

b2Vec2 CGCObjAOEBloodFlow::GetPushVector()
{
	return v2BloodFlowDirection;
}

void CGCObjAOEBloodFlow::SetPushVector( b2Vec2 _BloodFlowDirection )
{
	v2BloodFlowDirection = _BloodFlowDirection;
}
