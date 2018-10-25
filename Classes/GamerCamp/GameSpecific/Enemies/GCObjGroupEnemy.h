#ifndef	_GCOBJECTGROUPENEMY_H_
#define	_GCOBJECTGROUPENEMY_H_

#ifndef BOX2D_H
	#include "Box2d/Box2D.h"
#endif

#ifndef	_GCOBJECTGROUP_H_
	#include "GamerCamp/GCObject/GCObjectGroup.h"
#endif


//////////////////////////////////////////////////////////////////////////
// Forward Declarations
class CGCObjSprite;
class CGCObjEnemy;

//////////////////////////////////////////////////////////////////////////
// Responsible for newing, managing, & deleting the enemies
//////////////////////////////////////////////////////////////////////////
class CGCObjGroupEnemy
: public CGCObjectGroup
{
private:

	// Total number of enemies to create
	static const u32 k_uNumEnemies = 3;
	// Array of b2Vec2 for enemy position
	b2Vec2 m_v2EnemyPositionArray[k_uNumEnemies];
	// Array of floats for enemy patrol min/max points
	float m_v2EnemyPatrolMinimumArray[k_uNumEnemies];
	float m_v2EnemyPatrolMaximumArray[k_uNumEnemies];
	// Array of ints for enemy patrol direction
	int m_v2EnemyPatrolDirectionArray[k_uNumEnemies];
	

public:

	// Constructor
	CGCObjGroupEnemy();	

	//////////////////////////////////////////////////////////////////////////
	// We need a virtual destructor since delete will be called on pointers of 
	// this class to delete derived types.
	virtual ~CGCObjGroupEnemy() override;


	//////////////////////////////////////////////////////////////////////////
	// Overrides for CGCObjectGroup public interface

	// Handles GCObjenemy
	virtual bool		VHandlesThisTypeId					( GCTypeID idQueryType ) override;

	// Must return the typeid of the CGCObjectGroup derived class
	virtual GCTypeID	VGetTypeId							( void ) override;
	virtual void		VOnGroupResourceAcquire				( void ) override;
	virtual void		VOnGroupResourceAcquire_PostObject	( void ) override;
	virtual void		VOnGroupResourceRelease				( void ) override;

	// Overridden virtuals from the game object interface
	//////////////////////////////////////////////////////////////////////////

	void	CreateEnemies( void );
	void	DestroyEnemies( void );


	// Getters and Setters
		// Getters
	int GetNumberOfEnemies();
		// Setters
	void SetEnemyPosition( int _iEnemyNumber, b2Vec2 _v2EnemyPosition );
	void SetEnemyPatrol( int _iEnemyNumber, float _fpatrolMinimumPoint, float _fpatrolMaximumPoint, int _iPatrolDirection );
};

#endif // #ifndef _GCOBJECTGROUPENEMY_H_