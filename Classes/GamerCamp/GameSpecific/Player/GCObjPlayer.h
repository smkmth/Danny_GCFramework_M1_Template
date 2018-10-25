#ifndef _GCOBJPLAYER_H_
#define _GCOBJPLAYER_H_

#ifndef _GCOBJSPRITEPHYSICS_H_
	#include "../../GCCocosInterface/GCObjSpritePhysics.h"
#endif
#include <vector>;
// Macros to define the different colours of key
#define GREEN_KEY (0)
#define RED_KEY (1)
#define BLUE_KEY (2) 


class CGCObjPlayer 
: public CGCObjSpritePhysics
{
private:

	// Array of bools - Total number of keys the player can collect
	bool m_abKeys[3] {};
	// Int amount of keys
	int m_iAmountOfKeys;
	// Int number of lives
	int m_iLives;
	// Bool for alive/dead
	bool m_bAlive;	
	// Bool for win/lose
	bool m_bWin;
	// Bool for paralyzed
	bool m_bParalyzed;
	//bool for slowed
	bool m_bSlowed;
	//bool for moving
	bool m_bIsMoving;


	// Member variables for physics handling
	// Uses sprite to store position and has no rotation
	f32		m_fMaximumMoveForce_Horizontal;
	f32		m_fMaximumMoveForce_Vertical;
	f32		m_fDragCoefficient_Linear;
	f32		m_fDragCoefficient_Square;
	f32		m_fNoInput_ExtraDrag_Square;
	f32		m_fNoInput_VelocityThreshold;

	// Force added when space/tap is first pressed, gives a short burst of speed
	float	m_fImpulseForce;
	float	m_fSlowedHozMovement;
	float	m_fHozMovement;
	float	m_fSlowedVertMovement;
	float	m_fVertMovement;

	int m_iDNACollected;
	

public:

	// Constructor
	CGCObjPlayer();

	//////////////////////////////////////////////////////////////////////////
	// We need a virtual destructor since delete will be called on pointers of 
	// this class to delete derived types.
	virtual ~CGCObjPlayer()
	{}

	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the game object interface

		// This will be called exactly once for each CGCObject derived class instance 
		// registered with CGCObjectManager as soon as the TGB level file has stopped 
		// loading- it is an "explicit constructor".
		virtual void VOnResourceAcquire( void );

		// OnReset is called for each CGCObject derived class when CGCObjectManager is 
		// reset
		virtual void VOnReset( void ); 

		// OnUpdate is called for each CGCObject derived class when CGCObjectManager is 
		// updated in t2dSCeneGraph onProcessTick()
		virtual void VOnUpdate( f32 fTimeStep ); 

        // Called immediately before the managing object group releases its own assets
        virtual void VOnResourceRelease( void );

	// Overridden virtuals from the game object interface
	//////////////////////////////////////////////////////////////////////////

	// Updates all movement of the player's sprite
	void UpdateMovement( f32 fTimeStep );

	// Reset the position of the player's sprite
	void ResetPosition();

	//
	void GotNewKey(int);

	//
	bool CheckKey(int);

	// Push the player by a b2Vec2 amount, used for blood flow
	void PushByAmount(b2Vec2(_amount));

	void SlowMovement();


	void RestoreMovement();
	//Getters and Setters
		//Getters
	b2Vec2 GetCurrentPos();
	int GetAmountOfKeys();
	int GetLives();
	int GetAlive();
	bool GetWin();
	bool GetParalyzed();
	bool GetSlowed();
		//Setters
	void SetLives(int _input);
	void SetAlive(bool _input);
	void SetWin( bool _input );
	void SetParalyzed(bool _input);
	void SetSlowed( bool _input );
	void SetDNA();
	int GetDNA();
	bool GetMoving();
};
#endif // #ifndef _GCOBJPLAYER_H_