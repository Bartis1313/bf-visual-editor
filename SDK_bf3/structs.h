#pragma once

#include "declarations.h"
#include "enums.h"

namespace fb
{
    ////////////////////////////////////////
// RuntimeId: 06570
// TypeInfo:  0x02C27E64
    struct SensingLimits
    {
        float m_HearingDistance; //0x0000
        float m_HearingCutoff; //0x0004
        float m_ViewAngle; //0x0008
        float m_ViewDistance; //0x000C
        float m_PeripheralViewAngle; //0x0010
        float m_PeripheralViewDistance; //0x0014
        float m_PeripheralMovementCutoff; //0x0018
    };//Size=0x001C

    ////////////////////////////////////////
    // RuntimeId: 00011
    // TypeInfo:  0x028874E0
    struct Vec3
    {
        float m_x; //0x0000
        float m_y; //0x0004
        float m_z; //0x0008
        char _0x000C[4];
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 06558
    // TypeInfo:  0x02C27D94
    struct VisionBoxSettings
    {
        float m_DecreaseXz; //0x0000
        float m_LowerTop; //0x0004
        float m_RaiseBottom; //0x0008
        float m_RaiseBottomHuman; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 06540
    // TypeInfo:  0x02C27C78
    struct BurstLimit
    {
        __int32 m_Min; //0x0000
        __int32 m_Max; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 06532
    // TypeInfo:  0x02C27C38
    struct Strengths
    {
        float m_Infantry; //0x0000
        float m_LightArmour; //0x0004
        float m_HeavyArmour; //0x0008
        float m_Helicopter; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 06516
    // TypeInfo:  0x02C27AF4
    struct CoverSlot
    {
        Vec3 m_Position; //0x0000
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 06500
    // TypeInfo:  0x02C279E8
    struct SubsystemTimingConfigData
    {
        float m_Sensing; //0x0000
        float m_Evaluation; //0x0004
        float m_GoalUpdates; //0x0008
        float m_IntentUpdates; //0x000C
        float m_BehaviourSelection; //0x0010
        float m_BehaviourExecution; //0x0014
    };//Size=0x0018

    ////////////////////////////////////////
    // RuntimeId: 06492
    // TypeInfo:  0x02C27954
    struct UrgencyModifierData
    {
        char* m_Name; //0x0000
        float m_Modifier; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 06488
    // TypeInfo:  0x02C27918
    struct UIntRange
    {
        unsigned __int32 m_Min; //0x0000
        unsigned __int32 m_Max; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 06486
    // TypeInfo:  0x02C27908
    struct FloatRangeDefaultZero
    {
        float m_Min; //0x0000
        float m_Max; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 06484
    // TypeInfo:  0x02C278F8
    struct FloatRange
    {
        float m_Min; //0x0000
        float m_Max; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 06476
    // TypeInfo:  0x02C27864
    struct IntentData
    {
        char* m_Name; //0x0000
        float m_DefaultModifier; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 06472
    // TypeInfo:  0x02C27844
    struct ReadinessLevelCharacteristics
    {
        float m_MinReactionTime; //0x0000
        float m_MaxReactionTime; //0x0004
        float m_ReductionTime; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 06474
    // TypeInfo:  0x02C27854
    struct ReadinessLevels
    {
        ReadinessLevelCharacteristics m_Patrol; //0x0000
        ReadinessLevelCharacteristics m_Ready; //0x000C
        ReadinessLevelCharacteristics m_Combat; //0x0018
    };//Size=0x0024

    ////////////////////////////////////////
    // RuntimeId: 06466
    // TypeInfo:  0x02C26DD4
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
        unsigned __int32 m_GunMasterLevel; //0x0020
    };//Size=0x0024

    ////////////////////////////////////////
    // RuntimeId: 06460
    // TypeInfo:  0x02C26A8C
    struct UIMatchImagesRetrievedMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06459
    // TypeInfo:  0x02C26A7C
    struct UIRequestMatchImagesMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06458
    // TypeInfo:  0x02C26A6C
    struct UIVKBDInputDoneMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06457
    // TypeInfo:  0x02C26A5C
    struct UIHudMessageMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 06456
    // TypeInfo:  0x02C26A4C
    struct UIScreenCountChangeMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06455
    // TypeInfo:  0x02C26A3C
    struct UIControllerDisconnectedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06454
    // TypeInfo:  0x02C26A2C
    struct UIControllerConnectedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06453
    // TypeInfo:  0x02C26A1C
    struct UIUserDisconnectedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06452
    // TypeInfo:  0x02C26A0C
    struct UIUserConnectedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06451
    // TypeInfo:  0x02C269FC
    struct UIUserNotificationMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06450
    // TypeInfo:  0x02C269EC
    struct UIUserSkippedLoginMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06449
    // TypeInfo:  0x02C269DC
    struct MemoryCardBootCheckMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06448
    // TypeInfo:  0x02C269CC
    struct MemoryCardFindEntriesDoneMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06447
    // TypeInfo:  0x02C269BC
    struct MemoryCardFindEntriesMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06446
    // TypeInfo:  0x02C269AC
    struct MemoryCardSaveDoneMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06445
    // TypeInfo:  0x02C2699C
    struct MemoryCardSaveMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06444
    // TypeInfo:  0x02C2698C
    struct MemoryCardLoadDoneMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06443
    // TypeInfo:  0x02C2697C
    struct MemoryCardLoadMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06442
    // TypeInfo:  0x02C2696C
    struct MemoryCardDestroyedMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06441
    // TypeInfo:  0x02C2695C
    struct MemoryCardCreatedMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06440
    // TypeInfo:  0x02C2694C
    struct MemoryCardPopupHideMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06439
    // TypeInfo:  0x02C2693C
    struct MemoryCardPopupResponseMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06438
    // TypeInfo:  0x02C2692C
    struct MemoryCardPopupRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06425
    // TypeInfo:  0x02C26830
    struct UIPopupButton
    {
        UIInputAction m_InputConcept; //0x0000
        char* m_Label; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 06385
    // TypeInfo:  0x02C26514
    struct WidgetEventQueryPair
    {
        char* m_Name; //0x0000
        UIWidgetEventID m_Query; //0x0004
        char* m_InstanceName; //0x0008
        bool m_IsOutput; //0x000C
        char _0x000D[3];
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 06379
    // TypeInfo:  0x02C264AC
    struct UIDataSourceInfo
    {
        char* m_DataName; //0x0000
        UIComponentData* m_DataCategory; //0x0004
        __int32 m_DataKey; //0x0008
        bool m_UseDirectAccess; //0x000C
        bool m_UpdateOnInitialize; //0x000D
        char _0x000E[2];
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 06375
    // TypeInfo:  0x02C26470
    struct UIWidgetProperty
    {
        char* m_Name; //0x0000
        char* m_Value; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 06361
    // TypeInfo:  0x02C26358
    struct InterruptFlow
    {
        UIInterruptID m_interruptEnum; //0x0000
        UIGraphAsset* m_interruptFlow; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 06357
    // TypeInfo:  0x02C2631C
    struct UIAudioEventMapping
    {
        char* m_EventName; //0x0000
        SoundAsset* m_SoundAsset; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 06339
    // TypeInfo:  0x02C26238
    struct DataCopValueSetting
    {
        __int32 m_DataKey; //0x0000
        __int32 m_FrameSpacing; //0x0004
        __int32 m_Offset; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 06341
    // TypeInfo:  0x02C26248
    struct DataCopSettings
    {
        Array<DataCopValueSetting> m_Values; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 06335
    // TypeInfo:  0x02C261FC
    struct UIFontMapping
    {
        Array<char*> m_ScaleformFontName; //0x0000
        char* m_FontLongName; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 06327
    // TypeInfo:  0x02C26168
    struct UIBundleAssetState
    {
        char* m_StateName; //0x0000
        UIState m_UIState; //0x0004
        char* m_ResourceBundleName; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 06293
    // TypeInfo:  0x02C25294
    struct SpikeInternalUnreliableNopMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 06292
    // TypeInfo:  0x02C25284
    struct SpikeInternalNopMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 06291
    // TypeInfo:  0x02C25274
    struct CoreDemoStatusMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06290
    // TypeInfo:  0x02C2520C
    struct SpikeInternalMessagePartMessage
    {
        char _0x0000[208];
    };//Size=0x00D0

    ////////////////////////////////////////
    // RuntimeId: 06289
    // TypeInfo:  0x02C251FC
    struct SpikeInternalMessageWrapperMessage
    {
        char _0x0000[80];
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 06109
    // TypeInfo:  0x02C21408
    struct ESportsMatchManagerDelayedTerminationMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06108
    // TypeInfo:  0x02C213F8
    struct ESportsMatchManagerTooFewPlayersMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06107
    // TypeInfo:  0x02C213E8
    struct VeniceCommonMetricsTriggerMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06106
    // TypeInfo:  0x02C213D8
    struct ServerUnlockSystemSetupMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06103
    // TypeInfo:  0x02C213B8
    struct ServerMapSequencerLevelLoadedMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 06102
    // TypeInfo:  0x02C213A8
    struct ServerMapSequencerSwitchingLevelsMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 06101
    // TypeInfo:  0x02C21398
    struct ServerMapSequencerControlledInfoMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06100
    // TypeInfo:  0x02C21388
    struct ServerMapSequencerControlledMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06099
    // TypeInfo:  0x02C21378
    struct ServerMapSequencerStatusMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06098
    // TypeInfo:  0x02C21368
    struct ServerMapSequencerMaxPlayerCountMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06097
    // TypeInfo:  0x02C21358
    struct ServerMapSequencerEventMessageBase
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06094
    // TypeInfo:  0x02C21338
    struct ClientPersistenceAwardGainedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06093
    // TypeInfo:  0x02C21328
    struct PresenceXPromoRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06092
    // TypeInfo:  0x02C21318
    struct PresenceXPromoMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06091
    // TypeInfo:  0x02C21308
    struct PresenceWebRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06090
    // TypeInfo:  0x02C212F8
    struct PresenceWebFeedRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06089
    // TypeInfo:  0x02C212E8
    struct PresenceWebFeedMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06088
    // TypeInfo:  0x02C212D8
    struct PresenceWebMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06087
    // TypeInfo:  0x02C212C8
    struct PresenceUserTitleInfoMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06086
    // TypeInfo:  0x02C212B8
    struct PresenceUserTitleInfoRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06085
    // TypeInfo:  0x02C212A8
    struct PresenceUserSettingsRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06084
    // TypeInfo:  0x02C21298
    struct PresenceUserSettingsMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06083
    // TypeInfo:  0x02C21288
    struct PresenceUserIdRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06082
    // TypeInfo:  0x02C21278
    struct PresenceUserIdMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06081
    // TypeInfo:  0x02C21268
    struct PresenceSystemUIMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06080
    // TypeInfo:  0x02C21258
    struct PresenceStorageRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06079
    // TypeInfo:  0x02C21248
    struct PresenceStorageMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06078
    // TypeInfo:  0x02C21238
    struct PresenceServerBrowserRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06077
    // TypeInfo:  0x02C21228
    struct PresenceServerBrowserMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06076
    // TypeInfo:  0x02C21218
    struct PresenceServerBannerRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06075
    // TypeInfo:  0x02C21208
    struct PresenceServerBannerMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06074
    // TypeInfo:  0x02C211F8
    struct PresenceRspRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06073
    // TypeInfo:  0x02C211E8
    struct PresenceRspMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06072
    // TypeInfo:  0x02C211D8
    struct PresencePopupRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06071
    // TypeInfo:  0x02C211C8
    struct PresencePopupMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06070
    // TypeInfo:  0x02C211B8
    struct PresencePlaygroupAttributesMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06069
    // TypeInfo:  0x02C211A8
    struct PresencePlaygroupMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06068
    // TypeInfo:  0x02C21198
    struct PresencePlaygroupRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06067
    // TypeInfo:  0x02C21188
    struct PresencePingUpdatedMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06066
    // TypeInfo:  0x02C21178
    struct PresenceNewsTickerRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06065
    // TypeInfo:  0x02C21168
    struct PresenceNewsTickerMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06064
    // TypeInfo:  0x02C21158
    struct PresenceMatchmakerMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06063
    // TypeInfo:  0x02C21148
    struct PresenceMatchFeedRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06062
    // TypeInfo:  0x02C21138
    struct PresenceMatchImagesMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06061
    // TypeInfo:  0x02C21128
    struct PresenceMatchFeedMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06060
    // TypeInfo:  0x02C21118
    struct PresenceLivePartyMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06059
    // TypeInfo:  0x02C21108
    struct PresenceLicenseRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06058
    // TypeInfo:  0x02C210F8
    struct PresenceLicenseMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06057
    // TypeInfo:  0x02C210E8
    struct PresenceGameQueueMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06056
    // TypeInfo:  0x02C210D8
    struct PresenceJoinResultMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06055
    // TypeInfo:  0x02C210C8
    struct PresenceHistoryMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06054
    // TypeInfo:  0x02C210B8
    struct PresenceGameConfigurationRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06053
    // TypeInfo:  0x02C210A8
    struct PresenceGameConfigurationMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06052
    // TypeInfo:  0x02C21098
    struct PresencePeerGameRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06051
    // TypeInfo:  0x02C21088
    struct PresencePeerGameMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06050
    // TypeInfo:  0x02C21078
    struct PresenceGameRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06049
    // TypeInfo:  0x02C21068
    struct PresenceGameMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06048
    // TypeInfo:  0x02C21058
    struct PresenceFriendRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06047
    // TypeInfo:  0x02C21048
    struct PresenceFriendMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06046
    // TypeInfo:  0x02C21038
    struct PresenceCouchMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06045
    // TypeInfo:  0x02C21028
    struct PresenceConnectionRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06044
    // TypeInfo:  0x02C21018
    struct PresenceConnectionMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06043
    // TypeInfo:  0x02C21008
    struct PresenceCommerceRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06042
    // TypeInfo:  0x02C20FF8
    struct PresenceCommerceMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06041
    // TypeInfo:  0x02C20FE8
    struct PresenceCommRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06040
    // TypeInfo:  0x02C20FD8
    struct PresenceCommMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06039
    // TypeInfo:  0x02C20FC8
    struct PresenceCalendarRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06038
    // TypeInfo:  0x02C20FB8
    struct PresenceCalendarMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06037
    // TypeInfo:  0x02C20FA8
    struct PresenceBrowserRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06036
    // TypeInfo:  0x02C20F98
    struct PresenceBrowserMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06035
    // TypeInfo:  0x02C20F88
    struct PresenceBlobRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06034
    // TypeInfo:  0x02C20F78
    struct PresenceBlockListMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06033
    // TypeInfo:  0x02C20F68
    struct PresenceBlobMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06032
    // TypeInfo:  0x02C20F58
    struct PresenceAchievementRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06031
    // TypeInfo:  0x02C20F48
    struct VoiceChangedMicEnabledMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06030
    // TypeInfo:  0x02C20F38
    struct VoiceSetMuteStatusMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06029
    // TypeInfo:  0x02C20F28
    struct VoiceSettingsRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06028
    // TypeInfo:  0x02C20F18
    struct VoiceSettingsMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06027
    // TypeInfo:  0x02C20F08
    struct VoiceChannelRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06026
    // TypeInfo:  0x02C20EF8
    struct VoiceChannelMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06025
    // TypeInfo:  0x02C20EE8
    struct UIPreEndOfRoundReadyMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06024
    // TypeInfo:  0x02C20ED8
    struct UIEndOfRoundReadyMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06023
    // TypeInfo:  0x02C20EC8
    struct UIRequestEndOfRoundMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06022
    // TypeInfo:  0x02C20EB8
    struct UIRequestPreEndOfRoundMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06021
    // TypeInfo:  0x02C20EA8
    struct UIEnterFrontendMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06020
    // TypeInfo:  0x02C20E98
    struct UIControlpointChangedTeamMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06019
    // TypeInfo:  0x02C20E88
    struct UIPlayerVehicleListRefreshMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06018
    // TypeInfo:  0x02C20E78
    struct UIHudTicketBleedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06017
    // TypeInfo:  0x02C20E68
    struct UIHudResetWinningTeamMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06016
    // TypeInfo:  0x02C20E58
    struct UITrackedAwardChangedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06015
    // TypeInfo:  0x02C20E48
    struct UITrackedAwardUpdatedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06014
    // TypeInfo:  0x02C20E38
    struct UIHudReviveWeaponEquippedChangedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06013
    // TypeInfo:  0x02C20E28
    struct UIHudShowEnemiesMessage
    {
        char _0x0000[80];
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 06012
    // TypeInfo:  0x02C20E18
    struct UIHudShowCaptureProgressMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06011
    // TypeInfo:  0x02C20E08
    struct UIHudEnableMinimapMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06010
    // TypeInfo:  0x02C20DF8
    struct UIShowScoreboardMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06009
    // TypeInfo:  0x02C20DE8
    struct UIShowMinimapMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06008
    // TypeInfo:  0x02C20DD8
    struct UIShowProgressObjectMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06007
    // TypeInfo:  0x02C20DC8
    struct ServerSoldierSuppressionSuppressMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 06006
    // TypeInfo:  0x02C20DB8
    struct ClientGameplayPlayerBaseDestroyedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 06005
    // TypeInfo:  0x02C20DA8
    struct ServerGameplayCTFFlagReturnedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06004
    // TypeInfo:  0x02C20D98
    struct ServerGameplayCTFFlagCapturedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06003
    // TypeInfo:  0x02C20D88
    struct ServerGameplayCTFFlagDroppedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06002
    // TypeInfo:  0x02C20D78
    struct ServerGameplayCTFFlagPickedUpMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06001
    // TypeInfo:  0x02C20D68
    struct ServerGameplayTeamTakeLeadMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 06000
    // TypeInfo:  0x02C20D58
    struct ServerGameplayTeamLostLeadMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05999
    // TypeInfo:  0x02C20D48
    struct ServerGameplayPlayerBaseDestroyedMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 05998
    // TypeInfo:  0x02C20D38
    struct ServerGameplayTeamReinforceMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05997
    // TypeInfo:  0x02C20D28
    struct ServerGameplayCapturePointPlayerEnteredMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05996
    // TypeInfo:  0x02C20D18
    struct ServerGameplayCapturePointResetMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05995
    // TypeInfo:  0x02C20D08
    struct ServerGameplayCapturePointLostMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05994
    // TypeInfo:  0x02C20CF8
    struct ServerGameplayCapturePointCapturedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05993
    // TypeInfo:  0x02C20CE8
    struct ServerGameplayCapturePointChangedMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 05992
    // TypeInfo:  0x02C20CD8
    struct ServerPlayerLicensesAvailableMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05991
    // TypeInfo:  0x02C20CC8
    struct ServerPlayerManDownRevivedMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 05990
    // TypeInfo:  0x02C20CB8
    struct ServerPlayerMeleeInteruptedMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 05989
    // TypeInfo:  0x02C20CA8
    struct ServerPlayerTeamKillKickMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05988
    // TypeInfo:  0x02C20C98
    struct ServerPlayerNoInteractivityKickMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05987
    // TypeInfo:  0x02C20C88
    struct ServerPlayerSpawnOnSelectedSpawnPointMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05986
    // TypeInfo:  0x02C20C78
    struct ServerPlayerSpawnAtVehicleMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05985
    // TypeInfo:  0x02C20C68
    struct ServerPlayerSpawnOnPlayerMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 05984
    // TypeInfo:  0x02C20C58
    struct ServerPlayerSquadLeaderStatusChangedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05983
    // TypeInfo:  0x02C20C48
    struct ServerPlayerChangedSquadSpawnerStatusMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05982
    // TypeInfo:  0x02C20C38
    struct ServerPlayerChangedVoiceChannelMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05981
    // TypeInfo:  0x02C20C28
    struct ServerPlayerSwitchSquadMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05980
    // TypeInfo:  0x02C20C18
    struct ClientPlayerChangedMatchReadyStatusMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05979
    // TypeInfo:  0x02C20C08
    struct ClientPlayerChangedVoiceChannelMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05978
    // TypeInfo:  0x02C20BF8
    struct ClientPlayerChangedAllowedToSpawnOnMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 05977
    // TypeInfo:  0x02C20BE8
    struct ClientPlayerChangedSquadLeaderStatusMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05976
    // TypeInfo:  0x02C20BD8
    struct ClientPlayerNewSquadOrderMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05975
    // TypeInfo:  0x02C20BC8
    struct ClientPlayerSwitchSquadMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05974
    // TypeInfo:  0x02C20BB8
    struct ServerResendSyncedSettingsMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 05973
    // TypeInfo:  0x02C20BA8
    struct ServerRestartForESportsMatchMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 05972
    // TypeInfo:  0x02C20B98
    struct ServerRotateLevelMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05967
    // TypeInfo:  0x02C20B4C
    struct GunMasterLevelInfo
    {
        CustomizeSoldierData* m_CustomizeData; //0x0000
        __int32 m_KillsNeeded; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 05969
    // TypeInfo:  0x02C20B5C
    struct GunMasterWeaponsPreset
    {
        Array<GunMasterLevelInfo> m_GunMasterLevelInfos; //0x0000
        Array<CustomizeSoldierData*> m_LevelOverrides; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 00135
    // TypeInfo:  0x02BE3454
    struct AntRef
    {
        __int32 m_AssetId; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 05945
    // TypeInfo:  0x02C209A0
    struct CharacterMeleeBinding
    {
        AntRef m_TriggerMelee; //0x0000
        AntRef m_SelfActor; //0x0004
        AntRef m_OtherActor; //0x0008
        AntRef m_MeleeActive; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 05943
    // TypeInfo:  0x02C20990
    struct CharacterMeleeCommonBinding
    {
        AntRef m_MeleeDistance; //0x0000
        AntRef m_MeleeAngle; //0x0004
        AntRef m_MeleeDefenceTime; //0x0008
        AntRef m_MeleeSequenceTime; //0x000C
        AntRef m_MeleeAborted; //0x0010
        AntRef m_CloseToTarget; //0x0014
        AntRef m_MeleeType; //0x0018
    };//Size=0x001C

    ////////////////////////////////////////
    // RuntimeId: 05925
    // TypeInfo:  0x02C20820
    struct KillWeight
    {
        float m_LowIntenseWeight; //0x0000
        float m_HighIntenseWeight; //0x0004
        float m_CoolDownTime; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 05903
    // TypeInfo:  0x02C20658
    struct InteractionTypesData
    {
        bool m_InteractionEntity; //0x0000
        bool m_PickupEntity; //0x0001
        bool m_AmmoCrateEntity; //0x0002
        bool m_VehicleEntity; //0x0003
        bool m_ExplosionPackEntity; //0x0004
        bool m_SoldierEntity; //0x0005
    };//Size=0x0006

    ////////////////////////////////////////
    // RuntimeId: 05877
    // TypeInfo:  0x02C20454
    struct SupportedShootingProximityChecking
    {
        float m_RayAboveGunHeightOffset; //0x0000
        float m_RayAboveLength; //0x0004
        float m_RayBelowGunHeightOffset; //0x0008
        float m_RayBelowLength; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 05867
    // TypeInfo:  0x02C203E8
    struct SpecialMoveInterpolatedPoint
    {
        Vec3 m_Value; //0x0000
        float m_Time; //0x0010
        char _0x0014[12];
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 05871
    // TypeInfo:  0x02C20408
    struct SpecialMoveStateData
    {
        float m_MinHeightScale; //0x0000
        float m_OptimumDistanceFromObject; //0x0004
        float m_OptimumHeightOfObject; //0x0008
        float m_OptimumLengthOfObject; //0x000C
        float m_MaxDistanceScale; //0x0010
        float m_MinDistanceScale; //0x0014
        float m_MaxHeightScale; //0x0018
        Array<SpecialMoveInterpolatedPoint> m_MotionPoints; //0x001C
        float m_MaxObjectLengthScale; //0x0020
        float m_MinObjectLengthScale; //0x0024
        float m_ExitAnimationDrivenStateVelocity; //0x0028
        SpecialMovesPoseType m_ResetPose; //0x002C
        SpecialMovesPoseType m_OverridePose; //0x0030
        Array<SpecialMoveInterpolatedPoint> m_GravityPoints; //0x0034
        bool m_KeepInitialSoldierDirection; //0x0038
        bool m_ScaleWithDistanceToObject; //0x0039
        char _0x003A[2];
    };//Size=0x003C

    ////////////////////////////////////////
    // RuntimeId: 05873
    // TypeInfo:  0x02C20418
    struct SpecialMoveVaultStateCriteria
    {
        float m_DistToObject; //0x0000
        float m_DistToObjectTolerance; //0x0004
        float m_HeightOfObject; //0x0008
        float m_HeightOfObjectTolerance; //0x000C
        float m_LengthOfObject; //0x0010
        float m_LengthOfObjectTolerance; //0x0014
        SpecialMoveStateData m_SpecialMoveState; //0x0018
    };//Size=0x0054

    ////////////////////////////////////////
    // RuntimeId: 05863
    // TypeInfo:  0x02C203AC
    struct AirMissileJammingData
    {
        float m_MinTurnSpeed; //0x0000
        float m_MaxRadius; //0x0004
        float m_MinRadius; //0x0008
        float m_Theta; //0x000C
        float m_Phi; //0x0010
        float m_UpdateJammingPositionTime; //0x0014
        float m_MaxTurnSpeed; //0x0018
        float m_QuietModeProbability; //0x001C
        float m_QuietModeTime; //0x0020
        float m_JammingTime; //0x0024
        float m_TurnCalculationTime; //0x0028
        bool m_EnableAirMissileJamming; //0x002C
        char _0x002D[3];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05861
    // TypeInfo:  0x02C2039C
    struct SmokeJammingData
    {
        float m_JammingTime; //0x0000
        float m_MaxRadius; //0x0004
        float m_MinRadius; //0x0008
        float m_TargetUpdateTime; //0x000C
        bool m_EnableSmokeJamming; //0x0010
        bool m_JammingPositionIsLocationDependent; //0x0011
        bool m_ReportJammedBasedOnStartPosition; //0x0012
        char _0x0013[1];
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 05859
    // TypeInfo:  0x02C2038C
    struct SimpleAnimationControlBinding
    {
        AntRef m_LocoTarget; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 05857
    // TypeInfo:  0x02C2037C
    struct SupportedShootingBinding
    {
        AntRef m_Supported; //0x0000
        AntRef m_SupportedPos; //0x0004
        AntRef m_ForwardVector; //0x0008
        AntRef m_Yaw; //0x000C
        AntRef m_Pitch; //0x0010
        AntRef m_DistToObject; //0x0014
        AntRef m_HeightOfObject; //0x0018
        AntRef m_Undeploying; //0x001C
        AntRef m_AnimatedCamera; //0x0020
    };//Size=0x0024

    ////////////////////////////////////////
    // RuntimeId: 05855
    // TypeInfo:  0x02C2036C
    struct SpecialMovesBinding
    {
        AntRef m_TriggerHighVault; //0x0000
        AntRef m_TriggerUpVault; //0x0004
        AntRef m_TriggerLowVault; //0x0008
        AntRef m_TriggerSlideIntoProne; //0x000C
        AntRef m_TriggerSpecialAnimation; //0x0010
        AntRef m_SpecialAnimationIndex; //0x0014
        AntRef m_ObjectHeight; //0x0018
        AntRef m_ObjectDistance; //0x001C
        AntRef m_StartObjectDistanceTimer; //0x0020
        AntRef m_VaultObjectPositionX; //0x0024
        AntRef m_VaultObjectPositionZ; //0x0028
        AntRef m_CancelAnimation; //0x002C
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05853
    // TypeInfo:  0x02C2035C
    struct SoldierParachuteBinding
    {
        AntRef m_Enabled; //0x0000
        AntRef m_SteerLeftRight; //0x0004
        AntRef m_AccelerateBrake; //0x0008
        AntRef m_Pitch; //0x000C
        AntRef m_Roll; //0x0010
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 05851
    // TypeInfo:  0x02C2034C
    struct SoldierEmoteBinding
    {
        AntRef m_Speak; //0x0000
        AntRef m_IsSquadLeader; //0x0004
        AntRef m_EmoteSpot; //0x0008
        AntRef m_EmoteOk; //0x000C
        AntRef m_EmoteNeedARide; //0x0010
        AntRef m_EmoteGoGoGo; //0x0014
        AntRef m_EmoteNeedBackup; //0x0018
        AntRef m_EmoteThanks; //0x001C
        AntRef m_EmoteNeedMedic; //0x0020
        AntRef m_EmoteFollowMe; //0x0024
        AntRef m_EmoteNeedAmmo; //0x0028
        AntRef m_EmoteSorry; //0x002C
        AntRef m_EmoteNo; //0x0030
        AntRef m_EmoteDefendCapturePoint; //0x0034
        AntRef m_EmoteAttackCapturePoint; //0x0038
        AntRef m_EmoteMoveToPosition; //0x003C
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05849
    // TypeInfo:  0x02C2033C
    struct ClimbLadderBinding
    {
        AntRef m_ClimbLadderPhase; //0x0000
        AntRef m_ClimbLadderConnectPhase; //0x0004
        AntRef m_ClimbLadderTransitionPhase; //0x0008
        AntRef m_OnLadder; //0x000C
        AntRef m_LadderTopTransition; //0x0010
        AntRef m_FallHigh; //0x0014
    };//Size=0x0018

