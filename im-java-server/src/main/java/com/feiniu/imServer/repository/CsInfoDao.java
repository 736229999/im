package com.feiniu.imServer.repository;

import com.feiniu.imServer.bean.CSInfo;

import java.util.List;
import java.util.Map;

/**
 * Created by wangzhen on 2016/1/6.
 */
public interface CsInfoDao {
    List<String> getAllCsIdInGroup(String groupid);

    Map<Long,Integer> getAllUserIdToCsIdMap();

    List<String> getAllGroups();

    List<Long> getAllGroupsOfCsId(String csId);

    Integer getReceptionLimit(String csid);

    CSInfo getCSInfoById(Long userId);

    List<Long> getCSGroupCountList(Long groupId);
}
