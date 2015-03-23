/*
 * android-specific implementation of the FuseSDK extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "FuseSDK_internal.h"

#include "s3eEdk.h"
#include "s3eEdk_android.h"
#include <jni.h>
#include "IwDebug.h"

#include "cfuhash.h"

#ifdef __cplusplus
    extern "C" {
#endif

void __exidx_start() {}
void __exidx_end()   {}

#ifdef __cplusplus
    }
#endif


const char* kFuseSDKRewardedAdOptionKey_ShowPreRoll = "FuseRewardedAdOptionKey_ShowPreRoll"; // set "0" for false or "1" for true
const char* kFuseSDKRewardedAdOptionKey_ShowPostRoll = "FuseRewardedAdOptionKey_ShowPostRoll"; // set "0" for false or "1" for true
const char* kFuseSDKRewardedOptionKey_PreRollYesButtonText = "FuseRewardedOptionKey_PreRollYesButtonText";
const char* kFuseSDKRewardedOptionKey_PreRollNoButtonText = "FuseRewardedOptionKey_PreRollNoButtonText";
const char* kFuseSDKRewardedOptionKey_PostRollContinueButtonText = "FuseRewardedOptionKey_PostRollContinueButtonText";


static jobject g_Obj;
static jmethodID g_FuseSDKStartSession;
static jmethodID g_FuseSDKPauseSession;
static jmethodID g_FuseSDKResumeSession;
static jmethodID g_FuseSDKTerminateSession;
static jmethodID g_FuseSDKRegisterForPushNotifications;
static jmethodID g_FuseSDKRegisterInAppPurchase;
static jmethodID g_FuseSDKIsAdAvailableForZoneID;
static jmethodID g_FuseSDKPreloadAdForZoneID;
static jmethodID g_FuseSDKShowAdForZoneID;
static jmethodID g_FuseSDKDisplayNotifications;
static jmethodID g_FuseSDKDisplayMoreGames;
static jmethodID g_FuseSDKRegisterGender;
static jmethodID g_FuseSDKFacebookLogin;
static jmethodID g_FuseSDKTwitterLogin;
static jmethodID g_FuseSDKDeviceLogin;
static jmethodID g_FuseSDKFuseLogin;
static jmethodID g_FuseSDKGooglePlayLogin;
static jmethodID g_FuseSDKGetOriginalAccountId;
static jmethodID g_FuseSDKGetOriginalAccountAlias;
static jmethodID g_FuseSDKGetOriginalAccountType;
static jmethodID g_FuseSDKGetFuseID;
static jmethodID g_FuseSDKgamesPlayed;
static jmethodID g_FuseSDKLibraryVersion;
static jmethodID g_FuseSDKConnected;
static jmethodID g_FuseSDKTimeFromServer;
static jmethodID g_FuseSDKEnableData;
static jmethodID g_FuseSDKGetGameConfigurationValue;
static jmethodID g_FuseSDKRegisterLevel;
static jmethodID g_FuseSDKRegisterCurrency;

//----------------------------------------------------------------------------------------
/*
  FuseSDK Callback Functions

  A note about callback parameters:
  Callbacks have two arguments - void* systemData and void* userData
  FuseSDK callbacks will always pass information as systemData in the form of a 
  memory address to a struct - even when there is only one parameter.  
  Please see comments above each function for information on what structs to cast systemData to.
*/
//----------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------
// Session Callbacks
//-------------------------------------
// FuseSDKSessionStartReceived
// Params: none
//-------------------------------------
void FuseSDKSessionStartReceived(JNIEnv* env, jobject obj)
{
    IwTrace(FuseSDK, ("FuseSDKSessionStartReceived()"));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_SESSION_STARTED, 0, 0);
}