    ////////////////////////////////////////
    // RuntimeId: 05843
    // TypeInfo:  0x02C202D4
    struct UavCameraPointData
    {
        float m_MinTime; //0x0000
        float m_MaxTime; //0x0004
        float m_Speed; //0x0008
        float m_AccelerationTime; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 05841
    // TypeInfo:  0x02C202C4
    struct PunkBusterExecCmdMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05840
    // TypeInfo:  0x02C202B4
    struct PunkBusterMessageMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05839
    // TypeInfo:  0x02C202A4
    struct ServerUnlockListRefreshMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 05838
    // TypeInfo:  0x02C20294
    struct ServerUnlockListEventMessageBase
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05835
    // TypeInfo:  0x02C20274
    struct ServerReservedSlotsListEventMessageBase
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05834
    // TypeInfo:  0x02C20264
    struct ServerBanListEventMessageBase
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05833
    // TypeInfo:  0x02C20254
    struct NetworkGameAdministrationAddGameBanResponseMessage
    {
        char _0x0000[56];
        AddGameBanResponseType m_Result; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05830
    // TypeInfo:  0x02C20234
    struct NetworkGameAdministrationAddGameBanRequestMessage
    {
        char _0x0000[56];
        unsigned __int64 m_BannedUserNativeData; //0x0038
        char* m_BannedUserId; //0x0040
        char _0x0044[4];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 05829
    // TypeInfo:  0x02C20224
    struct NetworkGameAdministrationPacketMessage
    {
        char _0x0000[56];
        bool m_IsFromServer; //0x0038
        bool m_IsResponse; //0x0039
        char _0x003A[2];
        unsigned __int32 m_Sequence; //0x003C
        Array<char*> m_Words; //0x0040
        char _0x0044[4];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 05828
    // TypeInfo:  0x02C20214
    struct ServerGameAdministrationEventMessageBase
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05827
    // TypeInfo:  0x02C20204
    struct ClientPersistenceUnlockMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 05826
    // TypeInfo:  0x02C201F4
    struct VeniceClientMetricsQuitSPGameMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 05825
    // TypeInfo:  0x02C201E4
    struct VeniceClientMetricsRestartSPMissionMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 05824
    // TypeInfo:  0x02C201D4
    struct VeniceClientMetricsRestartSPCheckpointMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 05823
    // TypeInfo:  0x02C201C4
    struct VeniceClientMetricsResumeSPGameMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05822
    // TypeInfo:  0x02C201B4
    struct VeniceClientMetricsStartSPGameMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05821
    // TypeInfo:  0x02C201A4
    struct VeniceClientMetricsBootMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 05820
    // TypeInfo:  0x02C20194
    struct VeniceClientMetricsLeaveCriticalHealthMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05819
    // TypeInfo:  0x02C20184
    struct VeniceClientMetricsEnterCriticalHealthMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05818
    // TypeInfo:  0x02C20174
    struct VeniceClientMetricsTimeMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05817
    // TypeInfo:  0x02C20164
    struct VeniceClientMetricsListenerDestroyedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 05816
    // TypeInfo:  0x02C20154
    struct VeniceClientMetricsListenerCreatedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 05780
    // TypeInfo:  0x02C1E7A4
    struct StatPointsMessageBase
    {
        char _0x0000[72];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 05779
    // TypeInfo:  0x02C1E794
    struct StatEventMessageBase
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05778
    // TypeInfo:  0x02C1E784
    struct StatEventEntityTriggerMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 05777
    // TypeInfo:  0x02C1E774
    struct StatGainAwardMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05776
    // TypeInfo:  0x02C1E764
    struct StatFinalizeStatsMessageBase
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05775
    // TypeInfo:  0x02C1E754
    struct StatPlayerScoreFinalizedMessageBase
    {
        char _0x0000[80];
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 05774
    // TypeInfo:  0x02C1E744
    struct StatClubRecordClosingMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05773
    // TypeInfo:  0x02C1E734
    struct StatClubRecordCreatedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05772
    // TypeInfo:  0x02C1E724
    struct StatPlayerRecordClosingMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05771
    // TypeInfo:  0x02C1E714
    struct StatPlayerRecordCreatedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05770
    // TypeInfo:  0x02C1E704
    struct ServerGameManagerResetServerMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 05769
    // TypeInfo:  0x02C1E6F4
    struct ServerGameManagerUpdateGameServerListsMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 05768
    // TypeInfo:  0x02C1E6E4
    struct ServerGameManagerRefreshGameAttributesMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 05767
    // TypeInfo:  0x02C1E6D4
    struct ServerGameManagerUpdatePlayerCapacityResponseMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05766
    // TypeInfo:  0x02C1E6C4
    struct ServerGameManagerUpdatePlayerCapacityRequestMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05765
    // TypeInfo:  0x02C1E6B4
    struct ServerGameManagerMatchmakingStateMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05764
    // TypeInfo:  0x02C1E6A4
    struct ServerGameManagerUpdateMatchmakingStateMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05763
    // TypeInfo:  0x02C1E694
    struct ServerGameManagerAddQueuedPlayerMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05762
    // TypeInfo:  0x02C1E684
    struct ServerGameManagerValidateContentResponseMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 05761
    // TypeInfo:  0x02C1E674
    struct ServerGameManagerValidateContentRequestMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05760
    // TypeInfo:  0x02C1E664
    struct ServerGameManagerAddGameBanResponseMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05759
    // TypeInfo:  0x02C1E654
    struct ServerGameManagerAddGameBanRequestMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05758
    // TypeInfo:  0x02C1E644
    struct ServerGameManagerGetGameDataMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 05757
    // TypeInfo:  0x02C1E634
    struct ServerGameManagerRestartLevelRequestMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 05756
    // TypeInfo:  0x02C1E624
    struct ServerGameManagerGameSettingsUpdateMessage
    {
        char _0x0000[104];
    };//Size=0x0068

    ////////////////////////////////////////
    // RuntimeId: 05755
    // TypeInfo:  0x02C1E614
    struct ServerGameManagerPlayerJoiningQueueMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05754
    // TypeInfo:  0x02C1E604
    struct ServerGameManagerCheckAggressiveJoinStateMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05753
    // TypeInfo:  0x02C1E5F4
    struct ServerGameManagerPlayerJoiningMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05752
    // TypeInfo:  0x02C1E5E4
    struct ServerGameManagerGameAttributesUpdateMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 05751
    // TypeInfo:  0x02C1E5D4
    struct PeerServerBackendResponseMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 05750
    // TypeInfo:  0x02C1E5C4
    struct PeerServerBackendRequestMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 05749
    // TypeInfo:  0x02C1E5B4
    struct VeniceServerMetricsManDownImmobilizedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05748
    // TypeInfo:  0x02C1E5A4
    struct VeniceServerMetricsManDownMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05747
    // TypeInfo:  0x02C1E594
    struct VeniceServerMetricsRestartRoundMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05746
    // TypeInfo:  0x02C1E584
    struct VeniceServerMetricsStartRoundMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05745
    // TypeInfo:  0x02C1E574
    struct VeniceServerMetricsLifeCounterMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05744
    // TypeInfo:  0x02C1E564
    struct VeniceServerMetricsFlagCounterMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05743
    // TypeInfo:  0x02C1E554
    struct VeniceServerMetricsTicketCounterMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05742
    // TypeInfo:  0x02C1E544
    struct VeniceServerMetricsRankupMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 05741
    // TypeInfo:  0x02C1E534
    struct VeniceServerMetricsKitPickedupMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05740
    // TypeInfo:  0x02C1E524
    struct VeniceServerMetricsCreateLogMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05665
    // TypeInfo:  0x02C1DEB8
    struct AIProximityReactionsBinding
    {
        AntRef m_AIReaction; //0x0000
        AntRef m_AIExplosionReaction; //0x0004
        AntRef m_AIExplosionForce; //0x0008
        AntRef m_AIStunReaction; //0x000C
        AntRef m_AIStunReactionRandomFactor; //0x0010
        AntRef m_AIReactionDirection; //0x0014
        AntRef m_FireNearby; //0x0018
        AntRef m_AimUpAndDown; //0x001C
        AntRef m_FocusAimScale; //0x0020
        AntRef m_FocusAiming; //0x0024
        AntRef m_EnableProceduralHeadAim; //0x0028
        AntRef m_KillFromAnimation; //0x002C
        AntRef m_PlayerAnimationKill; //0x0030
        AntRef m_PlayerInitiatedRagdoll; //0x0034
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 05657
    // TypeInfo:  0x02C1DE78
    struct VeniceNetworkSetPingMessage
    {
        char _0x0000[56];
        unsigned __int16 m_PingTime; //0x0038
        char _0x003A[6];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05656
    // TypeInfo:  0x02C1DE68
    struct VeniceNetworkMetricsAchievementMessage
    {
        char _0x0000[56];
        __int32 m_Achievement; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05655
    // TypeInfo:  0x02C1DE58
    struct VeniceNetworkLocalClientConnectionInfoUpdatedMessageBase
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 05654
    // TypeInfo:  0x02C1DE48
    struct VeniceNetworkOnPlayerSelectedTeamMessage
    {
        char _0x0000[56];
        bool m_Success; //0x0038
        char _0x0039[3];
        __int32 m_ErrorCode; //0x003C
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05651
    // TypeInfo:  0x02C1DE28
    struct VeniceNetworkFriendZoneDebugEnableMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 05650
    // TypeInfo:  0x02C1DE18
    struct VeniceNetworkFriendZoneMessage
    {
        char _0x0000[56];
        TeamId m_TeamId; //0x0038
        __int8 m_PreferredZone; //0x003C
        char _0x003D[3];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05649
    // TypeInfo:  0x02C1DE08
    struct VeniceNetworkVirtualGameStateMessage
    {
        char _0x0000[56];
        unsigned __int32 m_VirtualGameState; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05648
    // TypeInfo:  0x02C1DDF8
    struct VeniceNetworkSendTelemetryTokenMessage
    {
        char _0x0000[56];
        char* m_AuthToken; //0x0038
        unsigned __int32 m_Locale; //0x003C
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05647
    // TypeInfo:  0x02C1DDE8
    struct VeniceNetworkNotifyMicStateMessage
    {
        char _0x0000[56];
        bool m_Enabled; //0x0038
        char _0x0039[7];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05646
    // TypeInfo:  0x02C1DDD8
    struct VeniceNetworkSetVoiceChannelMessage
    {
        char _0x0000[56];
        VoiceChannel m_Channel; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05645
    // TypeInfo:  0x02C1DDC8
    struct VeniceNetworkEorStatsMessage
    {
        char _0x0000[56];
        unsigned __int32 m_BestWeaponId; //0x0038
        unsigned __int16 m_BestWeaponKills; //0x003C
        unsigned __int16 m_BestWeaponShotsFired; //0x003E
        unsigned __int16 m_BestWeaponShotsHit; //0x0040
        unsigned __int16 m_BestWeaponTimeWith; //0x0042
        unsigned __int32 m_BestVehicleId; //0x0044
        unsigned __int16 m_BestVehicleKills; //0x0048
        unsigned __int16 m_BestVehicleRoadKills; //0x004A
        unsigned __int16 m_BestVehicleTimeIn; //0x004C
        unsigned __int16 m_EloSkill; //0x004E
        unsigned __int16 m_EloSkillDelta; //0x0050
        unsigned __int16 m_LongestStreak; //0x0052
        unsigned __int16 m_LongestNemesisStreak; //0x0054
        unsigned __int16 m_DogTagsTaken; //0x0056
        float m_LongestHeadshot; //0x0058
        float m_RoundPlayTime; //0x005C
        float m_TotalPlayTime; //0x0060
        Array<float> m_TeamStats; //0x0064
        bool m_WasCoopRequested; //0x0068
        char _0x0069[1];
        unsigned __int16 m_HeadShots; //0x006A
        unsigned __int16 m_KillAssists; //0x006C
        unsigned __int16 m_VehiclesDestroyed; //0x006E
        unsigned __int16 m_AvengerKills; //0x0070
        unsigned __int16 m_SpotAssists; //0x0072
        unsigned __int16 m_SaviorKills; //0x0074
        unsigned __int16 m_ManDownRevives; //0x0076
        float m_RoundTime; //0x0078
        float m_PrevBestScore; //0x007C
        unsigned __int16 m_OtherPlayerHeadshots; //0x0080
        unsigned __int16 m_OtherPlayerKillAssists; //0x0082
        unsigned __int16 m_OtherPlayerVehiclesDestroyed; //0x0084
        unsigned __int16 m_OtherPlayerAvengerKills; //0x0086
        unsigned __int16 m_OtherPlayerSpotAssists; //0x0088
        unsigned __int16 m_OtherPlayerSaviorKills; //0x008A
        unsigned __int16 m_OtherPlayerRevives; //0x008C
        bool m_IsRanked; //0x008E
        bool m_InMatch; //0x008F
        char* m_Team1Name; //0x0090
        char* m_Team2Name; //0x0094
        unsigned __int16 m_MatchTotalRounds; //0x0098
        unsigned __int16 m_MatchCurrentRound; //0x009A
        char _0x009C[4];
    };//Size=0x00A0

    ////////////////////////////////////////
    // RuntimeId: 05644
    // TypeInfo:  0x02C1DDB8
    struct VeniceNetworkRequestCoopPlayerInformationMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 05643
    // TypeInfo:  0x02C1DDA8
    struct VeniceNetworkRequestCoopEorStatsMessage
    {
        char _0x0000[56];
        __int32 m_OtherPlayerId; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05642
    // TypeInfo:  0x02C1DD98
    struct VeniceNetworkRequestEorStatsMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 05641
    // TypeInfo:  0x02C1DD88
    struct VeniceNetworkReviveResponseMessage
    {
        char _0x0000[56];
        bool m_Accepted; //0x0038
        char _0x0039[7];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05640
    // TypeInfo:  0x02C1DD78
    struct VeniceNetworkDefibKillMessage
    {
        char _0x0000[72];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 05639
    // TypeInfo:  0x02C1DD68
    struct VeniceNetworkReviveFinishedMessage
    {
        char _0x0000[56];
        bool m_Successful; //0x0038
        char _0x0039[7];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05638
    // TypeInfo:  0x02C1DD58
    struct VeniceNetworkReviveInitiatedMessage
    {
        char _0x0000[72];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 05634
    // TypeInfo:  0x02C1DD28
    struct CounterStatus
    {
        float m_CurrentValue; //0x0000
        float m_OriginalValue; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 05636
    // TypeInfo:  0x02C1DD38
    struct AwardStatus
    {
        char* m_Code; //0x0000
        unsigned __int32 m_CurrentValue; //0x0004
        unsigned __int32 m_OriginalValue; //0x0008
        Array<CounterStatus> m_Counters; //0x000C
        bool m_IsCounting; //0x0010
        char _0x0011[3];
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 05637
    // TypeInfo:  0x02C1DD48
    struct VeniceNetworkMissionAwardsStatusMessage
    {
        char _0x0000[56];
        Array<AwardStatus> m_Awards; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05632
    // TypeInfo:  0x02C1DD18
    struct VeniceNetworkDogTagMessage
    {
        char _0x0000[64];
        unsigned __int16 m_BasicDogTag; //0x0040
        unsigned __int16 m_AdvancedDogTag; //0x0042
        float m_AdvancedStat; //0x0044
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 05631
    // TypeInfo:  0x02C1DD08
    struct VeniceNetworkUnlockAchievementsMessage
    {
        char _0x0000[56];
        unsigned __int32 m_Mask0; //0x0038
        unsigned __int32 m_Mask1; //0x003C
        unsigned __int32 m_Mask2; //0x0040
        unsigned __int32 m_Mask3; //0x0044
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 05630
    // TypeInfo:  0x02C1DCF8
    struct VeniceNetworkAllWeaponKillsMessage
    {
        char _0x0000[56];
        PersistenceGameType m_GameType; //0x0038
        Array<unsigned __int32> m_WeaponKills; //0x003C
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05629
    // TypeInfo:  0x02C1DCE8
    struct VeniceNetworkRequestAllWeaponKillsMessage
    {
        char _0x0000[56];
        PersistenceGameType m_GameType; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05628
    // TypeInfo:  0x02C1DCD8
    struct VeniceNetworkUnlockGainedMessage
    {
        char _0x0000[56];
        unsigned __int32 m_Identifier; //0x0038
        unsigned __int32 m_UnlockTrackIndex; //0x003C
        VeniceNetworkUnlockGainedType m_ReasonType; //0x0040
        ScoringBucket m_ReasonBucket; //0x0044
        char* m_ReasonAward; //0x0048
        bool m_ReasonAwardIsMission; //0x004C
        char _0x004D[3];
        unsigned __int32 m_ReasonWeapon; //0x0050
        float m_ReasonValue; //0x0054
    };//Size=0x0058

    ////////////////////////////////////////
    // RuntimeId: 05625
    // TypeInfo:  0x02C1DCB8
    struct VeniceNetworkAwardGainedMessage
    {
        char _0x0000[56];
        char* m_AwardCode; //0x0038
        char _0x003C[4];
        unsigned __int64 m_Date; //0x0040
        unsigned __int32 m_TimesReceived; //0x0048
        bool m_IsSoloAward; //0x004C
        char _0x004D[3];
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 05624
    // TypeInfo:  0x02C1DCA8
    struct VeniceNetworkSetSquadPrivacyMessage
    {
        char _0x0000[56];
        bool m_Private; //0x0038
        char _0x0039[7];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05623
    // TypeInfo:  0x02C1DC98
    struct VeniceNetworkLeaveSquadMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 05622
    // TypeInfo:  0x02C1DC88
    struct VeniceNetworkKickFromSquadMessage
    {
        char _0x0000[56];
        unsigned __int32 m_PlayerId; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05621
    // TypeInfo:  0x02C1DC78
    struct VeniceNetworkCreateSquadMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 05620
    // TypeInfo:  0x02C1DC68
    struct VeniceNetworkJoinSquadByOnlineIdMessage
    {
        char _0x0000[56];
        unsigned __int64 m_OnlineId; //0x0038
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05619
    // TypeInfo:  0x02C1DC58
    struct VeniceNetworkJoinSquadMessage
    {
        char _0x0000[56];
        SquadId m_SquadId; //0x0038
        bool m_AllowPrivate; //0x003C
        char _0x003D[3];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05618
    // TypeInfo:  0x02C1DC48
    struct VeniceNetworkOrderMessage
    {
        char _0x0000[72];
        unsigned __int32 m_OrderType; //0x0048
        char _0x004C[4];
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 05617
    // TypeInfo:  0x02C1DC38
    struct VeniceNetworkFocusPointMessage
    {
        char _0x0000[72];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 05616
    // TypeInfo:  0x02C1DC28
    struct VeniceNetworkSpottingMessage
    {
        char _0x0000[128];
        SpotType m_SpotType; //0x0080
        char _0x0084[4];
    };//Size=0x0088

    ////////////////////////////////////////
    // RuntimeId: 05615
    // TypeInfo:  0x02C1DC18
    struct VeniceNetworkRemoveOrderMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05323
    // TypeInfo:  0x02C18A84
    struct ClientMetricsPauseGameMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05322
    // TypeInfo:  0x02C18A74
    struct ClientMetricsUIActionMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05321
    // TypeInfo:  0x02C18A64
    struct ClientMetricsPopUIScreenMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05320
    // TypeInfo:  0x02C18A54
    struct ClientMetricsPushUIScreenMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05170
    // TypeInfo:  0x02C16340
    struct ServerMetricsDetonateExplosionMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 05169
    // TypeInfo:  0x02C16330
    struct ServerMetricsObjectiveSuccessMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05168
    // TypeInfo:  0x02C16320
    struct ServerMetricsSaveGameSavedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05167
    // TypeInfo:  0x02C16310
    struct ServerMetricsSaveGameLoadedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05166
    // TypeInfo:  0x02C16300
    struct ServerLoadGameMessage
    {
        char _0x0000[72];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 05165
    // TypeInfo:  0x02C162F0
    struct SaveGameSaveMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 05164
    // TypeInfo:  0x02C162E0
    struct StatDisableMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 05163
    // TypeInfo:  0x02C162D0
    struct StatEnableMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 00013
    // TypeInfo:  0x02887510
    struct Vec4
    {
        float m_x; //0x0000
        float m_y; //0x0004
        float m_z; //0x0008
        float m_w; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 00009
    // TypeInfo:  0x028874B0
    struct Vec2
    {
        float m_x; //0x0000
        float m_y; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 05030
    // TypeInfo:  0x02C15030
    struct EmitterTextureInfo
    {
        char* m_TextureName; //0x0000
        char* m_NormalmapName; //0x0004
        float m_AnimationFrameCount; //0x0008
        float m_AnimationFrameColumnCount; //0x000C
        bool m_EnableAnimation; //0x0010
        char _0x0011[3];
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 04986
    // TypeInfo:  0x02C14120
    struct DebrisClusterPartInfoData
    {
        Vec3 m_AngularVelocity; //0x0000
        Vec3 m_LinearVelocity; //0x0010
        __int32 m_NumberOfChildren; //0x0020
        __int32 m_PartIndex; //0x0024
        float m_SplitSpeedThreshold; //0x0028
        bool m_InEffectWorldOnly; //0x002C
        bool m_SyncRestPosition; //0x002D
        bool m_SyncContinous; //0x002E
        char _0x002F[1];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 04980
    // TypeInfo:  0x02C140B8
    struct DebrisHavokInfo
    {
        HavokAsset* m_HavokAsset; //0x0000
        __int32 m_ReserveCount; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 04978
    // TypeInfo:  0x02C140A8
    struct DebrisSystemMetrics
    {
        __int32 m_HavokParticleCount; //0x0000
        __int32 m_HavokParticlePartCount; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 04960
    // TypeInfo:  0x02C13B64
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
    // RuntimeId: 04948
    // TypeInfo:  0x02C13A78
    struct RibbonPointData
    {
        Vec4 m_UserMaskRight; //0x0000
        Vec4 m_UserMaskLeft; //0x0010
        float m_Right; //0x0020
        float m_Left; //0x0024
        char _0x0028[8];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 04904
    // TypeInfo:  0x02C13704
    struct RectangularCoverageData
    {
        char _0x0000[1];
    };//Size=0x0001

    ////////////////////////////////////////
    // RuntimeId: 04896
    // TypeInfo:  0x02C1368C
    struct TerrainLayerCombinationDrawData
    {
        char _0x0000[1];
    };//Size=0x0001

    ////////////////////////////////////////
    // RuntimeId: 04894
    // TypeInfo:  0x02C1367C
    struct Surface3dDrawMethodData
    {
        char _0x0000[1];
    };//Size=0x0001

    ////////////////////////////////////////
    // RuntimeId: 04892
    // TypeInfo:  0x02C1366C
    struct MeshScatteringMaskScaleDrawMethodData
    {
        char _0x0000[1];
    };//Size=0x0001

    ////////////////////////////////////////
    // RuntimeId: 04890
    // TypeInfo:  0x02C1365C
    struct Surface2dDrawMethodData
    {
        char _0x0000[1];
    };//Size=0x0001

    ////////////////////////////////////////
    // RuntimeId: 04888
    // TypeInfo:  0x02C1364C
    struct Surface2dDrawPassData
    {
        char _0x0000[1];
    };//Size=0x0001

    ////////////////////////////////////////
    // RuntimeId: 04886
    // TypeInfo:  0x02C1363C
    struct TerrainLayerCombinationDrawPassData
    {
        char _0x0000[1];
    };//Size=0x0001

    ////////////////////////////////////////
    // RuntimeId: 04870
    // TypeInfo:  0x02C1354C
    struct TerrainLayerProceduralMask
    {
        float m_AltitudeMin; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 04868
    // TypeInfo:  0x02C1353C
    struct TerrainGeoTexture
    {
        char _0x0000[1];
    };//Size=0x0001

    ////////////////////////////////////////
    // RuntimeId: 04834
    // TypeInfo:  0x02C12720
    struct BreakableModelToComponentsSpawnPartObjectsOnCollapseMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 04833
    // TypeInfo:  0x02C12710
    struct BreakableModelToComponentsCollapseMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 04804
    // TypeInfo:  0x02C12130
    struct UILevelSpecificPageHeader
    {
        char* m_LevelNameSID; //0x0000
        char* m_Header; //0x0004
        char* m_SubHeader; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 04784
    // TypeInfo:  0x02C11FB0
    struct UIButton
    {
        char* m_ButtonLabel; //0x0000
        char* m_ButtonId; //0x0004
        bool m_IsPremium; //0x0008
        char _0x0009[3];
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 04786
    // TypeInfo:  0x02C11FC0
    struct UIButtonSet
    {
        Array<UIButton> m_Buttons; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 04766
    // TypeInfo:  0x02C11EE8
    struct DefaultSelectionItem
    {
        UIDataSourceInfo m_DefaultSelectionQuery; //0x0000
        __int32 m_DefaultSelectionIndex; //0x0010
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 04776
    // TypeInfo:  0x02C11F38
    struct StaticListItem
    {
        char* m_ItemName; //0x0000
        UIDataSourceInfo m_DynamicDisable; //0x0004
        UIDataSourceInfo m_DynamicNewContent; //0x0014
        bool m_AppendPlatformToSID; //0x0024
        bool m_ExcludeInRetail; //0x0025
        char _0x0026[2];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 04780
    // TypeInfo:  0x02C11F74
    struct NestedList
    {
        char* m_Label; //0x0000
        char* m_Index; //0x0004
        UIDataSourceInfo m_ListDataSource; //0x0008
        UIDataSourceInfo m_DynamicShowList; //0x0018
        Array<char*> m_StaticItems; //0x0028
        DefaultSelectionItem m_DefaultHighlighted; //0x002C
        UIListRowType m_RowType; //0x0040
        bool m_UseAsNormalListRows; //0x0044
        bool m_HiddenOnPC; //0x0045
        bool m_HiddenOnXenon; //0x0046
        bool m_HighLightOnUpdate; //0x0047
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 04762
    // TypeInfo:  0x02C11EAC
    struct UIListItem
    {
        char* m_Label; //0x0000
        UIDataSourceInfo m_IsEnabled; //0x0004
        UIDataSourceInfo m_IsVisible; //0x0014
        UIDataSourceInfo m_ToggleItems; //0x0024
        UIDataSourceInfo m_DataUpdate; //0x0034
        bool m_DefaultIsVisible; //0x0044
        bool m_DefaultIsEnabled; //0x0045
        char _0x0046[2];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 04756
    // TypeInfo:  0x02C11E60
    struct UICreditsLine
    {
        UICreditsTextType m_TextType; //0x0000
        char* m_Text; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 04758
    // TypeInfo:  0x02C11E70
    struct UICreditsPage
    {
        char* m_Header1; //0x0000
        char* m_Header2; //0x0004
        Array<UICreditsLine> m_Lines; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 04746
    // TypeInfo:  0x02C11DD8
    struct UIMinimapDistanceFieldParams
    {
        Vec4 m_ColorTint; //0x0000
        Vec4 m_OutlineColor; //0x0010
        float m_DistanceScale; //0x0020
        float m_AlphaThreshold; //0x0024
        float m_OutlineInner; //0x0028
        float m_OutlineOuter; //0x002C
        UIDistanceFieldAsset* m_DistanceField; //0x0030
        char _0x0034[12];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 04748
    // TypeInfo:  0x02C11DE8
    struct UIMinimapData
    {
        Vec2 m_WorldCenter; //0x0000
        char _0x0008[8];
        Vec4 m_CombatAreaColor; //0x0010
        Vec4 m_DetailTextureTint; //0x0020
        Vec2 m_CombatAreaMultiplyWrapAmount; //0x0030
        char _0x0038[8];
        UIMinimapDistanceFieldParams m_Vegetation; //0x0040
        TextureAsset* m_FadeTexture; //0x0080
        float m_StreamingMinimapDelay; //0x0084
        TextureAsset* m_AirRadarFadeTexture; //0x0088
        TextureAsset* m_DetailTexture; //0x008C
        UIMinimapDistanceFieldParams m_Detail; //0x0090
        float m_StreamingMinimapTransitionSpeed; //0x00D0
        float m_WorldRotation; //0x00D4
        float m_WorldRange; //0x00D8
        float m_CombatAreaDistanceScale; //0x00DC
        float m_CombatAreaFadeSpeed; //0x00E0
        float m_CombatAreaAlphaThreshold; //0x00E4
        TextureAsset* m_CombatAreaMultiplyTexture; //0x00E8
        float m_WorldSize; //0x00EC
        float m_AirRadarRange; //0x00F0
        float m_CombatAreaScale; //0x00F4
        bool m_UseStreamingMinimap; //0x00F8
        bool m_UseCombatAreaTexture; //0x00F9
        char _0x00FA[6];
    };//Size=0x0100

