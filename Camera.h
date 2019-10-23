#pragma once
#include "GTA.h"

// Many, many thanks to GTAForums community and re3 project, all of these headers are taken from there.

#define NUMBER_OF_VECTORS_FOR_AVERAGE 2

enum
{
	MODE_TOPDOWN1 = 1,
	MODE_TOPDOWN2,
	MODE_BEHINDCAR,
	MODE_FOLLOWPED,
	MODE_AIMING,
	MODE_DEBUG,
	MODE_SNIPER,
	MODE_ROCKET,
	MODE_MODELVIEW,
	MODE_BILL,
	MODE_SYPHON,
	MODE_CIRCLE,
	MODE_CHEESYZOOM,
	MODE_WHEELCAM,
	MODE_FIXED,
	MODE_FIRSTPERSON,
	MODE_FLYBY,
	MODE_CAMONASTRING,
	MODE_REACTIONCAM,
	MODE_FOLLOWPEDWITHBINDING,
	MODE_CHRISWITHBINDINGPLUSROTATION,
	MODE_BEHINDBOAT,
	MODE_PLAYERFALLENWATER,
	MODE_CAMONTRAINROOF,
	MODE_CAMRUNNINGSIDETRAIN,
	MODE_BLOODONTHETRACKS,
	MODE_IMTHEPASSENGERWOOWOO,
	MODE_SYPHONCRIMINFRONT,
	MODE_PEDSDEADBABY,
	MODE_CUSHYPILLOWSARSE,
	MODE_LOOKATCARS,
	MODE_ARRESTCAMONE,
	MODE_ARRESTCAMTWO,
	MODE_M16FIRSTPERSON_34,
	MODE_SPECIALFIXEDFORSYPHON,
	MODE_FIGHT,
	MODE_TOPDOWNPED,
	MODE_SNIPER_RUN_AROUND,
	MODE_ROCKET_RUN_AROUND,
	MODE_FIRSTPERSONPEDONPC_40,
	MODE_FIRSTPERSONPEDONPC_41,
	MODE_FIRSTPERSONPEDONPC_42,
	MODE_EDITOR,
	MODE_M16FIRSTPERSON_44
};

class CCamVC {
public:
	bool    bBelowMinDist; //used for follow ped mode
	bool    bBehindPlayerDesired; //used for follow ped mode
	bool    m_bCamLookingAtVector;
	bool    m_bCollisionChecksOn;
	bool    m_bFixingBeta; //used for camera on a string
	bool    m_bTheHeightFixerVehicleIsATrain;
	bool    LookBehindCamWasInFront;
	bool    LookingBehind;
	bool    LookingLeft; // 32
	bool    LookingRight;
	bool    ResetStatics; //for interpolation type stuff to work
	bool    Rotating;

	short   Mode;                   // CameraMode
	unsigned int  m_uiFinishTime; // 52

	int     m_iDoCollisionChecksOnFrameNum;
	int     m_iDoCollisionCheckEveryNumOfFrames;
	int     m_iFrameNumWereAt;  // 64
	int     m_iRunningVectorArrayPos;
	int     m_iRunningVectorCounter;
	int     DirectionWasLooking;

	float   f_max_role_angle; //=DEGTORAD(5.0f);    
	float   f_Roll; //used for adding a slight roll to the camera in the
	float   f_rollSpeed; //camera on a string mode
	float   m_fSyphonModeTargetZOffSet;
	float   m_fAmountFractionObscured;
	float   m_fAlphaSpeedOverOneFrame; // 100
	float   m_fBetaSpeedOverOneFrame;
	float   m_fBufferedTargetBeta;
	float   m_fBufferedTargetOrientation;
	float   m_fBufferedTargetOrientationSpeed;
	float   m_fCamBufferedHeight;
	float   m_fCamBufferedHeightSpeed;
	float   m_fCloseInPedHeightOffset;
	float   m_fCloseInPedHeightOffsetSpeed; // 132
	float   m_fCloseInCarHeightOffset;
	float   m_fCloseInCarHeightOffsetSpeed;
	float   m_fDimensionOfHighestNearCar;
	float   m_fDistanceBeforeChanges;
	float   m_fFovSpeedOverOneFrame;
	float   m_fMinDistAwayFromCamWhenInterPolating;
	float   m_fPedBetweenCameraHeightOffset;
	float   m_fPlayerInFrontSyphonAngleOffSet; // 164
	float   m_fRadiusForDead;
	float   m_fRealGroundDist; //used for follow ped mode
	float   m_fTargetBeta;
	float   m_fTimeElapsedFloat;
	float   m_fTilt;
	float   m_fTiltSpeed;

