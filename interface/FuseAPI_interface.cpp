/*
 * WARNING: this is an autogenerated file and will be overwritten by
 * the extension interface script.
 */

#include "s3eExt.h"
#include "IwDebug.h"
#include "s3eDevice.h"


#include "FuseAPI.h"


#ifndef S3E_EXT_SKIP_LOADER_CALL_LOCK
// For MIPs (and WP8) platform we do not have asm code for stack switching
// implemented. So we make LoaderCallStart call manually to set GlobalLock
#if defined __mips || defined S3E_ANDROID_X86 || (defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP))
#define LOADER_CALL_LOCK
#endif
#endif

/**
 * Definitions for functions types passed to/from s3eExt interface
 */
typedef  s3eResult(*FuseAPIRegister_t)(FuseAPICallback cbid, s3eCallback fn, void* userData);
typedef  s3eResult(*FuseAPIUnRegister_t)(FuseAPICallback cbid, s3eCallback fn);
typedef       void(*FuseAPIStartSession_t)(const char* key);
typedef       void(*FuseAPIPauseSession_t)();
typedef       void(*FuseAPIResumeSession_t)();
typedef       void(*FuseAPITerminateSession_t)();
typedef       void(*FuseAPIRegisterForPushNotifications_t)(const char* projectID);
typedef       void(*FuseAPIRegisterEvent_t)(const char* message);
typedef        int(*FuseAPIRegisterEventWithParam_t)(const char* name, const char* param_name, const char* param_value, const char* variable_name, const double* variable_value);
typedef        int(*FuseAPIRegisterEventWithEventData_t)(const char* name, const char* paramName, const char* paramValue, cfuhash_table_t* eventData);
typedef       void(*FuseAPIRegisterEventWithDictionary_t)(const char* message, cfuhash_table_t* eventData);
typedef       void(*FuseAPIRegisterInAppPurchaseAndroid_t)(FusePurchaseState purchaseState, const char* purchaseToken, const char* productId, const char* orderId, long purchaseTime, const char* developerPayload, const double* price, const char* currency);
typedef       void(*FuseAPIRegisterInAppPurchaseiOS_t)(FusePurchaseState purchaseState, const char* receiptData, int recieptDataLength, double* price, const char* currency, const char* productID);
typedef       void(*FuseAPICheckAdAvailable_t)();
typedef       void(*FuseAPIShowAd_t)();
typedef       void(*FuseAPIDisplayNotifications_t)();
typedef       void(*FuseAPIDisplayMoreGames_t)();
typedef       void(*FuseAPIRegisterGender_t)(int gender);
typedef       void(*FuseAPIFacebookLogin_t)(const char* facebookId, const char* name, const char* accessToken);
typedef       void(*FuseAPITwitterLogin_t)(const char* twitterId);
typedef       void(*FuseAPIDeviceLogin_t)(const char* alias);
typedef       void(*FuseAPIFuseLogin_t)(const char* fuseId, const char* alias);
typedef       void(*FuseAPIGameCenterLogin_t)();
typedef       void(*FuseAPIGooglePlayLogin_t)(const char* alias, const char* token);
typedef const char*(*FuseAPIGetOriginalAccountId_t)();
typedef const char*(*FuseAPIGetOriginalAccountAlias_t)();
typedef        int(*FuseAPIGetOriginalAccountType_t)();
typedef const char*(*FuseAPIGetFuseID_t)();
typedef        int(*FuseAPIgamesPlayed_t)();
typedef const char*(*FuseAPILibraryVersion_t)();
typedef       bool(*FuseAPIConnected_t)();
typedef       void(*FuseAPITimeFromServer_t)();
typedef       void(*FuseAPIEnableData_t)(bool enable);
typedef        int(*FuseAPISetGameData_t)(const char* key, const char* fuseId, cfuhash_table_t* gameData);
typedef        int(*FuseAPIGetGameData_t)(const char* key, const char* fuseId, const char** gameDataKeys, int numKeys);
typedef       void(*FuseAPIMigrateFriends_t)(const char* fuseId);
typedef       void(*FuseAPIUpdateFriendsListFromServer_t)();
typedef FusePlayer*(*FuseAPIGetFriendsList_t)(int* numPlayers);
typedef       void(*FuseAPIAddFriend_t)(const char* fuse_id);
typedef       void(*FuseAPIRemoveFriend_t)(const char* fuse_id);
typedef       void(*FuseAPIAcceptFriend_t)(const char* fuse_id);
typedef       void(*FuseAPIRejectFriend_t)(const char* fuse_id);
typedef       void(*FuseAPIUserPushNotification_t)(const char* fuse_id, const char* message);
typedef       void(*FuseAPIFriendsPushNotification_t)(const char* message);
typedef       void(*FuseAPIGetMailListFromServer_t)();
typedef       void(*FuseAPIGetMailListFriendFromServer_t)(const char* fuseId);
typedef       void(*FuseAPISetMailAsReceived_t)(int messageId);
typedef        int(*FuseAPISendMailWithGift_t)(const char* fuseId, const char* message, int giftId, int giftAmount);
typedef        int(*FuseAPISendMail_t)(const char* fuseId, const char* message);
typedef  FuseMail*(*FuseAPIGetMailList_t)(int* numEntries);
typedef const char*(*FuseAPIGetGameConfigurationValue_t)(const char* key);
typedef       void(*FuseAPIRegisterLevel_t)(int level);
typedef       void(*FuseAPIRegisterCurrency_t)(int type, int balance);
typedef       void(*FuseAPIRegisterFlurryView_t)();
typedef       void(*FuseAPIRegisterFlurryClick_t)();
typedef       void(*FuseAPIRegisterTapjoyReward_t)(int amount);