//-------------------------------------
// FuseSDKLoginError
// Params: int error
//-------------------------------------
void FuseSDKLoginError(JNIEnv* env, jobject obj, jint result)
{
	struct paramList
	{
		int error;
	};
	paramList params;
	params.error = result;
    IwTrace(FuseSDK, ("FuseSDKLoginError(%i)", result));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_SESSION_LOGIN_ERROR, &params, sizeof(paramList));
}
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
// Ad Callbacks
//-------------------------------------
// FuseSDKAdAvailabilityResponse
// Params: int available, int error
//-------------------------------------
void FuseSDKAdAvailabilityResponse(JNIEnv* env, jobject obj, jint available, jint error)
{
	struct paramList
	{
		int available;
		int error;
	};
	paramList params;
	params.available = available;
	params.error = error;

    IwTrace(FuseSDK, ("FuseSDKAdAvailabilityResponse(%i, %i)", available, error));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_AD_AVAILABILITY_RESPONSE, &params, sizeof(paramList));
}

//-------------------------------------
// FuseSDKAdWillClose
// Params: none
//-------------------------------------
void FuseSDKAdWillClose(JNIEnv* env, jobject obj)
{
	IwTrace(FuseSDK, ("FuseSDKAdWillClose()"));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_AD_WILL_CLOSE, 0, 0);
}

//-------------------------------------
// FuseSDKRewardedAdComplete
// Params: string preMessage, string rewardMessage, string rewardItem, int rewardAmount
//-------------------------------------
void FuseSDKRewardedAdComplete(JNIEnv* env, jobject obj, jstring preMessage, jstring rewardMessage, jstring rewardItem, int rewardAmount)
{
    struct paramList
    {
        const char* preMessage;
        const char* rewardMessage;
        const char* rewardItem;
        int rewardAmount;
    };
    paramList params;
    params.rewardAmount = rewardAmount;
    params.preMessage = s3eEdkGetStringUTF8Chars(preMessage);
    params.rewardMessage = s3eEdkGetStringUTF8Chars(rewardMessage);
    params.rewardItem = s3eEdkGetStringUTF8Chars(rewardItem);
    IwTrace(FuseSDK, ("FuseSDKRewardedAdComplete(%s, %s, %s, %i)", params.preMessage, params.rewardMessage, params.rewardItem, params.rewardAmount));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_REWARDED_AD_COMPLETED, &params, sizeof(paramList));
}

//-------------------------------------
// FuseSDKVirtualGoodsOfferAccepted
// Params: string purchaseCurrency, float purchasePrice, string itemName, int itemAmount
//-------------------------------------
void FuseSDKVirtualGoodsOfferAccepted(JNIEnv* env, jobject obj, jstring currency, float price, jstring itemName, int amount)
{
    struct paramList
    {
        const char* purchaseCurrency;
        float purchasePrice;
        const char* itemName;
        int itemAmount;
    };
    paramList params;
    params.itemAmount = amount;
    params.purchasePrice = price;
    params.purchaseCurrency = s3eEdkGetStringUTF8Chars(currency);
    params.itemName = s3eEdkGetStringUTF8Chars(itemName);
    IwTrace(FuseSDK, ("FuseSDKVirtualGoodsOfferAccepted(%s, %f, %s, %i)", params.purchaseCurrency, params.purchasePrice, params.itemName, params.itemAmount));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_VIRTUALGOODSOFFER_ACCEPTED, &params, sizeof(paramList));
}

//-------------------------------------
// FuseSDKIAPOfferAccepted
// Params: float productPrice, int itemAmount, string itemName, string productID
//-------------------------------------
void FuseSDKIAPOfferAccepted(JNIEnv* env, jobject obj, float price, int amount, jstring itemName, jstring productID)
{
    struct paramList
    {
        float productPrice;
        int itemAmount;
        const char* itemName;
        const char* productID;
    };
    paramList params;
    params.productPrice = price;
    params.itemAmount = amount;
    params.itemName = s3eEdkGetStringUTF8Chars(itemName);
    params.productID = s3eEdkGetStringUTF8Chars(productID);
    IwTrace(FuseSDK, ("FuseSDKIAPOfferAccepted(%f, %i, %s, %s)", params.productPrice, params.itemAmount, params.itemName, params.productID));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_IAPOFFER_ACCEPTED, &params, sizeof(paramList));
}