	float   m_fTransitionBeta;
	float   m_fTrueBeta;
	float   m_fTrueAlpha; // 200
	float   m_fInitialPlayerOrientation; //used for first person

	float   Alpha;
	float   AlphaSpeed;
	float   FOV;
	float   FOVSpeed;
	float   Beta;
	float   BetaSpeed;
	float   Distance; // 232
	float   DistanceSpeed;
	float   CA_MIN_DISTANCE;
	float   CA_MAX_DISTANCE;
	float   SpeedVar;

	// ped onfoot zoom distance
	float m_fTargetZoomGroundOne;
	float m_fTargetZoomGroundTwo; // 256
	float m_fTargetZoomGroundThree;
	// ped onfoot alpha angle offset
	float m_fTargetZoomOneZExtra;
	float m_fTargetZoomTwoZExtra;
	float m_fTargetZoomThreeZExtra;

	float m_fTargetZoomZCloseIn;
	float m_fMinRealGroundDist;
	float m_fTargetCloseInDist;

	CVector m_cvecSourceSpeedOverOneFrame; // 324
	CVector m_cvecTargetSpeedOverOneFrame; // 336
	CVector m_cvecUpOverOneFrame; // 348

	CVector m_cvecTargetCoorsForFudgeInter; // 360
	CVector m_cvecCamFixedModeVector; // 372
	CVector m_cvecCamFixedModeSource; // 384
	CVector m_cvecCamFixedModeUpOffSet; // 396
	CVector m_vecLastAboveWaterCamPosition; //408  //helper for when the player has gone under the water

	CVector m_vecBufferedPlayerBodyOffset; // 420

	// The three vectors that determine this camera for this frame
	CVector Front;  // 432                                              // Direction of looking in
	CVector Source;                                                 // Coors in world space
	CVector SourceBeforeLookBehind;
	CVector Up;                                                     // Just that
	CVector m_arrPreviousVectors[NUMBER_OF_VECTORS_FOR_AVERAGE];    // used to average stuff
	CEntity* CamTargetEntity;

	float       m_fCameraDistance;
	float       m_fIdealAlpha;
	float       m_fPlayerVelocity;
	CAutomobile* m_pLastCarEntered; // So interpolation works
	CPed* m_pLastPedLookedAt;// So interpolation works 
	bool        m_bFirstPersonRunAboutActive;

	void GetVectorsReadyForRW(void);
	void Process_FollowCar_SA_VC(const CVector&, float, float, float);
};

class CCamIII
{
public:
	bool    bBelowMinDist; //used for follow ped mode
	bool    bBehindPlayerDesired; //used for follow ped mode
	bool    m_bCamLookingAtVector;
	bool    m_bCollisionChecksOn;
	bool    m_bFixingBeta; //used for camera on a string
	bool    m_bTheHeightFixerVehicleIsATrain;
	bool    LookBehindCamWasInFront;
	bool    LookingBehind;
	bool    LookingLeft; // 32
	bool    LookingRight;
	bool    ResetStatics; //for interpolation type stuff to work
	bool    Rotating;

	int16   Mode;                   // CameraMode
	uint32  m_uiFinishTime; // 52

