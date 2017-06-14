package com.feiniu.fnSubAccount.controller;

import com.feiniu.fnSubAccount.base.ResponseResult;
import com.feiniu.fnSubAccount.bean.request.AttendanceRequest;
import com.feiniu.fnSubAccount.consts.SystemConfig;
import com.feiniu.fnSubAccount.bean.request.UpdateAttendanceRequest;
import com.feiniu.fnSubAccount.service.StatisticsService;
import com.feiniu.fnSubAccount.util.ExcelUtils;
import com.feiniu.fnSubAccount.util.FormatUtils;
import com.feiniu.fnSubAccount.util.StatisticsUtil;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.ServletRequestUtils;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.List;

/**
 * Created by wangzhen on 2015/9/18.
 */
@Controller
@RequestMapping(value = "attendance")
public class AttendanceController {
    @Autowired
    private StatisticsService statisticsService;

    //查询客服绩效的入口函数
    @RequestMapping(value = "getattendancemessagelist", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getAttendanceMessageListPerPage(AttendanceRequest attendanceRequest) {
        ResponseResult responseResult = new ResponseResult();

        attendanceRequest.setMerchantID(SystemConfig.CUSTOMER_SERVICE_MERCHANT_ID);

        List<Object> resultTotal = statisticsService.getAttendanceListByRequest(attendanceRequest);

        int startIndex = ( attendanceRequest.getCurrentPageNo()-1 ) * attendanceRequest.getPageSize();
        int endIndex = startIndex + attendanceRequest.getPageSize();

        responseResult.addData("pager", StatisticsUtil.getOnePageInfo(resultTotal, startIndex, endIndex));
        responseResult.addData("itemCount",resultTotal.size());
        responseResult.markSuccess();

        return responseResult;
    }

    //excel导出的入口函数
    @RequestMapping(value = "get-excel-file", method = RequestMethod.GET)
    public void exportExcelHandler(AttendanceRequest request, HttpServletResponse response, HttpServletRequest httpServletRequest) {
        request.setMerchantID(SystemConfig.CUSTOMER_SERVICE_MERCHANT_ID);

        String[][]values = statisticsService.getAttendanceArrayStringByRequest(request);
        String[] fieldNameList = statisticsService.getAttendanceTitileStringByRequest(request);
        String fileName = SystemConfig.EXPROT_FILE_NAME[request.getSearchType()] + FormatUtils.convertNumberToDate(request.getStartTime())
                + "-"+ FormatUtils.convertNumberToDate(request.getEndTime())+".xlsx";

        if (values != null && values.length !=0)
            ExcelUtils.generateExcelStream(fileName, "默认表单", fieldNameList, values, httpServletRequest, response);
        else {
            ExcelUtils.generateEmptyExcelStream(fileName, "默认表单",  httpServletRequest, response);
        }
    }

    //进行统计的入口函数，主要的为了方便重新报批任务。
    @RequestMapping(value = "update-attendance-detail", method = RequestMethod.GET)
    public void updateAttendanceDetail(HttpServletRequest servletRequest, UpdateAttendanceRequest request,HttpServletResponse response) {
        String startTimeStr = ServletRequestUtils.getStringParameter(servletRequest, "startTime", "");
        String endTimeStr = ServletRequestUtils.getStringParameter(servletRequest, "endTime", "");
        Long startTime =  FormatUtils.formatStringDate(startTimeStr).getTime()/ SystemConfig.STATISITC_MSECOND_TO_SECOND;
        Long endTime =  FormatUtils.formatStringDate(endTimeStr).getTime()/SystemConfig.STATISITC_MSECOND_TO_SECOND;;
        while (startTime < endTime){
            statisticsService.doUpdateDataBaseBuild(startTime,startTime+ SystemConfig.STATISITC_ONE_DAY_SECOUNDS);
            startTime += SystemConfig.STATISITC_ONE_DAY_SECOUNDS;
        }
    }
    @RequestMapping(value = "clearAllinfo", method = RequestMethod.GET)
    public void clearAllWorkLoadInfo(HttpServletRequest servletRequest, UpdateAttendanceRequest request,HttpServletResponse response) {
        statisticsService.clearAll();
    }
}

