#pragma once

namespace fb
{
    ////////////////////////////////////////
// RuntimeId: 06556
// TypeInfo:  0x02C27D84
    enum ReadinessLevel
    {
        RLPatrol, //0x0000
        RLReady, //0x0001
        RLCombat, //0x0002
        RLNone //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 06550
    // TypeInfo:  0x02C27D1C
    enum ReadinessState
    {
        RSPatrol, //0x0000
        RSReady, //0x0001
        RSCombat //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 06544
    // TypeInfo:  0x02C27CB4
    enum StrengthType
    {
        Infantry, //0x0000
        LightArmour, //0x0001
        HeavyArmour, //0x0002
        Helicopter, //0x0003
        NumberOfStrengthTypes //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 06538
    // TypeInfo:  0x02C27C68
    enum ReloadBehavior
    {
        RbInCover, //0x0000
        RbStationary, //0x0001
        RbAnywhere //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 06536
    // TypeInfo:  0x02C27C58
    enum AimOrigin
    {
        AoDefault, //0x0000
        AoCamera, //0x0001
        AoWeapon //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 06534
    // TypeInfo:  0x02C27C48
    enum SweepSelectionType
    {
        SstTargetLost, //0x0000
        SstFirstTime //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 06514
    // TypeInfo:  0x02C27AE4
    enum CoverType
    {
        CoverType_Stand, //0x0000
        CoverType_Medium, //0x0001
        CoverType_Crouch, //0x0002
        CoverType_Prone //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 06510
    // TypeInfo:  0x02C27AA8
    enum CoverSelectionMethod
    {
        CSMAgainstThreatPosition, //0x0000
        CSMAgainstHighestThreat, //0x0001
        CSMAgainstSelectedTarget, //0x0002
        CSMAgainstAllEnemies, //0x0003
        CSMAgainstAllEnemiesWithFallback //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 06435
    // TypeInfo:  0x02C268F0
    enum UIGraphPriority
    {
        UIGraphPriority_Low, //0x0000
        UIGraphPriority_Normal, //0x0001
        UIGraphPriority_High, //0x0002
        UIGraphPriority_OverlayScreens, //0x0003
        UIGraphPriority_Critical //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 06393
    // TypeInfo:  0x02C2658C
    enum WidgetVerticalAlignment
    {
        WVA_Top, //0x0000
        WVA_Center, //0x0001
        WVA_Bottom //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 06391
    // TypeInfo:  0x02C2657C
    enum WidgetHorisontalAlignment
    {
        WHA_Left, //0x0000
        WHA_Center, //0x0001
        WHA_Right //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 06355
    // TypeInfo:  0x02C2630C
    enum UILogicOperator
    {
        UILogicOperator_None, //0x0000
        UILogicOperator_Equal, //0x0001
        UILogicOperator_GreaterThan, //0x0002
        UILogicOperator_GreaterThanOrEqual, //0x0003
        UILogicOperator_LessThan, //0x0004
        UILogicOperator_LessThanOrEqual //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 06353
    // TypeInfo:  0x02C262FC
    enum UIInputEvent
    {
        UIInputEvent_None, //0x0000
        UIInputEvent_OnButtonA, //0x0001
        UIInputEvent_OnButtonB, //0x0002
        UIInputEvent_OnButtonBack, //0x0003
        UIInputEvent_OnButtonDPad_Down, //0x0004
        UIInputEvent_OnButtonDPad_Left, //0x0005
        UIInputEvent_OnButtonDPad_Right, //0x0006
        UIInputEvent_OnButtonDPad_Up, //0x0007
        UIInputEvent_OnButtonLB, //0x0008
        UIInputEvent_OnButtonRB, //0x0009
        UIInputEvent_OnButtonStart, //0x000A
        UIInputEvent_OnButtonX, //0x000B
        UIInputEvent_OnButtonY //0x000C
    };

    ////////////////////////////////////////
    // RuntimeId: 06351
    // TypeInfo:  0x02C262EC
    enum UIWidgetEventID
    {
        UIWidgetEventID_None, //0x0000
        UIWidgetEventID_OnItemSelected, //0x0001
        UIWidgetEventID_OnItemHighlighted, //0x0002
        UIWidgetEventID_OnItemReleased, //0x0003
        UIWidgetEventID_OnItemUnhighlighted, //0x0004
        UIWidgetEventID_OnItemPressed, //0x0005
        UIWidgetEventID_OnItemOver, //0x0006
        UIWidgetEventID_OnItemOut, //0x0007
        UIWidgetEventID_OnToggle, //0x0008
        UIWidgetEventID_OnChanged, //0x0009
        UIWidgetEventID_OnButtonBarChanged, //0x000A
        UIWidgetEventID_EnterTop, //0x000B
        UIWidgetEventID_EnterBottom, //0x000C
        UIWidgetEventID_EnterAny, //0x000D
        UIWidgetEventID_SetIndex, //0x000E
        UIWidgetEventID_SetPercentage, //0x000F
        UIWidgetEventID_OnReachedTop, //0x0010
        UIWidgetEventID_OnReachedBottom, //0x0011
        UIWidgetEventID_OnNavigateLeft, //0x0012
        UIWidgetEventID_OnNavigateRight, //0x0013
        UIWidgetEventID_Show, //0x0014
        UIWidgetEventID_Hide, //0x0015
        UIWidgetEventID_OnShow, //0x0016
        UIWidgetEventID_OnHide, //0x0017
        UIWidgetEventID_Toggle, //0x0018
        UIWidgetEventID_ToggleOn, //0x0019
        UIWidgetEventID_ToggleOff, //0x001A
        UIWidgetEventID_ToggledOn, //0x001B
        UIWidgetEventID_ToggledOff, //0x001C
        UIWidgetEventID_Update, //0x001D
        UIWidgetEventID_OnComplete, //0x001E
        UIWidgetEventID_Focus, //0x001F
        UIWidgetEventID_OnHeaderChanged, //0x0020
        UIWidgetEventID_OnDescriptionChanged, //0x0021
        UIWidgetEventID_OnMapChanged, //0x0022
        UIWidgetEventID_Clear, //0x0023
        UIWidgetEventID_Delete, //0x0024
        UIWidgetEventID_LThumb, //0x0025
        UIWidgetEventID_RThumb, //0x0026
        UIWidgetEventID_LTAB, //0x0027
        UIWidgetEventID_RTAB, //0x0028
        UIWidgetEventID_OnLarge, //0x0029
        UIWidgetEventID_OnSmall, //0x002A
        UIWidgetEventID_TextEntered, //0x002B
        UIWidgetEventID_PlaySoundNavigate, //0x002C
        UIWidgetEventID_PlaySoundSelect, //0x002D
        UIWidgetEventID_PlaySoundPageSwap, //0x002E
        UIWidgetEventID_OnComPressed, //0x002F
        UIWidgetEventID_OnComReleased, //0x0030
        UIWidgetEventID_NumItems //0x0031
    };

    ////////////////////////////////////////
    // RuntimeId: 06345
    // TypeInfo:  0x02C26284
    enum UIInterruptID
    {
        UIInterruptID_None, //0x0000
        UIInterruptID_ControllerPull //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 06319
    // TypeInfo:  0x02C260D4
    enum UIConsoleKeyboardStatus
    {
        UIConsoleKeyboardStatus_Success, //0x0000
        UIConsoleKeyboardStatus_Failed, //0x0001
        UIConsoleKeyboardStatus_Cancelled, //0x0002
        UIConsoleKeyboardStatus_Active, //0x0003
        UIConsoleKeyboardStatus_Inactive, //0x0004
        UIConsoleKeyboardStatus_Count //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 06317
    // TypeInfo:  0x02C260C4
    enum UIInputAction
    {
        UIInputAction_NavigateUp, //0x0000
        UIInputAction_NavigateDown, //0x0001
        UIInputAction_NavigateLeft, //0x0002
        UIInputAction_NavigateRight, //0x0003
        UIInputAction_TabLeft, //0x0004
        UIInputAction_TabRight, //0x0005
        UIInputAction_Activate, //0x0006
        UIInputAction_Deactivate, //0x0007
        UIInputAction_Menu, //0x0008
        UIInputAction_Cancel, //0x0009
        UIInputAction_OK, //0x000A
        UIInputAction_Back, //0x000B
        UIInputAction_Tab, //0x000C
        UIInputAction_Edit, //0x000D
        UIInputAction_View, //0x000E
        UIInputAction_LThumb, //0x000F
        UIInputAction_RThumb, //0x0010
        UIInputAction_MapZoom, //0x0011
        UIInputAction_MapSize, //0x0012
        UIInputAction_SayAllChat, //0x0013
        UIInputAction_TeamChat, //0x0014
        UIInputAction_SquadChat, //0x0015
        UIInputAction_CommoRose, //0x0016
        UIInputAction_ToggleChat, //0x0017
        UIInputAction_ToggleMinimapType, //0x0018
        UIInputAction_Size, //0x0019
        UIInputAction_None //0x001A
    };

    ////////////////////////////////////////
    // RuntimeId: 06315
    // TypeInfo:  0x02C260B4
    enum UIInputActionEventType
    {
        UIInputActionEventType_Pressed, //0x0000
        UIInputActionEventType_Released //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 06313
    // TypeInfo:  0x02C260A4
    enum UIKeyboardEventType
    {
        UIKeyboardEventType_KeyDown, //0x0000
        UIKeyboardEventType_KeyUp, //0x0001
        UIKeyboardEventType_Char //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 06311
    // TypeInfo:  0x02C26094
    enum UIMouseEventType
    {
        UIMouseEventType_MouseMove, //0x0000
        UIMouseEventType_MouseWheel, //0x0001
        UIMouseEventType_ButtonDown, //0x0002
        UIMouseEventType_ButtonUp //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 06309
    // TypeInfo:  0x02C26084
    enum UIMouseButton
    {
        UIMouseButton_Left, //0x0000
        UIMouseButton_Right, //0x0001
        UIMouseButton_Middle //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 06307
    // TypeInfo:  0x02C26074
    enum UISystemType
    {
        UISystem_None, //0x0000
        UISystem_Standard //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 06303
    // TypeInfo:  0x02C26038
    enum UIState
    {
        UIState_Menu, //0x0000
        UIState_LoadingSP, //0x0001
        UIState_LoadingMP, //0x0002
        UIState_Playing, //0x0003
        UIState_Unload, //0x0004
        UIState_Embedded, //0x0005
        UIState_Static, //0x0006
        UIState_StaticGenerated, //0x0007
        UIState_PreEndOfRound, //0x0008
        UIState_EndOfRound, //0x0009
        UIState_PlayingSP, //0x000A
        UIState_PlayingMP, //0x000B
        UIState_None //0x000C
    };

    ////////////////////////////////////////
    // RuntimeId: 06299
    // TypeInfo:  0x02C25FFC
    enum UIUpdateType
    {
        UIUpdateType_Always, //0x0000
        UIUpdateType_Odd, //0x0001
        UIUpdateType_Even, //0x0002
        UIUpdateType_Never //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 06295
    // TypeInfo:  0x02C252A4
    enum SecureReason
    {
        SecureReason_Ok, //0x0000
        SecureReason_WrongProtocolVersion, //0x0001
        SecureReason_WrongTitleVersion, //0x0002
        SecureReason_ServerFull, //0x0003
        SecureReason_KickedOut, //0x0004
        SecureReason_Banned, //0x0005
        SecureReason_GenericError, //0x0006
        SecureReason_WrongPassword, //0x0007
        SecureReason_KickedOutDemoOver, //0x0008
        SecureReason_RankRestricted, //0x0009
        SecureReason_ConfigurationNotAllowed, //0x000A
        SecureReason_ServerReclaimed, //0x000B
        SecureReason_MissingContent, //0x000C
        SecureReason_NotVerified, //0x000D
        SecureReason_TimedOut, //0x000E
        SecureReason_ConnectFailed, //0x000F
        SecureReason_NoReply, //0x0010
        SecureReason_AcceptFailed, //0x0011
        SecureReason_MismatchingContent, //0x0012
        SecureReason_InteractivityTimeout, //0x0013
        SecureReason_KickedFromQueue, //0x0014
        SecureReason_TeamKills, //0x0015
        SecureReason_KickedByAdmin, //0x0016
        SecureReason_KickedViaPunkBuster, //0x0017
        SecureReason_KickedOutServerFull, //0x0018
        SecureReason_ESportsMatchStarting, //0x0019
        SecureReason_NotInESportsRosters, //0x001A
        SecureReason_ESportsMatchEnding, //0x001B
        SecureReason_VirtualServerExpired, //0x001C
        SecureReason_VirtualServerRecreate, //0x001D
        SecureReason_ESportsTeamFull, //0x001E
        SecureReason_ESportsMatchAborted, //0x001F
        SecureReason_ESportsMatchWalkover, //0x0020
        SecureReason_ESportsMatchWarmupTimedOut, //0x0021
        SecureReason_NotAllowedToSpectate, //0x0022
        SecureReason_NoSpectateSlotAvailable, //0x0023
        SecureReason_InvalidSpectateJoin, //0x0024
        SecureReason_PremiumStatusMismatch //0x0025
    };

    ////////////////////////////////////////
    // RuntimeId: 06105
    // TypeInfo:  0x02C213C8
    enum ServerUnlockSystemType
    {
        ServerUnlockSystemType_All, //0x0000
        ServerUnlockSystemType_Common, //0x0001
        ServerUnlockSystemType_None, //0x0002
        ServerUnlockSystemType_FromStats, //0x0003
        ServerUnlockSystemType_FromWhiteList, //0x0004
        ServerUnlockSystemType_FromBlackList, //0x0005
        ServerUnlockSystemType_Count //0x0006
    };

    ////////////////////////////////////////
    // RuntimeId: 06096
    // TypeInfo:  0x02C21348
    enum ServerMapSequencerEventType
    {
        ServerMapSequencerEventType_Add, //0x0000
        ServerMapSequencerEventType_Remove, //0x0001
        ServerMapSequencerEventType_Clear, //0x0002
        ServerMapSequencerEventType_List, //0x0003
        ServerMapSequencerEventType_Load, //0x0004
        ServerMapSequencerEventType_Save, //0x0005
        ServerMapSequencerEventType_SetNextMapIndex, //0x0006
        ServerMapSequencerEventType_GetMapIndices, //0x0007
        ServerMapSequencerEventType_GetRounds, //0x0008
        ServerMapSequencerEventType_RunNextRound, //0x0009
        ServerMapSequencerEventType_RestartRound, //0x000A
        ServerMapSequencerEventType_AvailableMaps //0x000B
    };

    ////////////////////////////////////////
    // RuntimeId: 05891
    // TypeInfo:  0x02C2056C
    enum SoldierEmote
    {
        SoldierEmote_Spot, //0x0000
        SoldierEmote_Ok, //0x0001
        SoldierEmote_NeedARide, //0x0002
        SoldierEmote_GoGoGo, //0x0003
        SoldierEmote_NeedBackup, //0x0004
        SoldierEmote_Thanks, //0x0005
        SoldierEmote_NeedMedic, //0x0006
        SoldierEmote_FollowMe, //0x0007
        SoldierEmote_NeedAmmo, //0x0008
        SoldierEmote_Sorry, //0x0009
        SoldierEmote_No, //0x000A
        SoldierEmote_DefendCapturePoint, //0x000B
        SoldierEmote_AttackCapturePoint, //0x000C
        SoldierEmote_MoveToPosition, //0x000D
        SoldierEmote_Count //0x000E
    };

    ////////////////////////////////////////
    // RuntimeId: 05869
    // TypeInfo:  0x02C203F8
    enum SpecialMovesPoseType
    {
        SpecialMovesPoseType_None, //0x0000
        SpecialMovesPoseType_Stand, //0x0001
        SpecialMovesPoseType_Crouch, //0x0002
        SpecialMovesPoseType_Prone //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 05837
    // TypeInfo:  0x02C20284
    enum ServerUnlockListEventType
    {
        ServerUnlockListEventType_Add, //0x0000
        ServerUnlockListEventType_Remove, //0x0001
        ServerUnlockListEventType_Clear, //0x0002
        ServerUnlockListEventType_List, //0x0003
        ServerUnlockListEventType_Load, //0x0004
        ServerUnlockListEventType_Save, //0x0005
        ServerUnlockListEventType_GetMode, //0x0006
        ServerUnlockListEventType_SetMode, //0x0007
        ServerUnlockListEventType_SetFromBitmap //0x0008
    };

    ////////////////////////////////////////
    // RuntimeId: 05832
    // TypeInfo:  0x02C20244
    enum AddGameBanResponseType
    {
        AddGameBanResponseType_Success, //0x0000
        AddGameBanResponseType_Failure, //0x0001
        AddGameBanResponseType_UserNotFound, //0x0002
        AddGameBanResponseType_UserCannotBeBanned //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 05807
    // TypeInfo:  0x02C20094
    enum FaderType
    {
        FaderType_Linear, //0x0000
        FaderType_Cosine //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 05663
    // TypeInfo:  0x02C1DEA8
    enum AIStunReactionAnimationType
    {
        AIStunReactionAnimationType_1, //0x0000
        AIStunReactionAnimationType_2, //0x0001
        AIStunReactionAnimationType_3, //0x0002
        AIStunReactionAnimationType_4, //0x0003
        AIStunReactionAnimationType_5, //0x0004
        AIStunReactionAnimationType_Count //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 05661
    // TypeInfo:  0x02C1DE98
    enum InviteType
    {
        InviteType_Invalid, //0x0000
        InviteType_Invite, //0x0001
        InviteType_JoinSession, //0x0002
        InviteType_JoinFriendSession, //0x0003
        InviteType_Count //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 05659
    // TypeInfo:  0x02C1DE88
    enum InvitePlatform
    {
        InvitePlatform_Invalid, //0x0000
        InvitePlatform_X360, //0x0001
        InvitePlatform_PS3, //0x0002
        InvitePlatform_PC, //0x0003
        InvitePlatform_Count //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 05653
    // TypeInfo:  0x02C1DE38
    enum SwitchTeamsResult
    {
        SwitchTeamsResult_Success, //0x0000
        SwitchTeamsResult_FailedUnbalanced, //0x0001
        SwitchTeamsResult_FailedTooSoon, //0x0002
        SwitchTeamsResult_FailedTooFewTickets, //0x0003
        SwitchTeamsResult_FailedMustBeDead, //0x0004
        SwitchTeamsResult_FailedTeamFull, //0x0005
        SwitchTeamsResult_FailedDisabled //0x0006
    };

    ////////////////////////////////////////
    // RuntimeId: 05627
    // TypeInfo:  0x02C1DCC8
    enum VeniceNetworkUnlockGainedType
    {
        VeniceNetworkUnlockGainedType_Bucket, //0x0000
        VeniceNetworkUnlockGainedType_Rank, //0x0001
        VeniceNetworkUnlockGainedType_Weapon, //0x0002
        VeniceNetworkUnlockGainedType_Award //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 05602
    // TypeInfo:  0x02C1CFB4
    enum PoseType
    {
        PoseType_Current, //0x0000
        PoseType_Stand, //0x0001
        PoseType_Crouch //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 05600
    // TypeInfo:  0x02C1CFA4
    enum ShootType
    {
        ShootType_OneBurstPerTarget, //0x0000
        ShootType_Continuous //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 05594
    // TypeInfo:  0x02C1CF3C
    enum TargetPosKnowledgeType
    {
        TargetPosKnown, //0x0000
        TargetPosUnknown, //0x0001
        TargetPosFind //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 05574
    // TypeInfo:  0x02C1CDA0
    enum CombatPatrolMode
    {
        CombatPatrolMode_MustStayOnPath, //0x0000
        CombatPatrolMode_FreeToFight, //0x0001
        CombatPatrolMode_FreeToFightAtDestination //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 05486
    // TypeInfo:  0x02C1B588
    enum TerrainRenderMode
    {
        TerrainRenderMode_Default, //0x0000
        TerrainRenderMode_DrawPassCount2d, //0x0001
        TerrainRenderMode_LayerCount2d, //0x0002
        TerrainRenderMode_LayerCount3d, //0x0003
        TerrainRenderMode_MaskedLayerCount2d, //0x0004
        TerrainRenderMode_MaskedLayerCount3d //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 05319
    // TypeInfo:  0x02C18A44
    enum ClientGameType
    {
        ClientGameType_SinglePlayer, //0x0000
        ClientGameType_Hosted, //0x0001
        ClientGameType_Joined //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 05317
    // TypeInfo:  0x02C18A34
    enum ClientState
    {
        ClientState_WaitingForStaticBundleLoad, //0x0000
        ClientState_LoadProfileOptions, //0x0001
        ClientState_LostConnection, //0x0002
        ClientState_Startup, //0x0003
        ClientState_StartServer, //0x0004
        ClientState_WaitingForLevel, //0x0005
        ClientState_StartLoadingLevel, //0x0006
        ClientState_WaitingForLevelLoaded, //0x0007
        ClientState_WaitingForLevelLink, //0x0008
        ClientState_LevelLinked, //0x0009
        ClientState_WaitingUIIngame, //0x000A
        ClientState_Ingame, //0x000B
        ClientState_ConnectToServer, //0x000C
        ClientState_WaitingUILoading, //0x000D
        ClientState_WaitingUIUnload, //0x000E
        ClientState_None //0x000F
    };

    ////////////////////////////////////////
    // RuntimeId: 05098
    // TypeInfo:  0x02C15528
    enum PolynomialOperation
    {
        Multiplication, //0x0000
        Addition, //0x0001
        Subtraction //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 05078
    // TypeInfo:  0x02C153A8
    enum EmitterParameter
    {
        EmitterParameterNone, //0x0000
        EmitterParameter1, //0x0001
        EmitterParameter2, //0x0002
        EmitterParameter3, //0x0003
        EmitterParameterVec, //0x0004
        EmitterParameterVecAverage, //0x0005
        EmitterParameterDistance, //0x0006
        EmitterParameter4 //0x0007
    };

    ////////////////////////////////////////
    // RuntimeId: 05076
    // TypeInfo:  0x02C15398
    enum ProcessorType
    {
        PtBaseEmitter, //0x0000
        PtSpawnRate, //0x0001
        PtSpawnSpeed, //0x0002
        PtSpawnPosition, //0x0003
        PtSpawnDirection, //0x0004
        PtSpawnSize, //0x0005
        PtSpawnAnimation, //0x0006
        PtSpawnAnimationFrame, //0x0007
        PtSpawnRotation, //0x0008
        PtSpawnOrientation, //0x0009
        PtSpawnRotationSpeed, //0x000A
        PtUpdatePosition, //0x000B
        PtUpdateAge, //0x000C
        PtTurbulance, //0x000D
        PtGravity, //0x000E
        PtLocalForce, //0x000F
        PtAirResistance, //0x0010
        PtUpdateLinearVelocity, //0x0011
        PtUpdateOrientation, //0x0012
        PtEmitter, //0x0013
        PtUpdateColor, //0x0014
        PtUpdateColorLeaf, //0x0015
        PtUpdateTransparency, //0x0016
        PtUpdateTextureCoords, //0x0017
        PtUpdateRotation, //0x0018
        PtUpdateSizeX, //0x0019
        PtUpdateSizeY, //0x001A
        PtUpdateSizeZ, //0x001B
        PtUpdateSize, //0x001C
        PtUpdateAlphaLevelMin, //0x001D
        PtUpdateAlphaLevelMax, //0x001E
        PtUpdateAlphaLevelScale, //0x001F
        PtUpdateClipScale, //0x0020
        PtUpdateCameraProximity, //0x0021
        ProcessorTypeCount //0x0022
    };

    ////////////////////////////////////////
    // RuntimeId: 05074
    // TypeInfo:  0x02C15388
    enum EvaluatorType
    {
        EtDefault, //0x0000
        EtBox, //0x0001
        EtPolynomial, //0x0002
        EtPolynomialOperator, //0x0003
        EtRandom, //0x0004
        EtRandomXYZ, //0x0005
        EtRotateVector, //0x0006
        EtSampleTexture, //0x0007
        EtSphere, //0x0008
        EtConstant, //0x0009
        EtPolynomialColorInterp, //0x000A
        EtCamProx, //0x000B
        EvaluatorTypeCount //0x000C
    };

    ////////////////////////////////////////
    // RuntimeId: 05072
    // TypeInfo:  0x02C15378
    enum EmittableType
    {
        Point, //0x0000
        Quad, //0x0001
        ScreenAlignedQuad, //0x0002
        DirectionAlignedQuad, //0x0003
        WorldAlignedQuad, //0x0004
        Trail, //0x0005
        ParticleMesh, //0x0006
        EmittableTypeCount //0x0007
    };

    ////////////////////////////////////////
    // RuntimeId: 05070
    // TypeInfo:  0x02C15368
    enum EmittableField
    {
        EfZero, //0x0000
        EfOne, //0x0001
        EfNormTime, //0x0002
        EfEmitterNormTime, //0x0003
        EfSpawnAnimationSpeed, //0x0004
        EfSpawnAnimationFrameIndex, //0x0005
        EfVelocity, //0x0006
        EfParameters, //0x0007
        EfRotation, //0x0008
        EfSpeed, //0x0009
        EfUserDefined, //0x000A
        EfConstantFloat, //0x000B
        EfConstantVec, //0x000C
        EfNone //0x000D
    };

    ////////////////////////////////////////
    // RuntimeId: 04910
    // TypeInfo:  0x02C13750
    enum SampleCenter
    {
        SampleCenter_Center, //0x0000
        SampleCenter_TopLeft //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 04906
    // TypeInfo:  0x02C13714
    enum RasterNodeUsage
    {
        RasterNodeUsage_Default, //0x0000
        RasterNodeUsage_Disabled, //0x0001
        RasterNodeUsage_Persistent, //0x0002
        RasterNodeUsage_PersistentDedicatedServer, //0x0003
        RasterNodeUsage_Pruned //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 04898
    // TypeInfo:  0x02C1369C
    enum RasterTreeBuildMode
    {
        RasterTreeBuildMode_InlinePersistentStreamRest, //0x0000
        RasterTreeBuildMode_InlinePersistentRemoveRest, //0x0001
        RasterTreeBuildMode_InlineAll //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 04884
    // TypeInfo:  0x02C1362C
    enum TerrainDrawPassType
    {
        TerrainDrawPassType_SinglePass, //0x0000
        TerrainDrawPassType_MultipassFirst, //0x0001
        TerrainDrawPassType_MultipassConsecutive //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 04878
    // TypeInfo:  0x02C135C4
    enum TerrainBrushDetailOperation
    {
        Lerp, //0x0000
        Add, //0x0001
        Multiply //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 04872
    // TypeInfo:  0x02C1355C
    enum TerrainLayerType
    {
        TerrainLayerType_IgnoreMask, //0x0000
        TerrainLayerType_Masked, //0x0001
        TerrainLayerType_BinaryMasked //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 04862
    // TypeInfo:  0x02C134D4
    enum MeshScatteringInstanceDataMode
    {
        MeshScatteringInstanceDataMode_None, //0x0000
        MeshScatteringInstanceDataMode_Normal, //0x0001
        MeshScatteringInstanceDataMode_NormalAndColor //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 04860
    // TypeInfo:  0x02C134C4
    enum UndergrowthOrientationMode
    {
        UndergrowthOrientationMode_Horizontal, //0x0000
        UndergrowthOrientationMode_LeanToTerrain, //0x0001
        UndergrowthOrientationMode_SkewToTerrain //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 04858
    // TypeInfo:  0x02C134B4
    enum MeshScatteringOrientationMode
    {
        MeshScatteringOrientationMode_Horizontal, //0x0000
        MeshScatteringOrientationMode_LeanToTerrain, //0x0001
        MeshScatteringOrientationMode_SkewToTerrain //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 04856
    // TypeInfo:  0x02C134A4
    enum UndergrowthRotationMode
    {
        UndergrowthRotationMode_Random, //0x0000
        UndergrowthRotationMode_TowardsSlope, //0x0001
        UndergrowthRotationMode_Fixed //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 04854
    // TypeInfo:  0x02C13494
    enum MeshScatteringRotationMode
    {
        MeshScatteringRotationMode_Random, //0x0000
        MeshScatteringRotationMode_TowardsSlope, //0x0001
        MeshScatteringRotationMode_Fixed //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 04852
    // TypeInfo:  0x02C13484
    enum MeshScatteringElevationMode
    {
        MeshScatteringElevationMode_SnapBoundingBox, //0x0000
        MeshScatteringElevationMode_SnapPivotPoint //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 04824
    // TypeInfo:  0x02C12650
    enum BreakableControllerComponentNetworkIdMultiplier
    {
        BreakableControllerComponentNetworkIdMultiplier_HidePartObjects, //0x0000
        BreakableControllerComponentNetworkIdMultiplier_DisablePart, //0x0001
        BreakableControllerComponentNetworkIdMultiplier_PartCount //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 04822
    // TypeInfo:  0x02C12640
    enum BreakableControllerComponentNetworkId
    {
        BreakableControllerComponentNetworkId_Collapsed, //0x0000
        BreakableControllerComponentNetworkId_SpawnPartObjectsOnCollapse, //0x0001
        BreakableControllerComponentNetworkId_ExtraPartCount //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 04774
    // TypeInfo:  0x02C11F28
    enum UIListEmptyRowType
    {
        emptyRow_28px, //0x0000
        emptyRow_20px, //0x0001
        emptyRowNone //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 04772
    // TypeInfo:  0x02C11F18
    enum UIListUnFocusMode
    {
        DeSelect, //0x0000
        ChangeState, //0x0001
        None //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 04770
    // TypeInfo:  0x02C11F08
    enum UIListRowType
    {
        MixedRows, //0x0000
        basicRow, //0x0001
        baseSpawn, //0x0002
        squadSpawn, //0x0003
        MenuRow_34px, //0x0004
        MenuRow_64px, //0x0005
        ScoreboardRow, //0x0006
        ScoreboardRow64Players, //0x0007
        TextSelectorRow, //0x0008
        TextSelectorRow_28px, //0x0009
        BarSelectorRow, //0x000A
        SquadRow, //0x000B
        ServerListRow, //0x000C
        StoreRow, //0x000D
        ComCenterStandardDelimiter, //0x000E
        ComCenterSquadDelimiter, //0x000F
        ComCenterStandardFriendRow, //0x0010
        ComCenterSquadFriendRow, //0x0011
        ComCenterSquadButtonRow, //0x0012
        ComCenterLivePartyDelimiter, //0x0013
        ComCenterLivePartyFriendRow, //0x0014
        SquadDetailedRow, //0x0015
        ComCenterLivePartyButtonRow, //0x0016
        CoopMissionTeamRow, //0x0017
        LeaderBoardRow, //0x0018
        CoopLeaderBoardRow, //0x0019
        BattleLogStandardRow, //0x001A
        BattleLogNewsRow, //0x001B
        ServerLocationsRow, //0x001C
        ServerExtendRow, //0x001D
        ServerInfoSettingsRow, //0x001E
        ServerInfoPlayersRow, //0x001F
        CheckBoxRow, //0x0020
        BarSelectorRow_28px, //0x0021
        BasicRow_34px, //0x0022
        KeyBindingRow, //0x0023
        SquadListRow, //0x0024
        teamSpawn, //0x0025
        MatchListRow, //0x0026
        MatchPlayerListRow, //0x0027
        ManageServerPlayerRow, //0x0028
        ServerSettingsRow, //0x0029
        ServerPlayersRow, //0x002A
        TopPlayersRow, //0x002B
        MenuRowPremium_34px, //0x002C
        MenuRowPremium_64px //0x002D
    };

