package com.feiniu.fnSubAccount.controller;

import com.feiniu.fnSubAccount.base.ResponseResult;
import com.feiniu.fnSubAccount.bean.LoginOut.LoginOutInfo;
import com.feiniu.fnSubAccount.bean.SubMerchantInfo;
import com.feiniu.fnSubAccount.bean.request.MerchantLogInOutRequest;
import com.feiniu.fnSubAccount.domain.MerchantLogInOutInfo;
import com.feiniu.fnSubAccount.service.MonitorService;
import com.feiniu.fnSubAccount.service.external.MerchantService;
import com.feiniu.fnSubAccount.util.FormatUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by zhangmin on 2016/4/12.
 */
@Controller
@RequestMapping(value = "merchantLogInOut")
public class MerchantLogInOutController {

    @Autowired
    MonitorService monitorService;
    @Autowired
    MerchantService merchantService;

    @RequestMapping(value = "detail")
    @ResponseBody
    public ResponseResult LogInOutDetail(MerchantLogInOutRequest merchantLogInOutRequest) {
        ResponseResult responseResult = new ResponseResult();
        List<Long> listUserId = new ArrayList<>();
        if (merchantLogInOutRequest.getSubMerchantId() == Long.valueOf(0)){
            //获取商家所有子帐号
            List<SubMerchantInfo> subMerchantInfoList = merchantService.retrieveSubMerchantListInfos(
                    String.valueOf(merchantLogInOutRequest.getMerchantId()));

            for(SubMerchantInfo subMerchantInfo:subMerchantInfoList)
            {
                listUserId.add(subMerchantInfo.getMemberId());
            }
        }else
            listUserId.add(merchantLogInOutRequest.getSubMerchantId());

        Integer logInOutCount = monitorService.getMerchantLogInOutCount(merchantLogInOutRequest.getStartTime()
                ,merchantLogInOutRequest.getEndTime(),listUserId);

        List<MerchantLogInOutInfo> logInOutInfoList =
                monitorService.getMerchantLogInOut(merchantLogInOutRequest.getStartTime()
                        ,merchantLogInOutRequest.getEndTime(),merchantLogInOutRequest.getCurrentPageNo(),
                        merchantLogInOutRequest.getPageSize(),listUserId);

        List<LoginOutInfo> loginOutInfos = new ArrayList<>(logInOutInfoList.size());

        for (MerchantLogInOutInfo merchantLogInOutInfo : logInOutInfoList){
            LoginOutInfo loginOutInfo = new LoginOutInfo();
            loginOutInfo.setLogInOutTime(
                    FormatUtils.convertNumberToDateAndTime(merchantLogInOutInfo.getLogInOutTime().longValue()));
            loginOutInfo.setLogInOutType(merchantLogInOutInfo.getLogInOutType());
            loginOutInfo.setUserName(merchantLogInOutInfo.getUserName());
            loginOutInfos.add(loginOutInfo);
        }

        responseResult.addData("data", loginOutInfos);
        responseResult.addData("count",logInOutCount);
        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "get-submerchant-info-pair")
    @ResponseBody
    public ResponseResult getSubMerchantInfoPair(Long merchantId)
    {
        ResponseResult responseResult = new ResponseResult();
        if (merchantId == null){
            responseResult.setMessage("参数不能为空");
            return responseResult;
        }
        List<SubMerchantInfo> subMerchantInfoList =  merchantService.retrieveSubMerchantInfo(merchantId.toString());

        if (subMerchantInfoList== null || subMerchantInfoList.size() == 0){
            responseResult.setMessage("改商家没有客服");
            return responseResult;
        }
        responseResult.addData("data", subMerchantInfoList);
        responseResult.markSuccess();
        return responseResult;
    }


}
