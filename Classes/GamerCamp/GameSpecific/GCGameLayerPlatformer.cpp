#include "GCGameLayerPlatformer.h"
#include <algorithm>
#include <stdlib.h> 
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "MenuScene.h"
#include "GamerCamp/GameSpecific/Levels/GCObjGameOverScene.h"
#include "GamerCamp/GCCocosInterface/GCObjSprite.h"
#include "GamerCamp/GameSpecific/Player/GCObjPlayer.h"
#include "GamerCamp/GameSpecific/Platforms/GCObjPlatform.h" 
#include "GamerCamp/GameSpecific/Platforms/GCObjGroupPlatform.h"
#include "GamerCamp/GameSpecific/Items/GCObjItem.h" 
#include "GamerCamp/GameSpecific/Items/GCObjGroupItem.h"
#include "GamerCamp/GameSpecific/Enemies/GCObjEnemy.h"
#include "GamerCamp/GameSpecific/Enemies/GCObjGroupEnemy.h"
#include "GamerCamp/GameSpecific/Platforms/GCObjGroupDoor.h"
#include "GamerCamp/GameSpecific/Platforms/GCObjDoor.h"
#include "GamerCamp/GameSpecific/Platforms/GCObjWinCube.h"
#include "GamerCamp/GameSpecific/Platforms/GCObjAOEBloodFlow.h"
#include "GamerCamp/GameSpecific/Platforms/GCObjAOEParalysis.h"
#include "GamerCamp/GameSpecific/Platforms/CGCObjGroupAOEBloodFlow.h"
#include "GamerCamp/GameSpecific/Platforms/CGCObjGroupAOEParalysis.h"
#include "GamerCamp/GameSpecific/Enemies/GCObjHomingEnemy.h"
#include "GamerCamp/GameSpecific/Levels/GCObjLevel2Scene.h"
#include "GamerCamp/GameSpecific/Platforms/CGCObjGroupAOESlow.h"
#include "GamerCamp/GameSpecific/Platforms/CGCObjAOESlow.h"
#include "GamerCamp/GameSpecific/Items/GCObjDNA.h"


#include "AppDelegate.h"


// Using cocos namespace, helps to shorten the use of some built-in functionality
USING_NS_CC;


///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////
CGCGameLayerPlatformer::CGCGameLayerPlatformer()
	: IGCGameLayer(GetGCTypeIDOf(CGCGameLayerPlatformer))
	
		,m_pcGCGroupItem(NULL)
		, m_pcGCGroupEnemy(NULL)
		, m_pcGCSprBackGround(NULL)
		, m_pcGCOPlayer(NULL)
		, m_pcGCGroupDoor(NULL)
		, m_pcGCObjPlatform(NULL)
		, m_pcGCObjItem(NULL)
		, m_pcGCObjGroupParalysis(NULL)
		, m_pcGCParalysis(NULL)
		, m_pcGCObjSlow (NULL)
		, m_pcGCObjGroupAOESlow (NULL)
	    , m_pcGCSprTimeUp (NULL)
		, m_pcGCObjGroupDNA (NULL)
		, m_pcGCObjDNA (NULL)
{

}


//////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////
CGCGameLayerPlatformer::~CGCGameLayerPlatformer()
{

}
//////////////////////////////////////////////////////////////////////////
// on create
//////////////////////////////////////////////////////////////////////////
//virtual
void CGCGameLayerPlatformer::VOnCreate()
{ 

	//////////////////////////////////////////////////////////////////////////
	// cache some useful values 
	Size visibleSize	= Director::getInstance()->getVisibleSize();
	Point origin		= Director::getInstance()->getVisibleOrigin();
	b2Vec2 v2ScreenCentre_Pixels( ( origin.x + ( visibleSize.width * 0.5f ) ), ( origin.y + ( visibleSize.height * 0.5f ) ) );


	// create the default object group
	IGCGameLayer::VOnCreate();



	// create and register the object group for the platform objects
	CreateObjectGroups();

	AddMenu( visibleSize, origin );

	m_pcGCSprBackGround = CreateFullscreenSprite( visibleSize , "TexturePacker/Backgrounds/Brain/BrainBackground.plist", 0 );
	m_pcGCSprTimeUp = CreateFullscreenSprite(visibleSize, "TexturePacker/Backgrounds/Placeholder/TimeUp.plist", 3 );


	///////////////////////////////////////////////////////////////////////////
	// set up physics 
	///////////////////////////////////////////////////////////////////////////

	// set "self" as contact listener
	B2dGetWorld()->SetContactListener( this ); 

	// load the physics shapes from the plist created with PhysicsEditor
	B2dLoadShapesFromPlist( "PhysicsEditor/GameShapes.plist" );	


	// PTM_RATIO
	f32 PTM_RATIO = IGCGAMELAYER_B2D_PIXELS_PER_METER;
	


	CreateGameBounds( v2ScreenCentre_Pixels, visibleSize, PTM_RATIO );

	CreateLevelFromArray( origin );

	CreateEnemies();

	const char* pszPlist_playerparticles = "TexturePacker/Particles/ShipTrailBubbles.plist";

	m_pPShipTrail = CreateParticleSystem( pszPlist_playerparticles, 0 );


}