    ////////////////////////////////////////
    // RuntimeId: 04768
    // TypeInfo:  0x02C11EF8
    enum UIListNavigationType
    {
        loop, //0x0000
        noLoop, //0x0001
        sendEventTopBottom, //0x0002
        sendEventTop, //0x0003
        sendEventBottom, //0x0004
        none //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 04754
    // TypeInfo:  0x02C11E50
    enum UICreditsTextType
    {
        SingleLine, //0x0000
        MultiLine //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 04736
    // TypeInfo:  0x02C11D6C
    enum UIIconState
    {
        UIIconState_Default, //0x0000
        UIIconState_Enemy, //0x0001
        UIIconState_Friendly, //0x0002
        UIIconState_Neutral, //0x0003
        UIIconState_Squad, //0x0004
        UIIconState_Blink, //0x0005
        UIIconState_BlinkEnemy, //0x0006
        UIIconState_Count //0x0007
    };

    ////////////////////////////////////////
    // RuntimeId: 04681
    // TypeInfo:  0x02C1195C
    enum UIPopupType
    {
        PopupType_MemCard, //0x0000
        PopupType_ControllerDisconnected, //0x0001
        PopupType_Login, //0x0002
        PopupType_TermsOfService, //0x0003
        PopupType_PrivacyPolicy, //0x0004
        PopupType_RevivePrompt, //0x0005
        PopupType_Generic, //0x0006
        PopupType_ServerReminder, //0x0007
        PopupType_Error, //0x0008
        PopupType_ManagePlayer, //0x0009
        PopupType_Invalid, //0x000A
        PopupType_IngamePlayer, //0x000B
        PopupType_OnlinePass, //0x000C
        PopupType_Origin, //0x000D
        PopupType_UpdateAvailable, //0x000E
        PopupType_Dynamic, //0x000F
        PopupType_GameQueue, //0x0010
        PopupType_COOPMatchmaking, //0x0011
        PopupType_PromoInfo //0x0012
    };

    ////////////////////////////////////////
    // RuntimeId: 04659
    // TypeInfo:  0x02C117B0
    enum VehicleStateType
    {
        VehicleStateType_Normal, //0x0000
        VehicleStateType_LowHealth, //0x0001
        VehicleStateType_CriticalState, //0x0002
        VehicleStateType_LAST_ITEM //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 04653
    // TypeInfo:  0x02C11748
    enum ScoreboardPosition
    {
        ScoreboardPosition_FirstPlace, //0x0000
        ScoreboardPosition_SecondPlace, //0x0001
        ScoreboardPosition_ThirdPlace, //0x0002
        ScoreboardPosition_FourthPlace, //0x0003
        ScoreboardPosition_FifthPlace, //0x0004
        ScoreboardPosition_LAST_ITEM //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 04609
    // TypeInfo:  0x02C1139C
    enum OrderType
    {
        OrderType_None, //0x0000
        OrderType_RequestHeal, //0x0001
        OrderType_LetMeHeal, //0x0002
        OrderType_RequestAmmo, //0x0003
        OrderType_LetMeGiveAmmo, //0x0004
        OrderType_LetMeRepair, //0x0005
        OrderType_RequestRepair, //0x0006
        OrderType_RequestRide, //0x0007
        OrderType_GoGoGo, //0x0008
        OrderType_FollowMe, //0x0009
        OrderType_Negative, //0x000A
        OrderType_Sorry, //0x000B
        OrderType_ThankYou, //0x000C
        OrderType_RogerThat, //0x000D
        OrderType_NeedBackup, //0x000E
        OrderType_GetIn, //0x000F
        OrderType_GetOut, //0x0010
        OrderType_Attack, //0x0011
        OrderType_Count //0x0012
    };

    ////////////////////////////////////////
    // RuntimeId: 04607
    // TypeInfo:  0x02C1138C
    enum SpotType
    {
        SpotType_None, //0x0000
        SpotType_Active, //0x0001
        SpotType_Passive, //0x0002
        SpotType_Radar, //0x0003
        SpotType_Unspottable, //0x0004
        SpotType_Count //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 04533
    // TypeInfo:  0x02C10E48
    enum PresenceRequest
    {
        PresenceRequest_Invalid, //0x0000
        PresenceRequest_DownloadBlob, //0x0001
        PresenceRequest_UploadBlob, //0x0002
        PresenceRequest_DownloadUserSettings, //0x0003
        PresenceRequest_UploadUserSettings, //0x0004
        PresenceRequest_InitializeCommerceService, //0x0005
        PresenceRequest_ShutDownCommerceService, //0x0006
        PresenceRequest_EnumerateCommerceContent, //0x0007
        PresenceRequest_PurchaseCommerceContent, //0x0008
        PresenceRequest_PurchaseConsumableCommerceContent, //0x0009
        PresenceRequest_ConsumeCommerceContent, //0x000A
        PresenceRequest_GetCommerceContentImage, //0x000B
        PresenceRequest_CheckOnlinePass, //0x000C
        PresenceRequest_PurchaseOnlinePass, //0x000D
        PresenceRequest_RedeemOnlinePassCode, //0x000E
        PresenceRequest_SetOverlayUrl, //0x000F
        PresenceRequest_ListEntitlements, //0x0010
        PresenceRequest_GrantEntitlement, //0x0011
        PresenceRequest_GetFriends, //0x0012
        PresenceRequest_InviteFriend, //0x0013
        PresenceRequest_RemoveFriend, //0x0014
        PresenceRequest_HttpGet, //0x0015
        PresenceRequest_JoinGameByOnlineNativeData, //0x0016
        PresenceRequest_DownloadUpdate, //0x0017
        PresenceRequest_SendMessage, //0x0018
        PresenceRequest_CreatePlaygroup, //0x0019
        PresenceRequest_JoinPlaygroup, //0x001A
        PresenceRequest_JoinPlaygroupByUsername, //0x001B
        PresenceRequest_JoinPlaygroupByOnlineNativeData, //0x001C
        PresenceRequest_JoinPlaygroupByInviteToken, //0x001D
        PresenceRequest_SetPlaygroupAttribute, //0x001E
        PresenceRequest_LeavePlaygroup, //0x001F
        PresenceRequest_DownloadStatistics, //0x0020
        PresenceRequest_GetLeaderboard, //0x0021
        PresenceRequest_UploadProgression, //0x0022
        PresenceRequest_SendInvite, //0x0023
        PresenceRequest_KickFromPlaygroup, //0x0024
        PresenceRequest_ViewInvite, //0x0025
        PresenceRequest_SendInviteToLiveParty, //0x0026
        PresenceRequest_SetUserInfoAttribute, //0x0027
        PresenceRequest_GetServerBrowserSnapshot, //0x0028
        PresenceRequest_GetGamesWithFriends, //0x0029
        PresenceRequest_GetGamesByServerId, //0x002A
        PresenceRequest_GetDetailedServerInfo, //0x002B
        PresenceRequest_UnlockAchievements, //0x002C
        PresenceRequest_RspGetServers, //0x002D
        PresenceRequest_RspGetServerDetails, //0x002E
        PresenceRequest_RspUpdateServerSettings, //0x002F
        PresenceRequest_RspGetConfig, //0x0030
        PresenceRequest_RspRestartServer, //0x0031
        PresenceRequest_RspUpdatePreset, //0x0032
        PresenceRequest_RspUpdateMapRotation, //0x0033
        PresenceRequest_RspUpdateAdminUser, //0x0034
        PresenceRequest_RspUpdateBannedUser, //0x0035
        PresenceRequest_RspUpdateVipUser, //0x0036
        PresenceRequest_RspUpdateServerBanner, //0x0037
        PresenceRequest_ReportServerBanner, //0x0038
        PresenceRequest_GetUserId, //0x0039
        PresenceRequest_VerifyGrantEntitlement, //0x003A
        PresenceRequest_ResetStatistics, //0x003B
        PresenceRequest_GetXPromoInformation, //0x003C
        PresenceRequest_GetContentInformation, //0x003D
        PresenceRequest_DownloadPromoItem, //0x003E
        PresenceRequest_Count //0x003F
    };

    ////////////////////////////////////////
    // RuntimeId: 04529
    // TypeInfo:  0x02C10E28
    enum VirtualGameState
    {
        VirtualGameState_Inactive, //0x0000
        VirtualGameState_Active, //0x0001
        VirtualGameState_Expired, //0x0002
        VirtualGameState_Recreate, //0x0003
        VirtualGameState_Updated //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 04511
    // TypeInfo:  0x02C10D0C
    enum ServerBackendAttribute
    {
        ServerBackendAttribute_Level, //0x0000
        ServerBackendAttribute_GameMode, //0x0001
        ServerBackendAttribute_LevelLocation, //0x0002
        ServerBackendAttribute_Mod, //0x0003
        ServerBackendAttribute_Public, //0x0004
        ServerBackendAttribute_Vip, //0x0005
        ServerBackendAttribute_Preset, //0x0006
        ServerBackendAttribute_Settings1, //0x0007
        ServerBackendAttribute_Settings2, //0x0008
        ServerBackendAttribute_MapSettings, //0x0009
        ServerBackendAttribute_Maps1, //0x000A
        ServerBackendAttribute_Maps2, //0x000B
        ServerBackendAttribute_Description1, //0x000C
        ServerBackendAttribute_Description2, //0x000D
        ServerBackendAttribute_Admins1, //0x000E
        ServerBackendAttribute_Admins2, //0x000F
        ServerBackendAttribute_ReservedSlots1, //0x0010
        ServerBackendAttribute_ReservedSlots2, //0x0011
        ServerBackendAttribute_Message, //0x0012
        ServerBackendAttribute_BannerUrl, //0x0013
        ServerBackendAttribute_Mission, //0x0014
        ServerBackendAttribute_Difficulty, //0x0015
        ServerBackendAttribute_PingSite, //0x0016
        ServerBackendAttribute_MapsInfo, //0x0017
        ServerBackendAttribute_Owner, //0x0018
        ServerBackendAttribute_MatchId, //0x0019
        ServerBackendAttribute_RosterA1, //0x001A
        ServerBackendAttribute_RosterA2, //0x001B
        ServerBackendAttribute_RosterA3, //0x001C
        ServerBackendAttribute_RosterA4, //0x001D
        ServerBackendAttribute_RosterA5, //0x001E
        ServerBackendAttribute_RosterA6, //0x001F
        ServerBackendAttribute_RosterA7, //0x0020
        ServerBackendAttribute_RosterA8, //0x0021
        ServerBackendAttribute_RosterA9, //0x0022
        ServerBackendAttribute_RosterB1, //0x0023
        ServerBackendAttribute_RosterB2, //0x0024
        ServerBackendAttribute_RosterB3, //0x0025
        ServerBackendAttribute_RosterB4, //0x0026
        ServerBackendAttribute_RosterB5, //0x0027
        ServerBackendAttribute_RosterB6, //0x0028
        ServerBackendAttribute_RosterB7, //0x0029
        ServerBackendAttribute_RosterB8, //0x002A
        ServerBackendAttribute_RosterB9, //0x002B
        ServerBackendAttribute_Spectators, //0x002C
        ServerBackendAttribute_ExpirationDate, //0x002D
        ServerBackendAttribute_MatchState, //0x002E
        ServerBackendAttribute_PremiumStatus, //0x002F
        ServerBackendAttribute_Count //0x0030
    };

    ////////////////////////////////////////
    // RuntimeId: 04479
    // TypeInfo:  0x02C10AD8
    enum MatchmakingSessionMode
    {
        MatchmakingSessionMode_FindDedicatedServer, //0x0000
        MatchmakingSessionMode_ResetDedicatedServer, //0x0001
        MatchmakingSessionMode_FindPeerGame, //0x0002
        MatchmakingSessionMode_CreatePeerGame, //0x0003
        MatchmakingSessionMode_Invalid //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 04459
    // TypeInfo:  0x02C10A38
    enum MatchmakingRankedMode
    {
        MatchmakingRankedMode_Ranked, //0x0000
        MatchmakingRankedMode_Unranked, //0x0001
        MatchmakingRankedMode_Any //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 04451
    // TypeInfo:  0x02C109F8
    enum MatchmakingVirtualizationMode
    {
        MatchmakingVirtualizationMode_Virtualized, //0x0000
        MatchmakingVirtualizationMode_Standard, //0x0001
        MatchmakingVirtualizationMode_Any //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 04449
    // TypeInfo:  0x02C109E8
    enum MatchmakingPlatform
    {
        MatchmakingPlatform_PC, //0x0000
        MatchmakingPlatform_Console, //0x0001
        MatchmakingPlatform_360, //0x0002
        MatchmakingPlatform_PS3, //0x0003
        MatchmakingPlatform_Any //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 04447
    // TypeInfo:  0x02C109D8
    enum MatchmakingPeer2PeerMode
    {
        MatchmakingPeer2PeerMode_FullMesh, //0x0000
        MatchmakingPeer2PeerMode_PartialMesh, //0x0001
        MatchmakingPeer2PeerMode_DirtyCastFailover //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 04445
    // TypeInfo:  0x02C109C8
    enum MatchmakingNetworkTopology
    {
        MatchmakingNetworkTopology_Disabled, //0x0000
        MatchmakingNetworkTopology_Peer2Peer, //0x0001
        MatchmakingNetworkTopology_PeerHosted, //0x0002
        MatchmakingNetworkTopology_DedicatedServer //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 04377
    // TypeInfo:  0x02C10578
    enum DogTagCategory
    {
        DTC_Invalid, //0x0000
        DTC_Ribbons, //0x0001
        DTC_Medals, //0x0002
        DTC_Ranks, //0x0003
        DTC_ServiceStars, //0x0004
        DTC_Weapons, //0x0005
        DTC_Misc, //0x0006
        DTC_Assassin //0x0007
    };

    ////////////////////////////////////////
    // RuntimeId: 04287
    // TypeInfo:  0x02C0D670
    enum UISettingSymbols
    {
        UISettingSymbols_Pluses, //0x0000
        UISettingSymbols_Values, //0x0001
        UISettingSymbols_None //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 04285
    // TypeInfo:  0x02C0D660
    enum UISettingType
    {
        UISettingType_Slider, //0x0000
        UISettingType_TickerUpDown, //0x0001
        UISettingType_Droplist, //0x0002
        UISettingType_Checkbox, //0x0003
        UISettingType_Text, //0x0004
        UISettingType_NotSet //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 04171
    // TypeInfo:  0x02C0CD20
    enum UI3dIconQuery
    {
        UI3dIconQuery_Active3dIconsData //0x0000
    };

    ////////////////////////////////////////
    // RuntimeId: 04159
    // TypeInfo:  0x02C0CC50
    enum UIWidgetEventQuery
    {
        UIWidgetEventQuery_FireEvent //0x0000
    };

    ////////////////////////////////////////
    // RuntimeId: 04105
    // TypeInfo:  0x02C0C7F0
    enum DTagUploadPolicy
    {
        DTUP_FirstTimeSeenAndEndOfRound, //0x0000
        DTUP_Always //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 04093
    // TypeInfo:  0x02C0C720
    enum ProgressType
    {
        PT_CapturePointCooldown //0x0000
    };

    ////////////////////////////////////////
    // RuntimeId: 04065
    // TypeInfo:  0x02C0C544
    enum UIColorType
    {
        UIColorType_Unselected, //0x0000
        UIColorType_Selected, //0x0001
        UIColorType_SelectInside, //0x0002
        UIColorType_UnselectedInside, //0x0003
        UIColorType_Locked, //0x0004
        UIColorType_IconBg, //0x0005
        UIColorType_FrameSoft, //0x0006
        UIColorType_FrameHard, //0x0007
        UIColorType_HudYellow, //0x0008
        UIColorType_CriticalColor, //0x0009
        UIColorType_Friendly, //0x000A
        UIColorType_Enemy, //0x000B
        UIColorType_SquadColor, //0x000C
        UIColorType_NormalTextColor, //0x000D
        UIColorType_HeaderTextColor, //0x000E
        UIColorType_StaticHUDColor, //0x000F
        UIColorType_DynamicHUDColor, //0x0010
        UIColorType_MouseOver, //0x0011
        UIColorType_MouseOut //0x0012
    };

    ////////////////////////////////////////
    // RuntimeId: 04055
    // TypeInfo:  0x02C0C4D8
    enum UIMinimapZoomState
    {
        UIMinimapZoomState_Default, //0x0000
        UIMinimapZoomState_Overview, //0x0001
        UIMinimapZoomState_CombatArea, //0x0002
        UIMinimapZoomState_Mission, //0x0003
        UIMinimapZoomState_Artillery, //0x0004
        UIMinimapZoomState_Spawn, //0x0005
        UIMinimapZoomState_ManDown, //0x0006
        UIMinimapZoomState_Count //0x0007
    };

    ////////////////////////////////////////
    // RuntimeId: 04047
    // TypeInfo:  0x02C0C444
    enum MinimapIconInteractionConcept
    {
        MinimapIconInteractionConcept_None, //0x0000
        MinimapIconInteractionConcept_SelectSpawnPoint, //0x0001
        MinimapIconInteractionConcept_RemoveOrder, //0x0002
        MinimapIconInteractionConcept_AttackDefendCapturePoint //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 04045
    // TypeInfo:  0x02C0C434
    enum UIConstants
    {
        UIConstants_MaxCaptureAreas //0x0000
    };

    ////////////////////////////////////////
    // RuntimeId: 04007
    // TypeInfo:  0x02C0A538
    enum WeaponUnlocks
    {
        WeaponUnlocks_MaxAmount //0x0000
    };

    ////////////////////////////////////////
    // RuntimeId: 04005
    // TypeInfo:  0x02C0A528
    enum WeaponSlot
    {
        WeaponSlot_0, //0x0000
        WeaponSlot_1, //0x0001
        WeaponSlot_2, //0x0002
        WeaponSlot_3, //0x0003
        WeaponSlot_4, //0x0004
        WeaponSlot_5, //0x0005
        WeaponSlot_6, //0x0006
        WeaponSlot_7, //0x0007
        WeaponSlot_8, //0x0008
        WeaponSlot_9, //0x0009
        WeaponSlot_NumSlots, //0x000A
        WeaponSlot_NotDefined //0x000B
    };

    ////////////////////////////////////////
    // RuntimeId: 04003
    // TypeInfo:  0x02C0A518
    enum GearSlot
    {
        GearSlot_Melee, //0x0000
        GearSlot_Sidearm, //0x0001
        GearSlot_Primary, //0x0002
        GearSlot_Auxiliary, //0x0003
        GearSlot_Secondary, //0x0004
        GearSlot_Gadget1, //0x0005
        GearSlot_Gadget2, //0x0006
        GearSlot_Gadget3, //0x0007
        GearSlot_Gadget4, //0x0008
        GearSlot_Gadget5, //0x0009
        GearSlot_Gadget6, //0x000A
        GearSlot_Gadget7, //0x000B
        GearSlot_GearSlotCount, //0x000C
        GearSlot_NotEquipped //0x000D
    };

    ////////////////////////////////////////
    // RuntimeId: 03985
    // TypeInfo:  0x02C0A424
    enum WeaponOverrideValueType
    {
        OverrideValue_BulletEntity_DistributeDamageOverTime, //0x0000
        OverrideValue_BulletEntity_StartDamage, //0x0001
        OverrideValue_BulletEntity_EndDamage, //0x0002
        OverrideValue_BulletEntity_DamageFalloffStartDistance, //0x0003
        OverrideValue_BulletEntity_DamageFalloffEndDistance //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 03979
    // TypeInfo:  0x02C0A3BC
    enum QuickThrowTypeEnum
    {
        QttHand, //0x0000
        QttBag //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 03961
    // TypeInfo:  0x02C0A284
    enum WeaponAnimType
    {
        WeaponAnimType_NoAddon, //0x0000
        WeaponAnimType_Bipod, //0x0001
        WeaponAnimType_Foregrip, //0x0002
        WeaponAnimType_40mm_GL, //0x0003
        WeaponAnimType_40mm_GL_Fire, //0x0004
        WeaponAnimType_Underslung_Shotgun, //0x0005
        WeaponAnimType_Underslung_Shotgun_Fire, //0x0006
        WeaponAnimType_Straight_Pull_Bolt, //0x0007
        WeaponAnimType_Extended_Mag //0x0008
    };

    ////////////////////////////////////////
    // RuntimeId: 03935
    // TypeInfo:  0x02C0A09C
    enum WeaponClassEnum
    {
        wc12gauge, //0x0000
        wc338Magnum, //0x0001
        wc357Magnum, //0x0002
        wc44Magnum, //0x0003
        wc45cal, //0x0004
        wc46x30mm, //0x0005
        wc50cal, //0x0006
        wc545x45mmWP, //0x0007
        wc556x45mmNATO, //0x0008
        wc57x28mm, //0x0009
        wc58x42mm, //0x000A
        wc762x39mmWP, //0x000B
        wc762x51mmNATO, //0x000C
        wc762x54mmR, //0x000D
        wc9x19mm, //0x000E
        wc9x39mm, //0x000F
        wcAssault, //0x0010
        wcShotgun, //0x0011
        wcSmg, //0x0012
        wcLmg, //0x0013
        wcSniper, //0x0014
        wcUgl, //0x0015
        wcHgr, //0x0016
        wcAt, //0x0017
        wcATMine, //0x0018
        wcC4, //0x0019
        wcAutoInjector, //0x001A
        wcKnife, //0x001B
        wcLaserDesignator, //0x001C
        wcLaserPainter, //0x001D
        wcMedKit, //0x001E
        wcMortarStrike, //0x001F
        wcPowerTool, //0x0020
        wcTracerDart, //0x0021
        wcRadioBeacon, //0x0022
        wcEodBot, //0x0023
        wcReconMav, //0x0024
        wcUGS, //0x0025
        wcCount, //0x0026
        wcNone //0x0027
    };

    ////////////////////////////////////////
    // RuntimeId: 03933
    // TypeInfo:  0x02C0A08C
    enum WeaponAnimBaseSetEnum
    {
        wabsRif, //0x0000
        wabsPstl, //0x0001
        wabsHgr, //0x0002
        wabsAT, //0x0003
        wabsShg, //0x0004
        wabsLMG, //0x0005
        wabsBag, //0x0006
        wabsSnp, //0x0007
        wabsCount, //0x0008
        wabsNone //0x0009
    };

    ////////////////////////////////////////
    // RuntimeId: 03923
    // TypeInfo:  0x02C0A03C
    enum AnimatedAimingEnum
    {
        AnimatedAimingTwoHanded, //0x0000
        AnimatedAimingRightHanded, //0x0001
        AnimatedAimingDisabled //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 03921
    // TypeInfo:  0x02C0A02C
    enum AnimatedFireEnum
    {
        AnimatedFireAutomatic, //0x0000
        AnimatedFireSingle, //0x0001
        AnimatedFireHoldAndRelease, //0x0002
        AnimatedFireDelayedSingleShot, //0x0003
        AnimatedFireSimple //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 03893
    // TypeInfo:  0x02C09E6C
    enum ZoomLevelActivateEventType
    {
        ZoomLevelActivateEventType_Disable, //0x0000
        ZoomLevelActivateEventType_Enable, //0x0001
        ZoomLevelActivateEventType_ToggleOnLightSwitch //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 03859
    // TypeInfo:  0x02C09B9C
    enum LockType
    {
        LockAlways, //0x0000
        LockOnRadar, //0x0001
        LockOnHeat, //0x0002
        LockOnLaserPainted, //0x0003
        LockNever, //0x0004
        LockTypeCount //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 03853
    // TypeInfo:  0x02C09B34
    enum WeaponFiringEvent
    {
        WeaponFiringEvent_Push, //0x0000
        WeaponFiringEvent_Pop, //0x0001
        WeaponFiringEvent_PrimaryStartedFiringCallback, //0x0002
        WeaponFiringEvent_PrimaryFireCallback, //0x0003
        WeaponFiringEvent_PrimaryFireReleaseCallback, //0x0004
        WeaponFiringEvent_PrimaryFireShotSpawnedCallback, //0x0005
        WeaponFiringEvent_PrimaryFireAutomaticBeginCallback, //0x0006
        WeaponFiringEvent_PrimaryFireAutomaticEndCallback, //0x0007
        WeaponFiringEvent_PrimaryStoppedFiringCallback, //0x0008
        WeaponFiringEvent_ReloadPrimaryCallback, //0x0009
        WeaponFiringEvent_ReloadPrimaryEndCallback, //0x000A
        WeaponFiringEvent_BoltActionCallback, //0x000B
        WeaponFiringEvent_BoltActionEndCallback, //0x000C
        WeaponFiringEvent_DetonationSwitchCallback, //0x000D
        WeaponFiringEvent_HoldAndReleaseReleaseCallback, //0x000E
        WeaponFiringEvent_UpdateRequired //0x000F
    };

    ////////////////////////////////////////
    // RuntimeId: 03833
    // TypeInfo:  0x02C09A5C
    enum ReloadLogic
    {
        rlWeaponSwitchCancelsUnfinishedReload, //0x0000
        rlReloadUnaffectedByWeaponSwitch //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 03831
    // TypeInfo:  0x02C09A4C
    enum ReloadType
    {
        rtSingleBullet, //0x0000
        rtMagazine, //0x0001
        rtMagazineWithPossibleShorterReload //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 03829
    // TypeInfo:  0x02C09A3C
    enum FireLogicType
    {
        fltSingleFire, //0x0000
        fltSingleFireWithBoltAction, //0x0001
        fltAutomaticFire, //0x0002
        fltBurstFire, //0x0003
        fltHoldAndRelease, //0x0004
        fltDetonatedFiring, //0x0005
        fltCount //0x0006
    };

    ////////////////////////////////////////
    // RuntimeId: 03793
    // TypeInfo:  0x02C09874
    enum WarnTarget
    {
        wtWarnSoldierAndVehicle, //0x0000
        wtWarnSoldier, //0x0001
        wtWarnVehicle, //0x0002
        wtWarnNone //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 03787
    // TypeInfo:  0x02C0980C
    enum GrenadeType
    {
        GrenadeType_NotSet, //0x0000
        GrenadeType_Frag, //0x0001
        GrenadeType_Smoke, //0x0002
        GrenadeType_Flash //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 03779
    // TypeInfo:  0x02C09778
    enum AntHitReactionWeaponType
    {
        AntHitReactionWeaponType_Pistol, //0x0000
        AntHitReactionWeaponType_SMG, //0x0001
        AntHitReactionWeaponType_AssaultRifle, //0x0002
        AntHitReactionWeaponType_LMG, //0x0003
        AntHitReactionWeaponType_Shotgun, //0x0004
        AntHitReactionWeaponType_SniperRifle, //0x0005
        AntHitReactionWeaponType_Explosion, //0x0006
        AntHitReactionWeaponType_Flashbang, //0x0007
        AntHitReactionWeaponType_Melee //0x0008
    };

    ////////////////////////////////////////
    // RuntimeId: 03769
    // TypeInfo:  0x02C096D4
    enum RouteType
    {
        RouteStop, //0x0000
        RouteCircular //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 03767
    // TypeInfo:  0x02C096C4
    enum WaypointsSnappingSettings
    {
        UseShapeSettings, //0x0000
        SnapToTerrain, //0x0001
        NoSnap //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 03753
    // TypeInfo:  0x02C095E4
    enum WarningPlayerType
    {
        WPTInflictor, //0x0000
        WPTVictim //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 03673
    // TypeInfo:  0x02C0741C
    enum TeamAssignMode
    {
        TamEqualTeams, //0x0000
        TamFullTeams, //0x0001
        TamOneTeam //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 03657
    // TypeInfo:  0x02C072D8
    enum EventGateState
    {
        EGSInvalid, //0x0000
        EGSUpright, //0x0001
        EGSCrouched, //0x0002
        EGSProne, //0x0003
        EGSZoomed, //0x0004
        EGSCount //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 03651
    // TypeInfo:  0x02C07270
    enum EventCompareGateType
    {
        EventCompareGate_Equals, //0x0000
        EventCompareGate_NotEquals, //0x0001
        EventCompareGate_Less, //0x0002
        EventCompareGate_LessEquals, //0x0003
        EventCompareGate_Greater, //0x0004
        EventCompareGate_GreaterEquals //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 03637
    // TypeInfo:  0x02C07174
    enum MapMarkerColorType
    {
        MMCTRed, //0x0000
        MMCTBlue, //0x0001
        MMCTGreen //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 03635
    // TypeInfo:  0x02C07164
    enum MapMarkerType
    {
        MMTMissionObjective, //0x0000
        MMTSecondaryMissionObjective, //0x0001
        MMTSoldier, //0x0002
        MMTVehicle, //0x0003
        MMTAmmoCrate, //0x0004
        MMTGeneric, //0x0005
        MMTFlag //0x0006
    };

