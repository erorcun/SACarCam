#pragma once

// Many, many thanks to GTAForums community and re3 project, all of these headers are taken from there.

class CPed;
class CAutomobile;

extern int previousMode;

class CVector
{
public:
	float x, y, z;
	CVector(void) {}
	CVector(float x, float y, float z) : x(x), y(y), z(z) {}

	// (0,1,0) means no rotation. So get right vector and its atan
	float Heading(void) const { return atan2f(-x, y); }
	float Magnitude(void) const { return sqrtf(x * x + y * y + z * z); }
	float MagnitudeSqr(void) const { return x * x + y * y + z * z; }
	float Magnitude2D(void) const { return sqrtf(x * x + y * y); }
	float MagnitudeSqr2D(void) const { return x * x + y * y; }
	void Normalise(void) {
		float sq = MagnitudeSqr();
		if (sq > 0.0f) {
			float invsqrt = RecipSqrt(sq);
			x *= invsqrt;
			y *= invsqrt;
			z *= invsqrt;
		}
		else
			x = 1.0f;
	}

	const CVector& operator+=(CVector const& right) {
		x += right.x;
		y += right.y;
		z += right.z;
		return *this;
	}

	const CVector& operator-=(CVector const& right) {
		x -= right.x;
		y -= right.y;
		z -= right.z;
		return *this;
	}

	const CVector& operator*=(float right) {
		x *= right;
		y *= right;
		z *= right;
		return *this;
	}

	const CVector& operator/=(float right) {
		x /= right;
		y /= right;
		z /= right;
		return *this;
	}

	CVector operator-() const {
		return CVector(-x, -y, -z);
	}

	const bool operator==(CVector const& right) {
		return x == right.x && y == right.y && z == right.z;
	}

	bool IsZero(void) { return x == 0.0f && y == 0.0f && z == 0.0f; }
};