//////////////////////////////////////////////////////////////////////////
// in order to guarantee the actions this layer expects we need to 
// initialise the keyboard action map every time onEnter is called - this
// fixes the case where an IGCGameLayer with a different mapping was
// pushed over this one on Director's scene stack
//////////////////////////////////////////////////////////////////////////
void CGCGameLayerPlatformer::onEnter()
{

	IGCGameLayer::onEnter();

	//////////////////////////////////////////////////////////////////////////
	// init the actions
	// N.B. these 
	cocos2d::EventKeyboard::KeyCode aeKeyCodesForActions[] =
	{
		EventKeyboard::KeyCode::KEY_SPACE,			// EPA_Space,
		EventKeyboard::KeyCode::KEY_DOWN_ARROW,		// EPA_Down,
		EventKeyboard::KeyCode::KEY_LEFT_ARROW,		// EPA_Left,		
		EventKeyboard::KeyCode::KEY_RIGHT_ARROW,	// EPA_Right,
		EventKeyboard::KeyCode::KEY_SPACE,			// EPA_Fire	
	};

	u32 uSizeOfActionArray = (sizeof(aeKeyCodesForActions) / sizeof(cocos2d::EventKeyboard::KeyCode));

	// call base class function	to init the keyboard manager
	AppDelegate::InitialiseKeyboardManager(uSizeOfActionArray, aeKeyCodesForActions);
	m_pcGCSprTimeUp->SetVisible( false );
	

	m_pPShipTrail->resetSystem();
	//set up timer handlers 
	m_bParalyizeTimerSet		= false;
	m_bPulseTimerComplete		= false;
	m_bParalyzeTimerComplete	= false;
	m_bParalyizePulseOn			= false;
	m_bPulseTimerSet			= false;
	m_bLevelTimerComplete		= false;
	m_bLevelTimerSet			= false;
	m_bLevelTimerFinished		= false;
	

}


CGCObjSprite* CGCGameLayerPlatformer::CreateFullscreenSprite( cocos2d::Size &visibleSize, const char* pszPathToImage, int iZOrder )
{
	//const char* pszPlist_background = "TexturePacker/Backgrounds/Brain/BrainBackground.plist";

	CGCObjSprite* m_pcGCSprite = new CGCObjSprite();
	m_pcGCSprite->CreateSprite( pszPathToImage );

	m_pcGCSprite->SetResetPosition( b2Vec2( visibleSize.width / 2, visibleSize.height / 2 ) );
	m_pcGCSprite->SetParent( IGCGameLayer::ActiveInstance(), iZOrder );
	return m_pcGCSprite;
}

void CGCGameLayerPlatformer::AddMenu( cocos2d::Size &visibleSize, cocos2d::Point &origin )
{
	MenuItemImage* pResetItem
		= MenuItemImage::create( "Loose/CloseNormal.png",
								"Loose/CloseSelected.png",
								CC_CALLBACK_1( CGCGameLayerPlatformer::Callback_OnResetButton, this ) );

	pResetItem->setPosition( Vec2( ((visibleSize.width - (pResetItem->getContentSize().width * 0.5f)) + origin.x),
		(((pResetItem->getContentSize().height * 0.5f) + origin.y)) ) );

	MenuItemImage* pQuitItem 
		= MenuItemImage::create( "Loose/BackNormal.png",
								"Loose/BackSelected.png",
								CC_CALLBACK_1( CGCGameLayerPlatformer::Callback_OnQuitButton, this ) );

	pQuitItem->setPosition( Vec2( ((visibleSize.width - (pQuitItem->getContentSize().width * 0.5f)) + origin.x),
		((visibleSize.height - (pQuitItem->getContentSize().height * 0.5f)) + origin.y) ) );

	// create menu, it's an autorelease object
	Menu* pMenu = Menu::create( pResetItem, pQuitItem, NULL );
	pMenu->setPosition( Vec2::ZERO );
	this->addChild( pMenu, 1 );
}

cocos2d::ParticleSystemQuad* CGCGameLayerPlatformer::CreateParticleSystem( const char* pszPlist , int iZPos )
{

	ParticleSystemQuad* particle = ParticleSystemQuad::create( pszPlist );

	particle->setStartSize( 2 );

	this->addChild( particle, iZPos);

	return particle;
}

