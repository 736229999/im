package com.feiniu.subAccount.web;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import com.feiniu.core.web.annotation.LoginRequired;
import com.feiniu.subAccount.bean.request.ServiceAppraiseRequest;
import com.feiniu.subAccount.bean.response.StoreInfo;
import com.feiniu.subAccount.bean.response.SubMerchantInfo;
import com.feiniu.subAccount.constants.SystemConfig;
import com.feiniu.subAccount.domain.DiverseType;
import com.feiniu.subAccount.domain.MerchantDiverseConfig;
import com.feiniu.subAccount.domain.ServiceGroup;
import com.feiniu.subAccount.domain.StoreLightConfig;
import com.feiniu.subAccount.service.GuideConfigService;
import com.feiniu.subAccount.service.MappingGroupShowPlaceConfigService;
import com.feiniu.subAccount.service.MerchantDiverseConfigService;
import com.feiniu.subAccount.service.ServiceAppraiseService;
import com.feiniu.subAccount.service.ServiceGroupService;
import com.feiniu.subAccount.service.StoreLightConfigService;
import com.feiniu.subAccount.service.external.MerchantService;
import com.feiniu.subAccount.service.external.StoreService;

/**
 * Created by wangchen on 2015/1/8.
 */
@Controller
@RequestMapping
@LoginRequired
public class ViewController extends BaseController {
    @Autowired
    private MerchantService merchantService;
    @Autowired
    private StoreService storeService;
    @Autowired
    private MerchantDiverseConfigService merchantDiverseConfigService;
    @Autowired
    private ServiceGroupService serviceGroupService;
    @Autowired
    private StoreLightConfigService storeLightConfigService;
    @Autowired
    private MappingGroupShowPlaceConfigService mappingGroupShowPlaceConfigService;
    @Autowired
    private ServiceAppraiseService serviceAppraiseService;
    @Autowired
    private GuideConfigService guideConfigService;

    @RequestMapping(value = "monitor/chat-history-list", method = RequestMethod.GET)
    public ModelAndView getMonitorCharHistoryList(HttpServletRequest request) {
        String merchantID = getAuth(request).getMerchantId() + "";

        List<SubMerchantInfo> subMerchantInfoList = merchantService.retrieveSubMerchantInfo(merchantID);
        StoreInfo storeInfo = storeService.retrieveStoreInfo(merchantID);

        ModelAndView modelAndView = new ModelAndView("monitor/chat-history-list");
        Map<String, Object> modelMap = modelAndView.getModel();
        modelMap.put("subMerchantInfoList", subMerchantInfoList);
        modelMap.put("storeInfo", storeInfo);

        return modelAndView;
    }

    @RequestMapping(value = "monitor/service-appraise", method = RequestMethod.GET)
    public ModelAndView getMonitorServerAppraise(HttpServletRequest request) {
        String merchantID = getAuth(request).getMerchantId() + "";

        ModelAndView modelAndView = new ModelAndView("monitor/service-appraise");
        List<SubMerchantInfo> subMerchantInfoList = merchantService.retrieveSubMerchantInfo(merchantID);
        StoreInfo storeInfo = storeService.retrieveStoreInfo(merchantID);
        List<Long> subAccountIDList = new ArrayList<Long>();
        for (SubMerchantInfo subMerchantInfo : subMerchantInfoList) {
            subAccountIDList.add(subMerchantInfo.getMemberId());
        }

        ServiceAppraiseRequest serviceAppraiseRequest = new ServiceAppraiseRequest();
        serviceAppraiseRequest.setSubAccountIDList(subAccountIDList);
        serviceAppraiseRequest.setEndTime((int) (System.currentTimeMillis() / 1000));
        Calendar calendar = Calendar.getInstance();
        calendar.setTimeInMillis(System.currentTimeMillis());
        calendar.add(Calendar.MONTH, -3);
        serviceAppraiseRequest.setStartTime((int) (calendar.getTimeInMillis() / 1000));

        modelAndView.addObject("subMerchantInfoList", subMerchantInfoList);
        modelAndView.addObject("storeInfo", storeInfo);
        modelAndView.addObject("serviceAppraiseList", serviceAppraiseService.getServiceAppraiseList(serviceAppraiseRequest));

        return modelAndView;
    }

    @RequestMapping(value = "diverse/diverse-config", method = RequestMethod.GET)
    public ModelAndView getDiverseConfig(HttpServletRequest request) {
        String merchantID = getAuth(request).getMerchantId() + "";

        List<SubMerchantInfo> subMerchantInfoList = merchantService.retrieveSubMerchantInfo(merchantID);
        StoreInfo storeInfo = storeService.retrieveStoreInfo(merchantID);

        List<DiverseType> diverseTypeList = merchantDiverseConfigService.getDiverseTypeList();
        MerchantDiverseConfig merchantDiverseConfig = merchantDiverseConfigService.getDiverseConfigForMerchant(Long.parseLong(merchantID));

        boolean hasGuided = guideConfigService.hasMerchantIDBeingGuided(Long.parseLong(merchantID));

        ModelAndView modelAndView = new ModelAndView("diverse/diverse-config");
        Map<String, Object> modelMap = modelAndView.getModel();
        modelMap.put("merchantID", merchantID);
        modelMap.put("subMerchantInfoList", subMerchantInfoList);
        modelMap.put("storeInfo", storeInfo);
        modelMap.put("diverseTypeList", diverseTypeList);
        modelMap.put("currentMerchantDiverseConfig", merchantDiverseConfig);
        modelMap.put("hasGuided", hasGuided);

        guideConfigService.updateGuideStatus(Long.parseLong(merchantID));

        return modelAndView;
    }

