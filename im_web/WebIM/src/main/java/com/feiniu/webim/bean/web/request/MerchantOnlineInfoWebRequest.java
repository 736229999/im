package com.feiniu.webim.bean.web.request;

import com.feiniu.webim.core.Session;

import java.util.List;

/**
 * Created by wangchen on 2015/3/10.
 */
public class MerchantOnlineInfoWebRequest extends BaseWebRequest {
    private List<Integer> merchantIDList;

    public MerchantOnlineInfoWebRequest() {
        super();
    }

    public MerchantOnlineInfoWebRequest(Session session) {
        super(session);
    }

    public List<Integer> getMerchantIDList() {
        return merchantIDList;
    }

    public void setMerchantIDList(List<Integer> merchantIDList) {
        this.merchantIDList = merchantIDList;
    }
}