void CGCGameLayerPlatformer::CreateObjectGroups()
{

	///////////////////////////////////////////////////////////////////////////
// custom object groups
//
// N.B. Cannot do this in CGCObjectGroup internally on construction, 
// because ObjectGroupRegister calls a virtual function 
// in the CGCObjectManager interface to check the 
// types of objects that the group handles
///////////////////////////////////////////////////////////////////////////

	m_pcGCGroupPlatform = new CGCObjGroupPlatform();
	CGCObjectManager::ObjectGroupRegister( m_pcGCGroupPlatform );

	// create and register the object group for the item objects
	m_pcGCGroupItem = new CGCObjGroupItem();
	CGCObjectManager::ObjectGroupRegister( m_pcGCGroupItem );

	// create and register the object group for the invader objects
	m_pcGCGroupEnemy = new CGCObjGroupEnemy();
	CGCObjectManager::ObjectGroupRegister( m_pcGCGroupEnemy );

	m_pcGCGroupDoor = new CGCObjGroupDoor();
	CGCObjectManager::ObjectGroupRegister( m_pcGCGroupDoor );


	m_pcGCObjGroupParalysis = new CGCObjGroupAOEParalysis();
	CGCObjectManager::ObjectGroupRegister( m_pcGCObjGroupParalysis );

	m_pcGCObjGroupBloodFlow = new CGCObjGroupAOEBloodFlow();
	CGCObjectManager::ObjectGroupRegister( m_pcGCObjGroupBloodFlow );
}

void CGCGameLayerPlatformer::CreateEnemies()
{

	///////////////////////////////////////////////////////////////////////////
	// add enemies
	///////////////////////////////////////////////////////////////////////////
	 //Spawn enemies: enemy number followed by starting position
	m_pcGCGroupEnemy->SetEnemyPosition( 1, b2Vec2( 580.0f, (400.0f) ) );
	m_pcGCGroupEnemy->SetEnemyPosition( 2, b2Vec2( 500.0f, (550.0f) ) );
	m_pcGCGroupEnemy->SetEnemyPosition( 3, b2Vec2( 500.0f, (250.0f) ) );

	// Set enemy patrols: enemy number followed by patrol minimum & maximum points, followed by patrol direction
	// Patrol direction: 0 = left and right, 1 = up and down
	m_pcGCGroupEnemy->SetEnemyPatrol( 1, 350.0f, 460.0f, 1 );
	m_pcGCGroupEnemy->SetEnemyPatrol( 2, 400.0f, 700.0f, 0 );
	m_pcGCGroupEnemy->SetEnemyPatrol( 3, 350.0f, 710.0f, 0 );


	//m_pcGCObjHomingEnemey = new CGCObjHomingEnemy;
	//m_pcGCObjHomingEnemey->SetResetPosition( b2Vec2( 300.0f, 400.0f ) );



	///////////////////////////////////////////////////////////////////////////
	// add platforms & items
	///////////////////////////////////////////////////////////////////////////
	// CGCObjGroupItem & CGCObjGroupPlatform clean up all contained objects during VOnGroupResourceRelease(), so we can be 
	// very laissez faire about creating items & platforms

	////Paralysis block
	//m_pcGCParalysis = new CGCObjAOEParalysis;
	//m_pcGCParalysis->SetResetPosition( b2Vec2( 300.0f, 500.0f ) );

	////Slow block
	//m_pcGCObjSlow = new CGCObjAOESlow;
	//m_pcGCObjSlow->SetResetPosition( b2Vec2( 500.0f, 480.0f ) );

	//Blood Flow
	//m_pcGCBloodFlow = new CGCObjAOEBloodFlow(b2Vec2(0.0f, 1000.0f));
	//m_pcGCBloodFlow->SetResetPosition( b2Vec2( 330.0f, 90.0f ) );
}

void CGCGameLayerPlatformer::CreateGameBounds( const b2Vec2 &v2ScreenCentre_Pixels, cocos2d::Size &visibleSize, const f32 &PTM_RATIO )
{
	// define the ground body
	b2BodyDef groundBodyDef;
	groundBodyDef.position = IGCGameLayer::B2dPixelsToWorld( v2ScreenCentre_Pixels );
	groundBodyDef.type = b2_kinematicBody;

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = B2dGetWorld()->CreateBody( &groundBodyDef );

	// Define the ground box shape.
	b2PolygonShape groundBox;

	// bottom
	groundBox.SetAsBox( ((visibleSize.width * 0.5f) / PTM_RATIO), 0.5f, b2Vec2( 0.0f, -((visibleSize.height * 0.5f) / PTM_RATIO) ), 0.0f );
	groundBody->CreateFixture( &groundBox, 0 );

	// top
	groundBox.SetAsBox( ((visibleSize.width * 0.5f) / PTM_RATIO), 0.5f, b2Vec2( 0.0f, ((visibleSize.height * 0.5f) / PTM_RATIO) ), 0.0f );
	groundBody->CreateFixture( &groundBox, 0 );

	// left
	groundBox.SetAsBox( 0.5f, ((visibleSize.height * 0.5f) / PTM_RATIO), b2Vec2( -((visibleSize.width * 0.5f) / PTM_RATIO), 0.0f ), 0.0f );
	groundBody->CreateFixture( &groundBox, 0 );

	// right
	groundBox.SetAsBox( 0.5f, ((visibleSize.height * 0.5f) / PTM_RATIO), b2Vec2( ((visibleSize.width * 0.5f) / PTM_RATIO), 0.0f ), 0.0f );
	groundBody->CreateFixture( &groundBox, 0 );
}

