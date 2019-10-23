#include "common.h"
#include "MemoryMgr.h"
#include "GTA.h"
#include "Camera.h"
#include "Pad.h"

HMODULE dllModule, hDummyHandle;
int gtaversion = -1;

#define MI_III_YARDIE 135
#define MI_III_RCBANDIT 131
#define MI_III_DODO 126
#define MI_III_RHINO 122
#define MI_III_FIRETRUCK 97

#define MI_VC_VOODOO 142
#define MI_VC_FIRETRUCK 137
#define MI_VC_RHINO 162
#define MI_VC_RCBANDIT 171
#define MI_VC_RCBARON 194
#define MI_VC_RCRAIDER 195
#define MI_VC_RCGOBLIN 231

#define Tank (isIII() ? MI_III_RHINO : MI_VC_RHINO)
#define FireTruk (isIII() ? MI_III_FIRETRUCK : MI_VC_FIRETRUCK)
#define CarWithHydraulics (isIII() ? MI_III_YARDIE : MI_VC_VOODOO)

#define DefaultFOV 70.0f

// Car zoom modes per veh. types doesn't exist in III, so we will emulate it :)
// We're just injecting the values for VC
float CarZoomModes[] = {
	-1.8f, -0.2f, -3.2f, 0.05f, -3.21f, // near
	0.2f, 1.4f, 0.65f, 1.9f, 5.69f, // mid
	5.2f, 6.0f, 15.9f, 15.9f, 14.2f // far
};

/*
// Original SA array, but doesn't give same results as SA (Due to collision bounding box sizes?)
// LCS has different mid/far values, near is same
float CarZoomModes[] = {
	-1.0f, -0.2f, -3.2f, 0.05f, -2.41f, // near
	1.0f, 1.4f, 0.65f, 1.9f, 6.49f, // mid
	6.0f, 6.0f, 15.9f, 15.9f, 15.0f // far
};
*/

void(*&RwCamera) = *AddressByVersion<void**>(0x72676C, 0, 0, 0x8100BC, 0, 0);

CCameraIII *TheCameraIII = (CCameraIII*)0x6FACF8;
CCameraVC *TheCameraVC = (CCameraVC*)0x7E4688;

// Actually static member of CCamera
bool& m_bUseMouse3rdPerson = *AddressByVersion<bool*>(0x5F03D8, 0, 0, 0xA10B4C, 0, 0);

cDMAudio &DMAudio = *AddressByVersion<cDMAudio*>(0x95CDBE, 0, 0, 0xA10B8A, 0, 0);

// These are static members of CWorld
CColPoint& ms_testSpherePoint = *AddressByVersion<CColPoint*>(0x6E64C0, 0, 0, 0x7D18C0, 0, 0);
CEntity*& pIgnoreEntity = *AddressByVersion<CEntity **>(0x8F6494, 0, 0, 0x9B6E58, 0, 0);

addr plosAddress = AddressByVersion<addr>(0x4AF970, 0, 0, 0x4D92D0, 0, 0);
WRAPPER bool CWorldIII::ProcessLineOfSight(const CVector& point1, const CVector& point2, CColPoint& point, CEntity*& entity, bool checkBuildings, bool checkVehicles, bool checkPeds, bool checkObjects, bool checkDummies, bool ignoreSeeThrough, bool ignoreSomeObjects) { EAXJMP(plosAddress); }
WRAPPER bool CWorldVC::ProcessLineOfSight(const CVector& point1, const CVector& point2, CColPoint& point, CEntity*& entity, bool checkBuildings, bool checkVehicles, bool checkPeds, bool checkObjects, bool checkDummies, bool ignoreSeeThrough, bool ignoreSomeObjects, bool sth) { EAXJMP(plosAddress); }

addr tsawAddress = AddressByVersion<addr>(0x4B4710, 0, 0, 0x4D3F40, 0, 0);
WRAPPER CEntity* CWorldIII::TestSphereAgainstWorld(CVector centre, float distance, CEntity* entityToIgnore, bool checkBuildings, bool checkVehicles, bool checkPeds, bool checkObjects, bool checkDummies, bool ignoreSomeObjects) { EAXJMP(tsawAddress); }
WRAPPER CEntity* CWorldVC::TestSphereAgainstWorld(CVector centre, float distance, CEntity* entityToIgnore, bool checkBuildings, bool checkVehicles, bool checkPeds, bool checkObjects, bool checkDummies, bool ignoreSomeObjects) { EAXJMP(tsawAddress); }

addr gwlnwAddress = AddressByVersion<addr>(0x555440, 0, 0, 0x5C2BE0, 0, 0);
WRAPPER bool CWaterLevel::GetWaterLevelNoWaves(float fX, float fY, float fZ, float* pfOutLevel) { EAXJMP(gwlnwAddress); }

addr rcsncpAddress = AddressByVersion<addr>(0x5A5070, 0, 0, 0x64A860, 0, 0);
WRAPPER void* RwCameraSetNearClipPlane(void* camera, float nearClip) { EAXJMP(rcsncpAddress); }

addr posAddress = AddressByVersion<addr>(0x57C840, 0, 0, 0x5F9DA0, 0, 0);
WRAPPER void cDMAudio::PlayOneShot(int32 audioEntity, uint16 oneShot, float volume) { EAXJMP(posAddress); }

CBaseModelInfo** CModelInfo::ms_modelInfoPtrs = AddressByVersion<CBaseModelInfo **>(0x83D408, 0, 0, 0x92D4C8, 0, 0);

// Actually static member of CVehicle
bool &m_bDisableMouseSteering = *AddressByVersion<bool*>(0x60252C, 0, 0, 0x69C610, 0, 0);

uint32 &m_snTimeInMilliseconds = *AddressByVersion<uint32*>(0x885B48, 0, 0, 0x974B2C, 0, 0);
float &ms_fTimeStep = *AddressByVersion<float*>(0x8E2CB4, 0, 0, 0x975424, 0, 0);

#define GetDisablePlayerControls(pad) *((uint8*)((addr)pad + (isIII() ? 0xDF : 0xF0)))
#define GetHandlingFlags(veh) *((uint32*)((addr)veh->pHandling + (isIII() ? 0xC8 : 0xCC)))
#define GetWheelsOnGround(veh) *((uint8*)((addr)veh + (isIII() ? 0x591 : 0x5C4))) // In CAutomobile
#define GetWheelsOnGroundBike(veh) *((uint8*)((addr)veh + 0x4DC)) // In CBike, VC
#define GetDoomAnglePtrLR(veh) (float*)((addr)veh + (isIII() ? 0x580 : 0x5B0)) // In CAutomobile
#define GetDoomAnglePtrUD(veh) (float*)((addr)veh + (isIII() ? 0x584 : 0x5B4)) // In CAutomobile
#define GetPedObjective(ped) *((uint32*)((addr)ped + (isIII() ? 0x164 : 0x160)))
#define GetNearPlane() *(float*)((addr)RwCamera + 0x80)

