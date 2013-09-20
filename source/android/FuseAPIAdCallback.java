package com.fuseextension;

import android.util.Log;
import com.fusepowered.util.FuseAdCallback;

public class FuseAPIAdCallback extends FuseAdCallback
{
	private static final String _logTag = "FuseAPIAdCallback";
	private static FuseWrapper _wrapper;

	public FuseAPIAdCallback(FuseWrapper wrapper)
	{
		_wrapper = wrapper;
	}

	public void adAvailabilityResponse(int available, int error)
	{
		Log.d(_logTag, "adAvailabilityResponse(" + available + "," + error + ")");
		_wrapper.FuseAPIAdAvailabilityResponse(available, error);
	}
		
	public void adDisplayed()
	{
		Log.d(_logTag, "adDisplayed()");
		_wrapper.FuseAPIAdDisplayed();
	}

	public void adClicked()
	{
		Log.d(_logTag, "adClicked()");
		_wrapper.FuseAPIAdClicked();
	}

	public void adWillClose()
	{
		Log.d(_logTag, "adWillClose()");
		_wrapper.FuseAPIAdWillClose();
	}	
}
