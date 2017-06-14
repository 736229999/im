package com.feiniu.imServer.controller;

import com.feiniu.imServer.bean.base.WebResultInfo;
import com.feiniu.imServer.bean.request.SaveBuyerRequest;
import com.feiniu.imServer.bean.request.SaveSellerRequest;
import com.feiniu.imServer.repository.UserInfoDao;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

/**
 * Created by zhangmin on 2016/2/29.
 */
@Controller
@RequestMapping(value = "login")
public class LoginController {

//    private static final Logger LOGGER = LoggerFactory.getLogger(LoginController.class);

    @Autowired
    private UserInfoDao userInfoDao;

    @RequestMapping(value = "save-buyer")
    @ResponseBody
    public WebResultInfo saveBuyer(SaveBuyerRequest saveBuyerRequest)
    {
        WebResultInfo webResultInfo = new WebResultInfo();

        Integer userId = userInfoDao.saveBuyer(saveBuyerRequest);

        if(!userId.equals(0))
        {
            webResultInfo.markSuccess();
            webResultInfo.addData("userId", userId);
        }

        return webResultInfo;
    }

    @RequestMapping(value = "save-seller")
    @ResponseBody
    private WebResultInfo saveSeller(SaveSellerRequest saveSellerRequest)
    {
        WebResultInfo webResultInfo = new WebResultInfo();

        Integer returnCode = userInfoDao.saveSeller(saveSellerRequest);
        if(!returnCode.equals(-1)){
            webResultInfo.markSuccess();
        }
        return webResultInfo;
    }



}
