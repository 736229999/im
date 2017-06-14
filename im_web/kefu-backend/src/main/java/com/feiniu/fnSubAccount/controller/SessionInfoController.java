package com.feiniu.fnSubAccount.controller;

import com.feiniu.fnSubAccount.base.ResponseResult;
import com.feiniu.fnSubAccount.bean.SessionInfo.CsAccountName;
import com.feiniu.fnSubAccount.bean.SessionInfo.SessionCountInfoShow;
import com.feiniu.fnSubAccount.bean.SessionInfo.SessionInfoItemShow;
import com.feiniu.fnSubAccount.bean.SessionInfo.SessionProfileItem;
import com.feiniu.fnSubAccount.bean.request.SessionInfoRequest;
import com.feiniu.fnSubAccount.consts.SystemConfig;
import com.feiniu.fnSubAccount.domain.ChatMessage;
import com.feiniu.fnSubAccount.service.SessionInfoService;
import com.feiniu.fnSubAccount.util.ExcelUtils;
import com.feiniu.fnSubAccount.util.FormatUtils;
import com.feiniu.fnSubAccount.util.Pager;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.List;

/**
 * Created by wangzhen on 2016/4/6.
 */
@Controller
@RequestMapping(value = "sessionInfo")
public class SessionInfoController {

    protected Logger LOGGER = LoggerFactory.getLogger(this.getClass());
    @Autowired
    SessionInfoService sessionInfoService;

    @RequestMapping(value = "get-session-count-list")
    @ResponseBody
    public ResponseResult getSessionCountList(Long startTime,Long endTime){
        ResponseResult responseResult = new ResponseResult();
        if (startTime == null || endTime == null){
            LOGGER.error("获取会话统计失败，参数不全");
            responseResult.setMessage("参数不全，请重新输入");
            return  responseResult;
        }

        List<SessionCountInfoShow> sessionCountInfoShows = sessionInfoService.getSessionCountInfo(startTime,endTime);
        responseResult.markSuccess();
        responseResult.addData("data",sessionCountInfoShows);
        return responseResult;
    }

    @RequestMapping(value = "get-cs-username-by-partname")
    @ResponseBody
    public ResponseResult getCsUserNameByPartName(String partName){
        ResponseResult responseResult = new ResponseResult();
        List<CsAccountName> resultList = sessionInfoService.getCsAccountInfoListByPartName(partName);
        responseResult.addData("moumouNameList",resultList);
        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "get-session-info")
    @ResponseBody
    public ResponseResult getSessionInfoList(SessionInfoRequest sessionInfoRequest) {
        ResponseResult responseResult = new ResponseResult();

        if (sessionInfoRequest.getStartTime() == null ||
                sessionInfoRequest.getEndTime() == null||
                sessionInfoRequest.getPageNo()==null ||
                sessionInfoRequest.getPageSize() == null){
            LOGGER.error("获取会话详情失败，参数不全");
            responseResult.setMessage("参数不全，请重新输入");
            return  responseResult;
        }

        Pager<SessionInfoItemShow> result  = sessionInfoService.getSessionInfo(
                sessionInfoRequest.getStartTime(),sessionInfoRequest.getEndTime()
                ,sessionInfoRequest.getCustomerName(),sessionInfoRequest.getServiceId(),
                sessionInfoRequest.getPageNo(),sessionInfoRequest.getPageSize());

        if (result == null){
            responseResult.setMessage("找不到指定的客户账号");
            return responseResult;
        }
        responseResult.addData("pager", result);
        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "get-session-profile")
    @ResponseBody
    public ResponseResult getSessionProfile(SessionInfoRequest sessionInfoRequest) {
        ResponseResult responseResult = new ResponseResult();
        List<SessionProfileItem> result  = sessionInfoService.getSessionProfile(
                sessionInfoRequest.getStartTime(), sessionInfoRequest.getEndTime(),
                sessionInfoRequest.getCustomerName(),sessionInfoRequest.getServiceId()
                ,sessionInfoRequest.getType());

        responseResult.addData("resultList", result);
        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "get-session-chat-message")
    @ResponseBody
    public ResponseResult getSessionChatMessage(Long sessionId,Integer pageNo, Integer pageSize) {
        ResponseResult responseResult = new ResponseResult();

        Pager<ChatMessage> result  = sessionInfoService.getHistoryMessageListById(sessionId,pageNo,pageSize);
        responseResult.addData("pager", result);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "get-excel-file.xls", method = RequestMethod.GET)
    @ResponseBody
    public void excelExport(SessionInfoRequest sessionInfoRequest,HttpServletResponse response, HttpServletRequest httpServletRequest) {
        String[] fieldNameList = {"会话开始时间", "会话结束时间",
                "客户账号", "客服账号",
                "会话总时长","客户等待时间"
                ,"移动端入口","PC端入口"};

        sessionInfoRequest.setPageNo(1);
        sessionInfoRequest.setPageSize(SystemConfig.MAX_PAGE_ITEM_SIZE);

        Pager<SessionInfoItemShow> result  = sessionInfoService.getSessionInfo(
                sessionInfoRequest.getStartTime(), sessionInfoRequest.getEndTime()
                , sessionInfoRequest.getCustomerName(), sessionInfoRequest.getServiceId(),
                sessionInfoRequest.getPageNo(), sessionInfoRequest.getPageSize());


        if (result == null){
            LOGGER.error("找不到指定的客户账号");
            return ;
        }

        String[][] values;
        List<SessionInfoItemShow> resultList = result.getResultsForCurrentPage();
        if (result.getResultsForCurrentPage().size() > 0 ) {
            int len = resultList.size();
            values = new String[resultList.size()][8];
            result.getResultsForCurrentPage().get(0).setPcPortStr("--");
            result.getResultsForCurrentPage().get(0).setMobilePortStr("--");
            for (int i =0 ; i<len;i++){
                FormatUtils.formatSessionInfoItemShowToExcel(values, i, resultList.get(i));
            }
            ExcelUtils.generateExcelStream("会话详情表.xlsx", "默认表单", fieldNameList, values, httpServletRequest, response);
        }else {
            ExcelUtils.generateEmptyExcelStream("会话详情表.xlsx", "默认表单",  httpServletRequest, response);
        }
    }


    @RequestMapping(value = "get-session-count-file.xls", method = RequestMethod.GET)
    @ResponseBody
    public void excelSessionCountExport(long startTime , long endTime,HttpServletResponse response, HttpServletRequest httpServletRequest) {
        String[] fieldNameList = {"时段", "会话量","所占比例"};
        List<SessionCountInfoShow> sessionCountInfoShows = sessionInfoService.getSessionCountInfo(startTime,endTime);

        String[][] values;
        if (sessionCountInfoShows.size() > 0 ) {
            int len = sessionCountInfoShows.size();
            values = new String[len][3];
            for (int i =0 ; i<len;i++){
                values[i][0]=sessionCountInfoShows.get(i).getTime();
                values[i][1]=sessionCountInfoShows.get(i).getSessionCount().toString();
                values[i][2]=sessionCountInfoShows.get(i).getSessionRate() + " %";
            }
            ExcelUtils.generateExcelStream("会话统计表.xlsx", "默认表单", fieldNameList, values, httpServletRequest, response);
        }else {
            ExcelUtils.generateEmptyExcelStream("会话统计表.xlsx", "默认表单",  httpServletRequest, response);
        }
    }
}
