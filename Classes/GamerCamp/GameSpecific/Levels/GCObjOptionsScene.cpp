#include "GamerCamp/GameSpecific/Levels/GCObjOptionsScene.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "MenuScene.h"
#include "AppDelegate.h"

// Using cocos namespace, helps to shorten the use of some built-in functionality
USING_NS_CC;


// Constructor
CGCObjOptionsScene::CGCObjOptionsScene()
	: IGCGameLayer(GetGCTypeIDOf(CGCObjOptionsScene))
{

}


// Destructor
CGCObjOptionsScene::~CGCObjOptionsScene()
{

}


// Calls inherited on enter function
void CGCObjOptionsScene::onEnter() 
{
	IGCGameLayer::onEnter();
}


// Virtual
// Calls inherited on create function, makes splash screen and buttons
void CGCObjOptionsScene::VOnCreate()
{
	IGCGameLayer::VOnCreate();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();


	Label* pLabel = Label::createWithTTF("This is the options screen", "fonts/arial.ttf", 28);
	pLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 50));
	this->addChild(pLabel, 1);

	MenuItemImage* pItemBackButton = MenuItemImage::create(
		"Loose/BackNormal.png",
		"Loose/BackSelected.png",
		CC_CALLBACK_1(CGCObjOptionsScene::Callback_OnBackButton, this));

	pItemBackButton->setPosition(Vec2(100.0f, 100.0f));

	// create menu, it's an autorelease object
	Menu* pItemBackButtonMenu = Menu::create(pItemBackButton, NULL);
	pItemBackButtonMenu->setPosition(Vec2::ZERO);
	this->addChild(pItemBackButtonMenu, 1);
}


// Virtual
// Master update for level select scene
void CGCObjOptionsScene::VOnUpdate(f32 timestep)
{
	IGCGameLayer::VOnUpdate(timestep);
	AppDelegate::GetKeyboardManager()->Update();
}


// Virtual
// Calls inherited on destroy function
void CGCObjOptionsScene::VOnDestroy() 
{
	IGCGameLayer::VOnDestroy();
}


// Back button function
void CGCObjOptionsScene::Callback_OnBackButton(Ref* pSender)
{
	ReplaceScene(TransitionFade::create(1.0f, CMenuLayer::scene()));
}