	int     m_iDoCollisionChecksOnFrameNum;
	int     m_iDoCollisionCheckEveryNumOfFrames;
	int     m_iFrameNumWereAt;  // 64
	int     m_iRunningVectorArrayPos;
	int     m_iRunningVectorCounter;
	int     DirectionWasLooking;

	float   f_max_role_angle; //=DEGTORAD(5.0f);    
	float   f_Roll; //used for adding a slight roll to the camera in the
	float	f_rollSpeed;
	float   m_fSyphonModeTargetZOffSet;
	float	m_fUnknownZOffSet;
	float   m_fAmountFractionObscured;
	float   m_fAlphaSpeedOverOneFrame; // 100
	float   m_fBetaSpeedOverOneFrame;
	float   m_fBufferedTargetBeta;
	float   m_fBufferedTargetOrientation;
	float   m_fBufferedTargetOrientationSpeed;
	float   m_fCamBufferedHeight;
	float   m_fCamBufferedHeightSpeed;
	float   m_fCloseInPedHeightOffset;
	float   m_fCloseInPedHeightOffsetSpeed; // 132
	float   m_fCloseInCarHeightOffset;
	float   m_fCloseInCarHeightOffsetSpeed;
	float   m_fDimensionOfHighestNearCar;
	float   m_fDistanceBeforeChanges;
	float   m_fFovSpeedOverOneFrame;
	float   m_fMinDistAwayFromCamWhenInterPolating;
	float   m_fPedBetweenCameraHeightOffset;
	float   m_fPlayerInFrontSyphonAngleOffSet; // 164
	float   m_fRadiusForDead;
	float   m_fRealGroundDist; //used for follow ped mode
	float   m_fTargetBeta;
	float   m_fTimeElapsedFloat;

	float   m_fTransitionBeta;
	float   m_fTrueBeta;
	float   m_fTrueAlpha; // 200
	float   m_fInitialPlayerOrientation; //used for first person

	float   Alpha;
	float   AlphaSpeed;
	float   FOV;
	float   FOVSpeed;
	float   Beta;
	float   BetaSpeed;
	float   Distance; // 232
	float   DistanceSpeed;
	float   CA_MIN_DISTANCE;
	float   CA_MAX_DISTANCE;
	float   SpeedVar;

	// ped onfoot zoom distance
	float m_fTargetZoomGroundOne;
	float m_fTargetZoomGroundTwo; // 256
	float m_fTargetZoomGroundThree;
	// ped onfoot alpha angle offset
	float m_fTargetZoomOneZExtra;
	float m_fTargetZoomTwoZExtra;
	float m_fTargetZoomThreeZExtra;

	float m_fTargetZoomZCloseIn;
	float m_fMinRealGroundDist;
	float m_fTargetCloseInDist;

	CVector m_cvecTargetCoorsForFudgeInter; // 360
	CVector m_cvecCamFixedModeVector; // 372
	CVector m_cvecCamFixedModeSource; // 384
	CVector m_cvecCamFixedModeUpOffSet; // 396
	CVector m_vecLastAboveWaterCamPosition; //408  //helper for when the player has gone under the water
	CVector m_vecBufferedPlayerBodyOffset; // 420

	// The three vectors that determine this camera for this frame
	CVector Front;  // 432                                              // Direction of looking in
	CVector Source;                                                 // Coors in world space
	CVector SourceBeforeLookBehind;
	CVector Up;                                                     // Just that
	CVector m_arrPreviousVectors[NUMBER_OF_VECTORS_FOR_AVERAGE];    // used to average stuff
	CEntity* CamTargetEntity;

	float       m_fCameraDistance;
	float       m_fIdealAlpha;
	float       m_fPlayerVelocity;
	CAutomobile* m_pLastCarEntered; // So interpolation works
	CPed* m_pLastPedLookedAt;// So interpolation works 
	bool        m_bFirstPersonRunAboutActive;

