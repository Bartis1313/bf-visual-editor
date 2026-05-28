#pragma once

#include "declarations.h"
#include "enums.h"

namespace fb
{

////////////////////////////////////////
// RuntimeId: 10190
// TypeInfo:  0x0000000142C92740
struct WaterEntityClipInfo
{
    bool m_Enable; //0x0000
    bool m_ClipFaceNorth; //0x0001
    bool m_ClipFaceSouth; //0x0002
    bool m_ClipFaceEast; //0x0003
    bool m_ClipFaceWest; //0x0004
};//Size=0x0005

////////////////////////////////////////
// RuntimeId: 10186
// TypeInfo:  0x0000000142C92780
struct WaterAmbientFoamEffectSpawner
{
    EffectBlueprint* m_Effect; //0x0000
    float m_Threshold; //0x0008
    float m_Randomness; //0x000C
    float m_CoolDownTime; //0x0010
    float m_NearDistance; //0x0014
    float m_FarDistance; //0x0018
    float m_VerticalVelocityScale; //0x001C
    float m_HorizontalVelocityScale; //0x0020
    char _0x0024[4];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 03702
// TypeInfo:  0x0000000142305810
struct Vec4
{
    float m_x; //0x0000
    float m_y; //0x0004
    float m_z; //0x0008
    float m_w; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 10175
// TypeInfo:  0x0000000142C91E28
struct VoipInboundTalkersMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 10155
// TypeInfo:  0x0000000142C916A0
struct TwinkleHideLoadingSpinnerMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 10154
// TypeInfo:  0x0000000142C916C0
struct TwinkleShowLoadingSpinnerMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 10153
// TypeInfo:  0x0000000142C916E0
struct TwinkleJscModuleFailedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 10152
// TypeInfo:  0x0000000142C91700
struct TwinkleStartVoipStatusPollingMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 10151
// TypeInfo:  0x0000000142C91720
struct TwinkleJsBundleSignatureCheckFailedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 10150
// TypeInfo:  0x0000000142C91740
struct TwinkleManifestAssetDownloadFailedMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 10149
// TypeInfo:  0x0000000142C91760
struct TwinkleNewManifestBundleDownloadCompletedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 10148
// TypeInfo:  0x0000000142C91780
struct TwinkleNewManifestBundleDownloadStartedMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 10147
// TypeInfo:  0x0000000142C917A0
struct TwinkleToggleVirtualKeyboardMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 10146
// TypeInfo:  0x0000000142C917C0
struct TwinkleAssetsWrittenMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 10145
// TypeInfo:  0x0000000142C917E0
struct TwinkleEnableInputMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 10144
// TypeInfo:  0x0000000142C91800
struct TwinkleDisableInputMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 10143
// TypeInfo:  0x0000000142C91820
struct TwinkleRequestWriteBundledDataCompletedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 10142
// TypeInfo:  0x0000000142C91840
struct TwinkleRequestWriteBundledDataMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 10141
// TypeInfo:  0x0000000142C91860
struct TwinkleShowFallbackUIMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 10140
// TypeInfo:  0x0000000142C91880
struct TwinkleHideMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 10139
// TypeInfo:  0x0000000142C918A0
struct TwinkleShowMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 10138
// TypeInfo:  0x0000000142C918C0
struct TwinkleScriptEngineReloadedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 10137
// TypeInfo:  0x0000000142C918E0
struct TwinkleStateChangedMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 10136
// TypeInfo:  0x0000000142C91900
struct TwinkleProcessMessagesFromJSMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 10116
// TypeInfo:  0x0000000142C905F8
struct TireTrailConfiguration
{
    float m_SectionWidth; //0x0000
    float m_TrailOffset; //0x0004
    TireTrailVehicleSkidMarkConfigData* m_SkidMarkData; //0x0008
    WheelComponentData* m_LeftWheelComponent; //0x0010
    WheelComponentData* m_RightWheelComponent; //0x0018
    bool m_MirrorUVs; //0x0020
    bool m_FadeBasedOnSlip; //0x0021
    char _0x0022[6];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 10112
// TypeInfo:  0x0000000142C90618
struct VehicleEffectSet
{
    EffectBlueprint* m_WheelEffect; //0x0000
    RibbonVolumeTemplateData* m_ContactTrail; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 10085
// TypeInfo:  0x0000000142C8F1F8
struct SpartaGatewayHostSettings
{
    char* m_ManifestProtocol; //0x0000
    char* m_ManifestHost; //0x0008
    char* m_ManifestPath; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 10087
// TypeInfo:  0x0000000142C8F898
struct SpartaGatewayToOnlineEnvironmentMapping
{
    SpartaOnlineEnvironment m_SpartaOnlineEnvironment; //0x0000
    char _0x0004[4];
    SpartaGatewayHostSettings m_SpartaGatewayHostSettings; //0x0008
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 10081
// TypeInfo:  0x0000000142C8F218
struct SpartaModMaskUpdatedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 10080
// TypeInfo:  0x0000000142C8F238
struct SpartaShowOfflineMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 10079
// TypeInfo:  0x0000000142C8F258
struct SpartaVoipSpeakerStatusChangedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 10078
// TypeInfo:  0x0000000142C8F8B8
struct SpeakerChange
{
    char* m_SpeakerId; //0x0000
    char* m_OnlineId; //0x0008
    __int32 m_Status; //0x0010
    char _0x0014[4];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 10076
// TypeInfo:  0x0000000142C8F278
struct SpartaNucleusRegistrationMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 10075
// TypeInfo:  0x0000000142C8F298
struct SpartaFirstPartyConnectedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 10074
// TypeInfo:  0x0000000142C8F2B8
struct SpartaSessionInfoMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 10073
// TypeInfo:  0x0000000142C8F2D8
struct SpartaShowFirstPartyUserProfileMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 10072
// TypeInfo:  0x0000000142C8F2F8
struct SpartaPushNotificationMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 10071
// TypeInfo:  0x0000000142C8F318
struct SpartaNotificationInvokedMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 10070
// TypeInfo:  0x0000000142C8F338
struct SpartaInteractiveNotificationMessage
{
    char _0x0000[88];
};//Size=0x0058

////////////////////////////////////////
// RuntimeId: 10069
// TypeInfo:  0x0000000142C8F358
struct SpartaNotificationMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 10066
// TypeInfo:  0x0000000142C8F398
struct SpartaShowUserProfileMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 10065
// TypeInfo:  0x0000000142C8F3B8
struct SpartaStreamingInstallStateChangedMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 10064
// TypeInfo:  0x0000000142C8F8D8
struct SpartaStreamingInstallStateData
{
    float m_SpInstallProgress; //0x0000
    float m_InstallProgress; //0x0004
    bool m_IsInstalling; //0x0008
    bool m_IsSpInstalled; //0x0009
    bool m_IsMpInstalled; //0x000A
    char _0x000B[1];
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 10056
// TypeInfo:  0x0000000142C8F3F8
struct SpartaPresenceUserTitleInfoMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 10055
// TypeInfo:  0x0000000142C8F8F8
struct SpartaUserTitleInfo
{
    unsigned __int64 m_PersonaId; //0x0000
    char* m_Name; //0x0008
    __int32 m_Rank; //0x0010
    unsigned __int16 m_BasicDogTag; //0x0014
    unsigned __int16 m_AdvancedDogTag; //0x0016
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 10051
// TypeInfo:  0x0000000142C8F438
struct SpartaMatchmakingResultMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 10050
// TypeInfo:  0x0000000142C8F458
struct SpartaMatchmakingCanceledMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 10049
// TypeInfo:  0x0000000142C8F478
struct SpartaMatchmakingStartedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 10046
// TypeInfo:  0x0000000142C8F4B8
struct SpartaMatchmakingRequestMessage
{
    char _0x0000[5464];
};//Size=0x1558

////////////////////////////////////////
// RuntimeId: 10043
// TypeInfo:  0x0000000142C8F4F8
struct SpartaPartyResponseMessageBase
{
    char _0x0000[208];
};//Size=0x00D0

////////////////////////////////////////
// RuntimeId: 10042
// TypeInfo:  0x0000000142C8F518
struct SpartaPartyRequestMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 10033
// TypeInfo:  0x0000000142C8F5B8
struct SpartaStopLoadingTransitionEffectMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 10032
// TypeInfo:  0x0000000142C8F5D8
struct SpartaStartLoadingTransitionEffectMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 10031
// TypeInfo:  0x0000000142C8F5F8
struct SpartaCrossLaunchMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 10030
// TypeInfo:  0x0000000142C8F618
struct SpartaPressToStartMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 10029
// TypeInfo:  0x0000000142C8F638
struct SpartaLoadProfileDataCompleteMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 10028
// TypeInfo:  0x0000000142C8F658
struct SpartaLoadProfileDataMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 10027
// TypeInfo:  0x0000000142C8F678
struct SpartaShowDisconnectionMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 10024
// TypeInfo:  0x0000000142C8F6B8
struct SpartaShowLoadingScreenMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 10023
// TypeInfo:  0x0000000142C8F6D8
struct SpartaCommerceCheckoutCompleteMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 10020
// TypeInfo:  0x0000000142C8F718
struct SpartaCommerceCheckoutPsPlusMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 10019
// TypeInfo:  0x0000000142C8F738
struct SpartaCommerceShowDetailsMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 10018
// TypeInfo:  0x0000000142C8F758
struct SpartaCommerceCheckoutMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 10017
// TypeInfo:  0x0000000142C8F778
struct SpartaShowEndOfRoundMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 10016
// TypeInfo:  0x0000000142C8F798
struct SpartaGetFriendsMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 10015
// TypeInfo:  0x0000000142C8F7B8
struct SpartaEndOfRoundMessage
{
    char _0x0000[208];
};//Size=0x00D0

////////////////////////////////////////
// RuntimeId: 10014
// TypeInfo:  0x0000000142C8F7D8
struct SpartaCampaignInfoResponseMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 10013
// TypeInfo:  0x0000000142C8F7F8
struct SpartaCampaignInfoRequestMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 10012
// TypeInfo:  0x0000000142C8F818
struct SpartaReadyMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 10009
// TypeInfo:  0x0000000142C8F838
struct SpartaCampaignStatisticsInfo
{
    float m_Score; //0x0000
    float m_Kills; //0x0004
    float m_MeleeKills; //0x0008
    float m_Headshots; //0x000C
    float m_MultiKills; //0x0010
    float m_AdrenalineKills; //0x0014
    float m_KillStreaks; //0x0018
    float m_SquadKills; //0x001C
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 10011
// TypeInfo:  0x0000000142C8F918
struct SpartaCampaignLevelInfo
{
    char* m_Path; //0x0000
    char* m_Name; //0x0008
    unsigned __int32 m_SortIndex; //0x0010
    __int32 m_CompletedDifficulty; //0x0014
    SpartaCampaignStatisticsInfo m_StatisticsInfo; //0x0018
    bool m_IsUnlocked; //0x0038
    char _0x0039[7];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 10007
// TypeInfo:  0x0000000142C8F858
struct SpartaFirstPartyTokensMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 10006
// TypeInfo:  0x0000000142C8F938
struct SpartaFirstPartyToken
{
    char* m_ServiceName; //0x0000
    char* m_ClientId; //0x0008
    char* m_Token; //0x0010
    char* m_Issuer; //0x0018
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 10004
// TypeInfo:  0x0000000142C8F878
struct SpartaGetFirstPartyTokensMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 10003
// TypeInfo:  0x0000000142C8F958
struct SpartaFirstPartyService
{
    char* m_ServiceName; //0x0000
    char* m_ClientId; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 10001
// TypeInfo:  0x0000000142C8F978
struct SpartaEorTeamStats
{
    char* m_Name; //0x0000
    float m_Score; //0x0008
    unsigned __int16 m_TeamId; //0x000C
    char _0x000E[2];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09999
// TypeInfo:  0x0000000142C8F998
struct SpartaEorPlayerStats
{
    char* m_Name; //0x0000
    char* m_Platoon; //0x0008
    float m_Score; //0x0010
    char _0x0014[4];
    char* m_PlayerId; //0x0018
    unsigned __int16 m_Rank; //0x0020
    unsigned __int16 m_Kills; //0x0022
    unsigned __int16 m_Deaths; //0x0024
    unsigned __int16 m_SquadId; //0x0026
    unsigned __int16 m_TeamId; //0x0028
    bool m_IsMe; //0x002A
    bool m_IsSquadLeader; //0x002B
    char _0x002C[4];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 09997
// TypeInfo:  0x0000000142C8F9B8
struct SpartaEorSquadStats
{
    char* m_Name; //0x0000
    float m_Score; //0x0008
    unsigned __int16 m_SquadRank; //0x000C
    unsigned __int16 m_MemberCount; //0x000E
    unsigned __int16 m_Kills; //0x0010
    unsigned __int16 m_Deaths; //0x0012
    unsigned __int16 m_CompletedOrders; //0x0014
    unsigned __int16 m_Heals; //0x0016
    unsigned __int16 m_Revives; //0x0018
    unsigned __int16 m_Resupplies; //0x001A
    unsigned __int16 m_Repairs; //0x001C
    unsigned __int16 m_ReconSpots; //0x001E
    unsigned __int16 m_TeamId; //0x0020
    unsigned __int16 m_SquadId; //0x0022
    char _0x0024[4];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09995
// TypeInfo:  0x0000000142C8F9D8
struct SpartaEorAwardInfo
{
    char* m_Code; //0x0000
    float m_Score; //0x0008
    unsigned __int16 m_Count; //0x000C
    unsigned __int16 m_Record; //0x000E
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09993
// TypeInfo:  0x0000000142C8F9F8
struct SpartaEorPlayerInfo
{
    unsigned __int32 m_FlagCaptures; //0x0000
    unsigned __int32 m_Kills; //0x0004
    unsigned __int32 m_Deaths; //0x0008
    unsigned __int32 m_RoundScore; //0x000C
    unsigned __int32 m_RoundLength; //0x0010
    unsigned __int32 m_GlobalScore; //0x0014
    unsigned __int32 m_GlobalLength; //0x0018
    unsigned __int32 m_Skill; //0x001C
    unsigned __int32 m_Rank; //0x0020
    unsigned __int32 m_CodexScore; //0x0024
    unsigned __int32 m_ScoreTarget; //0x0028
    unsigned __int32 m_ScoreRoundBonus; //0x002C
    unsigned __int32 m_ScoreBoostBonus; //0x0030
    char _0x0034[4];
    char* m_BestWeapon; //0x0038
    unsigned __int32 m_BestWeaponKills; //0x0040
    char _0x0044[4];
    char* m_BestGadget; //0x0048
    unsigned __int32 m_BestGadgetUses; //0x0050
    char _0x0054[4];
    char* m_BestKit; //0x0058
    char* m_BestVehicle; //0x0060
    unsigned __int32 m_BestVehicleKills; //0x0068
    unsigned __int16 m_TeamId; //0x006C
    unsigned __int16 m_SquadId; //0x006E
};//Size=0x0070

////////////////////////////////////////
// RuntimeId: 09979
// TypeInfo:  0x0000000142C8FA18
struct SpartaPs4Service
{
    char* m_ServiceId; //0x0000
    char* m_ClientId; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09977
// TypeInfo:  0x0000000142C8FA38
struct SpartaUserId
{
    char* m_SceNpId; //0x0000
    char* m_Xuid; //0x0008
    char* m_NucleusId; //0x0010
    char* m_PersonaId; //0x0018
    char* m_PersonaName; //0x0020
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09971
// TypeInfo:  0x0000000142C8FA58
struct OffsetInfo
{
    char* m_AssetName; //0x0000
    unsigned __int32 m_Offset; //0x0008
    unsigned __int32 m_Size; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09952
// TypeInfo:  0x0000000142C8D2A8
struct WeaponZeroingEntry
{
    float m_Distance; //0x0000
    float m_Angle; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 09948
// TypeInfo:  0x0000000142C8D2C8
struct GunSwayModifierUnlock
{
    Guid m_UnlockAssetGuid; //0x0000
    char _0x0000[16];
    GunSwayModifierData* m_Modifier; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 09924
// TypeInfo:  0x0000000142C8D268
struct GunSwayDispersionModData
{
    float m_MinAngleModifier; //0x0000
    float m_MaxAngleModifier; //0x0004
    float m_IncreasePerShotModifier; //0x0008
    float m_DecreasePerSecondModifier; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09922
// TypeInfo:  0x0000000142C8D288
struct GunSwayDispersionData
{
    float m_MinAngle; //0x0000
    float m_MaxAngle; //0x0004
    float m_IncreasePerShot; //0x0008
    float m_DecreasePerSecond; //0x000C
    float m_NoFireDecreasePerSecond; //0x0010
    float m_NoFireTimeThreshold; //0x0014
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 09930
// TypeInfo:  0x0000000142C8D228
struct GunSwayRecoilData
{
    float m_RecoilAmplitudeMax; //0x0000
    float m_RecoilAmplitudeIncPerShot; //0x0004
    float m_HorizontalRecoilAmplitudeIncPerShotMin; //0x0008
    float m_HorizontalRecoilAmplitudeIncPerShotMax; //0x000C
    float m_HorizontalRecoilAmplitudeMax; //0x0010
    float m_RecoilAmplitudeDecreaseFactor; //0x0014
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 09926
// TypeInfo:  0x0000000142C8D248
struct GunSwayLagData
{
    float m_MoveStrafeModifier; //0x0000
    float m_MoveForwardModifier; //0x0004
    float m_RotateYawModifier; //0x0008
    float m_RotatePitchModifier; //0x000C
    float m_ReleaseModifier; //0x0010
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 09936
// TypeInfo:  0x0000000142C8D1E8
struct GunSwayBaseMoveData
{
    GunSwayDispersionData m_BaseValue; //0x0000
    GunSwayDispersionData m_Moving; //0x0018
    GunSwayRecoilData m_Recoil; //0x0030
    GunSwayLagData m_GunSwayLag; //0x0048
};//Size=0x005C

////////////////////////////////////////
// RuntimeId: 09934
// TypeInfo:  0x0000000142C8D2E8
struct GunSwayBaseData
{
    GunSwayDispersionData m_BaseValue; //0x0000
    GunSwayRecoilData m_Recoil; //0x0018
    GunSwayLagData m_GunSwayLag; //0x0030
};//Size=0x0044

////////////////////////////////////////
// RuntimeId: 09932
// TypeInfo:  0x0000000142C8D208
struct GunSwayStanceTransition
{
    GunSwayDispersionData m_MaxPenaltyValue; //0x0000
    float m_CoolDown; //0x0018
};//Size=0x001C

////////////////////////////////////////
// RuntimeId: 09938
// TypeInfo:  0x0000000142C8D1C8
struct GunSwayBaseMoveJumpData
{
    GunSwayDispersionData m_BaseValue; //0x0000
    GunSwayDispersionData m_Moving; //0x0018
    GunSwayDispersionData m_Jumping; //0x0030
    GunSwayDispersionData m_Sprinting; //0x0048
    GunSwayDispersionData m_VaultingSmallObject; //0x0060
    GunSwayDispersionData m_VaultingMediumObject; //0x0078
    GunSwayRecoilData m_Recoil; //0x0090
    GunSwayLagData m_GunSwayLag; //0x00A8
};//Size=0x00BC

////////////////////////////////////////
// RuntimeId: 09940
// TypeInfo:  0x0000000142C8D1A8
struct GunSwayStandData
{
    GunSwayBaseMoveJumpData m_NoZoom; //0x0000
    GunSwayBaseMoveJumpData m_Zoom; //0x00BC
};//Size=0x0178

////////////////////////////////////////
// RuntimeId: 09944
// TypeInfo:  0x0000000142C8D168
struct GunSwayStanceZoomModifierData
{
    GunSwayDispersionModData m_DispersionMod; //0x0000
    GunSwayDispersionModData m_MovingDispersionMod; //0x0010
    GunSwayDispersionModData m_SprintingDispersionMod; //0x0020
    float m_RecoilMagnitudeMod; //0x0030
    float m_RecoilAngleMod; //0x0034
    float m_FirstShotRecoilMod; //0x0038
    float m_LagYawMod; //0x003C
    float m_LagPitchMod; //0x0040
};//Size=0x0044

////////////////////////////////////////
// RuntimeId: 09942
// TypeInfo:  0x0000000142C8D188
struct GunSwayCrouchProneData
{
    GunSwayBaseMoveData m_NoZoom; //0x0000
    GunSwayBaseMoveData m_Zoom; //0x005C
};//Size=0x00B8

////////////////////////////////////////
// RuntimeId: 03877
// TypeInfo:  0x0000000142C00520
struct AntRef
{
    Guid m_AssetGuid; //0x0000
    char _0x0000[16];
    __int32 m_ProjectId; //0x0010
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 09914
// TypeInfo:  0x0000000142C89C68
struct WeaponMesh1p
{
    SkinnedMeshAsset* m_Mesh; //0x0000
    SkinnedMeshAsset* m_MeshZoom; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09898
// TypeInfo:  0x0000000142C89C88
struct WeaponModifierData
{
    Guid m_UnlockAssetGuid; //0x0000
    char _0x0000[16];
    Array<WeaponModifierBase*> m_Modifiers; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 09896
// TypeInfo:  0x0000000142C89CA8
struct WeaponModifier
{
    char _0x0000[144];
    __int32 m_Dummy2ToMakeFrostEDcompile; //0x0090
    char _0x0094[140];
};//Size=0x0120

////////////////////////////////////////
// RuntimeId: 09862
// TypeInfo:  0x0000000142C89728
struct SupplyData
{
    HealingSupplyUnitSphereData* m_Healing; //0x0000
    AmmoSupplyUnitSphereData* m_Ammo; //0x0008
    float m_ExclusionTimeout; //0x0010
    float m_ExcludeDamagedSoldierTime; //0x0014
    float m_ExcludeSuppressedSoldierTime; //0x0018
    bool m_SupplySoldiers; //0x001C
    bool m_SupplyVehicles; //0x001D
    bool m_TeamSpecific; //0x001E
    bool m_ExcludeSelf; //0x001F
    bool m_ExcludeSoldiersUnderwater; //0x0020
    bool m_ExcludeSoldierIfInMelee; //0x0021
    char _0x0022[6];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09846
// TypeInfo:  0x0000000142C89CC8
struct PlatformAimAssistData
{
    GamePlatform m_Platform; //0x0000
    char _0x0004[4];
    SoldierAimAssistData* m_AimAssistData; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09836
// TypeInfo:  0x0000000142C89748
struct NetworkDefibrillatorKillMessage
{
    char _0x0000[112];
};//Size=0x0070

////////////////////////////////////////
// RuntimeId: 09835
// TypeInfo:  0x0000000142C89768
struct NetworkDefibrillatorReviveMessage
{
    char _0x0000[112];
};//Size=0x0070

////////////////////////////////////////
// RuntimeId: 09832
// TypeInfo:  0x0000000142C89788
struct DefibrillatorUpgradeWeaponData
{
    ValueUnlockAsset* m_UpgradeUnlock; //0x0000
    float m_AttackTime; //0x0008
    float m_FullHealthTime; //0x000C
    float m_InitialStartHealth; //0x0010
    float m_DefibActionCooldownTime; //0x0014
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 09798
// TypeInfo:  0x0000000142C897A8
struct SoldierDetonationData
{
    float m_Angle; //0x0000
    float m_Radius; //0x0004
    float m_SoldierDetonationActivationDelay; //0x0008
    float m_MinSpeedForActivation; //0x000C
    bool m_HasSoldierDetonation; //0x0010
    bool m_UseAngle; //0x0011
    char _0x0012[2];
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 09796
// TypeInfo:  0x0000000142C897C8
struct NetworkSetCustomizationWeaponSlotMessage
{
    char _0x0000[88];
};//Size=0x0058

////////////////////////////////////////
// RuntimeId: 09795
// TypeInfo:  0x0000000142C897E8
struct NetworkSetActiveWeaponSlotMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 09794
// TypeInfo:  0x0000000142C89808
struct BulletToCharacterComponentsHitMessage
{
    char _0x0000[128];
};//Size=0x0080

////////////////////////////////////////
// RuntimeId: 03700
// TypeInfo:  0x00000001423057F0
struct Vec3
{
    float m_x; //0x0000
    float m_y; //0x0004
    float m_z; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09789
// TypeInfo:  0x0000000142C89828
struct CustomizationAccessoryPivots
{
    Vec3 m_BasePivotPos; //0x0000
    Vec3 m_OpticPivotPos; //0x0010
    Vec3 m_BarrelPivotPos; //0x0020
    Vec3 m_AccessoryPivotPos; //0x0030
    Vec3 m_UnderBarrelPivotPos; //0x0040
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 09785
// TypeInfo:  0x0000000142C89848
struct WeaponAnimationSettingsData
{
    float m_KickbackFactor; //0x0000
    float m_KickbackSpeedFactor; //0x0004
    float m_WeaponOffsetX; //0x0008
    float m_WeaponOffsetY; //0x000C
    float m_WeaponOffsetZ; //0x0010
    float m_ZoomOutSpeed; //0x0014
    float m_ZoomInSpeed; //0x0018
};//Size=0x001C

////////////////////////////////////////
// RuntimeId: 09779
// TypeInfo:  0x0000000142C89D08
struct WeaponOverrideValue
{
    WeaponOverrideValueType m_ValueType; //0x0000
    float m_Value; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 09769
// TypeInfo:  0x0000000142C89868
struct PickupSettingsData
{
    Vec3 m_MeshRenderOffset; //0x0000
    Vec3 m_MeshRenderRotation; //0x0010
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 03708
// TypeInfo:  0x0000000142305850
struct LinearTransform
{
    Vec3 m_right; //0x0000
    Vec3 m_up; //0x0010
    Vec3 m_forward; //0x0020
    Vec3 m_trans; //0x0030
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 09761
// TypeInfo:  0x0000000142C89D48
struct RigidMeshSocketTransform
{
    LinearTransform m_Transform; //0x0000
    WeaponRegularSocketObjectData* m_SocketObject; //0x0040
    char _0x0048[8];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 09759
// TypeInfo:  0x0000000142C89D68
struct AnimationConfigurationShootModuleData
{
    float m_ZoomedKickbackFactor; //0x0000
    float m_ZoomedKickbackSpeedFactor; //0x0004
    float m_KickbackFactor; //0x0008
    float m_KickbackSpeedFactor; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09749
// TypeInfo:  0x0000000142C89888
struct WeaponLagEffectForceData
{
    Vec3 m_OffsetForce; //0x0000
    Vec3 m_RotationForce; //0x0010
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 09745
// TypeInfo:  0x0000000142C898C8
struct WeaponLagEffectSpringData
{
    float m_Constant; //0x0000
    float m_Damping; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 09747
// TypeInfo:  0x0000000142C898A8
struct WeaponLagEffectSpringVector
{
    WeaponLagEffectSpringData m_SpringX; //0x0000
    WeaponLagEffectSpringData m_SpringY; //0x0008
    WeaponLagEffectSpringData m_SpringZ; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 09739
// TypeInfo:  0x0000000142C89D88
struct ZoomLevelSpecificTransitionTime
{
    __int32 m_FromZoomLevel; //0x0000
    __int32 m_ToZoomLevel; //0x0004
    float m_ZoomTransitionTime; //0x0008
    float m_FovTransitionTime; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09737
// TypeInfo:  0x0000000142C898E8
struct AimingPoseData
{
    float m_MinimumPitch; //0x0000
    float m_MaximumPitch; //0x0004
    float m_TargetingFov; //0x0008
    float m_AimSteadiness; //0x000C
    float m_SpeedMultiplier; //0x0010
    float m_RecoilMultiplier; //0x0014
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 09731
// TypeInfo:  0x0000000142C89908
struct UnlockWeaponAndSlot
{
    SoldierWeaponUnlockAsset* m_Weapon; //0x0000
    WeaponSlot m_Slot; //0x0008
    char _0x000C[4];
    Array<UnlockAssetBase*> m_CustomizationUnlockAssets; //0x0010
    Array<UnlockAssetBase*> m_UnlockAssets; //0x0018
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 09727
// TypeInfo:  0x0000000142C89DA8
struct UnlockToBlueprintBundle
{
    Guid m_UnlockAssetGuid; //0x0000
    char _0x0000[16];
    char* m_BundleName; //0x0010
    bool m_IsFirstPersonOnly; //0x0018
    char _0x0019[7];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 09725
// TypeInfo:  0x0000000142C89928
struct StoredWeaponData
{
    __int32 m_WeaponClass; //0x0000
    __int32 m_MagCount; //0x0004
    __int32 m_MagCapacity; //0x0008
    bool m_SwitchToPrimaryWhenOutOfAmmo; //0x000C
    char _0x000D[3];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09709
// TypeInfo:  0x0000000142C89DC8
struct DirectWeaponSwitchingMapData
{
    __int32 m_Action; //0x0000
    float m_MaxHoldTime; //0x0004
    WeaponSwitchingEnum m_ToWeapon; //0x0008
    float m_SwitchBackToPrevMaxTimePressed; //0x000C
    float m_PreventMeleeRepeatTime; //0x0010
    bool m_FireAndSwitchBackToPrev; //0x0014
    bool m_UseQuickSwitch; //0x0015
    char _0x0016[2];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 09707
// TypeInfo:  0x0000000142C89DE8
struct WeaponSwitchingMapData
{
    WeaponSwitchingEnum m_FromWeapon; //0x0000
    __int32 m_Action; //0x0004
    float m_MaxHoldTime; //0x0008
    char _0x000C[4];
    Array<WeaponSwitchingEnum> m_ToWeapon; //0x0010
    bool m_FireAndSwitchBackToPrev; //0x0018
    char _0x0019[7];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 09697
// TypeInfo:  0x0000000142C89E08
struct DamageEntryData
{
    float m_Damage; //0x0000
    float m_Timeout; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 09689
// TypeInfo:  0x0000000142C89988
struct SoldierToComponentsOnClearSpottingMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 09688
// TypeInfo:  0x0000000142C899A8
struct ClientSoldierToComponentsOnHandleDamageMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 09687
// TypeInfo:  0x0000000142C899C8
struct ClientSoldierToComponentsOnImpulseMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 09686
// TypeInfo:  0x0000000142C899E8
struct SoldierToComponentsOnActionStateChangedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 09685
// TypeInfo:  0x0000000142C89A08
struct SoldierToComponentsOnIncapableMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 09684
// TypeInfo:  0x0000000142C89A28
struct SoldierToComponentsOnCustomizeSoldierMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 09683
// TypeInfo:  0x0000000142C89A48
struct SoldierToComponentsInitializedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09674
// TypeInfo:  0x0000000142C89E28
struct ValueAtX
{
    float m_X; //0x0000
    float m_Value; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 09666
// TypeInfo:  0x0000000142C89E48
struct HidableSoldierMeshWeaponPart
{
    SkinnedMeshAsset* m_WeaponMesh; //0x0000
    ObjectBlueprint* m_WeaponMeshBlueprint; //0x0008
    char* m_BoneName; //0x0010
    bool m_HideAlways; //0x0018
    bool m_HideInVehicleEntries; //0x0019
    char _0x001A[6];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 09662
// TypeInfo:  0x0000000142C89E68
struct SoldierHeadCollisionPoseData
{
    Vec3 m_IdleOffset; //0x0000
    Vec3 m_MovingForwardOffset; //0x0010
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 09658
// TypeInfo:  0x0000000142C89E88
struct AutoAimData
{
    Vec3 m_AutoAimOuterBoxOffset; //0x0000
    Vec3 m_AutoAimOuterBoxExtends; //0x0010
    Vec3 m_AutoAimInnerBoxOffset; //0x0020
    Vec3 m_AutoAimInnerBoxExtends; //0x0030
    CharacterPoseType m_PoseType; //0x0040
    char _0x0044[12];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 09656
// TypeInfo:  0x0000000142C89EA8
struct FootStepTrigger
{
    float m_Time; //0x0000
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 09648
// TypeInfo:  0x0000000142C89A88
struct NetworkPickupWeaponFromKitMessage
{
    char _0x0000[88];
};//Size=0x0058

////////////////////////////////////////
// RuntimeId: 09645
// TypeInfo:  0x0000000142C89EC8
struct DynamicWeaponPickupSlotData
{
    unsigned __int32 m_WeaponSlot; //0x0000
    __int32 m_AltWeaponSlot; //0x0004
    __int32 m_LinkedToWeaponSlot; //0x0008
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 09637
// TypeInfo:  0x0000000142C89EE8
struct WeaponUnlockPickupData
{
    UnlockWeaponAndSlot m_UnlockWeaponAndSlot; //0x0000
    __int32 m_AltWeaponSlot; //0x0020
    __int32 m_LinkedToWeaponSlot; //0x0024
    unsigned __int32 m_MinAmmo; //0x0028
    unsigned __int32 m_MaxAmmo; //0x002C
    bool m_UnlimitedAmmoPickup; //0x0030
    char _0x0031[7];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 09619
// TypeInfo:  0x0000000142C89AA8
struct HitReactionComponentBinding
{
    AntRef m_Hit; //0x0000
    AntRef m_HitByAIPlayer; //0x0014
    AntRef m_AllowKillFromAnimation; //0x0028
    AntRef m_Direction; //0x003C
    AntRef m_FiringDistance; //0x0050
    AntRef m_BoneType; //0x0064
    AntRef m_ImpactType; //0x0078
    AntRef m_Immortal; //0x008C
    AntRef m_RandomAnimationIndex; //0x00A0
};//Size=0x00B4

////////////////////////////////////////
// RuntimeId: 09613
// TypeInfo:  0x0000000142C89AE8
struct BlockAimAssistData
{
    float m_BlockingRadius; //0x0000
    float m_LifeTime; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 09607
// TypeInfo:  0x0000000142C89B08
struct NetworkPlayerSelectedWeaponMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 09604
// TypeInfo:  0x0000000142C89B28
struct CoverPeekBinding
{
    AntRef m_CoverHeight1p; //0x0000
    AntRef m_CanCoverPeek; //0x0014
    AntRef m_CanPeekOver; //0x0028
    AntRef m_CanPeekLeft; //0x003C
    AntRef m_CanPeekRight; //0x0050
    AntRef m_WeaponDown; //0x0064
};//Size=0x0078

////////////////////////////////////////
// RuntimeId: 09580
// TypeInfo:  0x0000000142C89B48
struct MovementComponentBinding
{
    AntRef m_AnimationControlled; //0x0000
    AntRef m_FeedbackCollisions; //0x0014
    AntRef m_DisableAnimationControlledCamera; //0x0028
    AntRef m_DisableAnimationControlExitVelocity; //0x003C
    AntRef m_ForceVelocityMode; //0x0050
};//Size=0x0064

////////////////////////////////////////
// RuntimeId: 09576
// TypeInfo:  0x0000000142C89B68
struct CameraCommonBinding
{
    AntRef m_FirstPersonCameraHeight; //0x0000
    AntRef m_AnimatedCameraBlendTime; //0x0014
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09574
// TypeInfo:  0x0000000142C89B88
struct Camera1pBinding
{
    AntRef m_Render1pInBackground; //0x0000
    AntRef m_ForceRender1pInForeground; //0x0014
    AntRef m_ForceAnimatedCamera; //0x0028
};//Size=0x003C

////////////////////////////////////////
// RuntimeId: 09566
// TypeInfo:  0x0000000142C89BA8
struct AimingWorldSpaceLockEfficiencyData
{
    float m_EfficiencyYaw; //0x0000
    float m_EfficiencyPitch; //0x0004
    float m_EfficiencyYawZoom; //0x0008
    float m_EfficiencyPitchZoom; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09556
// TypeInfo:  0x0000000142C89F08
struct SoldierHealthModuleBinding
{
    AntRef m_HeadShot; //0x0000
    AntRef m_LeftArmHit; //0x0014
    AntRef m_RightArmHit; //0x0028
    AntRef m_LeftLegHit; //0x003C
    AntRef m_RightLegHit; //0x0050
    AntRef m_OnGround; //0x0064
    AntRef m_DeathAnimationTriggered; //0x0078
    AntRef m_RandomAnimationIndex; //0x008C
    AntRef m_Sprinting; //0x00A0
    AntRef m_HitLeft; //0x00B4
    AntRef m_HitRight; //0x00C8
    AntRef m_HitFront; //0x00DC
    AntRef m_HitBack; //0x00F0
    AntRef m_DeathHitDirection; //0x0104
    AntRef m_Explosion; //0x0118
    AntRef m_Dead; //0x012C
    AntRef m_Revived; //0x0140
    AntRef m_RandomValue; //0x0154
    AntRef m_Pose; //0x0168
    AntRef m_RightSpeed; //0x017C
    AntRef m_ForwardSpeed; //0x0190
    AntRef m_CriticallyHit; //0x01A4
    AntRef m_InteractiveManDown; //0x01B8
    AntRef m_Health; //0x01CC
};//Size=0x01E0

////////////////////////////////////////
// RuntimeId: 09552
// TypeInfo:  0x0000000142C89BE8
struct AnimatedWeaponBinding
{
    AntRef m_Deploy; //0x0000
    AntRef m_AltDeploy; //0x0014
    AntRef m_Undeploy; //0x0028
    AntRef m_QuickSwitch; //0x003C
    AntRef m_Reload; //0x0050
    AntRef m_ReloadStage; //0x0064
    AntRef m_ReloadShotgun; //0x0078
    AntRef m_Fire; //0x008C
    AntRef m_FireSingle; //0x00A0
    AntRef m_FireHoldAndRelease; //0x00B4
    AntRef m_FireSimple; //0x00C8
    AntRef m_FirstShotSpawned; //0x00DC
    AntRef m_PreFiring; //0x00F0
    AntRef m_BoltAction; //0x0104
    AntRef m_PumpAction; //0x0118
    AntRef m_MeleeAttack; //0x012C
    AntRef m_QuickThrow; //0x0140
    AntRef m_QuickThrowType; //0x0154
    AntRef m_AimBody; //0x0168
    AntRef m_AlwaysAimHead; //0x017C
    AntRef m_OneHanded; //0x0190
    AntRef m_OneHandedAiming; //0x01A4
    AntRef m_AimingEnabled; //0x01B8
    AntRef m_LowerGun; //0x01CC
    AntRef m_BreathControl; //0x01E0
    AntRef m_RflType; //0x01F4
    AntRef m_PstlType; //0x0208
    AntRef m_HgrType; //0x021C
    AntRef m_ATType; //0x0230
    AntRef m_ShgType; //0x0244
    AntRef m_LMGType; //0x0258
    AntRef m_BagType; //0x026C
    AntRef m_SnpType; //0x0280
    AntRef m_Zoom; //0x0294
    AntRef m_CantedZoom; //0x02A8
    AntRef m_MagnifierSightToggle; //0x02BC
    AntRef m_MagnifierSightIsUp; //0x02D0
    AntRef m_AimBodyWeight; //0x02E4
    AntRef m_DisableZoomToggleWeight; //0x02F8
    AntRef m_ZoomParameter; //0x030C
    AntRef m_ZoomScaleFactor; //0x0320
    AntRef m_Dispersion; //0x0334
    AntRef m_AimTargetPosBody; //0x0348
    AntRef m_ZoomOutSpeed; //0x035C
    AntRef m_ZoomInSpeed; //0x0370
    AntRef m_UnDeploySpeed; //0x0384
    AntRef m_DeploySpeed; //0x0398
    AntRef m_LightEnabled; //0x03AC
    AntRef m_FireModeChanged; //0x03C0
    AntRef m_AnimType; //0x03D4
    AntRef m_GunDown; //0x03E8
    AntRef m_AllowSwitchingToWeaponInWater; //0x03FC
    AntRef m_NumberOfBulletsLeftInGun; //0x0410
    AntRef m_BulletsLeftInGun; //0x0424
    AntRef m_AbortVehicleDeploy; //0x0438
    AntRef m_CustomizeWeapon; //0x044C
    AntRef m_CustomizeWeaponSwitch; //0x0460
    AntRef m_WeaponActionESIG; //0x0474
    AntRef m_IsSprinting; //0x0488
    AntRef m_PreparingToBash; //0x049C
    AntRef m_JustStartedSprinting; //0x04B0
    AntRef m_KickbackMagnitudeFactor; //0x04C4
    AntRef m_KickbackSpeedFactor; //0x04D8
    AntRef m_ZoomingTime; //0x04EC
    AntRef m_TriggerZoomGunTwitch; //0x0500
    AntRef m_WeaponChooserSignal; //0x0514
    AntRef m_WeaponClassSignal; //0x0528
    AntRef m_OffsetX; //0x053C
    AntRef m_OffsetY; //0x0550
    AntRef m_OffsetZ; //0x0564
    AntRef m_AIAllowFire; //0x0578
    AntRef m_AIAltFireFromAnt; //0x058C
};//Size=0x05A0

////////////////////////////////////////
// RuntimeId: 09550
// TypeInfo:  0x0000000142C89C08
struct Animated3pOnlyWeaponBinding
{
    AntRef m_Deploy3P; //0x0000
    AntRef m_HideWeapon3p; //0x0014
    AntRef m_WeaponAssembled3p; //0x0028
};//Size=0x003C

////////////////////////////////////////
// RuntimeId: 09548
// TypeInfo:  0x0000000142C89C28
struct Animated1pOnlyWeaponBinding
{
    AntRef m_UndeployFinished; //0x0000
    AntRef m_CameraFreeWeight; //0x0014
    AntRef m_Deploy1P; //0x0028
    AntRef m_HideWeapon1p; //0x003C
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 09472
// TypeInfo:  0x0000000142C82058
struct ServerSoldierSelfHealMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 09471
// TypeInfo:  0x0000000142C82078
struct ServerSoldierManDownMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 09470
// TypeInfo:  0x0000000142C82098
struct ServerSoldierOnInitMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 09469
// TypeInfo:  0x0000000142C820B8
struct ServerSoldierChangingWeaponMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 09468
// TypeInfo:  0x0000000142C820D8
struct ServerSoldierFiringMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 09467
// TypeInfo:  0x0000000142C820F8
struct ServerSoldierDamagedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 09466
// TypeInfo:  0x0000000142C82118
struct ServerProjectileMissileDestroyedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 09465
// TypeInfo:  0x0000000142C82138
struct ServerProjectileMissileDamagedMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 09464
// TypeInfo:  0x0000000142C82158
struct ClientSoldierChangeCoverStateMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 09463
// TypeInfo:  0x0000000142C82178
struct ClientSoldierOnLandMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09462
// TypeInfo:  0x0000000142C82198
struct ClientSoldierOnJumpMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09439
// TypeInfo:  0x0000000142C81620
struct OriginErrorMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 09438
// TypeInfo:  0x0000000142C81640
struct OriginResponseMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09437
// TypeInfo:  0x0000000142C81660
struct OriginRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09358
// TypeInfo:  0x0000000142C7EE88
struct OnlineProviderConfiguration
{
    GamePlatform m_Platform; //0x0000
    char _0x0004[4];
    char* m_ServiceName; //0x0008
    char* m_Client; //0x0010
    char* m_SKU; //0x0018
    char* m_Version; //0x0020
    unsigned __int32 m_ServerSocketPacketSize; //0x0028
    bool m_IsServer; //0x002C
    char _0x002D[3];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 09356
// TypeInfo:  0x0000000142C7EEA8
struct OnlinePlatformConfiguration
{
    GamePlatform m_Platform; //0x0000
    char _0x0004[4];
    OnlineServicesAsset* m_Services; //0x0008
    Array<PresenceBackendData*> m_ClientBackends; //0x0010
    Array<ServerBackendData*> m_ServerBackends; //0x0018
    bool m_IsFallback; //0x0020
    char _0x0021[7];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09346
// TypeInfo:  0x0000000142C7EEE8
struct OnlineEnvironmentUrlData
{
    char* m_Url; //0x0000
    OnlineEnvironment m_Environment; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09348
// TypeInfo:  0x0000000142C7EDC8
struct OnlineEnvironmentUrl
{
    Array<OnlineEnvironmentUrlData> m_Urls; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 09350
// TypeInfo:  0x0000000142C7EEC8
struct OnlineEnvironmentConsoleUrlData
{
    GamePlatform m_Platform; //0x0000
    char _0x0004[4];
    OnlineEnvironmentUrl m_Url; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09352
// TypeInfo:  0x0000000142C7EDA8
struct OnlineEnvironmentConsoleUrl
{
    Array<OnlineEnvironmentConsoleUrlData> m_Urls; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 09336
// TypeInfo:  0x0000000142C7EDE8
struct BlazeCreateGameParameters
{
    GameParametersData* m_Base; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 09322
// TypeInfo:  0x0000000142C7EF08
struct Ps4CountryAgeOverrides
{
    char* m_CountryCode; //0x0000
    __int32 m_AgeRequirement; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09324
// TypeInfo:  0x0000000142C7EE08
struct Ps4AgeSettings
{
    __int32 m_DefaultAgeRequirement; //0x0000
    char _0x0004[4];
    Array<Ps4CountryAgeOverrides> m_AgeOverrides; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09320
// TypeInfo:  0x0000000142C7EE28
struct Ps4TitleData
{
    char* m_TitleId; //0x0000
    char* m_TitleSecret; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09312
// TypeInfo:  0x0000000142C7EE48
struct Ps3ServiceId
{
    char* m_SPID; //0x0000
    char* m_ProductCode; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09314
// TypeInfo:  0x0000000142C7EF28
struct Ps3ServiceSettings
{
    char* m_Region; //0x0000
    char* m_ProductCode; //0x0008
    char* m_PrimaryProductCode; //0x0010
    Ps3ServiceId m_TicketingService; //0x0018
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09304
// TypeInfo:  0x0000000142C7EE68
struct Ps3AgeLevels
{
    __int32 m_AgeLevel7; //0x0000
    __int32 m_AgeLevel8; //0x0004
    __int32 m_AgeLevel9; //0x0008
    __int32 m_AgeLevel10; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09306
// TypeInfo:  0x0000000142C7EF68
struct Ps3ParentalLockAgeSettingsForCountry
{
    char* m_CountryCode; //0x0000
    Ps3AgeLevels m_AgeLevels; //0x0008
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 09310
// TypeInfo:  0x0000000142C7EF48
struct Ps3ParentalLockAgeSettings
{
    char* m_Region; //0x0000
    Ps3AgeLevels m_AgeLevels; //0x0008
    Ps3ParentalLockAgeSettingsOverrides* m_CountryOverrides; //0x0018
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 09262
// TypeInfo:  0x0000000142C7D9C0
struct MatchmakingSizeConfiguration
{
    MatchmakingPlatform m_Platform; //0x0000
    char _0x0004[4];
    Array<char*> m_Settings; //0x0008
    unsigned __int32 m_DesiredPlayerCount; //0x0010
    unsigned __int32 m_MinPlayerCount; //0x0014
    unsigned __int32 m_MaxPlayerCapacity; //0x0018
    char _0x001C[4];
    char* m_MinFitThreshold; //0x0020
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09284
// TypeInfo:  0x0000000142C7CFE0
struct MatchmakingFreePlayerSlotsRule
{
    unsigned __int32 m_MaxFreePlayerSlots; //0x0000
    unsigned __int32 m_MinFreePlayerSlots; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 09282
// TypeInfo:  0x0000000142C7D000
struct MatchmakingSlotUtilizationRule
{
    unsigned __int32 m_PreferredPercentage; //0x0000
    unsigned __int32 m_MinPercentage; //0x0004
    unsigned __int32 m_MaxPercentage; //0x0008
    char _0x000C[4];
    char* m_RangeOffsetListName; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 09278
// TypeInfo:  0x0000000142C7D940
struct MatchmakingMod
{
    Array<char*> m_Licenses; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 09280
// TypeInfo:  0x0000000142C7D020
struct MatchmakingModRule
{
    Array<MatchmakingMod> m_Mods; //0x0000
    char* m_MinFitThresholdListName; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09276
// TypeInfo:  0x0000000142C7D960
struct MatchmakingUserExtendedDataRule
{
    char* m_Rule; //0x0000
    char* m_MinFitThresHold; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09272
// TypeInfo:  0x0000000142C7D9A0
struct MatchmakingRuleString
{
    char* m_Value; //0x0000
    MatchmakingPlatform m_Platform; //0x0008
    char _0x000C[4];
    Array<char*> m_Licenses; //0x0010
    bool m_UseOnlyIfEmpty; //0x0018
    char _0x0019[7];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 09274
// TypeInfo:  0x0000000142C7D980
struct MatchmakingGenericRule
{
    char* m_Rule; //0x0000
    char* m_MinFitThresHold; //0x0008
    char* m_Setting; //0x0010
    Array<MatchmakingRuleString> m_DesiredValues; //0x0018
    bool m_IgnoreIfDefault; //0x0020
    bool m_MergeValues; //0x0021
    bool m_SortValues; //0x0022
    char _0x0023[5];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09270
// TypeInfo:  0x0000000142C7D040
struct MatchmakingRankedRule
{
    char* m_MinFitThreshold; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 09268
// TypeInfo:  0x0000000142C7D060
struct MatchmakingPingSiteRule
{
    char* m_MinFitThreshold; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 09264
// TypeInfo:  0x0000000142C7D0A0
struct MatchmakingSizeRule
{
    char* m_Setting; //0x0000
    Array<MatchmakingSizeConfiguration> m_Configurations; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09260
// TypeInfo:  0x0000000142C7D0C0
struct MatchmakingVirtualizedRule
{
    char* m_MinFitThreshold; //0x0000
    MatchmakingVirtualizationMode m_VirtualizationMode; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09286
// TypeInfo:  0x0000000142C7CFC0
struct MatchmakingCriteria
{
    MatchmakingSizeRule m_SizeRule; //0x0000
    MatchmakingPingSiteRule m_PingSiteRule; //0x0010
    MatchmakingRankedRule m_RankedRule; //0x0018
    Array<MatchmakingGenericRule> m_GenericRules; //0x0020
    MatchmakingVirtualizedRule m_VirtualizedRule; //0x0028
    Array<MatchmakingUserExtendedDataRule> m_UEDRules; //0x0038
    MatchmakingModRule m_ModRule; //0x0040
    MatchmakingSlotUtilizationRule m_SlotUtilizationRule; //0x0050
    MatchmakingFreePlayerSlotsRule m_FreePlayerSlotsRule; //0x0068
};//Size=0x0070

////////////////////////////////////////
// RuntimeId: 09254
// TypeInfo:  0x0000000142C7D0E0
struct ServerBackendEjectHostMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09253
// TypeInfo:  0x0000000142C7D100
struct ServerBackendStatsDownloadFailMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09252
// TypeInfo:  0x0000000142C7D120
struct ServerGameManagerAvailableMapsChangedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09251
// TypeInfo:  0x0000000142C7D140
struct ServerGameManagerKickPlayerMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 09250
// TypeInfo:  0x0000000142C7D160
struct ServerGameManagerAddQueuedPlayerMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 09249
// TypeInfo:  0x0000000142C7D180
struct ServerGameManagerValidateBannerResponseMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 09248
// TypeInfo:  0x0000000142C7D1A0
struct ServerGameManagerValidateBannerRequestMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 09247
// TypeInfo:  0x0000000142C7D1C0
struct ServerGameManagerAddGameBanResponseMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 09246
// TypeInfo:  0x0000000142C7D1E0
struct ServerGameManagerAddGameBanRequestMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 09245
// TypeInfo:  0x0000000142C7D200
struct ServerGameManagerGetGameDataMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09244
// TypeInfo:  0x0000000142C7D220
struct ServerGameManagerRestartLevelRequestMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09243
// TypeInfo:  0x0000000142C7D240
struct ServerGameManagerGameDestructingMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09242
// TypeInfo:  0x0000000142C7D260
struct ServerGameManagerPlayerRemovedMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09241
// TypeInfo:  0x0000000142C7D280
struct ServerGameManagerPlayerJoiningQueueMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 09240
// TypeInfo:  0x0000000142C7D2A0
struct ServerGameManagerCheckPlayerStatusMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 09239
// TypeInfo:  0x0000000142C7D2C0
struct ServerGameManagerPlayerJoiningMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09238
// TypeInfo:  0x0000000142C7D2E0
struct ServerGameManagerUpdateCapacityMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09237
// TypeInfo:  0x0000000142C7D300
struct ServerGameManagerGameParametersChangedMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09236
// TypeInfo:  0x0000000142C7D320
struct ServerGameManagerChangeGameParametersMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09235
// TypeInfo:  0x0000000142C7D340
struct ServerGameManagerReconfigurableGameCreatedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09234
// TypeInfo:  0x0000000142C7D360
struct ServerGameManagerCreatingReconfigurableGameMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09233
// TypeInfo:  0x0000000142C7D380
struct ServerGameManagerGameResetMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09232
// TypeInfo:  0x0000000142C7D3A0
struct ServerGameManagerGameCreatedMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09231
// TypeInfo:  0x0000000142C7D3C0
struct ServerGameManagerCreatingGameMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09230
// TypeInfo:  0x0000000142C7D3E0
struct ServerBackendFinalizingMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09229
// TypeInfo:  0x0000000142C7D400
struct ServerBackendDownloadCompleteMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09226
// TypeInfo:  0x0000000142C7D9E0
struct LicenseInfo
{
    char* m_Name; //0x0000
    bool m_ClientOnly; //0x0008
    char _0x0009[7];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09216
// TypeInfo:  0x0000000142C7DA00
struct GameAttributeData
{
    char* m_Attribute; //0x0000
    char* m_Value; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 09210
// TypeInfo:  0x0000000142C7D460
struct PresenceUserProfileRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09209
// TypeInfo:  0x0000000142C7D480
struct PresenceUserProfileMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09208
// TypeInfo:  0x0000000142C7D4A0
struct PresenceUserIdRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09207
// TypeInfo:  0x0000000142C7D4C0
struct PresenceUserIdMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09206
// TypeInfo:  0x0000000142C7D4E0
struct PresenceStorageRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09205
// TypeInfo:  0x0000000142C7D500
struct PresenceStorageMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09204
// TypeInfo:  0x0000000142C7D520
struct PresenceProfileInfoRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09203
// TypeInfo:  0x0000000142C7D540
struct PresenceProfileInfoMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09202
// TypeInfo:  0x0000000142C7D560
struct PresencePrivilegeRequestResultMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09201
// TypeInfo:  0x0000000142C7D580
struct PresencePrivilegeRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09200
// TypeInfo:  0x0000000142C7D5A0
struct PresencePlaygroupRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09199
// TypeInfo:  0x0000000142C7D5C0
struct PresencePlaygroupAttributesMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09198
// TypeInfo:  0x0000000142C7D5E0
struct PresencePlaygroupMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09197
// TypeInfo:  0x0000000142C7D600
struct PresenceOverlayMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09196
// TypeInfo:  0x0000000142C7D620
struct PresenceMatchmakerMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09195
// TypeInfo:  0x0000000142C7D640
struct PresenceLivePartyMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09194
// TypeInfo:  0x0000000142C7D660
struct PresenceLicenseMetricMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09193
// TypeInfo:  0x0000000142C7D680
struct PresenceLicenseRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09192
// TypeInfo:  0x0000000142C7D6A0
struct PresenceLicenseMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09191
// TypeInfo:  0x0000000142C7D6C0
struct PresenceJoinResultMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09190
// TypeInfo:  0x0000000142C7D6E0
struct PresenceGameRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09189
// TypeInfo:  0x0000000142C7D700
struct PresenceGameQueueMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09188
// TypeInfo:  0x0000000142C7D720
struct PresenceGameMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09187
// TypeInfo:  0x0000000142C7D740
struct PresenceInviteRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09186
// TypeInfo:  0x0000000142C7D760
struct PresenceInviteMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09185
// TypeInfo:  0x0000000142C7D780
struct PresenceFriendsListManagerSettingsMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09184
// TypeInfo:  0x0000000142C7D7A0
struct PresenceFriendRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09183
// TypeInfo:  0x0000000142C7D7C0
struct PresenceFriendMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09182
// TypeInfo:  0x0000000142C7D7E0
struct PresenceConnectionRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09181
// TypeInfo:  0x0000000142C7D800
struct PresenceConnectionMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09180
// TypeInfo:  0x0000000142C7D820
struct PresenceBrowserRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09179
// TypeInfo:  0x0000000142C7D840
struct PresenceBlockListRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09178
// TypeInfo:  0x0000000142C7D860
struct PresenceBlockListMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09177
// TypeInfo:  0x0000000142C7D880
struct PresenceBlobRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09176
// TypeInfo:  0x0000000142C7D8A0
struct PresenceBlobMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09175
// TypeInfo:  0x0000000142C7D8C0
struct PresenceAuthenticationRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09174
// TypeInfo:  0x0000000142C7D8E0
struct PresenceAuthenticationMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09173
// TypeInfo:  0x0000000142C7D900
struct PresenceAchievementRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09172
// TypeInfo:  0x0000000142C7D920
struct OnlineInternalGameMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09168
// TypeInfo:  0x0000000142C7B550
struct PresenceNickelRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09167
// TypeInfo:  0x0000000142C7B570
struct PresenceNickelMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 09111
// TypeInfo:  0x0000000142C78FA8
struct SkyCloudLayer
{
    Vec3 m_Color; //0x0000
    float m_Altitude; //0x0010
    float m_TileFactor; //0x0014
    float m_Rotation; //0x0018
    float m_Speed; //0x001C
    float m_SunLightIntensity; //0x0020
    float m_SunLightPower; //0x0024
    float m_AmbientLightIntensity; //0x0028
    float m_AlphaMul; //0x002C
    TextureAsset* m_Texture; //0x0030
    char _0x0038[8];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 03698
// TypeInfo:  0x00000001423057D0
struct Vec2
{
    float m_x; //0x0000
    float m_y; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 09033
// TypeInfo:  0x0000000142C78FC8
struct GroundHeightData
{
    float m_WorldSize; //0x0000
    Vec2 m_HeightSpan; //0x0004
    char _0x000C[4];
    Array<unsigned __int16> m_Data; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 08962
// TypeInfo:  0x0000000142C76010
struct VegetationEffectSlot
{
    EffectBlueprint* m_Effect; //0x0000
    float m_StrengthMin; //0x0008
    float m_StrengthMax; //0x000C
    float m_SizeMin; //0x0010
    float m_SizeMax; //0x0014
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 08953
// TypeInfo:  0x0000000142C73ED8
struct UIPremiumContentUpdateMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08952
// TypeInfo:  0x0000000142C73EF8
struct UIComponentDeferActionMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08951
// TypeInfo:  0x0000000142C73F18
struct UIComponentOnItemChangedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08950
// TypeInfo:  0x0000000142C73F38
struct UIComponentChangeCameraViewPointMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 08949
// TypeInfo:  0x0000000142C73F58
struct UIComponentSetSettingMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 08948
// TypeInfo:  0x0000000142C73F78
struct UIComponentLoadCompleteMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08947
// TypeInfo:  0x0000000142C73F98
struct UIReadyForUnloadMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08946
// TypeInfo:  0x0000000142C73FB8
struct UISoldierChangeMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08943
// TypeInfo:  0x0000000142C73FF8
struct UIAssetChangedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08942
// TypeInfo:  0x0000000142C74018
struct UIInputPressedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08941
// TypeInfo:  0x0000000142C74038
struct UIGraphExitedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08940
// TypeInfo:  0x0000000142C74058
struct UIScreenLoadedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08939
// TypeInfo:  0x0000000142C74078
struct UIFirstPartyUIMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08938
// TypeInfo:  0x0000000142C74098
struct UITransitionEffectStopMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08937
// TypeInfo:  0x0000000142C740B8
struct UITransitionEffectStartMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08936
// TypeInfo:  0x0000000142C740D8
struct UIExitToMenuReasonMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08935
// TypeInfo:  0x0000000142C740F8
struct UISpawnAllowedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08934
// TypeInfo:  0x0000000142C74118
struct UIVehicleHitUpdatedMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 08933
// TypeInfo:  0x0000000142C74138
struct UISquadStatusChangedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08932
// TypeInfo:  0x0000000142C74158
struct UISoldierHitUpdatedMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 08931
// TypeInfo:  0x0000000142C74178
struct UIReturnMouseToUIMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08930
// TypeInfo:  0x0000000142C74198
struct UIPlayerVehicleHealthChangeMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08929
// TypeInfo:  0x0000000142C741B8
struct UILevelWarmUpTimerMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08928
// TypeInfo:  0x0000000142C741D8
struct UIInputStatusChangedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08927
// TypeInfo:  0x0000000142C741F8
struct UIHudWarningMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08926
// TypeInfo:  0x0000000142C74218
struct UIHudUpdateCrosshairMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08925
// TypeInfo:  0x0000000142C74238
struct UIHudToggleMapZoomMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08924
// TypeInfo:  0x0000000142C74258
struct UIHudShowVoteResultsMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08923
// TypeInfo:  0x0000000142C74278
struct UIHudShowVoteMenuMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08922
// TypeInfo:  0x0000000142C74298
struct UIHudOutputStaticMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08921
// TypeInfo:  0x0000000142C742B8
struct UIHudDistortHudMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08920
// TypeInfo:  0x0000000142C742D8
struct UIHudDebugPauseMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08919
// TypeInfo:  0x0000000142C742F8
struct UIHudChatMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 08918
// TypeInfo:  0x0000000142C74318
struct UIHudChangeInventoryWeaponMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08917
// TypeInfo:  0x0000000142C74338
struct UIHasSuppressedEnemyMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08916
// TypeInfo:  0x0000000142C74358
struct UIDamageGivenToEnemyMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 08915
// TypeInfo:  0x0000000142C74378
struct UICycleRadioChannelMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08914
// TypeInfo:  0x0000000142C74398
struct UIMatchImagesRetrievedMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08913
// TypeInfo:  0x0000000142C743B8
struct UIRequestMatchImagesMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08912
// TypeInfo:  0x0000000142C743D8
struct UIVKBDInputDoneMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 08911
// TypeInfo:  0x0000000142C743F8
struct UISubtitleMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 08910
// TypeInfo:  0x0000000142C74418
struct UIMessageEntityMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 08909
// TypeInfo:  0x0000000142C74438
struct UIScreenCountChangeMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08908
// TypeInfo:  0x0000000142C74458
struct UIControllerDisconnectedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08907
// TypeInfo:  0x0000000142C74478
struct UIControllerConnectedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08906
// TypeInfo:  0x0000000142C74498
struct UIUserDisconnectedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08905
// TypeInfo:  0x0000000142C744B8
struct UIUserConnectedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08904
// TypeInfo:  0x0000000142C744D8
struct UIUserNotificationMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08903
// TypeInfo:  0x0000000142C744F8
struct UIUserSkippedLoginMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08902
// TypeInfo:  0x0000000142C74518
struct MemoryCardBootCheckMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08901
// TypeInfo:  0x0000000142C74538
struct MemoryCardFindEntriesDoneMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08900
// TypeInfo:  0x0000000142C74558
struct MemoryCardFindEntriesMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08899
// TypeInfo:  0x0000000142C74578
struct MemoryCardSaveDoneMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08898
// TypeInfo:  0x0000000142C74598
struct MemoryCardSaveMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08897
// TypeInfo:  0x0000000142C745B8
struct MemoryCardLoadDoneMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08896
// TypeInfo:  0x0000000142C745D8
struct MemoryCardLoadMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08895
// TypeInfo:  0x0000000142C745F8
struct MemoryCardDestroyedMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08894
// TypeInfo:  0x0000000142C74618
struct MemoryCardInitializedMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08893
// TypeInfo:  0x0000000142C74638
struct MemoryCardCreatedMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08892
// TypeInfo:  0x0000000142C74658
struct MemoryCardPopupHideMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08891
// TypeInfo:  0x0000000142C74678
struct MemoryCardPopupResponseMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08890
// TypeInfo:  0x0000000142C74698
struct MemoryCardPopupRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08875
// TypeInfo:  0x0000000142C74818
struct UIPopupButton
{
    UIInputAction m_InputConcept; //0x0000
    char _0x0004[4];
    char* m_Label; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 08827
// TypeInfo:  0x0000000142C74838
struct WidgetEventQueryPair
{
    char* m_Name; //0x0000
    UIWidgetEventID m_Query; //0x0008
    char _0x000C[4];
    char* m_InstanceName; //0x0010
    bool m_IsOutput; //0x0018
    char _0x0019[7];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 08817
// TypeInfo:  0x0000000142C74858
struct UIDataSourceInfo
{
    char* m_DataName; //0x0000
    UIComponentData* m_DataCategory; //0x0008
    __int32 m_DataKey; //0x0010
    bool m_UseDirectAccess; //0x0014
    bool m_UpdateOnInitialize; //0x0015
    char _0x0016[2];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 08815
// TypeInfo:  0x0000000142C746B8
struct UISimpleDataSource
{
    UIComponentData* m_DataCategory; //0x0000
    __int32 m_DataKey; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 08811
// TypeInfo:  0x0000000142C74878
struct UIWidgetProperty
{
    char* m_Name; //0x0000
    char* m_Value; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 08797
// TypeInfo:  0x0000000142C74898
struct InterruptFlow
{
    UIInterruptID m_interruptEnum; //0x0000
    char _0x0004[4];
    UIGraphAsset* m_interruptFlow; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 08793
// TypeInfo:  0x0000000142C748B8
struct UIAudioEventMapping
{
    char* m_EventName; //0x0000
    SoundAsset* m_SoundAsset; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 08773
// TypeInfo:  0x0000000142C748D8
struct UIResourceTableEntry
{
    // unhandled basic type ResourceRef m_Resource; //0x0000
    char _0x0000[8];
    unsigned __int32 m_Hash; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 08765
// TypeInfo:  0x0000000142C748F8
struct UIFontMapping
{
    Array<char*> m_ScaleformFontName; //0x0000
    char* m_FontLongName; //0x0008
    bool m_Bold; //0x0010
    char _0x0011[7];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 08759
// TypeInfo:  0x0000000142C74918
struct FontCollectionLookupEntry
{
    char* m_CollectionBasePath; //0x0000
    LanguageFormat m_Language; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 08757
// TypeInfo:  0x0000000142C74938
struct UIBundleAssetState
{
    char* m_StateName; //0x0000
    UIState m_UIState; //0x0008
    char _0x000C[4];
    char* m_ResourceBundleName; //0x0010
    char* m_StatePath; //0x0018
    char* m_BundlePath; //0x0020
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08695
// TypeInfo:  0x0000000142C6FA00
struct TerrainQuadDecalAtlasTile
{
    unsigned __int32 m_TileIndexX; //0x0000
    unsigned __int32 m_TileIndexY; //0x0004
    unsigned __int32 m_TileCountX; //0x0008
    unsigned __int32 m_TileCountY; //0x000C
    bool m_FlipX; //0x0010
    bool m_FlipY; //0x0011
    char _0x0012[2];
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 08683
// TypeInfo:  0x0000000142C6FBC0
struct RibbonPointData
{
    Vec4 m_UserMaskLeft; //0x0000
    Vec4 m_UserMaskRight; //0x0010
    float m_Left; //0x0020
    float m_Right; //0x0024
    char _0x0028[8];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08675
// TypeInfo:  0x0000000142C6FBE0
struct TerrainLayerShaderData
{
    char _0x0000[1];
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 08673
// TypeInfo:  0x0000000142C6FC00
struct MeshScatteringSpawnData
{
    char _0x0000[1];
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 08659
// TypeInfo:  0x0000000142C6FC20
struct TerrainLayerMaskData
{
    char _0x0000[1];
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 08647
// TypeInfo:  0x0000000142C6FC40
struct TerrainLayerCombinationDrawData
{
    char _0x0000[1];
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 08645
// TypeInfo:  0x0000000142C6FC60
struct Surface3dDrawMethodData
{
    char _0x0000[1];
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 08643
// TypeInfo:  0x0000000142C6FC80
struct MeshScatteringMaskScaleDrawMethodData
{
    char _0x0000[1];
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 08641
// TypeInfo:  0x0000000142C6FCA0
struct Surface2dDrawMethodData
{
    char _0x0000[1];
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 08639
// TypeInfo:  0x0000000142C6FCC0
struct Surface2dDrawPassData
{
    char _0x0000[1];
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 08637
// TypeInfo:  0x0000000142C6FCE0
struct TerrainLayerCombinationDrawPassData
{
    char _0x0000[1];
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 08621
// TypeInfo:  0x0000000142C6FD00
struct TerrainLayerProceduralMask
{
    float m_AltitudeMin; //0x0000
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 08619
// TypeInfo:  0x0000000142C6FD20
struct TerrainGeoTexture
{
    char _0x0000[1];
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 08575
// TypeInfo:  0x0000000142C6FD40
struct RectangularCoverageData
{
    char _0x0000[1];
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 08559
// TypeInfo:  0x0000000142C6C618
struct PerformanceClientMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08558
// TypeInfo:  0x0000000142C6C638
struct PerformanceShaderMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08507
// TypeInfo:  0x0000000142C6CBD8
struct TerrainShaderParameter
{
    unsigned __int32 m_ParameterHandle; //0x0000
    TerrainShaderParameterDataType m_ParameterType; //0x0004
    char* m_ParameterName; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 08455
// TypeInfo:  0x0000000142C6CC58
struct BoolShaderParameter
{
    char* m_ParameterName; //0x0000
    bool m_Value; //0x0008
    char _0x0009[7];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 08461
// TypeInfo:  0x0000000142C6CBF8
struct VectorArrayShaderParameter
{
    char* m_ParameterName; //0x0000
    ShaderParameterType m_ParameterType; //0x0008
    char _0x000C[4];
    Array<Vec4> m_Values; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 08459
// TypeInfo:  0x0000000142C6CC18
struct TextureShaderParameter
{
    char* m_ParameterName; //0x0000
    TextureBaseAsset* m_Value; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 08457
// TypeInfo:  0x0000000142C6CC38
struct VectorShaderParameter
{
    Vec4 m_Value; //0x0000
    char* m_ParameterName; //0x0010
    ShaderParameterType m_ParameterType; //0x0018
    char _0x001C[4];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 08467
// TypeInfo:  0x0000000142C6CA98
struct SurfaceShaderInstanceDataStruct
{
    SurfaceShaderBaseAsset* m_Shader; //0x0000
    Array<BoolShaderParameter> m_BoolParameters; //0x0008
    Array<VectorShaderParameter> m_VectorParameters; //0x0010
    Array<VectorArrayShaderParameter> m_VectorArrayParameters; //0x0018
    Array<TextureShaderParameter> m_TextureParameters; //0x0020
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 08439
// TypeInfo:  0x0000000142C6CAD8
struct PoissonRadialBlurData
{
    float m_BlendFactor; //0x0000
    Vec2 m_PoissonDiscScale; //0x0004
    float m_RadialScale; //0x000C
    float m_RadialExponent; //0x0010
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 08437
// TypeInfo:  0x0000000142C6CAF8
struct ColorTintData
{
    Vec3 m_Contrast; //0x0000
    Vec3 m_Brightness; //0x0010
    Vec3 m_Saturation; //0x0020
    float m_Hue; //0x0030
    char _0x0034[12];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 08385
// TypeInfo:  0x0000000142C6CC78
struct AnimatedPointCloudAttributeDesc
{
    PointCloudAttributeUsage m_Usage; //0x0000
    PointCloudAttributeQuantization m_IFrameQuantization; //0x0004
    PointCloudAttributeQuantization m_DFrameQuantization; //0x0008
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 08383
// TypeInfo:  0x0000000142C6CC98
struct VariationLink
{
    unsigned __int64 m_VariationKey; //0x0000
    unsigned __int32 m_ObjectVariationHash; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 08375
// TypeInfo:  0x0000000142C6CCB8
struct MeshVariationDatabaseRedirectEntry
{
    MeshAsset* m_Mesh; //0x0000
    unsigned __int32 m_VariationAssetNameHash; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 08371
// TypeInfo:  0x0000000142C6CCF8
struct MeshVariationDatabaseMaterial
{
    MeshMaterial* m_Material; //0x0000
    MeshMaterialVariation* m_MaterialVariation; //0x0008
    Array<TextureShaderParameter> m_TextureParameters; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 08373
// TypeInfo:  0x0000000142C6CCD8
struct MeshVariationDatabaseEntry
{
    MeshAsset* m_Mesh; //0x0000
    unsigned __int32 m_VariationAssetNameHash; //0x0008
    char _0x000C[4];
    Array<MeshVariationDatabaseMaterial> m_Materials; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 08324
// TypeInfo:  0x0000000142C690E8
struct DecalAtlasTile
{
    float m_TileIndexX; //0x0000
    float m_TileIndexY; //0x0004
    float m_TileCountX; //0x0008
    float m_TileCountY; //0x000C
    bool m_FlipX; //0x0010
    bool m_FlipY; //0x0011
    char _0x0012[2];
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 03712
// TypeInfo:  0x0000000142305890
struct AxisAlignedBox
{
    Vec3 m_min; //0x0000
    Vec3 m_max; //0x0010
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 08179
// TypeInfo:  0x0000000142C65348
struct PhysicsComponentOnImpulseMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08178
// TypeInfo:  0x0000000142C65368
struct PhysicsComponentOnDamageMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 08157
// TypeInfo:  0x0000000142C654C8
struct AssetAabbs
{
    Array<AxisAlignedBox> m_PartAabb; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 08149
// TypeInfo:  0x0000000142C65388
struct EndPointData
{
    float m_Pos; //0x0000
    float m_EndDamping; //0x0004
    float m_SpringLength; //0x0008
    float m_SpringAcceleration; //0x000C
    float m_SpringDamping; //0x0010
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 08133
// TypeInfo:  0x0000000142C654E8
struct FrictionScaleAtVelocity
{
    float m_FrictionScale; //0x0000
    float m_Velocity; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 08131
// TypeInfo:  0x0000000142C65508
struct SensitivityAtVelocity
{
    float m_SteeringSensitivity; //0x0000
    float m_Velocity; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 08129
// TypeInfo:  0x0000000142C653A8
struct SpringData
{
    float m_Length; //0x0000
    float m_Stiffness; //0x0004
    float m_Damping; //0x0008
    float m_ProgressiveStartRatio; //0x000C
    float m_ProgressiveExponent; //0x0010
    float m_VisualClipOffset; //0x0014
    float m_AttachOffsetY; //0x0018
    float m_DisabledStrenghModifier; //0x001C
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 08127
// TypeInfo:  0x0000000142C653C8
struct SphereCollisionData
{
    float m_ExtraRadius; //0x0000
    float m_CounterNormalBrakeForceMod; //0x0004
    bool m_Enabled; //0x0008
    char _0x0009[3];
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 08111
// TypeInfo:  0x0000000142C653E8
struct Boost
{
    float m_ForwardStrength; //0x0000
    float m_ReverseStrength; //0x0004
    float m_DissipationTime; //0x0008
    float m_RecoveryTime; //0x000C
    float m_CrawlStrength; //0x0010
    float m_AccelerationScale; //0x0014
    bool m_ToggleIfOnPress; //0x0018
    char _0x0019[3];
};//Size=0x001C

////////////////////////////////////////
// RuntimeId: 08079
// TypeInfo:  0x0000000142C65408
struct InputThrottle
{
    float m_ForwardSpeedSupressionAmount; //0x0000
    float m_BackwardSpeedSupressionAmount; //0x0004
    float m_SideSpeedSupressionAmount; //0x0008
    float m_IgnoreBrakeSpeedThreshold; //0x000C
    bool m_Enabled; //0x0010
    char _0x0011[3];
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 08077
// TypeInfo:  0x0000000142C65528
struct ConstantForceData
{
    Vec3 m_Value; //0x0000
    ForceCondition m_Condition; //0x0010
    ForceType m_TypeOfForce; //0x0014
    SpaceType m_Space; //0x0018
    char _0x001C[4];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 08069
// TypeInfo:  0x0000000142C65548
struct StabilizerSettings
{
    StabilizerProperty m_Property; //0x0000
    float m_Strength; //0x0004
    float m_Radius; //0x0008
    bool m_Advanced; //0x000C
    bool m_Use2DRadiusTest; //0x000D
    bool m_UseInputOverride; //0x000E
    char _0x000F[1];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 08067
// TypeInfo:  0x0000000142C65428
struct AntiRollBars
{
    AntiRollBar* m_Front; //0x0000
    AntiRollBar* m_Rear; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 08063
// TypeInfo:  0x0000000142C65448
struct VehicleInputData
{
    float m_ThrottleDeadzone; //0x0000
    float m_BrakeDeadzone; //0x0004
    float m_YawDeadzone; //0x0008
    float m_PitchDeadzone; //0x000C
    float m_RollDeadzone; //0x0010
    float m_ThrottleInertiaOutDuration; //0x0014
    float m_ThrottleInertiaInDuration; //0x0018
    float m_ThrottleInertiaMinRatio; //0x001C
    float m_BrakeInertiaOutDuration; //0x0020
    float m_BrakeInertiaInDuration; //0x0024
    float m_BrakeInertiaMinRatio; //0x0028
    float m_YawInertiaOutDuration; //0x002C
    float m_YawInertiaInDuration; //0x0030
    float m_YawInertiaMinRatio; //0x0034
    float m_PitchInertiaOutDuration; //0x0038
    float m_PitchInertiaInDuration; //0x003C
    float m_PitchInertiaMinRatio; //0x0040
    float m_RollInertiaOutDuration; //0x0044
    float m_RollInertiaInDuration; //0x0048
    float m_RollInertiaMinRatio; //0x004C
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 08031
// TypeInfo:  0x0000000142C65468
struct SpeedModifierData
{
    float m_ForwardConstant; //0x0000
    float m_BackwardConstant; //0x0004
    float m_LeftConstant; //0x0008
    float m_RightConstant; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 08027
// TypeInfo:  0x0000000142C65568
struct PoseTransitionTime
{
    CharacterPoseType m_ToPose; //0x0000
    float m_TransitionTime; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 08021
// TypeInfo:  0x0000000142C65588
struct CharacterPoseConstraintsData
{
    bool m_StandPose; //0x0000
    bool m_CrouchPose; //0x0001
    bool m_PronePose; //0x0002
};//Size=0x0003

////////////////////////////////////////
// RuntimeId: 08019
// TypeInfo:  0x0000000142C65488
struct LookConstraintsData
{
    float m_MinLookYaw; //0x0000
    float m_MaxLookYaw; //0x0004
    float m_MinLookPitch; //0x0008
    float m_MaxLookPitch; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 08007
// TypeInfo:  0x0000000142C62890
struct EdgeModelInstance
{
    LinearTransform m_Transform; //0x0000
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 07991
// TypeInfo:  0x0000000142C628D0
struct ClothStateSetupTransitionLookup
{
    unsigned __int32 m_FirstTransitionableLodIndex; //0x0000
    unsigned __int32 m_TransitionableLodCount; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 07993
// TypeInfo:  0x0000000142C628B0
struct ClothStateSetupTransition
{
    unsigned __int32 m_TransitionLodIndex; //0x0000
    unsigned __int32 m_TransitionLodStateIndex; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 07995
// TypeInfo:  0x0000000142C62870
struct ClothStatesSetup
{
    Array<unsigned __int32> m_States; //0x0000
    Array<ClothStateSetupTransitionLookup> m_StatesTransitionLookups; //0x0008
    Array<ClothStateSetupTransition> m_StatesTransitions; //0x0010
    unsigned __int32 m_EmptyState; //0x0018
    char _0x001C[4];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 07989
// TypeInfo:  0x0000000142C628F0
struct ClothBoneTableEntry
{
    char* m_BoneName; //0x0000
    unsigned __int32 m_TransformIndex; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 07987
// TypeInfo:  0x0000000142C62910
struct ClothSectionMapping
{
    char* m_ClothMeshName; //0x0000
    Array<unsigned __int32> m_SubsetIds; //0x0008
    unsigned __int32 m_LodIndex; //0x0010
    char _0x0014[4];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 07975
// TypeInfo:  0x0000000142C61FA8
struct SpikeInternalMessagePartMessage
{
    char _0x0000[120];
};//Size=0x0078

////////////////////////////////////////
// RuntimeId: 07974
// TypeInfo:  0x0000000142C61FC8
struct SpikeInternalMessageWrapperMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 07969
// TypeInfo:  0x0000000142C61ED0
struct CoreDemoStatusMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 07966
// TypeInfo:  0x0000000142C61D50
struct MovieAsyncUnloadMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 07961
// TypeInfo:  0x0000000142C61220
struct AntAnimatableComponentMeshData
{
    AntAnimatableComponentMeshRenderType m_MeshRenderType; //0x0000
    AntAnimatableComponentMeshRenderContext m_MeshRenderContext; //0x0004
    MeshAsset* m_Mesh; //0x0008
    float m_CameraRelativeScaleX; //0x0010
    float m_CameraRelativeScaleY; //0x0014
    float m_CameraRelativeScaleZ; //0x0018
    float m_CameraRelativeOffsetX; //0x001C
    float m_CameraRelativeOffsetY; //0x0020
    float m_CameraRelativeOffsetZ; //0x0024
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 03891
// TypeInfo:  0x0000000142C00580
struct AntAnimatableData
{
    AntRef m_Actor; //0x0000
    AntRef m_SceneOpMatrix; //0x0014
    ProceduralAwarenessAntRefs* m_ProceduralAwareness; //0x0028
    AntRef m_RightHandEffectorDisableOverride; //0x0030
    AntRef m_LeftHandEffectorDisableOverride; //0x0044
    MasterSkeletonAsset* m_MasterSkeletonAsset; //0x0058
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 07947
// TypeInfo:  0x0000000142C611C0
struct LodBinding
{
    AntRef m_DisableControllerUpdate; //0x0000
    AntRef m_DisablePoseUpdate; //0x0014
    AntRef m_DistanceFromCamera; //0x0028
    AntRef m_AnimatableInstanceId; //0x003C
    AntRef m_ResetController; //0x0050
};//Size=0x0064

////////////////////////////////////////
// RuntimeId: 07939
// TypeInfo:  0x0000000142C61240
struct PoseConstraintsData
{
    bool m_StandPose; //0x0000
    bool m_CrouchPose; //0x0001
    bool m_PronePose; //0x0002
};//Size=0x0003

////////////////////////////////////////
// RuntimeId: 07937
// TypeInfo:  0x0000000142C61260
struct AimingConstraintsData
{
    float m_MinYaw; //0x0000
    float m_MaxYaw; //0x0004
    float m_MinPitch; //0x0008
    float m_MaxPitch; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 07885
// TypeInfo:  0x0000000142C61280
struct DataVisualizerValue
{
    char* m_Value; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 07869
// TypeInfo:  0x0000000142C5F2C0
struct WeaponModifierCore
{
    __int32 m_DummyToMakeFrostEDcompile; //0x0000
    char _0x0004[140];
};//Size=0x0090

////////////////////////////////////////
// RuntimeId: 07851
// TypeInfo:  0x0000000142C5F2E0
struct WeaponMiscModifierSettings
{
    bool m_EnableBreathControl; //0x0000
    bool m_CanBeInSupportedShooting; //0x0001
    bool m_UnZoomOnBoltAction; //0x0002
    bool m_HoldBoltActionUntilZoomRelease; //0x0003
    bool m_IsSilenced; //0x0004
};//Size=0x0005

////////////////////////////////////////
// RuntimeId: 07835
// TypeInfo:  0x0000000142C5F020
struct ArtilleryDispersionData
{
    float m_MaxDispersion; //0x0000
    float m_DispersionAimMove; //0x0004
    float m_DispersionAimMoveThreshold; //0x0008
    float m_DispersionFiring; //0x000C
    float m_DispersionDeployment; //0x0010
    float m_DecreasePerSecond; //0x0014
    bool m_DispersionActive; //0x0018
    char _0x0019[3];
};//Size=0x001C

////////////////////////////////////////
// RuntimeId: 07817
// TypeInfo:  0x0000000142C5F060
struct RumbleFiringData
{
    float m_LowRumble; //0x0000
    float m_HighRumble; //0x0004
    float m_RumbleDuration; //0x0008
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 07807
// TypeInfo:  0x0000000142C5F0E0
struct FiringDispersionData
{
    float m_MinAngle; //0x0000
    float m_MaxAngle; //0x0004
    float m_IncreasePerShot; //0x0008
    float m_DecreasePerSecond; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 07809
// TypeInfo:  0x0000000142C5F0C0
struct FireEffectData
{
    Vec3 m_Offset; //0x0000
    Vec3 m_Rotation; //0x0010
    Vec3 m_ZoomOffset; //0x0020
    Vec3 m_ZoomRotation; //0x0030
    EffectBlueprint* m_Effect; //0x0040
    bool m_UseZoomOffset; //0x0048
    bool m_UseZoomRotation; //0x0049
    bool m_DisableDuringZoom; //0x004A
    bool m_UpdateTransform; //0x004B
    bool m_StopLoopingEffects; //0x004C
    char _0x004D[3];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 07811
// TypeInfo:  0x0000000142C5F0A0
struct OverHeatData
{
    FireEffectData m_OverHeatEffect; //0x0000
    float m_HeatPerBullet; //0x0050
    float m_HeatDropPerSecond; //0x0054
    float m_OverHeatPenaltyTime; //0x0058
    float m_OverHeatThreshold; //0x005C
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 07813
// TypeInfo:  0x0000000142C5F080
struct WeaponDispersion
{
    FiringDispersionData m_StandDispersion; //0x0000
    FiringDispersionData m_CrouchDispersion; //0x0010
    FiringDispersionData m_ProneDispersion; //0x0020
    float m_JumpDispersionAngle; //0x0030
    float m_ProneTransitionDispersionAngle; //0x0034
    float m_MoveDispersionAngle; //0x0038
    float m_MoveZoomedDispersionAngle; //0x003C
    float m_DecreasePerSecond; //0x0040
};//Size=0x0044

////////////////////////////////////////
// RuntimeId: 07793
// TypeInfo:  0x0000000142C5F160
struct HoldAndReleaseData
{
    float m_MaxHoldTime; //0x0000
    float m_MinPowerModifier; //0x0004
    float m_MaxPowerModifier; //0x0008
    float m_PowerIncreasePerSecond; //0x000C
    float m_Delay; //0x0010
    float m_KilledHoldingPowerModifier; //0x0014
    bool m_ForceFireWhenKilledHolding; //0x0018
    char _0x0019[3];
};//Size=0x001C

////////////////////////////////////////
// RuntimeId: 07803
// TypeInfo:  0x0000000142C5F120
struct RecoilData
{
    float m_MaxRecoilAngleX; //0x0000
    float m_MinRecoilAngleX; //0x0004
    float m_MaxRecoilAngleY; //0x0008
    float m_MinRecoilAngleY; //0x000C
    float m_MaxRecoilAngleZ; //0x0010
    float m_MinRecoilAngleZ; //0x0014
    float m_MaxRecoilFov; //0x0018
    float m_MinRecoilFov; //0x001C
    bool m_RecoilFollowsDispersion; //0x0020
    char _0x0021[3];
};//Size=0x0024

////////////////////////////////////////
// RuntimeId: 07795
// TypeInfo:  0x0000000142C5F140
struct BoltActionData
{
    float m_BoltActionDelay; //0x0000
    float m_BoltActionTime; //0x0004
    bool m_HoldBoltActionUntilFireRelease; //0x0008
    bool m_HoldBoltActionUntilZoomRelease; //0x0009
    bool m_ForceBoltActionOnFireTrigger; //0x000A
    bool m_UnZoomOnBoltAction; //0x000B
    bool m_ReturnToZoomAfterBoltAction; //0x000C
    char _0x000D[3];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 07805
// TypeInfo:  0x0000000142C5F100
struct FireLogicData
{
    HoldAndReleaseData m_HoldAndRelease; //0x0000
    BoltActionData m_BoltAction; //0x001C
    RecoilData m_Recoil; //0x002C
    __int32 m_FireInputAction; //0x0050
    __int32 m_ReloadInputAction; //0x0054
    __int32 m_CycleFireModeInputAction; //0x0058
    float m_TriggerPullWeight; //0x005C
    float m_RateOfFire; //0x0060
    float m_RateOfFireForBurst; //0x0064
    float m_DelayBetweenBursts; //0x0068
    float m_ClientFireRateMultiplier; //0x006C
    float m_ReloadDelay; //0x0070
    float m_ReloadTime; //0x0074
    Array<float> m_ReloadTimerArray; //0x0078
    float m_ReloadTimeBulletsLeft; //0x0080
    float m_ReloadThreshold; //0x0084
    float m_PreFireDelay; //0x0088
    float m_PreFireDelayZoomed; //0x008C
    float m_PreFireRequireHold; //0x0090
    float m_AutomaticDelay; //0x0094
    ReloadLogic m_ReloadLogic; //0x0098
    ReloadType m_ReloadType; //0x009C
    FireLogicType m_FireLogicType; //0x00A0
    char _0x00A4[4];
    Array<FireLogicType> m_FireLogicTypeArray; //0x00A8
    bool m_HoldOffReloadUntilFireRelease; //0x00B0
    bool m_HoldOffReloadUntilZoomRelease; //0x00B1
    bool m_ForceReloadActionOnFireTrigger; //0x00B2
    bool m_AlwaysAutoReload; //0x00B3
    bool m_ZoomTransitionDisableFire; //0x00B4
    char _0x00B5[3];
};//Size=0x00B8

////////////////////////////////////////
// RuntimeId: 07787
// TypeInfo:  0x0000000142C5F320
struct InitialDirectionScaleByPitchData
{
    Vec3 m_InitialDirectionScale; //0x0000
    float m_Pitch; //0x0010
    char _0x0014[12];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 07789
// TypeInfo:  0x0000000142C5F300
struct InitialSpeedScaleByPitchData
{
    Vec3 m_InitialSpeedScale; //0x0000
    float m_Pitch; //0x0010
    char _0x0014[12];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 07791
// TypeInfo:  0x0000000142C5F180
struct ShotConfigData
{
    Vec3 m_InitialPosition; //0x0000
    Vec3 m_InitialDirection; //0x0010
    Vec3 m_InitialSpeed; //0x0020
    Array<InitialDirectionScaleByPitchData> m_InitialDirectionScaleByPitch; //0x0030
    Array<InitialSpeedScaleByPitchData> m_InitialSpeedScaleByPitch; //0x0038
    float m_InheritWeaponSpeedAmount; //0x0040
    char _0x0044[4];
    ExplosionEntityData* m_MuzzleExplosion; //0x0048
    ProjectileEntityData* m_ProjectileData; //0x0050
    ProjectileEntityData* m_SecondaryProjectileData; //0x0058
    ProjectileBlueprint* m_Projectile; //0x0060
    ProjectileBlueprint* m_SecondaryProjectile; //0x0068
    float m_SpawnDelay; //0x0070
    float m_SpawnDelayZoomed; //0x0074
    unsigned __int32 m_NumberOfBulletsPerShell; //0x0078
    unsigned __int32 m_NumberOfBulletsPerShot; //0x007C
    unsigned __int32 m_NumberOfBulletsPerBurst; //0x0080
    bool m_RelativeTargetAiming; //0x0084
    bool m_ForceSpawnToCamera; //0x0085
    bool m_SpawnVisualAtWeaponBone; //0x0086
    bool m_ActiveForceSpawnToCamera; //0x0087
    char _0x0088[8];
};//Size=0x0090

////////////////////////////////////////
// RuntimeId: 07785
// TypeInfo:  0x0000000142C5F340
struct HealingSphereData
{
    float m_Radius; //0x0000
    float m_HealthIncSpeed; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 07781
// TypeInfo:  0x0000000142C5F1A0
struct NearTargetDetonationData
{
    float m_DetonationRadius; //0x0000
    float m_MaxDetonationDelay; //0x0004
    float m_MinDetonationDelay; //0x0008
    bool m_DetonateNearTarget; //0x000C
    char _0x000D[3];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 07779
// TypeInfo:  0x0000000142C5F1C0
struct MissileUnguidedData
{
    Vec2 m_StaticPosition; //0x0000
    Vec2 m_TargetPositionOffset; //0x0008
    bool m_UseStaticPosition; //0x0010
    bool m_UseTargetPosition; //0x0011
    char _0x0012[2];
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 07777
// TypeInfo:  0x0000000142C5F1E0
struct MissileLockableInfoData
{
    float m_HeatSignature; //0x0000
    float m_RadarSignature; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 07773
// TypeInfo:  0x0000000142C5F200
struct LockingAndHomingData
{
    WarnTarget m_WarnLock; //0x0000
    bool m_IsHoming; //0x0004
    bool m_IsGuided; //0x0005
    bool m_IsGuidedWhenZoomed; //0x0006
    bool m_IsGuidedHoming; //0x0007
    bool m_FireOnlyWhenLockedOn; //0x0008
    char _0x0009[3];
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 07771
// TypeInfo:  0x0000000142C5F360
struct ZoomLevelLockData
{
    float m_OutlineTaggedDistance; //0x0000
    LockType m_LockType; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 07755
// TypeInfo:  0x0000000142C5F220
struct AmmoConfigData
{
    __int32 m_MagazineCapacity; //0x0000
    __int32 m_NumberOfMagazines; //0x0004
    unsigned __int32 m_TraceFrequency; //0x0008
    unsigned __int32 m_AmmoPickupMinAmount; //0x000C
    unsigned __int32 m_AmmoPickupMaxAmount; //0x0010
    float m_AutoReplenishDelay; //0x0014
    float m_AmmoBagPickupDelayMultiplier; //0x0018
    __int32 m_AmmoBagPickupAmount; //0x001C
    bool m_AutoReplenishMagazine; //0x0020
    bool m_StartUnloaded; //0x0021
    char _0x0022[2];
};//Size=0x0024

////////////////////////////////////////
// RuntimeId: 07737
// TypeInfo:  0x0000000142C5F280
struct CannedAnimationBinding
{
    AntRef m_LevelIndex; //0x0000
    AntRef m_ScenarioIndex; //0x0014
    AntRef m_ActorIndex; //0x0028
    AntRef m_PartIndex; //0x003C
    AntRef m_TriggerCannedAnimation; //0x0050
    AntRef m_ExitCannedAnimation; //0x0064
    AntRef m_ExternalTime; //0x0078
    AntRef m_BlendValue; //0x008C
    AntRef m_AdvanceScenario; //0x00A0
    AntRef m_EnteredLoop; //0x00B4
    AntRef m_AlmostFinished; //0x00C8
};//Size=0x00DC

////////////////////////////////////////
// RuntimeId: 07735
// TypeInfo:  0x0000000142C5F2A0
struct WarpAnimationBinding
{
    AntRef m_ConnectJointGroup; //0x0000
    AntRef m_ConnectJointWeight; //0x0014
    AntRef m_EnableAlign; //0x0028
    AntRef m_DisableCulling; //0x003C
    AntRef m_TranslationYWarpTolerance; //0x0050
    AntRef m_AttachToAnimatableInstanceId; //0x0064
    AntRef m_JoinAttachment; //0x0078
};//Size=0x008C

////////////////////////////////////////
// RuntimeId: 07708
// TypeInfo:  0x0000000142C59110
struct VehicleCameraControlBinding
{
    AntRef m_EnableAnimatedVehicleCamera; //0x0000
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 07706
// TypeInfo:  0x0000000142C59130
struct VehicleEntryListenerBinding
{
    AntRef m_InVehicle; //0x0000
    AntRef m_InOpenEntry; //0x0014
    AntRef m_VehicleEntryFire; //0x0028
    AntRef m_VehicleEntryYaw; //0x003C
    AntRef m_VehicleEntryRelativeYaw; //0x0050
    AntRef m_VehicleEntryPitch; //0x0064
    AntRef m_VehicleEntryRoll; //0x0078
    AntRef m_VehicleEntryRotation; //0x008C
    AntRef m_VehicleEntryForceX; //0x00A0
    AntRef m_VehicleEntryForceY; //0x00B4
    AntRef m_VehicleEntryForceZ; //0x00C8
    AntRef m_VehicleEntryIndex; //0x00DC
    AntRef m_VehicleEntryIndexUpdated; //0x00F0
    AntRef m_VehicleOpenEntryIndex; //0x0104
    AntRef m_InVehicleRetrigger; //0x0118
    AntRef m_VehicleEntryChooserIndex; //0x012C
};//Size=0x0140

////////////////////////////////////////
// RuntimeId: 07704
// TypeInfo:  0x0000000142C59150
struct ClientCameraToComponentsInitMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 07703
// TypeInfo:  0x0000000142C59170
struct VehicleToComponentsStartDisabledDamagedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 07702
// TypeInfo:  0x0000000142C59190
struct ClientVehicleToComponentsHealthChangedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 07697
// TypeInfo:  0x0000000142C591B0
struct NormalizeSettings
{
    float m_Minimum; //0x0000
    float m_Maximum; //0x0004
    float m_Lower; //0x0008
    float m_Upper; //0x000C
    float m_Velocity; //0x0010
    bool m_Normalize; //0x0014
    char _0x0015[3];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 07693
// TypeInfo:  0x0000000142C591D0
struct VehicleLockableInfoData
{
    float m_HeatSignature; //0x0000
    float m_RadarSignature; //0x0004
    float m_LowAltitudeHeight; //0x0008
    float m_LowAltitudeHeatSignature; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 07691
// TypeInfo:  0x0000000142C591F0
struct AngleOfImpactData
{
    float m_Zone12Delimiter; //0x0000
    float m_Zone23Delimiter; //0x0004
    float m_Zone1Multiplier; //0x0008
    float m_Zone2Multiplier; //0x000C
    float m_Zone3Multiplier; //0x0010
    bool m_Enabled; //0x0014
    char _0x0015[3];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 07689
// TypeInfo:  0x0000000142C59210
struct MPModeData
{
    __int32 m_VehiclePoints; //0x0000
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 07687
// TypeInfo:  0x0000000142C59230
struct VehicleHealthZoneData
{
    float m_MaxHealth; //0x0000
    float m_MaxShieldHealth; //0x0004
    float m_MinDamageAngle; //0x0008
    float m_DamageAngleMultiplier; //0x000C
    Curve2D* m_DamageAngleMultiplierCurve; //0x0010
    float m_VerticalFactor; //0x0018
    bool m_UseDamageAngleCalculation; //0x001C
    char _0x001D[3];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 07685
// TypeInfo:  0x0000000142C59250
struct VehicleHudData
{
    Vec3 m_CustomizationOffset; //0x0000
    VehicleCustomizationAsset* m_Customization; //0x0010
    UIHudIcon m_MinimapIcon; //0x0018
    char _0x001C[4];
    char* m_IconName; //0x0020
    TextureAsset* m_Texture; //0x0028
    unsigned __int32 m_VehicleItemHash; //0x0030
    bool m_RotateInMap; //0x0034
    bool m_ShowHealth; //0x0035
    char _0x0036[10];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 05344
// TypeInfo:  0x0000000142C223A8
struct MaterialDecl
{
    unsigned __int32 m_Packed; //0x0000
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 07669
// TypeInfo:  0x0000000142C595B0
struct CameraRelativeControlPerSpeedData
{
    float m_SpeedThreshold; //0x0000
    float m_WorldSpaceLockEfficiency; //0x0004
    float m_WorldSpaceLockEfficiencyAway; //0x0008
    float m_ThrottleForSteeringHelp; //0x000C
    float m_ThrottleForSteeringHelpStrength; //0x0010
    float m_ThrottleHold; //0x0014
    float m_MinInputMagnitude; //0x0018
    float m_ThrottleExponent; //0x001C
    float m_TurningExponent; //0x0020
    float m_FullTurningThreshold; //0x0024
    float m_FullTurningBlending; //0x0028
    float m_ExtraForwardZoneAngle; //0x002C
    float m_AngularYawConstraint; //0x0030
    bool m_UseInForwardAndNeutralGears; //0x0034
    bool m_UseInReverseGear; //0x0035
    bool m_UseInBoost; //0x0036
    bool m_UseNotInBoost; //0x0037
    bool m_InvertExtraForwardZoneIfBackwards; //0x0038
    bool m_InvertExtraForwardZoneOnSideFacingCamera; //0x0039
    char _0x003A[2];
};//Size=0x003C

////////////////////////////////////////
// RuntimeId: 07663
// TypeInfo:  0x0000000142C59270
struct LandingGearConditionData
{
    float m_Height; //0x0000
    float m_Velocity; //0x0004
    float m_Angle; //0x0008
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 07651
// TypeInfo:  0x0000000142C595D0
struct TurretRotationInfo
{
    RotationAxis m_RotationAxis; //0x0000
    RotationChannel m_Channel; //0x0004
    float m_MaxRotation; //0x0008
    float m_MinRotation; //0x000C
    float m_PhaseOffset; //0x0010
    bool m_NormalizeRotation; //0x0014
    bool m_InvertRotation; //0x0015
    char _0x0016[2];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 07639
// TypeInfo:  0x0000000142C59290
struct RotorModelData
{
    float m_RotationRpm; //0x0000
    unsigned __int32 m_PartIndex; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 07615
// TypeInfo:  0x0000000142C595F0
struct UnlockIdTable
{
    Array<unsigned __int32> m_Identifiers; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 07613
// TypeInfo:  0x0000000142C59610
struct BasicUnlockInfo
{
    Guid m_UnlockGuid; //0x0000
    char _0x0000[16];
    unsigned __int32 m_Identifier; //0x0010
    unsigned __int32 m_UnlockScore; //0x0014
    Array<char*> m_Licenses; //0x0018
    Array<char*> m_AdditionalLicenses; //0x0020
    char* m_PersistentStorageCode; //0x0028
    char* m_StringId; //0x0030
    bool m_IgnoreAllUnlocked; //0x0038
    bool m_BypassLicenseCheckAllUnlocked; //0x0039
    char _0x003A[6];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 07601
// TypeInfo:  0x0000000142C59630
struct UnlockAssetPair
{
    UnlockAssetBase* m_Second; //0x0000
    UnlockAssetBase* m_Result; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 07465
// TypeInfo:  0x0000000142C59410
struct UIElementColor
{
    Vec3 m_Rgb; //0x0000
    float m_Alpha; //0x0010
    char _0x0014[4];
    char* m_PropertyName; //0x0018
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 07527
// TypeInfo:  0x0000000142C592F0
struct UIElementInclusionSettings
{
    Array<char*> m_CustomInclusionCritera; //0x0000
    bool m_IsSingleplayerLayer; //0x0008
    bool m_IsMultiplayerLayer; //0x0009
    bool m_IsWin32Layer; //0x000A
    bool m_IsXenonLayer; //0x000B
    bool m_IsPs3Layer; //0x000C
    bool m_IsGen4aLayer; //0x000D
    bool m_IsGen4bLayer; //0x000E
    bool m_IsSDLayer; //0x000F
    bool m_IsHDLayer; //0x0010
    bool m_IsAdvanced; //0x0011
    char _0x0012[6];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 07525
// TypeInfo:  0x0000000142C59650
struct UIElementEditorSettings
{
    float m_Opacity; //0x0000
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 07505
// TypeInfo:  0x0000000142C59310
struct UIElementGradient
{
    UIElementColor m_TopLeftColor; //0x0000
    UIElementColor m_TopRightColor; //0x0020
    UIElementColor m_BottomLeftColor; //0x0040
    UIElementColor m_BottomRightColor; //0x0060
};//Size=0x0080

////////////////////////////////////////
// RuntimeId: 07461
// TypeInfo:  0x0000000142C59450
struct UIDataSource
{
    UIComponentData* m_DataCategory; //0x0000
    __int32 m_DataKey; //0x0008
    char _0x000C[4];
    char* m_SchematicsProperty; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 07501
// TypeInfo:  0x0000000142C59350
struct UIElementTransform
{
    Vec3 m_Rotation; //0x0000
    Vec3 m_RotationPivot; //0x0010
    float m_Z; //0x0020
    char _0x0024[4];
    char* m_PropertyName; //0x0028
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 07499
// TypeInfo:  0x0000000142C59370
struct UIElementAnchor
{
    float m_X; //0x0000
    float m_Y; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 07497
// TypeInfo:  0x0000000142C59390
struct UIElementSize
{
    float m_X; //0x0000
    float m_Y; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 07495
// TypeInfo:  0x0000000142C593B0
struct UIElementOffset
{
    float m_X; //0x0000
    float m_Y; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 07493
// TypeInfo:  0x0000000142C593D0
struct UIElementRectExpansion
{
    float m_X; //0x0000
    float m_Y; //0x0004
    float m_Width; //0x0008
    float m_Height; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 07491
// TypeInfo:  0x0000000142C59670
struct UIElementRect
{
    float m_X; //0x0000
    float m_Y; //0x0004
    float m_Width; //0x0008
    float m_Height; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 07487
// TypeInfo:  0x0000000142C59690
struct UIElementTextEntry
{
    UITextEntryType m_TextType; //0x0000
    char _0x0004[4];
    char* m_Text; //0x0008
    char* m_FloatFormat; //0x0010
    UIDataSource m_DataSource; //0x0018
    bool m_UseLocalization; //0x0030
    char _0x0031[7];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 07469
// TypeInfo:  0x0000000142C593F0
struct UIElementFont
{
    char* m_ScaleformFontName; //0x0000
    float m_FontSize; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 07467
// TypeInfo:  0x0000000142C596B0
struct UIElementLineStyle
{
    UIElementColor m_Color; //0x0000
    float m_Width; //0x0020
    char _0x0024[12];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 07549
// TypeInfo:  0x0000000142C592D0
struct UIElementBitmapDistanceFieldParams
{
    UIElementColor m_OutlineColor; //0x0000
    float m_AlphaThreshold; //0x0020
    float m_DistanceScale; //0x0024
    float m_OutlineInner; //0x0028
    float m_OutlineOuter; //0x002C
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 07463
// TypeInfo:  0x0000000142C59430
struct UICompareDataSource
{
    UIComponentData* m_DataCategory; //0x0000
    __int32 m_DataKey; //0x0008
    char _0x000C[4];
    char* m_SchematicsProperty; //0x0010
    char* m_CompareValue; //0x0018
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 07503
// TypeInfo:  0x0000000142C59330
struct UIElementText
{
    char* m_Sid; //0x0000
    Array<UIElementTextEntry> m_Entries; //0x0008
    UIElementAlignment m_VerticalAlignment; //0x0010
    UIElementAlignment m_HorizonalAlignment; //0x0014
    bool m_Multiline; //0x0018
    bool m_Wordwrap; //0x0019
    char _0x001A[6];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 07457
// TypeInfo:  0x0000000142C59470
struct UIAutoScrollTextSettings
{
    float m_NoScrollWaitTime; //0x0000
    float m_FullyScrolledWaitTime; //0x0004
    float m_MaxScrollTime; //0x0008
    float m_PixelsPerSecond; //0x000C
    float m_ScrollbackMultiplier; //0x0010
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 07451
// TypeInfo:  0x0000000142C596D0
struct UITextureMapping
{
    char* m_Id; //0x0000
    TextureAsset* m_Texture; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 07449
// TypeInfo:  0x0000000142C596F0
struct UITextureMappingOutputEntry
{
    char* m_Id; //0x0000
    TextureAsset* m_Texture; //0x0008
    Vec2 m_Min; //0x0010
    Vec2 m_Max; //0x0018
    bool m_Streaming; //0x0020
    char _0x0021[7];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 07445
// TypeInfo:  0x0000000142C59710
struct UIImmediateModeFontLookup
{
    LanguageFormat m_Language; //0x0000
    char _0x0004[4];
    char* m_FontAssetPath; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 07443
// TypeInfo:  0x0000000142C59730
struct UIImmediateModeFontBundle
{
    LanguageFormat m_Language; //0x0000
    char _0x0004[4];
    char* m_BundlePath; //0x0008
    Array<UITtfAsset*> m_TtfAssets; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 07437
// TypeInfo:  0x0000000142C59490
struct UIFlowDialogNodeReachedMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 07436
// TypeInfo:  0x0000000142C594B0
struct UIDialogScreenPushedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 07379
// TypeInfo:  0x0000000142C59750
struct InputRecorderTrackData
{
    char* m_Name; //0x0000
    __int32 m_InputAction; //0x0008
    char _0x000C[4];
    FloatPropertyTrackData* m_PropertyTrack; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 07369
// TypeInfo:  0x0000000142C594F0
struct SimpleMovementActionTimeData
{
    float m_Time; //0x0000
    float m_MaxVariation; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 07309
// TypeInfo:  0x0000000142C59530
struct IndexRange
{
    unsigned __int32 m_First; //0x0000
    unsigned __int32 m_Last; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 07311
// TypeInfo:  0x0000000142C59770
struct ChildStaticModelNetworkInfo
{
    IndexRange m_NetworkRange; //0x0000
    unsigned __int32 m_ParentPartComponentIndex; //0x0008
    unsigned __int32 m_ParentHealthStateIndex; //0x000C
    unsigned __int32 m_InstanceIndex; //0x0010
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 07313
// TypeInfo:  0x0000000142C59510
struct StaticModelNetworkInfo
{
    Array<IndexRange> m_PartNetworkIdRanges; //0x0000
    unsigned __int32 m_NetworkIdCount; //0x0008
    char _0x000C[4];
    Array<ChildStaticModelNetworkInfo> m_ChildNetworkInfos; //0x0010
    unsigned __int32 m_ChildNetworkIdCount; //0x0018
    char _0x001C[4];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 07307
// TypeInfo:  0x0000000142C59790
struct PhysicsPartInfo
{
    unsigned __int32 m_PartComponentIndex; //0x0000
    unsigned __int32 m_HealthStateIndex; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 07301
// TypeInfo:  0x0000000142C59550
struct CharacterToComponentsSpawnTemplateMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 07272
// TypeInfo:  0x0000000142C50DF0
struct PID
{
    float m_P; //0x0000
    float m_I; //0x0004
    float m_D; //0x0008
    float m_MaxError; //0x000C
    float m_MaxPTerm; //0x0010
    float m_MaxITerm; //0x0014
    float m_MaxSumError; //0x0018
};//Size=0x001C

////////////////////////////////////////
// RuntimeId: 07250
// TypeInfo:  0x0000000142C51190
struct CustomizeVisual
{
    Array<UnlockAsset*> m_Visual; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 07248
// TypeInfo:  0x0000000142C50E90
struct NetworkPlayerSelectedUnlockAssetsMessage
{
    char _0x0000[112];
};//Size=0x0070

////////////////////////////////////////
// RuntimeId: 07247
// TypeInfo:  0x0000000142C511B0
struct CamoUnlockSelection
{
    __int32 m_Category; //0x0000
    char _0x0004[4];
    UnlockAssetBase* m_Asset; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 07227
// TypeInfo:  0x0000000142C511D0
struct CustomizedMeshMaterialsData
{
    ObjectBlueprint* m_MeshBlueprint; //0x0000
    Array<char*> m_MaterialNames; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 07197
// TypeInfo:  0x0000000142C50F10
struct PartComponentOverridePreDamagedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 07178
// TypeInfo:  0x0000000142C50F30
struct EntityToComponentsResetMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 07175
// TypeInfo:  0x0000000142C50F50
struct UnlockComponentEnabledMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 07170
// TypeInfo:  0x0000000142C511F0
struct SoldierAnimatedCameraData
{
    float m_CameraTransitionTime; //0x0000
    bool m_EnableFeature; //0x0004
    char _0x0005[3];
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 07160
// TypeInfo:  0x0000000142C51210
struct ShaderParameterVector
{
    Vec4 m_Value; //0x0000
    char* m_ParameterName; //0x0010
    char _0x0018[8];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 07158
// TypeInfo:  0x0000000142C51230
struct CameraBinding
{
    AntRef m_Render1pInBackground; //0x0000
    AntRef m_ForceRender1pInForeground; //0x0014
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 07118
// TypeInfo:  0x0000000142C512D0
struct ActionSuppressor
{
    __int32 m_ActionToSuppress; //0x0000
    float m_SuppressingValue; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 07154
// TypeInfo:  0x0000000142C50F90
struct InputSuppressionData
{
    Array<ActionSuppressor> m_SuppressVehicleInput; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 07142
// TypeInfo:  0x0000000142C51270
struct BuoyantPartsData
{
    BuoyantParts m_PartName; //0x0000
    float m_Buoyancy; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 07126
// TypeInfo:  0x0000000142C51010
struct PitchModifier
{
    Vec3 m_Offset; //0x0000
    float m_PitchVal; //0x0010
    float m_PitchAngle; //0x0014
    char _0x0018[8];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 07130
// TypeInfo:  0x0000000142C50FF0
struct AimAssistCollisionBonePrioritiesData
{
    __int32 m_StartPriority; //0x0000
    __int32 m_MidPriority; //0x0004
    __int32 m_EndPriority; //0x0008
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 07132
// TypeInfo:  0x0000000142C50FD0
struct AimAssistCollisionBoneSnapAimData
{
    float m_Bounding_LengthScale; //0x0000
    float m_Bounding_RadiusScale; //0x0004
    float m_Point_LengthScale; //0x0008
    AimAssistCollisionBonePrioritiesData m_Point_Priorities; //0x000C
    AimAssistCollisionBonePrioritiesData m_Point_Priorities_BoneUnderReticle; //0x0018
};//Size=0x0024

////////////////////////////////////////
// RuntimeId: 07136
// TypeInfo:  0x0000000142C51290
struct BoneCollisionData
{
    Vec4 m_DebugDrawColor; //0x0000
    Vec3 m_CapsuleOffset; //0x0010
    PitchModifier m_MaxPitch; //0x0020
    PitchModifier m_MinPitch; //0x0040
    char* m_BoneName; //0x0060
    HitReactionType m_AnimationHitReactionType; //0x0068
    MaterialDecl m_MaterialPair; //0x006C
    __int32 m_BoneAxis; //0x0070
    float m_CapsuleLength; //0x0074
    float m_CapsuleRadius; //0x0078
    char _0x007C[4];
    AimAssistCollisionBoneData* m_AimAssistTarget; //0x0080
    bool m_ValidInHiLod; //0x0088
    bool m_ValidInLowLod; //0x0089
    bool m_DeactivateIfBehindWall; //0x008A
    bool m_UsePhysicsRotation; //0x008B
    char _0x008C[4];
};//Size=0x0090

////////////////////////////////////////
// RuntimeId: 07124
// TypeInfo:  0x0000000142C512B0
struct HIKData
{
    float m_ReachT; //0x0000
    float m_ReachR; //0x0004
    float m_Pull; //0x0008
    float m_Resist; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 07122
// TypeInfo:  0x0000000142C51030
struct RagdollBinding
{
    AntRef m_RagdollOnBack; //0x0000
    AntRef m_RagdollBlend; //0x0014
    AntRef m_RagdollBlendEarly; //0x0028
    AntRef m_RagdollForceBlendDisabled; //0x003C
    AntRef m_RagdollActiveTime; //0x0050
    AntRef m_RagdollFullyBlendedIn; //0x0064
};//Size=0x0078

////////////////////////////////////////
// RuntimeId: 07156
// TypeInfo:  0x0000000142C51250
struct RegularCameraViewData
{
    Vec3 m_MeshOffset; //0x0000
    Vec3 m_MeshScale; //0x0010
    InputSuppressionData m_InputSuppression; //0x0020
    float m_FieldOfView; //0x0028
    char _0x002C[4];
    RigidMeshAsset* m_Mesh; //0x0030
    ObjectBlueprint* m_MaskMeshBlueprint; //0x0038
    float m_ScreenExposureAreaScale; //0x0040
    bool m_FLIREnabled; //0x0044
    bool m_AllowFieldOfViewScaling; //0x0045
    bool m_LockMeshToRenderView; //0x0046
    char _0x0047[9];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 07116
// TypeInfo:  0x0000000142C512F0
struct StanceCameraData
{
    Array<__int32> m_ValidStances; //0x0000
    Array<__int32> m_DefaultCameraForStances; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 07108
// TypeInfo:  0x0000000142C51050
struct EntryComponentStanceChangedMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 07103
// TypeInfo:  0x0000000142C51310
struct StanceSwitchSoundData
{
    SoundAsset* m_StanceSwitchSound; //0x0000
    Array<__int32> m_ValidStances; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 07099
// TypeInfo:  0x0000000142C51070
struct EntryComponentHudData
{
    __int32 m_Index; //0x0000
    EntrySeatType m_SeatType; //0x0004
    bool m_Frustum; //0x0008
    bool m_Visible; //0x0009
    bool m_MaximizeMiniMapOnEntry; //0x000A
    char _0x000B[1];
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 07073
// TypeInfo:  0x0000000142C510B0
struct CharacterToComponentsOnKilledMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 07066
// TypeInfo:  0x0000000142C510D0
struct CharacterToComponentsOnTeleportedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 07033
// TypeInfo:  0x0000000142C51330
struct CameraLeapData
{
    LinearTransform m_Transform; //0x0000
    float m_Time; //0x0040
    float m_FovModifier; //0x0044
    char _0x0048[8];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 07021
// TypeInfo:  0x0000000142C51350
struct HudImpactData
{
    float m_MinHealth; //0x0000
    float m_MaxHealth; //0x0004
    float m_MinTimeVisible; //0x0008
    float m_MaxTimeVisible; //0x000C
    bool m_Enable; //0x0010
    char _0x0011[3];
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 07017
// TypeInfo:  0x0000000142C510F0
struct BlurEffectData
{
    float m_DispersionStrength; //0x0000
    float m_DispersionThreshhold; //0x0004
    float m_ExplosionStrength; //0x0008
    float m_BulletStrength; //0x000C
    float m_ExplosionFalloffSpeed; //0x0010
    float m_BulletFalloffSpeed; //0x0014
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 07015
// TypeInfo:  0x0000000142C51110
struct TurnEffectData
{
    float m_MaxRollAngle; //0x0000
    float m_MaxPitchAngle; //0x0004
    float m_YawVelocityThreshhold; //0x0008
    float m_VelocityThreshhold; //0x000C
    float m_SafeTime; //0x0010
    float m_BeginTime; //0x0014
    float m_HoldTime; //0x0018
    float m_EndTime; //0x001C
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 07013
// TypeInfo:  0x0000000142C51370
struct CameraSineCurveData
{
    float m_Frequency; //0x0000
    float m_Amplitude; //0x0004
    float m_PhaseShift; //0x0008
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 05008
// TypeInfo:  0x0000000142C1E0A8
struct BundleHeapInfo
{
    BundleHeapType m_HeapType; //0x0000
    unsigned __int32 m_InitialSize; //0x0004
    bool m_AllowGrow; //0x0008
    char _0x0009[3];
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 06991
// TypeInfo:  0x0000000142C51150
struct BlueprintBundleSettings
{
    BundleHeapInfo m_Heap; //0x0000
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 05048
// TypeInfo:  0x0000000142C1DF08
struct EventSpec
{
    __int32 m_Id; //0x0000
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 06939
// TypeInfo:  0x0000000142C4B0C0
struct SubLevelPreloadInfo
{
    char* m_SubLevelBundlePath; //0x0000
    Array<char*> m_PreloadedBlueprintBundles; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 06941
// TypeInfo:  0x0000000142C4AEA0
struct LevelPreloadInfo
{
    Array<char*> m_PreloadedBlueprintBundles; //0x0000
    Array<SubLevelPreloadInfo> m_SubLevelPreloadInfoMap; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 06929
// TypeInfo:  0x0000000142C4B0E0
struct PathfindingBlob
{
    Guid m_BlobId; //0x0000
    char _0x0000[16];
    unsigned __int32 m_BlobSize; //0x0010
    char _0x0014[4];
    Array<unsigned __int32> m_ChunkSizes; //0x0018
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 06923
// TypeInfo:  0x0000000142C4B100
struct FaceAnimationWaveMapping
{
    __int32 m_WaveNameHash; //0x0000
    __int32 m_AntEnumValue; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 06913
// TypeInfo:  0x0000000142C4B120
struct LevelStartPoint
{
    char* m_Name; //0x0000
    Array<char*> m_AutoloadSublevels; //0x0008
    bool m_IsDefault; //0x0010
    char _0x0011[7];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 06911
// TypeInfo:  0x0000000142C4B140
struct LevelBundleLoad
{
    char* m_Name; //0x0000
    bool m_TryKeepBetweenLevels; //0x0008
    char _0x0009[7];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 06909
// TypeInfo:  0x0000000142C4B160
struct LevelDescriptionInclusionCategory
{
    char* m_Category; //0x0000
    Array<char*> m_Mode; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 06905
// TypeInfo:  0x0000000142C4AEC0
struct LevelDescription
{
    char* m_Name; //0x0000
    char* m_Description; //0x0008
    Array<LevelDescriptionComponent*> m_Components; //0x0010
    bool m_IsMultiplayer; //0x0018
    bool m_IsCoop; //0x0019
    bool m_IsMenu; //0x001A
    bool m_IsEpilogue; //0x001B
    char _0x001C[4];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 06901
// TypeInfo:  0x0000000142C4AEE0
struct InputMessagesSingleInputEventMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06858
// TypeInfo:  0x0000000142C4AF00
struct EditableActions
{
    Array<EditableAction*> m_Actions; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 06860
// TypeInfo:  0x0000000142C4B180
struct EditableActionMap
{
    char* m_Id; //0x0000
    char* m_NameId; //0x0008
    EntryInputActionMapsData* m_ActionMap; //0x0010
    EditableActions m_ConfigurationLayout; //0x0018
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 06844
// TypeInfo:  0x0000000142C4B1A0
struct EntryInputActionBinding
{
    __int32 m_Action; //0x0000
    __int32 m_Alias; //0x0004
    EntryInputActionType m_ActionType; //0x0008
    bool m_Networked; //0x000C
    char _0x000D[3];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 06820
// TypeInfo:  0x0000000142C4B1C0
struct HudData
{
    float m_CrosshairScaleMin; //0x0000
    float m_CrosshairScaleMax; //0x0004
    float m_CrosshairOpacityMin; //0x0008
    float m_CrosshairOpacityMax; //0x000C
    float m_CrosshairOpacityModifier; //0x0010
    char _0x0014[4];
    char* m_CrosshairTypeId; //0x0018
    CrosshairTypeAsset* m_CrosshairType; //0x0020
    LockingTypeAsset* m_LockingType; //0x0028
    char* m_WeaponClass; //0x0030
    float m_LowAmmoWarning; //0x0038
    float m_ReloadPrompt; //0x003C
    __int32 m_RenderTargetIndex; //0x0040
    char _0x0044[4];
    UIPartPropertyList* m_HudPropertyList; //0x0048
    float m_SeaLevelAltFreq; //0x0050
    float m_CameraShakeModifier; //0x0054
    char* m_HudIcon; //0x0058
    bool m_ShowMinimap; //0x0060
    bool m_HideAmmo; //0x0061
    bool m_InfiniteAmmo; //0x0062
    bool m_HideCrosshairWhenAimOnFriend; //0x0063
    bool m_UseRenderTarget; //0x0064
    bool m_UseRangeMeter; //0x0065
    bool m_UseAimWarning; //0x0066
    bool m_UsePredictedSight; //0x0067
    bool m_UseWeaponOrientations; //0x0068
    bool m_UseVelocityVectorMarker; //0x0069
    bool m_UseLockingController; //0x006A
    bool m_UseThrust; //0x006B
    bool m_UseGForce; //0x006C
    bool m_UseSkidSlip; //0x006D
    bool m_UseClimbRate; //0x006E
    char _0x006F[1];
};//Size=0x0070

////////////////////////////////////////
// RuntimeId: 06812
// TypeInfo:  0x0000000142C4B1E0
struct UIPartProperties
{
    UIPartIdentifier m_Identifier; //0x0000
    float m_Range; //0x0004
    float m_Frequency; //0x0008
    bool m_CompensateFreelook; //0x000C
    char _0x000D[3];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 06798
// TypeInfo:  0x0000000142C4B200
struct StaticModelGroupMemberData
{
    Array<LinearTransform> m_InstanceTransforms; //0x0000
    Array<unsigned __int32> m_InstanceObjectVariation; //0x0008
    Array<bool> m_InstanceCastSunShadow; //0x0010
    Array<bool> m_InstanceCastReflection; //0x0018
    Array<RadiosityTypeOverride> m_InstanceRadiosityTypeOverride; //0x0020
    Array<bool> m_InstanceTerrainShaderNodesEnable; //0x0028
    GamePhysicsEntityData* m_MemberType; //0x0030
    MeshAsset* m_MeshAsset; //0x0038
    unsigned __int32 m_InstanceCount; //0x0040
    unsigned __int32 m_HealthStateEntityManagerId; //0x0044
    IndexRange m_PhysicsPartRange; //0x0048
    unsigned __int32 m_PhysicsPartCountPerInstance; //0x0050
    IndexRange m_NetworkIdRange; //0x0054
    unsigned __int32 m_NetworkIdCountPerInstance; //0x005C
    unsigned __int32 m_PartComponentCount; //0x0060
    char _0x0064[4];
};//Size=0x0068

////////////////////////////////////////
// RuntimeId: 06756
// TypeInfo:  0x0000000142C4AFC0
struct ServerControllableToComponentsPlayerExitMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06755
// TypeInfo:  0x0000000142C4AFE0
struct ServerControllableToComponentsPlayerEnteredMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06754
// TypeInfo:  0x0000000142C4B000
struct ClientControllableToComponentsPlayerExitMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06753
// TypeInfo:  0x0000000142C4B020
struct ClientControllableToComponentsPlayerEnteredMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06744
// TypeInfo:  0x0000000142C4B220
struct VoiceOverConversationEntityTrackInfo
{
    unsigned __int32 m_TakeControlId; //0x0000
    unsigned __int32 m_SourceId; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 06734
// TypeInfo:  0x0000000142C4B040
struct FbProxyControllerEntityBinding
{
    AntRef m_Trigger; //0x0000
    AntRef m_Stop; //0x0014
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06728
// TypeInfo:  0x0000000142C4B080
struct EventSyncReachedClientMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 06723
// TypeInfo:  0x0000000142C4B240
struct EntitlementData
{
    char* m_License; //0x0000
    char* m_EntitlementTag; //0x0008
    char* m_GroupName; //0x0010
    char* m_ProductId; //0x0018
    char* m_ProjectId; //0x0020
    EntitlementType m_UsageType; //0x0028
    bool m_VerifyOwnership; //0x002C
    char _0x002D[3];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06721
// TypeInfo:  0x0000000142C4B260
struct PlatformProjectId
{
    GamePlatform m_Platform; //0x0000
    char _0x0004[4];
    char* m_ProjectId; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 06719
// TypeInfo:  0x0000000142C4B280
struct EntitlementPlatformType
{
    GamePlatform m_Platform; //0x0000
    EntitlementType m_EntitlementType; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 06699
// TypeInfo:  0x0000000142C4B0A0
struct AntDynamicAvoidanceBinding
{
    AntRef m_TimeUntilCollision; //0x0000
    AntRef m_DistanceUntilCollision; //0x0014
    AntRef m_NormalizedVelocity; //0x0028
    AntRef m_Speed; //0x003C
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 06695
// TypeInfo:  0x0000000142C45228
struct CongestionControlSettings
{
    float m_LatencyMsMax; //0x0000
    float m_PacketLossMin; //0x0004
    float m_PacketLossMax; //0x0008
    float m_LatencyIncrPercMax; //0x000C
    float m_LatencyIncrJitter; //0x0010
    float m_LatencyIncrThreshold; //0x0014
    float m_ConnectionGradeGood; //0x0018
    float m_ConnectionGradeBad; //0x001C
    float m_FreqDistScaleHFMax; //0x0020
    float m_FreqDistScaleBaseMax; //0x0024
    float m_AdaptionStepCount; //0x0028
    float m_MaxBwUtilForIncrease; //0x002C
    bool m_Enabled; //0x0030
    char _0x0031[3];
};//Size=0x0034

////////////////////////////////////////
// RuntimeId: 06657
// TypeInfo:  0x0000000142C45368
struct ProfileOptionDataEnumItem
{
    char* m_DisplayName; //0x0000
    bool m_Default; //0x0008
    char _0x0009[7];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 06647
// TypeInfo:  0x0000000142C45388
struct BinaryOption
{
    char* m_Name; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 06645
// TypeInfo:  0x0000000142C453A8
struct StringOption
{
    char* m_Name; //0x0000
    char* m_Value; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 06643
// TypeInfo:  0x0000000142C453C8
struct IntOption
{
    char* m_Name; //0x0000
    __int32 m_Value; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 06641
// TypeInfo:  0x0000000142C453E8
struct FloatOption
{
    char* m_Name; //0x0000
    float m_Value; //0x0008
    float m_Min; //0x000C
    float m_Max; //0x0010
    float m_Step; //0x0014
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 06633
// TypeInfo:  0x0000000142C45268
struct ProceduralAwarenessComponentBinding
{
    AntRef m_StrengthScale; //0x0000
    AntRef m_UseTargetOverride; //0x0014
    AntRef m_TargetOverride; //0x0028
    AntRef m_NewTargetChosen; //0x003C
    AntRef m_DoRandomBlendOut; //0x0050
};//Size=0x0064

////////////////////////////////////////
// RuntimeId: 06621
// TypeInfo:  0x0000000142C45288
struct PhysicsDrivenAnimationEntityBinding
{
    AntRef m_PhysicsMotionTarget; //0x0000
    AntRef m_AimLeftRight; //0x0014
    AntRef m_AimUpDown; //0x0028
    AntRef m_Crouch; //0x003C
    AntRef m_ForceSetTrajectory; //0x0050
    AntRef m_InAir; //0x0064
    AntRef m_Skydive; //0x0078
    AntRef m_Parachute; //0x008C
    AntRef m_Swim; //0x00A0
    AntRef m_InputBackward; //0x00B4
    AntRef m_InputForward; //0x00C8
    AntRef m_InputLeft; //0x00DC
    AntRef m_InputRight; //0x00F0
    AntRef m_IsEnemy; //0x0104
    AntRef m_Jump; //0x0118
    AntRef m_LeanLeftRight; //0x012C
    AntRef m_Prone; //0x0140
    AntRef m_Sprint; //0x0154
    AntRef m_GroundSupported; //0x0168
    AntRef m_GroundNormal; //0x017C
    AntRef m_GroundDistance; //0x0190
    AntRef m_GroundAngleZ; //0x01A4
    AntRef m_GroundAngleX; //0x01B8
    AntRef m_GroundAngleFromNormal; //0x01CC
    AntRef m_IsClientAnimatable; //0x01E0
    AntRef m_CustomizationScreen; //0x01F4
    AntRef m_Minimal3pServer; //0x0208
    AntRef m_VerticalImpact; //0x021C
    AntRef m_VerticalImpactSpeed; //0x0230
    AntRef m_FalseSignal; //0x0244
    AntRef m_LockArmsToCameraWeight; //0x0258
    AntRef m_WindDirection; //0x026C
    AntRef m_WindStrength; //0x0280
    AntRef m_WaterDepth; //0x0294
    AntRef m_EyeWaterDepth; //0x02A8
};//Size=0x02BC

////////////////////////////////////////
// RuntimeId: 06605
// TypeInfo:  0x0000000142C45408
struct RichPresenceProperty
{
    char* m_Name; //0x0000
    RichPresencePropertyType m_PropertyType; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 06601
// TypeInfo:  0x0000000142C45428
struct RichPresenceContextSetting
{
    RichPresenceContext* m_Context; //0x0000
    RichPresenceContextValue* m_Value; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 06589
// TypeInfo:  0x0000000142C45448
struct PersistenceConsumableMapping
{
    ConsumableGroup m_Group; //0x0000
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 06575
// TypeInfo:  0x0000000142C452E8
struct PersistentValueTemplateData
{
    char* m_Name; //0x0000
    char* m_DefaultValue; //0x0008
    float m_DefaultFloatValue; //0x0010
    PersistentValueType m_ValueType; //0x0014
    PersistentValueDataType m_DataType; //0x0018
    PersistentValueHistoryType m_HistoryType; //0x001C
    bool m_ClubStat; //0x0020
    char _0x0021[7];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06519
// TypeInfo:  0x0000000142C45468
struct EffectWithSpeedRange
{
    EffectBlueprint* m_Effect; //0x0000
    float m_MinSpeed; //0x0008
    float m_MaxSpeed; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 06252
// TypeInfo:  0x0000000142C3A0D0
struct ServerProjectileOnSpawnMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06251
// TypeInfo:  0x0000000142C3A0F0
struct ServerAdministrationRestartRequiredMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06250
// TypeInfo:  0x0000000142C3A110
struct AIDirectorStateMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 06249
// TypeInfo:  0x0000000142C3A130
struct AISpawnBotMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 06248
// TypeInfo:  0x0000000142C3A150
struct AIPlayerEnableAsTargetMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 06247
// TypeInfo:  0x0000000142C3A170
struct ServerPeerLoadLevelMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06246
// TypeInfo:  0x0000000142C3A190
struct ServerPeerInitializedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06245
// TypeInfo:  0x0000000142C3A1B0
struct ServerMissionObjectiveCompletedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06244
// TypeInfo:  0x0000000142C3A1D0
struct ServerSubLevelOnStreamedInMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06243
// TypeInfo:  0x0000000142C3A1F0
struct ServerLevelCompletedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06242
// TypeInfo:  0x0000000142C3A210
struct ServerLevelStartedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06241
// TypeInfo:  0x0000000142C3A230
struct ServerLevelSpawnEntitiesEndMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06240
// TypeInfo:  0x0000000142C3A250
struct ServerLevelSpawnEntitiesBeginMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06239
// TypeInfo:  0x0000000142C3A270
struct ServerRoundInterruptedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06238
// TypeInfo:  0x0000000142C3A290
struct ServerRoundOverMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06237
// TypeInfo:  0x0000000142C3A2B0
struct ServerRoundResetMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06236
// TypeInfo:  0x0000000142C3A2D0
struct ServerGameplayCheckpointActivatedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06235
// TypeInfo:  0x0000000142C3A2F0
struct ServerGameplayCheckpointTriggeredMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06234
// TypeInfo:  0x0000000142C3A310
struct ServerGameModeResetMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06233
// TypeInfo:  0x0000000142C3A330
struct ServerGameplaySetPostRoundLogicMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06232
// TypeInfo:  0x0000000142C3A350
struct ServerGameplaySetPreRoundLogicMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06231
// TypeInfo:  0x0000000142C3A370
struct ServerGameplayGameModeResetMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06230
// TypeInfo:  0x0000000142C3A390
struct ServerGameplayServerPlayerMenuCancelMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06229
// TypeInfo:  0x0000000142C3A3B0
struct ServerGameplayServerPlayerMenuOkMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06228
// TypeInfo:  0x0000000142C3A3D0
struct ServerGameplayPreviousWeatherStateMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06227
// TypeInfo:  0x0000000142C3A3F0
struct ServerGameplayFightHarderMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06226
// TypeInfo:  0x0000000142C3A410
struct ServerGameplayDeserterReturnMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06225
// TypeInfo:  0x0000000142C3A430
struct ServerGameplayDeserterMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06224
// TypeInfo:  0x0000000142C3A450
struct ServerGameplayPlayerMenuCancelMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06223
// TypeInfo:  0x0000000142C3A470
struct ServerGameplayPlayerMenuOkMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06222
// TypeInfo:  0x0000000142C3A490
struct ServerGameplayVoiceOverFinishedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06221
// TypeInfo:  0x0000000142C3A4B0
struct ServerStaticModelDamagedPartByPlayerMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 06220
// TypeInfo:  0x0000000142C3A4D0
struct ServerStaticModelDestroyedPartMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 06219
// TypeInfo:  0x0000000142C3A4F0
struct ServerStaticModelGroupDestroyedPartMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 06218
// TypeInfo:  0x0000000142C3A510
struct ServerStaticModelDestroyedAllCollapsablePartsMessage
{
    char _0x0000[160];
};//Size=0x00A0

////////////////////////////////////////
// RuntimeId: 06217
// TypeInfo:  0x0000000142C3A530
struct ServerStaticModelSpawnMessage
{
    char _0x0000[144];
};//Size=0x0090

////////////////////////////////////////
// RuntimeId: 06216
// TypeInfo:  0x0000000142C3A550
struct ServerSoundVoiceOverFinishedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06215
// TypeInfo:  0x0000000142C3A570
struct ServerInputDeactivateInputRestrictionMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06214
// TypeInfo:  0x0000000142C3A590
struct ServerInputReactivateInputRestrictionMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06213
// TypeInfo:  0x0000000142C3A5B0
struct ServerCollisionExplosionPackDestroyedMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 06212
// TypeInfo:  0x0000000142C3A5D0
struct ServerCollisionExplosionPackPlacedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06211
// TypeInfo:  0x0000000142C3A5F0
struct ServerCollisionExplosionUnSpawnMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 06210
// TypeInfo:  0x0000000142C3A610
struct ServerCollisionExplosionDamageMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 06209
// TypeInfo:  0x0000000142C3A630
struct ServerCollisionExplosionSpawnMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 06208
// TypeInfo:  0x0000000142C3A650
struct ServerCollisionProjectileTimeoutMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 06207
// TypeInfo:  0x0000000142C3A670
struct ServerCollisionProjectileImpactMessage
{
    char _0x0000[112];
};//Size=0x0070

////////////////////////////////////////
// RuntimeId: 06206
// TypeInfo:  0x0000000142C3A690
struct ServerCollisionProjectileFireMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 06205
// TypeInfo:  0x0000000142C3A6B0
struct ServerCollisionGrenadeCollisionMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 06204
// TypeInfo:  0x0000000142C3A6D0
struct ServerCollisionGrenadeThrowMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 06203
// TypeInfo:  0x0000000142C3A6F0
struct ServerComponentEntryComponentProcessedInputMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06202
// TypeInfo:  0x0000000142C3A710
struct ServerComponentEntryOnPlayerExitsMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 06201
// TypeInfo:  0x0000000142C3A730
struct ServerComponentEntryOnPlayerEntersMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 06200
// TypeInfo:  0x0000000142C3A750
struct ServerComponentEntryOnUnspawnMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06199
// TypeInfo:  0x0000000142C3A770
struct ServerComponentWeaponOnUnspawnMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06198
// TypeInfo:  0x0000000142C3A790
struct ServerComponentWeaponOnSpawnMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06197
// TypeInfo:  0x0000000142C3A7B0
struct ServerEntityPickupOnUnspawnMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06196
// TypeInfo:  0x0000000142C3A7D0
struct ServerEntityPickupOnSpawnMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06195
// TypeInfo:  0x0000000142C3A7F0
struct ServerEntityBangerEntityOnUnspawnMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06194
// TypeInfo:  0x0000000142C3A810
struct ServerEntityBangerEntityOnSpawnMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06193
// TypeInfo:  0x0000000142C3A830
struct ServerClubMemberDeletedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06192
// TypeInfo:  0x0000000142C3A850
struct ServerClubMemberCreatedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06191
// TypeInfo:  0x0000000142C3A870
struct ServerControllableUnspawnDoneMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06190
// TypeInfo:  0x0000000142C3A890
struct ServerControllableSpawnDoneMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06189
// TypeInfo:  0x0000000142C3A8B0
struct ServerConnectionExitMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 06188
// TypeInfo:  0x0000000142C3A8D0
struct ServerConnectionInitMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06187
// TypeInfo:  0x0000000142C3A8F0
struct ServerWeaponPlayerWeaponPickupMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 06184
// TypeInfo:  0x0000000142C3A930
struct ServerWeaponReplacedMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 06183
// TypeInfo:  0x0000000142C3A950
struct ServerWeaponWeaponComponentActivateMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06182
// TypeInfo:  0x0000000142C3A970
struct ServerWeaponWeaponComponentReloadMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06181
// TypeInfo:  0x0000000142C3A990
struct ServerWeaponPlayerResupplyMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 06180
// TypeInfo:  0x0000000142C3A9B0
struct ServerWeaponPlayerReloadMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 06179
// TypeInfo:  0x0000000142C3A9D0
struct ServerWeaponPlayerWeaponDestroyedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06178
// TypeInfo:  0x0000000142C3A9F0
struct ServerWeaponPlayerWeaponRemovedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06177
// TypeInfo:  0x0000000142C3AA10
struct ServerWeaponPlayerWeaponReloadEndMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06176
// TypeInfo:  0x0000000142C3AA30
struct ServerWeaponPlayerPrimaryFireShotSpawnedMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 06175
// TypeInfo:  0x0000000142C3AA50
struct ServerWeaponPlayerPrimaryOutOfAmmoMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 06174
// TypeInfo:  0x0000000142C3AA70
struct ServerWeaponArtilleryFiredMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 06173
// TypeInfo:  0x0000000142C3AA90
struct ServerWeaponLaserDesignatorMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06172
// TypeInfo:  0x0000000142C3AAB0
struct ServerWeaponMortarStrikeMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 06171
// TypeInfo:  0x0000000142C3AAD0
struct ServerVehicleLockableMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 06170
// TypeInfo:  0x0000000142C3AAF0
struct ServerVehicleExitMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 06169
// TypeInfo:  0x0000000142C3AB10
struct ServerVehicleEnterMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 06168
// TypeInfo:  0x0000000142C3AB30
struct ServerVehicleDisabledMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 06167
// TypeInfo:  0x0000000142C3AB50
struct ServerVehicleDamageMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 06166
// TypeInfo:  0x0000000142C3AB70
struct ServerVehicleEnterRestrictionMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 06165
// TypeInfo:  0x0000000142C3AB90
struct ServerVehicleUnspawnMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06164
// TypeInfo:  0x0000000142C3ABB0
struct ServerVehicleSpawnDoneMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06163
// TypeInfo:  0x0000000142C3ABD0
struct ServerVehicleForceArmamentReturnMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06162
// TypeInfo:  0x0000000142C3ABF0
struct ServerVehicleSwitchTeamMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06161
// TypeInfo:  0x0000000142C3AC10
struct ServerVehicleDestroyedMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 06160
// TypeInfo:  0x0000000142C3AC30
struct ServerPlayerDisconnectMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 06159
// TypeInfo:  0x0000000142C3AC50
struct ServerPlayerStartedFireMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06158
// TypeInfo:  0x0000000142C3AC70
struct ServerPlayerInteractionEntityInRangeChangedMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 06157
// TypeInfo:  0x0000000142C3AC90
struct ServerPlayerOnPickupMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 06154
// TypeInfo:  0x0000000142C3ACD0
struct ServerPlayerAssistCountsAsKillMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 06153
// TypeInfo:  0x0000000142C3ACF0
struct ServerPlayerApplyCustomizationMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 06152
// TypeInfo:  0x0000000142C3AD10
struct ServerPlayerCustomizationDoneMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 06151
// TypeInfo:  0x0000000142C3AD30
struct ServerPlayerChatMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 06150
// TypeInfo:  0x0000000142C3AD50
struct ServerPlayerTickMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 06149
// TypeInfo:  0x0000000142C3AD70
struct ServerPlayerExitEntryMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 06148
// TypeInfo:  0x0000000142C3AD90
struct ServerPlayerEnterEntryMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 06147
// TypeInfo:  0x0000000142C3ADB0
struct ServerPlayerAboutToClearCharacterMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06146
// TypeInfo:  0x0000000142C3ADD0
struct ServerPlayerInstantSuicideMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 06145
// TypeInfo:  0x0000000142C3ADF0
struct ServerPlayerKilledMessage
{
    char _0x0000[112];
};//Size=0x0070

////////////////////////////////////////
// RuntimeId: 06144
// TypeInfo:  0x0000000142C3AE10
struct ServerPlayerManuallySelectedSpawnPointMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06143
// TypeInfo:  0x0000000142C3AE30
struct ServerPlayerChangeChatChannelMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 06142
// TypeInfo:  0x0000000142C3AE50
struct ServerPlayerSwitchTeamMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 06141
// TypeInfo:  0x0000000142C3AE70
struct ServerPlayerKitPickedupMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 06140
// TypeInfo:  0x0000000142C3AE90
struct ServerPlayerKitReplacedMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 06139
// TypeInfo:  0x0000000142C3AEB0
struct ServerPlayerChangedCharacterMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 06138
// TypeInfo:  0x0000000142C3AED0
struct ServerPlayerReviveRefusedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06137
// TypeInfo:  0x0000000142C3AEF0
struct ServerPlayerReviveAcceptedMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 06136
// TypeInfo:  0x0000000142C3AF10
struct ServerPlayerReviveMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 06135
// TypeInfo:  0x0000000142C3AF30
struct ServerPlayerLeftLevelMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06134
// TypeInfo:  0x0000000142C3AF50
struct ServerPlayerReleasingLevelMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06133
// TypeInfo:  0x0000000142C3AF70
struct ServerPlayerEnteredLevelMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 06132
// TypeInfo:  0x0000000142C3AF90
struct ServerPlayerLevelLoadedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06131
// TypeInfo:  0x0000000142C3AFB0
struct ServerPlayerDebugFriendZoneSpawnMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 06130
// TypeInfo:  0x0000000142C3AFD0
struct ServerPlayerRespawnMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06129
// TypeInfo:  0x0000000142C3AFF0
struct ServerPlayerDestroyMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06128
// TypeInfo:  0x0000000142C3B010
struct ServerPlayerCreatedForConnectionMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 06127
// TypeInfo:  0x0000000142C3B030
struct ServerPlayerCreateMessage
{
    char _0x0000[88];
};//Size=0x0058

////////////////////////////////////////
// RuntimeId: 06126
// TypeInfo:  0x0000000142C3B050
struct ServerPlayerAboutToCreateForConnectionMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 06125
// TypeInfo:  0x0000000142C3B070
struct ServerCharacterCharacterDamageMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 06124
// TypeInfo:  0x0000000142C3B090
struct ServerCharacterKilledMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06123
// TypeInfo:  0x0000000142C3B0B0
struct ServerMetricsDetonateExplosionMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 06122
// TypeInfo:  0x0000000142C3B0D0
struct ServerMetricsObjectiveSuccessMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06121
// TypeInfo:  0x0000000142C3B0F0
struct ServerMetricsSaveGameSavedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06120
// TypeInfo:  0x0000000142C3B110
struct ServerMetricsSaveGameLoadedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06119
// TypeInfo:  0x0000000142C3B130
struct ServerPlayerAccessLockedVisualContentMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06118
// TypeInfo:  0x0000000142C3B150
struct ServerClientConnectionRemovedMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 06117
// TypeInfo:  0x0000000142C3B170
struct ServerClientConnectionConnectedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06116
// TypeInfo:  0x0000000142C3B190
struct ServerAdminBanPlayerMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 06115
// TypeInfo:  0x0000000142C3B1B0
struct ServerAdminSetServerNameMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06114
// TypeInfo:  0x0000000142C3B1D0
struct ServerScriptTickMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06113
// TypeInfo:  0x0000000142C3B1F0
struct ServerStopMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06112
// TypeInfo:  0x0000000142C3B210
struct ServerLoadLevelMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06111
// TypeInfo:  0x0000000142C3B230
struct ServerLevelUnloadedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06110
// TypeInfo:  0x0000000142C3B250
struct ServerUnloadLevelMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06109
// TypeInfo:  0x0000000142C3B270
struct ServerLevelLoadedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06108
// TypeInfo:  0x0000000142C3B290
struct ServerStoppedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06107
// TypeInfo:  0x0000000142C3B2B0
struct ServerApplyConfigurationMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06106
// TypeInfo:  0x0000000142C3B2D0
struct ServerResetConfigurationMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06105
// TypeInfo:  0x0000000142C3B2F0
struct ServerStartedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 06104
// TypeInfo:  0x0000000142C3B310
struct ServerDoneLoadGameMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06103
// TypeInfo:  0x0000000142C3B330
struct ServerLoadGameMessage
{
    char _0x0000[112];
};//Size=0x0070

////////////////////////////////////////
// RuntimeId: 06102
// TypeInfo:  0x0000000142C3B350
struct SaveGameSaveMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06101
// TypeInfo:  0x0000000142C3B370
struct StatDisableMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06100
// TypeInfo:  0x0000000142C3B390
struct StatEnableMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 06058
// TypeInfo:  0x0000000142C368A8
struct PlayerScore
{
    __int32 m_Rank; //0x0000
    unsigned __int32 m_Kills; //0x0004
    unsigned __int32 m_Deaths; //0x0008
    unsigned __int32 m_ScoreMultiplier; //0x000C
    __int32 m_Score; //0x0010
    __int32 m_GlobalScoreOriginal; //0x0014
    __int32 m_GlobalScoreUpdated; //0x0018
    float m_Time; //0x001C
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 06056
// TypeInfo:  0x0000000142C35E88
struct UINetworkGunMasterNotificationMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 06053
// TypeInfo:  0x0000000142C35EC8
struct UINetworkCheckLevelInstalledMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 06052
// TypeInfo:  0x0000000142C35EE8
struct UINetworkCoopPlayerMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 06049
// TypeInfo:  0x0000000142C35F28
struct UINetworkCoopGameMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 06048
// TypeInfo:  0x0000000142C35F48
struct UINetworkSetCoopServerLobbyGameTypeMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 06047
// TypeInfo:  0x0000000142C35F68
struct UINetworkSquadWipeInstigatorMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 06046
// TypeInfo:  0x0000000142C35F88
struct UINetworkSquadWipeMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 06045
// TypeInfo:  0x0000000142C35FA8
struct UINetworkRoundWarningMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 06044
// TypeInfo:  0x0000000142C35FC8
struct UINetworkRollCreditsMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 06043
// TypeInfo:  0x0000000142C35FE8
struct UINetworkKilledOtherPlayerMessage
{
    char _0x0000[120];
};//Size=0x0078

////////////////////////////////////////
// RuntimeId: 06042
// TypeInfo:  0x0000000142C36008
struct UINetworkOwnPlayerKilledMessage
{
    char _0x0000[152];
};//Size=0x0098

////////////////////////////////////////
// RuntimeId: 06039
// TypeInfo:  0x0000000142C36048
struct UINetworkStealBodyMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 06038
// TypeInfo:  0x0000000142C36068
struct UINetworkEndOfRoundBonusMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 06037
// TypeInfo:  0x0000000142C36088
struct UINetworkMenuResponseMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 06036
// TypeInfo:  0x0000000142C360A8
struct UINetworkFieldUpgradeScoringMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 06035
// TypeInfo:  0x0000000142C360C8
struct UINetworkHudScoringMessage
{
    char _0x0000[152];
};//Size=0x0098

////////////////////////////////////////
// RuntimeId: 06034
// TypeInfo:  0x0000000142C360E8
struct UINetworkPlayerRankUpMessage
{
    char _0x0000[88];
};//Size=0x0058

////////////////////////////////////////
// RuntimeId: 06033
// TypeInfo:  0x0000000142C36108
struct UINetworkVoiceOverSubtitleTextMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 06032
// TypeInfo:  0x0000000142C36128
struct UINetworkPlayerCollectibleTextMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 06031
// TypeInfo:  0x0000000142C36148
struct UINetworkPlayerTutorialInstructionsTextMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 06030
// TypeInfo:  0x0000000142C36168
struct UINetworkPlayerMissionObjectiveTextMessage
{
    char _0x0000[104];
};//Size=0x0068

////////////////////////////////////////
// RuntimeId: 06029
// TypeInfo:  0x0000000142C36188
struct UINetworkVideoDoneMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 06028
// TypeInfo:  0x0000000142C361A8
struct UINetworkAllowSkipVideoMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 06027
// TypeInfo:  0x0000000142C361C8
struct UINetworkSkipVideoMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 06026
// TypeInfo:  0x0000000142C361E8
struct UINetworkStopVideoMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 06025
// TypeInfo:  0x0000000142C36208
struct UINetworkPauseVideoMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 06024
// TypeInfo:  0x0000000142C36228
struct UINetworkPlayVideoMessage
{
    char _0x0000[136];
};//Size=0x0088

////////////////////////////////////////
// RuntimeId: 06023
// TypeInfo:  0x0000000142C36248
struct UINetworkPlayerKillsTextMessage
{
    char _0x0000[136];
};//Size=0x0088

////////////////////////////////////////
// RuntimeId: 06022
// TypeInfo:  0x0000000142C36268
struct UINetworkPlayerDisconnectMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 06021
// TypeInfo:  0x0000000142C36288
struct UINetworkPlayerConnectMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 06020
// TypeInfo:  0x0000000142C362A8
struct UINetworkPlayerDeserterTextMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 06019
// TypeInfo:  0x0000000142C362C8
struct UINetworkAdminYellMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 06018
// TypeInfo:  0x0000000142C362E8
struct UINetworkPlayerTextMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 06017
// TypeInfo:  0x0000000142C368C8
struct UINetworkTextInfo
{
    char* m_StringId; //0x0000
    float m_DisplayTime; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 06015
// TypeInfo:  0x0000000142C36308
struct UINetworkHudTextMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 06014
// TypeInfo:  0x0000000142C36328
struct UINetworkHudTooltipMessage
{
    char _0x0000[88];
};//Size=0x0058

////////////////////////////////////////
// RuntimeId: 06013
// TypeInfo:  0x0000000142C36348
struct UINetworkEnableHudMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 06006
// TypeInfo:  0x0000000142C363C8
struct NetworkLevelInstalledMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 06005
// TypeInfo:  0x0000000142C363E8
struct NetworkSynchronizeInternetSimulationStateMessage
{
    char _0x0000[184];
};//Size=0x00B8

////////////////////////////////////////
// RuntimeId: 06004
// TypeInfo:  0x0000000142C36408
struct NetworkMatchReadyStatusChangedMessage
{
    char _0x0000[88];
};//Size=0x0058

////////////////////////////////////////
// RuntimeId: 06003
// TypeInfo:  0x0000000142C36428
struct ServerRestartTimerMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 06002
// TypeInfo:  0x0000000142C36448
struct NetworkMetricsLevelCompleteMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 06001
// TypeInfo:  0x0000000142C36468
struct NetworkMetricsLevelProgressMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 06000
// TypeInfo:  0x0000000142C36488
struct NetworkMetricsSaveGameSavedMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 05999
// TypeInfo:  0x0000000142C364A8
struct NetworkSetPlayerViewMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 05998
// TypeInfo:  0x0000000142C364C8
struct NetworkSuicideMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 05997
// TypeInfo:  0x0000000142C364E8
struct NetworkGameplayContinueMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 05996
// TypeInfo:  0x0000000142C36508
struct PerformanceLogMessage
{
    char _0x0000[256];
};//Size=0x0100

////////////////////////////////////////
// RuntimeId: 05995
// TypeInfo:  0x0000000142C368E8
struct PerformanceFpsHistogram
{
    float m_Below5; //0x0000
    float m_Below10; //0x0004
    float m_Below15; //0x0008
    float m_Below20; //0x000C
    float m_Below25; //0x0010
    float m_Below30; //0x0014
    float m_Below35; //0x0018
    float m_Below40; //0x001C
    float m_Below45; //0x0020
    float m_Below50; //0x0024
    float m_Below55; //0x0028
    float m_Below60; //0x002C
    float m_Above60; //0x0030
};//Size=0x0034

////////////////////////////////////////
// RuntimeId: 05993
// TypeInfo:  0x0000000142C36528
struct NetworkFirstPlayerEnteredMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 05992
// TypeInfo:  0x0000000142C36548
struct NetworkDifficultyChangedMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 05991
// TypeInfo:  0x0000000142C36568
struct NetworkSettingsMessage
{
    char _0x0000[120];
};//Size=0x0078

////////////////////////////////////////
// RuntimeId: 05990
// TypeInfo:  0x0000000142C36588
struct DebugSpawnGameEntityMessage
{
    char _0x0000[144];
};//Size=0x0090

////////////////////////////////////////
// RuntimeId: 05989
// TypeInfo:  0x0000000142C365A8
struct NetworkCreatePlayerMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 05988
// TypeInfo:  0x0000000142C365C8
struct NetworkCameraReplayMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 05987
// TypeInfo:  0x0000000142C365E8
struct NetworkCameraFreeCameraMessage
{
    char _0x0000[128];
};//Size=0x0080

////////////////////////////////////////
// RuntimeId: 05986
// TypeInfo:  0x0000000142C36608
struct NetworkMovePlayerMessage
{
    char _0x0000[144];
};//Size=0x0090

////////////////////////////////////////
// RuntimeId: 05985
// TypeInfo:  0x0000000142C36628
struct NetworkJuiceSessionMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 05984
// TypeInfo:  0x0000000142C36648
struct NetworkSelectTeamMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 05983
// TypeInfo:  0x0000000142C36668
struct NetworkOnPlayerSpawnedMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 05982
// TypeInfo:  0x0000000142C36688
struct NetworkSelectSpawnGroupMessage
{
    char _0x0000[104];
};//Size=0x0068

////////////////////////////////////////
// RuntimeId: 05981
// TypeInfo:  0x0000000142C366A8
struct NetworkSpawnVehicleCustomizationMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 05980
// TypeInfo:  0x0000000142C366C8
struct NetworkUnSpawnCustomizationMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 05979
// TypeInfo:  0x0000000142C366E8
struct NetworkSpawnCustomizationMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 05978
// TypeInfo:  0x0000000142C36708
struct NetworkSpawnOnSelectedMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 05977
// TypeInfo:  0x0000000142C36728
struct NetworkSpawnHereMessage
{
    char _0x0000[144];
};//Size=0x0090

////////////////////////////////////////
// RuntimeId: 05976
// TypeInfo:  0x0000000142C36748
struct NetworkSpawnMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 05975
// TypeInfo:  0x0000000142C36768
struct NetworkRequestLoadLevelMessage
{
    char _0x0000[120];
};//Size=0x0078

////////////////////////////////////////
// RuntimeId: 05974
// TypeInfo:  0x0000000142C36788
struct NetworkScreenFadeMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 05973
// TypeInfo:  0x0000000142C367A8
struct NetworkChangeGameSettingMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 05970
// TypeInfo:  0x0000000142C367E8
struct LevelConsoleSetActiveHealthStateMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 05969
// TypeInfo:  0x0000000142C36808
struct NetworkTimeSyncMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 05968
// TypeInfo:  0x0000000142C36828
struct NetworkLoadLevelMessage
{
    char _0x0000[136];
};//Size=0x0088

////////////////////////////////////////
// RuntimeId: 05967
// TypeInfo:  0x0000000142C36848
struct NetworkLevelLoadedAckMessage
{
    char _0x0000[88];
};//Size=0x0058

////////////////////////////////////////
// RuntimeId: 05966
// TypeInfo:  0x0000000142C36868
struct NetworkTinyEventMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 05965
// TypeInfo:  0x0000000142C36888
struct NetworkPerformanceProfileMessage
{
    char _0x0000[112];
};//Size=0x0070

////////////////////////////////////////
// RuntimeId: 05442
// TypeInfo:  0x0000000142C250E0
struct LevelSetupOption
{
    char* m_Criterion; //0x0000
    char* m_Value; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 05962
// TypeInfo:  0x0000000142C36928
struct BlueprintBundlePreloadInfo
{
    char* m_Name; //0x0000
    __int32 m_Compartment; //0x0008
    __int32 m_ParentCompartment; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 05960
// TypeInfo:  0x0000000142C344D8
struct SyncedSequenceStateChangeMessageBase
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 05959
// TypeInfo:  0x0000000142C344F8
struct JuiceSoldierRagdollDeactivateMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05958
// TypeInfo:  0x0000000142C34518
struct JuiceSoldierRagdollActivateMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05957
// TypeInfo:  0x0000000142C34538
struct CoreReadSaveGameDoneMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 05956
// TypeInfo:  0x0000000142C34558
struct CoreWriteSaveGameDoneMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05955
// TypeInfo:  0x0000000142C34578
struct CoreWriteProfileGameMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 05954
// TypeInfo:  0x0000000142C34598
struct CoreWriteSaveGameMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 05953
// TypeInfo:  0x0000000142C345B8
struct StatisticsEventMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05952
// TypeInfo:  0x0000000142C345D8
struct BlueprintBundleStreamedInMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05951
// TypeInfo:  0x0000000142C345F8
struct LoadGameBeginLoadMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 05950
// TypeInfo:  0x0000000142C34618
struct SaveGameBeginSaveMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05949
// TypeInfo:  0x0000000142C34638
struct SessionPlayerJoinedMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 05948
// TypeInfo:  0x0000000142C34658
struct SessionPlayerAuthenticatedMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 05947
// TypeInfo:  0x0000000142C34678
struct SessionPlayerLeftMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 05946
// TypeInfo:  0x0000000142C34698
struct CoreToggleBugSentryMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05945
// TypeInfo:  0x0000000142C346B8
struct CoreGameTimerMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05944
// TypeInfo:  0x0000000142C346D8
struct CoreExitIngameMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05943
// TypeInfo:  0x0000000142C346F8
struct CoreEnteredIngameMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05942
// TypeInfo:  0x0000000142C34718
struct PerformanceClientNetworkMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05941
// TypeInfo:  0x0000000142C34738
struct PerformanceServerNetworkMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05940
// TypeInfo:  0x0000000142C34758
struct PerformanceServerMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05939
// TypeInfo:  0x0000000142C34778
struct ProfileOptionsSettingsSavedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05938
// TypeInfo:  0x0000000142C34798
struct ProfileOptionsSettingsPreSaveMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05937
// TypeInfo:  0x0000000142C347B8
struct ProfileOptionsSettingsLoadedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05936
// TypeInfo:  0x0000000142C347D8
struct ProfileOptionsApplyMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 05933
// TypeInfo:  0x0000000142C34818
struct NetworkDisconnectedMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 05932
// TypeInfo:  0x0000000142C34838
struct NetworkConnectedMessage
{
    char _0x0000[112];
};//Size=0x0070

////////////////////////////////////////
// RuntimeId: 05931
// TypeInfo:  0x0000000142C34858
struct AIClientBridgeDynamicModelEntityOnUnspawnMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05930
// TypeInfo:  0x0000000142C34878
struct AIClientBridgeDynamicModelEntityOnSpawnMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05927
// TypeInfo:  0x0000000142C348B8
struct BundleManagerBundleUnloadMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05926
// TypeInfo:  0x0000000142C348D8
struct CoreReadSaveGameDataDoneMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05925
// TypeInfo:  0x0000000142C348F8
struct CoreDebugReadProfileGameDataMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 05924
// TypeInfo:  0x0000000142C34918
struct CoreDebugReadSaveGameDataMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 05923
// TypeInfo:  0x0000000142C34938
struct NetworkLogicFireDoublePlayerEventMessageBase
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 05922
// TypeInfo:  0x0000000142C34958
struct NetworkLogicFirePlayerEventMessageBase
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 05921
// TypeInfo:  0x0000000142C34978
struct NetworkLogicFireEventMessageBase
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 05920
// TypeInfo:  0x0000000142C34998
struct SubLevelFromClientSubLevelLoadedMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 05919
// TypeInfo:  0x0000000142C349B8
struct SubLevelFromClientRequestBundleBaselineMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 05918
// TypeInfo:  0x0000000142C349D8
struct SubLevelToClientDropBundleBaselineMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 05917
// TypeInfo:  0x0000000142C349F8
struct SubLevelToClientUnloadRequestsMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 05916
// TypeInfo:  0x0000000142C34A18
struct SubLevelToClientLoadRequestsMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 05915
// TypeInfo:  0x0000000142C34B38
struct SubLevelBundleInfo
{
    __int32 m_CompartmentIndex; //0x0000
    BundleHeapInfo m_HeapInfo; //0x0004
    char* m_Name; //0x0010
    unsigned __int16 m_SubLevelNameInx; //0x0018
    unsigned __int16 m_SubLevelId; //0x001A
    unsigned __int16 m_ParentSubLevelId; //0x001C
    unsigned __int8 m_Priority; //0x001E
    bool m_IsBlueprintBundle; //0x001F
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 05913
// TypeInfo:  0x0000000142C34A38
struct SubLevelToClientSubLevelNameMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 05912
// TypeInfo:  0x0000000142C34B58
struct BundleNameAndIndex
{
    char* m_Name; //0x0000
    unsigned __int16 m_Index; //0x0008
    char _0x000A[6];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 05906
// TypeInfo:  0x0000000142C34A98
struct ServerAdministrationPasswordMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05905
// TypeInfo:  0x0000000142C34AB8
struct ServerAdministrationEventsEnabledMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 05904
// TypeInfo:  0x0000000142C34AD8
struct ServerAdministrationQuitMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05903
// TypeInfo:  0x0000000142C34AF8
struct ServerAdministrationLoginMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 05902
// TypeInfo:  0x0000000142C34B18
struct ServerAdministrationPacketMessageBase
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 05822
// TypeInfo:  0x0000000142C2F330
struct ClientSetServerPasswordMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05821
// TypeInfo:  0x0000000142C2F350
struct ClientWantFullscreenMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05820
// TypeInfo:  0x0000000142C2F370
struct ClientLeftRemoteServerMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05819
// TypeInfo:  0x0000000142C2F390
struct ClientDisconnectedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05818
// TypeInfo:  0x0000000142C2F3B0
struct ClientConnectedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05817
// TypeInfo:  0x0000000142C2F3D0
struct ClientAbortCutsceneMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05816
// TypeInfo:  0x0000000142C2F3F0
struct ClientLevelLoadedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05815
// TypeInfo:  0x0000000142C2F410
struct ClientLevelLoadProgressMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05814
// TypeInfo:  0x0000000142C2F430
struct ClientLevelDescriptionLoadedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05813
// TypeInfo:  0x0000000142C2F450
struct ClientLevelUnloadedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05812
// TypeInfo:  0x0000000142C2F470
struct ClientLoadLevelMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05811
// TypeInfo:  0x0000000142C2F490
struct ClientLoadLevelRequestedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05810
// TypeInfo:  0x0000000142C2F4B0
struct ClientEnteredIngameMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05809
// TypeInfo:  0x0000000142C2F4D0
struct ClientEnterHudIngameMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05808
// TypeInfo:  0x0000000142C2F4F0
struct ClientExitGameMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05807
// TypeInfo:  0x0000000142C2F510
struct ClientExitToMenuMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05806
// TypeInfo:  0x0000000142C2F530
struct ClientReturnToMenuMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05805
// TypeInfo:  0x0000000142C2F550
struct ClientStartMultiplayerMessage
{
    char _0x0000[104];
};//Size=0x0068

////////////////////////////////////////
// RuntimeId: 05804
// TypeInfo:  0x0000000142C2F570
struct ClientContinueSingleplayerMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05803
// TypeInfo:  0x0000000142C2F590
struct ClientStartedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05802
// TypeInfo:  0x0000000142C2F5B0
struct ClientJoinServerJobMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05801
// TypeInfo:  0x0000000142C2F5D0
struct ClientPeerNetworkRemovedMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05800
// TypeInfo:  0x0000000142C2F5F0
struct ClientRequirePasswordMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05799
// TypeInfo:  0x0000000142C2F610
struct ClientJoinMultiplayerMessageBase
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 05798
// TypeInfo:  0x0000000142C2F630
struct ClientRestartSingleplayerMessageBase
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 05797
// TypeInfo:  0x0000000142C2F650
struct ClientStartSingleplayerMessageBase
{
    char _0x0000[112];
};//Size=0x0070

////////////////////////////////////////
// RuntimeId: 05644
// TypeInfo:  0x0000000142C28EB0
struct UserGamerpicBufferReceivedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05635
// TypeInfo:  0x0000000142C28ED0
struct UIBundleUnloadedMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 05634
// TypeInfo:  0x0000000142C28EF0
struct UIBundleLoadedMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 05627
// TypeInfo:  0x0000000142C28F30
struct ClientInputDeviceAddedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05626
// TypeInfo:  0x0000000142C28F50
struct ClientEffectPlayMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 05625
// TypeInfo:  0x0000000142C28F70
struct ClientCommanderPlayerChangedMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 05624
// TypeInfo:  0x0000000142C28F90
struct ClientConnectionUnloadLevelMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05623
// TypeInfo:  0x0000000142C28FB0
struct ClientConnectionLinkLevelMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05622
// TypeInfo:  0x0000000142C28FD0
struct ClientConnectionLoadLevelMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05621
// TypeInfo:  0x0000000142C28FF0
struct ClientConnectionInitializedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05620
// TypeInfo:  0x0000000142C29010
struct ClientStateChangingStateMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05619
// TypeInfo:  0x0000000142C29030
struct ClientLevelFinalizedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05618
// TypeInfo:  0x0000000142C29050
struct ClientLevelSpawnEntitiesEndMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05617
// TypeInfo:  0x0000000142C29070
struct ClientLevelSpawnDebugEntitiesMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05616
// TypeInfo:  0x0000000142C29090
struct ClientGameplaySoldierHealthRequestMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05615
// TypeInfo:  0x0000000142C290B0
struct ClientGameplayControllableLowHealthMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05614
// TypeInfo:  0x0000000142C290D0
struct ClientGameplaySoldierHitMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 05613
// TypeInfo:  0x0000000142C290F0
struct ClientCameraShowKillerMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05612
// TypeInfo:  0x0000000142C29110
struct ClientCollisionProjectileImpactMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 05611
// TypeInfo:  0x0000000142C29130
struct ClientCollisionSpawnExplosionMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 05610
// TypeInfo:  0x0000000142C29150
struct ClientCollisionExplosionPackDetonatedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05609
// TypeInfo:  0x0000000142C29170
struct ClientCollisionExplosionPackPlacedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05608
// TypeInfo:  0x0000000142C29190
struct ClientCollisionGrenadeCollisionMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 05607
// TypeInfo:  0x0000000142C291B0
struct ClientSoundVoiceOverFinishedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05606
// TypeInfo:  0x0000000142C291D0
struct ClientVehicleCriticalDamageMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05605
// TypeInfo:  0x0000000142C291F0
struct ClientEntityDummyToGetTheClientEntityMessageCategoryMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05604
// TypeInfo:  0x0000000142C29210
struct ClientCharacterSpawnDoneMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05603
// TypeInfo:  0x0000000142C29230
struct ClientCharacterLocalPlayerDeletedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05602
// TypeInfo:  0x0000000142C29250
struct ClientCharacterLocalPlayerSetMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05601
// TypeInfo:  0x0000000142C29270
struct ClientSpawnSpawnedOrUnSpawnedMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 05600
// TypeInfo:  0x0000000142C29290
struct ClientControllableUnspawnDoneMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05599
// TypeInfo:  0x0000000142C292B0
struct ClientControllableSpawnDoneMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05598
// TypeInfo:  0x0000000142C292D0
struct ClientInputUnchangedInputMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05597
// TypeInfo:  0x0000000142C292F0
struct ClientInputSettingsRefreshMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05596
// TypeInfo:  0x0000000142C29310
struct ClientWeaponDispersionUpdatedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05595
// TypeInfo:  0x0000000142C29330
struct ClientWeaponPlayerPrimaryOutOfAmmoMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05594
// TypeInfo:  0x0000000142C29350
struct ClientWeaponPlayerWeaponReloadEndMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05593
// TypeInfo:  0x0000000142C29370
struct ClientWeaponPlayerWeaponReloadBeginMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05592
// TypeInfo:  0x0000000142C29390
struct ClientWeaponPlayerPrimaryWeaponFireMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05591
// TypeInfo:  0x0000000142C293B0
struct ClientWeaponPlayerWeaponChangeMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05590
// TypeInfo:  0x0000000142C293D0
struct ClientPlayerReviveRefusedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05589
// TypeInfo:  0x0000000142C293F0
struct ClientPlayerReviveAcceptedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05588
// TypeInfo:  0x0000000142C29410
struct ClientPlayerReviveMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05587
// TypeInfo:  0x0000000142C29430
struct ClientPlayerInteractionEntityInRangeChangedMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 05586
// TypeInfo:  0x0000000142C29450
struct ClientPlayerOnWeaponUndeployFinishedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05585
// TypeInfo:  0x0000000142C29470
struct ClientPlayerSelectedUnlocksMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05584
// TypeInfo:  0x0000000142C29490
struct ClientPlayerDeletedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05583
// TypeInfo:  0x0000000142C294B0
struct ClientPlayerConnectMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 05582
// TypeInfo:  0x0000000142C294D0
struct ClientPlayerLocalSetMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 05581
// TypeInfo:  0x0000000142C294F0
struct ClientPlayerChangedPlayerViewMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05580
// TypeInfo:  0x0000000142C29510
struct ClientPlayerSwitchGroupMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 05579
// TypeInfo:  0x0000000142C29530
struct ClientPlayerSwitchTeamMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 05578
// TypeInfo:  0x0000000142C29550
struct ClientPlayerManDownMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05577
// TypeInfo:  0x0000000142C29570
struct ClientPlayerWeaponPickupMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05576
// TypeInfo:  0x0000000142C29590
struct ClientPlayerAmmoPickupMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05575
// TypeInfo:  0x0000000142C295B0
struct ClientPlayerShowKillCardOnlyMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05574
// TypeInfo:  0x0000000142C295D0
struct ClientPlayerShowKillCameraMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 05573
// TypeInfo:  0x0000000142C295F0
struct ClientPlayerKilledMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05572
// TypeInfo:  0x0000000142C29610
struct ClientPlayerEnterEntryMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 05571
// TypeInfo:  0x0000000142C29630
struct ClientPlayerEnterExitVehicleMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 05570
// TypeInfo:  0x0000000142C29650
struct ClientPlayerRequestCameraChangeMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 05569
// TypeInfo:  0x0000000142C29670
struct ClientPlayerUpdateCameraComponentMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05568
// TypeInfo:  0x0000000142C29690
struct ClientMetricsPauseGameMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05567
// TypeInfo:  0x0000000142C296B0
struct ClientMetricsUIActionMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 05566
// TypeInfo:  0x0000000142C296D0
struct ClientMetricsPopUIScreenMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05565
// TypeInfo:  0x0000000142C296F0
struct ClientMetricsPushUIScreenMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 05514
// TypeInfo:  0x0000000142C264F8
struct EventKeyframe
{
    float m_Time; //0x0000
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 05490
// TypeInfo:  0x0000000142C26518
struct ColorKeyframe
{
    Vec4 m_RGBColor; //0x0000
    float m_Time; //0x0010
    char _0x0014[12];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 05486
// TypeInfo:  0x0000000142C26538
struct BoolKeyframe
{
    float m_Time; //0x0000
    bool m_Value; //0x0004
    char _0x0005[3];
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 05444
// TypeInfo:  0x0000000142C250C0
struct LevelSetup
{
    char* m_Name; //0x0000
    Array<LevelSetupOption> m_InclusionOptions; //0x0008
    unsigned __int32 m_DifficultyIndex; //0x0010
    char _0x0014[4];
    Array<char*> m_SubLevelNames; //0x0018
    char* m_StartPoint; //0x0020
    Array<__int32> m_SubLevelStates; //0x0028
    bool m_IsSaveGame; //0x0030
    bool m_ForceReloadResources; //0x0031
    char _0x0032[6];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 05964
// TypeInfo:  0x0000000142C36908
struct LoadLevelInfo
{
    LevelSetup m_Setup; //0x0000
    Array<BlueprintBundlePreloadInfo> m_BlueprintBundlePreloads; //0x0038
    unsigned __int32 m_LevelSequenceNumber; //0x0040
    char _0x0044[4];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 05402
// TypeInfo:  0x0000000142C223C8
struct TransformPartPropertyKey
{
    float m_Value; //0x0000
    AnimTangentType m_InAnimTangentType; //0x0004
    float m_InAngle; //0x0008
    float m_InWeight; //0x000C
    AnimTangentType m_OutAnimTangentType; //0x0010
    float m_OutAngle; //0x0014
    float m_OutWeight; //0x0018
};//Size=0x001C

////////////////////////////////////////
// RuntimeId: 05360
// TypeInfo:  0x0000000142C223E8
struct SequenceEventData
{
    EventSpec m_Event; //0x0000
    __int32 m_Time; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 05352
// TypeInfo:  0x0000000142C22428
struct MaterialRelationPropertyPair
{
    Array<PhysicsMaterialRelationPropertyData*> m_PhysicsMaterialProperties; //0x0000
    Array<PhysicsPropertyRelationPropertyData*> m_PhysicsPropertyProperties; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 05354
// TypeInfo:  0x0000000142C22408
struct MaterialInteractionGridRow
{
    Array<MaterialRelationPropertyPair> m_Items; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 07673
// TypeInfo:  0x0000000142C59590
struct OnImpulseModifierData
{
    MaterialDecl m_MaterialPair; //0x0000
    float m_MagnitudeScale; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 05314
// TypeInfo:  0x0000000142C1DE68
struct SubLevelDestroyedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05313
// TypeInfo:  0x0000000142C1DE88
struct SubLevelEntitiesCreatedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 05286
// TypeInfo:  0x0000000142C1DF48
struct SubSkeleton
{
    SkeletonAsset* m_Skeleton; //0x0000
    Array<__int32> m_BoneMap; //0x0008
    Array<LinearTransform> m_TransformMap; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 05280
// TypeInfo:  0x0000000142C1DF68
struct GameplayBone
{
    char* m_Name; //0x0000
    GameplayBones m_Bone; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 05138
// TypeInfo:  0x0000000142C1DF88
struct EntityStableUid
{
    unsigned __int32 m_Id; //0x0000
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 05136
// TypeInfo:  0x0000000142C1DFA8
struct EntityOwnerUid
{
    unsigned __int32 m_Id; //0x0000
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 05134
// TypeInfo:  0x0000000142C1DFC8
struct EntityUid
{
    unsigned __int32 m_Id; //0x0000
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 05090
// TypeInfo:  0x0000000142C1DFE8
struct PropertyChannel
{
    Realm m_Realm; //0x0000
    __int32 m_Id; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 05088
// TypeInfo:  0x0000000142C1E008
struct EventChannel
{
    Realm m_Realm; //0x0000
    __int32 m_Id; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 05086
// TypeInfo:  0x0000000142C1E028
struct LinkChannel
{
    Realm m_Realm; //0x0000
    __int32 m_Id; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 05056
// TypeInfo:  0x0000000142C1E048
struct DynamicLink
{
    __int32 m_Id; //0x0000
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 05054
// TypeInfo:  0x0000000142C1E068
struct DynamicEvent
{
    __int32 m_Id; //0x0000
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 05052
// TypeInfo:  0x0000000142C1E088
struct EventConnection
{
    DataContainer* m_Source; //0x0000
    DataContainer* m_Target; //0x0008
    EventSpec m_SourceEvent; //0x0010
    EventSpec m_TargetEvent; //0x0014
    EventConnectionTargetType m_TargetType; //0x0018
    char _0x001C[4];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 06969
// TypeInfo:  0x0000000142C51390
struct AntEventData
{
    __int32 m_TagId; //0x0000
    EventSpec m_Event; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 06995
// TypeInfo:  0x0000000142C51130
struct BlueprintBundleReference
{
    char* m_Name; //0x0000
    BlueprintBundleSettings m_Settings; //0x0008
    unsigned __int32 m_BaseBundleHash; //0x0014
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 04903
// TypeInfo:  0x0000000142C152C8
struct EmitterExclusionVolumeBoundingSphereSoA
{
    Vec4 m_PosX; //0x0000
    Vec4 m_PosY; //0x0010
    Vec4 m_PosZ; //0x0020
    Vec4 m_RadiusSqr; //0x0030
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 04901
// TypeInfo:  0x0000000142C152E8
struct EmitterExclusionVolume
{
    Vec4 m_Left; //0x0000
    Vec4 m_Up; //0x0010
    Vec4 m_Forward; //0x0020
    Vec4 m_HalfExtents; //0x0030
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 04753
// TypeInfo:  0x0000000142C15250
struct PolynomialTempData
{
    Vec4 m_Coefficients; //0x0000
    float m_ScaleValue; //0x0010
    float m_MinClamp; //0x0014
    float m_MaxClamp; //0x0018
    char _0x001C[4];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 04744
// TypeInfo:  0x0000000142C12BC0
struct BreakablePartToStaticEntityPart
{
    unsigned __int32 m_BreakablePartIndex; //0x0000
    unsigned __int32 m_StaticModelPartIndex; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 04742
// TypeInfo:  0x0000000142C12BE0
struct StaticModelToBreakableParts
{
    unsigned __int32 m_StaticModelIndex; //0x0000
    unsigned __int32 m_BreakablePartStartIndex; //0x0004
    unsigned __int32 m_BreakablePartCount; //0x0008
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 04734
// TypeInfo:  0x0000000142C12B40
struct BreakableModelToComponentsSpawnPartObjectsOnCollapseMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 04733
// TypeInfo:  0x0000000142C12B60
struct BreakableModelToComponentsCollapseMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 04706
// TypeInfo:  0x0000000142C11B88
struct DebrisClusterPartInfoData
{
    Vec3 m_LinearVelocity; //0x0000
    Vec3 m_AngularVelocity; //0x0010
    __int32 m_PartIndex; //0x0020
    __int32 m_NumberOfChildren; //0x0024
    float m_SplitSpeedThreshold; //0x0028
    bool m_SyncRestPosition; //0x002C
    bool m_SyncContinous; //0x002D
    bool m_InEffectWorldOnly; //0x002E
    char _0x002F[1];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 04700
// TypeInfo:  0x0000000142C11BA8
struct DebrisHavokInfo
{
    HavokAsset* m_HavokAsset; //0x0000
    __int32 m_ReserveCount; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 04698
// TypeInfo:  0x0000000142C11BC8
struct DebrisSystemMetrics
{
    __int32 m_HavokParticleCount; //0x0000
    __int32 m_HavokParticlePartCount; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 04625
// TypeInfo:  0x0000000142C09B90
struct AudioLanguageMapping
{
    AudioLanguage* m_Source; //0x0000
    AudioLanguage* m_Target; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 04617
// TypeInfo:  0x0000000142C09BB0
struct SoundPatchPublicNode
{
    AudioGraphNodeData* m_Node; //0x0000
    unsigned __int32 m_Id; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 04599
// TypeInfo:  0x0000000142C09BD0
struct SoundWaveLocalizationInfo
{
    AudioLanguage* m_Language; //0x0000
    unsigned __int16 m_FirstVariationIndex; //0x0008
    unsigned __int16 m_VariationCount; //0x000A
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 04593
// TypeInfo:  0x0000000142C09BF0
struct SoundDataReference
{
    DataContainer* m_DataOwner; //0x0000
    SoundDataAsset* m_SoundData; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 04581
// TypeInfo:  0x0000000142C09C10
struct MixerInputInfo
{
    MixerValueAccumulateMode m_Mode; //0x0000
    bool m_KeepValue; //0x0004
    char _0x0005[3];
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 04575
// TypeInfo:  0x0000000142C09C30
struct MixerPresetNodeData
{
    AudioGraphNodeData* m_Node; //0x0000
    float m_Value; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 04571
// TypeInfo:  0x0000000142C09C70
struct MixGroupPropertyValue
{
    unsigned __int32 m_Property; //0x0000
    float m_Value; //0x0004
    bool m_Controlled; //0x0008
    char _0x0009[3];
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 04573
// TypeInfo:  0x0000000142C09C50
struct MixerPresetGroupData
{
    MixGroup* m_Group; //0x0000
    MixGroupState m_State; //0x0008
    float m_AttackTime; //0x000C
    float m_ReleaseTime; //0x0010
    __int32 m_Priority; //0x0014
    Array<MixGroupPropertyValue> m_Properties; //0x0018
    bool m_IsDominant; //0x0020
    bool m_IsAdditive; //0x0021
    char _0x0022[6];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 04565
// TypeInfo:  0x0000000142C09C90
struct MixGroupPropertyParameters
{
    unsigned __int32 m_Property; //0x0000
    float m_MinOffset; //0x0004
    float m_MaxOffset; //0x0008
    float m_DefaultValue; //0x000C
    bool m_EnableOffsetMinMax; //0x0010
    bool m_OverrideDefaultValue; //0x0011
    char _0x0012[2];
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 04511
// TypeInfo:  0x0000000142C09A10
struct AudioGraphNodePort
{
    float m_UnconnectedValue; //0x0000
    unsigned __int16 m_ValueIndex; //0x0004
    bool m_IsConnected; //0x0006
    char _0x0007[1];
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 04453
// TypeInfo:  0x0000000142C09CB0
struct RangeMapperEntry
{
    float m_RangeStart; //0x0000
    float m_RangeEnd; //0x0004
    float m_OutputValue; //0x0008
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 04355
// TypeInfo:  0x0000000142C09CD0
struct VoiceOverManuscriptLanguageColumns
{
    AudioLanguage* m_Language; //0x0000
    char* m_TextColumn; //0x0008
    char* m_SubtitleOffsetColumn; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 04327
// TypeInfo:  0x0000000142C09A30
struct VoiceOverConversationInfo
{
    Array<VoiceOverDialogGroup*> m_Groups; //0x0000
    Array<VoiceOverDialogTrack*> m_Tracks; //0x0008
    Array<VoiceOverPronunciation*> m_Pronunciations; //0x0010
    VoiceOverConversationInterruptMode m_InterruptMode; //0x0018
    __int32 m_Priority; //0x001C
    VoiceOverConversationQueueMode m_QueueMode; //0x0020
    float m_Relevancy; //0x0024
    unsigned __int8 m_LastSequenceIndex; //0x0028
    char _0x0029[7];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 04315
// TypeInfo:  0x0000000142C09CF0
struct VoiceOverDialogTakeMapping
{
    float m_TakeControlMin; //0x0000
    float m_TakeControlMax; //0x0004
    unsigned __int8 m_TakeIndex; //0x0008
    char _0x0009[3];
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 04309
// TypeInfo:  0x0000000142C09D10
struct VoiceOverDialogTake
{
    SoundWaveAsset* m_Wave; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 04229
// TypeInfo:  0x0000000142C09A50
struct VoiceOverValueConnection
{
    VoiceOverExpressionNode* m_TargetNode; //0x0000
    VoiceOverValue* m_TargetValue; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 04059
// TypeInfo:  0x0000000142C09B30
struct SoundGraphPluginRef
{
    bool m_IsValid; //0x0000
    unsigned __int8 m_VoiceIndex; //0x0001
    unsigned __int8 m_PluginIndex; //0x0002
};//Size=0x0003

////////////////////////////////////////
// RuntimeId: 04167
// TypeInfo:  0x0000000142C09D50
struct MusicOverlayVariation
{
    float m_MinimumTimeRemaining; //0x0000
    char _0x0004[4];
    SoundWaveAsset* m_Wave; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 04115
// TypeInfo:  0x0000000142C09D70
struct SoundScopeStrategyMapping
{
    SoundScopeData* m_Scope; //0x0000
    SoundScopeStrategyData* m_Strategy; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 04107
// TypeInfo:  0x0000000142C09D90
struct SoundWaveRuntimeVariation
{
    unsigned __int32 m_PersistentDataSize; //0x0000
    unsigned __int16 m_FirstSegmentIndex; //0x0004
    unsigned __int8 m_FirstSubtitleIndex; //0x0006
    unsigned __int8 m_SubtitleCount; //0x0007
    unsigned __int8 m_SegmentCount; //0x0008
    unsigned __int8 m_ChunkIndex; //0x0009
    unsigned __int8 m_FirstLoopSegmentIndex; //0x000A
    unsigned __int8 m_LastLoopSegmentIndex; //0x000B
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 04103
// TypeInfo:  0x0000000142C09DB0
struct SoundWaveSubtitle
{
    float m_Time; //0x0000
    unsigned __int8 m_StringIndex; //0x0004
    char _0x0005[3];
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 04101
// TypeInfo:  0x0000000142C09DD0
struct SoundWaveVariationSegment
{
    unsigned __int32 m_SamplesOffset; //0x0000
    unsigned __int32 m_SeekTableOffset; //0x0004
    float m_SegmentLength; //0x0008
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 04087
// TypeInfo:  0x0000000142C09DF0
struct SoundDataChunk
{
    Guid m_ChunkId; //0x0000
    char _0x0000[16];
    unsigned __int32 m_ChunkSize; //0x0010
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 04061
// TypeInfo:  0x0000000142C09E50
struct SoundGraphPluginInfo
{
    unsigned __int32 m_Id; //0x0000
    unsigned __int32 m_EnableAttributeReadMask; //0x0004
    unsigned __int8 m_ConnectionIndex; //0x0008
    unsigned __int8 m_OutputChannelCount; //0x0009
    unsigned __int8 m_ConstructParamsIndex; //0x000A
    unsigned __int8 m_ConstructParamCount; //0x000B
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 04065
// TypeInfo:  0x0000000142C09E10
struct SoundGraphVoiceInfo
{
    Array<SoundGraphPluginInfo> m_Plugins; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 04063
// TypeInfo:  0x0000000142C09E30
struct SoundGraphLinkedPluginAttribute
{
    float m_UnconnectedValue; //0x0000
    unsigned __int16 m_ValueIndex; //0x0004
    unsigned __int16 m_NodeIndexAndFlags; //0x0006
    unsigned __int8 m_VoiceIndex; //0x0008
    unsigned __int8 m_PluginIndex; //0x0009
    unsigned __int8 m_AttributeIndex; //0x000A
    char _0x000B[1];
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 04051
// TypeInfo:  0x0000000142C09EB0
struct SoundGraphPluginConnectionParam
{
    __int32 m_Value; //0x0000
    unsigned __int8 m_Index; //0x0004
    char _0x0005[3];
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 04199
// TypeInfo:  0x0000000142C09D30
struct MusicPlayerPlugins
{
    SoundGraphPluginRef m_SndPlayer; //0x0000
    SoundGraphPluginRef m_Rechannel; //0x0003
    SoundGraphPluginRef m_Resample; //0x0006
    SoundGraphPluginRef m_Pause; //0x0009
    SoundGraphPluginRef m_Gain; //0x000C
};//Size=0x000F

////////////////////////////////////////
// RuntimeId: 04057
// TypeInfo:  0x0000000142C09E70
struct SoundGraphPluginConstructParam
{
    float m_Value; //0x0000
    unsigned __int8 m_Index; //0x0004
    char _0x0005[3];
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 04055
// TypeInfo:  0x0000000142C09E90
struct SoundGraphPluginConnection
{
    SoundGraphPluginConnectionType m_ConnectionType; //0x0000
    char _0x0004[4];
    SoundBusData* m_Bus; //0x0008
    Array<SoundGraphPluginConnectionParam> m_Parameters; //0x0010
    unsigned __int8 m_VoiceIndex; //0x0018
    unsigned __int8 m_PluginIndex; //0x0019
    unsigned __int8 m_SignalIndex; //0x001A
    char _0x001B[5];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 04067
// TypeInfo:  0x0000000142C09B10
struct SoundGraphInfo
{
    Array<SoundGraphVoiceInfo> m_Voices; //0x0000
    Array<SoundGraphLinkedPluginAttribute> m_LinkedPluginAttributes; //0x0008
    Array<SoundGraphPluginConnection> m_Connections; //0x0010
    Array<SoundGraphPluginConstructParam> m_ConstructParams; //0x0018
    unsigned __int32 m_PluginsParamCount; //0x0020
    unsigned __int32 m_PluginCount; //0x0024
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 04045
// TypeInfo:  0x0000000142C09ED0
struct PointEnvelopePoint
{
    float m_X; //0x0000
    float m_Y; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 04017
// TypeInfo:  0x0000000142C09EF0
struct SamplerPlugins
{
    SoundGraphPluginRef m_SndPlayer; //0x0000
    SoundGraphPluginRef m_Resample; //0x0003
    SoundGraphPluginRef m_Pause; //0x0006
    SoundGraphPluginRef m_Gain; //0x0009
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 03931
// TypeInfo:  0x0000000142C09F10
struct DivisibleLoopPlayerPlugins
{
    SoundGraphPluginRef m_SndPlayer; //0x0000
    SoundGraphPluginRef m_Pause; //0x0003
    SoundGraphPluginRef m_Gain; //0x0006
    SoundGraphPluginRef m_GainFader; //0x0009
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 07949
// TypeInfo:  0x0000000142C611A0
struct AntAnimationHandlerData
{
    AntAnimatableData m_Animatable; //0x0000
    AntRef m_RootController; //0x0060
    LodBinding m_LodBinding; //0x0074
    Array<__int32> m_JointOutputFieldHashes; //0x00D8
    Array<__int32> m_JointEnabledFieldHashes; //0x00E0
    AntControllerComplexity m_ControllerComplexity; //0x00E8
    bool m_ReportBackFromAnt; //0x00EC
    bool m_EnableMasterSlaveCopy; //0x00ED
    bool m_IsProp; //0x00EE
    char _0x00EF[1];
};//Size=0x00F0

////////////////////////////////////////
// RuntimeId: 03883
// TypeInfo:  0x0000000142C005A0
struct AntScenario
{
    char _0x0000[1];
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 03881
// TypeInfo:  0x0000000142C005C0
struct AntRefInfo
{
    Guid m_FrostbitePartition; //0x0000
    char _0x0000[16];
    AntRef m_AntRef; //0x0010
};//Size=0x0024

////////////////////////////////////////
// RuntimeId: 09918
// TypeInfo:  0x0000000142C89708
struct WeaponLagBinding
{
    AntRef m_EntityPitch; //0x0000
    AntRef m_EntityYaw; //0x0014
    AntRef m_PitchResult; //0x0028
    AntRef m_YawResult; //0x003C
    AntRef m_RollResult; //0x0050
};//Size=0x0064

////////////////////////////////////////
// RuntimeId: 03850
// TypeInfo:  0x0000000142BFF7E8
struct CrossGameLauncherSuccessMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 03849
// TypeInfo:  0x0000000142BFF808
struct CrossGameLauncherFailedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 03848
// TypeInfo:  0x0000000142BFF828
struct CrossGameLauncherStartGameMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 03845
// TypeInfo:  0x0000000142BFF848
struct CrossLaunchIds
{
    char* m_XboxOneId; //0x0000
    char* m_Ps4NAId; //0x0008
    char* m_Ps4EUId; //0x0010
    char* m_Ps4JPId; //0x0018
    char* m_MasterTitleId; //0x0020
    char* m_MultiplayerId; //0x0028
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 03847
// TypeInfo:  0x0000000142BFF868
struct StartGameArgs
{
    char* m_GameArguments; //0x0000
    CrossLaunchIds m_Ids; //0x0008
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 03841
// TypeInfo:  0x0000000142BFF128
struct CoreSettings
{
    char* m_Host; //0x0000
    char* m_HostUser; //0x0008
    char* m_HostUserDomain; //0x0010
    char* m_InitSeed; //0x0018
    CoreLogLevel m_LogLevel; //0x0020
    CoreLogLevel m_DialogLevel; //0x0024
    unsigned __int32 m_DebugOutputMode; //0x0028
    __int32 m_RandomTimeSeed; //0x002C
    __int32 m_RandomTickSeed; //0x0030
    __int32 m_RandomSessionId; //0x0034
    char* m_GameConfigurationName; //0x0038
    HardwareProfile m_HardwareProfile; //0x0040
    float m_HardwareCpuBias; //0x0044
    float m_HardwareGpuBias; //0x0048
    char _0x004C[4];
    char* m_ProfileDirectoryName; //0x0050
    float m_ScatterJobYieldGranularity; //0x0058
    char _0x005C[4];
    char* m_AvailableLanguages; //0x0060
    bool m_LiveEditingEnable; //0x0068
    bool m_UserLogEnabled; //0x0069
    bool m_DisplayAsserts; //0x006A
    bool m_CrashOnFatalErrors; //0x006B
    bool m_UseStorageServer; //0x006C
    bool m_EnableLocalization; //0x006D
    bool m_EnableSpikePacketBuffering; //0x006E
    char _0x006F[1];
};//Size=0x0070

////////////////////////////////////////
// RuntimeId: 03833
// TypeInfo:  0x0000000142BFF148
struct PlatformScalableBool
{
    bool m_Default; //0x0000
    bool m_Xenon; //0x0001
    bool m_Ps3; //0x0002
    bool m_Gen4a; //0x0003
    bool m_Gen4b; //0x0004
};//Size=0x0005

////////////////////////////////////////
// RuntimeId: 03831
// TypeInfo:  0x0000000142BFF168
struct PlatformScalableFloat
{
    float m_Default; //0x0000
    float m_Xenon; //0x0004
    float m_Ps3; //0x0008
    float m_Gen4a; //0x000C
    float m_Gen4b; //0x0010
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 03829
// TypeInfo:  0x0000000142BFF188
struct PlatformScalableInt
{
    __int32 m_Default; //0x0000
    __int32 m_Xenon; //0x0004
    __int32 m_Ps3; //0x0008
    __int32 m_Gen4a; //0x000C
    __int32 m_Gen4b; //0x0010
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 03825
// TypeInfo:  0x0000000142BFF1A8
struct QualityScalableBool
{
    bool m_Low; //0x0000
    bool m_Medium; //0x0001
    bool m_High; //0x0002
    bool m_Ultra; //0x0003
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 03823
// TypeInfo:  0x0000000142BFF1C8
struct QualityScalableFloat
{
    float m_Low; //0x0000
    float m_Medium; //0x0004
    float m_High; //0x0008
    float m_Ultra; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 03821
// TypeInfo:  0x0000000142BFF1E8
struct QualityScalableInt
{
    __int32 m_Low; //0x0000
    __int32 m_Medium; //0x0004
    __int32 m_High; //0x0008
    __int32 m_Ultra; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 03811
// TypeInfo:  0x0000000142BFF208
struct DataField
{
    char* m_Value; //0x0000
    DataContainer* m_ValueRef; //0x0008
    __int32 m_Id; //0x0010
    FieldAccessType m_AccessType; //0x0014
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 03807
// TypeInfo:  0x0000000142BFF228
struct LinkConnection
{
    DataContainer* m_Source; //0x0000
    DataContainer* m_Target; //0x0008
    __int32 m_SourceFieldId; //0x0010
    __int32 m_TargetFieldId; //0x0014
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 03805
// TypeInfo:  0x0000000142BFF248
struct PropertyConnection
{
    DataContainer* m_Source; //0x0000
    DataContainer* m_Target; //0x0008
    __int32 m_SourceFieldId; //0x0010
    __int32 m_TargetFieldId; //0x0014
    bool m_ShouldNetworkValue; //0x0018
    char _0x0019[7];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 03785
// TypeInfo:  0x0000000142BFF268
struct InternetSimulationState
{
    float m_ReorderRatio; //0x0000
    float m_LatencyMin; //0x0004
    float m_LatencyMax; //0x0008
    float m_DuplicateRatio; //0x000C
    float m_DropRatio; //0x0010
    float m_CorruptRatio; //0x0014
    float m_SizeRatio; //0x0018
    float m_SpikeDurationMin; //0x001C
    float m_SpikeDurationMax; //0x0020
    float m_SpikeCooldownMin; //0x0024
    float m_SpikeCooldownMax; //0x0028
    float m_BandwidthMax; //0x002C
    float m_BandwidthDelayMax; //0x0030
    bool m_Enabled; //0x0034
    char _0x0035[3];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 03783
// TypeInfo:  0x0000000142BFEF48
struct StreamInstallGameInstalledMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 03782
// TypeInfo:  0x0000000142BFEF68
struct StreamInstallRequestResumeMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 03781
// TypeInfo:  0x0000000142BFEF88
struct StreamInstallRequestSuspendMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 03780
// TypeInfo:  0x0000000142BFEFA8
struct StreamInstallInstallDoneMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 03779
// TypeInfo:  0x0000000142BFEFC8
struct StreamInstallChunkInstalledMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 03778
// TypeInfo:  0x0000000142BFEFE8
struct StreamInstallInstallProgressMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 03777
// TypeInfo:  0x0000000142BFF008
struct StreamInstallInstallingMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 03776
// TypeInfo:  0x0000000142BFF028
struct CoreCleanupMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 03775
// TypeInfo:  0x0000000142BFF048
struct CoreSublevelStartStreamOutMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 03774
// TypeInfo:  0x0000000142BFF068
struct CorePanicMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 03773
// TypeInfo:  0x0000000142BFF088
struct CoreQuitMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 03772
// TypeInfo:  0x0000000142BFF0A8
struct CoreUpdateClipboardMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 03771
// TypeInfo:  0x0000000142BFF0C8
struct CoreMainThreadInitMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 03770
// TypeInfo:  0x0000000142BFF0E8
struct CoreHibernateMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 03769
// TypeInfo:  0x0000000142BFF108
struct CoreQuittingInitiatedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 03762
// TypeInfo:  0x0000000142BFDDE8
struct SplineCurve
{
    Vec4 m_XValues0; //0x0000
    Vec4 m_XValues1; //0x0010
    Vec4 m_XValues2; //0x0020
    Vec4 m_YValues0; //0x0030
    Vec4 m_YValues1; //0x0040
    Vec4 m_YValues2; //0x0050
    Vec4 m_YValues3; //0x0060
    Vec4 m_GValues0; //0x0070
    Vec4 m_GValues1; //0x0080
    Vec4 m_GValues2; //0x0090
    Vec4 m_GValues3; //0x00A0
    Vec4 m_GValues4; //0x00B0
    Vec4 m_GValues5; //0x00C0
    SplineType m_SplineType; //0x00D0
    char _0x00D4[12];
};//Size=0x00E0

////////////////////////////////////////
// RuntimeId: 03754
// TypeInfo:  0x0000000142BFDE28
struct AudioCurvePoint
{
    float m_X; //0x0000
    float m_Y; //0x0004
    float m_K; //0x0008
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 03758
// TypeInfo:  0x0000000142BFDE08
struct AudioCurve
{
    Array<AudioCurvePoint> m_Points; //0x0000
    AudioCurveType m_CurveType; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 08197
// TypeInfo:  0x0000000142C654A8
struct PartInfoData
{
    AxisAlignedBox m_Aabb; //0x0000
    Vec3 m_Translation; //0x0020
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 03710
// TypeInfo:  0x0000000142305870
struct Mat4
{
    float m_m11; //0x0000
    float m_m12; //0x0004
    float m_m13; //0x0008
    float m_m14; //0x000C
    float m_m21; //0x0010
    float m_m22; //0x0014
    float m_m23; //0x0018
    float m_m24; //0x001C
    float m_m31; //0x0020
    float m_m32; //0x0024
    float m_m33; //0x0028
    float m_m34; //0x002C
    float m_m41; //0x0030
    float m_m42; //0x0034
    float m_m43; //0x0038
    float m_m44; //0x003C
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 09763
// TypeInfo:  0x0000000142C89D28
struct WeaponStateData
{
    Array<unsigned __int32> m_ReferencedAssetHashes; //0x0000
    Array<BoneFakePhysicsData*> m_BoneFakePhysics; //0x0008
    ObjectBlueprint* m_WeaponMesh3p; //0x0010
    SkinnedMeshAsset* m_Mesh3p; //0x0018
    AntRef m_Weapon; //0x0020
    AntRef m_Weapon3p; //0x0034
    float m_ZoomMeshTransitionFactor; //0x0048
    float m_ZoomScaleFactor; //0x004C
    Array<float> m_ZoomInOutMeshTransitionFactors; //0x0050
    float m_KeepAimingTime; //0x0058
    AnimatedFireEnum m_AnimatedFireType; //0x005C
    AnimatedAimingEnum m_AnimatedAimingType; //0x0060
    float m_HideProjectileAfterFireTime; //0x0064
    char* m_ProjectileBoneName; //0x0068
    float m_UnDeploySpeed; //0x0070
    float m_DeploySpeed; //0x0074
    Array<LinearTransform> m_Mesh3pTransforms; //0x0078
    bool m_AlwaysAimHead; //0x0080
    bool m_IsOneHanded; //0x0081
    bool m_PlayDeployAfterFire; //0x0082
    bool m_SkipDeployAnimation; //0x0083
    bool m_SkipReloadAnimation; //0x0084
    bool m_SkipFireAnimation; //0x0085
    bool m_SendFireWhenPrefiring; //0x0086
    char _0x0087[1];
};//Size=0x0088

////////////////////////////////////////
// RuntimeId: 03706
// TypeInfo:  0x00000001423058B0
struct Plane
{
    float m_x; //0x0000
    float m_y; //0x0004
    float m_z; //0x0008
    float m_w; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 03704
// TypeInfo:  0x0000000142305830
struct Quat
{
    float m_x; //0x0000
    float m_y; //0x0004
    float m_z; //0x0008
    float m_w; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 10184
// TypeInfo:  0x0000000142C92760
struct AmbientWaveSettings
{
    SplineCurve m_WindDistribution; //0x0000
    float m_WaveAmplitude; //0x00E0
    float m_WindSpeed; //0x00E4
    float m_WindAngle; //0x00E8
    float m_MinWavelength; //0x00EC
    float m_LargeWaveReduction; //0x00F0
    float m_FoamHalfLife; //0x00F4
    float m_FoamThreshold; //0x00F8
    float m_FoamMaxValue; //0x00FC
};//Size=0x0100

////////////////////////////////////////
// RuntimeId: 09793
// TypeInfo:  0x0000000142C89CE8
struct BulletHitInfo
{
    Vec3 m_Direction; //0x0000
    Vec3 m_SpawnPosition; //0x0010
    Vec3 m_HitPosition; //0x0020
    UnlockAssetBase* m_WeaponUnlockAsset; //0x0030
    AntHitReactionWeaponType m_WeaponType; //0x0038
    __int32 m_BoneType; //0x003C
    __int32 m_ShooterPlayerId; //0x0040
    bool m_ShooterIsAIPlayer; //0x0044
    char _0x0045[11];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 09037
// TypeInfo:  0x0000000142C78FE8
struct LensFlareElement
{
    Vec4 m_SizeOccluderCurve; //0x0000
    Vec4 m_SizeScreenPosCurve; //0x0010
    Vec4 m_SizeAngleCurve; //0x0020
    Vec4 m_SizeCamDistCurve; //0x0030
    Vec4 m_AlphaOccluderCurve; //0x0040
    Vec4 m_AlphaScreenPosCurve; //0x0050
    Vec4 m_AlphaAngleCurve; //0x0060
    Vec4 m_AlphaCamDistCurve; //0x0070
    Vec4 m_RotationDistCurve; //0x0080
    SurfaceShaderBaseAsset* m_Shader; //0x0090
    float m_RayDistance; //0x0098
    Vec2 m_Size; //0x009C
    float m_SizeCamDistMax; //0x00A4
    float m_AlphaCamDistMax; //0x00A8
    float m_RotationLocal; //0x00AC
    float m_RotationDistMultiplier; //0x00B0
    bool m_RotationAlignedToRay; //0x00B4
    char _0x00B5[11];
};//Size=0x00C0

////////////////////////////////////////
// RuntimeId: 03690
// TypeInfo:  0x0000000142BFAF68
struct SuppressionReactionData
{
    float m_SuppressionHighThreshold; //0x0000
    float m_SuppressionLowThreshold; //0x0004
    float m_SuppressionUIThreshold; //0x0008
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 03542
// TypeInfo:  0x0000000142BFB068
struct ScoringBucketModifierData
{
    ScoringBucket m_Bucket; //0x0000
    float m_Modifier; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 03540
// TypeInfo:  0x0000000142BFB088
struct ScoringBucketUnlockData
{
    unsigned __int32 m_PointsNeeded; //0x0000
    char _0x0004[4];
    BasicUnlockInfo m_UnlockInfo; //0x0008
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 03530
// TypeInfo:  0x0000000142BFB0C8
struct MapRotationEntry
{
    char* m_ShortGameModeName; //0x0000
    char* m_FullGameModeName; //0x0008
    char* m_MapName; //0x0010
    char* m_FullMapName; //0x0018
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 03532
// TypeInfo:  0x0000000142BFB0A8
struct MapRotationConfig
{
    __int32 m_MapRotationId; //0x0000
    char _0x0004[4];
    Array<GamePlatform> m_Platforms; //0x0008
    char* m_NameSid; //0x0010
    char* m_DescSid; //0x0018
    char* m_Mod; //0x0020
    char* m_Key; //0x0028
    Array<MapRotationEntry> m_MapRotation; //0x0030
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 03528
// TypeInfo:  0x0000000142BFB0E8
struct SettingCategory
{
    unsigned __int32 m_CategoryId; //0x0000
    char _0x0004[4];
    char* m_NameSid; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 03522
// TypeInfo:  0x0000000142BFB148
struct SettingRange
{
    char* m_Key; //0x0000
    unsigned __int32 m_Min; //0x0008
    unsigned __int32 m_Max; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 03524
// TypeInfo:  0x0000000142BFB128
struct RSPPlatformSettingRange
{
    Array<GamePlatform> m_Platforms; //0x0000
    Array<SettingRange> m_Ranges; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 03526
// TypeInfo:  0x0000000142BFB108
struct SettingsRangeConfig
{
    char* m_Key; //0x0000
    Array<SettingRange> m_Ranges; //0x0008
    Array<RSPPlatformSettingRange> m_PlatformRanges; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 03516
// TypeInfo:  0x0000000142BFB1A8
struct LockedSettingConfig
{
    char* m_Key; //0x0000
    unsigned __int32 m_Value; //0x0008
    bool m_ValidateSetting; //0x000C
    char _0x000D[3];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 03518
// TypeInfo:  0x0000000142BFB188
struct RSPPlatformSetting
{
    Array<GamePlatform> m_Platforms; //0x0000
    Array<LockedSettingConfig> m_LockedSettings; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 03520
// TypeInfo:  0x0000000142BFB168
struct PresetTypeConfig
{
    __int32 m_PresetId; //0x0000
    char _0x0004[4];
    char* m_Key; //0x0008
    char* m_NameSid; //0x0010
    char* m_DescSid; //0x0018
    char* m_ServerType; //0x0020
    Array<LockedSettingConfig> m_LockedSettings; //0x0028
    Array<RSPPlatformSetting> m_PlatformSettings; //0x0030
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 03514
// TypeInfo:  0x0000000142BFB1C8
struct SettingConfig
{
    char* m_Key; //0x0000
    char* m_NameSid; //0x0008
    char* m_DescSid; //0x0010
    unsigned __int32 m_CategoryId; //0x0018
    char _0x001C[4];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 03498
// TypeInfo:  0x0000000142BFB1E8
struct StatSpamSetting
{
    StatEvent m_Event; //0x0000
    unsigned __int32 m_AmountLimit; //0x0004
    float m_TimeLimit; //0x0008
    bool m_PlayerToPlayer; //0x000C
    char _0x000D[3];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 03492
// TypeInfo:  0x0000000142BFB048
struct SkillLevelComponent
{
    float m_MaxValue; //0x0000
    float m_Weight; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 03466
// TypeInfo:  0x0000000142BF11B8
struct SquadIcon
{
    char* m_Black; //0x0000
    char* m_White; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 03430
// TypeInfo:  0x0000000142BF11D8
struct UIPerformanceMetricSettings
{
    UIPerformanceMetric m_MetricType; //0x0000
    float m_HighThreshold; //0x0004
    float m_LowThreshold; //0x0008
    char _0x000C[4];
    char* m_GoodSid; //0x0010
    char* m_MediumSid; //0x0018
    char* m_BadSid; //0x0020
    char* m_GoodHeaderSid; //0x0028
    char* m_MediumHeaderSid; //0x0030
    char* m_BadHeaderSid; //0x0038
    bool m_LessIsBetter; //0x0040
    char _0x0041[7];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 03408
// TypeInfo:  0x0000000142BF0CB8
struct UICustomizationDisplay
{
    Array<UICustomizationCategoryOrdering*> m_CategoryOrdering; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 03366
// TypeInfo:  0x0000000142BF11F8
struct TitanScoringInfoState
{
    char* m_ShipBarOutlineTexture; //0x0000
    char* m_ShipBarFillTexture; //0x0008
    float m_PulseInterval; //0x0010
    char _0x0014[4];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 03356
// TypeInfo:  0x0000000142BF1218
struct OverrideVideoData
{
    MovieTextureAsset* m_Video; //0x0000
    GamePlatform m_Platform; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 03326
// TypeInfo:  0x0000000142BF1238
struct CellTemplateData
{
    char* m_MappingID; //0x0000
    UIWidgetBlueprint* m_WidgetTemplate; //0x0008
    UIServerFilterType m_FilterMapping; //0x0010
    char _0x0014[4];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 03298
// TypeInfo:  0x0000000142BF1278
struct UIInputConceptFilterData
{
    InputConceptIdentifiers m_First; //0x0000
    InputConceptIdentifiers m_Last; //0x0004
    bool m_IncludeThisRange; //0x0008
    char _0x0009[3];
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 03300
// TypeInfo:  0x0000000142BF1258
struct UIOptionKeyBindingsType
{
    char* m_ActivatorEventName; //0x0000
    char* m_ID; //0x0008
    Array<UIInputConceptFilterData> m_Filters; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 03296
// TypeInfo:  0x0000000142BF1298
struct UIOptionTableColumn
{
    float m_widthMin; //0x0000
    float m_expandWeight; //0x0004
    bool m_rightSpacing; //0x0008
    char _0x0009[3];
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 03288
// TypeInfo:  0x0000000142BF0CD8
struct UIOptionsConsoleControlsTextFieldMappings
{
    Array<UIOptionControlTextFieldMapping_NonChanging*> m_NonChanging; //0x0000
    Array<UIOptionControlTextFieldMapping_UnaryChanging*> m_UnaryChanging; //0x0008
    Array<UIOptionControlTextFieldMapping_BothChanging*> m_BothChanging; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 03276
// TypeInfo:  0x0000000142BF12B8
struct UIOptionsOption
{
    ProfileOptionData* m_OptionData; //0x0000
    char* m_PlayerShownName; //0x0008
    char* m_Description; //0x0010
    UIElementInclusionSettings m_Inclusion; //0x0018
    char* m_VisibilityPropertyName; //0x0030
    UIOptionsSpecialCase m_SpecialCase; //0x0038
    char _0x003C[4];
    char* m_OnLabel; //0x0040
    char* m_OffLabel; //0x0048
    char* m_EventName; //0x0050
    char* m_SelectedEventName; //0x0058
    UIOptionsSliderDisplay m_SliderDisplayMode; //0x0060
    char _0x0064[4];
    CustomOptionData* m_CustomData; //0x0068
    bool m_IsButton; //0x0070
    bool m_ApplyOnChange; //0x0071
    char _0x0072[6];
};//Size=0x0078

////////////////////////////////////////
// RuntimeId: 03270
// TypeInfo:  0x0000000142BF12F8
struct SquadDeployGameModeData
{
    SquadDeployGameMode m_GameMode; //0x0000
    char _0x0004[4];
    char* m_Experience; //0x0008
    __int32 m_CommanderCount; //0x0010
    bool m_Disabled; //0x0014
    bool m_IsGen4Only; //0x0015
    char _0x0016[2];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 03272
// TypeInfo:  0x0000000142BF12D8
struct SquadDeployMapPackData
{
    SquadDeployMapPack m_MapPack; //0x0000
    BFModMask m_MapPackModIndex; //0x0004
    char* m_MapPackDisplayName; //0x0008
    char* m_License; //0x0010
    Array<SquadDeployGameModeData> m_SupportedGameModes; //0x0018
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 03254
// TypeInfo:  0x0000000142BF1318
struct UITabBarButton
{
    char* m_Sid; //0x0000
    char* m_OnSelectedOutputEvent; //0x0008
    char* m_VisibleProperty; //0x0010
    char* m_SelectableProperty; //0x0018
    char* m_NewContentProperty; //0x0020
    bool m_ButtonSelectable; //0x0028
    char _0x0029[7];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 03248
// TypeInfo:  0x0000000142BF0D18
struct UIWorldIconZoneFloats
{
    float m_Interaction; //0x0000
    float m_Critical; //0x0004
    float m_Close; //0x0008
    float m_Far; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 03250
// TypeInfo:  0x0000000142BF0CF8
struct UIWorldIconFadeConfiguration
{
    UIWorldIconZoneFloats m_ZoneFadeValues; //0x0000
    UIWorldIconZoneFloats m_ZoneLookAtFadeValues; //0x0010
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 03228
// TypeInfo:  0x0000000142BF0D38
struct BFUIColorizationData
{
    BFUIColorizationMode m_ColorizationMode; //0x0000
    UIIconState m_IconState; //0x0004
    UIIconState m_SpectatorIconState; //0x0008
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 03216
// TypeInfo:  0x0000000142BF0D58
struct UIAirRadarConfiguration
{
    float m_RadarSensitivity; //0x0000
    float m_HeatSensitivity; //0x0004
    bool m_TrackLaserPaintedObjects; //0x0008
    char _0x0009[3];
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 03214
// TypeInfo:  0x0000000142BF0D78
struct UIChatConfiguration
{
    UICppScreenData* m_ChatScreenData; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 03212
// TypeInfo:  0x0000000142BF0D98
struct UIMinimapIconConfiguration
{
    UIElementColor m_BgCol; //0x0000
    UIElementFontStyle* m_SmallFont; //0x0020
    char _0x0028[8];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 03210
// TypeInfo:  0x0000000142BF0DB8
struct UIDeployConfiguration
{
    UIElementFontStyle* m_SpawnLabelFont; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 03200
// TypeInfo:  0x0000000142BF0E38
struct UILockingConfiguration
{
    UIElementColor m_TextColor; //0x0000
    UIElementFontStyle* m_Font; //0x0020
    float m_TextScale; //0x0028
    float m_TextOffset; //0x002C
    float m_DistanceScale; //0x0030
    float m_DistanceMaxScaleMod; //0x0034
    char* m_DefaultTargetIcon; //0x0038
    char* m_DefaultChevronIcon; //0x0040
    __int32 m_LockingQuadSpacingStart; //0x0048
    __int32 m_LockedQuadSpacing; //0x004C
    __int32 m_LockTextOffset; //0x0050
    char _0x0054[4];
    char* m_LockedText; //0x0058
    char* m_LockedGuidingText; //0x0060
    char* m_NotLockedGuidingText; //0x0068
    __int32 m_DistanceTextOffset; //0x0070
    float m_LockingBlinkRate; //0x0074
    __int32 m_PulseTargetChevronSpacing; //0x0078
    float m_PulseDuration; //0x007C
    float m_UnavalibleAlpha; //0x0080
    char _0x0084[12];
};//Size=0x0090

////////////////////////////////////////
// RuntimeId: 03206
// TypeInfo:  0x0000000142BF0DF8
struct UIWorldZoneDistanceConfiguration
{
    float m_CriticalDistance; //0x0000
    float m_CloseDistance; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 03202
// TypeInfo:  0x0000000142BF0E18
struct UIShieldIconConfiguration
{
    float m_BlinkRate; //0x0000
    char _0x0004[4];
    char* m_PortableGadgetIcon; //0x0008
    char* m_PortableShieldIcon; //0x0010
    char* m_VehicleShieldIcon; //0x0018
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 03198
// TypeInfo:  0x0000000142BF0E58
struct UIColorConfiguration
{
    UIElementColor m_Neutral; //0x0000
    UIElementColor m_Team; //0x0020
    UIElementColor m_Squad; //0x0040
    UIElementColor m_Enemy; //0x0060
    UIElementColor m_Team1; //0x0080
    UIElementColor m_Team2; //0x00A0
    UIElementColor m_Team3; //0x00C0
    UIElementColor m_Team4; //0x00E0
    UIElementColor m_Commander; //0x0100
    UIElementColor m_Spectator; //0x0120
};//Size=0x0140

////////////////////////////////////////
// RuntimeId: 03188
// TypeInfo:  0x0000000142BF0ED8
struct UIGrenadeConfiguration
{
    float m_IconVerticalOffset; //0x0000
    float m_FadeStartDist; //0x0004
    float m_DisappearDist; //0x0008
    float m_WarningFadeStartDist; //0x000C
    float m_WarningDisappearDist; //0x0010
    float m_MaxScaleAtDist; //0x0014
    float m_MinScaleAtDist; //0x0018
    float m_MaxScale; //0x001C
    float m_MinScale; //0x0020
    float m_MagneticEdgeOffsetMultiplierNear; //0x0024
    float m_MagneticEdgeOffsetMultiplierFar; //0x0028
    float m_MagneticEdgeNearDist; //0x002C
    float m_MagneticEdgeFarDist; //0x0030
};//Size=0x0034

////////////////////////////////////////
// RuntimeId: 03190
// TypeInfo:  0x0000000142BF0EB8
struct UINametagVisibilityValues
{
    bool m_VisibleClose; //0x0000
    bool m_VisibleFar; //0x0001
};//Size=0x0002

////////////////////////////////////////
// RuntimeId: 03192
// TypeInfo:  0x0000000142BF0E98
struct UINametagVisibilityConfiguration
{
    UINametagVisibilityValues m_LookAtVisibility; //0x0000
    UINametagVisibilityValues m_Visibility; //0x0002
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 03194
// TypeInfo:  0x0000000142BF0E78
struct UINametagBehavior
{
    float m_CloseDistance; //0x0000
    bool m_MagneticToScreen; //0x0004
    UINametagVisibilityConfiguration m_IconVisibility; //0x0005
    UINametagVisibilityConfiguration m_NameVisibility; //0x0009
    UINametagVisibilityConfiguration m_HealthVisibility; //0x000D
    UINametagVisibilityConfiguration m_DistanceVisibility; //0x0011
    char _0x0015[3];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 03208
// TypeInfo:  0x0000000142BF0DD8
struct UIWorldIconConfiguration
{
    UILockingConfiguration m_LockingCfg; //0x0000
    UIColorConfiguration m_StdColors; //0x0090
    UINametagConfiguration* m_NametagCfg; //0x01D0
    UINametagConfiguration* m_SPNametagCfg; //0x01D8
    UINametagConfiguration* m_HardcoreNametagCfg; //0x01E0
    UIShieldIconConfiguration m_ShieldIconCfg; //0x01E8
    Array<UIColorConfiguration> m_ColorBlindModes; //0x0208
    UIWorldZoneDistanceConfiguration m_ZoneDistanceCfg; //0x0210
    float m_RequestIconBlinkRate; //0x0218
    UIGrenadeConfiguration m_GrenadeCfg; //0x021C
    UIElementFontStyle* m_MapFont; //0x0250
    UIElementFontStyle* m_HudFont; //0x0258
};//Size=0x0260

////////////////////////////////////////
// RuntimeId: 03160
// TypeInfo:  0x0000000142BF0EF8
struct UIGridStepperConfig
{
    UIElementScrollbarStyle* m_ScrollBarStyle; //0x0000
    unsigned __int32 m_ScrollbarWidth; //0x0008
    unsigned __int32 m_ScrollbarMargin; //0x000C
    __int32 m_ScrollbarTopOffset; //0x0010
    __int32 m_ScrollbarBottomOffset; //0x0014
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 03100
// TypeInfo:  0x0000000142BF1338
struct UIHitIndicatorIcon
{
    float m_BaseShowTime; //0x0000
    float m_MaxShowTime; //0x0004
    float m_DamageToSecondsFactor; //0x0008
    float m_TimeBeforeFadeOut; //0x000C
    float m_StartOffset; //0x0010
    float m_EndOffset; //0x0014
    float m_OffsetDuration; //0x0018
    UIHitIndicatorIconType m_IconType; //0x001C
    char* m_TextureId; //0x0020
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 03056
// TypeInfo:  0x0000000142BF1358
struct UIBattledashHorisontalListData
{
    unsigned __int32 m_Index; //0x0000
    char _0x0004[4];
    char* m_Name; //0x0008
    char* m_TextureId; //0x0010
    char* m_IdString; //0x0018
    char* m_Description; //0x0020
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 03052
// TypeInfo:  0x0000000142BF1378
struct UIAutoListCategoryData
{
    UIAutoListType m_listtype; //0x0000
    char _0x0004[4];
    char* m_idString; //0x0008
    char* m_displayTitle; //0x0010
    char* m_EmptyListDescription; //0x0018
    bool m_showEmptyCategory; //0x0020
    char _0x0021[7];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 03044
// TypeInfo:  0x0000000142BF1398
struct UIBattledashDetailPane
{
    char* m_title; //0x0000
    char* m_value; //0x0008
    char* m_textureId; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 03036
// TypeInfo:  0x0000000142BF13B8
struct UIBattledashIndicatorData
{
    char* m_Name; //0x0000
    char* m_TextureId; //0x0008
    float m_IndicatorFadeTime; //0x0010
    float m_OverrideIconSize; //0x0014
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 03008
// TypeInfo:  0x0000000142BF13D8
struct UINPXTooltipWidgetLayout
{
    UILayoutMode m_LayoutMode; //0x0000
    UIElementOffset m_Offset; //0x0004
    UIElementAnchor m_Anchor; //0x000C
    UIElementOffset m_Position; //0x0014
    UIElementRectExpansion m_Expansion; //0x001C
};//Size=0x002C

////////////////////////////////////////
// RuntimeId: 02988
// TypeInfo:  0x0000000142BF0F18
struct UIScoreboardColumnStyle
{
    float m_Width; //0x0000
    UIElementAlignment m_TextAlignment; //0x0004
    float m_TextOffset; //0x0008
    bool m_HasLine; //0x000C
    char _0x000D[3];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 02976
// TypeInfo:  0x0000000142BF13F8
struct UICommoRoseButtonInfo
{
    char* m_ButtonId; //0x0000
    char* m_ButtonText; //0x0008
    char* m_ChatText; //0x0010
    ChatChannelType m_ChatChannel; //0x0018
    char _0x001C[4];
    UIElementAction* m_Action; //0x0020
    char* m_IconTextureId; //0x0028
    UICommoRoseTextAlignment m_TextAlignment; //0x0030
    UIElementRect m_InactiveRect; //0x0034
    UIElementRect m_ActiveRect; //0x0044
    char _0x0054[4];
    UIElementFontStyle* m_InactiveFontStyle; //0x0058
    UIElementFontStyle* m_ActiveFontStyle; //0x0060
    bool m_UseLocalRect; //0x0068
    bool m_IsSelectable; //0x0069
    char _0x006A[6];
};//Size=0x0070

////////////////////////////////////////
// RuntimeId: 02930
// TypeInfo:  0x0000000142BF0F38
struct UIDeactivateCameraMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 02929
// TypeInfo:  0x0000000142BF0F58
struct UIDeployScreenUpdateCameraMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 02928
// TypeInfo:  0x0000000142BF0F78
struct UIMapWidgetHelperData
{
    Vec4 m_MapFocusMargins; //0x0000
    UIVisualWidgetType m_VisualWidgetType; //0x0010
    char _0x0014[12];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 02888
// TypeInfo:  0x0000000142BF0F98
struct BFServerConfigurationData
{
    unsigned __int32 m_ScoreMultiplier; //0x0000
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 02890
// TypeInfo:  0x0000000142BF1418
struct BFServerConfigurationSchedule
{
    Array<char*> m_Licenses; //0x0000
    Array<char*> m_Levels; //0x0008
    BFServerConfigurationData m_Data; //0x0010
    char _0x0014[4];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 02858
// TypeInfo:  0x0000000142BF1438
struct GameConfigurationContentMapping
{
    GamePlatform m_Platform; //0x0000
    char _0x0004[4];
    char* m_License; //0x0008
    Array<char*> m_Content; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 02834
// TypeInfo:  0x0000000142BF0FD8
struct PeerCreateGameParameters
{
    GameParametersData* m_Base; //0x0000
    unsigned __int32 m_PlayerCapacity; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 02830
// TypeInfo:  0x0000000142BF1458
struct NucleusPlatformConfiguration
{
    GamePlatform m_Platform; //0x0000
    char _0x0004[4];
    char* m_ClientId; //0x0008
    char* m_ClientSecret; //0x0010
    char* m_LoginScope; //0x0018
    char* m_ClientRedirectUrl; //0x0020
    char* m_DisplayType; //0x0028
    char* m_BlazeServerClientId; //0x0030
    char* m_BlazeServerRedirectUrl; //0x0038
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 02828
// TypeInfo:  0x0000000142BF1478
struct NucleusEnvironmentConfiguration
{
    NucleusEnvironment m_Env; //0x0000
    char _0x0004[4];
    char* m_BaseUrl; //0x0008
    char* m_NucleusClientId; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 02816
// TypeInfo:  0x0000000142BF1498
struct BFModMaskMapping
{
    char* m_ModName; //0x0000
    char* m_License; //0x0008
    BFModMask m_MaskBit; //0x0010
    char _0x0014[4];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 02770
// TypeInfo:  0x0000000142BF14B8
struct PCAchievementSettings
{
    char _0x0000[1];
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 02768
// TypeInfo:  0x0000000142BF0FF8
struct DurangoAchievementSettings
{
    unsigned __int32 m_CompletionValue; //0x0000
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 02762
// TypeInfo:  0x0000000142BF14D8
struct AwardStarNameInstance
{
    char* m_Name; //0x0000
    StatsCategoryBaseData* m_Category; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 02754
// TypeInfo:  0x0000000142BF14F8
struct EventScaleData
{
    StatEvent m_Event; //0x0000
    float m_Scale; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 02752
// TypeInfo:  0x0000000142BF1518
struct CriteriaAward
{
    AwardData* m_Award; //0x0000
    unsigned __int32 m_Count; //0x0008
    char _0x000C[4];
    char* m_Sid; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 02746
// TypeInfo:  0x0000000142BF1538
struct CriteriaStarCategoryInstance
{
    char* m_Name; //0x0000
    StatsCategoryBaseData* m_ParamX; //0x0008
    StatsCategoryBaseData* m_ParamY; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 02728
// TypeInfo:  0x0000000142BF1558
struct AIProximityReactionsBinding
{
    AntRef m_AIReaction; //0x0000
    AntRef m_AIExplosionReaction; //0x0014
    AntRef m_AIExplosionForce; //0x0028
    AntRef m_AIStunReaction; //0x003C
    AntRef m_AIStunReactionRandomFactor; //0x0050
    AntRef m_AIReactionDirection; //0x0064
    AntRef m_IsPanicking; //0x0078
    AntRef m_FireNearby; //0x008C
    AntRef m_AimUpAndDown; //0x00A0
    AntRef m_FocusAimScale; //0x00B4
    AntRef m_FocusAiming; //0x00C8
    AntRef m_EnableProceduralHeadAim; //0x00DC
    AntRef m_KillFromAnimation; //0x00F0
    AntRef m_PlayerAnimationKill; //0x0104
    AntRef m_PlayerInitiatedRagdoll; //0x0118
    AntRef m_AISuppressed; //0x012C
    AntRef m_HumanAwareness; //0x0140
    AntRef m_HumanIsClose; //0x0154
    AntRef m_HumanTargetYaw; //0x0168
    AntRef m_LookAtHuman; //0x017C
    AntRef m_RetriggerIdle; //0x0190
    AntRef m_HasWantedPrecision; //0x01A4
    AntRef m_TargetVisible; //0x01B8
    AntRef m_TargetSpotted; //0x01CC
    AntRef m_TargetDistance; //0x01E0
    AntRef m_CruiseSpeedLevel; //0x01F4
    AntRef m_DrasticAimYawChange; //0x0208
};//Size=0x021C

////////////////////////////////////////
// RuntimeId: 02710
// TypeInfo:  0x0000000142BF1098
struct ScenarioTaskData
{
    Vec3 m_StartPoint; //0x0000
    Vec3 m_EndPointWorldOffset; //0x0010
    PlayAnimationData* m_PlayAnimation; //0x0020
    float m_WorldAngle; //0x0028
    __int32 m_ScenarioId; //0x002C
    __int32 m_ActorId; //0x0030
    __int32 m_PartId; //0x0034
    __int32 m_LevelId; //0x0038
    char _0x003C[4];
    Array<LinearTransform> m_ConnectTransforms; //0x0040
    float m_StartTurnDistance; //0x0048
    float m_StartTimerDistance; //0x004C
    float m_TriggerScenarioDelay; //0x0050
    bool m_TriggerScenario; //0x0054
    bool m_UseClientPosition; //0x0055
    bool m_ForceWaitForAnimation; //0x0056
    bool m_OvershootWaypoint; //0x0057
    char _0x0058[8];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 02708
// TypeInfo:  0x0000000142BF10B8
struct AILocoCoverTaskData
{
    Vec3 m_WantedPos; //0x0000
    Vec3 m_ThreatPosition; //0x0010
    AntCoverEnum m_CoverType; //0x0020
    AntPoseEnum m_CoverPose; //0x0024
    float m_WaitTime; //0x0028
    float m_WorldAngle; //0x002C
    CoverPeekType m_PeekOut; //0x0030
    CoverIdleType m_IdleBehavior; //0x0034
    AntPoseEnum m_ExitPose; //0x0038
    float m_OffsetLength; //0x003C
    CoverEnterStrategy m_EnterStrategy; //0x0040
    float m_ExitAngle; //0x0044
    float m_DistanceToNextWaypoint; //0x0048
    float m_AlignOffsetAlong; //0x004C
    float m_AlignOffsetPerpendicular; //0x0050
    CoverExitStyle m_ExitStyle; //0x0054
    CoverFireType m_PrepareFireType; //0x0058
    bool m_ForceExitCover; //0x005C
    bool m_UseClientPosition; //0x005D
    char _0x005E[2];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 02694
// TypeInfo:  0x0000000142BF10F8
struct AILocoBaseTaskData
{
    AntPoseEnum m_PoseChangeMovingTowards; //0x0000
    AntAttentionStateEnum m_AttentionChangeMovingTowards; //0x0004
    AntSpeedLevel m_SpeedChangeMovingTowards; //0x0008
    AntPoseEnum m_PoseChange; //0x000C
    AntAttentionStateEnum m_AttentionStateChange; //0x0010
    AntSpeedLevel m_SpeedLevelChange; //0x0014
    float m_Radius; //0x0018
    bool m_IsScripted; //0x001C
    char _0x001D[3];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 02692
// TypeInfo:  0x0000000142BF1118
struct AILocoMoveTaskData
{
    Vec3 m_WantedPos; //0x0000
    float m_WaitTime; //0x0010
    float m_WorldAngle; //0x0014
    AntPoseEnum m_EnterPose; //0x0018
    AntPoseEnum m_ExitPose; //0x001C
    float m_AlmostFinishedDistance; //0x0020
    bool m_OverrideAngle; //0x0024
    bool m_UseClientPosition; //0x0025
    bool m_OnlySignalAlmostFinishedWhenMovingTowardsWaypoint; //0x0026
    char _0x0027[9];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 02690
// TypeInfo:  0x0000000142BF1138
struct AILocoVaultTaskData
{
    Vec3 m_StartPoint; //0x0000
    float m_DistanceBeforeVault; //0x0010
    float m_HeightBeforeVault; //0x0014
    float m_LengthOfVaultableObject; //0x0018
    float m_HeightAfterVault; //0x001C
    float m_DistanceAfterVault; //0x0020
    float m_WorldAngle; //0x0024
    WaypointVaultType m_VaultType; //0x0028
    bool m_UseClientPosition; //0x002C
    char _0x002D[3];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 02680
// TypeInfo:  0x0000000142BF1158
struct AILocoCoverBinding
{
    AntRef m_PrepareFire; //0x0000
    AntRef m_PrepareChangedToTrue; //0x0014
    AntRef m_ThrowGrenade; //0x0028
    AntRef m_PeekOut; //0x003C
    AntRef m_IdleBehindCover; //0x0050
    AntRef m_PeekType; //0x0064
    AntRef m_IdleTypePrevious; //0x0078
    AntRef m_IdleType; //0x008C
    AntRef m_IdleTypeChanged; //0x00A0
    AntRef m_CoverTypeEnum; //0x00B4
    AntRef m_CoverFireStyle; //0x00C8
    AntRef m_EnterCover; //0x00DC
    AntRef m_ExitCover; //0x00F0
    AntRef m_CoverDistanceSmall; //0x0104
    AntRef m_CoverDistanceMedium; //0x0118
    AntRef m_DistanceScale; //0x012C
    AntRef m_AbsoluteDistance; //0x0140
    AntRef m_AngleToNormal; //0x0154
    AntRef m_DirectionToCoverVsAITrajectory; //0x0168
    AntRef m_CoverNormalVsAITrajectory; //0x017C
    AntRef m_OutAngle; //0x0190
    AntRef m_ExitCoverDistance; //0x01A4
    AntRef m_ThreatAngle; //0x01B8
    AntRef m_StopExitCoverOutAround; //0x01CC
    AntRef m_EnterStrategy; //0x01E0
    AntRef m_ExitStyle; //0x01F4
};//Size=0x0208

////////////////////////////////////////
// RuntimeId: 02678
// TypeInfo:  0x0000000142BF1178
struct AILocoVaultBinding
{
    AntRef m_Vault; //0x0000
    AntRef m_DistanceBeforeVault; //0x0014
    AntRef m_HeightBeforeVault; //0x0028
    AntRef m_LengthOfVaultableObject; //0x003C
    AntRef m_HeightAfterVault; //0x0050
    AntRef m_DistanceAfterVault; //0x0064
    AntRef m_VaultType; //0x0078
};//Size=0x008C

////////////////////////////////////////
// RuntimeId: 02676
// TypeInfo:  0x0000000142BF1198
struct AILocoBinding
{
    AntRef m_LocoTarget; //0x0000
    AntRef m_AttentionState; //0x0014
    AntRef m_EndPoseEnum; //0x0028
    AntRef m_CurrentPoseEnum; //0x003C
    AntRef m_EnterExitPoseEnum; //0x0050
    AntRef m_RunStyle; //0x0064
    AntRef m_TargetWaypointPosition; //0x0078
    AntRef m_TargetWaypointRadius; //0x008C
    AntRef m_TargetSpeedLevel; //0x00A0
    AntRef m_CruiseSpeedLevel; //0x00B4
    AntRef m_EnableProceduralHeadAim; //0x00C8
};//Size=0x00DC

////////////////////////////////////////
// RuntimeId: 02670
// TypeInfo:  0x0000000142BF1578
struct AdvertisementZoneMember
{
    char* m_Identifier; //0x0000
    char* m_Filename; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 02666
// TypeInfo:  0x0000000142BE28C8
struct XdpChallengeData
{
    char _0x0000[1];
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 02664
// TypeInfo:  0x0000000142BE28E8
struct XdpHeroStatData
{
    char _0x0000[1];
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 02662
// TypeInfo:  0x0000000142BE2908
struct XdpStatCodeData
{
    char* m_XdpCode; //0x0000
    char* m_StorageCode; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 02658
// TypeInfo:  0x0000000142BE2928
struct XdpStatExport
{
    char _0x0000[1];
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 02656
// TypeInfo:  0x0000000142BE2948
struct WebExportLoadingImages
{
    char _0x0000[1];
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 02654
// TypeInfo:  0x0000000142BE2968
struct WebExportLevelLoadingImage
{
    LevelData* m_Level; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 02628
// TypeInfo:  0x0000000142BE2988
struct SurveyButton
{
    char* m_Sid; //0x0000
    UIInputAction m_Action; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 02624
// TypeInfo:  0x0000000142BE29A8
struct QuickMatchFilterInfo
{
    char* m_ID; //0x0000
    char* m_Title; //0x0008
    char* m_Description; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 02542
// TypeInfo:  0x0000000142BE29E8
struct IngameMenuElementData
{
    char* m_Sid; //0x0000
    char* m_Identifier; //0x0008
    char* m_EnabledFunc; //0x0010
    char* m_VisibleFunc; //0x0018
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 02546
// TypeInfo:  0x0000000142BE29C8
struct IngameMenuCategory
{
    IngameMenuCategoryType m_CategoryType; //0x0000
    char _0x0004[4];
    Array<IngameMenuElementData> m_Elements; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 02534
// TypeInfo:  0x0000000142BE2A48
struct UISettingsItem
{
    char* m_NameSid; //0x0000
    UISettingSymbols m_StartEndSymbols; //0x0008
    char _0x000C[4];
    ProfileOptionData* m_Setting; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 02536
// TypeInfo:  0x0000000142BE2A28
struct UISettingsGroup
{
    char* m_NameSid; //0x0000
    __int32 m_ScreenColumn; //0x0008
    char _0x000C[4];
    Array<UISettingsItem> m_SettingsItems; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 02538
// TypeInfo:  0x0000000142BE2A08
struct UISettingsPage
{
    char* m_NameSid; //0x0000
    Array<UISettingsGroup> m_SettingsGroups; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 02524
// TypeInfo:  0x0000000142BE2A68
struct ScreenshotInfo
{
    char* m_Name; //0x0000
    unsigned __int32 m_CropImageX1; //0x0008
    unsigned __int32 m_CropImageX2; //0x000C
    unsigned __int32 m_CropImageY1; //0x0010
    unsigned __int32 m_CropImageY2; //0x0014
    unsigned __int32 m_ResizeOutputImageWidth; //0x0018
    unsigned __int32 m_ResizeOutputImageHeight; //0x001C
    bool m_CropImage; //0x0020
    bool m_ResizeOutputImage; //0x0021
    char _0x0022[6];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 02502
// TypeInfo:  0x0000000142BE2A88
struct UILeaderboardData
{
    char* m_Name; //0x0000
    char* m_Description; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 02488
// TypeInfo:  0x0000000142BE2AA8
struct ServerInfoSetting
{
    char* m_Key; //0x0000
    char* m_Name; //0x0008
    bool m_IsBool; //0x0010
    char _0x0011[7];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 02482
// TypeInfo:  0x0000000142BE2AC8
struct SlotRange
{
    __int32 m_Min; //0x0000
    __int32 m_Max; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 02466
// TypeInfo:  0x0000000142BE2AE8
struct DataSource
{
    UIComponentData* m_Category; //0x0000
    __int32 m_Value; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 02448
// TypeInfo:  0x0000000142BE2B08
struct UIStreamInstallGroup
{
    char* m_MainMenuSid; //0x0000
    char* m_GroupId; //0x0008
    bool m_DisableIfIncomplete; //0x0010
    char _0x0011[7];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 02408
// TypeInfo:  0x0000000142BE2708
struct SoldierToComponentsReviveMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 02405
// TypeInfo:  0x0000000142BE2728
struct RotateToHitData
{
    float m_RotationSpeedFactor; //0x0000
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 02393
// TypeInfo:  0x0000000142BE2B28
struct MinimapData
{
    Vec3 m_OverlayColor; //0x0000
    Vec3 m_CameraPosition; //0x0010
    Vec2 m_Position; //0x0020
    float m_AnchorPosHorizontal; //0x0028
    float m_AnchorPosVertical; //0x002C
    Vec2 m_Size; //0x0030
    float m_CameraFov; //0x0038
    float m_CameraDistance; //0x003C
    float m_CameraLookDistance; //0x0040
    float m_OverlayAlpha; //0x0044
    float m_CameraRotation; //0x0048
    __int32 m_InnerZoomFactor; //0x004C
    __int32 m_MaxZoomLevels; //0x0050
    __int32 m_StartZoomLevel; //0x0054
    bool m_RotationFromPlayer; //0x0058
    bool m_PositionFromPlayer; //0x0059
    bool m_CenterOnCombatArea; //0x005A
    char _0x005B[5];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 02371
// TypeInfo:  0x0000000142BE2748
struct ClientRoundOverMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 02362
// TypeInfo:  0x0000000142BE2B48
struct Minimap2DTile
{
    TextureAsset* m_Texture; //0x0000
    __int32 m_ChildIndex0; //0x0008
    __int32 m_ChildIndex1; //0x000C
    __int32 m_ChildIndex2; //0x0010
    __int32 m_ChildIndex3; //0x0014
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 02350
// TypeInfo:  0x0000000142BE2B68
struct AreaValueTeam
{
    __int32 m_AreaValue; //0x0000
    TeamId m_Team; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 02346
// TypeInfo:  0x0000000142BE2B88
struct TicketCountPercentage
{
    __int32 m_MaxPercentOfTicketsLeft; //0x0000
    __int32 m_TicketLossPerMin; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 02296
// TypeInfo:  0x0000000142BE2BA8
struct SimpleAnimationControlBinding
{
    AntRef m_LocoTarget; //0x0000
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 02294
// TypeInfo:  0x0000000142BE2BC8
struct SupportedShootingBinding
{
    AntRef m_Supported; //0x0000
    AntRef m_SupportedPos; //0x0014
    AntRef m_ForwardVector; //0x0028
    AntRef m_Yaw; //0x003C
    AntRef m_Pitch; //0x0050
    AntRef m_DistToObject; //0x0064
    AntRef m_HeightOfObject; //0x0078
    AntRef m_Undeploying; //0x008C
    AntRef m_AnimatedCamera; //0x00A0
};//Size=0x00B4

////////////////////////////////////////
// RuntimeId: 02292
// TypeInfo:  0x0000000142BE2BE8
struct SpecialMovesBinding
{
    AntRef m_UseNewVault; //0x0000
    AntRef m_TriggerHighVault; //0x0014
    AntRef m_TriggerUpVault; //0x0028
    AntRef m_TriggerLowVault; //0x003C
    AntRef m_TriggerSlideIntoProne; //0x0050
    AntRef m_TriggerSpecialAnimation; //0x0064
    AntRef m_SpecialAnimationIndex; //0x0078
    AntRef m_ObjectHeight; //0x008C
    AntRef m_ObjectDistance; //0x00A0
    AntRef m_ObjectThickness; //0x00B4
    AntRef m_StartObjectDistanceTimer; //0x00C8
    AntRef m_VaultObjectPositionX; //0x00DC
    AntRef m_VaultObjectPositionZ; //0x00F0
    AntRef m_CancelAnimation; //0x0104
};//Size=0x0118

////////////////////////////////////////
// RuntimeId: 02290
// TypeInfo:  0x0000000142BE2C08
struct SoldierParachuteBinding
{
    AntRef m_Enabled; //0x0000
    AntRef m_SteerLeftRight; //0x0014
    AntRef m_AccelerateBrake; //0x0028
    AntRef m_Pitch; //0x003C
    AntRef m_Roll; //0x0050
};//Size=0x0064

////////////////////////////////////////
// RuntimeId: 02288
// TypeInfo:  0x0000000142BE2768
struct SoldierEmoteBinding
{
    AntRef m_Speak; //0x0000
    AntRef m_IsSquadLeader; //0x0014
    AntRef m_EmoteSpot; //0x0028
    AntRef m_EmoteOk; //0x003C
    AntRef m_EmoteNeedARide; //0x0050
    AntRef m_EmoteGoGoGo; //0x0064
    AntRef m_EmoteNeedBackup; //0x0078
    AntRef m_EmoteThanks; //0x008C
    AntRef m_EmoteNeedMedic; //0x00A0
    AntRef m_EmoteFollowMe; //0x00B4
    AntRef m_EmoteNeedAmmo; //0x00C8
    AntRef m_EmoteSorry; //0x00DC
    AntRef m_EmoteNo; //0x00F0
    AntRef m_EmoteDefendCapturePoint; //0x0104
    AntRef m_EmoteAttackCapturePoint; //0x0118
    AntRef m_EmoteMoveToPosition; //0x012C
};//Size=0x0140

////////////////////////////////////////
// RuntimeId: 02286
// TypeInfo:  0x0000000142BE2C28
struct ClimbLadderBinding
{
    AntRef m_ClimbLadderPhase; //0x0000
    AntRef m_ClimbLadderConnectPhase; //0x0014
    AntRef m_ClimbLadderTransitionPhase; //0x0028
    AntRef m_OnLadder; //0x003C
    AntRef m_LadderTopTransition; //0x0050
    AntRef m_FallHigh; //0x0064
};//Size=0x0078

////////////////////////////////////////
// RuntimeId: 02284
// TypeInfo:  0x0000000142BE2788
struct UIPopupUpdateMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 02283
// TypeInfo:  0x0000000142BE27A8
struct UIPopupHideMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 02282
// TypeInfo:  0x0000000142BE27C8
struct UIPopupShowMessage
{
    char _0x0000[104];
};//Size=0x0068

////////////////////////////////////////
// RuntimeId: 02281
// TypeInfo:  0x0000000142BE27E8
struct UITriggerPopupHideMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 02280
// TypeInfo:  0x0000000142BE2808
struct UITriggerPopupShowMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 02279
// TypeInfo:  0x0000000142BE2C48
struct UIPopupTextInput
{
    char* m_Id; //0x0000
    char* m_Label; //0x0008
    char* m_DefaultValue; //0x0010
    bool m_IsPassword; //0x0018
    char _0x0019[7];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 02267
// TypeInfo:  0x0000000142BE2C68
struct UILevelSpecificPageHeader
{
    char* m_LevelNameSID; //0x0000
    char* m_Header; //0x0008
    char* m_SubHeader; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 02247
// TypeInfo:  0x0000000142BE2C88
struct UIButton
{
    char* m_ButtonLabel; //0x0000
    char* m_ButtonId; //0x0008
    bool m_IsPremium; //0x0010
    char _0x0011[7];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 02249
// TypeInfo:  0x0000000142BE2848
struct UIButtonSet
{
    Array<UIButton> m_Buttons; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 02229
// TypeInfo:  0x0000000142BE2868
struct DefaultSelectionItem
{
    UIDataSourceInfo m_DefaultSelectionQuery; //0x0000
    __int32 m_DefaultSelectionIndex; //0x0018
    char _0x001C[4];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 02239
// TypeInfo:  0x0000000142BE2CC8
struct StaticListItem
{
    char* m_ItemName; //0x0000
    char* m_ItemIndex; //0x0008
    UISimpleDataSource m_DynamicDisable; //0x0010
    UISimpleDataSource m_DynamicNewContent; //0x0020
    bool m_AppendPlatformToSID; //0x0030
    bool m_ExcludeInRetail; //0x0031
    char _0x0032[6];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 02243
// TypeInfo:  0x0000000142BE2CA8
struct NestedList
{
    char* m_Label; //0x0000
    char* m_Index; //0x0008
    UIDataSourceInfo m_ListDataSource; //0x0010
    UIDataSourceInfo m_DynamicShowList; //0x0028
    Array<char*> m_StaticItems; //0x0040
    DefaultSelectionItem m_DefaultHighlighted; //0x0048
    UIListRowType m_RowType; //0x0068
    bool m_UseAsNormalListRows; //0x006C
    bool m_HiddenOnPC; //0x006D
    bool m_HiddenOnXenon; //0x006E
    bool m_HighLightOnUpdate; //0x006F
};//Size=0x0070

////////////////////////////////////////
// RuntimeId: 02213
// TypeInfo:  0x0000000142BE2CE8
struct HudCameraShakeParams
{
    float m_WeaponDispersionMin; //0x0000
    float m_WeaponDispersionMax; //0x0004
    float m_WeaponDispersionScale; //0x0008
    unsigned __int32 m_WeaponDispersionFilterSize; //0x000C
    float m_JumpEffectDurationMin; //0x0010
    float m_JumpEffectDurationMax; //0x0014
    float m_JumpEffectMin; //0x0018
    float m_JumpEffectMax; //0x001C
    float m_LandEffectDurationMin; //0x0020
    float m_LandEffectDurationMax; //0x0024
    float m_LandEffectMin; //0x0028
    float m_LandEffectMax; //0x002C
    unsigned __int32 m_JumpAndLandEffectFilterSize; //0x0030
    float m_CameraShakeMinAngle; //0x0034
    float m_CameraShakeMaxAngle; //0x0038
    float m_CameraShakeXScale; //0x003C
    float m_CameraShakeYScale; //0x0040
    unsigned __int32 m_CameraShakeFilterSize; //0x0044
    float m_WeaponLagXScale; //0x0048
    float m_WeaponLagYScale; //0x004C
    unsigned __int32 m_LagFilterSize; //0x0050
    float m_DisplacementPivotZ; //0x0054
    float m_MaxDisplacementAngle; //0x0058
    float m_MaxDisplacementZ; //0x005C
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 02193
// TypeInfo:  0x0000000142BE2D08
struct UITooltipLine
{
    __int32 m_InputAction; //0x0000
    UIInputActionAxisDisplay m_Axis; //0x0004
    char* m_Description; //0x0008
    bool m_HideWhenMissingIcon; //0x0010
    char _0x0011[7];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 02187
// TypeInfo:  0x0000000142BE2D28
struct UIMouseButtonsGroup
{
    bool m_IsWin32; //0x0000
    bool m_IsXenon; //0x0001
    bool m_IsPS3; //0x0002
    bool m_IsDurango; //0x0003
    bool m_IsOrbis; //0x0004
};//Size=0x0005

////////////////////////////////////////
// RuntimeId: 02137
// TypeInfo:  0x0000000142BE28A8
struct UITextureAtlasInfo
{
    Vec2 m_MinUv; //0x0000
    Vec2 m_MaxUv; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 02183
// TypeInfo:  0x0000000142BE2D68
struct UIKeysButtonsGroup
{
    bool m_IsWin32; //0x0000
    bool m_IsXenon; //0x0001
    bool m_IsPS3; //0x0002
    bool m_IsDurango; //0x0003
    bool m_IsOrbis; //0x0004
};//Size=0x0005

////////////////////////////////////////
// RuntimeId: 02181
// TypeInfo:  0x0000000142BE2D88
struct UIKeysButtonTexture
{
    InputDeviceKeys m_Key; //0x0000
    UITextureAtlasInfo m_AtlasInfo; //0x0004
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 02179
// TypeInfo:  0x0000000142BE2DA8
struct UIPadButtonsGroup
{
    bool m_IsWin32; //0x0000
    bool m_IsXenon; //0x0001
    bool m_IsPS3; //0x0002
    bool m_IsDurango; //0x0003
    bool m_IsOrbis; //0x0004
};//Size=0x0005

////////////////////////////////////////
// RuntimeId: 02177
// TypeInfo:  0x0000000142BE2DC8
struct UIPadButtonTexture
{
    InputDevicePadButtons m_PadButton; //0x0000
    UITextureAtlasInfo m_AtlasInfo; //0x0004
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 02175
// TypeInfo:  0x0000000142BE2DE8
struct UIAxesButtonsGroup
{
    bool m_IsWin32; //0x0000
    bool m_IsXenon; //0x0001
    bool m_IsPS3; //0x0002
    bool m_IsDurango; //0x0003
    bool m_IsOrbis; //0x0004
};//Size=0x0005

////////////////////////////////////////
// RuntimeId: 02173
// TypeInfo:  0x0000000142BE2E08
struct UIAxesButtonTexture
{
    InputDeviceAxes m_Axis; //0x0000
    UITextureAtlasInfo m_AtlasInfo; //0x0004
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 02167
// TypeInfo:  0x0000000142BE2E48
struct UICreditsColumn
{
    char* m_Text; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 02169
// TypeInfo:  0x0000000142BE2E28
struct UICreditsRow
{
    UICreditsTextType m_TextType; //0x0000
    char _0x0004[4];
    Array<UICreditsColumn> m_Columns; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 02163
// TypeInfo:  0x0000000142BE2E68
struct UIMinimapData
{
    float m_WorldRotation; //0x0000
    float m_AirRadarRange; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 02161
// TypeInfo:  0x0000000142BE2E88
struct UIMinimapDistanceFieldParams
{
    Vec4 m_ColorTint; //0x0000
    Vec4 m_OutlineColor; //0x0010
    UIDistanceFieldAsset* m_DistanceField; //0x0020
    float m_AlphaThreshold; //0x0028
    float m_DistanceScale; //0x002C
    float m_OutlineInner; //0x0030
    float m_OutlineOuter; //0x0034
    char _0x0038[8];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 02151
// TypeInfo:  0x0000000142BE2EE8
struct UIMinimapIconUv
{
    Vec2 m_MinUv; //0x0000
    Vec2 m_MaxUv; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 02153
// TypeInfo:  0x0000000142BE2EC8
struct UIMinimapIconTextureState
{
    UIIconMode m_Mode; //0x0000
    UIIconState m_State; //0x0004
    float m_FrameRate; //0x0008
    char _0x000C[4];
    Array<UIMinimapIconUv> m_TextureInfos; //0x0010
    bool m_ShouldRotate; //0x0018
    char _0x0019[7];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 02155
// TypeInfo:  0x0000000142BE2EA8
struct UIMinimapIconTexture
{
    UIHudIcon m_IconType; //0x0000
    char _0x0004[4];
    char* m_Name; //0x0008
    Array<UIMinimapIconTextureState> m_States; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 02185
// TypeInfo:  0x0000000142BE2D48
struct UIMouseButtonTexture
{
    InputDeviceMouseButtons m_MouseButton; //0x0000
    UITextureAtlasInfo m_AtlasInfo; //0x0004
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 02127
// TypeInfo:  0x0000000142BD91A8
struct UIScreenActionInputEventMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 02040
// TypeInfo:  0x0000000142BD93E8
struct ManualDataEntry
{
    GamePlatform m_Platform; //0x0000
    LanguageFormat m_Language; //0x0004
    Array<TextureAsset*> m_Textures; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 02022
// TypeInfo:  0x0000000142BD9428
struct InspectAnimationTriggerData
{
    AntRef m_Animation; //0x0000
    float m_Probability; //0x0014
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 02024
// TypeInfo:  0x0000000142BD9408
struct InspectViewPointData
{
    char* m_ViewPointID; //0x0000
    Array<InspectAnimationTriggerData> m_AnimationTriggers; //0x0008
    Array<AntRef> m_ContinuousAnimationSignal; //0x0010
    float m_LookAtHeight; //0x0018
    float m_Yaw; //0x001C
    float m_Pitch; //0x0020
    float m_Distance; //0x0024
    float m_FovOffset; //0x0028
    float m_AdjustmentYaw; //0x002C
    bool m_Fixed; //0x0030
    char _0x0031[7];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 02002
// TypeInfo:  0x0000000142BD91C8
struct UIMapNavigationData
{
    float m_NearestZoomRange; //0x0000
    Vec2 m_ManualViewCenter; //0x0004
    float m_ManualViewRange; //0x000C
    float m_PanSpeed; //0x0010
    bool m_UseAutoView; //0x0014
    char _0x0015[3];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 01996
// TypeInfo:  0x0000000142BD9468
struct UILevelLoadIconData
{
    char* m_Label; //0x0000
    char* m_IconName; //0x0008
    Vec2 m_Pos; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 01998
// TypeInfo:  0x0000000142BD9448
struct UILevelLoadGameModeData
{
    char* m_GameModeIdentifier; //0x0000
    unsigned __int32 m_PackageIdentifier; //0x0008
    char _0x000C[4];
    Array<char*> m_BackgroundImagesPath; //0x0010
    Array<UILevelLoadIconData> m_IconData; //0x0018
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 01994
// TypeInfo:  0x0000000142BD91E8
struct UIGPSPosition
{
    double m_Latitude; //0x0000
    double m_Longitude; //0x0008
    double m_SeaLevelOffset; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 01992
// TypeInfo:  0x0000000142BD9208
struct UILevelStatData
{
    char* m_StatEasy; //0x0000
    char* m_StatMedium; //0x0008
    char* m_StatHard; //0x0010
    char* m_StatHardcore; //0x0018
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 01986
// TypeInfo:  0x0000000142BD9228
struct UIGeoLongitude
{
    __int32 m_Degrees; //0x0000
    __int32 m_Minuites; //0x0004
    __int32 m_Seconds; //0x0008
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 01984
// TypeInfo:  0x0000000142BD9248
struct UIGeoLatitude
{
    __int32 m_Degrees; //0x0000
    __int32 m_Minuites; //0x0004
    __int32 m_Seconds; //0x0008
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 01980
// TypeInfo:  0x0000000142BD9268
struct SoldierLeaningBinding
{
    AntRef m_Lean; //0x0000
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 01976
// TypeInfo:  0x0000000142BD9488
struct GameTipData
{
    char* m_Text; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 01966
// TypeInfo:  0x0000000142BD94C8
struct GameModeTeamSize
{
    unsigned __int32 m_PlayerCount; //0x0000
    unsigned __int32 m_SquadSize; //0x0004
    unsigned __int32 m_SquadAutoFillLimit; //0x0008
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 01968
// TypeInfo:  0x0000000142BD9288
struct GameModeSize
{
    char* m_Name; //0x0000
    char* m_ShortName; //0x0008
    char* m_MetaIdentifier; //0x0010
    unsigned __int32 m_PlayerCount; //0x0018
    unsigned __int32 m_CommanderCount; //0x001C
    Array<GameModeTeamSize> m_Teams; //0x0020
    unsigned __int32 m_RoundsPerMap; //0x0028
    bool m_ForceSquad; //0x002C
    bool m_AddPlatformToMetaIdentifier; //0x002D
    char _0x002E[2];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 01970
// TypeInfo:  0x0000000142BD94A8
struct GameModeInformation
{
    GamePlatform m_Platform; //0x0000
    char _0x0004[4];
    Array<GameModeSize> m_Sizes; //0x0008
    GameModeSize m_DefaultSize; //0x0010
    bool m_AllowFallbackToDefault; //0x0040
    char _0x0041[7];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 01960
// TypeInfo:  0x0000000142BD94E8
struct FreeStreamingChunksLoadInfo
{
    char* m_Name; //0x0000
    __int32 m_NumPartitions; //0x0008
    bool m_IsLocalized; //0x000C
    bool m_IsOptional; //0x000D
    char _0x000E[2];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 01940
// TypeInfo:  0x0000000142BD9508
struct CounterHelper
{
    bool m_dummy; //0x0000
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 01868
// TypeInfo:  0x0000000142BD93C8
struct AntiSpamConfig
{
    float m_DetectionInterval; //0x0000
    unsigned __int32 m_DetectionIntervalMaxMessageCount; //0x0004
    unsigned __int32 m_ServerSideMessageCountTolerance; //0x0008
    unsigned __int32 m_SecondsBlocked; //0x000C
    unsigned __int32 m_ServerSideSecondsBlockedTolerance; //0x0010
    char _0x0014[4];
    char* m_NotificationSid; //0x0018
    char* m_StillBlockedSid; //0x0020
    SoundWaveAsset* m_WarningSound; //0x0028
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 01852
// TypeInfo:  0x0000000142BD9528
struct UILevelLocation
{
    char _0x0000[1];
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 01848
// TypeInfo:  0x0000000142BD9548
struct UIMatchmakingGamemodeMappack
{
    char* m_Experience; //0x0000
    char* m_MapPackIdentifier; //0x0008
    char* m_MapPackLicense; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 01818
// TypeInfo:  0x0000000142BD9568
struct GameModeVariation
{
    char* m_Identifier; //0x0000
    char* m_Name; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 01742
// TypeInfo:  0x0000000142BD1CE0
struct GameTweakBlindedBehavior
{
    float m_BlindedTime; //0x0000
    float m_BlindedRadius; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 01740
// TypeInfo:  0x0000000142BD1C40
struct GameTweakPanicBehavior
{
    float m_PrePanicTime; //0x0000
    float m_TotalPanicTime; //0x0004
    float m_PanicBlastRadiusPercentage; //0x0008
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 01738
// TypeInfo:  0x0000000142BD1D60
struct GameTweakSniper
{
    float m_GlintToShotFiredDelay; //0x0000
    float m_HideAfterShotDelay; //0x0004
    float m_TimeUpBeforeScopeGlint; //0x0008
    float m_TimeHidden; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 01736
// TypeInfo:  0x0000000142BD1DC0
struct GameTweakAIDamage
{
    float m_DamageBucketSizeHumanSquad; //0x0000
    float m_DamageBucketSizeFriendlies; //0x0004
    float m_DamageBucketSizeEnemies; //0x0008
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 01730
// TypeInfo:  0x0000000142BD1DA0
struct EvasiveBucketTimeSettings
{
    Array<float> m_SprintDamageBucketTimes; //0x0000
    Array<float> m_CrouchDamageBucketTimes; //0x0008
    Array<float> m_ProneDamageBucketTimes; //0x0010
    Array<float> m_MovingVehicleDamageBucketTimes; //0x0018
    Array<float> m_VehicleStillDamageBucketTimes; //0x0020
    float m_SprintSpeed; //0x0028
    float m_MovingVehicleSpeed; //0x002C
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 01724
// TypeInfo:  0x0000000142BD1DE0
struct TinyPiecewiseLinearCurve
{
    float m_X0; //0x0000
    float m_Y0; //0x0004
    float m_X1; //0x0008
    float m_Y1; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 01734
// TypeInfo:  0x0000000142BD1E00
struct GameTweakBucketDamage
{
    Array<float> m_PlayerDamageBucketTimes; //0x0000
    EvasiveBucketTimeSettings m_EvasiveBucketTimeSettings; //0x0008
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 01728
// TypeInfo:  0x0000000142BD1EC0
struct AccuracyPenaltySettings
{
    float m_SprintMultiplier; //0x0000
    float m_CrouchMultiplier; //0x0004
    float m_ProneMultiplier; //0x0008
    float m_MovingVehicleMultiplier; //0x000C
    float m_VehicleStillMultiplier; //0x0010
    float m_SprintSpeed; //0x0014
    float m_MovingVehicleSpeed; //0x0018
};//Size=0x001C

////////////////////////////////////////
// RuntimeId: 01726
// TypeInfo:  0x0000000142BD1E20
struct GameTweakCommon
{
    float m_SensingConeRange; //0x0000
    float m_SensingConeRangeWhenAlerted; //0x0004
    float m_SensingConeAngle; //0x0008
    float m_SensingConeAngleWhenAlerted; //0x000C
    float m_SensingConeAngleAbove; //0x0010
    float m_SensingConeAngleBelow; //0x0014
    float m_SensingShotFiredDistance; //0x0018
    float m_SensingHumanDrivenVehicleDistance; //0x001C
    float m_SensingExplosionDistance; //0x0020
    float m_SensingGrenadeDistance; //0x0024
    TinyPiecewiseLinearCurve m_DistanceToAwareTimeCurveCentral; //0x0028
    float m_AwareCurveCentralAngle; //0x0038
    TinyPiecewiseLinearCurve m_DistanceToAwareTimeCurvePeripheral; //0x003C
    float m_AwareStandingPenaltyMultiplier; //0x004C
    float m_AwareToAlertTime; //0x0050
    float m_HighIntensityRadius; //0x0054
    float m_ConcealmentCoolDownTime; //0x0058
    float m_ConcealmentDisableAtDistance; //0x005C
    float m_PassiveTime; //0x0060
    float m_GrenadeMinimumTime; //0x0064
    float m_GrenadeMaximumTime; //0x0068
    float m_ChargeGrenadeMinimumTime; //0x006C
    float m_ChargeGrenadeMaximumTime; //0x0070
    __int32 m_SimultaneousChargeCount; //0x0074
    __int32 m_SimultaneousVehicleUsage; //0x0078
    float m_MinVehicleBookingDelayOnTokenAvailable; //0x007C
    float m_MaxVehicleBookingDelayOnTokenAvailable; //0x0080
    float m_EnemyMinTimeInMediumIntensity; //0x0084
};//Size=0x0088

////////////////////////////////////////
// RuntimeId: 01732
// TypeInfo:  0x0000000142BD1D00
struct GameTweakAITargeting
{
    float m_IntervalBetweenAdditionalEnemiesTargetingPlayer; //0x0000
    TinyPiecewiseLinearCurve m_FiringDelayAfterAquiringTarget; //0x0004
    float m_ReaquireTargetDistanceThreshold; //0x0014
    TinyPiecewiseLinearCurve m_FiringDelayAfterReaquiringTarget; //0x0018
    TinyPiecewiseLinearCurve m_TimeUntilDamageFromStartOfFirstBurst; //0x0028
    TinyPiecewiseLinearCurve m_TimeUntilDamageTimeIsFullyRestored; //0x0038
    AccuracyPenaltySettings m_AccuracyPenaltySettings; //0x0048
};//Size=0x0064

////////////////////////////////////////
// RuntimeId: 01720
// TypeInfo:  0x0000000142BD1F20
struct FiringPatternData
{
    unsigned __int64 m_Pattern; //0x0000
    __int32 m_Id; //0x0008
    FiringWeaponClass m_WeaponClass; //0x000C
    IntensityClass m_Intensity; //0x0010
    __int32 m_Delay; //0x0014
    bool m_SingleShot; //0x0018
    char _0x0019[7];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 01706
// TypeInfo:  0x0000000142BD1C60
struct Distances
{
    float m_Min; //0x0000
    float m_Max; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 01708
// TypeInfo:  0x0000000142BD1D20
struct VehicleVsSoldier
{
    Distances m_Distances; //0x0000
    __int32 m_NumRepositions; //0x0008
    float m_FlushOutTime; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 01710
// TypeInfo:  0x0000000142BD1CC0
struct VehicleVsVehicle
{
    Distances m_Distances; //0x0000
    float m_SearchTime; //0x0008
    float m_KeepFocusWhileSearchingTime; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 01700
// TypeInfo:  0x0000000142BD1E40
struct VehicleSuppressionSettings
{
    float m_SuppressionDistance; //0x0000
    float m_ReevaluateCoverDistance; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 01698
// TypeInfo:  0x0000000142BD1D80
struct WeaponSuppressionSettings
{
    float m_SuppressionValue; //0x0000
    float m_ContinuousSuppressionTime; //0x0004
    float m_SuppressionAreaSize; //0x0008
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 01696
// TypeInfo:  0x0000000142BD1EA0
struct EngageSettings
{
    float m_DistanceToTarget; //0x0000
    float m_SuppressionValue; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 01682
// TypeInfo:  0x0000000142BD1E60
struct AttackCoverRequestSettings
{
    float m_MinimumTimeInCoverToRequest; //0x0000
    float m_MaximumTimeInCoverToRequest; //0x0004
    float m_MinimumTimeIntervalToRequestProtectiveCover; //0x0008
    float m_MaximumTimeIntervalToRequestProtectiveCover; //0x000C
    float m_MinimumTimeInCoverAfterTargetLost; //0x0010
    float m_MaximumTimeInCoverAfterTargetLost; //0x0014
    float m_DistanceToTargetCompromisedThreshold; //0x0018
    float m_MinimumDistanceToTarget; //0x001C
    TinyPiecewiseLinearCurve m_MinimumInitialDelay; //0x0020
    TinyPiecewiseLinearCurve m_MaximumInitialDelay; //0x0030
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 01600
// TypeInfo:  0x0000000142BD1F00
struct BFCoverSlot
{
    Vec3 m_Position; //0x0000
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 01594
// TypeInfo:  0x0000000142BD1F60
struct BFCoverData
{
    Vec4 m_PosAndExtra0; //0x0000
    Vec4 m_DirectionAndExtra1; //0x0010
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 01590
// TypeInfo:  0x0000000142BD1F40
struct CoverQueryScoreRuntime
{
    __int32 m_RelationMask; //0x0000
    __int32 m_Flags; //0x0004
    __int32 m_Filter; //0x0008
    CoverQueryStyle m_XStyle; //0x000C
    CoverQueryPosition m_FromPosition; //0x0010
    CoverQueryPosition m_ToPosition; //0x0014
    CoverQueryDirection m_ReferenceDirection; //0x0018
    CoverQueryCurveXSource m_CurveXSource; //0x001C
    Array<float> m_Curve; //0x0020
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 01584
// TypeInfo:  0x0000000142BD1CA0
struct CoverQueryFilter
{
    bool m_LeftBlocked; //0x0000
    bool m_LeftNotBlocked; //0x0001
    bool m_RightBlocked; //0x0002
    bool m_RightNotBlocked; //0x0003
    bool m_TopBlocked; //0x0004
    bool m_TopNotBlocked; //0x0005
    bool m_StandCovers; //0x0006
    bool m_MediumCovers; //0x0007
    bool m_CrouchCovers; //0x0008
    bool m_ProneCovers; //0x0009
    bool m_OpenCovers; //0x000A
    bool m_OpenDirectionalCovers; //0x000B
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 01564
// TypeInfo:  0x0000000142BD1C80
struct BFCoverZones
{
    BFCoverZoneDefinition* m_CoverZonesOverAndOut; //0x0000
    BFCoverZoneDefinition* m_CoverZonesOver; //0x0008
    BFCoverZoneDefinition* m_CoverZonesOut; //0x0010
    BFCoverZoneDefinition* m_CoverZonesOpen; //0x0018
    BFCoverZoneDefinition* m_CoverZonesOpenDirectional; //0x0020
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 01560
// TypeInfo:  0x0000000142BD1F80
struct BFCoverZone
{
    float m_FirstAngle; //0x0000
    float m_SecondAngle; //0x0004
    float m_CloseDistance; //0x0008
    float m_FarDistance; //0x000C
    bool m_Peek; //0x0010
    bool m_Over; //0x0011
    bool m_Out; //0x0012
    bool m_Flanked; //0x0013
    bool m_AlwaysRequestCover; //0x0014
    bool m_RequestCoverIfUnderFire; //0x0015
    bool m_ForceOutFiring; //0x0016
    bool m_TriggerFlanked; //0x0017
    bool m_SuppressedByEnemyBullets; //0x0018
    bool m_AllowProjectileFire; //0x0019
    char _0x001A[2];
};//Size=0x001C

////////////////////////////////////////
// RuntimeId: 01522
// TypeInfo:  0x0000000142BCE678
struct BurstInterval
{
    float m_BurstLength; //0x0000
    float m_BurstLengthVariation; //0x0004
    float m_TimeBetweenBursts; //0x0008
    float m_TimeBetweenBurstsVariation; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 01516
// TypeInfo:  0x0000000142BCE638
struct Interval
{
    float m_MinimumTime; //0x0000
    float m_MaximumTime; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 01504
// TypeInfo:  0x0000000142BCE558
struct BFAIProjectileBinding
{
    AntRef m_FireProjectile; //0x0000
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 01498
// TypeInfo:  0x0000000142BCE5B8
struct LinearTime
{
    float m_Distance; //0x0000
    float m_Time; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 01500
// TypeInfo:  0x0000000142BCE5F8
struct Dispersion
{
    float m_MinimumHorizontal; //0x0000
    float m_MaximumHorizontal; //0x0004
    float m_MinimumVertical; //0x0008
    float m_MaximumVertical; //0x000C
    float m_ScalingDistance; //0x0010
    bool m_ScaleWithDistance; //0x0014
    char _0x0015[3];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 01502
// TypeInfo:  0x0000000142BCE658
struct BFAIProjectile
{
    BFAIProjectileCurveType m_Curve; //0x0000
    LinearTime m_Minimum; //0x0004
    LinearTime m_Maximum; //0x000C
    Dispersion m_Dispersion; //0x0014
    char _0x002C[4];
    ProjectileBlueprint* m_Projectile; //0x0030
    SoundAsset* m_Sound; //0x0038
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 01484
// TypeInfo:  0x0000000142BCE4B8
struct AreaBookingSettings
{
    float m_EnemyTeamBookingRadius; //0x0000
    float m_FriendlyTeamBookingRadius; //0x0004
    float m_CorpseBookingTime; //0x0008
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 01482
// TypeInfo:  0x0000000142BCE598
struct SquadEngageData
{
    float m_EngageTime; //0x0000
    float m_FindCoverTime; //0x0004
    float m_FailureCooldownTime; //0x0008
    float m_CooldownTime; //0x000C
    float m_SecondaryTargetRadius; //0x0010
    float m_CoverFireTargetRadius; //0x0014
    float m_AllowedTargetHiddenTime; //0x0018
    float m_WaitingToFireTime; //0x001C
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 01480
// TypeInfo:  0x0000000142BCE4D8
struct ShootingData
{
    float m_KeepFiringAtPlayerTime; //0x0000
    float m_KeepFiringAtAITime; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 01476
// TypeInfo:  0x0000000142BCE538
struct SquadTargettingData
{
    float m_VisibleTargetLimit; //0x0000
    float m_TargetLostTimer; //0x0004
    Array<float> m_TargetImportanceThresholds; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 01478
// TypeInfo:  0x0000000142BCE578
struct TargetingData
{
    float m_ForcePlayerAsTargetRange; //0x0000
    float m_ForcePlayerAsTargetAfterAttackedTime; //0x0004
    float m_AimOffsetY; //0x0008
    char _0x000C[4];
    SquadTargettingData m_SquadTargettingData; //0x0010
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 01474
// TypeInfo:  0x0000000142BCE618
struct DamageAndPrecisionData
{
    float m_BulletDamage; //0x0000
    float m_AiDamageBucketTime; //0x0004
    bool m_IsSquadUsingDamageBucket; //0x0008
    char _0x0009[3];
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 01472
// TypeInfo:  0x0000000142BCE4F8
struct BFAI2SpottingPotentialMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 01471
// TypeInfo:  0x0000000142BCE5D8
struct BFAI2SpottingFoundMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 00954
// TypeInfo:  0x0000000142BB4A40
struct StatDamageHelperMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00953
// TypeInfo:  0x0000000142BB4A60
struct StatPointsMessageBase
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 00952
// TypeInfo:  0x0000000142BB4A80
struct StatEventMessageBase
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 00951
// TypeInfo:  0x0000000142BB4AA0
struct StatTriggerEntityMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00950
// TypeInfo:  0x0000000142BB4AC0
struct StatEventEntityTriggerMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00949
// TypeInfo:  0x0000000142BB4AE0
struct StatGainRankMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00948
// TypeInfo:  0x0000000142BB4B00
struct StatGainAwardMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00947
// TypeInfo:  0x0000000142BB4B20
struct StatFinalizeStatsMessageBase
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00946
// TypeInfo:  0x0000000142BB4B40
struct StatClubRecordClosingMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00945
// TypeInfo:  0x0000000142BB4B60
struct StatClubRecordCreatedMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00944
// TypeInfo:  0x0000000142BB4B80
struct StatPlayerRecordClosingMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00943
// TypeInfo:  0x0000000142BB4BA0
struct StatPlayerRecordCreatedMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00942
// TypeInfo:  0x0000000142BB4BC0
struct ServerBackendExternalIdResponseMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00941
// TypeInfo:  0x0000000142BB4BE0
struct ServerBackendExternalIdRequestMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00940
// TypeInfo:  0x0000000142BB4C00
struct ServerBackendStringMetricMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00939
// TypeInfo:  0x0000000142BB4C20
struct ServerBackendCounterMetricMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00938
// TypeInfo:  0x0000000142BB4C40
struct ServerBackendGaugeMetricMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00937
// TypeInfo:  0x0000000142BB4C60
struct ServerBackendTagMetricMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00936
// TypeInfo:  0x0000000142BB4C80
struct ServerBackendLiveScoreboardDisableMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00935
// TypeInfo:  0x0000000142BB4CA0
struct ServerBackendLiveScoreboardEnableMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00934
// TypeInfo:  0x0000000142BB4CC0
struct ServerBackendLiveServerProxyDisableMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00933
// TypeInfo:  0x0000000142BB4CE0
struct ServerBackendLiveServerProxyEnableMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00932
// TypeInfo:  0x0000000142BB4D00
struct ServerBackendLiveServerProxyConfigUpdatedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00931
// TypeInfo:  0x0000000142BB4D20
struct ServerBackendLogTransmitterSettingsUpdatedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00930
// TypeInfo:  0x0000000142BB4D40
struct ServerBackendBattlelogTransactionMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00929
// TypeInfo:  0x0000000142BB4D60
struct PlaygroundServerBackendResponseMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00928
// TypeInfo:  0x0000000142BB4D80
struct PeerServerBackendResponseMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00927
// TypeInfo:  0x0000000142BB4DA0
struct PeerServerBackendRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00926
// TypeInfo:  0x0000000142BB4DC0
struct ServerBackendRoleResponseMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00925
// TypeInfo:  0x0000000142BB4DE0
struct ServerBackendRoleRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00924
// TypeInfo:  0x0000000142BB4E00
struct ServerBackendBattlepackResponseMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00923
// TypeInfo:  0x0000000142BB4E20
struct ServerBackendBattlepackRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00922
// TypeInfo:  0x0000000142BB4E40
struct ServerBackendOnGameReportMetricMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00921
// TypeInfo:  0x0000000142BB4E60
struct ServerBackendOnGameReportCompleteMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00920
// TypeInfo:  0x0000000142BB4E80
struct ServerBackendGameReportCompleteMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00919
// TypeInfo:  0x0000000142BB4EA0
struct ServerBackendOnFinalizationCompleteMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00918
// TypeInfo:  0x0000000142BB4EC0
struct VeniceServerMetricsFriendZoneSpawnMessage
{
    char _0x0000[128];
};//Size=0x0080

////////////////////////////////////////
// RuntimeId: 00917
// TypeInfo:  0x0000000142BB4EE0
struct VeniceServerMetricsManDownImmobilizedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00916
// TypeInfo:  0x0000000142BB4F00
struct VeniceServerMetricsManDownMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00915
// TypeInfo:  0x0000000142BB4F20
struct VeniceServerMetricsRestartRoundMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00914
// TypeInfo:  0x0000000142BB4F40
struct VeniceServerMetricsStartRoundMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00913
// TypeInfo:  0x0000000142BB4F60
struct VeniceServerMetricsLifeCounterMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00912
// TypeInfo:  0x0000000142BB4F80
struct VeniceServerMetricsTicketCounterMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00911
// TypeInfo:  0x0000000142BB4FA0
struct VeniceServerMetricsRankupMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00910
// TypeInfo:  0x0000000142BB4FC0
struct VeniceServerMetricsCreateLogMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00818
// TypeInfo:  0x0000000142BAD180
struct ServerLevolutionTriggeredMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 00813
// TypeInfo:  0x0000000142BAD1A0
struct VeniceNetworkWeaponAimingMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 00800
// TypeInfo:  0x0000000142BAD1C0
struct IndirectFireDispersionData
{
    float m_MaxAngle; //0x0000
    float m_MinAngle; //0x0004
    float m_DispersionAimMove; //0x0008
    float m_DispersionAimMoveThreshold; //0x000C
    float m_DispersionFiring; //0x0010
    float m_DispersionDeployment; //0x0014
    float m_DecreasePerSecond; //0x0018
};//Size=0x001C

////////////////////////////////////////
// RuntimeId: 00794
// TypeInfo:  0x0000000142BAD1E0
struct PlayerFriendManagerFriendJoinEventMessage
{
    char _0x0000[104];
};//Size=0x0068

////////////////////////////////////////
// RuntimeId: 00791
// TypeInfo:  0x0000000142BAD220
struct PlayerFriendManagerSetAllowCreateFriendSquadMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00790
// TypeInfo:  0x0000000142BAD240
struct PlayerFriendManagerAbortFriendJoinMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00781
// TypeInfo:  0x0000000142BAD260
struct ColorPalletesMessageBase
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00780
// TypeInfo:  0x0000000142BAD280
struct VeniceNetworkHardwareIdMessage
{
    char _0x0000[88];
};//Size=0x0058

////////////////////////////////////////
// RuntimeId: 00779
// TypeInfo:  0x0000000142BAD2A0
struct VeniceNetworkRequestHardwareIdMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00778
// TypeInfo:  0x0000000142BAD2C0
struct VeniceNetworkRequestFrontBufferScreenshot2Message
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 00777
// TypeInfo:  0x0000000142BAD2E0
struct VeniceNetworkRequestFrontBufferScreenshotMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 00776
// TypeInfo:  0x0000000142BAD300
struct VeniceNetworkVictimDisabledPartMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 00775
// TypeInfo:  0x0000000142BAD320
struct VeniceNetworkVictimMissingPhysicsComponentMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 00774
// TypeInfo:  0x0000000142BAD340
struct VeniceNetworkIgnoredDamageMessage
{
    char _0x0000[120];
};//Size=0x0078

////////////////////////////////////////
// RuntimeId: 00773
// TypeInfo:  0x0000000142BAD360
struct VeniceNetworkSavePersistenceMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00772
// TypeInfo:  0x0000000142BAD380
struct VeniceNetworkClientLanguageMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00771
// TypeInfo:  0x0000000142BAD3A0
struct VeniceNetworkRequestClientLanguageMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00770
// TypeInfo:  0x0000000142BAD3C0
struct VeniceNetworkRequestScreenshotMessage
{
    char _0x0000[88];
};//Size=0x0058

////////////////////////////////////////
// RuntimeId: 00769
// TypeInfo:  0x0000000142BAD3E0
struct VeniceNetworkClientCameraInfoMessageBase
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00768
// TypeInfo:  0x0000000142BAD400
struct VeniceNetworkServerStatInfoMessageBase
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00767
// TypeInfo:  0x0000000142BAD420
struct VeniceNetworkRequestServerStatInfoMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 00766
// TypeInfo:  0x0000000142BAD440
struct VeniceNetworkSuspiciousKeyMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00765
// TypeInfo:  0x0000000142BAD460
struct VeniceNetworkPunkBusterActiveMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00764
// TypeInfo:  0x0000000142BAD480
struct VeniceNetworkPunkBusterPacketMessageBase
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00763
// TypeInfo:  0x0000000142BAD4A0
struct VeniceNetworkServerInfoMessage
{
    char _0x0000[144];
};//Size=0x0090

////////////////////////////////////////
// RuntimeId: 00762
// TypeInfo:  0x0000000142BAD4C0
struct VeniceNetworkRequestServerInfoMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00761
// TypeInfo:  0x0000000142BAD4E0
struct VeniceNetworkSetPingMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00760
// TypeInfo:  0x0000000142BAD500
struct VeniceNetworkMetricsAchievementMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00759
// TypeInfo:  0x0000000142BAD520
struct VeniceNetworkLocalClientConnectionInfoUpdatedMessageBase
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00758
// TypeInfo:  0x0000000142BAD540
struct VeniceNetworkOnPlayerSelectedTeamMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00755
// TypeInfo:  0x0000000142BAD580
struct VeniceNetworkFriendZoneDebugEnableMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00754
// TypeInfo:  0x0000000142BAD5A0
struct VeniceNetworkFriendZoneMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00753
// TypeInfo:  0x0000000142BAD5C0
struct VeniceNetworkVirtualGameStateMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00752
// TypeInfo:  0x0000000142BAD5E0
struct VeniceNetworkSendTelemetryTokenMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 00751
// TypeInfo:  0x0000000142BAD600
struct VeniceNetworkNotifyMicStateMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00750
// TypeInfo:  0x0000000142BAD620
struct VeniceNetworkSetVoiceChannelMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00749
// TypeInfo:  0x0000000142BAD640
struct VeniceNetworkVoiceTokenMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00748
// TypeInfo:  0x0000000142BAD660
struct VeniceNetworkEorStatsMessage
{
    char _0x0000[176];
};//Size=0x00B0

////////////////////////////////////////
// RuntimeId: 00747
// TypeInfo:  0x0000000142BAD680
struct VeniceNetworkSettingsMessage
{
    char _0x0000[216];
};//Size=0x00D8

////////////////////////////////////////
// RuntimeId: 00746
// TypeInfo:  0x0000000142BAD6A0
struct VeniceNetworkRequestCoopPlayerInformationMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00745
// TypeInfo:  0x0000000142BAD6C0
struct VeniceNetworkRequestCoopEorStatsMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00744
// TypeInfo:  0x0000000142BAD6E0
struct VeniceNetworkRequestEorStatsMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00743
// TypeInfo:  0x0000000142BAD700
struct VeniceNetworkDefibKillMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 00742
// TypeInfo:  0x0000000142BAD720
struct VeniceNetworkScoreChangeMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00741
// TypeInfo:  0x0000000142BAD740
struct VeniceNetworkCounterStatusMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00740
// TypeInfo:  0x0000000142BAD760
struct VeniceNetworkRequestCounterStatusMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00739
// TypeInfo:  0x0000000142BAD780
struct VeniceNetworkMissionAwardsStatusMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00736
// TypeInfo:  0x0000000142BAF120
struct CounterStatus
{
    float m_CurrentValue; //0x0000
    float m_OriginalValue; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 00738
// TypeInfo:  0x0000000142BAF100
struct AwardStatus
{
    char* m_Code; //0x0000
    unsigned __int32 m_CurrentValue; //0x0008
    unsigned __int32 m_OriginalValue; //0x000C
    Array<CounterStatus> m_Counters; //0x0010
    bool m_IsCounting; //0x0018
    char _0x0019[7];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 00734
// TypeInfo:  0x0000000142BAF140
struct StatCounterStatus
{
    char* m_Code; //0x0000
    float m_CurrentValue; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 00732
// TypeInfo:  0x0000000142BAD7A0
struct VeniceNetworkDogTagMessage
{
    char _0x0000[88];
};//Size=0x0058

////////////////////////////////////////
// RuntimeId: 00731
// TypeInfo:  0x0000000142BAD7C0
struct VeniceNetworkAchievementUpdateMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00730
// TypeInfo:  0x0000000142BAD7E0
struct VeniceNetworkAchievementProgressMessage
{
    char _0x0000[88];
};//Size=0x0058

////////////////////////////////////////
// RuntimeId: 00729
// TypeInfo:  0x0000000142BAD800
struct VeniceNetworkUnlockAchievementsMessage
{
    char _0x0000[88];
};//Size=0x0058

////////////////////////////////////////
// RuntimeId: 00728
// TypeInfo:  0x0000000142BAD820
struct VeniceNetworkAllWeaponKillsMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 00727
// TypeInfo:  0x0000000142BAD840
struct VeniceNetworkRequestAllWeaponKillsMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00726
// TypeInfo:  0x0000000142BAD860
struct VeniceNetworkBoostUpdateMessage
{
    char _0x0000[88];
};//Size=0x0058

////////////////////////////////////////
// RuntimeId: 00723
// TypeInfo:  0x0000000142BAD8A0
struct VeniceNetworkUnlockGainedMessage
{
    char _0x0000[120];
};//Size=0x0078

////////////////////////////////////////
// RuntimeId: 00720
// TypeInfo:  0x0000000142BAD8E0
struct VeniceNetworkAwardGainedMessage
{
    char _0x0000[112];
};//Size=0x0070

////////////////////////////////////////
// RuntimeId: 00719
// TypeInfo:  0x0000000142BAD900
struct VeniceNetworkSetSquadPrivacyMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00718
// TypeInfo:  0x0000000142BAD920
struct VeniceNetworkLeaveSquadMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00717
// TypeInfo:  0x0000000142BAD940
struct VeniceNetworkKickFromSquadMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00716
// TypeInfo:  0x0000000142BAD960
struct VeniceNetworkCreateSquadMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00715
// TypeInfo:  0x0000000142BAD980
struct VeniceNetworkJoinSquadByOnlineIdMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00714
// TypeInfo:  0x0000000142BAD9A0
struct VeniceNetworkJoinSquadMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 00713
// TypeInfo:  0x0000000142BAD9C0
struct VeniceNetworkOrderMessage
{
    char _0x0000[104];
};//Size=0x0068

////////////////////////////////////////
// RuntimeId: 00712
// TypeInfo:  0x0000000142BAD9E0
struct VeniceNetworkFocusPointMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 00711
// TypeInfo:  0x0000000142BADA00
struct VeniceNetworkSpottingOverheatMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00710
// TypeInfo:  0x0000000142BADA20
struct VeniceNetworkSpottingMessage
{
    char _0x0000[232];
};//Size=0x00E8

////////////////////////////////////////
// RuntimeId: 00709
// TypeInfo:  0x0000000142BADA40
struct StaticModelDamagedPartByPlayerMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00708
// TypeInfo:  0x0000000142BADA60
struct StaticModelAllPartsDisabledMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00707
// TypeInfo:  0x0000000142BADA80
struct BallisticShieldCreatedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00706
// TypeInfo:  0x0000000142BADAA0
struct EmblemTextureFinishedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00705
// TypeInfo:  0x0000000142BADAC0
struct ServerScreenshotReceivedMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00704
// TypeInfo:  0x0000000142BADAE0
struct FieldUpgradesLevelUpPlayerMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00703
// TypeInfo:  0x0000000142BADB00
struct PushNotificationChallengeActiveDeltaMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00702
// TypeInfo:  0x0000000142BADB20
struct PushNotificationPresencePlayingDeltaMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00701
// TypeInfo:  0x0000000142BADB40
struct PushNotificationPresentMessageBase
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00698
// TypeInfo:  0x0000000142BADB80
struct GameControlJoinServerMessageBase
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00697
// TypeInfo:  0x0000000142BADBA0
struct ESportsMatchManagerDelayedTerminationMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00696
// TypeInfo:  0x0000000142BADBC0
struct ESportsMatchManagerTooFewPlayersMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00695
// TypeInfo:  0x0000000142BADBE0
struct VeniceCommonMetricsTriggerMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00694
// TypeInfo:  0x0000000142BADC00
struct ServerUnlockSystemSetupMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00691
// TypeInfo:  0x0000000142BADC40
struct ServerMapSequencerLevelLoadedMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00690
// TypeInfo:  0x0000000142BADC60
struct ServerMapSequencerSwitchingLevelsMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00689
// TypeInfo:  0x0000000142BADC80
struct ServerMapSequencerControlledInfoMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00688
// TypeInfo:  0x0000000142BADCA0
struct ServerMapSequencerMaxPlayerCountMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00687
// TypeInfo:  0x0000000142BADCC0
struct ServerMapSequencerEventMessageBase
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00684
// TypeInfo:  0x0000000142BADD00
struct ClientPersistenceAwardGainedMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00683
// TypeInfo:  0x0000000142BADD20
struct VoiceSetPushToTalkStateMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00682
// TypeInfo:  0x0000000142BADD40
struct VoiceChangedMicEnabledMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00681
// TypeInfo:  0x0000000142BADD60
struct VoiceSetMuteStatusMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00680
// TypeInfo:  0x0000000142BADD80
struct VoiceSettingsRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00679
// TypeInfo:  0x0000000142BADDA0
struct VoiceSettingsMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00678
// TypeInfo:  0x0000000142BADDC0
struct VoiceChannelRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00677
// TypeInfo:  0x0000000142BADDE0
struct VoiceChannelMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00676
// TypeInfo:  0x0000000142BADE00
struct UIMemoryCardDestroyedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00675
// TypeInfo:  0x0000000142BADE20
struct UIMemoryCardCreatedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00674
// TypeInfo:  0x0000000142BADE40
struct UIIOStatusMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00671
// TypeInfo:  0x0000000142BADE80
struct UINPXTooltipMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00670
// TypeInfo:  0x0000000142BADEA0
struct UITooltipMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00667
// TypeInfo:  0x0000000142BADEE0
struct UISelectedWeaponMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00666
// TypeInfo:  0x0000000142BADF00
struct UIClearWeaponsListMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00665
// TypeInfo:  0x0000000142BADF20
struct UISelectedVisualUnlocksMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00664
// TypeInfo:  0x0000000142BADF40
struct UIReceivedCustomizationFromBlazeMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00663
// TypeInfo:  0x0000000142BADF60
struct UIRefreshCustomizationSelectionsMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00662
// TypeInfo:  0x0000000142BADF80
struct UIPreEndOfRoundReadyMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00661
// TypeInfo:  0x0000000142BADFA0
struct UIEndOfRoundReadyMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00660
// TypeInfo:  0x0000000142BADFC0
struct UIRequestEndOfRoundMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00659
// TypeInfo:  0x0000000142BADFE0
struct UIRequestPreEndOfRoundMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00658
// TypeInfo:  0x0000000142BAE000
struct UILoadingProgressHideMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00657
// TypeInfo:  0x0000000142BAE020
struct UILoadingProgressShowMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00656
// TypeInfo:  0x0000000142BAE040
struct UIEnterFrontendMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00655
// TypeInfo:  0x0000000142BAE060
struct UIControlpointChangedTeamMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00654
// TypeInfo:  0x0000000142BAE080
struct UIPlayerVehicleListRefreshMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00653
// TypeInfo:  0x0000000142BAE0A0
struct UIHudTicketBleedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00652
// TypeInfo:  0x0000000142BAE0C0
struct UIHudSquadLevelChangeMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00651
// TypeInfo:  0x0000000142BAE0E0
struct UIHudLastManStandingMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00650
// TypeInfo:  0x0000000142BAE100
struct UIHudResetWinningTeamMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00649
// TypeInfo:  0x0000000142BAE120
struct UITrackedAwardChangedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00648
// TypeInfo:  0x0000000142BAE140
struct UITrackedAwardUpdatedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00647
// TypeInfo:  0x0000000142BAE160
struct UIHudReviveWeaponEquippedChangedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00646
// TypeInfo:  0x0000000142BAE180
struct UIHudShowEnemiesMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 00645
// TypeInfo:  0x0000000142BAE1A0
struct UIHudShowCaptureProgressMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00644
// TypeInfo:  0x0000000142BAE1C0
struct UIHudEnableMinimapMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00643
// TypeInfo:  0x0000000142BAE1E0
struct UIShowScoreboardMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00642
// TypeInfo:  0x0000000142BAE200
struct UIShowMinimapMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00641
// TypeInfo:  0x0000000142BAE220
struct ServerSoldierSuppressionSuppressMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00640
// TypeInfo:  0x0000000142BAE240
struct UIMinimapObjectiveEventMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00639
// TypeInfo:  0x0000000142BAE260
struct UIObjectiveEventMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00636
// TypeInfo:  0x0000000142BAE2A0
struct ClientGameplayPlayerBaseDestroyedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00635
// TypeInfo:  0x0000000142BAE2C0
struct ServerGameplayChainlinkDoubleBreakMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00634
// TypeInfo:  0x0000000142BAE2E0
struct ServerGameplayChainlinkDoubleLinkMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00633
// TypeInfo:  0x0000000142BAE300
struct ServerGameplayChainlinkLinkBrokenMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00632
// TypeInfo:  0x0000000142BAE320
struct ServerGameplayChainlinkLinkCreatedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00631
// TypeInfo:  0x0000000142BAE340
struct ServerGameplayCTFFlagReturnedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00630
// TypeInfo:  0x0000000142BAE360
struct ServerGameplayCTFFlagCapturedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00629
// TypeInfo:  0x0000000142BAE380
struct ServerGameplayCTFFlagDroppedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00628
// TypeInfo:  0x0000000142BAE3A0
struct ServerGameplayCTFFlagPickedUpMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00627
// TypeInfo:  0x0000000142BAE3C0
struct ServerGameplayTeamTakeLeadMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00626
// TypeInfo:  0x0000000142BAE3E0
struct ServerGameplayTeamLostLeadMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00625
// TypeInfo:  0x0000000142BAE400
struct ServerGameplayPlayerBaseDestroyedMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00624
// TypeInfo:  0x0000000142BAE420
struct ServerGameplayTeamReinforceMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00623
// TypeInfo:  0x0000000142BAE440
struct ServerGameplayCapturePointPlayerEnteredMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00622
// TypeInfo:  0x0000000142BAE460
struct ServerGameplayCapturePointResetMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00621
// TypeInfo:  0x0000000142BAE480
struct ServerGameplayCapturePointLostMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00620
// TypeInfo:  0x0000000142BAE4A0
struct ServerGameplayCapturePointCapturedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00619
// TypeInfo:  0x0000000142BAE4C0
struct ServerGameplayCapturePointChangedMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00618
// TypeInfo:  0x0000000142BAE4E0
struct ServerPlayerTrialAccountOnUnofficialKickMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00617
// TypeInfo:  0x0000000142BAE500
struct ServerPlayerRankTooHighKickMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00616
// TypeInfo:  0x0000000142BAE520
struct ServerPlayerBoostUpdateMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00615
// TypeInfo:  0x0000000142BAE540
struct ServerPlayerConsumableConsumedMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00614
// TypeInfo:  0x0000000142BAE560
struct ServerPlayerFieldUpgradeUpdateMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00613
// TypeInfo:  0x0000000142BAE580
struct ServerPlayerCommanderOrderReplyMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00612
// TypeInfo:  0x0000000142BAE5A0
struct ServerPlayerNewCommanderOrderMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 00611
// TypeInfo:  0x0000000142BAE5C0
struct ServerPlayerLicensesAvailableMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00610
// TypeInfo:  0x0000000142BAE5E0
struct ServerPlayerManDownRevivedMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00609
// TypeInfo:  0x0000000142BAE600
struct ServerPlayerMeleeInteruptedMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00608
// TypeInfo:  0x0000000142BAE620
struct ServerPlayerTeamKillKickMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00607
// TypeInfo:  0x0000000142BAE640
struct ServerPlayerNoInteractivityKickMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00606
// TypeInfo:  0x0000000142BAE660
struct ServerPlayerSpawnOnSelectedSpawnPointMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00605
// TypeInfo:  0x0000000142BAE680
struct ServerPlayerSpawnAtVehicleMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00604
// TypeInfo:  0x0000000142BAE6A0
struct ServerPlayerSpawnOnPlayerMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00603
// TypeInfo:  0x0000000142BAE6C0
struct ServerPlayerSquadLeaderStatusChangedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00602
// TypeInfo:  0x0000000142BAE6E0
struct ServerPlayerChangedSquadSpawnerStatusMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00601
// TypeInfo:  0x0000000142BAE700
struct ServerPlayerChangedVoiceChannelMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00600
// TypeInfo:  0x0000000142BAE720
struct ServerPlayerSwitchSquadMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00599
// TypeInfo:  0x0000000142BAE740
struct ClientPlayerMeleeCounterWindowEndsMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00598
// TypeInfo:  0x0000000142BAE760
struct ClientPlayerMeleeCounterWindowStartsMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00597
// TypeInfo:  0x0000000142BAE780
struct ClientPlayerChangedMatchReadyStatusMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00596
// TypeInfo:  0x0000000142BAE7A0
struct ClientPlayerVoiceSlotMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00595
// TypeInfo:  0x0000000142BAE7C0
struct ClientPlayerChangedVoiceChannelMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00594
// TypeInfo:  0x0000000142BAE7E0
struct ClientPlayerChangedAllowedToSpawnOnMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00593
// TypeInfo:  0x0000000142BAE800
struct ClientPlayerChangedSquadLeaderStatusMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00592
// TypeInfo:  0x0000000142BAE820
struct ClientPlayerNewSquadOrderMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00591
// TypeInfo:  0x0000000142BAE840
struct ClientPlayerSwitchSquadMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00590
// TypeInfo:  0x0000000142BAE860
struct ServerAutoBalanceSettingsChangedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00589
// TypeInfo:  0x0000000142BAE880
struct ServerResendSyncedSettingsMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00588
// TypeInfo:  0x0000000142BAE8A0
struct ServerRestartForESportsMatchMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00587
// TypeInfo:  0x0000000142BAE8C0
struct ServerRotateLevelMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 00586
// TypeInfo:  0x0000000142BAE8E0
struct CommanderActivateCameraMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00585
// TypeInfo:  0x0000000142BAE900
struct CommanderAwardCommanderScoreMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00584
// TypeInfo:  0x0000000142BAE920
struct CommanderAwardCommanderSquadScoreMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00583
// TypeInfo:  0x0000000142BAE940
struct CommanderNetworkEMPDroneDeployedMessage
{
    char _0x0000[88];
};//Size=0x0058

////////////////////////////////////////
// RuntimeId: 00582
// TypeInfo:  0x0000000142BAE960
struct CommanderNetworkUAVDroneDeployedMessage
{
    char _0x0000[88];
};//Size=0x0058

////////////////////////////////////////
// RuntimeId: 00581
// TypeInfo:  0x0000000142BAE980
struct CommanderNetworkGunshipDeployedMessage
{
    char _0x0000[88];
};//Size=0x0058

////////////////////////////////////////
// RuntimeId: 00580
// TypeInfo:  0x0000000142BAE9A0
struct CommanderNetworkEarlyWarningEndedMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00579
// TypeInfo:  0x0000000142BAE9C0
struct CommanderNetworkEarlyWarningStartedMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 00578
// TypeInfo:  0x0000000142BAE9E0
struct CommanderNetworkRadarScanStartedMessage
{
    char _0x0000[128];
};//Size=0x0080

////////////////////////////////////////
// RuntimeId: 00577
// TypeInfo:  0x0000000142BAEA00
struct CommanderNetworkTomahawkLaunchedMessage
{
    char _0x0000[88];
};//Size=0x0058

////////////////////////////////////////
// RuntimeId: 00576
// TypeInfo:  0x0000000142BAEA20
struct CommanderNetworkSatelliteEMPEndedMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00575
// TypeInfo:  0x0000000142BAEA40
struct CommanderNetworkSatelliteEMPStartedMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00574
// TypeInfo:  0x0000000142BAEA60
struct CommanderNetworkJamEndedMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00573
// TypeInfo:  0x0000000142BAEA80
struct CommanderNetworkJamStartedMessage
{
    char _0x0000[112];
};//Size=0x0070

////////////////////////////////////////
// RuntimeId: 00572
// TypeInfo:  0x0000000142BAEAA0
struct CommanderNetworkScanEndedMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00571
// TypeInfo:  0x0000000142BAEAC0
struct CommanderNetworkScanStartedMessage
{
    char _0x0000[112];
};//Size=0x0070

////////////////////////////////////////
// RuntimeId: 00570
// TypeInfo:  0x0000000142BAEAE0
struct CommanderNetworkOrderReplyMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00569
// TypeInfo:  0x0000000142BAEB00
struct CommanderNetworkRemoveOrderMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00568
// TypeInfo:  0x0000000142BAEB20
struct CommanderNetworkIssueOrderMessage
{
    char _0x0000[128];
};//Size=0x0080

////////////////////////////////////////
// RuntimeId: 00567
// TypeInfo:  0x0000000142BAEB40
struct CommanderNetworkAbortActionMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00566
// TypeInfo:  0x0000000142BAEB60
struct CommanderNetworkSelectedSquadMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00565
// TypeInfo:  0x0000000142BAEB80
struct CommanderNetworkTriggerOrderReplyMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00564
// TypeInfo:  0x0000000142BAEBA0
struct CommanderNetworkTriggerRequestMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00563
// TypeInfo:  0x0000000142BAEBC0
struct CommanderNetworkTriggerActionMessage
{
    char _0x0000[144];
};//Size=0x0090

////////////////////////////////////////
// RuntimeId: 00562
// TypeInfo:  0x0000000142BAEBE0
struct CommanderNetworkLeaveSeatMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00561
// TypeInfo:  0x0000000142BAEC00
struct CommanderNetworkApplyForSeatMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00560
// TypeInfo:  0x0000000142BAEC20
struct CommanderNetworkSquadReinforcedMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00559
// TypeInfo:  0x0000000142BAEC40
struct CommanderNetworkMutinyAbortedMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00558
// TypeInfo:  0x0000000142BAEC60
struct CommanderNetworkMutinyCompletedMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00557
// TypeInfo:  0x0000000142BAEC80
struct CommanderNetworkMutinyStartedMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00556
// TypeInfo:  0x0000000142BAECA0
struct CommanderNetworkSquadLeaderRateCommanderMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 00555
// TypeInfo:  0x0000000142BAECC0
struct CommanderNetworkRemoveRequestFromSquadMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 00554
// TypeInfo:  0x0000000142BAECE0
struct CommanderNetworkAddRequestFromSquadMessage
{
    char _0x0000[96];
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 00553
// TypeInfo:  0x0000000142BAED00
struct CommanderNetworkRequestActionFromCommanderMessage
{
    char _0x0000[112];
};//Size=0x0070

////////////////////////////////////////
// RuntimeId: 00552
// TypeInfo:  0x0000000142BAED20
struct CommanderActionsActionStateChangedMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00551
// TypeInfo:  0x0000000142BAED40
struct CommanderSquadChangedMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00550
// TypeInfo:  0x0000000142BAED60
struct CommanderCommanderChangedMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00545
// TypeInfo:  0x0000000142BAF180
struct GunMasterLevelInfo
{
    CustomizeSoldierData* m_CustomizeData; //0x0000
    __int32 m_KillsNeeded; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 00547
// TypeInfo:  0x0000000142BAF160
struct GunMasterWeaponsPreset
{
    Array<GunMasterLevelInfo> m_GunMasterLevelInfos; //0x0000
    Array<CustomizeSoldierData*> m_LevelOverrides; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 00479
// TypeInfo:  0x0000000142BAF1A0
struct MessageLineData
{
    char* m_MessageSid; //0x0000
    __int32 m_EntryInputAction; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 00461
// TypeInfo:  0x0000000142BAEDA0
struct AimingConstraintEntityBinding
{
    AntRef m_ProneAimLimit; //0x0000
    AntRef m_ProneAimDistance; //0x0014
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00453
// TypeInfo:  0x0000000142BAEDC0
struct CharacterMeleeBinding
{
    AntRef m_TriggerMelee; //0x0000
    AntRef m_DefendMelee; //0x0014
    AntRef m_MeleeKill; //0x0028
    AntRef m_SelfActor; //0x003C
    AntRef m_OtherActor; //0x0050
    AntRef m_MeleeActive; //0x0064
};//Size=0x0078

////////////////////////////////////////
// RuntimeId: 00451
// TypeInfo:  0x0000000142BAEDE0
struct CharacterMeleeCommonBinding
{
    AntRef m_MeleeDistance; //0x0000
    AntRef m_MeleeAngle; //0x0014
    AntRef m_MeleeDirection; //0x0028
    AntRef m_DefendEnabled; //0x003C
    AntRef m_MeleeDefenceTime; //0x0050
    AntRef m_MeleeSequenceTime; //0x0064
    AntRef m_MeleeAborted; //0x0078
    AntRef m_IsTimeToKill; //0x008C
    AntRef m_CloseToTarget; //0x00A0
    AntRef m_MeleeType; //0x00B4
};//Size=0x00C8

////////////////////////////////////////
// RuntimeId: 00433
// TypeInfo:  0x0000000142BAEE00
struct KillWeight
{
    float m_LowIntenseWeight; //0x0000
    float m_HighIntenseWeight; //0x0004
    float m_CoolDownTime; //0x0008
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 00379
// TypeInfo:  0x0000000142BAF1C0
struct LookAtHuman
{
    AntSpeedLevel m_SpeedLevel; //0x0000
    float m_DelayMin; //0x0004
    float m_DelayMax; //0x0008
    float m_LengthMin; //0x000C
    float m_LengthMax; //0x0010
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 00361
// TypeInfo:  0x0000000142BAF1E0
struct WeaponInteractionData
{
    SoldierWeaponUnlockAsset* m_WeaponToInteractWith; //0x0000
    bool m_OnlyManDownTargets; //0x0008
    char _0x0009[7];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 00333
// TypeInfo:  0x0000000142BAEE20
struct SupportedShootingProximityChecking
{
    float m_RayAboveGunHeightOffset; //0x0000
    float m_RayAboveLength; //0x0004
    float m_RayBelowGunHeightOffset; //0x0008
    float m_RayBelowLength; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 00323
// TypeInfo:  0x0000000142BAF220
struct SpecialMoveInterpolatedPoint
{
    Vec3 m_Value; //0x0000
    float m_Time; //0x0010
    char _0x0014[12];
};//Size=0x0020

////////////////////////////////////////
// RuntimeId: 00327
// TypeInfo:  0x0000000142BAEE40
struct SpecialMoveStateData
{
    float m_OptimumDistanceFromObject; //0x0000
    float m_OptimumHeightOfObject; //0x0004
    float m_OptimumLengthOfObject; //0x0008
    float m_MaxDistanceScale; //0x000C
    float m_MinDistanceScale; //0x0010
    float m_MaxHeightScale; //0x0014
    float m_MinHeightScale; //0x0018
    float m_MaxObjectLengthScale; //0x001C
    float m_MinObjectLengthScale; //0x0020
    float m_ExitAnimationDrivenStateVelocity; //0x0024
    SpecialMovesPoseType m_OverridePose; //0x0028
    SpecialMovesPoseType m_ResetPose; //0x002C
    Array<SpecialMoveInterpolatedPoint> m_GravityPoints; //0x0030
    Array<SpecialMoveInterpolatedPoint> m_MotionPoints; //0x0038
    bool m_ScaleWithDistanceToObject; //0x0040
    bool m_KeepInitialSoldierDirection; //0x0041
    char _0x0042[6];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00329
// TypeInfo:  0x0000000142BAF200
struct SpecialMoveVaultStateCriteria
{
    float m_DistToObject; //0x0000
    float m_DistToObjectTolerance; //0x0004
    float m_HeightOfObject; //0x0008
    float m_HeightOfObjectTolerance; //0x000C
    float m_LengthOfObject; //0x0010
    float m_LengthOfObjectTolerance; //0x0014
    SpecialMoveStateData m_SpecialMoveState; //0x0018
};//Size=0x0060

////////////////////////////////////////
// RuntimeId: 00317
// TypeInfo:  0x0000000142BAEE60
struct AirMissileJammingData
{
    float m_MaxRadius; //0x0000
    float m_MinRadius; //0x0004
    float m_Theta; //0x0008
    float m_Phi; //0x000C
    float m_MinTurnSpeed; //0x0010
    float m_MaxTurnSpeed; //0x0014
    float m_QuietModeProbability; //0x0018
    float m_QuietModeTime; //0x001C
    float m_JammingTime; //0x0020
    float m_TurnCalculationTime; //0x0024
    float m_UpdateJammingPositionTime; //0x0028
    bool m_EnableAirMissileJamming; //0x002C
    char _0x002D[3];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00315
// TypeInfo:  0x0000000142BAEE80
struct SmokeJammingData
{
    float m_MaxRadius; //0x0000
    float m_MinRadius; //0x0004
    float m_JammingTime; //0x0008
    float m_TargetUpdateTime; //0x000C
    bool m_EnableSmokeJamming; //0x0010
    bool m_JammingPositionIsLocationDependent; //0x0011
    bool m_ReportJammedBasedOnStartPosition; //0x0012
    char _0x0013[1];
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 00311
// TypeInfo:  0x0000000142BAEEA0
struct Vault3pOnlyBinding
{
    AntRef m_IsVaulting; //0x0000
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 00309
// TypeInfo:  0x0000000142BAEEC0
struct Vault1pOnlyBinding
{
    AntRef m_IsVaulting; //0x0000
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 00307
// TypeInfo:  0x0000000142BAEEE0
struct VaultBinding
{
    AntRef m_VaultActive; //0x0000
    AntRef m_VaultCancel; //0x0014
    AntRef m_VaultDepthDistance; //0x0028
    AntRef m_VaultEndHeight; //0x003C
    AntRef m_VaultStartDistance; //0x0050
    AntRef m_VaultStartHeight; //0x0064
    AntRef m_VaultType; //0x0078
};//Size=0x008C

////////////////////////////////////////
// RuntimeId: 00301
// TypeInfo:  0x0000000142BAEF00
struct AwarenessTargetInfo
{
    float m_Priority; //0x0000
    AwarenessTargetType m_TargetType; //0x0004
    float m_TargetRange; //0x0008
    bool m_MustBeInsideFrustum; //0x000C
    char _0x000D[3];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 00297
// TypeInfo:  0x0000000142BAEF20
struct AwarenessComponentBinding
{
    AntRef m_RelativeLookAtPitch; //0x0000
    AntRef m_RelativeLookAtYaw; //0x0014
    AntRef m_HasAwarenessTarget; //0x0028
    AntRef m_ExplosionNearby; //0x003C
    AntRef m_ExplosionDirection; //0x0050
    AntRef m_NearbyThreat; //0x0064
    AntRef m_NearbyThreatDirection; //0x0078
    AntRef m_AmbientThreat; //0x008C
};//Size=0x00A0

////////////////////////////////////////
// RuntimeId: 00295
// TypeInfo:  0x0000000142BAF240
struct TabletCommanderClientHasLoadedLevelMessage
{
    bool m_Loaded; //0x0000
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 00293
// TypeInfo:  0x0000000142BAF260
struct TabletCommanderSelectedSquadChangedMessage
{
    SquadId m_SelectedSquad; //0x0000
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 00291
// TypeInfo:  0x0000000142BAF280
struct TabletCommanderAbortActionMessage
{
    unsigned __int8 m_ActionType; //0x0000
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 00289
// TypeInfo:  0x0000000142BAF2A0
struct TabletCommanderTriggerActionMessage
{
    unsigned __int8 m_ActionType; //0x0000
    char _0x0001[3];
    unsigned __int32 m_PlayerId; //0x0004
    unsigned __int32 m_MapMarkerId; //0x0008
    unsigned __int16 m_X; //0x000C
    unsigned __int16 m_Y; //0x000E
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 00287
// TypeInfo:  0x0000000142BAF2C0
struct TabletCommanderPingMessage
{
    float m_PingTime; //0x0000
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 00285
// TypeInfo:  0x0000000142BAF2E0
struct TabletCommanderTryConnectMessage
{
    unsigned __int32 m_TCClientProtocolVersion; //0x0000
    char _0x0004[4];
    unsigned __int64 m_PlayerId; //0x0008
    char* m_PlayerName; //0x0010
    TabletCommanderPlatform m_Platform; //0x0018
    char _0x001C[4];
    char* m_DeviceModel; //0x0020
    char* m_DeviceOsVersion; //0x0028
    char* m_DeviceNetworkConnection; //0x0030
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00283
// TypeInfo:  0x0000000142BAF300
struct TabletCommanderCarrierStateMessage
{
    unsigned __int8 m_Team1CarrierHealth; //0x0000
    unsigned __int8 m_Team2CarrierHealth; //0x0001
    unsigned __int8 m_Team1CarrierState; //0x0002
    unsigned __int8 m_Team2CarrierState; //0x0003
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 00281
// TypeInfo:  0x0000000142BAF320
struct TabletCommanderChatMessage
{
    char* m_ChatMessage; //0x0000
    unsigned __int32 m_SenderPlayerId; //0x0008
    TabletCommanderChatChannel m_Channel; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 00279
// TypeInfo:  0x0000000142BAF340
struct TabletCommanderEnemyCommanderChangedMessage
{
    char* m_CommanderName; //0x0000
    char* m_CommanderEmblem; //0x0008
    unsigned __int32 m_CommanderPlayerId; //0x0010
    char _0x0014[4];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 00277
// TypeInfo:  0x0000000142BAF360
struct TabletCommanderMutinyCompletedMessage
{
    bool m_Dummy; //0x0000
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 00275
// TypeInfo:  0x0000000142BAF380
struct TabletCommanderRankGainedMessage
{
    unsigned __int8 m_OldRank; //0x0000
    unsigned __int8 m_NewRank; //0x0001
};//Size=0x0002

////////////////////////////////////////
// RuntimeId: 00193
// TypeInfo:  0x0000000142BAEFC0
struct TabletCommanderAwardData
{
    char* m_AwardCode; //0x0000
    char* m_Name; //0x0008
    char* m_Description; //0x0010
    char* m_ImageName; //0x0018
    float m_Score; //0x0020
    unsigned __int32 m_TimesTakenCount; //0x0024
    unsigned __int8 m_Group; //0x0028
    char _0x0029[7];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00191
// TypeInfo:  0x0000000142BAF820
struct TabletCommanderScoreBoardData
{
    unsigned __int32 m_Id; //0x0000
    unsigned __int32 m_Score; //0x0004
    unsigned __int16 m_Kills; //0x0008
    unsigned __int16 m_Deaths; //0x000A
    unsigned __int16 m_Rank; //0x000C
    char _0x000E[2];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 00195
// TypeInfo:  0x0000000142BAEFA0
struct TabletCommanderAwardCriteria
{
    char* m_Description; //0x0000
    float m_CompletionValue; //0x0008
    float m_OriginalValue; //0x000C
    float m_AbsoluteValue; //0x0010
    char _0x0014[4];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 00201
// TypeInfo:  0x0000000142BAF7E0
struct TabletCommanderRankLevelData
{
    unsigned __int32 m_RankPointsNeeded; //0x0000
    unsigned __int8 m_RankNumber; //0x0004
    char _0x0005[3];
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 00265
// TypeInfo:  0x0000000142BAF420
struct TabletCommanderPlayerLeftMessage
{
    unsigned __int32 m_PlayerId; //0x0000
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 00185
// TypeInfo:  0x0000000142BAF000
struct TabletCommanderPlayerData
{
    unsigned __int32 m_Id; //0x0000
    char _0x0004[4];
    char* m_Name; //0x0008
    unsigned __int8 m_VoiceSlot; //0x0010
    char _0x0011[7];
    unsigned __int64 m_XboxUserId; //0x0018
    TeamId m_TeamId; //0x0020
    SquadId m_SquadId; //0x0024
    unsigned __int8 m_SoldierKit; //0x0028
    bool m_IsSquadLeader; //0x0029
    bool m_IsAlive; //0x002A
    char _0x002B[5];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00183
// TypeInfo:  0x0000000142BAF020
struct TabletCommanderLevolutionState
{
    LevolutionType m_LevolutionType; //0x0000
    char _0x0004[4];
    char* m_LevolutionId; //0x0008
    unsigned __int16 m_X; //0x0010
    unsigned __int16 m_Y; //0x0012
    bool m_Activated; //0x0014
    char _0x0015[3];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 00259
// TypeInfo:  0x0000000142BAF480
struct TabletCommanderScoringEventMessage
{
    char* m_Description; //0x0000
    float m_Score; //0x0008
    float m_RawScore; //0x000C
    unsigned __int8 m_StatEvent; //0x0010
    unsigned __int8 m_VisibilityType; //0x0011
    bool m_IsEnemyCommander; //0x0012
    char _0x0013[5];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 00257
// TypeInfo:  0x0000000142BAF4A0
struct TabletCommanderPlayerKilledMessage
{
    unsigned __int32 m_KillerId; //0x0000
    unsigned __int32 m_VictimId; //0x0004
    char* m_WeaponName; //0x0008
    bool m_KilledByVehicle; //0x0010
    char _0x0011[7];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 00255
// TypeInfo:  0x0000000142BAF4C0
struct TabletCommanderDeniedActionMessage
{
    unsigned __int8 m_ActionType; //0x0000
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 00253
// TypeInfo:  0x0000000142BAF4E0
struct TabletCommanderAcknowledgedActionMessage
{
    unsigned __int8 m_ActionType; //0x0000
};//Size=0x0001

////////////////////////////////////////
// RuntimeId: 00187
// TypeInfo:  0x0000000142BAEFE0
struct TabletCommanderActionState
{
    unsigned __int8 m_ActionType; //0x0000
    unsigned __int8 m_ActionState; //0x0001
    char _0x0002[2];
    TeamId m_TeamId; //0x0004
    float m_StateDuration; //0x0008
    float m_Radius; //0x000C
    unsigned __int16 m_TargetX; //0x0010
    unsigned __int16 m_TargetY; //0x0012
    bool m_IsExcluded; //0x0014
    char _0x0015[3];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 00189
// TypeInfo:  0x0000000142BAF840
struct TabletCommanderCombatAreaPoint
{
    unsigned __int16 m_X; //0x0000
    unsigned __int16 m_Y; //0x0002
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 00247
// TypeInfo:  0x0000000142BAF540
struct TabletCommanderCommanderSwitchedTeamMessage
{
    TeamId m_TeamId; //0x0000
    TeamId m_EnemyTeamId; //0x0004
    __int32 m_Faction; //0x0008
    __int32 m_EnemyFaction; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 00245
// TypeInfo:  0x0000000142BAF560
struct TabletCommanderLoadLevelRushMessage
{
    unsigned __int8 m_StartCratesCount; //0x0000
    unsigned __int8 m_CurrentCratesCount; //0x0001
    bool m_IsAttacker; //0x0002
};//Size=0x0003

////////////////////////////////////////
// RuntimeId: 00243
// TypeInfo:  0x0000000142BAF580
struct TabletCommanderLoadLevelSquadBarMessage
{
    Array<unsigned __int8> m_ActionTypes; //0x0000
    unsigned __int32 m_PointsPerSegment; //0x0008
    char _0x000C[4];
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 00241
// TypeInfo:  0x0000000142BAF5A0
struct TabletCommanderLoadLevelLevolutionStateMessage
{
    Array<TabletCommanderLevolutionState> m_LevolutionStates; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 00239
// TypeInfo:  0x0000000142BAF5C0
struct TabletCommanderLoadLevelActionStateMessage
{
    Array<TabletCommanderActionState> m_ActionStates; //0x0000
    RadarScanMode m_InfantryScanDir; //0x0008
    RadarScanMode m_VehicleScanDir; //0x000C
    unsigned __int32 m_EarlyWarningDuration; //0x0010
    char _0x0014[4];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 00237
// TypeInfo:  0x0000000142BAF5E0
struct TabletCommanderLoadLevelPlayerDataMessage
{
    Array<TabletCommanderPlayerData> m_PlayerData; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 00209
// TypeInfo:  0x0000000142BAF7A0
struct TabletCommanderLevolutionMiniMap
{
    char* m_Tag; //0x0000
    float m_Size; //0x0008
    unsigned __int16 m_X; //0x000C
    unsigned __int16 m_Y; //0x000E
    bool m_IsPatch; //0x0010
    char _0x0011[7];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 00233
// TypeInfo:  0x0000000142BAF620
struct TabletCommanderJoinServerMessage
{
    char* m_ServerName; //0x0000
    char* m_ServerVersion; //0x0008
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 00231
// TypeInfo:  0x0000000142BAF640
struct TabletCommanderPongMessage
{
    float m_PingTime; //0x0000
    float m_PongTime; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 00229
// TypeInfo:  0x0000000142BAF660
struct TabletCommanderConnectionDeniedMessage
{
    unsigned __int32 m_TCServerProtocolVersion; //0x0000
    TabletCommanderConnectionDeniedReason m_DeniedReason; //0x0004
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 00227
// TypeInfo:  0x0000000142BAF680
struct TabletCommanderConnectionApprovedMessage
{
    unsigned __int32 m_TCServerProtocolVersion; //0x0000
};//Size=0x0004

////////////////////////////////////////
// RuntimeId: 00171
// TypeInfo:  0x0000000142BAF080
struct TabletCommanderPacketBase
{
    TabletCommanderPacketType m_PacketType; //0x0000
    float m_ServerTime; //0x0004
    unsigned __int8 m_PacketFrequency; //0x0008
    char _0x0009[3];
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 00223
// TypeInfo:  0x0000000142BAF6C0
struct TabletCommanderSquadRequest
{
    SquadId m_SquadId; //0x0000
    unsigned __int8 m_ActionType; //0x0004
    char _0x0005[3];
    unsigned __int32 m_ControlPointId; //0x0008
    unsigned __int16 m_TargetX; //0x000C
    unsigned __int16 m_TargetY; //0x000E
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 00221
// TypeInfo:  0x0000000142BAF6E0
struct TabletCommanderOrder
{
    unsigned __int8 m_OrderType; //0x0000
    unsigned __int8 m_ReplyType; //0x0001
    char _0x0002[2];
    unsigned __int32 m_IssuedToPlayerId; //0x0004
    unsigned __int32 m_ControlPointId; //0x0008
    unsigned __int32 m_ControllableId; //0x000C
    unsigned __int16 m_TargetX; //0x0010
    unsigned __int16 m_TargetY; //0x0012
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 00219
// TypeInfo:  0x0000000142BAF700
struct TabletCommanderMapMarker
{
    unsigned __int32 m_MapMarkerId; //0x0000
    unsigned __int16 m_X; //0x0004
    unsigned __int16 m_Y; //0x0006
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 00217
// TypeInfo:  0x0000000142BAF720
struct TabletCommanderObjective
{
    char* m_ObjectiveName; //0x0000
    char* m_LocationTextSid; //0x0008
    unsigned __int32 m_ObjectiveId; //0x0010
    TabletCommanderObjectiveType m_ObjectiveType; //0x0014
    unsigned __int8 m_ActionType; //0x0018
    char _0x0019[3];
    TeamId m_ControllingTeamId; //0x001C
    TeamId m_CapturingTeamId; //0x0020
    unsigned __int16 m_X; //0x0024
    unsigned __int16 m_Y; //0x0026
    __int16 m_ProgressVelocity; //0x0028
    unsigned __int8 m_ProgressValue; //0x002A
    bool m_Blink; //0x002B
    bool m_IsControlled; //0x002C
    char _0x002D[3];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00173
// TypeInfo:  0x0000000142BAF060
struct TabletCommanderNetworkState
{
    unsigned __int32 m_Id; //0x0000
    unsigned __int16 m_X; //0x0004
    unsigned __int16 m_Y; //0x0006
    unsigned __int8 m_Rotation; //0x0008
    char _0x0009[3];
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 00175
// TypeInfo:  0x0000000142BAF8A0
struct TabletCommanderNetworkSoldierState
{
    TabletCommanderNetworkState m_NetworkState; //0x0000
    unsigned __int32 m_InVehicleId; //0x000C
    unsigned __int8 m_SoldierKit; //0x0010
    bool m_IsSquadLeader; //0x0011
    bool m_IsOnKillStreak; //0x0012
    bool m_IsHighValueTarget; //0x0013
    bool m_IsFiring; //0x0014
    char _0x0015[3];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 00213
// TypeInfo:  0x0000000142BAF760
struct TabletCommanderHighFrequencyFriendlySquad
{
    SquadId m_SquadId; //0x0000
    float m_NormalizedSquadScore; //0x0004
    bool m_IsSquadWipeProtected; //0x0008
    bool m_IsSquadReinforced; //0x0009
    char _0x000A[6];
    Array<TabletCommanderNetworkSoldierState> m_Soldiers; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 00235
// TypeInfo:  0x0000000142BAF600
struct TabletCommanderLoadLevelMessage
{
    char* m_LevelName; //0x0000
    TabletCommanderGameMode m_GameMode; //0x0008
    char _0x000C[4];
    char* m_CommanderName; //0x0010
    char* m_CommanderEmblem; //0x0018
    unsigned __int32 m_CommanderPlayerId; //0x0020
    float m_WorldSize; //0x0024
    float m_WorldCenterX; //0x0028
    float m_WorldCenterY; //0x002C
    float m_WorldRotation; //0x0030
    unsigned __int32 m_RoundTimeLimit; //0x0034
    unsigned __int32 m_PreRoundPlayerRequiredCount; //0x0038
    bool m_RoundInPreRoundState; //0x003C
    unsigned __int8 m_MaxPlayerCount; //0x003D
    unsigned __int8 m_CurrentPlayerCount; //0x003E
    unsigned __int8 m_MaxCommanderCount; //0x003F
    unsigned __int8 m_CurrentCommanderCount; //0x0040
    char _0x0041[7];
    Array<TabletCommanderLevolutionMiniMap> m_LevolutionMiniMaps; //0x0048
    bool m_RoundEnded; //0x0050
    char _0x0051[7];
};//Size=0x0058

////////////////////////////////////////
// RuntimeId: 00207
// TypeInfo:  0x0000000142BAEF40
struct TabletCommanderMutinyState
{
    float m_MutinyTreshold; //0x0000
    unsigned __int32 m_MutinyDuration; //0x0004
    unsigned __int8 m_ActiveSquadCount; //0x0008
    unsigned __int8 m_NegativeRatingCount; //0x0009
    char _0x000A[2];
};//Size=0x000C

////////////////////////////////////////
// RuntimeId: 00205
// TypeInfo:  0x0000000142BAF7C0
struct TabletCommanderServiceStarProgression
{
    TabletCommanderAwardData m_ServiceStarAwardData; //0x0000
    TabletCommanderAwardCriteria m_Criteria; //0x0030
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00203
// TypeInfo:  0x0000000142BAEF60
struct TabletCommanderRankProgression
{
    Array<TabletCommanderRankLevelData> m_RankLevelData; //0x0000
    unsigned __int32 m_OriginalGlobalScore; //0x0008
    unsigned __int32 m_CurrentGlobalScore; //0x000C
    float m_ScoreMultiplier; //0x0010
    char _0x0014[4];
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 00267
// TypeInfo:  0x0000000142BAF400
struct TabletCommanderRoundOverMessage
{
    TeamId m_WinningTeam; //0x0000
    unsigned __int32 m_RoundTime; //0x0004
    unsigned __int32 m_EoRTime; //0x0008
    char _0x000C[4];
    TabletCommanderRankProgression m_RankProgress; //0x0010
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00197
// TypeInfo:  0x0000000142BAF800
struct TabletCommanderDogTagData
{
    char* m_Name; //0x0000
    char* m_Description; //0x0008
    char* m_ImageName; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 00199
// TypeInfo:  0x0000000142BAEF80
struct TabletCommanderAssignment
{
    TabletCommanderAwardData m_AssignmentAwardData; //0x0000
    Array<TabletCommanderAwardData> m_Dependencies; //0x0030
    Array<TabletCommanderAwardCriteria> m_Criterias; //0x0038
    Array<TabletCommanderDogTagData> m_DogTags; //0x0040
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00269
// TypeInfo:  0x0000000142BAF3E0
struct TabletCommanderAssignmentMessage
{
    TabletCommanderAssignment m_Assignment; //0x0000
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00273
// TypeInfo:  0x0000000142BAF3A0
struct TabletCommanderAwardGainedMessage
{
    TabletCommanderAwardData m_Award; //0x0000
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00271
// TypeInfo:  0x0000000142BAF3C0
struct TabletCommanderScoreBoardMessage
{
    Array<TabletCommanderScoreBoardData> m_ScoreData; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 00249
// TypeInfo:  0x0000000142BAF520
struct TabletCommanderCombatAreaMessage
{
    Array<TabletCommanderCombatAreaPoint> m_CombatAreaPoints; //0x0000
};//Size=0x0008

////////////////////////////////////////
// RuntimeId: 00251
// TypeInfo:  0x0000000142BAF500
struct TabletCommanderActionEnteredStateMessage
{
    TabletCommanderActionState m_ActionState; //0x0000
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 00263
// TypeInfo:  0x0000000142BAF440
struct TabletCommanderPlayerJoinedMessage
{
    TabletCommanderPlayerData m_PlayerData; //0x0000
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00261
// TypeInfo:  0x0000000142BAF460
struct TabletCommanderLevolutionMessage
{
    TabletCommanderLevolutionState m_LevolutionState; //0x0000
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 00181
// TypeInfo:  0x0000000142BAF040
struct TabletCommanderTickets
{
    unsigned __int32 m_FriendTickets; //0x0000
    unsigned __int32 m_FriendMaxTickets; //0x0004
    unsigned __int32 m_EnemyTickets; //0x0008
    unsigned __int32 m_EnemyMaxTickets; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 00179
// TypeInfo:  0x0000000142BAF860
struct TabletCommanderNetworkBombState
{
    TabletCommanderNetworkState m_NetworkState; //0x0000
    TeamId m_TeamId; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 00177
// TypeInfo:  0x0000000142BAF880
struct TabletCommanderNetworkVehicleState
{
    TabletCommanderNetworkState m_NetworkState; //0x0000
    unsigned __int32 m_IconType; //0x000C
    bool m_IsFiring; //0x0010
    char _0x0011[3];
};//Size=0x0014

////////////////////////////////////////
// RuntimeId: 00215
// TypeInfo:  0x0000000142BAF740
struct TabletCommanderLowFrequencyPacket
{
    TabletCommanderPacketBase m_PacketBase; //0x0000
    char _0x000C[4];
    Array<TabletCommanderNetworkSoldierState> m_FriendlyDeadSoldierList; //0x0010
    Array<TabletCommanderObjective> m_ObjectiveList; //0x0018
    Array<TabletCommanderOrder> m_OrderList; //0x0020
    Array<TabletCommanderSquadRequest> m_SquadRequestList; //0x0028
    Array<TabletCommanderMapMarker> m_MapMarkerList; //0x0030
    TabletCommanderTickets m_Tickets; //0x0038
    TabletCommanderMutinyState m_MutinyState; //0x0048
    unsigned __int32 m_RoundTimeSeconds; //0x0054
    unsigned __int32 m_SquadPoints; //0x0058
    unsigned __int32 m_PreRoundPlayerReadyCount; //0x005C
    unsigned __int32 m_PreRoundRestartTime; //0x0060
    __int32 m_ObliterationTargetsArmedBitmask; //0x0064
    __int32 m_ObliterationTargetsDestroyedBitmask; //0x0068
    unsigned __int8 m_CurrentCratesCount; //0x006C
    char _0x006D[3];
};//Size=0x0070

////////////////////////////////////////
// RuntimeId: 00211
// TypeInfo:  0x0000000142BAF780
struct TabletCommanderHighFrequencyPacket
{
    TabletCommanderPacketBase m_PacketBase; //0x0000
    char _0x000C[4];
    Array<TabletCommanderHighFrequencyFriendlySquad> m_FriendlySquadList; //0x0010
    Array<TabletCommanderNetworkVehicleState> m_FriendlyVehicleList; //0x0018
    Array<TabletCommanderNetworkState> m_FriendlyTomahawkList; //0x0020
    Array<TabletCommanderNetworkSoldierState> m_SpottedEnemySoldierList; //0x0028
    Array<TabletCommanderNetworkVehicleState> m_SpottedEnemyVehicleList; //0x0030
    Array<TabletCommanderNetworkState> m_SpottedEnemyTomahawkList; //0x0038
    Array<TabletCommanderNetworkBombState> m_ObliterationBomb; //0x0040
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00225
// TypeInfo:  0x0000000142BAF6A0
struct TabletCommanderMessageListPacket
{
    TabletCommanderPacketBase m_PacketBase; //0x0000
    char _0x000C[4];
    Array<TabletCommanderMessageType> m_MessageTypeList; //0x0010
};//Size=0x0018

////////////////////////////////////////
// RuntimeId: 00149
// TypeInfo:  0x0000000142BAF0E0
struct UavCameraPointData
{
    float m_MinTime; //0x0000
    float m_MaxTime; //0x0004
    float m_Speed; //0x0008
    float m_AccelerationTime; //0x000C
};//Size=0x0010

////////////////////////////////////////
// RuntimeId: 00133
// TypeInfo:  0x0000000142BA38D8
struct PunkBusterExecCmdMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00132
// TypeInfo:  0x0000000142BA38F8
struct PunkBusterMessageMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00131
// TypeInfo:  0x0000000142BA3918
struct ServerUnlockListRefreshMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00130
// TypeInfo:  0x0000000142BA3938
struct ServerUnlockListEventMessageBase
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00127
// TypeInfo:  0x0000000142BA3978
struct ServerReservedSlotsListAggressiveJoinVIPPlayerMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00126
// TypeInfo:  0x0000000142BA3998
struct ServerReservedSlotsListEventMessageBase
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00125
// TypeInfo:  0x0000000142BA39B8
struct ServerBanListEventMessageBase
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00124
// TypeInfo:  0x0000000142BA39D8
struct NetworkGameAdministrationAddGameBanResponseMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00121
// TypeInfo:  0x0000000142BA3A18
struct NetworkGameAdministrationAddGameBanRequestMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 00120
// TypeInfo:  0x0000000142BA3A38
struct NetworkGameAdministrationPacketMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 00119
// TypeInfo:  0x0000000142BA3A58
struct ServerGameAdministrationUpdateGameServerListsMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00118
// TypeInfo:  0x0000000142BA3A78
struct ServerGameAdministrationResetServerMessage
{
    char _0x0000[72];
};//Size=0x0048

////////////////////////////////////////
// RuntimeId: 00117
// TypeInfo:  0x0000000142BA3A98
struct ServerGameAdministrationRefreshGameAttributesMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00116
// TypeInfo:  0x0000000142BA3AB8
struct ServerGameAdministrationGameSettingsUpdateMessage
{
    char _0x0000[152];
};//Size=0x0098

////////////////////////////////////////
// RuntimeId: 00115
// TypeInfo:  0x0000000142BA3AD8
struct ServerGameAdministrationEventMessageBase
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00114
// TypeInfo:  0x0000000142BA3AF8
struct ClientPersistenceUnlockMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00113
// TypeInfo:  0x0000000142BA3B18
struct PresenceStateRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00112
// TypeInfo:  0x0000000142BA3B38
struct PresenceStateMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00111
// TypeInfo:  0x0000000142BA3B58
struct PresenceXPromoRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00110
// TypeInfo:  0x0000000142BA3B78
struct PresenceXPromoMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00109
// TypeInfo:  0x0000000142BA3B98
struct PresenceWebRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00108
// TypeInfo:  0x0000000142BA3BB8
struct PresenceWebFeedRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00107
// TypeInfo:  0x0000000142BA3BD8
struct PresenceWebAuthenticationRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00106
// TypeInfo:  0x0000000142BA3BF8
struct PresenceWebAuthenticationMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00105
// TypeInfo:  0x0000000142BA3C18
struct PresenceWebMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00104
// TypeInfo:  0x0000000142BA3C38
struct PresenceUserTitleInfoRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00103
// TypeInfo:  0x0000000142BA3C58
struct PresenceUserTitleInfoMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00102
// TypeInfo:  0x0000000142BA3C78
struct PresenceUserProfileInfoMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00101
// TypeInfo:  0x0000000142BA3C98
struct PresenceUserSettingsRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00100
// TypeInfo:  0x0000000142BA3CB8
struct PresenceUserSettingsMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00099
// TypeInfo:  0x0000000142BA3CD8
struct PresenceConsoleNewsRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00098
// TypeInfo:  0x0000000142BA3CF8
struct PresenceConsoleNewsMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00097
// TypeInfo:  0x0000000142BA3D18
struct PresenceStoreRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00096
// TypeInfo:  0x0000000142BA3D38
struct PresenceStoreMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00095
// TypeInfo:  0x0000000142BA3D58
struct PresenceServerRentalRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00094
// TypeInfo:  0x0000000142BA3D78
struct PresenceServerRentalMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00093
// TypeInfo:  0x0000000142BA3D98
struct PresenceServerBrowserRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00092
// TypeInfo:  0x0000000142BA3DB8
struct PresenceServerBrowserMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00091
// TypeInfo:  0x0000000142BA3DD8
struct PresencePlayerEmblemRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00090
// TypeInfo:  0x0000000142BA3DF8
struct PresencePlayerEmblemMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00089
// TypeInfo:  0x0000000142BA3E18
struct PresenceSetupInfoMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00088
// TypeInfo:  0x0000000142BA3E38
struct PresenceServerBannerRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00087
// TypeInfo:  0x0000000142BA3E58
struct PresenceServerBannerMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00086
// TypeInfo:  0x0000000142BA3E78
struct PresenceRspRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00085
// TypeInfo:  0x0000000142BA3E98
struct PresenceRspMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00084
// TypeInfo:  0x0000000142BA3EB8
struct PresenceResetStorageRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00083
// TypeInfo:  0x0000000142BA3ED8
struct PresenceResetStorageMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00082
// TypeInfo:  0x0000000142BA3EF8
struct PresenceProgressionRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00081
// TypeInfo:  0x0000000142BA3F18
struct PresenceProgressionMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00080
// TypeInfo:  0x0000000142BA3F38
struct PresencePingUpdatedMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00079
// TypeInfo:  0x0000000142BA3F58
struct PresencePlaygroundRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00078
// TypeInfo:  0x0000000142BA3F78
struct PresencePeerGameRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00077
// TypeInfo:  0x0000000142BA3F98
struct PresencePeerGameMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00076
// TypeInfo:  0x0000000142BA3FB8
struct PresencePartyRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00075
// TypeInfo:  0x0000000142BA3FD8
struct PresencePartyMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00074
// TypeInfo:  0x0000000142BA3FF8
struct PresenceNucleusLoginUIRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00073
// TypeInfo:  0x0000000142BA4018
struct PresenceMigrateDataRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00072
// TypeInfo:  0x0000000142BA4038
struct PresenceMigrateDataMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00071
// TypeInfo:  0x0000000142BA4058
struct PresenceMatchFeedMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00070
// TypeInfo:  0x0000000142BA4078
struct PresenceGameDataRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00069
// TypeInfo:  0x0000000142BA4098
struct PresenceGameDataMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00068
// TypeInfo:  0x0000000142BA40B8
struct PresenceGameConfigurationRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00067
// TypeInfo:  0x0000000142BA40D8
struct PresenceGameConfigurationMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00066
// TypeInfo:  0x0000000142BA40F8
struct PresenceFriendsStatsRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00065
// TypeInfo:  0x0000000142BA4118
struct PresenceFriendsStatsMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00064
// TypeInfo:  0x0000000142BA4138
struct PresenceDownloadRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00063
// TypeInfo:  0x0000000142BA4158
struct PresenceDownloadMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00062
// TypeInfo:  0x0000000142BA4178
struct PresencePushNotificationMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00061
// TypeInfo:  0x0000000142BA4198
struct PresenceCommMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00060
// TypeInfo:  0x0000000142BA41B8
struct PresenceChallengesRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00059
// TypeInfo:  0x0000000142BA41D8
struct PresenceChallengesMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00058
// TypeInfo:  0x0000000142BA41F8
struct PresenceCalendarRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00057
// TypeInfo:  0x0000000142BA4218
struct PresenceCalendarMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00056
// TypeInfo:  0x0000000142BA4238
struct PresenceBFCommerceMetricMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00055
// TypeInfo:  0x0000000142BA4258
struct PresenceBFCommerceRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00054
// TypeInfo:  0x0000000142BA4278
struct PresenceBFCommerceMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00053
// TypeInfo:  0x0000000142BA4298
struct PresenceBattlepackRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00052
// TypeInfo:  0x0000000142BA42B8
struct PresenceBattlepackMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00051
// TypeInfo:  0x0000000142BA42D8
struct PresenceBattlelogRequestMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00050
// TypeInfo:  0x0000000142BA42F8
struct PresenceBattlelogMessageBase
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00049
// TypeInfo:  0x0000000142BA4318
struct NuiGrammarLoadCompletedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00048
// TypeInfo:  0x0000000142BA4338
struct NuiCheckSpeechRecognizingMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00047
// TypeInfo:  0x0000000142BA4358
struct NuiSpeechGrammarActivationSettingsMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00046
// TypeInfo:  0x0000000142BA4378
struct NuiSpeechGrammarDeactivationMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00045
// TypeInfo:  0x0000000142BA4398
struct NuiSpeechGrammarActivationMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00044
// TypeInfo:  0x0000000142BA43B8
struct NuiSpeechProblemDetectedMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00043
// TypeInfo:  0x0000000142BA43D8
struct NuiSpeechOngoingActivityMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00042
// TypeInfo:  0x0000000142BA43F8
struct NuiSpeechRuleRecognitionFailedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00041
// TypeInfo:  0x0000000142BA4418
struct NuiSpeechRuleRecognitionStartsMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00040
// TypeInfo:  0x0000000142BA4438
struct NuiSpeechSoundEventMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00039
// TypeInfo:  0x0000000142BA4458
struct NuiSpeechRuleRecognizedMessage
{
    char _0x0000[80];
};//Size=0x0050

////////////////////////////////////////
// RuntimeId: 00038
// TypeInfo:  0x0000000142BA4478
struct VeniceClientMetricsJoinGameMessage
{
    char _0x0000[64];
};//Size=0x0040

////////////////////////////////////////
// RuntimeId: 00035
// TypeInfo:  0x0000000142BA44B8
struct VeniceClientMetricsQuitSPGameMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00034
// TypeInfo:  0x0000000142BA44D8
struct VeniceClientMetricsRestartSPMissionMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00033
// TypeInfo:  0x0000000142BA44F8
struct VeniceClientMetricsRestartSPCheckpointMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00032
// TypeInfo:  0x0000000142BA4518
struct VeniceClientMetricsResumeSPGameMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00031
// TypeInfo:  0x0000000142BA4538
struct VeniceClientMetricsStartSPGameMessage
{
    char _0x0000[56];
};//Size=0x0038

////////////////////////////////////////
// RuntimeId: 00030
// TypeInfo:  0x0000000142BA4558
struct VeniceClientMetricsBootMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00029
// TypeInfo:  0x0000000142BA4578
struct VeniceClientMetricsLeaveCriticalHealthMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00028
// TypeInfo:  0x0000000142BA4598
struct VeniceClientMetricsEnterCriticalHealthMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00027
// TypeInfo:  0x0000000142BA45B8
struct VeniceClientMetricsTimeMessage
{
    char _0x0000[48];
};//Size=0x0030

////////////////////////////////////////
// RuntimeId: 00026
// TypeInfo:  0x0000000142BA45D8
struct VeniceClientMetricsListenerDestroyedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00025
// TypeInfo:  0x0000000142BA45F8
struct VeniceClientMetricsListenerCreatedMessage
{
    char _0x0000[40];
};//Size=0x0028

////////////////////////////////////////
// RuntimeId: 00012
// TypeInfo:  0x0000000142BA4618
struct DVRScoringChainMomentMessage
{
    char _0x0000[48];
};//Size=0x0030
}
