package com.feiniu.subAccount.web;

import java.util.Arrays;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import com.feiniu.core.web.annotation.LoginRequired;
import com.feiniu.subAccount.base.ResponseResult;
import com.feiniu.subAccount.bean.request.ServiceAppraiseRequest;
import com.feiniu.subAccount.service.ServiceAppraiseService;
import com.feiniu.subAccount.util.ListUtils;

/**
 * Created by wangchen on 2015/2/12.
 */
@Controller
@RequestMapping(value = "service-appraise")
@LoginRequired
public class ServiceAppraiseController {
    @Autowired
    private ServiceAppraiseService serviceAppraiseService;

    @RequestMapping(value = "query", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getServiceAppraise(ServiceAppraiseRequest serviceAppraiseRequest) {
        ResponseResult responseResult = new ResponseResult();
        List<String> subAccountIDList = Arrays.asList(serviceAppraiseRequest.getSubAccountIDListStr().split(";"));

        serviceAppraiseRequest.setSubAccountIDList(
                ListUtils.transformElementToNumber(subAccountIDList, Long.class));

        responseResult.addData("serviceAppraiseStat", serviceAppraiseService.getServiceAppraiseList(serviceAppraiseRequest));
        responseResult.markSuccess();

        return responseResult;
    }
}