    @RequestMapping(value = "service-group/service-group-config", method = RequestMethod.GET)
    public ModelAndView getServiceGroupConfig(HttpServletRequest request) {
        ModelAndView modelAndView = new ModelAndView("service-group/service-group-config");

        String merchantID = getAuth(request).getMerchantId() + "";

        List<SubMerchantInfo> subMerchantInfoList = merchantService.retrieveSubMerchantInfo(merchantID);
        StoreInfo storeInfo = storeService.retrieveStoreInfo(merchantID);

        modelAndView.addObject("merchantID", merchantID);
        modelAndView.addObject("subMerchantInfo", subMerchantInfoList);
        modelAndView.addObject("storeInfo", storeInfo);
        modelAndView.addObject("serviceGroupList", serviceGroupService.getServiceGroupsUnderMerchant(Integer.parseInt(merchantID)));
        modelAndView.addObject("serviceGroupShowPlaceList",
                mappingGroupShowPlaceConfigService.getAllMappingUnderMerchant(Long.parseLong(merchantID)));
        modelAndView.addObject("allShowPlace", mappingGroupShowPlaceConfigService.getAllShowPlace());

        return modelAndView;
    }

    @RequestMapping(value = "store-light/store-light-config", method = RequestMethod.GET)
    public ModelAndView getStoreLightConfig(HttpServletRequest request) {
        ModelAndView modelAndView = new ModelAndView("store-light/store-light-config");
        long merchantID = getAuth(request).getMerchantId();

        List<ServiceGroup> serviceGroupList = serviceGroupService.getServiceGroupsUnderMerchant(merchantID);
        StoreLightConfig storeLightConfig = storeLightConfigService.getStoreConfigUnderMerchant(merchantID);

        if (storeLightConfig != null) {
            storeLightConfig.setPhone(storeLightConfig.getPhone().replaceAll(" ", ""));
            storeLightConfig.setTelphone(storeLightConfig.getTelphone().replaceAll(" ", ""));
            if (SystemConfig.EMPTY_PHONE.equals(storeLightConfig.getPhone()))
                storeLightConfig.setPhone("");
            if (SystemConfig.EMPTY_PHONE.equals(storeLightConfig.getTelphone()))
                storeLightConfig.setTelphone("");
        }else {
            storeLightConfig = new StoreLightConfig();
            storeLightConfig.setPhone("");
            storeLightConfig.setTelphone("");
        }

        modelAndView.addObject("merchantID", merchantID);
        modelAndView.addObject("serviceGroupList", serviceGroupList);
        modelAndView.addObject("serviceGroupConfig", storeLightConfig);

        return modelAndView;
    }

    @RequestMapping(value = "store-light/store-light-diagnose", method = RequestMethod.GET)
    public ModelAndView getStoreLightDiagnose(HttpServletRequest request) {
        ModelAndView modelAndView = new ModelAndView("store-light/store-light-diagnose");
        long merchantID = getAuth(request).getMerchantId();

        modelAndView.addObject("merchantID", merchantID);
        StoreLightConfig storeLightConfig = storeLightConfigService.getStoreConfigUnderMerchant(merchantID);
        modelAndView.addObject("storeLightConfig", storeLightConfig);
        StoreInfo storeInfo = storeService.retrieveStoreInfo(merchantID + "");
        modelAndView.addObject("storeInfo", storeInfo);
        List<ServiceGroup> serviceGroupListShowOnStore = serviceGroupService.getServiceGroupShowOnStore(merchantID);
        modelAndView.addObject("serviceGroupListShowOnStore", serviceGroupListShowOnStore);

        return modelAndView;
    }

    @RequestMapping(value = "attendance/attendance")
    public ModelAndView getAttendance(HttpServletRequest request) {
        ModelAndView modelAndView = new ModelAndView("attendance/attendance");
        //在当前页面保存mechantID和所有的子账号ID
        long merchantID = getAuth(request).getMerchantId();
        modelAndView.addObject("merchantID", merchantID);

        List<SubMerchantInfo> subMerchantInfoList = merchantService.retrieveSubMerchantInfo(String.valueOf(merchantID));
        modelAndView.addObject("subMerchantInfoList", subMerchantInfoList);

        return modelAndView;
    }

    @RequestMapping(value = "checkSrv")
    public ModelAndView getCheckSrv(HttpServletRequest request) {
        ModelAndView modelAndView = new ModelAndView("checkSrv/checkSrv");

        return modelAndView;
    }
}
