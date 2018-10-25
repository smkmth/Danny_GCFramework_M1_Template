#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "MenuScene.h"
#include "AppDelegate.h"
#include "GamerCamp/GameSpecific/Levels/GCObjLevel2Scene.h"

// Using cocos namespace, helps to shorten the use of some built-in functionality
USING_NS_CC;


// Constructor
CGCObjLevel2Scene::CGCObjLevel2Scene()
	: IGCGameLayer( GetGCTypeIDOf( CGCObjLevel2Scene ) )
{
}


// Destructor
CGCObjLevel2Scene::~CGCObjLevel2Scene()
{
}


// Calls inherited on enter function
void CGCObjLevel2Scene::onEnter()
{
	IGCGameLayer::onEnter();
}


// Virtual
// Calls inherited on create function, makes splash screen and buttons
void CGCObjLevel2Scene::VOnCreate()
{
	IGCGameLayer::VOnCreate();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	Sprite* pSprite = Sprite::create( "TexturePacker/Backgrounds/WinScreen/WinScreen.png" );
	pSprite->setPosition( Vec2( origin.x + (visibleSize.width * 0.5f), origin.y + (visibleSize.height* 0.5f) ) );

	this->addChild( pSprite, 1 );

	MenuItemImage* pItemStartGame = MenuItemImage::create(
		"Loose/CloseNormal.png",
		"Loose/CloseSelected.png",
		CC_CALLBACK_1( CGCObjLevel2Scene::Callback_OnResetButton, this ) );

	pItemStartGame->setPosition( Vec2( origin.x + (visibleSize.width * 0.5f),
		origin.y + (visibleSize.height * 0.25f) ) );

	// Create menu, it's an autorelease object
	Menu* pMenu = Menu::create( pItemStartGame, NULL );
	pMenu->setPosition( Vec2::ZERO );
	this->addChild( pMenu, 1 );
}


// Virtual
// Master update for level 2 scene
void CGCObjLevel2Scene::VOnUpdate( f32 timestep ) {

	IGCGameLayer::VOnUpdate( timestep );
	AppDelegate::GetKeyboardManager()->Update();

}


// Virtual
// Calls inherited on destroy function
void CGCObjLevel2Scene::VOnDestroy()
{
	IGCGameLayer::VOnDestroy();
}


// Reset button function
void CGCObjLevel2Scene::Callback_OnResetButton( Ref* pSender )
{
	Director::getInstance()->replaceScene( TransitionCrossFade::create( 1.0f, TGCGameLayerSceneCreator< CGCGameLayerPlatformer >::CreateScene() ) );
}