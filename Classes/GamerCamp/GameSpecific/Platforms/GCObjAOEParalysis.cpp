////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "GCObjAOEParalysis.h"


#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "../../GCCocosInterface/GB2ShapeCache-x.h"		


USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjAOEParalysis::VOnResourceAcquire(void)
{
	const char* pszPlist = "TexturePacker/Sprites/Paralyze/Zapper.plist";
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(pszPlist);
	AcquireResources(pszPlist, "Zapper1", b2_staticBody, true);
	SetParent(IGCGameLayer::ActiveInstance());

	// animate!

	const char* pszAnim_Zap = "Zap";
	ValueMap dicPList = GCCocosHelpers::CreateDictionaryFromPlist(pszPlist);
	m_pZapAnimation = GCCocosHelpers::CreateAnimationActionLoop(GCCocosHelpers::CreateAnimation(dicPList, pszAnim_Zap));
	m_pZapAnimation->retain();





}



void CGCObjAOEParalysis::Pulse() {
	if (!IsActionStarted(m_pZapAnimation)) {
		RunAction(m_pZapAnimation);
	}
	

	

}

void CGCObjAOEParalysis::StopPulse() {
	

	StopAction(m_pZapAnimation);

	


}




