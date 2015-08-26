/*
 * Internal header for the FuseSDK extension.
 *
 * This file should be used for any common function definitions etc that need to
 * be shared between the platform-dependent and platform-indepdendent parts of
 * this extension.
 */

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#ifndef FUSESDK_INTERNAL_H
#define FUSESDK_INTERNAL_H

#include "s3eTypes.h"
#include "FuseSDK.h"
#include "FuseSDK_autodefs.h"


/**
 * Initialise the extension.  This is called once then the extension is first
 * accessed by s3eregister.  If this function returns S3E_RESULT_ERROR the
 * extension will be reported as not-existing on the device.
 */
s3eResult FuseSDKInit();

/**
 * Platform-specific initialisation, implemented on each platform
 */
s3eResult FuseSDKInit_platform();

/**
 * Terminate the extension.  This is called once on shutdown, but only if the
 * extension was loader and Init() was successful.
 */
void FuseSDKTerminate();

/**
 * Platform-specific termination, implemented on each platform
 */
void FuseSDKTerminate_platform();
void FuseSDKStartSession_platform(const char* key, cfuhash_table_t* options);

void FuseSDKPauseSession_platform();

void FuseSDKResumeSession_platform();

void FuseSDKTerminateSession_platform();

void FuseSDKRegisterForPushNotifications_platform(const char* projectID);

void FuseSDKRegisterInAppPurchaseAndroid_platform(FusePurchaseStateAndroid purchaseState, const char* purchaseToken, const char* productId, const char* orderId, long purchaseTime, const char* developerPayload, const double* price, const char* currency);

void FuseSDKRegisterInAppPurchaseiOS_platform(FusePurchaseStateiOS purchaseState, const char* receiptData, int recieptDataLength, double* price, const char* currency, const char* productID, const char* transactionID);

void FuseSDKRegisterVirtualGoodsPurchase_platform(int virtualGoodsID, int purchaseAmount, int currencyID);

void FuseSDKPreloadAdForZoneID_platform(const char* zoneID);

bool FuseSDKIsAdAvailableForZoneID_platform(const char* zoneID);

void FuseSDKShowAdForZoneID_platform(const char* zoneID, cfuhash_table_t* options);

void FuseSDKDisplayNotifications_platform();

void FuseSDKDisplayMoreGames_platform();

void FuseSDKRegisterGender_platform(int gender);

void FuseSDKFacebookLogin_platform(const char* facebookId, const char* name, const char* accessToken);

void FuseSDKTwitterLogin_platform(const char* twitterId);

void FuseSDKGameCenterLogin_platform();

void FuseSDKDeviceLogin_platform(const char* twitterId);

void FuseSDKFuseLogin_platform(const char* fuseId, const char* alias);

void FuseSDKGooglePlayLogin_platform(const char* alias, const char* token);

const char* FuseSDKGetOriginalAccountId_platform();

const char* FuseSDKGetOriginalAccountAlias_platform();

int FuseSDKGetOriginalAccountType_platform();

const char* FuseSDKGetFuseID_platform();

int FuseSDKgamesPlayed_platform();

const char* FuseSDKLibraryVersion_platform();

bool FuseSDKConnected_platform();

void FuseSDKTimeFromServer_platform();

void FuseSDKEnableData_platform(bool enable);

const char* FuseSDKGetGameConfigurationValue_platform(const char* key);

void FuseSDKRegisterLevel_platform(int level);

void FuseSDKRegisterCurrency_platform(int type, int balance);

void FuseSDKRegisterParentalConsent_platform(bool enabled);

bool FuseSDKRegisterCustomEventInt_platform(int eventID, int eventValue);

bool FuseSDKRegisterCustomEventString_platform(int eventID, const char* eventValue);

void FuseSDKSetRewardedVideoUserID_platform(const char* userID);

#endif /* !FUSESDK_INTERNAL_H */