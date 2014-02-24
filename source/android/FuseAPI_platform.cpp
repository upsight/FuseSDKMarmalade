/*
 * android-specific implementation of the FuseAPI extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "FuseAPI_internal.h"

#include "s3eEdk.h"
#include "s3eEdk_android.h"
#include <jni.h>
#include "IwDebug.h"

#include "cfuhash.h"

static jobject g_Obj;
static jmethodID g_FuseAPIStartSession;
static jmethodID g_FuseAPIPauseSession;
static jmethodID g_FuseAPIResumeSession;
static jmethodID g_FuseAPITerminateSession;
static jmethodID g_FuseAPIRegisterForPushNotifications;
static jmethodID g_FuseAPIRegisterEvent;
static jmethodID g_FuseAPIRegisterEventWithParam;
static jmethodID g_FuseAPIRegisterEventStart;
static jmethodID g_FuseAPIRegisterEventKeyValue;
static jmethodID g_FuseAPIRegisterEventEnd;
static jmethodID g_FuseAPIRegisterEventWithDictionaryStart;
static jmethodID g_FuseAPIRegisterEventWithDictionaryKeyValue;
static jmethodID g_FuseAPIRegisterEventWithDictionaryEnd;
static jmethodID g_FuseAPIRegisterInAppPurchase;
static jmethodID g_FuseAPICheckAdAvailable;
static jmethodID g_FuseAPIShowAd;
static jmethodID g_FuseAPIDisplayNotifications;
static jmethodID g_FuseAPIDisplayMoreGames;
static jmethodID g_FuseAPIRegisterGender;
static jmethodID g_FuseAPIFacebookLogin;
static jmethodID g_FuseAPITwitterLogin;
static jmethodID g_FuseAPIDeviceLogin;
static jmethodID g_FuseAPIFuseLogin;
static jmethodID g_FuseAPIGooglePlayLogin;
static jmethodID g_FuseAPIGetOriginalAccountId;
static jmethodID g_FuseAPIGetOriginalAccountAlias;
static jmethodID g_FuseAPIGetOriginalAccountType;
static jmethodID g_FuseAPIGetFuseID;
static jmethodID g_FuseAPIgamesPlayed;
static jmethodID g_FuseAPILibraryVersion;
static jmethodID g_FuseAPIConnected;
static jmethodID g_FuseAPITimeFromServer;
static jmethodID g_FuseAPIEnableData;
static jmethodID g_FuseAPISetGameDataStart;
static jmethodID g_FuseAPISetGameDataKeyValue;
static jmethodID g_FuseAPISetGameDataEnd;
static jmethodID g_FuseAPIGetGameDataStart;
static jmethodID g_FuseAPIGetGameDataKey;
static jmethodID g_FuseAPIGetGameDataEnd;
static jmethodID g_FuseAPIAddFriend;
static jmethodID g_FuseAPIRemoveFriend;
static jmethodID g_FuseAPIAcceptFriend;
static jmethodID g_FuseAPIRejectFriend;
static jmethodID g_FuseAPIMigrateFriends;
static jmethodID g_FuseAPIUserPushNotification;
static jmethodID g_FuseAPIFriendsPushNotification;
static jmethodID g_FuseAPIUpdateFriendsListFromServer;
static jmethodID g_FuseAPIGetMailListFromServer;
static jmethodID g_FuseAPIGetMailListFriendFromServer;
static jmethodID g_FuseAPISetMailAsReceived;
static jmethodID g_FuseAPISendMailWithGift;
static jmethodID g_FuseAPISendMail;
static jmethodID g_FuseAPIGetGameConfigurationValue;
static jmethodID g_FuseAPIRegisterLevel;
static jmethodID g_FuseAPIRegisterCurrency;
static jmethodID g_FuseAPIRegisterFlurryView;
static jmethodID g_FuseAPIRegisterFlurryClick;
static jmethodID g_FuseAPIRegisterTapjoyReward;

//----------------------------------------------------------------------------------------
/*
  FuseAPI Callback Functions

  A note about callback parameters:
  Callbacks have two arguments - void* systemData and void* userData
  FuseAPI callbacks will always pass information as systemData in the form of a 
  memory address to a struct - even when there is only one parameter.  
  Please see comments above each function for information on what structs to cast systemData to.
*/
//----------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------
// Session Callbacks
//-------------------------------------
// FuseAPISessionStartReceived
// Params: none
//-------------------------------------
void FuseAPISessionStartReceived(JNIEnv* env, jobject obj)
{
    IwTrace(FuseAPI, ("FuseAPISessionStartReceived()"));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_SESSION_STARTED, 0, 0);
}

//-------------------------------------
// FuseAPILoginError
// Params: int error
//-------------------------------------
void FuseAPILoginError(JNIEnv* env, jobject obj, jint result)
{
	struct paramList
	{
		int error;
	};
	paramList params;
	params.error = result;
    IwTrace(FuseAPI, ("FuseAPILoginError(%i)", result));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_SESSION_LOGIN_ERROR, &params, sizeof(paramList));
}
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
// Ad Callbacks
//-------------------------------------
// FuseAPIAdAvailabilityResponse
// Params: int available, int error
//-------------------------------------
void FuseAPIAdAvailabilityResponse(JNIEnv* env, jobject obj, jint available, jint error)
{
	struct paramList
	{
		int available;
		int error;
	};
	paramList params;
	params.available = available;
	params.error = error;

    IwTrace(FuseAPI, ("FuseAPIAdAvailabilityResponse(%i, %i)", available, error));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_AD_AVAILABILITY_RESPONSE, &params, sizeof(paramList));
}

//-------------------------------------
// FuseAPIAdWillClose
// Params: none
//-------------------------------------
void FuseAPIAdWillClose(JNIEnv* env, jobject obj)
{
	IwTrace(FuseAPI, ("FuseAPIAdWillClose()"));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_AD_WILL_CLOSE, 0, 0);
}

//-------------------------------------
// FuseAPIAdDisplayed
// Params: none
//-------------------------------------
void FuseAPIAdDisplayed(JNIEnv* env, jobject obj)
{
	IwTrace(FuseAPI, ("FuseAPIAdDisplayed()"));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_AD_DISPLAYED, 0, 0);
}

//-------------------------------------
// FuseAPIAdClicked
// Params: none
//-------------------------------------
void FuseAPIAdClicked(JNIEnv* env, jobject obj)
{
	IwTrace(FuseAPI, ("FuseAPIAdClicked()"));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_AD_CLICKED, 0, 0);
}
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
// Game Data Callbacks
//-------------------------------------
// FuseAPINotificationAction
// Params: string action
//-------------------------------------
void FuseAPINotificationAction(JNIEnv* env, jobject obj, jstring action)
{
	struct paramList
	{
		const char* action;
	};
	paramList params;
	params.action = s3eEdkGetStringUTF8Chars(action);
    IwTrace(FuseAPI, ("FuseAPINotificationAction(%s)", params.action));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_NOTIFICATION_ACTION, &params, sizeof(paramList));
}

