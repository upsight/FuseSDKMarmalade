FuseSDK for Marmalade
========================
FuseSDK  Version v2.0.0

This Extension currently supports
      
    ANDROID 
	IOS

Description
========================

Provides a [Marmalade](http://www.madewithmarmalade.com) wrapper around [FuseSDK](http://www.fuseboxx.com) for Android.

Version: 2.0.0

Platform: Android, iOS
Extension Developer: Fuse Powered
Library Developer: www.fusepowered.com

Installation
-----------------
1. Copy the FuseSDK directory into your extensions folder.

2. Subproject the FuseSDK Extension in your Marmalade Project's MKB file
subprojects
{
	FuseSDK
}

Getting Started
-----------------
1. Enable Push Notifications
	Edit FuseSDK/source/ExtraAppManifests.txt
		Replace com.fusepowered.marmaladesample with your application bundle ID in the two activities

	Edit FuseSDK/source/ExtraManifests.txt
		Replace com.fusepowered.marmaladesample with your application bundle ID for the two permissions

	Replace notification_large.png and notification_small.png in FuseSDK/res/drawable with your own icons
		The names must be the same.
		notification_large.png is a 72x72 24-bit png
		notification_small.png is a 36x36 24-bit png

2. To use the FuseSDK extension in your project, first include the FuseSDK header: 
    #include "FuseSDK.h"
   
   Next, call 'FuseSDKStartSession' as soon as the app is initialized:
	FuseSDKStartSession("app_key"); 


3. Project Settings - Register for callbacks as needed
	eg)
	FuseSDKRegister(FUSESDK_SESSION_STARTED, &GotSessionReceived, NULL);
	FuseSDKRegister(FUSESDK_SESSION_LOGIN_ERROR, &SessionLoginError, NULL);

	See FuseSDK/docs/Callbacks.txt for information about callback functions available from the FuseSDK

   The Fuse API key can be found in your dashboard view for the app your are trying to integrate by navigating to Admin->Integrate SDK in the [Fuseboxx](https://www.fuseboxx.com) dashboard.


4. UnRegister Callbacks when application terminates
	eg)
	FuseSDKUnRegister(FUSESDK_SESSION_STARTED, &GotSessionReceived);
	FuseSDKUnRegister(FUSESDK_SESSION_LOGIN_ERROR, &SessionLoginError);

See the test project in FuseSDK/test for a very simple example of registering and unregistering callbacks, starting a session, and displaying an ad. 

### Features ####
See FuseSDK/docs/FuseSDK.txt for a list of function calls supported by the extension

### Callbacks ####
Many FuseSDK callbacks send information as structs through the systemData parameter.  
Information about callbacks and the structures they pass can be found in FuseSDK/docs/Callbacks.txt
The test project gives some examples of retrieving callback data from these structs.