    ////////////////////////////////////////
    // RuntimeId: 03629
    // TypeInfo:  0x02C07118
    enum UIMessageEntityType
    {
        UIMessageEntityType_Message, //0x0000
        UIMessageEntityType_Tooltip, //0x0001
        UIMessageEntityType_LogLines, //0x0002
        UIMessageEntityType_Subtitle, //0x0003
        UIMessageEntityType_FailState, //0x0004
        UIMessageEntityType_OutOfBounds //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 03621
    // TypeInfo:  0x02C07084
    enum MessageReciever
    {
        MrAll, //0x0000
        MrTeam, //0x0001
        MrTeamAndSquad, //0x0002
        MrPlayer //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 03617
    // TypeInfo:  0x02C07048
    enum ObjectiveType
    {
        OTPrimary, //0x0000
        OTTactical, //0x0001
        OTChildMisson //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 03605
    // TypeInfo:  0x02C06F5C
    enum BlinkType
    {
        BTHold, //0x0000
        BTBlink, //0x0001
        BTCount //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 03603
    // TypeInfo:  0x02C06F4C
    enum InteractionEntityType
    {
        IET_None, //0x0000
        IET_RushCrateArm, //0x0001
        IET_RushCrateDisarm, //0x0002
        IET_CTFFlag //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 03601
    // TypeInfo:  0x02C06F3C
    enum DisableTeamType
    {
        DttDisable, //0x0000
        DttUnspawn //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 03581
    // TypeInfo:  0x02C06DA0
    enum CapturableType
    {
        CTDefault, //0x0000
        CTUnableToChangeTeam, //0x0001
        CTCaptureEnablable //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 03571
    // TypeInfo:  0x02C06D18
    enum TicketDecreaseType
    {
        TDTOnKilled, //0x0000
        TDTOnRespawned, //0x0001
        TDTNoDecreaseOnSoldierDeath //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 03515
    // TypeInfo:  0x02C0690C
    enum InputDeviceKeys
    {
        IDK_None, //0x0000
        IDK_Escape, //0x0001
        IDK_1, //0x0002
        IDK_2, //0x0003
        IDK_3, //0x0004
        IDK_4, //0x0005
        IDK_5, //0x0006
        IDK_6, //0x0007
        IDK_7, //0x0008
        IDK_8, //0x0009
        IDK_9, //0x000A
        IDK_0, //0x000B
        IDK_Minus, //0x000C
        IDK_Equals, //0x000D
        IDK_Backspace, //0x000E
        IDK_Tab, //0x000F
        IDK_Q, //0x0010
        IDK_W, //0x0011
        IDK_E, //0x0012
        IDK_R, //0x0013
        IDK_T, //0x0014
        IDK_Y, //0x0015
        IDK_U, //0x0016
        IDK_I, //0x0017
        IDK_O, //0x0018
        IDK_P, //0x0019
        IDK_LeftBracket, //0x001A
        IDK_RightBracket, //0x001B
        IDK_Enter, //0x001C
        IDK_LeftCtrl, //0x001D
        IDK_A, //0x001E
        IDK_S, //0x001F
        IDK_D, //0x0020
        IDK_F, //0x0021
        IDK_G, //0x0022
        IDK_H, //0x0023
        IDK_J, //0x0024
        IDK_K, //0x0025
        IDK_L, //0x0026
        IDK_Semicolon, //0x0027
        IDK_Apostrophe, //0x0028
        IDK_Grave, //0x0029
        IDK_LeftShift, //0x002A
        IDK_Backslash, //0x002B
        IDK_Z, //0x002C
        IDK_X, //0x002D
        IDK_C, //0x002E
        IDK_V, //0x002F
        IDK_B, //0x0030
        IDK_N, //0x0031
        IDK_M, //0x0032
        IDK_Comma, //0x0033
        IDK_Period, //0x0034
        IDK_Slash, //0x0035
        IDK_RightShift, //0x0036
        IDK_Multiply, //0x0037
        IDK_LeftAlt, //0x0038
        IDK_Space, //0x0039
        IDK_Capital, //0x003A
        IDK_F1, //0x003B
        IDK_F2, //0x003C
        IDK_F3, //0x003D
        IDK_F4, //0x003E
        IDK_F5, //0x003F
        IDK_F6, //0x0040
        IDK_F7, //0x0041
        IDK_F8, //0x0042
        IDK_F9, //0x0043
        IDK_F10, //0x0044
        IDK_Numlock, //0x0045
        IDK_ScrollLock, //0x0046
        IDK_Numpad7, //0x0047
        IDK_Numpad8, //0x0048
        IDK_Numpad9, //0x0049
        IDK_Subtract, //0x004A
        IDK_Numpad4, //0x004B
        IDK_Numpad5, //0x004C
        IDK_Numpad6, //0x004D
        IDK_Add, //0x004E
        IDK_Numpad1, //0x004F
        IDK_Numpad2, //0x0050
        IDK_Numpad3, //0x0051
        IDK_Numpad0, //0x0052
        IDK_Decimal, //0x0053
        IDK_OEM_102, //0x0054
        IDK_F11, //0x0055
        IDK_F12, //0x0056
        IDK_F13, //0x0057
        IDK_F14, //0x0058
        IDK_F15, //0x0059
        IDK_Kana, //0x005A
        IDK_Convert, //0x005B
        IDK_NoConvert, //0x005C
        IDK_Yen, //0x005D
        IDK_NumpadEquals, //0x005E
        IDK_PrevTrack, //0x005F
        IDK_At, //0x0060
        IDK_Colon, //0x0061
        IDK_Underline, //0x0062
        IDK_Kanji, //0x0063
        IDK_Stop, //0x0064
        IDK_Ax, //0x0065
        IDK_Unlabeled, //0x0066
        IDK_NextTrack, //0x0067
        IDK_NumpadEnter, //0x0068
        IDK_RightCtrl, //0x0069
        IDK_Mute, //0x006A
        IDK_Calculator, //0x006B
        IDK_PlayPause, //0x006C
        IDK_MediaStop, //0x006D
        IDK_VolumeDown, //0x006E
        IDK_VolumeUp, //0x006F
        IDK_WebHome, //0x0070
        IDK_NumpadComma, //0x0071
        IDK_Divide, //0x0072
        IDK_PrintScreen, //0x0073
        IDK_RightAlt, //0x0074
        IDK_Home, //0x0075
        IDK_ArrowUp, //0x0076
        IDK_PageUp, //0x0077
        IDK_ArrowLeft, //0x0078
        IDK_ArrowRight, //0x0079
        IDK_End, //0x007A
        IDK_ArrowDown, //0x007B
        IDK_PageDown, //0x007C
        IDK_Insert, //0x007D
        IDK_Delete, //0x007E
        IDK_LeftWin, //0x007F
        IDK_RightWin, //0x0080
        IDK_AppMenu, //0x0081
        IDK_Power, //0x0082
        IDK_Sleep, //0x0083
        IDK_Wake, //0x0084
        IDK_WebSearch, //0x0085
        IDK_WebFavorites, //0x0086
        IDK_WebRefresh, //0x0087
        IDK_WebStop, //0x0088
        IDK_WebForward, //0x0089
        IDK_WebBack, //0x008A
        IDK_MyComputer, //0x008B
        IDK_Mail, //0x008C
        IDK_MediaSelect, //0x008D
        IDK_Pause, //0x008E
        IDK_Undefined //0x008F
    };

    ////////////////////////////////////////
    // RuntimeId: 03513
    // TypeInfo:  0x02C068FC
    enum InputDevicePOVs
    {
        IDP_POV_0, //0x0000
        IDP_POV_1, //0x0001
        IDP_POV_2, //0x0002
        IDP_POV_Undefined //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 03511
    // TypeInfo:  0x02C068EC
    enum InputDeviceMouseButtons
    {
        IDB_Button_0, //0x0000
        IDB_Button_1, //0x0001
        IDB_Button_2, //0x0002
        IDB_Button_3, //0x0003
        IDB_Button_4, //0x0004
        IDB_Button_5, //0x0005
        IDB_Button_6, //0x0006
        IDB_Button_7, //0x0007
        IDB_Button_Undefined //0x0008
    };

    ////////////////////////////////////////
    // RuntimeId: 03509
    // TypeInfo:  0x02C068DC
    enum InputDeviceMotionControllerButtons
    {
        IDMCB_Rup, //0x0000
        IDMCB_Rdown, //0x0001
        IDMCB_Rleft, //0x0002
        IDMCB_Rright, //0x0003
        IDMCB_start, //0x0004
        IDMCB_alt, //0x0005
        IDMCB_center, //0x0006
        IDMCB_trigger, //0x0007
        IDMCB_reload, //0x0008
        IDMCB_pumpaction, //0x0009
        IDMCB_Gstab, //0x000A
        IDMCB_Grotleft, //0x000B
        IDMCB_Grotright, //0x000C
        IDMCB_Undefined //0x000D
    };

    ////////////////////////////////////////
    // RuntimeId: 03507
    // TypeInfo:  0x02C068CC
    enum InputDevicePadButtons
    {
        IDB_Lup, //0x0000
        IDB_Ldown, //0x0001
        IDB_Lleft, //0x0002
        IDB_Lright, //0x0003
        IDB_Rup, //0x0004
        IDB_Rdown, //0x0005
        IDB_Rleft, //0x0006
        IDB_Rright, //0x0007
        IDB_Rtopleft, //0x0008
        IDB_Rtopright, //0x0009
        IDB_Lthumb, //0x000A
        IDB_Rthumb, //0x000B
        IDB_start, //0x000C
        IDB_alt, //0x000D
        IDB_Ltrigger, //0x000E
        IDB_Rtrigger, //0x000F
        IDB_Ltrigger2, //0x0010
        IDB_Rtrigger2, //0x0011
        IDB_XButton1, //0x0012
        IDB_XButton2, //0x0013
        IDB_XButton3, //0x0014
        IDB_XButton4, //0x0015
        IDB_XButton5, //0x0016
        IDB_XButton6, //0x0017
        IDB_XButton7, //0x0018
        IDB_XButton8, //0x0019
        IDB_XButton9, //0x001A
        IDB_XButton10, //0x001B
        IDB_XButton11, //0x001C
        IDB_XButton12, //0x001D
        IDB_XButton13, //0x001E
        IDB_XButton14, //0x001F
        IDB_XButton15, //0x0020
        IDB_XButton16, //0x0021
        IDB_XButton17, //0x0022
        IDB_XButton18, //0x0023
        IDB_PosZAxis, //0x0024
        IDB_NegZAxis, //0x0025
        IDB_XRotationPos, //0x0026
        IDB_XRotationNeg, //0x0027
        IDB_YRotationPos, //0x0028
        IDB_YRotationNeg, //0x0029
        IDB_ZRotationPos, //0x002A
        IDB_ZRotationNeg, //0x002B
        IDB_Pov1North, //0x002C
        IDB_Pov1South, //0x002D
        IDB_Pov1West, //0x002E
        IDB_Pov1East, //0x002F
        IDB_Pov2North, //0x0030
        IDB_Pov2South, //0x0031
        IDB_Pov2West, //0x0032
        IDB_Pov2East, //0x0033
        IDB_Pov3North, //0x0034
        IDB_Pov3South, //0x0035
        IDB_Pov3West, //0x0036
        IDB_Pov3East, //0x0037
        IDB_Pov4North, //0x0038
        IDB_Pov4South, //0x0039
        IDB_Pov4West, //0x003A
        IDB_Pov4East, //0x003B
        IDB_Undefined, //0x003C
        IDB_NumPadButtons //0x003D
    };

    ////////////////////////////////////////
    // RuntimeId: 03505
    // TypeInfo:  0x02C068BC
    enum InputDeviceAxes
    {
        IDA_Axis0X, //0x0000
        IDA_Axis0Y, //0x0001
        IDA_Axis0XPos, //0x0002
        IDA_Axis0YPos, //0x0003
        IDA_Axis0XNeg, //0x0004
        IDA_Axis0YNeg, //0x0005
        IDA_Axis1X, //0x0006
        IDA_Axis1Y, //0x0007
        IDA_Axis1XPos, //0x0008
        IDA_Axis1YPos, //0x0009
        IDA_Axis1XNeg, //0x000A
        IDA_Axis1YNeg, //0x000B
        IDA_Axis2X, //0x000C
        IDA_Axis2Y, //0x000D
        IDA_Axis3X, //0x000E
        IDA_Axis3Y, //0x000F
        IDA_Axis4X, //0x0010
        IDA_Axis4Y, //0x0011
        IDA_Axis5X, //0x0012
        IDA_Axis5Y, //0x0013
        IDA_Axis6X, //0x0014
        IDA_Axis6Y, //0x0015
        IDA_Axis7X, //0x0016
        IDA_Axis7Y, //0x0017
        IDA_Undefined //0x0018
    };

    ////////////////////////////////////////
    // RuntimeId: 03503
    // TypeInfo:  0x02C068AC
    enum InputConceptIdentifiers
    {
        ConceptMoveFB, //0x0000
        ConceptMoveLR, //0x0001
        ConceptMoveForward, //0x0002
        ConceptMoveBackward, //0x0003
        ConceptMoveLeft, //0x0004
        ConceptMoveRight, //0x0005
        ConceptYaw, //0x0006
        ConceptPitch, //0x0007
        ConceptRoll, //0x0008
        ConceptRecenterCamera, //0x0009
        ConceptFire, //0x000A
        ConceptAltFire, //0x000B
        ConceptFireCountermeasure, //0x000C
        ConceptReload, //0x000D
        ConceptZoom, //0x000E
        ConceptToggleCamera, //0x000F
        ConceptSprint, //0x0010
        ConceptCrawl, //0x0011
        ConceptToggleWeaponLight, //0x0012
        ConceptJump, //0x0013
        ConceptCrouch, //0x0014
        ConceptCrouchOnHold, //0x0015
        ConceptProne, //0x0016
        ConceptInteract, //0x0017
        ConceptPickUp, //0x0018
        ConceptDrop, //0x0019
        ConceptBreathControl, //0x001A
        ConceptParachute, //0x001B
        ConceptSwitchInventoryItem, //0x001C
        ConceptSelectInventoryItem1, //0x001D
        ConceptSelectInventoryItem2, //0x001E
        ConceptSelectInventoryItem3, //0x001F
        ConceptSelectInventoryItem4, //0x0020
        ConceptSelectInventoryItem5, //0x0021
        ConceptSelectInventoryItem6, //0x0022
        ConceptSelectInventoryItem7, //0x0023
        ConceptSelectInventoryItem8, //0x0024
        ConceptSelectInventoryItem9, //0x0025
        ConceptSwitchToPrimaryWeapon, //0x0026
        ConceptSwitchToGrenadeLauncher, //0x0027
        ConceptSwitchToStaticGadget, //0x0028
        ConceptSwitchToDynamicGadget1, //0x0029
        ConceptSwitchToDynamicGadget2, //0x002A
        ConceptMeleeAttack, //0x002B
        ConceptThrowGrenade, //0x002C
        ConceptCycleFireMode, //0x002D
        ConceptChangeVehicle, //0x002E
        ConceptBrake, //0x002F
        ConceptHandBrake, //0x0030
        ConceptClutch, //0x0031
        ConceptGearUp, //0x0032
        ConceptGearDown, //0x0033
        ConceptGearSwitch, //0x0034
        ConceptNextPosition, //0x0035
        ConceptSelectPosition1, //0x0036
        ConceptSelectPosition2, //0x0037
        ConceptSelectPosition3, //0x0038
        ConceptSelectPosition4, //0x0039
        ConceptSelectPosition5, //0x003A
        ConceptSelectPosition6, //0x003B
        ConceptSelectPosition7, //0x003C
        ConceptSelectPosition8, //0x003D
        ConceptCameraPitch, //0x003E
        ConceptCameraYaw, //0x003F
        ConceptMapZoom, //0x0040
        ConceptMapInnerZoom, //0x0041
        ConceptMapSize, //0x0042
        ConceptMapThreeDimensional, //0x0043
        ConceptScoreboard, //0x0044
        ConceptMenu, //0x0045
        ConceptSpawnMenu, //0x0046
        ConceptCancel, //0x0047
        ConceptCommMenu1, //0x0048
        ConceptCommMenu2, //0x0049
        ConceptCommMenu3, //0x004A
        ConceptAccept, //0x004B
        ConceptDecline, //0x004C
        ConceptSelect, //0x004D
        ConceptBack, //0x004E
        ConceptActivate, //0x004F
        ConceptDeactivate, //0x0050
        ConceptEdit, //0x0051
        ConceptView, //0x0052
        ConceptParentNavigateLeft, //0x0053
        ConceptParentNavigateRight, //0x0054
        ConceptMenuZoomIn, //0x0055
        ConceptMenuZoomOut, //0x0056
        ConceptPanX, //0x0057
        ConceptPanY, //0x0058
        ConceptVoiceFunction1, //0x0059
        ConceptSayAllChat, //0x005A
        ConceptTeamChat, //0x005B
        ConceptSquadChat, //0x005C
        ConceptSquadLeaderChat, //0x005D
        ConceptToggleChat, //0x005E
        ConceptQuicktimeInteractDrag, //0x005F
        ConceptQuicktimeFire, //0x0060
        ConceptQuicktimeBlock, //0x0061
        ConceptQuicktimeFastMelee, //0x0062
        ConceptQuicktimeJumpClimb, //0x0063
        ConceptQuicktimeCrouchDuck, //0x0064
        ConceptFreeCameraMoveUp, //0x0065
        ConceptFreeCameraMoveDown, //0x0066
        ConceptFreeCameraMoveLR, //0x0067
        ConceptFreeCameraMoveFB, //0x0068
        ConceptFreeCameraRotateX, //0x0069
        ConceptFreeCameraRotateY, //0x006A
        ConceptFreeCameraIncreaseSpeed, //0x006B
        ConceptFreeCameraDecreaseSpeed, //0x006C
        ConceptFreeCameraFOVModifier, //0x006D
        ConceptFreeCameraChangeFOV, //0x006E
        ConceptFreeCameraSwitchSpeed, //0x006F
        ConceptFreeCameraTurboSpeed, //0x0070
        ConceptFreeCameraActivator1, //0x0071
        ConceptFreeCameraActivator2, //0x0072
        ConceptFreeCameraActivator3, //0x0073
        ConceptFreeCameraMayaInputActivator, //0x0074
        ConceptTargetedCameraDistance, //0x0075
        ConceptTargetedCameraRotateX, //0x0076
        ConceptTargetedCameraRotateY, //0x0077
        ConceptTargetedCameraChangeSpeed, //0x0078
        ConceptLThumb, //0x0079
        ConceptRThumb, //0x007A
        ConceptToggleMinimapType, //0x007B
        ConceptUndefined, //0x007C
        ConceptSize //0x007D
    };

    ////////////////////////////////////////
    // RuntimeId: 03495
    // TypeInfo:  0x02C05294
    enum ExampleVehicleType
    {
        ExampleVehicleType_Small, //0x0000
        ExampleVehicleType_Medium, //0x0001
        ExampleVehicleType_Large //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 03449
    // TypeInfo:  0x02C04F50
    enum AwardKitAssociation
    {
        AwardKitAssociation_Undefined, //0x0000
        AwardKitAssociation_Assault, //0x0001
        AwardKitAssociation_Engineer, //0x0002
        AwardKitAssociation_Recon, //0x0003
        AwardKitAssociation_Support, //0x0004
        AwardKitAssociation_All, //0x0005
        AwardKitAssociation_LAST_ITEM //0x0006
    };

    ////////////////////////////////////////
    // RuntimeId: 03447
    // TypeInfo:  0x02C04F40
    enum StatsMultiplicity
    {
        StatsMultiplicity_Invalid, //0x0000
        StatsMultiplicity_Solo, //0x0001
        StatsMultiplicity_Club, //0x0002
        StatsMultiplicity_LAST_ITEM //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 03445
    // TypeInfo:  0x02C04F30
    enum AwardGroup
    {
        AwardGroup_Undefined, //0x0000
        AwardGroup_Ribbons, //0x0001
        AwardGroup_DogTags, //0x0002
        AwardGroup_Medals, //0x0003
        AwardGroup_Star, //0x0004
        AwardGroup_Mission, //0x0005
        AwardGroup_LevelComplete, //0x0006
        AwardGroup_Achievements, //0x0007
        AwardGroup_LastAwardGroup //0x0008
    };

    ////////////////////////////////////////
    // RuntimeId: 03443
    // TypeInfo:  0x02C04F20
    enum AwardType
    {
        AwardType_OnceGlobally, //0x0000
        AwardType_OncePerRound, //0x0001
        AwardType_Unlimited, //0x0002
        AwardType_Limited, //0x0003
        AwardType_GateList //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 03431
    // TypeInfo:  0x02C04E6C
    enum AIDeathReactionAnimationType
    {
        AIDeathReactionAnimationType_1, //0x0000
        AIDeathReactionAnimationType_2, //0x0001
        AIDeathReactionAnimationType_3, //0x0002
        AIDeathReactionAnimationType_4, //0x0003
        AIDeathReactionAnimationType_5, //0x0004
        AIDeathReactionAnimationType_Count //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 03421
    // TypeInfo:  0x02C04E00
    enum AIAltFireFromAntState
    {
        AltFireFromAntState_NoForcedFire, //0x0000
        AltFireFromAntState_Prepare, //0x0001
        AltFireFromAntState_ForceFire //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 03393
    // TypeInfo:  0x02C04C24
    enum CoverExitStyle
    {
        CoverExitStyle_ExitCover, //0x0000
        CoverExitStyle_VaultFromCrouchCover, //0x0001
        CoverExitStyle_StrafeFire, //0x0002
        CoverExitStyle_Surprised, //0x0003
        CoverExitStyle_Sprint //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 03391
    // TypeInfo:  0x02C04C14
    enum CoverEnterStrategy
    {
        CoverEnterStrategy_EnterNormally, //0x0000
        CoverEnterStrategy_SkipEnterButAlign, //0x0001
        CoverEnterStrategy_SkipEnterDontAlign //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 03389
    // TypeInfo:  0x02C04C04
    enum CoverPeekType
    {
        CoverPeekType_Out, //0x0000
        CoverPeekType_Over, //0x0001
        CoverPeekType_None //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 03387
    // TypeInfo:  0x02C04BF4
    enum AntCoverFireType
    {
        AntCoverFireType_PeekFire, //0x0000
        AntCoverFireType_FireStepOut, //0x0001
        AntCoverFireType_FireOver, //0x0002
        AntCoverFireType_BlindFireAround, //0x0003
        AntCoverFireType_BlindFireOver, //0x0004
        AntCoverFireType_FlankedWhileFiring, //0x0005
        AntCoverFireType_FireStepOutSlow, //0x0006
        AntCoverFireType_FireStepOutUrgent, //0x0007
        AntCoverFireType_FireStepOut_Crouched, //0x0008
        AntCoverFireType_FireLmg, //0x0009
        AntCoverFireType_FireFromHipStepOut //0x000A
    };

    ////////////////////////////////////////
    // RuntimeId: 03385
    // TypeInfo:  0x02C04BE4
    enum CoverFireType
    {
        CoverFireType_NoFire, //0x0000
        CoverFireType_Peek, //0x0001
        CoverFireType_Out, //0x0002
        CoverFireType_Over, //0x0003
        CoverFireType_BlindOut, //0x0004
        CoverFireType_BlindOver, //0x0005
        CoverFireType_GrenadeOut, //0x0006
        CoverFireType_GrenadeBlindOver, //0x0007
        CoverFireType_Flanked, //0x0008
        CoverFireType_OutSlow, //0x0009
        CoverFireType_OutUrgent, //0x000A
        CoverFireType_OutStandToCrouch, //0x000B
        CoverFireType_Lmg, //0x000C
        CoverFireType_OutFireFromHip, //0x000D
        CoverFireType_FlankedSurprised //0x000E
    };

    ////////////////////////////////////////
    // RuntimeId: 03377
    // TypeInfo:  0x02C04BA4
    enum AntSpeedLevel
    {
        AntSpeedLevel_Still, //0x0000
        AntSpeedLevel_Walk, //0x0001
        AntSpeedLevel_Jog_DEPRECATED_DO_NOT_USE, //0x0002
        AntSpeedLevel_Sneak, //0x0003
        AntSpeedLevel_Run, //0x0004
        AntSpeedLevel_Sprint, //0x0005
        AntSpeedLevel_Stairs_Walk_Up, //0x0006
        AntSpeedLevel_Stairs_Walk_Down, //0x0007
        AntSpeedLevel_Stairs_Run_Up, //0x0008
        AntSpeedLevel_Stairs_Run_Down, //0x0009
        AntSpeedLevel_Stairs_Sprint_Up, //0x000A
        AntSpeedLevel_Stairs_Sprint_Down, //0x000B
        AntSpeedLevel_AntEnumCount, //0x000C
        AntSpeedLevel_NoChange, //0x000D
        AntSpeedLevel_Count //0x000E
    };

    ////////////////////////////////////////
    // RuntimeId: 03375
    // TypeInfo:  0x02C04B94
    enum AntAttentionStateEnum
    {
        AntAttentionStateEnum_Relaxed, //0x0000
        AntAttentionStateEnum_Ready, //0x0001
        AntAttentionStateEnum_Combat, //0x0002
        AntAttentionStateEnum_Evasive, //0x0003
        AntAttentionStateEnum_AntEnumCount, //0x0004
        AntAttentionStateEnum_NoChange, //0x0005
        AntAttentionStateEnum_Count, //0x0006
        AntAttentionStateEnum_Dummy1 //0x0007
    };

    ////////////////////////////////////////
    // RuntimeId: 03373
    // TypeInfo:  0x02C04B84
    enum AntCoverEnum
    {
        AntCoverEnum_RightCover, //0x0000
        AntCoverEnum_LeftCover, //0x0001
        AntCoverEnum_MiddleCover, //0x0002
        AntCoverEnum_MediumLeftCover, //0x0003
        AntCoverEnum_MediumRightCover, //0x0004
        AntCoverEnum_Prone, //0x0005
        AntCoverEnum_Count //0x0006
    };

    ////////////////////////////////////////
    // RuntimeId: 03371
    // TypeInfo:  0x02C04B74
    enum AntPoseEnum
    {
        AntPoseEnum_Stand, //0x0000
        AntPoseEnum_Crouch, //0x0001
        AntPoseEnum_AntEnumCount, //0x0002
        AntPoseEnum_NoChange, //0x0003
        AntPoseEnum_Count //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 03335
    // TypeInfo:  0x02C03D24
    enum VehicleHudType
    {
        VehicleHudType_Car, //0x0000
        VehicleHudType_Tank, //0x0001
        VehicleHudType_Boat, //0x0002
        VehicleHudType_Helicopter, //0x0003
        VehicleHudType_Stationary, //0x0004
        VehicleHudType_Strategic, //0x0005
        VehicleHudType_Dummy, //0x0006
        VehicleHudType_Count //0x0007
    };

    ////////////////////////////////////////
    // RuntimeId: 03307
    // TypeInfo:  0x02C03B2C
    enum UnlockAvailability
    {
        UnlockAvailability_All, //0x0000
        UnlockAvailability_HumanPlayerOnly, //0x0001
        UnlockAvailability_AIOnly //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 03269
    // TypeInfo:  0x02C03864
    enum CharacterPoseAction
    {
        CPTNoAction, //0x0000
        CPTForceToStand, //0x0001
        CPTForceToCroch, //0x0002
        CPTForceToProne //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 03255
    // TypeInfo:  0x02C0374C
    enum AreaTriggerInclude
    {
        ATNone, //0x0000
        ATAll, //0x0001
        ATPlayers, //0x0002
        ATAI, //0x0003
        ATAI_Allies, //0x0004
        ATAI_Enemies, //0x0005
        ATPlayersNotInVehicle //0x0006
    };

    ////////////////////////////////////////
    // RuntimeId: 03223
    // TypeInfo:  0x02C034C4
    enum CameraMovementActionMode
    {
        CameraMovementActionMode_Start, //0x0000
        CameraMovementActionMode_Stop, //0x0001
        CameraMovementActionMode_StartAndStop //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 03203
    // TypeInfo:  0x02C03344
    enum DebugMenuItemType
    {
        DmitItem, //0x0000
        DmitToggle //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 03186
    // TypeInfo:  0x02C031F0
    enum SyncAnimationsState
    {
        Idle, //0x0000
        Play, //0x0001
        Skip, //0x0002
        Stop //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 03174
    // TypeInfo:  0x02C03104
    enum PersistentValueHistoryType
    {
        PersistentValueHistoryType_None, //0x0000
        PersistentValueHistoryType_Delta, //0x0001
        PersistentValueHistoryType_DeltaAndAbsolute, //0x0002
        PersistentValueHistoryType_HistoryOnly //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 03127
    // TypeInfo:  0x02C02D8C
    enum EntryEnterRestriction
    {
        EntryEnterRestriction_NoRestriction, //0x0000
        EntryEnterRestriction_OnlyPrimaryEntry //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 03125
    // TypeInfo:  0x02C02D7C
    enum PlayerSpawnType
    {
        PlayerSpawnType_HumanPlayer, //0x0000
        PlayerSpawnType_AiPlayer, //0x0001
        PlayerSpawnType_Actor //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 03121
    // TypeInfo:  0x02C017F4
    enum CriteriaType
    {
        CriteriaType_IAR_InARound, //0x0000
        CriteriaType_IAR_InARoundResetIfValueNotChanged, //0x0001
        CriteriaType_IAS_InASpawn, //0x0002
        CriteriaType_IAS_InASpawnNotResetable, //0x0003
        CriteriaType_IAS_InASpawnWithoutTakingDamage, //0x0004
        CriteriaType_GLOBAL_AllTimeTotal, //0x0005
        CriteriaType_GLOBAL_HighestValue, //0x0006
        CriteriaType_GLOBAL_HighestValueAlways, //0x0007
        CriteriaType_GLOBAL_HighestValueInASpawn, //0x0008
        CriteriaType_GLOBAL_LowestValueAlways, //0x0009
        CriteriaType_IfNotTrue, //0x000A
        CriteriaType_IAD_InADeathStreak, //0x000B
        CriteriaType_GLOBAL_HighestValuePersistent, //0x000C
        CriteriaType_LAST_ITEM //0x000D
    };

