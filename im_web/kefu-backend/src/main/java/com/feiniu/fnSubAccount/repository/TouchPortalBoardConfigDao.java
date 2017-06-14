package com.feiniu.fnSubAccount.repository;

import com.feiniu.fnSubAccount.domain.TouchPortalBoardConfig;

import java.util.List;

/**
 * Author: morningking
 * Date: 2016/3/8
 * Contact: 243717042@qq.com
 */
public interface TouchPortalBoardConfigDao {
    void add(TouchPortalBoardConfig config);

    List<TouchPortalBoardConfig> getRuntimeBoardList();

    List<TouchPortalBoardConfig> getDesignTimeBoardList();

    void delete(int id);

    void modify(TouchPortalBoardConfig touchPortalBoardConfig);

    void changeOrder(int srcID, int destID);

    List<TouchPortalBoardConfig> syncEditToRuntime();
}
