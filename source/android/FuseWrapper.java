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
import android.app.AlertDialog;
import com.fusepowered.fuseapi.*;
import com.fusepowered.util.FuseCallback;
import com.fusepowered.fuseactivities.*;
import com.fusepowered.util.VerifiedPurchase;
import com.fusepowered.util.GameValue;
import com.fusepowered.util.GameKeyValuePairs;
import com.fusepowered.util.FuseEvent;
//import com.fuseextension.R;

public class FuseWrapper
{
    private static final String _logTag = "Fuse_Marmalade";
	private static FuseWrapper _this = null;
	private static Activity _activity;
	private static FuseCallback _gameDataCallback;
	private static FuseAPIAdCallback _adCallback; 	

	public static final int notification_large=0x7f020002;
    public static final int notification_small=0x7f020003;
    
    // Native JNI functions used for EDK Callback
    public native void FuseAPISessionStartReceived();
    public native void FuseAPILoginError(int error);
    public native void FuseAPIAdAvailabilityResponse(int available, int error);
    public native void FuseAPIAdWillClose();
	public native void FuseAPIAdDisplayed();
	public native void FuseAPIAdClicked();
	public native void FuseAPINotificationAction(String action);
	public native void FuseAPIAccountLoginComplete(int accountType, String accountID);
	public native void FuseAPITimeUpdated(int timestamp);	
	public native void FuseGameDataError(int requestId, int error);
	public native void FuseGameDataSetAcknowledged(int requestID);	
	public native void FuseFriendsListError(int error);	
	public native void FuseMailAcknowledged(int messageId, String fuseId, int requestID);
	public native void FuseMailError(int error);
	public native void FuseMailListError(int error);
	public native void FuseGameConfigurationReceived();

	//TODO:
	public native void FuseAPIGameDataReceivedStart(String accountId);
	public native void FuseAPIGameDataReceivedKVP(boolean isBinary, String key, String value);
	public native void FuseAPIGameDataReceivedEnd(int requestId);
	//public native void FuseFriendsListUpdated(ArrayList<Player> friendsList); //TODO: array of Player
	//public native void FuseMailListReceived(ArrayList<Mail> mailList, String fuseId); //TODO: array of mail
	//TODO: Enemies List callbacks

	/**+------------------+
	// | Session Creation |
	// +------------------*/
	public FuseWrapper()
	{
		Log.d(_logTag, "FuseWrapper Init");
		_this = this;
		_activity = LoaderAPI.getActivity();
		_gameDataCallback = new FuseAPIGameDataCallback(this);
		_adCallback = new FuseAPIAdCallback(this);
		
		FuseAPI.initializeFuseAPI(_activity, _activity.getApplicationContext());		
	}

	public void FuseAPIStartSession(String key)
    {
		Log.i(_logTag, "Session Start for key " + key);
		FuseAPI.startSession(key, _activity, _activity.getApplicationContext(), _gameDataCallback);
    }

	public void FuseAPIPauseSession()
	{
		Log.i(_logTag, "FuseAPIPauseSession");
		FuseAPI.suspendSession();
	}

	public void FuseAPIResumeSession()
	{
		Log.i(_logTag, "FuseAPIResumeSession");
		FuseAPI.resumeSession(_gameDataCallback);
	}

	public void FuseAPITerminateSession()
	{
		Log.i(_logTag, "FuseAPITerminateSession");
		FuseAPI.endSession();
	}

