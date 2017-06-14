package com.cn.feiniu.logManagement.web.controller;

import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.RequestMapping;

/**
 * Created by wangchen on 2015/4/15.
 */
@Controller
public class ViewController {

    @RequestMapping(value = "checkSrv")
    public String getCheckSrv(ModelMap modelMap) {

        return  "checkSrv/checkSrv";
    }
}
