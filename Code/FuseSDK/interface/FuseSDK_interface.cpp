/*
 * WARNING: this is an autogenerated file and will be overwritten by
 * the extension interface script.
 */

#include "s3eExt.h"
#include "IwDebug.h"
#include "s3eDevice.h"


#include "FuseSDK.h"


// Define S3E_EXT_SKIP_LOADER_CALL_LOCK on the user-side to skip LoaderCallStart/LoaderCallDone()-entry.
// e.g. in s3eNUI this is used for generic user-side IwUI-based implementation.
#ifndef S3E_EXT_SKIP_LOADER_CALL_LOCK
#if defined I3D_ARCH_MIPS || defined S3E_ANDROID_X86 || (defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)) || defined I3D_ARCH_NACLX86_64
// For platforms missing stack-switching (MIPS, WP8, Android-x86, NaCl, etc.) make loader-entry via LoaderCallStart/LoaderCallDone() on the user-side.
#define LOADER_CALL_LOCK
#endif
#endif

/**
 * Definitions for functions types passed to/from s3eExt interface
 */
typedef  s3eResult(*FuseSDKRegister_t)(FuseSDKCallback cbid, s3eCallback fn, void* userData);
typedef  s3eResult(*FuseSDKUnRegister_t)(FuseSDKCallback cbid, s3eCallback fn);
typedef       void(*FuseSDKStartSession_t)(const char* appID, cfuhash_table_t* options);
typedef       void(*FuseSDKPauseSession_t)();
typedef       void(*FuseSDKResumeSession_t)();
typedef       void(*FuseSDKTerminateSession_t)();
typedef       void(*FuseSDKRegisterForPushNotifications_t)(const char* projectID);
typedef       void(*FuseSDKPreloadAdForZoneID_t)(const char* zoneID);
typedef       bool(*FuseSDKIsAdAvailableForZoneID_t)(const char* zoneID);
typedef       void(*FuseSDKShowAdForZoneID_t)(const char* zoneID, cfuhash_table_t* options);
typedef       void(*FuseSDKSetRewardedVideoUserID_t)(const char* userID);
typedef       void(*FuseSDKRegisterInAppPurchaseAndroid_t)(FusePurchaseStateAndroid purchaseState, const char* purchaseToken, const char* productId, const char* orderId, long purchaseTime, const char* developerPayload, const double* price, const char* currency);
typedef       void(*FuseSDKRegisterInAppPurchaseiOS_t)(FusePurchaseStateiOS purchaseState, const char* receiptData, int recieptDataLength, double* price, const char* currency, const char* productID, const char* transactionID);
typedef       void(*FuseSDKRegisterVirtualGoodsPurchase_t)(int virtualGoodsID, int purchaseAmount, int currencyID);
typedef       void(*FuseSDKDisplayNotifications_t)();
typedef       void(*FuseSDKDisplayMoreGames_t)();
typedef       void(*FuseSDKRegisterGender_t)(int gender);
typedef       void(*FuseSDKFacebookLogin_t)(const char* facebookId, const char* name, const char* accessToken);
typedef       void(*FuseSDKTwitterLogin_t)(const char* twitterId);
typedef       void(*FuseSDKDeviceLogin_t)(const char* alias);
typedef       void(*FuseSDKFuseLogin_t)(const char* fuseId, const char* alias);
typedef       void(*FuseSDKGameCenterLogin_t)();
typedef       void(*FuseSDKGooglePlayLogin_t)(const char* alias, const char* token);
typedef const char*(*FuseSDKGetOriginalAccountId_t)();
typedef const char*(*FuseSDKGetOriginalAccountAlias_t)();
typedef        int(*FuseSDKGetOriginalAccountType_t)();
typedef const char*(*FuseSDKGetFuseID_t)();
typedef        int(*FuseSDKgamesPlayed_t)();
typedef const char*(*FuseSDKLibraryVersion_t)();
typedef       bool(*FuseSDKConnected_t)();
typedef       void(*FuseSDKTimeFromServer_t)();
typedef       void(*FuseSDKEnableData_t)(bool enable);
typedef const char*(*FuseSDKGetGameConfigurationValue_t)(const char* key);
typedef       void(*FuseSDKRegisterLevel_t)(int level);
typedef       void(*FuseSDKRegisterCurrency_t)(int type, int balance);
typedef       void(*FuseSDKRegisterParentalConsent_t)(bool enabled);
typedef       bool(*FuseSDKRegisterCustomEventInt_t)(int eventID, int eventValue);
typedef       bool(*FuseSDKRegisterCustomEventString_t)(int eventID, const char* eventValue);

