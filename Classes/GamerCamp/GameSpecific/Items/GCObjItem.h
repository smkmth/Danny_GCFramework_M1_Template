#ifndef _GCOBJITEMS_H_
#define _GCOBJITEMS_H_

#include "../../GCCocosInterface/GCObjSpritePhysics.h"

class CGCObjPlayer;

class CGCObjItem
: public CGCObjSpritePhysics
{
private:

	// Int for key number, gives player certain colour of key based on its number
	int m_iKeyNumber;

public:

	// Default Constructor
	CGCObjItem()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CGCObjItem ) )
	{}

	// Constructor that takes in key number
	CGCObjItem( int keyNumber)
		: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjItem))
	{
		m_iKeyNumber = keyNumber;
	}


	// Initialisation for the key sprites
	virtual void VOnResourceAcquire() override;

	// Pick up item, takes in player and gives them the key number they have collected
	void PickUpItem(CGCObjPlayer*);
};

#endif // #ifndef _GCOBJEnemy_H_