	void GetVectorsReadyForRW(void);
	void Process_FollowCar_SA_III(const CVector&, float, float, float);
};
static_assert(sizeof(CCamIII) == 0x1A4, "CCam: wrong size");

struct CCamPathSplines
{
	float m_arr_PathData[800];
};

struct CTrainCamNode
{
	CVector m_cvecCamPosition;
	CVector m_cvecPointToLookAt;
	CVector m_cvecMinPointInRange;
	CVector m_cvecMaxPointInRange;
	float m_fDesiredFOV;
	float m_fNearClip;
};

struct CQueuedMode
{
	int16 Mode;
	float Duration;
	int16 MinZoom;
	int16 MaxZoom;
};

enum
{
	LOOKING_BEHIND,
	LOOKING_LEFT,
	LOOKING_RIGHT,
	LOOKING_FORWARD,
};

enum
{
	// TODO: figure out
	FADE_0,
	FADE_1,	// mid fade
	FADE_2,

	FADE_OUT = 0,
	FADE_IN,
};

enum
{
	MBLUR_NONE,
	MBLUR_SNIPER,
	MBLUR_NORMAL,
	MBLUR_INTRO1,		// green camera
	MBLUR_INTRO2,		// unused
	MBLUR_INTRO3,		// bank scene
	MBLUR_INTRO4,		// jail break scene
	MBLUR_INTRO5,		// explosion
	MBLUR_INTRO6,		// player shot
	MBLUR_UNUSED,		// pinkish
};

struct CCameraVC : public CPlaceableVC
{
	bool    m_bAboveGroundTrainNodesLoaded;
	bool    m_bBelowGroundTrainNodesLoaded;
	bool    m_bCamDirectlyBehind;
	bool    m_bCamDirectlyInFront;
	bool    m_bCameraJustRestored;
	bool    m_bcutsceneFinished;
	bool    m_bCullZoneChecksOn;
	bool    m_bFirstPersonBeingUsed; // To indicate if the m_bFirstPersonBeingUsed viewer is being used.
	bool    m_bJustJumpedOutOf1stPersonBecauseOfTarget;
	bool    m_bIdleOn;
	bool    m_bInATunnelAndABigVehicle;
	bool    m_bInitialNodeFound;
	bool    m_bInitialNoNodeStaticsSet;
	bool    m_bIgnoreFadingStuffForMusic;
	bool    m_bPlayerIsInGarage;
	bool    m_bPlayerWasOnBike;
	bool    m_bJustCameOutOfGarage;
	bool    m_bJustInitalised;	//Just so the speed thingy doesn't go mad right at the start
	bool	m_bJust_Switched;	//Variable to indicate that we have jumped somewhere
	bool    m_bLookingAtPlayer;
	bool    m_bLookingAtVector;
	bool    m_bMoveCamToAvoidGeom;
	bool    m_bObbeCinematicPedCamOn;
	bool    m_bObbeCinematicCarCamOn;
	bool    m_bRestoreByJumpCut;
	bool    m_bUseNearClipScript;
	bool    m_bStartInterScript;
	bool	m_bStartingSpline;
	bool    m_bTargetJustBeenOnTrain;	//this variable is needed to be able to restore the camera
	bool    m_bTargetJustCameOffTrain;
	bool    m_bUseSpecialFovTrain;
	bool    m_bUseTransitionBeta;
	bool    m_bUseScriptZoomValuePed;
	bool    m_bUseScriptZoomValueCar;
	bool    m_bWaitForInterpolToFinish;
	bool    m_bItsOkToLookJustAtThePlayer;	 //Used when interpolating
	bool    m_bWantsToSwitchWidescreenOff;
	bool    m_WideScreenOn;
	bool    m_1rstPersonRunCloseToAWall;
	bool    m_bHeadBob;
	bool    m_bVehicleSuspenHigh;
	bool    m_bEnable1rstPersonCamCntrlsScript;