    ////////////////////////////////////////
    // RuntimeId: 04738
    // TypeInfo:  0x02C11D7C
    struct UIMinimapIconUv
    {
        Vec2 m_MinUv; //0x0000
        Vec2 m_MaxUv; //0x0008
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 04740
    // TypeInfo:  0x02C11D8C
    struct UIMinimapIconTextureState
    {
        UIIconState m_State; //0x0000
        float m_FrameRate; //0x0004
        Array<UIMinimapIconUv> m_TextureInfos; //0x0008
        bool m_ShouldRotate; //0x000C
        char _0x000D[3];
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 04742
    // TypeInfo:  0x02C11D9C
    struct UIMinimapIconTexture
    {
        UIHudIcon m_IconType; //0x0000
        Array<UIMinimapIconTextureState> m_States; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 04724
    // TypeInfo:  0x02C11C80
    struct UILevelLocation
    {
        char _0x0000[1];
    };//Size=0x0001

    ////////////////////////////////////////
    // RuntimeId: 04704
    // TypeInfo:  0x02C11AE4
    struct GameModeVariation
    {
        char* m_Identifier; //0x0000
        char* m_Name; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 04692
    // TypeInfo:  0x02C119F8
    struct UITextureAtlasInfo
    {
        Vec2 m_MinUv; //0x0000
        Vec2 m_MaxUv; //0x0008
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 04690
    // TypeInfo:  0x02C119E8
    struct UISetValueMessage
    {
        char _0x0000[72];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 04689
    // TypeInfo:  0x02C119D8
    struct UIPopupHideMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 04688
    // TypeInfo:  0x02C119C8
    struct UIPopupShowMessage
    {
        char _0x0000[80];
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 04687
    // TypeInfo:  0x02C119B8
    struct UITriggerPopupHideMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 04686
    // TypeInfo:  0x02C119A8
    struct UITriggerPopupShowMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 04685
    // TypeInfo:  0x02C11998
    struct UIPopupTextInput
    {
        char* m_Id; //0x0000
        char* m_Label; //0x0004
        char* m_DefaultValue; //0x0008
        bool m_IsPassword; //0x000C
        char _0x000D[3];
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 04671
    // TypeInfo:  0x02C1189C
    struct SuppressionReactionData
    {
        float m_SuppressionHighThreshold; //0x0000
        float m_SuppressionLowThreshold; //0x0004
        float m_SuppressionUIThreshold; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 04603
    // TypeInfo:  0x02C11350
    struct MapRotationConfig
    {
        __int32 m_MapRotationId; //0x0000
        char* m_NameSid; //0x0004
        char* m_DescSid; //0x0008
        char* m_Mod; //0x000C
        char* m_GameMode; //0x0010
        bool m_RandomizeStartingMap; //0x0014
        char _0x0015[3];
    };//Size=0x0018

    ////////////////////////////////////////
    // RuntimeId: 04601
    // TypeInfo:  0x02C11340
    struct MapRotataionLookup
    {
        char* m_GameMode; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 04597
    // TypeInfo:  0x02C11320
    struct LockedSettingConfig
    {
        char* m_Key; //0x0000
        unsigned __int32 m_Value; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 04599
    // TypeInfo:  0x02C11330
    struct PresetTypeConfig
    {
        __int32 m_PresetId; //0x0000
        char* m_Key; //0x0004
        char* m_NameSid; //0x0008
        char* m_DescSid; //0x000C
        Array<LockedSettingConfig> m_LockedSettings; //0x0010
        bool m_Predefined; //0x0014
        char _0x0015[3];
    };//Size=0x0018

    ////////////////////////////////////////
    // RuntimeId: 04595
    // TypeInfo:  0x02C11310
    struct SettingConfig
    {
        char* m_Key; //0x0000
        char* m_NameSid; //0x0004
        char* m_DescSid; //0x0008
        unsigned __int32 m_Max; //0x000C
        unsigned __int32 m_Default; //0x0010
        unsigned __int32 m_Min; //0x0014
        bool m_Ranked; //0x0018
        char _0x0019[3];
    };//Size=0x001C

    ////////////////////////////////////////
    // RuntimeId: 04587
    // TypeInfo:  0x02C1127C
    struct StatSpamSetting
    {
        StatEvent m_Event; //0x0000
        unsigned __int32 m_AmountLimit; //0x0004
        float m_TimeLimit; //0x0008
        bool m_PlayerToPlayer; //0x000C
        char _0x000D[3];
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 04579
    // TypeInfo:  0x02C11220
    struct EloFunctionPoint
    {
        float m_XValue; //0x0000
        float m_YValue; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 04581
    // TypeInfo:  0x02C11230
    struct EloExpectedFunctionPoint
    {
        float m_XValue; //0x0000
        float m_YValue; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 04583
    // TypeInfo:  0x02C11240
    struct EloParameters
    {
        Array<EloFunctionPoint> m_KWinner; //0x0000
        Array<EloFunctionPoint> m_KLoser; //0x0004
        Array<EloFunctionPoint> m_KNewbie; //0x0008
        Array<EloFunctionPoint> m_KCompetitor; //0x000C
        Array<EloExpectedFunctionPoint> m_Expected; //0x0010
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 04531
    // TypeInfo:  0x02C10E38
    struct OnlinePlatformConfiguration
    {
        GamePlatform m_Platform; //0x0000
        OnlineServicesAsset* m_Services; //0x0004
        Array<PresenceBackendData*> m_ClientBackends; //0x0008
        Array<ServerBackendData*> m_ServerBackends; //0x000C
        bool m_IsFallback; //0x0010
        char _0x0011[3];
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 04475
    // TypeInfo:  0x02C10AB8
    struct MatchmakingGameSettings
    {
        bool m_OpenToBrowsing; //0x0000
        bool m_OpenToInvites; //0x0001
        bool m_OpenToMatchmaking; //0x0002
        bool m_OpenToJoinByPlayer; //0x0003
        bool m_HostMigratable; //0x0004
        bool m_Ranked; //0x0005
        bool m_AdminOnlyInvites; //0x0006
        bool m_EnforceSingleGroupJoin; //0x0007
        bool m_JoinInProgressSupported; //0x0008
        bool m_AdminInvitesOnlyIgnoreEntryChecks; //0x0009
        bool m_EnablePersistedGameId; //0x000A
        bool m_AllowSameTeamId; //0x000B
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 04473
    // TypeInfo:  0x02C10AA8
    struct MatchmakingGameAttribute
    {
        char* m_Attribute; //0x0000
        char* m_Value; //0x0004
        bool m_Override; //0x0008
        char _0x0009[3];
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 04513
    // TypeInfo:  0x02C10D1C
    struct ServerBackendAttributeMapping
    {
        ServerBackendAttribute m_Attribute; //0x0000
        char* m_Setting; //0x0004
        char* m_DefaultValue; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 04497
    // TypeInfo:  0x02C10C2C
    struct Ps3AgeLevels
    {
        __int32 m_AgeLevel7; //0x0000
        __int32 m_AgeLevel8; //0x0004
        __int32 m_AgeLevel9; //0x0008
        __int32 m_AgeLevel10; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 04499
    // TypeInfo:  0x02C10C3C
    struct Ps3ParentalLockAgeSettingsForCountry
    {
        char* m_CountryCode; //0x0000
        Ps3AgeLevels m_AgeLevels; //0x0004
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 04503
    // TypeInfo:  0x02C10C78
    struct Ps3ParentalLockAgeSettings
    {
        char* m_Region; //0x0000
        Ps3AgeLevels m_AgeLevels; //0x0004
        Ps3ParentalLockAgeSettingsOverrides* m_CountryOverrides; //0x0014
    };//Size=0x0018

    ////////////////////////////////////////
    // RuntimeId: 04495
    // TypeInfo:  0x02C10C1C
    struct Ps3SkuSettings
    {
        char* m_TitleId; //0x0000
        char* m_SpId; //0x0004
        bool m_GrantsOnlinePass; //0x0008
        char _0x0009[3];
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 04477
    // TypeInfo:  0x02C10AC8
    struct MatchmakingCreateGameParameters
    {
        MatchmakingNetworkTopology m_GameTopology; //0x0000
        MatchmakingPeer2PeerMode m_PeerMode; //0x0004
        MatchmakingNetworkTopology m_VoipTopology; //0x0008
        MatchmakingGameSettings m_Settings; //0x000C
        Array<MatchmakingGameAttribute> m_Attributes; //0x0018
        unsigned __int32 m_QueueCapacity; //0x001C
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 04521
    // TypeInfo:  0x02C10DB0
    struct PeerCreateGameParameters
    {
        MatchmakingCreateGameParameters m_Base; //0x0000
        unsigned __int32 m_PlayerCapacity; //0x0020
    };//Size=0x0024

    ////////////////////////////////////////
    // RuntimeId: 04525
    // TypeInfo:  0x02C10DEC
    struct BlazeCreateGameParameters
    {
        MatchmakingCreateGameParameters m_Base; //0x0000
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 04455
    // TypeInfo:  0x02C10A18
    struct MatchmakingSizeConfiguration
    {
        MatchmakingPlatform m_Platform; //0x0000
        Array<char*> m_Settings; //0x0004
        unsigned __int32 m_DesiredPlayerCount; //0x0008
        unsigned __int32 m_MinPlayerCount; //0x000C
        unsigned __int32 m_MaxPlayerCapacity; //0x0010
        char* m_MinFitThreshold; //0x0014
    };//Size=0x0018

    ////////////////////////////////////////
    // RuntimeId: 04469
    // TypeInfo:  0x02C10A88
    struct MatchmakingUserExtendedDataRule
    {
        char* m_Rule; //0x0000
        char* m_MinFitThresHold; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 04465
    // TypeInfo:  0x02C10A68
    struct MatchmakingRuleString
    {
        char* m_Value; //0x0000
        MatchmakingPlatform m_Platform; //0x0004
        Array<char*> m_Licenses; //0x0008
        bool m_UseOnlyIfEmpty; //0x000C
        char _0x000D[3];
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 04467
    // TypeInfo:  0x02C10A78
    struct MatchmakingGenericRule
    {
        char* m_Rule; //0x0000
        char* m_MinFitThresHold; //0x0004
        char* m_Setting; //0x0008
        Array<MatchmakingRuleString> m_DesiredValues; //0x000C
        bool m_IgnoreIfDefault; //0x0010
        bool m_MergeValues; //0x0011
        bool m_SortValues; //0x0012
        char _0x0013[1];
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 04463
    // TypeInfo:  0x02C10A58
    struct MatchmakingRankedRule
    {
        char* m_MinFitThreshold; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 04461
    // TypeInfo:  0x02C10A48
    struct MatchmakingPingSiteRule
    {
        char* m_MinFitThreshold; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 04457
    // TypeInfo:  0x02C10A28
    struct MatchmakingSizeRule
    {
        char* m_Setting; //0x0000
        Array<MatchmakingSizeConfiguration> m_Configurations; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 04453
    // TypeInfo:  0x02C10A08
    struct MatchmakingVirtualizedRule
    {
        char* m_MinFitThreshold; //0x0000
        MatchmakingVirtualizationMode m_VirtualizationMode; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 04471
    // TypeInfo:  0x02C10A98
    struct MatchmakingCriteria
    {
        MatchmakingSizeRule m_SizeRule; //0x0000
        MatchmakingPingSiteRule m_PingSiteRule; //0x0008
        MatchmakingRankedRule m_RankedRule; //0x000C
        Array<MatchmakingGenericRule> m_GenericRules; //0x0010
        MatchmakingVirtualizedRule m_VirtualizedRule; //0x0014
        Array<MatchmakingUserExtendedDataRule> m_UEDRules; //0x001C
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 04441
    // TypeInfo:  0x02C1098C
    struct ManualDataEntry
    {
        GamePlatform m_Platform; //0x0000
        LanguageFormat m_Language; //0x0004
        Array<TextureAsset*> m_Textures; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 04433
    // TypeInfo:  0x02C10914
    struct InspectAnimationTriggerData
    {
        AntRef m_Animation; //0x0000
        float m_Probability; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 04435
    // TypeInfo:  0x02C10924
    struct InspectViewPointData
    {
        char* m_ViewPointID; //0x0000
        Array<InspectAnimationTriggerData> m_AnimationTriggers; //0x0004
        Array<AntRef> m_ContinuousAnimationSignal; //0x0008
        float m_LookAtHeight; //0x000C
        float m_Yaw; //0x0010
        float m_Pitch; //0x0014
        float m_Distance; //0x0018
        float m_FovOffset; //0x001C
        float m_AdjustmentYaw; //0x0020
        bool m_Fixed; //0x0024
        char _0x0025[3];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 04429
    // TypeInfo:  0x02C108D8
    struct LicenseInfo
    {
        char* m_Name; //0x0000
        GamePlatform m_StagingPlatform; //0x0004
        bool m_AllowStaging; //0x0008
        bool m_ClientOnly; //0x0009
        char _0x000A[2];
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 04419
    // TypeInfo:  0x02C10818
    struct Consumable
    {
        Guid m_Instance; //0x0000
        char _0x0000[16];
        ConsumableGroup m_Group; //0x0010
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 04413
    // TypeInfo:  0x02C107B0
    struct UIGPSPosition
    {
        double m_Latitude; //0x0000
        double m_Longitude; //0x0008
        double m_SeaLevelOffset; //0x0010
    };//Size=0x0018

    ////////////////////////////////////////
    // RuntimeId: 04411
    // TypeInfo:  0x02C107A0
    struct UILevelStatData
    {
        char* m_StatEasy; //0x0000
        char* m_StatMedium; //0x0004
        char* m_StatHard; //0x0008
        char* m_StatHardcore; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 04405
    // TypeInfo:  0x02C10738
    struct UIGeoLongitude
    {
        __int32 m_Degrees; //0x0000
        __int32 m_Minuites; //0x0004
        __int32 m_Seconds; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 04403
    // TypeInfo:  0x02C10728
    struct UIGeoLatitude
    {
        __int32 m_Degrees; //0x0000
        __int32 m_Minuites; //0x0004
        __int32 m_Seconds; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 04399
    // TypeInfo:  0x02C106EC
    struct GameTipData
    {
        char* m_Text; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 04389
    // TypeInfo:  0x02C10664
    struct GameModeTeamSize
    {
        unsigned __int32 m_PlayerCount; //0x0000
        unsigned __int32 m_SquadSize; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 04391
    // TypeInfo:  0x02C10674
    struct GameModeSize
    {
        char* m_Name; //0x0000
        char* m_ShortName; //0x0004
        char* m_MetaIdentifier; //0x0008
        unsigned __int32 m_PlayerCount; //0x000C
        Array<GameModeTeamSize> m_Teams; //0x0010
        unsigned __int32 m_RoundsPerMap; //0x0014
        bool m_ForceSquad; //0x0018
        char _0x0019[3];
    };//Size=0x001C

    ////////////////////////////////////////
    // RuntimeId: 04393
    // TypeInfo:  0x02C10684
    struct GameModeInformation
    {
        GamePlatform m_Platform; //0x0000
        Array<GameModeSize> m_Sizes; //0x0004
        GameModeSize m_DefaultSize; //0x0008
        bool m_AllowFallbackToDefault; //0x0024
        char _0x0025[3];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 04369
    // TypeInfo:  0x02C10500
    struct BFServerConfigurationData
    {
        unsigned __int32 m_ScoreMultiplier; //0x0000
        char* m_Licenses; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 04371
    // TypeInfo:  0x02C10510
    struct BFServerConfigurationSchedule
    {
        Array<char*> m_Licenses; //0x0000
        Array<char*> m_Levels; //0x0004
        BFServerConfigurationData m_Data; //0x0008
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 04361
    // TypeInfo:  0x02C1046C
    struct AdvertisementZoneMember
    {
        char* m_Identifier; //0x0000
        char* m_Filename; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 04289
    // TypeInfo:  0x02C0D680
    struct UISettingsItem
    {
        char* m_NameSid; //0x0000
        UISettingSymbols m_StartEndSymbols; //0x0004
        ProfileOptionData* m_Setting; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 04291
    // TypeInfo:  0x02C0D690
    struct UISettingsGroup
    {
        char* m_NameSid; //0x0000
        __int32 m_ScreenColumn; //0x0004
        Array<UISettingsItem> m_SettingsItems; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 04293
    // TypeInfo:  0x02C0D6A0
    struct UISettingsPage
    {
        char* m_NameSid; //0x0000
        Array<UISettingsGroup> m_SettingsGroups; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 04279
    // TypeInfo:  0x02C0D5F8
    struct MessageInfo
    {
        char* m_RowTypeName; //0x0000
        unsigned __int32 m_MessageQueueSize; //0x0004
        float m_NormalMessageTime; //0x0008
        float m_ShortMessageTime; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 04267
    // TypeInfo:  0x02C0D50C
    struct ScreenshotInfo
    {
        char* m_Name; //0x0000
        unsigned __int32 m_CropImageY1; //0x0004
        unsigned __int32 m_CropImageX1; //0x0008
        unsigned __int32 m_CropImageX2; //0x000C
        unsigned __int32 m_CropImageY2; //0x0010
        unsigned __int32 m_ResizeOutputImageHeight; //0x0014
        unsigned __int32 m_ResizeOutputImageWidth; //0x0018
        bool m_ResizeOutputImage; //0x001C
        bool m_CropImage; //0x001D
        char _0x001E[2];
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 04235
    // TypeInfo:  0x02C0D268
    struct UILeaderboardData
    {
        char* m_Name; //0x0000
        char* m_Description; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 04213
    // TypeInfo:  0x02C0D0A0
    struct ServerInfoSetting
    {
        char* m_Key; //0x0000
        char* m_Name; //0x0004
        bool m_IsBool; //0x0008
        char _0x0009[3];
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 04161
    // TypeInfo:  0x02C0CC60
    struct DataSource
    {
        UIComponentData* m_Category; //0x0000
        __int32 m_Value; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 04113
    // TypeInfo:  0x02C0C878
    struct SoldierToComponentsOnIncapableMessage
    {
        char _0x0000[80];
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 04112
    // TypeInfo:  0x02C0C868
    struct SoldierToComponentsReviveMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 04109
    // TypeInfo:  0x02C0C82C
    struct RotateToHitData
    {
        float m_RotationSpeedFactor; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 04097
    // TypeInfo:  0x02C0C75C
    struct SpawnScreenWeaponData
    {
        char* m_WeaponName; //0x0000
        TextureAsset* m_Texture; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 04081
    // TypeInfo:  0x02C0C634
    struct VehicleTrackerRenderTargetData
    {
        Vec2 m_PositionOffset; //0x0000
        __int32 m_RenderTargetIndex; //0x0008
        float m_RenderTargetApectRatio; //0x000C
        float m_ScaleSize; //0x0010
        bool m_UseRenderTarget; //0x0014
        char _0x0015[3];
    };//Size=0x0018

    ////////////////////////////////////////
    // RuntimeId: 04079
    // TypeInfo:  0x02C0C624
    struct EdgePointerData
    {
        float m_NegativeXLimit; //0x0000
        float m_PositiveXLimit; //0x0004
        float m_NegativeYLimit; //0x0008
        float m_PositiveYLimit; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 04075
    // TypeInfo:  0x02C0C5E8
    struct SubScreenData
    {
        Vec3 m_ScreenPosition; //0x0000
        float m_ProjectionAngleY; //0x0010
        float m_ProjectionAngleZ; //0x0014
        float m_ProjectionAngleX; //0x0018
        float m_ScreenSize; //0x001C
        float m_IconSize; //0x0020
        float m_ScaleIconSizeByDepthStrength; //0x0024
        float m_RenderTargetApectRatio; //0x0028
        __int32 m_RenderTargetIndex; //0x002C
        bool m_UseRenderTarget; //0x0030
        char _0x0031[15];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 04071
    // TypeInfo:  0x02C0C5AC
    struct UITaggedVehicleCrosshair
    {
        float m_MinDistance; //0x0000
        float m_MaxDistance; //0x0004
        __int32 m_MinSize; //0x0008
        __int32 m_MaxSize; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 04057
    // TypeInfo:  0x02C0C4E8
    struct MinimapData
    {
        Vec2 m_Position; //0x0000
        char _0x0008[8];
        Vec3 m_CameraPosition; //0x0010
        Vec2 m_Size; //0x0020
        char _0x0028[8];
        Vec3 m_OverlayColor; //0x0030
        float m_AnchorPosVertical; //0x0040
        float m_AnchorPosHorizontal; //0x0044
        float m_CameraLookDistance; //0x0048
        float m_CameraDistance; //0x004C
        float m_CameraFov; //0x0050
        float m_OverlayAlpha; //0x0054
        float m_CameraRotation; //0x0058
        __int32 m_StartZoomLevel; //0x005C
        __int32 m_InnerZoomFactor; //0x0060
        __int32 m_MaxZoomLevels; //0x0064
        bool m_CenterOnCombatArea; //0x0068
        bool m_RotationFromPlayer; //0x0069
        bool m_PositionFromPlayer; //0x006A
        char _0x006B[5];
    };//Size=0x0070

    ////////////////////////////////////////
    // RuntimeId: 04059
    // TypeInfo:  0x02C0C4F8
    struct UIMinimapZoomConfig
    {
        UIMinimapZoomState m_State; //0x0000
        char _0x0004[12];
        MinimapData m_Data; //0x0010
    };//Size=0x0080

    ////////////////////////////////////////
    // RuntimeId: 04061
    // TypeInfo:  0x02C0C508
    struct UIMinimapConfig
    {
        float m_CombatAreaCameraDelay; //0x0000
        float m_CameraInterpolationTime; //0x0004
        float m_CombatAreaFadeSpeed; //0x0008
        Array<UIMinimapZoomConfig> m_ZoomStates; //0x000C
        float m_CameraPanSensitivity; //0x0010
        float m_MinVelocity; //0x0014
        float m_MaxVelocity; //0x0018
        float m_VelocityModifier; //0x001C
        bool m_DisableDefaultState; //0x0020
        char _0x0021[3];
    };//Size=0x0024

    ////////////////////////////////////////
    // RuntimeId: 04015
    // TypeInfo:  0x02C0A594
    struct GunSwayDispersionModData
    {
        float m_MinAngleModifier; //0x0000
        float m_MaxAngleModifier; //0x0004
        float m_IncreasePerShotModifier; //0x0008
        float m_DecreasePerSecondModifier; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 04013
    // TypeInfo:  0x02C0A584
    struct GunSwayDispersionData
    {
        float m_MinAngle; //0x0000
        float m_MaxAngle; //0x0004
        float m_IncreasePerShot; //0x0008
        float m_DecreasePerSecond; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 04021
    // TypeInfo:  0x02C0A5E0
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
    // RuntimeId: 04017
    // TypeInfo:  0x02C0A5A4
    struct GunSwayLagData
    {
        float m_MoveStrafeModifier; //0x0000
        float m_MoveForwardModifier; //0x0004
        float m_RotateYawModifier; //0x0008
        float m_RotatePitchModifier; //0x000C
        float m_ReleaseModifier; //0x0010
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 04027
    // TypeInfo:  0x02C0A610
    struct GunSwayBaseMoveData
    {
        GunSwayDispersionData m_BaseValue; //0x0000
        GunSwayDispersionData m_Moving; //0x0010
        GunSwayRecoilData m_Recoil; //0x0020
        GunSwayLagData m_GunSwayLag; //0x0038
    };//Size=0x004C

    ////////////////////////////////////////
    // RuntimeId: 04025
    // TypeInfo:  0x02C0A600
    struct GunSwayBaseData
    {
        GunSwayDispersionData m_BaseValue; //0x0000
        GunSwayRecoilData m_Recoil; //0x0010
        GunSwayLagData m_GunSwayLag; //0x0028
    };//Size=0x003C

    ////////////////////////////////////////
    // RuntimeId: 04023
    // TypeInfo:  0x02C0A5F0
    struct GunSwayStanceTransition
    {
        GunSwayDispersionData m_MaxPenaltyValue; //0x0000
        float m_CoolDown; //0x0010
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 04029
    // TypeInfo:  0x02C0A620
    struct GunSwayBaseMoveJumpData
    {
        GunSwayDispersionData m_BaseValue; //0x0000
        GunSwayDispersionData m_Moving; //0x0010
        GunSwayDispersionData m_Jumping; //0x0020
        GunSwayDispersionData m_Sprinting; //0x0030
        GunSwayDispersionData m_VaultingSmallObject; //0x0040
        GunSwayDispersionData m_VaultingMediumObject; //0x0050
        GunSwayRecoilData m_Recoil; //0x0060
        GunSwayLagData m_GunSwayLag; //0x0078
    };//Size=0x008C

    ////////////////////////////////////////
    // RuntimeId: 04031
    // TypeInfo:  0x02C0A630
    struct GunSwayStandData
    {
        GunSwayBaseMoveJumpData m_NoZoom; //0x0000
        GunSwayBaseMoveJumpData m_Zoom; //0x008C
    };//Size=0x0118

    ////////////////////////////////////////
    // RuntimeId: 04035
    // TypeInfo:  0x02C0A650
    struct GunSwayStanceZoomModifierData
    {
        GunSwayDispersionModData m_DispersionMod; //0x0000
        float m_RecoilMagnitudeMod; //0x0010
        float m_RecoilAngleMod; //0x0014
        float m_LagYawMod; //0x0018
        float m_LagPitchMod; //0x001C
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 04033
    // TypeInfo:  0x02C0A640
    struct GunSwayCrouchProneData
    {
        GunSwayBaseMoveData m_NoZoom; //0x0000
        GunSwayBaseMoveData m_Zoom; //0x004C
    };//Size=0x0098

    ////////////////////////////////////////
    // RuntimeId: 04009
    // TypeInfo:  0x02C0A548
    struct WeaponLagBinding
    {
        AntRef m_EntityPitch; //0x0000
        AntRef m_EntityYaw; //0x0004
        AntRef m_PitchResult; //0x0008
        AntRef m_YawResult; //0x000C
        AntRef m_RollResult; //0x0010
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 04001
    // TypeInfo:  0x02C0A508
    struct NetworkSetActiveWeaponSlotMessage
    {
        char _0x0000[56];
        __int32 m_PlayerId; //0x0038
        WeaponSlot m_Slot; //0x003C
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 04000
    // TypeInfo:  0x02C0A4F8
    struct BulletToSoldierComponentsHitMessage
    {
        char _0x0000[96];
    };//Size=0x0060

    ////////////////////////////////////////
    // RuntimeId: 03999
    // TypeInfo:  0x02C0A4E8
    struct BulletHitInfo
    {
        Vec3 m_Direction; //0x0000
        Vec3 m_SpawnPosition; //0x0010
        SoldierWeaponUnlockAsset* m_WeaponUnlockAsset; //0x0020
        AntHitReactionWeaponType m_WeaponType; //0x0024
        __int32 m_BoneType; //0x0028
        __int32 m_ShooterPlayerId; //0x002C
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 03991
    // TypeInfo:  0x02C0A470
    struct SupplyUnitSphereData
    {
        float m_Radius; //0x0000
        float m_SupplyIncSpeed; //0x0004
        float m_SupplyPointsCapacity; //0x0008
        float m_SupplyPointsRefillSpeed; //0x000C
        bool m_InfiniteCapacity; //0x0010
        char _0x0011[3];
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 03993
    // TypeInfo:  0x02C0A480
    struct SupplyData
    {
        SupplyUnitSphereData m_Healing; //0x0000
        SupplyUnitSphereData m_Ammo; //0x0014
        bool m_SupplyVehicles; //0x0028
        bool m_SupplySoldiers; //0x0029
        bool m_TeamSpecific; //0x002A
        bool m_ExcludeSelf; //0x002B
    };//Size=0x002C

    ////////////////////////////////////////
    // RuntimeId: 03987
    // TypeInfo:  0x02C0A434
    struct WeaponOverrideValue
    {
        WeaponOverrideValueType m_ValueType; //0x0000
        float m_Value; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03977
    // TypeInfo:  0x02C0A3AC
    struct WeaponModifier
    {
        __int32 m_DummyToMakeFrostEDcompile; //0x0000
        char _0x0004[228];
    };//Size=0x00E8

    ////////////////////////////////////////
    // RuntimeId: 03975
    // TypeInfo:  0x02C0A39C
    struct WeaponModifierData
    {
        UnlockAssetBase* m_UnlockAsset; //0x0000
        Array<WeaponModifierBase*> m_Modifiers; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03945
    // TypeInfo:  0x02C0A140
    struct WeaponMiscModifierSettings
    {
        bool m_EnableBreathControl; //0x0000
        bool m_CanBeInSupportedShooting; //0x0001
        bool m_UnZoomOnBoltAction; //0x0002
        bool m_HoldBoltActionUntilZoomRelease; //0x0003
        bool m_IsSilenced; //0x0004
    };//Size=0x0005

