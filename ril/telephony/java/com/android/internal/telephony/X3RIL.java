package com.android.internal.telephony;

import static com.android.internal.telephony.RILConstants.*;

import android.content.Context;
import android.content.Intent;
import android.os.AsyncResult;
import android.os.Message;
import android.os.Parcel;
import android.os.SystemService;

import android.telephony.PhoneNumberUtils;

public class X3RIL extends RIL implements CommandsInterface {
    private boolean sentHwBootstrap = false;

    public X3RIL(Context context, int networkMode, int cdmaSubscription, Integer instanceId) {
        super(context, networkMode, cdmaSubscription, instanceId);
    }

    private static void x3Sleep(int value) {
    try {
            Thread.sleep(value);
        } catch (InterruptedException ie) {}
    }

    public void
    lgeSendCommand(int command) {
        //RIL_REQUEST_LGE_SEND_COMMAND
        RILRequest rrLSC = RILRequest.obtain(
                0x113, null);
        rrLSC.mParcel.writeInt(1);
        rrLSC.mParcel.writeInt(command);
        send(rrLSC);
        riljLog("X3RIL: LGE COMMAND " + (command) + " sendt");
    }
    
    protected void
    setSpeechCodec(Object oldret) {
        if (oldret == null) {
            riljLog("ERROR: LGE_SELECTED_SPEECH_CODEC is NULL");
            return;
        }
        Intent SelectedSpeechCodecIntent = new Intent("com.lge.ril.SELECTED_SPEECH_CODEC");
        String SelectedSpeechCodecString = (String)oldret;
        int SelectedSpeechCodecNumber = Integer.parseInt(SelectedSpeechCodecString);

        if (RILJ_LOGD) {
        StringBuilder sb = new StringBuilder();
        sb.append("LGE_SELECTED_SPEECH_CODEC setSpeechCodec = ");
        sb.append(SelectedSpeechCodecNumber);
        riljLog(sb.toString());
        }

        SelectedSpeechCodecIntent.putExtra("SelectedSpeechCodec", SelectedSpeechCodecNumber);
        mContext.sendBroadcast(SelectedSpeechCodecIntent);
    }

    @Override
    public void
    queryCallForwardStatus(int cfReason, int serviceClass,
                String number, Message response) {
        RILRequest rr
            = RILRequest.obtain(RIL_REQUEST_QUERY_CALL_FORWARD_STATUS, response);

        rr.mParcel.writeInt(2); // 2 is for query action, not in use anyway
        rr.mParcel.writeInt(cfReason);
        if (serviceClass == 0)
            serviceClass = 255;
        rr.mParcel.writeInt(serviceClass);
        rr.mParcel.writeInt(PhoneNumberUtils.toaFromString(number));
        rr.mParcel.writeString(number);
        rr.mParcel.writeInt (0);

        if (RILJ_LOGD) riljLog(rr.serialString() + "> " + requestToString(rr.mRequest)
                + " " + cfReason + " " + serviceClass);

        send(rr);
    }
    
    @Override
    public void
    setCallForward(int action, int cfReason, int serviceClass,
                String number, int timeSeconds, Message response) {
        RILRequest rr
                = RILRequest.obtain(RIL_REQUEST_SET_CALL_FORWARD, response);

        rr.mParcel.writeInt(action);
        rr.mParcel.writeInt(cfReason);
        if (serviceClass == 0)
            serviceClass = 255;
        rr.mParcel.writeInt(serviceClass);
        rr.mParcel.writeInt(PhoneNumberUtils.toaFromString(number));
        rr.mParcel.writeString(number);
        rr.mParcel.writeInt (timeSeconds);

        if (RILJ_LOGD) riljLog(rr.serialString() + "> " + requestToString(rr.mRequest)
                    + " " + action + " " + cfReason + " " + serviceClass
                    + timeSeconds);

        send(rr);
    }

    static final int RIL_UNSOL_LGE_STK_PROACTIVE_SESSION_STATUS = 1041;
    static final int RIL_UNSOL_LGE_NETWORK_REGISTRATION_ERROR = 1047;
    static final int RIL_UNSOL_LGE_PBREADY = 1049;
    static final int RIL_UNSOL_LGE_BATTERY_LEVEL_UPDATE = 1050;
    static final int RIL_UNSOL_LGE_XCALLSTAT = 1053;
    static final int RIL_UNSOL_LGE_RESTART_RILD = 1055;
    static final int RIL_UNSOL_LGE_RESPONSE_PS_SIGNALING_STATUS = 1058;
    static final int RIL_UNSOL_LGE_SELECTED_SPEECH_CODEC = 1074;
    static final int RIL_UNSOL_LGE_SIM_STATE_CHANGED = 1060;
    static final int RIL_UNSOL_LGE_SIM_STATE_CHANGED_NEW = 1061;
    static final int RIL_UNSOL_LGE_FACTORY_READY = 1080;

