package com.feiniu.subAccount.web;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import com.feiniu.core.web.annotation.LoginRequired;
import com.feiniu.subAccount.base.ResponseResult;
import com.feiniu.subAccount.bean.request.DiverseConfigRequest;
import com.feiniu.subAccount.domain.MerchantDiverseConfig;
import com.feiniu.subAccount.service.MerchantDiverseConfigService;

/**
 * Created by wangchen on 2015/2/7.
 */
@Controller
@RequestMapping(value = "diver-config")
@LoginRequired
public class DiverseConfigController {
    @Autowired
    private MerchantDiverseConfigService merchantDiverseConfigService;

    @RequestMapping(value = "save-config", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult saveDiverseConfig(DiverseConfigRequest diverseConfigRequest) {
        ResponseResult responseResult = new ResponseResult();
        MerchantDiverseConfig merchantDiverseConfig = new MerchantDiverseConfig();

        merchantDiverseConfigService.setDiverseConfigForMerchant(diverseConfigRequest);
        responseResult.markSuccess();
        return responseResult;
    }
}