/**
 * struct that gets filled in by FuseSDKRegister
 */
typedef struct FuseSDKFuncs
{
    FuseSDKRegister_t m_FuseSDKRegister;
    FuseSDKUnRegister_t m_FuseSDKUnRegister;
    FuseSDKStartSession_t m_FuseSDKStartSession;
    FuseSDKPauseSession_t m_FuseSDKPauseSession;
    FuseSDKResumeSession_t m_FuseSDKResumeSession;
    FuseSDKTerminateSession_t m_FuseSDKTerminateSession;
    FuseSDKRegisterForPushNotifications_t m_FuseSDKRegisterForPushNotifications;
    FuseSDKPreloadAdForZoneID_t m_FuseSDKPreloadAdForZoneID;
    FuseSDKIsAdAvailableForZoneID_t m_FuseSDKIsAdAvailableForZoneID;
    FuseSDKShowAdForZoneID_t m_FuseSDKShowAdForZoneID;
    FuseSDKSetRewardedVideoUserID_t m_FuseSDKSetRewardedVideoUserID;
    FuseSDKRegisterInAppPurchaseAndroid_t m_FuseSDKRegisterInAppPurchaseAndroid;
    FuseSDKRegisterInAppPurchaseiOS_t m_FuseSDKRegisterInAppPurchaseiOS;
    FuseSDKRegisterVirtualGoodsPurchase_t m_FuseSDKRegisterVirtualGoodsPurchase;
    FuseSDKDisplayNotifications_t m_FuseSDKDisplayNotifications;
    FuseSDKDisplayMoreGames_t m_FuseSDKDisplayMoreGames;
    FuseSDKRegisterGender_t m_FuseSDKRegisterGender;
    FuseSDKFacebookLogin_t m_FuseSDKFacebookLogin;
    FuseSDKTwitterLogin_t m_FuseSDKTwitterLogin;
    FuseSDKDeviceLogin_t m_FuseSDKDeviceLogin;
    FuseSDKFuseLogin_t m_FuseSDKFuseLogin;
    FuseSDKGameCenterLogin_t m_FuseSDKGameCenterLogin;
    FuseSDKGooglePlayLogin_t m_FuseSDKGooglePlayLogin;
    FuseSDKGetOriginalAccountId_t m_FuseSDKGetOriginalAccountId;
    FuseSDKGetOriginalAccountAlias_t m_FuseSDKGetOriginalAccountAlias;
    FuseSDKGetOriginalAccountType_t m_FuseSDKGetOriginalAccountType;
    FuseSDKGetFuseID_t m_FuseSDKGetFuseID;
    FuseSDKgamesPlayed_t m_FuseSDKgamesPlayed;
    FuseSDKLibraryVersion_t m_FuseSDKLibraryVersion;
    FuseSDKConnected_t m_FuseSDKConnected;
    FuseSDKTimeFromServer_t m_FuseSDKTimeFromServer;
    FuseSDKEnableData_t m_FuseSDKEnableData;
    FuseSDKGetGameConfigurationValue_t m_FuseSDKGetGameConfigurationValue;
    FuseSDKRegisterLevel_t m_FuseSDKRegisterLevel;
    FuseSDKRegisterCurrency_t m_FuseSDKRegisterCurrency;
    FuseSDKRegisterParentalConsent_t m_FuseSDKRegisterParentalConsent;
    FuseSDKRegisterCustomEventInt_t m_FuseSDKRegisterCustomEventInt;
    FuseSDKRegisterCustomEventString_t m_FuseSDKRegisterCustomEventString;
} FuseSDKFuncs;

static FuseSDKFuncs g_Ext;
static bool g_GotExt = false;
static bool g_TriedExt = false;
static bool g_TriedNoMsgExt = false;

static bool _extLoad()
{
    if (!g_GotExt && !g_TriedExt)
    {
        s3eResult res = s3eExtGetHash(0xc8b9be9a, &g_Ext, sizeof(g_Ext));
        if (res == S3E_RESULT_SUCCESS)
            g_GotExt = true;
        else
            s3eDebugAssertShow(S3E_MESSAGE_CONTINUE_STOP_IGNORE,                 "error loading extension: FuseSDK");

        g_TriedExt = true;
        g_TriedNoMsgExt = true;
    }

    return g_GotExt;
}

