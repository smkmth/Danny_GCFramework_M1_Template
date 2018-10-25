////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	_GCOBJECTGROUPAOESLOW_H_
#define	_GCOBJECTGROUPAOESLOW_H_


#ifndef BOX2D_H
	#include "Box2d/Box2D.h"
#endif

#ifndef	_GCOBJECTGROUP_H_
	#include "GamerCamp/GCObject/GCObjectGroup.h"
#endif


//////////////////////////////////////////////////////////////////////////
// forward declare
class CGCObjSprite;
class CGCObjGroupAOESlow;


//////////////////////////////////////////////////////////////////////////
// 
class CGCObjGroupAOESlow
: public CGCObjectGroup
{
	// destroy managed platforms
	void DestroyPlatforms( void );

public:
	//// number of platforms
	//static const u32 k_uNumInvaders = 16;

	CGCObjGroupAOESlow();		
	virtual ~CGCObjGroupAOESlow();

//////////////////////////////////////////////////////////////////////////
// overrides for CGCObjectGroup public interface

	// handles GCObjPlatform
	virtual bool		VHandlesThisTypeId					( GCTypeID idQueryType );

	// must return the typeid of the CGCObjectGroup derived class
	virtual GCTypeID	VGetTypeId							();

	virtual void		VOnGroupResourceRelease				();



	virtual void		VOnGroupResourceAcquire();



// CGCObjectGroup public interface
//////////////////////////////////////////////////////////////////////////
};

#endif