void CGCGameLayerPlatformer::CreateLevelFromArray( cocos2d::Point &origin  )
{
	const int m_ciWidth = 40;
	const int m_ciLength = 40;
	const int m_ciSpacing = 25;

	int piaLevelArray[m_ciWidth * m_ciLength]{
		
	  //0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//0
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//1
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//2
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//3
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//4
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//5
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//6
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//7
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//8
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//9
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//0
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//1
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//2
		0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,		//3
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,		//4
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,30,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,		//5
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,5,0,1,0,0,0,0,0,0,0,0,0,		//6
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,		//7
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,		//8
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,		//9
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,		//0
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,		//1
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,		//2
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,		//3
		0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,		//4
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,31,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,		//5
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,31,0,10,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,		//6
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,		//7
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,		//8
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,		//9
		0,0,0,0,0,0,0,0,0,0,0,0,1,11,11,11,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,		//0
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,		//1
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,		//2
		0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,21,21,1,0,0,0,0,0,0,0,0,0,		//3
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,		//4
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,		//5
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,		//6
		0,0,0,0,0,0,0,0,0,0,0,0,1,20,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,		//7
		0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,51,51,1,0,0,0,0,0,0,0,0,0			//9
	};


	//int piaLevelArray[m_ciWidth * m_ciLength]{

	//	//0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//0
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//1
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//2
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//3
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//4
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//5
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//6
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//7
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//8
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//9
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//0
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//1
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//2
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//3
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//4
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//5
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,		//6
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//7
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//8
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//9
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//0
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//1
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//2
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//3
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//4
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//5
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//6
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//7
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//8
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//9
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//0
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//1
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//2
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//3
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//4
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//5
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//6
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		//7
	//	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0			//9
	//};
	//		This array is the level. The for loop below itterates through the array and 
	//		builds the level. 

	//		-KEY FOR ARRAY-
	//		31 is a blue door
	//		30 is a blue key
	//		21 is a red door
	//		20 is a red key
	//		11 is a green door 
	//		10 is a green key 
	//		5 is a player
	//		1 is a wall
	//		0 is empty space
	b2Vec2 v2NextPlatformPos;
	b2Vec2 v2NextCornerPos;
	b2Vec2 v2PlayerStartPos;
	b2Vec2 v2KeyPos;
	for (int x = 0; x < m_ciWidth; ++x) {

		for (int y = 0; y < m_ciLength; ++y)
		{
			int index = (y * m_ciWidth) + x;


			// the individual parts of this switch statements are in empty case blocks to prevent multiple inits
			switch (piaLevelArray[index])
			{
			case 1:
			{
				m_pcGCObjPlatform = new CGCObjPlatform();
				v2NextPlatformPos.Set( (origin.x + (x * m_ciSpacing)), (origin.y + ((m_ciLength - y) * m_ciSpacing)) );
				m_pcGCObjPlatform->SetResetPosition( v2NextPlatformPos );


				break;
			}
			case 5:
			{

				v2PlayerStartPos.Set( (origin.x + (x * m_ciSpacing)), (origin.y + ((m_ciLength - y) * m_ciSpacing)) );
				m_pcGCOPlayer = new CGCObjPlayer();
				m_pcGCOPlayer->SetResetPosition( v2PlayerStartPos );

				break;

			}
		
			case 10:
			{
				m_pcGCObjItem = new CGCObjItem( 0 );
				v2KeyPos.Set( (origin.x + (x * m_ciSpacing)), (origin.y + ((m_ciLength - y) * m_ciSpacing)) );
				m_pcGCObjItem->SetResetPosition( v2KeyPos );

				break;
			}
			case 11:
			{
				b2Vec2 v2DoorPos ((origin.x + (x * m_ciSpacing)), (origin.y + ((m_ciLength - y) * m_ciSpacing)));
				CGCObjDoor* m_pcGCObjDoor = new CGCObjDoor( 0 );
				m_pcGCObjDoor->SetResetPosition( v2DoorPos );
				break;
			}
			case 12:
			{

				CGCObjDNA* m_pcGCObjDNA = new CGCObjDNA( 0 );
				v2KeyPos.Set( (origin.x + (x * m_ciSpacing)), (origin.y + ((m_ciLength - y) * m_ciSpacing)) );
				m_pcGCObjDNA->SetResetPosition( v2KeyPos );

				break;
			}
			case 20:
			{
				m_pcGCObjItem = new CGCObjItem( 1 );
				v2KeyPos.Set( (origin.x + (x * m_ciSpacing)), (origin.y + ((m_ciLength - y) * m_ciSpacing)) );
				m_pcGCObjItem->SetResetPosition( v2KeyPos );

				break;
			}
			case 21:
			{
				b2Vec2 v2DoorPos( (origin.x + (x * m_ciSpacing)), (origin.y + ((m_ciLength - y) * m_ciSpacing)) );

				CGCObjDoor* m_pcGCObjDoor = new CGCObjDoor( 1 );
				m_pcGCObjDoor->SetResetPosition( v2DoorPos );
				break;
			}
			case 30:
			{
				b2Vec2 v2KeyPos( (origin.x + (x * m_ciSpacing)), (origin.y + ((m_ciLength - y) * m_ciSpacing)) );

				m_pcGCObjItem = new CGCObjItem( 2 );
				m_pcGCObjItem->SetResetPosition( v2KeyPos );
				break;
			}
			case 31:
			{
				b2Vec2 v2DoorPos( (origin.x + (x * m_ciSpacing)), (origin.y + ((m_ciLength - y) * m_ciSpacing)) );

				CGCObjDoor* m_pcGCObjDoor = new CGCObjDoor( 2 );
				m_pcGCObjDoor->SetResetPosition( v2DoorPos );
				break;
			}
			case 51:
			{


				b2Vec2 v2WinDoorPos( (origin.x + (x * m_ciSpacing)), (origin.y + ((m_ciLength - y) * m_ciSpacing)) );
				CGCObjWinCube* m_pcGCWinCube = new CGCObjWinCube();
				m_pcGCWinCube->SetResetPosition ( v2WinDoorPos );
				break;


			}
			default:
				//atm the default case is a 0 so nothing 

				break;


			}


		}


	}
}




