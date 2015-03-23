/*
 * This file provided to demonstrate how to use the FuseSDK Extension
 *
 */

#include "FuseSDK.h"

#include "s3e.h"
#include "s3eDebug.h"

#include <string>
using namespace std;

string sessionStatus = "`xffffffStarting FuseSDK session...";
string adStatus = "`xffffffAd pending...";
string loginStatus = "`xffffffTesting account login...";

// Fuse Callbacks
int32 GotSessionReceived(void* systemData, void* userData)
{
	// Display success message (see main loop)
	sessionStatus = "`xffffffFuseSDK session started!";

	// Check for ad availibility
	// This will cause an FUSESDK_AD_AVAILABILITY_RESPONSE callback response 
	FuseSDKPreloadAdForZoneID(NULL);

	// Register for Google Cloud Messaging (GCM) push notifications
	FuseSDKRegisterForPushNotifications("538100748941");

	// test Fuse login - see FuseSDK/docs/FuseSDK.txt for more information
	// this will return a FUSESDK_ACCOUNT_LOGIN_COMPLETE callback
	FuseSDKFuseLogin("233963519", "tony stark");

	return 1;
}

int32 SessionLoginError(void* systemData, void* userData)
{
	// see FuseSDK/docs/Callbacks.txt for more information about FuseSDK callbacks and their parameters
	struct paramList
	{
		int error;
	};
	const paramList* params = (paramList*)systemData;
	
	// display error message (see main loop)
	sessionStatus = "`xffffffFuseSDK session start error";

	return 1;
}

int32 AdAvailableResponse(void* systemData, void* userData)
{
	// see FuseSDK/docs/Callbacks.txt for more information about FuseSDK callbacks and their parameters
	struct paramList
	{
		int available;
		int error;
	};
	const paramList* params = (paramList*)systemData;

	// display status update for ad (see main loop)
	if( params->available > 0 )
	{
		adStatus = "`xffffffDisplaying Ad";		
	}
	else if( params->error > 0 )
	{
		adStatus = "`xffffffError in fetching ad";
	}
	else
	{
		adStatus = "`xffffffNo ad to display";
        FuseSDKPreloadAdForZoneID(NULL);
        return 1;
	}

	// Display the ad
	// Note: You can call FuseSDKShowAd without checking if ads are available first
	// FuseSDK will skip displaying the ad and immediately call the 
	// FUSESDK_AD_WILL_CLOSE callback if an ad is not available, or if there is an error fetching the ad.
	FuseSDKShowAdForZoneID(NULL, NULL);

	return 1;
}

int32 AdWasClosed(void* systemData, void* userData)
{
	// update ad status (see main loop)
	adStatus = "`xffffffAd was closed";

	return 1;
}

int32 RewardedAdCompleted(void* systemData, void* userData)
{
    struct paramList
    {
        const char* preMessage;
        const char* rewardMessage;
        const char* rewardItem;
        int rewardAmount;
    };
    const paramList* params = (paramList*)systemData;

    s3eDebugTracePrintf("RewardedAdCompleted with parameters:");
    s3eDebugTracePrintf("    preroll message - %s", params->preMessage);
    s3eDebugTracePrintf("    reward message - %s", params->rewardMessage);
    s3eDebugTracePrintf("    reward item - %s", params->rewardItem);
    s3eDebugTracePrintf("    reward amount - %i", params->rewardAmount);

    return 1;
}

int32 IAPOfferAccepted(void* systemData, void* userData)
{
    struct paramList
    {
        float productPrice;
        int itemAmount;
        const char* itemName;
        const char* productID;
    };
    const paramList* params = (paramList*)systemData;

    s3eDebugTracePrintf("IAPOfferAccepted with parameters:");
    s3eDebugTracePrintf("    product price - %.2f", params->productPrice);
    s3eDebugTracePrintf("    item amount - %i", params->itemAmount);
    s3eDebugTracePrintf("    item name - %s", params->itemName);
    s3eDebugTracePrintf("    product ID - %s", params->productID);

    return 1;
}

int32 VGOfferAccepted(void* systemData, void* userData)
{
    struct paramList
    {
        const char* purchaseCurrency;
        float purchasePrice;
        const char* itemName;
        int itemAmount;
    };
    const paramList* params = (paramList*)systemData;

    s3eDebugTracePrintf("VGOfferAccepted with parameters:");
    s3eDebugTracePrintf("    purchase currency - %s", params->purchaseCurrency);
    s3eDebugTracePrintf("    purchase price - %.2f", params->purchasePrice);
    s3eDebugTracePrintf("    item name - %s", params->itemName);
    s3eDebugTracePrintf("    item amount - %i", params->itemAmount);

    return 1;
}