//-------------------------------------
// FuseAPIAccountLoginComplete
// Params: int accountType, string accountID
// Note: accountType is defined in FuseAPI.h as FuseAccountType
//-------------------------------------
void FuseAPIAccountLoginComplete(JNIEnv* env, jobject obj, int accountType, jstring accountID)
{
	struct paramList
	{
		int accountType;
		const char* accountID;
	};
	paramList params;
	params.accountType = accountType;
	params.accountID = s3eEdkGetStringUTF8Chars(accountID);
    IwTrace(FuseAPI, ("FuseAPIAccountLoginComplete(%i, %s)", params.accountType, params.accountID));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_ACCOUNT_LOGIN_COMPLETE, &params, sizeof(paramList));
}

//-------------------------------------
// FuseAPITimeUpdated
// Params: int timestamp
//-------------------------------------
void FuseAPITimeUpdated(JNIEnv* env, jobject obj, jint timestamp)
{
	struct paramList
	{
		int timestamp;
	};
	paramList params;
	params.timestamp = timestamp;
    IwTrace(FuseAPI, ("FuseAPITimeUpdated(%i)", timestamp));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_TIME_UPDATED, &params, sizeof(paramList));
}

//-------------------------------------
// FuseGameDataError
// Params: int requestId, int error
//-------------------------------------
void FuseGameDataError(JNIEnv* env, jobject obj, jint requestId, jint error)
{
	struct paramList
	{
		int requestId;
		int error;
	};
	paramList params;
	params.requestId = requestId;
	params.error = error;
    IwTrace(FuseAPI, ("FuseGameDataError(%i, %i)", requestId, error));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_GAME_DATA_ERROR, &params, sizeof(paramList));
}

//-------------------------------------
// FuseGameDataSetAcknowledged
// Params: int requestID
//-------------------------------------
void FuseGameDataSetAcknowledged(JNIEnv* env, jobject obj, jint requestID)
{
	struct paramList
	{
		int requestID;
	};
	paramList params;
	params.requestID = requestID;
    IwTrace(FuseAPI, ("FuseGameDataSetAcknowledged(%i)", requestID));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_GAME_DATASET_ACKNOWLEDGED, &params, sizeof(paramList));
}

//-------------------------------------
// FuseFriendAdded
// Params: const char* fuseId, int error
//-------------------------------------
void FuseFriendAdded(JNIEnv* env, jobject obj, jstring fuseId, jint error)
{
	struct paramList
	{
		const char* fuseId;
		int error;
	};
	paramList params;
	params.error = error;
	params.fuseId = s3eEdkGetStringUTF8Chars(fuseId);

	IwTrace(FuseAPI, ("FuseFriendAdded(%s, %i)", params.fuseId, params.error));
	s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_FRIEND_ADDED, &params, sizeof(paramList));
}

//-------------------------------------
// FuseFriendRemoved
// Params: const char* fuseId, int error
//-------------------------------------
void FuseFriendRemoved(JNIEnv* env, jobject obj, jstring fuseId, jint error)
{
	struct paramList
	{
		const char* fuseId;
		int error;
	};
	paramList params;
	params.error = error;
	params.fuseId = s3eEdkGetStringUTF8Chars(fuseId);

	IwTrace(FuseAPI, ("FuseFriendRemoved(%s, %i)", params.fuseId, params.error));
	s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_FRIEND_REMOVED, &params, sizeof(paramList));
}

//-------------------------------------
// FuseFriendAccepted
// Params: const char* fuseId, int error
//-------------------------------------
void FuseFriendAccepted(JNIEnv* env, jobject obj, jstring fuseId, jint error)
{
	struct paramList
	{
		const char* fuseId;
		int error;
	};
	paramList params;
	params.error = error;
	params.fuseId = s3eEdkGetStringUTF8Chars(fuseId);

	IwTrace(FuseAPI, ("FuseFriendAccepted(%s, %i)", params.fuseId, params.error));
	s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_FRIEND_ACCEPTED, &params, sizeof(paramList));
}

//-------------------------------------
// FuseFriendRejected
// Params: const char* fuseId, int error
//-------------------------------------
void FuseFriendRejected(JNIEnv* env, jobject obj, jstring fuseId, jint error)
{
	struct paramList
	{
		const char* fuseId;
		int error;
	};
	paramList params;
	params.error = error;
	params.fuseId = s3eEdkGetStringUTF8Chars(fuseId);

	IwTrace(FuseAPI, ("FuseFriendRejected(%s, %i)", params.fuseId, params.error));
	s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_FRIEND_REJECTED, &params, sizeof(paramList));
}

//-------------------------------------
// FuseFriendsMigrated
// Params: const char* fuseId, int error
//-------------------------------------
void FuseFriendsMigrated(JNIEnv* env, jobject obj, jstring fuseId, jint error)
{
	struct paramList
	{
		const char* fuseId;
		int error;
	};
	paramList params;
	params.error = error;
	params.fuseId = s3eEdkGetStringUTF8Chars(fuseId);

	IwTrace(FuseAPI, ("FuseFriendsMigrated(%s, %i)", params.fuseId, params.error));
	s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_FRIENDS_MIGRATED, &params, sizeof(paramList));
}

//-------------------------------------
// FuseAPIFriendsListUpdated
// Params: none
//-------------------------------------
FusePlayer* g_playerList = NULL;
int g_playerIndex = 0;
void FuseAPIFriendsListUpdateStart(JNIEnv* env, jobject obj, jint numPlayers)
{
	IwTrace(FuseAPI, ("FuseAPIFriendsListUpdateStart()"));
	if( g_playerList != NULL )
	{
		delete(g_playerList);
	}
	g_playerList = (FusePlayer*)malloc(sizeof(FusePlayer)*numPlayers);
	g_playerIndex = 0;
}
 
void FuseAPIFriendsListUpdatePlayer(JNIEnv* env, jobject obj, jstring fuseId, jstring alias, jstring type, jstring accountId, jint level, jint pending, bool canAttack)
{
	IwTrace(FuseAPI, ("FuseAPIFriendsListUpdatePlayer()"));
	g_playerList[g_playerIndex].fuseId = s3eEdkGetStringUTF8Chars(fuseId);
	g_playerList[g_playerIndex].alias = s3eEdkGetStringUTF8Chars(alias);
	g_playerList[g_playerIndex].type = s3eEdkGetStringUTF8Chars(type);
	g_playerList[g_playerIndex].accountId = s3eEdkGetStringUTF8Chars(accountId);
	g_playerList[g_playerIndex].level = level;
	g_playerList[g_playerIndex].pending = pending;
	g_playerList[g_playerIndex].canAttack = canAttack;
	g_playerIndex++;
}