/**
 * struct that gets filled in by FuseAPIRegister
 */
typedef struct FuseAPIFuncs
{
    FuseAPIRegister_t m_FuseAPIRegister;
    FuseAPIUnRegister_t m_FuseAPIUnRegister;
    FuseAPIStartSession_t m_FuseAPIStartSession;
    FuseAPIPauseSession_t m_FuseAPIPauseSession;
    FuseAPIResumeSession_t m_FuseAPIResumeSession;
    FuseAPITerminateSession_t m_FuseAPITerminateSession;
    FuseAPIRegisterForPushNotifications_t m_FuseAPIRegisterForPushNotifications;
    FuseAPIRegisterEvent_t m_FuseAPIRegisterEvent;
    FuseAPIRegisterEventWithParam_t m_FuseAPIRegisterEventWithParam;
    FuseAPIRegisterEventWithEventData_t m_FuseAPIRegisterEventWithEventData;
    FuseAPIRegisterEventWithDictionary_t m_FuseAPIRegisterEventWithDictionary;
    FuseAPIRegisterInAppPurchaseAndroid_t m_FuseAPIRegisterInAppPurchaseAndroid;
    FuseAPIRegisterInAppPurchaseiOS_t m_FuseAPIRegisterInAppPurchaseiOS;
    FuseAPICheckAdAvailable_t m_FuseAPICheckAdAvailable;
    FuseAPIShowAd_t m_FuseAPIShowAd;
    FuseAPIDisplayNotifications_t m_FuseAPIDisplayNotifications;
    FuseAPIDisplayMoreGames_t m_FuseAPIDisplayMoreGames;
    FuseAPIRegisterGender_t m_FuseAPIRegisterGender;
    FuseAPIFacebookLogin_t m_FuseAPIFacebookLogin;
    FuseAPITwitterLogin_t m_FuseAPITwitterLogin;
    FuseAPIDeviceLogin_t m_FuseAPIDeviceLogin;
    FuseAPIFuseLogin_t m_FuseAPIFuseLogin;
    FuseAPIGameCenterLogin_t m_FuseAPIGameCenterLogin;
    FuseAPIGooglePlayLogin_t m_FuseAPIGooglePlayLogin;
    FuseAPIGetOriginalAccountId_t m_FuseAPIGetOriginalAccountId;
    FuseAPIGetOriginalAccountAlias_t m_FuseAPIGetOriginalAccountAlias;
    FuseAPIGetOriginalAccountType_t m_FuseAPIGetOriginalAccountType;
    FuseAPIGetFuseID_t m_FuseAPIGetFuseID;
    FuseAPIgamesPlayed_t m_FuseAPIgamesPlayed;
    FuseAPILibraryVersion_t m_FuseAPILibraryVersion;
    FuseAPIConnected_t m_FuseAPIConnected;
    FuseAPITimeFromServer_t m_FuseAPITimeFromServer;
    FuseAPIEnableData_t m_FuseAPIEnableData;
    FuseAPISetGameData_t m_FuseAPISetGameData;
    FuseAPIGetGameData_t m_FuseAPIGetGameData;
    FuseAPIMigrateFriends_t m_FuseAPIMigrateFriends;
    FuseAPIUpdateFriendsListFromServer_t m_FuseAPIUpdateFriendsListFromServer;
    FuseAPIGetFriendsList_t m_FuseAPIGetFriendsList;
    FuseAPIAddFriend_t m_FuseAPIAddFriend;
    FuseAPIRemoveFriend_t m_FuseAPIRemoveFriend;
    FuseAPIAcceptFriend_t m_FuseAPIAcceptFriend;
    FuseAPIRejectFriend_t m_FuseAPIRejectFriend;
    FuseAPIUserPushNotification_t m_FuseAPIUserPushNotification;
    FuseAPIFriendsPushNotification_t m_FuseAPIFriendsPushNotification;
    FuseAPIGetMailListFromServer_t m_FuseAPIGetMailListFromServer;
    FuseAPIGetMailListFriendFromServer_t m_FuseAPIGetMailListFriendFromServer;
    FuseAPISetMailAsReceived_t m_FuseAPISetMailAsReceived;
    FuseAPISendMailWithGift_t m_FuseAPISendMailWithGift;
    FuseAPISendMail_t m_FuseAPISendMail;
    FuseAPIGetMailList_t m_FuseAPIGetMailList;
    FuseAPIGetGameConfigurationValue_t m_FuseAPIGetGameConfigurationValue;
    FuseAPIRegisterLevel_t m_FuseAPIRegisterLevel;
    FuseAPIRegisterCurrency_t m_FuseAPIRegisterCurrency;
    FuseAPIRegisterFlurryView_t m_FuseAPIRegisterFlurryView;
    FuseAPIRegisterFlurryClick_t m_FuseAPIRegisterFlurryClick;
    FuseAPIRegisterTapjoyReward_t m_FuseAPIRegisterTapjoyReward;
} FuseAPIFuncs;