	bool    m_bAllow1rstPersonWeaponsCamera;
	bool    m_bFailedCullZoneTestPreviously;
	bool    m_FadeTargetIsSplashScreen;	//used as hack for fading 
	bool    WorldViewerBeingUsed;	// To indicate if the world viewer is being used.                                      
	unsigned char   ActiveCam;
	uint8 somePad;
	unsigned int    m_uiCamShakeStart;          // When did the camera shake start.
	unsigned int    m_uiFirstPersonCamLastInputTime;
	unsigned int    m_uiLongestTimeInMill;
	unsigned int    m_uiNumberOfTrainCamNodes;

	unsigned char     m_uiTransitionJUSTStarted;  // This is the first frame of a transition.
	unsigned char     m_uiTransitionState;        // 0:one mode 1:transition
	uint8 somePad2;
	uint8 somePad3;
	unsigned int    m_uiTimeLastChange;
	unsigned int    m_uiTimeWeLeftIdle_StillNoInput;
	unsigned int    m_uiTimeWeEnteredIdle;
	unsigned int    m_uiTimeTransitionStart;    // When was the transition started ?
	unsigned int    m_uiTransitionDuration;     // How long does the transition take ?
	unsigned int    m_uiTransitionDurationTargetCoors;
	int     m_BlurBlue;
	int     m_BlurGreen;
	int     m_BlurRed;
	int     m_BlurType;
	int     m_iWorkOutSpeedThisNumFrames;
	int     m_iNumFramesSoFar;				//counter
	int     m_iCurrentTrainCamNode;			//variable indicating which camera node we are at for the train
	int     m_motionBlur;					//to indicate that we are fading

	int     m_imotionBlurAddAlpha;
	int     m_iCheckCullZoneThisNumFrames;
	int     m_iZoneCullFrameNumWereAt;
	int     WhoIsInControlOfTheCamera;		//to discern between obbe and scripts
	float   CamFrontXNorm, CamFrontYNorm;
	float CarZoomIndicator; // m_nCarZoom in SA
	float CarZoomValue;		// m_nCarZoomBase in SA
	float CarZoomValueSmooth; // m_fCarZoomSmoothed in SA
	float   DistanceToWater;
	float   FOVDuringInter;
	float   LODDistMultiplier;				 // This takes into account the FOV and the standard LOD multiplier Smaller aperture->bigger LOD multipliers.
	float   GenerationDistMultiplier;		// This takes into account the FOV but noy the standard LOD multiplier

	float   m_fAlphaSpeedAtStartInter;
	float   m_fAlphaWhenInterPol;
	float   m_fAlphaDuringInterPol;
	float   m_fBetaDuringInterPol;
	float   m_fBetaSpeedAtStartInter;
	float   m_fBetaWhenInterPol;
	float   m_fFOVWhenInterPol;
	float   m_fFOVSpeedAtStartInter;
	float   m_fStartingBetaForInterPol;
	float   m_fStartingAlphaForInterPol;
	float   m_PedOrientForBehindOrInFront;

	float   m_CameraAverageSpeed;		//this is an average depending on how many frames we work it out
	float   m_CameraSpeedSoFar;		 //this is a running total
	float   m_fCamShakeForce;			 // How severe is the camera shake.
	float	m_fCarZoomValueScript; // m_fCarZoomValueScript in SA
	float   m_fFovForTrain;
	float   m_fFOV_Wide_Screen;
	float   m_fNearClipScript;
	float   m_fOldBetaDiff;						 // Needed for interpolation between 2 modes
	float   m_fPedZoomValue;
	float   m_fPedZoomValueSmooth;
	float   m_fPedZoomValueScript;
	float   m_fPositionAlongSpline;				//Variable used to indicate how far along the spline we are 0-1 for started to completed respectively
	float   m_ScreenReductionPercentage;
	float   m_ScreenReductionSpeed;
	float   m_AlphaForPlayerAnim1rstPerson;
	float   Orientation;            // The orientation of the camera. Used for peds walking.
	float   PedZoomIndicator;
	float   PlayerExhaustion;       // How tired is player (inaccurate sniping) 0.0f-1.0f
									// The following things are used by the sound code to
									// play reverb depending on the surroundings. From a point
									// in front of the camera the disance is measured to the
									// nearest obstacle (building)
	float   SoundDistUp;			//, SoundDistLeft, SoundDistRight;     // These ones are buffered and should be used by the audio
	float   SoundDistUpAsRead;		//, SoundDistLeftAsRead, SoundDistRightAsRead;
	float   SoundDistUpAsReadOld;	 //, SoundDistLeftAsReadOld, SoundDistRightAsReadOld;
									 // Very rough distance to the nearest water for the sound to use
								  // Front vector X&Y normalised to 1. Used by loads of stuff.


