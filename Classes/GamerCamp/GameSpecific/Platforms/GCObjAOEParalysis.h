////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJAOEPARALYSIS_H_
#define _GCOBJAOEPARALYSIS_H_

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "../../GCCocosInterface/GCObjSpritePhysics.h"


USING_NS_CC;


class CGCObjAOEParalysis
	: public CGCObjSpritePhysics
{
private:

	ActionInterval* m_pZapAnimation;
public:
	CGCObjAOEParalysis()
		: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjAOEParalysis))
	{}

	virtual void VOnResourceAcquire() override;



	void Pulse(); 
	void StopPulse();

};


#endif
