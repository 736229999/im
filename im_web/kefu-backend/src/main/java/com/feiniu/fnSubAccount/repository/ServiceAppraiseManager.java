package com.feiniu.fnSubAccount.repository;


import com.feiniu.fnSubAccount.bean.ServiceAppraiseDetailStat;
import com.feiniu.fnSubAccount.bean.ServiceAppraiseStat;
import com.feiniu.fnSubAccount.bean.request.ServiceAppraiseRequest;

import java.util.List;

/**
 * Created by wangchen on 2015/2/12.
 */
public interface ServiceAppraiseManager {
    public List<ServiceAppraiseStat> getServiceAppraiseList(ServiceAppraiseRequest serviceAppraiseRequest);
    public List<ServiceAppraiseDetailStat> getServiceAppraiseList(long startTime,long endTime,String subMerchantId,int searchType,int startRow, int endRow);

}
