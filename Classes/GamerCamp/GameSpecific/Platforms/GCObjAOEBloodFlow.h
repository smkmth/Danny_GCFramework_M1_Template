#ifndef _GCOBJAOEBLOODFLOW_H_
#define _GCOBJAOEBLOODFLOW_H_

#include "../../GCCocosInterface/GCObjSpritePhysics.h"


class CGCObjAOEBloodFlow
	: public CGCObjSpritePhysics
{
private:
	b2Vec2 v2BloodFlowDirection;


public:

	// Constructor
	CGCObjAOEBloodFlow()
		: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjAOEBloodFlow))
	{}

	CGCObjAOEBloodFlow( b2Vec2 _setBloodFlowDirection)
		: CGCObjSpritePhysics( GetGCTypeIDOf( CGCObjAOEBloodFlow ) )
	{
		v2BloodFlowDirection = _setBloodFlowDirection;

	
	}


	//////////////////////////////////////////////////////////////////////////
	// Overridden virtuals from the game object interface

	virtual void VOnResourceAcquire() override;
	b2Vec2 GetPushVector();
	void SetPushVector(b2Vec2 _BloodFlowDirection);

	// Overridden virtuals from the game object interface
	//////////////////////////////////////////////////////////////////////////
};
#endif // #ifndef _GCOBJAOEBLOODFLOW_H_