	float   m_fAvoidTheGeometryProbsTimer;
	short   m_nAvoidTheGeometryProbsDirn;

	float   m_fWideScreenReductionAmount;	//0 for not reduced 1 for fully reduced (Variable for Les)
	float   m_fStartingFOVForInterPol;

	CCamVC Cams[3];                // The actual cameras (usually only one of the two is active)
								 // And to complicate this we have a third camera, this camera is 
								 // used for debugging when we want to have a look at the world.
								 // We can't change the camera mode because other objects depend on their

	void* pToGarageWeAreIn;
	void* pToGarageWeAreInForHackAvoidFirstPerson;
	CQueuedMode m_PlayerMode;

	// The higher priority player camera mode. This one is used
	// for the sniper mode and rocket launcher mode.
	// This one overwrites the m_PlayerMode above.
	CQueuedMode PlayerWeaponMode;
	CVector m_PreviousCameraPosition;		//needed to work out speed
	CVector m_RealPreviousCameraPosition;	// This cane be used by stuff outside the camera code. The one above is the same as the current coordinates outwidth the camera code.
											 // an active camera for range finding etc
	CVector m_cvecAimingTargetCoors;        // Coors to look at with Gordons aiming thing
											// The player camera that is waiting to be used


	CVector m_vecFixedModeVector;
	CVector m_vecFixedModeSource;
	CVector m_vecFixedModeUpOffSet;
	CVector m_vecCutSceneOffset;

	CVector m_cvecStartingSourceForInterPol;
	CVector m_cvecStartingTargetForInterPol;
	CVector m_cvecStartingUpForInterPol;
	CVector m_cvecSourceSpeedAtStartInter;
	CVector m_cvecTargetSpeedAtStartInter;
	CVector m_cvecUpSpeedAtStartInter;
	CVector m_vecSourceWhenInterPol;
	CVector m_vecTargetWhenInterPol;
	CVector m_vecUpWhenInterPol;
	CVector m_vecClearGeometryVec;
	CVector m_vecGameCamPos;

	CVector SourceDuringInter, TargetDuringInter, UpDuringInter;
	// RenderWare camera pointer
	void* m_pRwCamera;

	CEntity* pTargetEntity;
	// removed the fields we don't use
};
extern CCameraVC *TheCameraVC;

