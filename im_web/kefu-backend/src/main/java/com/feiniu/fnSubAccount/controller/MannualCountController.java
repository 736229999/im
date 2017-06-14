package com.feiniu.fnSubAccount.controller;

import com.feiniu.fnSubAccount.base.ResponseResult;
import com.feiniu.fnSubAccount.controller.websocket.ChatSessionManager;
import com.feiniu.fnSubAccount.service.FAQCategoryService;
import com.feiniu.fnSubAccount.service.FAQService;
import org.apache.commons.lang.time.DateUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.WebDataBinder;
import org.springframework.web.bind.annotation.InitBinder;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import java.beans.PropertyEditorSupport;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

/**
 * Author: morningking
 * Date: 2015/12/16
 * Contact: 243717042@qq.com
 */
@Controller
@RequestMapping(value = "mannual-count")
public class MannualCountController {
    @Autowired
    private FAQService faqService;
    @Autowired
    private FAQCategoryService faqCategoryService;
    @Autowired
    private ChatSessionManager chatSessionManager;

    @InitBinder
    public void initBinder(WebDataBinder binder) {
        binder.registerCustomEditor(Date.class, new PropertyEditorSupport() {
            public void setAsText(String value) {
                try {
                    setValue(new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").parse(value));
                } catch (ParseException e) {
                    try {
                        setValue(new SimpleDateFormat("yyyy-MM-dd").parse(value));
                    } catch (ParseException e1) {
                        try {
                            setValue(new SimpleDateFormat("HH:mm:ss").parse(value));
                        } catch (ParseException e2) {
                            setValue(null);
                        }
                    }
                }
            }
        });
    }

    @RequestMapping(value = "do-count-faq-browse")
    @ResponseBody
    public ResponseResult doCountFAQBrowserInfo(Date date) {
        ResponseResult responseResult = new ResponseResult();
        faqService.updateAllBrowseInfo(DateUtils.truncate(date, Calendar.DATE).getTime());
        return responseResult;
    }

    @RequestMapping(value = "do-count-faq-category-consult-info")
    @ResponseBody
    public ResponseResult doCountFAQCategoryConsultInfo(Date date) {
        ResponseResult responseResult = new ResponseResult();
        faqCategoryService.updateAllConsultInfo(DateUtils.truncate(date, Calendar.DATE).getTime());
        return responseResult;
    }
}
