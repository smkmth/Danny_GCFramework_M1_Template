/////////////////////////////////////////////////////////////////// /////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GAMELAYERSPACEINVADERS_H_
#define _GAMELAYERSPACEINVADERS_H_

#include "../GCCocosInterface/IGCGameLayer.h"

//////////////////////////////////////////////////////////////////////////
// fwd decl
class CGCObjSprite;
class CGCObjPlayer;
class CGCObjPlatform;
class CGCObjGroupPlatform;
class CGCObjItem;
class CGCObjGroupItem;
class CGCObjEnemy;
class CGCObjGroupEnemy;
class CGCObjGroupProjectilePlayer;
class CGCObjGroupDoor;
class CLevel2Scene;
class CGCObjGroupAOEParalysis;
class CGCObjGroupAOEBloodFlow;
class CGCObjHomingEnemy;
class CGCObjAOEParalysis;
class CGCObjAOESlow;
class CGCObjGroupAOESlow;
class CGCObjAOEBloodFlow;
class CGCObjDNA;
class CGCObjGroupDNA;


//////////////////////////////////////////////////////////////////////////
// sample class that creates a 'game' by deriving from IGCGameLayer
//////////////////////////////////////////////////////////////////////////
class CGCGameLayerPlatformer 
: public IGCGameLayer
, public b2ContactListener 
{
private:
	// object groups
	CGCObjGroupPlatform*			m_pcGCGroupPlatform;
	CGCObjGroupItem*				m_pcGCGroupItem;
	CGCObjGroupEnemy*				m_pcGCGroupEnemy;
	CGCObjGroupDoor*				m_pcGCGroupDoor;
	CGCObjGroupAOEParalysis*		m_pcGCObjGroupParalysis;
	CGCObjGroupAOEBloodFlow*		m_pcGCObjGroupBloodFlow;
	CGCObjHomingEnemy*				m_pcGCObjHomingEnemey;
	CGCObjGroupAOESlow*				m_pcGCObjGroupAOESlow;

	CGCObjGroupDNA*					m_pcGCObjGroupDNA;
	CGCObjDNA*						m_pcGCObjDNA;

	CGCObjAOEBloodFlow*				m_pcGCBloodFlow;

	// backgrounds
	CGCObjSprite*					m_pcGCSprBackGround;
	CGCObjSprite*					m_pcGCSprTimeUp;

	// player
	CGCObjPlayer*					m_pcGCOPlayer;
	
	//platforms
	CGCObjPlatform*					m_pcGCObjPlatform;
	CGCObjItem*						m_pcGCObjItem;
	CGCObjAOEParalysis*				m_pcGCParalysis;
	CGCObjAOESlow*					m_pcGCObjSlow;

	
	// particles
	cocos2d::ParticleSystem*		m_pPShipTrail;

	//player impulse bar
	cocos2d::DrawNode*				m_pdImpulseBar;
		//impulse bar rectangle
	cocos2d::Vec2					m_v2ImpulseBarRectangle[4];
		//impulse bar width
	int								m_iImpulseBarWidth;
		//impulse bar colour
	cocos2d::Color4F				m_cGreen;

	//timer floats, count down in each timer method
	float							m_fParalyzeTime;
	float							m_fPulseTime;
	float							m_fLevelTime;
	float							m_fSlowPlayerTime;

	//timer bools, fire when a timer is complete and set
	bool							m_bParalyzeTimerComplete;
	bool							m_bParalyizeTimerSet;
	bool							m_bPulseTimerSet;
	bool							m_bPulseTimerComplete;
	bool							m_bLevelTimerSet;
	bool							m_bLevelTimerComplete; 	
	bool							m_bSlowTimerSet;
	bool							m_bSlowTimerComplete;
	bool							m_bLevelTimerFinished;
	
	//pulse function called when a pulse is fired
	bool							m_bParalyizePulseOn;	
	
	enum							ETimers
	{
		eParalyzeTimer,
		ePulseTimer,
		eLevelTimer,
		eSlowPlayertimer

	};
									


public:
	CGCGameLayerPlatformer	( void );
	~CGCGameLayerPlatformer	( void );

	//////////////////////////////////////////////////////////////////////////
	// player actions 
	enum EPlayerActions
	{
		EPA_Space = 0,
		EPA_Down,
		EPA_Left,
		EPA_Right,
		EPA_Fire
	};
	// player actions 
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
    // 'selector' callbacks for menu buttons
    void Callback_OnQuitButton	( Ref* pSender );
    void Callback_OnResetButton	( Ref* pSender );

	// called from VOnUpdate
	void HandleCollisions( void );
	void PulseTimer(float);
	void ParalyzeTimer(float);
	void LevelTimer(float);
	void SlowPlayerTimer( float dt );
	void CreateTimer( ETimers ETimerID, float iTimeTaken );
	void ResolveTimer( ETimers ETimerID );



	//////////////////////////////////////////////////////////////////////////
	// CCNode interface...
	virtual void onEnter();
	// CCNode interface...
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// IGCGameLayer interface

		virtual	void VOnCreate	( void );
		CGCObjSprite* CreateFullscreenSprite( cocos2d::Size & visibleSize, const char* pszPathToImage, int iZOrder );
	//	void CreateBackground( cocos2d::Size &visibleSize );
		void AddMenu( cocos2d::Size &visibleSize, cocos2d::Point &origin );
		virtual void VOnUpdate	( f32 fTimeStep );
		virtual	void VOnDestroy ( void );
		void DestroyObjectGroups();
		void VOnReset();
		cocos2d::ParticleSystemQuad* CreateParticleSystem( const char* pszPlist, int iZPos  );
		void CreateEnemies();
		void CreateGameBounds( const b2Vec2 &v2ScreenCentre_Pixels, cocos2d::Size &visibleSize, const f32 &PTM_RATIO );
		void CreateObjectGroups();
		void CreateLevelFromArray( cocos2d::Point &origin);
		

	// IGCGameLayer interface
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// b2ContactListener interface - see b2ContactListener for details of 
	// when these get called and what they are

		virtual void BeginContact	( b2Contact* pB2Contact );
		virtual void EndContact		( b2Contact* pB2Contact );
		virtual void PreSolve		( b2Contact* pB2Contact, const b2Manifold* pOldManifold );
		virtual void PostSolve		( b2Contact* pB2Contact, const b2ContactImpulse* pImpulse );

	// b2ContactListener interface - see b2ContactListener for details of 
	// when these get called and what they are
	//////////////////////////////////////////////////////////////////////////
};

#endif // __CGCGameLayerPlatformer_SCENE_H__
