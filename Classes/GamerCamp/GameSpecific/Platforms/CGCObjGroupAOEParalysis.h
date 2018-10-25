////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	_GCOBJECTGROUPAOEPARALYSIS_H_
#define	_GCOBJECTGROUPAOEPARALYSIS_H_


#ifndef BOX2D_H
	#include "Box2d/Box2D.h"
#endif

#ifndef	_GCOBJECTGROUP_H_
	#include "GamerCamp/GCObject/GCObjectGroup.h"
#endif


//////////////////////////////////////////////////////////////////////////
// forward declare
class CGCObjSprite;
class CGCObjGroupAOEParalysis;


//////////////////////////////////////////////////////////////////////////
// 
class CGCObjGroupAOEParalysis
: public CGCObjectGroup
{
	// destroy managed platforms
	void DestroyPlatforms( void );

public:
	//// number of platforms
	//static const u32 k_uNumInvaders = 16;

	CGCObjGroupAOEParalysis();		
	virtual ~CGCObjGroupAOEParalysis();

//////////////////////////////////////////////////////////////////////////
// overrides for CGCObjectGroup public interface

	// handles GCObjPlatform
	virtual bool		VHandlesThisTypeId					( GCTypeID idQueryType );

	// must return the typeid of the CGCObjectGroup derived class
	virtual GCTypeID	VGetTypeId							();

	virtual void		VOnGroupResourceRelease				();
	void				PulseAll							();


	virtual void		VOnGroupResourceAcquire();

	struct				SArrayOfAOEParalysis;

// CGCObjectGroup public interface
//////////////////////////////////////////////////////////////////////////
};

#endif