struct CCameraIII : public CPlaceable
{
	bool m_bAboveGroundTrainNodesLoaded;
	bool m_bBelowGroundTrainNodesLoaded;
	bool m_bCamDirectlyBehind;
	bool m_bCamDirectlyInFront;
	bool m_bCameraJustRestored;
	bool m_bcutsceneFinished;
	bool m_bCullZoneChecksOn;
	bool m_bFirstPersonBeingUsed;
	bool m_bJustJumpedOutOf1stPersonBecauseOfTarget;
	bool m_bIdleOn;
	bool m_bInATunnelAndABigVehicle;
	bool m_bInitialNodeFound;
	bool m_bInitialNoNodeStaticsSet;
	bool m_bIgnoreFadingStuffForMusic;
	bool m_bPlayerIsInGarage;
	bool m_bJustCameOutOfGarage;
	bool m_bJustInitalised;
	bool m_bJust_Switched;
	bool m_bLookingAtPlayer;
	bool m_bLookingAtVector;
	bool m_bMoveCamToAvoidGeom;
	bool m_bObbeCinematicPedCamOn;
	bool m_bObbeCinematicCarCamOn;
	bool m_bRestoreByJumpCut;
	bool m_bUseNearClipScript;
	bool m_bStartInterScript;
	bool m_bStartingSpline;
	bool m_bTargetJustBeenOnTrain;
	bool m_bTargetJustCameOffTrain;
	bool m_bUseSpecialFovTrain;
	bool m_bUseTransitionBeta;
	bool m_bUseScriptZoomValuePed;
	bool m_bUseScriptZoomValueCar;
	bool m_bWaitForInterpolToFinish;
	bool m_bItsOkToLookJustAtThePlayer;
	bool m_bWantsToSwitchWidescreenOff;
	bool m_WideScreenOn;
	bool m_1rstPersonRunCloseToAWall;
	bool m_bHeadBob;
	bool m_bFailedCullZoneTestPreviously;

	bool m_FadeTargetIsSplashScreen;

	bool WorldViewerBeingUsed;
	uint8 ActiveCam;
	uint32 m_uiCamShakeStart;
	uint32 m_uiFirstPersonCamLastInputTime;
	// where are those?
	//bool m_bVehicleSuspenHigh;
	//bool m_bEnable1rstPersonCamCntrlsScript;
	//bool m_bAllow1rstPersonWeaponsCamera;

	uint32 m_uiLongestTimeInMill;
	uint32 m_uiNumberOfTrainCamNodes;
	uint8   m_uiTransitionJUSTStarted;
	uint8   m_uiTransitionState;        // 0:one mode 1:transition

	uint32 m_uiTimeLastChange;
	uint32 m_uiTimeWeEnteredIdle;
	uint32 m_uiTimeTransitionStart;
	uint32 m_uiTransitionDuration;
	int m_BlurBlue;
	int m_BlurGreen;
	int m_BlurRed;
	int m_BlurType;

	uint32    unknown;
	int m_iWorkOutSpeedThisNumFrames;
	int m_iNumFramesSoFar;


	int m_iCurrentTrainCamNode;
	int m_motionBlur;
	int m_imotionBlurAddAlpha;
	int m_iCheckCullZoneThisNumFrames;
	int m_iZoneCullFrameNumWereAt;
	int WhoIsInControlOfTheCamera;

	float CamFrontXNorm;
	float CamFrontYNorm;
	float CarZoomIndicator; // m_nCarZoom in SA
	float CarZoomValue;		// m_nCarZoomBase in SA
	float CarZoomValueSmooth; // m_fCarZoomSmoothed in SA

	float DistanceToWater;
	float FOVDuringInter;
	float LODDistMultiplier;
	float GenerationDistMultiplier;
	float m_fAlphaSpeedAtStartInter;
	float m_fAlphaWhenInterPol;
	float m_fAlphaDuringInterPol;
	float m_fBetaDuringInterPol;
	float m_fBetaSpeedAtStartInter;
	float m_fBetaWhenInterPol;
	float m_fFOVWhenInterPol;
	float m_fFOVSpeedAtStartInter;
	float m_fStartingBetaForInterPol;
	float m_fStartingAlphaForInterPol;
	float m_PedOrientForBehindOrInFront;
	float m_CameraAverageSpeed;
	float m_CameraSpeedSoFar;
	float m_fCamShakeForce;
	float m_fCarZoomValueScript; // m_fCarZoomValueScript in SA
	float m_fFovForTrain;
	float m_fFOV_Wide_Screen;
	float m_fNearClipScript;
	float m_fOldBetaDiff;
	float m_fPedZoomValue;

