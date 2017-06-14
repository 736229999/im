package com.feiniu.webim.bean.domain;

import java.math.BigDecimal;

/**
 * Author: morningking
 * Date: 2015/11/10
 * Contact: 243717042@qq.com
 */
public class FeiniuItemInfo {
    private String smSeq;
    private BigDecimal smPrice;
    private String smTitle;
    private String smPic;

    public String getSmSeq() {
        return smSeq;
    }

    public void setSmSeq(String smSeq) {
        this.smSeq = smSeq;
    }

    public BigDecimal getSmPrice() {
        return smPrice;
    }

    public void setSmPrice(BigDecimal smPrice) {
        this.smPrice = smPrice;
    }

    public String getSmTitle() {
        return smTitle;
    }

    public void setSmTitle(String smTitle) {
        this.smTitle = smTitle;
    }

    public String getSmPic() {
        return smPic;
    }

    public void setSmPic(String smPic) {
        this.smPic = smPic;
    }
}
