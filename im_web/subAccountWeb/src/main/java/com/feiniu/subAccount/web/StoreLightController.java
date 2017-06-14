package com.feiniu.subAccount.web;

import javax.servlet.http.HttpServletRequest;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import com.feiniu.core.web.annotation.LoginRequired;
import com.feiniu.subAccount.base.ResponseResult;
import com.feiniu.subAccount.service.StoreLightConfigService;

/**
 * Created by wangchen on 2015/2/11.
 */
@Controller
@RequestMapping(value = "store-light")
@LoginRequired
public class StoreLightController extends BaseController{
    @Autowired
    private StoreLightConfigService storeLightConfigService;

    @RequestMapping(value = "diagnose", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult diagnoseStoreLight(HttpServletRequest request) {
        ResponseResult responseResult = new ResponseResult();

        storeLightConfigService.diagnoseStoreLight(getAuth(request).getMerchantId());
        responseResult.markSuccess();
        return responseResult;
    }
}
