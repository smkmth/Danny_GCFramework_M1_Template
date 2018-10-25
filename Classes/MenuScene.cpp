////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MenuScene.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GamerCamp/GameSpecific/Levels/GCObjLevelSelectScene.h"
#include "GamerCamp/GameSpecific/Levels/GCObjOptionsScene.h"


USING_NS_CC;


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
Scene* CMenuLayer::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();

	// 'layer' is an autorelease object
	CMenuLayer *layer = CMenuLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
bool CMenuLayer::init()
{
    //////////////////////////////
	//Init
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize	= Director::getInstance()->getVisibleSize();
    Vec2 origin			= Director::getInstance()->getVisibleOrigin();



    /////////////////////////////
	//Menu Items

    //Play Button
    MenuItemImage* pItemPlayButton = MenuItemImage::create(
                                        "Loose/PlayNormal.png",
                                        "Loose/PlaySelected.png",
                                        CC_CALLBACK_1( CMenuLayer::CB_OnGameStartButton, this ) );
	pItemPlayButton->setPosition(Vec2(513.0f, 422.0f));
    Menu* pItemPlayButtonMenu = Menu::create(pItemPlayButton, NULL);
	pItemPlayButtonMenu->setPosition( Vec2::ZERO );
    this->addChild(pItemPlayButtonMenu, 1);

	//Level Select Button
	MenuItemImage* pItemLevelSelectButton = MenuItemImage::create(
		"Loose/LevelSelectNormal.png",
		"Loose/LevelSelectSelected.png",
		CC_CALLBACK_1(CMenuLayer::CB_OnGameLevelSelectButton, this));
	pItemLevelSelectButton->setPosition(Vec2(513.0f, 322.0f));
	Menu* pItemLevelSelectButtonMenu = Menu::create(pItemLevelSelectButton, NULL);
	pItemLevelSelectButtonMenu->setPosition(Vec2::ZERO);
	this->addChild(pItemLevelSelectButtonMenu, 1);

	//Options Button
	MenuItemImage* pItemOptionsButton = MenuItemImage::create(
		"Loose/OptionsNormal.png",
		"Loose/OptionsSelected.png",
		CC_CALLBACK_1(CMenuLayer::CB_OnGameOptionsButton, this));
	pItemOptionsButton->setPosition(Vec2(513.0f, 222.0f));
	Menu* pItemOptionsButtonMenu = Menu::create(pItemOptionsButton, NULL);
	pItemOptionsButtonMenu->setPosition(Vec2::ZERO);
	this->addChild(pItemOptionsButtonMenu, 1);

	//Exit Button
	MenuItemImage* pItemExitButton = MenuItemImage::create(
		"Loose/ExitNormal.png",
		"Loose/ExitSelected.png",
		CC_CALLBACK_1(CMenuLayer::CB_OnGameExitButton, this));
	pItemExitButton->setPosition(Vec2(513.0f, 122.0f));
	Menu* pItemExitButtonMenu = Menu::create(pItemExitButton, NULL);
	pItemExitButtonMenu->setPosition(Vec2::ZERO);
	this->addChild(pItemExitButtonMenu, 1);

    //Splash Screen
    Sprite* pSprite = Sprite::create("Loose/HelloWorld.png");
    pSprite->setPosition( Vec2( visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y ) );
    this->addChild(pSprite, 0);
    
    return true;
}



//////////////////////////////////////////////////////////////////////////
//	Menu Button Functions
//////////////////////////////////////////////////////////////////////////
void CMenuLayer::CB_OnGameStartButton( Ref* pSender)
{
	Director::getInstance()->replaceScene( TransitionRotoZoom::create( 1.0f, TGCGameLayerSceneCreator< CGCGameLayerPlatformer >::CreateScene() ) );
}

void CMenuLayer::CB_OnGameLevelSelectButton(Ref * pSender)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, TGCGameLayerSceneCreator< CGCObjLevelSelectScene >::CreateScene()));
}

void CMenuLayer::CB_OnGameOptionsButton(Ref * pSender)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, TGCGameLayerSceneCreator< CGCObjOptionsScene >::CreateScene()));
}

void CMenuLayer::CB_OnGameExitButton(Ref * pSender)
{
	exit(0);
}