//----------------------------------------------------------------------------------------
// Game Data Callbacks

//-------------------------------------
// FuseSDKPurchaseVerification
// Params: int verified, string transaction_id, string original_transaction_id
//-------------------------------------
void FuseSDKPurchaseVerification(JNIEnv* env, jobject obj, int verified, jstring t_id, jstring o_id)
{
    struct paramList
    {
        int verified;
        const char* transaction_id;
        const char* original_transaction_id;
    };
    paramList params;
    params.verified = verified;
    params.transaction_id = s3eEdkGetStringUTF8Chars(t_id);
    params.original_transaction_id = s3eEdkGetStringUTF8Chars(o_id);
    IwTrace(FuseSDK, ("FuseSDKPurchaseVerification(%i, %s, %s)", params.verified, params.transaction_id, params.original_transaction_id));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_PURCHASE_VERIFIED, &params, sizeof(paramList));
}


//-------------------------------------
// FuseSDKNotificationAction
// Params: string action
//-------------------------------------
void FuseSDKNotificationAction(JNIEnv* env, jobject obj, jstring action)
{
	struct paramList
	{
		const char* action;
	};
	paramList params;
	params.action = s3eEdkGetStringUTF8Chars(action);
    IwTrace(FuseSDK, ("FuseSDKNotificationAction(%s)", params.action));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_NOTIFICATION_ACTION, &params, sizeof(paramList));
}

//-------------------------------------
// FuseSDKAccountLoginComplete
// Params: int accountType, string accountID
// Note: accountType is defined in FuseSDK.h as FuseAccountType
//-------------------------------------
void FuseSDKAccountLoginComplete(JNIEnv* env, jobject obj, int accountType, jstring accountID)
{
	struct paramList
	{
		int accountType;
		const char* accountID;
	};
	paramList params;
	params.accountType = accountType;
	params.accountID = s3eEdkGetStringUTF8Chars(accountID);
    IwTrace(FuseSDK, ("FuseSDKAccountLoginComplete(%i, %s)", params.accountType, params.accountID));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_ACCOUNT_LOGIN_COMPLETE, &params, sizeof(paramList));
}

//-------------------------------------
// FuseSDKTimeUpdated
// Params: int timestamp
//-------------------------------------
void FuseSDKTimeUpdated(JNIEnv* env, jobject obj, jint timestamp)
{
	struct paramList
	{
		int timestamp;
	};
	paramList params;
	params.timestamp = timestamp;
    IwTrace(FuseSDK, ("FuseSDKTimeUpdated(%i)", timestamp));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_TIME_UPDATED, &params, sizeof(paramList));
}

//-------------------------------------
// FuseGameConfigurationReceived
// Params: none
//-------------------------------------
cfuhash_table_t *gameConfig = NULL;
void FuseGameConfigurationStart(JNIEnv* env, jobject obj, jint numKeys)
{
	IwTrace(FuseSDK, ("FuseGameConfigurationStart()"));
	if( gameConfig == NULL )
	{
		gameConfig = cfuhash_new_with_initial_size(numKeys);
	}
	cfuhash_clear(gameConfig);	
}
void FuseGameConfigurationKeyValue(JNIEnv* env, jobject obj, jstring key, jstring value)
{
	IwTrace(FuseSDK, ("FuseGameConfigurationKeyValue(%s, %s)", s3eEdkGetStringUTF8Chars(key), s3eEdkGetStringUTF8Chars(value)));
	cfuhash_put(gameConfig, (const char*)s3eEdkGetStringUTF8Chars(key), (void*)s3eEdkGetStringUTF8Chars(value));
}
void FuseGameConfigurationReceived(JNIEnv* env, jobject obj)
{
	IwTrace(FuseSDK, ("FuseGameConfigurationReceived()"));

	struct paramList
	{
		cfuhash_table_t* gameConfig;
	};
	paramList params;
	params.gameConfig = gameConfig;

	s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_GAME_CONFIGURATION_RECEIVED, &params, sizeof(paramList));
}



