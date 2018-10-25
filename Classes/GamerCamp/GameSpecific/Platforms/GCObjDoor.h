#ifndef _GCOBJDOOR_H_
#define _GCOBJDOOR_H_
#include "../../GCCocosInterface/GCObjSpritePhysics.h"
//GREEN_KEY (0)
//RED_KEY (1)
//BLUE_KEY (2) 



class CGCObjDoor	:
	public CGCObjSpritePhysics
{
private:
	int m_iDoorNumber;

public:
	CGCObjDoor()
		: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjDoor))
	{}


	CGCObjDoor(int doorNumber)
		: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjDoor))

	{

		m_iDoorNumber = doorNumber;

	}
	virtual void VOnResourceAcquire() override;

	virtual void VOnResourceRelease() override;

	int CGCObjDoor::GetDoorNumber();

	void CGCObjDoor::SetDoorNumber(int);



};

#endif

