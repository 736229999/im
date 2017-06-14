package com.feiniu.imServer.controller;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;

/**
 * Created by wangzhen on 2015/12/29.
 */
@Controller
public class ViewController {

    private static final Logger LOGGER = LoggerFactory.getLogger(ViewController.class);


    @RequestMapping(value = "test-page")
    public ModelAndView csAccountManager(ModelMap modelMap) {
        ModelAndView modelAndView = new ModelAndView("test/test-page");
        return modelAndView;
    }

    @RequestMapping(value = "checksrv")
    public ModelAndView getCheckSrv(HttpServletRequest request) {
        ModelAndView modelAndView = new ModelAndView("checksrv");

        return modelAndView;
    }
}