void FuseAPIFriendsListUpdateEnd(JNIEnv* env, jobject obj)
{
	IwTrace(FuseAPI, ("FuseAPIFriendsListUpdateEnd()"));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_FRIENDSLIST_UPDATED, 0, 0);
}

//-------------------------------------
// FuseFriendsListError
// Params: int error
//-------------------------------------
void FuseFriendsListError(JNIEnv* env, jobject obj, jint error)
{
	struct paramList
	{
		int error;
	};
	paramList params;
	params.error = error;
    IwTrace(FuseAPI, ("FuseFriendsListError(%i)", error));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_FRIENDSLIST_ERROR, &params, sizeof(paramList));
}

//-------------------------------------
// FuseMailAcknowledged
// Params: int messageId, string fuseID, int requestID
//-------------------------------------
void FuseMailAcknowledged(JNIEnv* env, jobject obj, jint messageId, jstring fuseID, jint requestID)
{
	struct paramList
	{
		int messageId;
		const char* fuseID;
		int requestID;
	};
	paramList params;
	params.messageId = messageId;
	params.fuseID = s3eEdkGetStringUTF8Chars(fuseID);
	params.requestID = requestID;
    IwTrace(FuseAPI, ("FuseMailAcknowledged(%i, %s, %i)", params.messageId, params.fuseID, params.requestID));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_MAIL_ACKNOWLEDGED, &params, sizeof(paramList));
}

//-------------------------------------
// FuseMailListReceived
// Params: string fuseId
//-------------------------------------
FuseMail* g_mailList = NULL;
int g_numMailEntries = 0;
void FuseAPIMailListUpdateStart(JNIEnv* env, jobject obj, jint numEntries)
{
	IwTrace(FuseAPI, ("FuseAPIMailListUpdateStart()"));
	if( g_mailList != NULL )
	{
		delete(g_mailList);
	}
	g_mailList = (FuseMail*)malloc(sizeof(FuseMail)*numEntries);
	g_numMailEntries = 0;
}

void FuseAPIMailListUpdate(JNIEnv* env, jobject obj, jint id, jstring alias, jstring fuseId, jstring message, jstring date)
{
	g_mailList[g_numMailEntries].id = id;
	g_mailList[g_numMailEntries].alias = s3eEdkGetStringUTF8Chars(alias);
	g_mailList[g_numMailEntries].fuseId = s3eEdkGetStringUTF8Chars(fuseId);
	g_mailList[g_numMailEntries].message = s3eEdkGetStringUTF8Chars(message);
	g_mailList[g_numMailEntries].date = s3eEdkGetStringUTF8Chars(date);	
}
void FuseAPIMailListUpdateGift(JNIEnv* env, jobject obj, jint id, jstring name, jstring url, jint amount)
{
	g_mailList[g_numMailEntries].gift.id = id;
	g_mailList[g_numMailEntries].gift.name = s3eEdkGetStringUTF8Chars(name);
	g_mailList[g_numMailEntries].gift.url = s3eEdkGetStringUTF8Chars(url);
	g_mailList[g_numMailEntries].gift.amount = amount;
	g_numMailEntries++;
}
void FuseMailListReceived(JNIEnv* env, jobject obj, jstring fuseId)
{
	struct paramList
	{
		const char* fuseId;
	};
	paramList params;
	params.fuseId = s3eEdkGetStringUTF8Chars(fuseId);
    IwTrace(FuseAPI, ("FuseMailListReceived( %s )", params.fuseId));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_MAILLIST_RECEIVED, &params, sizeof(paramList));
}

//-------------------------------------
// FuseMailError
// Params: int error
//-------------------------------------
void FuseMailError(JNIEnv* env, jobject obj, jint error)
{
	struct paramList
	{
		int error;
	};
	paramList params;
	params.error = error;
    IwTrace(FuseAPI, ("FuseMailError(%i)", error));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_MAIL_ERROR, &params, sizeof(paramList));
}

//-------------------------------------
// FuseMailListError
// Params: int error
//-------------------------------------
void FuseMailListError(JNIEnv* env, jobject obj, jint error)
{
	struct paramList
	{
		int error;
	};
	paramList params;
	params.error = error;
    IwTrace(FuseAPI, ("FuseMailListError(%i)", error));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_MAILLIST_ERROR, &params, sizeof(paramList));
}

//-------------------------------------
// FuseGameConfigurationReceived
// Params: none
//-------------------------------------
cfuhash_table_t *gameConfig = NULL;
void FuseGameConfigurationStart(JNIEnv* env, jobject obj, jint numKeys)
{
	IwTrace(FuseAPI, ("FuseGameConfigurationStart()"));
	if( gameConfig == NULL )
	{
		gameConfig = cfuhash_new_with_initial_size(numKeys);
	}
	cfuhash_clear(gameConfig);	
}
void FuseGameConfigurationKeyValue(JNIEnv* env, jobject obj, jstring key, jstring value)
{
	IwTrace(FuseAPI, ("FuseGameConfigurationKeyValue(%s, %s)", s3eEdkGetStringUTF8Chars(key), s3eEdkGetStringUTF8Chars(value)));
	cfuhash_put(gameConfig, (const char*)s3eEdkGetStringUTF8Chars(key), (void*)s3eEdkGetStringUTF8Chars(value));
}
void FuseGameConfigurationReceived(JNIEnv* env, jobject obj)
{
	IwTrace(FuseAPI, ("FuseGameConfigurationReceived()"));

	struct paramList
	{
		cfuhash_table_t* gameConfig;
	};
	paramList params;
	params.gameConfig = gameConfig;

	s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_GAME_CONFIGURATION_RECEIVED, &params, sizeof(paramList));
}

