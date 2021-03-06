/*
 * iphone-specific implementation of the FuseSDK extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "FuseSDK_internal.h"
#include "FuseSDK-iOS.h"
#import "NSData+Base64.h"
#include "s3eEdk.h"
#include "s3eDevice.h"
#include "IwDebug.h"
#include "cfuhash.h"

#define FuseSafeRelease(obj) if (obj != nil){ \
[obj release]; \
obj = nil;}

const char* kFuseSDKRewardedAdOptionKey_ShowPreRoll = "FuseRewardedAdOptionKey_ShowPreRoll"; // set "0" for false or "1" for true
const char* kFuseSDKRewardedAdOptionKey_ShowPostRoll = "FuseRewardedAdOptionKey_ShowPostRoll"; // set "0" for false or "1" for true
const char* kFuseSDKRewardedOptionKey_PreRollYesButtonText = "FuseRewardedOptionKey_PreRollYesButtonText";
const char* kFuseSDKRewardedOptionKey_PreRollNoButtonText = "FuseRewardedOptionKey_PreRollNoButtonText";
const char* kFuseSDKRewardedOptionKey_PostRollContinueButtonText = "FuseRewardedOptionKey_PostRollContinueButtonText";

// declaration of the FuseSDK delegate for callbacks
@interface FuseSDK_Delegate : NSObject<UIApplicationDelegate, FuseDelegate>
{
    id <UIApplicationDelegate>  m_appDelegate;
}
@end

// global variables for use in plugin
cfuhash_table_t *gameConfig = NULL;
static FuseSDK_Delegate* _FuseSDK_delegate = nil;

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
    // Add any platform-specific initialisation code here
    IwTrace(FuseSDK, ("Starting init..."));
    _FuseSDK_delegate = [FuseSDK_Delegate new];
    //s3eDeviceRegister(S3E_DEVICE_PAUSE, &FusePause, 0);
	//s3eDeviceRegister(S3E_DEVICE_UNPAUSE, &FuseResume, 0);
    IwTrace(FuseSDK, ("Init complete!"));
    
    return S3E_RESULT_SUCCESS;
}

void FuseSDKTerminate_platform()
{
    // Add any platform-specific termination code here
    if( gameConfig != NULL )
    {
        cfuhash_destroy(gameConfig);
        gameConfig = NULL;
    }
}

void FuseSDKStartSession_platform(const char* apikey, cfuhash_table_t* options)
{
	int numOptions = cfuhash_num_entries(options);
	NSMutableDictionary* sessionOptions = nil;

	if( numOptions > 0 )
	{
		sessionOptions = [[NSMutableDictionary alloc] initWithCapacity:numOptions];
		char* key = NULL;
		void* data = NULL;
		if( cfuhash_each(options, &key,&data) )
		{
			do
			{
                NSString* valueString = data ? [NSString stringWithUTF8String:(const char*)data] : @"";
                NSString* keyString = key ? [NSString stringWithUTF8String:key] : @"";
				[sessionOptions setValue:valueString forKey:keyString];
			}
			while( cfuhash_next(options, &key, &data) );
		}
	}
    
    [FuseSDK setPlatform:@"marmalade-ios"]; 
    [FuseSDK startSession:[NSString stringWithUTF8String:apikey] delegate:_FuseSDK_delegate withOptions:sessionOptions];
}

void FuseSDKPauseSession_platform()
{
}

void FuseSDKResumeSession_platform()
{
}

void FuseSDKTerminateSession_platform()
{
}

void FuseSDKRegisterForPushNotifications_platform(const char* projectID)
{
	[FuseSDK registerForPushToken];
}

void FuseSDKRegisterInAppPurchaseAndroid_platform(FusePurchaseStateAndroid purchaseState, const char* purchaseToken, const char* productId, const char* orderId, long purchaseTime, const char* developerPayload, const double* price, const char* currency)
{
    // Android only
}

void FuseSDKRegisterInAppPurchaseiOS_platform(FusePurchaseStateiOS purchaseState, const char* receiptData, int recieptDataLength, double* price, const char* currency, const char* productID, const char* transactionID)
{
    NSData* receipt = receiptData ? [[NSData marmaladeDataFromBase64String:[NSString stringWithUTF8String:receiptData]] retain] : nil;
    [FuseSDK registerInAppPurchase:receipt
                           TxState:purchaseState
                             Price:[NSString stringWithFormat:@"%.2f", *price]
                          Currency:[NSString stringWithUTF8String:currency]
                         ProductID:[NSString stringWithUTF8String:productID]
                     TransactionID:[NSString stringWithUTF8String:transactionID]];
    FuseSafeRelease(receipt);
}

void FuseSDKRegisterVirtualGoodsPurchase_platform(int virtualGoodsID, int purchaseAmount, int currencyID)
{
    [FuseSDK registerVirtualGoodsPurchase:virtualGoodsID Amount:purchaseAmount CurrencyID:currencyID];
}

void FuseSDKPreloadAdForZoneID_platform(const char* zoneID)
{
    [FuseSDK preloadAdForZoneID:zoneID ? [NSString stringWithUTF8String:zoneID] : nil];
}

bool FuseSDKIsAdAvailableForZoneID_platform(const char* zoneID)
{
    return [FuseSDK isAdAvailableForZoneID:zoneID ? [NSString stringWithUTF8String:zoneID] : nil];
}

void FuseSDKShowAdForZoneID_platform(const char* zoneID, cfuhash_table_t* options)
{
    int numOptions = cfuhash_num_entries(options);
	NSMutableDictionary* adOptions = nil;

	if( numOptions > 0 )
	{
		adOptions = [[NSMutableDictionary alloc] initWithCapacity:numOptions];
		char* key = NULL;
		void* data = NULL;
		if( cfuhash_each(options, &key,&data) )
		{
			do
			{
                NSString* keyString = key ? [NSString stringWithUTF8String:key] : @"";
                NSString* valueString = data ? [NSString stringWithUTF8String:(const char*)data] : @"";
				[adOptions setValue:valueString forKey:keyString];
			}
			while( cfuhash_next(options, &key, &data) );
		}
	}

    [FuseSDK showAdForZoneID:zoneID ? [NSString stringWithUTF8String:zoneID] : nil options:adOptions];
}

void FuseSDKDisplayNotifications_platform()
{
    [FuseSDK displayNotifications];
}

void FuseSDKDisplayMoreGames_platform()
{
    [FuseSDK displayMoreGames];
}

void FuseSDKRegisterGender_platform(int gender)
{
    [FuseSDK registerGender:gender];
}

void FuseSDKFacebookLogin_platform(const char* facebookId, const char* name, const char* accessToken)
{
    [FuseSDK facebookLogin:facebookId != NULL ? [NSString stringWithUTF8String:facebookId] : @""
                      Name:name != NULL ? [NSString stringWithUTF8String:name] : @""
           withAccessToken:accessToken != NULL ? [NSString stringWithUTF8String:accessToken] : @""];
}

void FuseSDKTwitterLogin_platform(const char* twitterId)
{
    [FuseSDK twitterLogin:twitterId ? [NSString stringWithUTF8String:twitterId] : @""];
}

void FuseSDKGameCenterLogin_platform()
{
    [FuseSDK gameCenterLogin:[GKLocalPlayer localPlayer]];
}

void FuseSDKDeviceLogin_platform(const char* alias)
{
    [FuseSDK deviceLogin:alias ? [NSString stringWithUTF8String:alias] : @""];
}

void FuseSDKFuseLogin_platform(const char* fuseId, const char* alias)
{
    [FuseSDK fuseLogin:fuseId ? [NSString stringWithUTF8String:fuseId] : @"" Alias:alias ? [NSString stringWithUTF8String:alias] : @""];
}

void FuseSDKGooglePlayLogin_platform(const char* alias, const char* token)
{
    [FuseSDK googlePlayLogin:alias ? [NSString stringWithUTF8String:alias] : @"" AccessToken:token ? [NSString stringWithUTF8String:token] : @""];
}

const char* FuseSDKGetOriginalAccountId_platform()
{
    NSString* retValue = [FuseSDK getOriginalAccountID];
    if( retValue == nil )
        return "";
    const char* string = retValue.UTF8String;
	char* copy = (char*)malloc(strlen(string) + 1);
	strcpy(copy, string);
    
    return copy;
}

const char* FuseSDKGetOriginalAccountAlias_platform()
{
    NSString* retValue = [FuseSDK getOriginalAccountAlias];
    if( retValue == nil )
        return "";
    const char* string = retValue.UTF8String;
	char* copy = (char*)malloc(strlen(string) + 1);
	strcpy(copy, string);
    
    return copy;
}

int FuseSDKGetOriginalAccountType_platform()
{
    int retValue = [FuseSDK getOriginalAccountType];
    return retValue;
}

const char* FuseSDKGetFuseID_platform()
{
    NSString* retValue = [FuseSDK getFuseID];
    if( retValue == nil )
        return "";
    
    const char* string = retValue.UTF8String;
	char* copy = (char*)malloc(strlen(string) + 1);
	strcpy(copy, string);
    
    return copy;
}

int FuseSDKgamesPlayed_platform()
{
    int retValue = [FuseSDK gamesPlayed];
    return retValue;
}

const char* FuseSDKLibraryVersion_platform()
{
    NSString* ver = [FuseSDK libraryVersion];
    if( ver == nil )
    {
        return "n/a";
    }
    const char* string = ver.UTF8String;
	char* copy = (char*)malloc(strlen(string) + 1);
	strcpy(copy, string);
    
    return copy;
}

bool FuseSDKConnected_platform()
{
    return [FuseSDK connected];
}

void FuseSDKTimeFromServer_platform()
{
    [FuseSDK utcTimeFromServer];
}

void FuseSDKEnableData_platform(bool enable)
{
    if( enable )
    {
        [FuseSDK enableData];
    }
    else
    {
        [FuseSDK disableData];
    }
}

const char* FuseSDKGetGameConfigurationValue_platform(const char* key)
{
    NSString* keyString = key ? [NSString stringWithUTF8String:key] : @"";
    NSString* valueString = [FuseSDK getGameConfigurationValue:keyString];
    if( valueString == nil )
    {
        return "";
    }
    
    const char* string = valueString.UTF8String;
	char* copy = (char*)malloc(strlen(string) + 1);
	strcpy(copy, string);
    
    return copy;
}

void FuseSDKRegisterLevel_platform(int level)
{
    [FuseSDK registerLevel:level];
}

void FuseSDKRegisterCurrency_platform(int type, int balance)
{
    [FuseSDK registerCurrency:type Balance:balance];
}

void FuseSDKRegisterParentalConsent_platform(bool enabled)
{
    [FuseSDK registerParentalConsent:enabled];
}

bool FuseSDKRegisterCustomEventInt_platform(int eventID, int eventValue)
{
    return [FuseSDK registerCustomEvent:eventID withInt:eventValue];
}

bool FuseSDKRegisterCustomEventString_platform(int eventID, const char* eventValue)
{
    NSString* eventString = eventValue ? [NSString stringWithUTF8String:eventValue] : @"";
    return [FuseSDK registerCustomEvent:eventID withString:eventString];
}

void FuseSDKSetRewardedVideoUserID_platform(const char* userID)
{
    NSString* userIDString = userID ? [NSString stringWithUTF8String:userID] : @"";
    [FuseSDK setRewardedVideoUserID:userIDString];
}

#pragma mark - Callback

@implementation FuseSDK_Delegate

#pragma mark Initialization

- (instancetype) init
{
    self = [super init];
    
	if (self)
    {
        m_appDelegate = [[[UIApplication sharedApplication] delegate] retain];

        NSLog(@"setting app delegate...");
        [[UIApplication sharedApplication] setDelegate:self];
    }

    NSLog(@"Finishing init!");
    
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
    IwTrace(FuseSDK, ("FuseSDKSessionStartReceived()"));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_SESSION_STARTED, 0, 0);
}

- (void)sessionLoginError:(NSError*)_error
{
    struct paramList
	{
		int error;
	};
	paramList params;
	params.error = _error.code;
    IwTrace(FuseSDK, ("FuseSDKLoginError(%i)", params.error));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_SESSION_LOGIN_ERROR, &params, sizeof(paramList));
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
    const char* string = NULL;
    char* copy = NULL;
    char* copy_otxid = NULL;
    
    if( _tx_id )
    {
        string = _tx_id.UTF8String;
        copy = (char*)malloc(strlen(string) + 1);
        strcpy(copy, string);
    }
    
    if( _o_tx_id )
    {
        string = _o_tx_id.UTF8String;
        copy_otxid = (char*)malloc(strlen(string) + 1);
        strcpy(copy_otxid, string);
    }
    
    params.transactionID = copy ?: "";
    params.originalTransactionID = copy_otxid ?: "";
    
    
    IwTrace(FuseSDK, ("FuseSDKPurchaseVerified(%i, %s, %s)", params.verified, params.transactionID, params.originalTransactionID));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_PURCHASE_VERIFIED, &params, sizeof(paramList));
}

#pragma mark Fuse Interstitial Ads

- (void) adAvailabilityResponse:(NSNumber*)_available Error:(NSError*)_error
{
	struct paramList
	{
		int available;
		int error;
	};
	paramList params;
	params.available = _available.intValue;
	params.error = _error.code;
    IwTrace(FuseSDK, ("FuseSDKAdAvailabilityResponse(%i, %i)", params.available, params.error));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_AD_AVAILABILITY_RESPONSE, &params, sizeof(paramList));
}

- (void)adWillClose
{
	IwTrace(FuseSDK, ("FuseSDKAdWillClose()"));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_AD_WILL_CLOSE, 0, 0);
}

-(void) adDidShow:(NSNumber *)_networkID mediaType:(NSNumber *)_mediaType
{
    struct paramList
    {
        int networkID;
        int mediaType;
    };
    paramList params;
    params.networkID = _networkID ? _networkID.intValue : 0;
    params.mediaType = _mediaType ? _mediaType.intValue : 0;
    IwTrace(FuseSDK, ("FuseSDKAdDidShow(%i, %i)", params.networkID, params.mediaType));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_AD_DID_SHOW, &params, sizeof(paramList));
}

-(void) adFailedToDisplay
{
    IwTrace(FuseSDK, ("FuseSDKAdFailedToDisplay()"));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_AD_FAILED_TO_DISPLAY, 0, 0);
}

-(void) rewardedAdCompleteWithObject:(FuseRewardedObject*) reward
{
    struct paramList
    {
        const char* preMessage;
        const char* rewardMessage;
        const char* rewardItem;
        int rewardAmount;
        int itemID; // the rewarded item ID as specified on the Fuse Dashboard
    };

    // copy the strings
    const char* string = NULL;
    char* preroll_copy = NULL;
    char* rewardmsg_copy = NULL;
    char* rewarditem_copy = NULL;
    
    if( reward.preRollMessage )
    {
        string = reward.preRollMessage.UTF8String;
        preroll_copy = (char*)malloc(strlen(string) + 1);
        strcpy(preroll_copy, string);
    }

    if( reward.rewardMessage )
    {
        string =  reward.rewardMessage.UTF8String;
        rewardmsg_copy = (char*)malloc(strlen(string) + 1);
        strcpy(rewardmsg_copy, string);
    }

    if( reward.rewardItem )
    {
        string = reward.rewardItem ?  reward.rewardItem.UTF8String : "";
        rewarditem_copy = (char*)malloc(strlen(string) + 1);
        strcpy(rewarditem_copy, string);
    }
    
    // set the int values
    int rewardAmountInt = 0;
    int itemIDInt = 0;
    if( reward )
    {
        if( reward.rewardAmount )
        {
            rewardAmountInt = reward.rewardAmount.intValue;
        }
        
        itemIDInt = reward.itemID;
    }    

    // set the params
    paramList params;
    params.rewardAmount = rewardAmountInt;
    params.preMessage = preroll_copy ?: "";
    params.rewardMessage = rewardmsg_copy ?: "";
    params.rewardItem = rewarditem_copy ?: "";
    params.itemID = itemIDInt;
    
    
    IwTrace(FuseSDK, ("FuseSDKRewardedAdComplete(%s, %s, %s, %i, %i)", params.preMessage, params.rewardMessage, params.rewardItem, params.rewardAmount, params.itemID));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_REWARDED_AD_COMPLETED, &params, sizeof(paramList));
}


-(void) IAPOfferAcceptedWithObject:(FuseIAPOfferObject*) _offer
{
    struct paramList
    {
        float productPrice;
        int itemAmount;
        const char* itemName;
        const char* productID;
        int startTime;
        int endTime;
    };

    IwTrace(FuseSDK, ("IAPOfferAcceptedWithObject was called!"));

    // copy the strings
    const char* string = NULL;
    char* productid_copy = NULL;
    char* itemname_copy = NULL;
    
    if( _offer && _offer.productID )
    {
        string = _offer.productID.UTF8String;
        productid_copy = (char*)malloc(strlen(string) + 1);
        strcpy(productid_copy, string);
    }
    
    if( _offer && _offer.itemName )
    {
        string = _offer.itemName.UTF8String;
        itemname_copy =  (char*)malloc(strlen(string) + 1);
        strcpy(itemname_copy, string);
    }
    
    // set the number values
    float productPriceFloat = 0;
    int itemAmountInt = 0;
    int startTimeInt = 0;
    int endTimeInt = 0;
    if( _offer )
    {
        productPriceFloat = _offer.productPrice ? _offer.productPrice.floatValue : 0;
        itemAmountInt = _offer.itemAmount ? _offer.itemAmount.intValue : 0;
        startTimeInt = _offer.startTime ? _offer.startTime.intValue : 0;
        endTimeInt = _offer.endTime ? _offer.endTime.intValue : 0;
    }

    // set the params
    paramList params;
    params.productPrice = productPriceFloat;
    params.itemAmount = itemAmountInt;
    params.itemName = itemname_copy ?: "";
    params.productID = productid_copy ?: "";
    params.startTime = startTimeInt;
    params.endTime = endTimeInt;

    IwTrace(FuseSDK, ("FuseSDKIAPOfferAccepted(%f, %i, %s, %s, %i, %i)", params.productPrice, params.itemAmount, params.itemName, params.productID, params.startTime, params.endTime));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_IAPOFFER_ACCEPTED, &params, sizeof(paramList));
}

-(void) virtualGoodsOfferAcceptedWithObject:(FuseVirtualGoodsOfferObject*) _offer
{
    struct paramList
    {
        const char* purchaseCurrency;
        float purchasePrice;
        const char* itemName;
        int itemAmount;
        int startTime;
        int endTime;
        int currencyID;
        int virtualGoodsID;
    };

    // copy the strings
    const char* string = NULL;
    char* currency_copy = NULL;
    char* itemname_copy = NULL;

    if( _offer.purchaseCurrency )
    {
        string = _offer.purchaseCurrency.UTF8String;
        currency_copy = (char*)malloc(strlen(string) + 1);
        strcpy(currency_copy, string);
    }
    
    if( _offer.itemName )
    {
        string = _offer.itemName.UTF8String;
        itemname_copy =  (char*)malloc(strlen(string) + 1);
        strcpy(itemname_copy, string);
    }
    
    // set the number values
    float purchasePriceFloat = 0;
    int itemAmountInt = 0;
    int startTimeInt = 0;
    int endTimeInt = 0;
    int currencyIDInt = 0;
    int virtualGoodIDsInt = 0;
    if( _offer )
    {
        purchasePriceFloat = _offer.purchasePrice ? _offer.purchasePrice.floatValue : 0;
        itemAmountInt = _offer.itemAmount ? _offer.itemAmount.intValue : 0;
        startTimeInt = _offer.startTime ? _offer.startTime.intValue : 0;
        endTimeInt = _offer.endTime ? _offer.endTime.intValue : 0;
        currencyIDInt = _offer.currencyID ? _offer.currencyID.intValue : 0;
        virtualGoodIDsInt = _offer.virtualGoodID ? _offer.virtualGoodID.intValue : 0;
    }
    

    // set the params
    paramList params;
    params.purchaseCurrency = currency_copy ?: "";
    params.purchasePrice = purchasePriceFloat;
    params.itemName = itemname_copy ?: "";
    params.itemAmount = itemAmountInt;
    params.startTime = startTimeInt;
    params.endTime = endTimeInt;
    params.currencyID = currencyIDInt;
    params.virtualGoodsID = virtualGoodIDsInt;
    IwTrace(FuseSDK, ("FuseSDKVirtualGoodsOfferAccepted(%s, %f, %s, %i, %i, %i, %i, %i)", params.purchaseCurrency, params.purchasePrice, params.itemName, params.itemAmount, params.startTime, params.endTime, params.currencyID, params.virtualGoodsID));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_VIRTUALGOODSOFFER_ACCEPTED, &params, sizeof(paramList));
}

#pragma mark Notifications

-(void) notificationAction:(NSString*)_action
{
	struct paramList
	{
		const char* action;
	};
	paramList params;
    char* copy = NULL;
    if( _action )
    {
        const char* string = _action.UTF8String;
        copy = (char*)malloc(strlen(string) + 1);
        strcpy(copy, string);
    }
	params.action = copy ?: "";
    IwTrace(FuseSDK, ("FuseSDKNotificationAction(%s)", params.action));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_NOTIFICATION_ACTION, &params, sizeof(paramList));
}

#pragma mark More Games

- (void)overlayWillClose
{
	IwTrace(FuseSDK, ("FuseSDKOverLayWillClose()"));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_OVERLAY_WILL_CLOSE, 0, 0);
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
    char* copy = NULL;
	params.accountType = _type.intValue;
    
    if( _account_id )
    {
        const char* string = _account_id.UTF8String;
        char* copy = (char*)malloc(strlen(string) + 1);
        strcpy(copy, string);
    }
    
	params.accountID = copy ?: "";
    IwTrace(FuseSDK, ("FuseSDKAccountLoginComplete(%i, %s)", params.accountType, params.accountID));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_ACCOUNT_LOGIN_COMPLETE, &params, sizeof(paramList));    
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
    IwTrace(FuseSDK, ("FuseSDKTimeUpdated(%i)", params.timestamp));
    s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_TIME_UPDATED, &params, sizeof(paramList));
}

#pragma mark Game Configuration Data

- (void)gameConfigurationReceived
{
    IwTrace(FuseSDK, ("FuseGameConfigurationReceived()"));
    
    struct paramList
	{
		cfuhash_table_t* gameConfig;
	};
	paramList params;	
    
    NSMutableDictionary* gc = [FuseSDK getGameConfiguration];
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
            if( key == nil )
            {
                continue;
            }
            NSString *value = [test objectForKey:key];
            
            //NSLog(@"Key: %@, Value: %@", key, value);
            
            const char* string = value ? value.UTF8String : "";
            char* copy = (char*)malloc(strlen(string) + 1);
            strcpy(copy, string);

            cfuhash_put(gameConfig, key.UTF8String, (void*)copy);
        }
    }
    FuseSafeRelease(test);
    params.gameConfig = gameConfig;
	s3eEdkCallbacksEnqueue(S3E_EXT_FUSESDK_HASH, FUSESDK_GAME_CONFIGURATION_RECEIVED, &params, sizeof(paramList));
}

#pragma mark - Application Delegate Functions
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
	[FuseSDK applicationdidRegisterForRemoteNotificationsWithDeviceToken:deviceToken];
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

- (void) application:(UIApplication *)application didChangeStatusBarFrame:(CGRect)oldStatusBarFrame
{
    if( [m_appDelegate respondsToSelector:@selector(application:didChangeStatusBarFrame:)] )
    {
        [m_appDelegate application:application didChangeStatusBarFrame:oldStatusBarFrame];
    }
}

- (void) application:(UIApplication *)application didChangeStatusBarOrientation:(UIInterfaceOrientation)oldStatusBarOrientation
{
    if( [m_appDelegate respondsToSelector:@selector(application:didChangeStatusBarOrientation:)] )
    {
        [m_appDelegate application:application didChangeStatusBarOrientation:oldStatusBarOrientation];
    }
}

- (void) application:(UIApplication *)application didDecodeRestorableStateWithCoder:(NSCoder *)coder
{
    if( [m_appDelegate respondsToSelector:@selector(application:didDecodeRestorableStateWithCoder:)] )
    {
        [m_appDelegate application:application didDecodeRestorableStateWithCoder:coder];
    }
}

- (void) application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification *)notification
{
    if( [m_appDelegate respondsToSelector:@selector(application:didReceiveLocalNotification:)] )
    {
        [m_appDelegate application:application didReceiveLocalNotification:notification];
    }
}

- (void) application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
{
    if( [m_appDelegate respondsToSelector:@selector(application:didReceiveRemoteNotification:)] )
    {
        [m_appDelegate application:application didReceiveRemoteNotification:userInfo];
    }
}

- (BOOL) application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    if( [m_appDelegate respondsToSelector:@selector(application:openURL:sourceApplication:annotation:)] )
    {
        return [m_appDelegate application:application openURL:url sourceApplication:sourceApplication annotation:annotation];
    }
    return YES;
}

//- (BOOL) application:(UIApplication *)application shouldRestoreApplicationState:(NSCoder *)coder
//{
//    if( [m_appDelegate respondsToSelector:@selector(application:shouldRestoreApplicationState:)] )
//    {
//        return [m_appDelegate application:application shouldRestoreApplicationState:coder];
//    }
//    return NO;
//}

//- (BOOL) application:(UIApplication *)application shouldSaveApplicationState:(NSCoder *)coder
//{
//    if( [m_appDelegate respondsToSelector:@selector(application:shouldSaveApplicationState:)] )
//    {
//        return [m_appDelegate application:application shouldSaveApplicationState:coder];
//    }
//    return NO;
//}

//- (NSUInteger) application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window
//{
//    if( [m_appDelegate respondsToSelector:@selector(application:supportedInterfaceOrientationsForWindow:)] )
//    {
//        return [m_appDelegate application:application supportedInterfaceOrientationsForWindow:window];
//    }
//    return NO;
//}

//- (UIViewController*) application:(UIApplication *)application viewControllerWithRestorationIdentifierPath:(NSArray *)identifierComponents coder:(NSCoder *)coder
//{
//   if( [m_appDelegate respondsToSelector:@selector(application:viewControllerWithRestorationIdentifierPath:coder:)] )
//    {
//        return [m_appDelegate application:application viewControllerWithRestorationIdentifierPath:identifierComponents coder:coder];
//    }
//    return nil;
//}

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
    if( [m_appDelegate respondsToSelector:@selector(application:willEncodeRestorableStateWithCoder:)] )
    {
        [m_appDelegate application:application willEncodeRestorableStateWithCoder:coder];
    }
}

- (BOOL) application:(UIApplication *)application willFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    return [m_appDelegate application:application willFinishLaunchingWithOptions:launchOptions];
}

- (void) applicationDidBecomeActive:(UIApplication *)application
{
    if( [m_appDelegate respondsToSelector:@selector(applicationDidBecomeActive:)] )
    {
        [m_appDelegate applicationDidBecomeActive:application];
    }
}

- (void) applicationDidEnterBackground:(UIApplication *)application
{
    if( [m_appDelegate respondsToSelector:@selector(applicationDidEnterBackground:)] )
    {
        [m_appDelegate applicationDidEnterBackground:application];
    }
}

- (void) applicationDidFinishLaunching:(UIApplication *)application
{
    if( [m_appDelegate respondsToSelector:@selector(applicationDidFinishLaunching:)] )
    {
        [m_appDelegate applicationDidFinishLaunching:application];
    }
}

- (void) applicationDidReceiveMemoryWarning:(UIApplication *)application
{
    if( [m_appDelegate respondsToSelector:@selector(applicationDidReceiveMemoryWarning:)] )
    {
        [m_appDelegate applicationDidReceiveMemoryWarning:application];
    }
}

- (void) applicationProtectedDataDidBecomeAvailable:(UIApplication *)application
{
    if( [m_appDelegate respondsToSelector:@selector(applicationProtectedDataDidBecomeAvailable:)] )
    {
        [m_appDelegate applicationProtectedDataDidBecomeAvailable:application];
    }
}

- (void) applicationProtectedDataWillBecomeUnavailable:(UIApplication *)application
{
    if( [m_appDelegate respondsToSelector:@selector(applicationProtectedDataWillBecomeUnavailable:)] )
    {
        [m_appDelegate applicationProtectedDataWillBecomeUnavailable:application];
    }
}

- (void) applicationSignificantTimeChange:(UIApplication *)application
{
    if( [m_appDelegate respondsToSelector:@selector(applicationSignificantTimeChange:)] )
    {
        [m_appDelegate applicationSignificantTimeChange:application];
    }
}

- (void) applicationWillEnterForeground:(UIApplication *)application
{
    if( [m_appDelegate respondsToSelector:@selector(applicationWillEnterForeground:)] )
    {
        [m_appDelegate applicationWillEnterForeground:application];
    }
}

- (void) applicationWillResignActive:(UIApplication *)application
{
    if( [m_appDelegate respondsToSelector:@selector(applicationWillResignActive:)] )
    {
        [m_appDelegate applicationWillResignActive:application];
    }
}

- (void) applicationWillTerminate:(UIApplication *)application
{
    if( [m_appDelegate respondsToSelector:@selector(applicationWillTerminate:)] )
    {
        [m_appDelegate applicationWillTerminate:application];
    }
}


@end
