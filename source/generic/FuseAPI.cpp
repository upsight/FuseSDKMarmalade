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
    return FuseAPIInit_platform();
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

void FuseAPIRegisterInAppPurchase(PurchaseState purchaseState, const char* purchaseToken, const char* productId, const char* orderId, long purchaseTime, const char* developerPayload, const double* price, const char* currency)
{
	FuseAPIRegisterInAppPurchase_platform(purchaseState, purchaseToken, productId, orderId, purchaseTime, developerPayload, price, currency);
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

void FuseAPIUpdateFriendsListFromServer()
{
	FuseAPIUpdateFriendsListFromServer_platform();
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