    ////////////////////////////////////////
    // RuntimeId: 03937
    // TypeInfo:  0x02C0A0AC
    struct PickupSettingsData
    {
        Vec3 m_MeshRenderOffset; //0x0000
        Vec3 m_MeshRenderRotation; //0x0010
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 00019
    // TypeInfo:  0x028875D0
    struct LinearTransform
    {
        Vec3 m_right; //0x0000
        Vec3 m_up; //0x0010
        Vec3 m_forward; //0x0020
        Vec3 m_trans; //0x0030
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 03929
    // TypeInfo:  0x02C0A06C
    struct RigidMeshSocketTransform
    {
        LinearTransform m_Transform; //0x0000
        WeaponRegularSocketObjectData* m_SocketObject; //0x0040
        char _0x0044[12];
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 03925
    // TypeInfo:  0x02C0A04C
    struct AnimationConfigurationShootModuleData
    {
        float m_ZoomedKickbackFactor; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 03927
    // TypeInfo:  0x02C0A05C
    struct AnimationConfigurationData
    {
        AnimationConfigurationShootModuleData m_ShootModuleData; //0x0000
        WeaponOffsetData* m_WeaponOffsetModuleData; //0x0004
        WeaponSpeedData* m_WeaponSpeedModuleData; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 03915
    // TypeInfo:  0x02C09FC4
    struct WeaponLagEffectForceData
    {
        Vec3 m_OffsetForce; //0x0000
        Vec3 m_RotationForce; //0x0010
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 03911
    // TypeInfo:  0x02C09FA4
    struct WeaponLagEffectSpringData
    {
        float m_Constant; //0x0000
        float m_Damping; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03913
    // TypeInfo:  0x02C09FB4
    struct WeaponLagEffectSpringVector
    {
        WeaponLagEffectSpringData m_SpringX; //0x0000
        WeaponLagEffectSpringData m_SpringY; //0x0008
        WeaponLagEffectSpringData m_SpringZ; //0x0010
    };//Size=0x0018

    ////////////////////////////////////////
    // RuntimeId: 03903
    // TypeInfo:  0x02C09F10
    struct ZoomLevelSpecificTransitionTime
    {
        __int32 m_FromZoomLevel; //0x0000
        __int32 m_ToZoomLevel; //0x0004
        float m_ZoomTransitionTime; //0x0008
        float m_FovTransitionTime; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 03901
    // TypeInfo:  0x02C09F00
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
    // RuntimeId: 03849
    // TypeInfo:  0x02C09AF8
    struct RumbleFiringData
    {
        float m_LowRumble; //0x0000
        float m_HighRumble; //0x0004
        float m_RumbleDuration; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 03843
    // TypeInfo:  0x02C09AAC
    struct FireEffectData
    {
        Vec3 m_Rotation; //0x0000
        Vec3 m_Offset; //0x0010
        Vec3 m_ZoomRotation; //0x0020
        Vec3 m_ZoomOffset; //0x0030
        EffectBlueprint* m_Effect; //0x0040
        bool m_UseZoomOffset; //0x0044
        bool m_UseZoomRotation; //0x0045
        bool m_DisableDuringZoom; //0x0046
        bool m_UpdateTransform; //0x0047
        bool m_StopLoopingEffects; //0x0048
        char _0x0049[7];
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 03845
    // TypeInfo:  0x02C09ABC
    struct OverHeatData
    {
        float m_HeatPerBullet; //0x0000
        float m_HeatDropPerSecond; //0x0004
        float m_OverHeatPenaltyTime; //0x0008
        float m_OverHeatThreshold; //0x000C
        FireEffectData m_OverHeatEffect; //0x0010
    };//Size=0x0060

    ////////////////////////////////////////
    // RuntimeId: 03839
    // TypeInfo:  0x02C09A8C
    struct FiringDispersionData
    {
        float m_MinAngle; //0x0000
        float m_MaxAngle; //0x0004
        float m_IncreasePerShot; //0x0008
        float m_DecreasePerSecond; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 03841
    // TypeInfo:  0x02C09A9C
    struct SoldierWeaponDispersion
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
    // RuntimeId: 03825
    // TypeInfo:  0x02C09A1C
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
    // RuntimeId: 03835
    // TypeInfo:  0x02C09A6C
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
    // RuntimeId: 03827
    // TypeInfo:  0x02C09A2C
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
    // RuntimeId: 03837
    // TypeInfo:  0x02C09A7C
    struct FireLogicData
    {
        HoldAndReleaseData m_HoldAndRelease; //0x0000
        BoltActionData m_BoltAction; //0x001C
        RecoilData m_Recoil; //0x002C
        EntryInputActionEnum m_FireInputAction; //0x0050
        EntryInputActionEnum m_ReloadInputAction; //0x0054
        EntryInputActionEnum m_CycleFireModeInputAction; //0x0058
        float m_TriggerPullWeight; //0x005C
        float m_RateOfFire; //0x0060
        float m_RateOfFireForBurst; //0x0064
        float m_ClientFireRateMultiplier; //0x0068
        float m_ReloadDelay; //0x006C
        Array<FireLogicType> m_FireLogicTypeArray; //0x0070
        float m_ReloadThreshold; //0x0074
        float m_PreFireDelay; //0x0078
        float m_ReloadTime; //0x007C
        float m_ReloadTimeBulletsLeft; //0x0080
        FireLogicType m_FireLogicType; //0x0084
        ReloadLogic m_ReloadLogic; //0x0088
        float m_AutomaticDelay; //0x008C
        ReloadType m_ReloadType; //0x0090
        bool m_HoldOffReloadUntilZoomRelease; //0x0094
        bool m_ForceReloadActionOnFireTrigger; //0x0095
        bool m_HoldOffReloadUntilFireRelease; //0x0096
        bool m_AlwaysAutoReload; //0x0097
    };//Size=0x0098

    ////////////////////////////////////////
    // RuntimeId: 03821
    // TypeInfo:  0x02C099FC
    struct UnlockableProjectile
    {
        Vec3 m_InitialSpeed; //0x0000
        ProjectileEntityData* m_ProjectileData; //0x0010
        ProjectileBlueprint* m_Projectile; //0x0014
        UnlockAssetBase* m_Unlock; //0x0018
        char _0x001C[4];
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 03823
    // TypeInfo:  0x02C09A0C
    struct ShotConfigData
    {
        Vec3 m_InitialPosition; //0x0000
        Vec3 m_InitialDirection; //0x0010
        Vec3 m_InitialSpeed; //0x0020
        float m_InheritWeaponSpeedAmount; //0x0030
        ExplosionEntityData* m_MuzzleExplosion; //0x0034
        ProjectileEntityData* m_ProjectileData; //0x0038
        ProjectileEntityData* m_SecondaryProjectileData; //0x003C
        ProjectileBlueprint* m_Projectile; //0x0040
        ProjectileBlueprint* m_SecondaryProjectile; //0x0044
        Array<UnlockableProjectile> m_AlternateProjectiles; //0x0048
        float m_SpawnDelay; //0x004C
        unsigned __int32 m_NumberOfBulletsPerShell; //0x0050
        unsigned __int32 m_NumberOfBulletsPerShot; //0x0054
        unsigned __int32 m_NumberOfBulletsPerBurst; //0x0058
        bool m_RelativeTargetAiming; //0x005C
        bool m_ForceSpawnToCamera; //0x005D
        bool m_SpawnVisualAtWeaponBone; //0x005E
        bool m_ActiveForceSpawnToCamera; //0x005F
    };//Size=0x0060

    ////////////////////////////////////////
    // RuntimeId: 03813
    // TypeInfo:  0x02C09968
    struct HealingSphereData
    {
        float m_Radius; //0x0000
        float m_HealthIncSpeed; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03811
    // TypeInfo:  0x02C09958
    struct HealerData
    {
        float m_Radius; //0x0000
        float m_Health; //0x0004
        float m_HealingTime; //0x0008
        float m_PickUpDelay; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 03807
    // TypeInfo:  0x02C0991C
    struct SoldierDetonationData
    {
        float m_Angle; //0x0000
        float m_MinSpeedForActivation; //0x0004
        float m_Radius; //0x0008
        float m_SoldierDetonationActivationDelay; //0x000C
        bool m_UseAngle; //0x0010
        bool m_HasSoldierDetonation; //0x0011
        char _0x0012[2];
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 03803
    // TypeInfo:  0x02C098E0
    struct NearTargetDetonationData
    {
        float m_DetonationRadius; //0x0000
        float m_MinDetonationDelay; //0x0004
        float m_MaxDetonationDelay; //0x0008
        bool m_DetonateNearTarget; //0x000C
        char _0x000D[3];
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 03801
    // TypeInfo:  0x02C098D0
    struct MissileUnguidedData
    {
        Vec2 m_StaticPosition; //0x0000
        Vec2 m_TargetPositionOffset; //0x0008
        bool m_UseTargetPosition; //0x0010
        bool m_UseStaticPosition; //0x0011
        char _0x0012[2];
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 03799
    // TypeInfo:  0x02C098C0
    struct MissileLockableInfoData
    {
        float m_HeatSignature; //0x0000
        float m_RadarSignature; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03795
    // TypeInfo:  0x02C09884
    struct ZoomLevelLockData
    {
        float m_OutlineTaggedDistance; //0x0000
        LockType m_LockType; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03777
    // TypeInfo:  0x02C09768
    struct AmmoConfigData
    {
        __int32 m_MagazineCapacity; //0x0000
        __int32 m_NumberOfMagazines; //0x0004
        unsigned __int32 m_TraceFrequency; //0x0008
        unsigned __int32 m_AmmoPickupMinAmount; //0x000C
        unsigned __int32 m_AmmoPickupMaxAmount; //0x0010
        float m_AutoReplenishDelay; //0x0014
        __int32 m_AmmoBagPickupAmount; //0x0018
        float m_AmmoBagPickupDelayMultiplier; //0x001C
        bool m_AutoReplenishMagazine; //0x0020
        char _0x0021[3];
    };//Size=0x0024

    ////////////////////////////////////////
    // RuntimeId: 03759
    // TypeInfo:  0x02C09630
    struct CannedAnimationBinding
    {
        AntRef m_LevelIndex; //0x0000
        AntRef m_ScenarioIndex; //0x0004
        AntRef m_ActorIndex; //0x0008
        AntRef m_PartIndex; //0x000C
        AntRef m_TriggerCannedAnimation; //0x0010
        AntRef m_ExitCannedAnimation; //0x0014
        AntRef m_ExternalTime; //0x0018
        AntRef m_BlendValue; //0x001C
        AntRef m_AdvanceScenario; //0x0020
        AntRef m_EnteredLoop; //0x0024
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 03757
    // TypeInfo:  0x02C09620
    struct WarpAnimationBinding
    {
        AntRef m_ConnectJointGroup; //0x0000
        AntRef m_ConnectJointWeight; //0x0004
        AntRef m_EnableAlign; //0x0008
        AntRef m_DisableCulling; //0x000C
        AntRef m_TranslationYWarpTolerance; //0x0010
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 03699
    // TypeInfo:  0x02C0763C
    struct AntInteractionBinding
    {
        AntRef m_OtherAnimatableID; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 03647
    // TypeInfo:  0x02C07234
    struct ContinueWeaponAmmoData
    {
        __int32 m_MinMags; //0x0000
        __int32 m_WeaponSlot; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03631
    // TypeInfo:  0x02C07128
    struct MessageLineData
    {
        char* m_MessageSid; //0x0000
        EntryInputActionEnum m_EntryInputAction; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03579
    // TypeInfo:  0x02C06D90
    struct AreaValueTeam
    {
        __int32 m_AreaValue; //0x0000
        TeamId m_Team; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03573
    // TypeInfo:  0x02C06D28
    struct TicketCountPercentage
    {
        __int32 m_MaxPercentOfTicketsLeft; //0x0000
        __int32 m_TicketLossPerMin; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03553
    // TypeInfo:  0x02C06BA8
    struct AnimatedSkeletonDatabaseItem
    {
        SkeletonAsset* m_Asset; //0x0000
        Array<__int32> m_Names; //0x0004
        SkeletonCollisionData* m_SoldierCollision; //0x0008
        SkeletonCollisionData* m_RagdollCollision; //0x000C
        RagdollAsset* m_Ragdoll; //0x0010
        Array<char*> m_SpecialBones; //0x0014
        char* m_HipBone; //0x0018
        char* m_HeadBone; //0x001C
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 03543
    // TypeInfo:  0x02C06AE8
    struct PathfindingBlob
    {
        Guid m_BlobId; //0x0000
        char _0x0000[16];
        unsigned __int32 m_BlobSize; //0x0010
        Array<unsigned __int32> m_ChunkSizes; //0x0014
    };//Size=0x0018

    ////////////////////////////////////////
    // RuntimeId: 03535
    // TypeInfo:  0x02C06A54
    struct FaceAnimationWaveMapping
    {
        __int32 m_WaveNameHash; //0x0000
        __int32 m_AntEnumValue; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03525
    // TypeInfo:  0x02C06994
    struct LevelDescriptionInclusionCategory
    {
        char* m_Category; //0x0000
        Array<char*> m_Mode; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03521
    // TypeInfo:  0x02C06958
    struct LevelDescription
    {
        char* m_Name; //0x0000
        char* m_Description; //0x0004
        Array<LevelDescriptionComponent*> m_Components; //0x0008
        bool m_IsCoop; //0x000C
        bool m_IsMenu; //0x000D
        bool m_IsMultiplayer; //0x000E
        char _0x000F[1];
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 03517
    // TypeInfo:  0x02C0691C
    struct MinimapCameraOffsetData
    {
        Vec2 m_Position; //0x0000
        float m_CameraFov; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 03485
    // TypeInfo:  0x02C051D4
    struct BlueprintBundleClientEntitiesCreatedMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 03484
    // TypeInfo:  0x02C051C4
    struct BlueprintBundleClientBundleUnloadedMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 03483
    // TypeInfo:  0x02C051B4
    struct BlueprintBundleClientBundleLoadedMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 03482
    // TypeInfo:  0x02C051A4
    struct BlueprintBundleClientOnStreamedInMessage
    {
        char _0x0000[56];
        char* m_BundleName; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 03481
    // TypeInfo:  0x02C05194
    struct BlueprintBundleClientLoadBundleMessage
    {
        char _0x0000[56];
        char* m_BundleName; //0x0038
        bool m_WaitForLoad; //0x003C
        char _0x003D[3];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 03480
    // TypeInfo:  0x02C05184
    struct BlueprintBundleStateChangeMessageBase
    {
        char _0x0000[56];
        char* m_BundleName; //0x0038
        unsigned __int32 m_ProxyEntityBusNetworkId; //0x003C
        bool m_StreamedIn; //0x0040
        char _0x0041[7];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 03467
    // TypeInfo:  0x02C0506C
    struct AwardStarNameInstance
    {
        char* m_Name; //0x0000
        StatsCategoryBaseData* m_Category; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03459
    // TypeInfo:  0x02C04FD8
    struct EventScaleData
    {
        StatEvent m_Event; //0x0000
        float m_Scale; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03457
    // TypeInfo:  0x02C04FC8
    struct CriteriaAward
    {
        AwardData* m_Award; //0x0000
        unsigned __int32 m_Count; //0x0004
        char* m_Sid; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 03451
    // TypeInfo:  0x02C04F60
    struct CriteriaStarCategoryInstance
    {
        char* m_Name; //0x0000
        StatsCategoryBaseData* m_ParamX; //0x0004
        StatsCategoryBaseData* m_ParamY; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 00178
    // TypeInfo:  0x02BE51AC
    struct EventSpec
    {
        __int32 m_Id; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 03433
    // TypeInfo:  0x02C04E7C
    struct SoldierHealthModuleBinding
    {
        AntRef m_HeadShot; //0x0000
        AntRef m_LeftArmHit; //0x0004
        AntRef m_RightArmHit; //0x0008
        AntRef m_LeftLegHit; //0x000C
        AntRef m_RightLegHit; //0x0010
        AntRef m_OnGround; //0x0014
        AntRef m_DeathAnimationTriggered; //0x0018
        AntRef m_RandomAnimationIndex; //0x001C
        AntRef m_Sprinting; //0x0020
        AntRef m_HitLeft; //0x0024
        AntRef m_HitRight; //0x0028
        AntRef m_HitFront; //0x002C
        AntRef m_HitBack; //0x0030
        AntRef m_DeathHitDirection; //0x0034
        AntRef m_Explosion; //0x0038
        AntRef m_Dead; //0x003C
        AntRef m_Revived; //0x0040
        AntRef m_RandomValue; //0x0044
        AntRef m_Pose; //0x0048
        AntRef m_RightSpeed; //0x004C
        AntRef m_ForwardSpeed; //0x0050
        AntRef m_CriticallyHit; //0x0054
        AntRef m_InteractiveManDown; //0x0058
    };//Size=0x005C

    ////////////////////////////////////////
    // RuntimeId: 03427
    // TypeInfo:  0x02C04E30
    struct AnimatedWeaponBinding
    {
        AntRef m_Deploy; //0x0000
        AntRef m_AltDeploy; //0x0004
        AntRef m_Undeploy; //0x0008
        AntRef m_QuickSwitch; //0x000C
        AntRef m_Reload; //0x0010
        AntRef m_ReloadShotgun; //0x0014
        AntRef m_Fire; //0x0018
        AntRef m_FireSingle; //0x001C
        AntRef m_FireHoldAndRelease; //0x0020
        AntRef m_FireSimple; //0x0024
        AntRef m_FirstShotSpawned; //0x0028
        AntRef m_BoltAction; //0x002C
        AntRef m_PumpAction; //0x0030
        AntRef m_MeleeAttack; //0x0034
        AntRef m_QuickThrow; //0x0038
        AntRef m_QuickThrowType; //0x003C
        AntRef m_AimBody; //0x0040
        AntRef m_AlwaysAimHead; //0x0044
        AntRef m_OneHanded; //0x0048
        AntRef m_OneHandedAiming; //0x004C
        AntRef m_AimingEnabled; //0x0050
        AntRef m_LowerGun; //0x0054
        AntRef m_BreathControl; //0x0058
        AntRef m_RflType; //0x005C
        AntRef m_PstlType; //0x0060
        AntRef m_HgrType; //0x0064
        AntRef m_ATType; //0x0068
        AntRef m_ShgType; //0x006C
        AntRef m_LMGType; //0x0070
        AntRef m_BagType; //0x0074
        AntRef m_SnpType; //0x0078
        AntRef m_Zoom; //0x007C
        AntRef m_AimBodyWeight; //0x0080
        AntRef m_DisableZoomToggleWeight; //0x0084
        AntRef m_ZoomParameter; //0x0088
        AntRef m_ZoomScaleFactor; //0x008C
        AntRef m_Dispersion; //0x0090
        AntRef m_AimTargetPosBody; //0x0094
        AntRef m_ZoomOutSpeed; //0x0098
        AntRef m_ZoomInSpeed; //0x009C
        AntRef m_UnDeploySpeed; //0x00A0
        AntRef m_DeploySpeed; //0x00A4
        AntRef m_LightEnabled; //0x00A8
        AntRef m_FireModeChanged; //0x00AC
        AntRef m_AnimType; //0x00B0
        AntRef m_GunDown; //0x00B4
        AntRef m_NumberOfBulletsLeftInGun; //0x00B8
        AntRef m_BulletsLeftInGun; //0x00BC
        AntRef m_AbortVehicleDeploy; //0x00C0
        AntRef m_CurrentProjectileIndex; //0x00C4
        AntRef m_WeaponActionESIG; //0x00C8
        AntRef m_IsSprinting; //0x00CC
        AntRef m_PreparingToBash; //0x00D0
        AntRef m_JustStartedSprinting; //0x00D4
        AntRef m_KickBackInIronSight; //0x00D8
        AntRef m_ZoomingTime; //0x00DC
        AntRef m_TriggerZoomGunTwitch; //0x00E0
        AntRef m_WeaponChooserSignal; //0x00E4
        AntRef m_WeaponClassSignal; //0x00E8
        AntRef m_OffsetX; //0x00EC
        AntRef m_OffsetY; //0x00F0
        AntRef m_OffsetZ; //0x00F4
        AntRef m_AIAllowFire; //0x00F8
        AntRef m_AIAltFireFromAnt; //0x00FC
    };//Size=0x0100

    ////////////////////////////////////////
    // RuntimeId: 03425
    // TypeInfo:  0x02C04E20
    struct Animated3pOnlyWeaponBinding
    {
        AntRef m_Deploy3P; //0x0000
        AntRef m_HideWeapon3p; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03423
    // TypeInfo:  0x02C04E10
    struct Animated1pOnlyWeaponBinding
    {
        AntRef m_UndeployFinished; //0x0000
        AntRef m_CameraFreeWeight; //0x0004
        AntRef m_Deploy1P; //0x0008
        AntRef m_HideWeapon1p; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 03419
    // TypeInfo:  0x02C04DF0
    struct PoseConstraintsData
    {
        bool m_StandPose; //0x0000
        bool m_CrouchPose; //0x0001
        bool m_PronePose; //0x0002
    };//Size=0x0003

    ////////////////////////////////////////
    // RuntimeId: 03417
    // TypeInfo:  0x02C04DE0
    struct AimingConstraintsData
    {
        float m_MinYaw; //0x0000
        float m_MaxYaw; //0x0004
        float m_MinPitch; //0x0008
        float m_MaxPitch; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 03397
    // TypeInfo:  0x02C04C44
    struct ScenarioTaskData
    {
        Vec3 m_EndPointWorldOffset; //0x0000
        Vec3 m_StartPoint; //0x0010
        __int32 m_ScenarioId; //0x0020
        __int32 m_ActorId; //0x0024
        __int32 m_PartId; //0x0028
        __int32 m_LevelId; //0x002C
        float m_WorldAngle; //0x0030
        Array<LinearTransform> m_ConnectTransforms; //0x0034
        float m_StartTurnDistance; //0x0038
        float m_StartTimerDistance; //0x003C
        float m_TriggerScenarioDelay; //0x0040
        bool m_TriggerScenario; //0x0044
        bool m_UseClientPosition; //0x0045
        char _0x0046[10];
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 03395
    // TypeInfo:  0x02C04C34
    struct AILocoCoverTaskData
    {
        Vec3 m_WantedPos; //0x0000
        Vec3 m_ThreatPosition; //0x0010
        AntPoseEnum m_CoverPose; //0x0020
        float m_WaitTime; //0x0024
        CoverPeekType m_PeekOut; //0x0028
        float m_WorldAngle; //0x002C
        float m_OffsetLength; //0x0030
        AntPoseEnum m_ExitPose; //0x0034
        float m_ExitAngle; //0x0038
        float m_DistanceToNextWaypoint; //0x003C
        AntCoverEnum m_CoverType; //0x0040
        CoverEnterStrategy m_EnterStrategy; //0x0044
        CoverExitStyle m_ExitStyle; //0x0048
        CoverFireType m_PrepareFireType; //0x004C
        bool m_ForceExitCover; //0x0050
        bool m_UseClientPosition; //0x0051
        char _0x0052[14];
    };//Size=0x0060

    ////////////////////////////////////////
    // RuntimeId: 03383
    // TypeInfo:  0x02C04BD4
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
    // RuntimeId: 03381
    // TypeInfo:  0x02C04BC4
    struct AILocoMoveTaskData
    {
        Vec3 m_WantedPos; //0x0000
        float m_WorldAngle; //0x0010
        float m_WaitTime; //0x0014
        AntPoseEnum m_EnterPose; //0x0018
        AntPoseEnum m_ExitPose; //0x001C
        bool m_OverrideAngle; //0x0020
        bool m_UseClientPosition; //0x0021
        char _0x0022[14];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 03379
    // TypeInfo:  0x02C04BB4
    struct AILocoVaultTaskData
    {
        Vec3 m_StartPoint; //0x0000
        float m_DistanceBeforeVault; //0x0010
        float m_HeightBeforeVault; //0x0014
        float m_HeightAfterVault; //0x0018
        float m_LengthOfVaultableObject; //0x001C
        float m_WorldAngle; //0x0020
        float m_DistanceAfterVault; //0x0024
        WaypointVaultType m_VaultType; //0x0028
        bool m_UseClientPosition; //0x002C
        char _0x002D[3];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 03369
    // TypeInfo:  0x02C04B64
    struct AILocoCoverBinding
    {
        AntRef m_PrepareFire; //0x0000
        AntRef m_ThrowGrenade; //0x0004
        AntRef m_PeekOut; //0x0008
        AntRef m_IdleBehindCover; //0x000C
        AntRef m_PeekType; //0x0010
        AntRef m_CoverTypeEnum; //0x0014
        AntRef m_CoverFireStyle; //0x0018
        AntRef m_EnterCover; //0x001C
        AntRef m_ExitCover; //0x0020
        AntRef m_IsCloseCover; //0x0024
        AntRef m_DistanceScale; //0x0028
        AntRef m_AbsoluteDistance; //0x002C
        AntRef m_AngleToNormal; //0x0030
        AntRef m_OutAngle; //0x0034
        AntRef m_ThreatAngle; //0x0038
        AntRef m_StopExitCoverOutAround; //0x003C
        AntRef m_EnterStrategy; //0x0040
        AntRef m_ExitStyle; //0x0044
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 03367
    // TypeInfo:  0x02C04B54
    struct AILocoVaultBinding
    {
        AntRef m_Vault; //0x0000
        AntRef m_DistanceBeforeVault; //0x0004
        AntRef m_HeightBeforeVault; //0x0008
        AntRef m_LengthOfVaultableObject; //0x000C
        AntRef m_HeightAfterVault; //0x0010
        AntRef m_DistanceAfterVault; //0x0014
        AntRef m_VaultType; //0x0018
    };//Size=0x001C

    ////////////////////////////////////////
    // RuntimeId: 03365
    // TypeInfo:  0x02C04B44
    struct AILocoBinding
    {
        AntRef m_LocoTarget; //0x0000
        AntRef m_AttentionState; //0x0004
        AntRef m_EndPoseEnum; //0x0008
        AntRef m_CurrentPoseEnum; //0x000C
        AntRef m_EnterExitPoseEnum; //0x0010
        AntRef m_RunStyle; //0x0014
        AntRef m_TargetWaypointPosition; //0x0018
        AntRef m_TargetSpeedLevel; //0x001C
        AntRef m_EnableProceduralHeadAim; //0x0020
    };//Size=0x0024

    ////////////////////////////////////////
    // RuntimeId: 03360
    // TypeInfo:  0x02C03E58
    struct VehicleCameraControlBinding
    {
        AntRef m_EnableAnimatedVehicleCamera; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 03358
    // TypeInfo:  0x02C03E48
    struct VehicleEntryListenerBinding
    {
        AntRef m_InVehicle; //0x0000
        AntRef m_InOpenEntry; //0x0004
        AntRef m_VehicleEntryFire; //0x0008
        AntRef m_VehicleEntryYaw; //0x000C
        AntRef m_VehicleEntryPitch; //0x0010
        AntRef m_VehicleEntryRoll; //0x0014
        AntRef m_VehicleEntryRotation; //0x0018
        AntRef m_VehicleEntryForceX; //0x001C
        AntRef m_VehicleEntryForceY; //0x0020
        AntRef m_VehicleEntryForceZ; //0x0024
        AntRef m_VehicleEntryIndex; //0x0028
        AntRef m_VehicleEntryIndexUpdated; //0x002C
        AntRef m_VehicleOpenEntryIndex; //0x0030
        AntRef m_InVehicleRetrigger; //0x0034
        AntRef m_VehicleEntryChooserIndex; //0x0038
    };//Size=0x003C

    ////////////////////////////////////////
    // RuntimeId: 03356
    // TypeInfo:  0x02C03E38
    struct ClientCameraToComponentsInitMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 03355
    // TypeInfo:  0x02C03E28
    struct VehicleToComponentsStartDisabledDamagedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 03354
    // TypeInfo:  0x02C03E18
    struct ClientVehicleToComponentsHealthChangedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 03349
    // TypeInfo:  0x02C03DB0
    struct NormalizeSettings
    {
        float m_Maximum; //0x0000
        float m_Minimum; //0x0004
        float m_Velocity; //0x0008
        float m_Lower; //0x000C
        float m_Upper; //0x0010
        bool m_Normalize; //0x0014
        char _0x0015[3];
    };//Size=0x0018

    ////////////////////////////////////////
    // RuntimeId: 03345
    // TypeInfo:  0x02C03D74
    struct VehicleLockableInfoData
    {
        float m_HeatSignature; //0x0000
        float m_RadarSignature; //0x0004
        float m_LowAltitudeHeight; //0x0008
        float m_LowAltitudeHeatSignature; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 03343
    // TypeInfo:  0x02C03D64
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
    // RuntimeId: 03341
    // TypeInfo:  0x02C03D54
    struct MPModeData
    {
        __int32 m_VehiclePoints; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 03339
    // TypeInfo:  0x02C03D44
    struct VehicleHealthZoneData
    {
        float m_MaxHealth; //0x0000
        float m_MaxShieldHealth; //0x0004
        float m_DamageAngleMultiplier; //0x0008
        float m_MinDamageAngle; //0x000C
        bool m_UseDamageAngleCalculation; //0x0010
        char _0x0011[3];
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 03337
    // TypeInfo:  0x02C03D34
    struct VehicleHudData
    {
        Vec3 m_CustomizationOffset; //0x0000
        VehicleCustomizationAsset* m_Customization; //0x0010
        UIHudIcon m_MinimapIcon; //0x0014
        TextureAsset* m_Texture; //0x0018
        unsigned __int32 m_VehicleItemHash; //0x001C
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 03333
    // TypeInfo:  0x02C03D14
    struct BasicUnlockInfo
    {
        Guid m_UnlockGuid; //0x0000
        char _0x0000[16];
        unsigned __int32 m_Identifier; //0x0010
        unsigned __int32 m_UnlockScore; //0x0014
        Array<char*> m_Licenses; //0x0018
        Array<char*> m_AdditionalLicenses; //0x001C
        char* m_StringId; //0x0020
    };//Size=0x0024

