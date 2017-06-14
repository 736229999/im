package com.feiniu.csWebIm.bean.domain;

import java.util.List;

/**
 * Created by wangzhen on 2015/11/5.
 */
public class PackageDetail {
    private List<GoodPicInfo> goodPicInfoList;//图片
//    private List<LogisticsDetail> logisticsDetailList;
    private LogisticsDetail logisticsDetailList;//物流

    public List<GoodPicInfo> getGoodPicInfoList() {
        return goodPicInfoList;
    }

    public void setGoodPicInfoList(List<GoodPicInfo> goodPicInfoList) {
        this.goodPicInfoList = goodPicInfoList;
    }

    public LogisticsDetail getLogisticsDetailList() {
        return logisticsDetailList;
    }

    public void setLogisticsDetailList(LogisticsDetail logisticsDetailList) {
        this.logisticsDetailList = logisticsDetailList;
    }
}
