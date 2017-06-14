package com.feiniu.fnSubAccount.controller;

import com.feiniu.fnSubAccount.base.ResponseResult;
import com.feiniu.fnSubAccount.bean.AppraiseStat;
import com.feiniu.fnSubAccount.bean.ServiceAppraiseDetailStat;
import com.feiniu.fnSubAccount.bean.request.ServiceAppraiseDetailRequest;
import com.feiniu.fnSubAccount.bean.request.ServiceAppraiseRequest;
import com.feiniu.fnSubAccount.consts.SystemConfig;
import com.feiniu.fnSubAccount.domain.ChatMessage;
import com.feiniu.fnSubAccount.service.ServiceAppraiseService;
import com.feiniu.fnSubAccount.util.ExcelUtils;
import com.feiniu.fnSubAccount.util.FormatUtils;
import com.feiniu.fnSubAccount.util.ListUtils;
import com.feiniu.fnSubAccount.util.Pager;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Map;

/**
 * Created by wangchen on 2015/2/12.
 */
@Controller
@RequestMapping(value = "service-appraise")
public class ServiceAppraiseController {
    @Autowired
    private ServiceAppraiseService serviceAppraiseService;

    @RequestMapping(value = "query", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getServiceAppraise(ServiceAppraiseRequest serviceAppraiseRequest) {
        ResponseResult responseResult = new ResponseResult();
        List<String> subAccountIDList = Arrays.asList(serviceAppraiseRequest.getSubAccountIDListStr().split(";"));

        serviceAppraiseRequest.setSubAccountIDList(
                ListUtils.transformElementToNumber(subAccountIDList, Long.class));

        Map<Long ,AppraiseStat> idToAppraiseStatMap = serviceAppraiseService.getServiceAppraiseList(serviceAppraiseRequest);

        responseResult.addData("serviceAppraiseStat", new ArrayList<AppraiseStat>(idToAppraiseStatMap.values()));
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "queryDetail", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getServiceAppraiseDetail(ServiceAppraiseDetailRequest serviceAppraiseDetailRequest) {
        ResponseResult responseResult = new ResponseResult();

        List<ServiceAppraiseDetailStat> result  = serviceAppraiseService.getServiceAppraiseList(serviceAppraiseDetailRequest);
        responseResult.addData("serviceAppraiseDetailStat", result);
        responseResult.addData("itemCount", result.size());
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "get-history-chat-list-by-sessionId", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getChatMessageBySessionId(ServiceAppraiseDetailRequest serviceAppraiseDetailRequest) {
        ResponseResult responseResult = new ResponseResult();

        Pager<ChatMessage> result  = serviceAppraiseService.getHistoryMessageListById(serviceAppraiseDetailRequest);
        responseResult.addData("pager", result);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "get-excel-file", method = RequestMethod.GET)
    public void exportExcelHandler(ServiceAppraiseRequest serviceAppraiseRequest, HttpServletResponse response, HttpServletRequest httpServletRequest) {
        String[] fieldNameList = {"帐号名", "未评估", "非常满意", "满意", "一般","不满意" ,"非常不满意","好评率"};
        String[][] values;

        List<String> subAccountIDList = Arrays.asList(serviceAppraiseRequest.getSubAccountIDListStr().split(";"));

        serviceAppraiseRequest.setSubAccountIDList(
                ListUtils.transformElementToNumber(subAccountIDList, Long.class));

        Map<Long ,AppraiseStat> idToAppraiseStatMap = serviceAppraiseService.getServiceAppraiseList(serviceAppraiseRequest);

        if (idToAppraiseStatMap.size() == 0 ){
            ExcelUtils.generateEmptyExcelStream("服务评价统计表.xls", "默认表单",  httpServletRequest, response);
        }
        int len = idToAppraiseStatMap.size();
        values = new String[len][8];
        int i = 0;

        for (Map.Entry<Long ,AppraiseStat> entry:idToAppraiseStatMap.entrySet()){
            FormatUtils.formatAppraiseStatToExcel(values, i, entry.getValue());
            i++;
        }

        ExcelUtils.generateExcelStream("服务评价统计表.xlsx", "默认表单", fieldNameList, values, httpServletRequest, response);
    }
    @RequestMapping(value = "get-detail-excel-file", method = RequestMethod.GET)
    public void exportDetailExcelHandler(ServiceAppraiseDetailRequest serviceAppraiseDetailRequest, HttpServletResponse response, HttpServletRequest httpServletRequest) {
        String[] fieldNameList = {"账号名", "买家账号", "满意度", "评价建议", "评价时间","会话开始时间" ,"会话结束时间"};

        serviceAppraiseDetailRequest.setCurrentPageNo(1);
        serviceAppraiseDetailRequest.setPageSize(SystemConfig.MAX_PAGE_ITEM_SIZE);

        List<ServiceAppraiseDetailStat> result  = serviceAppraiseService.getServiceAppraiseList(serviceAppraiseDetailRequest);

        String[][] values;
        if (result.size() > 0 ) {
            int len = result.size();
            values = new String[result.size()][7];
            for (int i =0 ; i<len;i++){
                FormatUtils.formatServiceAppraiseDetailStatToExcel(values,i,result.get(i));
            }
            ExcelUtils.generateExcelStream("服务评价统计表.xlsx", "默认表单", fieldNameList, values, httpServletRequest, response);
        }else {
            ExcelUtils.generateEmptyExcelStream("服务评价统计表.xlsx", "默认表单",  httpServletRequest, response);
        }

    }
}
