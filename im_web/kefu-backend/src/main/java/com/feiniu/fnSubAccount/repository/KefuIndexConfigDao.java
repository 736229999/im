package com.feiniu.fnSubAccount.repository;

import com.feiniu.fnSubAccount.domain.KefuIndexConfig;

/**
 * Author: morningking
 * Date: 2015/10/14
 * Contact: 243717042@qq.com
 */
public interface KefuIndexConfigDao {
    //获取设计时数据
    KefuIndexConfig getDesignTimeData();

    //获取运行时数据
    KefuIndexConfig getRuntimeData();

    //编辑
    void update(KefuIndexConfig kefuIndexConfig);

    //更新底部
    void updateFooter(String footerJson);

    //将设计时数据 同步至 运行时
    KefuIndexConfig syncEditToRuntime();
}
