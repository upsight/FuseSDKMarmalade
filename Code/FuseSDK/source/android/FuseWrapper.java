package com.fuseextension;

import com.ideaworks3d.marmalade.LoaderAPI;
import com.ideaworks3d.marmalade.LoaderActivity;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Locale;
import java.util.Currency;
import android.util.Log;
import android.text.TextUtils;
import android.os.Bundle;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import com.fusepowered.FuseSDK;
import com.fusepowered.FuseSDKConstants;
import com.fusepowered.FuseSDKListener;
import com.fusepowered.util.VerifiedPurchase;
import com.fusepowered.util.AccountType;
import com.fusepowered.util.Gender;

public class FuseWrapper
{
    private static final String _logTag = "Fuse_Marmalade";
	private static FuseWrapper _this = null;
	private static Activity _activity = null;
    private static FuseSDKListener _listener = null;

	public static final int notification_large=0x7f020002;
    public static final int notification_small=0x7f020003;
    
    // Native JNI functions used for EDK Callback
    public native void FuseSDKSessionStartReceived();
    public native void FuseSDKLoginError(int error);
    public native void FuseSDKAdAvailabilityResponse(int available, int error);
    public native void FuseSDKAdWillClose();
	public native void FuseSDKNotificationAction(String action);
	public native void FuseSDKAccountLoginComplete(int accountType, String accountID);
	public native void FuseSDKTimeUpdated(int timestamp);	
	public native void FuseGameConfigurationStart(int numKeys);
	public native void FuseGameConfigurationKeyValue(String key, String value);
	public native void FuseGameConfigurationReceived();
    public native void FuseSDKPurchaseVerification(int verified, String transaction_id,  String originalTransactionID);
    public native void FuseSDKRewardedAdComplete(String preRollMessage, String rewardMessage, String rewardItem, int rewardAmount);
    public native void FuseSDKVirtualGoodsOfferAccepted(String purchaseCurrency, float purchasePrice, String itemName, int itemAmount);
    public native void FuseSDKIAPOfferAccepted(float productPrice, int itemAmount, String itemName, String productID);

	/**+------------------+
	// | Session Creation |
	// +------------------*/
	public FuseWrapper()
	{
		Log.d(_logTag, "FuseWrapper Init");
		_this = this;
		_activity = LoaderAPI.getActivity();
		_listener = new FuseSDKListenerImpl(this);		
	}

	public void FuseSDKStartSession(String key)
    {
		Log.i(_logTag, "Session Start for key " + key);
		FuseSDK.startSession(key, _activity, _listener, null);
        FuseSDK.setPlatform("marmalade-android");
    }

	public void FuseSDKPauseSession()
	{
		Log.i(_logTag, "FuseSDKPauseSession");
		FuseSDK.pauseSession();
	}

	public void FuseSDKResumeSession()
	{
		Log.i(_logTag, "FuseSDKResumeSession");
		FuseSDK.resumeSession(_activity);
	}

	public void FuseSDKTerminateSession()
	{
		Log.i(_logTag, "FuseSDKTerminateSession");
		FuseSDK.endSession();
	}

	public void FuseSDKRegisterForPushNotifications(String projectID)
	{
		Log.i(_logTag, "Register for push notifications: " + projectID);
		String packageName = LoaderAPI.getActivity().getPackageName();
		Intent notificationIntent = null;
		try 
		{
			notificationIntent = new Intent(_activity.getApplicationContext(), Class.forName(packageName + ".Main"));
		} 
		catch (ClassNotFoundException e) 
		{
			Log.e(_logTag, "Main activity class not found: " + packageName);
			e.printStackTrace();
		}
		FuseSDK.setupPushNotifications(projectID, notificationIntent, notification_small, notification_large);
	}

	