    ////////////////////////////////////////
    // RuntimeId: 03119
    // TypeInfo:  0x02C017E4
    enum StatEvent
    {
        StatEvent_Kill, //0x0000
        StatEvent_Kill_In_X, //0x0001
        StatEvent_Kill_As_X, //0x0002
        StatEvent_KillWith_X_As_Y, //0x0003
        StatEvent_KillDistance, //0x0004
        StatEvent_KillOf_X_As_Y, //0x0005
        StatEvent_KillOf_X_In_Y, //0x0006
        StatEvent_TeamKill, //0x0007
        StatEvent_AvengerKill, //0x0008
        StatEvent_SaviorKill, //0x0009
        StatEvent_DriverAssist, //0x000A
        StatEvent_TeamDriverAssist, //0x000B
        StatEvent_SquadDriverAssist, //0x000C
        StatEvent_PassengerAssist, //0x000D
        StatEvent_TeamPassengerAssist, //0x000E
        StatEvent_SquadPassengerAssist, //0x000F
        StatEvent_RoadKill, //0x0010
        StatEvent_Death, //0x0011
        StatEvent_DoubleKill, //0x0012
        StatEvent_TripleKill, //0x0013
        StatEvent_MultiKill, //0x0014
        StatEvent_PostMortemKill, //0x0015
        StatEvent_KillStreak, //0x0016
        StatEvent_KillStreakX, //0x0017
        StatEvent_CancelKillStreak, //0x0018
        StatEvent_Comeback, //0x0019
        StatEvent_SquadWipe, //0x001A
        StatEvent_NemesisKill, //0x001B
        StatEvent_NemesisKillX, //0x001C
        StatEvent_CancelNemesisKill, //0x001D
        StatEvent_SecondsAs_X, //0x001E
        StatEvent_SecondsWith_X, //0x001F
        StatEvent_SecondsIn_X, //0x0020
        StatEvent_SecondsOn_X, //0x0021
        StatEvent_Destroy_X, //0x0022
        StatEvent_Destroy_X_As_Y, //0x0023
        StatEvent_Destroy_X_In_Y, //0x0024
        StatEvent_DestroyWith_X_As_Y, //0x0025
        StatEvent_Destroy_X_With_Y, //0x0026
        StatEvent_DestroyExplosives, //0x0027
        StatEvent_Disabled_X, //0x0028
        StatEvent_KillOfWith_X_As_Y, //0x0029
        StatEvent_TaggedDamageAssist_X, //0x002A
        StatEvent_Tagged_X, //0x002B
        StatEvent_SuppressWith_X_As_Y, //0x002C
        StatEvent_Level_X_Completed_Y_Difficulty, //0x002D
        StatEvent_Level_X_Completed_Y_Difficulty_WithTimeValue, //0x002E
        StatEvent_CleanLevel_X_Completed, //0x002F
        StatEvent_RoundOverOutcome_X_On_Y, //0x0030
        StatEvent_Rounds, //0x0031
        StatEvent_Headshot_X_With_Y, //0x0032
        StatEvent_HeadshotAs_X_With_Y, //0x0033
        StatEvent_HeadshotDistance_With_X, //0x0034
        StatEvent_SelfHeal, //0x0035
        StatEvent_BestSquad, //0x0036
        StatEvent_ShotsFired_With_X, //0x0037
        StatEvent_ShotsHit_With_X, //0x0038
        StatEvent_SpotAssist, //0x0039
        StatEvent_SpotAssistFrom_X, //0x003A
        StatEvent_SuppressionAssist, //0x003B
        StatEvent_Heal, //0x003C
        StatEvent_TeamHeal, //0x003D
        StatEvent_SquadHeal, //0x003E
        StatEvent_Revive, //0x003F
        StatEvent_TeamRevive, //0x0040
        StatEvent_SquadRevive, //0x0041
        StatEvent_ReviveAccepted, //0x0042
        StatEvent_TeamReviveAccepted, //0x0043
        StatEvent_SquadReviveAccepted, //0x0044
        StatEvent_ManDownRevive, //0x0045
        StatEvent_Resupply, //0x0046
        StatEvent_TeamResupply, //0x0047
        StatEvent_SquadResupply, //0x0048
        StatEvent_KillAssist, //0x0049
        StatEvent_Repair, //0x004A
        StatEvent_TeamRepair, //0x004B
        StatEvent_SquadRepair, //0x004C
        StatEvent_VehicleDestroyAssist, //0x004D
        StatEvent_TreeDestroyed, //0x004E
        StatEvent_WallDestroyed, //0x004F
        StatEvent_CaptureCapturePoint, //0x0050
        StatEvent_CaptureCapturePointAssist, //0x0051
        StatEvent_NeutralizeCapturePoint, //0x0052
        StatEvent_NeutralizeCapturePointAssist, //0x0053
        StatEvent_CapturePointDefense, //0x0054
        StatEvent_CapturePointAttack, //0x0055
        StatEvent_CrateArmed, //0x0056
        StatEvent_CrateDisarmed, //0x0057
        StatEvent_CrateDestroyed, //0x0058
        StatEvent_CrateDefendKill, //0x0059
        StatEvent_CrateAttackKill, //0x005A
        StatEvent_SquadDefendOrder, //0x005B
        StatEvent_SquadAttackOrder, //0x005C
        StatEvent_SquadLeaderDefendOrder, //0x005D
        StatEvent_SquadLeaderAttackOrder, //0x005E
        StatEvent_RankUp, //0x005F
        StatEvent_UniqueAward, //0x0060
        StatEvent_DogTag, //0x0061
        StatEvent_DogTagSavior, //0x0062
        StatEvent_SquadSpawn, //0x0063
        StatEvent_SquadSpawnOnRemoteVehicle_X, //0x0064
        StatEvent_Suicide, //0x0065
        StatEvent_GainAward_X, //0x0066
        StatEvent_PlayerScoreboardPosition_X, //0x0067
        StatEvent_GainedHighestRank, //0x0068
        StatEvent_GainedHighestUnlockInBucket_X, //0x0069
        StatEvent_Misc_X_and_Y, //0x006A
        StatEvent_TotalScoreOn_X_Difficulty_Y, //0x006B
        StatEvent_Invalid, //0x006C
        StatEvent_Damage, //0x006D
        StatEvent_PlayerScoreboardPosition_X_On_Y, //0x006E
        StatEvent_CaptureCapturePointDOM, //0x006F
        StatEvent_CaptureCapturePointAssistDOM, //0x0070
        StatEvent_NeutralizeCapturePointDOM, //0x0071
        StatEvent_NeutralizeCapturePointAssistDOM, //0x0072
        StatEvent_Kill_With_X_In_Y, //0x0073
        StatEvent_DoubleKill_With_X_In_Y, //0x0074
        StatEvent_ShotFired_With_X, //0x0075
        StatEvent_SpotAssistFor_X, //0x0076
        StatEvent_Misc_X_In_Y, //0x0077
        StatEvent_CTF_Capture, //0x0078
        StatEvent_CTF_CaptureStreak, //0x0079
        StatEvent_CTF_CaptureAssist, //0x007A
        StatEvent_CTF_PickUp, //0x007B
        StatEvent_CTF_PickUpAssist, //0x007C
        StatEvent_CTF_Return, //0x007D
        StatEvent_CTF_CarrierAssist, //0x007E
        StatEvent_CTF_CarrierAssistStreak, //0x007F
        StatEvent_CTF_FlagTransporter_In_X, //0x0080
        StatEvent_CTF_CarrierSavior, //0x0081
        StatEvent_CTF_CarrierSuppression, //0x0082
        StatEvent_CTF_CarrierKiller, //0x0083
        StatEvent_CTF_CarrierKillAssist, //0x0084
        StatEvent_CTF_CarrierHeal, //0x0085
        StatEvent_BadLuck, //0x0086
        StatEvent_Jump_In_X, //0x0087
        StatEvent_Kill_From_X, //0x0088
        StatEvent_ParadropDestroy_X, //0x0089
        StatEvent_ParadropKill, //0x008A
        StatEvent_FreefallDeath, //0x008B
        StatEvent_PassengerKill_In_X, //0x008C
        StatEvent_TakeDogTagAssassin, //0x008D
        StatEvent_Spawn_In_X, //0x008E
        StatEvent_RoadKill_With_X, //0x008F
        StatEvent_LAST_ITEM //0x0090
    };

    ////////////////////////////////////////
    // RuntimeId: 03117
    // TypeInfo:  0x02C017D4
    enum IKEffectorEnum
    {
        IKLeftHand, //0x0000
        IKRightHand //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 03115
    // TypeInfo:  0x02C017C4
    enum RotationAxis
    {
        raX, //0x0000
        raY, //0x0001
        raZ //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 03113
    // TypeInfo:  0x02C017B4
    enum WheelPhysicsType
    {
        wptNormal, //0x0000
        wptFollow, //0x0001
        wptStatic //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 03111
    // TypeInfo:  0x02C017A4
    enum EntryClass
    {
        ecPrimary, //0x0000
        ecSecondary //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 03071
    // TypeInfo:  0x02C01494
    enum CustomizationConstants
    {
        MaxVisualSockets, //0x0000
        MaxAttachedSockets //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 03045
    // TypeInfo:  0x02C0131C
    enum RotationChannel
    {
        RotationChannel_Pitch, //0x0000
        RotationChannel_Yaw, //0x0001
        RotationChannel_Roll //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 03021
    // TypeInfo:  0x02C01144
    enum RotationAxisEnum
    {
        RALeft, //0x0000
        RAUp, //0x0001
        RADof //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 03003
    // TypeInfo:  0x02C00FF0
    enum WeaponClassification
    {
        WCNone, //0x0000
        WCPrimary, //0x0001
        WCSecondary //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 02991
    // TypeInfo:  0x02C00F3C
    enum HitReactionType
    {
        HRT_Body, //0x0000
        HRT_Head, //0x0001
        HRT_RightArm, //0x0002
        HRT_LeftArm, //0x0003
        HRT_RightLeg, //0x0004
        HRT_LeftLeg, //0x0005
        HRT_Count //0x0006
    };

    ////////////////////////////////////////
    // RuntimeId: 02975
    // TypeInfo:  0x02C00E68
    enum GameplayBones
    {
        GameplayBones_UndefinedBone, //0x0000
        GameplayBones_HeadBone, //0x0001
        GameplayBones_AimBone, //0x0002
        GameplayBones_RootBone, //0x0003
        GameplayBones_WeaponBone, //0x0004
        GameplayBones_ConnectBone, //0x0005
        GameplayBones_RootMeshBone, //0x0006
        GameplayBones_MagazineBone, //0x0007
        GameplayBones_Count //0x0008
    };

    ////////////////////////////////////////
    // RuntimeId: 02963
    // TypeInfo:  0x02C00DB4
    enum AntEntryIdEnum
    {
        AntEntryIdEnum_Humvee_Driver, //0x0000
        AntEntryIdEnum_Humvee_Gunner, //0x0001
        AntEntryIdEnum_Humvee_PassengerRearLeft, //0x0002
        AntEntryIdEnum_Humvee_PassengerRearRight, //0x0003
        AntEntryIdEnum_F16_Pilot, //0x0004
        AntEntryIdEnum_SU37_Pilot, //0x0005
        AntEntryIdEnum_AH6_Pilot, //0x0006
        AntEntryIdEnum_AH64_Pilot, //0x0007
        AntEntryIdEnum_AH64_Gunner, //0x0008
        AntEntryIdEnum_Vodnik_Driver, //0x0009
        AntEntryIdEnum_Vodnik_Gunner, //0x000A
        AntEntryIdEnum_Humvee_PassengerFrontRight, //0x000B
        AntEntryIdEnum_Vodnik_PassengerFrontRight, //0x000C
        AntEntryIdEnum_Vodnik_PassengerRearRight, //0x000D
        AntEntryIdEnum_Vodnik_PassengerRearLeft, //0x000E
        AntEntryIdEnum_VadsGunner, //0x000F
        AntEntryIdEnum_F18F_Pilot, //0x0010
        AntEntryIdEnum_F18F_Navigator, //0x0011
        AntEntryIdEnum_M1A2_Gunner, //0x0012
        AntEntryIdEnum_M1A2_Driver //0x0013
    };

    ////////////////////////////////////////
    // RuntimeId: 02952
    // TypeInfo:  0x02C00D00
    enum EntrySpottingSettings
    {
        ESSDefault, //0x0000
        ESSSendAndReceive, //0x0001
        ESSReceive, //0x0002
        ESSCount //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 02948
    // TypeInfo:  0x02C00CE0
    enum EntrySeatType
    {
        EST_Driver, //0x0000
        EST_Gunner, //0x0001
        EST_Passenger //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 02942
    // TypeInfo:  0x02C00C78
    enum PartComponentConstants
    {
        MaxHealthStateCount //0x0000
    };

    ////////////////////////////////////////
    // RuntimeId: 02926
    // TypeInfo:  0x02C00B34
    enum ChatChannelType
    {
        CctSayAll, //0x0000
        CctTeam, //0x0001
        CctSquad, //0x0002
        CctSquadLeader, //0x0003
        CctAdmin, //0x0004
        CctSize //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 02920
    // TypeInfo:  0x02C00AE8
    enum PersonViewMode
    {
        PersonViewMode_FirstPerson, //0x0000
        PersonViewMode_ThirdPerson //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 02902
    // TypeInfo:  0x02C009B0
    enum TargetMode
    {
        TargetMode_FirstPerson, //0x0000
        TargetMode_ThirdPerson, //0x0001
        TargetMode_Index //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 02818
    // TypeInfo:  0x02BFEAB4
    enum PoseTransition
    {
        PTStandToProne, //0x0000
        PTStandToCrouch, //0x0001
        PTCrouchToStand, //0x0002
        PTCrouchToProne, //0x0003
        PTProneToStand, //0x0004
        PTProneToCrouch //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 02814
    // TypeInfo:  0x02BFEA78
    enum EntryInputActionEnum
    {
        EIAThrottle, //0x0000
        EIAStrafe, //0x0001
        EIABrake, //0x0002
        EIASwitchPrimaryInventory, //0x0003
        EIAYaw, //0x0004
        EIAPitch, //0x0005
        EIARoll, //0x0006
        EIAFire, //0x0007
        EIACameraPitch, //0x0008
        EIACameraYaw, //0x0009
        EIAFireCountermeasure, //0x000A
        EIAZoom, //0x000B
        EIAJump, //0x000C
        EIAChangePose, //0x000D
        EIAProne, //0x000E
        EIAReload, //0x000F
        EIASelectWeapon1, //0x0010
        EIASelectWeapon2, //0x0011
        EIASelectWeapon3, //0x0012
        EIASelectWeapon4, //0x0013
        EIASelectWeapon5, //0x0014
        EIASelectWeapon6, //0x0015
        EIASelectWeapon7, //0x0016
        EIASelectWeapon8, //0x0017
        EIASelectWeapon9, //0x0018
        EIASwitchPrimaryWeapon, //0x0019
        EIAGrenadeLauncher, //0x001A
        EIAStaticGadget, //0x001B
        EIADynamicGadget1, //0x001C
        EIADynamicGadget2, //0x001D
        EIAMeleeAttack, //0x001E
        EIAThrowGrenade, //0x001F
        EIASprint, //0x0020
        EIACrawlSpeed, //0x0021
        EIACycleFireMode, //0x0022
        EIAInteract, //0x0023
        EIAToggleParachute, //0x0024
        EIACycleRadioChannel, //0x0025
        EIAToggleCamera, //0x0026
        EIAScoreboardMenu, //0x0027
        EIAGearUp, //0x0028
        EIAGearUpOrToggleWeaponLight, //0x0029
        EIAGearDown, //0x002A
        EIAGearDownOrExitSupportedShooting, //0x002B
        EIAClutch, //0x002C
        EIAHandBrake, //0x002D
        EIAGiveOrder, //0x002E
        EIABreathControl, //0x002F
        EIAMapZoom, //0x0030
        EIAChangeVehicle, //0x0031
        EIAChangeEntry, //0x0032
        EIAChangeEntry1, //0x0033
        EIAChangeEntry2, //0x0034
        EIAChangeEntry3, //0x0035
        EIAChangeEntry4, //0x0036
        EIAChangeEntry5, //0x0037
        EIAChangeEntry6, //0x0038
        EIAChangeEntry7, //0x0039
        EIAChangeEntry8, //0x003A
        EIAThreeDimensionalMap, //0x003B
        EIAShowCommoRose, //0x003C
        EIAShowLeaderCommoRose, //0x003D
        EIAQuicktimeInteractDrag, //0x003E
        EIAQuicktimeFire, //0x003F
        EIAQuicktimeBlock, //0x0040
        EIAQuicktimeFastMelee, //0x0041
        EIAQuicktimeJumpClimb, //0x0042
        EIAQuicktimeCrouchDuck, //0x0043
        EIAUndefined, //0x0044
        EIANoInput //0x0045
    };

    ////////////////////////////////////////
    // RuntimeId: 02804
    // TypeInfo:  0x02BFE9B8
    enum InputActionMapSlot
    {
        InputActionMapSlot_Undefined, //0x0000
        InputActionMapSlot_Root1, //0x0001
        InputActionMapSlot_Root2, //0x0002
        InputActionMapSlot_Root3, //0x0003
        InputActionMapSlot_Root4, //0x0004
        InputActionMapSlot_Root5, //0x0005
        InputActionMapSlot_Root6, //0x0006
        InputActionMapSlot_Root7, //0x0007
        InputActionMapSlot_Root8, //0x0008
        InputActionMapSlot_Root9, //0x0009
        InputActionMapSlot_Root10, //0x000A
        InputActionMapSlot_Root11, //0x000B
        InputActionMapSlot_Root12, //0x000C
        InputActionMapSlot_Root13, //0x000D
        InputActionMapSlot_Root14, //0x000E
        InputActionMapSlot_Root15, //0x000F
        InputActionMapSlot_Root16, //0x0010
        InputActionMapSlot_Sticks1, //0x0011
        InputActionMapSlot_Sticks2, //0x0012
        InputActionMapSlot_Sticks3, //0x0013
        InputActionMapSlot_Sticks4, //0x0014
        InputActionMapSlot_Buttons1, //0x0015
        InputActionMapSlot_Buttons2, //0x0016
        InputActionMapSlot_Buttons3, //0x0017
        InputActionMapSlot_Buttons4, //0x0018
        InputActionMapSlot_Sticks1Buttons1, //0x0019
        InputActionMapSlot_Sticks1Buttons2, //0x001A
        InputActionMapSlot_Sticks1Buttons3, //0x001B
        InputActionMapSlot_Sticks1Buttons4, //0x001C
        InputActionMapSlot_Sticks2Buttons1, //0x001D
        InputActionMapSlot_Sticks2Buttons2, //0x001E
        InputActionMapSlot_Sticks2Buttons3, //0x001F
        InputActionMapSlot_Sticks2Buttons4, //0x0020
        InputActionMapSlot_Sticks3Buttons1, //0x0021
        InputActionMapSlot_Sticks3Buttons2, //0x0022
        InputActionMapSlot_Sticks3Buttons3, //0x0023
        InputActionMapSlot_Sticks3Buttons4, //0x0024
        InputActionMapSlot_Sticks4Buttons1, //0x0025
        InputActionMapSlot_Sticks4Buttons2, //0x0026
        InputActionMapSlot_Sticks4Buttons3, //0x0027
        InputActionMapSlot_Sticks4Buttons4, //0x0028
        InputActionMapSlot_Count //0x0029
    };

    ////////////////////////////////////////
    // RuntimeId: 02802
    // TypeInfo:  0x02BFE9A8
    enum InputActionMapPlatform
    {
        IAMPWin32, //0x0000
        IAMPXenon, //0x0001
        IAMPPs3, //0x0002
        IAMPAllPlatforms //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 02798
    // TypeInfo:  0x02BFE96C
    enum SpottingType
    {
        STGunner, //0x0000
        STSquadLeader, //0x0001
        STDefault, //0x0002
        STPersonal, //0x0003
        STSnapshot, //0x0004
        STSquadUav, //0x0005
        STTypesCount //0x0006
    };

    ////////////////////////////////////////
    // RuntimeId: 02788
    // TypeInfo:  0x02BFE8E4
    enum UIHudIcon
    {
        UIHudIcon_Unused, //0x0000
        UIHudIcon_LocalPlayer, //0x0001
        UIHudIcon_LocalDirection, //0x0002
        UIHudIcon_FriendlyPlayer, //0x0003
        UIHudIcon_EnemyPlayer, //0x0004
        UIHudIcon_NeutralPlayer, //0x0005
        UIHudIcon_SquadMember, //0x0006
        UIHudIcon_SquadLeader, //0x0007
        UIHudIcon_SquadLeaderTargeted, //0x0008
        UIHudIcon_Vehicle, //0x0009
        UIHudIcon_PrimaryObjective, //0x000A
        UIHudIcon_PrimaryObjectiveBlink, //0x000B
        UIHudIcon_SecondaryObjective, //0x000C
        UIHudIcon_AreaMapMarker, //0x000D
        UIHudIcon_ObjectiveDestroy, //0x000E
        UIHudIcon_ObjectiveScout, //0x000F
        UIHudIcon_ObjectiveDefend, //0x0010
        UIHudIcon_ObjectiveMoveTo, //0x0011
        UIHudIcon_ObjectiveAttack, //0x0012
        UIHudIcon_ObjectiveFollow, //0x0013
        UIHudIcon_ObjectiveGeneral, //0x0014
        UIHudICon_UAV, //0x0015
        UIHudIcon_AmmoCrate, //0x0016
        UIHudIcon_MedicBag, //0x0017
        UIHudIcon_C4, //0x0018
        UIHudIcon_ATMine, //0x0019
        UIHudIcon_StationaryWeapon, //0x001A
        UIHudIcon_North, //0x001B
        UIHudIcon_South, //0x001C
        UIHudIcon_West, //0x001D
        UIHudIcon_East, //0x001E
        UIHudIcon_NeutralFlag, //0x001F
        UIHudIcon_FriendlyFlag, //0x0020
        UIHudIcon_EnemyFlag, //0x0021
        UIHudIcon_FriendlyBase, //0x0022
        UIHudIcon_EnemyBase, //0x0023
        UIHudIcon_Team1Flag, //0x0024
        UIHudIcon_Team2Flag, //0x0025
        UIHudIcon_NeutralFlagLit, //0x0026
        UIHudIcon_FriendlyFlagLit, //0x0027
        UIHudIcon_EnemyFlagLit, //0x0028
        UIHudIcon_SelectableSpawnPoint, //0x0029
        UIHudIcon_SelectedSpawnPoint, //0x002A
        UIHudIcon_NonSelectableSpawnPoint, //0x002B
        UIHudIcon_FriendlyFlagUnderAttack, //0x002C
        UIHudIcon_EnemyFlagUnderAttack, //0x002D
        UIHudIcon_OrderAttack, //0x002E
        UIHudIcon_OrderDefend, //0x002F
        UIHudIcon_OrderAttackObserved, //0x0030
        UIHudIcon_OrderDefendObserved, //0x0031
        UIHudIcon_Boat, //0x0032
        UIHudIcon_Car, //0x0033
        UIHudIcon_Jeep, //0x0034
        UIHudIcon_HeliAttack, //0x0035
        UIHudIcon_HeliScout, //0x0036
        UIHudIcon_Tank, //0x0037
        UIHudIcon_TankIFV, //0x0038
        UIHudIcon_TankArty, //0x0039
        UIHudIcon_TankAA, //0x003A
        UIHudIcon_TankAT, //0x003B
        UIHudIcon_Jet, //0x003C
        UIHudIcon_JetBomber, //0x003D
        UIHudIcon_Stationary, //0x003E
        UIHudIcon_Strategic, //0x003F
        UIHudIcon_MotionRadarSweep, //0x0040
        UIHudIcon_NeedBackup, //0x0041
        UIHudIcon_NeedAmmo, //0x0042
        UIHudIcon_NeedMedic, //0x0043
        UIHudIcon_NeedPickup, //0x0044
        UIHudIcon_NeedRepair, //0x0045
        UIHudIcon_KitAssault, //0x0046
        UIHudIcon_KitDemolition, //0x0047
        UIHudIcon_KitRecon, //0x0048
        UIHudIcon_KitSpecialist, //0x0049
        UIHudIcon_KitSupport, //0x004A
        UIHudIcon_KitMedic, //0x004B
        UIHudIcon_KitEngineer, //0x004C
        UIHudIcon_KitPickupAssault, //0x004D
        UIHudIcon_KitPickupDemolition, //0x004E
        UIHudIcon_KitPickupRecon, //0x004F
        UIHudIcon_KitPickupSpecialist, //0x0050
        UIHudIcon_KitPickupSupport, //0x0051
        UIHudIcon_KitPickupMedic, //0x0052
        UIHudIcon_KitPickupEngineer, //0x0053
        UIHudIcon_Pickup, //0x0054
        UIHudIcon_TaggedVehicle, //0x0055
        UIHudIcon_LaserPaintedVehicle, //0x0056
        UIHudIcon_HeliTargetEnemy, //0x0057
        UIHudIcon_HeliTargetFriendly, //0x0058
        UIHudIcon_ArtilleryTarget, //0x0059
        UIHudIcon_NeutralFlagAttacker, //0x005A
        UIHudIcon_FriendlyFlagAttacker, //0x005B
        UIHudIcon_EnemyFlagAttacker, //0x005C
        UIHudIcon_LaserTarget, //0x005D
        UIHudIcon_ObjectiveAttacker, //0x005E
        UIHudIcon_ObjectiveDefender, //0x005F
        UIHudIcon_HealthbarBackground, //0x0060
        UIHudIcon_Healthbar, //0x0061
        UIHudIcon_RadarSweepComponent, //0x0062
        UIHudIcon_Blank, //0x0063
        UIHudIcon_LocalPlayerBigIcon, //0x0064
        UIHudIcon_LocalPlayerOutOfMap, //0x0065
        UIHudIcon_PrimaryObjectiveLarge, //0x0066
        UIHudIcon_TargetUnlocked, //0x0067
        UIHudIcon_TargetLocked, //0x0068
        UIHudIcon_TargetLocking, //0x0069
        UIHudIcon_ArtilleryStrikeNametag, //0x006A
        UIHudIcon_ArtilleryStrikeMinimap, //0x006B
        UIHudIcon_CapturePointContested, //0x006C
        UIHudIcon_CapturePointDefended, //0x006D
        UIHudIcon_RoundBar, //0x006E
        UIHudIcon_RoundBarBg, //0x006F
        UIHudIcon_RoundBarBgPlate, //0x0070
        UIHudIcon_SnapOvalArrow, //0x0071
        UIHudIcon_SquadleaderBg, //0x0072
        UIHudIcon_VehicleBg, //0x0073
        UIHudIcon_NonTakeableControlPoint, //0x0074
        UIHudIcon_SpottedPosition, //0x0075
        UIHudIcon_Grenade, //0x0076
        UIHudIcon_Revive, //0x0077
        UIHudIcon_Repair, //0x0078
        UIHudIcon_Interact, //0x0079
        UIHudIcon_Voip, //0x007A
        UIHudIcon_Claymore, //0x007B
        UIHudIcon_EodBot, //0x007C
        UIHudIcon_Explosive, //0x007D
        UIHudIcon_LaserDesignator, //0x007E
        UIHudIcon_Mav, //0x007F
        UIHudIcon_Mortar, //0x0080
        UIHudIcon_RadioBeacon, //0x0081
        UIHudIcon_Ugs, //0x0082
        UIHudIcon_PercetageBarMiddle, //0x0083
        UIHudIcon_PercetageBarEdge, //0x0084
        UIHudIcon_PercentageBarBackground, //0x0085
        UIHudIcon_TankLC, //0x0086
        UIHudIcon_HeliTrans, //0x0087
        UIHudIcon_StaticAT, //0x0088
        UIHudIcon_StaticAA, //0x0089
        UIHudIcon_SprintBoost, //0x008A
        UIHudIcon_AmmoBoost, //0x008B
        UIHudIcon_ExplosiveBoost, //0x008C
        UIHudIcon_ExplosiveResistBoost, //0x008D
        UIHudIcon_SuppressionBoost, //0x008E
        UIHudIcon_SuppressionResistBoost, //0x008F
        UIHudIcon_GrenadeBoost, //0x0090
        UIHudIcon_HealSpeedBoost, //0x0091
        UIHudIcon_NeedAmmoHighlight, //0x0092
        UIHudIcon_NeedMedicHighlight, //0x0093
        UIHudIcon_NeedRepairHighlight, //0x0094
        UIHudIcon_NeedPickupHighlight, //0x0095
        UIHudIcon_PlayerDead, //0x0096
        UIHudIcon_Player, //0x0097
        UIHudIcon_Flag, //0x0098
        UIHudIcon_Base, //0x0099
        UIHudIcon_TankDestroyer, //0x009A
        UIHudIcon_Gunship, //0x009B
        UIHudIcon_ATV, //0x009C
        UIHudIcon_WeaponPickupTier1, //0x009D
        UIHudIcon_WeaponPickupTier2, //0x009E
        UIHudIcon_WeaponPickupTier3, //0x009F
        UIHudIcon_VehicleParachuted, //0x00A0
        UIHudIcon_CTFFlagFriendlyHome, //0x00A1
        UIHudIcon_CTFFlagFriendlyPicked, //0x00A2
        UIHudIcon_CTFFlagFriendlyDropped, //0x00A3
        UIHudIcon_CTFFlagEnemyHome, //0x00A4
        UIHudIcon_CTFFlagEnemyPicked, //0x00A5
        UIHudIcon_CTFFlagEnemyDropped, //0x00A6
        UIHudIcon_FriendlyRedFlag, //0x00A7
        UIHudIcon_EnemyGreenFlag, //0x00A8
        UIHudIcon_DirtBike, //0x00A9
        UIHudIcon_Count //0x00AA
    };