//-------------------------------------
// FuseAPIGameDataReceived
// Params: string accountId, hash_map gameData, int requestId
//-------------------------------------
cfuhash_table_t* gameData = cfuhash_new_with_initial_size(10);
void FuseAPIGameDataReceivedStart(JNIEnv* env, jobject obj)
{
	IwTrace(FuseAPI, ("FuseAPIGameDataReceivedStart()"));
	cfuhash_clear(gameData);
}
void FuseAPIGameDataReceivedKVP(JNIEnv* env, jobject obj, bool isBinary, jstring key, jstring value)
{
	IwTrace(FuseAPI, ("FuseAPIGameDataReceivedKVP(%s, %s, %s)", isBinary ? "true" : "false", s3eEdkGetStringUTF8Chars(key), s3eEdkGetStringUTF8Chars(value)));
	cfuhash_put(gameData, (const char*)s3eEdkGetStringUTF8Chars(key), (void*)s3eEdkGetStringUTF8Chars(value));
}
void FuseAPIGameDataReceivedEnd(JNIEnv* env, jobject obj, jstring accountID, int requestID)
{
	IwTrace(FuseAPI, ("FuseAPIGameDataReceivedEnd(%s, %i)", s3eEdkGetStringUTF8Chars(accountID), requestID));

	struct paramList
	{
		cfuhash_table_t* gameData;
		const char* accountID;
		int requestID;
	};
	paramList params;
	params.gameData = gameData;
	params.accountID = s3eEdkGetStringUTF8Chars(accountID);
	params.requestID = requestID;

    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_GAME_DATA_RECEIVED, &params, sizeof(paramList));
}
//----------------------------------------------------------------------------------------

int32 FusePause(void* systemData, void* userData)
{
	if( gameConfig != NULL )
	{
		cfuhash_destroy(gameConfig);
		gameConfig = NULL;
	}

    FuseAPIPauseSession_platform();

    return 1;
}

int32 FuseResume(void* systemData, void* userData)
{
    FuseAPIResumeSession_platform();

    return 1;
}