static FuseAPIFuncs g_Ext;
static bool g_GotExt = false;
static bool g_TriedExt = false;
static bool g_TriedNoMsgExt = false;

static bool _extLoad()
{
    if (!g_GotExt && !g_TriedExt)
    {
        s3eResult res = s3eExtGetHash(0xc8b97392, &g_Ext, sizeof(g_Ext));
        if (res == S3E_RESULT_SUCCESS)
            g_GotExt = true;
        else
            s3eDebugAssertShow(S3E_MESSAGE_CONTINUE_STOP_IGNORE,                 "error loading extension: FuseAPI");

        g_TriedExt = true;
        g_TriedNoMsgExt = true;
    }

    return g_GotExt;
}

static bool _extLoadNoMsg()
{
    if (!g_GotExt && !g_TriedNoMsgExt)
    {
        s3eResult res = s3eExtGetHash(0xc8b97392, &g_Ext, sizeof(g_Ext));
        if (res == S3E_RESULT_SUCCESS)
            g_GotExt = true;
        g_TriedNoMsgExt = true;
        if (g_TriedExt)
            g_TriedExt = true;
    }

    return g_GotExt;
}

s3eBool FuseAPIAvailable()
{
    _extLoadNoMsg();
    return g_GotExt ? S3E_TRUE : S3E_FALSE;
}

s3eResult FuseAPIRegister(FuseAPICallback cbid, s3eCallback fn, void* userData)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[0] func: FuseAPIRegister"));

    if (!_extLoad())
        return S3E_RESULT_ERROR;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    s3eResult ret = g_Ext.m_FuseAPIRegister(cbid, fn, userData);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return ret;
}

s3eResult FuseAPIUnRegister(FuseAPICallback cbid, s3eCallback fn)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[1] func: FuseAPIUnRegister"));

    if (!_extLoad())
        return S3E_RESULT_ERROR;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    s3eResult ret = g_Ext.m_FuseAPIUnRegister(cbid, fn);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return ret;
}

void FuseAPIStartSession(const char* key)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[2] func: FuseAPIStartSession"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIStartSession(key);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIPauseSession()
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[3] func: FuseAPIPauseSession"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIPauseSession();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIResumeSession()
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[4] func: FuseAPIResumeSession"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIResumeSession();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPITerminateSession()
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[5] func: FuseAPITerminateSession"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPITerminateSession();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIRegisterForPushNotifications(const char* projectID)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[6] func: FuseAPIRegisterForPushNotifications"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIRegisterForPushNotifications(projectID);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIRegisterEvent(const char* message)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[7] func: FuseAPIRegisterEvent"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIRegisterEvent(message);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

int FuseAPIRegisterEventWithParam(const char* name, const char* param_name, const char* param_value, const char* variable_name, const double* variable_value)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[8] func: FuseAPIRegisterEventWithParam"));

    if (!_extLoad())
		return 0;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    int ret = g_Ext.m_FuseAPIRegisterEventWithParam(name, param_name, param_value, variable_name, variable_value);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return ret;
}

