/*
Generic implementation of the FuseAPI extension.
This file should perform any platform-indepedentent functionality
(e.g. error checking) before calling platform-dependent implementations.
*/

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#include "FuseAPI_internal.h"
s3eResult FuseAPIInit()
{
    //Add any generic initialisation code here
    s3eResult rtn = FuseAPIInit_platform();
    return rtn;
}

void FuseAPITerminate()
{
    //Add any generic termination code here
    FuseAPITerminate_platform();
}

void FuseAPIStartSession(const char* key)
{
	FuseAPIStartSession_platform(key);
}

void FuseAPIPauseSession()
{
	FuseAPIPauseSession_platform();
}

void FuseAPIResumeSession()
{
	FuseAPIResumeSession_platform();
}

void FuseAPITerminateSession()
{
	FuseAPITerminateSession_platform();
}

void FuseAPIRegisterForPushNotifications(const char* projectID)
{
	FuseAPIRegisterForPushNotifications_platform(projectID);
}

void FuseAPIRegisterEvent(const char* message)
{
	FuseAPIRegisterEvent_platform(message);
}

int FuseAPIRegisterEventWithParam(const char* name, const char* param_name, const char* param_value, const char* variable_name, const double* variable_value)
{
	return FuseAPIRegisterEventWithParam_platform(name, param_name, param_value, variable_name, variable_value);
}

int FuseAPIRegisterEventWithEventData(const char* name, const char* paramName, const char* paramValue, cfuhash_table_t* eventData)
{
	return FuseAPIRegisterEventWithEventData_platform(name, paramName, paramValue, eventData);
}

void FuseAPIRegisterEventWithDictionary(const char* message, cfuhash_table_t* eventData)
{
	FuseAPIRegisterEventWithDictionary_platform(message, eventData);
}

void FuseAPIRegisterInAppPurchaseAndroid(FusePurchaseState purchaseState, const char* purchaseToken, const char* productId, const char* orderId, long purchaseTime, const char* developerPayload, const double* price, const char* currency)
{
	FuseAPIRegisterInAppPurchaseAndroid_platform(purchaseState, purchaseToken, productId, orderId, purchaseTime, developerPayload, price, currency);
}

void FuseAPIRegisterInAppPurchaseiOS(FusePurchaseState purchaseState, const char* receiptData, int recieptDataLength, double* price, const char* currency, const char* productID)
{
    FuseAPIRegisterInAppPurchaseiOS_platform(purchaseState, receiptData, recieptDataLength, price, currency, productID);
}

void FuseAPICheckAdAvailable()
{
	FuseAPICheckAdAvailable_platform();
}

void FuseAPIShowAd()
{
	FuseAPIShowAd_platform();
}

void FuseAPIDisplayNotifications()
{
	FuseAPIDisplayNotifications_platform();
}

void FuseAPIDisplayMoreGames()
{
	FuseAPIDisplayMoreGames_platform();
}

void FuseAPIRegisterGender(int gender)
{
	FuseAPIRegisterGender_platform(gender);
}

void FuseAPIFacebookLogin(const char* facebookId, const char* name, const char* accessToken)
{
	FuseAPIFacebookLogin_platform(facebookId, name, accessToken);
}

void FuseAPITwitterLogin(const char* twitterId)
{
	FuseAPITwitterLogin_platform(twitterId);
}

void FuseAPIGameCenterLogin()
{
	FuseAPIGameCenterLogin_platform();
}

void FuseAPIDeviceLogin(const char* alias)
{
	FuseAPIDeviceLogin_platform(alias);
}

void FuseAPIFuseLogin(const char* fuseId, const char* alias)
{
	FuseAPIFuseLogin_platform(fuseId, alias);
}

void FuseAPIGooglePlayLogin(const char* alias, const char* token)
{
	FuseAPIGooglePlayLogin_platform(alias, token);
}

const char* FuseAPIGetOriginalAccountId()
{
	return FuseAPIGetOriginalAccountId_platform();
}

const char* FuseAPIGetOriginalAccountAlias()
{
	return FuseAPIGetOriginalAccountAlias_platform();
}

int FuseAPIGetOriginalAccountType()
{
	return FuseAPIGetOriginalAccountType_platform();
}

const char* FuseAPIGetFuseID()
{
	return FuseAPIGetFuseID_platform();
}

int FuseAPIgamesPlayed()
{
	return FuseAPIgamesPlayed_platform();
}

const char* FuseAPILibraryVersion()
{
	return FuseAPILibraryVersion_platform();
}

bool FuseAPIConnected()
{
	return FuseAPIConnected_platform();
}

void FuseAPITimeFromServer()
{
	FuseAPITimeFromServer_platform();
}

void FuseAPIEnableData(bool enable)
{
	FuseAPIEnableData_platform(enable);
}

int FuseAPISetGameData(const char* key, const char* fuseId, cfuhash_table_t* gameData)
{
	return FuseAPISetGameData_platform(key, fuseId, gameData);
}

int FuseAPIGetGameData(const char* key, const char* fuseId, const char** gameDataKeys, int numKeys)
{
	return FuseAPIGetGameData_platform(key, fuseId, gameDataKeys, numKeys);
}

void FuseAPIMigrateFriends(const char* fuseId)
{
	FuseAPIMigrateFriends_platform(fuseId);
}

void FuseAPIUpdateFriendsListFromServer()
{
	FuseAPIUpdateFriendsListFromServer_platform();
}

FusePlayer* FuseAPIGetFriendsList(int* numPlayers)
{
	return FuseAPIGetFriendsList_platform(numPlayers);
}

void FuseAPIGetMailListFromServer()
{
	FuseAPIGetMailListFromServer_platform();
}

void FuseAPIGetMailListFriendFromServer(const char* fuseId)
{
	FuseAPIGetMailListFriendFromServer_platform(fuseId);
}

void FuseAPISetMailAsReceived(int messageId)
{
	FuseAPISetMailAsReceived_platform(messageId);
}

int FuseAPISendMailWithGift(const char* fuseId, const char* message, int giftId, int giftAmount)
{
	return FuseAPISendMailWithGift_platform(fuseId, message, giftId, giftAmount);
}

int FuseAPISendMail(const char* fuseId, const char* message)
{
	return FuseAPISendMail_platform(fuseId, message);
}

FuseMail* FuseAPIGetMailList(int* numEntries)
{
	return FuseAPIGetMailList_platform(numEntries);
}

const char* FuseAPIGetGameConfigurationValue(const char* key)
{
	return FuseAPIGetGameConfigurationValue_platform(key);
}

void FuseAPIRegisterLevel(int level)
{
	FuseAPIRegisterLevel_platform(level);
}

void FuseAPIRegisterCurrency(int type, int balance)
{
	FuseAPIRegisterCurrency_platform(type, balance);
}

void FuseAPIRegisterFlurryView()
{
	FuseAPIRegisterFlurryView_platform();
}

void FuseAPIRegisterFlurryClick()
{
	FuseAPIRegisterFlurryClick_platform();
}

void FuseAPIRegisterTapjoyReward(int amount)
{
	FuseAPIRegisterTapjoyReward_platform(amount);
}
