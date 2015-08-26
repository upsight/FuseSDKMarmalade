/*
Generic implementation of the FuseSDK extension.
This file should perform any platform-indepedentent functionality
(e.g. error checking) before calling platform-dependent implementations.
*/

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#include "FuseSDK_internal.h"
s3eResult FuseSDKInit()
{
    //Add any generic initialisation code here
    s3eResult rtn = FuseSDKInit_platform();
    return rtn;
}

void FuseSDKTerminate()
{
    //Add any generic termination code here
    FuseSDKTerminate_platform();
}

void FuseSDKStartSession(const char* key, cfuhash_table_t* options)
{
	FuseSDKStartSession_platform(key, options);
}

void FuseSDKPauseSession()
{
	FuseSDKPauseSession_platform();
}

void FuseSDKResumeSession()
{
	FuseSDKResumeSession_platform();
}

void FuseSDKTerminateSession()
{
	FuseSDKTerminateSession_platform();
}

void FuseSDKRegisterForPushNotifications(const char* projectID)
{
	FuseSDKRegisterForPushNotifications_platform(projectID);
}

void FuseSDKRegisterInAppPurchaseAndroid(FusePurchaseStateAndroid purchaseState, const char* purchaseToken, const char* productId, const char* orderId, long purchaseTime, const char* developerPayload, const double* price, const char* currency)
{
	FuseSDKRegisterInAppPurchaseAndroid_platform(purchaseState, purchaseToken, productId, orderId, purchaseTime, developerPayload, price, currency);
}

void FuseSDKRegisterInAppPurchaseiOS(FusePurchaseStateiOS purchaseState, const char* receiptData, int recieptDataLength, double* price, const char* currency, const char* productID, const char* transactionID)
{
    FuseSDKRegisterInAppPurchaseiOS_platform(purchaseState, receiptData, recieptDataLength, price, currency, productID, transactionID);
}

void FuseSDKRegisterVirtualGoodsPurchase(int virtualGoodsID, int purchaseAmount, int currencyID)
{
    FuseSDKRegisterVirtualGoodsPurchase_platform(virtualGoodsID, purchaseAmount, currencyID);
}

void FuseSDKPreloadAdForZoneID(const char* zoneID)
{
    FuseSDKPreloadAdForZoneID_platform(zoneID);
}

bool FuseSDKIsAdAvailableForZoneID(const char* zoneID)
{
    return FuseSDKIsAdAvailableForZoneID_platform(zoneID);
}

void FuseSDKShowAdForZoneID(const char* zoneID, cfuhash_table_t* options)
{
    FuseSDKShowAdForZoneID_platform(zoneID, options);
}

void FuseSDKDisplayNotifications()
{
	FuseSDKDisplayNotifications_platform();
}

void FuseSDKDisplayMoreGames()
{
	FuseSDKDisplayMoreGames_platform();
}

void FuseSDKRegisterGender(int gender)
{
	FuseSDKRegisterGender_platform(gender);
}

void FuseSDKFacebookLogin(const char* facebookId, const char* name, const char* accessToken)
{
	FuseSDKFacebookLogin_platform(facebookId, name, accessToken);
}

void FuseSDKTwitterLogin(const char* twitterId)
{
	FuseSDKTwitterLogin_platform(twitterId);
}

void FuseSDKGameCenterLogin()
{
	FuseSDKGameCenterLogin_platform();
}

void FuseSDKDeviceLogin(const char* alias)
{
	FuseSDKDeviceLogin_platform(alias);
}

void FuseSDKFuseLogin(const char* fuseId, const char* alias)
{
	FuseSDKFuseLogin_platform(fuseId, alias);
}

void FuseSDKGooglePlayLogin(const char* alias, const char* token)
{
	FuseSDKGooglePlayLogin_platform(alias, token);
}

const char* FuseSDKGetOriginalAccountId()
{
	return FuseSDKGetOriginalAccountId_platform();
}

const char* FuseSDKGetOriginalAccountAlias()
{
	return FuseSDKGetOriginalAccountAlias_platform();
}

int FuseSDKGetOriginalAccountType()
{
	return FuseSDKGetOriginalAccountType_platform();
}

const char* FuseSDKGetFuseID()
{
	return FuseSDKGetFuseID_platform();
}

int FuseSDKgamesPlayed()
{
	return FuseSDKgamesPlayed_platform();
}

const char* FuseSDKLibraryVersion()
{
	return FuseSDKLibraryVersion_platform();
}

bool FuseSDKConnected()
{
	return FuseSDKConnected_platform();
}

void FuseSDKTimeFromServer()
{
	FuseSDKTimeFromServer_platform();
}

void FuseSDKEnableData(bool enable)
{
	FuseSDKEnableData_platform(enable);
}

const char* FuseSDKGetGameConfigurationValue(const char* key)
{
	return FuseSDKGetGameConfigurationValue_platform(key);
}

void FuseSDKRegisterLevel(int level)
{
	FuseSDKRegisterLevel_platform(level);
}

void FuseSDKRegisterCurrency(int type, int balance)
{
	FuseSDKRegisterCurrency_platform(type, balance);
}

void FuseSDKRegisterParentalConsent(bool enabled)
{
    FuseSDKRegisterParentalConsent_platform(enabled);
}

bool FuseSDKRegisterCustomEventInt(int eventID, int eventValue)
{
    return FuseSDKRegisterCustomEventInt_platform(eventID, eventValue);
}

bool FuseSDKRegisterCustomEventString(int eventID, const char* eventValue)
{
    return FuseSDKRegisterCustomEventString_platform(eventID, eventValue);
}

void FuseSDKSetRewardedVideoUserID(const char* userID)
{
    FuseSDKSetRewardedVideoUserID_platform(userID);
}

