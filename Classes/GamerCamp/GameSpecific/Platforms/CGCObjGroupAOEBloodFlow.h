#ifndef	_GCOBJECTGROUPAOEBLOODFLOW_H_
#define	_GCOBJECTGROUPAOEBLOODFLOW_H_


#ifndef BOX2D_H
	#include "Box2d/Box2D.h"
#endif

#ifndef	_GCOBJECTGROUP_H_
	#include "GamerCamp/GCObject/GCObjectGroup.h"
#endif


//////////////////////////////////////////////////////////////////////////
// Forward declarations
class CGCObjSprite;
class CGCObjGroupAOEBloodFlow;


class CGCObjGroupAOEBloodFlow
: public CGCObjectGroup
{

private:


public:

	// Constructor
	CGCObjGroupAOEBloodFlow();

	//////////////////////////////////////////////////////////////////////////
	// We need a virtual destructor since delete will be called on pointers of 
	// this class to delete derived types.
	virtual ~CGCObjGroupAOEBloodFlow();

	//////////////////////////////////////////////////////////////////////////
	// Overrides for CGCObjectGroup public interface

	// handles GCObjPlatform
	virtual bool		VHandlesThisTypeId					( GCTypeID idQueryType );

	// must return the typeid of the CGCObjectGroup derived class
	virtual GCTypeID	VGetTypeId							( void );

	virtual void		VOnGroupResourceRelease				( void );	

	// Overridden virtuals from the game object interface
	//////////////////////////////////////////////////////////////////////////

	// Destroy managed platforms
	void DestroyPlatforms( void );
};

#endif