// Virtual func. in GTA
#define GetHeightAboveRoad(veh, classToCast) (veh->IsCar() ? *((float*)((addr)veh + (isIII() ? 0x50C : 0x530))) : \
															-1.0f * ((classToCast*)veh->GetColModel())->boundingBox.min.z)

float aspectRatio;
bool gotTheAR = false;
float GetAspectRatio() {
	if (isVC())
		return *(float*)0x94DD38; // CDraw::ms_fAspectRatio
	else {
		if (!gotTheAR) {
			gotTheAR = true;

			RsGlobalType& RsGlobal = *(RsGlobalType*)0x8F4360;
			return aspectRatio = ((float)RsGlobal.width) / ((float)RsGlobal.height);
		} else
			return aspectRatio;
	}
}

float GetATanOfXY(float x, float y) {
	if (x == 0.0f && y == 0.0f)
		return 0.0f;

	float xabs = fabsf(x);
	float yabs = fabsf(y);

	if (xabs < yabs) {
		if (y > 0.0f) {
			if (x > 0.0f)
				return 0.5f * PI - atan2f(x / y, 1.0f);
			else
				return 0.5f * PI + atan2f(-x / y, 1.0f);
		}
		else {
			if (x > 0.0f)
				return 1.5f * PI + atan2f(x / -y, 1.0f);
			else
				return 1.5f * PI - atan2f(-x / -y, 1.0f);
		}
	}
	else {
		if (y > 0.0f) {
			if (x > 0.0f)
				return atan2f(y / x, 1.0f);
			else
				return PI - atan2f(y / -x, 1.0f);
		}
		else {
			if (x > 0.0f)
				return 2.0f * PI - atan2f(-y / x, 1.0f);
			else
				return PI + atan2f(-y / -x, 1.0f);
		}
	}
}

const CVector
Multiply3x3(const CMatrix& mat, const CVector& vec)
{
	return CVector(
		mat.m_matrix.right.x * vec.x + mat.m_matrix.up.x * vec.y + mat.m_matrix.at.x * vec.z,
		mat.m_matrix.right.y * vec.x + mat.m_matrix.up.y * vec.y + mat.m_matrix.at.y * vec.z,
		mat.m_matrix.right.z * vec.x + mat.m_matrix.up.z * vec.y + mat.m_matrix.at.z * vec.z);
}

const CVector
Multiply3x3(const CVector& vec, const CMatrix& mat)
{
	return CVector(
		mat.m_matrix.right.x * vec.x + mat.m_matrix.right.y * vec.y + mat.m_matrix.right.z * vec.z,
		mat.m_matrix.up.x * vec.x + mat.m_matrix.up.y * vec.y + mat.m_matrix.up.z * vec.z,
		mat.m_matrix.at.x * vec.x + mat.m_matrix.at.y * vec.y + mat.m_matrix.at.z * vec.z);
}

int previousMode = 0;

