FuseAPI for Marmalade SDK 6.3
========================
FuseAPI  Version v1.35

This Extension currently supports
      
    ANDROID 
	IOS

Description
========================

Provides a [Marmalade](http://www.madewithmarmalade.com) wrapper around [FuseAPI](http://www.fuseboxx.com) for Android.

Version: 1.35

Platform: Android, iOS<br>
Extension Developer: Fuse Powered<br>
Library Developer: www.fusepowered.com

Installation
-----------------
1. Copy the FuseAPI directory into your extensions folder.

2. Subproject the FuseAPI Extension in your Marmalade Project's MKB file
```
   subprojects
    {
        FuseAPI
    }
```

Getting Started
-----------------
1. Enable Push Notifications<br><br>
	Edit FuseAPI/source/ExtraAppManifests.txt<br>
		Replace com.fusepowered.marmaladesample with your application bundle ID in the two activities

	Edit FuseAPI/source/ExtraManifests.txt<br>
		Replace com.fusepowered.marmaladesample with your application bundle ID for the two permissions

	Replace notification_large.png and notification_small.png in FuseAPI/res/drawable with your own icons<br>
		The names must be the same.<br>
		notification_large.png is a 72x72 24-bit png<br>
		notification_small.png is a 36x36 24-bit png<br>

2. To use the FuseAPI extension in your project, first include the FuseAPI header:
```
    #include "FuseAPI.h"
```
   
   Next, call 'FuseAPIStartSession' as soon as the app is initialized:
```
    FuseAPIStartSession("app_key"); 
```


3. Project Settings - Register for callbacks as needed<br>
	eg)
```
    FuseAPIRegister(FUSEAPI_SESSION_STARTED, &GotSessionReceived, NULL);
    FuseAPIRegister(FUSEAPI_SESSION_LOGIN_ERROR, &SessionLoginError, NULL);
```

	See FuseAPI/docs/Callbacks.txt for information about callback functions available from the FuseAPI

   The Fuse API key can be found in your dashboard view for the app your are integrating by navigating to Admin->Integrate SDK in the [Fuseboxx](https://www.fuseboxx.com) dashboard.


4. UnRegister Callbacks when application terminates<br>
	eg)
```
    FuseAPIUnRegister(FUSEAPI_SESSION_STARTED, &GotSessionReceived);
    FuseAPIUnRegister(FUSEAPI_SESSION_LOGIN_ERROR, &SessionLoginError);
```

See the test project in FuseAPI/test for a very simple example of registering and unregistering callbacks, starting a session, and displaying an ad. 

### Features ####
See FuseAPI/docs/FuseAPI.txt for a list of function calls supported by the extension

### Callbacks ####
Many FuseAPI callbacks send information as structs through the systemData parameter.  
Information about callbacks and the structures they pass can be found in FuseAPI/docs/Callbacks.txt
The test project gives some examples of retrieving callback data from these structs.