s3eResult FuseAPIInit_platform()
{
    // Get the environment from the pointer
    JNIEnv* env = s3eEdkJNIGetEnv();
    jobject obj = NULL;
    jmethodID cons = NULL;

	// callbacks
	const JNINativeMethod nativeMethodDefs[] =
    {
		{ "FuseAPISessionStartReceived",				"()V",			(void*)&FuseAPISessionStartReceived },
		{ "FuseAPILoginError",							"(I)V",			(void*)&FuseAPILoginError },
		{ "FuseAPIAdAvailabilityResponse",				"(II)V",		(void*)&FuseAPIAdAvailabilityResponse },
		{ "FuseAPIAdWillClose",							"()V",			(void*)&FuseAPIAdWillClose },
		{ "FuseAPIAdDisplayed",							"()V",			(void*)&FuseAPIAdDisplayed },
		{ "FuseAPIAdClicked",							"()V",			(void*)&FuseAPIAdClicked },
		{ "FuseAPINotificationAction",	"(Ljava/lang/String;)V",		(void*)&FuseAPINotificationAction },
		{ "FuseAPIAccountLoginComplete","(ILjava/lang/String;)V",		(void*)&FuseAPIAccountLoginComplete },
		{ "FuseAPITimeUpdated",							"(I)V",			(void*)&FuseAPITimeUpdated },
		{ "FuseGameDataError",							"(II)V",		(void*)&FuseGameDataError },
		{ "FuseGameDataSetAcknowledged",				"(I)V",			(void*)&FuseGameDataSetAcknowledged },
		{ "FuseFriendAdded",		"(Ljava/lang/String;I)V",			(void*)&FuseFriendAdded },
		{ "FuseFriendRemoved",		"(Ljava/lang/String;I)V",			(void*)&FuseFriendRemoved },
		{ "FuseFriendAccepted",		"(Ljava/lang/String;I)V",			(void*)&FuseFriendAccepted },
		{ "FuseFriendRejected",		"(Ljava/lang/String;I)V",			(void*)&FuseFriendRejected },
		{ "FuseFriendsMigrated",	"(Ljava/lang/String;I)V",			(void*)&FuseFriendsMigrated },
		{ "FuseFriendsListError",						"(I)V",			(void*)&FuseFriendsListError },
		{ "FuseMailAcknowledged",	 "(ILjava/lang/String;I)V",			(void*)&FuseMailAcknowledged },
		{ "FuseMailError",								"(II)V",		(void*)&FuseMailError },
		{ "FuseMailListError",							"(I)V",			(void*)&FuseMailListError },
		{ "FuseGameConfigurationStart",					"(I)V",			(void*)&FuseGameConfigurationStart },
		{ "FuseGameConfigurationKeyValue", "(Ljava/lang/String;Ljava/lang/String;)V", (void*)&FuseGameConfigurationKeyValue },
		{ "FuseGameConfigurationReceived",				"()V",			(void*)&FuseGameConfigurationReceived },
		{ "FuseAPIGameDataReceivedStart",				"()V",			(void*)&FuseAPIGameDataReceivedStart },
		{ "FuseAPIGameDataReceivedKVP",	"(ZLjava/lang/String;Ljava/lang/String;)V",	(void*)&FuseAPIGameDataReceivedKVP },
		{ "FuseAPIGameDataReceivedEnd",	 "(Ljava/lang/String;I)V",		(void*)&FuseAPIGameDataReceivedEnd },
		{ "FuseAPIFriendsListUpdateStart",				"(I)V",			(void*)&FuseAPIFriendsListUpdateStart },
		{ "FuseAPIFriendsListUpdatePlayer", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;IIZ)V", (void*)&FuseAPIFriendsListUpdatePlayer },
		{ "FuseAPIFriendsListUpdateEnd",				"()V",			(void*)&FuseAPIFriendsListUpdateEnd },
		{ "FuseAPIMailListUpdateStart",					"(I)V",			(void*)&FuseAPIMailListUpdateStart },
		{ "FuseAPIMailListUpdate", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", (void*)&FuseAPIMailListUpdate },
		{ "FuseAPIMailListUpdateGift", "(ILjava/lang/String;Ljava/lang/String;I)V", (void*)&FuseAPIMailListUpdateGift },
		{ "FuseMailListReceived",		"(Ljava/lang/String;)V",		(void*)&FuseMailListReceived },
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
    g_FuseAPIStartSession = env->GetMethodID(cls, "FuseAPIStartSession", "(Ljava/lang/String;)V");
    if (!g_FuseAPIStartSession)
        goto fail; 

	g_FuseAPIPauseSession = env->GetMethodID(cls, "FuseAPIPauseSession", "()V");
    if (!g_FuseAPIPauseSession)
        goto fail;

	g_FuseAPIResumeSession = env->GetMethodID(cls, "FuseAPIResumeSession", "()V");
    if (!g_FuseAPIResumeSession)
        goto fail;

	g_FuseAPITerminateSession = env->GetMethodID(cls, "FuseAPITerminateSession", "()V");
    if (!g_FuseAPITerminateSession)
        goto fail;

    g_FuseAPIRegisterForPushNotifications = env->GetMethodID(cls, "FuseAPIRegisterForPushNotifications", "(Ljava/lang/String;)V");
    if (!g_FuseAPIRegisterForPushNotifications)
        goto fail;

    g_FuseAPIRegisterEvent = env->GetMethodID(cls, "FuseAPIRegisterEvent", "(Ljava/lang/String;)V");
    if (!g_FuseAPIRegisterEvent)
        goto fail;
		
	g_FuseAPIRegisterEventWithParam = env->GetMethodID(cls, "FuseAPIRegisterEventWithParam", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;D)I");
    if (!g_FuseAPIRegisterEventWithParam)
        goto fail;

	g_FuseAPIRegisterEventStart = env->GetMethodID(cls, "FuseAPIRegisterEventStart", "()V");
    if (!g_FuseAPIRegisterEventStart)
        goto fail;

	g_FuseAPIRegisterEventKeyValue = env->GetMethodID(cls, "FuseAPIRegisterEventKeyValue", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!g_FuseAPIRegisterEventKeyValue)
        goto fail;

	g_FuseAPIRegisterEventEnd = env->GetMethodID(cls, "FuseAPIRegisterEventEnd", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I");
    if (!g_FuseAPIRegisterEventEnd)
        goto fail;

	g_FuseAPIRegisterEventWithDictionaryStart = env->GetMethodID(cls, "FuseAPIRegisterEventWithDictionaryStart", "()V");
    if (!g_FuseAPIRegisterEventWithDictionaryStart)
        goto fail;

	g_FuseAPIRegisterEventWithDictionaryKeyValue = env->GetMethodID(cls, "FuseAPIRegisterEventWithDictionaryKeyValue", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!g_FuseAPIRegisterEventWithDictionaryKeyValue)
        goto fail;

	g_FuseAPIRegisterEventWithDictionaryEnd = env->GetMethodID(cls, "FuseAPIRegisterEventWithDictionaryEnd", "(Ljava/lang/String;)V");
    if (!g_FuseAPIRegisterEventWithDictionaryEnd)
        goto fail;

    g_FuseAPIRegisterInAppPurchase = env->GetMethodID(cls, "FuseAPIRegisterInAppPurchase", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;JLjava/lang/String;DLjava/lang/String;)V");
    if (!g_FuseAPIRegisterInAppPurchase)
        goto fail;

    g_FuseAPICheckAdAvailable = env->GetMethodID(cls, "FuseAPICheckAdAvailable", "()V");
    if (!g_FuseAPICheckAdAvailable)
        goto fail;

    g_FuseAPIShowAd = env->GetMethodID(cls, "FuseAPIShowAd", "()V");
    if (!g_FuseAPIShowAd)
        goto fail;

    g_FuseAPIDisplayNotifications = env->GetMethodID(cls, "FuseAPIDisplayNotifications", "()V");
    if (!g_FuseAPIDisplayNotifications)
        goto fail;

    g_FuseAPIDisplayMoreGames = env->GetMethodID(cls, "FuseAPIDisplayMoreGames", "()V");
    if (!g_FuseAPIDisplayMoreGames)
        goto fail;

    g_FuseAPIRegisterGender = env->GetMethodID(cls, "FuseAPIRegisterGender", "(I)V");
    if (!g_FuseAPIRegisterGender)
        goto fail;

    g_FuseAPIFacebookLogin = env->GetMethodID(cls, "FuseAPIFacebookLogin", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if (!g_FuseAPIFacebookLogin)
        goto fail;

    g_FuseAPITwitterLogin = env->GetMethodID(cls, "FuseAPITwitterLogin", "(Ljava/lang/String;)V");
    if (!g_FuseAPITwitterLogin)
        goto fail;

	g_FuseAPIDeviceLogin = env->GetMethodID(cls, "FuseAPIDeviceLogin", "(Ljava/lang/String;)V");
    if (!g_FuseAPIDeviceLogin)
        goto fail;	

    g_FuseAPIFuseLogin = env->GetMethodID(cls, "FuseAPIFuseLogin", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!g_FuseAPIFuseLogin)
        goto fail;

    g_FuseAPIGooglePlayLogin = env->GetMethodID(cls, "FuseAPIGooglePlayLogin", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!g_FuseAPIGooglePlayLogin)
        goto fail;

    g_FuseAPIGetOriginalAccountId = env->GetMethodID(cls, "FuseAPIGetOriginalAccountId", "()Ljava/lang/String;");
    if (!g_FuseAPIGetOriginalAccountId)
        goto fail;

    g_FuseAPIGetOriginalAccountAlias = env->GetMethodID(cls, "FuseAPIGetOriginalAccountAlias", "()Ljava/lang/String;");
    if (!g_FuseAPIGetOriginalAccountAlias)
        goto fail;

    g_FuseAPIGetOriginalAccountType = env->GetMethodID(cls, "FuseAPIGetOriginalAccountType", "()I");
    if (!g_FuseAPIGetOriginalAccountType)
        goto fail;

    g_FuseAPIGetFuseID = env->GetMethodID(cls, "FuseAPIGetFuseID", "()Ljava/lang/String;");
    if (!g_FuseAPIGetFuseID)
        goto fail;

    g_FuseAPIgamesPlayed = env->GetMethodID(cls, "FuseAPIgamesPlayed", "()I");
    if (!g_FuseAPIgamesPlayed)
        goto fail;

    g_FuseAPILibraryVersion = env->GetMethodID(cls, "FuseAPILibraryVersion", "()Ljava/lang/String;");
    if (!g_FuseAPILibraryVersion)
        goto fail;

    g_FuseAPIConnected = env->GetMethodID(cls, "FuseAPIConnected", "()Z");
    if (!g_FuseAPIConnected)
        goto fail;

	g_FuseAPITimeFromServer = env->GetMethodID(cls, "FuseAPIConnected", "()Z");
    if (!g_FuseAPITimeFromServer)
        goto fail;

    g_FuseAPIEnableData = env->GetMethodID(cls, "FuseAPIEnableData", "(Z)V");
    if (!g_FuseAPIEnableData)
        goto fail;

	g_FuseAPISetGameDataStart = env->GetMethodID(cls, "FuseAPISetGameDataStart", "()V");
    if (!g_FuseAPISetGameDataStart)
        goto fail;

	g_FuseAPISetGameDataKeyValue = env->GetMethodID(cls, "FuseAPISetGameDataKeyValue", "(Ljava/lang/String;Ljava/lang/String;Z)V");
    if (!g_FuseAPISetGameDataKeyValue)
        goto fail;

	g_FuseAPISetGameDataEnd = env->GetMethodID(cls, "FuseAPISetGameDataEnd", "(Ljava/lang/String;Ljava/lang/String;)I");
    if (!g_FuseAPISetGameDataEnd)
        goto fail;

	g_FuseAPIGetGameDataStart = env->GetMethodID(cls, "FuseAPIGetGameDataStart", "()V");
    if (!g_FuseAPIGetGameDataStart)
        goto fail;

	g_FuseAPIGetGameDataKey = env->GetMethodID(cls, "FuseAPIGetGameDataKey", "(Ljava/lang/String;)V");
    if (!g_FuseAPIGetGameDataKey)
        goto fail;

	g_FuseAPIGetGameDataEnd = env->GetMethodID(cls, "FuseAPIGetGameDataEnd", "(Ljava/lang/String;Ljava/lang/String;)I");
    if (!g_FuseAPIGetGameDataEnd)
        goto fail;

	g_FuseAPIAddFriend = env->GetMethodID(cls, "FuseAPIAddFriend", "(Ljava/lang/String;)V");
    if (!g_FuseAPIAddFriend)
        goto fail;

	g_FuseAPIRemoveFriend = env->GetMethodID(cls, "FuseAPIRemoveFriend", "(Ljava/lang/String;)V");
    if (!g_FuseAPIRemoveFriend)
        goto fail;

	g_FuseAPIAcceptFriend = env->GetMethodID(cls, "FuseAPIAcceptFriend", "(Ljava/lang/String;)V");
    if (!g_FuseAPIAcceptFriend)
        goto fail;

	g_FuseAPIRejectFriend = env->GetMethodID(cls, "FuseAPIRejectFriend", "(Ljava/lang/String;)V");
    if (!g_FuseAPIRejectFriend)
        goto fail;

	g_FuseAPIMigrateFriends = env->GetMethodID(cls, "FuseAPIMigrateFriends", "(Ljava/lang/String;)V");
    if (!g_FuseAPIMigrateFriends)
        goto fail;

	g_FuseAPIUserPushNotification = env->GetMethodID(cls, "FuseAPIUserPushNotification", "(Ljava/lang/String;)V");
    if (!g_FuseAPIUserPushNotification)
        goto fail;
	g_FuseAPIFriendsPushNotification = env->GetMethodID(cls, "FuseAPIFriendsPushNotification", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!g_FuseAPIFriendsPushNotification)
        goto fail;

    g_FuseAPIUpdateFriendsListFromServer = env->GetMethodID(cls, "FuseAPIUpdateFriendsListFromServer", "()V");
    if (!g_FuseAPIUpdateFriendsListFromServer)
        goto fail;

    g_FuseAPIGetMailListFromServer = env->GetMethodID(cls, "FuseAPIGetMailListFromServer", "()V");
    if (!g_FuseAPIGetMailListFromServer)
        goto fail;

    g_FuseAPIGetMailListFriendFromServer = env->GetMethodID(cls, "FuseAPIGetMailListFriendFromServer", "(Ljava/lang/String;)V");
    if (!g_FuseAPIGetMailListFriendFromServer)
        goto fail;

    g_FuseAPISetMailAsReceived = env->GetMethodID(cls, "FuseAPISetMailAsReceived", "(I)V");
    if (!g_FuseAPISetMailAsReceived)
        goto fail;

    g_FuseAPISendMailWithGift = env->GetMethodID(cls, "FuseAPISendMailWithGift", "(Ljava/lang/String;Ljava/lang/String;II)I");
    if (!g_FuseAPISendMailWithGift)
        goto fail;

    g_FuseAPISendMail = env->GetMethodID(cls, "FuseAPISendMail", "(Ljava/lang/String;Ljava/lang/String;)I");
    if (!g_FuseAPISendMail)
        goto fail;

    g_FuseAPIGetGameConfigurationValue = env->GetMethodID(cls, "FuseAPIGetGameConfigurationValue", "(Ljava/lang/String;)Ljava/lang/String;");
    if (!g_FuseAPIGetGameConfigurationValue)
        goto fail;

    g_FuseAPIRegisterLevel = env->GetMethodID(cls, "FuseAPIRegisterLevel", "(I)V");
    if (!g_FuseAPIRegisterLevel)
        goto fail;

    g_FuseAPIRegisterCurrency = env->GetMethodID(cls, "FuseAPIRegisterCurrency", "(II)V");
    if (!g_FuseAPIRegisterCurrency)
        goto fail;

    g_FuseAPIRegisterFlurryView = env->GetMethodID(cls, "FuseAPIRegisterFlurryView", "()V");
    if (!g_FuseAPIRegisterFlurryView)
        goto fail;

    g_FuseAPIRegisterFlurryClick = env->GetMethodID(cls, "FuseAPIRegisterFlurryClick", "()V");
    if (!g_FuseAPIRegisterFlurryClick)
        goto fail;

    g_FuseAPIRegisterTapjoyReward = env->GetMethodID(cls, "FuseAPIRegisterTapjoyReward", "(I)V");
    if (!g_FuseAPIRegisterTapjoyReward)
        goto fail;

	if(env->RegisterNatives(cls, nativeMethodDefs, sizeof(nativeMethodDefs)/sizeof(nativeMethodDefs[0])))
		goto fail;

    IwTrace(FUSEAPI, ("FUSEAPI init success"));
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
        IwTrace(FuseAPI, ("One or more java methods could not be found"));
    }
    return S3E_RESULT_ERROR;

}

void FuseAPITerminate_platform()
{
    // Add any platform-specific termination code here
	if( gameConfig != NULL )
	{
		cfuhash_destroy(gameConfig);
		gameConfig = NULL;
	}

	FuseAPITerminateSession_platform();
}

void FuseAPIStartSession_platform(const char* key)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring key_jstr = env->NewStringUTF(key);
    env->CallVoidMethod(g_Obj, g_FuseAPIStartSession, key_jstr);
}

void FuseAPIPauseSession_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseAPIPauseSession);
}