template<class CamClass, class CameraClass, class VehicleClass, class WorldClass, class ColModelClass>
void
Process_FollowCar_SA(const CVector& CameraTarget, float TargetOrientation, CamClass* cam, CameraClass* TheCamera) // bool sthForScript)
{
	// Reminder: SA vehicle subclass 3 is heli, 4 is plane, class 9 is bike

	// Missing things on III CCam
	static CVector m_aTargetHistoryPosOne;
	static CVector m_aTargetHistoryPosTwo;
	static CVector m_aTargetHistoryPosThree;
	static int m_nCurrentHistoryPoints = 0;
	static float lastBeta = -9999.0f;
	static float lastAlpha = -9999.0f;
	static float stepsLeftToChangeBetaByMouse;
	static float flt_9BF250;
	static bool alphaCorrected;
	static float heightIncreaseMult;

	if (!cam->CamTargetEntity->IsVehicle())
		return;

	VehicleClass* car = (VehicleClass*)cam->CamTargetEntity;
	CVector TargetCoors = CameraTarget;
	uint8 camSetArrPos = 0;

	// For compatibility with III with Aircraft mod and VC
	bool isPlane = isIII() && car->m_modelIndex == MI_III_DODO || GetHandlingFlags(car) & 0x40000;
	bool isHeli = GetHandlingFlags(car) & 0x20000;
	bool isBike = (GetHandlingFlags(car) & 0x10000) || car->IsBike();
	bool isCar = car->IsCar() && !isPlane && !isHeli && !isBike;

	CPad* pad = &pad0;

	// Next direction is 0x8CC384 in SA, non-existent in III
	uint8 nextDirectionIsForward = !(pad->GetLookBehindForCar() || pad->GetLookBehindForPed() || pad->GetLookLeft() || pad->GetLookRight()) &&
		cam->DirectionWasLooking == LOOKING_FORWARD;

	if ((isIII() && car->m_modelIndex == MI_III_RCBANDIT) ||
		(isVC() && (car->m_modelIndex == MI_VC_RCBARON || car->m_modelIndex == MI_VC_RCBANDIT))) {
		camSetArrPos = 5;
	}
	else if (isVC() && (car->m_modelIndex == MI_VC_RCRAIDER || car->m_modelIndex == MI_VC_RCGOBLIN)) {
		camSetArrPos = 6;
	}
	else if (car->IsBoat()) {
		camSetArrPos = 4;
	}
	else if (isBike) {
		camSetArrPos = 1;
	}
	else if (isPlane) {
		camSetArrPos = 3;
	}
	else if (isHeli) {
		camSetArrPos = 2;
	}

	// Seems same with LCS, at least the ones I've looked.
	float CARCAM_SET[][15] = {
		{1.3f, 1.0f, 0.4f, 10.0f, 15.0f, 0.5f, 1.0f, 1.0f, 0.85f, 0.2f, 0.075f, 0.05f, 0.8f, 0.785398f, 1.5533431f},
		{1.1f, 1.0f, 0.1f, 10.0f, 11.0f, 0.5f, 1.0f, 1.0f, 0.85f, 0.2f, 0.075f, 0.05f, 0.75f, 0.78539819f, 1.5533431f},
		{1.1f, 1.0f, 0.2f, 10.0f, 15.0f, 0.05f, 0.05f, 0.0f, 0.9f, 0.05f, 0.01f, 0.05f, 1.0f, 0.17453294f, 1.2217305f},
		{1.1f, 3.5f, 0.2f, 10.0f, 25.0f, 0.5f, 1.0f, 1.0f, 0.75f, 0.1f, 0.005f, 0.2f, 1.0f, 1.5533431f, 1.5533431f},
		{1.3f, 1.0f, 0.4f, 10.0f, 15.0f, 0.5f, 1.0f, 0.0f, 0.9f, 0.05f, 0.005f, 0.05f, 1.0f, 0.34906587f, 1.2217305f},
		{1.1f, 1.0f, 0.2f, 10.0f, 5.0f, 0.5f, 1.0f, 1.0f, 0.75f, 0.1f, 0.005f, 0.2f, 1.0f, 0.78539819f, 1.5533431f}, // rc cars
		{1.1f, 1.0f, 0.2f, 10.0f, 5.0f, 0.5f, 1.0f, 1.0f, 0.75f, 0.1f, 0.005f, 0.2f, 1.0f, 0.34906587f, 1.2217305f} // rc heli/planes
	};

	// First value of the array below is 0.12f in LCS (camera is higher in near mode)
	float ZmOneAlphaOffset[] = { 0.08f, 0.08f, 0.15f, 0.08f, 0.08f };
	float ZmTwoAlphaOffset[] = { 0.07f, 0.08f, 0.3f, 0.08f, 0.08f };
	float ZmThreeAlphaOffset[] = { 0.055f, 0.05f, 0.15f, 0.06f, 0.08f };

	// RC Heli/planes use same alpha values with heli/planes
	uint8 alphaArrPos = (camSetArrPos > 4 ? (isPlane ? 3 : (isHeli ? 2 : 0)) : camSetArrPos);
	float zoomModeAlphaOffset = 0.0f;

	if (isHeli && car->m_status == STATUS_PLAYER_REMOTE)
		zoomModeAlphaOffset = ZmTwoAlphaOffset[alphaArrPos];
	else {
		switch ((int)TheCamera->CarZoomIndicator) {
			// near
			case 1:
				zoomModeAlphaOffset = ZmOneAlphaOffset[alphaArrPos];
				break;
			// mid
			case 2:
				zoomModeAlphaOffset = ZmTwoAlphaOffset[alphaArrPos];
				break;
			// far
			case 3:
				zoomModeAlphaOffset = ZmThreeAlphaOffset[alphaArrPos];
				break;
			default:
				break;
		}
	}

	ColModelClass* carCol = (ColModelClass*)car->GetColModel();
	float colMaxZ = carCol->boundingBox.max.z;  // As opposed to LCS and SA, VC does this: carCol->boundingBox.max.z - carCol->boundingBox.min.z;

	float approxCarLength = 2.0f * fabsf(carCol->boundingBox.min.y);

	if (!isBike) {
		heightIncreaseMult = 0.0f;
	} else {
		// Increase colMaxZ slowly when there is a passenger on bike
		if (car->pPassengers[0])
		{
			if (heightIncreaseMult < 1.0f) {
				heightIncreaseMult = min(1.0f, ms_fTimeStep * 0.02f + heightIncreaseMult);
			}
		}
		else
		{
			if (heightIncreaseMult > 0.0f) {
				heightIncreaseMult = max(0.0f, heightIncreaseMult - ms_fTimeStep * 0.02f);
			}
		}
		colMaxZ += 0.4f * heightIncreaseMult;
	}

	float hackedZoomValue = TheCamera->CarZoomValueSmooth;

	// Emulate the zoom values per veh. type in III!
	// Original values: 3.9 - far, 1.9 - mid, 0.05 - near
	// Reminder: We don't have near to far transitions, only far to near.
	if (isIII()) {
		if (TheCamera->CarZoomIndicator == 3.0f)
			hackedZoomValue = CarZoomModes[alphaArrPos + 2 * 5];
		else if (TheCamera->CarZoomIndicator == 2.0f) {
			hackedZoomValue = (CarZoomModes[alphaArrPos + 1 * 5]) +
							  (hackedZoomValue - 1.9f) * (CarZoomModes[alphaArrPos + 2 * 5] - CarZoomModes[alphaArrPos + 1 * 5]) / (3.9f - 1.9f);
		} else if (TheCamera->CarZoomIndicator == 1.0f) {
			hackedZoomValue = (CarZoomModes[alphaArrPos + 0 * 5]) +
				(hackedZoomValue - 0.05f) * (CarZoomModes[alphaArrPos + 1 * 5] - CarZoomModes[alphaArrPos + 0 * 5]) / (1.9f - 0.05f);
		}

		// Had to put this condition to prevent zooming even more in tunnels etc.
		if (hackedZoomValue < CarZoomModes[alphaArrPos])
			hackedZoomValue = CarZoomModes[alphaArrPos];
	}

	float newDistance = hackedZoomValue + CARCAM_SET[camSetArrPos][1] + approxCarLength;

	// Taken from VC CCam::Cam_On_A_String_Unobscured. If we don't this, we will end up seeing the world from the inside of RC Goblin/Raider.
	// I couldn't find where SA does that. It's possible that they've increased the size of these veh.'s collision bounding box.
	if (isVC()) {
		if (car->m_modelIndex != MI_VC_RCRAIDER && car->m_modelIndex != MI_VC_RCGOBLIN) {
			if (car->m_modelIndex == MI_VC_RCBARON)
				newDistance += 9.5f;
		} else
			newDistance += 6.0f;
	}

	float minDistForThisCar = approxCarLength * CARCAM_SET[camSetArrPos][3];

	if (!isHeli || car->m_status == STATUS_PLAYER_REMOTE) {
		float radiusToStayOutside = colMaxZ * CARCAM_SET[camSetArrPos][0] - CARCAM_SET[camSetArrPos][2];
		if (radiusToStayOutside > 0.0f) {
			TargetCoors.z += radiusToStayOutside;
			newDistance += radiusToStayOutside;
			zoomModeAlphaOffset += 0.3f / newDistance * radiusToStayOutside;
		}
	}
	else {
		// 0.6f = fTestShiftHeliCamTarget
		TargetCoors.x += 0.6f * car->GetUp().x * colMaxZ;
		TargetCoors.y += 0.6f * car->GetUp().y * colMaxZ;
		TargetCoors.z += 0.6f * car->GetUp().z * colMaxZ;
	}

	// SA sets CurrentTweakAngle to this value for RCGOBLIN. VC also adds 0.2f to it
	if (isVC() && car->m_modelIndex == MI_VC_RCGOBLIN)
		zoomModeAlphaOffset += 0.178997f;

	float minDistForVehType = CARCAM_SET[camSetArrPos][4];
	if (TheCamera->CarZoomIndicator == 1.0f && (camSetArrPos < 2)) {
		minDistForVehType = minDistForVehType * 0.65f;
	}

	float nextDistance = max(newDistance, minDistForVehType);

	cam->CA_MAX_DISTANCE = newDistance;
	cam->CA_MIN_DISTANCE = 3.5f;

	if (cam->ResetStatics) {
		cam->FOV = DefaultFOV;

		if (isIII()) {
			// GTA 3 has this in veh. camera
			if (TheCamera->m_bIdleOn)
				TheCamera->m_uiTimeWeEnteredIdle = m_snTimeInMilliseconds;
		}
	}
	else {
		if (isCar || isBike) {
			// 0.4f: CAR_FOV_START_SPEED
			if (DotProduct(car->GetForward(), car->m_vecMoveSpeed) > 0.4f)
				cam->FOV = (DotProduct(car->GetForward(), car->m_vecMoveSpeed) - 0.4f) * ms_fTimeStep + cam->FOV;
		}
		if (cam->FOV > DefaultFOV)
			// 0.98f: CAR_FOV_FADE_MULT
			cam->FOV = pow(0.98f, ms_fTimeStep) * (cam->FOV - DefaultFOV) + DefaultFOV;

		if (cam->FOV <= 100.0)
		{
			if (cam->FOV < DefaultFOV)
				cam->FOV = DefaultFOV;
		}
		else
			cam->FOV = 100.0;
	}

	// WORKAROUND: I still don't know how looking behind works (m_bCamDirectlyInFront is unused in III, they seem to use m_bUseTransitionBeta)
	if (pad->GetLookBehindForCar())
		if (cam->DirectionWasLooking == LOOKING_FORWARD || !cam->LookingBehind)
			TheCamera->m_bCamDirectlyInFront = true;

	// Taken from RotCamIfInFrontCar, because we don't call it anymore
	if (!(pad->GetLookBehindForCar() || pad->GetLookBehindForPed() || pad->GetLookLeft() || pad->GetLookRight()))
		if (cam->DirectionWasLooking != LOOKING_FORWARD)
			TheCamera->m_bCamDirectlyBehind = true;

	// Called when we just entered the car, just started to look behind or returned back from looking left, right or behind
	if (cam->ResetStatics || TheCamera->m_bCamDirectlyBehind || TheCamera->m_bCamDirectlyInFront) {
		cam->ResetStatics = false;
		cam->Rotating = false;
		cam->m_bCollisionChecksOn = true;
		// TheCamera.m_bResetOldMatrix = 1;
		if (!TheCamera->m_bJustCameOutOfGarage) // && !sthForScript)
		{
			cam->Alpha = 0.0f;
			cam->Beta = car->GetForward().Heading() - HALFPI;
			if (TheCamera->m_bCamDirectlyInFront) {
				cam->Beta += PI;
			}
		}
		cam->BetaSpeed = 0.0;
		cam->AlphaSpeed = 0.0;
		cam->Distance = 1000.0;

		cam->Front.x = -(cos(cam->Beta) * cos(cam->Alpha));
		cam->Front.y = -(sin(cam->Beta) * cos(cam->Alpha));
		cam->Front.z = sin(cam->Alpha);

		m_aTargetHistoryPosOne = TargetCoors - nextDistance * cam->Front;

		m_aTargetHistoryPosTwo = TargetCoors - newDistance * cam->Front;

		m_nCurrentHistoryPoints = 0;
		if (!TheCamera->m_bJustCameOutOfGarage) // && !sthForScript)
			cam->Alpha = -zoomModeAlphaOffset;
	}

	cam->Front = TargetCoors - m_aTargetHistoryPosOne;
	cam->Front.Normalise();

	// Code that makes cam follow the car
	float camRightHeading = cam->Front.Heading() - HALFPI;
	if (camRightHeading < -PI)
		camRightHeading = camRightHeading + TWOPI;

	float velocityRightHeading;
	if (car->m_vecMoveSpeed.Magnitude2D() <= 0.02f)
		velocityRightHeading = camRightHeading;
	else
		velocityRightHeading = car->m_vecMoveSpeed.Heading() - HALFPI;

	if (velocityRightHeading < camRightHeading - PI)
		velocityRightHeading = velocityRightHeading + TWOPI;
	else if (velocityRightHeading > camRightHeading + PI)
		velocityRightHeading = velocityRightHeading - TWOPI;

	float v70 = ms_fTimeStep * CARCAM_SET[camSetArrPos][10];
	float v153 = ms_fTimeStep * CARCAM_SET[camSetArrPos][11];

	float a6f = (car->m_vecMoveSpeed - DotProduct(car->m_vecMoveSpeed, cam->Front) * cam->Front).Magnitude();

	float v76 = min(1.0f, v70 * a6f) * (velocityRightHeading - camRightHeading);
	if (v76 <= v153)
	{
		if (v76 < -v153)
			v76 = -v153;
	}
	else
	{
		v76 = v153;
	}
	float targetBeta = camRightHeading + v76;

	if (targetBeta < cam->Beta - HALFPI)
		targetBeta += TWOPI;
	else if (targetBeta > cam->Beta + PI)
		targetBeta -= TWOPI;

	float carPosChange = (TargetCoors - m_aTargetHistoryPosTwo).Magnitude();
	if (carPosChange < newDistance && newDistance > minDistForThisCar) {
		newDistance = max(minDistForThisCar, carPosChange);
	}
	float maxAlphaAllowed = CARCAM_SET[camSetArrPos][13];

	// Originally this is to prevent camera enter into car while we're standing, but what about moving???
	// This is also original LCS and SA bug, or some attempt to fix lag. We'll never know

	// Fix camera enter into car by removing "standing" check
//	if (car->m_vecMoveSpeed.MagnitudeSqr() < 0.04f)
//	{

	if (!isBike) { // SA also checks for whether bike's four wheels are on the ground 
		if (!isHeli && (!isPlane || GetWheelsOnGround(car))) {
			CVector out = CrossProduct(car->GetForward(), CVector(0.0f, 0.0f, 1.0f));
			out.Normalise();
			CVector v173 = CrossProduct(out, car->GetForward());
			v173.Normalise();
			float v83 = DotProduct(v173, cam->Front);
			if (v83 > 0.0)
			{
				float v88 = asinf(fabsf(sinf(cam->Beta - (car->GetForward().Heading() - HALFPI))));
				float v200;
				if (v88 <= atan2f(carCol->boundingBox.max.x, -carCol->boundingBox.min.y))
				{
					v200 = (1.5f - carCol->boundingBox.min.y) / cosf(v88);
				}
				else
				{
					float a6g = 1.2f + carCol->boundingBox.max.x;
					v200 = a6g / cos(max(0.0f, HALFPI - v88));
				}
				maxAlphaAllowed = cos(cam->Beta - (car->GetForward().Heading() - HALFPI)) * atan2f(car->GetForward().z, car->GetForward().Magnitude2D())
					+ atan2f(TargetCoors.z - car->GetPosition().z + GetHeightAboveRoad(car, ColModelClass), v200 * 1.2f);
				if (isCar && GetWheelsOnGround(car) > 1
					&& fabsf(DotProduct(car->m_vecTurnSpeed, car->GetForward())) < 0.05f)
				{
					maxAlphaAllowed += cosf(cam->Beta - (car->GetForward().Heading() - HALFPI) + HALFPI) * atan2f(car->GetRight().z, car->GetRight().Magnitude2D());
				}
			}
		}
	}
	//	}
	float targetAlpha = asinf(clamp(cam->Front.z, -1.0f, 1.0f)) - zoomModeAlphaOffset;
	if (targetAlpha <= maxAlphaAllowed)
	{
		if (targetAlpha < -CARCAM_SET[camSetArrPos][14])
			targetAlpha = -CARCAM_SET[camSetArrPos][14];
	}
	else
	{
		targetAlpha = maxAlphaAllowed;
	}
	float maxAlphaBlendAmount = ms_fTimeStep * CARCAM_SET[camSetArrPos][6];
	float targetAlphaBlendAmount = (1.0f - pow(CARCAM_SET[camSetArrPos][5], ms_fTimeStep)) * (targetAlpha - cam->Alpha);
	if (targetAlphaBlendAmount <= maxAlphaBlendAmount)
	{
		if (targetAlphaBlendAmount < -maxAlphaBlendAmount)
			targetAlphaBlendAmount = -maxAlphaBlendAmount;
	}
	else
	{
		targetAlphaBlendAmount = maxAlphaBlendAmount;
	}

	// Right stick
	float stickX = -(pad->GetCarGunLeftRight());
	float stickY = pad->GetCarGunUpDown();

	// Why??
	if (m_bUseMouse3rdPerson)
		stickY = 0.0f;

	float v103 = cam->FOV * 0.0125f;

	float xMovement = fabsf(stickX) * (v103 * 0.071428575) * stickX * 0.007f * 0.007f;
	float yMovement = fabsf(stickY) * (v103 * 0.042857144) * stickY * 0.007f * 0.007f;

	bool correctAlpha = true;
	//	if (SA checks if we aren't in work car, why?) {
			if (!isCar || car->m_modelIndex != CarWithHydraulics) {
				correctAlpha = false;
			} else {
				xMovement = 0.0f;
				yMovement = 0.0f;
			}
	//	} else
	//		yMovement = 0.0;

	if (!nextDirectionIsForward) {
		yMovement = 0.0;
		xMovement = 0.0;
	}

	if (camSetArrPos == 0) {
		// This is not working on cars as SA
		// Because III/VC doesn't have any buttons tied to LeftStick if you're not in Classic Configuration, using Dodo or using GInput/Pad, so :shrug:
		if (fabsf(pad->GetSteeringUpDown()) > 120.0f) {

			// OBJECTIVE_LEAVE_VEHICLE
			if (car->pDriver && GetPedObjective(car->pDriver) != (isIII() ? 13 : 16)) {
				yMovement += fabsf(pad->GetSteeringUpDown()) * (cam->FOV * 0.0125 * 0.042857144) * pad->GetSteeringUpDown() * 0.007f * 0.007f * 0.5;
			}
		}
	}

	if (yMovement > 0.0)
		yMovement = yMovement * 0.5;

	bool mouseChangesBeta = false;
	if (m_bUseMouse3rdPerson && !GetDisablePlayerControls(pad))
	{
		float mouseY = CPad::NewMouseControllerState.y * 2.0f;
		float mouseX = CPad::NewMouseControllerState.x * -2.0f;

		// If you want an ability to toggle free cam while steering with mouse, you can add an OR after DisableMouseSteering.
		// There was a pad->NewState.m_bVehicleMouseLook in SA, which doesn't exists in III.

		if ((mouseX != 0.0 || mouseY != 0.0) && (m_bDisableMouseSteering))
		{
			float v113 = cam->FOV * 0.0125;
			yMovement = mouseY * v113 * 0.0025f; // TheCamera.m_fMouseAccelHorzntl?, I think there is no TheCamera.m_fMouseAccelVertical in SA
			cam->BetaSpeed = 0.0;
			cam->AlphaSpeed = 0.0;
			xMovement = mouseX * v113 * 0.0025f; // TheCamera.m_fMouseAccelHorzntl;
			targetAlpha = cam->Alpha;
			stepsLeftToChangeBetaByMouse = 1.0f * 50.0f;
			mouseChangesBeta = true;
		}
		else if (stepsLeftToChangeBetaByMouse > 0.0f)
		{
			// Finish rotation by decreasing speed when we stopped moving mouse
			cam->BetaSpeed = 0.0;
			cam->AlphaSpeed = 0.0;
			yMovement = 0.0;
			xMovement = 0.0;
			targetAlpha = cam->Alpha;
			stepsLeftToChangeBetaByMouse = max(0.0f, stepsLeftToChangeBetaByMouse - ms_fTimeStep);
			mouseChangesBeta = true;
		}
	}

	/*
	// That doesn't exist on LCS, and I didn't put much thought into it
	if (car->pPassengers[0] && car->pPassengers[0]->m_objective == OBJECTIVE_SOLICIT)
	{
		if (BYTE1(CTaskManager::GetActiveTask(&car->__parent.__parent.m_apPassengers[0]->intelligence->m_taskManager)[5].m_pParentTask) & 1)
		{
			if (maxAlphaAllowed - flt_8CCEC8 <= v7->Alpha)
				yMovement = 0.0;
			else
				yMovement = ms_fTimeStep * flt_8CCEC4;
		}
	} */
	if (correctAlpha) {
		if (previousMode != MODE_CAMONASTRING)
			alphaCorrected = false;

		if (!alphaCorrected && fabsf(zoomModeAlphaOffset + cam->Alpha) > 0.05f) {
			yMovement = (-zoomModeAlphaOffset - cam->Alpha) * 0.05f;
		} else
			alphaCorrected = true;
	}
	float alphaSpeedFromStickY = yMovement * CARCAM_SET[camSetArrPos][12];
	float betaSpeedFromStickX = xMovement * CARCAM_SET[camSetArrPos][12];
	float v117 = CARCAM_SET[camSetArrPos][9];
	float angleChangeStep = pow(CARCAM_SET[camSetArrPos][8], ms_fTimeStep);
	float targetBetaWithStickBlendAmount = betaSpeedFromStickX + (targetBeta - cam->Beta) / max(ms_fTimeStep, 1.0f);

	if (targetBetaWithStickBlendAmount < -v117)
		targetBetaWithStickBlendAmount = -v117;
	else if (targetBetaWithStickBlendAmount > v117)
		targetBetaWithStickBlendAmount = v117;

	float angleChangeStepLeft = 1.0 - angleChangeStep;
	cam->BetaSpeed = targetBetaWithStickBlendAmount * angleChangeStepLeft + angleChangeStep * cam->BetaSpeed;
	if (fabsf(cam->BetaSpeed) < 0.0001f)
		cam->BetaSpeed = 0.0;

	float v121;
	if (mouseChangesBeta)
		v121 = betaSpeedFromStickX;
	else
		v121 = ms_fTimeStep * cam->BetaSpeed;
	cam->Beta = v121 + cam->Beta;

	if (TheCamera->m_bJustCameOutOfGarage)
		cam->Beta = GetATanOfXY(cam->Front.x, cam->Front.y) + PI;

	if (cam->Beta < -PI)
		cam->Beta += TWOPI;
	else if (cam->Beta > PI)
		cam->Beta -= TWOPI;

	if (camSetArrPos <= 1 && targetAlpha < cam->Alpha && carPosChange >= newDistance) {
		if (isCar && GetWheelsOnGround(car) > 1 ||
			isBike && GetWheelsOnGroundBike(car)  > 1)
				alphaSpeedFromStickY += (targetAlpha - cam->Alpha) * 0.075f;
	}

	cam->AlphaSpeed = angleChangeStepLeft * alphaSpeedFromStickY + angleChangeStep * cam->AlphaSpeed;
	float maxAlphaSpeed = v117;
	if (alphaSpeedFromStickY > 0.0f)
		maxAlphaSpeed = maxAlphaSpeed * 0.5;

	if (cam->AlphaSpeed <= maxAlphaSpeed)
	{
		float minAlphaSpeed = -maxAlphaSpeed;
		if (cam->AlphaSpeed < minAlphaSpeed)
			cam->AlphaSpeed = minAlphaSpeed;
	}
	else
	{
		cam->AlphaSpeed = maxAlphaSpeed;
	}

	if (fabsf(cam->AlphaSpeed) < 0.0001f)
		cam->AlphaSpeed = 0.0f;

	float alphaWithSpeedAccounted;
	if (mouseChangesBeta)
	{
		alphaWithSpeedAccounted = alphaSpeedFromStickY + targetAlpha;
		cam->Alpha += alphaSpeedFromStickY;
	}
	else
	{
		alphaWithSpeedAccounted = ms_fTimeStep * cam->AlphaSpeed + targetAlpha;
		cam->Alpha += targetAlphaBlendAmount;
	}

	if (cam->Alpha <= maxAlphaAllowed)
	{
		float minAlphaAllowed = -CARCAM_SET[camSetArrPos][14];
		if (minAlphaAllowed > cam->Alpha)
		{
			cam->Alpha = minAlphaAllowed;
			cam->AlphaSpeed = 0.0;
		}
	}
	else
	{
		cam->Alpha = maxAlphaAllowed;
		cam->AlphaSpeed = 0.0;
	}

	// Prevent unsignificant angle changes
	if (fabsf(lastAlpha - cam->Alpha) < 0.0001f)
		cam->Alpha = lastAlpha;

	lastAlpha = cam->Alpha;

	if (fabsf(lastBeta - cam->Beta) < 0.0001f)
		cam->Beta = lastBeta;

	lastBeta = cam->Beta;

	cam->Front.x = -(cos(cam->Beta) * cos(cam->Alpha));
	cam->Front.y = -(sin(cam->Beta) * cos(cam->Alpha));
	cam->Front.z = sin(cam->Alpha);
	cam->GetVectorsReadyForRW();
	TheCamera->m_bCamDirectlyBehind = false;
	TheCamera->m_bCamDirectlyInFront = false;

	cam->Source = TargetCoors - newDistance * cam->Front;


	cam->m_cvecTargetCoorsForFudgeInter = TargetCoors;
	m_aTargetHistoryPosThree = m_aTargetHistoryPosOne;
	float v140 = alphaWithSpeedAccounted + zoomModeAlphaOffset;
	float v144 = -(cos(cam->Beta) * cos(v140));
	float v145 = -(sin(cam->Beta) * cos(v140));
	float v146 = sin(v140);

	m_aTargetHistoryPosOne.x = TargetCoors.x - v144 * nextDistance;
	m_aTargetHistoryPosOne.y = TargetCoors.y - v145 * nextDistance;
	m_aTargetHistoryPosOne.z = TargetCoors.z - v146 * nextDistance;

	m_aTargetHistoryPosTwo.x = TargetCoors.x - v144 * newDistance;
	m_aTargetHistoryPosTwo.y = TargetCoors.y - v145 * newDistance;
	m_aTargetHistoryPosTwo.z = TargetCoors.z - v146 * newDistance;

	// SA calls SetColVarsVehicle in here
	if (nextDirectionIsForward) {
		/* SA
			CWorld::pIgnoreEntity = car;
			TheCamera.m_nExtraEntitiesCount = 0;
			TheCamera.sub_50CDE0(car);
			CCamera::CameraColDetAndReact(Source, TargetCoors);
			TheCamera->ImproveNearClip(car, 0, Source, TargetCoors);
			CWorld::pIgnoreEntity = 0;
		*/
		// Instead of above code (because it's rather impossible) I will use LCS FollowCar and SA FollowPedWithMouse col. detection


		// Move cam if there are collisions
		float v206 = powf(0.99, ms_fTimeStep);
		flt_9BF250 = (v206 * flt_9BF250) + ((1.0f - v206) * car->m_vecMoveSpeed.Magnitude());

		CColPoint foundCol;
		CEntity* foundEnt;
		pIgnoreEntity = cam->CamTargetEntity;
		if (WorldClass::ProcessLineOfSight(TargetCoors, cam->Source, foundCol, foundEnt, true, flt_9BF250 < 0.1f, false, true, false, true, false))
		{
			float obstacleTargetDist = (TargetCoors - foundCol.point).Magnitude();
			float obstacleCamDist = newDistance - obstacleTargetDist;
			if (!foundEnt->IsPed() || obstacleCamDist <= 1.0f)
			{
				cam->Source = foundCol.point;
				if (obstacleTargetDist < 1.2f)
				{
					RwCameraSetNearClipPlane(RwCamera, max(0.05f, obstacleTargetDist - 0.3f));
				}
			}
			else
			{
				if (!WorldClass::ProcessLineOfSight(foundCol.point, cam->Source, foundCol, foundEnt, true, flt_9BF250 < 0.1f, false, true, false, true, false))
				{
					float lessClip = obstacleCamDist - 0.35f;
					if (lessClip <= 0.9f)
						RwCameraSetNearClipPlane(RwCamera, lessClip);
					else
						RwCameraSetNearClipPlane(RwCamera, 0.9f);
				}
				else {
					obstacleTargetDist = (TargetCoors - foundCol.point).Magnitude();
					cam->Source = foundCol.point;
					if (obstacleTargetDist < 1.2f)
					{
						float lessClip = obstacleTargetDist - 0.3f;
						if (lessClip >= 0.05f)
							RwCameraSetNearClipPlane(RwCamera, lessClip);
						else
							RwCameraSetNearClipPlane(RwCamera, 0.05f);
					}
				}
			}
		}
		pIgnoreEntity = nil;
		float nearClip = GetNearPlane();
		float radius = tanf(cam->FOV * 0.017453292f * 0.5f) * GetAspectRatio() * 1.1f;

		// If we're seeing blue hell due to camera intersects some surface, fix it.
		// SA and LCS have this unrolled.
		for (int i = 0;
			i <= 5 && WorldClass::TestSphereAgainstWorld((nearClip * cam->Front) + cam->Source, radius * nearClip, nil, true, true, false, true, false, false);
			i++) {

			CVector surfaceCamDist = ms_testSpherePoint.point - cam->Source;
			CVector v52 = DotProduct(surfaceCamDist, cam->Front) * cam->Front;
			float v86 = (surfaceCamDist - v52).Magnitude() / radius;

			if (v86 > nearClip)
				v86 = nearClip;
			if (v86 < 0.1)
				v86 = 0.1;
			if (nearClip > v86)
				RwCameraSetNearClipPlane(RwCamera, v86);

			if (v86 == 0.1f)
				cam->Source += (TargetCoors - cam->Source) * 0.3f;

			nearClip = GetNearPlane();
			radius = tanf(cam->FOV * 0.017453292f * 0.5f) * GetAspectRatio() * 1.1f;
		}
	}
	TheCamera->m_bCamDirectlyBehind = false;
	TheCamera->m_bCamDirectlyInFront = false;

	// ------- LCS specific part starts

	if (camSetArrPos == 5 && cam->Source.z < 1.0f) // RC Bandit and Baron
		cam->Source.z = 1.0f;

	// Obviously some specific place in LC
	if (isIII()) {
		if (cam->Source.x > 11.0f && cam->Source.x < 91.0f) {
			if (cam->Source.y > -680.0f && cam->Source.y < -600.0f && cam->Source.z < 24.4f)
				cam->Source.z = 24.4f;
		}
	}
	// CCam::FixSourceAboveWaterLevel
	if (CameraTarget.z >= -2.0f) {
		float level = -6000.0;
		// +0.5f is needed for III
		if (CWaterLevel::GetWaterLevelNoWaves(cam->Source.x, cam->Source.y, cam->Source.z, &level)) {
			if (cam->Source.z < level + 0.5f)
				cam->Source.z = level + 0.5f;
		}
	}

	// -------- LCS specific part ends

	cam->Front = TargetCoors - cam->Source;
	cam->GetVectorsReadyForRW();

	// SA code from CAutomobile::TankControl/FireTruckControl.
	if (car->m_modelIndex == Tank || car->m_modelIndex == FireTruk) {
		CVector hi = Multiply3x3(cam->Front, car->GetMatrix());

		// III/VC's firetruck turret angle is reversed
		float angleToFace = (car->m_modelIndex == FireTruk ? -hi.Heading() : hi.Heading());

		if (angleToFace <= *GetDoomAnglePtrLR(car) + PI) {
			if (angleToFace < *GetDoomAnglePtrLR(car) - PI)
				angleToFace = angleToFace + TWOPI;
		} else {
			angleToFace = angleToFace - TWOPI;
		}

		float neededTurn = angleToFace - *GetDoomAnglePtrLR(car);
		float turnPerFrame = ms_fTimeStep * (car->m_modelIndex == FireTruk ? 0.05f : 0.015f);
		if (neededTurn <= turnPerFrame) {
			if (neededTurn < -turnPerFrame)
				angleToFace = *GetDoomAnglePtrLR(car) - turnPerFrame;
		} else {
			angleToFace = turnPerFrame + *GetDoomAnglePtrLR(car);
		}

		if (car->m_modelIndex == Tank && *GetDoomAnglePtrLR(car) != angleToFace) {
			DMAudio.PlayOneShot(car->m_audioEntityId, (isIII() ? 26 : 28), fabsf(angleToFace - *GetDoomAnglePtrLR(car)));
		}
		*GetDoomAnglePtrLR(car) = angleToFace;

		if (*GetDoomAnglePtrLR(car) < -PI) {
			*GetDoomAnglePtrLR(car) += TWOPI;
		} else if (*GetDoomAnglePtrLR(car) > PI) {
			*GetDoomAnglePtrLR(car) -= TWOPI;
		}

		// Because tank turret has no Y movement
		if (car->m_modelIndex == FireTruk) {
			float alphaToFace = atan2f(hi.z, hi.Magnitude2D()) + 0.2617994f;
			float neededAlphaTurn = alphaToFace - *GetDoomAnglePtrUD(car);
			float alphaTurnPerFrame = ms_fTimeStep * 0.02f;

			if (neededAlphaTurn > alphaTurnPerFrame) {
				neededTurn = alphaTurnPerFrame;
				*GetDoomAnglePtrUD(car) = neededTurn + *GetDoomAnglePtrUD(car);
			} else {
				if (neededAlphaTurn >= -alphaTurnPerFrame) {
					*GetDoomAnglePtrUD(car) = alphaToFace;
				} else {
					*GetDoomAnglePtrUD(car) = *GetDoomAnglePtrUD(car) - alphaTurnPerFrame;
				}
			}

			float turretMinY = -0.34906587f;
			float turretMaxY = 0.34906587f;
			if (turretMinY <= *GetDoomAnglePtrUD(car)) {
				if (*GetDoomAnglePtrUD(car) > turretMaxY)
					*GetDoomAnglePtrUD(car) = turretMaxY;
			} else {
				*GetDoomAnglePtrUD(car) = turretMinY;
			}
		}
	}

	previousMode = cam->Mode;
	// SA
	// gTargetCoordsForLookingBehind = TargetCoors;
}