int FuseAPIRegisterEventWithEventData(const char* name, const char* paramName, const char* paramValue, cfuhash_table_t* eventData)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[9] func: FuseAPIRegisterEventWithEventData"));

    if (!_extLoad())
		return 0;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    int ret = g_Ext.m_FuseAPIRegisterEventWithEventData(name, paramName, paramValue, eventData);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return ret;
}

void FuseAPIRegisterEventWithDictionary(const char* message, cfuhash_table_t* eventData)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[10] func: FuseAPIRegisterEventWithDictionary"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIRegisterEventWithDictionary(message, eventData);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIRegisterInAppPurchaseAndroid(FusePurchaseState purchaseState, const char* purchaseToken, const char* productId, const char* orderId, long purchaseTime, const char* developerPayload, const double* price, const char* currency)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[11] func: FuseAPIRegisterInAppPurchaseAndroid"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIRegisterInAppPurchaseAndroid(purchaseState, purchaseToken, productId, orderId, purchaseTime, developerPayload, price, currency);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIRegisterInAppPurchaseiOS(FusePurchaseState purchaseState, const char* receiptData, int recieptDataLength, double* price, const char* currency, const char* productID)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[12] func: FuseAPIRegisterInAppPurchaseiOS"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIRegisterInAppPurchaseiOS(purchaseState, receiptData, recieptDataLength, price, currency, productID);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPICheckAdAvailable()
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[13] func: FuseAPICheckAdAvailable"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPICheckAdAvailable();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIShowAd()
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[14] func: FuseAPIShowAd"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIShowAd();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIDisplayNotifications()
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[15] func: FuseAPIDisplayNotifications"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIDisplayNotifications();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIDisplayMoreGames()
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[16] func: FuseAPIDisplayMoreGames"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIDisplayMoreGames();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIRegisterGender(int gender)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[17] func: FuseAPIRegisterGender"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIRegisterGender(gender);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIFacebookLogin(const char* facebookId, const char* name, const char* accessToken)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[18] func: FuseAPIFacebookLogin"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIFacebookLogin(facebookId, name, accessToken);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPITwitterLogin(const char* twitterId)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[19] func: FuseAPITwitterLogin"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPITwitterLogin(twitterId);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIDeviceLogin(const char* alias)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[20] func: FuseAPIDeviceLogin"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIDeviceLogin(alias);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIFuseLogin(const char* fuseId, const char* alias)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[21] func: FuseAPIFuseLogin"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIFuseLogin(fuseId, alias);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIGameCenterLogin()
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[22] func: FuseAPIGameCenterLogin"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIGameCenterLogin();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIGooglePlayLogin(const char* alias, const char* token)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[23] func: FuseAPIGooglePlayLogin"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIGooglePlayLogin(alias, token);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

const char* FuseAPIGetOriginalAccountId()
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[24] func: FuseAPIGetOriginalAccountId"));

    if (!_extLoad())
		return NULL;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    const char* ret = g_Ext.m_FuseAPIGetOriginalAccountId();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return ret;
}

const char* FuseAPIGetOriginalAccountAlias()
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[25] func: FuseAPIGetOriginalAccountAlias"));

    if (!_extLoad())
		return NULL;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    const char* ret = g_Ext.m_FuseAPIGetOriginalAccountAlias();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return ret;
}

int FuseAPIGetOriginalAccountType()
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[26] func: FuseAPIGetOriginalAccountType"));

    if (!_extLoad())
		return 0;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    int ret = g_Ext.m_FuseAPIGetOriginalAccountType();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return ret;
}

const char* FuseAPIGetFuseID()
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[27] func: FuseAPIGetFuseID"));

    if (!_extLoad())
		return NULL;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    const char* ret = g_Ext.m_FuseAPIGetFuseID();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return ret;
}

int FuseAPIgamesPlayed()
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[28] func: FuseAPIgamesPlayed"));

    if (!_extLoad())
		return 0;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    int ret = g_Ext.m_FuseAPIgamesPlayed();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return ret;
}

const char* FuseAPILibraryVersion()
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[29] func: FuseAPILibraryVersion"));

    if (!_extLoad())
		return NULL;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    const char* ret = g_Ext.m_FuseAPILibraryVersion();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return ret;
}

bool FuseAPIConnected()
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[30] func: FuseAPIConnected"));

    if (!_extLoad())
		return false;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    bool ret = g_Ext.m_FuseAPIConnected();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return ret;
}