int32 FusePause(void* systemData, void* userData)
{
	if( gameConfig != NULL )
	{
		cfuhash_destroy(gameConfig);
		gameConfig = NULL;
	}

    FuseSDKPauseSession_platform();

    return 1;
}

int32 FuseResume(void* systemData, void* userData)
{
    FuseSDKResumeSession_platform();

    return 1;
}

s3eResult FuseSDKInit_platform()
{
    // Get the environment from the pointer
    JNIEnv* env = s3eEdkJNIGetEnv();
    jobject obj = NULL;
    jmethodID cons = NULL;

	// callbacks
	const JNINativeMethod nativeMethodDefs[] =
    {
		{ "FuseSDKSessionStartReceived",				"()V",                              (void*)&FuseSDKSessionStartReceived },
		{ "FuseSDKLoginError",							"(I)V",                             (void*)&FuseSDKLoginError },
		{ "FuseSDKAdAvailabilityResponse",				"(II)V",                            (void*)&FuseSDKAdAvailabilityResponse },
		{ "FuseSDKAdWillClose",							"()V",                              (void*)&FuseSDKAdWillClose },
		{ "FuseSDKNotificationAction",	"(Ljava/lang/String;)V",                            (void*)&FuseSDKNotificationAction },
		{ "FuseSDKAccountLoginComplete","(ILjava/lang/String;)V",                           (void*)&FuseSDKAccountLoginComplete },
		{ "FuseSDKTimeUpdated",							"(I)V",                             (void*)&FuseSDKTimeUpdated },
		{ "FuseGameConfigurationStart",					"(I)V",                             (void*)&FuseGameConfigurationStart },
		{ "FuseGameConfigurationKeyValue", "(Ljava/lang/String;Ljava/lang/String;)V",       (void*)&FuseGameConfigurationKeyValue },
		{ "FuseGameConfigurationReceived",				"()V",                              (void*)&FuseGameConfigurationReceived },
        { "FuseSDKPurchaseVerification", "(ILjava/lang/String;Ljava/lang/String;)V",        (void*)&FuseSDKPurchaseVerification },
        { "FuseSDKRewardedAdComplete", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V", (void*)&FuseSDKRewardedAdComplete },
        { "FuseSDKVirtualGoodsOfferAccepted", "(Ljava/lang/String;FLjava/lang/String;I)V",  (void*)&FuseSDKVirtualGoodsOfferAccepted },
        { "FuseSDKIAPOfferAccepted", "(FILjava/lang/String;Ljava/lang/String;)V",           (void*)&FuseSDKIAPOfferAccepted },
	};

    // Get the extension class
    jclass cls = s3eEdkAndroidFindClass("com/fuseextension/FuseWrapper");
    if (!cls)
        goto fail;

    // Get its constructor
    cons = env->GetMethodID(cls, "<init>", "()V");
    if (!cons)
        goto fail;

    // Construct the java class
    obj = env->NewObject(cls, cons);
    if (!obj)
        goto fail;

    // Get all the extension methods
    g_FuseSDKStartSession = env->GetMethodID(cls, "FuseSDKStartSession", "(Ljava/lang/String;)V");
    if (!g_FuseSDKStartSession)
        goto fail; 

	g_FuseSDKPauseSession = env->GetMethodID(cls, "FuseSDKPauseSession", "()V");
    if (!g_FuseSDKPauseSession)
        goto fail;

	g_FuseSDKResumeSession = env->GetMethodID(cls, "FuseSDKResumeSession", "()V");
    if (!g_FuseSDKResumeSession)
        goto fail;

	g_FuseSDKTerminateSession = env->GetMethodID(cls, "FuseSDKTerminateSession", "()V");
    if (!g_FuseSDKTerminateSession)
        goto fail;

    g_FuseSDKRegisterForPushNotifications = env->GetMethodID(cls, "FuseSDKRegisterForPushNotifications", "(Ljava/lang/String;)V");
    if (!g_FuseSDKRegisterForPushNotifications)
        goto fail;

    g_FuseSDKRegisterInAppPurchase = env->GetMethodID(cls, "FuseSDKRegisterInAppPurchase", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;JLjava/lang/String;DLjava/lang/String;)V");
    if (!g_FuseSDKRegisterInAppPurchase)
        goto fail;

    g_FuseSDKIsAdAvailableForZoneID = env->GetMethodID(cls, "FuseSDKIsAdAvailableForZoneID", "(Ljava/lang/String;)Z");
    if (!g_FuseSDKIsAdAvailableForZoneID)
        goto fail;

    g_FuseSDKShowAdForZoneID = env->GetMethodID(cls, "FuseSDKShowAdForZoneID", "(Ljava/lang/String;ZZLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if (!g_FuseSDKShowAdForZoneID)
        goto fail;

	g_FuseSDKPreloadAdForZoneID = env->GetMethodID(cls, "FuseSDKPreloadAdForZoneID", "(Ljava/lang/String;)V");
    if (!g_FuseSDKPreloadAdForZoneID)
        goto fail;

    g_FuseSDKDisplayNotifications = env->GetMethodID(cls, "FuseSDKDisplayNotifications", "()V");
    if (!g_FuseSDKDisplayNotifications)
        goto fail;

    g_FuseSDKDisplayMoreGames = env->GetMethodID(cls, "FuseSDKDisplayMoreGames", "()V");
    if (!g_FuseSDKDisplayMoreGames)
        goto fail;

    g_FuseSDKRegisterGender = env->GetMethodID(cls, "FuseSDKRegisterGender", "(I)V");
    if (!g_FuseSDKRegisterGender)
        goto fail;

    g_FuseSDKFacebookLogin = env->GetMethodID(cls, "FuseSDKFacebookLogin", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if (!g_FuseSDKFacebookLogin)
        goto fail;

    g_FuseSDKTwitterLogin = env->GetMethodID(cls, "FuseSDKTwitterLogin", "(Ljava/lang/String;)V");
    if (!g_FuseSDKTwitterLogin)
        goto fail;

	g_FuseSDKDeviceLogin = env->GetMethodID(cls, "FuseSDKDeviceLogin", "(Ljava/lang/String;)V");
    if (!g_FuseSDKDeviceLogin)
        goto fail;	

    g_FuseSDKFuseLogin = env->GetMethodID(cls, "FuseSDKFuseLogin", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!g_FuseSDKFuseLogin)
        goto fail;

    g_FuseSDKGooglePlayLogin = env->GetMethodID(cls, "FuseSDKGooglePlayLogin", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!g_FuseSDKGooglePlayLogin)
        goto fail;

    g_FuseSDKGetOriginalAccountId = env->GetMethodID(cls, "FuseSDKGetOriginalAccountId", "()Ljava/lang/String;");
    if (!g_FuseSDKGetOriginalAccountId)
        goto fail;

    g_FuseSDKGetOriginalAccountAlias = env->GetMethodID(cls, "FuseSDKGetOriginalAccountAlias", "()Ljava/lang/String;");
    if (!g_FuseSDKGetOriginalAccountAlias)
        goto fail;

    g_FuseSDKGetOriginalAccountType = env->GetMethodID(cls, "FuseSDKGetOriginalAccountType", "()I");
    if (!g_FuseSDKGetOriginalAccountType)
        goto fail;

    g_FuseSDKGetFuseID = env->GetMethodID(cls, "FuseSDKGetFuseID", "()Ljava/lang/String;");
    if (!g_FuseSDKGetFuseID)
        goto fail;

    g_FuseSDKgamesPlayed = env->GetMethodID(cls, "FuseSDKgamesPlayed", "()I");
    if (!g_FuseSDKgamesPlayed)
        goto fail;

    g_FuseSDKLibraryVersion = env->GetMethodID(cls, "FuseSDKLibraryVersion", "()Ljava/lang/String;");
    if (!g_FuseSDKLibraryVersion)
        goto fail;

    g_FuseSDKConnected = env->GetMethodID(cls, "FuseSDKConnected", "()Z");
    if (!g_FuseSDKConnected)
        goto fail;

	g_FuseSDKTimeFromServer = env->GetMethodID(cls, "FuseSDKTimeFromServer", "()V");
    if (!g_FuseSDKTimeFromServer)
        goto fail;

    g_FuseSDKEnableData = env->GetMethodID(cls, "FuseSDKEnableData", "(Z)V");
    if (!g_FuseSDKEnableData)
        goto fail;

    g_FuseSDKGetGameConfigurationValue = env->GetMethodID(cls, "FuseSDKGetGameConfigurationValue", "(Ljava/lang/String;)Ljava/lang/String;");
    if (!g_FuseSDKGetGameConfigurationValue)
        goto fail;

    g_FuseSDKRegisterLevel = env->GetMethodID(cls, "FuseSDKRegisterLevel", "(I)V");
    if (!g_FuseSDKRegisterLevel)
        goto fail;

    g_FuseSDKRegisterCurrency = env->GetMethodID(cls, "FuseSDKRegisterCurrency", "(II)V");
    if (!g_FuseSDKRegisterCurrency)
        goto fail;

	if(env->RegisterNatives(cls, nativeMethodDefs, sizeof(nativeMethodDefs)/sizeof(nativeMethodDefs[0])))
		goto fail;

    IwTrace(FUSESDK, ("FUSESDK init success"));
    g_Obj = env->NewGlobalRef(obj);
    env->DeleteLocalRef(obj);
    env->DeleteGlobalRef(cls);

    // Add any platform-specific initialisation code here
	s3eDeviceRegister(S3E_DEVICE_PAUSE, &FusePause, 0);
	s3eDeviceRegister(S3E_DEVICE_UNPAUSE, &FuseResume, 0);

    return S3E_RESULT_SUCCESS;

fail:
    jthrowable exc = env->ExceptionOccurred();
    if (exc)
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
        IwTrace(FuseSDK, ("One or more java methods could not be found"));
    }
    return S3E_RESULT_ERROR;

}

void FuseSDKTerminate_platform()
{
    // Add any platform-specific termination code here
	if( gameConfig != NULL )
	{
		cfuhash_destroy(gameConfig);
		gameConfig = NULL;
	}

	FuseSDKTerminateSession_platform();
}

void FuseSDKStartSession_platform(const char* key, cfuhash_table_t* options)
{
	// there are no options on Android for now, so we won't pass them over to the SDK here
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring key_jstr = env->NewStringUTF(key);
    env->CallVoidMethod(g_Obj, g_FuseSDKStartSession, key_jstr);
}

void FuseSDKPauseSession_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseSDKPauseSession);
}