void CGCGameLayerPlatformer::VOnUpdate( f32 fTimeStep )
{

	if (m_pcGCOPlayer->GetAlive() == false)
	{

		Director::getInstance()->replaceScene( TransitionRotoZoom::create( 1.0f, TGCGameLayerSceneCreator< CGCObjGameOverScene >::CreateScene() ) );

	}

	if (m_pcGCOPlayer->GetWin() == true)
	{
		Director::getInstance()->replaceScene( TransitionRotoZoom::create( 1.0f, TGCGameLayerSceneCreator< CGCObjLevel2Scene >::CreateScene() ) );


	}

	//Create timer functions, create timers at appropriate times


	CreateTimer( ETimers::eParalyzeTimer, 3.0f );
	CreateTimer( ETimers::ePulseTimer, 4.0f );
	CreateTimer( ETimers::eSlowPlayertimer, 3.0f );
	CreateTimer( ETimers::eLevelTimer, 30.0f );

	//resolve timer functions, set bools when timers are finished
	
	//You cant set visible in place other then vonupdate or onenter, so
	//this cant be extracted to a method
	if (m_pcGCSprTimeUp != NULL) {
		if (!m_bLevelTimerFinished)
		{
			m_pcGCSprTimeUp->SetVisible( false );
		}
		else
		{

			m_pcGCSprTimeUp->SetVisible( true );
			Director::getInstance()->pause();
		}
	}

	if (m_bParalyzeTimerComplete) {
		ResolveTimer( ETimers::eParalyzeTimer );
	}
	if (m_bPulseTimerComplete) {
		ResolveTimer( ETimers::ePulseTimer );
	}

	if (m_bLevelTimerComplete) {
		ResolveTimer( ETimers::eLevelTimer );
	}
	if (m_bSlowTimerComplete){
		ResolveTimer( ETimers::eSlowPlayertimer );

	}


	m_pPShipTrail->setEmissionRate( m_pcGCOPlayer->GetVelocity().Length() );

	
	m_pPShipTrail->setPositionX(m_pcGCOPlayer->GetSpritePosition().x);
	m_pPShipTrail->setPositionY(m_pcGCOPlayer->GetSpritePosition().y);




	m_pPShipTrail->setAngle(m_pcGCOPlayer->GetSpriteRotation() * 10);


	//b2Vec2 dif = (b2Vec2( m_pcGCOPlayer->GetSpritePosition().x, m_pcGCOPlayer->GetSpritePosition().y ) - b2Vec2( m_pcGCObjHomingEnemey->GetSpritePosition().x, m_pcGCObjHomingEnemey->GetSpritePosition().y ));
	//m_pcGCObjHomingEnemey->SetPlayerDirection( dif );
	////m_pTestParticle->setPositionY( origin.y + 300 );


	IGCGameLayer::VOnUpdate(fTimeStep);

	
	//Set x scale of impulse bar based on current impulse level (managed in Player)
	//m_pdImpulseBar->setScaleX(((float)m_pcGCOPlayer->GetImpulseCharge() / 100));


	AppDelegate::GetKeyboardManager()->Update();
	HandleCollisions();



}


