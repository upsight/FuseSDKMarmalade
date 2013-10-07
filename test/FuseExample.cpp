/*
 * This file provided to demonstrate how to use the FuseAPI Extension
 *
 */

#include "FuseAPI.h"

#include "s3e.h"
#include "s3eDebug.h"

#include <string>
using namespace std;

string sessionStatus = "`xffffffStarting FuseAPI session...";
string adStatus = "`xffffffAd pending...";
string loginStatus = "`xffffffTesting account login...";

// Fuse Callbacks
int32 GotSessionReceived(void* systemData, void* userData)
{
	// Display success message (see main loop)
	sessionStatus = "`xffffffFuseAPI session started!";

	// Check for ad availibility
	// This will cause an FUSEAPI_AD_AVAILABILITY_RESPONSE callback response 
	FuseAPICheckAdAvailable();

	// Register for Google Cloud Messaging (GCM) push notifications
	FuseAPIRegisterForPushNotifications("538100748941");

	// test Fuse login - see FuseAPI/docs/FuseAPI.txt for more information
	// this will return a FUSEAPI_ACCOUNT_LOGIN_COMPLETE callback
	FuseAPIFuseLogin("233963519", "tony stark");

	// Example event registrations:
	// 1) with just a message
	FuseAPIRegisterEvent("Session Started");
	
	// 2) With paramter values:
	double var = 319;
	FuseAPIRegisterEventWithParam("Test Message", "Param Name", "Param Value", "Variable", &var);

	return 1;
}

int32 SessionLoginError(void* systemData, void* userData)
{
	// see FuseAPI/docs/Callbacks.txt for more information about FuseAPI callbacks and their parameters
	struct paramList
	{
		int error;
	};
	const paramList* params = (paramList*)systemData;
	
	// display error message (see main loop)
	sessionStatus = "`xffffffFuseAPI session start error";

	return 1;
}

int32 AdAvailableResponse(void* systemData, void* userData)
{
	// see FuseAPI/docs/Callbacks.txt for more information about FuseAPI callbacks and their parameters
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
		adStatus = "`xffffffDNo ad to display";
	}

	// Display the ad
	// Note: You can call FuseAPIShowAd without checking if ads are available first
	// FuseAPI will skip displaying the ad and immediately call the 
	// FUSEAPI_AD_WILL_CLOSE callback if an ad is not available, or if there is an error fetching the ad.
	FuseAPIShowAd();

	return 1;
}

int32 AdWasClosed(void* systemData, void* userData)
{
	// update ad status (see main loop)
	adStatus = "`xffffffAd was closed";

	// register an event
	// See FuseAPI/docs/FuseAPI.txt for more information about registering events
	FuseAPIRegisterEvent("Viewed Ad");

	return 1;
}

int32 LoginComplete(void* systemData, void* userData)
{
	// see FuseAPI/docs/Callbacks.txt for more information about FuseAPI callbacks and their parameters
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
	FuseAPIRegister(FUSEAPI_SESSION_STARTED, &GotSessionReceived, NULL);
	FuseAPIRegister(FUSEAPI_SESSION_LOGIN_ERROR, &SessionLoginError, NULL);
	FuseAPIRegister(FUSEAPI_AD_AVAILABILITY_RESPONSE, &AdAvailableResponse, NULL);
	FuseAPIRegister(FUSEAPI_AD_WILL_CLOSE, &AdWasClosed, NULL);
	FuseAPIRegister(FUSEAPI_ACCOUNT_LOGIN_COMPLETE, &LoginComplete, NULL);
}

void UnregisterFuseCallbacks()
{
	FuseAPIUnRegister(FUSEAPI_SESSION_STARTED, &GotSessionReceived);
	FuseAPIUnRegister(FUSEAPI_SESSION_LOGIN_ERROR, &SessionLoginError);
	FuseAPIUnRegister(FUSEAPI_AD_AVAILABILITY_RESPONSE, &AdAvailableResponse);
	FuseAPIUnRegister(FUSEAPI_AD_WILL_CLOSE, &AdWasClosed);
	FuseAPIUnRegister(FUSEAPI_ACCOUNT_LOGIN_COMPLETE, &LoginComplete);
}

// Main entry point for the application
int main()
{	
	// Register for callbacks 
	RegisterFuseCallbacks();

	// start Fuse session
	if( s3eDeviceGetInt(S3E_DEVICE_OS) == S3E_OS_ID_IPHONE )
	{
		// For iOS platform
		FuseAPIStartSession("1cc6d30a-7479-4273-a0d9-a8ec06ab1d2c");
	}
	else
	{
		// For Android platform
		FuseAPIStartSession("b6ed4e20-1d57-4def-936a-5d0d6b1badba");
	}

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