void FuseSDKResumeSession_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseSDKResumeSession);
}

void FuseSDKTerminateSession_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseSDKTerminateSession);
}

void FuseSDKRegisterForPushNotifications_platform(const char* projectID)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring projectID_jstr = env->NewStringUTF(projectID);
    env->CallVoidMethod(g_Obj, g_FuseSDKRegisterForPushNotifications, projectID_jstr);
}

void FuseSDKRegisterInAppPurchaseAndroid_platform(FusePurchaseStateAndroid purchaseState, const char* purchaseToken, const char* productId, const char* orderId, long purchaseTime, const char* developerPayload, const double* price, const char* currency)
{	
    JNIEnv* env = s3eEdkJNIGetEnv();    
	jint ps = (jint)purchaseState;
	jlong pt = (jlong)purchaseTime;
	jdouble pp = (jdouble)(*price);
    jstring purchaseToken_jstr = env->NewStringUTF(purchaseToken);
    jstring productId_jstr = env->NewStringUTF(productId);
    jstring orderId_jstr = env->NewStringUTF(orderId);
    jstring developerPayload_jstr = env->NewStringUTF(developerPayload);
    jstring currency_jstr = env->NewStringUTF(currency);
    env->CallVoidMethod(g_Obj, g_FuseSDKRegisterInAppPurchase, ps, purchaseToken_jstr, productId_jstr, orderId_jstr, pt, developerPayload_jstr, pp, currency_jstr);
}

