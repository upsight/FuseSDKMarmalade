package com.fuseextension;

import com.fusepowered.*;
import com.fusepowered.util.Player;
import java.util.ArrayList;
import android.util.Log;

/**
 * Created by Brennan on 2/17/15.
 */
public class FuseSDKListenerImpl implements FuseSDKListener
{
	private static FuseWrapper _wrapper;
    private static final String _logTag = "FuseSDKListener";

	public FuseSDKListenerImpl(FuseWrapper wrapper)
	{
		_wrapper = wrapper;		
	}

    // ------------------------------------ Session

    /**
     * @brief Called when session is successfully started
     */
    public void sessionStartReceived()
	{
		Log.d(_logTag, "sessionStartReceived()");
		_wrapper.FuseSDKSessionStartReceived();
	}

    /**
     * @brief Called when the sessions fails to start (usually due to lack of connectivity)
     * @param error
     */
    public void sessionLoginError(FuseError error)
	{
		Log.d(_logTag, "sessionLoginError(" + error.errorCode + ")");
		_wrapper.FuseSDKLoginError(error.errorCode);
	}

    // ------------------------------------ Game Date

    /**
     * @brief Called when game configuration is received or updated
     */
    public void gameConfigurationReceived()
	{
        //@TODO: implement this callback
	}

    // ------------------------------------ Login

    /**
     * @brief Called after an account is logged in to successfully
     * @param type
     * @param accountId
     */
    public void accountLoginComplete(int type, String accountId)
	{
        _wrapper.FuseSDKAccountLoginComplete(type, accountId);
	}
    public void accountLoginError(String accountId, FuseError error)
	{
        _wrapper.FuseSDKLoginError(1);
	}

    // ------------------------------------ Notifications

    /**
     * @brief Called after a user clicks on a Fuse notification (e.g. an update notification)
     * @param action
     */
    public void notificationAction(String action)
	{
        _wrapper.FuseSDKNotificationAction(action);
	}
    public void notificationWillClose()
	{
	 // TODO: This doesn't exist either!
	 }

	 // ------------------------------------ Friends
    public void friendAdded(String fuseId, FuseError error)
    {

    }
    public void friendRemoved(String fuseId, FuseError error)
    {

    }
    public void friendAccepted(String fuseId, FuseError error)
    {

    }
    public void friendRejected(String fuseId, FuseError error)
    {

    }
    public void friendsMigrated(String fuseId, FuseError migrateFriendsError)
    {

    }
    public void friendsListUpdated(ArrayList<Player> fuseId)
    {

    }
    public void friendsListError(FuseError error)
    {
        
    }

    // ------------------------------------ IAP

    /**
     * @brief Called after reporting an in app purchase to Fuse
     * @param verified
     * @param transaction_id
     * @param originalTransactionID
     */
    public void purchaseVerification(int verified, String transaction_id,  String originalTransactionID)
	{
        _wrapper.FuseSDKPurchaseVerification(verified, transaction_id, originalTransactionID);
	}

    // ------------------------------------ Ads

    /**
     * @brief Called some time after calling FuseSDK.preloadAdForZoneID to report if an ad is available or not
     * @param available
     * @param error
     */
    public void adAvailabilityResponse(boolean available, int error)
	{
        _wrapper.FuseSDKAdAvailabilityResponse(available ? 1 : 0, error);
	}

    /**
     * @brief Called after FuseSDK.showAdForZoneID when the ad is closed, or if Fuse is unable to show an ad within the 3 second timeout
     */
    public void adWillClose()
	{
        _wrapper.FuseSDKAdWillClose();
	}

    public void adFailedToDisplay()
    {
        //TODO
    }

    // ------------------------------------ Rewarded Ads

    /**
     * @brief Called after a rewarded video is shown and a reward should be given to the user
     * @param rewardInfo
     * @see com.fusepowered.RewardedInfo
     */
    public void rewardedAdCompleteWithObject(RewardedInfo rewardInfo)
	{
        _wrapper.FuseSDKRewardedAdComplete(rewardInfo.preRollMessage, rewardInfo.rewardMessage, rewardInfo.rewardItem, rewardInfo.rewardAmount);
	}

    // ------------------------------------ Offers

    /**
     * @brief Called after an IAP offer is accepted by the user
     * @param offer
     * @see com.fusepowered.OfferInfo
     */
    public void IAPOfferAcceptedWithObject(IAPOfferInfo offer)
	{
        _wrapper.FuseSDKIAPOfferAccepted(offer.productPrice, offer.itemAmount, offer.itemName, offer.productID);
	}

    /**
     * @brief Called after an virtual goods offer is accepted by the user
     * @param offer
     * @see com.fusepowered.OfferInfo
     */
    public void virtualGoodsOfferAcceptedWithObject(VGOfferInfo offer)
	{
        _wrapper.FuseSDKVirtualGoodsOfferAccepted(offer.purchaseCurrency, offer.purchasePrice, offer.itemName, offer.itemAmount);
	}

    // ------------------------------------ Misc

    /**
     * @brief Called after the current timestamp is received from the server
     * @param timestamp
     */
    public void timeUpdated(int timestamp)
	{
        _wrapper.FuseSDKTimeUpdated(timestamp);
	}

    public void didRecieveGCMRegistrationToken(String newRegistrationID)
	{
	}
}