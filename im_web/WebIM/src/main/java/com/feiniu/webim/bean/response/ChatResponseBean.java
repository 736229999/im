package com.feiniu.webim.bean.response;

import com.fasterxml.jackson.annotation.JsonInclude;
import com.feiniu.webim.bean.domain.MessageFeatures;
import com.feiniu.webim.constants.Constants;

@JsonInclude(JsonInclude.Include.NON_NULL)
public class ChatResponseBean extends BaseResponseBean implements Comparable<ChatResponseBean> {
	private int merchantId;//聊天返回结果添加店铺ID,用于维护店铺维度的聊天消息
    private int recver;
    private int sender;
    private int msgtype;
    private int origin_id;
    private int type;
    private String msg;
    private MessageFeatures features;
    private long time;

    /**
     * 当买家向卖家发送聊天消息时，会copy一个模拟的聊天消息,
     * 发送给买家对应session里面各个token对应的messageQueue，
     * 对于token等于当前request-token的，不发送此模拟聊天信息
     **/
    private boolean fake;
    private String token;
    private String uuid;
    private boolean fromCS;
	
    public int getMerchantId() {
        return merchantId;
    }

    public void setMerchantId(int merchantId) {
        this.merchantId = merchantId;
    }

    public int getMsgtype() {
        return msgtype;
    }

    public void setMsgtype(int msgtype) {
        this.msgtype = msgtype;
    }

    public int getOrigin_id() {
        return origin_id;
    }

    public void setOrigin_id(int origin_id) {
        this.origin_id = origin_id;
    }

    public int getRecver() {
        return recver;
    }

    public void setRecver(int recver) {
        this.recver = recver;
    }

    public int getSender() {
        return sender;
    }

    public void setSender(int sender) {
        this.sender = sender;
    }

    public String getMsg() {
        return msg;
    }

    public void setMsg(String msg) {
        this.msg = msg;
    }

    public MessageFeatures getFeatures() {
        return features;
    }

    public void setFeatures(MessageFeatures features) {
        this.features = features;
    }

    public int getType() {
        return type;
    }

    public void setType(int type) {
        this.type = type;
    }

    @Override
    public int getSeq() {
        return seq;
    }

    @Override
    public void setSeq(int seq) {
        this.seq = seq;
    }

    public long getTime() {
        return time * Constants.MSG_TIME_TRANSFORM;
    }

    public void setTime(long time) {
        this.time = time;
    }

    public boolean isFake() {
        return fake;
    }

    public void setFake(boolean fake) {
        this.fake = fake;
    }

    public String getToken() {
        return token;
    }

    public void setToken(String token) {
        this.token = token;
    }

    public String getUuid() {
        return uuid;
    }

    public void setUuid(String uuid) {
        this.uuid = uuid;
    }

    public boolean isFromCS() {
        return fromCS;
    }

    public void setFromCS(boolean fromCS) {
        this.fromCS = fromCS;
    }

    public static ChatResponseBean getFakeResponseBean() {
        ChatResponseBean chatResponseBean = new ChatResponseBean();
        chatResponseBean.setCmd(ResponseMessageType.CHAT_FORWARD.getTypeValue());
        chatResponseBean.setFake(true);
        chatResponseBean.setTime(System.currentTimeMillis() / Constants.MSG_TIME_TRANSFORM);

        return chatResponseBean;
    }

    @Override
    public int compareTo(ChatResponseBean o) {
        return (int) (time - o.time);
    }

}