int32 LoginComplete(void* systemData, void* userData)
{
	// see FuseSDK/docs/Callbacks.txt for more information about FuseSDK callbacks and their parameters
	struct paramList
	{
		int accountType;
		const char* accountID;
	};
	const paramList* params = (paramList*)systemData;
	
	// update login status (see main loop)
	loginStatus = "`xffffffLogged in to account";

	return 1;
}

// Callback (un)registration
void RegisterFuseCallbacks()
{
    // session start
	FuseSDKRegister(FUSESDK_SESSION_STARTED, &GotSessionReceived, NULL);
	FuseSDKRegister(FUSESDK_SESSION_LOGIN_ERROR, &SessionLoginError, NULL);

    // account login
    FuseSDKRegister(FUSESDK_ACCOUNT_LOGIN_COMPLETE, &LoginComplete, NULL);

    // ad availability
    FuseSDKRegister(FUSESDK_AD_AVAILABILITY_RESPONSE, &AdAvailableResponse, NULL);
	FuseSDKRegister(FUSESDK_AD_WILL_CLOSE, &AdWasClosed, NULL);
    FuseSDKRegister(FUSESDK_REWARDED_AD_COMPLETED, &RewardedAdCompleted, NULL);
    FuseSDKRegister(FUSESDK_IAPOFFER_ACCEPTED, &IAPOfferAccepted, NULL);
    FuseSDKRegister(FUSESDK_VIRTUALGOODSOFFER_ACCEPTED, &VGOfferAccepted, NULL);
}

void UnregisterFuseCallbacks()
{
	FuseSDKUnRegister(FUSESDK_SESSION_STARTED, &GotSessionReceived);
	FuseSDKUnRegister(FUSESDK_SESSION_LOGIN_ERROR, &SessionLoginError);
	FuseSDKUnRegister(FUSESDK_AD_AVAILABILITY_RESPONSE, &AdAvailableResponse);
	FuseSDKUnRegister(FUSESDK_AD_WILL_CLOSE, &AdWasClosed);
	FuseSDKUnRegister(FUSESDK_ACCOUNT_LOGIN_COMPLETE, &LoginComplete);
    FuseSDKUnRegister(FUSESDK_REWARDED_AD_COMPLETED, &RewardedAdCompleted);
    FuseSDKUnRegister(FUSESDK_IAPOFFER_ACCEPTED, &IAPOfferAccepted);
    FuseSDKUnRegister(FUSESDK_VIRTUALGOODSOFFER_ACCEPTED, &VGOfferAccepted);
}

// Main entry point for the application
int main()
{
	// Register for callbacks 
	RegisterFuseCallbacks();
    s3eDebugOutputString("Callbacks registered");

	// start Fuse session
	if( s3eDeviceGetInt(S3E_DEVICE_OS) == S3E_OS_ID_IPHONE )
	{
		// For iOS platform
		FuseSDKStartSession("1cc6d30a-7479-4273-a0d9-a8ec06ab1d2c", NULL);
	}
	else
	{
		// For Android platform
		FuseSDKStartSession("b6ed4e20-1d57-4def-936a-5d0d6b1badba", NULL);
	}

    s3eDebugOutputString("StartSession was called");

    // Wait for a quit request from the host OS
    while (!s3eDeviceCheckQuitRequest())
    {
        // Fill background blue
        s3eSurfaceClear(0, 0, 255);

        // Print a line of debug text to the screen at top left (0,0)
        // Starting the text with the ` (backtick) char followed by 'x' and a hex value
        // determines the colour of the text.
        s3eDebugPrint(120, 150, sessionStatus.c_str(), 0);
		s3eDebugPrint(120, 180, adStatus.c_str(), 0);
		s3eDebugPrint(120, 210, loginStatus.c_str(), 0);
		
        // Flip the surface buffer to screen
        s3eSurfaceShow();

        // Sleep for 0ms to allow the OS to process events etc.
        s3eDeviceYield(0);
    }

	// UnRegister callbacks
	UnregisterFuseCallbacks();

    return 0;
}