void
CCamVC::GetVectorsReadyForRW(void)
{
	CVector right;
	Up = CVector(0.0f, 0.0f, 1.0f);
	Front.Normalise();
	if (Front.x == 0.0f && Front.y == 0.0f) {
		Front.x = 0.0001f;
		Front.y = 0.0001f;
	}
	right = CrossProduct(Front, Up);
	right.Normalise();
	Up = CrossProduct(right, Front);
}

void
CCamIII::GetVectorsReadyForRW(void)
{
	CVector right;
	Up = CVector(0.0f, 0.0f, 1.0f);
	Front.Normalise();
	if (Front.x == 0.0f && Front.y == 0.0f) {
		Front.x = 0.0001f;
		Front.y = 0.0001f;
	}
	right = CrossProduct(Front, Up);
	right.Normalise();
	Up = CrossProduct(right, Front);
}

#define currentMode (isIII() ? TheCameraIII->Cams[TheCameraIII->ActiveCam].Mode : TheCameraVC->Cams[TheCameraVC->ActiveCam].Mode)

// Needed for storing previous mode for some unknown alpha angle effect.
// Credits goes to The Hero - aap for reversing it (comments are belong to him)
void
WellBufferMe(float Target, float* CurrentValue, float* CurrentSpeed, float MaxSpeed, float Acceleration, bool IsAngle)
{
	// MaxSpeed is a limit of how fast the value is allowed to change. 1.0 = to Target in up to 1ms
	// Acceleration is how fast the speed will change to MaxSpeed. 1.0 = to MaxSpeed in 1ms

	float Delta = Target - *CurrentValue;

	if (IsAngle) {
		while (Delta >= PI) Delta -= 2 * PI;
		while (Delta < -PI) Delta += 2 * PI;
	}

	float TargetSpeed = Delta * MaxSpeed;
	*CurrentSpeed += Acceleration * (TargetSpeed - *CurrentSpeed) * ms_fTimeStep;

	// Clamp speed if we overshot
	if (TargetSpeed < 0.0f && *CurrentSpeed < TargetSpeed)
		* CurrentSpeed = TargetSpeed;
	else if (TargetSpeed > 0.0f && *CurrentSpeed > TargetSpeed)
		* CurrentSpeed = TargetSpeed;

	*CurrentValue += *CurrentSpeed * min(10.0f, ms_fTimeStep);

	previousMode = currentMode;
}

