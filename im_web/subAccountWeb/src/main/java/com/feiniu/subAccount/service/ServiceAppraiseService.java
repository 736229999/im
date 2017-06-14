package com.feiniu.subAccount.service;

import com.feiniu.subAccount.bean.request.ServiceAppraiseRequest;
import com.feiniu.subAccount.bean.response.ServiceAppraiseStat;
import com.feiniu.subAccount.component.repository.ServiceAppraiseManager;
import com.feiniu.subAccount.domain.ServiceAppraise;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

/**
 * Created by wangchen on 2015/2/12.
 */
@Service
@Transactional
public class ServiceAppraiseService {
    @Autowired
    private ServiceAppraiseManager serviceAppraiseManager;

    public List<ServiceAppraiseStat> getServiceAppraiseList(ServiceAppraiseRequest serviceAppraiseRequest) {
        return serviceAppraiseManager.getServiceAppraiseList(serviceAppraiseRequest);
    }
}
