package com.feiniu.subAccount.web;

import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.ServletRequestUtils;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import com.feiniu.core.web.annotation.LoginRequired;
import com.feiniu.subAccount.base.ResponseResult;
import com.feiniu.subAccount.bean.request.AttendanceRequest;
import com.feiniu.subAccount.bean.request.UpdateAttendanceRequest;
import com.feiniu.subAccount.consts.SystemConfig;
import com.feiniu.subAccount.service.StatisticsService;
import com.feiniu.subAccount.util.ExcelUtils;
import com.feiniu.subAccount.util.FormatUtils;
import com.feiniu.subAccount.util.StatisticsUtil;

/**
 * Created by wangzhen on 2015/7/27.
 */

@Controller
@RequestMapping(value = "attendance")
@LoginRequired
public class AttendanceController {
    @Autowired
    private StatisticsService statisticsService;

    //查询客服绩效的入口函数
    @RequestMapping(value = "getattendancemessagelist", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getAttendanceMessageListPerPage(AttendanceRequest attendanceRequest) {
        ResponseResult responseResult = new ResponseResult();
        List<Object> resultTotal = statisticsService.getAttendanceListByRequest(attendanceRequest);

        int startIndex = ( attendanceRequest.getCurrentPageNo()-1 ) * attendanceRequest.getPageSize();
        int endIndex = startIndex + attendanceRequest.getPageSize();

        responseResult.addData("pager", StatisticsUtil.getOnePageInfo((List) resultTotal, startIndex, endIndex));
        responseResult.addData("itemCount",resultTotal.size());
        responseResult.markSuccess();

        return responseResult;
    }

    //excel导出的入口函数
    @RequestMapping(value = "get-excel-file", method = RequestMethod.GET)
    public void exportExcelHandler(AttendanceRequest request, HttpServletResponse response, HttpServletRequest httpServletRequest) {
        String[][]values = statisticsService.getAttendanceArrayStringByRequest(request);
        String[] fieldNameList = statisticsService.getAttendanceTitileStringByRequest(request);
        String fileName = "商家工时统计报表" + FormatUtils.convertNumberToDate(request.getStartTime())
                + "-"+ FormatUtils.convertNumberToDate(request.getEndTime())+".xls";
        ExcelUtils.generateExcelStream(fileName, "默认表单", fieldNameList, values, httpServletRequest, response);
    }

    //进行统计的入口函数，主要的为了方便重新报批任务。
    @RequestMapping(value = "update-attendance-detail", method = RequestMethod.GET)
    public void updateAttendanceDetail(HttpServletRequest servletRequest, UpdateAttendanceRequest request,HttpServletResponse response) {
        String startTimeStr = ServletRequestUtils.getStringParameter(servletRequest, "startTime", "");
        String endTimeStr = ServletRequestUtils.getStringParameter(servletRequest, "endTime", "");
        Long startTime =  FormatUtils.formatStringDate(startTimeStr).getTime()/ SystemConfig.STATISITC_MSECOND_TO_SECOND;
        Long endTime =  FormatUtils.formatStringDate(endTimeStr).getTime()/SystemConfig.STATISITC_MSECOND_TO_SECOND;;
        while (startTime < endTime){
            statisticsService.doUpdateDataBaseBuild(startTime,startTime+SystemConfig.STATISITC_ONE_DAY_SECOUNDS);
            startTime += SystemConfig.STATISITC_ONE_DAY_SECOUNDS;
        }
    }
 }