void FuseSDKRegisterInAppPurchaseiOS_platform(FusePurchaseStateiOS purchaseState, const char* receiptData, int recieptDataLength, double* price, const char* currency, const char* productID, const char* transactionID)
{
    // iOS only
}

void FuseSDKPreloadAdForZoneID_platform(const char* zoneID)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring zoneID_jstr = env->NewStringUTF(zoneID);
    env->CallVoidMethod(g_Obj, g_FuseSDKPreloadAdForZoneID, zoneID_jstr);
}

bool FuseSDKIsAdAvailableForZoneID_platform(const char* zoneID)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring zoneID_jstr = env->NewStringUTF(zoneID);
	return (bool)env->CallBooleanMethod(g_Obj, g_FuseSDKIsAdAvailableForZoneID, zoneID_jstr);
}

void FuseSDKShowAdForZoneID_platform(const char* zoneID, cfuhash_table_t* options)
{
	// parse out the options here and pass them to Java
	bool showPre = true;
	bool showPost = true;
	const char* yesText = NULL;
	const char* noText = NULL;
	const char* continueText = NULL;

	int numOptions = cfuhash_num_entries(options);

	if( numOptions > 0 )
	{
		char* key = NULL;
		void* data = NULL;
		if( cfuhash_each(options, &key,&data) )
		{
			do
			{
				if( strcmp(key, kFuseSDKRewardedAdOptionKey_ShowPreRoll) == 0 )
				{
					if( strcmp((const char*)data, "0") == 0 || strcmp((const char*)data, "false") == 0 )
					{
						showPre = false;
					}
				}
				else if( strcmp(key, kFuseSDKRewardedAdOptionKey_ShowPostRoll) == 0 )
				{
					if( strcmp((const char*)data, "0") == 0 || strcmp((const char*)data, "false") == 0 )
					{
						showPost = false;
					}					
				}
				else if( strcmp(key, kFuseSDKRewardedOptionKey_PreRollYesButtonText) == 0 )
				{
					yesText = (const char*)data;					
				}
				else if( strcmp(key, kFuseSDKRewardedOptionKey_PreRollNoButtonText) == 0 )
				{
					noText = (const char*)data;
				}
				else if( strcmp(key, kFuseSDKRewardedOptionKey_PostRollContinueButtonText) == 0 )
				{
					continueText = (const char*)data;
				}
			}
			while( cfuhash_next(options, &key, &data) );
		}
	}
	
	JNIEnv* env = s3eEdkJNIGetEnv();
    jstring zoneID_jstr = env->NewStringUTF(zoneID);
	jstring yesText_jstr = env->NewStringUTF(yesText);
	jstring noText_jstr = env->NewStringUTF(noText);
	jstring continueText_jstr = env->NewStringUTF(continueText);
	
    env->CallVoidMethod(g_Obj, g_FuseSDKShowAdForZoneID, zoneID_jstr, showPre, showPost, yesText_jstr, noText_jstr, continueText_jstr);
}

