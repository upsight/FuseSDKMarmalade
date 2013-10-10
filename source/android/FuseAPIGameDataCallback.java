package com.fuseextension;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import com.fusepowered.fuseapi.*;
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
		//GameKeyValuePairs user=390463487 rowKey=my data reqId=5 [requestIdMap=(5:my data)][objectMap={Key:my data(myawesomekey,value)(cat,good)}]
		Log.d(_logTag, "gameDataReceived(" + accountId + ",[data]," + requestId + ")");
		
		// received game data with no master key
		if( gameKeyValuePairs.getMap() != null && gameKeyValuePairs.getMap().size() > 0 )
		{
			HashMap<String,GameValue> hashMap = gameKeyValuePairs.getMap();
			_wrapper.FuseAPIGameDataReceivedStart();			
			for (Map.Entry<String,GameValue> entry : hashMap.entrySet())
			{
				boolean isBinary = entry.getValue().isBinary();
				String key = entry.getKey();
				String value = entry.getValue().getValue();
				//Log.d(_logTag, "Key: " + key + " Value: " + value);
				_wrapper.FuseAPIGameDataReceivedKVP(isBinary, key, value);
			}			
			_wrapper.FuseAPIGameDataReceivedEnd(accountId, requestId);
		}
		// received data with a master key
		else if( gameKeyValuePairs.getRequestIdToObjectMapKeyMap() != null && gameKeyValuePairs.getRequestIdToObjectMapKeyMap().size() > 0 )
		{
			HashMap<String,String> hashMap = gameKeyValuePairs.getRequestIdToObjectMapKeyMap();
			_wrapper.FuseAPIGameDataReceivedStart();			
			for (Map.Entry<String,String> entry : hashMap.entrySet())
			{				
				String key = entry.getKey();
				String value = entry.getValue();
				//Log.d(_logTag, "Key: " + key + " Value: " + value);
				HashMap<String,String> gameData = gameKeyValuePairs.getMapForKey(value);
				for( Map.Entry<String,String> dataEntry : gameData.entrySet() )
				{
					_wrapper.FuseAPIGameDataReceivedKVP(false, dataEntry.getKey(), dataEntry.getValue());
				}
			}			
			_wrapper.FuseAPIGameDataReceivedEnd(accountId, requestId);
		}
		else
		{
			Log.d(_logTag, "No KVPs");
			_wrapper.FuseAPIGameDataReceivedStart();
			_wrapper.FuseAPIGameDataReceivedEnd(accountId, requestId);
		}		
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
				
		_wrapper.FuseAPIFriendsListUpdateStart(friendsList.size());
		for (Player friend : friendsList)
		{
			_wrapper.FuseAPIFriendsListUpdatePlayer(friend.getFuseId(), friend.getAlias(), friend.getType(), friend.getAccountId(), friend.getLevel(), friend.getPending(), friend.getCanAttack());
		}
		_wrapper.FuseAPIFriendsListUpdateEnd();
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
		
		_wrapper.FuseAPIMailListUpdateStart(mailList.size());
		if (mailList != null)
		{
			for (Mail mail : mailList)
			{
				_wrapper.FuseAPIMailListUpdate(mail.getId(), mail.getAlias(), mail.getFuseId(), mail.getMessage(), mail.getDate());				
				if( mail.getGift() != null )
				{
					_wrapper.FuseAPIMailListUpdateGift(mail.getGift().getId(), mail.getGift().getName(), mail.getGift().getUrl(), mail.getGift().getAmount());
				}
				else
				{
					// No Gift
					_wrapper.FuseAPIMailListUpdateGift(0, "", "", 0);
				}
			}
		}
		_wrapper.FuseMailListReceived(fuseId);		
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

		//FIXME: is the requestID correct?
		_wrapper.FuseMailError(fuseMailError.ordinal(), getRequestId());
	}



// +-------------------------+
// | Game Configuration Data |
// +-------------------------+
	public void gameConfigurationReceived()
	{
		Log.d(_logTag, "gameConfigurationReceived()");

		HashMap<String, String> gameConfig = FuseAPI.getGameConfiguration();
		_wrapper.FuseGameConfigurationStart(gameConfig.size());
		for (Map.Entry<String, String> entry : gameConfig.entrySet())
		{
			_wrapper.FuseGameConfigurationKeyValue(entry.getKey(), entry.getValue());
		}

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
