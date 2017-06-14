package com.feiniu.fnSubAccount.util.AccountUtil;

import com.feiniu.fnSubAccount.bean.CSAccountResponse;
import com.feiniu.fnSubAccount.bean.CSMoitorInfoShow;
import com.feiniu.fnSubAccount.bean.CSMonitorInfo;
import com.feiniu.fnSubAccount.bean.ImApi.CSMonitorInfoNew;
import com.feiniu.fnSubAccount.util.FormatUtils;

/**
 * Created by wangzhen on 2015/11/19.
 */
public class AccountFormatUtil {
    public static String[] USER_LEVEL_STR={"主管","员工"};
    public static String[] USER_STATUS={"停用","开启"};
    public  static void  formatAccountInfo(CSAccountResponse index){
        if (index==null)
            return;
        index.setMoumouUserLevel(USER_LEVEL_STR[Integer.valueOf(index.getMoumouUserLevel())]);
        index.setImportTime(FormatUtils.convertNumberToDate(Long.valueOf(index.getImportTime())));
        index.setAccountStates(USER_STATUS[Integer.valueOf(index.getAccountStates())]);
    }

    public static CSMoitorInfoShow getEmptyCSMoitorInfoShow(Long cServiceId,String fullName){
        CSMoitorInfoShow csMoitorInfoShow =new CSMoitorInfoShow();
        csMoitorInfoShow.setcServiceId(cServiceId);
        csMoitorInfoShow.setUserNameFull(fullName);
        csMoitorInfoShow.setOnlineNameFull("--");
        csMoitorInfoShow.setLimitNameFull("--");
        csMoitorInfoShow.setOfflineNameFull(fullName);
        csMoitorInfoShow.setAskingCont("0");
        csMoitorInfoShow.setReceptionCount("0");
        csMoitorInfoShow.setStatus("19");//离线
        return csMoitorInfoShow;
    }

    public static CSMoitorInfoShow fromatCSMoitorInfoToshow(CSMonitorInfo csMoitorInfo,Long cServiceId,String fullName) {
        if (csMoitorInfo == null)
            return getEmptyCSMoitorInfoShow(cServiceId,fullName);
        CSMoitorInfoShow csMoitorInfoShow = new CSMoitorInfoShow();
        csMoitorInfoShow.setUserNameFull(fullName);
        csMoitorInfoShow.setOnlineNameFull(fullName);
        if (csMoitorInfo.getCustomerlist().size() >= csMoitorInfo.getRecpLimit())
            csMoitorInfoShow.setLimitNameFull(fullName);
        else
            csMoitorInfoShow.setLimitNameFull("--");
        csMoitorInfoShow.setOfflineNameFull("--");
        csMoitorInfoShow.setAskingCont(csMoitorInfo.getConsult().toString());
        csMoitorInfoShow.setReceptionCount(csMoitorInfo.getRecption().toString());
        return csMoitorInfoShow;
    }

    public static CSMoitorInfoShow fromatCSMoitorInfoNewToshow(CSMonitorInfoNew csMoitorInfoNew,Long cServiceId,String fullName) {
        if (csMoitorInfoNew == null)
            return getEmptyCSMoitorInfoShow(cServiceId,fullName);
        CSMoitorInfoShow csMoitorInfoShow = new CSMoitorInfoShow();
        csMoitorInfoShow.setcServiceId(cServiceId);
        csMoitorInfoShow.setUserNameFull(fullName);
        csMoitorInfoShow.setOnlineNameFull(fullName);
        if (csMoitorInfoNew.getConsultCount() >=
                Integer.valueOf(csMoitorInfoNew.getCachedUserInfo().getReceptionLimit()))
            csMoitorInfoShow.setLimitNameFull(fullName);
        else
            csMoitorInfoShow.setLimitNameFull("--");
        csMoitorInfoShow.setOfflineNameFull("--");
        csMoitorInfoShow.setAskingCont(csMoitorInfoNew.getConsultCount().toString());
        csMoitorInfoShow.setReceptionCount(csMoitorInfoNew.getReceptionCount().toString());
        csMoitorInfoShow.setStatus(csMoitorInfoNew.getCachedUserInfo().getStatus());
        return csMoitorInfoShow;
    }
}
