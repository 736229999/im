package com.feiniu.webim.bean.response;

import com.feiniu.webim.bean.domain.MerchantOnlineInfo;

import java.util.List;

/**
 * Created by wangchen on 2014/12/25.
 */
public class MerchantOnlineInfoResponseBean extends BaseResponseBean {
    private List<MerchantOnlineInfo> list;

    public List<MerchantOnlineInfo> getList() {
        return list;
    }

    public void setList(List<MerchantOnlineInfo> list) {
        this.list = list;
    }
}