void FuseAPITimeFromServer()
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[31] func: FuseAPITimeFromServer"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPITimeFromServer();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIEnableData(bool enable)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[32] func: FuseAPIEnableData"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIEnableData(enable);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

int FuseAPISetGameData(const char* key, const char* fuseId, cfuhash_table_t* gameData)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[33] func: FuseAPISetGameData"));

    if (!_extLoad())
		return 0;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    int ret = g_Ext.m_FuseAPISetGameData(key, fuseId, gameData);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return ret;
}

int FuseAPIGetGameData(const char* key, const char* fuseId, const char** gameDataKeys, int numKeys)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[34] func: FuseAPIGetGameData"));

    if (!_extLoad())
		return 0;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    int ret = g_Ext.m_FuseAPIGetGameData(key, fuseId, gameDataKeys, numKeys);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return ret;
}

void FuseAPIMigrateFriends(const char* fuseId)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[35] func: FuseAPIMigrateFriends"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIMigrateFriends(fuseId);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIUpdateFriendsListFromServer()
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[36] func: FuseAPIUpdateFriendsListFromServer"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIUpdateFriendsListFromServer();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

FusePlayer* FuseAPIGetFriendsList(int* numPlayers)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[37] func: FuseAPIGetFriendsList"));

    if (!_extLoad())
		return NULL;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    FusePlayer* ret = g_Ext.m_FuseAPIGetFriendsList(numPlayers);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return ret;
}

void FuseAPIAddFriend(const char* fuse_id)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[38] func: FuseAPIAddFriend"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIAddFriend(fuse_id);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIRemoveFriend(const char* fuse_id)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[39] func: FuseAPIRemoveFriend"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIRemoveFriend(fuse_id);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIAcceptFriend(const char* fuse_id)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[40] func: FuseAPIAcceptFriend"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIAcceptFriend(fuse_id);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIRejectFriend(const char* fuse_id)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[41] func: FuseAPIRejectFriend"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIRejectFriend(fuse_id);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIUserPushNotification(const char* fuse_id, const char* message)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[42] func: FuseAPIUserPushNotification"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIUserPushNotification(fuse_id, message);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIFriendsPushNotification(const char* message)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[43] func: FuseAPIFriendsPushNotification"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIFriendsPushNotification(message);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIGetMailListFromServer()
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[44] func: FuseAPIGetMailListFromServer"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIGetMailListFromServer();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIGetMailListFriendFromServer(const char* fuseId)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[45] func: FuseAPIGetMailListFriendFromServer"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIGetMailListFriendFromServer(fuseId);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPISetMailAsReceived(int messageId)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[46] func: FuseAPISetMailAsReceived"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPISetMailAsReceived(messageId);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

int FuseAPISendMailWithGift(const char* fuseId, const char* message, int giftId, int giftAmount)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[47] func: FuseAPISendMailWithGift"));

    if (!_extLoad())
		return 0;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    int ret = g_Ext.m_FuseAPISendMailWithGift(fuseId, message, giftId, giftAmount);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return ret;
}

int FuseAPISendMail(const char* fuseId, const char* message)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[48] func: FuseAPISendMail"));

    if (!_extLoad())
		return 0;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    int ret = g_Ext.m_FuseAPISendMail(fuseId, message);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return ret;
}

FuseMail* FuseAPIGetMailList(int* numEntries)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[49] func: FuseAPIGetMailList"));

    if (!_extLoad())
		return NULL;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    FuseMail* ret = g_Ext.m_FuseAPIGetMailList(numEntries);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return ret;
}

const char* FuseAPIGetGameConfigurationValue(const char* key)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[50] func: FuseAPIGetGameConfigurationValue"));

    if (!_extLoad())
		return NULL;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    const char* ret = g_Ext.m_FuseAPIGetGameConfigurationValue(key);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return ret;
}

void FuseAPIRegisterLevel(int level)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[51] func: FuseAPIRegisterLevel"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIRegisterLevel(level);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIRegisterCurrency(int type, int balance)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[52] func: FuseAPIRegisterCurrency"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIRegisterCurrency(type, balance);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIRegisterFlurryView()
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[53] func: FuseAPIRegisterFlurryView"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIRegisterFlurryView();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIRegisterFlurryClick()
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[54] func: FuseAPIRegisterFlurryClick"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIRegisterFlurryClick();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void FuseAPIRegisterTapjoyReward(int amount)
{
    IwTrace(FUSEAPI_VERBOSE, ("calling FuseAPI[55] func: FuseAPIRegisterTapjoyReward"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_FuseAPIRegisterTapjoyReward(amount);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}
