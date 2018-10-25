////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJWINCUBE_H_
#define _GCOBJWINCUBE_H_

#include "../../GCCocosInterface/GCObjSpritePhysics.h"


class CGCObjWinCube
	: public CGCObjSpritePhysics
{
public:
	CGCObjWinCube()
		: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjWinCube))
	{}

	virtual void VOnResourceAcquire() override;


};


#endif
