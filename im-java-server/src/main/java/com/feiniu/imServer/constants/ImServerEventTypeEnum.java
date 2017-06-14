package com.feiniu.imServer.constants;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by wangzhen on 2016/3/4.
 */
public enum ImServerEventTypeEnum {
    EVENT_RECEPTION_END_BY_SERVICE(0,"0客服结束接待"), //0客服结束接待
    EVENT_CANCEL_QUEUE_BY_CUSTOMER(1,"1客户取消排队"),//1客户取消排队
    EVENT_CANCEL_RECEPTION_BY_CUSTOMER(2,"2客户取消接待"),//2客户取消接待
    EVENT_SERVICE_ON_LINE(3,"3客服上线"),//3客服上线
    EVENT_SERVICE_OFF_LINE(4,"4客服下线"),//4客服下线
    EVENT_SHUNT_SUCCESSED(5,"5客服分流成功"),//5客服分流成功

    EVENT_QUEUE_REQUESET_BY_CUSTOMER(6,"6客户排队申请"),//6客户排队申请
    EVENT_INVITE_SESSION_REQUSET(7,"7主动邀约"),//7主动邀约
    EVENT_TRANSFER_SESSION_REQUSET(8,"8会话转接"),//8会话转接
    EVENT_CATCHER_SESSION_REQUSET(9,"9转接会话申请"),//9转接会话申请
    EVENT_GROUP_MEMBER_CHANGE(10,"10组内成员发生变化"),//10组内成员发生变化
    EVENT_CLEAR_ALL(11,"11清空redis"),//11清空redis，服务器重启会发送


    EVENT_QUEUE_REQUESET_SUCCESSED(12,"排队成功"),//排队成功
    EVENT_QUEUE_REQUESET_FAILED(13,"排队失败"),//排队失败

    EVENT_TRANSFER_SESSION_SUCCESSED(14,"会话转接成功"),//会话转接成功
    EVENT_TRANSFER_SESSION_FAILED(15,"会话转接失败"),//会话转接失败

    EVENT_INVITE_SESSION_SUCCESSED(16,"主动邀约成功"),//主动邀约成功
    EVENT_INVITE_SESSION_FAILED(17,"主动邀约失败"),//主动邀约失败

    EVENT_CATCHER_SESSION_SUCCESSED(18,"转接会话成功"),//转接会话成功
    EVENT_CATCHER_SESSION_FAILED(19,"转接会话失败");//转接会话失败;



    private int id;
    private String eventInfo;

    private static  final Map<Integer,ImServerEventTypeEnum> idToInfo = new HashMap<>(values().length);

    static {
        for (ImServerEventTypeEnum embedEnum : values()) {
            idToInfo.put(embedEnum.id, embedEnum);
        }
    }

    ImServerEventTypeEnum(int id, String eventInfo) {
        this.id = id;
        this.eventInfo= eventInfo;
    }

    public static final String getName(int id) {
        ImServerEventTypeEnum imServerEventTypeEnum = idToInfo.get(id);

        if (imServerEventTypeEnum == null) {
            return null;
        } else {
            return imServerEventTypeEnum.eventInfo;
        }
    }
    public int getId() {
        return id;
    }

    public String getEventInfo() {
        return eventInfo;
    }

    public static ImServerEventTypeEnum getImServerEventType(int id){
        return  idToInfo.get(id);
    }
}
