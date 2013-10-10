/*
 * Internal header for the FuseAPI extension.
 *
 * This file should be used for any common function definitions etc that need to
 * be shared between the platform-dependent and platform-indepdendent parts of
 * this extension.
 */

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#ifndef FUSEAPI_INTERNAL_H
#define FUSEAPI_INTERNAL_H

#include "s3eTypes.h"
#include "FuseAPI.h"
#include "FuseAPI_autodefs.h"


/**
 * Initialise the extension.  This is called once then the extension is first
 * accessed by s3eregister.  If this function returns S3E_RESULT_ERROR the
 * extension will be reported as not-existing on the device.
 */
s3eResult FuseAPIInit();

/**
 * Platform-specific initialisation, implemented on each platform
 */
s3eResult FuseAPIInit_platform();

/**
 * Terminate the extension.  This is called once on shutdown, but only if the
 * extension was loader and Init() was successful.
 */
void FuseAPITerminate();

/**
 * Platform-specific termination, implemented on each platform
 */
void FuseAPITerminate_platform();
void FuseAPIStartSession_platform(const char* key); 

void FuseAPIPauseSession_platform();

void FuseAPIResumeSession_platform();

void FuseAPITerminateSession_platform();

void FuseAPIRegisterForPushNotifications_platform(const char* projectID);

void FuseAPIRegisterEvent_platform(const char* message);

int FuseAPIRegisterEventWithParam_platform(const char* name, const char* param_name, const char* param_value, const char* variable_name, const double* variable_value);

int FuseAPIRegisterEventWithEventData_platform(const char* name, const char* paramName, const char* paramValue, cfuhash_table_t* eventData);

void FuseAPIRegisterEventWithDictionary_platform(const char* message, cfuhash_table_t* eventData);

void FuseAPIRegisterInAppPurchaseAndroid_platform(FusePurchaseState purchaseState, const char* purchaseToken, const char* productId, const char* orderId, long purchaseTime, const char* developerPayload, const double* price, const char* currency);

void FuseAPIRegisterInAppPurchaseiOS_platform(FusePurchaseState purchaseState, const char* receiptData, int recieptDataLength, double* price, const char* currency, const char* productID);

void FuseAPICheckAdAvailable_platform();

void FuseAPIShowAd_platform();

void FuseAPIDisplayNotifications_platform();

void FuseAPIDisplayMoreGames_platform();

void FuseAPIRegisterGender_platform(int gender);

void FuseAPIFacebookLogin_platform(const char* facebookId, const char* name, const char* accessToken);

void FuseAPITwitterLogin_platform(const char* twitterId);

void FuseAPIGameCenterLogin_platform();

void FuseAPIDeviceLogin_platform(const char* twitterId);

void FuseAPIFuseLogin_platform(const char* fuseId, const char* alias);

void FuseAPIGooglePlayLogin_platform(const char* alias, const char* token);

const char* FuseAPIGetOriginalAccountId_platform();

const char* FuseAPIGetOriginalAccountAlias_platform();

int FuseAPIGetOriginalAccountType_platform();

const char* FuseAPIGetFuseID_platform();

int FuseAPIgamesPlayed_platform();

const char* FuseAPILibraryVersion_platform();

bool FuseAPIConnected_platform();

void FuseAPITimeFromServer_platform();

void FuseAPIEnableData_platform(bool enable);

int FuseAPISetGameData_platform(const char* key, const char* fuseId, cfuhash_table_t* gameData);

int FuseAPIGetGameData_platform(const char* key, const char* fuseId, const char** gameDataKeys, int numKeys);

void FuseAPIUpdateFriendsListFromServer_platform();

FusePlayer* FuseAPIGetFriendsList_platform(int* numPlayers);

void FuseAPIGetMailListFromServer_platform();

void FuseAPIGetMailListFriendFromServer_platform(const char* fuseId);

void FuseAPISetMailAsReceived_platform(int messageId);

int FuseAPISendMailWithGift_platform(const char* fuseId, const char* message, int giftId, int giftAmount);

int FuseAPISendMail_platform(const char* fuseId, const char* message);

FuseMail* FuseAPIGetMailList_platform(int* numEntries);

const char* FuseAPIGetGameConfigurationValue_platform(const char* key);

void FuseAPIRegisterLevel_platform(int level);

void FuseAPIRegisterCurrency_platform(int type, int balance);

void FuseAPIRegisterFlurryView_platform();

void FuseAPIRegisterFlurryClick_platform();

void FuseAPIRegisterTapjoyReward_platform(int amount);


#endif /* !FUSEAPI_INTERNAL_H */