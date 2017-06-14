package com.feiniu.fnSubAccount.service;

import com.feiniu.fnSubAccount.domain.KefuIndexConfig;
import com.feiniu.fnSubAccount.repository.KefuIndexConfigDao;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

/**
 * Author: morningking
 * Date: 2015/11/24
 * Contact: 243717042@qq.com
 */
@Service
public class KefuIndexConfigService {
    @Autowired
    private CacheClientService cacheClientService;
    @Autowired
    private KefuIndexConfigDao kefuIndexConfigDao;
    //首页 其他相关配置
    public static final String KEFU_INDEX_CONFIG = "KefuIndexConfig#Singleton";
    private static final Logger LOGGER = LoggerFactory.getLogger(KefuIndexConfigService.class);

    //获取设计时数据
    public KefuIndexConfig getDesignTimeData() {
        return kefuIndexConfigDao.getDesignTimeData();
    }

    //获取运行时数据
    public KefuIndexConfig getRuntimeData() {
        KefuIndexConfig kefuIndexConfig = cacheClientService.getValue(KEFU_INDEX_CONFIG, KefuIndexConfig.class);

        if (kefuIndexConfig == null) {
            kefuIndexConfig = kefuIndexConfigDao.getRuntimeData();
            cacheClientService.put(KEFU_INDEX_CONFIG, kefuIndexConfig);

            LOGGER.debug("Read kefuIndexConfig from db");
        } else {
            LOGGER.debug("Read kefuIndexConfig from cache");
        }

        return kefuIndexConfig;
    }

    //编辑
    public void update(KefuIndexConfig kefuIndexConfig) {
        kefuIndexConfigDao.update(kefuIndexConfig);
    }

    //更新底部
    public void updateFooter(String footerJson) {
        kefuIndexConfigDao.updateFooter(footerJson);
    }

    //将设计时数据 同步至 运行时
    public void syncEditToRuntime() {
        KefuIndexConfig runtimeConfig = kefuIndexConfigDao.syncEditToRuntime();
        cacheClientService.put(KEFU_INDEX_CONFIG, runtimeConfig);
    }

}
