package com.feiniu.fnSubAccount.repository;

import com.feiniu.fnSubAccount.domain.AfterSaleBoardConfig;

import java.util.List;
import java.util.Map;

/**
 * Author: morningking
 * Date: 2015/10/14
 * Contact: 243717042@qq.com
 */
public interface AfterSaleBoardConfigDao {
    //获取设计时数据
    List<AfterSaleBoardConfig> getDesignTimeData();

    //获取运行时数据
    List<AfterSaleBoardConfig> getRuntimeData();

    //编辑 售后版块 运行时数据
    void update(AfterSaleBoardConfig afterSaleBoardConfig);

    //将设计时数据 同步至 运行时
    List<AfterSaleBoardConfig> syncEditToRuntime();

    //判断是否分组被绑定过
    boolean isGroupBinded(int groupID);

    //判断各分组是否被订单绑定过
    Map<Integer, Boolean> checkGroupsIfBind(List<Integer> groupIDList);
}