	public void FuseAPIRegisterForPushNotifications(String projectID)
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
		FuseAPI.setupGCM(projectID, notificationIntent, _activity, notification_small, notification_large);
	}

	/**+-----------------+
	// | Analytics Event |
	// +-----------------*/	
	public void FuseAPIRegisterEvent(String message)
	{
		Log.d(_logTag, "FuseAPIRegisterEvent(" + message + ")");
		FuseEvent e = FuseAPI.registerEvent(message, "", "", "", null);		
	}

	public int FuseAPIRegisterEventWithParam(String name, String param_name, String param_value, String variable_name, double variable_value)
	{
		Log.d(_logTag, "FuseAPIRegisterEventWithParam(" + name + "," + param_name + "," + param_value + "," + variable_name + "," + variable_value + ")");
		return FuseAPI.registerEvent(name, param_name, param_value, variable_name, variable_value).ordinal(); 
	}

	//TODO: FuseAPIRegisterEventWithDictionary(String message, HashMap<String,String> eventData)
	private HashMap<String,String> _eventStringData;
	void FuseAPIRegisterEventWithDictionaryStart()
	{
		Log.d(_logTag, "FuseAPIRegisterEventWithDictionaryStart()");
		_eventStringData = new HashMap<String, String>();
	}
	void FuseAPIRegisterEventWithDictionaryKeyValue(String key, String value)
	{
		Log.d(_logTag, "FuseAPIRegisterEventWithDictionaryKeyValue(" + key + "," + value + ")");
		_eventStringData.put(key, value);
	}
	void FuseAPIRegisterEventWithDictionaryEnd(String message)
	{
		Log.d(_logTag, "FuseAPIRegisterEventWithDictionaryEnd(" + message + ")");
		FuseAPI.registerEvent(message, _eventStringData);
		_eventStringData = null;
	}

	//TODO: FuseAPIRegisterEvent(String name, String paramName, String paramValue, HashMap<String,Number> eventData)
	private HashMap<String,Number> _eventNumberData;
	void FuseAPIRegisterEventStart()
	{
		Log.d(_logTag, "FuseAPIRegisterEventStart()");
		_eventNumberData = new HashMap<String,Number>();
	}
	void FuseAPIRegisterEventKeyValue(String key, double value)
	{
		Log.d(_logTag, "FuseAPIRegisterEventKeyValue(" + key + "," + value + ")");
		_eventNumberData.put(key, value);
	}
	int FuseAPIRegisterEventEnd(String name, String paramName, String paramValue)
	{
		Log.d(_logTag, "FuseAPIRegisterEventEnd(" + name + "," + paramName + "," + paramValue + ")");
		int result = FuseAPI.registerEvent(name, paramName, paramValue, _eventNumberData).ordinal();
		_eventNumberData = null;
		return result;
	}

	/**+-------------------------+
	// | In-App Purchase Logging |
	// +-------------------------*/
	public void FuseAPIRegisterInAppPurchase(int purchaseState, String purchaseToken, String productId, String orderId, long purchaseTime, String developerPayload)
	{
		Log.d(_logTag, "FuseAPIRegisterInAppPurchase(" + purchaseState + "," + purchaseToken + "," + productId + "," + orderId + "," + purchaseTime + "," + developerPayload + ")");
		String szPurchaseState = "CANCELED";
		if( purchaseState == 0 )
		{
			szPurchaseState = "PURCHASED";
		}
		else if( purchaseState == 2 )
		{
			szPurchaseState = "REFUNDED";
		}
		VerifiedPurchase purchase = new VerifiedPurchase(szPurchaseState, purchaseToken, productId, orderId, purchaseTime, developerPayload);
		FuseAPI.registerInAppPurchase(purchase);
	}
	public void FuseAPIRegisterInAppPurchase(int purchaseState, String purchaseToken, String productId, String orderId, long purchaseTime, String developerPayload, double price, String currency)
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

		Log.d(_logTag, "FuseAPIRegisterInAppPurchase(" + purchaseState + "," + purchaseToken + "," + productId + "," + orderId + "," + purchaseTime + "," + developerPayload + "," + price + "," + currency + ")");
		VerifiedPurchase purchase = new VerifiedPurchase(szPurchaseState, purchaseToken, productId, orderId, purchaseTime, developerPayload);
		FuseAPI.registerInAppPurchase(purchase, price, currency);
	}

	/**+-----------------------+
	// | Fuse Interstitial Ads |
	// +-----------------------*/
	public void FuseAPIShowAd()
	{
		Log.d(_logTag, "FuseAPIShowAd()");
		
		_activity.runOnUiThread(new Runnable() 
		{
		    public void run() 
			{
		    	FuseAPI.displayAd(new FuseApiAdBrowser(), _adCallback);
		    }
		});
	}

	public void FuseAPICheckAdAvailable()
	{
		Log.d(_logTag, "FuseAPICheckAdAvailable()");
    	FuseAPI.checkAdAvailable(_adCallback);
	}

	/**+---------------+
	// | Notifications |
	// +---------------*/
	public void FuseAPIDisplayNotifications()
	{
		Log.d(_logTag, "FuseAPIDisplayNotifications()");

		_activity.runOnUiThread(new Runnable() 
		{
		    public void run() 
			{
		    	FuseAPI.displayNotifications(new AlertDialog.Builder(_activity));
		    }
		});
	}

	/**+------------+
	// | More games |
	// +------------*/
	public void FuseAPIDisplayMoreGames()
	{
		Log.d(_logTag, "FuseAPIDisplayMoreGames()");

		_activity.runOnUiThread(new Runnable()
		{
		    public void run() 
			{
		    	FuseAPI.displayMoreGames(new FuseApiMoregamesBrowser());
		    }
		});
	}

	/**+--------+
	// | Gender |
	// +--------*/
	public void FuseAPIRegisterGender(int gender)
	{
		Log.d(_logTag, "FuseAPIRegisterGender(" + gender + ")");
		FuseAPI.registerGender(gender);
	}

	/**+---------------+
	// | Account Login |
	// +---------------*/
	public void FuseAPIFacebookLogin(String facebookId, String name, String accessToken)
	{
		Log.d(_logTag, "FuseAPIFacebookLogin(" + facebookId + "," + name + "," + accessToken + ")");
		FuseAPI.facebookLogin(facebookId, name, accessToken, _gameDataCallback);
	}

	public void FuseAPITwitterLogin(String twitterId)
	{
		Log.d(_logTag, "FuseAPITwitterLogin(" + twitterId + ")");
		FuseAPI.twitterLogin(twitterId, _gameDataCallback);
	}

	public void FuseAPIDeviceLogin(String alias)
	{
		Log.d(_logTag, "FuseAPIDeviceLogin(" + alias + ")");
		FuseAPI.deviceLogin(alias, _gameDataCallback);
	}

	public void FuseAPIFuseLogin(String fuseId, String alias)
	{
		Log.d(_logTag, "FuseAPIFuseLogin(" + fuseId + "," + alias + ")");
		FuseAPI.fuseLogin(fuseId, alias, _gameDataCallback);
	}

	public void FuseAPIGooglePlayLogin(String alias, String token)
	{
		Log.d(_logTag, "FuseAPIGooglePlayLogin(" + alias + "," + token + ")");		
		FuseAPI.googlePlayLogin(alias, token, _gameDataCallback);
	}

	public String FuseAPIGetOriginalAccountId()
	{
		Log.d(_logTag, "FuseAPIGetOriginalAccountId() = " + FuseAPI.getOriginalAccountId());
		return FuseAPI.getOriginalAccountId();
	}

	public String FuseAPIGetOriginalAccountAlias()
	{
		Log.d(_logTag, "FuseAPIGetOriginalAccountAlias() = " + FuseAPI.getOriginalAccountAlias());
		return FuseAPI.getOriginalAccountAlias();
	}

	public int FuseAPIGetOriginalAccountType()
	{
		Log.d(_logTag, "getOriginalAccountType() = " + FuseAPI.getOriginalAccountType());
		return FuseAPI.getOriginalAccountType(); 
	}

	public String FuseAPIGetFuseID()
	{
		Log.d(_logTag, "FuseAPIGetFuseID() = " + FuseAPI.getFuseID());
		return FuseAPI.getFuseID();
	}

	/**+---------------+
	// | Miscellaneous |
	// +---------------*/
	public int FuseAPIgamesPlayed()
	{
		Log.d(_logTag, "FuseAPIgamesPlayed() = " + FuseAPI.gamesPlayed());
		return FuseAPI.gamesPlayed();
	}

	public String FuseAPILibraryVersion()
	{
		Log.d(_logTag, "FuseAPILibraryVersion() = " + FuseAPI.libraryVersion());
		return FuseAPI.libraryVersion();
	}

	public boolean FuseAPIConnected()
	{
		Log.d(_logTag, "FuseAPIConnected() = " + FuseAPI.connected());
		return FuseAPI.connected();
	}
	
	public void FuseAPITimeFromServer()
	{
		Log.d(_logTag, "FuseAPITimeFromServer()");
		FuseAPI.utcTimeFromServer(_gameDataCallback);
	}

	/**+-----------------+
	// | Data Opt In/Out |
	// +-----------------*/
	public void FuseAPIEnableData(boolean enable)
	{
		Log.d(_logTag, "FuseAPIEnableData(" + enable + ")");
		FuseAPI.userOptOut(enable ? 0 : 1);
	}

	/**TODO:*/
	//bool FuseAPIDataEnabled() run_on_os_thread

	/**+----------------+
	// | User Game Data |
	// +----------------*/
	//TODO: int FuseAPISetGameData(String key, bool isCollection, String fuseId, HashTable<String, GameValue> gameData) run_on_os_thread
	private static HashMap<String,GameValue> _setGameData;
	public void FuseAPISetGameDataStart()
	{
		Log.d(_logTag, "FuseAPISetGameDataStart()");
		_setGameData = new HashMap<String,GameValue>();
	}

	public void FuseAPISetGameDataKeyValue(String entryKey, String entryValue, boolean isBinary)
	{
		Log.d(_logTag, "FuseAPISetGameDataKeyValue(" + entryKey + "," + entryValue + "," + isBinary + ")");
		_setGameData.put(entryKey, new GameValue(entryValue, isBinary));
	}

	public int FuseAPISetGameDataEnd(String key, boolean isCollection, String fuseId)
	{
		Log.d(_logTag, "FuseAPISetGameDataEnd(" + key + "," + isCollection + "," + fuseId + ")");

		GameKeyValuePairs gameKeyValuePairs = new GameKeyValuePairs();
		gameKeyValuePairs.setMap(_setGameData);

		FuseAPIGameDataCallback callback = new FuseAPIGameDataCallback();

		if (key != null && !key.equals(""))
		{
			FuseAPI.setGameData(fuseId, key, gameKeyValuePairs, callback);
		}
		else if (fuseId.equals(FuseAPIGetFuseID()))
		{
			FuseAPI.setGameData(gameKeyValuePairs, callback);
		}

		_setGameData = null;

		return callback.getRequestId();
	}

	//TODO: int FuseAPIGetGameData(String key, String fuseId, ArrayList<String> gameData) run_on_os_thread
	private static ArrayList<String> _getGameData;
	public void FuseAPIGetGameDataStart()
	{
		Log.d(_logTag, "FuseAPIGetGameDataStart()");
		_getGameData = new ArrayList<String>();
	}

	public void FuseAPIGetGameDataKey(String entryKey)
	{
		Log.d(_logTag, "FuseAPIGetGameDataKey(" + entryKey + ")");
		_getGameData.add(entryKey);
	}

	public int FuseAPIGetGameDataEnd(String key, String fuseId)
	{
		Log.d(_logTag, "FuseAPIGetGameDataEnd(" + key + "," + fuseId + ")");

		FuseAPIGameDataCallback callback = new FuseAPIGameDataCallback();

		if (fuseId != null && !fuseId.equals(""))
		{
			if (key != null && !key.equals(""))
			{
				FuseAPI.getFriendGameData(key, _getGameData, callback, fuseId);
			}
			else
			{
				FuseAPI.getFriendGameData(_getGameData, callback, fuseId);
			}
		}
		else
		{
			if (key != null && !key.equals(""))
			{
				FuseAPI.getGameData(key, _getGameData, callback);
			}
			else
			{
				FuseAPI.getGameData(_getGameData, callback);
			}
		}

		_getGameData = null;

		return callback.getRequestId();
	}

	/**+-------------+
	// | Friend List |
	// +-------------*/
	public void FuseAPIUpdateFriendsListFromServer()
	{
		Log.d(_logTag, "FuseAPIUpdateFriendsListFromServer()");
		FuseAPI.updateFriendsListFromServer(_gameDataCallback);
	}

	//TODO: List<Player> FuseAPIGetFriendsList()
	//List<Player> FuseAPIGetFriendsList() run_on_os_thread

	/**+---------+
	// | Gifting |
	// +---------*/
	public void FuseAPIGetMailListFromServer()
	{
		Log.d(_logTag, "FuseAPIGetMailListFromServer()");
		FuseAPI.getMailListFromServer(_gameDataCallback);
	}

	public void FuseAPIGetMailListFriendFromServer(String fuseId)
	{
		Log.d(_logTag, "FuseAPIGetMailListFriendFromServer(" + fuseId + ")");
		FuseAPI.getMailListFriendFromServer(fuseId, _gameDataCallback);
	}

	public void FuseAPISetMailAsReceived(int messageId)
	{
		Log.d(_logTag, "FuseAPISetMailAsReceived(" + messageId + ")");
		FuseAPI.setMailAsReceived(messageId);
	}

	public int FuseAPISendMailWithGift(String fuseId, String message, int giftId, int giftAmount)
	{
		Log.d(_logTag, "FuseAPISendMailWithGift(" + fuseId + "," + message + "," + giftId + "," + giftAmount + ")");
		return FuseAPI.sendMailWithGift(fuseId, message, giftId, giftAmount, _gameDataCallback);
	}

	public int FuseAPISendMail(String fuseId, String message)
	{
		Log.d(_logTag, "FuseAPISendMail(" + fuseId + "," + message + ")");
		return FuseAPI.sendMail(fuseId, message, _gameDataCallback);
	}

	//TODO: return mail list
	//List<Mail> FuseAPIGetMailList(String fuseId) run_on_os_thread

	/**+-------------------------+
	// | Game Configuration Data |
	// +-------------------------*/
	public String FuseAPIGetGameConfigurationValue(String key)
	{
		Log.d(_logTag, "FuseAPIGetGameConfigurationValue(" + key + ") = " + FuseAPI.getGameConfigurationValue(key));
		return FuseAPI.getGameConfigurationValue(key);
	}

	/**TODO: return a hash map or array of kvps, or just an array of keys?*/
	//String[]  FuseAPIGetGameConfigKeys() run_on_os_thread 

	/**+-----------------------------+
	// | Specific Event Registration |
	// +-----------------------------*/
	public void FuseAPIRegisterLevel(int level)
	{
		Log.d(_logTag, "FuseAPIRegisterLevel(" + level + ")");
		FuseAPI.registerLevel(level);
	}

	public void FuseAPIRegisterCurrency(int type, int balance)
	{
		Log.d(_logTag, "FuseAPIRegisterCurrency(" + type + "," + balance + ")");
		FuseAPI.registerCurrency(type, balance);
	}

	public void FuseAPIRegisterFlurryView()
	{
		Log.d(_logTag, "FuseAPIRegisterFlurryView()");
		FuseAPI.registerFlurryView();
	}

	void FuseAPIRegisterFlurryClick()
	{
		Log.d(_logTag, "FuseAPIRegisterFlurryClick()");
		FuseAPI.registerFlurryClick();
	}

	void FuseAPIRegisterTapjoyReward(int amount)
	{
		Log.d(_logTag, "FuseAPIRegisterTapjoyReward(" + amount + ")");
		FuseAPI.registerTapjoyReward(amount);
	}

}

