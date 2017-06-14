package com.feiniu.fnSubAccount.controller;

import com.feiniu.fnSubAccount.base.ResponseResult;
import com.feiniu.fnSubAccount.bean.CSMoitorInfoShow;
import com.feiniu.fnSubAccount.bean.CSMonitorInfo;
import com.feiniu.fnSubAccount.bean.CSMonitorProfile;
import com.feiniu.fnSubAccount.bean.request.CsMonitorRequest;
import com.feiniu.fnSubAccount.bean.shunt.QueueItemShow;
import com.feiniu.fnSubAccount.constants.BusinessConfig;
import com.feiniu.fnSubAccount.consts.SystemConfig;
import com.feiniu.fnSubAccount.service.CacheClientService;
import com.feiniu.fnSubAccount.service.CsMonitorService;
import com.feiniu.fnSubAccount.service.external.ImService;
import com.feiniu.fnSubAccount.util.ExcelUtils;
import com.feiniu.fnSubAccount.util.FormatUtils;
import com.feiniu.fnSubAccount.util.Pager;
import com.fn.cache.client.RedisCacheClient;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * Created by wanbingwen on 2015/10/28.
 */

@Controller
@RequestMapping(value = "cs-monitor")
public class CSMonitorController {
    @Autowired
    BusinessConfig businessConfig;
    @Autowired
    CsMonitorService csMonitorService;
    @Autowired
    ImService imService;
    @Autowired
    CacheClientService cacheClientService;
    @Autowired
    RedisCacheClient redisCacheClient;

    protected org.slf4j.Logger LOGGER = LoggerFactory.getLogger(this.getClass());

    @RequestMapping(value = "get-monitor-result-list", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getMonitorResultNumberList(CsMonitorRequest csMonitorRequest) {
        ResponseResult responseResult = new ResponseResult();
        Pager<CSMoitorInfoShow> pager = new Pager<CSMoitorInfoShow>();

        int startRow = csMonitorRequest.getPageSize() * (csMonitorRequest.getCurrentPageNo() - 1);
        int maxResults = csMonitorRequest.getPageSize();
        List<CSMoitorInfoShow> csMonitorInfoList = new ArrayList<>();
        CSMonitorProfile csMonitorProfile = csMonitorService.doGetMonitorReuslt(startRow, maxResults, csMonitorInfoList);

        responseResult.addData("csMonitorProfile", csMonitorProfile);
        pager.setCurrentPageNo(csMonitorRequest.getCurrentPageNo());
        pager.setPageSize(maxResults);
        pager.setTotalRecordSize(csMonitorProfile.getCSCount());
        pager.setResultsForCurrentPage(csMonitorInfoList);

        responseResult.addData("pager", pager);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "ask-service-offline")
    @ResponseBody
    public ResponseResult askServiceOffline(Long csId){
        ResponseResult result = new ResponseResult();
        boolean canOffline = imService.askCServiceOffline(csId);
        result.addData("canOffline",canOffline);
        result.markSuccess();
        return  result;
    }


    @RequestMapping(value = "get-excel-file")
    public void exportCSMonitorList(CSMonitorInfo csMonitorInfo, HttpServletResponse response, HttpServletRequest httpServletRequest) {
        int startRow = 0;
        int maxResults = SystemConfig.MAX_PAGE_ITEM_SIZE;

        List<CSMoitorInfoShow> csMonitorInfoList = new ArrayList<>();
        CSMonitorProfile csMonitorProfile = csMonitorService.doGetMonitorReuslt(startRow,maxResults,csMonitorInfoList);

        //获取数据
        String[][] content = new String[csMonitorInfoList.size() +1][6];
        String[] fieldNameList = new String[6];

        fieldNameList[0] ="账号总数:" + csMonitorProfile.getCSCount() + "人";
        fieldNameList[1] ="在线客服:" + csMonitorProfile.getOnlineCSCount()+ "人";
        fieldNameList[2] ="接待已达上限" + csMonitorProfile.getArriveRecpLimitCount()+ "人";
        fieldNameList[3] ="离线客服:" + csMonitorProfile.getOfflineCSCount()+ "人";
        fieldNameList[4] ="客人咨询量:" + csMonitorProfile.getConsultCount()+ "人";
        fieldNameList[5] ="客服接待量:" + csMonitorProfile.getCSRecpCount()+ "人";
        content[0][0] ="正在排队客人:" + csMonitorProfile.getQueuingCount()+ "位";

        for (int i = 0; i < csMonitorInfoList.size(); i ++) {
            CSMoitorInfoShow csMonitorInfoIndex = csMonitorInfoList.get(i);
            content[i+1][0] = csMonitorInfoIndex.getUserNameFull();
            content[i+1][1] = csMonitorInfoIndex.getOnlineNameFull();
            content[i+1][2] = csMonitorInfoIndex.getLimitNameFull();
            content[i+1][3] = csMonitorInfoIndex.getOfflineNameFull();
            content[i+1][4] = csMonitorInfoIndex.getAskingCont();
            content[i+1][5] = csMonitorInfoIndex.getReceptionCount();
        }

        ExcelUtils.generateExcelStream(
                "实时监控-" + FormatUtils.formatTime(new Date())+ ".xlsx",
                "默认表单", fieldNameList, content, httpServletRequest, response);
    }

    @RequestMapping(value = "get-queue-info", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getQueueInfos(){
        ResponseResult responseResult = new ResponseResult();
        List<QueueItemShow> resultList = csMonitorService.getQueueItemList();
        responseResult.addData("data",resultList);
        responseResult.markSuccess();
        return responseResult;
    }
}

