package com.feiniu.webim.bean.request;

import com.feiniu.webim.constants.Constants;

/**
 * Author: morningking
 * Date: 2016/4/19 17:06
 * Contact: 243717042@qq.com
 */
public class RetrieveOfflineMessageRequest extends BaseRequestBean {
    public RetrieveOfflineMessageRequest() {
        this.setVlink(0);
        this.setFlag(0xf4);
        this.setCmd(0x98B);
        this.setFrom(Constants.SERVER_VERSION);
        this.setSeq(0);
        this.setDtype((byte) 1);
        this.setLen(0);
        this.setDid(0);
    }
}
