package com.fuseextension;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import com.fusepowered.util.FuseAttackErrors;
import com.fusepowered.util.FuseEnemiesListError;
import com.fusepowered.util.FuseFriendsListError;
import com.fusepowered.util.FuseGameDataCallback;
import com.fusepowered.util.FuseGameDataError;
import com.fusepowered.util.FuseMailError;
import com.fusepowered.util.GameKeyValuePairs;
import com.fusepowered.util.GameValue;
import com.fusepowered.util.Mail;
import com.fusepowered.util.Player;
import com.fusepowered.util.UserTransactionLog;

import android.util.Log;

public class FuseAPIGameDataCallback extends FuseGameDataCallback
{
	private static final String _logTag = "FuseAPIGameDataCallback";
	private int _ourRequestId = 0;
	private static int _nextRequestId = 1;
	private static FuseWrapper _wrapper;
	
	public FuseAPIGameDataCallback()
	{
		_ourRequestId = _nextRequestId++;
	}

	public FuseAPIGameDataCallback(FuseWrapper wrapper)
	{
		_wrapper = wrapper;
		_ourRequestId = _nextRequestId++;
	}

	public int getRequestId()
	{
		Log.d(_logTag, "getRequestId() = " + _ourRequestId);
		return _ourRequestId;
	}

	public void callback()
	{
		Log.d(_logTag, "callback()");
	}



// +------------------+
// | Session Creation |
// +------------------+
	public void sessionStartReceived()
	{
		Log.d(_logTag, "sessionStartReceived()");
		_wrapper.FuseAPISessionStartReceived();
	}
	
	public void sessionLoginError(int error)
	{
		Log.d(_logTag, "sessionLoginError(" + error + ")");
		_wrapper.FuseAPILoginError(error);
	}



// +---------------+
// | Notifications |
// +---------------+	
	public void notificationAction(final String action)
	{
		Log.d(_logTag, "notificationAction(" + action + ")");
		_wrapper.FuseAPINotificationAction(action);
	}



// +---------------+
// | Account Login |
// +---------------+
	public void accountLoginComplete(final int accountType, final String accountId)
	{
		Log.d(_logTag, "accountLoginComplete(" + accountType + "," + accountId + ")");
		_wrapper.FuseAPIAccountLoginComplete(accountType, accountId);
	}



// +---------------+
// | Miscellaneous |
// +---------------+
	public void timeUpdated(final int timestamp)
	{
		Log.d(_logTag, "timeUpdated(" + timestamp + ")");
		_wrapper.FuseAPITimeUpdated(timestamp);
	}

	

// +----------------+
// | User Game Data |
// +----------------+
	public void gameDataReceived(String accountId, GameKeyValuePairs gameKeyValuePairs)
	{
		gameDataReceived(accountId, gameKeyValuePairs, -1);
	}

	public void gameDataReceived(String accountId, GameKeyValuePairs gameKeyValuePairs, int requestId)
	{
		Log.d(_logTag, "gameDataReceived(" + accountId + ",[data]," + requestId + ")");
	
		_wrapper.FuseAPIGameDataReceivedStart(accountId);
		HashMap<String,GameValue> hashMap = gameKeyValuePairs.getMap();
		for (Map.Entry<String,GameValue> entry : hashMap.entrySet())
		{
			boolean isBinary = entry.getValue().isBinary();
			String key = entry.getKey();
			String value = entry.getValue().getValue();

			Log.d(_logTag, "Key: " + key + " Value: " + value);
			_wrapper.FuseAPIGameDataReceivedKVP(isBinary, key, value);
		}

		_wrapper.FuseAPIGameDataReceivedEnd(requestId);
	}

	public void gameDataError(FuseGameDataError fuseGameDataError)
	{
		gameDataError(fuseGameDataError, -1);
	}

	public void gameDataError(FuseGameDataError fuseGameDataError, int requestId)
	{
		Log.d(_logTag, "gameDataError(" + fuseGameDataError + "," + requestId + ")");
		_wrapper.FuseGameDataError(requestId, fuseGameDataError.ordinal());
	}