void FuseSDKDisplayNotifications_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseSDKDisplayNotifications);
}

void FuseSDKDisplayMoreGames_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseSDKDisplayMoreGames);
}

void FuseSDKRegisterGender_platform(int gender)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseSDKRegisterGender, gender);
}

void FuseSDKFacebookLogin_platform(const char* facebookId, const char* name, const char* accessToken)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring facebookId_jstr = env->NewStringUTF(facebookId);
    jstring name_jstr = env->NewStringUTF(name);
    jstring accessToken_jstr = env->NewStringUTF(accessToken);
    env->CallVoidMethod(g_Obj, g_FuseSDKFacebookLogin, facebookId_jstr, name_jstr, accessToken_jstr);
}

void FuseSDKTwitterLogin_platform(const char* twitterId)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring twitterId_jstr = env->NewStringUTF(twitterId);
    env->CallVoidMethod(g_Obj, g_FuseSDKTwitterLogin, twitterId_jstr);
}

void FuseSDKDeviceLogin_platform(const char* alias)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring alias_jstr = env->NewStringUTF(alias);
    env->CallVoidMethod(g_Obj, g_FuseSDKDeviceLogin, alias_jstr);
}

void FuseSDKFuseLogin_platform(const char* fuseId, const char* alias)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring fuseId_jstr = env->NewStringUTF(fuseId);
    jstring alias_jstr = env->NewStringUTF(alias);
    env->CallVoidMethod(g_Obj, g_FuseSDKFuseLogin, fuseId_jstr, alias_jstr);
}