    ////////////////////////////////////////
    // RuntimeId: 02786
    // TypeInfo:  0x02BFE8D4
    enum UIPartIdentifier
    {
        HCI_None, //0x0000
        HCI_RangeMeter, //0x0001
        HCI_PredictedSight, //0x0002
        HCI_VelocityVector, //0x0003
        HCI_RadioAltitude //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 02784
    // TypeInfo:  0x02BFE8C4
    enum CollisionMethodEnum
    {
        CMESimple, //0x0000
        CMEDetailed //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 02778
    // TypeInfo:  0x02BFE878
    enum AIHitReactionAnimationType
    {
        AIHitReactionAnimationType_1, //0x0000
        AIHitReactionAnimationType_2, //0x0001
        AIHitReactionAnimationType_3, //0x0002
        AIHitReactionAnimationType_4, //0x0003
        AIHitReactionAnimationType_5, //0x0004
        AIHitReactionAnimationType_Count //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 02768
    // TypeInfo:  0x02BFE7D4
    enum GameSplineType
    {
        GameSplineType_Generic, //0x0000
        GameSplineType_Pipe, //0x0001
        GameSplineType_Zipline, //0x0002
        GameSplineType_Swing //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 02750
    // TypeInfo:  0x02BFE664
    enum LogFileCollisionMode
    {
        LFCM_Overwrite, //0x0000
        LFCM_Rotate, //0x0001
        LFCM_TimeStamp //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 02748
    // TypeInfo:  0x02BFE654
    enum SKU
    {
        WW, //0x0000
        EU, //0x0001
        US, //0x0002
        JPN, //0x0003
        ROA, //0x0004
        RU, //0x0005
        EN //0x0006
    };

    ////////////////////////////////////////
    // RuntimeId: 02746
    // TypeInfo:  0x02BFE644
    enum GamePlatform
    {
        GamePlatform_Ps3, //0x0000
        GamePlatform_Win32, //0x0001
        GamePlatform_Xenon, //0x0002
        GamePlatform_Any, //0x0003
        GamePlatform_Invalid, //0x0004
        GamePlatform_Count //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 02728
    // TypeInfo:  0x02BFE4F0
    enum DamageIndicationType
    {
        DamageIndicationType_GiverPlayer, //0x0000
        DamageIndicationType_ExplosionCenter //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 02714
    // TypeInfo:  0x02BFE3F0
    enum ExitToMenuReason
    {
        ExitToMenuReason_None, //0x0000
        ExitToMenuReason_UserDisconnected, //0x0001
        ExitToMenuReason_DisconnectedFromServer, //0x0002
        ExitToMenuReason_UnableToConnectToServer, //0x0003
        ExitToMenuReason_ClientDisconnected, //0x0004
        ExitToMenuReason_FirstPartyConnectionFailed, //0x0005
        ExitToMenuReason_MissingContent, //0x0006
        ExitToMenuReason_TeamKills, //0x0007
        ExitToMenuReason_KickedByAdmin, //0x0008
        ExitToMenuReason_Banned, //0x0009
        ExitToMenuReason_InteractivityTimeout, //0x000A
        ExitToMenuReason_KickedOutServerFull, //0x000B
        ExitToMenuReason_ESportsMatchStarting, //0x000C
        ExitToMenuReason_NotInESportsRosters, //0x000D
        ExitToMenuReason_ESportsMatchEnding, //0x000E
        ExitToMenuReason_VirtualServerExpired, //0x000F
        ExitToMenuReason_VirtualServerRecreate, //0x0010
        ExitToMenuReason_ESportsTeamFull, //0x0011
        ExitToMenuReason_ESportsMatchAborted, //0x0012
        ExitToMenuReason_ESportsMatchWalkover, //0x0013
        ExitToMenuReason_ESportsMatchWarmupTimedOut, //0x0014
        ExitToMenuReason_PremiumStatusMismatch //0x0015
    };

    ////////////////////////////////////////
    // RuntimeId: 02705
    // TypeInfo:  0x02BFE368
    enum EntitlementType
    {
        EntitlementType_Ignored, //0x0000
        EntitlementType_Write, //0x0001
        EntitlementType_Read //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 02699
    // TypeInfo:  0x02BFE300
    enum DynamicAvoidanceMode
    {
        DynamicAvoidanceMode_Never, //0x0000
        DynamicAvoidanceMode_WhenScripted, //0x0001
        DynamicAvoidanceMode_WhenNotScripted, //0x0002
        DynamicAvoidanceMode_Always //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 02683
    // TypeInfo:  0x02BFE1D8
    enum Difficulty
    {
        Difficulty_Easy, //0x0000
        Difficulty_Medium, //0x0001
        Difficulty_Hard, //0x0002
        Difficulty_Hardcore, //0x0003
        Difficulty_None //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 02677
    // TypeInfo:  0x02BFE18C
    enum CompareOperation
    {
        Equal, //0x0000
        NotEqual, //0x0001
        LesserEqual, //0x0002
        GreaterEqual, //0x0003
        Lesser, //0x0004
        Greater //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 02675
    // TypeInfo:  0x02BFE17C
    enum VoiceChannel
    {
        VoiceChannel_Off, //0x0000
        VoiceChannel_Team, //0x0001
        VoiceChannel_Squad, //0x0002
        VoiceChannel_Loopback, //0x0003
        VoiceChannel_Count //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 02663
    // TypeInfo:  0x02BFE090
    enum SquadAction
    {
        SquadAction_PlayerJoined, //0x0000
        SquadAction_PlayerLeft, //0x0001
        SquadAction_PlayerWasKicked //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 02661
    // TypeInfo:  0x02BFE080
    enum SquadId
    {
        SquadNone, //0x0000
        Squad1, //0x0001
        Squad2, //0x0002
        Squad3, //0x0003
        Squad4, //0x0004
        Squad5, //0x0005
        Squad6, //0x0006
        Squad7, //0x0007
        Squad8, //0x0008
        Squad9, //0x0009
        Squad10, //0x000A
        Squad11, //0x000B
        Squad12, //0x000C
        Squad13, //0x000D
        Squad14, //0x000E
        Squad15, //0x000F
        Squad16, //0x0010
        Squad17, //0x0011
        Squad18, //0x0012
        Squad19, //0x0013
        Squad20, //0x0014
        Squad21, //0x0015
        Squad22, //0x0016
        Squad23, //0x0017
        Squad24, //0x0018
        Squad25, //0x0019
        Squad26, //0x001A
        Squad27, //0x001B
        Squad28, //0x001C
        Squad29, //0x001D
        Squad30, //0x001E
        Squad31, //0x001F
        Squad32, //0x0020
        SquadIdCount //0x0021
    };

    ////////////////////////////////////////
    // RuntimeId: 02659
    // TypeInfo:  0x02BFE070
    enum FactionId
    {
        FactionNeutral, //0x0000
        FactionUS, //0x0001
        FactionRUS, //0x0002
        FactionMEC, //0x0003
        FactionIdCount, //0x0004
        FactionInvalid //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 02657
    // TypeInfo:  0x02BFE060
    enum TeamId
    {
        TeamNeutral, //0x0000
        Team1, //0x0001
        Team2, //0x0002
        Team3, //0x0003
        Team4, //0x0004
        Team5, //0x0005
        Team6, //0x0006
        Team7, //0x0007
        Team8, //0x0008
        Team9, //0x0009
        Team10, //0x000A
        Team11, //0x000B
        Team12, //0x000C
        Team13, //0x000D
        Team14, //0x000E
        Team15, //0x000F
        Team16, //0x0010
        TeamIdCount //0x0011
    };

    ////////////////////////////////////////
    // RuntimeId: 02653
    // TypeInfo:  0x02BFE024
    enum VehicleCategory
    {
        VehicleCategory_Land, //0x0000
        VehicleCategory_Air //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 02611
    // TypeInfo:  0x02BFDCE8
    enum SocketType
    {
        SocketType_Undefined, //0x0000
        SocketType_Gameplay, //0x0001
        SocketType_Visual //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 02601
    // TypeInfo:  0x02BFDC28
    enum SocketPhysics
    {
        SocketPhysics_Default, //0x0000
        SocketPhysics_FakePhysics, //0x0001
        SocketPhysics_Droppable //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 02563
    // TypeInfo:  0x02BFC000
    enum WeaponSwitchingEnum
    {
        wsSlot0, //0x0000
        wsSlot1, //0x0001
        wsSlot2, //0x0002
        wsSlot3, //0x0003
        wsSlot4, //0x0004
        wsSlot5, //0x0005
        wsSlot6, //0x0006
        wsSlot7, //0x0007
        wsSlot8, //0x0008
        wsSlot9, //0x0009
        wsMaxSlots, //0x000A
        wsLastPrimary, //0x000B
        wsOtherPrimary, //0x000C
        wsNotUsed, //0x000D
        wsMeleeWeapon //0x000E
    };

    ////////////////////////////////////////
    // RuntimeId: 02561
    // TypeInfo:  0x02BFBFF0
    enum SoldierBuffId
    {
        SBImprovedBodyArmor, //0x0000
        SBCount //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 02559
    // TypeInfo:  0x02BFBFE0
    enum PlayerRole
    {
        PRSquadLeader, //0x0000
        PRSquadMember, //0x0001
        PRUnknown //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 02540
    // TypeInfo:  0x02BFBE88
    enum DeathAnimationOperation
    {
        DALessThan, //0x0000
        DAGreaterThan, //0x0001
        DAEqualTo //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 02510
    // TypeInfo:  0x02BFBC9C
    enum ModelAnimationTransformType
    {
        ModelAnimationTransformType_World, //0x0000
        ModelAnimationTransformType_Model, //0x0001
        ModelAnimationTransformType_Local //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 02508
    // TypeInfo:  0x02BFBC8C
    enum ModelAnimationUpdateOrder
    {
        ModelAnimationUpdateOrder_PostAnimation, //0x0000
        ModelAnimationUpdateOrder_PreInput, //0x0001
        ModelAnimationUpdateOrder_PreFrame //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 02468
    // TypeInfo:  0x02BFB938
    enum ScoringVisibilityType
    {
        ScoringVisibilityType_Major, //0x0000
        ScoringVisibilityType_Minor, //0x0001
        ScoringVisibilityType_Hidden //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 02462
    // TypeInfo:  0x02BFB8EC
    enum ScoringBucketType
    {
        ScoringBucketType_Disabled, //0x0000
        ScoringBucketType_Normal, //0x0001
        ScoringBucketType_Vehicle, //0x0002
        ScoringBucketType_Kit, //0x0003
        ScoringBucketType_Count //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 02460
    // TypeInfo:  0x02BFB8DC
    enum ScoringBucket
    {
        ScoringBucket_General, //0x0000
        ScoringBucket_Teamwork, //0x0001
        ScoringBucket_Bonus, //0x0002
        ScoringBucket_Squad, //0x0003
        ScoringBucket_Objective, //0x0004
        ScoringBucket_Award, //0x0005
        ScoringBucket_VehicleMBT, //0x0006
        ScoringBucket_VehicleIFV, //0x0007
        ScoringBucket_VehicleAA, //0x0008
        ScoringBucket_VehicleAttackHeli, //0x0009
        ScoringBucket_VehicleScoutHeli, //0x000A
        ScoringBucket_VehicleJet, //0x000B
        ScoringBucket_KitAssault, //0x000C
        ScoringBucket_KitEngineer, //0x000D
        ScoringBucket_KitRecon, //0x000E
        ScoringBucket_KitSupport, //0x000F
        ScoringBucket_SinglePlayer, //0x0010
        ScoringBucket_Cooperation, //0x0011
        ScoringBucket_CoopIndivid, //0x0012
        ScoringBucket_Unlock, //0x0013
        ScoringBucket_SpecialKit, //0x0014
        ScoringBucket_VehicleLBT, //0x0015
        ScoringBucket_VehicleArtillery, //0x0016
        ScoringBucket_LastScoringBucketItem //0x0017
    };

    ////////////////////////////////////////
    // RuntimeId: 02418
    // TypeInfo:  0x02BFB5E8
    enum ProfileOptionsType
    {
        GstAudio, //0x0000
        GstRender, //0x0001
        GstInput, //0x0002
        GstAI, //0x0003
        GstGameplay, //0x0004
        GstPlayerProfile, //0x0005
        GstPersistence, //0x0006
        GstBinary, //0x0007
        GstKeyBinding, //0x0008
        GstCount //0x0009
    };

    ////////////////////////////////////////
    // RuntimeId: 02392
    // TypeInfo:  0x02BFB41C
    enum PickupPlayerEnum
    {
        PickupPlayerEnum_None, //0x0000
        PickupPlayerEnum_Both, //0x0001
        PickupPlayerEnum_HumanOnly, //0x0002
        PickupPlayerEnum_AIOnly //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 02372
    // TypeInfo:  0x02BFB2B8
    enum RichPresencePropertyType
    {
        RichPresencePropertyType_Float, //0x0000
        RichPresencePropertyType_Integer, //0x0001
        RichPresencePropertyType_LongFloat, //0x0002
        RichPresencePropertyType_LongInteger //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 02356
    // TypeInfo:  0x02BFB1AC
    enum PersistenceGameType
    {
        PersistenceGameType_Singleplayer, //0x0000
        PersistenceGameType_Cooperative, //0x0001
        PersistenceGameType_Multiplayer, //0x0002
        PersistenceGameType_Count //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 02352
    // TypeInfo:  0x02BFB170
    enum StatPeriod
    {
        StatPeriod_AllTime, //0x0000
        StatPeriod_Monthly, //0x0001
        StatPeriod_Weekly, //0x0002
        StatPeriod_Daily, //0x0003
        StatPeriod_Size, //0x0004
        StatPeriod_Error //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 02336
    // TypeInfo:  0x02BFB064
    enum CustomReportValueKind
    {
        CustomReportValueKind_ScoreMultiplier, //0x0000
        CustomReportValueKind_GunMasterLevel //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 02334
    // TypeInfo:  0x02BFB054
    enum PersistentValueDataKind
    {
        PersistentValueDataKind_Raw, //0x0000
        PersistentValueDataKind_Seconds, //0x0001
        PersistentValueDataKind_Minutes, //0x0002
        PersistentValueDataKind_Hours, //0x0003
        PersistentValueDataKind_Percent //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 02332
    // TypeInfo:  0x02BFB044
    enum PersistentValueDataType
    {
        PersistentValueDataType_Decimal, //0x0000
        PersistentValueDataType_Integer, //0x0001
        PersistentValueDataType_String, //0x0002
        PersistentValueDataType_HighPrecisionDecimal //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 02330
    // TypeInfo:  0x02BFB034
    enum PersistentValueType
    {
        PersistentValueType_Set, //0x0000
        PersistentValueType_High, //0x0001
        PersistentValueType_Low, //0x0002
        PersistentValueType_Increment, //0x0003
        PersistentValueType_Decrement //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 02328
    // TypeInfo:  0x02BFB024
    enum ConsumableGroup
    {
        ConsumableGroup_0, //0x0000
        ConsumableGroup_1, //0x0001
        ConsumableGroup_2, //0x0002
        ConsumableGroup_3, //0x0003
        ConsumableGroup_4, //0x0004
        ConsumableGroup_5, //0x0005
        ConsumableGroup_6, //0x0006
        ConsumableGroup_7, //0x0007
        ConsumableGroup_Count //0x0008
    };

    ////////////////////////////////////////
    // RuntimeId: 02324
    // TypeInfo:  0x02BFAFE8
    enum PathfindingReplayMode
    {
        PathfindingReplayMode_Disabled, //0x0000
        PathfindingReplayMode_Binary, //0x0001
        PathfindingReplayMode_Text //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 02320
    // TypeInfo:  0x02BFAFAC
    enum PathfindingStreamAndConnect
    {
        PathfindingStreamAndConnect_Grid, //0x0000
        PathfindingStreamAndConnect_Manual //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 02312
    // TypeInfo:  0x02BFAF34
    enum PathfindingRepresentation
    {
        PathfindingRepresentation_Ignore, //0x0000
        PathfindingRepresentation_WalkableMesh, //0x0001
        PathfindingRepresentation_ObstacleMesh, //0x0002
        PathfindingRepresentation_ObstacleMeshAndThinSideLink, //0x0003
        PathfindingRepresentation_BoundingBoxExclusion //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 02306
    // TypeInfo:  0x02BFAECC
    enum BackendType
    {
        Backend_Lan, //0x0000
        Backend_Blaze, //0x0001
        Backend_Peer, //0x0002
        Backend_Local, //0x0003
        Backend_Count_ //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 02292
    // TypeInfo:  0x02BFAE40
    enum OnlineEnvironment
    {
        OnlineEnvironment_Development, //0x0000
        OnlineEnvironment_Test, //0x0001
        OnlineEnvironment_Certification, //0x0002
        OnlineEnvironment_Production, //0x0003
        OnlineEnvironment_Count //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 02256
    // TypeInfo:  0x02BF8F80
    enum AdministrationRestrictionLevel
    {
        AdministrationRestrictionLevel_Zero, //0x0000
        AdministrationRestrictionLevel_One, //0x0001
        AdministrationRestrictionLevel_Two, //0x0002
        AdministrationRestrictionLevel_Three, //0x0003
        AdministrationRestrictionLevel_Count //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 02254
    // TypeInfo:  0x02BF8F70
    enum AdministrationEventType
    {
        AdministrationEventType_Add, //0x0000
        AdministrationEventType_Remove, //0x0001
        AdministrationEventType_Clear, //0x0002
        AdministrationEventType_List, //0x0003
        AdministrationEventType_Load, //0x0004
        AdministrationEventType_Save //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 02228
    // TypeInfo:  0x02BF6E40
    enum CoopPlayerEvent
    {
        CoopPlayerEvent_Connecting, //0x0000
        CoopPlayerEvent_Connected, //0x0001
        CoopPlayerEvent_Disconnected, //0x0002
        CoopPlayerEvent_Stats, //0x0003
        CoopPlayerEvent_StatsFailed //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 02218
    // TypeInfo:  0x02BF6DB0
    enum PlayerKilledWeaponType
    {
        PlayerKilledWeaponType_Unknown, //0x0000
        PlayerKilledWeaponType_HandheldWeapon, //0x0001
        PlayerKilledWeaponType_Vehicle, //0x0002
        PlayerKilledWeaponType_Count //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 02196
    // TypeInfo:  0x02BF6C60
    enum GunMasterNotificationType
    {
        GunMasterNotificationType_LevelUp, //0x0000
        GunMasterNotificationType_Demoted, //0x0001
        GunMasterNotificationType_ReachedLevel, //0x0002
        GunMasterNotificationType_TookLead, //0x0003
        GunMasterNotificationType_LostLead //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 02185
    // TypeInfo:  0x02BF6BE0
    enum ShooterInfo
    {
        ShooterInfoInVehicle, //0x0000
        ShooterInfoInLandVehicle, //0x0001
        ShooterInfoInAerialVehicle, //0x0002
        ShooterInfoDrivingVehicle, //0x0003
        ShooterInfoInStationaryWeapon, //0x0004
        ShooterInfoInLightArmoredVehicle, //0x0005
        ShooterInfoInMediumArmoredVehicle, //0x0006
        ShooterInfoInHeavyArmoredVehicle, //0x0007
        ShooterInfoInBoat, //0x0008
        ShooterInfoUsedGrenade, //0x0009
        ShooterInfoUsedArtilleryStrike, //0x000A
        ShooterInfoUsedLaserDesignator, //0x000B
        ShooterInfoUsedMortarStrike, //0x000C
        ShooterInfoUsedStingerMissile, //0x000D
        ShooterInfoNoTeamKillPenalty //0x000E
    };

    ////////////////////////////////////////
    // RuntimeId: 02183
    // TypeInfo:  0x02BF6BD0
    enum GameOverResult
    {
        GameOver_MajorDefeat, //0x0000
        GameOver_Defeat, //0x0001
        GameOver_MinorDefeat, //0x0002
        GameOver_MajorVictory, //0x0003
        GameOver_Victory, //0x0004
        GameOver_MinorVictory, //0x0005
        GameOver_Draw //0x0006
    };

    ////////////////////////////////////////
    // RuntimeId: 02181
    // TypeInfo:  0x02BF6BC0
    enum ToolTipType
    {
        AimAtTeamMateToolTip, //0x0000
        AimAtEnemyToolTip //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 02179
    // TypeInfo:  0x02BF6BB0
    enum Buttons
    {
        NoneButton, //0x0000
        XButton, //0x0001
        SquareButton, //0x0002
        CircleButton, //0x0003
        TriangleButton //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 02177
    // TypeInfo:  0x02BF6BA0
    enum VotingType
    {
        VotingMap, //0x0000
        VotingGameMode, //0x0001
        VotingFriendlyFire //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 02175
    // TypeInfo:  0x02BF6B90
    enum NotifyType
    {
        NotifyPersonal, //0x0000
        NotifyGeneral, //0x0001
        NotifyConsole, //0x0002
        NotityTooltip, //0x0003
        NotifyCount //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 02173
    // TypeInfo:  0x02BF6B80
    enum MenuResponse
    {
        ResponseOk, //0x0000
        ResponseCancel, //0x0001
        ResponseSquare, //0x0002
        ResponseCount //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 02171
    // TypeInfo:  0x02BF6B70
    enum InputIds
    {
        NoInputId, //0x0000
        SpectatorInputId, //0x0001
        FreeCameraInputId, //0x0002
        EntryInputId //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 02169
    // TypeInfo:  0x02BF6B60
    enum CameraIds
    {
        NoCameraId, //0x0000
        FreeCameraId, //0x0001
        EntryCameraId, //0x0002
        CameraIdCount //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 02167
    // TypeInfo:  0x02BF6B50
    enum UIMessageType
    {
        UIMessageType_Text, //0x0000
        UIMessageType_EnemyTank, //0x0001
        UIMessageType_EnemyCar, //0x0002
        UIMessageType_EnemyBoat, //0x0003
        UIMessageType_EnemyHelicopter, //0x0004
        UIMessageType_EnemyInfantry, //0x0005
        UIMessageType_EnemyActivity, //0x0006
        UIMessageType_EnemyC4, //0x0007
        UIMessageType_EnemyATMine, //0x0008
        UIMessageType_ChatGo, //0x0009
        UIMessageType_ChatFollowMe, //0x000A
        UIMessageType_ChatNeedBackup, //0x000B
        UIMessageType_ChatNeedPickup, //0x000C
        UIMessageType_ChatNeedAmmo, //0x000D
        UIMessageType_ChatNeedMedic, //0x000E
        UIMessageType_ChatYes, //0x000F
        UIMessageType_ChatNo, //0x0010
        UIMessageType_ChatThanks, //0x0011
        UIMessageType_ChatSorry, //0x0012
        UIMessageType_ChatNeedRepair, //0x0013
        UIMessageType_ChatGetIn, //0x0014
        UIMessageType_ChatGetOut, //0x0015
        UIMessageType_ChatDropMeOff, //0x0016
        UIMessageType_OrderDefendBase, //0x0017
        UIMessageType_OrderAttackBase, //0x0018
        UIMessageType_OrderMoveToPos, //0x0019
        UIMessageType_JoinedSquad, //0x001A
        UIMessageType_LeftSquad, //0x001B
        UIMessageType_RequestSquad, //0x001C
        UIMessageType_InviteSquad, //0x001D
        UIMessageType_KickSquad, //0x001E
        UIMessageType_LeaderPromoted, //0x001F
        UIMessageType_Connected, //0x0020
        UIMessageType_Disconnected, //0x0021
        UIMessageType_Killed, //0x0022
        UIMessageType_Died //0x0023
    };

    ////////////////////////////////////////
    // RuntimeId: 02165
    // TypeInfo:  0x02BF6B40
    enum HudTextReceiver
    {
        HudTextReceiver_Team, //0x0000
        HudTextReceiver_Squad, //0x0001
        HudTextReceiver_All, //0x0002
        HudTextReceiver_Count //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 02131
    // TypeInfo:  0x02BF6940
    enum ChangeGameSettingType
    {
        CGSNone, //0x0000
        CGSInvertVerticalLookAxis, //0x0001
        CGSCount //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 02124
    // TypeInfo:  0x02BF68E0
    enum PerformanceProfilePlatform
    {
        PPP_Win32, //0x0000
        PPP_Xenon, //0x0001
        PPP_PS3 //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 02002
    // TypeInfo:  0x02BF6170
    enum SoldierChangeAction
    {
        SCA_Done, //0x0000
        SCA_Altered, //0x0001
        SCA_None //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01836
    // TypeInfo:  0x02BF5720
    enum TinyEvent
    {
        TinyEvent_AckTimeSyncDone, //0x0000
        TinyEvent_AckLevelLinked, //0x0001
        TinyEvent_AckLevelRestarted, //0x0002
        TinyEvent_AckEnterPatchRecvState, //0x0003
        TinyEvent_AckAuthentication, //0x0004
        TinyEvent_NackAuthentication, //0x0005
        TinyEvent_AckExitLevel, //0x0006
        TinyEvent_CmdLinkLevel, //0x0007
        TinyEvent_CmdEnterPatchRecvState, //0x0008
        TinyEvent_CmdExitLevel, //0x0009
        TinyEvent_CmdContinueLevel //0x000A
    };

    ////////////////////////////////////////
    // RuntimeId: 01797
    // TypeInfo:  0x02BF444C
    enum MipmapFilterMode
    {
        MipmapFilterMode_Box, //0x0000
        MipmapFilterMode_Renormalize, //0x0001
        MipmapFilterMode_Poisson13, //0x0002
        MipmapFilterMode_Poisson13Clamped //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 01795
    // TypeInfo:  0x02BF443C
    enum WorldViewMode
    {
        WorldViewMode_Default, //0x0000
        WorldViewMode_RawLinear, //0x0001
        WorldViewMode_RawLinearAlpha, //0x0002
        WorldViewMode_Diffuse, //0x0003
        WorldViewMode_Specular, //0x0004
        WorldViewMode_Emissive, //0x0005
        WorldViewMode_Normal, //0x0006
        WorldViewMode_Smoothness, //0x0007
        WorldViewMode_Material, //0x0008
        WorldViewMode_Light, //0x0009
        WorldViewMode_LightDiffuse, //0x000A
        WorldViewMode_LightSpecular, //0x000B
        WorldViewMode_LightIndirect, //0x000C
        WorldViewMode_LightTranslucency, //0x000D
        WorldViewMode_LightOverdraw, //0x000E
        WorldViewMode_SkyVisibility, //0x000F
        WorldViewMode_SkyVisibilityRaw, //0x0010
        WorldViewMode_Overdraw, //0x0011
        WorldViewMode_DynamicAO, //0x0012
        WorldViewMode_Occluders, //0x0013
        WorldViewMode_RadiosityLightMaps, //0x0014
        WorldViewMode_RadiosityDiffuseColor, //0x0015
        WorldViewMode_RadiosityTargetUV, //0x0016
        WorldViewMode_VelocityVector, //0x0017
        WorldViewMode_DistortionVector //0x0018
    };

    ////////////////////////////////////////
    // RuntimeId: 01777
    // TypeInfo:  0x02BF42E8
    enum CharacterLightingMode
    {
        CharacterLightingMode_Add, //0x0000
        CharacterLightingMode_Blend //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 01771
    // TypeInfo:  0x02BF4280
    enum ScreenEffectFrameType
    {
        ScreenEffectFrameType_FullFrame, //0x0000
        ScreenEffectFrameType_SingleFramePart, //0x0001
        ScreenEffectFrameType_SingleSquareFramePart //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01729
    // TypeInfo:  0x02BF3F00
    enum EnlightenColorMode
    {
        EnlightenColorMode_Multiply, //0x0000
        EnlightenColorMode_Override //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 01703
    // TypeInfo:  0x02BF3D18
    enum RenderVolumeTransformType
    {
        RenderVolumeTransformType_WorldSpaceInv, //0x0000
        RenderVolumeTransformType_WorldSpaceNoScale //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 01669
    // TypeInfo:  0x02BF2DBC
    enum TextureCompressQualityMode
    {
        TextureCompressQualityMode_Default, //0x0000
        TextureCompressQualityMode_Simple, //0x0001
        TextureCompressQualityMode_HighQuality //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01667
    // TypeInfo:  0x02BF2DAC
    enum ShaderConstantSystemTexture
    {
        ShaderConstantSystemTexture_DepthBufferTexture, //0x0000
        ShaderConstantSystemTextureCount //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 01665
    // TypeInfo:  0x02BF2D9C
    enum ShaderTimeType
    {
        ShaderTimeType_Game, //0x0000
        ShaderTimeType_Real //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 01663
    // TypeInfo:  0x02BF2D8C
    enum ShaderInterpolationType
    {
        ShaderInterpolationType_Linear, //0x0000
        ShaderInterpolationType_Centroid, //0x0001
        ShaderInterpolationType_NoInterpolation, //0x0002
        ShaderInterpolationType_NoPerspective, //0x0003
        ShaderInterpolationType_Sample, //0x0004
        ShaderInterpolationType_Count //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 01661
    // TypeInfo:  0x02BF2D7C
    enum ShaderDepthBiasGroup
    {
        ShaderDepthBiasGroup_Default, //0x0000
        ShaderDepthBiasGroup_Decal, //0x0001
        ShaderDepthBiasGroup_EmitterOcclusion, //0x0002
        ShaderDepthBiasGroup_EdgeModel, //0x0003
        ShaderDepthBiasGroup_TerrainDecal, //0x0004
        ShaderDepthBiasGroup_TerrainDecalZPass, //0x0005
        ShaderDepthBiasGroup_Shadow16Bit, //0x0006
        ShaderDepthBiasGroup_Shadow24Bit, //0x0007
        ShaderDepthBiasGroup_ZPass, //0x0008
        ShaderDepthBiasGroup_Emissive, //0x0009
        ShaderDepthBiasGroupCount //0x000A
    };

