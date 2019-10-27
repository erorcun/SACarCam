#include "common.h"
#include "MemoryMgr.h"
#include "Camera.h"
#include "Pad.h"

// These functions (which is commented out as of now) is belong to Fire_Head.

CPad &pad0 = *AddressByVersion<CPad*>(0x6F0360, 0, 0, 0x7DBCB0, 0, 0);
CMouseControllerState &CPad::NewMouseControllerState = *AddressByVersion<CMouseControllerState*>(0x8809F0, 0, 0, 0x94D788, 0, 0);

addr glcAddress = AddressByVersion<addr>(0x4932F0, 0, 0, 0x4AAC30, 0, 0);
addr glpAddress = AddressByVersion<addr>(0x493320, 0, 0, 0x4AAC00, 0, 0);
addr glrAddress = AddressByVersion<addr>(0x4932C0, 0, 0, 0x4AAC60, 0, 0);
addr gllAddress = AddressByVersion<addr>(0x493290, 0, 0, 0x4AAC90, 0, 0);
addr gcglrAddress = AddressByVersion<addr>(0x4930C0, 0, 0, 0x4AAEB0, 0, 0);
addr gcgudAddress = AddressByVersion<addr>(0x493070, 0, 0, 0x4AAF00, 0, 0);
addr gsudAddress = AddressByVersion<addr>(0x492FF0, 0, 0, 0x4AAF50, 0, 0);
addr lalrAddress = AddressByVersion<addr>(0x493F80, 0, 0, 0x4A9A80, 0, 0);
addr laudAddress = AddressByVersion<addr>(0x494130, 0, 0, 0x4A98F0, 0, 0);

WRAPPER bool CPad::GetLookBehindForCar(void) { EAXJMP(glcAddress); }
WRAPPER bool CPad::GetLookBehindForPed(void) { EAXJMP(glpAddress); }
WRAPPER bool CPad::GetLookRight(void) { EAXJMP(glrAddress); }
WRAPPER bool CPad::GetLookLeft(void) { EAXJMP(gllAddress); }
WRAPPER int16 CPad::GetCarGunLeftRight(void) { EAXJMP(gcglrAddress); }
WRAPPER int16 CPad::GetCarGunUpDown(void) { EAXJMP(gcgudAddress); }
WRAPPER int16 CPad::LookAroundLeftRight(void) { EAXJMP(lalrAddress); }
WRAPPER int16 CPad::LookAroundUpDown(void) { EAXJMP(laudAddress); }
WRAPPER int16 CPad::GetSteeringUpDown(void) { EAXJMP(gsudAddress); }

#if 0
int16 CPad::GetSteeringUpDown(void)
{
	if (ArePlayerControlsDisabled())
		return 0;

	switch (Mode)
	{
	case 0:
	case 2:
	{
		int16 axis = NewState.LeftStickY;
		int16 dpad = (NewState.DPadUp - NewState.DPadDown) / 2;

		if (fabs(axis) > fabs(dpad))
			return axis;
		else
			return dpad;

		break;
	}

	case 1:
	case 3:
	{
		return NewState.LeftStickY;

		break;
	}
	}

	return 0;
}

int16 CPad::GetCarGunUpDown(void)
{
	if (ArePlayerControlsDisabled())
		return 0;

	switch (Mode)
	{
	case 0:
	case 1:
	case 2:
	{
		return NewState.RightStickY;

		break;
	}

	case 3:
	{
		return (NewState.DPadUp - NewState.DPadDown) / 2;

		break;
	}
	}

	return 0;
}

int16 CPad::GetCarGunLeftRight(void)
{
	if (ArePlayerControlsDisabled())
		return 0;

	switch (Mode)
	{
	case 0:
	case 1:
	case 2:
	{
		return NewState.RightStickX;

		break;
	}

	case 3:
	{
		return (NewState.DPadRight - NewState.DPadLeft) / 2;

		break;
	}
	}

	return 0;
}

bool CPad::GetLookLeft(void)
{
	if (ArePlayerControlsDisabled())
		return false;

	return !!(NewState.LeftShoulder2 && !NewState.RightShoulder2);
}

bool CPad::GetLookRight(void)
{
	if (ArePlayerControlsDisabled())
		return false;

	return !!(NewState.RightShoulder2 && !NewState.LeftShoulder2);
}


bool CPad::GetLookBehindForCar(void)
{
	if (ArePlayerControlsDisabled())
		return false;

	return !!(NewState.RightShoulder2 && NewState.LeftShoulder2);
}

bool CPad::GetLookBehindForPed(void)
{
	if (ArePlayerControlsDisabled())
		return false;

	return !!NewState.RightShock;
}
#endif