#ifndef	_GCOBJECTGROUPITEM_H_
#define	_GCOBJECTGROUPITEM_H_

#ifndef BOX2D_H
#include "Box2d/Box2D.h"
#endif

#ifndef	_GCOBJECTGROUP_H_
#include "GamerCamp/GCObject/GCObjectGroup.h"
#endif


//////////////////////////////////////////////////////////////////////////
// Forward Declarations
class CGCObjSprite;
class CGCObjItem;


class CGCObjGroupItem
	: public CGCObjectGroup
{

private:


public:

	// Constructor
	CGCObjGroupItem();

	//////////////////////////////////////////////////////////////////////////
	// We need a virtual destructor since delete will be called on pointers of 
	// this class to delete derived types.
	virtual ~CGCObjGroupItem();


	//////////////////////////////////////////////////////////////////////////
	// Overrides for CGCObjectGroup public interface

	// Handles GCObjItem
	virtual bool		VHandlesThisTypeId( GCTypeID idQueryType );

	// Must return the typeid of the CGCObjectGroup derived class
	virtual GCTypeID	VGetTypeId( void );

	virtual void		VOnGroupResourceAcquire_PostObject( void );

	virtual void		VOnGroupResourceRelease( void );

	// Overridden virtuals from the game object interface
	//////////////////////////////////////////////////////////////////////////

	// Destroy managed items
	void DestroyItems( void );
};

#endif // #ifndef _GCOBJECTGROUPITEM_H_