static bool _extLoadNoMsg()
{
    if (!g_GotExt && !g_TriedNoMsgExt)
    {
        s3eResult res = s3eExtGetHash(0xc8b9be9a, &g_Ext, sizeof(g_Ext));
        if (res == S3E_RESULT_SUCCESS)
            g_GotExt = true;
        g_TriedNoMsgExt = true;
        if (g_TriedExt)
            g_TriedExt = true;
    }

    return g_GotExt;
}

s3eBool FuseSDKAvailable()
{
    _extLoadNoMsg();
    return g_GotExt ? S3E_TRUE : S3E_FALSE;
}

s3eResult FuseSDKRegister(FuseSDKCallback cbid, s3eCallback fn, void* userData)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[0] func: FuseSDKRegister"));

    if (!_extLoad())
        return S3E_RESULT_ERROR;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKRegister);
#endif

    s3eResult ret = g_Ext.m_FuseSDKRegister(cbid, fn, userData);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKRegister);
#endif

    return ret;
}

s3eResult FuseSDKUnRegister(FuseSDKCallback cbid, s3eCallback fn)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[1] func: FuseSDKUnRegister"));

    if (!_extLoad())
        return S3E_RESULT_ERROR;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKUnRegister);
#endif

    s3eResult ret = g_Ext.m_FuseSDKUnRegister(cbid, fn);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKUnRegister);
#endif

    return ret;
}

void FuseSDKStartSession(const char* appID, cfuhash_table_t* options)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[2] func: FuseSDKStartSession"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKStartSession);
#endif

    g_Ext.m_FuseSDKStartSession(appID, options);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKStartSession);
#endif

    return;
}

void FuseSDKPauseSession()
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[3] func: FuseSDKPauseSession"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKPauseSession);
#endif

    g_Ext.m_FuseSDKPauseSession();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKPauseSession);
#endif

    return;
}

void FuseSDKResumeSession()
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[4] func: FuseSDKResumeSession"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKResumeSession);
#endif

    g_Ext.m_FuseSDKResumeSession();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKResumeSession);
#endif

    return;
}

void FuseSDKTerminateSession()
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[5] func: FuseSDKTerminateSession"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKTerminateSession);
#endif

    g_Ext.m_FuseSDKTerminateSession();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKTerminateSession);
#endif

    return;
}

void FuseSDKRegisterForPushNotifications(const char* projectID)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[6] func: FuseSDKRegisterForPushNotifications"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKRegisterForPushNotifications);
#endif

    g_Ext.m_FuseSDKRegisterForPushNotifications(projectID);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKRegisterForPushNotifications);
#endif

    return;
}

void FuseSDKPreloadAdForZoneID(const char* zoneID)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[7] func: FuseSDKPreloadAdForZoneID"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKPreloadAdForZoneID);
#endif

    g_Ext.m_FuseSDKPreloadAdForZoneID(zoneID);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKPreloadAdForZoneID);
#endif

    return;
}

bool FuseSDKIsAdAvailableForZoneID(const char* zoneID)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[8] func: FuseSDKIsAdAvailableForZoneID"));

    if (!_extLoad())
		return false;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKIsAdAvailableForZoneID);
#endif

    bool ret = g_Ext.m_FuseSDKIsAdAvailableForZoneID(zoneID);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKIsAdAvailableForZoneID);
#endif

    return ret;
}

void FuseSDKShowAdForZoneID(const char* zoneID, cfuhash_table_t* options)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[9] func: FuseSDKShowAdForZoneID"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKShowAdForZoneID);
#endif

    g_Ext.m_FuseSDKShowAdForZoneID(zoneID, options);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKShowAdForZoneID);
#endif

    return;
}

void FuseSDKSetRewardedVideoUserID(const char* userID)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[10] func: FuseSDKSetRewardedVideoUserID"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKSetRewardedVideoUserID);
#endif

    g_Ext.m_FuseSDKSetRewardedVideoUserID(userID);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKSetRewardedVideoUserID);
#endif

    return;
}