///////////////////////////////////////////////////////////////////////////////
// on destroy
///////////////////////////////////////////////////////////////////////////////
// virtual
void CGCGameLayerPlatformer::VOnDestroy()
{
	///////////////////////////////////////////////////////////////////////////
	// clean up anything we allocated in opposite order to creation
	///////////////////////////////////////////////////////////////////////////	
	if (m_pcGCOPlayer != NULL)
	{
		delete m_pcGCOPlayer;
		m_pcGCOPlayer = NULL;
	}
	if (m_pcGCSprBackGround != NULL)
	{
		m_pcGCSprBackGround->DestroySprite();
		delete m_pcGCSprBackGround;
		m_pcGCSprBackGround = NULL;
	}
	if (m_pcGCSprTimeUp != NULL) 
	{
		m_pcGCSprTimeUp->DestroySprite();
		delete m_pcGCSprTimeUp;
		m_pcGCSprTimeUp = NULL;


	}


	 
	///////////////////////////////////////////////////////////////////////////
	// N.B. because object groups must register manually, 																				  
	// we also unregister them manually
	///////////////////////////////////////////////////////////////////////////
	DestroyObjectGroups();

	IGCGameLayer::VOnDestroy();
}

void CGCGameLayerPlatformer::DestroyObjectGroups()
{
	if (m_pcGCGroupPlatform != NULL)
	{
		CGCObjectManager::ObjectGroupUnRegister( m_pcGCGroupPlatform );
		delete m_pcGCGroupPlatform;
		m_pcGCGroupPlatform = NULL;
	}

	if (m_pcGCGroupEnemy != NULL)
	{
		CGCObjectManager::ObjectGroupUnRegister( m_pcGCGroupEnemy );
		delete m_pcGCGroupEnemy;
		m_pcGCGroupEnemy = NULL;
	}
	if (m_pcGCGroupItem != NULL)
	{
		CGCObjectManager::ObjectGroupUnRegister( m_pcGCGroupItem );
		delete m_pcGCGroupItem;
		m_pcGCGroupItem = NULL;
	}
	if (m_pcGCGroupDoor != NULL)
	{
		CGCObjectManager::ObjectGroupUnRegister( m_pcGCGroupDoor );
		delete m_pcGCGroupDoor;
		m_pcGCGroupDoor = NULL;
	}

	if (m_pcGCObjGroupParalysis != NULL)
	{
		CGCObjectManager::ObjectGroupUnRegister( m_pcGCObjGroupParalysis );
		delete m_pcGCObjGroupParalysis;
		m_pcGCObjGroupParalysis = NULL;
	}
	if (m_pcGCObjGroupBloodFlow != NULL)
	{
		CGCObjectManager::ObjectGroupUnRegister( m_pcGCObjGroupBloodFlow );
		delete m_pcGCObjGroupBloodFlow;
		m_pcGCObjGroupBloodFlow = NULL;
	}
}


///////////////////////////////////////////////////////////////////////////////
// on quit button
///////////////////////////////////////////////////////////////////////////////
void CGCGameLayerPlatformer::Callback_OnQuitButton( Ref* pSender )
{
	ReplaceScene( TransitionRotoZoom::create( 1.0f, CMenuLayer::scene() ) );
}


///////////////////////////////////////////////////////////////////////////////
// on reset button
///////////////////////////////////////////////////////////////////////////////
void CGCGameLayerPlatformer::Callback_OnResetButton(Ref* pSender)
{
	VOnReset();
}


///////////////////////////////////////////////////////////////////////////////
// begin contact
// insert any logic that relies on detecting the first frame where a 
// contact exists
///////////////////////////////////////////////////////////////////////////////
//virtual 
void CGCGameLayerPlatformer::BeginContact( b2Contact* pB2Contact )
{
}


///////////////////////////////////////////////////////////////////////////////
// end contact
// insert any logic that relies on detecting the last frame where a 
// contact exists
///////////////////////////////////////////////////////////////////////////////
//virtual 
void CGCGameLayerPlatformer::EndContact( b2Contact* pB2Contact )
{
}