    ////////////////////////////////////////
    // RuntimeId: 01659
    // TypeInfo:  0x02BF2D6C
    enum ShaderTextureCoordType
    {
        ShaderTextureCoordType_Unknown, //0x0000
        ShaderTextureCoordType_VertexElement, //0x0001
        ShaderTextureCoordType_WorldPos //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01657
    // TypeInfo:  0x02BF2D5C
    enum ShaderValueFormat
    {
        ShaderValueFormat_Half, //0x0000
        ShaderValueFormat_Float, //0x0001
        ShaderValueFormat_Int, //0x0002
        ShaderValueFormat_UInt, //0x0003
        ShaderValueFormat_Bool //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 01655
    // TypeInfo:  0x02BF2D4C
    enum ShaderValueType
    {
        ShaderValueType_None, //0x0000
        ShaderValueType_Half1, //0x0001
        ShaderValueType_Half1x2, //0x0002
        ShaderValueType_Half1x3, //0x0003
        ShaderValueType_Half1x4, //0x0004
        ShaderValueType_Half2, //0x0005
        ShaderValueType_Half2x2, //0x0006
        ShaderValueType_Half2x3, //0x0007
        ShaderValueType_Half2x4, //0x0008
        ShaderValueType_Half3, //0x0009
        ShaderValueType_Half3x2, //0x000A
        ShaderValueType_Half3x3, //0x000B
        ShaderValueType_Half3x4, //0x000C
        ShaderValueType_Half4, //0x000D
        ShaderValueType_Half4x2, //0x000E
        ShaderValueType_Half4x3, //0x000F
        ShaderValueType_Half4x4, //0x0010
        ShaderValueType_Float1, //0x0011
        ShaderValueType_Float1x2, //0x0012
        ShaderValueType_Float1x3, //0x0013
        ShaderValueType_Float1x4, //0x0014
        ShaderValueType_Float2, //0x0015
        ShaderValueType_Float2x2, //0x0016
        ShaderValueType_Float2x3, //0x0017
        ShaderValueType_Float2x4, //0x0018
        ShaderValueType_Float3, //0x0019
        ShaderValueType_Float3x2, //0x001A
        ShaderValueType_Float3x3, //0x001B
        ShaderValueType_Float3x4, //0x001C
        ShaderValueType_Float4, //0x001D
        ShaderValueType_Float4x2, //0x001E
        ShaderValueType_Float4x3, //0x001F
        ShaderValueType_Float4x4, //0x0020
        ShaderValueType_Int1, //0x0021
        ShaderValueType_Int1x2, //0x0022
        ShaderValueType_Int1x3, //0x0023
        ShaderValueType_Int1x4, //0x0024
        ShaderValueType_Int2, //0x0025
        ShaderValueType_Int2x2, //0x0026
        ShaderValueType_Int2x3, //0x0027
        ShaderValueType_Int2x4, //0x0028
        ShaderValueType_Int3, //0x0029
        ShaderValueType_Int3x2, //0x002A
        ShaderValueType_Int3x3, //0x002B
        ShaderValueType_Int3x4, //0x002C
        ShaderValueType_Int4, //0x002D
        ShaderValueType_Int4x2, //0x002E
        ShaderValueType_Int4x3, //0x002F
        ShaderValueType_Int4x4, //0x0030
        ShaderValueType_UInt1, //0x0031
        ShaderValueType_UInt1x2, //0x0032
        ShaderValueType_UInt1x3, //0x0033
        ShaderValueType_UInt1x4, //0x0034
        ShaderValueType_UInt2, //0x0035
        ShaderValueType_UInt2x2, //0x0036
        ShaderValueType_UInt2x3, //0x0037
        ShaderValueType_UInt2x4, //0x0038
        ShaderValueType_UInt3, //0x0039
        ShaderValueType_UInt3x2, //0x003A
        ShaderValueType_UInt3x3, //0x003B
        ShaderValueType_UInt3x4, //0x003C
        ShaderValueType_UInt4, //0x003D
        ShaderValueType_UInt4x2, //0x003E
        ShaderValueType_UInt4x3, //0x003F
        ShaderValueType_UInt4x4, //0x0040
        ShaderValueType_Bool1, //0x0041
        ShaderValueType_Bool1x2, //0x0042
        ShaderValueType_Bool1x3, //0x0043
        ShaderValueType_Bool1x4, //0x0044
        ShaderValueType_Bool2, //0x0045
        ShaderValueType_Bool2x2, //0x0046
        ShaderValueType_Bool2x3, //0x0047
        ShaderValueType_Bool2x4, //0x0048
        ShaderValueType_Bool3, //0x0049
        ShaderValueType_Bool3x2, //0x004A
        ShaderValueType_Bool3x3, //0x004B
        ShaderValueType_Bool3x4, //0x004C
        ShaderValueType_Bool4, //0x004D
        ShaderValueType_Bool4x2, //0x004E
        ShaderValueType_Bool4x3, //0x004F
        ShaderValueType_Bool4x4, //0x0050
        ShaderValueType_Sampler1d, //0x0051
        ShaderValueType_Sampler1dArray, //0x0052
        ShaderValueType_Sampler2d, //0x0053
        ShaderValueType_Sampler2dArray, //0x0054
        ShaderValueType_Sampler3d, //0x0055
        ShaderValueType_SamplerCube //0x0056
    };

    ////////////////////////////////////////
    // RuntimeId: 01653
    // TypeInfo:  0x02BF2D3C
    enum ShaderGeometrySpace
    {
        ShaderGeometrySpace_Object, //0x0000
        ShaderGeometrySpace_ObjectScaled, //0x0001
        ShaderGeometrySpace_World, //0x0002
        ShaderGeometrySpace_Screen //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 01651
    // TypeInfo:  0x02BF2D2C
    enum XenonTessellationMode
    {
        XenonTessellationMode_Discrete, //0x0000
        XenonTessellationMode_Continuous, //0x0001
        XenonTessellationMode_PerEdge //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01649
    // TypeInfo:  0x02BF2D1C
    enum ShaderShadowmapMethod
    {
        ShaderShadowmapMethod_None, //0x0000
        ShaderShadowmapMethod_Single, //0x0001
        ShaderShadowmapMethod_SingleDiscard, //0x0002
        ShaderShadowmapMethod_CascadedBox3, //0x0003
        ShaderShadowmapMethod_CascadedBox4 //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 01647
    // TypeInfo:  0x02BF2D0C
    enum ShaderShadowmapQuality
    {
        ShaderShadowmapQuality_Pcf2x2, //0x0000
        ShaderShadowmapQuality_Pcf4x4, //0x0001
        ShaderShadowmapQuality_Gaussian //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01645
    // TypeInfo:  0x02BF2CFC
    enum ShaderObjectLighting
    {
        ShaderObjectLighting_None, //0x0000
        ShaderObjectLighting_LightProbe, //0x0001
        ShaderObjectLighting_LightMap //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01643
    // TypeInfo:  0x02BF2CEC
    enum ShaderGBufferLayout
    {
        ShaderGBufferLayout_Default, //0x0000
        ShaderGBufferLayout_Test //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 01641
    // TypeInfo:  0x02BF2CDC
    enum ShaderRenderMode
    {
        ShaderRenderMode_Default, //0x0000
        ShaderRenderMode_DynamicEnvmap, //0x0001
        ShaderRenderMode_ZOnly, //0x0002
        ShaderRenderMode_DeferredShadingGBufferLayout0, //0x0003
        ShaderRenderMode_DeferredShadingGBufferLayout1, //0x0004
        ShaderRenderMode_DeferredShadingEmissive, //0x0005
        ShaderRenderMode_DeferredShadingUnlit, //0x0006
        ShaderRenderMode_VelocityVector, //0x0007
        ShaderRenderMode_DistortionVector, //0x0008
        ShaderRenderMode_AmbientOcclusionVolume, //0x0009
        ShaderRenderMode_DebugSimple, //0x000A
        ShaderRenderMode_DebugMulti, //0x000B
        ShaderRenderMode_DebugOverdraw, //0x000C
        ShaderRenderMode_DebugShaderCost, //0x000D
        ShaderRenderModeCount //0x000E
    };

    ////////////////////////////////////////
    // RuntimeId: 01639
    // TypeInfo:  0x02BF2CCC
    enum ShaderSkinningMethod
    {
        ShaderSkinningMethod_None, //0x0000
        ShaderSkinningMethod_Linear1Bone, //0x0001
        ShaderSkinningMethod_Linear2Bone, //0x0002
        ShaderSkinningMethod_Linear4Bone, //0x0003
        ShaderSkinningMethod_Linear8Bone, //0x0004
        ShaderSkinningMethod_Null, //0x0005
        ShaderSkinningMethod_DualQuaternion4Bone //0x0006
    };

    ////////////////////////////////////////
    // RuntimeId: 01637
    // TypeInfo:  0x02BF2CBC
    enum ShaderInstancingMethod
    {
        ShaderInstancingMethod_None, //0x0000
        ShaderInstancingMethod_ObjectTransform4x3Half, //0x0001
        ShaderInstancingMethod_ObjectTransform4x3InstanceData4x1Half, //0x0002
        ShaderInstancingMethod_ObjectTransform4x3InstanceData4x2Half, //0x0003
        ShaderInstancingMethod_WorldTransform4x3Float, //0x0004
        ShaderInstancingMethod_ObjectTranslationScaleHalf, //0x0005
        ShaderInstancingMethod_ObjectTranslationScaleHalfInstanceData4x1Half, //0x0006
        ShaderInstancingMethod_ObjectTranslationScaleHalfInstanceData4x2Half, //0x0007
        ShaderInstancingMethod_VertexAnimation, //0x0008
        ShaderInstancingMethod_DxBuffer, //0x0009
        ShaderInstancingMethodCount //0x000A
    };

    ////////////////////////////////////////
    // RuntimeId: 01635
    // TypeInfo:  0x02BF2CAC
    enum ShaderBlendMode
    {
        ShaderBlendMode_Lerp, //0x0000
        ShaderBlendMode_Additive, //0x0001
        ShaderBlendMode_Multiply, //0x0002
        ShaderBlendMode_LerpPremultiplied, //0x0003
        ShaderBlendMode_DecalLerpNoSpec, //0x0004
        ShaderBlendMode_DecalLerpNormal, //0x0005
        ShaderBlendMode_DecalLerpNormalMultiplyDiffuse, //0x0006
        ShaderBlendMode_DecalLerpDiffuse, //0x0007
        ShaderBlendMode_DecalLerpDiffuseNoSpec //0x0008
    };

    ////////////////////////////////////////
    // RuntimeId: 01633
    // TypeInfo:  0x02BF2C9C
    enum SurfaceShaderType
    {
        SurfaceShaderType_Opaque, //0x0000
        SurfaceShaderType_OpaqueAlphaTest, //0x0001
        SurfaceShaderType_OpaqueAlphaTestSimple, //0x0002
        SurfaceShaderType_Transparent, //0x0003
        SurfaceShaderType_TransparentDecal //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 01627
    // TypeInfo:  0x02BF2850
    enum XenonShaderBranchType
    {
        XsbtDefault, //0x0000
        XsbtPredicate, //0x0001
        XsbtPredicateBlock, //0x0002
        XsbtBranch //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 01625
    // TypeInfo:  0x02BF2840
    enum ShaderBranchMethod
    {
        SbmStatic, //0x0000
        SbmFlat, //0x0001
        SbmDynamicIfElse, //0x0002
        SbmDynamicIfTrue, //0x0003
        SbmDynamicIfFalse //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 01623
    // TypeInfo:  0x02BF2830
    enum ShaderComparisonOperator
    {
        ScoEquals, //0x0000
        ScoNotEquals, //0x0001
        ScoLess, //0x0002
        ScoLessEquals, //0x0003
        ScoGreater, //0x0004
        ScoGreaterEquals //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 01621
    // TypeInfo:  0x02BF2820
    enum BlendShaderMode
    {
        BsmLerp, //0x0000
        BsmAdd, //0x0001
        BsmSubtract, //0x0002
        BsmMultiply, //0x0003
        BsmMultiply2x, //0x0004
        BsmScreen, //0x0005
        BsmDifference, //0x0006
        BsmLighten, //0x0007
        BsmDarken, //0x0008
        BsmOverlay //0x0009
    };

    ////////////////////////////////////////
    // RuntimeId: 01619
    // TypeInfo:  0x02BF2810
    enum CurveShaderType
    {
        CstSine, //0x0000
        CstSineNormalized, //0x0001
        CstSawtooth, //0x0002
        CstTriangle, //0x0003
        CstSquare //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 01617
    // TypeInfo:  0x02BF2800
    enum EyeVectorSpace
    {
        EyeVectorSpace_World, //0x0000
        EyeVectorSpace_Object, //0x0001
        EyeVectorSpace_Tangent //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01615
    // TypeInfo:  0x02BF27F0
    enum PixelNormalSpace
    {
        PnsTangent //0x0000
    };

    ////////////////////////////////////////
    // RuntimeId: 01613
    // TypeInfo:  0x02BF27E0
    enum VertexNormalSpace
    {
        VnsObject, //0x0000
        VnsInstance, //0x0001
        VnsWorld //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01611
    // TypeInfo:  0x02BF27D0
    enum ShaderPositionSpace
    {
        ShaderPositionSpace_Object, //0x0000
        ShaderPositionSpace_World //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 01609
    // TypeInfo:  0x02BF27C0
    enum ShaderTextureDecompression
    {
        ShaderTextureDecompression_None, //0x0000
        ShaderTextureDecompression_NormalAG, //0x0001
        ShaderTextureDecompression_NormalRG, //0x0002
        ShaderTextureDecompression_NormalRGA, //0x0003
        ShaderTextureDecompression_RGBE, //0x0004
        ShaderTextureDecompression_SrgbToLinear, //0x0005
        ShaderTextureDecompression_NormalRGorAG //0x0006
    };

    ////////////////////////////////////////
    // RuntimeId: 01607
    // TypeInfo:  0x02BF27B0
    enum ShaderValueParameterType
    {
        SvptLiteral, //0x0000
        SvptExternalPermutation, //0x0001
        SvptExternalConstant, //0x0002
        SvptExternalSubMaterial //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 01605
    // TypeInfo:  0x02BF27A0
    enum ShaderLightingModel
    {
        ShaderLightingModel_Standard, //0x0000
        ShaderLightingModel_Metallic, //0x0001
        ShaderLightingModel_Skin, //0x0002
        ShaderLightingModel_DynamicEnvmap, //0x0003
        ShaderLightingModel_Translucent //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 01603
    // TypeInfo:  0x02BF2790
    enum ShaderPortType
    {
        SptBool, //0x0000
        SptInteger, //0x0001
        SptScalar, //0x0002
        SptVec2, //0x0003
        SptVec3, //0x0004
        SptVec4, //0x0005
        SptColor //0x0006
    };

    ////////////////////////////////////////
    // RuntimeId: 01585
    // TypeInfo:  0x02BF26AC
    enum ShaderParameterType
    {
        ShaderParameterType_Bool, //0x0000
        ShaderParameterType_Int, //0x0001
        ShaderParameterType_Scalar, //0x0002
        ShaderParameterType_Vec2, //0x0003
        ShaderParameterType_Vec3, //0x0004
        ShaderParameterType_Vec4, //0x0005
        ShaderParameterType_Color, //0x0006
        ShaderParameterType_TextureSlice, //0x0007
        ShaderParameterTypeCount //0x0008
    };

    ////////////////////////////////////////
    // RuntimeId: 01579
    // TypeInfo:  0x02BF2644
    enum ShaderConstantFunction
    {
        ShaderConstantFunction_ViewMatrix, //0x0000
        ShaderConstantFunction_ViewProjMatrix, //0x0001
        ShaderConstantFunction_WorldViewMatrix, //0x0002
        ShaderConstantFunction_WorldViewProjMatrix, //0x0003
        ShaderConstantFunction_CrViewProjMatrix, //0x0004
        ShaderConstantFunction_CrWorldViewProjMatrix, //0x0005
        ShaderConstantFunction_CameraPos, //0x0006
        ShaderConstantFunction_PrevViewProjMatrix, //0x0007
        ShaderConstantFunction_PrevWorldViewProjMatrix, //0x0008
        ShaderConstantFunction_WorldMatrix, //0x0009
        ShaderConstantFunction_PrevWorldMatrix, //0x000A
        ShaderConstantFunction_BoneVectors, //0x000B
        ShaderConstantFunction_PrevBoneVectors, //0x000C
        ShaderConstantFunction_LightProbeShR, //0x000D
        ShaderConstantFunction_LightProbeShG, //0x000E
        ShaderConstantFunction_LightProbeShB, //0x000F
        ShaderConstantFunction_LightProbeShO, //0x0010
        ShaderConstantFunction_LightMapUvTransform, //0x0011
        ShaderConstantFunction_LightMapUvTranslation, //0x0012
        ShaderConstantFunction_LightMapIrradianceTexture, //0x0013
        ShaderConstantFunction_LightMapIrradianceChromaTexture, //0x0014
        ShaderConstantFunction_LightMapIrradianceLumaTexture, //0x0015
        ShaderConstantFunction_LightMapDirectionTexture, //0x0016
        ShaderConstantFunction_LightMapSkyVisibilityTexture, //0x0017
        ShaderConstantFunction_DepthBufferTexture, //0x0018
        ShaderConstantFunction_ViewportZMinMaxKzKw, //0x0019
        ShaderConstantFunction_Time, //0x001A
        ShaderConstantFunction_ScreenSize, //0x001B
        ShaderConstantFunction_OutdoorLightDir, //0x001C
        ShaderConstantFunction_OutdoorLightHemisphereDir, //0x001D
        ShaderConstantFunction_OutdoorLightKeyColor, //0x001E
        ShaderConstantFunction_OutdoorLightKeySpecularColorAndHemisphereVisibility, //0x001F
        ShaderConstantFunction_OutdoorLightTopColor, //0x0020
        ShaderConstantFunction_OutdoorLightBottomColor, //0x0021
        ShaderConstantFunction_OutdoorLightShadowTransform, //0x0022
        ShaderConstantFunction_OutdoorLightShadowmapSizeAndInvSize, //0x0023
        ShaderConstantFunction_OutdoorLightShadowmapTexture, //0x0024
        ShaderConstantFunction_OutdoorLightTransparencyShadowmapTexture, //0x0025
        ShaderConstantFunction_OutdoorLightSkyEnvmap, //0x0026
        ShaderConstantFunction_OutdoorLightDynamicEnvmap, //0x0027
        ShaderConstantFunction_OutdoorLightTopColorEnvmap, //0x0028
        ShaderConstantFunction_OutdoorLightBottomColorEnvmap, //0x0029
        ShaderConstantFunction_OutdoorLightKeyColorEnvmap, //0x002A
        ShaderConstantFunction_OutdoorLightSkyEnvmap8BitTexInvScale, //0x002B
        ShaderConstantFunction_TransparentStartAndEndAndClamp, //0x002C
        ShaderConstantFunction_BestFitNormalScaleTexture, //0x002D
        ShaderConstantFunction_SceneTexture, //0x002E
        ShaderConstantFunction_ColorScale, //0x002F
        ShaderConstantFunction_DebugNonFiniteColor, //0x0030
        ShaderConstantFunction_XenonInstancingIndexCount, //0x0031
        ShaderConstantFunction_Ps3ClipPlane, //0x0032
        ShaderConstantFunction_DxVectorBufferOffsets, //0x0033
        ShaderConstantFunctionCount //0x0034
    };

    ////////////////////////////////////////
    // RuntimeId: 01569
    // TypeInfo:  0x02BF25C8
    enum BlurFilter
    {
        BfNone, //0x0000
        BfGaussian3Pixels, //0x0001
        BfGaussian5Pixels, //0x0002
        BfGaussian7Pixels, //0x0003
        BfGaussian9Pixels, //0x0004
        BfGaussian15Pixels, //0x0005
        BfGaussian31Pixels //0x0006
    };

    ////////////////////////////////////////
    // RuntimeId: 01559
    // TypeInfo:  0x02BF2508
    enum DynamicAOMethod
    {
        DynamicAOMethod_SSAO, //0x0000
        DynamicAOMethod_HBAO //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 01557
    // TypeInfo:  0x02BF24F8
    enum PostProcessDebugMode
    {
        PpdmDefault, //0x0000
        PpdmBloom, //0x0001
        PpdmBloomStep, //0x0002
        PpdmDofBlur, //0x0003
        PpdmBlur, //0x0004
        PpdmBlurStep, //0x0005
        PpdmDepth //0x0006
    };

    ////////////////////////////////////////
    // RuntimeId: 01541
    // TypeInfo:  0x02BF23B4
    enum TonemapMethod
    {
        TonemapMethod_None, //0x0000
        TonemapMethod_Linear, //0x0001
        TonemapMethod_Filmic, //0x0002
        TonemapMethod_FilmicNeutral, //0x0003
        TonemapMethodCount //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 01539
    // TypeInfo:  0x02BF23A4
    enum EntropyCodecType
    {
        EntropyCodecType_Arithmetic //0x0000
    };

    ////////////////////////////////////////
    // RuntimeId: 01537
    // TypeInfo:  0x02BF2394
    enum PointCloudAttributeQuantization
    {
        PointCloudAttributeQuantization_s5e5, //0x0000
        PointCloudAttributeQuantization_s7e5, //0x0001
        PointCloudAttributeQuantization_s10e5, //0x0002
        PointCloudAttributeQuantization_s16e7, //0x0003
        PointCloudAttributeQuantization_None, //0x0004
        PointCloudAttributeQuantization_Count //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 01535
    // TypeInfo:  0x02BF2384
    enum PointCloudAttributeUsage
    {
        PointCloudAttributeUsage_Position, //0x0000
        PointCloudAttributeUsage_Normal, //0x0001
        PointCloudAttributeUsage_Tangent, //0x0002
        PointCloudAttributeUsage_Binormal, //0x0003
        PointCloudAttributeUsage_Count //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 01533
    // TypeInfo:  0x02BF2374
    enum MeshSubsetCategoryFlags
    {
        MeshSubsetCategoryFlags_Opaque, //0x0000
        MeshSubsetCategoryFlags_Transparent, //0x0001
        MeshSubsetCategoryFlags_TransparentDecal, //0x0002
        MeshSubsetCategoryFlags_ZOnly, //0x0003
        MeshSubsetCategoryFlags_DynamicEnvmap, //0x0004
        MeshSubsetCategoryFlags_PlanarReflection, //0x0005
        MeshSubsetCategoryFlags_Normal, //0x0006
        MeshSubsetCategoryFlags_All //0x0007
    };

    ////////////////////////////////////////
    // RuntimeId: 01531
    // TypeInfo:  0x02BF2364
    enum MeshSubsetCategory
    {
        MeshSubsetCategory_Opaque, //0x0000
        MeshSubsetCategory_Transparent, //0x0001
        MeshSubsetCategory_TransparentDecal, //0x0002
        MeshSubsetCategory_ZOnly, //0x0003
        MeshSubsetCategoryCount //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 01529
    // TypeInfo:  0x02BF2354
    enum MeshType
    {
        MeshType_Rigid, //0x0000
        MeshType_Skinned, //0x0001
        MeshType_Composite //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01527
    // TypeInfo:  0x02BF2344
    enum MeshLimits
    {
        MaxMeshLodCount //0x0000
    };

    ////////////////////////////////////////
    // RuntimeId: 01525
    // TypeInfo:  0x02BF2334
    enum MeshHandleFlags
    {
        InvalidMeshHandle //0x0000
    };

    ////////////////////////////////////////
    // RuntimeId: 01489
    // TypeInfo:  0x02BF208C
    enum EnlightenType
    {
        EnlightenType_Dynamic, //0x0000
        EnlightenType_LightProbe, //0x0001
        EnlightenType_Static //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01485
    // TypeInfo:  0x02BF2050
    enum SpotLightShape
    {
        SpotLightShape_Cone, //0x0000
        SpotLightShape_Frustum, //0x0001
        SpotLightShape_OrthoFrustum //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01447
    // TypeInfo:  0x02BF1DEC
    enum VertexElementClassification
    {
        VertexElementClassification_PerVertex, //0x0000
        VertexElementClassification_PerInstance, //0x0001
        VertexElementClassification_Index //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01445
    // TypeInfo:  0x02BF1DDC
    enum VertexElementUsage
    {
        VertexElementUsage_Unknown, //0x0000
        VertexElementUsage_Pos, //0x0001
        VertexElementUsage_BoneIndices, //0x0002
        VertexElementUsage_BoneIndices2, //0x0003
        VertexElementUsage_BoneWeights, //0x0004
        VertexElementUsage_BoneWeights2, //0x0005
        VertexElementUsage_Normal, //0x0006
        VertexElementUsage_Tangent, //0x0007
        VertexElementUsage_Binormal, //0x0008
        VertexElementUsage_BinormalSign, //0x0009
        VertexElementUsage_WorldTrans1, //0x000A
        VertexElementUsage_WorldTrans2, //0x000B
        VertexElementUsage_WorldTrans3, //0x000C
        VertexElementUsage_InstanceId, //0x000D
        VertexElementUsage_InstanceUserData0, //0x000E
        VertexElementUsage_InstanceUserData1, //0x000F
        VertexElementUsage_XenonIndex, //0x0010
        VertexElementUsage_XenonBarycentric, //0x0011
        VertexElementUsage_XenonQuadID, //0x0012
        VertexElementUsage_Index, //0x0013
        VertexElementUsage_ViewIndex, //0x0014
        VertexElementUsage_Color0, //0x0015
        VertexElementUsage_Color1, //0x0016
        VertexElementUsage_TexCoord0, //0x0017
        VertexElementUsage_TexCoord1, //0x0018
        VertexElementUsage_TexCoord2, //0x0019
        VertexElementUsage_TexCoord3, //0x001A
        VertexElementUsage_TexCoord4, //0x001B
        VertexElementUsage_TexCoord5, //0x001C
        VertexElementUsage_TexCoord6, //0x001D
        VertexElementUsage_TexCoord7, //0x001E
        VertexElementUsage_RadiosityTexCoord, //0x001F
        VertexElementUsage_VisInfo, //0x0020
        VertexElementUsage_SpriteSize, //0x0021
        VertexElementUsage_PackedTexCoord0, //0x0022
        VertexElementUsage_PackedTexCoord1, //0x0023
        VertexElementUsage_PackedTexCoord2, //0x0024
        VertexElementUsage_PackedTexCoord3, //0x0025
        VertexElementUsage_ClipDistance0, //0x0026
        VertexElementUsage_ClipDistance1, //0x0027
        VertexElementUsage_SubMaterialIndex, //0x0028
        VertexElementUsage_BranchInfo, //0x0029
        VertexElementUsage_PosAndScale, //0x002A
        VertexElementUsage_Rotation, //0x002B
        VertexElementUsage_SpriteSizeAndUv, //0x002C
        VertexElementUsage_FadePos, //0x002D
        VertexElementUsage_SpawnTime, //0x002E
        VertexElementUsage_PosAndSoftMul, //0x002F
        VertexElementUsage_Alpha, //0x0030
        VertexElementUsage_Misc0, //0x0031
        VertexElementUsage_Misc1, //0x0032
        VertexElementUsage_LeftAndRotation, //0x0033
        VertexElementUsage_UpAndNormalBlend, //0x0034
        VertexElementUsage_SH_R, //0x0035
        VertexElementUsage_SH_G, //0x0036
        VertexElementUsage_SH_B, //0x0037
        VertexElementUsage_PosAndRejectCulling, //0x0038
        VertexElementUsage_Shadow, //0x0039
        VertexElementUsage_PatchUv, //0x003A
        VertexElementUsage_Height, //0x003B
        VertexElementUsage_MaskUVs0, //0x003C
        VertexElementUsage_MaskUVs1, //0x003D
        VertexElementUsage_MaskUVs2, //0x003E
        VertexElementUsage_MaskUVs3, //0x003F
        VertexElementUsage_UserMasks, //0x0040
        VertexElementUsage_HeightfieldUv, //0x0041
        VertexElementUsage_MaskUv, //0x0042
        VertexElementUsage_GlobalColorUv, //0x0043
        VertexElementUsage_HeightfieldPixelSizeAndAspect, //0x0044
        VertexElementUsage_WorldPositionXz, //0x0045
        VertexElementUsage_TerrainTextureNodeUv, //0x0046
        VertexElementUsage_ParentTerrainTextureNodeUv, //0x0047
        VertexElementUsage_Uv01, //0x0048
        VertexElementUsage_WorldPos, //0x0049
        VertexElementUsage_EyeVector, //0x004A
        VertexElementUsage_LightParams1, //0x004B
        VertexElementUsage_LightParams2, //0x004C
        VertexElementUsage_LightSubParams, //0x004D
        VertexElementUsage_LightSideVector, //0x004E
        VertexElementUsage_LightInnerAndOuterAngle, //0x004F
        VertexElementUsage_LightDir, //0x0050
        VertexElementUsage_LightMatrix1, //0x0051
        VertexElementUsage_LightMatrix2, //0x0052
        VertexElementUsage_LightMatrix3, //0x0053
        VertexElementUsage_LightMatrix4, //0x0054
        VertexElementUsage_Custom //0x0055
    };