inline float
DotProduct(const CVector& v1, const CVector& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline CVector
CrossProduct(const CVector& v1, const CVector& v2)
{
	return CVector(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x);
}

inline CVector operator+(const CVector& left, const CVector& right)
{
	return CVector(left.x + right.x, left.y + right.y, left.z + right.z);
}

inline CVector operator-(const CVector& left, const CVector& right)
{
	return CVector(left.x - right.x, left.y - right.y, left.z - right.z);
}

inline CVector operator*(const CVector& left, float right)
{
	return CVector(left.x * right, left.y * right, left.z * right);
}

inline CVector operator*(float left, const CVector& right)
{
	return CVector(left * right.x, left * right.y, left * right.z);
}

inline CVector operator/(const CVector& left, float right)
{
	return CVector(left.x / right, left.y / right, left.z / right);
}


class CVector2D
{
public:
	float x, y;
	CVector2D(void) {}
	CVector2D(float x, float y) : x(x), y(y) {}
	CVector2D(const CVector& v) : x(v.x), y(v.y) {}
	float Heading(void) const { return atan2f(-x, y); }
	float Magnitude(void) const { return sqrtf(x * x + y * y); }
	float MagnitudeSqr(void) const { return x * x + y * y; }

	void Normalise(void) {
		float sq = MagnitudeSqr();
		if (sq > 0.0f) {
			float invsqrt = RecipSqrt(sq);
			x *= invsqrt;
			y *= invsqrt;
		}
		else
			x = 1.0f;
	}
	const CVector2D& operator+=(CVector2D const& right) {
		x += right.x;
		y += right.y;
		return *this;
	}

	const CVector2D& operator-=(CVector2D const& right) {
		x -= right.x;
		y -= right.y;
		return *this;
	}

	const CVector2D& operator*=(float right) {
		x *= right;
		y *= right;
		return *this;
	}

	const CVector2D& operator/=(float right) {
		x /= right;
		y /= right;
		return *this;
	}
	CVector2D operator-(const CVector2D& rhs) const {
		return CVector2D(x - rhs.x, y - rhs.y);
	}
	CVector2D operator+(const CVector2D& rhs) const {
		return CVector2D(x + rhs.x, y + rhs.y);
	}
	CVector2D operator*(float t) const {
		return CVector2D(x * t, y * t);
	}
	CVector2D operator/(float t) const {
		return CVector2D(x / t, y / t);
	}
};

struct CColSphereIII
{
	CVector center;
	float radius;
	uint8 surface;
	uint8 piece;
};

struct CColSphereVC
{
	CVector center;
	float radius;
};

struct CColBoxIII
{
	CVector min;
	CVector max;
	uint8 surface;
	uint8 piece;
};

struct CColBoxVC
{
	CVector min;
	CVector max;
};

struct CColLine
{
	CVector p0;
	int pad0;
	CVector p1;
	int pad1;
};

struct CColTriangle
{
	uint16 a;
	uint16 b;
	uint16 c;
	uint8 surface;
};

struct CColTrianglePlane
{
	CVector normal;
	float dist;
	uint8 dir;
};

struct CColPoint
{
	CVector point;
	int pad1;
	// the surface normal on the surface of point
	CVector normal;
	int pad2;
	uint8 surfaceA;
	uint8 pieceA;
	uint8 surfaceB;
	uint8 pieceB;
	float depth;
};

class CColModelVC {
public:
	CColSphereVC boundingSphere;
	CColBoxVC boundingBox;
	unsigned short m_nNumSpheres;
	unsigned short m_nNumBoxes;
	unsigned short m_nNumTriangles;
	unsigned char m_nNumLines;
	unsigned char m_nColStoreIndex;
	bool m_bLoaded;
	char _pad31[3];
	CColSphereVC* m_pSpheres;
	void* m_pLines;
	CColBoxVC* m_pBoxes;
	void* m_pVertices;
	void* m_pTriangles;
	void* m_pTrianglePlanes;
};

struct CColModelIII
{
	CColSphereIII boundingSphere;
	CColBoxIII boundingBox;
	short numSpheres;
	short numLines;
	short numBoxes;
	short numTriangles;
	int level;
	bool ownsCollisionVolumes;
	CColSphereIII* spheres;
	CColLine* lines;
	CColBoxIII* boxes;
	CVector* vertices;
	CColTriangle* triangles;
	CColTrianglePlane* trianglePlanes;

	~CColModelIII(void) { };
};

// This is III one, not same in VC but it doesn't matter
class CBaseModelInfo
{
public:
	char         m_name[24];
	void* m_colModel;
	void* m_twodEffects;
	int16        m_objectId;

	uint16       m_refCount;
	int16        m_txdSlot;
	uint8 m_type;
	uint8        m_num2dEffects;
	bool         m_freeCol;

	virtual ~CBaseModelInfo() {};

	void* GetColModel(void) { return m_colModel; }
};

static_assert(sizeof(CBaseModelInfo) == 0x30, "CBaseModelInfo: error");

class CModelInfo
{
	static CBaseModelInfo** ms_modelInfoPtrs;
public:
	static CBaseModelInfo* GetModelInfo(int id) {
		return ms_modelInfoPtrs[id];
	}
};

struct RwMatrix
{
	CVector           right;
	uint32            flags;
	CVector           up;
	uint32            pad1;
	CVector           at;
	uint32            pad2;
	CVector			  pos;
	uint32            pad3;
};

class CMatrix
{
public:
	RwMatrix m_matrix;
	RwMatrix* m_attachment;
	bool m_hasRwMatrix;	// are we the owner?

	CVector& GetPosition(void) { return *(CVector*)& m_matrix.pos; }
	CVector& GetRight(void) { return *(CVector*)& m_matrix.right; }
	CVector& GetForward(void) { return *(CVector*)& m_matrix.up; }
	CVector& GetUp(void) { return *(CVector*)& m_matrix.at; }
};

// Without vtbl. Used for CCameraVC
class CPlaceableVC
{
public:
	CMatrix m_matrix;

	CVector& GetPosition(void) { return m_matrix.GetPosition(); }
	CVector& GetRight(void) { return m_matrix.GetRight(); }
	CVector& GetForward(void) { return m_matrix.GetForward(); }
	CVector& GetUp(void) { return m_matrix.GetUp(); }
	CMatrix& GetMatrix(void) { return m_matrix; }
};

// Interchangable between III-VC, but the child class should also have virtual func. (Which isn't a case in CCameraVC)
class CPlaceable
{
public:
	CMatrix m_matrix;

	virtual ~CPlaceable(void) { };
	CVector& GetPosition(void) { return m_matrix.GetPosition(); }
	CVector& GetRight(void) { return m_matrix.GetRight(); }
	CVector& GetForward(void) { return m_matrix.GetForward(); }
	CVector& GetUp(void) { return m_matrix.GetUp(); }
	CMatrix& GetMatrix(void) { return m_matrix; }
};
static_assert(sizeof(CPlaceable) == 0x4C, "CPlaceable: error");

struct CReference;

enum eEntityType
{
	ENTITY_TYPE_NOTHING = 0,
	ENTITY_TYPE_BUILDING,
	ENTITY_TYPE_VEHICLE,
	ENTITY_TYPE_PED,
	ENTITY_TYPE_OBJECT,
	ENTITY_TYPE_DUMMY,
};

enum eEntityStatus
{
	STATUS_PLAYER,
	STATUS_PLAYER_PLAYBACKFROMBUFFER,
	STATUS_SIMPLE,
	STATUS_PHYSICS,
	STATUS_ABANDONED,
	STATUS_WRECKED,
	STATUS_TRAIN_MOVING,
	STATUS_TRAIN_NOT_MOVING,
	STATUS_HELI,
	STATUS_PLANE,
	STATUS_PLAYER_REMOTE,
	STATUS_PLAYER_DISABLED,
};

// Again, not actually same between III and VC but doesn't matter to us.
class CEntity : public CPlaceable
{
public:
	void* m_rwObject;
	uint32 m_type : 3;
	uint32 m_status : 5;

	// flagsA
	uint32 bUsesCollision : 1;
	uint32 bCollisionProcessed : 1;
	uint32 bIsStatic : 1;
	uint32 bHasContacted : 1;
	uint32 bPedPhysics : 1;
	uint32 bIsStuck : 1;
	uint32 bIsInSafePosition : 1;
	uint32 bUseCollisionRecords : 1;

	// flagsB
	uint32 bWasPostponed : 1;
	uint32 bExplosionProof : 1;
	uint32 bIsVisible : 1;
	uint32 bHasCollided : 1;	//
	uint32 bRenderScorched : 1;
	uint32 bHasBlip : 1;
	uint32 bIsBIGBuilding : 1;
	// VC inserts one more flag here: if drawdist <= 2000
	uint32 bRenderDamaged : 1;

	// flagsC
	uint32 bBulletProof : 1;
	uint32 bFireProof : 1;
	uint32 bCollisionProof : 1;
	uint32 bMeleeProof : 1;
	uint32 bOnlyDamagedByPlayer : 1;
	uint32 bStreamingDontDelete : 1;
	uint32 bZoneCulled : 1;
	uint32 bZoneCulled2 : 1;	// only treadables+10m

	// flagsD
	uint32 bRemoveFromWorld : 1;
	uint32 bHasHitWall : 1;
	uint32 bImBeingRendered : 1;
	uint32 m_flagD8 : 1;	// used by cBuoyancy::ProcessBuoyancy
	uint32 bIsSubway : 1;	// set when subway, but maybe different meaning?
	uint32 bDrawLast : 1;
	uint32 bNoBrightHeadLights : 1;
	uint32 m_flagD80 : 1;	// CObject visibility?

	// flagsE
	uint32 bDistanceFade : 1;
	uint32 m_flagE2 : 1;

	uint16 m_scanCode;
	int16 m_randomSeed;
	int16 m_modelIndex;
	uint16 m_level;	// int16
	CReference* m_pFirstReference;

	void* GetColModel(void) { return CModelInfo::GetModelInfo(m_modelIndex)->GetColModel(); }

	virtual ~CEntity(void) { };

	bool IsBuilding(void) { return m_type == ENTITY_TYPE_BUILDING; }
	bool IsVehicle(void) { return m_type == ENTITY_TYPE_VEHICLE; }
	bool IsPed(void) { return m_type == ENTITY_TYPE_PED; }
	bool IsObject(void) { return m_type == ENTITY_TYPE_OBJECT; }
	bool IsDummy(void) { return m_type == ENTITY_TYPE_DUMMY; }

	int GetModelIndex(void) { return m_modelIndex; }
};
static_assert(sizeof(CEntity) == 0x64, "CEntity: error");

class CPhysicalIII : public CEntity
{
public:
	// The not properly indented fields haven't been checked properly yet

	int32 m_audioEntityId;
	float unk1;
	void* m_treadable[2];	// car and ped
	uint32 m_nLastTimeCollided;
	CVector m_vecMoveSpeed;		// velocity
	CVector m_vecTurnSpeed;		// angular velocity
	CVector m_vecMoveFriction;
	CVector m_vecTurnFriction;
	CVector m_vecMoveSpeedAvg;
	CVector m_vecTurnSpeedAvg;
	float m_fMass;
	float m_fTurnMass;	// moment of inertia
	float fForceMultiplier;
	float m_fAirResistance;
	float m_fElasticity;
	float m_fBuoyancy;
	CVector m_vecCentreOfMass;
	void* m_entryInfoList;
	void* m_movingListNode;

	char field_EC;
	uint8 m_nStaticFrames;
	uint8 m_nCollisionRecords;
	bool field_EF;
	CEntity* m_aCollisionRecords[6];

	float m_fDistanceTravelled;

	// damaged piece
	float m_fDamageImpulse;
	CEntity* m_pDamageEntity;
	CVector m_vecDamageNormal;
	int16 m_nDamagePieceType;

	uint8 bIsHeavy : 1;
	uint8 bAffectedByGravity : 1;
	uint8 bInfiniteMass : 1;
	uint8 bIsInWater : 1;
	uint8 m_phy_flagA10 : 1;
	uint8 m_phy_flagA20 : 1;
	uint8 bHitByTrain : 1;
	uint8 m_phy_flagA80 : 1;

	uint8 m_nSurfaceTouched;
	int8 m_nZoneLevel;

	virtual ~CPhysicalIII(void) { };
};
static_assert(sizeof(CPhysicalIII) == 0x128, "CPhysicalIII: error");

struct CStoredCollPoly
{
	CVector verts[3];
	bool valid;
};

enum eVehicleType {
	VEHICLE_TYPE_CAR,
	VEHICLE_TYPE_BOAT,
	VEHICLE_TYPE_TRAIN,
	VEHICLE_TYPE_HELI,
	VEHICLE_TYPE_PLANE,
	VEHICLE_TYPE_BIKE,
	NUM_VEHICLE_TYPES
};

class CVehicleIII : public CPhysicalIII
{
public:
	// 0x128
	void* pHandling;
	char AutoPilot[112];
	uint8 m_currentColour1;
	uint8 m_currentColour2;
	uint8 m_aExtras[2];
	int16 m_nAlarmState;
	int16 m_nMissionValue;
	CPed* pDriver;
	CPed* pPassengers[8];
	uint8 m_nNumPassengers;
	int8 m_nNumGettingIn;
	int8 m_nGettingInFlags;
	int8 m_nGettingOutFlags;
	uint8 m_nNumMaxPassengers;
	char field_1CD[19];
	CEntity* m_pCurGroundEntity;
	void* m_pCarFire;
	float m_fSteerAngle;
	float m_fGasPedal;
	float m_fBrakePedal;
	uint8 VehicleCreatedBy;

	// cf. https://github.com/DK22Pac/plugin-sdk/blob/master/plugin_sa/game_sa/CVehicle.h from R*
	uint8 bIsLawEnforcer : 1; // Is this guy chasing the player at the moment
	uint8 bIsAmbulanceOnDuty : 1; // Ambulance trying to get to an accident
	uint8 bIsFireTruckOnDuty : 1; // Firetruck trying to get to a fire
	uint8 bIsLocked : 1; // Is this guy locked by the script (cannot be removed)
	uint8 bEngineOn : 1; // For sound purposes. Parked cars have their engines switched off (so do destroyed cars)
	uint8 bIsHandbrakeOn : 1; // How's the handbrake doing ?
	uint8 bLightsOn : 1; // Are the lights switched on ?
	uint8 bFreebies : 1; // Any freebies left in this vehicle ?

	uint8 bIsVan : 1; // Is this vehicle a van (doors at back of vehicle)
	uint8 bIsBus : 1; // Is this vehicle a bus
	uint8 bIsBig : 1; // Is this vehicle a bus
	uint8 bLowVehicle : 1; // Need this for sporty type cars to use low getting-in/out anims
	uint8 bComedyControls : 1; // Will make the car hard to control (hopefully in a funny way)
	uint8 bWarnedPeds : 1; // Has scan and warn peds of danger been processed?
	uint8 bCraneMessageDone : 1; // A crane message has been printed for this car allready
	uint8 bExtendedRange : 1; // This vehicle needs to be a bit further away to get deleted

	uint8 bTakeLessDamage : 1; // This vehicle is stronger (takes about 1/4 of damage)
	uint8 bIsDamaged : 1; // This vehicle has been damaged and is displaying all its components
	uint8 bHasBeenOwnedByPlayer : 1;// To work out whether stealing it is a crime
	uint8 bFadeOut : 1; // Fade vehicle out
	uint8 bIsBeingCarJacked : 1; // Fade vehicle out
	uint8 bCreateRoadBlockPeds : 1; // If this vehicle gets close enough we will create peds (coppers or gang members) round it
	uint8 bCanBeDamaged : 1; // Set to FALSE during cut scenes to avoid explosions
	uint8 bUsingSpecialColModel : 1;// Is player vehicle using special collision model, stored in player strucure

	uint8 bOccupantsHaveBeenGenerated : 1;  // Is true if the occupants have already been generated. (Shouldn't happen again)
	uint8 bGunSwitchedOff : 1;  // Level designers can use this to switch off guns on boats
	uint8 bVehicleColProcessed : 1;// Has ProcessEntityCollision been processed for this car?
	uint8 bIsCarParkVehicle : 1; // Car has been created using the special CAR_PARK script command
	uint8 bHasAlreadyBeenRecorded : 1; // Used for replays

	int8 m_numPedsUseItAsCover;
	uint8 m_nAmmoInClip;    // Used to make the guns on boat do a reload (20 by default)
	int8 m_nPacManPickupsCarried;
	uint8 m_nRoadblockType;
	int16 m_nRoadblockNode;
	float m_fHealth;           // 1000.0f = full health. 250.0f = fire. 0 -> explode
	uint8 m_nCurrentGear;
	int8 field_205[3];
	float m_fChangeGearTime;
	uint32 m_nGunFiringTime;    // last time when gun on vehicle was fired (used on boats)
	uint32 m_nTimeOfDeath;
	uint16 m_nTimeBlocked;
	int16 m_nBombTimer;        // goes down with each frame
	CEntity* m_pBlowUpEntity;
	float m_fMapObjectHeightAhead;	// front Z?
	float m_fMapObjectHeightBehind;	// rear Z?
	uint32 m_nDoorLock;
	int8 m_nLastWeaponDamage; // see eWeaponType, -1 if no damage
	int8 m_nRadioStation;
	uint8 m_bRainAudioCounter;
	uint8 m_bRainSamplesCounter;
	uint8 m_nCarHornTimer;
	int8 field_22D;
	bool m_bSirenOrAlarm;
	int8 m_comedyControlState;
	CStoredCollPoly m_aCollPolys[2];     // poly which is under front/rear part of car
	float m_fSteerRatio;
	eVehicleType m_vehType;

	bool IsCar(void) { return m_vehType == VEHICLE_TYPE_CAR; }
	bool IsBoat(void) { return m_vehType == VEHICLE_TYPE_BOAT; }
	bool IsBike(void) { return m_vehType == VEHICLE_TYPE_BIKE; }

	CBaseModelInfo* GetModelInfo() { return CModelInfo::GetModelInfo(GetModelIndex()); }
};
static_assert(sizeof(CVehicleIII) == 0x288, "CVehicleIII: error");

struct CPhysicalVC : CEntity
{
	unsigned int m_audioEntityId;
	float fUnknownX;
	float fUnknownY;
	CVector m_vecMoveSpeed;
	CVector m_vecTurnSpeed;
	CVector m_vecFrictionMoveForce;
	CVector m_vecFrictionTurnForce;
	CVector m_vecForce;
	CVector m_vecTorque;
	float m_fMass;
	float m_fTurnMass;
	float m_fVelocityFrequency;
	float m_fAirResistance;
	float m_fElasticity;
	float m_fBuoyancyConstant;
	CVector m_vecCentreOfMass;
	void* m_collisionList;
	void* m_pMovingListNode;
	unsigned char uCollideExtra;
	unsigned char uCollideInfo;
	unsigned char m_nNumCollisionRecords;
	char field_E7;
	CEntity* m_apCollisionRecords[6];
	float m_fTotSpeed;
	float m_fCollisionPower;
	CPhysicalVC* m_pPhysColliding;
	CVector m_vecCollisionPower;
	unsigned short m_wComponentCol;
	unsigned char m_nMoveFlags;
	unsigned char m_nCollFlags;
	unsigned char m_nLastCollType;
	unsigned char m_nZoneLevel;
	char field_11E[2];
};

struct CVehicleVC : CPhysicalVC
{
	void* pHandling;
	void* m_pFlyingHandling;
	char m_autoPilot[116];
	CVehicleVC* m_pVehicleToRam;
	unsigned char m_nPrimaryColor;
	unsigned char m_nSecondaryColor;
	char m_anExtras[2];
	short m_wWantedStarsOnEnter;
	short m_wMissionValue;
	CPed* pDriver;
	CPed* pPassengers[8];
	unsigned char m_nNumPassengers;
	unsigned char m_nNumGettingIn;
	unsigned char m_nGettingInFlags;
	unsigned char m_nGettingOutFlags;
	unsigned char m_nMaxPassengers;
	char __f01CD[3];
	int field_1D4;
	CVector field_1D8;
	CEntity* m_pEntityWeAreOn;
	void* m_pFire;
	float m_fSteerAngle;
	float m_fGasPedal;
	float m_fBreakPedal;
	unsigned char m_nCreatedBy; // see eVehicleCreatedBy
	char flagsAndSoOn[9];
	unsigned char m_nAmmoInClip;
	char field_201;
	float m_fHealth;
	unsigned char m_nCurrentGear;
	char __f0205[3];
	int field_20C;
	int field_210;
	int m_nTimeTillWeNeedThisCar;
	int field_218;
	int m_nTimeOfDeath;
	short field_220;
	short m_wBombTimer; // goes down with each frame
	int field_224;
	int field_228;
	int field_22C;
	unsigned int m_nLockStatus;
	unsigned char m_nLastWeaponDamage;
	char __f0231[3];
	CEntity* pLastDamEntity;
	unsigned char m_nRadioStation;
	char field_23D;
	char field_23E;
	unsigned int m_bHornEnabled;
	char field_244;
	unsigned char m_nSirenOrAlarm;
	unsigned char m_nSirenExtra;
	char field_247;
	CStoredCollPoly m_aCollPolys[2];     // poly which is under front/rear part of car
	float m_fSteerRatio;
	eVehicleType m_vehType;

	bool IsCar(void) { return m_vehType == VEHICLE_TYPE_CAR; }
	bool IsBoat(void) { return m_vehType == VEHICLE_TYPE_BOAT; }
	bool IsBike(void) { return m_vehType == VEHICLE_TYPE_BIKE; }

	CBaseModelInfo* GetModelInfo() { return CModelInfo::GetModelInfo(GetModelIndex()); }
};

typedef struct RsGlobalType RsGlobalType;
struct RsGlobalType
{
	const char* appName;
	int32 width;
	int32 height;
	int32 maximumWidth;
	int32 maximumHeight;
	int32 maxFPS;
	bool  quit;

	void* ps; /* platform specific data */

	// Removed the fields we don't use
};

class CWorldIII
{
public:
	static bool ProcessLineOfSight(const CVector& point1, const CVector& point2, CColPoint& point, CEntity*& entity, bool checkBuildings, bool checkVehicles, bool checkPeds, bool checkObjects, bool checkDummies, bool ignoreSeeThrough, bool ignoreSomeObjects = false);
	static CEntity* TestSphereAgainstWorld(CVector centre, float distance, CEntity* entityToIgnore, bool checkBuildings, bool checkVehicles, bool checkPeds, bool checkObjects, bool checkDummies, bool ignoreSomeObjects);
};

class CWorldVC
{
public:
	static bool ProcessLineOfSight(const CVector& point1, const CVector& point2, CColPoint& point, CEntity*& entity, bool checkBuildings, bool checkVehicles, bool checkPeds, bool checkObjects, bool checkDummies, bool ignoreSeeThrough, bool ignoreSomeObjects = false, bool ignoreShootThrough = true);
	static CEntity* TestSphereAgainstWorld(CVector centre, float distance, CEntity* entityToIgnore, bool checkBuildings, bool checkVehicles, bool checkPeds, bool checkObjects, bool checkDummies, bool ignoreSomeObjects);
};

class CWaterLevel
{
public:
	static bool GetWaterLevelNoWaves(float fX, float fY, float fZ, float* pfOutLevel);
};

class cDMAudio
{
public:
	void PlayOneShot(int32 audioEntity, uint16 oneShot, float volume);
};
extern cDMAudio &DMAudio;