    ////////////////////////////////////////
    // RuntimeId: 03321
    // TypeInfo:  0x02C03C28
    struct UnlockWeaponAndSlot
    {
        SoldierWeaponUnlockAsset* m_Weapon; //0x0000
        WeaponSlot m_Slot; //0x0004
        Array<UnlockAssetBase*> m_UnlockAssets; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 03315
    // TypeInfo:  0x02C03BC0
    struct UnlockAssetPair
    {
        UnlockAssetBase* m_Second; //0x0000
        UnlockAssetBase* m_Result; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03301
    // TypeInfo:  0x02C03AC4
    struct UIFlowDialogNodeReachedMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 03300
    // TypeInfo:  0x02C03AB4
    struct UIDialogScreenPushedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 03295
    // TypeInfo:  0x02C03A4C
    struct MeshProxyDataBasePoseTransforms
    {
        MeshProxyEntityData* m_Mesh; //0x0000
        Array<LinearTransform> m_BasePoseTransforms; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03293
    // TypeInfo:  0x02C03A3C
    struct GroupedBlueprintInstance
    {
        ReferenceObjectData* m_GroupedData; //0x0000
        Blueprint* m_GroupedDataOwner; //0x0004
        Blueprint* m_GroupedDataRootOwner; //0x0008
        GroupedBlueprintInfo* m_GroupedBlueprintInfo; //0x000C
        StaticModelGroupEntityData* m_GroupEntity; //0x0010
        unsigned __int32 m_PartStartIndex; //0x0014
        unsigned __int32 m_PartIndexCount; //0x0018
        bool m_HasPhysics; //0x001C
        char _0x001D[3];
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 03289
    // TypeInfo:  0x02C03A00
    struct GroupedOriginalData
    {
        ReferenceObjectData* m_Reference; //0x0000
        PrefabBlueprint* m_Owner; //0x0004
        ReferenceObjectData* m_NonGroupedReference; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 03231
    // TypeInfo:  0x02C03558
    struct InputRecorderTrackData
    {
        char* m_Name; //0x0000
        FloatPropertyTrackData* m_PropertyTrack; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03221
    // TypeInfo:  0x02C034B4
    struct SimpleMovementActionTimeData
    {
        float m_Time; //0x0000
        float m_MaxVariation; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03187
    // TypeInfo:  0x02C03200
    struct SyncAnimationsStateChangeMessageBase
    {
        char _0x0000[56];
        SyncAnimationsEntityData* m_Data; //0x0038
        float m_Time; //0x003C
        SyncAnimationsState m_State; //0x0040
        char _0x0044[4];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 03152
    // TypeInfo:  0x02C02F74
    struct IndexRange
    {
        unsigned __int32 m_First; //0x0000
        unsigned __int32 m_Last; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03154
    // TypeInfo:  0x02C02F84
    struct ChildStaticModelNetworkInfo
    {
        IndexRange m_NetworkRange; //0x0000
        unsigned __int32 m_ParentPartComponentIndex; //0x0008
        unsigned __int32 m_ParentHealthStateIndex; //0x000C
        unsigned __int32 m_InstanceIndex; //0x0010
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 03156
    // TypeInfo:  0x02C02F94
    struct StaticModelNetworkInfo
    {
        Array<IndexRange> m_PartNetworkIdRanges; //0x0000
        unsigned __int32 m_NetworkIdCount; //0x0004
        Array<ChildStaticModelNetworkInfo> m_ChildNetworkInfos; //0x0008
        unsigned __int32 m_ChildNetworkIdCount; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 03150
    // TypeInfo:  0x02C02F64
    struct PhysicsPartInfo
    {
        unsigned __int32 m_PartComponentIndex; //0x0000
        unsigned __int32 m_HealthStateIndex; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03142
    // TypeInfo:  0x02C02ED0
    struct CharacterToComponentsSpawnTemplateMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 03123
    // TypeInfo:  0x02C01804
    struct CounterHelper
    {
        bool m_dummy; //0x0000
    };//Size=0x0001

    ////////////////////////////////////////
    // RuntimeId: 03103
    // TypeInfo:  0x02C01710
    struct EntityToComponentsResetMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 03098
    // TypeInfo:  0x02C016A8
    struct UnlockComponentEnabledMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 03087
    // TypeInfo:  0x02C015BC
    struct LandingGearConditionData
    {
        float m_Height; //0x0000
        float m_Velocity; //0x0004
        float m_Angle; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 03077
    // TypeInfo:  0x02C014FC
    struct SoldierAnimatedCameraData
    {
        float m_CameraTransitionTime; //0x0000
        bool m_EnableFeature; //0x0004
        char _0x0005[3];
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03067
    // TypeInfo:  0x02C01458
    struct ShaderParameterVector
    {
        Vec4 m_Value; //0x0000
        char* m_ParameterName; //0x0010
        char _0x0014[12];
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 03063
    // TypeInfo:  0x02C0141C
    struct CameraBinding
    {
        AntRef m_Render1pInBackground; //0x0000
        AntRef m_ForceRender1pInForeground; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 02971
    // TypeInfo:  0x02C00E2C
    struct ActionSuppressor
    {
        EntryInputActionEnum m_ActionToSuppress; //0x0000
        float m_SuppressingValue; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03055
    // TypeInfo:  0x02C013A4
    struct InputSuppressionData
    {
        Array<ActionSuppressor> m_SuppressVehicleInput; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 03051
    // TypeInfo:  0x02C01368
    struct HIKData
    {
        float m_ReachT; //0x0000
        float m_ReachR; //0x0004
        float m_Pull; //0x0008
        float m_Resist; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 03047
    // TypeInfo:  0x02C0132C
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
    // RuntimeId: 03029
    // TypeInfo:  0x02C011D8
    struct RotorModelData
    {
        float m_RotationRpm; //0x0000
        unsigned __int32 m_PartIndex; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03007
    // TypeInfo:  0x02C0102C
    struct VehicleParachuteBinding
    {
        AntRef m_Enabled; //0x0000
        AntRef m_VehicleType; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 02999
    // TypeInfo:  0x02C00FB4
    struct RagdollBinding
    {
        AntRef m_RagdollOnBack; //0x0000
        AntRef m_RagdollBlend; //0x0004
        AntRef m_RagdollForceBlendDisabled; //0x0008
        AntRef m_RagdollActiveTime; //0x000C
        AntRef m_RagdollFullyBlendedIn; //0x0010
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 02989
    // TypeInfo:  0x02C00F2C
    struct PitchModifier
    {
        Vec3 m_Offset; //0x0000
        float m_PitchVal; //0x0010
        float m_PitchAngle; //0x0014
        char _0x0018[8];
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 02993
    // TypeInfo:  0x02C00F4C
    struct BoneCollisionData
    {
        Vec4 m_DebugDrawColor; //0x0000
        Vec3 m_CapsuleOffset; //0x0010
        char* m_BoneName; //0x0020
        HitReactionType m_AnimationHitReactionType; //0x0024
        MaterialContainerPair* m_MaterialPair; //0x0028
        __int32 m_BoneAxis; //0x002C
        float m_CapsuleLength; //0x0030
        float m_CapsuleRadius; //0x0034
        char _0x0038[8];
        PitchModifier m_MinPitch; //0x0040
        PitchModifier m_MaxPitch; //0x0060
        bool m_ValidInHiLod; //0x0080
        bool m_ValidInLowLod; //0x0081
        bool m_UsePhysicsRotation; //0x0082
        bool m_DeactivateIfBehindWall; //0x0083
        char _0x0084[12];
    };//Size=0x0090

    ////////////////////////////////////////
    // RuntimeId: 00145
    // TypeInfo:  0x02BE34F8
    struct AntAnimatableData
    {
        AntRef m_Actor; //0x0000
        ProceduralAwarenessAntRefs* m_ProceduralAwareness; //0x0004
        AntRef m_RightHandEffectorDisableOverride; //0x0008
        AntRef m_LeftHandEffectorDisableOverride; //0x000C
        MasterSkeletonAsset* m_MasterSkeletonAsset; //0x0010
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 02979
    // TypeInfo:  0x02C00E88
    struct LodBinding
    {
        AntRef m_DisableControllerUpdate; //0x0000
        AntRef m_DisablePoseUpdate; //0x0004
        AntRef m_DistanceFromCamera; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 02977
    // TypeInfo:  0x02C00E78
    struct GameplayBone
    {
        char* m_Name; //0x0000
        GameplayBones m_Bone; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 03057
    // TypeInfo:  0x02C013B4
    struct RegularCameraViewData
    {
        Vec3 m_MeshOffset; //0x0000
        InputSuppressionData m_InputSuppression; //0x0010
        float m_FieldOfView; //0x0014
        RigidMeshAsset* m_Mesh; //0x0018
        float m_ScreenExposureAreaScale; //0x001C
        ObjectBlueprint* m_MaskMeshBlueprint; //0x0020
        bool m_LockMeshToRenderView; //0x0024
        bool m_AllowFieldOfViewScaling; //0x0025
        bool m_FLIREnabled; //0x0026
        char _0x0027[9];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02969
    // TypeInfo:  0x02C00E1C
    struct StanceCameraData
    {
        Array<__int32> m_ValidStances; //0x0000
        Array<__int32> m_DefaultCameraForStances; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 02959
    // TypeInfo:  0x02C00D78
    struct EntryComponentStanceChangedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02954
    // TypeInfo:  0x02C00D10
    struct StanceSwitchSoundData
    {
        SoundAsset* m_StanceSwitchSound; //0x0000
        Array<__int32> m_ValidStances; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 02950
    // TypeInfo:  0x02C00CF0
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
    // RuntimeId: 02924
    // TypeInfo:  0x02C00B24
    struct AntiSpamConfig
    {
        float m_DetectionInterval; //0x0000
        unsigned __int32 m_DetectionIntervalMaxMessageCount; //0x0004
        unsigned __int32 m_ServerSideMessageCountTolerance; //0x0008
        unsigned __int32 m_SecondsBlocked; //0x000C
        unsigned __int32 m_ServerSideSecondsBlockedTolerance; //0x0010
        char* m_NotificationSid; //0x0014
        char* m_StillBlockedSid; //0x0018
        SoundWaveAsset* m_WarningSound; //0x001C
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 02914
    // TypeInfo:  0x02C00A80
    struct AnimationDrivenBinding
    {
        AntRef m_Velocity; //0x0000
        AntRef m_EnableCollision; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 02908
    // TypeInfo:  0x02C00A18
    struct CharacterWeaponBinding
    {
        AntRef m_WeaponId; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 02890
    // TypeInfo:  0x02C008C4
    struct CameraLeapData
    {
        LinearTransform m_Transform; //0x0000
        float m_Time; //0x0040
        float m_FovModifier; //0x0044
        char _0x0048[8];
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 02878
    // TypeInfo:  0x02C007D8
    struct HudImpactData
    {
        float m_MaxHealth; //0x0000
        float m_MinHealth; //0x0004
        float m_MaxTimeVisible; //0x0008
        float m_MinTimeVisible; //0x000C
        bool m_Enable; //0x0010
        char _0x0011[3];
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 02874
    // TypeInfo:  0x02C0079C
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
    // RuntimeId: 02872
    // TypeInfo:  0x02C0078C
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
    // RuntimeId: 02870
    // TypeInfo:  0x02C0077C
    struct CameraSineCurveData
    {
        float m_Frequency; //0x0000
        float m_Amplitude; //0x0004
        float m_PhaseShift; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 02826
    // TypeInfo:  0x02BFEB48
    struct EditableActions
    {
        Array<EditableAction*> m_Actions; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 02828
    // TypeInfo:  0x02BFEB58
    struct EditableActionMap
    {
        char* m_Id; //0x0000
        char* m_NameId; //0x0004
        EntryInputActionMapsData* m_ActionMap; //0x0008
        EditableActions m_ConfigurationLayout; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 02794
    // TypeInfo:  0x02BFE930
    struct HudData
    {
        float m_CrosshairScaleMin; //0x0000
        float m_CrosshairScaleMax; //0x0004
        float m_CrosshairOpacityMin; //0x0008
        float m_CrosshairOpacityMax; //0x000C
        float m_CrosshairOpacityModifier; //0x0010
        char* m_CrosshairTypeId; //0x0014
        char* m_WeaponClass; //0x0018
        float m_CameraShakeModifier; //0x001C
        float m_SeaLevelAltFreq; //0x0020
        float m_LowAmmoWarning; //0x0024
        float m_ReloadPrompt; //0x0028
        __int32 m_RenderTargetIndex; //0x002C
        UIPartPropertyList* m_HudPropertyList; //0x0030
        bool m_ShowMinimap; //0x0034
        bool m_InfiniteAmmo; //0x0035
        bool m_HideCrosshairWhenAimOnFriend; //0x0036
        bool m_HideAmmo; //0x0037
        bool m_UseRangeMeter; //0x0038
        bool m_UsePredictedSight; //0x0039
        bool m_UseWeaponOrientations; //0x003A
        bool m_UseVelocityVectorMarker; //0x003B
        bool m_UseLockingController; //0x003C
        bool m_UseThrust; //0x003D
        bool m_UseGForce; //0x003E
        bool m_UseSkidSlip; //0x003F
        bool m_UseClimbRate; //0x0040
        bool m_UseAimWarning; //0x0041
        bool m_UseRenderTarget; //0x0042
        char _0x0043[1];
    };//Size=0x0044

    ////////////////////////////////////////
    // RuntimeId: 02790
    // TypeInfo:  0x02BFE8F4
    struct UIPartProperties
    {
        UIPartIdentifier m_Identifier; //0x0000
        float m_Range; //0x0004
        float m_Frequency; //0x0008
        bool m_CompensateFreelook; //0x000C
        char _0x000D[3];
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 02780
    // TypeInfo:  0x02BFE888
    struct HitReactionComponentBinding
    {
        AntRef m_Hit; //0x0000
        AntRef m_AllowKillFromAnimation; //0x0004
        AntRef m_Direction; //0x0008
        AntRef m_FiringDistance; //0x000C
        AntRef m_BoneType; //0x0010
        AntRef m_ImpactType; //0x0014
        AntRef m_Immortal; //0x0018
        AntRef m_RandomAnimationIndex; //0x001C
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 02774
    // TypeInfo:  0x02BFE83C
    struct StaticModelGroupMemberData
    {
        Array<LinearTransform> m_InstanceTransforms; //0x0000
        Array<float> m_InstanceScale; //0x0004
        Array<unsigned __int32> m_InstanceObjectVariation; //0x0008
        Array<bool> m_InstanceCastSunShadow; //0x000C
        Array<bool> m_InstanceEnabled; //0x0010
        GamePhysicsEntityData* m_MemberType; //0x0014
        EntityData* m_MeshEntityType; //0x0018
        unsigned __int32 m_InstanceCount; //0x001C
        unsigned __int32 m_HealthStateEntityManagerId; //0x0020
        IndexRange m_PhysicsPartRange; //0x0024
        unsigned __int32 m_PhysicsPartCountPerInstance; //0x002C
        IndexRange m_NetworkIdRange; //0x0030
        unsigned __int32 m_NetworkIdCountPerInstance; //0x0038
    };//Size=0x003C

    ////////////////////////////////////////
    // RuntimeId: 02742
    // TypeInfo:  0x02BFE608
    struct StreamGridCell
    {
        ReferenceObjectData* m_ReferenceData; //0x0000
        __int32 m_X; //0x0004
        __int32 m_Z; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 02726
    // TypeInfo:  0x02BFE4E0
    struct ServerControllableToComponentsPlayerExitMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02725
    // TypeInfo:  0x02BFE4D0
    struct ServerControllableToComponentsPlayerEnteredMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02724
    // TypeInfo:  0x02BFE4C0
    struct ClientControllableToComponentsPlayerExitMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02723
    // TypeInfo:  0x02BFE4B0
    struct ClientControllableToComponentsPlayerEnteredMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 00278
    // TypeInfo:  0x02BE584C
    struct EntityUid
    {
        unsigned __int32 m_Id; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 02707
    // TypeInfo:  0x02BFE378
    struct EntitlementData
    {
        char* m_License; //0x0000
        char* m_EntitlementTag; //0x0004
        char* m_GroupName; //0x0008
        char* m_ProductId; //0x000C
        char* m_ProjectId; //0x0010
        EntitlementType m_UsageType; //0x0014
        bool m_VerifyOwnership; //0x0018
        char _0x0019[3];
    };//Size=0x001C

    ////////////////////////////////////////
    // RuntimeId: 02697
    // TypeInfo:  0x02BFE2F0
    struct AntDynamicAvoidanceBinding
    {
        AntRef m_TimeUntilCollision; //0x0000
        AntRef m_DistanceUntilCollision; //0x0004
        AntRef m_NormalizedVelocity; //0x0008
        AntRef m_Speed; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 02679
    // TypeInfo:  0x02BFE19C
    struct DataVisualizerValue
    {
        char* m_Value; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 02643
    // TypeInfo:  0x02BFDF64
    struct CustomizeVisual
    {
        Array<UnlockAsset*> m_Visual; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 02641
    // TypeInfo:  0x02BFDF54
    struct NetworkPlayerSelectedWeaponMessage
    {
        char _0x0000[56];
        WeaponSlot m_Slot; //0x0038
        SoldierWeaponUnlockAsset* m_SoldierWeaponUnlockAsset; //0x003C
        Array<UnlockAssetBase*> m_UnlockAssets; //0x0040
        bool m_Spawning; //0x0044
        bool m_FirstWeapon; //0x0045
        char _0x0046[2];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 02640
    // TypeInfo:  0x02BFDF44
    struct NetworkPlayerSelectedUnlockAssetsMessage
    {
        char _0x0000[56];
        CharacterCustomizationAsset* m_CharacterCustomizationAsset; //0x0038
        Array<UnlockAsset*> m_VisualUnlockAssets; //0x003C
        Array<UnlockAssetBase*> m_MiscUnlockAssets; //0x0040
        bool m_Spawning; //0x0044
        char _0x0045[3];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 02617
    // TypeInfo:  0x02BFDD50
    struct CustomizedMeshMaterialsData
    {
        ObjectBlueprint* m_MeshBlueprint; //0x0000
        Array<char*> m_MaterialNames; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 02567
    // TypeInfo:  0x02BFC020
    struct DirectWeaponSwitchingMapData
    {
        EntryInputActionEnum m_Action; //0x0000
        WeaponSwitchingEnum m_ToWeapon; //0x0004
        float m_PreventMeleeRepeatTime; //0x0008
        float m_SwitchBackToPrevMaxTimePressed; //0x000C
        bool m_UseQuickSwitch; //0x0010
        bool m_FireAndSwitchBackToPrev; //0x0011
        char _0x0012[2];
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 02565
    // TypeInfo:  0x02BFC010
    struct WeaponSwitchingMapData
    {
        WeaponSwitchingEnum m_FromWeapon; //0x0000
        EntryInputActionEnum m_Action; //0x0004
        Array<WeaponSwitchingEnum> m_ToWeapon; //0x0008
        bool m_FireAndSwitchBackToPrev; //0x000C
        char _0x000D[3];
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 02557
    // TypeInfo:  0x02BFBFD0
    struct SoldierToComponentsOnCustomizeSoldierMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02556
    // TypeInfo:  0x02BFBFC0
    struct SoldierToComponentsOnKilledMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02555
    // TypeInfo:  0x02BFBFB0
    struct SoldierToComponentsInitializedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02546
    // TypeInfo:  0x02BFBEF0
    struct ValueAtX
    {
        float m_X; //0x0000
        float m_Value; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 02538
    // TypeInfo:  0x02BFBE78
    struct HidableSoldierMeshWeaponPart
    {
        SkinnedMeshAsset* m_WeaponMesh; //0x0000
        ObjectBlueprint* m_WeaponMeshBlueprint; //0x0004
        char* m_BoneName; //0x0008
        bool m_HideAlways; //0x000C
        bool m_HideInVehicleEntries; //0x000D
        char _0x000E[2];
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 02534
    // TypeInfo:  0x02BFBE3C
    struct SoldierHeadCollisionPoseData
    {
        Vec3 m_IdleOffset; //0x0000
        Vec3 m_MovingForwardOffset; //0x0010
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 02532
    // TypeInfo:  0x02BFBE2C
    struct SoldierMeshData
    {
        SkinnedMeshAsset* m_Mesh; //0x0000
        ObjectBlueprint* m_MeshBlueprint; //0x0004
        __int32 m_RandomGroup; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 02528
    // TypeInfo:  0x02BFBDF0
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
    // RuntimeId: 02526
    // TypeInfo:  0x02BFBDE0
    struct FootStepTrigger
    {
        float m_Time; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 02464
    // TypeInfo:  0x02BFB8FC
    struct ScoringBucketUnlockData
    {
        unsigned __int32 m_PointsNeeded; //0x0000
        BasicUnlockInfo m_UnlockInfo; //0x0004
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02436
    // TypeInfo:  0x02BFB6E8
    struct ProfileOptionDataEnumItem
    {
        char* m_DisplayName; //0x0000
        bool m_Default; //0x0004
        char _0x0005[3];
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 02426
    // TypeInfo:  0x02BFB628
    struct BinaryOption
    {
        char* m_Name; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 02424
    // TypeInfo:  0x02BFB618
    struct StringOption
    {
        char* m_Name; //0x0000
        char* m_Value; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 02422
    // TypeInfo:  0x02BFB608
    struct IntOption
    {
        char* m_Name; //0x0000
        __int32 m_Value; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 02420
    // TypeInfo:  0x02BFB5F8
    struct FloatOption
    {
        char* m_Name; //0x0000
        float m_Value; //0x0004
        float m_Min; //0x0008
        float m_Max; //0x000C
        float m_Step; //0x0010
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 02406
    // TypeInfo:  0x02BFB4FC
    struct DynamicWeaponPickupSlotData
    {
        unsigned __int32 m_WeaponSlot; //0x0000
        __int32 m_AltWeaponSlot; //0x0004
        __int32 m_LinkedToWeaponSlot; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 02400
    // TypeInfo:  0x02BFB494
    struct WeaponUnlockPickupData
    {
        UnlockWeaponAndSlot m_UnlockWeaponAndSlot; //0x0000
        __int32 m_AltWeaponSlot; //0x000C
        __int32 m_LinkedToWeaponSlot; //0x0010
        unsigned __int32 m_MinAmmo; //0x0014
        unsigned __int32 m_MaxAmmo; //0x0018
        bool m_DefaultToFullAmmo; //0x001C
        char _0x001D[3];
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 02396
    // TypeInfo:  0x02BFB458
    struct WeaponPickupData
    {
        SoldierWeaponBlueprint* m_Weapon; //0x0000
        unsigned __int32 m_WeaponSlot; //0x0004
        __int32 m_AltWeaponSlot; //0x0008
        __int32 m_LinkedToWeaponSlot; //0x000C
        unsigned __int32 m_MinAmmo; //0x0010
        unsigned __int32 m_MaxAmmo; //0x0014
    };//Size=0x0018

    ////////////////////////////////////////
    // RuntimeId: 02388
    // TypeInfo:  0x02BFB3E0
    struct PhysicsDrivenAnimationEntityBinding
    {
        AntRef m_AimLeftRight; //0x0000
        AntRef m_AimUpDown; //0x0004
        AntRef m_Crouch; //0x0008
        AntRef m_ForceSetTrajectory; //0x000C
        AntRef m_InAir; //0x0010
        AntRef m_Skydive; //0x0014
        AntRef m_Parachute; //0x0018
        AntRef m_Swim; //0x001C
        AntRef m_InputBackward; //0x0020
        AntRef m_InputForward; //0x0024
        AntRef m_InputLeft; //0x0028
        AntRef m_InputRight; //0x002C
        AntRef m_IsEnemy; //0x0030
        AntRef m_Jump; //0x0034
        AntRef m_LeanLeftRight; //0x0038
        AntRef m_Prone; //0x003C
        AntRef m_Sprint; //0x0040
        AntRef m_GroundSupported; //0x0044
        AntRef m_GroundNormal; //0x0048
        AntRef m_GroundDistance; //0x004C
        AntRef m_GroundAngleZ; //0x0050
        AntRef m_GroundAngleX; //0x0054
        AntRef m_GroundAngleFromNormal; //0x0058
        AntRef m_IsClientAnimatable; //0x005C
        AntRef m_CustomizationScreen; //0x0060
        AntRef m_Minimal3pServer; //0x0064
        AntRef m_VerticalImpact; //0x0068
        AntRef m_FalseSignal; //0x006C
        AntRef m_LockArmsToCameraWeight; //0x0070
    };//Size=0x0074

    ////////////////////////////////////////
    // RuntimeId: 02374
    // TypeInfo:  0x02BFB2C8
    struct RichPresenceProperty
    {
        char* m_Name; //0x0000
        RichPresencePropertyType m_PropertyType; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 02370
    // TypeInfo:  0x02BFB2A8
    struct RichPresenceContextSetting
    {
        RichPresenceContext* m_Context; //0x0000
        RichPresenceContextValue* m_Value; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 02358
    // TypeInfo:  0x02BFB1BC
    struct PersistenceConsumableMapping
    {
        ConsumableGroup m_Group; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 02340
    // TypeInfo:  0x02BFB084
    struct PersistentValueTemplateData
    {
        char* m_Name; //0x0000
        char* m_DefaultValue; //0x0004
        float m_DefaultFloatValue; //0x0008
        __int32 m_DefaultIntValue; //0x000C
        PersistentValueType m_ValueType; //0x0010
        PersistentValueDataType m_DataType; //0x0014
        PersistentValueHistoryType m_HistoryType; //0x0018
        AwardGroup m_Group; //0x001C
        bool m_ClubStat; //0x0020
        bool m_ForceIntoTemplate; //0x0021
        char _0x0022[2];
    };//Size=0x0024

    ////////////////////////////////////////
    // RuntimeId: 02338
    // TypeInfo:  0x02BFB074
    struct CustomReportValueData
    {
        char* m_Name; //0x0000
        CustomReportValueKind m_Kind; //0x0004
        PersistentValueDataType m_DataType; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 02314
    // TypeInfo:  0x02BFAF44
    struct PathfindingObjectCategoryRepresentation
    {
        PathfindingObjectCategoryAsset* m_Category; //0x0000
        PathfindingRepresentation m_Representation; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 02302
    // TypeInfo:  0x02BFAE90
    struct OnlineProviderConfiguration
    {
        GamePlatform m_Platform; //0x0000
        char* m_Client; //0x0004
        char* m_ServiceName; //0x0008
        char* m_SKU; //0x000C
        char* m_Version; //0x0010
        unsigned __int32 m_ServerSocketPacketSize; //0x0014
        bool m_IsServer; //0x0018
        char _0x0019[3];
    };//Size=0x001C