void FuseAPIResumeSession_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseAPIResumeSession);
}

void FuseAPITerminateSession_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseAPITerminateSession);
}

void FuseAPIRegisterForPushNotifications_platform(const char* projectID)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring projectID_jstr = env->NewStringUTF(projectID);
    env->CallVoidMethod(g_Obj, g_FuseAPIRegisterForPushNotifications, projectID_jstr);
}

void FuseAPIRegisterEvent_platform(const char* message)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring message_jstr = env->NewStringUTF(message);
    env->CallVoidMethod(g_Obj, g_FuseAPIRegisterEvent, message_jstr);
}

int FuseAPIRegisterEventWithParam_platform(const char* name, const char* param_name, const char* param_value, const char* variable_name, const double* variable_value)
{
	JNIEnv* env = s3eEdkJNIGetEnv();
	jstring name_jstr = env->NewStringUTF(name);
	jstring param_name_jstr = env->NewStringUTF(param_name);
	jstring param_value_jstr = env->NewStringUTF(param_value);
	jstring variable_name_jstr = env->NewStringUTF(variable_name);
    return (int)env->CallIntMethod(g_Obj, g_FuseAPIRegisterEventWithParam, name_jstr, param_name_jstr, param_value_jstr, param_value_jstr, variable_name_jstr, *variable_value);
}

