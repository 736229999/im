package com.feiniu.subAccount.web;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import com.feiniu.core.web.annotation.LoginRequired;
import com.feiniu.subAccount.base.ResponseResult;
import com.feiniu.subAccount.component.repository.HistoryMessageListRequest;
import com.feiniu.subAccount.service.MonitorService;

/**
 * Created by wangchen on 2015/2/4.
 */
@Controller
@RequestMapping(value = "monitor")
@LoginRequired
public class MonitorController {
    @Autowired
    private MonitorService monitorService;

    @RequestMapping(value = "gethistorymessagelist", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getHistoryMessageListPerPage(HistoryMessageListRequest historyMessageListRequest) {
        ResponseResult responseResult = new ResponseResult();

        responseResult.addData("pager", monitorService.getHistoryMessageList(historyMessageListRequest));
        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "getserviceappraise")
    @ResponseBody
    public ResponseResult getServiceAppraise() {
        ResponseResult responseResult = new ResponseResult();

        responseResult.markSuccess();
        return responseResult;
    }
}
