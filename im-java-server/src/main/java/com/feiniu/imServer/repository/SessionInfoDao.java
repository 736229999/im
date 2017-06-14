package com.feiniu.imServer.repository;

import com.feiniu.imServer.bean.Shunt.CurrentSessionItem;
import com.feiniu.imServer.bean.Shunt.SessionInfoItem;

import java.util.List;

/**
 * Created by wangzhen on 2016/4/7.
 */
public interface SessionInfoDao {
    void putSessionInfo(SessionInfoItem sessionInfoItem);
    void updateSessionInfoEndTime(SessionInfoItem sessionInfoItem);
    void transSessionInfo(SessionInfoItem sessionInfoItemRequest);
    List<CurrentSessionItem> getCurrentSessionItemList();
}