int FuseAPIRegisterEventWithEventData_platform(const char* name, const char* paramName, const char* paramValue, cfuhash_table_t* eventData)
{
	JNIEnv* env = s3eEdkJNIGetEnv();
	jstring name_jstr = env->NewStringUTF(name);
	jstring param_name_jstr = env->NewStringUTF(paramName);
	jstring param_value_jstr = env->NewStringUTF(paramValue);
		
	env->CallVoidMethod(g_Obj, g_FuseAPIRegisterEventStart);
	if( cfuhash_num_entries(eventData) > 0 )
	{
		char* key = NULL;
		void* value = NULL;
		cfuhash_each(eventData, &key, &value);
		do
		{
			env->CallVoidMethod(g_Obj, g_FuseAPIRegisterEventKeyValue, env->NewStringUTF(key), env->NewStringUTF((const char*)value));
		} 
		while( cfuhash_next(eventData, &key, &value) );
	}
	return (int)env->CallIntMethod(g_Obj, g_FuseAPIRegisterEventEnd, name_jstr, param_name_jstr, param_value_jstr);
}

void FuseAPIRegisterEventWithDictionary_platform(const char* message, cfuhash_table_t* eventData)
{
	JNIEnv* env = s3eEdkJNIGetEnv();
	jstring message_jstr = env->NewStringUTF(message);

	env->CallVoidMethod(g_Obj, g_FuseAPIRegisterEventWithDictionaryStart);
	if( cfuhash_num_entries(eventData) > 0 )
	{
		char* key = NULL;
		void* value = NULL;
		cfuhash_each(eventData, &key, &value);
		do
		{
			env->CallVoidMethod(g_Obj, g_FuseAPIRegisterEventWithDictionaryKeyValue, env->NewStringUTF(key), env->NewStringUTF((const char*)value));
		} 
		while( cfuhash_next(eventData, &key, &value) );
	}
	env->CallVoidMethod(g_Obj, g_FuseAPIRegisterEventWithDictionaryEnd, message_jstr);
}

void FuseAPIRegisterInAppPurchaseAndroid_platform(FusePurchaseState purchaseState, const char* purchaseToken, const char* productId, const char* orderId, long purchaseTime, const char* developerPayload, const double* price, const char* currency)
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
    env->CallVoidMethod(g_Obj, g_FuseAPIRegisterInAppPurchase, ps, purchaseToken_jstr, productId_jstr, orderId_jstr, pt, developerPayload_jstr, pp, currency_jstr);
}

void FuseAPIRegisterInAppPurchaseiOS_platform(FusePurchaseState purchaseState, const char* receiptData, int recieptDataLength, double* price, const char* currency, const char* productID)
{
    // iOS only
}

void FuseAPICheckAdAvailable_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseAPICheckAdAvailable);
}

void FuseAPIShowAd_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseAPIShowAd);
}

void FuseAPIDisplayNotifications_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseAPIDisplayNotifications);
}

void FuseAPIDisplayMoreGames_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseAPIDisplayMoreGames);
}

void FuseAPIRegisterGender_platform(int gender)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseAPIRegisterGender, gender);
}

void FuseAPIFacebookLogin_platform(const char* facebookId, const char* name, const char* accessToken)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring facebookId_jstr = env->NewStringUTF(facebookId);
    jstring name_jstr = env->NewStringUTF(name);
    jstring accessToken_jstr = env->NewStringUTF(accessToken);
    env->CallVoidMethod(g_Obj, g_FuseAPIFacebookLogin, facebookId_jstr, name_jstr, accessToken_jstr);
}

void FuseAPITwitterLogin_platform(const char* twitterId)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring twitterId_jstr = env->NewStringUTF(twitterId);
    env->CallVoidMethod(g_Obj, g_FuseAPITwitterLogin, twitterId_jstr);
}

void FuseAPIDeviceLogin_platform(const char* alias)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring alias_jstr = env->NewStringUTF(alias);
    env->CallVoidMethod(g_Obj, g_FuseAPIDeviceLogin, alias_jstr);
}

void FuseAPIFuseLogin_platform(const char* fuseId, const char* alias)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring fuseId_jstr = env->NewStringUTF(fuseId);
    jstring alias_jstr = env->NewStringUTF(alias);
    env->CallVoidMethod(g_Obj, g_FuseAPIFuseLogin, fuseId_jstr, alias_jstr);
}

void FuseAPIGameCenterLogin_platform()
{
	// only available on iOS
}

void FuseAPIGooglePlayLogin_platform(const char* alias, const char* token)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring alias_jstr = env->NewStringUTF(alias);
    jstring token_jstr = env->NewStringUTF(token);
    env->CallVoidMethod(g_Obj, g_FuseAPIGooglePlayLogin, alias_jstr, token_jstr);
}

const char* FuseAPIGetOriginalAccountId_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring returnString = (jstring)env->CallObjectMethod(g_Obj, g_FuseAPIGetOriginalAccountId);
	return s3eEdkGetStringUTF8Chars(returnString);
}

const char* FuseAPIGetOriginalAccountAlias_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring returnString  = (jstring)env->CallObjectMethod(g_Obj, g_FuseAPIGetOriginalAccountAlias);
	return s3eEdkGetStringUTF8Chars(returnString);
}

int FuseAPIGetOriginalAccountType_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    return (int)env->CallIntMethod(g_Obj, g_FuseAPIGetOriginalAccountType);
}

const char* FuseAPIGetFuseID_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring returnString  = (jstring)env->CallObjectMethod(g_Obj, g_FuseAPIGetFuseID);
	return s3eEdkGetStringUTF8Chars(returnString);
}

int FuseAPIgamesPlayed_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    return (int)env->CallIntMethod(g_Obj, g_FuseAPIgamesPlayed);
}

const char* FuseAPILibraryVersion_platform()
{
	JNIEnv* env = s3eEdkJNIGetEnv();
    jstring returnString  = (jstring)env->CallObjectMethod(g_Obj, g_FuseAPILibraryVersion);
	return s3eEdkGetStringUTF8Chars(returnString);
}

bool FuseAPIConnected_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    return (bool)env->CallBooleanMethod(g_Obj, g_FuseAPIConnected);
}

void FuseAPITimeFromServer_platform()
{
	JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseAPITimeFromServer);
}

void FuseAPIEnableData_platform(bool enable)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseAPIEnableData, enable);
}

