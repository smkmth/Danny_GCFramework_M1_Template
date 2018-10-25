#ifndef _GCOBJHOMINGENEMY_H_
#include "GamerCamp/GameSpecific/Enemies/GCObjEnemy.h"	

#define _GCOBJHOMINGENEMY_H_

#ifndef _GCOBJSPRITEPHYSICS_H_
#include "../../GCCocosInterface/GCObjSpritePhysics.h"

#endif


class CGCObjHomingEnemy
	: public CGCObjSpritePhysics
{
private:

	// b2Vec2 for player's previous position, used to follow player
	b2Vec2	m_v2LastPlayerPos;


public:

	// Constructor
	CGCObjHomingEnemy();

	//////////////////////////////////////////////////////////////////////////
	// We need a virtual destructor since delete will be called on pointers of 
	// this class to delete derived types.
	virtual ~CGCObjHomingEnemy()
	{}

	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the game object interface

	virtual void VOnResourceAcquire( void ) override;

	virtual void VOnResurrected( void ) override;

	virtual void VOnUpdate( f32 fTimeStep ) override;

	// Overridden virtuals from the game object interface
	//////////////////////////////////////////////////////////////////////////


	// Getters and Setters
		//Setters
	void SetPlayerDirection( b2Vec2 _target );
};
#endif // #ifndef _GCOBJHOMINGENEMY_H_