	/**+-------------------------+
	// | In-App Purchase Logging |
	// +-------------------------*/
		public void FuseSDKRegisterInAppPurchase(int purchaseState, String purchaseToken, String productId, String orderId, long purchaseTime, String developerPayload, double price, String currency)
	{
		// If we haven't been passed a currency string, make a guess based on the current locale
		if( TextUtils.isEmpty(currency) ) 
		{
			Locale locale = Locale.getDefault();

			if (locale != null)
			{
				Currency c = Currency.getInstance(locale);

				if (c != null)
				{
					currency = c.getCurrencyCode();
				}
			}
		}
		String szPurchaseState = "CANCELED";
		if( purchaseState == 0 )
		{
			szPurchaseState = "PURCHASED";
		}
		else if( purchaseState == 2 )
		{
			szPurchaseState = "REFUNDED";
		}

		Log.d(_logTag, "FuseSDKRegisterInAppPurchase(" + purchaseState + "," + purchaseToken + "," + productId + "," + orderId + "," + purchaseTime + "," + developerPayload + "," + price + "," + currency + ")");
		VerifiedPurchase purchase = new VerifiedPurchase(szPurchaseState, purchaseToken, productId, orderId, purchaseTime, developerPayload);
		FuseSDK.registerInAppPurchase(purchase, price, currency);
	}

	/**+-----------------------+
	// | Fuse Interstitial Ads |
	// +-----------------------*/
	public boolean FuseSDKIsAdAvailableForZoneID(String zoneID)
	{
		return FuseSDK.isAdAvailableForZoneID(zoneID);
	}
	 
    
	public void FuseSDKShowAdForZoneID(final String zoneID, boolean showPre, boolean showPost, String yesText, String noText, String continueText)
	{
		Log.d(_logTag, "FuseSDKShowAd()");
		
		// create the options table
		final HashMap<String, String> options = new HashMap<String, String>();
		if( !showPre )
		{
			options.put(FuseSDKConstants.FuseRewardedAdOptionKey_ShowPreRoll, "false");
		}
		if( !showPost )
		{
			options.put(FuseSDKConstants.FuseRewardedAdOptionKey_ShowPostRoll, "false");
		}
		if( yesText != null)
		{
			options.put(FuseSDKConstants.FuseRewardedOptionKey_PreRollYesButtonText, yesText);
		}
		if( noText != null )
		{
			options.put(FuseSDKConstants.FuseRewardedOptionKey_PreRollNoButtonText, noText);
		}
		if( continueText != null )
		{
			options.put(FuseSDKConstants.FuseRewardedOptionKey_PostRollContinueButtonText, continueText);
		}		

		_activity.runOnUiThread(new Runnable() 
		{
		    public void run() 
			{
		    	FuseSDK.showAdForZoneID(zoneID, options);
		    }
		});
	}

	public void FuseSDKPreloadAdForZoneID(String zoneID)
	{
		FuseSDK.preloadAdForZoneID(zoneID);
	}
	

	/**+---------------+
	// | Notifications |
	// +---------------*/
	public void FuseSDKDisplayNotifications()
	{
		Log.d(_logTag, "FuseSDKDisplayNotifications()");

		_activity.runOnUiThread(new Runnable() 
		{
		    public void run() 
			{
		    	FuseSDK.displayNotifications();
		    }
		});
	}

	/**+------------+
	// | More games |
	// +------------*/
	public void FuseSDKDisplayMoreGames()
	{
		Log.d(_logTag, "FuseSDKDisplayMoreGames()");

		_activity.runOnUiThread(new Runnable()
		{
		    public void run() 
			{
		    	FuseSDK.displayMoreGames();
		    }
		});
	}

	/**+--------+
	// | Gender |
	// +--------*/
	public void FuseSDKRegisterGender(int gender)
	{
		Log.d(_logTag, "FuseSDKRegisterGender(" + gender + ")");
		FuseSDK.registerGender(Gender.getGenderByCode(gender));
	}

	/**+---------------+
	// | Account Login |
	// +---------------*/
	public void FuseSDKFacebookLogin(String facebookId, String name, String accessToken)
	{
		Log.d(_logTag, "FuseSDKFacebookLogin(" + facebookId + "," + name + "," + accessToken + ")");
		FuseSDK.facebookLogin(facebookId, name, accessToken);
	}

