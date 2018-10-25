#ifndef _GCOBJDNA_H_
#define _GCOBJDNA_H_

#include "../../GCCocosInterface/GCObjSpritePhysics.h"

class CGCObjPlayer;

class CGCObjDNA
: public CGCObjSpritePhysics
{
private:

	// Int for key number, gives player certain colour of key based on its number
	int m_iDNAColour;

public:

	// Default Constructor
	CGCObjDNA()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CGCObjDNA ) )
	{}

	// Constructor that takes in key number
	CGCObjDNA( int dnacolour)
		: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjDNA))
	{
		m_iDNAColour = dnacolour;
	}


	// Initialisation for the key sprites
	virtual void VOnResourceAcquire() override;

	// Pick up item, takes in player and gives them the key number they have collected
	void PickUpItem(CGCObjPlayer*);
};

#endif // #ifndef _GCOBJEnemy_H_