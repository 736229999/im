package com.feiniu.webim.bean.web.request;

import com.feiniu.webim.core.Session;

import java.util.List;

/**
 * Created by wangchen on 2015/3/10.
 */
public class SubMerchantOnlineInfoWebRequest extends BaseWebRequest {
    private List<Long> subMerchantIDList;

    public SubMerchantOnlineInfoWebRequest() {
        super();
    }

    public SubMerchantOnlineInfoWebRequest(Session session) {
        super(session);
    }

    public List<Long> getSubMerchantIDList() {
        return subMerchantIDList;
    }

    public void setSubMerchantIDList(List<Long> subMerchantIDList) {
        this.subMerchantIDList = subMerchantIDList;
    }
}
