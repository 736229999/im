package com.feiniu.webim.bean.domain;

/**
 * Author: morningking
 * Date: 2015/8/20
 * Contact: 243717042@qq.com
 * <p/>
 * 浏览器聊天窗口句柄，为了唯一标识前台一个窗口，搬运的mfc的窗口句柄概念
 */
public class ChatWindowHandle {
    //该次聊天 身份验证 基于的token
    private String authToken;
    //基于身份验证token下，聊天窗口的唯一序列号
    private String uuid;
    //是否来自自营
    private boolean isFromCS;
    //是否来自手机
    private boolean fromMobile;
    //商家ID，用于过滤不同商家转发的消息
    private Integer merchantID;

    //----对于自营客服，记录组id
    private int groupID;
    //当前排队号
    private int queueLength;
    //当前被分流到的客服信息
    private MerchantSubAccountInfo merchantSubAccountInfo;
    //最近一次活动时间
    private long lastUpdateTime;

    public synchronized String getAuthToken() {
        return authToken;
    }

    public synchronized void setAuthToken(String authToken) {
        this.authToken = authToken;
    }

    public synchronized String getUuid() {
        return uuid;
    }

    public synchronized void setUuid(String uuid) {
        this.uuid = uuid;
    }

    public synchronized boolean isFromCS() {
        return isFromCS;
    }

    public synchronized void setIsFromCS(boolean isFromCS) {
        this.isFromCS = isFromCS;
    }

    public synchronized boolean isFromMobile() {
        return fromMobile;
    }

    public synchronized void setFromMobile(boolean fromMobile) {
        this.fromMobile = fromMobile;
    }

    public synchronized Integer getMerchantID() {
        return merchantID;
    }

    public synchronized void setMerchantID(Integer merchantID) {
        this.merchantID = merchantID;
    }

    public synchronized int getGroupID() {
        return groupID;
    }

    public synchronized void setGroupID(int groupID) {
        this.groupID = groupID;
    }

    public synchronized int getQueueLength() {
        return queueLength;
    }

    public synchronized void setQueueLength(int queueLength) {
        this.queueLength = queueLength;
    }

    public synchronized MerchantSubAccountInfo getMerchantSubAccountInfo() {
        return merchantSubAccountInfo;
    }

    public synchronized void setMerchantSubAccountInfo(MerchantSubAccountInfo merchantSubAccountInfo) {
        this.merchantSubAccountInfo = merchantSubAccountInfo;
    }

    public long getLastUpdateTime() {
        return lastUpdateTime;
    }

    public void setLastUpdateTime(long lastUpdateTime) {
        this.lastUpdateTime = lastUpdateTime;
    }

    @Override
    public synchronized boolean equals(Object obj) {
        ChatWindowHandle other;

        return obj != null && obj instanceof ChatWindowHandle
                && ((other = (ChatWindowHandle) obj) != null)
                && other.authToken != null && other.authToken.equals(authToken)
                && other.uuid != null && other.uuid.equals(uuid)
                && other.isFromCS == isFromCS;
    }
}