	float m_fPedZoomValueScript;
	float m_fPedZoomValueSmooth;
	float m_fPositionAlongSpline;
	float m_ScreenReductionPercentage;
	float m_ScreenReductionSpeed;
	float m_AlphaForPlayerAnim1rstPerson;
	float Orientation;
	float PedZoomIndicator;
	float PlayerExhaustion;
	float SoundDistUp, SoundDistLeft, SoundDistRight;
	float SoundDistUpAsRead, SoundDistLeftAsRead, SoundDistRightAsRead;
	float SoundDistUpAsReadOld, SoundDistLeftAsReadOld, SoundDistRightAsReadOld;
	float m_fWideScreenReductionAmount;
	float m_fStartingFOVForInterPol;

	// not static yet
	float m_fMouseAccelHorzntl;// acceleration multiplier for 1st person controls
	float m_fMouseAccelVertical;// acceleration multiplier for 1st person controls
	float m_f3rdPersonCHairMultX;
	float m_f3rdPersonCHairMultY;


	CCamIII Cams[3];
	void* pToGarageWeAreIn;
	void* pToGarageWeAreInForHackAvoidFirstPerson;
	CQueuedMode m_PlayerMode;
	CQueuedMode PlayerWeaponMode;
	CVector m_PreviousCameraPosition;
	CVector m_RealPreviousCameraPosition;
	CVector m_cvecAimingTargetCoors;
	CVector m_vecFixedModeVector;
	CVector m_vecFixedModeSource;
	CVector m_vecFixedModeUpOffSet;
	CVector m_vecCutSceneOffset;

	// one of those has to go
	CVector m_cvecStartingSourceForInterPol;
	CVector m_cvecStartingTargetForInterPol;
	CVector m_cvecStartingUpForInterPol;
	CVector m_cvecSourceSpeedAtStartInter;
	CVector m_cvecTargetSpeedAtStartInter;
	CVector m_cvecUpSpeedAtStartInter;
	CVector m_vecSourceWhenInterPol;
	CVector m_vecTargetWhenInterPol;
	CVector m_vecUpWhenInterPol;
	//CVector m_vecClearGeometryVec;

	CVector m_vecGameCamPos;
	CVector SourceDuringInter;
	CVector TargetDuringInter;
	CVector UpDuringInter;
	void* m_pRwCamera;
	CEntity* pTargetEntity;
	CCamPathSplines m_arrPathArray[4];
	CTrainCamNode m_arrTrainCamNode[800];
	CMatrix m_cameraMatrix;
	bool m_bGarageFixedCamPositionSet;
	bool m_vecDoingSpecialInterPolation;
	bool m_bScriptParametersSetForInterPol;
	bool m_bFading;
	bool m_bMusicFading;
	CMatrix m_viewMatrix;
	CVector m_vecFrustumNormals[4];
	CVector m_vecOldSourceForInter;
	CVector m_vecOldFrontForInter;
	CVector m_vecOldUpForInter;

	float m_vecOldFOVForInter;
	float m_fFLOATingFade;
	float m_fFLOATingFadeMusic;
	float m_fTimeToFadeOut;
	float m_fTimeToFadeMusic;
	float m_fFractionInterToStopMovingTarget;
	float m_fFractionInterToStopCatchUpTarget;
	float m_fGaitSwayBuffer;
	float m_fScriptPercentageInterToStopMoving;
	float m_fScriptPercentageInterToCatchUp;

	uint32  m_fScriptTimeForInterPolation;


	int16   m_iFadingDirection;
	int     m_iModeObbeCamIsInForCar;
	int16   m_iModeToGoTo;
	int16   m_iMusicFadingDirection;
	int16   m_iTypeOfSwitch;

	uint32 m_uiFadeTimeStarted;
	uint32 m_uiFadeTimeStartedMusic;

	virtual ~CCameraIII() { };
};
static_assert(sizeof(CCameraIII) == 0xE9D8, "CCameraIII: wrong size");
extern CCameraIII *TheCameraIII;