    ////////////////////////////////////////
    // RuntimeId: 01443
    // TypeInfo:  0x02BF1DCC
    enum VertexElementFormat
    {
        VertexElementFormat_None, //0x0000
        VertexElementFormat_Float, //0x0001
        VertexElementFormat_Float2, //0x0002
        VertexElementFormat_Float3, //0x0003
        VertexElementFormat_Float4, //0x0004
        VertexElementFormat_Half, //0x0005
        VertexElementFormat_Half2, //0x0006
        VertexElementFormat_Half3, //0x0007
        VertexElementFormat_Half4, //0x0008
        VertexElementFormat_UByteN, //0x0009
        VertexElementFormat_Byte4, //0x000A
        VertexElementFormat_Byte4N, //0x000B
        VertexElementFormat_UByte4, //0x000C
        VertexElementFormat_UByte4N, //0x000D
        VertexElementFormat_Short, //0x000E
        VertexElementFormat_Short2, //0x000F
        VertexElementFormat_Short3, //0x0010
        VertexElementFormat_Short4, //0x0011
        VertexElementFormat_ShortN, //0x0012
        VertexElementFormat_Short2N, //0x0013
        VertexElementFormat_Short3N, //0x0014
        VertexElementFormat_Short4N, //0x0015
        VertexElementFormat_UShort2, //0x0016
        VertexElementFormat_UShort4, //0x0017
        VertexElementFormat_UShort2N, //0x0018
        VertexElementFormat_UShort4N, //0x0019
        VertexElementFormat_Int, //0x001A
        VertexElementFormat_Int2, //0x001B
        VertexElementFormat_Int3, //0x001C
        VertexElementFormat_Int4, //0x001D
        VertexElementFormat_IntN, //0x001E
        VertexElementFormat_Int2N, //0x001F
        VertexElementFormat_Int4N, //0x0020
        VertexElementFormat_UInt, //0x0021
        VertexElementFormat_UInt2, //0x0022
        VertexElementFormat_UInt3, //0x0023
        VertexElementFormat_UInt4, //0x0024
        VertexElementFormat_UIntN, //0x0025
        VertexElementFormat_UInt2N, //0x0026
        VertexElementFormat_UInt4N, //0x0027
        VertexElementFormat_Comp3_10_10_10, //0x0028
        VertexElementFormat_Comp3N_10_10_10, //0x0029
        VertexElementFormat_UComp3_10_10_10, //0x002A
        VertexElementFormat_UComp3N_10_10_10, //0x002B
        VertexElementFormat_Comp3_11_11_10, //0x002C
        VertexElementFormat_Comp3N_11_11_10, //0x002D
        VertexElementFormat_UComp3_11_11_10, //0x002E
        VertexElementFormat_UComp3N_11_11_10, //0x002F
        VertexElementFormat_Comp4_10_10_10_2, //0x0030
        VertexElementFormat_Comp4N_10_10_10_2, //0x0031
        VertexElementFormat_UComp4_10_10_10_2, //0x0032
        VertexElementFormat_UComp4N_10_10_10_2 //0x0033
    };

    ////////////////////////////////////////
    // RuntimeId: 01427
    // TypeInfo:  0x02BF0B3C
    enum ShaderProgram
    {
        ShaderProgram_DebugRender_Font2d, //0x0000
        ShaderProgram_DebugRender_FontShadow2d, //0x0001
        ShaderProgram_DebugRender_Color2d, //0x0002
        ShaderProgram_DebugRender_Color3d, //0x0003
        ShaderProgram_DebugRender_Lit3d, //0x0004
        ShaderProgram_DebugRender_LitGrid3d, //0x0005
        ShaderProgram_SimpleWhite, //0x0006
        ShaderProgram_MaterialID, //0x0007
        ShaderProgram_DownsampleQuarter, //0x0008
        ShaderProgram_DownsampleQuarterZ, //0x0009
        ShaderProgram_DownsampleHalf, //0x000A
        ShaderProgram_DownsampleDepthHalfSingleSample, //0x000B
        ShaderProgram_DownsampleHalfGrayscaleLog, //0x000C
        ShaderProgram_DownsampleCubeHalfPoisson13, //0x000D
        ShaderProgram_DownsampleCubeHalfPoisson13Clamped, //0x000E
        ShaderProgram_SeperableFilter, //0x000F
        ShaderProgram_GaussianBlurRgb15HCube, //0x0010
        ShaderProgram_GaussianBlurRgb15VCube, //0x0011
        ShaderProgram_FastGaussian9H, //0x0012
        ShaderProgram_FastGaussian9V, //0x0013
        ShaderProgram_GreyscaleGaussian9H, //0x0014
        ShaderProgram_GreyscaleGaussian9V, //0x0015
        ShaderProgram_CosineBlurRgbHCube, //0x0016
        ShaderProgram_CosineBlurRgbVCube, //0x0017
        ShaderProgram_GaussianBlurRgba15HCube, //0x0018
        ShaderProgram_GaussianBlurRgba15VCube, //0x0019
        ShaderProgram_CosineBlurRgbaHCube, //0x001A
        ShaderProgram_CosineBlurRgbaVCube, //0x001B
        ShaderProgram_Tex2d, //0x001C
        ShaderProgram_Tex2dConstantAlpha, //0x001D
        ShaderProgram_Tex2dSaturateAlpha, //0x001E
        ShaderProgram_Tex2dAlpha, //0x001F
        ShaderProgram_Tex2dAlphaOverlayAdd, //0x0020
        ShaderProgram_Tex2dAlphaOverlayLerp, //0x0021
        ShaderProgram_Tex2dPow, //0x0022
        ShaderProgram_Tex2dPowAlpha, //0x0023
        ShaderProgram_Tex2dMrt2, //0x0024
        ShaderProgram_Tex2dMrt3, //0x0025
        ShaderProgram_Tex2dMrt4, //0x0026
        ShaderProgram_Tex2dGrayscaleLog, //0x0027
        ShaderProgram_Tex3d, //0x0028
        ShaderProgram_TexCube, //0x0029
        ShaderProgram_TexCubeAlpha, //0x002A
        ShaderProgram_TexCubeAlphaOverlayAdd, //0x002B
        ShaderProgram_TexCubeAlphaOverlayLerp, //0x002C
        ShaderProgram_Color, //0x002D
        ShaderProgram_Tonemap, //0x002E
        ShaderProgram_ColorGrade, //0x002F
        ShaderProgram_Dof, //0x0030
        ShaderProgram_UnprojectDepthBuffer, //0x0031
        ShaderProgram_UnprojectDepthBufferHalf, //0x0032
        ShaderProgram_UnprojectDepthBufferMsaa, //0x0033
        ShaderProgram_SingleDepthSampleMsaa, //0x0034
        ShaderProgram_MaxDepthSampleMsaa, //0x0035
        ShaderProgram_TexYCrCb, //0x0036
        ShaderProgram_CombineTextures2, //0x0037
        ShaderProgram_CombineTextures3, //0x0038
        ShaderProgram_CombineTextures4, //0x0039
        ShaderProgram_CombineTextures5, //0x003A
        ShaderProgram_CombineTextures6, //0x003B
        ShaderProgram_CombineTextures2Bilinear, //0x003C
        ShaderProgram_CombineTextures3Bilinear, //0x003D
        ShaderProgram_CombineTextures4Bilinear, //0x003E
        ShaderProgram_CombineTextures5Bilinear, //0x003F
        ShaderProgram_CombineTextures6Bilinear, //0x0040
        ShaderProgram_CombineOpaqueAndTrans2d, //0x0041
        ShaderProgram_CombineOpaqueAndTransCube, //0x0042
        ShaderProgram_CombineOpaqueAndTrans2dSpecial, //0x0043
        ShaderProgram_CombineOpaqueAndTransCubeSpecial, //0x0044
        ShaderProgram_CrtHudFx, //0x0045
        ShaderProgram_CrtAnalogLinkHudFx, //0x0046
        ShaderProgram_RadialBlur, //0x0047
        ShaderProgram_PoissonRadialBlur, //0x0048
        ShaderProgram_ChromaticAberrationWithRadialBlend, //0x0049
        ShaderProgram_FLIR, //0x004A
        ShaderProgram_ColorTint, //0x004B
        ShaderProgram_WriteZ, //0x004C
        ShaderProgram_WriteHalfResZ, //0x004D
        ShaderProgram_WriteHalfResZFat, //0x004E
        ShaderProgram_WriteHalfResZAverage, //0x004F
        ShaderProgram_LinearZ, //0x0050
        ShaderProgram_ParticleVertexShadows, //0x0051
        ShaderProgram_ParticleVertexShadowsAccum, //0x0052
        ShaderProgram_ParticleVertexShadowsAccumBlend, //0x0053
        ShaderProgram_HalfResUpsampleBilateral, //0x0054
        ShaderProgram_HalfResUpsampleBilateralTiled, //0x0055
        ShaderProgram_HalfResUpsampleBilinearTiled, //0x0056
        ShaderProgram_HalfResCategorize, //0x0057
        ShaderProgram_HalfResCategorizeDownsample, //0x0058
        ShaderProgram_DigitalDistortion, //0x0059
        ShaderProgram_MotionBlur, //0x005A
        ShaderProgram_CalcVelocityFromZBuffer, //0x005B
        ShaderProgram_FXAA, //0x005C
        ShaderProgram_HorizonBasedAO, //0x005D
        ShaderProgram_HorizonBasedAO_BlurPS, //0x005E
        ShaderProgram_HorizonBasedAO_BlurCS, //0x005F
        ShaderProgram_HorizonBasedAO_PackAODepth, //0x0060
        ShaderProgram_HorizonBasedAO_CompositeTemporal, //0x0061
        ShaderProgram_HorizonBasedAO_NoiseDetection, //0x0062
        ShaderProgram_SSAO, //0x0063
        ShaderProgram_SSAO_DilateDraw, //0x0064
        ShaderProgram_LightTileDownsampleHalf, //0x0065
        ShaderProgram_LightTileDownsampleHalfStage2, //0x0066
        ShaderProgram_LightTileDownsampleHalfStage3, //0x0067
        ShaderProgram_TiledLightVisibility_Point, //0x0068
        ShaderProgram_TiledLightVisibility_Spot, //0x0069
        ShaderProgram_TiledLightVisibility_Line, //0x006A
        ShaderProgram_TiledLightVisibility_CalcTileFrustaData, //0x006B
        ShaderProgram_DeferredGBufferSimpleFixup, //0x006C
        ShaderProgram_MsaaClassify, //0x006D
        ShaderProgram_DeferredPointLight, //0x006E
        ShaderProgram_DeferredPointLightCS, //0x006F
        ShaderProgram_DeferredPointLightTile, //0x0070
        ShaderProgram_DeferredPointLightZ, //0x0071
        ShaderProgram_DeferredLineLight, //0x0072
        ShaderProgram_DeferredLineLightTile, //0x0073
        ShaderProgram_DeferredSpotLight, //0x0074
        ShaderProgram_DeferredSpotLightZ, //0x0075
        ShaderProgram_DeferredConeLight, //0x0076
        ShaderProgram_DeferredConeLightTile, //0x0077
        ShaderProgram_DeferredOutdoorLight, //0x0078
        ShaderProgram_DeferredOutdoorLightTile, //0x0079
        ShaderProgram_DeferredShadow, //0x007A
        ShaderProgram_Sky, //0x007B
        ShaderProgram_HeightfieldTessellation, //0x007C
        ShaderProgram_XenonMemCopy, //0x007D
        ShaderProgram_XenonFastUntile, //0x007E
        ShaderProgram_XenonFastDepthStencilFill, //0x007F
        ShaderProgram_XenonFastDepthStencilFill_ZOnly, //0x0080
        ShaderProgram_XenonJobEnable, //0x0081
        ShaderProgram_Ui_Textured, //0x0082
        ShaderProgram_Ui_Textured2, //0x0083
        ShaderProgram_Ui_Textured3, //0x0084
        ShaderProgram_Ui_Textured2GlobalColor, //0x0085
        ShaderProgram_Ui_Scaleform_StripSolidColor, //0x0086
        ShaderProgram_Ui_Scaleform_StripCxformTextureMultiply, //0x0087
        ShaderProgram_Ui_Scaleform_StripCxformTexture, //0x0088
        ShaderProgram_Ui_Scaleform_StripCxformGauraudNoAddAlpha, //0x0089
        ShaderProgram_Ui_Scaleform_StripCxformGauraud, //0x008A
        ShaderProgram_Ui_Scaleform_StripCxformGauraudTexture, //0x008B
        ShaderProgram_Ui_Scaleform_StripCxform2Texture, //0x008C
        ShaderProgram_Ui_Scaleform_StripCxformGauraudMultiply, //0x008D
        ShaderProgram_Ui_Scaleform_StripCxformGauraudMultiplyNoAddAlpha, //0x008E
        ShaderProgram_Ui_Scaleform_StripCxformGauraudMultiplyTexture, //0x008F
        ShaderProgram_Ui_Scaleform_StripCxformMultiply2Texture, //0x0090
        ShaderProgram_Ui_Scaleform_XY16iC32SolidColor, //0x0091
        ShaderProgram_Ui_Scaleform_XY16iC32CxformTextureMultiply, //0x0092
        ShaderProgram_Ui_Scaleform_XY16iC32CxformTexture, //0x0093
        ShaderProgram_Ui_Scaleform_XY16iC32CxformGauraudNoAddAlpha, //0x0094
        ShaderProgram_Ui_Scaleform_XY16iC32CxformGauraud, //0x0095
        ShaderProgram_Ui_Scaleform_XY16iC32CxformGauraudTexture, //0x0096
        ShaderProgram_Ui_Scaleform_XY16iC32Cxform2Texture, //0x0097
        ShaderProgram_Ui_Scaleform_XY16iC32CxformGauraudMultiply, //0x0098
        ShaderProgram_Ui_Scaleform_XY16iC32CxformGauraudMultiplyNoAddAlpha, //0x0099
        ShaderProgram_Ui_Scaleform_XY16iC32CxformGauraudMultiplyTexture, //0x009A
        ShaderProgram_Ui_Scaleform_XY16iC32CxformMultiply2Texture, //0x009B
        ShaderProgram_Ui_Scaleform_XY16iCF32SolidColor, //0x009C
        ShaderProgram_Ui_Scaleform_XY16iCF32CxformTextureMultiply, //0x009D
        ShaderProgram_Ui_Scaleform_XY16iCF32CxformTexture, //0x009E
        ShaderProgram_Ui_Scaleform_XY16iCF32CxformGauraudNoAddAlpha, //0x009F
        ShaderProgram_Ui_Scaleform_XY16iCF32CxformGauraud, //0x00A0
        ShaderProgram_Ui_Scaleform_XY16iCF32CxformGauraudTexture, //0x00A1
        ShaderProgram_Ui_Scaleform_XY16iCF32Cxform2Texture, //0x00A2
        ShaderProgram_Ui_Scaleform_XY16iCF32CxformGauraudMultiply, //0x00A3
        ShaderProgram_Ui_Scaleform_XY16iCF32CxformGauraudMultiplyNoAddAlpha, //0x00A4
        ShaderProgram_Ui_Scaleform_XY16iCF32CxformGauraudMultiplyTexture, //0x00A5
        ShaderProgram_Ui_Scaleform_XY16iCF32CxformMultiply2Texture, //0x00A6
        ShaderProgram_Ui_Scaleform_XY16iCF32_T2SolidColor, //0x00A7
        ShaderProgram_Ui_Scaleform_XY16iCF32_T2CxformTextureMultiply, //0x00A8
        ShaderProgram_Ui_Scaleform_XY16iCF32_T2CxformTexture, //0x00A9
        ShaderProgram_Ui_Scaleform_XY16iCF32_T2CxformGauraudNoAddAlpha, //0x00AA
        ShaderProgram_Ui_Scaleform_XY16iCF32_T2CxformGauraud, //0x00AB
        ShaderProgram_Ui_Scaleform_XY16iCF32_T2CxformGauraudTexture, //0x00AC
        ShaderProgram_Ui_Scaleform_XY16iCF32_T2Cxform2Texture, //0x00AD
        ShaderProgram_Ui_Scaleform_XY16iCF32_T2CxformGauraudMultiply, //0x00AE
        ShaderProgram_Ui_Scaleform_XY16iCF32_T2CxformGauraudMultiplyNoAddAlpha, //0x00AF
        ShaderProgram_Ui_Scaleform_XY16iCF32_T2CxformGauraudMultiplyTexture, //0x00B0
        ShaderProgram_Ui_Scaleform_XY16iCF32_T2CxformMultiply2Texture, //0x00B1
        ShaderProgram_Ui_Scaleform_GlyphTextTextureAlpha, //0x00B2
        ShaderProgram_Ui_Scaleform_GlyphTextTextureColorMultiply, //0x00B3
        ShaderProgram_Ui_Scaleform_GlyphTextTextureColor, //0x00B4
        ShaderProgram_Ui_Scaleform_StripFilterBlur, //0x00B5
        ShaderProgram_Ui_Scaleform_StripFilterShadow, //0x00B6
        ShaderProgram_Ui_Scaleform_XY16iC32FilterBlur, //0x00B7
        ShaderProgram_Ui_Scaleform_XY16iC32FilterShadow, //0x00B8
        ShaderProgram_Ui_Scaleform_XY16iCF32FilterBlur, //0x00B9
        ShaderProgram_Ui_Scaleform_XY16iCF32FilterShadow, //0x00BA
        ShaderProgram_Ui_Scaleform_XY16iCF32_T2FilterBlur, //0x00BB
        ShaderProgram_Ui_Scaleform_XY16iCF32_T2FilterShadow, //0x00BC
        ShaderProgram_Ui_Scaleform_GlyphFilterBlur, //0x00BD
        ShaderProgram_Ui_Scaleform_GlyphFilterShadow, //0x00BE
        ShaderProgram_Ui_Minimap, //0x00BF
        ShaderProgram_Ui_MinimapIcon, //0x00C0
        ShaderProgram_Ui_CombatArea, //0x00C1
        ShaderProgram_ExperimentShader_A, //0x00C2
        ShaderProgram_ExperimentShader_B, //0x00C3
        ShaderProgram_TextureCompress_A, //0x00C4
        ShaderProgram_TextureCompress_DXT1, //0x00C5
        ShaderProgram_TextureCompress_DXT5, //0x00C6
        ShaderProgram_ErrorDifferenceSurface_RGB, //0x00C7
        ShaderProgram_ErrorDifferenceSurface_Alpha, //0x00C8
        ShaderProgram_XenonEncodeDXT5MemExport, //0x00C9
        ShaderProgram_TerrainEncodeSurface_A, //0x00CA
        ShaderProgram_TerrainEncodeSurface_B, //0x00CB
        ShaderProgram_TerrainPatchVertexYFetch, //0x00CC
        ShaderProgram_TerrainMeshScatteringYFetch, //0x00CD
        ShaderProgram_TexYCrCbA, //0x00CE
        ShaderProgram_RgbSeparation, //0x00CF
        ShaderProgram_Distort, //0x00D0
        ShaderProgram_DiffusionDofComputeRadius, //0x00D1
        ShaderProgram_DiffusionDofTridiagonalFill, //0x00D2
        ShaderProgram_DiffusionDofForwardCR, //0x00D3
        ShaderProgram_DiffusionDofBackwardCR, //0x00D4
        ShaderProgram_DiffusionDofSolveFinal, //0x00D5
        ShaderProgram_DiffusionDofOutputPS, //0x00D6
        ShaderProgram_DiffusionDofLinearRadiusPS, //0x00D7
        ShaderProgram_DiffusionDofNonLinearRadiusPS, //0x00D8
        ShaderProgram_DiffusionDofDetectForegroundPS, //0x00D9
        ShaderProgram_DiffusionDofCombineForegroundPS, //0x00DA
        ShaderProgram_DiffusionDofBlurXForegroundPS, //0x00DB
        ShaderProgram_DiffusionDofBlurYForegroundPS, //0x00DC
        ShaderProgram_Ui_DetailTexturedMinimap, //0x00DD
        ShaderProgramCount //0x00DE
    };

    ////////////////////////////////////////
    // RuntimeId: 01417
    // TypeInfo:  0x02BF0A7C
    enum QualityLevel
    {
        QualityLevel_Low, //0x0000
        QualityLevel_Medium, //0x0001
        QualityLevel_High, //0x0002
        QualityLevel_Ultra //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 01415
    // TypeInfo:  0x02BF0A6C
    enum ShaderRenderPath
    {
        ShaderRenderPath_Dx10, //0x0000
        ShaderRenderPath_Dx10Plus, //0x0001
        ShaderRenderPath_Dx10_1, //0x0002
        ShaderRenderPath_Dx11, //0x0003
        ShaderRenderPath_Xenon, //0x0004
        ShaderRenderPath_Ps3, //0x0005
        ShaderRenderPath_Gl, //0x0006
        ShaderRenderPathCount //0x0007
    };

    ////////////////////////////////////////
    // RuntimeId: 01413
    // TypeInfo:  0x02BF0A5C
    enum ShaderStageType
    {
        ShaderStageType_Vertex, //0x0000
        ShaderStageType_Pixel, //0x0001
        ShaderStageType_Hull, //0x0002
        ShaderStageType_Domain, //0x0003
        ShaderStageType_Geometry, //0x0004
        ShaderStageType_Compute, //0x0005
        ShaderStageTypeCount //0x0006
    };

    ////////////////////////////////////////
    // RuntimeId: 01411
    // TypeInfo:  0x02BF0A4C
    enum StencilOperation
    {
        StencilOperation_Keep, //0x0000
        StencilOperation_Zero, //0x0001
        StencilOperation_Replace, //0x0002
        StencilOperation_IncrementSaturate, //0x0003
        StencilOperation_DecrementSaturate, //0x0004
        StencilOperation_Invert, //0x0005
        StencilOperation_IncrementWrap, //0x0006
        StencilOperation_DecrementWrap //0x0007
    };

    ////////////////////////////////////////
    // RuntimeId: 01409
    // TypeInfo:  0x02BF0A3C
    enum DepthStencilCompareFunc
    {
        DepthStencilCompareFunc_Never, //0x0000
        DepthStencilCompareFunc_Less, //0x0001
        DepthStencilCompareFunc_Equal, //0x0002
        DepthStencilCompareFunc_LessEqual, //0x0003
        DepthStencilCompareFunc_Greater, //0x0004
        DepthStencilCompareFunc_NotEqual, //0x0005
        DepthStencilCompareFunc_GreaterEqual, //0x0006
        DepthStencilCompareFunc_Always //0x0007
    };

    ////////////////////////////////////////
    // RuntimeId: 01407
    // TypeInfo:  0x02BF0A2C
    enum RenderFillMode
    {
        RenderFillMode_Solid, //0x0000
        RenderFillMode_Wireframe //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 01405
    // TypeInfo:  0x02BF0A1C
    enum RenderDepthMode
    {
        RenderDepthMode_Disabled, //0x0000
        RenderDepthMode_ReadOnly, //0x0001
        RenderDepthMode_ReadAndWrite, //0x0002
        RenderDepthMode_WriteOnly //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 01403
    // TypeInfo:  0x02BF0A0C
    enum RenderClearMask
    {
        RenderClearMask_Color0, //0x0000
        RenderClearMask_Color1, //0x0001
        RenderClearMask_Color2, //0x0002
        RenderClearMask_Color3, //0x0003
        RenderClearMask_Color4, //0x0004
        RenderClearMask_Color5, //0x0005
        RenderClearMask_Color6, //0x0006
        RenderClearMask_Color7, //0x0007
        RenderClearMask_Depth, //0x0008
        RenderClearMask_Stencil, //0x0009
        RenderClearMask_Color, //0x000A
        RenderClearMask_All //0x000B
    };

    ////////////////////////////////////////
    // RuntimeId: 01401
    // TypeInfo:  0x02BF09FC
    enum RenderWriteMask
    {
        RenderWriteMask_Red, //0x0000
        RenderWriteMask_Green, //0x0001
        RenderWriteMask_Blue, //0x0002
        RenderWriteMask_Alpha, //0x0003
        RenderWriteMask_None, //0x0004
        RenderWriteMask_Color, //0x0005
        RenderWriteMask_All //0x0006
    };

    ////////////////////////////////////////
    // RuntimeId: 01399
    // TypeInfo:  0x02BF09EC
    enum RenderBlendOp
    {
        RenderBlendOp_Add, //0x0000
        RenderBlendOp_Subtract, //0x0001
        RenderBlendOp_RevSubtract, //0x0002
        RenderBlendOp_Min, //0x0003
        RenderBlendOp_Max //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 01397
    // TypeInfo:  0x02BF09DC
    enum RenderBlendMode
    {
        RenderBlendMode_Zero, //0x0000
        RenderBlendMode_One, //0x0001
        RenderBlendMode_SourceColor, //0x0002
        RenderBlendMode_InvSourceColor, //0x0003
        RenderBlendMode_SourceAlpha, //0x0004
        RenderBlendMode_InvSourceAlpha, //0x0005
        RenderBlendMode_DestColor, //0x0006
        RenderBlendMode_InvDestColor, //0x0007
        RenderBlendMode_DestAlpha, //0x0008
        RenderBlendMode_InvDestAlpha, //0x0009
        RenderBlendMode_SourceAlphaSaturate //0x000A
    };

    ////////////////////////////////////////
    // RuntimeId: 01395
    // TypeInfo:  0x02BF09CC
    enum RenderCullMode
    {
        RenderCullMode_None, //0x0000
        RenderCullMode_Front, //0x0001
        RenderCullMode_Back //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01393
    // TypeInfo:  0x02BF09BC
    enum IndexBufferFormat
    {
        IndexBufferFormat_16Bit, //0x0000
        IndexBufferFormat_32Bit //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 01391
    // TypeInfo:  0x02BF09AC
    enum PrimitiveType
    {
        PrimitiveType_PointList, //0x0000
        PrimitiveType_LineList, //0x0001
        PrimitiveType_LineStrip, //0x0002
        PrimitiveType_TriangleList, //0x0003
        PrimitiveType_TriangleStrip, //0x0004
        PrimitiveType_QuadList, //0x0005
        PrimitiveType_XenonRectList //0x0006
    };

    ////////////////////////////////////////
    // RuntimeId: 01389
    // TypeInfo:  0x02BF099C
    enum TextureFilter
    {
        TfNone, //0x0000
        TfPoint, //0x0001
        TfLinear, //0x0002
        TfAnisotropic, //0x0003
        TfDefault //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 01387
    // TypeInfo:  0x02BF098C
    enum TextureAddress
    {
        TaWrap, //0x0000
        TaMirror, //0x0001
        TaClamp, //0x0002
        TaBorder, //0x0003
        TaMirrorOnce //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 01385
    // TypeInfo:  0x02BF097C
    enum TextureType
    {
        TextureType_1d, //0x0000
        TextureType_1dArray, //0x0001
        TextureType_2d, //0x0002
        TextureType_2dArray, //0x0003
        TextureType_Cube, //0x0004
        TextureType_3d //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 01383
    // TypeInfo:  0x02BF096C
    enum TextureFormat
    {
        TextureFormat_DXT1, //0x0000
        TextureFormat_DXT3, //0x0001
        TextureFormat_DXT5, //0x0002
        TextureFormat_DXT5A, //0x0003
        TextureFormat_DXN, //0x0004
        TextureFormat_RGB565, //0x0005
        TextureFormat_RGB888, //0x0006
        TextureFormat_ARGB1555, //0x0007
        TextureFormat_ARGB4444, //0x0008
        TextureFormat_ARGB8888, //0x0009
        TextureFormat_L8, //0x000A
        TextureFormat_L16, //0x000B
        TextureFormat_ABGR16, //0x000C
        TextureFormat_ABGR16F, //0x000D
        TextureFormat_ABGR32F, //0x000E
        TextureFormat_R16F, //0x000F
        TextureFormat_R32F, //0x0010
        TextureFormat_NormalDXN, //0x0011
        TextureFormat_NormalDXT1, //0x0012
        TextureFormat_NormalDXT5, //0x0013
        TextureFormat_NormalDXT5RGA, //0x0014
        TextureFormat_RG8, //0x0015
        TextureFormat_GR16, //0x0016
        TextureFormat_GR16F, //0x0017
        TextureFormat_D16, //0x0018
        TextureFormat_D24S8, //0x0019
        TextureFormat_D24FS8, //0x001A
        TextureFormat_D32F, //0x001B
        TextureFormat_ABGR32, //0x001C
        TextureFormat_GR32F, //0x001D
        TextureFormat_A2R10G10B10, //0x001E
        TextureFormat_Unknown //0x001F
    };

    ////////////////////////////////////////
    // RuntimeId: 01377
    // TypeInfo:  0x02BF051C
    enum MixerValueUIScale
    {
        MixerValueUIScale_Linear, //0x0000
        MixerValueUIScale_Logarithmic //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 01375
    // TypeInfo:  0x02BF050C
    enum MixerValueUIOrientation
    {
        MixerValueUIOrientation_Horizontal, //0x0000
        MixerValueUIOrientation_Vertical //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 01373
    // TypeInfo:  0x02BF04FC
    enum MixerValueUI
    {
        MixerValueUI_None, //0x0000
        MixerValueUI_Fader, //0x0001
        MixerValueUI_Knob, //0x0002
        MixerValueUI_EditBox //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 01341
    // TypeInfo:  0x02BF0258
    enum MixerValueAccumulateMode
    {
        MixerValueAccumulateMode_None, //0x0000
        MixerValueAccumulateMode_Min, //0x0001
        MixerValueAccumulateMode_Max //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01331
    // TypeInfo:  0x02BF01EC
    enum MixGroupPropertyType
    {
        MixGroupPropertyType_Gain, //0x0000
        MixGroupPropertyType_Pitch, //0x0001
        MixGroupPropertyTypeCount //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01327
    // TypeInfo:  0x02BF01B0
    enum MixGroupState
    {
        MixGroupState_Normal, //0x0000
        MixGroupState_Mute, //0x0001
        MixGroupState_Solo //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01319
    // TypeInfo:  0x02BF011C
    enum TimerMode
    {
        TimerMode_Repeating, //0x0000
        TimerMode_SingleShot //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 01309
    // TypeInfo:  0x02BF005C
    enum AngleUnit
    {
        AngleUnit_Radians, //0x0000
        AngleUnit_Degrees //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 01307
    // TypeInfo:  0x02BF004C
    enum SimpleTransformOperation
    {
        SimpleTransformOperation_Add, //0x0000
        SimpleTransformOperation_Subtract, //0x0001
        SimpleTransformOperation_Multiply, //0x0002
        SimpleTransformOperation_Divide, //0x0003
        SimpleTransformOperation_Sine, //0x0004
        SimpleTransformOperation_Cosine, //0x0005
        SimpleTransformOperation_ArcSine, //0x0006
        SimpleTransformOperation_ArcCosine, //0x0007
        SimpleTransformOperation_Modulus, //0x0008
        SimpleTransformOperation_Round, //0x0009
        SimpleTransformOperation_Floor, //0x000A
        SimpleTransformOperation_Ceiling, //0x000B
        SimpleTransformOperation_Power, //0x000C
        SimpleTransformOperation_SquareRoot, //0x000D
        SimpleTransformOperation_Absolute, //0x000E
        SimpleTransformOperation_Exp, //0x000F
        SimpleTransformOperation_Log, //0x0010
        SimpleTransformOperation_Log10, //0x0011
        SimpleTransformOperation_Tangent, //0x0012
        SimpleTransformOperation_ArcTangent //0x0013
    };