///////////////////////////////////////////////////////////////////////////////
// pre solve
// insert any logic that needs to be done before a contact is resolved
///////////////////////////////////////////////////////////////////////////////
//virtual 
void CGCGameLayerPlatformer::PreSolve( b2Contact* pB2Contact, const b2Manifold* pOldManifold ) 
{
	const b2Fixture* pFixtureA = pB2Contact->GetFixtureA();
	const b2Fixture* pFixtureB = pB2Contact->GetFixtureB();

	const b2Body* pBodyA = pFixtureA->GetBody();
	const b2Body* pBodyB = pFixtureB->GetBody();

	CGCObjSpritePhysics* pGcSprPhysA = (CGCObjSpritePhysics*) pBodyA->GetUserData();
	// if( this is not a GC object )
	if( pGcSprPhysA == NULL )
	{
		return;
	}

	CGCObjSpritePhysics* pGcSprPhysB = (CGCObjSpritePhysics*) pBodyB->GetUserData();
	// if( this is not a GC object )
	if( pGcSprPhysB == NULL )
	{
		return;
	}


	
}


///////////////////////////////////////////////////////////////////////////////
// post solve
// insert any logic that needs to be done after a contact is resolved
// e.g. check the types and double the impulse
///////////////////////////////////////////////////////////////////////////////
//virtual 
void CGCGameLayerPlatformer::PostSolve( b2Contact* pB2Contact, const b2ContactImpulse* pImpulse )
{
}

///////////////////////////////////////////////////////////////////////////////
// this function can only look at the collisions that happened (and which 
// have potentially been resolved) in the previous physics step
// 
// N.B. it is totally possible to get a callback after collisions have been 
// detected, but before they're resolved so you can choose to ignore them - 
// e.g. for gamplay reasons like jumping up through a platform
// 
///////////////////////////////////////////////////////////////////////////////
void CGCGameLayerPlatformer::HandleCollisions()
{
	// check for collisions
	b2Body* pBodyToDestroy = NULL;
	for(	const b2Contact* pB2Contact	= IGCGameLayer::ActiveInstance()->B2dGetWorld()->GetContactList();
			NULL						!= pB2Contact;
			pB2Contact					= pB2Contact->GetNext() )
	{
		const b2Fixture* pFixtureA = pB2Contact->GetFixtureA();
		const b2Fixture* pFixtureB = pB2Contact->GetFixtureB();

		const b2Body* pBodyA = pFixtureA->GetBody();
		const b2Body* pBodyB = pFixtureB->GetBody();


		// return if either physics body has null user data
		CGCObjSpritePhysics* pGcSprPhysA = (CGCObjSpritePhysics*) pBodyA->GetUserData();
		if( !pGcSprPhysA )
		{
			return;
		}

		CGCObjSpritePhysics* pGcSprPhysB = (CGCObjSpritePhysics*) pBodyB->GetUserData();
		if( !pGcSprPhysB )
		{
			return;
		}

		// different?
		if(	   pB2Contact->IsEnabled()
			&& ( pGcSprPhysA->GetGCTypeID() != pGcSprPhysB->GetGCTypeID() ) )
		{

	
			if ((pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf(CGCObjPlayer))
				|| (pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf(CGCObjPlayer)))
			{
				CGCObjSpritePhysics*	pObjSprPhys = (pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf(CGCObjPlayer)) ? pGcSprPhysA : pGcSprPhysB;
				CGCObjSpritePhysics*	tidNotPlayer = (pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf(CGCObjPlayer)) ? pGcSprPhysB : pGcSprPhysA;
				CGCObjPlayer*			pPlayer = static_cast<CGCObjPlayer*>(pObjSprPhys);

				// Enemy collision, reduces player's lives by 1 and resets the player's position
				if (GetGCTypeIDOf(CGCObjEnemy) == tidNotPlayer->GetGCTypeID())
				{
					pPlayer->SetLives(pPlayer->GetLives() - 1);
					if (pPlayer->GetLives() > 0)
					{
						pPlayer->ResetPosition();

					}
					else {
						pPlayer->SetAlive(false);
					}
				}
				else if (GetGCTypeIDOf(CGCObjItem) == tidNotPlayer->GetGCTypeID()) {
					CGCObjItem* pItem = static_cast<CGCObjItem*>(tidNotPlayer);

					pItem->PickUpItem(pPlayer);
					CGCObjectManager::ObjectKill(tidNotPlayer);


				}
				else if (GetGCTypeIDOf(CGCObjDoor) == tidNotPlayer->GetGCTypeID()) {
					CGCObjDoor* pDoor = static_cast<CGCObjDoor*>(tidNotPlayer);
					if (pPlayer->CheckKey(pDoor->GetDoorNumber()))
					{
						CGCObjectManager::ObjectKill(tidNotPlayer);
						//eventually switch this for a specific method to open the door  

					} else
					{
						
					}

				}
				// Next level cube collision, takes you to the next level
				else if (GetGCTypeIDOf(CGCObjWinCube) == tidNotPlayer->GetGCTypeID()) {
					pPlayer->SetWin(true);
				}
				//	Paralysis collision
				else if (GetGCTypeIDOf(CGCObjAOEParalysis) == tidNotPlayer->GetGCTypeID()) {
					if (m_bParalyizePulseOn) {


						pPlayer->SetParalyzed(true);

					}
				}
				//	Blood Flow collision
				else if (GetGCTypeIDOf(CGCObjAOEBloodFlow) == tidNotPlayer->GetGCTypeID()) {
					CGCObjAOEBloodFlow* pBloodFlow = static_cast<CGCObjAOEBloodFlow*>(tidNotPlayer);


					//Push player by certain amount
					pPlayer->PushByAmount( pBloodFlow->GetPushVector());
				}
				else if (GetGCTypeIDOf( CGCObjAOESlow ) == tidNotPlayer->GetGCTypeID()) {

					//Push player by certain amount
					pPlayer->SlowMovement();
				}
				else if (GetGCTypeIDOf( CGCObjDNA ) == tidNotPlayer->GetGCTypeID()) {
					CGCObjDNA* pDNA = static_cast<CGCObjDNA*>(tidNotPlayer);

					pDNA->PickUpItem( pPlayer );
					CGCObjectManager::ObjectKill( tidNotPlayer );


				}

			}
		}
	}
}