    ////////////////////////////////////////
    // RuntimeId: 02294
    // TypeInfo:  0x02BFAE50
    struct OnlineEnvironmentUrlData
    {
        char* m_Url; //0x0000
        OnlineEnvironment m_Environment; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 02296
    // TypeInfo:  0x02BFAE60
    struct OnlineEnvironmentUrl
    {
        Array<OnlineEnvironmentUrlData> m_Urls; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 02298
    // TypeInfo:  0x02BFAE70
    struct OnlineEnvironmentConsoleUrlData
    {
        GamePlatform m_Platform; //0x0000
        OnlineEnvironmentUrl m_Url; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 02300
    // TypeInfo:  0x02BFAE80
    struct OnlineEnvironmentConsoleUrl
    {
        Array<OnlineEnvironmentConsoleUrlData> m_Urls; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 02282
    // TypeInfo:  0x02BFAD80
    struct MovementComponentBinding
    {
        AntRef m_AnimationControlled; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 02271
    // TypeInfo:  0x02BF907C
    struct CoreDebugReadSaveGameDataMessage
    {
        char _0x0000[56];
        unsigned __int32 m_HeaderSize; //0x0038
        unsigned __int32 m_HeaderData; //0x003C
        unsigned __int32 m_BodySize; //0x0040
        unsigned __int32 m_BodyData; //0x0044
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 02270
    // TypeInfo:  0x02BF906C
    struct NetworkLogicFireDoublePlayerEventMessageBase
    {
        char _0x0000[56];
        unsigned __int32 m_EventIndex; //0x0038
        __int32 m_PlayerId; //0x003C
        __int32 m_ExtraPlayerId; //0x0040
        char _0x0044[4];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 02269
    // TypeInfo:  0x02BF905C
    struct NetworkLogicFirePlayerEventMessageBase
    {
        char _0x0000[56];
        unsigned __int32 m_EventIndex; //0x0038
        __int32 m_PlayerId; //0x003C
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02268
    // TypeInfo:  0x02BF904C
    struct NetworkLogicFireEventMessageBase
    {
        char _0x0000[56];
        unsigned __int32 m_EventIndex; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02267
    // TypeInfo:  0x02BF903C
    struct SubLevelDestroyedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02266
    // TypeInfo:  0x02BF902C
    struct SubLevelClientBundleUnloadedMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 02265
    // TypeInfo:  0x02BF901C
    struct SubLevelClientBundleLoadedMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 02264
    // TypeInfo:  0x02BF900C
    struct SubLevelClientOnStreamedOutBySaveGameMessage
    {
        char _0x0000[56];
        char* m_BundleName; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02263
    // TypeInfo:  0x02BF8FFC
    struct SubLevelClientOnStreamedInMessage
    {
        char _0x0000[56];
        char* m_BundleName; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02262
    // TypeInfo:  0x02BF8FEC
    struct SubLevelClientLoadBundleMessage
    {
        char _0x0000[56];
        char* m_BundleName; //0x0038
        char* m_ParentBundleName; //0x003C
        bool m_WaitForLoad; //0x0040
        char _0x0041[7];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 02261
    // TypeInfo:  0x02BF8FDC
    struct SubLevelStateChangeMessageBase
    {
        char _0x0000[56];
        char* m_BundleName; //0x0038
        unsigned __int32 m_ProxyEntityBusNetworkId; //0x003C
        bool m_StreamedIn; //0x0040
        bool m_AllowDeferredCreation; //0x0041
        char _0x0042[6];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 02260
    // TypeInfo:  0x02BF8FCC
    struct BlueprintEntityStateChangeMessageBase
    {
        char _0x0000[56];
        ReferenceObjectData* m_Data; //0x0038
        unsigned __int32 m_ProxyEntityBusNetworkId; //0x003C
        bool m_StreamedIn; //0x0040
        char _0x0041[7];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 02259
    // TypeInfo:  0x02BF8FBC
    struct DLCStateChangeMessageBase
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 02252
    // TypeInfo:  0x02BF8F60
    struct ServerAdministrationPasswordMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02251
    // TypeInfo:  0x02BF8F50
    struct ServerAdministrationEventsEnabledMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02250
    // TypeInfo:  0x02BF8F40
    struct ServerAdministrationQuitMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02249
    // TypeInfo:  0x02BF8F30
    struct ServerAdministrationLoginMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02248
    // TypeInfo:  0x02BF8F20
    struct ServerAdministrationPacketMessageBase
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 02229
    // TypeInfo:  0x02BF6E50
    struct UINetworkCoopPlayerMessage
    {
        char _0x0000[56];
        unsigned __int32 m_PlayerId; //0x0038
        bool m_IsBroadcasted; //0x003C
        char _0x003D[3];
        CoopPlayerEvent m_PlayerEvent; //0x0040
        bool m_IsPublicGame; //0x0044
        char _0x0045[3];
        Array<Difficulty> m_CompletedMissionsDifficulty; //0x0048
        Array<float> m_CompletedMissionsScore; //0x004C
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 02226
    // TypeInfo:  0x02BF6E30
    struct UINetworkCoopGameMessage
    {
        char _0x0000[56];
        char* m_SelectedMission; //0x0038
        char* m_SelectedDifficulty; //0x003C
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02225
    // TypeInfo:  0x02BF6E20
    struct UINetworkSetCoopServerLobbyGameTypeMessage
    {
        char _0x0000[56];
        bool m_IsPublicGame; //0x0038
        char _0x0039[7];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02224
    // TypeInfo:  0x02BF6E10
    struct UINetworkSquadWipeInstigatorMessage
    {
        char _0x0000[56];
        SquadId m_SquadId; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02223
    // TypeInfo:  0x02BF6E00
    struct UINetworkSquadWipeMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 02222
    // TypeInfo:  0x02BF6DF0
    struct UINetworkRoundWarningMessage
    {
        char _0x0000[56];
        TeamId m_LosingTeam; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02221
    // TypeInfo:  0x02BF6DE0
    struct UINetworkRollCreditsMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 02220
    // TypeInfo:  0x02BF6DD0
    struct UINetworkKilledOtherPlayerMessage
    {
        char _0x0000[64];
        PlayerKilledWeaponType m_WeaponType; //0x0040
        SoldierWeaponUnlockAsset* m_WeaponUnlock; //0x0044
        VehicleEntityData* m_VehicleData; //0x0048
        bool m_HeadShot; //0x004C
        char _0x004D[3];
        unsigned __int32 m_OwnKillCount; //0x0050
        unsigned __int32 m_VictimKillCount; //0x0054
        bool m_KilledByAdmin; //0x0058
        char _0x0059[7];
    };//Size=0x0060

    ////////////////////////////////////////
    // RuntimeId: 02219
    // TypeInfo:  0x02BF6DC0
    struct UINetworkOwnPlayerKilledMessage
    {
        char _0x0000[64];
        PlayerKilledWeaponType m_WeaponType; //0x0040
        SoldierWeaponUnlockAsset* m_WeaponUnlock; //0x0044
        VehicleEntityData* m_VehicleData; //0x0048
        bool m_HeadShot; //0x004C
        char _0x004D[3];
        unsigned __int32 m_OwnKillCount; //0x0050
        unsigned __int32 m_KillerKillCount; //0x0054
        unsigned __int16 m_KillerBasicDogTag; //0x0058
        unsigned __int16 m_KillerAdvancedDogTag; //0x005A
        float m_KillerAdvancedDogTagStat; //0x005C
        unsigned __int32 m_KillerWeaponKills; //0x0060
        unsigned __int32 m_KillerKitScore; //0x0064
        unsigned __int32 m_KillerHealth; //0x0068
        bool m_KilledByAdmin; //0x006C
        char _0x006D[3];
    };//Size=0x0070

    ////////////////////////////////////////
    // RuntimeId: 02216
    // TypeInfo:  0x02BF6DA0
    struct UINetworkStealBodyMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02215
    // TypeInfo:  0x02BF6D90
    struct UINetworkSetPlayerTeamMessage
    {
        char _0x0000[56];
        TeamId m_Team; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02214
    // TypeInfo:  0x02BF6D80
    struct UINetworkEndOfRoundBonusMessage
    {
        char _0x0000[56];
        unsigned __int32 m_AwayBonus; //0x0038
        unsigned __int32 m_VictoryBonus; //0x003C
        unsigned __int32 m_SquadBonusSupport; //0x0040
        unsigned __int32 m_SquadBonusDamageAssists; //0x0044
        unsigned __int32 m_SquadBonusCaptures; //0x0048
        unsigned __int32 m_OrderBonus; //0x004C
        __int8 m_RankBeforeRound; //0x0050
        __int8 m_RankAfterRound; //0x0051
        char _0x0052[6];
    };//Size=0x0058

    ////////////////////////////////////////
    // RuntimeId: 02213
    // TypeInfo:  0x02BF6D70
    struct UINetworkMenuResponseMessage
    {
        char _0x0000[56];
        MenuResponse m_Response; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02212
    // TypeInfo:  0x02BF6D60
    struct UINetworkHudScoringMessage
    {
        char _0x0000[56];
        char* m_DescriptionSid; //0x0038
        float m_Amount; //0x003C
        ScoringVisibilityType m_VisibilityType; //0x0040
        bool m_OtherPlayerScore; //0x0044
        char _0x0045[3];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 02211
    // TypeInfo:  0x02BF6D50
    struct UINetworkPlayerRankUpMessage
    {
        char _0x0000[56];
        __int32 m_Rank; //0x0038
        bool m_IsSoloRankUp; //0x003C
        char _0x003D[3];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02192
    // TypeInfo:  0x02BF6C30
    struct UINetworkTextInfo
    {
        char* m_StringId; //0x0000
        float m_DisplayTime; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 02209
    // TypeInfo:  0x02BF6D30
    struct UINetworkPlayerCollectibleTextMessage
    {
        char _0x0000[56];
        UINetworkTextInfo m_TextInfo; //0x0038
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02208
    // TypeInfo:  0x02BF6D20
    struct UINetworkPlayerTutorialInstructionsTextMessage
    {
        char _0x0000[56];
        UINetworkTextInfo m_TextInfo; //0x0038
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02207
    // TypeInfo:  0x02BF6D10
    struct UINetworkPlayerMissionObjectiveTextMessage
    {
        char _0x0000[56];
        UINetworkTextInfo m_TextInfo; //0x0038
        char* m_ObjectiveBriefSid; //0x0040
        __int32 m_ObjectiveId; //0x0044
        unsigned __int32 m_ObjectiveCount; //0x0048
        ObjectiveType m_ObjectiveType; //0x004C
        bool m_DisplayObjective; //0x0050
        bool m_IsSuccess; //0x0051
        char _0x0052[6];
    };//Size=0x0058

    ////////////////////////////////////////
    // RuntimeId: 02206
    // TypeInfo:  0x02BF6D00
    struct UINetworkVideoDoneMessage
    {
        char _0x0000[56];
        unsigned __int64 m_MoviePlayerId; //0x0038
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02205
    // TypeInfo:  0x02BF6CF0
    struct UINetworkAllowSkipVideoMessage
    {
        char _0x0000[56];
        unsigned __int64 m_MoviePlayerId; //0x0038
        bool m_AllowSkip; //0x0040
        char _0x0041[7];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 02204
    // TypeInfo:  0x02BF6CE0
    struct UINetworkSkipVideoMessage
    {
        char _0x0000[56];
        unsigned __int64 m_MoviePlayerId; //0x0038
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02203
    // TypeInfo:  0x02BF6CD0
    struct UINetworkStopVideoMessage
    {
        char _0x0000[56];
        unsigned __int64 m_MoviePlayerId; //0x0038
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02202
    // TypeInfo:  0x02BF6CC0
    struct UINetworkPlayVideoMessage
    {
        char _0x0000[56];
        char* m_VideoName; //0x0038
        char _0x003C[4];
        unsigned __int64 m_MoviePlayerId; //0x0040
        Guid m_ChunkGuid; //0x0048
        char _0x0048[16];
        unsigned __int32 m_ChunkSize; //0x0058
        Guid m_SubtitleChunkGuid; //0x005C
        char _0x005C[16];
        unsigned __int32 m_SubtitleChunkSize; //0x006C
        bool m_KeepBlackScreen; //0x0070
        bool m_AllowSkip; //0x0071
        bool m_HasLocalizedAudioTracks; //0x0072
        bool m_OverrideBackgroundMusic; //0x0073
        char _0x0074[4];
    };//Size=0x0078

    ////////////////////////////////////////
    // RuntimeId: 02201
    // TypeInfo:  0x02BF6CB0
    struct UINetworkPlayerKillsTextMessage
    {
        char _0x0000[56];
        UINetworkTextInfo m_TextInfo; //0x0038
        unsigned __int32 m_KillerId; //0x0040
        unsigned __int32 m_VictimId; //0x0044
        bool m_IsHeadshot; //0x0048
        char _0x0049[3];
        PlayerKilledWeaponType m_WeaponType; //0x004C
        SoldierWeaponUnlockAsset* m_WeaponUnlock; //0x0050
        VehicleEntityData* m_VehicleData; //0x0054
    };//Size=0x0058

    ////////////////////////////////////////
    // RuntimeId: 02200
    // TypeInfo:  0x02BF6CA0
    struct UINetworkPlayerDisconnectMessage
    {
        char _0x0000[56];
        TeamId m_TeamId; //0x0038
        char* m_PlayerName; //0x003C
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02199
    // TypeInfo:  0x02BF6C90
    struct UINetworkPlayerConnectMessage
    {
        char _0x0000[56];
        TeamId m_TeamId; //0x0038
        char* m_PlayerName; //0x003C
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02198
    // TypeInfo:  0x02BF6C80
    struct UINetworkPlayerDeserterTextMessage
    {
        char _0x0000[56];
        unsigned __int32 m_TimeToReturn; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02197
    // TypeInfo:  0x02BF6C70
    struct UINetworkGunMasterNotificationMessage
    {
        char _0x0000[56];
        GunMasterNotificationType m_NotificationType; //0x0038
        unsigned __int32 m_Level; //0x003C
        unsigned __int32 m_PlayerId; //0x0040
        char _0x0044[4];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 02194
    // TypeInfo:  0x02BF6C50
    struct UINetworkAdminYellMessage
    {
        char _0x0000[56];
        UINetworkTextInfo m_TextInfo; //0x0038
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02193
    // TypeInfo:  0x02BF6C40
    struct UINetworkPlayerTextMessage
    {
        char _0x0000[56];
        UINetworkTextInfo m_TextInfo; //0x0038
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02210
    // TypeInfo:  0x02BF6D40
    struct UINetworkVoiceOverSubtitleTextMessage
    {
        char _0x0000[56];
        UINetworkTextInfo m_TextInfo; //0x0038
        TeamId m_Team; //0x0040
        char* m_Sid2; //0x0044
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 02190
    // TypeInfo:  0x02BF6C20
    struct UINetworkHudTextMessage
    {
        char _0x0000[56];
        char* m_Text; //0x0038
        char _0x003C[8];
        HudTextReceiver m_Receiver; //0x0044
        UIMessageType m_TextType; //0x0048
        char _0x004C[4];
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 02189
    // TypeInfo:  0x02BF6C10
    struct UINetworkHudTooltipMessage
    {
        char _0x0000[56];
        unsigned __int32 m_EntryInputAction; //0x0038
        char* m_TooltipSid; //0x003C
        bool m_Show; //0x0040
        char _0x0041[7];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 02188
    // TypeInfo:  0x02BF6C00
    struct UINetworkEnableHudMessage
    {
        char _0x0000[56];
        bool m_Enabled; //0x0038
        char _0x0039[7];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02187
    // TypeInfo:  0x02BF6BF0
    struct NetworkVoteDetails
    {
        unsigned __int32 m_Issuer; //0x0000
        bool m_Personal; //0x0004
        __int8 m_VoteId; //0x0005
        char _0x0006[2];
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 02163
    // TypeInfo:  0x02BF6B30
    struct NetworkMatchReadyStatusChangedMessage
    {
        char _0x0000[64];
        bool m_Status; //0x0040
        char _0x0041[7];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 02162
    // TypeInfo:  0x02BF6B20
    struct VeniceNetworkSpotExplosionPackMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02161
    // TypeInfo:  0x02BF6B10
    struct NetworkMetricsSaveGameSavedMessage
    {
        char _0x0000[56];
        char* m_Name; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02160
    // TypeInfo:  0x02BF6B00
    struct NetworkSetPlayerViewMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02159
    // TypeInfo:  0x02BF6AF0
    struct NetworkSuicideMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 02158
    // TypeInfo:  0x02BF6AE0
    struct NetworkGameplayContinueMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 02156
    // TypeInfo:  0x02BF6AC0
    struct PerformanceFpsHistogram
    {
        float m_Below5; //0x0000
        float m_Below10; //0x0004
        float m_Below15; //0x0008
        float m_Below20; //0x000C
        float m_Below25; //0x0010
        float m_Below30; //0x0014
        float m_Below60; //0x0018
        float m_Above60; //0x001C
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 02157
    // TypeInfo:  0x02BF6AD0
    struct PerformanceLogMessage
    {
        char _0x0000[56];
        float m_Cpu; //0x0038
        float m_CpuCustomTimingEvent; //0x003C
        float m_Gpu; //0x0040
        float m_FreeCpuMemory; //0x0044
        float m_FreeGpuMemory; //0x0048
        float m_AllocCpuMemory; //0x004C
        float m_AllocGpuMemory; //0x0050
        unsigned __int32 m_SpikeDrawCallCount; //0x0054
        unsigned __int32 m_SpikePrimitiveCount; //0x0058
        unsigned __int32 m_Received; //0x005C
        unsigned __int32 m_Sent; //0x0060
        float m_CpuAverage; //0x0064
        float m_CpuCustomTimingEventAverage; //0x0068
        float m_GpuAverage; //0x006C
        float m_ReceivedAverage; //0x0070
        float m_SentAverage; //0x0074
        float m_SpikeAverage; //0x0078
        unsigned __int32 m_LastFps; //0x007C
        unsigned __int32 m_LastDrawCallCount; //0x0080
        unsigned __int32 m_LastPrimitiveCount; //0x0084
        unsigned __int32 m_ServerPlayerCount; //0x0088
        unsigned __int32 m_VehicleActiveCount; //0x008C
        unsigned __int32 m_VehicleTotalCount; //0x0090
        float m_LargestRSXBlockFree; //0x0094
        char _0x0098[8];
        Vec3 m_Position; //0x00A0
        Vec3 m_Direction; //0x00B0
        char* m_Location; //0x00C0
        PerformanceFpsHistogram m_FpsHistogram; //0x00C4
        char _0x00E4[12];
    };//Size=0x00F0

    ////////////////////////////////////////
    // RuntimeId: 02154
    // TypeInfo:  0x02BF6AB0
    struct NetworkFirstPlayerEnteredMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 02153
    // TypeInfo:  0x02BF6AA0
    struct NetworkDifficultyChangedMessage
    {
        char _0x0000[56];
        unsigned __int32 m_DifficultyIndex; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02152
    // TypeInfo:  0x02BF6A90
    struct NetworkSettingsMessage
    {
        char _0x0000[56];
        bool m_AllUnlocksUnlocked; //0x0038
        bool m_UnlicensedUnlocksUnlocked; //0x0039
        bool m_EnableFriendlyFire; //0x003A
        bool m_NoMinimap; //0x003B
        bool m_NoHud; //0x003C
        bool m_NoMinimapSpotting; //0x003D
        bool m_No3dSpotting; //0x003E
        bool m_NoNameTag; //0x003F
        bool m_DisableToggleEntryCamera; //0x0040
        bool m_DisableRegenerateHealth; //0x0041
        bool m_AllowClientSideDamageArbitration; //0x0042
        bool m_OnlySquadLeaderSpawn; //0x0043
        bool m_TeamSwitchingAllowed; //0x0044
        bool m_MemoryDefragUI; //0x0045
        bool m_MemoryGarbageCollectHavok; //0x0046
        bool m_MemoryDefragGlobal; //0x0047
        unsigned __int32 m_DifficultyIndex; //0x0048
        float m_ManDownTimeModifier; //0x004C
        float m_BulletDamageModifier; //0x0050
        float m_GameModeCounterModifier; //0x0054
        float m_MaxAllowedLatency; //0x0058
        char _0x005C[4];
    };//Size=0x0060

    ////////////////////////////////////////
    // RuntimeId: 02151
    // TypeInfo:  0x02BF6A80
    struct DebugSpawnGameEntityMessage
    {
        char _0x0000[56];
        ObjectBlueprint* m_Blueprint; //0x0038
        char _0x003C[4];
        LinearTransform m_Transform; //0x0040
    };//Size=0x0080

    ////////////////////////////////////////
    // RuntimeId: 02150
    // TypeInfo:  0x02BF6A70
    struct NetworkCreatePlayerMessage
    {
        char _0x0000[56];
        char* m_Name; //0x0038
        bool m_IsSpectator; //0x003C
        char _0x003D[3];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02149
    // TypeInfo:  0x02BF6A60
    struct NetworkCameraReplayMessage
    {
        char _0x0000[56];
        char* m_Filename; //0x0038
        __int32 m_CameraFileIndex; //0x003C
        bool m_Enable; //0x0040
        char _0x0041[7];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 02148
    // TypeInfo:  0x02BF6A50
    struct NetworkCameraFreeCameraMessage
    {
        char _0x0000[64];
        LinearTransform m_Transform; //0x0040
    };//Size=0x0080

    ////////////////////////////////////////
    // RuntimeId: 02147
    // TypeInfo:  0x02BF6A40
    struct NetworkClientCameraControlMessage
    {
        char _0x0000[56];
        bool m_IsClientControlled; //0x0038
        char _0x0039[7];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02146
    // TypeInfo:  0x02BF6A30
    struct NetworkCameraPositionMessage
    {
        char _0x0000[64];
        LinearTransform m_Transform; //0x0040
    };//Size=0x0080

    ////////////////////////////////////////
    // RuntimeId: 02145
    // TypeInfo:  0x02BF6A20
    struct NetworkMovePlayerMessage
    {
        char _0x0000[64];
        LinearTransform m_Transform; //0x0040
        unsigned __int32 m_PlayerId; //0x0080
        char _0x0084[12];
    };//Size=0x0090

    ////////////////////////////////////////
    // RuntimeId: 02144
    // TypeInfo:  0x02BF6A10
    struct NetworkJuiceSessionMessage
    {
        char _0x0000[56];
        __int32 m_SessionId; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02143
    // TypeInfo:  0x02BF6A00
    struct NetworkSelectTeamMessage
    {
        char _0x0000[56];
        TeamId m_Team; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02142
    // TypeInfo:  0x02BF69F0
    struct NetworkOnPlayerSpawnedMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02141
    // TypeInfo:  0x02BF69E0
    struct NetworkSelectSpawnGroupMessage
    {
        char _0x0000[72];
        __int32 m_PlayerToSpawnOnId; //0x0048
        char _0x004C[4];
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 02140
    // TypeInfo:  0x02BF69D0
    struct NetworkSpawnVehicleCustomizationMessage
    {
        char _0x0000[56];
        VehicleBlueprint* m_VehicleBlueprint; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02139
    // TypeInfo:  0x02BF69C0
    struct NetworkUnSpawnCustomizationMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 02138
    // TypeInfo:  0x02BF69B0
    struct NetworkSpawnCustomizationMessage
    {
        char _0x0000[56];
        SoldierBlueprint* m_SoldierBlueprint; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02137
    // TypeInfo:  0x02BF69A0
    struct NetworkSpawnOnSelectedMessage
    {
        char _0x0000[56];
        SoldierBlueprint* m_SoldierBlueprint; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02136
    // TypeInfo:  0x02BF6990
    struct NetworkSpawnHereMessage
    {
        char _0x0000[56];
        SoldierBlueprint* m_SoldierBlueprint; //0x0038
        char _0x003C[4];
        LinearTransform m_Transform; //0x0040
    };//Size=0x0080

    ////////////////////////////////////////
    // RuntimeId: 02135
    // TypeInfo:  0x02BF6980
    struct NetworkSpawnMessage
    {
        char _0x0000[56];
        SoldierBlueprint* m_SoldierBlueprint; //0x0038
        char _0x003C[8];
        bool m_Squad; //0x0044
        bool m_SpawnInstantly; //0x0045
        char _0x0046[2];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 02120
    // TypeInfo:  0x02BF68C0
    struct LevelSetupOption
    {
        char* m_Criterion; //0x0000
        char* m_Value; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 02133
    // TypeInfo:  0x02BF6960
    struct NetworkScreenFadeMessage
    {
        char _0x0000[56];
        float m_FadeTime; //0x0038
        bool m_FadeToBlack; //0x003C
        char _0x003D[3];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02132
    // TypeInfo:  0x02BF6950
    struct NetworkChangeGameSettingMessage
    {
        char _0x0000[56];
        ChangeGameSettingType m_SettingType; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02129
    // TypeInfo:  0x02BF6930
    struct LevelConsoleSetActiveHealthStateMessage
    {
        char _0x0000[56];
        unsigned __int32 m_HealthStateIndex; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02128
    // TypeInfo:  0x02BF6920
    struct NetworkTimeSyncMessage
    {
        char _0x0000[56];
        bool m_TriggerTimeSync; //0x0038
        char _0x0039[3];
        unsigned __int32 m_Ticks; //0x003C
        float m_Time; //0x0040
        char _0x0044[4];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 02122
    // TypeInfo:  0x02BF68D0
    struct LevelSetup
    {
        char* m_Name; //0x0000
        Array<LevelSetupOption> m_InclusionOptions; //0x0004
        unsigned __int32 m_DifficultyIndex; //0x0008
        Array<char*> m_SubLevelNames; //0x000C
        Array<__int32> m_SubLevelStates; //0x0010
        bool m_IsSaveGame; //0x0014
        bool m_ForceReloadResources; //0x0015
        char _0x0016[2];
    };//Size=0x0018

    ////////////////////////////////////////
    // RuntimeId: 02126
    // TypeInfo:  0x02BF6900
    struct NetworkLevelLoadedAckMessage
    {
        char _0x0000[56];
        char* m_LevelName; //0x0038
        Guid m_Checksum; //0x003C
        char _0x003C[20];
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 02125
    // TypeInfo:  0x02BF68F0
    struct NetworkPerformanceProfileMessage
    {
        char _0x0000[56];
        unsigned __int32 m_ProcessorCount; //0x0038
        unsigned __int32 m_ProcessorCoreCount; //0x003C
        unsigned __int32 m_ProcessorClock; //0x0040
        char _0x0044[4];
        unsigned __int64 m_TotalMem; //0x0048
        unsigned __int64 m_GpuMem; //0x0050
        char* m_GraphicAdapterName; //0x0058
        PerformanceProfilePlatform m_Platform; //0x005C
    };//Size=0x0060

    ////////////////////////////////////////
    // RuntimeId: 02134
    // TypeInfo:  0x02BF6970
    struct NetworkRequestLoadLevelMessage
    {
        char _0x0000[56];
        LevelSetup m_Setup; //0x0038
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 02127
    // TypeInfo:  0x02BF6910
    struct NetworkLoadLevelMessage
    {
        char _0x0000[56];
        LevelSetup m_Setup; //0x0038
        unsigned __int32 m_LevelCounter; //0x0050
        char _0x0054[4];
    };//Size=0x0058

    ////////////////////////////////////////
    // RuntimeId: 02118
    // TypeInfo:  0x02BF68B0
    struct SyncedSequenceStateChangeMessageBase
    {
        char _0x0000[56];
        SyncedSequenceEntityData* m_Data; //0x0038
        float m_ServerTime; //0x003C
        bool m_Start; //0x0040
        char _0x0041[7];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 02117
    // TypeInfo:  0x02BF68A0
    struct DamageArbitrationClientDamageMessage
    {
        char _0x0000[112];
    };//Size=0x0070

    ////////////////////////////////////////
    // RuntimeId: 02116
    // TypeInfo:  0x02BF6890
    struct ClientConnectionUnloadLevelMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02115
    // TypeInfo:  0x02BF6880
    struct ClientConnectionLinkLevelMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02114
    // TypeInfo:  0x02BF6870
    struct ClientConnectionLoadLevelMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02113
    // TypeInfo:  0x02BF6860
    struct ClientConnectionInitializedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02112
    // TypeInfo:  0x02BF6850
    struct ServerPeerLoadLevelMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02111
    // TypeInfo:  0x02BF6840
    struct ServerPeerInitializedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02110
    // TypeInfo:  0x02BF6830
    struct JuiceSoldierRagdollDeactivateMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02109
    // TypeInfo:  0x02BF6820
    struct JuiceSoldierRagdollActivateMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02108
    // TypeInfo:  0x02BF6810
    struct ServerMissionObjectiveCompletedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02107
    // TypeInfo:  0x02BF6800
    struct CoreWriteSaveGameDoneMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02106
    // TypeInfo:  0x02BF67F0
    struct CoreWriteSaveGameMessage
    {
        char _0x0000[72];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 02105
    // TypeInfo:  0x02BF67E0
    struct StatisticsEventMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02104
    // TypeInfo:  0x02BF67D0
    struct AISpawnBotMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 02103
    // TypeInfo:  0x02BF67C0
    struct AIPlayerEnableAsTargetMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02102
    // TypeInfo:  0x02BF67B0
    struct SessionPlayerJoinedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02101
    // TypeInfo:  0x02BF67A0
    struct SessionPlayerAuthenticatedMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 02100
    // TypeInfo:  0x02BF6790
    struct SessionPlayerLeftMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02099
    // TypeInfo:  0x02BF6780
    struct ClientSetServerPasswordMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02098
    // TypeInfo:  0x02BF6770
    struct ClientWantFullscreenMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02097
    // TypeInfo:  0x02BF6760
    struct ClientLeftRemoteServerMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02096
    // TypeInfo:  0x02BF6750
    struct ClientDisconnectedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02095
    // TypeInfo:  0x02BF6740
    struct ClientConnectedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02094
    // TypeInfo:  0x02BF6730
    struct ClientAbortCutsceneMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02093
    // TypeInfo:  0x02BF6720
    struct ClientLevelLoadedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02092
    // TypeInfo:  0x02BF6710
    struct ClientLevelLoadProgressMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02091
    // TypeInfo:  0x02BF6700
    struct ClientLevelDescriptionLoadedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02090
    // TypeInfo:  0x02BF66F0
    struct ClientLevelUnloadedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02089
    // TypeInfo:  0x02BF66E0
    struct ClientLoadLevelMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02088
    // TypeInfo:  0x02BF66D0
    struct ClientLoadLevelRequestedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02087
    // TypeInfo:  0x02BF66C0
    struct ClientEnteredIngameMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02086
    // TypeInfo:  0x02BF66B0
    struct ClientEnterHudIngameMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02085
    // TypeInfo:  0x02BF66A0
    struct ClientExitToMenuMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02084
    // TypeInfo:  0x02BF6690
    struct ClientReturnToMenuMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02083
    // TypeInfo:  0x02BF6680
    struct ClientStartMultiplayerMessage
    {
        char _0x0000[72];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 02082
    // TypeInfo:  0x02BF6670
    struct ClientContinueSingleplayerMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02081
    // TypeInfo:  0x02BF6660
    struct ClientStartedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02080
    // TypeInfo:  0x02BF6650
    struct ClientJoinServerJobMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02079
    // TypeInfo:  0x02BF6640
    struct ClientPeerNetworkRemovedMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02078
    // TypeInfo:  0x02BF6630
    struct ClientJoinMultiplayerMessageBase
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02077
    // TypeInfo:  0x02BF6620
    struct ClientRestartSingleplayerMessageBase
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02076
    // TypeInfo:  0x02BF6610
    struct ClientStartSingleplayerMessageBase
    {
        char _0x0000[72];
    };//Size=0x0048