void FuseSDKRegisterInAppPurchaseAndroid(FusePurchaseStateAndroid purchaseState, const char* purchaseToken, const char* productId, const char* orderId, long purchaseTime, const char* developerPayload, const double* price, const char* currency)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[11] func: FuseSDKRegisterInAppPurchaseAndroid"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKRegisterInAppPurchaseAndroid);
#endif

    g_Ext.m_FuseSDKRegisterInAppPurchaseAndroid(purchaseState, purchaseToken, productId, orderId, purchaseTime, developerPayload, price, currency);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKRegisterInAppPurchaseAndroid);
#endif

    return;
}

void FuseSDKRegisterInAppPurchaseiOS(FusePurchaseStateiOS purchaseState, const char* receiptData, int recieptDataLength, double* price, const char* currency, const char* productID, const char* transactionID)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[12] func: FuseSDKRegisterInAppPurchaseiOS"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKRegisterInAppPurchaseiOS);
#endif

    g_Ext.m_FuseSDKRegisterInAppPurchaseiOS(purchaseState, receiptData, recieptDataLength, price, currency, productID, transactionID);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKRegisterInAppPurchaseiOS);
#endif

    return;
}

void FuseSDKRegisterVirtualGoodsPurchase(int virtualGoodsID, int purchaseAmount, int currencyID)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[13] func: FuseSDKRegisterVirtualGoodsPurchase"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKRegisterVirtualGoodsPurchase);
#endif

    g_Ext.m_FuseSDKRegisterVirtualGoodsPurchase(virtualGoodsID, purchaseAmount, currencyID);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKRegisterVirtualGoodsPurchase);
#endif

    return;
}

void FuseSDKDisplayNotifications()
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[14] func: FuseSDKDisplayNotifications"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKDisplayNotifications);
#endif

    g_Ext.m_FuseSDKDisplayNotifications();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKDisplayNotifications);
#endif

    return;
}

void FuseSDKDisplayMoreGames()
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[15] func: FuseSDKDisplayMoreGames"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKDisplayMoreGames);
#endif

    g_Ext.m_FuseSDKDisplayMoreGames();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKDisplayMoreGames);
#endif

    return;
}

void FuseSDKRegisterGender(int gender)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[16] func: FuseSDKRegisterGender"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKRegisterGender);
#endif

    g_Ext.m_FuseSDKRegisterGender(gender);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKRegisterGender);
#endif

    return;
}

void FuseSDKFacebookLogin(const char* facebookId, const char* name, const char* accessToken)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[17] func: FuseSDKFacebookLogin"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKFacebookLogin);
#endif

    g_Ext.m_FuseSDKFacebookLogin(facebookId, name, accessToken);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKFacebookLogin);
#endif

    return;
}

void FuseSDKTwitterLogin(const char* twitterId)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[18] func: FuseSDKTwitterLogin"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKTwitterLogin);
#endif

    g_Ext.m_FuseSDKTwitterLogin(twitterId);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKTwitterLogin);
#endif

    return;
}

void FuseSDKDeviceLogin(const char* alias)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[19] func: FuseSDKDeviceLogin"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKDeviceLogin);
#endif

    g_Ext.m_FuseSDKDeviceLogin(alias);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKDeviceLogin);
#endif

    return;
}

void FuseSDKFuseLogin(const char* fuseId, const char* alias)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[20] func: FuseSDKFuseLogin"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKFuseLogin);
#endif

    g_Ext.m_FuseSDKFuseLogin(fuseId, alias);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKFuseLogin);
#endif

    return;
}

void FuseSDKGameCenterLogin()
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[21] func: FuseSDKGameCenterLogin"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKGameCenterLogin);
#endif

    g_Ext.m_FuseSDKGameCenterLogin();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKGameCenterLogin);
#endif

    return;
}

void FuseSDKGooglePlayLogin(const char* alias, const char* token)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[22] func: FuseSDKGooglePlayLogin"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKGooglePlayLogin);
#endif

    g_Ext.m_FuseSDKGooglePlayLogin(alias, token);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKGooglePlayLogin);
#endif

    return;
}

const char* FuseSDKGetOriginalAccountId()
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[23] func: FuseSDKGetOriginalAccountId"));

    if (!_extLoad())
		return NULL;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKGetOriginalAccountId);
#endif

    const char* ret = g_Ext.m_FuseSDKGetOriginalAccountId();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKGetOriginalAccountId);
#endif

    return ret;
}

const char* FuseSDKGetOriginalAccountAlias()
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[24] func: FuseSDKGetOriginalAccountAlias"));

    if (!_extLoad())
		return NULL;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKGetOriginalAccountAlias);
