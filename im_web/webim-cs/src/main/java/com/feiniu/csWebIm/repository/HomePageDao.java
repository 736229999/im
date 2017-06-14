package com.feiniu.csWebIm.repository;

import com.feiniu.csWebIm.bean.domain.AfterSaleBoardConfig;
import com.feiniu.csWebIm.bean.domain.KefuIndexConfig;

import java.util.List;

/**
 * Author: morningking
 * Date: 2015/11/24
 * Contact: 243717042@qq.com
 */
public interface HomePageDao {
    List<AfterSaleBoardConfig> getAfterSaleBoardConfigList();

    KefuIndexConfig getIndexConfig();
}