	public void gameDataSetAcknowledged(int requestId)
	{
		Log.d(_logTag, "gameDataSetAcknowledged(" + requestId + ")");
		_wrapper.FuseGameDataSetAcknowledged(requestId);
	}

	

// +-------------+
// | Friend List |
// +-------------+
	public void friendsListUpdated(ArrayList<Player> friendsList)
	{
		Log.d(_logTag, "friendsListUpdated([data])");

		//TODO: pass friends list data to wrapper
		//FuseUnityAPI.SendMessage("FuseAPI_Android", "_ClearArgumentList", "");

		for (Player friend : friendsList)
		{
			//FuseUnityAPI.SendMessage("FuseAPI_Android", "_AddArgument", friend.getFuseId());
			//FuseUnityAPI.SendMessage("FuseAPI_Android", "_AddArgument", friend.getAccountId()); 
			//FuseUnityAPI.SendMessage("FuseAPI_Android", "_AddArgument", friend.getAlias());
			//FuseUnityAPI.SendMessage("FuseAPI_Android", "_AddArgument", Integer.toString(friend.getPending()));
		}

		//FuseUnityAPI.SendMessage("FuseAPI_Android", "_FriendsListUpdated", "");
	}

	public void friendsListError(FuseFriendsListError fuseFriendsListError)
	{
		Log.d(_logTag, "friendsListError(" + fuseFriendsListError + ")");
		_wrapper.FuseFriendsListError(fuseFriendsListError.ordinal());
	}



// +---------+
// | Gifting |
// +---------+
	public void mailListReceived(ArrayList<Mail> mailList, String fuseId)
	{
		Log.d(_logTag, "mailListReceived([data]," + fuseId + ")");

		//TODO: send mail list data to wrapper

		/*
		FuseUnityAPI.SendMessage("FuseAPI_Android", "_ClearArgumentList", "");
		if (mailList != null)
		{
			for (Mail mail : mailList)
			{
				FuseUnityAPI.SendMessage("FuseAPI_Android", "_AddArgument", Integer.toString(mail.getId()));
				FuseUnityAPI.SendMessage("FuseAPI_Android", "_AddArgument", mail.getDate());
				FuseUnityAPI.SendMessage("FuseAPI_Android", "_AddArgument", mail.getAlias());
				FuseUnityAPI.SendMessage("FuseAPI_Android", "_AddArgument", mail.getMessage());
				if( mail.getGift() != null )
				{
					FuseUnityAPI.SendMessage("FuseAPI_Android", "_AddArgument", Integer.toString(mail.getGift().getId()));
					FuseUnityAPI.SendMessage("FuseAPI_Android", "_AddArgument", mail.getGift().getName());
					FuseUnityAPI.SendMessage("FuseAPI_Android", "_AddArgument", Integer.toString(mail.getGift().getAmount()));
				}
				else
				{
					// No Gift
					FuseUnityAPI.SendMessage("FuseAPI_Android", "_AddArgument", "0");
					FuseUnityAPI.SendMessage("FuseAPI_Android", "_AddArgument", "");
					FuseUnityAPI.SendMessage("FuseAPI_Android", "_AddArgument", "0");
				}
			}
		}

		FuseUnityAPI.SendMessage("FuseAPI_Android", "_MailListReceived", fuseId);
		*/
	}

	public void mailListError(FuseMailError fuseMailError)
	{
		Log.d(_logTag, "mailListError(" + fuseMailError + ")");
		_wrapper.FuseMailListError(fuseMailError.ordinal());
	}

	public void mailAcknowledged(int messageId, String fuseId, int requestID)
	{
		Log.d(_logTag, "mailAcknowledged(" + messageId + "," + fuseId + "," + requestID + ")");
		_wrapper.FuseMailAcknowledged(messageId, fuseId, requestID);
	}

	public void mailError(FuseMailError fuseMailError)
	{
		Log.d(_logTag, "mailError(" + fuseMailError + ")");
		//FuseUnityAPI.SendMessage("FuseAPI_Android", "_MailError", Integer.toString(fuseMailError.ordinal()));
	}



// +-------------------------+
// | Game Configuration Data |
// +-------------------------+
	public void gameConfigurationReceived()
	{
		Log.d(_logTag, "gameConfigurationReceived()");
		_wrapper.FuseGameConfigurationReceived();
	}



	//TODO: enemies list
	public void enemiesListError(FuseEnemiesListError a)
	{
	}

	public void enemiesListResult(ArrayList<Player> a)
	{
	}

	public void attackRobberyLogReceived(ArrayList<UserTransactionLog> a)
	{
	}

	public void attackRobberyLogError(FuseAttackErrors a)
	{
	}
}