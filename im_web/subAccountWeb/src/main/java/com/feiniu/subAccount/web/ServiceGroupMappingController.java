package com.feiniu.subAccount.web;

import com.feiniu.core.web.annotation.LoginRequired;
import com.feiniu.subAccount.base.ResponseResult;
import com.feiniu.subAccount.constants.SystemConfig;
import com.feiniu.subAccount.domain.MappingForGroupServicePlace;
import com.feiniu.subAccount.domain.ServiceGroup;
import com.feiniu.subAccount.domain.ServiceGroupMapping;
import com.feiniu.subAccount.service.ServiceGroupMappingService;
import com.feiniu.subAccount.service.MappingGroupShowPlaceConfigService;
import com.feiniu.subAccount.service.external.MerchantService;
import com.feiniu.subAccount.util.JsonUtil;
import com.feiniu.subAccount.util.ListUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * Created by wangchen on 2015/2/9.
 */
@Controller
@RequestMapping(value = "service-group-mapping")
@LoginRequired
public class ServiceGroupMappingController {
    @Autowired
    private MerchantService merchantService;
    @Autowired
    private ServiceGroupMappingService serviceGroupMappingService;
    @Autowired
    private MappingGroupShowPlaceConfigService mappingGroupShowPlaceConfigService;

    @RequestMapping(value = "get-categorized-subaccounts", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getUncategorizedUnderMerchant(long merchantID) {
        ResponseResult responseResult = new ResponseResult();

        responseResult.addData("categorizedSubAccountList",
                serviceGroupMappingService.getCategorizedSubAccountList(merchantID));
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "get-subaccounts-under-group", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getSubAccountListUnderGroup(long groupID) {
        ResponseResult responseResult = new ResponseResult();

        responseResult.addData("subAccountMappingList", serviceGroupMappingService.getSubAccountListUnderGroup(groupID));
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "add-accounts-to-group", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult addSubAccountListToGroup(long groupID, String subAccountListString) {
        ResponseResult responseResult = new ResponseResult();

        List<Long> subAccountIDList =
                ListUtils.transformElementToNumber(Arrays.asList(subAccountListString.split(";")), Long.class);
        List<ServiceGroupMapping> serviceGroupMappingList = new ArrayList<ServiceGroupMapping>();
        for (Long subAccountID : subAccountIDList) {
            ServiceGroupMapping serviceGroupMapping = new ServiceGroupMapping();
            ServiceGroup serviceGroup = new ServiceGroup();
            serviceGroup.setGroupID(groupID);

            serviceGroupMapping.setServiceGroup(serviceGroup);
            serviceGroupMapping.setSubAccountID(subAccountID);

            serviceGroupMappingList.add(serviceGroupMapping);
        }

        serviceGroupMappingService.addSubAccountListToGroup(serviceGroupMappingList);

        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "remove-accounts-from-group", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult removeSubAccountListFromGroup(String subAccountListString, long groupID) {
        ResponseResult responseResult = new ResponseResult();
        List<Long> subAccountIDList =
                ListUtils.transformElementToNumber(
                        Arrays.asList(subAccountListString.split(SystemConfig.DEFAULT_SPLIT_SEPARATOR)), Long.class);

        serviceGroupMappingService.removeSubAccountListFromGroup(subAccountIDList);
        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "transfer-accounts-to-group", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult transferSubAccountListToGroup(String subAccountListString, long groupID) {
        ResponseResult responseResult = new ResponseResult();
        List<Long> subAccountIDList =
                ListUtils.transformElementToNumber(
                        Arrays.asList(subAccountListString.split(SystemConfig.DEFAULT_SPLIT_SEPARATOR)), Long.class);
        ServiceGroup serviceGroup = new ServiceGroup();
        serviceGroup.setGroupID(groupID);

        serviceGroupMappingService.transferSubAccountListToGroup(subAccountIDList, serviceGroup);

        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "update-diverse-state-for-account", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult updateDiverseStateForSubAccount(int subAccountID, char diverseState) {
        ResponseResult responseResult = new ResponseResult();

        ServiceGroupMapping serviceGroupMapping = new ServiceGroupMapping();
        serviceGroupMapping.setSubAccountID(subAccountID);
        serviceGroupMapping.setDiverseState(diverseState);

        serviceGroupMappingService.updateDiverseStateForSubAccount(serviceGroupMapping);
        responseResult.markSuccess();

        return responseResult;
    }

    /***
     * @param showPlaceJson
     * @param serviceGroupID 如果serviceGroupID不为空，那么先把serviceGroupID的映射全部干掉
     * @return
     */
    @RequestMapping(value = "update-show-place-mapping-for-service-group", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult updateMappingForServiceGroupShowPlace(String showPlaceJson, Long serviceGroupID) {
        ResponseResult responseResult = new ResponseResult();

        List<MappingForGroupServicePlace> mappingForGroupServicePlaceList = JsonUtil.toList(showPlaceJson, MappingForGroupServicePlace.class);
        mappingGroupShowPlaceConfigService.setGroupPlaceList(mappingForGroupServicePlaceList, serviceGroupID);

        responseResult.addData("updatedMapping", mappingForGroupServicePlaceList);
        responseResult.addData("serviceGroupID", serviceGroupID);
        responseResult.markSuccess();

        return responseResult;
    }
}