    ////////////////////////////////////////
    // RuntimeId: 02075
    // TypeInfo:  0x02BF6600
    struct ClientStateResponseLoadingIngameBundlesMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02074
    // TypeInfo:  0x02BF65F0
    struct ClientStateRequestLoadingIngameBundlesMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02073
    // TypeInfo:  0x02BF65E0
    struct ClientStateRequestUnloadMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02072
    // TypeInfo:  0x02BF65D0
    struct ClientStateRequestLoadingLevelMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02071
    // TypeInfo:  0x02BF65C0
    struct ClientStateRequestIngameMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02070
    // TypeInfo:  0x02BF65B0
    struct ClientStateRequestFrontendMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02069
    // TypeInfo:  0x02BF65A0
    struct ClientStateRequestStaticMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02068
    // TypeInfo:  0x02BF6590
    struct ClientStateEnteredUnloadMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02067
    // TypeInfo:  0x02BF6580
    struct ClientStateEnteredLoadingLevelMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02066
    // TypeInfo:  0x02BF6570
    struct ClientStateEnteredIngameMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02065
    // TypeInfo:  0x02BF6560
    struct ClientStateEnteredFrontendMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02064
    // TypeInfo:  0x02BF6550
    struct ClientStateEnteredStaticMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02063
    // TypeInfo:  0x02BF6540
    struct CoreGameTimerMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02062
    // TypeInfo:  0x02BF6530
    struct CoreExitIngameMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02061
    // TypeInfo:  0x02BF6520
    struct CoreEnteredIngameMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02060
    // TypeInfo:  0x02BF6510
    struct PerformanceClientNetworkMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02059
    // TypeInfo:  0x02BF6500
    struct PerformanceServerNetworkMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02058
    // TypeInfo:  0x02BF64F0
    struct PerformanceServerMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02057
    // TypeInfo:  0x02BF64E0
    struct ProfileOptionsSettingsSavedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02056
    // TypeInfo:  0x02BF64D0
    struct ProfileOptionsSettingsPreSaveMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02055
    // TypeInfo:  0x02BF64C0
    struct ProfileOptionsSettingsLoadedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02054
    // TypeInfo:  0x02BF64B0
    struct ProfileOptionsApplyMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02053
    // TypeInfo:  0x02BF64A0
    struct NetworkDisconnectedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02052
    // TypeInfo:  0x02BF6490
    struct NetworkConnectedMessage
    {
        char _0x0000[112];
    };//Size=0x0070

    ////////////////////////////////////////
    // RuntimeId: 02051
    // TypeInfo:  0x02BF6480
    struct ServerClientConnectionRemovedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02050
    // TypeInfo:  0x02BF6470
    struct ServerClientConnectionConnectedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02049
    // TypeInfo:  0x02BF6460
    struct ServerAdminBanPlayerMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02048
    // TypeInfo:  0x02BF6450
    struct ServerAdminSetServerNameMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02047
    // TypeInfo:  0x02BF6440
    struct ServerScriptTickMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02046
    // TypeInfo:  0x02BF6430
    struct ClientRestartTimerMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02045
    // TypeInfo:  0x02BF6420
    struct ServerRestartTimerMessage
    {
        char _0x0000[56];
        __int32 m_Ticks; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 02044
    // TypeInfo:  0x02BF6410
    struct ServerStopMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02043
    // TypeInfo:  0x02BF6400
    struct ServerLoadLevelMessageBase
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02042
    // TypeInfo:  0x02BF63F0
    struct ServerInternalLoadLevelMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02041
    // TypeInfo:  0x02BF63E0
    struct ServerLevelUnloadedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02040
    // TypeInfo:  0x02BF63D0
    struct ServerUnloadLevelMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02039
    // TypeInfo:  0x02BF63C0
    struct ServerLevelLoadedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02038
    // TypeInfo:  0x02BF63B0
    struct ServerStoppedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02037
    // TypeInfo:  0x02BF63A0
    struct ServerApplyConfigurationMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02036
    // TypeInfo:  0x02BF6390
    struct ServerResetConfigurationMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02035
    // TypeInfo:  0x02BF6380
    struct ServerStartedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02034
    // TypeInfo:  0x02BF6370
    struct ServerSubLevelOnStreamedInMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02033
    // TypeInfo:  0x02BF6360
    struct ServerLevelCompletedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02032
    // TypeInfo:  0x02BF6350
    struct ServerLevelStartedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02031
    // TypeInfo:  0x02BF6340
    struct ServerLevelSpawnEntitiesEndMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02030
    // TypeInfo:  0x02BF6330
    struct ServerLevelSpawnEntitiesBeginMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02029
    // TypeInfo:  0x02BF6320
    struct ClientLevelFinalizedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02028
    // TypeInfo:  0x02BF6310
    struct ClientLevelSpawnEntitiesEndMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02027
    // TypeInfo:  0x02BF6300
    struct ClientLevelSpawnDebugEntitiesMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02026
    // TypeInfo:  0x02BF62F0
    struct ServerRoundInterruptedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02025
    // TypeInfo:  0x02BF62E0
    struct ServerRoundOverMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02024
    // TypeInfo:  0x02BF62D0
    struct ServerRoundResetMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02023
    // TypeInfo:  0x02BF62C0
    struct ServerGameModeResetMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02022
    // TypeInfo:  0x02BF62B0
    struct ServerGameplayGameModeResetMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02021
    // TypeInfo:  0x02BF62A0
    struct ServerGameplayServerPlayerMenuCancelMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02020
    // TypeInfo:  0x02BF6290
    struct ServerGameplayServerPlayerMenuOkMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02019
    // TypeInfo:  0x02BF6280
    struct ServerGameplayPreviousWeatherStateMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02018
    // TypeInfo:  0x02BF6270
    struct ServerGameplayFightHarderMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02017
    // TypeInfo:  0x02BF6260
    struct ServerGameplayMedkitMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02016
    // TypeInfo:  0x02BF6250
    struct ServerGameplayDeserterReturnMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02015
    // TypeInfo:  0x02BF6240
    struct ServerGameplayDeserterMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02014
    // TypeInfo:  0x02BF6230
    struct ServerGameplayPlayerMenuCancelMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02013
    // TypeInfo:  0x02BF6220
    struct ServerGameplayPlayerMenuOkMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02012
    // TypeInfo:  0x02BF6210
    struct ServerGameplayVoiceOverFinishedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02011
    // TypeInfo:  0x02BF6200
    struct ClientGameplayControllableLowHealthMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02010
    // TypeInfo:  0x02BF61F0
    struct ClientGameplaySoldierHitMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02009
    // TypeInfo:  0x02BF61E0
    struct UIComponentDeferActionMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02008
    // TypeInfo:  0x02BF61D0
    struct UIComponentOnItemChangedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02007
    // TypeInfo:  0x02BF61C0
    struct UIComponentChangeCameraViewPointMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02006
    // TypeInfo:  0x02BF61B0
    struct UIComponentSetSettingMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 02005
    // TypeInfo:  0x02BF61A0
    struct UIComponentLoadCompleteMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02004
    // TypeInfo:  0x02BF6190
    struct UIReadyForUnloadMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 02003
    // TypeInfo:  0x02BF6180
    struct UISoldierChangeMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 02000
    // TypeInfo:  0x02BF6160
    struct UIInputPressedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01999
    // TypeInfo:  0x02BF6150
    struct UIGraphExitedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01998
    // TypeInfo:  0x02BF6140
    struct UIScreenLoadedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01997
    // TypeInfo:  0x02BF6130
    struct UIBundleLoadedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01996
    // TypeInfo:  0x02BF6120
    struct UITransitionEffectStopMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 01995
    // TypeInfo:  0x02BF6110
    struct UITransitionEffectStartMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 01994
    // TypeInfo:  0x02BF6100
    struct UIExitToMenuReasonMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01993
    // TypeInfo:  0x02BF60F0
    struct UIVehicleHitDirectionUpdatedMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 01992
    // TypeInfo:  0x02BF60E0
    struct UISquadStatusChangedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 01991
    // TypeInfo:  0x02BF60D0
    struct UISoldierHitUpdatedMessage
    {
        char _0x0000[80];
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 01990
    // TypeInfo:  0x02BF60C0
    struct UIReturnMouseToUIMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 01989
    // TypeInfo:  0x02BF60B0
    struct UIPlayerVehicleHealthChangeMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01988
    // TypeInfo:  0x02BF60A0
    struct UIInputStatusChangedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01987
    // TypeInfo:  0x02BF6090
    struct UIHudWarningMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01986
    // TypeInfo:  0x02BF6080
    struct UIHudUpdateCrosshairMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 01985
    // TypeInfo:  0x02BF6070
    struct UIHudToggleMapZoomMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 01984
    // TypeInfo:  0x02BF6060
    struct UIHudShowVoteResultsMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 01983
    // TypeInfo:  0x02BF6050
    struct UIHudShowVoteMenuMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 01982
    // TypeInfo:  0x02BF6040
    struct UIHudOutputStaticMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01981
    // TypeInfo:  0x02BF6030
    struct UIHudDistortHudMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01980
    // TypeInfo:  0x02BF6020
    struct UIHudDebugPauseMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01979
    // TypeInfo:  0x02BF6010
    struct UIHudChatMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01978
    // TypeInfo:  0x02BF6000
    struct UIHudChangeInventoryWeaponMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 01977
    // TypeInfo:  0x02BF5FF0
    struct UIHasSuppressedEnemyMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01976
    // TypeInfo:  0x02BF5FE0
    struct UIDamageGivenToEnemyMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01975
    // TypeInfo:  0x02BF5FD0
    struct UICycleRadioChannelMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01974
    // TypeInfo:  0x02BF5FC0
    struct ClientCameraShowKillerMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 01973
    // TypeInfo:  0x02BF5FB0
    struct ServerStaticModelDamagedPartByPlayerMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 01972
    // TypeInfo:  0x02BF5FA0
    struct ServerStaticModelDamagedPartMessage
    {
        char _0x0000[112];
    };//Size=0x0070

    ////////////////////////////////////////
    // RuntimeId: 01971
    // TypeInfo:  0x02BF5F90
    struct ServerStaticModelGroupDestroyedPartMessage
    {
        char _0x0000[96];
    };//Size=0x0060

    ////////////////////////////////////////
    // RuntimeId: 01970
    // TypeInfo:  0x02BF5F80
    struct ServerStaticModelDestroyedAllCollapsablePartsMessage
    {
        char _0x0000[160];
    };//Size=0x00A0

    ////////////////////////////////////////
    // RuntimeId: 01969
    // TypeInfo:  0x02BF5F70
    struct ServerStaticModelSpawnMessage
    {
        char _0x0000[144];
    };//Size=0x0090

    ////////////////////////////////////////
    // RuntimeId: 01968
    // TypeInfo:  0x02BF5F60
    struct ServerSoundVoiceOverFinishedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01967
    // TypeInfo:  0x02BF5F50
    struct ServerInputDeactivateInputRestrictionMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01966
    // TypeInfo:  0x02BF5F40
    struct ServerInputReactivateInputRestrictionMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01965
    // TypeInfo:  0x02BF5F30
    struct ServerCollisionExplosionPackDestroyedMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 01964
    // TypeInfo:  0x02BF5F20
    struct ServerCollisionExplosionPackPlacedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01963
    // TypeInfo:  0x02BF5F10
    struct ServerCollisionExplosionUnSpawnMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01962
    // TypeInfo:  0x02BF5F00
    struct ServerCollisionExplosionDamageMessage
    {
        char _0x0000[80];
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 01961
    // TypeInfo:  0x02BF5EF0
    struct ServerCollisionExplosionPreDamageMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 01960
    // TypeInfo:  0x02BF5EE0
    struct ServerCollisionExplosionSpawnMessage
    {
        char _0x0000[80];
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 01959
    // TypeInfo:  0x02BF5ED0
    struct ServerCollisionProjectileTimeoutMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 01958
    // TypeInfo:  0x02BF5EC0
    struct ServerCollisionProjectileImpactMessage
    {
        char _0x0000[80];
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 01957
    // TypeInfo:  0x02BF5EB0
    struct ServerCollisionProjectileFireMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 01956
    // TypeInfo:  0x02BF5EA0
    struct ServerCollisionGrenadeCollisionMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 01955
    // TypeInfo:  0x02BF5E90
    struct ServerCollisionGrenadeThrowMessage
    {
        char _0x0000[80];
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 01954
    // TypeInfo:  0x02BF5E80
    struct ClientCollisionExplosionPackDetonatedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01953
    // TypeInfo:  0x02BF5E70
    struct ClientCollisionExplosionPackPlacedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01952
    // TypeInfo:  0x02BF5E60
    struct ClientCollisionGrenadeCollisionMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 01951
    // TypeInfo:  0x02BF5E50
    struct ServerWeaponPlayerPrimaryFireLockedShotSpawnedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01950
    // TypeInfo:  0x02BF5E40
    struct ServerWeaponWeaponComponentActivateMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01949
    // TypeInfo:  0x02BF5E30
    struct ServerWeaponWeaponComponentReloadMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01948
    // TypeInfo:  0x02BF5E20
    struct ServerWeaponPlayerResupplyMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 01947
    // TypeInfo:  0x02BF5E10
    struct ServerWeaponPlayerReloadMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 01946
    // TypeInfo:  0x02BF5E00
    struct ServerWeaponPlayerWeaponRemovedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01945
    // TypeInfo:  0x02BF5DF0
    struct ServerWeaponPlayerWeaponReloadEndMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01944
    // TypeInfo:  0x02BF5DE0
    struct ServerWeaponPlayerPrimaryFireShotSpawnedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01943
    // TypeInfo:  0x02BF5DD0
    struct ServerWeaponSoldierWeaponReloadMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01942
    // TypeInfo:  0x02BF5DC0
    struct ServerWeaponSoldierWeaponActivateMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01941
    // TypeInfo:  0x02BF5DB0
    struct ServerWeaponPlayerPrimaryOutOfAmmoMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01940
    // TypeInfo:  0x02BF5DA0
    struct ServerWeaponArtilleryFiredMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 01939
    // TypeInfo:  0x02BF5D90
    struct ServerWeaponLaserDesignatorMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01938
    // TypeInfo:  0x02BF5D80
    struct ServerWeaponMortarStrikeMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 01937
    // TypeInfo:  0x02BF5D70
    struct ClientSoundVoiceOverFinishedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01936
    // TypeInfo:  0x02BF5D60
    struct ClientWeaponDispersionUpdatedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 01935
    // TypeInfo:  0x02BF5D50
    struct ClientWeaponPlayerPrimaryOutOfAmmoMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01934
    // TypeInfo:  0x02BF5D40
    struct ClientWeaponPlayerWeaponReloadEndMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 01933
    // TypeInfo:  0x02BF5D30
    struct ClientWeaponPlayerWeaponReloadBeginMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01932
    // TypeInfo:  0x02BF5D20
    struct ClientWeaponPlayerPrimaryWeaponFireMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01931
    // TypeInfo:  0x02BF5D10
    struct ClientWeaponPlayerWeaponChangeMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 01930
    // TypeInfo:  0x02BF5D00
    struct ClientVehicleCriticalDamageMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01929
    // TypeInfo:  0x02BF5CF0
    struct ServerVehicleJumpMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 01928
    // TypeInfo:  0x02BF5CE0
    struct ServerVehicleLockableMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 01927
    // TypeInfo:  0x02BF5CD0
    struct ServerVehicleExitMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01926
    // TypeInfo:  0x02BF5CC0
    struct ServerVehicleEnterMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01925
    // TypeInfo:  0x02BF5CB0
    struct ServerVehicleDisabledMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01924
    // TypeInfo:  0x02BF5CA0
    struct ServerVehicleDamageMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 01923
    // TypeInfo:  0x02BF5C90
    struct ServerVehicleEnterRestrictionMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 01922
    // TypeInfo:  0x02BF5C80
    struct ServerVehicleUnspawnMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01921
    // TypeInfo:  0x02BF5C70
    struct ServerVehicleSpawnDoneMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01920
    // TypeInfo:  0x02BF5C60
    struct ServerVehicleForceArmamentReturnMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01919
    // TypeInfo:  0x02BF5C50
    struct ServerVehicleSwitchTeamMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01918
    // TypeInfo:  0x02BF5C40
    struct ServerVehicleDestroyedMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 01917
    // TypeInfo:  0x02BF5C30
    struct ServerComponentEntryComponentProcessedInputMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01916
    // TypeInfo:  0x02BF5C20
    struct ServerComponentEntryOnPlayerExitsMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 01915
    // TypeInfo:  0x02BF5C10
    struct ServerComponentEntryOnPlayerEntersMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 01914
    // TypeInfo:  0x02BF5C00
    struct ServerComponentEntryOnUnspawnMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01913
    // TypeInfo:  0x02BF5BF0
    struct ServerComponentWeaponOnUnspawnMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01912
    // TypeInfo:  0x02BF5BE0
    struct ServerComponentWeaponOnSpawnMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01911
    // TypeInfo:  0x02BF5BD0
    struct AIClientBridgeDynamicModelEntityOnUnspawnMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01910
    // TypeInfo:  0x02BF5BC0
    struct AIClientBridgeDynamicModelEntityOnSpawnMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01909
    // TypeInfo:  0x02BF5BB0
    struct ClientEntityDummyToGetTheClientEntityMessageCategoryMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 01908
    // TypeInfo:  0x02BF5BA0
    struct ServerEntityPickupOnUnspawnMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01907
    // TypeInfo:  0x02BF5B90
    struct ServerEntityPickupOnSpawnMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01906
    // TypeInfo:  0x02BF5B80
    struct ServerEntityOnDamageMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01905
    // TypeInfo:  0x02BF5B70
    struct ServerEntityBangerEntityOnUnspawnMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01904
    // TypeInfo:  0x02BF5B60
    struct ServerEntityBangerEntityOnSpawnMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01903
    // TypeInfo:  0x02BF5B50
    struct ServerClubMemberDeletedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01902
    // TypeInfo:  0x02BF5B40
    struct ServerClubMemberCreatedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01901
    // TypeInfo:  0x02BF5B30
    struct ClientCharacterSpawnDoneMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01900
    // TypeInfo:  0x02BF5B20
    struct ClientCharacterLocalPlayerDeletedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 01899
    // TypeInfo:  0x02BF5B10
    struct ClientCharacterLocalPlayerSetMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01898
    // TypeInfo:  0x02BF5B00
    struct ServerSoldierSelfHealMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01897
    // TypeInfo:  0x02BF5AF0
    struct ServerSoldierSoldierDamageMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 01896
    // TypeInfo:  0x02BF5AE0
    struct ServerSoldierManDownMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01895
    // TypeInfo:  0x02BF5AD0
    struct ServerSoldierWeaponReplacedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01894
    // TypeInfo:  0x02BF5AC0
    struct ServerSoldierUnspawnDoneMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01893
    // TypeInfo:  0x02BF5AB0
    struct ServerSoldierSpawnDoneMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01892
    // TypeInfo:  0x02BF5AA0
    struct ServerSoldierOnInitMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01891
    // TypeInfo:  0x02BF5A90
    struct ServerSoldierChangingWeaponMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01890
    // TypeInfo:  0x02BF5A80
    struct ServerSoldierFiringMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01889
    // TypeInfo:  0x02BF5A70
    struct ServerSoldierKilledMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01888
    // TypeInfo:  0x02BF5A60
    struct ServerSoldierDamagedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01887
    // TypeInfo:  0x02BF5A50
    struct ClientSpawnSpawnedOrUnSpawnedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01886
    // TypeInfo:  0x02BF5A40
    struct ClientControllableUnspawnDoneMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01885
    // TypeInfo:  0x02BF5A30
    struct ClientControllableSpawnDoneMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01884
    // TypeInfo:  0x02BF5A20
    struct ServerControllableUnspawnDoneMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01883
    // TypeInfo:  0x02BF5A10
    struct ServerControllableSpawnDoneMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01882
    // TypeInfo:  0x02BF5A00
    struct ServerConnectionExitMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01881
    // TypeInfo:  0x02BF59F0
    struct ServerPlayerChatMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 01880
    // TypeInfo:  0x02BF59E0
    struct ServerPlayerCustomizationDoneMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01879
    // TypeInfo:  0x02BF59D0
    struct ServerPlayerTickMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01878
    // TypeInfo:  0x02BF59C0
    struct ServerPlayerExitEntryMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 01877
    // TypeInfo:  0x02BF59B0
    struct ServerPlayerEnterEntryMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 01876
    // TypeInfo:  0x02BF59A0
    struct ServerPlayerAboutToClearSoldierMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01875
    // TypeInfo:  0x02BF5990
    struct ServerPlayerInstantSuicideMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 01874
    // TypeInfo:  0x02BF5980
    struct ServerPlayerGunMasterLevelChangedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01873
    // TypeInfo:  0x02BF5970
    struct ServerPlayerKilledMessage
    {
        char _0x0000[96];
    };//Size=0x0060

    ////////////////////////////////////////
    // RuntimeId: 01872
    // TypeInfo:  0x02BF5960
    struct ServerPlayerManuallySelectedSpawnPointMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01871
    // TypeInfo:  0x02BF5950
    struct ServerPlayerChangeChatChannelMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 01870
    // TypeInfo:  0x02BF5940
    struct ServerPlayerSwitchTeamMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01869
    // TypeInfo:  0x02BF5930
    struct ServerPlayerKitReplacedMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 01868
    // TypeInfo:  0x02BF5920
    struct ServerPlayerChangedSoldierMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 01867
    // TypeInfo:  0x02BF5910
    struct ServerPlayerReviveRefusedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01866
    // TypeInfo:  0x02BF5900
    struct ServerPlayerReviveAcceptedMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 01865
    // TypeInfo:  0x02BF58F0
    struct ServerPlayerReviveMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 01864
    // TypeInfo:  0x02BF58E0
    struct ServerPlayerLeftLevelMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01863
    // TypeInfo:  0x02BF58D0
    struct ServerPlayerReleasingLevelMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01862
    // TypeInfo:  0x02BF58C0
    struct ServerPlayerEnteredLevelMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01861
    // TypeInfo:  0x02BF58B0
    struct ServerPlayerLevelLoadedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01860
    // TypeInfo:  0x02BF58A0
    struct ServerPlayerDebugFriendZoneSpawnMessage
    {
        char _0x0000[80];
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 01859
    // TypeInfo:  0x02BF5890
    struct ServerPlayerRespawnMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01858
    // TypeInfo:  0x02BF5880
    struct ServerPlayerDestroyMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01857
    // TypeInfo:  0x02BF5870
    struct ServerPlayerCreatedForConnectionMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01856
    // TypeInfo:  0x02BF5860
    struct ServerPlayerCreateMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01855
    // TypeInfo:  0x02BF5850
    struct ServerPlayerAboutToCreateForConnectionMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 01854
    // TypeInfo:  0x02BF5840
    struct ClientSoldierChangeCoverStateMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01853
    // TypeInfo:  0x02BF5830
    struct ClientSoldierOnLandMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 01852
    // TypeInfo:  0x02BF5820
    struct ClientSoldierOnJumpMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 01851
    // TypeInfo:  0x02BF5810
    struct ClientPlayerDeletedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01850
    // TypeInfo:  0x02BF5800
    struct ClientPlayerConnectMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01849
    // TypeInfo:  0x02BF57F0
    struct ClientPlayerLocalSetMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01848
    // TypeInfo:  0x02BF57E0
    struct ClientPlayerChangedPlayerViewMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01847
    // TypeInfo:  0x02BF57D0
    struct ClientPlayerSwitchTeamMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01846
    // TypeInfo:  0x02BF57C0
    struct ClientPlayerManDownMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01845
    // TypeInfo:  0x02BF57B0
    struct ClientPlayerWeaponPickupMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 01844
    // TypeInfo:  0x02BF57A0
    struct ClientPlayerAmmoPickupMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 01843
    // TypeInfo:  0x02BF5790
    struct ClientPlayerKilledMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01842
    // TypeInfo:  0x02BF5780
    struct ClientPlayerEnterEntryMessage
    {
        char _0x0000[64];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 01841
    // TypeInfo:  0x02BF5770
    struct ClientPlayerEnterExitVehicleMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 01840
    // TypeInfo:  0x02BF5760
    struct ClientPlayerRequestCameraChangeMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 01839
    // TypeInfo:  0x02BF5750
    struct ClientPlayerUpdateCameraComponentMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01838
    // TypeInfo:  0x02BF5740
    struct ClientInputUnchangedInputMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01837
    // TypeInfo:  0x02BF5730
    struct ClientInputSettingsRefreshMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01787
    // TypeInfo:  0x02BF43A8
    struct SkyCloudLayer
    {
        Vec3 m_Color; //0x0000
        float m_TileFactor; //0x0010
        float m_Rotation; //0x0014
        float m_Altitude; //0x0018
        float m_Speed; //0x001C
        float m_SunLightPower; //0x0020
        float m_AmbientLightIntensity; //0x0024
        float m_SunLightIntensity; //0x0028
        float m_AlphaMul; //0x002C
        TextureAsset* m_Texture; //0x0030
        char _0x0034[12];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 01725
    // TypeInfo:  0x02BF3EC4
    struct LensFlareElement
    {
        Vec4 m_SizeCamDistCurve; //0x0000
        Vec4 m_SizeAngleCurve; //0x0010
        Vec2 m_Size; //0x0020
        char _0x0028[8];
        Vec4 m_SizeOccluderCurve; //0x0030
        Vec4 m_SizeScreenPosCurve; //0x0040
        Vec4 m_AlphaCamDistCurve; //0x0050
        Vec4 m_AlphaOccluderCurve; //0x0060
        Vec4 m_AlphaAngleCurve; //0x0070
        Vec4 m_AlphaScreenPosCurve; //0x0080
        float m_SizeCamDistMax; //0x0090
        float m_RayDistance; //0x0094
        SurfaceShaderBaseAsset* m_Shader; //0x0098
        float m_AlphaCamDistMax; //0x009C
    };//Size=0x00A0

    ////////////////////////////////////////
    // RuntimeId: 01721
    // TypeInfo:  0x02BF3E88
    struct GroundHeightData
    {
        Vec2 m_HeightSpan; //0x0000
        float m_WorldSize; //0x0008
        Array<unsigned __int16> m_Data; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 01689
    // TypeInfo:  0x02BF3C00
    struct DestructionVolumeProjectionData
    {
        Vec3 m_Normal; //0x0000
        float m_TangentAngle; //0x0010
        char _0x0014[12];
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 01687
    // TypeInfo:  0x02BF3BF0
    struct DestructionVolumeAtlasTextureData
    {
        float m_AtlasAlphaInfluence; //0x0000
        float m_MaskEdgeThickness; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 01587
    // TypeInfo:  0x02BF26BC
    struct BoolShaderParameter
    {
        char* m_ParameterName; //0x0000
        bool m_Value; //0x0004
        char _0x0005[3];
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 01593
    // TypeInfo:  0x02BF26EC
    struct VectorArrayShaderParameter
    {
        char* m_ParameterName; //0x0000
        ShaderParameterType m_ParameterType; //0x0004
        Array<Vec4> m_Values; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 01591
    // TypeInfo:  0x02BF26DC
    struct TextureShaderParameter
    {
        char* m_ParameterName; //0x0000
        TextureBaseAsset* m_Value; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 01589
    // TypeInfo:  0x02BF26CC
    struct VectorShaderParameter
    {
        Vec4 m_Value; //0x0000
        ShaderParameterType m_ParameterType; //0x0010
        char* m_ParameterName; //0x0014
        char _0x0018[8];
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 01599
    // TypeInfo:  0x02BF2754
    struct SurfaceShaderInstanceDataStruct
    {
        SurfaceShaderBaseAsset* m_Shader; //0x0000
        Array<BoolShaderParameter> m_BoolParameters; //0x0004
        Array<VectorShaderParameter> m_VectorParameters; //0x0008
        Array<VectorArrayShaderParameter> m_VectorArrayParameters; //0x000C
        Array<TextureShaderParameter> m_TextureParameters; //0x0010
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 01577
    // TypeInfo:  0x02BF2634
    struct PerformanceClientMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01576
    // TypeInfo:  0x02BF2624
    struct PerformanceShaderMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 01573
    // TypeInfo:  0x02BF25E8
    struct PoissonRadialBlurData
    {
        Vec2 m_PoissonDiscScale; //0x0000
        float m_BlendFactor; //0x0008
        float m_RadialScale; //0x000C
        float m_RadialExponent; //0x0010
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 01571
    // TypeInfo:  0x02BF25D8
    struct ColorTintData
    {
        Vec3 m_Contrast; //0x0000
        Vec3 m_Brightness; //0x0010
        Vec3 m_Saturation; //0x0020
        float m_Hue; //0x0030
        char _0x0034[12];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 01515
    // TypeInfo:  0x02BF2274
    struct AnimatedPointCloudAttributeDesc
    {
        PointCloudAttributeUsage m_Usage; //0x0000
        PointCloudAttributeQuantization m_IFrameQuantization; //0x0004
        PointCloudAttributeQuantization m_DFrameQuantization; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 01513
    // TypeInfo:  0x02BF2264
    struct MeshVariationInfo
    {
        MeshAsset* m_MeshAsset; //0x0000
        MeshAsset* m_OrigMeshAsset; //0x0004
        ObjectVariation* m_VariationAsset; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 01503
    // TypeInfo:  0x02BF21A4
    struct MeshVariationDatabaseMaterial
    {
        MeshMaterial* m_Material; //0x0000
        MeshMaterialVariation* m_MaterialVariation; //0x0004
        Array<TextureShaderParameter> m_TextureParameters; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 00023
    // TypeInfo:  0x02887670
    struct AxisAlignedBox
    {
        Vec3 m_min; //0x0000
        Vec3 m_max; //0x0010
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 01473
    // TypeInfo:  0x02BF1F9C
    struct EnlightenDbInstance
    {
        AxisAlignedBox m_WorldBoundingbox; //0x0000
        LinearTransform m_WorldTransform; //0x0020
        Vec2 m_UvTranslation; //0x0060
        char _0x0068[8];
        Vec4 m_UvTransform; //0x0070
        unsigned __int32 m_GeometryId; //0x0080
        char* m_ObjectVariation; //0x0084
        char* m_GuidString; //0x0088
        unsigned __int32 m_UvSizeX; //0x008C
        unsigned __int32 m_UvSizeY; //0x0090
        float m_PixelSize; //0x0094
        char* m_EnlightenMeshName; //0x0098
        char* m_MeshAsset; //0x009C
        Array<unsigned __int64> m_InstanceAlbedos; //0x00A0
        Guid m_CacheKey; //0x00A4
        char _0x00A4[28];
    };//Size=0x00C0

