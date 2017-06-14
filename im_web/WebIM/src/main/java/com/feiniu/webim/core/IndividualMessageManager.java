package com.feiniu.webim.core;

import com.feiniu.webim.bean.response.BaseResponseBean;

import java.util.List;

/**
 * Created by wangchen on 2015/1/6.
 * 对于每个买家 建立的 与每个卖家的会话，有一个单独的消息管理器与之对应
 */
public interface IndividualMessageManager {
    BaseResponseBean retrieveGeneralMessage();

    List<BaseResponseBean> retrieveAllChatMessage(String token);

    List<BaseResponseBean> retrieveAllHistoryMessage(String uuid);

    int putMessage(BaseResponseBean responseMessage);

    void destroy();
}