void FuseSDKGameCenterLogin_platform()
{
	// only available on iOS
}

void FuseSDKGooglePlayLogin_platform(const char* alias, const char* token)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring alias_jstr = env->NewStringUTF(alias);
    jstring token_jstr = env->NewStringUTF(token);
    env->CallVoidMethod(g_Obj, g_FuseSDKGooglePlayLogin, alias_jstr, token_jstr);
}

const char* FuseSDKGetOriginalAccountId_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring returnString = (jstring)env->CallObjectMethod(g_Obj, g_FuseSDKGetOriginalAccountId);
	return s3eEdkGetStringUTF8Chars(returnString);
}

const char* FuseSDKGetOriginalAccountAlias_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring returnString  = (jstring)env->CallObjectMethod(g_Obj, g_FuseSDKGetOriginalAccountAlias);
	return s3eEdkGetStringUTF8Chars(returnString);
}

int FuseSDKGetOriginalAccountType_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    return (int)env->CallIntMethod(g_Obj, g_FuseSDKGetOriginalAccountType);
}

const char* FuseSDKGetFuseID_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring returnString  = (jstring)env->CallObjectMethod(g_Obj, g_FuseSDKGetFuseID);
	return s3eEdkGetStringUTF8Chars(returnString);
}

int FuseSDKgamesPlayed_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    return (int)env->CallIntMethod(g_Obj, g_FuseSDKgamesPlayed);
}

const char* FuseSDKLibraryVersion_platform()
{
	JNIEnv* env = s3eEdkJNIGetEnv();
    jstring returnString  = (jstring)env->CallObjectMethod(g_Obj, g_FuseSDKLibraryVersion);
	return s3eEdkGetStringUTF8Chars(returnString);
}

bool FuseSDKConnected_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    return (bool)env->CallBooleanMethod(g_Obj, g_FuseSDKConnected);
}

void FuseSDKTimeFromServer_platform()
{
	JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseSDKTimeFromServer);
}

void FuseSDKEnableData_platform(bool enable)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseSDKEnableData, enable);
}

const char* FuseSDKGetGameConfigurationValue_platform(const char* key)
{
	JNIEnv* env = s3eEdkJNIGetEnv();
	jstring key_jstr = env->NewStringUTF(key);
    jstring returnString  = (jstring)env->CallObjectMethod(g_Obj, g_FuseSDKGetGameConfigurationValue, key_jstr);
	return s3eEdkGetStringUTF8Chars(returnString);
}

void FuseSDKRegisterLevel_platform(int level)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseSDKRegisterLevel, level);
}

void FuseSDKRegisterCurrency_platform(int type, int balance)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseSDKRegisterCurrency, type, balance);
}