    ////////////////////////////////////////
    // RuntimeId: 01465
    // TypeInfo:  0x02BF1F40
    struct PrecomputeCache
    {
        Guid m_Guid; //0x0000
        char _0x0000[16];
        char* m_Key; //0x0010
        bool m_CachedDataEnable; //0x0014
        char _0x0015[3];
    };//Size=0x0018

    ////////////////////////////////////////
    // RuntimeId: 01469
    // TypeInfo:  0x02BF1F60
    struct EnlightenLightProbeSet
    {
        LinearTransform m_Transform; //0x0000
        AxisAlignedBox m_BoundingBox; //0x0040
        unsigned __int32 m_SizeX; //0x0060
        unsigned __int32 m_SizeZ; //0x0064
        unsigned __int32 m_SizeY; //0x0068
        __int32 m_SystemId; //0x006C
        float m_BlendDistance; //0x0070
        __int32 m_Priority; //0x0074
        Array<Vec3> m_Positions; //0x0078
        Array<unsigned __int32> m_ValidIndices; //0x007C
        Array<__int32> m_InputSystems; //0x0080
        PrecomputeCache m_Cache; //0x0084
        bool m_StaticProbeSet; //0x009C
        char _0x009D[3];
    };//Size=0x00A0

    ////////////////////////////////////////
    // RuntimeId: 01467
    // TypeInfo:  0x02BF1F50
    struct EnlightenInputInstance
    {
        LinearTransform m_Transform; //0x0000
        char* m_Asset; //0x0040
        char* m_ObjectVariation; //0x0044
        __int32 m_SystemId; //0x0048
        char _0x004C[4];
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 01475
    // TypeInfo:  0x02BF1FAC
    struct EnlightenDbSystem
    {
        AxisAlignedBox m_BoundingBox; //0x0000
        unsigned __int32 m_AtlasStartX; //0x0020
        unsigned __int32 m_AtlasStartY; //0x0024
        unsigned __int32 m_OutputWidth; //0x0028
        unsigned __int32 m_OutputHeight; //0x002C
        unsigned __int32 m_PixelCount; //0x0030
        float m_PixelSize; //0x0034
        __int32 m_SystemId; //0x0038
        Array<EnlightenDbInstance> m_Instances; //0x003C
        Array<__int32> m_InputSystems; //0x0040
        PrecomputeCache m_SystemCache; //0x0044
        PrecomputeCache m_ClusteringCache; //0x005C
        PrecomputeCache m_PreClusteringCache; //0x0074
        PrecomputeCache m_LightTransportCache; //0x008C
        PrecomputeCache m_VisibilityCache; //0x00A4
        bool m_TerrainSystem; //0x00BC
        char _0x00BD[3];
    };//Size=0x00C0

    ////////////////////////////////////////
    // RuntimeId: 01451
    // TypeInfo:  0x02BF1E28
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
    // RuntimeId: 01337
    // TypeInfo:  0x02BF021C
    struct MixerPresetNodeData
    {
        AudioGraphNodeData* m_Node; //0x0000
        float m_Value; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 01333
    // TypeInfo:  0x02BF01FC
    struct MixGroupPropertyValue
    {
        unsigned __int32 m_Property; //0x0000
        float m_Value; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 01335
    // TypeInfo:  0x02BF020C
    struct MixerPresetGroupData
    {
        MixGroup* m_Group; //0x0000
        MixGroupState m_State; //0x0004
        float m_AttackTime; //0x0008
        float m_ReleaseTime; //0x000C
        Array<MixGroupPropertyValue> m_Properties; //0x0010
        bool m_IsDominant; //0x0014
        char _0x0015[3];
    };//Size=0x0018

    ////////////////////////////////////////
    // RuntimeId: 01225
    // TypeInfo:  0x02BEF9CC
    struct AudioGraphNodePort
    {
        float m_UnconnectedValue; //0x0000
        unsigned __int16 m_ValueIndex; //0x0004
        bool m_IsConnected; //0x0006
        char _0x0007[1];
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 01196
    // TypeInfo:  0x02BEE7A8
    struct VoiceOverManuscriptLanguageColumns
    {
        AudioLanguage* m_Language; //0x0000
        char* m_TextColumn; //0x0004
        char* m_SubtitleOffsetColumn; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 01166
    // TypeInfo:  0x02BEE5A0
    struct VoiceOverDialogTakeMapping
    {
        float m_TakeControlMin; //0x0000
        float m_TakeControlMax; //0x0004
        __int8 m_TakeIndex; //0x0008
        char _0x0009[3];
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 01160
    // TypeInfo:  0x02BEE538
    struct VoiceOverDialogTake
    {
        SoundWaveAsset* m_Wave; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 00898
    // TypeInfo:  0x02BED26C
    struct SoundGraphPluginRef
    {
        bool m_IsValid; //0x0000
        __int8 m_VoiceIndex; //0x0001
        __int8 m_PluginIndex; //0x0002
    };//Size=0x0003

    ////////////////////////////////////////
    // RuntimeId: 00986
    // TypeInfo:  0x02BED7CC
    struct DivisibleLoopPlayerPlugins
    {
        SoundGraphPluginRef m_SndPlayer; //0x0000
        SoundGraphPluginRef m_Pause; //0x0003
        SoundGraphPluginRef m_Gain; //0x0006
        SoundGraphPluginRef m_GainFader; //0x0009
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 00950
    // TypeInfo:  0x02BED508
    struct SoundScopeStrategyMapping
    {
        SoundScopeData* m_Scope; //0x0000
        SoundScopeStrategyData* m_Strategy; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 00942
    // TypeInfo:  0x02BED474
    struct SoundWaveSubtitle
    {
        float m_Time; //0x0000
        __int8 m_StringIndex; //0x0004
        char _0x0005[3];
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 00940
    // TypeInfo:  0x02BED464
    struct SoundWaveVariationSegment
    {
        unsigned __int32 m_SamplesOffset; //0x0000
        unsigned __int32 m_SeekTableOffset; //0x0004
        float m_SegmentLength; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 00938
    // TypeInfo:  0x02BED454
    struct SoundDataChunk
    {
        Guid m_ChunkId; //0x0000
        char _0x0000[16];
        unsigned __int32 m_ChunkSize; //0x0010
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 00912
    // TypeInfo:  0x02BED314
    struct AudioCurvePoint
    {
        float m_X; //0x0000
        float m_Y; //0x0004
        float m_K; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 00916
    // TypeInfo:  0x02BED334
    struct AudioCurve
    {
        Array<AudioCurvePoint> m_Points; //0x0000
        AudioCurveType m_CurveType; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 00900
    // TypeInfo:  0x02BED27C
    struct SoundGraphPluginInfo
    {
        unsigned __int32 m_Id; //0x0000
        unsigned __int32 m_EnableAttributeReadMask; //0x0004
        __int8 m_ConnectionIndex; //0x0008
        __int8 m_OutputChannelCount; //0x0009
        __int8 m_ConstructParamsIndex; //0x000A
        __int8 m_ConstructParamCount; //0x000B
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 00904
    // TypeInfo:  0x02BED29C
    struct SoundGraphVoiceInfo
    {
        Array<SoundGraphPluginInfo> m_Plugins; //0x0000
        __int8 m_ProcessingStage; //0x0004
        char _0x0005[3];
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 00902
    // TypeInfo:  0x02BED28C
    struct SoundGraphLinkedPluginAttribute
    {
        SoundGraphPluginRef m_Plugin; //0x0000
        char _0x0003[1];
        AudioGraphNodeData* m_Node; //0x0004
        AudioGraphNodePort m_Port; //0x0008
        __int8 m_AttributeIndex; //0x0010
        bool m_AllowExtremeValues; //0x0011
        char _0x0012[2];
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 00890
    // TypeInfo:  0x02BED22C
    struct SoundGraphPluginConnectionParam
    {
        float m_Value; //0x0000
        __int8 m_Index; //0x0004
        char _0x0005[3];
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 01056
    // TypeInfo:  0x02BEDD28
    struct SamplerPlugins
    {
        SoundGraphPluginRef m_SndPlayer; //0x0000
        SoundGraphPluginRef m_Resample; //0x0003
        SoundGraphPluginRef m_Pause; //0x0006
        SoundGraphPluginRef m_Gain; //0x0009
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 00896
    // TypeInfo:  0x02BED25C
    struct SoundGraphPluginConstructParam
    {
        float m_Value; //0x0000
        __int8 m_Index; //0x0004
        char _0x0005[3];
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 00894
    // TypeInfo:  0x02BED24C
    struct SoundGraphPluginConnection
    {
        SoundGraphPluginConnectionType m_ConnectionType; //0x0000
        Array<SoundGraphPluginConnectionParam> m_Parameters; //0x0004
        SoundBusData* m_Bus; //0x0008
        __int8 m_VoiceIndex; //0x000C
        char _0x000D[3];
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 00906
    // TypeInfo:  0x02BED2AC
    struct SoundGraphInfo
    {
        Array<SoundGraphVoiceInfo> m_Voices; //0x0000
        Array<SoundGraphLinkedPluginAttribute> m_LinkedPluginAttributes; //0x0004
        Array<SoundGraphPluginConnection> m_Connections; //0x0008
        Array<SoundGraphPluginConstructParam> m_ConstructParams; //0x000C
        unsigned __int32 m_PluginsParamCount; //0x0010
        unsigned __int32 m_PluginCount; //0x0014
    };//Size=0x0018

    ////////////////////////////////////////
    // RuntimeId: 00884
    // TypeInfo:  0x02BED1E0
    struct PointEnvelopePoint
    {
        float m_X; //0x0000
        float m_Y; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 00874
    // TypeInfo:  0x02BED13C
    struct AudioObstructionMaterialInfo
    {
        MaterialContainerPair* m_Material; //0x0000
        float m_ObstructionFrequency; //0x0004
        float m_GainReduction; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 00836
    // TypeInfo:  0x02BECE4C
    struct AudioLanguageMapping
    {
        AudioLanguage* m_Source; //0x0000
        AudioLanguage* m_Target; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 00828
    // TypeInfo:  0x02BECDD4
    struct SoundPatchPublicNode
    {
        AudioGraphNodeData* m_Node; //0x0000
        unsigned __int32 m_Id; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 00808
    // TypeInfo:  0x02BECC70
    struct SoundWaveLocalizationInfo
    {
        AudioLanguage* m_Language; //0x0000
        unsigned __int16 m_FirstVariationIndex; //0x0004
        unsigned __int16 m_VariationCount; //0x0006
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 00775
    // TypeInfo:  0x02BEA11C
    struct EndPointData
    {
        float m_Pos; //0x0000
        float m_EndDamping; //0x0004
        float m_SpringLength; //0x0008
        float m_SpringAcceleration; //0x000C
        float m_SpringDamping; //0x0010
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 00761
    // TypeInfo:  0x02BEA004
    struct FrictionScaleAtVelocity
    {
        float m_FrictionScale; //0x0000
        float m_Velocity; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 00759
    // TypeInfo:  0x02BE9FF4
    struct SensitivityAtVelocity
    {
        float m_SteeringSensitivity; //0x0000
        float m_Velocity; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 00757
    // TypeInfo:  0x02BE9FE4
    struct SpringData
    {
        float m_Length; //0x0000
        float m_Stiffness; //0x0004
        float m_Damping; //0x0008
        float m_MaxSpringForce; //0x000C
        float m_ProgressiveStartRatio; //0x0010
        float m_ProgressiveExponent; //0x0014
        float m_VisualClipOffset; //0x0018
        float m_AttachOffsetY; //0x001C
        float m_DisabledStrenghModifier; //0x0020
    };//Size=0x0024

    ////////////////////////////////////////
    // RuntimeId: 00755
    // TypeInfo:  0x02BE9FD4
    struct SphereCollisionData
    {
        float m_ExtraRadius; //0x0000
        float m_CounterNormalBrakeForceMod; //0x0004
        bool m_Enabled; //0x0008
        char _0x0009[3];
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 00739
    // TypeInfo:  0x02BE9EC8
    struct Boost
    {
        float m_ForwardStrength; //0x0000
        float m_ReverseStrength; //0x0004
        float m_DissipationTime; //0x0008
        float m_RecoveryTime; //0x000C
        float m_CrawlStrength; //0x0010
        float m_AccelerationScale; //0x0014
    };//Size=0x0018

    ////////////////////////////////////////
    // RuntimeId: 00705
    // TypeInfo:  0x02BE9C30
    struct ConstantForceData
    {
        Vec3 m_Value; //0x0000
        ForceCondition m_Condition; //0x0010
        ForceType m_TypeOfForce; //0x0014
        SpaceType m_Space; //0x0018
        char _0x001C[4];
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 00697
    // TypeInfo:  0x02BE9BF0
    struct StabilizerSettings
    {
        StabilizerProperty m_Property; //0x0000
        float m_Strength; //0x0004
        bool m_Advanced; //0x0008
        char _0x0009[3];
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 00695
    // TypeInfo:  0x02BE9BE0
    struct AntiRollBars
    {
        AntiRollBar* m_Front; //0x0000
        AntiRollBar* m_Rear; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 00691
    // TypeInfo:  0x02BE9BA4
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
    // RuntimeId: 00675
    // TypeInfo:  0x02BE9A7C
    struct PartInfoData
    {
        AxisAlignedBox m_Aabb; //0x0000
        Vec3 m_Translation; //0x0020
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 00637
    // TypeInfo:  0x02BE9818
    struct AssetAabbs
    {
        Array<AxisAlignedBox> m_PartAabb; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 00605
    // TypeInfo:  0x02BE9590
    struct SpeedModifierData
    {
        float m_ForwardConstant; //0x0000
        float m_BackwardConstant; //0x0004
        float m_LeftConstant; //0x0008
        float m_RightConstant; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 00601
    // TypeInfo:  0x02BE9554
    struct PoseTransitionTime
    {
        CharacterPoseType m_ToPose; //0x0000
        float m_TransitionTime; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 00595
    // TypeInfo:  0x02BE9524
    struct CharacterPoseConstraintsData
    {
        bool m_StandPose; //0x0000
        bool m_CrouchPose; //0x0001
        bool m_PronePose; //0x0002
    };//Size=0x0003

    ////////////////////////////////////////
    // RuntimeId: 00593
    // TypeInfo:  0x02BE9514
    struct LookConstraintsData
    {
        float m_MinLookYaw; //0x0000
        float m_MaxLookYaw; //0x0004
        float m_MinLookPitch; //0x0008
        float m_MaxLookPitch; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 00589
    // TypeInfo:  0x02BE94D8
    struct EdgeModelLightMapInstance
    {
        Vec4 m_LightMapUv; //0x0000
        Vec3 m_Position; //0x0010
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 00583
    // TypeInfo:  0x02BE9470
    struct EdgeModelInstance
    {
        LinearTransform m_Transform; //0x0000
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 00567
    // TypeInfo:  0x02BE8060
    struct NetworkDataId
    {
        unsigned __int32 m_Value; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 00502
    // TypeInfo:  0x02BE717C
    struct ChunkStreamerCell
    {
        Array<Vec2> m_Shape; //0x0000
        char* m_BundleName; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 00494
    // TypeInfo:  0x02BE7104
    struct PackagingDetailInfo
    {
        __int32 m_MipsToSkip; //0x0000
        __int32 m_LodsToSkip; //0x0004
        bool m_IncludeAnimation; //0x0008
        bool m_IncludeMeshes; //0x0009
        bool m_IncludeSounds; //0x000A
        bool m_IncludeTextures; //0x000B
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 00452
    // TypeInfo:  0x02BE6D84
    struct SubSkeleton
    {
        SkeletonAsset* m_Skeleton; //0x0000
        Array<__int32> m_BoneMap; //0x0004
        Array<LinearTransform> m_TransformMap; //0x0008
    };//Size=0x000C

    ////////////////////////////////////////
    // RuntimeId: 00430
    // TypeInfo:  0x02BE63F0
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
    // RuntimeId: 00388
    // TypeInfo:  0x02BE60E0
    struct SequenceEventData
    {
        EventSpec m_Event; //0x0000
        __int32 m_Time; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 00296
    // TypeInfo:  0x02BE59BC
    struct MaterialRelationPropertyPair
    {
        Array<PhysicsMaterialRelationPropertyData*> m_PhysicsMaterialProperties; //0x0000
        Array<PhysicsPropertyRelationPropertyData*> m_PhysicsPropertyProperties; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 00298
    // TypeInfo:  0x02BE59CC
    struct MaterialInteractionGridRow
    {
        Array<MaterialRelationPropertyPair> m_Items; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 02712
    // TypeInfo:  0x02BFE3E0
    struct EventSyncReachedClientMessage
    {
        char _0x0000[56];
        EntityUid m_EntityUid; //0x0038
        char _0x003C[4];
    };//Size=0x0040

    ////////////////////////////////////////
    // RuntimeId: 00250
    // TypeInfo:  0x02BE561C
    struct GuidNamePair
    {
        Guid m_Guid; //0x0000
        char _0x0000[16];
        char* m_Name; //0x0010
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 00186
    // TypeInfo:  0x02BE51EC
    struct DynamicLink
    {
        __int32 m_Id; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 00184
    // TypeInfo:  0x02BE51DC
    struct DynamicEvent
    {
        __int32 m_Id; //0x0000
    };//Size=0x0004

    ////////////////////////////////////////
    // RuntimeId: 00182
    // TypeInfo:  0x02BE51CC
    struct EventConnection
    {
        DataContainer* m_Source; //0x0000
        DataContainer* m_Target; //0x0004
        EventSpec m_SourceEvent; //0x0008
        EventSpec m_TargetEvent; //0x000C
        EventConnectionTargetType m_TargetType; //0x0010
    };//Size=0x0014

    ////////////////////////////////////////
    // RuntimeId: 03435
    // TypeInfo:  0x02C04E8C
    struct AntEventData
    {
        __int32 m_TagId; //0x0000
        EventSpec m_Event; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 02981
    // TypeInfo:  0x02C00E98
    struct AntAnimationHandlerData
    {
        AntAnimatableData m_Animatable; //0x0000
        AntRef m_RootController; //0x0014
        LodBinding m_LodBinding; //0x0018
        Array<AntPackageAsset*> m_AntPackageData; //0x0024
        Array<GameplayBone> m_BonesToMirror; //0x0028
        bool m_ReportBackFromAnt; //0x002C
        bool m_EnableMasterSlaveCopy; //0x002D
        bool m_IsProp; //0x002E
        char _0x002F[1];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 05953
    // TypeInfo:  0x02C20A34
    struct AimingConstraintEntityBinding
    {
        AntRef m_ProneAimLimit; //0x0000
        AntRef m_ProneAimDistance; //0x0004
    };//Size=0x0008

    ////////////////////////////////////////
    // RuntimeId: 00116
    // TypeInfo:  0x02BE2F5C
    struct DataField
    {
        char* m_Value; //0x0000
        DataContainer* m_ValueRef; //0x0004
        __int32 m_Id; //0x0008
        FieldAccessType m_AccessType; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 00112
    // TypeInfo:  0x02BE2F3C
    struct LinkConnection
    {
        DataContainer* m_Source; //0x0000
        DataContainer* m_Target; //0x0004
        __int32 m_SourceFieldId; //0x0008
        __int32 m_TargetFieldId; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 00110
    // TypeInfo:  0x02BE2F2C
    struct PropertyConnection
    {
        DataContainer* m_Source; //0x0000
        DataContainer* m_Target; //0x0004
        __int32 m_SourceFieldId; //0x0008
        __int32 m_TargetFieldId; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 00096
    // TypeInfo:  0x02BE2E30
    struct CoreTweakBlueprintBundleUnloadedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 00095
    // TypeInfo:  0x02BE2E20
    struct CoreTweakBlueprintBundleLoadedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 00094
    // TypeInfo:  0x02BE2E10
    struct CoreTweakSubLevelBundleUnloadedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 00093
    // TypeInfo:  0x02BE2E00
    struct CoreTweakSubLevelBundleLoadedMessage
    {
        char _0x0000[56];
    };//Size=0x0038

    ////////////////////////////////////////
    // RuntimeId: 00092
    // TypeInfo:  0x02BE2DF0
    struct CorePropertyTweakedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 00091
    // TypeInfo:  0x02BE2DE0
    struct FrameUpdatedMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 00090
    // TypeInfo:  0x02BE2DD0
    struct CoreBudgetManagerMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 00089
    // TypeInfo:  0x02BE2DC0
    struct CoreEndMemoryFileStreamMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 00088
    // TypeInfo:  0x02BE2DB0
    struct CoreEndMemoryTrackingMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 00087
    // TypeInfo:  0x02BE2DA0
    struct CoreCleanupMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 00086
    // TypeInfo:  0x02BE2D90
    struct CoreResourceLoadingFailureMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 00085
    // TypeInfo:  0x02BE2D80
    struct CorePanicMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 00084
    // TypeInfo:  0x02BE2D70
    struct CoreQuitMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 00083
    // TypeInfo:  0x02BE2D60
    struct CoreUpdateClipboardMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 00082
    // TypeInfo:  0x02BE2D50
    struct CoreMainThreadInitMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 00081
    // TypeInfo:  0x02BE2D40
    struct CoreHibernateMessage
    {
        char _0x0000[40];
    };//Size=0x0028

    ////////////////////////////////////////
    // RuntimeId: 00080
    // TypeInfo:  0x02BE2D30
    struct CoreQuittingInitiatedMessage
    {
        char _0x0000[48];
    };//Size=0x0030

    ////////////////////////////////////////
    // RuntimeId: 00069
    // TypeInfo:  0x02BE2804
    struct CoreSettings
    {
        __int32 m_RandomTickSeed; //0x0000
        char* m_AvailableLanguages; //0x0004
        __int32 m_RandomSessionId; //0x0008
        __int32 m_RandomTimeSeed; //0x000C
        char* m_Ps3FootSwitchCommands; //0x0010
        float m_HardwareGpuBias; //0x0014
        HardwareProfile m_HardwareProfile; //0x0018
        char* m_B4BugServerIP; //0x001C
        __int32 m_JobProcessorCount; //0x0020
        __int32 m_MaxJobThreadCount; //0x0024
        unsigned __int32 m_AssertLimit; //0x0028
        char* m_Host; //0x002C
        char* m_HostUser; //0x0030
        char* m_HostUserDomain; //0x0034
        char* m_InitSeed; //0x0038
        CoreLogLevel m_LogLevel; //0x003C
        CoreLogLevel m_JuiceLogLevel; //0x0040
        float m_HardwareCpuBias; //0x0044
        JuiceUserType m_JuiceUserType; //0x0048
        BugSubmitTool m_BugSubmitTool; //0x004C
        char* m_GameConfigurationName; //0x0050
        char* m_ProfileDirectoryName; //0x0054
        __int32 m_HttpDvdSeekPenalty; //0x0058
        char* m_JuiceServerIP; //0x005C
        bool m_DisplayAsserts; //0x0060
        bool m_EnableJuice; //0x0061
        bool m_EnableDebugRendererLog; //0x0062
        bool m_LiveEditingEnable; //0x0063
        bool m_JuiceValidateAsAssert; //0x0064
        bool m_EnableTrophies; //0x0065
        bool m_ValidateAsWarning; //0x0066
        bool m_IgnoreAlways; //0x0067
        bool m_EnableBitstreamTypeDumping; //0x0068
        bool m_UseStorageServer; //0x0069
        bool m_UseDiskCaching; //0x006A
        bool m_ForceDataContainerRegistration; //0x006B
        bool m_EnableBitstreamTypeVerification; //0x006C
        bool m_CrashOnFatalErrors; //0x006D
        bool m_EnableBitstreamExtendedVerification; //0x006E
        bool m_MemoryStatsEnabled; //0x006F
        bool m_DetailedMemoryStatsEnabled; //0x0070
        bool m_EnableResolveErrorTracking; //0x0071
        bool m_HttpDvdSimulation; //0x0072
        bool m_EnableDrone; //0x0073
        bool m_EnableDbErrorPolicy; //0x0074
        char _0x0075[3];
    };//Size=0x0078

    ////////////////////////////////////////
    // RuntimeId: 01479
    // TypeInfo:  0x02BF1FE8
    struct EnlightenPipelineTerrainMesh
    {
        Vec2 m_UvTranslation; //0x0000
        char _0x0008[8];
        AxisAlignedBox m_BoundingBox; //0x0010
        Vec4 m_UvTransform; //0x0030
        unsigned __int32 m_LightMapResolution; //0x0040
        char* m_TerrainMeshName; //0x0044
        char _0x0048[8];
    };//Size=0x0050

    ////////////////////////////////////////
    // RuntimeId: 00021
    // TypeInfo:  0x02887630
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
    // RuntimeId: 03931
    // TypeInfo:  0x02C0A07C
    struct WeaponStateData
    {
        Array<unsigned __int32> m_ReferencedAssetHashes; //0x0000
        Array<BoneFakePhysicsData*> m_BoneFakePhysics; //0x0004
        SkinnedMeshAsset* m_Mesh1p; //0x0008
        SkinnedMeshAsset* m_MeshZoom1p; //0x000C
        SkinnedMeshAsset* m_Mesh3p; //0x0010
        AntRef m_Weapon; //0x0014
        float m_ZoomMeshTransitionFactor; //0x0018
        float m_ZoomScaleFactor; //0x001C
        Array<float> m_ZoomInOutMeshTransitionFactors; //0x0020
        ScopeFilterData* m_ZoomedScopeFilter; //0x0024
        ScopeFilterData* m_NonZoomedScopeFilter; //0x0028
        float m_KeepAimingTime; //0x002C
        AnimationConfigurationData m_AnimationConfiguration; //0x0030
        AnimatedFireEnum m_AnimatedFireType; //0x003C
        AnimatedAimingEnum m_AnimatedAimingType; //0x0040
        Array<LinearTransform> m_Mesh3pTransforms; //0x0044
        Array<RigidMeshSocketTransform> m_Mesh3pRigidMeshSocketObjectTransforms; //0x0048
        float m_HideProjectileAfterFireTime; //0x004C
        char* m_ProjectileBoneName; //0x0050
        bool m_IsOneHanded; //0x0054
        bool m_PlayDeployAfterFire; //0x0055
        bool m_SkipFireAnimation; //0x0056
        bool m_SkipDeployAnimation; //0x0057
        bool m_SkipReloadAnimation; //0x0058
        bool m_AlwaysAimHead; //0x0059
        char _0x005A[2];
    };//Size=0x005C

    ////////////////////////////////////////
    // RuntimeId: 00017
    // TypeInfo:  0x02887570
    struct Plane
    {
        float m_x; //0x0000
        float m_y; //0x0004
        float m_z; //0x0008
        float m_w; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 00015
    // TypeInfo:  0x02887540
    struct Quat
    {
        float m_x; //0x0000
        float m_y; //0x0004
        float m_z; //0x0008
        float m_w; //0x000C
    };//Size=0x0010

    ////////////////////////////////////////
    // RuntimeId: 05096
    // TypeInfo:  0x02C15518
    struct PolynomialTempData
    {
        Vec4 m_Coefficients; //0x0000
        float m_ScaleValue; //0x0010
        float m_MinClamp; //0x0014
        float m_MaxClamp; //0x0018
        char _0x001C[4];
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 06564
    // TypeInfo:  0x02C27DFC
    struct SensingSphere
    {
        Vec3 m_Position; //0x0000
        float m_RadiusSqr; //0x0010
        char _0x0014[12];
    };//Size=0x0020

    ////////////////////////////////////////
    // RuntimeId: 05062
    // TypeInfo:  0x02C152D4
    struct EmitterTextureAtlasInfo
    {
        Vec2 m_MinUv; //0x0000
        Vec2 m_MaxUv; //0x0008
        char* m_TextureName; //0x0010
    };//Size=0x0014


}