/*
 * iphone-specific implementation of the FuseAPI extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "FuseAPI_internal.h"
#include "FuseAPI-iOS.h"
#import "NSData+Base64.h"
#include "s3eEdk.h"
#include "s3eDevice.h"
#include "IwDebug.h"

#define FuseSafeRelease(obj) if (obj != nil){ \
[obj release]; \
obj = nil;}

//@TODO: We alloc new strings and copy them over to pass them to the game.  We need a way to prevent leaking of string memory.
// A string table?  Manually grabage collect the table on Pause?

// declaration of the FuseAPI delegate for callbacks
@interface FuseAPI_Delegate : NSObject<UIApplicationDelegate, FuseDelegate, FuseAdDelegate, FuseOverlayDelegate, FuseGameDataDelegate>
{
    id <UIApplicationDelegate>  m_appDelegate;
}
@end

// global variables for use in plugin
cfuhash_table_t *gameConfig = NULL;
static FuseAPI_Delegate* _FuseAPI_delegate = nil;
cfuhash_table_t* gameData = cfuhash_new_with_initial_size(10);

// friends list
FusePlayer* g_playerList = NULL;
int g_playerIndex = 0;

// mail list
FuseMail* g_mailList = NULL;
int g_numMailEntries = 0;

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
    // Add any platform-specific initialisation code here
    _FuseAPI_delegate = [FuseAPI_Delegate new];
    //s3eDeviceRegister(S3E_DEVICE_PAUSE, &FusePause, 0);
	//s3eDeviceRegister(S3E_DEVICE_UNPAUSE, &FuseResume, 0);
    
    return S3E_RESULT_SUCCESS;
}

void FuseAPITerminate_platform()
{
    // Add any platform-specific termination code here
    if( gameConfig != NULL )
    {
        cfuhash_destroy(gameConfig);
        gameConfig = NULL;
    }
}

void FuseAPIStartSession_platform(const char* key)
{
    [FuseAPI startSession:[NSString stringWithUTF8String:key] Delegate:_FuseAPI_delegate];
}

void FuseAPIPauseSession_platform()
{
}

void FuseAPIResumeSession_platform()
{
}

void FuseAPITerminateSession_platform()
{
}

void FuseAPIRegisterForPushNotifications_platform(const char* projectID)
{
    [[UIApplication sharedApplication] registerForRemoteNotificationTypes:
     (UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound | UIRemoteNotificationTypeAlert)];
}

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
void FuseAPIRegisterEvent_platform(const char* message)
{
    [FuseAPI registerEvent:[NSString stringWithUTF8String:message]];
}
#pragma GCC diagnostic warning "-Wdeprecated-declarations"

int FuseAPIRegisterEventWithParam_platform(const char* name, const char* param_name, const char* param_value, const char* variable_name, const double* variable_value)
{
    return [FuseAPI registerEvent:[NSString stringWithUTF8String:name]
                    ParameterName:[NSString stringWithUTF8String:param_name]
                   ParameterValue:[NSString stringWithUTF8String:param_value]
                     VariableName:[NSString stringWithUTF8String:variable_name]
                    VariableValue:[NSNumber numberWithDouble:*variable_value]];
}

int FuseAPIRegisterEventWithEventData_platform(const char* name, const char* paramName, const char* paramValue, cfuhash_table_t* eventData)
{
    int numEntries = cfuhash_num_entries(eventData);
    
    NSMutableDictionary* values = NULL;
    if( numEntries > 0 )
    {
        values = [[NSMutableDictionary alloc] initWithCapacity:numEntries];
        
        char* key = NULL;
        void* value = NULL;
        NSNumberFormatter* f = [[NSNumberFormatter alloc] init];
        [f setNumberStyle:NSNumberFormatterDecimalStyle];
        cfuhash_each(eventData, &key, &value);
        do
        {
            [values setObject:[f numberFromString:[NSString stringWithUTF8String:(const char*)value]] forKey:[NSString stringWithUTF8String:key]];
        }
        while( cfuhash_next(eventData, &key, &value) );
    }
    
    int retVal = [FuseAPI registerEvent:[NSString stringWithUTF8String:name]
                          ParameterName:[NSString stringWithUTF8String:paramName]
                          ParameterValue:[NSString stringWithUTF8String:paramValue]
                              Variables:values];

    FuseSafeRelease(values);
    return retVal;
}

void FuseAPIRegisterEventWithDictionary_platform(const char* message, cfuhash_table_t* eventData)
{
    int numEntries = cfuhash_num_entries(eventData);
    
    NSMutableDictionary* values = NULL;    
    if( numEntries > 0 )
    {
        values = [[NSMutableDictionary alloc] initWithCapacity:numEntries];
        
        char* key = NULL;
        void* value = NULL;
        cfuhash_each(eventData, &key, &value);
        do
        {
            [values setObject:[NSString stringWithUTF8String:(const char*)value] forKey:[NSString stringWithUTF8String:key]];
        }
        while( cfuhash_next(eventData, &key, &value) );
    }
    [FuseAPI registerEvent:[NSString stringWithUTF8String:message] withDict:values];
    FuseSafeRelease(values);    
}

void FuseAPIRegisterInAppPurchaseAndroid_platform(FusePurchaseStateAndroid purchaseState, const char* purchaseToken, const char* productId, const char* orderId, long purchaseTime, const char* developerPayload, const double* price, const char* currency)
{
    // Android only
}

void FuseAPIRegisterInAppPurchaseiOS_platform(FusePurchaseStateiOS purchaseState, const char* receiptData, int recieptDataLength, double* price, const char* currency, const char* productID, const char* transactionID)
{
    NSData* receipt = [[NSData marmaladeDataFromBase64String:[NSString stringWithUTF8String:receiptData]] retain]; 
    [FuseAPI registerInAppPurchase:receipt
                           TxState:purchaseState
                             Price:[NSString stringWithFormat:@"%.2f", *price]
                          Currency:[NSString stringWithUTF8String:currency]
                         ProductID:[NSString stringWithUTF8String:productID]
                     TransactionID:[NSString stringWithUTF8String:transactionID]];
    FuseSafeRelease(receipt);
}

void FuseAPICheckAdAvailable_platform()
{
    [FuseAPI checkAdAvailableWithDelegate:_FuseAPI_delegate];
}

void FuseAPIShowAd_platform()
{
    [FuseAPI showAdWithDelegate:_FuseAPI_delegate];
}

void FuseAPIDisplayNotifications_platform()
{
    [FuseAPI displayNotifications];
}

void FuseAPIDisplayMoreGames_platform()
{
    [FuseAPI displayMoreGames:_FuseAPI_delegate];
}

void FuseAPIRegisterGender_platform(int gender)
{
    [FuseAPI registerGender:gender];
}

void FuseAPIFacebookLogin_platform(const char* facebookId, const char* name, const char* accessToken)
{
    [FuseAPI facebookLogin:facebookId != NULL ? [NSString stringWithUTF8String:facebookId] : @""
                      Name:name != NULL ? [NSString stringWithUTF8String:name] : @""
           withAccessToken:accessToken != NULL ? [NSString stringWithUTF8String:accessToken] : @""];
}

void FuseAPITwitterLogin_platform(const char* twitterId)
{
    [FuseAPI twitterLogin:[NSString stringWithUTF8String:twitterId]];
}

void FuseAPIGameCenterLogin_platform()
{
    [FuseAPI gameCenterLogin:[GKLocalPlayer localPlayer]];
}

void FuseAPIDeviceLogin_platform(const char* alias)
{
    [FuseAPI deviceLogin:[NSString stringWithUTF8String:alias]];
}

void FuseAPIFuseLogin_platform(const char* fuseId, const char* alias)
{
    [FuseAPI fuseLogin:[NSString stringWithUTF8String:fuseId] Alias:[NSString stringWithUTF8String:alias]];
}

void FuseAPIGooglePlayLogin_platform(const char* alias, const char* token)
{
    [FuseAPI googlePlayLogin:[NSString stringWithUTF8String:alias] AccessToken:[NSString stringWithUTF8String:token]];
}

const char* FuseAPIGetOriginalAccountId_platform()
{
    NSString* retValue = [FuseAPI getOriginalAccountID];
    if( retValue == nil )
        return "";
    const char* string = retValue.UTF8String;
	char* copy = (char*)malloc(strlen(string) + 1);
	strcpy(copy, string);
    
    return copy;
}

const char* FuseAPIGetOriginalAccountAlias_platform()
{
    NSString* retValue = [FuseAPI getOriginalAccountAlias];
    if( retValue == nil )
        return "";
    const char* string = retValue.UTF8String;
	char* copy = (char*)malloc(strlen(string) + 1);
	strcpy(copy, string);
    
    return copy;
}

int FuseAPIGetOriginalAccountType_platform()
{
    int retValue = [FuseAPI getOriginalAccountType];
    return retValue;
}

const char* FuseAPIGetFuseID_platform()
{
    NSString* retValue = [FuseAPI getFuseID];
    if( retValue == nil )
        return "";
    
    const char* string = retValue.UTF8String;
	char* copy = (char*)malloc(strlen(string) + 1);
	strcpy(copy, string);
    
    return copy;
}

int FuseAPIgamesPlayed_platform()
{
    int retValue = [FuseAPI gamesPlayed];
    return retValue;
}

const char* FuseAPILibraryVersion_platform()
{
    NSString* ver = [FuseAPI libraryVersion];    
    const char* string = ver.UTF8String;
	char* copy = (char*)malloc(strlen(string) + 1);
	strcpy(copy, string);
    
    return copy;
}

bool FuseAPIConnected_platform()
{
    return [FuseAPI connected];
}

void FuseAPITimeFromServer_platform()
{
    [FuseAPI utcTimeFromServer];
}

void FuseAPIEnableData_platform(bool enable)
{
    if( enable )
    {
        [FuseAPI enableData];
    }
    else
    {
        [FuseAPI disableData];
    }
}

int FuseAPISetGameData_platform(const char* masterKey, const char* fuseId, cfuhash_table_t* gameData)
{
    
    int numEntries = cfuhash_num_entries(gameData);
    
    NSMutableDictionary* values = NULL;
    if( numEntries > 0 )
    {
        values = [[NSMutableDictionary alloc] initWithCapacity:numEntries];
        
        char* key = NULL;
        void* value = NULL;
        cfuhash_each(gameData, &key, &value);
        do
        {
            FuseGameValue* gv = (FuseGameValue*)value;
            //NSLog(@"key: %s, value: %s", key, (const char*)value);
            if( gv->isBinary )
            {
                NSData* buffer = [NSData marmaladeDataFromBase64String:[NSString stringWithUTF8String:gv->value]];
                [values setValue:buffer forKey:[NSString stringWithUTF8String:key]];
            }
            else
            {
                [values setObject:[NSString stringWithUTF8String:gv->value] forKey:[NSString stringWithUTF8String:key]];
            }            
        }
        while( cfuhash_next(gameData, &key, &value) );
    }
    
    int retValue = [FuseAPI setGameData:values
                                 FuseID:[NSString stringWithUTF8String:fuseId]
                                    Key:[NSString stringWithUTF8String:masterKey]
                               Delegate:_FuseAPI_delegate
                           IsCollection:NO];    //@TODO: collection?
    FuseSafeRelease(values);
    
    return retValue;
}

int FuseAPIGetGameData_platform(const char* key, const char* fuseId, const char** gameDataKeys, int numKeys)
{
    
    NSMutableArray* keys = [[NSMutableArray alloc] init];
    for( int i = 0; i < numKeys; i++ )
    {
        [keys addObject:[NSString stringWithUTF8String:gameDataKeys[i]]];
    }
    
    NSString* gameDataKey = [NSString stringWithUTF8String:key];
    NSString* gameDataFuseID = [NSString stringWithUTF8String:fuseId];
    
    int requestId = -1;
	if (gameDataKey.length != 0)
	{
		if (gameDataFuseID.length != 0)
		{
			requestId = [FuseAPI getFriendGameData:keys
                                               Key:gameDataKey
                                            FuseID:gameDataFuseID
                                          Delegate:_FuseAPI_delegate];
		}
		else
		{
			requestId = [FuseAPI getGameData:keys
                                         Key:gameDataKey
                                    Delegate:_FuseAPI_delegate];
		}
	}
	else
	{
		if (gameDataFuseID.length != 0)
		{
			requestId = [FuseAPI getFriendGameData:keys
                                            FuseID:gameDataFuseID
                                          Delegate:_FuseAPI_delegate];
		}
		else
		{
			requestId = [FuseAPI getGameData:keys
                                    Delegate:_FuseAPI_delegate];
		}
	}
    FuseSafeRelease(keys);
	gameDataKey = nil;
	gameDataFuseID = nil;
	
	return requestId;
}

void FuseAPIAddFriend_platform(const char* fuseId)
{
	[FuseAPI addFriend:[NSString stringWithUTF8String:fuseId]];
}

void FuseAPIRemoveFriend_platform(const char* fuseId)
{
	[FuseAPI removeFriend:[NSString stringWithUTF8String:fuseId]];
}

void FuseAPIAcceptFriend_platform(const char* fuseId)
{
	[FuseAPI acceptFriend:[NSString stringWithUTF8String:fuseId]];
}

void FuseAPIRejectFriend_platform(const char* fuseId)
{
	[FuseAPI rejectFriend:[NSString stringWithUTF8String:fuseId]];
}

void FuseAPIMigrateFriends_platform(const char* fuseId)
{
    [FuseAPI migrateFriends:[NSString stringWithUTF8String:fuseId]];
}

void FuseAPIUserPushNotification_platform(const char* fuseId, const char* message)
{
	[FuseAPI userPushNotification:[NSString stringWithUTF8String:fuseId] Message:[NSString stringWithUTF8String:message]];
}

void FuseAPIFriendsPushNotification_platform(const char* message)
{
	[FuseAPI friendsPushNotification:[NSString stringWithUTF8String:message]];
}

void FuseAPIUpdateFriendsListFromServer_platform()
{
    [FuseAPI updateFriendsListFromServer];
}

FusePlayer* FuseAPIGetFriendsList_platform(int* numPlayers)
{
    *numPlayers = g_playerIndex;
    return g_playerList;
}

void FuseAPIGetMailListFromServer_platform()
{
    [FuseAPI getMailListFromServer];
}

void FuseAPIGetMailListFriendFromServer_platform(const char* fuseId)
{
    [FuseAPI getMailListFriendFromServer:[NSString stringWithUTF8String:fuseId]];
}

void FuseAPISetMailAsReceived_platform(int messageId)
{
    [FuseAPI setMailAsReceived:messageId];
}

int FuseAPISendMailWithGift_platform(const char* fuseId, const char* message, int giftId, int giftAmount)
{
    return [FuseAPI sendMailWithGift:[NSString stringWithUTF8String:fuseId]
                             Message:[NSString stringWithUTF8String:message]
                              GiftID:giftId
                          GiftAmount:giftAmount];
}

int FuseAPISendMail_platform(const char* fuseId, const char* message)
{
    return [FuseAPI sendMail:[NSString stringWithUTF8String:fuseId]
                     Message:[NSString stringWithUTF8String:message]];
}

FuseMail* FuseAPIGetMailList_platform(int* numEntries)
{
    *numEntries = g_numMailEntries;
    return g_mailList;
}

const char* FuseAPIGetGameConfigurationValue_platform(const char* key)
{
    const char* string = [FuseAPI getGameConfigurationValue:[NSString stringWithUTF8String:key]].UTF8String;
	char* copy = (char*)malloc(strlen(string) + 1);
	strcpy(copy, string);
    
    return copy;
}

void FuseAPIRegisterLevel_platform(int level)
{
    [FuseAPI registerLevel:level];
}

void FuseAPIRegisterCurrency_platform(int type, int balance)
{
    [FuseAPI registerCurrency:type Balance:balance];
}

void FuseAPIRegisterFlurryView_platform()
{
    [FuseAPI registerFlurryView];
}

void FuseAPIRegisterFlurryClick_platform()
{
    [FuseAPI registerFlurryClick];
}

void FuseAPIRegisterTapjoyReward_platform(int amount)
{
    [FuseAPI registerTapjoyReward:amount];
}

#pragma mark - Callback

@implementation FuseAPI_Delegate

#pragma mark Initialization

- (id) init
{
    self = [super init];
    
	if (self)
    {
        m_appDelegate = [[UIApplication sharedApplication] delegate];        
        [[UIApplication sharedApplication] setDelegate:self];
    }
    
    return self;
}

+ (void)load
{
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationDidFinishLaunching:) name:UIApplicationDidFinishLaunchingNotification object:nil];
}

+ (void)applicationDidFinishLaunching:(NSNotification *)notification
{	
}

#pragma mark Session Creation

- (void)sessionStartReceived
{
    IwTrace(FuseAPI, ("FuseAPISessionStartReceived()"));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_SESSION_STARTED, 0, 0);
}

- (void)sessionLoginError:(NSNumber*)_error
{
    struct paramList
	{
		int error;
	};
	paramList params;
	params.error = _error.intValue;
    IwTrace(FuseAPI, ("FuseAPILoginError(%i)", params.error));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_SESSION_LOGIN_ERROR, &params, sizeof(paramList));
}

#pragma mark In-App Purchase Logging

- (void)puchaseVerification:(NSNumber*)_verified TransactionID:(NSString*)_tx_id OriginalTransactionID:(NSString*)_o_tx_id
{
    struct paramList
	{
		int verified;
        const char* transactionID;
        const char* originalTransactionID;
	};
	paramList params;
    params.verified = _verified.intValue;
    
    const char* string = _tx_id.UTF8String;
	char* copy = (char*)malloc(strlen(string) + 1);
	strcpy(copy, string);
    params.transactionID = copy;
    
    string = _o_tx_id.UTF8String;
    copy = (char*)malloc(strlen(string) + 1);
	strcpy(copy, string);
    params.originalTransactionID = copy;    
    
    IwTrace(FuseAPI, ("FuseAPIPurchaseVerified(%i, %s, %s)", params.verified, params.transactionID, params.originalTransactionID));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_PURCHASE_VERIFIED, &params, sizeof(paramList));
}

#pragma mark Fuse Interstitial Ads

- (void) adAvailabilityResponse:(NSNumber*)_available Error:(NSNumber*)_error
{
	struct paramList
	{
		int available;
		int error;
	};
	paramList params;
	params.available = _available.intValue;
	params.error = _error.intValue;
    IwTrace(FuseAPI, ("FuseAPIAdAvailabilityResponse(%i, %i)", params.available, params.error));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_AD_AVAILABILITY_RESPONSE, &params, sizeof(paramList));
}

- (void)adWillClose
{
	IwTrace(FuseAPI, ("FuseAPIAdWillClose()"));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_AD_WILL_CLOSE, 0, 0);
}

#pragma mark Notifications

-(void) notificationAction:(NSString*)_action
{
	struct paramList
	{
		const char* action;
	};
	paramList params;
    const char* string = _action.UTF8String;
	char* copy = (char*)malloc(strlen(string) + 1);
	strcpy(copy, string);    
	params.action = copy;
    IwTrace(FuseAPI, ("FuseAPINotificationAction(%s)", params.action));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_NOTIFICATION_ACTION, &params, sizeof(paramList));
}

#pragma mark More Games

- (void)overlayWillClose
{
	IwTrace(FuseAPI, ("FuseAPIOverLayWillClose()"));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_OVERLAY_WILL_CLOSE, 0, 0);
}

#pragma mark Account Login

- (void)accountLoginComplete:(NSNumber*)_type Account:(NSString*)_account_id
{    
    struct paramList
	{
		int accountType;
		const char* accountID;
	};
	paramList params;
	params.accountType = _type.intValue;
    const char* string = _account_id.UTF8String;
	char* copy = (char*)malloc(strlen(string) + 1);
	strcpy(copy, string);    
	params.accountID = copy;
    IwTrace(FuseAPI, ("FuseAPIAccountLoginComplete(%i, %s)", params.accountType, params.accountID));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_ACCOUNT_LOGIN_COMPLETE, &params, sizeof(paramList));    
}

#pragma mark Miscellaneous

- (void)timeUpdated:(NSNumber*)_timeStamp
{    
    struct paramList
	{
		int timestamp;
	};
	paramList params;
	params.timestamp = [_timeStamp longLongValue];
    IwTrace(FuseAPI, ("FuseAPITimeUpdated(%i)", params.timestamp));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_TIME_UPDATED, &params, sizeof(paramList));
}

#pragma mark User Game Data

-(void) gameDataReceived:(NSString*)_fuse_id ForKey:(NSString*)_key Data:(NSMutableDictionary*)_data
{
    [self gameDataReceived:_fuse_id ForKey:_key Data:_data RequestID:[NSNumber numberWithInt:0]];
}

- (void)gameDataReceived:(NSString *)_user_account_id ForKey:(NSString *)_key Data:(NSMutableDictionary *)_data RequestID:(NSNumber *)_request_id
{
    IwTrace(FuseAPI, ("FuseAPIGameDataReceived()"));
    struct paramList
	{
		cfuhash_table_t* gameData;
        const char* accountID;
        int requestID;
	};
	paramList params;
    
    cfuhash_clear(gameData);    
    
	for (NSString* key in _data)
	{
		NSObject* value = [_data objectForKey:key];		
        NSString* stringVal = [_data objectForKey:key];
		if (value && [value isKindOfClass:[NSData class]] == YES)
		{
			NSData* buffer = (NSData*)value;			
			stringVal = buffer.marmaladeBase64EncodedString;
		}
        const char* string = stringVal.UTF8String;
        char* copy = (char*)malloc(strlen(string) + 1);
        strcpy(copy, string);
        cfuhash_put(gameData, key.UTF8String, (void*)copy);
	}
    params.gameData = gameData;
    params.accountID = _user_account_id.UTF8String;    
    params.requestID = _request_id.intValue;
    
	s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_GAME_DATA_RECEIVED, &params, sizeof(paramList));
}

- (void)gameDataError:(NSNumber*)_error RequestID:(NSNumber*)_request_id
{
    struct paramList
	{
		int requestId;
		int error;
	};
	paramList params;
	params.requestId = _request_id.intValue;
	params.error = _error.intValue;
    IwTrace(FuseAPI, ("FuseGameDataError(%i, %i)", params.requestId, params.error));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_GAME_DATA_ERROR, &params, sizeof(paramList));
}

- (void)gameDataSetAcknowledged:(NSNumber*)_request_id
{
    struct paramList
	{
		int requestID;
	};
	paramList params;
	params.requestID = _request_id.intValue;
    IwTrace(FuseAPI, ("FuseGameDataSetAcknowledged(%i)", params.requestID));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_GAME_DATASET_ACKNOWLEDGED, &params, sizeof(paramList));
}

#pragma mark Friends List
-(void) friendAdded:(NSString*)_fuse_id Error:(NSNumber*)_error
{
    struct paramList
	{
        const char* fuseId;
		int error;
	};
	paramList params;
	params.error = _error.intValue;
    params.fuseId = _fuse_id.UTF8String;
    IwTrace(FuseAPI, ("FuseFriendAdded(%s, %i)", params.fuseId, params.error));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_FRIEND_ADDED, &params, sizeof(paramList));
}

-(void) friendRemoved:(NSString*)_fuse_id Error:(NSNumber*)_error
{
    struct paramList
	{
        const char* fuseId;
		int error;
	};
	paramList params;
	params.error = _error.intValue;
    params.fuseId = _fuse_id.UTF8String;
    IwTrace(FuseAPI, ("FuseFriendRemoved(%s, %i)", params.fuseId, params.error));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_FRIEND_REMOVED, &params, sizeof(paramList));
}

-(void) friendAccepted:(NSString*)_fuse_id Error:(NSNumber*)_error
{
    struct paramList
	{
        const char* fuseId;
		int error;
	};
	paramList params;
	params.error = _error.intValue;
    params.fuseId = _fuse_id.UTF8String;
    IwTrace(FuseAPI, ("FuseFriendAccepted(%s, %i)", params.fuseId, params.error));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_FRIEND_ACCEPTED, &params, sizeof(paramList));
}

-(void) friendRejected:(NSString*)_fuse_id Error:(NSNumber*)_error
{
    struct paramList
	{
        const char* fuseId;
		int error;
	};
	paramList params;
	params.error = _error.intValue;
    params.fuseId = _fuse_id.UTF8String;
    IwTrace(FuseAPI, ("FuseFriendsRejected(%s, %i)", params.fuseId, params.error));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_FRIEND_REJECTED, &params, sizeof(paramList));
}

-(void) friendsMigrated:(NSString*)_fuse_id Error:(NSNumber*)_error
{
    struct paramList
	{
        const char* fuseId;
		int error;
	};
	paramList params;
	params.error = _error.intValue;
    params.fuseId = _fuse_id.UTF8String;
    IwTrace(FuseAPI, ("FuseFriendsMigrated(%s, %i)", params.fuseId, params.error));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_FRIENDS_MIGRATED, &params, sizeof(paramList));
}

-(void) friendsListUpdated:(NSDictionary*)_friendsList
{
    IwTrace(FuseAPI, ("FuseAPIFriendsListUpdated"));
	if( g_playerList != NULL )
	{
		delete(g_playerList);
	}
	g_playerList = (FusePlayer*)malloc(sizeof(FusePlayer)*_friendsList.count);
	g_playerIndex = 0;
    
	for (NSString* fuseId in _friendsList)
	{
		NSDictionary* friendEntry = (NSDictionary*)[_friendsList objectForKey:fuseId];
        
        const char* string = fuseId.UTF8String;
        char* copy = (char*)malloc(strlen(string) + 1);
        strcpy(copy, string);
        g_playerList[g_playerIndex].fuseId = copy;
        
		NSString* accountId = [friendEntry objectForKey:@"account_id"];
        string = accountId.UTF8String;
        copy = (char*)malloc(strlen(string) + 1);
        strcpy(copy, string);
        g_playerList[g_playerIndex].accountId = copy;
        
		NSString* alias = [friendEntry objectForKey:@"alias"];
        string = alias.UTF8String;
        copy = (char*)malloc(strlen(string) + 1);
        strcpy(copy, string);
        g_playerList[g_playerIndex].alias = copy;
        
        NSString* type = [friendEntry objectForKey:@"type"];
        string = type.UTF8String;
        copy = (char*)malloc(strlen(string) + 1);
        strcpy(copy, string);
        g_playerList[g_playerIndex].type = copy;
        
        int level = [[friendEntry objectForKey:@"level"] intValue];
		int pending = [[friendEntry objectForKey:@"pending"] intValue];
        int canAttack = [[friendEntry objectForKey:@"canAttack"] intValue];
        g_playerList[g_playerIndex].level = level;
        g_playerList[g_playerIndex].pending = pending;
        g_playerList[g_playerIndex].canAttack = (bool)canAttack;
        g_playerIndex++;        
	}
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_FRIENDSLIST_UPDATED, 0, 0);
}

-(void) friendsListError:(NSNumber*)_error
{
	struct paramList
	{
		int error;
	};
	paramList params;
	params.error = _error.intValue;
    IwTrace(FuseAPI, ("FuseFriendsListError(%i)", params.error));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_FRIENDSLIST_ERROR, &params, sizeof(paramList));
}

#pragma mark Gifting

-(void) mailListRecieved:(NSDictionary*)_messages User:(NSString*)_fuse_id
{
    IwTrace(FuseAPI, ("FuseAPIMailListReceived()"));
	if( g_mailList != NULL )
	{
		delete(g_mailList);
	}
	g_mailList = (FuseMail*)malloc(sizeof(FuseMail)*_messages.count);
	g_numMailEntries = 0;
    
    struct paramList
	{
		const char* fuseId;
	};
	paramList params;
    const char* fuseIDString = _fuse_id.UTF8String;
    char* fuseIDCopy = (char*)malloc(strlen(fuseIDString) + 1);
    strcpy(fuseIDCopy, fuseIDString);
	params.fuseId = fuseIDCopy;
    
    for (NSNumber* messageId in _messages)
	{
		NSDictionary* mail = (NSDictionary*)[_messages objectForKey:messageId];
		
        // copy strings
        NSString* alias = [mail objectForKey:@"alias"];
        const char* string = alias.UTF8String;
        char* copy = (char*)malloc(strlen(string) + 1);
        strcpy(copy, string);
        g_mailList[g_numMailEntries].alias = copy;
        
        NSString* message = [mail objectForKey:@"message"];
        string = message.UTF8String;
        copy = (char*)malloc(strlen(string) + 1);
        strcpy(copy, string);        
        g_mailList[g_numMailEntries].message = copy;
        
        NSString* giftName = [mail objectForKey:@"gift_name"];
        string = giftName.UTF8String;
        copy = (char*)malloc(strlen(string) + 1);
        strcpy(copy, string);
        g_mailList[g_numMailEntries].gift.name = copy;
        
        g_mailList[g_numMailEntries].fuseId = fuseIDCopy;
        
        NSNumber* timestamp = [mail objectForKey:@"timestamp"];
        string = timestamp.stringValue.UTF8String;
        copy = (char*)malloc(strlen(string) + 1);
        strcpy(copy, string);
        g_mailList[g_numMailEntries].date = copy;
        
        // do we need a gift url?
        //NSString* giftUrl = [mail objectForKey:@"url"];
        g_mailList[g_numMailEntries].gift.url = "";//giftUrl.UTF8String;
        
        // numbers
		int giftId = [[mail objectForKey:@"gift_id"] intValue];
        int giftAmount = [[mail objectForKey:@"gift_amount"] intValue];        
        
        g_mailList[g_numMailEntries].id = messageId.intValue;
        g_mailList[g_numMailEntries].gift.id = giftId;        
        g_mailList[g_numMailEntries].gift.amount = giftAmount;        
        g_numMailEntries++;
	}    
    
	s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_MAILLIST_RECEIVED, &params, sizeof(paramList));
}

-(void) mailListError:(NSNumber*)_error
{
    struct paramList
	{
		int error;
	};
	paramList params;
	params.error = _error.intValue;
    IwTrace(FuseAPI, ("FuseMailError(%i)", params.error));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_MAIL_ERROR, &params, sizeof(paramList));
}

-(void) mailAcknowledged:(NSNumber*)_message_id User:(NSString*)_fuse_id
{
    struct paramList
	{
		int messageId;
		const char* fuseID;
		int requestID;
	};
	paramList params;
	params.messageId = _message_id.intValue;
	params.fuseID = _fuse_id.UTF8String;
	params.requestID = 0;
    IwTrace(FuseAPI, ("FuseMailAcknowledged(%i, %s)", params.messageId, params.fuseID));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_MAIL_ACKNOWLEDGED, &params, sizeof(paramList));
}

-(void) mailError:(NSNumber*)_error
{
	struct paramList
	{
		int error;
	};
	paramList params;
	params.error = _error.intValue;
    IwTrace(FuseAPI, ("FuseMailError(%i)", params.error));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_MAIL_ERROR, &params, sizeof(paramList));
}

#pragma mark Game Configuration Data

- (void)gameConfigurationReceived
{
    IwTrace(FuseAPI, ("FuseGameConfigurationReceived()"));
    
    struct paramList
	{
		cfuhash_table_t* gameConfig;
	};
	paramList params;	
    
    NSMutableDictionary* gc = [FuseAPI getGameConfiguration];
    NSMutableDictionary* test = gc.copy;
    if( gameConfig == NULL )
	{
		gameConfig = cfuhash_new_with_initial_size(gc.count);
	}
	cfuhash_clear(gameConfig);
    
    if (gc != nil && [test count] > 0)
    {
        NSArray *keys = [test allKeys];
        
        for (unsigned int i = 0; i < [keys count]; i++)
        {
            NSString *key = [keys objectAtIndex:i];
            NSString *value = [test objectForKey:key];
            
            //NSLog(@"Key: %@, Value: %@", key, value);
            const char* string = value.UTF8String;
            char* copy = (char*)malloc(strlen(string) + 1);
            strcpy(copy, string);

            cfuhash_put(gameConfig, key.UTF8String, (void*)copy);
        }
    }
    FuseSafeRelease(test);
    params.gameConfig = gameConfig;
	s3eEdkCallbacksEnqueue(S3E_EXT_FUSEAPI_HASH, FUSEAPI_GAME_CONFIGURATION_RECEIVED, &params, sizeof(paramList));
}

#pragma mark - Application Delegate Functions
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
	[FuseAPI applicationdidRegisterForRemoteNotificationsWithDeviceToken:deviceToken];
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
    NSLog(@"ERROR registering for remote notification: %@", [error localizedDescription]);
}

// These are forwarded to the real application delegate
- (BOOL) application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    return [m_appDelegate application:application didFinishLaunchingWithOptions:launchOptions];
}

//- (void) application:(UIApplication *)application didChangeStatusBarFrame:(CGRect)oldStatusBarFrame
//{
//    [m_appDelegate application:application didChangeStatusBarFrame:oldStatusBarFrame];
//}

//- (void) application:(UIApplication *)application didChangeStatusBarOrientation:(UIInterfaceOrientation)oldStatusBarOrientation
//{
//    [m_appDelegate application:application didChangeStatusBarOrientation:oldStatusBarOrientation];
//}

- (void) application:(UIApplication *)application didDecodeRestorableStateWithCoder:(NSCoder *)coder
{
    [m_appDelegate application:application didDecodeRestorableStateWithCoder:coder];
}

- (void) application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification *)notification
{
    [m_appDelegate application:application didReceiveLocalNotification:notification];
}

- (void) application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
{
    [m_appDelegate application:application didReceiveRemoteNotification:userInfo];
}

- (BOOL) application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    return [m_appDelegate application:application openURL:url sourceApplication:sourceApplication annotation:annotation];
}

//- (BOOL) application:(UIApplication *)application shouldRestoreApplicationState:(NSCoder *)coder
//{
//    return [m_appDelegate application:application shouldRestoreApplicationState:coder];
//}
//
//- (BOOL) application:(UIApplication *)application shouldSaveApplicationState:(NSCoder *)coder
//{
//    return [m_appDelegate application:application shouldSaveApplicationState:coder];
//}

//- (NSUInteger) application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window
//{
//    return [m_appDelegate application:application supportedInterfaceOrientationsForWindow:window];
//}

- (UIViewController*) application:(UIApplication *)application viewControllerWithRestorationIdentifierPath:(NSArray *)identifierComponents coder:(NSCoder *)coder
{
    return [m_appDelegate application:application viewControllerWithRestorationIdentifierPath:identifierComponents coder:coder];
}

//- (void) application:(UIApplication *)application willChangeStatusBarFrame:(CGRect)newStatusBarFrame
//{
//    [m_appDelegate application:application willChangeStatusBarFrame:newStatusBarFrame];
//}

//- (void) application:(UIApplication *)application willChangeStatusBarOrientation:(UIInterfaceOrientation)newStatusBarOrientation duration:(NSTimeInterval)duration
//{
//    [m_appDelegate application:application willChangeStatusBarOrientation:newStatusBarOrientation duration:duration];
//}

- (void) application:(UIApplication *)application willEncodeRestorableStateWithCoder:(NSCoder *)coder
{
    [m_appDelegate application:application willEncodeRestorableStateWithCoder:coder];
}

- (BOOL) application:(UIApplication *)application willFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    return [m_appDelegate application:application willFinishLaunchingWithOptions:launchOptions];
}

- (void) applicationDidBecomeActive:(UIApplication *)application
{
    [m_appDelegate applicationDidBecomeActive:application];
}

- (void) applicationDidEnterBackground:(UIApplication *)application
{
    [m_appDelegate applicationDidEnterBackground:application];
}

- (void) applicationDidFinishLaunching:(UIApplication *)application
{
    [m_appDelegate applicationDidFinishLaunching:application];
}

- (void) applicationDidReceiveMemoryWarning:(UIApplication *)application
{
    [m_appDelegate applicationDidReceiveMemoryWarning:application];
}

- (void) applicationProtectedDataDidBecomeAvailable:(UIApplication *)application
{
    [m_appDelegate applicationProtectedDataDidBecomeAvailable:application];
}

- (void) applicationProtectedDataWillBecomeUnavailable:(UIApplication *)application
{
    [m_appDelegate applicationProtectedDataWillBecomeUnavailable:application];
}

- (void) applicationSignificantTimeChange:(UIApplication *)application
{
    [m_appDelegate applicationSignificantTimeChange:application];
}

- (void) applicationWillEnterForeground:(UIApplication *)application
{
    [m_appDelegate applicationWillEnterForeground:application];
}

- (void) applicationWillResignActive:(UIApplication *)application
{
    [m_appDelegate applicationWillResignActive:application];
}

- (void) applicationWillTerminate:(UIApplication *)application
{
    [m_appDelegate applicationWillTerminate:application];
}


@end
