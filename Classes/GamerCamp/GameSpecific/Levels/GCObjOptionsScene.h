#ifndef __GCOBJOPTIONSSCENE_H__
#define __GCOBJOPTIONSSCENE_H__

#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "cocos2d.h"

class CGCObjOptionsScene
	: public IGCGameLayer
{
public:

	// Constructor
	CGCObjOptionsScene();
	// Destructor
	~CGCObjOptionsScene();


	// Calls inherited on enter function
	virtual void onEnter();
	// Calls inherited on create function, makes splash screen and buttons
	virtual void VOnCreate();
	// Master update for level 2 scene
	virtual void VOnUpdate( f32 timestep );
	// Calls inherited on destroy function
	virtual void VOnDestroy();
	// Reset button function
	void Callback_OnBackButton( Ref* pSender );


	// Explicit constructor - called from the CREATE_FUNC() macro
	// n.b. this must back-chain calls to the base class versions of init()
	// or the behaviour of your code will be 'unexpected'

	// The cocos2d convention is to have the root layer of a scene
	// be able to create and return a CCScene* (set to autorelease)
	// which has an instance of itself as a child
	//static cocos2d::Scene* scene();    
};
#endif // __GCOBJOPTIONSSCENE_H__
