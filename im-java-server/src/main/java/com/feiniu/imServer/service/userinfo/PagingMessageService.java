package com.feiniu.imServer.service.userinfo;

import com.feiniu.imServer.bean.*;
import com.feiniu.imServer.repository.PagingMsgDao;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.*;

/**
 * Created by chenqing on 2016/1/28.
 */
@Service
public class PagingMessageService {

    private static final Logger LOGGER = LoggerFactory.getLogger(PagingMessageService.class);
    @Autowired
    PagingMsgDao pagingMsgDao ;



    //历史消息记录分页查询
    public List<HistoryMessage> queryHistoryPagingMsg(HistoryMessageListRequest historyMessageListRequest){
        List<HistoryMessage> messageList;
        messageList = pagingMsgDao.getHistoryPagingMessgeById(historyMessageListRequest);
        return messageList;
    }

    //离线消息记录分页查询
    public  List<OfflineMessage> queryOfflinePagingMsg(OfflineMessageRequest offlineMessageRequest){
        return pagingMsgDao.getOfflinePagingMessgeById(offlineMessageRequest);
    }

    //根据本次上线时间得到上次离线时间
    public Long getLastOfflineTimeById(Integer serviceId , Long onlineTime){
        return pagingMsgDao.getLastOfflineTimeById(serviceId , onlineTime);
    }



}