	public void FuseSDKTwitterLogin(String twitterId)
	{
        //@TODO: this needs to be fixed: params = twitterID, alias
		Log.d(_logTag, "FuseSDKTwitterLogin(" + twitterId + ")");
		FuseSDK.twitterLogin(twitterId, twitterId);
	}

	public void FuseSDKDeviceLogin(String alias)
	{
		Log.d(_logTag, "FuseSDKDeviceLogin(" + alias + ")");
		FuseSDK.deviceLogin(alias);
	}

	public void FuseSDKFuseLogin(String fuseId, String alias)
	{
		Log.d(_logTag, "FuseSDKFuseLogin(" + fuseId + "," + alias + ")");
		FuseSDK.fuseLogin(fuseId, alias);
	}

	public void FuseSDKGooglePlayLogin(String alias, String token)
	{
		Log.d(_logTag, "FuseSDKGooglePlayLogin(" + alias + "," + token + ")");		
		FuseSDK.googlePlayLogin(alias, token);
	}

	public String FuseSDKGetOriginalAccountId()
	{
		Log.d(_logTag, "FuseSDKGetOriginalAccountId() = " + FuseSDK.getOriginalAccountID());
		return FuseSDK.getOriginalAccountID();
	}

	public String FuseSDKGetOriginalAccountAlias()
	{
		Log.d(_logTag, "FuseSDKGetOriginalAccountAlias() = " + FuseSDK.getOriginalAccountAlias());
		return FuseSDK.getOriginalAccountAlias();
	}

	public int FuseSDKGetOriginalAccountType()
	{
		Log.d(_logTag, "getOriginalAccountType() = " + FuseSDK.getOriginalAccountType());
		return FuseSDK.getOriginalAccountType().getAccountNumber();
	}

	public String FuseSDKGetFuseID()
	{
		Log.d(_logTag, "FuseSDKGetFuseID() = " + FuseSDK.getFuseID());
		return FuseSDK.getFuseID();
	}

	/**+---------------+
	// | Miscellaneous |
	// +---------------*/
	public int FuseSDKgamesPlayed()
	{
		Log.d(_logTag, "FuseSDKgamesPlayed() = " + FuseSDK.gamesPlayed());
		return FuseSDK.gamesPlayed();
	}

	public String FuseSDKLibraryVersion()
	{
		Log.d(_logTag, "FuseSDKLibraryVersion() = " + FuseSDK.libraryVersion());
		return FuseSDK.libraryVersion();
	}

	public boolean FuseSDKConnected()
	{
		Log.d(_logTag, "FuseSDKConnected() = " + FuseSDK.connected());
		return FuseSDK.connected();
	}
	
	public void FuseSDKTimeFromServer()
	{
		Log.d(_logTag, "FuseSDKTimeFromServer()");
		FuseSDK.utcTimeFromServer();
	}

	/**+-----------------+
	// | Data Opt In/Out |
	// +-----------------*/
	public void FuseSDKEnableData(boolean enable)
	{
		Log.d(_logTag, "FuseSDKEnableData(" + enable + ")");
        if( enable )
        {
            FuseSDK.enableData();
        }
        else
        {
            FuseSDK.disableData();
        }
	}

	/**TODO:*/
	//bool FuseSDKDataEnabled() run_on_os_thread

	/**+-------------------------+
	// | Game Configuration Data |
	// +-------------------------*/
	public String FuseSDKGetGameConfigurationValue(String key)
	{
		Log.d(_logTag, "FuseSDKGetGameConfigurationValue(" + key + ") = " + FuseSDK.getGameConfigurationValue(key));
		return FuseSDK.getGameConfigurationValue(key);
	}

	/**+-----------------------------+
	// | Specific Event Registration |
	// +-----------------------------*/
	public void FuseSDKRegisterLevel(int level)
	{
		Log.d(_logTag, "FuseSDKRegisterLevel(" + level + ")");
		FuseSDK.registerLevel(level);
	}

	public void FuseSDKRegisterCurrency(int type, int balance)
	{
		Log.d(_logTag, "FuseSDKRegisterCurrency(" + type + "," + balance + ")");
		FuseSDK.registerCurrency(type, balance);
	}
}

