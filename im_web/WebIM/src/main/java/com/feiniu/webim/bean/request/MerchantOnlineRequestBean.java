package com.feiniu.webim.bean.request;

import java.util.List;

/**
 * Created by wangchen on 2014/12/25.
 */
public class MerchantOnlineRequestBean extends BaseRequestBean{
    private List<Integer> list;

    private MerchantOnlineRequestBean(){}
    public static MerchantOnlineRequestBean getMessage(){
        MerchantOnlineRequestBean merchantOnlineRequestBean = new MerchantOnlineRequestBean();

        merchantOnlineRequestBean.setCmd(2373);
        merchantOnlineRequestBean.setDtype((byte)1);
        merchantOnlineRequestBean.setFrom(1002);
        merchantOnlineRequestBean.setFlag(244);

        return merchantOnlineRequestBean;
    }

    public List<Integer> getList() {
        return list;
    }

    public void setList(List<Integer> list) {
        this.list = list;
    }
}
