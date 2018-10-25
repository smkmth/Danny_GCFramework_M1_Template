#include <memory.h>
#include "AppDelegate.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GCObjPlayer.h"


// Using cocos namespace, helps to shorten the use of some built-in functionality
USING_NS_CC;


//////////////////////////////////////////////////////////////////////////
// GetGCTypeIDOf uses the template in GCTypeID to generate a unique ID for 
// this type - need this to construct our base type
CGCObjPlayer::CGCObjPlayer()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CGCObjPlayer ) )
	, m_fMaximumMoveForce_Horizontal( 10.0f )
	, m_fDragCoefficient_Linear( 0.25f )
	, m_fDragCoefficient_Square( 0.2f )
	, m_fNoInput_ExtraDrag_Square( 0.2f )
	, m_fNoInput_VelocityThreshold( 0.25f )
	, m_fImpulseForce( 10.0f )
	, m_iLives( 1 )
	, m_iAmountOfKeys( 0 )
	, m_bAlive( true )
	, m_bWin( false )
	, m_bParalyzed( false )
	, m_bSlowed (false)
{
}


// Virtual 
// Initialisation for the player's sprite
void CGCObjPlayer::VOnResourceAcquire()
{
	const char* pszPlist_mario = "TexturePacker/Sprites/NanoBot2/NanoBot.plist";
	AcquireResources( pszPlist_mario, "NanoBot", b2_dynamicBody, true );
	SetParent( IGCGameLayer::ActiveInstance() );
	/*// Animate
	const char* pszAnim_marioJog	= "Frames/Idle";
	ValueMap dicPList = GCCocosHelpers::CreateDictionaryFromPlist( pszPlist_mario );
	RunAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( dicPList, pszAnim_marioJog ) ) );*/	

	// Physics variable initialisation
	m_fHozMovement = 150.0f;
	m_fVertMovement = 100.0f;		
	m_fSlowedHozMovement = 10.0f;
	m_fSlowedVertMovement = 30.0f;
	m_fMaximumMoveForce_Horizontal = m_fHozMovement;	// newton
	m_fMaximumMoveForce_Vertical = m_fVertMovement;		// newton
	m_fDragCoefficient_Linear = 0.25f;			// unitless
	m_fDragCoefficient_Square = 0.2f;			// unitless
	m_fNoInput_ExtraDrag_Square = 0.2f;			// unitless
	m_fNoInput_VelocityThreshold = 0.25f;		// m/s
	m_iDNACollected = 0;
	m_bIsMoving = false;



}


// Virtual
// Reset functionality, called on game start and when game is reset
void CGCObjPlayer::VOnReset( void )
{
	m_bSlowed = false;
	m_iDNACollected = 0;

	// Inherited reset function
	CGCObjSpritePhysics::VOnReset();
	// Reset velocity and flip state
	SetFlippedX( false );
	SetFlippedY( false );
	// Set keys collected to 0
	m_iAmountOfKeys = 0;
	// Resetting player movement and position
	if (GetPhysicsBody())
	{
		GetPhysicsBody()->SetLinearVelocity( b2Vec2( 0.0f, 0.0f ) );
		GetPhysicsBody()->SetTransform( IGCGameLayer::B2dPixelsToWorld( GetSpritePosition() ), 0.0f );
		GetPhysicsBody()->SetFixedRotation( true );
		// Extra gravity scale set to make the player fall
		GetPhysicsBody()->SetGravityScale( 60.0f );
	}
}


// Virtual
// Master update for the player class
void CGCObjPlayer::VOnUpdate( f32 fTimeStep )
{
	// Update movement
	UpdateMovement( fTimeStep );



	// Set alive to false if lives drops to 0
	if (m_iLives <= 0)
	{
		m_bAlive = false;
	}
}


// Virtual
// Removes player sprite from the scene when it needs to be destroyed
void CGCObjPlayer::VOnResourceRelease( void )
{
	DestroySprite();
}