#endif

    const char* ret = g_Ext.m_FuseSDKGetOriginalAccountAlias();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKGetOriginalAccountAlias);
#endif

    return ret;
}

int FuseSDKGetOriginalAccountType()
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[25] func: FuseSDKGetOriginalAccountType"));

    if (!_extLoad())
		return 0;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKGetOriginalAccountType);
#endif

    int ret = g_Ext.m_FuseSDKGetOriginalAccountType();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKGetOriginalAccountType);
#endif

    return ret;
}

const char* FuseSDKGetFuseID()
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[26] func: FuseSDKGetFuseID"));

    if (!_extLoad())
		return NULL;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKGetFuseID);
#endif

    const char* ret = g_Ext.m_FuseSDKGetFuseID();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKGetFuseID);
#endif

    return ret;
}

int FuseSDKgamesPlayed()
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[27] func: FuseSDKgamesPlayed"));

    if (!_extLoad())
		return 0;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKgamesPlayed);
#endif

    int ret = g_Ext.m_FuseSDKgamesPlayed();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKgamesPlayed);
#endif

    return ret;
}

const char* FuseSDKLibraryVersion()
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[28] func: FuseSDKLibraryVersion"));

    if (!_extLoad())
		return NULL;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKLibraryVersion);
#endif

    const char* ret = g_Ext.m_FuseSDKLibraryVersion();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKLibraryVersion);
#endif

    return ret;
}

bool FuseSDKConnected()
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[29] func: FuseSDKConnected"));

    if (!_extLoad())
		return false;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKConnected);
#endif

    bool ret = g_Ext.m_FuseSDKConnected();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKConnected);
#endif

    return ret;
}

void FuseSDKTimeFromServer()
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[30] func: FuseSDKTimeFromServer"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKTimeFromServer);
#endif

    g_Ext.m_FuseSDKTimeFromServer();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKTimeFromServer);
#endif

    return;
}

void FuseSDKEnableData(bool enable)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[31] func: FuseSDKEnableData"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKEnableData);
#endif

    g_Ext.m_FuseSDKEnableData(enable);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKEnableData);
#endif

    return;
}

const char* FuseSDKGetGameConfigurationValue(const char* key)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[32] func: FuseSDKGetGameConfigurationValue"));

    if (!_extLoad())
		return NULL;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKGetGameConfigurationValue);
#endif

    const char* ret = g_Ext.m_FuseSDKGetGameConfigurationValue(key);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKGetGameConfigurationValue);
#endif

    return ret;
}

void FuseSDKRegisterLevel(int level)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[33] func: FuseSDKRegisterLevel"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKRegisterLevel);
#endif

    g_Ext.m_FuseSDKRegisterLevel(level);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKRegisterLevel);
#endif

    return;
}

void FuseSDKRegisterCurrency(int type, int balance)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[34] func: FuseSDKRegisterCurrency"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKRegisterCurrency);
#endif

    g_Ext.m_FuseSDKRegisterCurrency(type, balance);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKRegisterCurrency);
#endif

    return;
}

void FuseSDKRegisterParentalConsent(bool enabled)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[35] func: FuseSDKRegisterParentalConsent"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKRegisterParentalConsent);
#endif

    g_Ext.m_FuseSDKRegisterParentalConsent(enabled);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKRegisterParentalConsent);
#endif

    return;
}

bool FuseSDKRegisterCustomEventInt(int eventID, int eventValue)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[36] func: FuseSDKRegisterCustomEventInt"));

    if (!_extLoad())
		return false;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKRegisterCustomEventInt);
#endif

    bool ret = g_Ext.m_FuseSDKRegisterCustomEventInt(eventID, eventValue);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKRegisterCustomEventInt);
#endif

    return ret;
}

bool FuseSDKRegisterCustomEventString(int eventID, const char* eventValue)
{
    IwTrace(FUSESDK_VERBOSE, ("calling FuseSDK[37] func: FuseSDKRegisterCustomEventString"));

    if (!_extLoad())
		return false;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, (void*)g_Ext.m_FuseSDKRegisterCustomEventString);
#endif

    bool ret = g_Ext.m_FuseSDKRegisterCustomEventString(eventID, eventValue);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, (void*)g_Ext.m_FuseSDKRegisterCustomEventString);
#endif

    return ret;
}
