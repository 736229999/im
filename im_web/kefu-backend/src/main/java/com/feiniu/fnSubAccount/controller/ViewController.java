package com.feiniu.fnSubAccount.controller;

import com.feiniu.common.core.SystemEnv;
import com.feiniu.fnSubAccount.base.ResponseResult;
import com.feiniu.fnSubAccount.bean.AppraiseStat;
import com.feiniu.fnSubAccount.bean.CSBackendInfo;
import com.feiniu.fnSubAccount.bean.OrganNameInfo;
import com.feiniu.fnSubAccount.bean.SubMerchantInfo;
import com.feiniu.fnSubAccount.bean.request.ServiceAppraiseRequest;
import com.feiniu.fnSubAccount.bean.vo.WorkInstallerConfig;
import com.feiniu.fnSubAccount.bean.vo.PCInstallerConfig;
import com.feiniu.fnSubAccount.domain.FAQ;
import com.feiniu.fnSubAccount.service.*;
import com.feiniu.fnSubAccount.service.external.BackendService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;
import java.util.Map;

/**
 * Created by wangchen on 2015/4/15.
 */
@Controller
public class ViewController {
    @Autowired
    private DirectorService directorService;

    @Autowired
    private ServiceAppraiseService serviceAppraiseService;
    @Autowired
    FAQService faqService;
    @Autowired
    private GroupInfoService groupInfoService;
    @Autowired
    private BackendService backendService;
    @Autowired
    private PCInstallerConfig pcInstallerConfig;
    @Autowired
    private WorkInstallerConfig workInstallerConfig;
    @Autowired
    private CSAccountService csAccountService;

    @RequestMapping(value = "kefu/history-message-list")
    public ModelAndView historyMessageListPage(HttpServletRequest request) {
        ModelAndView modelAndView = new ModelAndView("monitor/history-message-list");

        long dirctorID = SystemEnv.getCurrentUser().getUserId();
        modelAndView.addObject("dirctorID", dirctorID);

        return modelAndView;
    }

    @RequestMapping(value = "monitor/work-statistics")
    public String historyWorkStatistics(ModelMap modelMap) {
//        modelMap.put("historyMessageList", monitorService.getHistoryMessageList(new HistoryMessageListRequest()));

        return "monitor/work-statistics";
    }

    @RequestMapping(value = "kefu/attendance")
    public ModelAndView getAttendance(HttpServletRequest request) {

        long directorID = SystemEnv.getCurrentUser().getUserId();

        ModelAndView modelAndView = new ModelAndView("attendance/attendance");
        //在当前页面保存mechantID和所有的子账号ID
        modelAndView.addObject("directorID", directorID);

        return modelAndView;
    }

    @RequestMapping(value = "kefu/workload")
    public ModelAndView getWorkload(HttpServletRequest request) {
        long directorID = SystemEnv.getCurrentUser().getUserId();

        ModelAndView modelAndView = new ModelAndView("attendance/workload");
        //在当前页面保存mechantID和所有的子账号ID
        modelAndView.addObject("directorID", directorID);

        return modelAndView;
    }


    @RequestMapping(value = "checksrv")
    public String getCheckSrv(ModelMap modelMap) {

        return "checkSrv/checkSrv";
    }

    @RequestMapping(value = "kefu/service-appraise", method = RequestMethod.GET)
    public ModelAndView getMonitorServerAppraise(HttpServletRequest request) {

        ModelAndView modelAndView = new ModelAndView("monitor/service-appraise");
        List<SubMerchantInfo> subMerchantInfoList = directorService.getSubMerchantInfoListByDirectorId();

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
        Map<Long, AppraiseStat> idToAppraiseStatMap = serviceAppraiseService.getServiceAppraiseList(serviceAppraiseRequest);
        modelAndView.addObject("serviceAppraiseList", new ArrayList<AppraiseStat>(idToAppraiseStatMap.values()));

        return modelAndView;
    }

    @RequestMapping(value = "kefu/cs-account-manager")
    public ModelAndView csAccountManager(ModelMap modelMap) {
        ModelAndView modelAndView = new ModelAndView("monitor/cs-account-manager");

        List<OrganNameInfo> serviceGroupDetailList = backendService.organNameList();
        List<CSBackendInfo> allMemberList = backendService.getAllMemberList();
        for (CSBackendInfo csBackendInfo : allMemberList) {
            for (OrganNameInfo indexGrop : serviceGroupDetailList) {
                if (indexGrop.getId().equals(String.valueOf(csBackendInfo.getOrganid()))) {
                    csBackendInfo.setOrganName(indexGrop.getOrgnName());
                    break;
                }
            }
        }
        modelAndView.addObject("serviceGroupDetailList", serviceGroupDetailList);
        modelAndView.addObject("allMemberList", allMemberList);

        return modelAndView;
    }

    @RequestMapping(value = "kefu/service-group-setting")
    public String getduyong(ModelMap modelMap) {
        modelMap.put("groupList", groupInfoService.getAllGroup());

        modelMap.put("embedSet", groupInfoService.getEmbedRelatedGroupInfo());

        return "monitor/service-group-setting";
    }

    @RequestMapping(value = "kefu/cs-monitor")
    public String getmonitor(ModelMap modelMap) {

        return "CS-monitor/cs-monitor";
    }

    @RequestMapping(value = "get-faq-list")
    @ResponseBody
    public ResponseResult getFaqList() {
        ResponseResult responseResult = new ResponseResult();

        List<FAQ> faqList = faqService.getFAQListForMoumou();

        responseResult.addData("faqList", faqList);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "pc-installer")
    public String getPCInstaller(ModelMap modelMap) {
        modelMap.put("config", pcInstallerConfig);
        return "download/pc-installer";
    }

    @RequestMapping(value = "work-installer")
    public String getBusinessInstaller(ModelMap modelMap) {
        modelMap.put("config", workInstallerConfig);
        return "download/work-installer";
    }

    @RequestMapping(value = "test-invoke-app")
    public String getTestInvokeApp() {
        return "testInvokeApp";
    }

    @RequestMapping(value = "test-web-socket")
    public String getTestWebSocket() {
        return "testWebSocket";
    }

    @RequestMapping(value = "monitor/supervisor-monitor")
    public String getSupervisorMonitor(ModelMap modelMap) {
        Long userIDInBackend = SystemEnv.getCurrentUser().getUserId();
        Long userIDInMoumou = csAccountService.getUserIDInMouMou(userIDInBackend);

        modelMap.put("userIDInBackend", userIDInBackend);
        modelMap.put("userIDInMoumou", userIDInMoumou);

        return "monitor/supervisor-monitor";
    }

    @RequestMapping(value = "kefu/kefu-history-message")
    public String getKefuHistoryMessage() {
        return "kefuHistoryMessage/csHistoryMessage";
    }

    @RequestMapping(value = "kefu/session-info")
    public String getKefuSessionCount() {
        return "sessionInfo/SessionCount";
    }

    @RequestMapping(value = "loginDetail/loginDetail")
    public String getLoginDetail(){
        return "loginSearch/LoginSearchPage";
    }
}
