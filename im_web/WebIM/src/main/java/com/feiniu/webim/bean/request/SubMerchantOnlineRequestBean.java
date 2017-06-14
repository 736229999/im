package com.feiniu.webim.bean.request;

import java.util.List;

/**
 * Created by wangchen on 2014/12/25.
 */
public class SubMerchantOnlineRequestBean extends BaseRequestBean{
    private List<Long> list;

    private SubMerchantOnlineRequestBean(){}
    public static SubMerchantOnlineRequestBean getMessage(){
        SubMerchantOnlineRequestBean merchantOnlineRequestBean = new SubMerchantOnlineRequestBean();

        merchantOnlineRequestBean.setCmd(0x947);
        merchantOnlineRequestBean.setDtype((byte)1);
        merchantOnlineRequestBean.setFrom(1002);
        merchantOnlineRequestBean.setFlag(244);

        return merchantOnlineRequestBean;
    }

    public List<Long> getList() {
        return list;
    }

    public void setList(List<Long> list) {
        this.list = list;
    }
}
