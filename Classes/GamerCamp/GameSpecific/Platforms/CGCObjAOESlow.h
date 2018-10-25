////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJAOESLOW_H_
#define _GCOBJAOESLOW_H_

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "../../GCCocosInterface/GCObjSpritePhysics.h"


USING_NS_CC;


class CGCObjAOESlow
	: public CGCObjSpritePhysics
{

public:
	CGCObjAOESlow()
		: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjAOESlow))
	{}

	virtual void VOnResourceAcquire() override;




};


#endif
