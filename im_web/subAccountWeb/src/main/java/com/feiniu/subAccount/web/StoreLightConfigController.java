package com.feiniu.subAccount.web;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import com.feiniu.core.web.annotation.LoginRequired;
import com.feiniu.subAccount.base.ResponseResult;
import com.feiniu.subAccount.constants.SystemConfig;
import com.feiniu.subAccount.domain.ServiceGroup;
import com.feiniu.subAccount.domain.StoreLightConfig;
import com.feiniu.subAccount.service.ServiceGroupService;
import com.feiniu.subAccount.service.StoreLightConfigService;
import com.feiniu.subAccount.util.JsonUtil;

/**
 * Created by wangchen on 2015/2/10.
 */
@Controller
@RequestMapping(value = "store-light-config")
public class StoreLightConfigController {
    @Autowired
    private StoreLightConfigService storeLightConfigService;
    @Autowired
    private ServiceGroupService serviceGroupService;

    @RequestMapping(value = "update", method = RequestMethod.POST)
    @ResponseBody
    @LoginRequired
    public ResponseResult updateServiceGroupConfig(String serviceGroupListJson, StoreLightConfig storeLightConfig) {
        ResponseResult responseResult = new ResponseResult();

        storeLightConfig.setPhone(storeLightConfig.getPhone().replaceAll(" ", ""));
        storeLightConfig.setTelphone(storeLightConfig.getTelphone().replaceAll(" ", ""));

        List<ServiceGroup> serviceGroupList = JsonUtil.toList(serviceGroupListJson, ServiceGroup.class);
        if(storeLightConfig.getPhone() == "")
            storeLightConfig.setPhone(SystemConfig.EMPTY_PHONE);
        if(storeLightConfig.getTelphone() == "")
            storeLightConfig.setTelphone(SystemConfig.EMPTY_PHONE);
        storeLightConfigService.updateStoreLightConfig(serviceGroupList, storeLightConfig);

        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "get-all", method = RequestMethod.POST)
    @ResponseBody
    @LoginRequired
    public ResponseResult getAllServiceGroupConfig(long merchantID) {
        ResponseResult responseResult = new ResponseResult();

        StoreLightConfig result = storeLightConfigService.getStoreConfigUnderMerchant(merchantID);
        result.setPhone(result.getPhone().replaceAll("/ /", ""));
        result.setTelphone(result.getTelphone().replaceAll("/ /", ""));
        responseResult.addData("serviceGroupConfigList", result);
        responseResult.markSuccess();

        return responseResult;
    }
}
