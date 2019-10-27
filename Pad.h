#pragma once

class CControllerState
{
public:
	int16 LeftStickX, LeftStickY;
	int16 RightStickX, RightStickY;
	int16 LeftShoulder1, LeftShoulder2;
	int16 RightShoulder1, RightShoulder2;
	int16 DPadUp, DPadDown, DPadLeft, DPadRight;
	int16 Start, Select;
	int16 Square, Triangle, Cross, Circle;
	int16 LeftShock, RightShock;
	int16 NetworkTalk;
	float GetLeftStickX(void) { return LeftStickX / 32767.0f; };
	float GetLeftStickY(void) { return LeftStickY / 32767.0f; };
	float GetRightStickX(void) { return RightStickX / 32767.0f; };
	float GetRightStickY(void) { return RightStickY / 32767.0f; };
};

class CMouseControllerState
{
public:
	//uint32 btns;	// bit 0-2 button 1-3

	bool LMB;
	bool RMB;
	bool MMB;
	bool WHEELUP;
	bool WHEELDN;
	bool MXB1;
	bool MXB2;
	char _pad0;

	float x, y;
};

class CPad
{
public:
	/*
		// III
		CControllerState NewState;
		CControllerState OldState;
		CControllerState PCTempKeyState;
		CControllerState PCTempJoyState;
		CControllerState PCTempMouseState;
		int16 Phase;
		int16 Mode;
		int16 ShakeDur;
		uint8 ShakeFreq;
		bool bHornHistory[5];
		uint8 iCurrHornHistory;
		uint8 DisablePlayerControls;
		int8 bApplyBrakes;
		char _unk[12]; //int32 unk[3];
		char _pad0[3];
		int32 LastTimeTouched;
		int32 AverageWeapon;
		int32 AverageEntries;
	*/
	static CMouseControllerState& NewMouseControllerState;

	int16 LookAroundUpDown(void);
	int16 LookAroundLeftRight(void);
	int16 GetSteeringUpDown(void);
	int16 GetCarGunUpDown(void);
	int16 GetCarGunLeftRight(void);
	bool GetLookLeft(void);
	bool GetLookRight(void);
	bool GetLookBehindForCar(void);
	bool GetLookBehindForPed(void);

	int16 FakeCarGunUpDown(void);
	int16 FakeCarGunLeftRight(void);
};
//static_assert(sizeof(CPad) == 0xFC, "CPad: this is broken as shit");

extern CPad& pad0;