#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "MenuScene.h"
#include "AppDelegate.h"
#include "GamerCamp/GameSpecific/Levels/GCObjGameOverScene.h"
#include "GamerCamp/GCCocosInterface/GCObjSprite.h"

// Using cocos namespace, helps to shorten the use of some built-in functionality
USING_NS_CC;


// Constructor
CGCObjGameOverScene::CGCObjGameOverScene()
	: IGCGameLayer(GetGCTypeIDOf( CGCObjGameOverScene ))
{
}


// Destructor
CGCObjGameOverScene::~CGCObjGameOverScene()
{
}


// Calls inherited on enter function
void CGCObjGameOverScene::onEnter() 
{
	IGCGameLayer::onEnter();
}


// Virtual
// Calls inherited on create function, makes splash screen and buttons
void CGCObjGameOverScene::VOnCreate()
{
	IGCGameLayer::VOnCreate();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();


	//m_pcGCSprGameOver = CGCObjSprite::create( "TexturePacker/Backgrounds/GameOver/GameOver.png" );
	//m_pcGCSprGameOver->setPosition( Vec2( origin.x + (visibleSize.width * 0.5f), origin.y + (visibleSize.height* 0.5f) ) );

	m_pcGCSprGameOver = new CGCObjSprite();
	m_pcGCSprGameOver->CreateSprite( "TexturePacker/Backgrounds/GameOver/GameOver.plist" );
	m_pcGCSprGameOver->SetResetPosition( b2Vec2( visibleSize.width / 2, visibleSize.height / 2 ) );
	m_pcGCSprGameOver->SetParent( IGCGameLayer::ActiveInstance(), 1 );


	MenuItemImage* pItemStartGame = MenuItemImage::create(
		"Loose/CloseNormal.png",
		"Loose/CloseSelected.png",
		CC_CALLBACK_1( CGCObjGameOverScene::Callback_OnResetButton, this));

	pItemStartGame->setPosition(Vec2(origin.x + (visibleSize.width * 0.5f),
		origin.y + (visibleSize.height * 0.25f)));

	// create menu, it's an autorelease object
	Menu* pMenu = Menu::create(pItemStartGame, NULL);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu, 1);
}


// Virtual
// Master update for game over scene
void CGCObjGameOverScene::VOnUpdate(f32 timestep)
{
	IGCGameLayer::VOnUpdate(timestep);
	AppDelegate::GetKeyboardManager()->Update();
}


// Virtual
// Calls inherited on destroy function
void CGCObjGameOverScene::VOnDestroy()
{
	m_pcGCSprGameOver->DestroySprite();
	delete m_pcGCSprGameOver;
	m_pcGCSprGameOver = NULL;

	IGCGameLayer::VOnDestroy();
}


// Reset button function
void CGCObjGameOverScene::Callback_OnResetButton(Ref* pSender)
{
	Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0f, TGCGameLayerSceneCreator< CGCGameLayerPlatformer >::CreateScene()));

}