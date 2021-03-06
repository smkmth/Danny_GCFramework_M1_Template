#ifndef	_GCOBJECTGROUPDOOR_H_
#define	_GCOBJECTGROUPDOOR_H_


#ifndef BOX2D_H
#include "Box2d/Box2D.h"
#endif

#ifndef	_GCOBJECTGROUP_H_
#include "GamerCamp/GCObject/GCObjectGroup.h"
#endif


//////////////////////////////////////////////////////////////////////////
// forward declare
class CGCObjSprite;
class CGCObjDoor;


//////////////////////////////////////////////////////////////////////////
// 
class CGCObjGroupDoor
	: public CGCObjectGroup
{
	// destroy managed platforms
	void DestroyPlatforms(void);

public:
	//// number of platforms
	//static const u32 k_uNumInvaders = 16;

	CGCObjGroupDoor();
	virtual ~CGCObjGroupDoor();

	//////////////////////////////////////////////////////////////////////////
	// overrides for CGCObjectGroup public interface

		// handles GCObjPlatform
	virtual bool		VHandlesThisTypeId(GCTypeID idQueryType);

	// must return the typeid of the CGCObjectGroup derived class
	virtual GCTypeID	VGetTypeId(void);

	virtual void		VOnGroupResourceRelease(void);

	// CGCObjectGroup public interface
	//////////////////////////////////////////////////////////////////////////
};

#endif
