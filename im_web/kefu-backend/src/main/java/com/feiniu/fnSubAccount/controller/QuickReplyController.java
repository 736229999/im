package com.feiniu.fnSubAccount.controller;

import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

/**
 * Created by zhangmin on 2015/10/20.
 */
@Controller
@RequestMapping(value = "relative-settings/quick-reply")
public class QuickReplyController {

    @RequestMapping(value = "config", method = RequestMethod.GET)
    public String getQuickReplyIndex(ModelMap modelMap) {
        return "relative-settings/quick-reply-index";
    }
}
