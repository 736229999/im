package com.feiniu.fnSubAccount.service;

import com.feiniu.fnSubAccount.domain.TouchPortalBoardConfig;
import com.feiniu.fnSubAccount.repository.TouchPortalBoardConfigDao;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

/**
 * Author: morningking
 * Date: 2016/3/8
 * Contact: 243717042@qq.com
 */
@Service
@Transactional
public class TouchPortalBoardConfigService {
    @Autowired
    private CacheClientService cacheClientService;
    @Autowired
    private TouchPortalBoardConfigDao touchPortalBoardConfigDao;

    private static final String GET_ALL_TOUCH_BOARD_LIST_KEY = "TouchPortalBoardConfig#allList";
    private static final Logger LOGGER = LoggerFactory.getLogger(TouchPortalBoardConfigService.class);

    public void add(TouchPortalBoardConfig config) {
        touchPortalBoardConfigDao.add(config);
    }

    public List<TouchPortalBoardConfig> getBoardList() {
        List<TouchPortalBoardConfig> configList =
                cacheClientService.getListValue(GET_ALL_TOUCH_BOARD_LIST_KEY, TouchPortalBoardConfig.class);

        if (configList == null) {
            configList = touchPortalBoardConfigDao.getRuntimeBoardList();
            cacheClientService.put(GET_ALL_TOUCH_BOARD_LIST_KEY, configList);

            LOGGER.debug("read touch-portal-board-list from db");
        } else {
            LOGGER.debug("read touch-portal-board-list from cache");
        }

        return configList;
    }

    public void delete(int id) {
        touchPortalBoardConfigDao.delete(id);
    }

    public void modify(TouchPortalBoardConfig touchPortalBoardConfig) {
        touchPortalBoardConfigDao.modify(touchPortalBoardConfig);
    }

    public void changeOrder(int srcID, int destID) {
        touchPortalBoardConfigDao.changeOrder(srcID, destID);
    }

    //获取设计时数据
    public List<TouchPortalBoardConfig> getDesignTimeData() {
        return touchPortalBoardConfigDao.getDesignTimeBoardList();
    }

    //获取运行时数据
    public List<TouchPortalBoardConfig> getRuntimeData() {
        List<TouchPortalBoardConfig> touchPortalBoardConfigList =
                cacheClientService.getListValue(GET_ALL_TOUCH_BOARD_LIST_KEY, TouchPortalBoardConfig.class);

        if (touchPortalBoardConfigList == null) {
            touchPortalBoardConfigList = touchPortalBoardConfigDao.getRuntimeBoardList();
            cacheClientService.put(GET_ALL_TOUCH_BOARD_LIST_KEY, touchPortalBoardConfigList);

            LOGGER.debug("Read TouchPortalBoardConfig from db");
        } else {
            LOGGER.debug("Read TouchPortalBoardConfig from cache");
        }

        return touchPortalBoardConfigList;
    }

    //将设计时数据 同步至 运行时
    public void syncEditToRuntime() {
        List<TouchPortalBoardConfig> touchPortalBoardConfigList = touchPortalBoardConfigDao.syncEditToRuntime();
        cacheClientService.put(GET_ALL_TOUCH_BOARD_LIST_KEY, touchPortalBoardConfigList);
    }
}
