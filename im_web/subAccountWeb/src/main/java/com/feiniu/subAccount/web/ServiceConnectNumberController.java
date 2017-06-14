package com.feiniu.subAccount.web;

import com.feiniu.subAccount.base.ResponseResult;
import com.feiniu.subAccount.domain.MerchantConnectInfo;
import com.feiniu.subAccount.service.PhoneNumberMemcacheService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by wangzhen on 2015/11/26.
 */
@Controller
@RequestMapping(value = "searchConnectNumber")
public class ServiceConnectNumberController {


    @Autowired
    PhoneNumberMemcacheService phoneNumberMemcacheService;


    @RequestMapping(value = "query-batch")
    @ResponseBody
    public ResponseResult queryConnectNumbers(String merchantIDs) {
        ResponseResult responseResult = new ResponseResult();
        Map<Long,MerchantConnectInfo> merchantConnectInfoMap = new HashMap<>();

        if (merchantIDs == null || merchantIDs.isEmpty()){
            responseResult.setMessage("id不能为空");
            return responseResult;
        }
        String[] merchantIdList = merchantIDs.split(",");

        for (String merchantId : merchantIdList){
            Long merchantIdLong = Long.valueOf(merchantId);

            MerchantConnectInfo  merchantConnectInfo =
                    phoneNumberMemcacheService.doGetConnectNumber(merchantIdLong);

            merchantConnectInfoMap.put(merchantIdLong,merchantConnectInfo);
        }
        responseResult.markSuccess();
        responseResult.addData("phoneInfoMap", merchantConnectInfoMap);
        return responseResult;
    }

    @RequestMapping(value = "query")
    @ResponseBody
    public ResponseResult queryConnectNumber(Long merchantID) {
        ResponseResult responseResult = new ResponseResult();

        //参数错误
        if (merchantID == null ){
            responseResult.setMessage("id不能为空");
            return responseResult;
        }

        MerchantConnectInfo  merchantConnectInfo =
                phoneNumberMemcacheService.doGetConnectNumber(merchantID);

        //memcache里面有，直接返回
        responseResult.markSuccess();
        responseResult.addData("phoneInfo",merchantConnectInfo);
        return responseResult;
    }
}