    private void restartRild() {
        if (RILJ_LOGD) {
            riljLog("X3RIL: restartRild started");
        }
        setRadioState(RadioState.RADIO_UNAVAILABLE);
        SystemService.stop("ril-daemon");
        RILRequest.resetSerial();

        // Clear request list
        clearRequestList(RADIO_NOT_AVAILABLE, false);
        // Thread sleeps are ususally a terrible idea, but we do want the radio
        // stack to back off for a bit
        SystemService.start("ril-daemon");
        x3Sleep(500);
        setRadioState(RadioState.RADIO_ON);
    }

    @Override
    protected void
    processUnsolicited (Parcel p) {
        Object ret;
        int dataPosition = p.dataPosition(); // save off position within the Parcel
        int response = p.readInt();

        switch(response) {
            case RIL_UNSOL_LGE_STK_PROACTIVE_SESSION_STATUS: ret =  responseVoid(p); break;
            case RIL_UNSOL_LGE_NETWORK_REGISTRATION_ERROR: ret =  responseVoid(p); break;
            case RIL_UNSOL_LGE_PBREADY: ret =  responseVoid(p); break;
            case RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED: ret =  responseVoid(p); break;
            case RIL_UNSOL_LGE_BATTERY_LEVEL_UPDATE: ret =  responseVoid(p); break;
            case RIL_UNSOL_LGE_XCALLSTAT: ret =  responseVoid(p); break;
            case RIL_UNSOL_LGE_SELECTED_SPEECH_CODEC: ret =  responseVoid(p); break;
            case RIL_UNSOL_LGE_SIM_STATE_CHANGED:
            case RIL_UNSOL_LGE_SIM_STATE_CHANGED_NEW: ret =  responseVoid(p); break;
            case RIL_UNSOL_LGE_RESTART_RILD: ret =  responseVoid(p); break;
            case RIL_UNSOL_LGE_RESPONSE_PS_SIGNALING_STATUS: ret = responseVoid(p); break;
            case RIL_UNSOL_LGE_FACTORY_READY: ret =  responseVoid(p); break;
            default:
                // Rewind the Parcel
                p.setDataPosition(dataPosition);

                // Forward responses that we are not overriding to the super class
                super.processUnsolicited(p);
                return;
        }
        switch(response) {
            case RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED:
                /* has bonus radio state int */
                RadioState newState = getRadioStateFromInt(p.readInt());
                p.setDataPosition(dataPosition);
                super.processUnsolicited(p);
                if (RadioState.RADIO_ON == newState) {
                    setNetworkSelectionModeAutomatic(null);
                } else if (RadioState.RADIO_OFF == newState) {
                    sentHwBootstrap = false;
                }
                return;
            case RIL_UNSOL_LGE_FACTORY_READY:
                if (!sentHwBootstrap) {
                    lgeSendCommand(1);
                    sentHwBootstrap = true;
                } else if (RILJ_LOGD) riljLog("sinking LGE request > " + response);
                break;
            case RIL_UNSOL_LGE_PBREADY:
                  x3Sleep(200);  
                  lgeSendCommand(0);
                break;
            case RIL_UNSOL_LGE_RESTART_RILD:
                restartRild();
                break;
            case RIL_UNSOL_LGE_RESPONSE_PS_SIGNALING_STATUS:
                if (RILJ_LOGD) riljLog("sinking LGE request > " + response);
                break;
            case RIL_UNSOL_LGE_STK_PROACTIVE_SESSION_STATUS:
            case RIL_UNSOL_LGE_NETWORK_REGISTRATION_ERROR:
            case RIL_UNSOL_LGE_BATTERY_LEVEL_UPDATE:
                if (RILJ_LOGD) riljLog("sinking LGE request > " + response);
                break;
            case RIL_UNSOL_LGE_XCALLSTAT:
            case RIL_UNSOL_LGE_SELECTED_SPEECH_CODEC:
                setSpeechCodec(ret);
                break;
            case RIL_UNSOL_LGE_SIM_STATE_CHANGED:
            case RIL_UNSOL_LGE_SIM_STATE_CHANGED_NEW:
                if (RILJ_LOGD) unsljLog(response);

                if (mIccStatusChangedRegistrants != null) {
                    mIccStatusChangedRegistrants.notifyRegistrants();
                }
                break;
        }
    }
    
    @Override
    public void getNeighboringCids(Message response) {
        AsyncResult.forMessage(response).exception =
            new CommandException(CommandException.Error.REQUEST_NOT_SUPPORTED);
        response.sendToTarget();
        response = null;
    }
    
    @Override
    public void getImsRegistrationState(Message result) {
        if (mRilVersion >= 8) {
            super.getImsRegistrationState(result);
        } else {
            if (result != null) {
                CommandException ex = new CommandException(
                    CommandException.Error.REQUEST_NOT_SUPPORTED);
                AsyncResult.forMessage(result, null, ex);
                result.sendToTarget();
            }
        }
    }
}
