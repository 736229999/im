package com.feiniu.webim.config;

/**
 * webim protocol
 * 如果枚举值与其名不同,可以带参
 *
 * @author zb
 */
public enum RequestParams {

    //--------------传递参数(目前为固定,不可更改)---------
    /**
     * event 请求类型
     */
    EVENT,
    /**
     * sid,发送者id
     */
    SID,
    /**
     * gid,接收id
     */
    DID,
    /**
     * text,消息内容
     */
    MSG,
    /**
     * feature, 消息样式
     */
    FEATURES,
    /**满意度评价**/
    VERSION,
    MSGNUM,
    TIME,
    /**
     * sname,发送者用户名
     */
    SNAME,
    /**
     * userid, 发送者id信息
     */
    USER_ID,
    /**
     * 消息类型
     */
    TYPE,
    ID,
    /**
     * 获取子账号时，相应的主账号id
     */
    PID,
    /**客服分组id，用于客服分组内分流*/
    GROUP_ID,
    //自营卖场id
    SM_SEQ,
    /**客户端发送的jsonp请求**/
    CALLBACK,


    /**商家id列表**/
    MERCHANTIDS,
    SUB_MERCHANT_IDS,
    MERCHANT_ID,
    SUB_MERCHANT_ID,
    PRODUCT_ID,
    USER_GID,
    USER_NAME,
    GROUP_IDS,

    /**飞牛员工id**/
    STAFF_ID
    ;

    /**
     * 重写toString()方法,
     * type==null，返回小写字符串<p>
     * type!=null,返回type;
     */
    @Override
    public String toString() {
        return this.name().toLowerCase();
    }

}
