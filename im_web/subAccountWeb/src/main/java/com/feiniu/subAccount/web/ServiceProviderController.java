package com.feiniu.subAccount.web;

import com.feiniu.subAccount.base.ResponseResult;
import com.feiniu.subAccount.bean.response.SubMerchantInfo;
import com.feiniu.subAccount.constants.SystemConfig;
import com.feiniu.subAccount.domain.MerchantDiverseConfig;
import com.feiniu.subAccount.service.MerchantDiverseConfigService;
import com.feiniu.subAccount.service.ServiceGroupMappingService;
import com.feiniu.subAccount.service.ServiceGroupService;
import com.feiniu.subAccount.service.StoreLightConfigService;
import com.feiniu.subAccount.service.external.MerchantService;
import com.feiniu.subAccount.util.StringUtils;
import org.apache.commons.collections4.CollectionUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by wangchen on 2015/2/26.
 * 主要是为其他系统提供服务接口
 */
@Controller
@RequestMapping(value = "service-provider")
public class ServiceProviderController {
    @Autowired
    private StoreLightConfigService storeLightConfigService;
    @Autowired
    private ServiceGroupService serviceGroupService;
    @Autowired
    private ServiceGroupMappingService serviceGroupMappingService;
    @Autowired
    private MerchantDiverseConfigService merchantDiverseConfigService;
    @Autowired
    private MerchantService merchantService;

    @RequestMapping(value = "retrieve-config-for-webim")
    @ResponseBody
    public ResponseResult retrieveStoreLightConfigForWebIM(long merchantID) {
        ResponseResult responseResult = new ResponseResult();

        responseResult.addData("generalConfig", storeLightConfigService.getStoreConfigUnderMerchant(merchantID));
        responseResult.addData("serviceGroupConfig", serviceGroupService.getServiceGroupShowOnStore(merchantID));

        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "retrieve-subaccounts-under-group")
    @ResponseBody
    public ResponseResult retrieveSubAccountsUnderServiceGroup(String groupIDListJoinString) {
        ResponseResult responseResult = new ResponseResult();
        List<Long> groupIDList = StringUtils.stringToLongList(groupIDListJoinString, SystemConfig.DEFAULT_SPLIT_SEPARATOR);

        responseResult.addData("subAccountAndGroupMap", serviceGroupMappingService.getSubaccountsUnderGroups(groupIDList));
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "retrieve-proxy-subaccount-for-merchant")
    @ResponseBody
    public ResponseResult retrieveProxySubAccountForMerchant(long merchantID) {
        ResponseResult responseResult = new ResponseResult();

        MerchantDiverseConfig merchantDiverseConfig = merchantDiverseConfigService.getDiverseConfigForMerchant(merchantID);
        long subAccountID = -1l;

        if (merchantDiverseConfig != null) {
            subAccountID = merchantDiverseConfig.getProxySubAccountID();
        } else {
            List<SubMerchantInfo> subMerchantInfoList = merchantService.retrieveSubMerchantInfo(merchantID + "");

            if (CollectionUtils.isNotEmpty(subMerchantInfoList)) {
                for (SubMerchantInfo subMerchantInfo : subMerchantInfoList) {
                    if (subMerchantInfo.isMainAccount()) {
                        subAccountID = subMerchantInfo.getMemberId();
                        break;
                    }
                }
            }
        }

        if (subAccountID == -1) {
            responseResult.setMessage("商家账号下面不存在任何子账号");
        } else {
            responseResult.addData("proxySubAccountID", subAccountID);
            responseResult.markSuccess();
        }

        return responseResult;
    }

}