int FuseAPISetGameData_platform(const char* key, const char* fuseId, cfuhash_table_t* gameData)
{
	JNIEnv* env = s3eEdkJNIGetEnv();
	jstring key_jstr = env->NewStringUTF(key);
	jstring fuseId_jstr = env->NewStringUTF(fuseId);

	env->CallVoidMethod(g_Obj, g_FuseAPISetGameDataStart);
	if( cfuhash_num_entries(gameData) > 0 )
	{
		char* key = NULL;
		void* gameValue = NULL;
		cfuhash_each(gameData, &key, &gameValue);
		do
		{
			FuseGameValue* gv = (FuseGameValue*)gameValue;
			env->CallVoidMethod(g_Obj, g_FuseAPISetGameDataKeyValue, env->NewStringUTF(key), env->NewStringUTF(gv->value), gv->isBinary);
		} 
		while( cfuhash_next(gameData, &key, &gameValue) );
	}
	return (int)env->CallIntMethod(g_Obj, g_FuseAPISetGameDataEnd, key_jstr, fuseId_jstr);
}

int FuseAPIGetGameData_platform(const char* key, const char* fuseId, const char** gameDataKeys, int numKeys)
{
	JNIEnv* env = s3eEdkJNIGetEnv();
	jstring key_jstr = env->NewStringUTF(key);
	jstring fuseId_jstr = env->NewStringUTF(fuseId);

	env->CallVoidMethod(g_Obj, g_FuseAPIGetGameDataStart);
	for( int i = 0; i < numKeys; i++ )
	{
		env->CallVoidMethod(g_Obj, g_FuseAPIGetGameDataKey, env->NewStringUTF(gameDataKeys[i]));
	}
	return (int)env->CallIntMethod(g_Obj, g_FuseAPIGetGameDataEnd, key_jstr, fuseId_jstr);
}

void FuseAPIAddFriend_platform(const char* fuseId)
{
	JNIEnv* env = s3eEdkJNIGetEnv();
	jstring fuseId_jstr = env->NewStringUTF(fuseId);

	env->CallVoidMethod(g_Obj, g_FuseAPIAddFriend, fuseId_jstr);
}

void FuseAPIRemoveFriend_platform(const char* fuseId)
{
	JNIEnv* env = s3eEdkJNIGetEnv();
	jstring fuseId_jstr = env->NewStringUTF(fuseId);

	env->CallVoidMethod(g_Obj, g_FuseAPIRemoveFriend, fuseId_jstr);
}

void FuseAPIAcceptFriend_platform(const char* fuseId)
{
	JNIEnv* env = s3eEdkJNIGetEnv();
	jstring fuseId_jstr = env->NewStringUTF(fuseId);

	env->CallVoidMethod(g_Obj, g_FuseAPIAcceptFriend, fuseId_jstr);
}

void FuseAPIRejectFriend_platform(const char* fuseId)
{
	JNIEnv* env = s3eEdkJNIGetEnv();
	jstring fuseId_jstr = env->NewStringUTF(fuseId);

	env->CallVoidMethod(g_Obj, g_FuseAPIRejectFriend, fuseId_jstr);
}

void FuseAPIMigrateFriends_platform(const char* fuseId)
{
	JNIEnv* env = s3eEdkJNIGetEnv();
	jstring fuseId_jstr = env->NewStringUTF(fuseId);

	env->CallVoidMethod(g_Obj, g_FuseAPIMigrateFriends, fuseId_jstr);
}

void FuseAPIUserPushNotification_platform(const char* fuseId, const char* message)
{
	JNIEnv* env = s3eEdkJNIGetEnv();
	jstring fuseId_jstr = env->NewStringUTF(fuseId);
	jstring message_jstr = env->NewStringUTF(message);

	env->CallVoidMethod(g_Obj, g_FuseAPIUserPushNotification, fuseId_jstr, message_jstr);
}

void FuseAPIFriendsPushNotification_platform(const char* message)
{
	JNIEnv* env = s3eEdkJNIGetEnv();
	jstring message_jstr = env->NewStringUTF(message);

	env->CallVoidMethod(g_Obj, g_FuseAPIFriendsPushNotification, message_jstr);
}

void FuseAPIUpdateFriendsListFromServer_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseAPIUpdateFriendsListFromServer);
}

FusePlayer* FuseAPIGetFriendsList_platform(int* numPlayers)
{
	*numPlayers = g_playerIndex;
	return g_playerList;
}

void FuseAPIGetMailListFromServer_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseAPIGetMailListFromServer);
}

void FuseAPIGetMailListFriendFromServer_platform(const char* fuseId)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring fuseId_jstr = env->NewStringUTF(fuseId);
    env->CallVoidMethod(g_Obj, g_FuseAPIGetMailListFriendFromServer, fuseId_jstr);
}

void FuseAPISetMailAsReceived_platform(int messageId)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseAPISetMailAsReceived, messageId);
}

int FuseAPISendMailWithGift_platform(const char* fuseId, const char* message, int giftId, int giftAmount)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring fuseId_jstr = env->NewStringUTF(fuseId);
    jstring message_jstr = env->NewStringUTF(message);
    return (int)env->CallIntMethod(g_Obj, g_FuseAPISendMailWithGift, fuseId_jstr, message_jstr, giftId, giftAmount);
}

int FuseAPISendMail_platform(const char* fuseId, const char* message)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring fuseId_jstr = env->NewStringUTF(fuseId);
    jstring message_jstr = env->NewStringUTF(message);
    return (int)env->CallIntMethod(g_Obj, g_FuseAPISendMail, fuseId_jstr, message_jstr);
}

FuseMail* FuseAPIGetMailList_platform(int* numEntries)
{
	*numEntries = g_numMailEntries;
	return g_mailList;
}

const char* FuseAPIGetGameConfigurationValue_platform(const char* key)
{
	JNIEnv* env = s3eEdkJNIGetEnv();
	jstring key_jstr = env->NewStringUTF(key);
    jstring returnString  = (jstring)env->CallObjectMethod(g_Obj, g_FuseAPIGetGameConfigurationValue, key_jstr);
	return s3eEdkGetStringUTF8Chars(returnString);
}

void FuseAPIRegisterLevel_platform(int level)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseAPIRegisterLevel, level);
}

void FuseAPIRegisterCurrency_platform(int type, int balance)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseAPIRegisterCurrency, type, balance);
}

void FuseAPIRegisterFlurryView_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseAPIRegisterFlurryView);
}

void FuseAPIRegisterFlurryClick_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseAPIRegisterFlurryClick);
}

void FuseAPIRegisterTapjoyReward_platform(int amount)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_FuseAPIRegisterTapjoyReward, amount);
}