int16
CPad::FakeCarGunLeftRight(void)
{
	if (currentMode == MODE_CAMONASTRING)
		return 0;
	else
		return pad0.GetCarGunLeftRight();
}

int16
CPad::FakeCarGunUpDown(void)
{
	if (currentMode == MODE_CAMONASTRING)
		return 0;
	else
		return pad0.GetCarGunUpDown();
}
#undef currentMode

void
CCamIII::Process_FollowCar_SA_III(const CVector &CameraTarget, float TargetOrientation, float, float)
{
	Process_FollowCar_SA<CCamIII, CCameraIII, CVehicleIII, CWorldIII, CColModelIII>(CameraTarget, TargetOrientation, this, TheCameraIII);
}

void
CCamVC::Process_FollowCar_SA_VC(const CVector &CameraTarget, float TargetOrientation, float, float)
{
	Process_FollowCar_SA<CCamVC, CCameraVC, CVehicleVC, CWorldVC, CColModelVC>(CameraTarget, TargetOrientation, this, TheCameraVC);
}

BOOL WINAPI
DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH) {
		dllModule = hInst;

		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR)& DllMain, &hDummyHandle);

		// III
		if (*(DWORD*)0x5C1E70 == 0x53E58955) {
			InjectHook(0x456F40, &WellBufferMe, PATCH_JUMP);
			InjectHook(0x459A54, &CCamIII::Process_FollowCar_SA_III, PATCH_NOTHING);
			InjectHook(0x459B36, &CCamIII::Process_FollowCar_SA_III, PATCH_NOTHING);

			// To block original rhino-firetruck turret movement
			InjectHook(0x52260E, &CPad::FakeCarGunUpDown, PATCH_NOTHING);
			InjectHook(0x53D628, &CPad::FakeCarGunLeftRight, PATCH_NOTHING);
			InjectHook(0x5225D2, &CPad::FakeCarGunLeftRight, PATCH_NOTHING);

		// VC
		} else if (*(DWORD*)0x667BF5 == 0xB85548EC) {

			InjectHook(0x4864E3, &WellBufferMe, PATCH_JUMP);
			InjectHook(0x483B3B, &CCamVC::Process_FollowCar_SA_VC, PATCH_NOTHING);
			InjectHook(0x483B79, &CCamVC::Process_FollowCar_SA_VC, PATCH_NOTHING);
			InjectHook(0x483C3C, &CCamVC::Process_FollowCar_SA_VC, PATCH_NOTHING);

			// To block original rhino-firetruck turret movement
			InjectHook(0x57ABAE, &CPad::FakeCarGunUpDown, PATCH_NOTHING);
			InjectHook(0x57AB72, &CPad::FakeCarGunLeftRight, PATCH_NOTHING);
			InjectHook(0x5865B8, &CPad::FakeCarGunLeftRight, PATCH_NOTHING);

			// Patch zoom modes
			// Only for VC atm., III doesn't have zoom values per veh. types
			for (int i = 0; i < sizeof(CarZoomModes) / sizeof(CarZoomModes[0]); i++) {
				addr a = 0x68AB70 + i*sizeof(CarZoomModes[0]);
				Patch(a, CarZoomModes[i]);
			}
		}
		else return FALSE;
	}
	return TRUE;
}