void CGCGameLayerPlatformer::VOnReset() 
{
	IGCGameLayer::VOnReset();      
 
}

//set up new timer, any new timers need to be added to this method and enum
void CGCGameLayerPlatformer::CreateTimer( ETimers ETimerID, float fTimeTaken )
{
	switch (ETimerID)
	{
	case ETimers::ePulseTimer:
		if (m_pcGCParalysis != NULL) {
			if (!m_bPulseTimerSet) {
				schedule( schedule_selector( CGCGameLayerPlatformer::PulseTimer ), fTimeTaken );
				m_bPulseTimerSet = true;
			}
		}

		break;
	case  ETimers::eParalyzeTimer:
		if (m_pcGCParalysis != NULL) {
			if ((m_pcGCOPlayer->GetParalyzed() == true) && (!m_bParalyizeTimerSet)) {

				schedule( schedule_selector( CGCGameLayerPlatformer::ParalyzeTimer ), fTimeTaken );
				m_bParalyizeTimerSet = true;
			}
		}

		break;

	case ETimers::eLevelTimer:
		if (!m_bLevelTimerSet) {

			schedule( schedule_selector( CGCGameLayerPlatformer::LevelTimer ), fTimeTaken );
			m_bLevelTimerSet = true;
		}
	
		
		break;

	case ETimers::eSlowPlayertimer:
		if (m_pcGCObjSlow != NULL) {
			if ((m_pcGCOPlayer->GetSlowed() == true && !m_bSlowTimerSet )) {

				schedule( schedule_selector( CGCGameLayerPlatformer::SlowPlayerTimer),fTimeTaken);
				m_bSlowTimerSet = true;
			}
		}


		break;
	default:
		break;
	}

}

//resolve any timer activity, takes timers enum
void CGCGameLayerPlatformer::ResolveTimer( ETimers ETimerID ) 
{
	switch (ETimerID)
	{
	case ETimers::eParalyzeTimer:
		
		
		m_pcGCOPlayer->SetParalyzed( false );
		m_bParalyzeTimerComplete = false;
		m_bParalyizeTimerSet = false;
		break;
	case ETimers::ePulseTimer:

		if (m_pcGCParalysis != NULL) {
			if (m_bParalyizePulseOn)
			{
				m_bParalyizePulseOn = false;
				m_pcGCParalysis->StopPulse();
			}
			else
			{
				m_bParalyizePulseOn = true;
				m_pcGCParalysis->Pulse();
			}
			m_bPulseTimerSet = false;
			m_bPulseTimerComplete = false;
		}

		break;
	case ETimers::eLevelTimer:

		m_bLevelTimerFinished = true;
		m_bLevelTimerSet = false;
		m_bLevelTimerComplete = false;

		break;
	case ETimers::eSlowPlayertimer:

		m_pcGCOPlayer->RestoreMovement();
		m_bSlowTimerSet = false;
		m_bSlowTimerComplete = false;
		break;

	default:
		break;
	}


}


//actual timer count down methods - need to be seperate cos each sets a 
//diferent flag
void CGCGameLayerPlatformer::ParalyzeTimer(float dt)
{
	m_fParalyzeTime += dt;
	m_bParalyzeTimerComplete = true;

}

void CGCGameLayerPlatformer::PulseTimer( float dt )
{

	m_fPulseTime += dt;
	m_bPulseTimerComplete = true;


}

void CGCGameLayerPlatformer::LevelTimer( float dt ) 
{

	m_fLevelTime += dt;
	m_bLevelTimerComplete = true;

}			

void CGCGameLayerPlatformer::SlowPlayerTimer( float dt ) 
{

	m_fLevelTime += dt;
	m_bSlowTimerComplete = true;

}