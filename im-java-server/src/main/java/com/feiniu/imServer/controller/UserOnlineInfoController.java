package com.feiniu.imServer.controller;

import com.feiniu.imServer.base.ResponseResult;
import com.feiniu.imServer.service.userinfoOnline.UserInfoOnlineService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;



/**
 * Created by chenqing on 2016/3/18.
 */
@Controller
public class UserOnlineInfoController {

    @Autowired
    UserInfoOnlineService userInfoOnlineService;

    @RequestMapping(value = "get-user-online-time")
    @ResponseBody
    public ResponseResult getUserOnlineTime(Long userId){
        ResponseResult result = new ResponseResult();
        Integer onlineTime = userInfoOnlineService.getUserOnlineTime(userId);
        result.addData("onlineTime",onlineTime);
        result.markSuccess();
        return result;
    }

}