    ////////////////////////////////////////
    // RuntimeId: 01287
    // TypeInfo:  0x02BEFEB0
    enum NumberGeneratorMode
    {
        NumberGeneratorMode_RandomUniform //0x0000
    };

    ////////////////////////////////////////
    // RuntimeId: 01277
    // TypeInfo:  0x02BEFDF0
    enum LogicalExpressionOperator
    {
        LogicalExpressionOperator_And, //0x0000
        LogicalExpressionOperator_Or, //0x0001
        LogicalExpressionOperator_Nand, //0x0002
        LogicalExpressionOperator_Nor //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 01243
    // TypeInfo:  0x02BEFB20
    enum DebugRenderType
    {
        DebugRenderType_Text, //0x0000
        DebugRenderType_Plot, //0x0001
        DebugRenderType_Bar //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01235
    // TypeInfo:  0x02BEFA8C
    enum ConditionType
    {
        ConditionType_And, //0x0000
        ConditionType_Or, //0x0001
        ConditionType_XOr, //0x0002
        ConditionType_Equal, //0x0003
        ConditionType_Less, //0x0004
        ConditionType_Greater, //0x0005
        ConditionType_LessOrEqual, //0x0006
        ConditionType_GreaterOrEqual //0x0007
    };

    ////////////////////////////////////////
    // RuntimeId: 01204
    // TypeInfo:  0x02BEE820
    enum VoiceOverPronunciationFallback
    {
        VoiceOverPronunciationFallback_None, //0x0000
        VoiceOverPronunciationFallback_Master //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 01200
    // TypeInfo:  0x02BEE7E4
    enum VoiceOverConversationQueueGroupPolyphony
    {
        VoiceOverConversationQueueGroupPolyphony_Sources, //0x0000
        VoiceOverConversationQueueGroupPolyphony_Conversations //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 01194
    // TypeInfo:  0x02BEE798
    enum VoiceOverWaveNameTranslation
    {
        VoiceOverWaveNameTranslation_FullName, //0x0000
        VoiceOverWaveNameTranslation_PathOnly, //0x0001
        VoiceOverWaveNameTranslation_NameOnly //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01176
    // TypeInfo:  0x02BEE628
    enum VoiceOverConversationQueueMode
    {
        VoiceOverConversationQueueMode_Always, //0x0000
        VoiceOverConversationQueueMode_Never, //0x0001
        VoiceOverConversationQueueMode_SamePriority //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01174
    // TypeInfo:  0x02BEE618
    enum VoiceOverConversationInterruptMode
    {
        VoiceOverConversationInterruptMode_Disallow, //0x0000
        VoiceOverConversationInterruptMode_Allow, //0x0001
        VoiceOverConversationInterruptMode_AllowSame //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01168
    // TypeInfo:  0x02BEE5B0
    enum VoiceOverDialogTakeBehavior
    {
        VoiceOverDialogTakeBehavior_Start, //0x0000
        VoiceOverDialogTakeBehavior_Clip, //0x0001
        VoiceOverDialogTakeBehavior_Continuous //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01156
    // TypeInfo:  0x02BEE4FC
    enum VoiceOverContainerConditionMode
    {
        VoiceOverContainerConditionMode_All, //0x0000
        VoiceOverContainerConditionMode_Any //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 01152
    // TypeInfo:  0x02BEE4C0
    enum VoiceOverLogicFlowMode
    {
        VoiceOverLogicFlowMode_All, //0x0000
        VoiceOverLogicFlowMode_One //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 01142
    // TypeInfo:  0x02BEE400
    enum VoiceOverLabelCompareMode
    {
        VoiceOverLabelCompareMode_Any, //0x0000
        VoiceOverLabelCompareMode_All, //0x0001
        VoiceOverLabelCompareMode_One //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01140
    // TypeInfo:  0x02BEE3F0
    enum VoiceOverLabelSourceMode
    {
        VoiceOverLabelSourceMode_Combined, //0x0000
        VoiceOverLabelSourceMode_Shared //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 01112
    // TypeInfo:  0x02BEE1A4
    enum VoiceOverCompareExpressionType
    {
        VoiceOverCompareExpressionType_Equals, //0x0000
        VoiceOverCompareExpressionType_Differs, //0x0001
        VoiceOverCompareExpressionType_Greater, //0x0002
        VoiceOverCompareExpressionType_Less, //0x0003
        VoiceOverCompareExpressionType_GreaterOrEquals, //0x0004
        VoiceOverCompareExpressionType_LessOrEquals //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 01108
    // TypeInfo:  0x02BEE168
    enum VoiceOverArithmeticExpressionType
    {
        VoiceOverArithmeticExpressionType_Addition, //0x0000
        VoiceOverArithmeticExpressionType_Subtraction, //0x0001
        VoiceOverArithmeticExpressionType_Multiplication, //0x0002
        VoiceOverArithmeticExpressionType_Division, //0x0003
        VoiceOverArithmeticExpressionType_Difference //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 01080
    // TypeInfo:  0x02BEDF1C
    enum VoiceOverValueType
    {
        VoiceOverValueType_Boolean, //0x0000
        VoiceOverValueType_Integer, //0x0001
        VoiceOverValueType_Float, //0x0002
        VoiceOverValueType_Vector, //0x0003
        VoiceOverValueType_Object, //0x0004
        VoiceOverValueTypeCount //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 01050
    // TypeInfo:  0x02BEDCC0
    enum RouteChannel
    {
        RouteChannel_0, //0x0000
        RouteChannel_1, //0x0001
        RouteChannel_2, //0x0002
        RouteChannel_3, //0x0003
        RouteChannel_4, //0x0004
        RouteChannel_5, //0x0005
        RouteChannel_6, //0x0006
        RouteChannel_7, //0x0007
        RouteChannel_8, //0x0008
        RouteChannel_9, //0x0009
        RouteChannel_10, //0x000A
        RouteChannel_11, //0x000B
        RouteChannel_12, //0x000C
        RouteChannel_13, //0x000D
        RouteChannel_14, //0x000E
        RouteChannel_15, //0x000F
        RouteChannel_16, //0x0010
        RouteChannel_17, //0x0011
        RouteChannel_18, //0x0012
        RouteChannel_19, //0x0013
        RouteChannel_21, //0x0014
        RouteChannel_22, //0x0015
        RouteChannel_23, //0x0016
        RouteChannel_24, //0x0017
        RouteChannel_25, //0x0018
        RouteChannel_26, //0x0019
        RouteChannel_27, //0x001A
        RouteChannel_28, //0x001B
        RouteChannel_29, //0x001C
        RouteChannel_30, //0x001D
        RouteChannel_31, //0x001E
        RouteChannel_32, //0x001F
        RouteChannel_33, //0x0020
        RouteChannel_34, //0x0021
        RouteChannel_35, //0x0022
        RouteChannel_36, //0x0023
        RouteChannel_37, //0x0024
        RouteChannel_38, //0x0025
        RouteChannel_39, //0x0026
        RouteChannel_40, //0x0027
        RouteChannel_41, //0x0028
        RouteChannel_42, //0x0029
        RouteChannel_43, //0x002A
        RouteChannel_44, //0x002B
        RouteChannel_45, //0x002C
        RouteChannel_46, //0x002D
        RouteChannel_47, //0x002E
        RouteChannel_48, //0x002F
        RouteChannel_49, //0x0030
        RouteChannel_50, //0x0031
        RouteChannel_51, //0x0032
        RouteChannel_52, //0x0033
        RouteChannel_53, //0x0034
        RouteChannel_54, //0x0035
        RouteChannel_55, //0x0036
        RouteChannel_56, //0x0037
        RouteChannel_57, //0x0038
        RouteChannel_58, //0x0039
        RouteChannel_59, //0x003A
        RouteChannel_60, //0x003B
        RouteChannel_61, //0x003C
        RouteChannel_62, //0x003D
        RouteChannel_63 //0x003E
    };

    ////////////////////////////////////////
    // RuntimeId: 01028
    // TypeInfo:  0x02BEDAF8
    enum OutputTransformSource
    {
        OutputTransformSource_Sound, //0x0000
        OutputTransformSource_Listener, //0x0001
        OutputTransformSource_Output //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 01008
    // TypeInfo:  0x02BED95C
    enum LimiterChannelMode
    {
        LimiterChannelMode_Independent, //0x0000
        LimiterChannelMode_Grouped //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 00996
    // TypeInfo:  0x02BED870
    enum GainFaderFadeType
    {
        GainFaderFadeType_LinearAmplitude, //0x0000
        GainFaderFadeType_LinearPower, //0x0001
        GainFaderFadeType_SineAmplitude //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 00990
    // TypeInfo:  0x02BED808
    enum FrequencyShiftSsbFilter
    {
        FrequencyShiftSsbFilter_None, //0x0000
        FrequencyShiftSsbFilter_Fir64 //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 00982
    // TypeInfo:  0x02BED790
    enum ExpanderChannelMode
    {
        ExpanderChannelMode_Independent, //0x0000
        ExpanderChannelMode_Grouped //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 00968
    // TypeInfo:  0x02BED678
    enum CompressorChannelMode
    {
        CompressorChannelMode_Independent, //0x0000
        CompressorChannelMode_Grouped //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 00936
    // TypeInfo:  0x02BED444
    enum SoundDataState
    {
        SoundDataState_Pending, //0x0000
        SoundDataState_Cancelled, //0x0001
        SoundDataState_Lost, //0x0002
        SoundDataState_Valid //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 00934
    // TypeInfo:  0x02BED434
    enum SoundDataRequestResult
    {
        SoundDataRequestResult_NotRequested, //0x0000
        SoundDataRequestResult_Requested, //0x0001
        SoundDataRequestResult_Available //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 00932
    // TypeInfo:  0x02BED424
    enum SoundWaveVariationSelection
    {
        SwvsSequential, //0x0000
        SwvsRandom //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 00930
    // TypeInfo:  0x02BED414
    enum SoundWaveReadPriority
    {
        SoundWaveReadPriority_Unspecified, //0x0000
        SoundWaveReadPriority_Low, //0x0001
        SoundWaveReadPriority_Medium, //0x0002
        SoundWaveReadPriority_Normal, //0x0003
        SoundWaveReadPriority_High //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 00928
    // TypeInfo:  0x02BED404
    enum SoundWaveVoicePriority
    {
        SoundWaveVoicePriority_Unspecified, //0x0000
        SoundWaveVoicePriority_Low, //0x0001
        SoundWaveVoicePriority_Normal, //0x0002
        SoundWaveVoicePriority_High, //0x0003
        SoundWaveVoicePriority_Permanent //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 00926
    // TypeInfo:  0x02BED3F4
    enum WavePriority
    {
        WpNone, //0x0000
        WpLow, //0x0001
        WpNormal, //0x0002
        WpHigh, //0x0003
        WpPermanent //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 00914
    // TypeInfo:  0x02BED324
    enum AudioCurveType
    {
        AudioCurveType_Spline //0x0000
    };

    ////////////////////////////////////////
    // RuntimeId: 00892
    // TypeInfo:  0x02BED23C
    enum SoundGraphPluginConnectionType
    {
        SoundGraphPluginConnectionType_Unconnected, //0x0000
        SoundGraphPluginConnectionType_Voice, //0x0001
        SoundGraphPluginConnectionType_Bus //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 00888
    // TypeInfo:  0x02BED21C
    enum FadeCurveType
    {
        FadeCurveType_LinearAmplitude, //0x0000
        FadeCurveType_SineAmplitude, //0x0001
        FadeCurveType_LinearDecibel //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 00880
    // TypeInfo:  0x02BED1A4
    enum AudioSystemSpeakerSetup
    {
        FiveDotOne, //0x0000
        SevenDotOne //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 00854
    // TypeInfo:  0x02BECFA0
    enum SoundTestTransformBehavior
    {
        SoundTestTransformBehavior_Static //0x0000
    };

    ////////////////////////////////////////
    // RuntimeId: 00848
    // TypeInfo:  0x02BECF38
    enum SoundTestParamBehavior
    {
        SoundTestParamBehavior_Constant, //0x0000
        SoundTestParamBehavior_LinearRamp, //0x0001
        SoundTestParamBehavior_Random //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 00830
    // TypeInfo:  0x02BECDE4
    enum SoundPatchDefaultEvent
    {
        SoundPatchDefaultEvent_Start, //0x0000
        SoundPatchDefaultEvent_Stop, //0x0001
        SoundPatchDefaultEvent_EnterScope, //0x0002
        SoundPatchDefaultEvent_ForceInit //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 00814
    // TypeInfo:  0x02BECCBC
    enum StreamStarveMode
    {
        StreamStarveMode_Off, //0x0000
        StreamStarveMode_On, //0x0001
        StreamStarveMode_Dynamic //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 00812
    // TypeInfo:  0x02BECCAC
    enum WaveCodec
    {
        WaveCodec_EaLayer3, //0x0000
        WaveCodec_EaXma, //0x0001
        WaveCodec_Xas, //0x0002
        WaveCodec_EaSpeex, //0x0003
        WaveCodec_Pcm, //0x0004
        WaveCodec_Count //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 00802
    // TypeInfo:  0x02BECC08
    enum LoopType
    {
        LtNone, //0x0000
        LtForward, //0x0001
        LtRelease, //0x0002
        LtInstantRelease //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 00747
    // TypeInfo:  0x02BE9F40
    enum ForceMagnitudeInputType
    {
        FMITYaw, //0x0000
        FMITPitch, //0x0001
        FMITRoll, //0x0002
        FMITThrottle //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 00745
    // TypeInfo:  0x02BE9F30
    enum PropellerType
    {
        PropellerType_Regular, //0x0000
        PropellerType_Rotor //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 00733
    // TypeInfo:  0x02BE9E60
    enum VehicleInputTweakType
    {
        CombinedTimedSpeedTweakType, //0x0000
        CombinedSpeedTweakType, //0x0001
        SpeedTweakType, //0x0002
        ScaledSpeedTweakType //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 00727
    // TypeInfo:  0x02BE9DF8
    enum StabilizerProperty
    {
        SPPitchAngle, //0x0000
        SPYawAngle, //0x0001
        SPRollAngle, //0x0002
        SPVerticalPosition, //0x0003
        SPVerticalVelocity //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 00703
    // TypeInfo:  0x02BE9C20
    enum SpaceType
    {
        STBody, //0x0000
        STWorld //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 00701
    // TypeInfo:  0x02BE9C10
    enum ForceType
    {
        FTForce, //0x0000
        FTTorque //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 00699
    // TypeInfo:  0x02BE9C00
    enum ForceCondition
    {
        FCNever, //0x0000
        FCNotCriticalDamaged, //0x0001
        FCCriticalDamaged, //0x0002
        FCNotOccupied, //0x0003
        FCOccupied, //0x0004
        FCAlways //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 00689
    // TypeInfo:  0x02BE9B94
    enum VehicleMode
    {
        VmIdle, //0x0000
        VmEntering, //0x0001
        VmEntered, //0x0002
        VmStarting, //0x0003
        VmStarted, //0x0004
        VmStopping, //0x0005
        VmLeaving //0x0006
    };

    ////////////////////////////////////////
    // RuntimeId: 00673
    // TypeInfo:  0x02BE9A6C
    enum RigidBodyCollisionLayer
    {
        RigidBodyCollisionLayer_Invalid, //0x0000
        RigidBodyCollisionLayer_VehicleLayer, //0x0001
        RigidBodyCollisionLayer_VehicleAndCharacterCollisionLayer, //0x0002
        RigidBodyCollisionLayer_Size //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 00671
    // TypeInfo:  0x02BE9A5C
    enum RigidBodyQualityType
    {
        RigidBodyQualityType_Invalid, //0x0000
        RigidBodyQualityType_Fixed, //0x0001
        RigidBodyQualityType_Debris, //0x0002
        RigidBodyQualityType_DebrisSimpleToi, //0x0003
        RigidBodyQualityType_Moving, //0x0004
        RigidBodyQualityType_Critical, //0x0005
        RigidBodyQualityType_Size //0x0006
    };

    ////////////////////////////////////////
    // RuntimeId: 00669
    // TypeInfo:  0x02BE9A4C
    enum RigidBodyMotionType
    {
        RigidBodyMotionType_Invalid, //0x0000
        RigidBodyMotionType_Fixed, //0x0001
        RigidBodyMotionType_Keyframed, //0x0002
        RigidBodyMotionType_Dynamic, //0x0003
        RigidBodyMotionType_Size //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 00667
    // TypeInfo:  0x02BE9A3C
    enum ShapeType
    {
        ShapeType_Hull, //0x0000
        ShapeType_Box, //0x0001
        ShapeType_OBB, //0x0002
        ShapeType_Sphere, //0x0003
        ShapeType_Cylinder, //0x0004
        ShapeType_Capsule, //0x0005
        ShapeType_Mesh, //0x0006
        ShapeType_Decompose, //0x0007
        ShapeType_Unknown //0x0008
    };

    ////////////////////////////////////////
    // RuntimeId: 00665
    // TypeInfo:  0x02BE9A2C
    enum RigidBodyType
    {
        RBTypeCollision, //0x0000
        RBTypeDetail, //0x0001
        RBTypeCharacter, //0x0002
        RBTypeRaycast, //0x0003
        RBTypeGroup, //0x0004
        RBTypeSize //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 00659
    // TypeInfo:  0x02BE99C4
    enum PhysicsWorldType
    {
        PhysicsWorldType_Client, //0x0000
        PhysicsWorldType_ClientEffect, //0x0001
        PhysicsWorldType_Server, //0x0002
        PhysicsWorldType_Unknown //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 00643
    // TypeInfo:  0x02BE9880
    enum ProximityObjectType
    {
        PotProximityDisabled, //0x0000
        PotVaultableLow, //0x0001
        PotVaultableHigh, //0x0002
        PotSupportedShooting, //0x0003
        PotInteractWith, //0x0004
        PotBashable //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 00611
    // TypeInfo:  0x02BE95F8
    enum CharacterStateType
    {
        CharacterStateType_OnGround, //0x0000
        CharacterStateType_Jumping, //0x0001
        CharacterStateType_InAir, //0x0002
        CharacterStateType_Climbing, //0x0003
        CharacterStateType_Falling, //0x0004
        CharacterStateType_User_0, //0x0005
        CharacterStateType_User_1, //0x0006
        CharacterStateType_User_2, //0x0007
        CharacterStateType_User_3, //0x0008
        CharacterStateType_User_4, //0x0009
        CharacterStateType_User_5, //0x000A
        CharacterStateType_StateCount, //0x000B
        CharacterStateType_Parachute, //0x000C
        CharacterStateType_Swimming, //0x000D
        CharacterStateType_AnimationControlled, //0x000E
        CharacterStateType_Sliding //0x000F
    };

    ////////////////////////////////////////
    // RuntimeId: 00599
    // TypeInfo:  0x02BE9544
    enum CharacterPoseCollisionType
    {
        CharacterPoseCollisionType_Capsule, //0x0000
        CharacterPoseCollisionType_Pencil //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 00597
    // TypeInfo:  0x02BE9534
    enum CharacterPoseType
    {
        CharacterPoseType_Stand, //0x0000
        CharacterPoseType_Crouch, //0x0001
        CharacterPoseType_Prone, //0x0002
        CharacterPoseTypeCount //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 00579
    // TypeInfo:  0x02BE9434
    enum DestructionConnectivityType
    {
        DestructionConnectivityType_Full, //0x0000
        DestructionConnectivityType_Partial, //0x0001
        DestructionConnectivityType_None, //0x0002
        DestructionConnectivityType_Inherited //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 00498
    // TypeInfo:  0x02BE7140
    enum PackagingDetailLevel
    {
        PackagingDetailLevel_Low, //0x0000
        PackagingDetailLevel_Medium, //0x0001
        PackagingDetailLevel_High, //0x0002
        PackagingDetailLevel_Base, //0x0003
        PackagingDetailLevel_Count //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 00428
    // TypeInfo:  0x02BE63E0
    enum AnimTangentType
    {
        kTangentFixed, //0x0000
        kTangentLinear, //0x0001
        kTangentFlat, //0x0002
        kTangentStep, //0x0003
        kTangentStepNext, //0x0004
        kTangentSlow, //0x0005
        kTangentFast, //0x0006
        kTangentSmooth, //0x0007
        kTangentClamped //0x0008
    };

    ////////////////////////////////////////
    // RuntimeId: 00426
    // TypeInfo:  0x02BE63D0
    enum CurveInfinityType
    {
        CurveInfinityType_Constant, //0x0000
        CurveInfinityType_Linear, //0x0001
        CurveInfinityType_Cycle, //0x0002
        CurveInfinityType_CycleWithOffset, //0x0003
        CurveInfinityType_Oscillate //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 00424
    // TypeInfo:  0x02BE63C0
    enum TransformPart
    {
        TransformPart_TranslationX, //0x0000
        TransformPart_TranslationY, //0x0001
        TransformPart_TranslationZ, //0x0002
        TransformPart_RotationX, //0x0003
        TransformPart_RotationY, //0x0004
        TransformPart_RotationZ //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 00390
    // TypeInfo:  0x02BE60F0
    enum InterpolationType
    {
        InterpolationType_None, //0x0000
        InterpolationType_Linear, //0x0001
        InterpolationType_CatmullRom, //0x0002
        InterpolationType_Curves //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 00378
    // TypeInfo:  0x02BE6020
    enum CompareOp
    {
        CompareOp_Equal, //0x0000
        CompareOp_NotEqual, //0x0001
        CompareOp_Greater, //0x0002
        CompareOp_Less, //0x0003
        CompareOp_GreaterOrEqual, //0x0004
        CompareOp_LessOrEqual //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 00372
    // TypeInfo:  0x02BE5FB8
    enum Vec3MathOp
    {
        Vec3MathOp_Add, //0x0000
        Vec3MathOp_Subtract, //0x0001
        Vec3MathOp_Multiply, //0x0002
        Vec3MathOp_Divide, //0x0003
        Vec3MathOp_Min, //0x0004
        Vec3MathOp_Max, //0x0005
        Vec3MathOp_Dot, //0x0006
        Vec3MathOp_Len, //0x0007
        Vec3MathOp_LenSq, //0x0008
        Vec3MathOp_Cross, //0x0009
        Vec3MathOp_Norm //0x000A
    };

    ////////////////////////////////////////
    // RuntimeId: 00370
    // TypeInfo:  0x02BE5FA8
    enum MathOp
    {
        MathOp_Add, //0x0000
        MathOp_Subtract, //0x0001
        MathOp_Multiply, //0x0002
        MathOp_Divide, //0x0003
        MathOp_Min, //0x0004
        MathOp_Max //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 00354
    // TypeInfo:  0x02BE5E64
    enum ModifierAxis
    {
        maLeft, //0x0000
        maUp, //0x0001
        maForward //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 00348
    // TypeInfo:  0x02BE5DFC
    enum ModifierEuler
    {
        Roll, //0x0000
        Pitch, //0x0001
        Yaw, //0x0002
        Trans //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 00274
    // TypeInfo:  0x02BE5810
    enum ExecutionModeType
    {
        ExecutionMode_Play, //0x0000
        ExecutionMode_GameView, //0x0001
        ExecutionMode_PlayFromHere //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 00248
    // TypeInfo:  0x02BE560C
    enum LinkDirection
    {
        LinkDirection_In, //0x0000
        LinkDirection_Out //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 00246
    // TypeInfo:  0x02BE55FC
    enum PropertyDirection
    {
        PropertyDirection_In, //0x0000
        PropertyDirection_Out //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 00244
    // TypeInfo:  0x02BE55EC
    enum EventDirection
    {
        EventDirection_In, //0x0000
        EventDirection_Out //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 00242
    // TypeInfo:  0x02BE55DC
    enum EntityUpdateOrder
    {
        EntityUpdateOrder_Async, //0x0000
        EntityUpdateOrder_Sync, //0x0001
        EntityUpdateOrder_Both //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 00240
    // TypeInfo:  0x02BE55CC
    enum UpdatePass
    {
        UpdatePass_PreSim, //0x0000
        UpdatePass_PostSim, //0x0001
        UpdatePass_PostFrame, //0x0002
        UpdatePass_FrameInterpolation, //0x0003
        UpdatePass_PreInput, //0x0004
        UpdatePass_PreFrame, //0x0005
        UpdatePass_Count //0x0006
    };

    ////////////////////////////////////////
    // RuntimeId: 00226
    // TypeInfo:  0x02BE54B4
    enum EntityInitPass
    {
        EntityInitPass_Early, //0x0000
        EntityInitPass_Normal, //0x0001
        EntityInitPass_Count //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 00224
    // TypeInfo:  0x02BE54A4
    enum EntityCreatorType
    {
        EntityCreatorType_Unknown, //0x0000
        EntityCreatorType_Level, //0x0001
        EntityCreatorType_Spawner, //0x0002
        EntityCreatorType_Owner, //0x0003
        EntityCreatorType_Ghost //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 00220
    // TypeInfo:  0x02BE5468
    enum IndexInBlueprint
    {
        IndexInBlueprint_HighestIndex, //0x0000
        IndexInBlueprint_Unknown //0x0001
    };

    ////////////////////////////////////////
    // RuntimeId: 00218
    // TypeInfo:  0x02BE5458
    enum SubRealm
    {
        SubRealm_All, //0x0000
        SubRealm_LocalPlayer, //0x0001
        SubRealm_RemotePlayer //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 00216
    // TypeInfo:  0x02BE5448
    enum Realm
    {
        Realm_Client, //0x0000
        Realm_Server, //0x0001
        Realm_ClientAndServer, //0x0002
        Realm_None, //0x0003
        Realm_Pipeline //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 00208
    // TypeInfo:  0x02BE53B4
    enum StreamRealm
    {
        StreamRealm_None, //0x0000
        StreamRealm_Client, //0x0001
        StreamRealm_Both //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 00180
    // TypeInfo:  0x02BE51BC
    enum EventConnectionTargetType
    {
        EventConnectionTargetType_Invalid, //0x0000
        EventConnectionTargetType_ClientAndServer, //0x0001
        EventConnectionTargetType_Client, //0x0002
        EventConnectionTargetType_Server, //0x0003
        EventConnectionTargetType_NetworkedClient, //0x0004
        EventConnectionTargetType_NetworkedClientAndServer //0x0005
    };

    ////////////////////////////////////////
    // RuntimeId: 00153
    // TypeInfo:  0x02BE358C
    enum WaypointVaultType
    {
        WaypointVaultType_VaultOverHigh, //0x0000
        WaypointVaultType_VaultOntoLow, //0x0001
        WaypointVaultType_VaultDownLow, //0x0002
        WaypointVaultType_VaultDownHigh, //0x0003
        WaypointVaultType_ClimbUpHigh, //0x0004
        WaypointVaultType_ClimbOverHigh, //0x0005
        WaypointVaultType_JumpAcross, //0x0006
        WaypointVaultType_Count //0x0007
    };

    ////////////////////////////////////////
    // RuntimeId: 00137
    // TypeInfo:  0x02BE3464
    enum AntPackagingType
    {
        AntPackagingType_Static, //0x0000
        AntPackagingType_Chunk, //0x0001
        AntPackagingType_Bundle, //0x0002
        AntPackagingType_AnimationSet //0x0003
    };

    ////////////////////////////////////////
    // RuntimeId: 00124
    // TypeInfo:  0x02BE2FF0
    enum ResourceBundleKind
    {
        ResourceBundleKind_AlwaysLoaded, //0x0000
        ResourceBundleKind_GlobalPackage, //0x0001
        ResourceBundleKind_AssetPackage, //0x0002
        ResourceBundleKind_SubLevelPackage, //0x0003
        ResourceBundleKind_BlueprintBundle //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 00114
    // TypeInfo:  0x02BE2F4C
    enum FieldAccessType
    {
        FieldAccessType_Source, //0x0000
        FieldAccessType_Target, //0x0001
        FieldAccessType_SourceAndTarget //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 00108
    // TypeInfo:  0x02BE2F1C
    enum LanguageFormat
    {
        LanguageFormat_English, //0x0000
        LanguageFormat_French, //0x0001
        LanguageFormat_German, //0x0002
        LanguageFormat_Spanish, //0x0003
        LanguageFormat_Italian, //0x0004
        LanguageFormat_Japanese, //0x0005
        LanguageFormat_Russian, //0x0006
        LanguageFormat_Polish, //0x0007
        LanguageFormat_Dutch, //0x0008
        LanguageFormat_Portuguese, //0x0009
        LanguageFormat_TraditionalChinese, //0x000A
        LanguageFormat_Korean, //0x000B
        LanguageFormat_Czech, //0x000C
        LanguageFormat_Count, //0x000D
        LanguageFormat_Undefined //0x000E
    };

    ////////////////////////////////////////
    // RuntimeId: 00067
    // TypeInfo:  0x02BE27F4
    enum BugSubmitTool
    {
        BST_None, //0x0000
        BST_FBBugsubmitter, //0x0001
        BST_B4Bug //0x0002
    };

    ////////////////////////////////////////
    // RuntimeId: 00065
    // TypeInfo:  0x02BE27E4
    enum HardwareProfile
    {
        Hardware_Autodetect, //0x0000
        Hardware_LowEnd, //0x0001
        Hardware_Medium, //0x0002
        Hardware_HighEnd, //0x0003
        Hardware_Maximum //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 00063
    // TypeInfo:  0x02BE27D4
    enum JuiceUserType
    {
        jutDev, //0x0000
        jutQA, //0x0001
        jutBuildMonkey, //0x0002
        jutJuiceDevelopment, //0x0003
        jutUnknown //0x0004
    };

    ////////////////////////////////////////
    // RuntimeId: 00061
    // TypeInfo:  0x02BE27C4
    enum CoreLogLevel
    {
        CllNone, //0x0000
        CllEventRecord, //0x0001
        CllCrash, //0x0002
        CllError, //0x0003
        CllAssert, //0x0004
        CllFatalAssert, //0x0005
        CllValidate, //0x0006
        CllWarning, //0x0007
        CllInfo, //0x0008
        CllOutput, //0x0009
        CllDebug //0x000A
    };


}