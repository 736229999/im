package com.feiniu.subAccount.component.repository;

import com.feiniu.subAccount.bean.request.ServiceAppraiseRequest;
import com.feiniu.subAccount.bean.response.ServiceAppraiseStat;
import com.feiniu.subAccount.domain.ServiceAppraise;

import java.util.List;

/**
 * Created by wangchen on 2015/2/12.
 */
public interface ServiceAppraiseManager {
    public List<ServiceAppraiseStat> getServiceAppraiseList(ServiceAppraiseRequest serviceAppraiseRequest);
}
