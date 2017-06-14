package com.feiniu.webim.constants;

/**
 * Created by wangyixiong on 2015/12/17.
 */
public class ErrorCode {
    //客户端非法参数异常
    public static final int CLIENT_ILLEGAL_ARGUMENT_FAIL = 100;

	//返回正常时错误码为200
    public static final int ERRORCODE_NORMAL = 200;

    //imserver异常，响应超时暂时都用ERRORCODE_SERVER_FAIL，表示服务器响应超时或者返回错误数据
    public static final int ERRORCODE_SERVER_FAIL = 300;
    public static final int ERRORCODE_LOGIN_FAIL = 301;
    public static final int ERRORCODE_GET_MERCHANT_ONLINE_INFO_FAIL = 302;
    public static final int ERRORCODE_GET_MERCHANT_SUBACCOUNT_FAIL = 303;
    public static final int ERRORCODE_GET_RECENTLY_MOBILE_CONTACTS_FOR_BUYER_FAIL = 304;

    //webim异常
	//内部异常为ERRORCODE_CHAT_WEBIM_FAIL
    public static final int ERRORCODE_CHAT_WEBIM_FAIL = 400;
	//请求不合法
    public static final int ERRORCODE_NOT_LEGAL = 401;
	//对话不存在,需要刷新页面
    public static final int ERRORCODE_CHAT_SESSION_NOT_EXIST = 402;
	//发送消息超时
    public static final int ERRORCODE_SEND_TIME_OUT = 403;

    //外部接口异常
	//登录验证失败
    public static final int ERRORCODE_CHECK_LOGIN_FAIL = 500;
	//获取商家信息失败
    public static final int ERRORCODE_GET_STORE_INFO_FAIL = 501;
}
