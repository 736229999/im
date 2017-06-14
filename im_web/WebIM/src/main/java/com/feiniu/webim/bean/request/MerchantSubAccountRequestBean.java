package com.feiniu.webim.bean.request;

/**
 * Created by wangchen on 2014/12/18.
 */
public class MerchantSubAccountRequestBean extends BaseRequestBean{
    private int type;
    private int merchant_id;
    private int group_id;

    public static final MerchantSubAccountRequestBean getMessage(){
        MerchantSubAccountRequestBean merchantSubAccountRequestBean =
                new MerchantSubAccountRequestBean();

        merchantSubAccountRequestBean.setFlag(0xf4);
        merchantSubAccountRequestBean.setFrom(1004);
        merchantSubAccountRequestBean.setCmd(0x0943);
        merchantSubAccountRequestBean.setDid(0);
        merchantSubAccountRequestBean.setDtype((byte) 1);
        merchantSubAccountRequestBean.setLen(0);

        return merchantSubAccountRequestBean;
    }

    public int getType() {
        return type;
    }

    public void setType(int type) {
        this.type = type;
    }

    public int getMerchant_id() {
        return merchant_id;
    }

    public void setMerchant_id(int merchant_id) {
        this.merchant_id = merchant_id;
    }

    public int getGroup_id() {
        return group_id;
    }

    public void setGroup_id(int group_id) {
        this.group_id = group_id;
    }
}