// Updates all movement of the player's sprite
void CGCObjPlayer::UpdateMovement( f32 fTimeStep )
{
	// Accumulate total force over the frame and apply it at the end
	b2Vec2 v2TotalForce( 0.0f, 0.0f );
	// Calculate the control force direction
	b2Vec2 v2ControlForceDirection( 0.0f, 0.0f );
	// This float is used to add / remove the effect of various terms 
	// in equations based on whether input has been applied this frame
	f32 fIsInputInactive = 1.0f;


	// Get keyboard manager input to check if a button is being pressed by the player
	const CGCKeyboardManager* pKeyManager = AppDelegate::GetKeyboardManager();
	// If not paralyzed, convert pressed key into control force direction as a b2Vec2
	if (m_bParalyzed == false)
	{
		if (pKeyManager->ActionHasJustBeenPressed( CGCGameLayerPlatformer::EPA_Space ))
		{

			m_bIsMoving = true;
			m_fImpulseForce = 20.0f;
			
		}   
		else
		{
			m_bIsMoving = false;

			m_fImpulseForce = 1.0f;
		}
		if (pKeyManager->ActionIsPressed( CGCGameLayerPlatformer::EPA_Space ))
		{
			m_bIsMoving = true;
			v2ControlForceDirection.y = 1.0f;
			fIsInputInactive = 0.0f;
		}
		if (pKeyManager->ActionIsPressed( CGCGameLayerPlatformer::EPA_Down ))
		{
			v2ControlForceDirection.y = -1.0f;
			fIsInputInactive = 0.0f;
		

		}

		if (pKeyManager->ActionIsPressed( CGCGameLayerPlatformer::EPA_Left ))
		{
			m_bIsMoving = true;

			v2ControlForceDirection.x = -1.0f;
			fIsInputInactive = 0.0f;
		}
		else
		{

			m_bIsMoving = false;

		}
		if (pKeyManager->ActionIsPressed( CGCGameLayerPlatformer::EPA_Right ))
		{
			m_bIsMoving = true;

			v2ControlForceDirection.x = 1.0f;
			fIsInputInactive = 0.0f;
		}
		else
		{
			m_bIsMoving = false;

		}

	}

	// Normalise the control vector and multiply by movement force
	v2ControlForceDirection.Normalize();
	v2ControlForceDirection.x *= m_fMaximumMoveForce_Horizontal;
	v2ControlForceDirection.y *= (m_fMaximumMoveForce_Vertical)* m_fImpulseForce;
	// Accumulate the force
	v2TotalForce += v2ControlForceDirection;

	// Calculate drag force
	b2Vec2 v2Velocity_Unit = GetPhysicsBody()->GetLinearVelocity();
	f32 fVelocity = v2Velocity_Unit.Normalize();
	f32 fDragForce = ((m_fDragCoefficient_Linear * fVelocity)
		+ (m_fDragCoefficient_Square * (fVelocity * fVelocity))
		+ (m_fNoInput_ExtraDrag_Square * (fVelocity * fVelocity) * fIsInputInactive));
	// Drag is applied in the opposite direction to the current velocity of the object
	v2TotalForce += (-fDragForce * v2Velocity_Unit);


	// Overall force added to the player's sprite at the end of all calculations
	GetPhysicsBody()->ApplyForceToCenter( v2TotalForce, true );


	// Set sprite left/right flip based on horizontal velocity
	if (GetPhysicsBody()->GetLinearVelocity().x >= 0.0f)
	{
		SetFlippedX( true );
	}
	else if (GetPhysicsBody()->GetLinearVelocity().x < 0.0f)
	{
		SetFlippedX( false );
	}
}


// Reset the position of the player's sprite
void CGCObjPlayer::ResetPosition()
{
	SetSpritePosition( b2Vec2( GetResetPosition() ) );
	GetPhysicsBody()->SetTransform( IGCGameLayer::B2dPixelsToWorld( GetSpritePosition() ), 0.0f );
	GetPhysicsBody()->SetLinearVelocity( b2Vec2( 0.0f, 0.0f ) );
	
}


// The player has just picked up a key, set the index of the player key array to true
void CGCObjPlayer::GotNewKey( int KeyNumber )
{
	m_iAmountOfKeys++;
	m_abKeys[KeyNumber] = true;
}


//check	the index of the player key array, return if it is true.
bool CGCObjPlayer::CheckKey( int keyToLookFor )
{
	if (m_abKeys[keyToLookFor] == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}


// Push the player by a b2Vec2 amount, used for blood flow
void CGCObjPlayer::PushByAmount( b2Vec2( _amount ) )
{
	GetPhysicsBody()->ApplyForceToCenter( _amount, true );
}

void CGCObjPlayer::SlowMovement() {

	m_bSlowed = true;
	m_fMaximumMoveForce_Horizontal	 = m_fSlowedHozMovement;
	m_fMaximumMoveForce_Vertical	 = m_fSlowedVertMovement;



}

void CGCObjPlayer::RestoreMovement() {

	m_bSlowed = false;
	m_fMaximumMoveForce_Horizontal = m_fHozMovement; 
	m_fMaximumMoveForce_Vertical = m_fVertMovement;
	



}


//Getters and Setters
	//Getters
b2Vec2 CGCObjPlayer::GetCurrentPos()
{
	return GetPhysicsBody()->GetPosition();
}
int CGCObjPlayer::GetAmountOfKeys() 
{
	return m_iAmountOfKeys;
}
int CGCObjPlayer::GetLives()
{
	return m_iLives;
}
int CGCObjPlayer::GetAlive()
{
	return m_bAlive;
}
bool CGCObjPlayer::GetWin()
{
	return m_bWin;
}
bool CGCObjPlayer::GetParalyzed()
{
	return m_bParalyzed;
}
bool CGCObjPlayer::GetSlowed() 
{
	return m_bSlowed;
}
	//Setters
void CGCObjPlayer::SetLives( int _input )
{
	m_iLives = _input;
}
void CGCObjPlayer::SetAlive( bool _input )
{
	m_bAlive = _input;
}
void CGCObjPlayer::SetWin( bool _value ) 
{
	m_bWin = _value;
}
void CGCObjPlayer::SetParalyzed( bool _input )
{
	m_bParalyzed = _input;
}
void CGCObjPlayer::SetSlowed( bool _input ) {

	m_bSlowed = _input;
}

void CGCObjPlayer::SetDNA() {

	m_iDNACollected++;
}
int CGCObjPlayer::GetDNA() {

	return m_iDNACollected;
}

bool CGCObjPlayer::GetMoving() 
{
	return m_bIsMoving;
}

