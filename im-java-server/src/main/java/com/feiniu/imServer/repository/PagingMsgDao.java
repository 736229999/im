package com.feiniu.imServer.repository;

import com.feiniu.imServer.bean.HistoryMessage;
import com.feiniu.imServer.bean.HistoryMessageListRequest;
import com.feiniu.imServer.bean.OfflineMessage;
import com.feiniu.imServer.bean.OfflineMessageRequest;

import java.util.List;

/**
 * Created by chenqing on 2016/1/28.
 */
public interface PagingMsgDao {

    public List<HistoryMessage> getHistoryPagingMessgeById(HistoryMessageListRequest historyMessageListRequest);
    public List<OfflineMessage> getOfflinePagingMessgeById(OfflineMessageRequest offlineMessageRequest);
    //根据本次上线时间得到上次离线时间
    public long getLastOfflineTimeById(Integer serviceId , Long onlineTime);

}
