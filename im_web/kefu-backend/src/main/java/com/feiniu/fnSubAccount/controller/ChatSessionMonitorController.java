package com.feiniu.fnSubAccount.controller;

import com.feiniu.fnSubAccount.base.ResponseResult;
import com.feiniu.fnSubAccount.bean.request.GetChatSessionRequest;
import com.feiniu.fnSubAccount.bean.request.MonitorInterveneSessionRequest;
import com.feiniu.fnSubAccount.service.ChatSessionMonitorService;
import com.feiniu.fnSubAccount.service.external.ImService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

/**
 * Author: morningking
 * Date: 2016/4/7 14:00
 * Contact: 243717042@qq.com
 */
@Controller
public class ChatSessionMonitorController {
    @Autowired
    private ChatSessionMonitorService chatSessionMonitorService;
    @Autowired
    private ImService imService;

    @RequestMapping(value = "monitor/get-active-chat-session-list")
    @ResponseBody
    public ResponseResult getActiveChatSessionList(GetChatSessionRequest request) {
        ResponseResult responseResult = new ResponseResult();

        responseResult.addData("list", chatSessionMonitorService.getActiveChatSessionList(request));
        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "monitor/check-is-legal-to-intervene-session")
    @ResponseBody
    public ResponseResult checkIsLegalToInterveneSession(MonitorInterveneSessionRequest request) {
        ResponseResult responseResult = new ResponseResult();
        boolean isChatSessionClosed = chatSessionMonitorService.isChatSessionClosed(request.getSessionID());

        if (isChatSessionClosed) {
            responseResult.addData("isChatSessionClosed", true);
        } else {
            responseResult.addData("isIntervendByOtherMonitor", imService.checkIsOtherMonitorInInterveneProgress(request));
        }

        responseResult.markSuccess();
        return responseResult;
    }

}
