package com.feiniu.fnSubAccount.service;

import com.feiniu.fnSubAccount.domain.AfterSaleBoardConfig;
import com.feiniu.fnSubAccount.repository.AfterSaleBoardConfigDao;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.Map;

/**
 * Author: morningking
 * Date: 2015/11/24
 * Contact: 243717042@qq.com
 */
@Service
public class AfterSaleBoardService {
    @Autowired
    private AfterSaleBoardConfigDao afterSaleBoardConfigDao;
    @Autowired
    private CacheClientService cacheClientService;
    //首页 售后板块配置列表
    public static final String AFTER_SALE_BOARD_CONFIG_LIST = "AfterSaleBoardConfig#AllList";
    private static final Logger LOGGER = LoggerFactory.getLogger(AfterSaleBoardService.class);

    //获取设计时数据
    public List<AfterSaleBoardConfig> getDesignTimeData() {
        return afterSaleBoardConfigDao.getDesignTimeData();
    }

    //获取运行时数据
    public List<AfterSaleBoardConfig> getRuntimeData() {
        List<AfterSaleBoardConfig> afterSaleBoardConfigList =
                cacheClientService.getListValue(AFTER_SALE_BOARD_CONFIG_LIST, AfterSaleBoardConfig.class);

        if (afterSaleBoardConfigList == null) {
            afterSaleBoardConfigList = afterSaleBoardConfigDao.getRuntimeData();
            cacheClientService.put(AFTER_SALE_BOARD_CONFIG_LIST, afterSaleBoardConfigList);

            LOGGER.debug("Read AfterSaleBoardConfig from db");
        } else {
            LOGGER.debug("Read AfterSaleBoardConfig from cache");
        }

        return afterSaleBoardConfigList;
    }

    //编辑 售后版块 运行时数据
    public void update(AfterSaleBoardConfig afterSaleBoardConfig) {
        afterSaleBoardConfigDao.update(afterSaleBoardConfig);
    }

    //将设计时数据 同步至 运行时
    public void syncEditToRuntime() {
        List<AfterSaleBoardConfig> afterSaleBoardConfigList = afterSaleBoardConfigDao.syncEditToRuntime();
        cacheClientService.put(AFTER_SALE_BOARD_CONFIG_LIST, afterSaleBoardConfigList);
    }

    //判断是否分组被绑定过
    public boolean isGroupBinded(int groupID) {
        return afterSaleBoardConfigDao.isGroupBinded(groupID);
    }

    //判断各分组是否被订单绑定过
    public Map<Integer, Boolean> checkGroupsIfBind(List<Integer> groupIDList) {
        return afterSaleBoardConfigDao.checkGroupsIfBind(groupIDList);
    }
}
