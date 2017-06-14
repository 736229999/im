package com.feiniu.fnSubAccount.controller;

import com.feiniu.fnSubAccount.base.ResponseResult;
import com.feiniu.fnSubAccount.domain.FAQ;
import com.feiniu.fnSubAccount.domain.FAQCategory;
import com.feiniu.fnSubAccount.service.*;
import com.feiniu.fnSubAccount.util.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * Author: morningking
 * Date: 2015/11/24
 * Contact: 243717042@qq.com
 */
@RequestMapping(value = "service-provider")
@Controller
public class ServiceProviderController {
    @Autowired
    private KefuIndexConfigService kefuIndexConfigService;
    @Autowired
    private AfterSaleBoardService afterSaleBoardService;
    @Autowired
    private FAQCategoryService faqCategoryService;
    @Autowired
    private QuickPhraseService quickPhraseService;
    @Autowired
    private FAQService faqService;
    @Autowired
    private GroupInfoService groupInfoService;
    @Autowired
    private TouchPortalBoardConfigService touchPortalBoardConfigService;

    //咨询次数
    private AtomicInteger consultNum;

    @RequestMapping(value = "get-home-page-config")
    @ResponseBody
    public ResponseResult getHomePageConfig() {
        ResponseResult responseResult = new ResponseResult();

        responseResult.addData("kefuIndexConfig", kefuIndexConfigService.getRuntimeData());
        responseResult.addData("afterSaleBoardList", afterSaleBoardService.getRuntimeData());

        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "faq-category/get-all-showed-faq-category")
    @ResponseBody
    public ResponseResult getCategoryList() {
        ResponseResult responseResult = new ResponseResult();

        List<FAQCategory> categoryList = faqCategoryService.getFAQCategoryListForMoumou();

        responseResult.addData("faqCategoryList", categoryList);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "faq-category/update-consult-info")
    @ResponseBody
    public ResponseResult updateConsultInfo(String categoryIDs) {
        ResponseResult responseResult = new ResponseResult();

        faqCategoryService.updateConsultInfo(StringUtils.stringToIntegerList(categoryIDs, ","));

        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "faq/update-browse-info")
    @ResponseBody
    public ResponseResult updateFaqBrowserInfo(int faqID) {
        ResponseResult responseResult = new ResponseResult();

        faqService.updateBrowseInfo(faqID);

        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "get-quick-phrase")
    @ResponseBody
    public ResponseResult getQuickPhraseForImServer() {
        ResponseResult responseResult = new ResponseResult();
        responseResult.addData("quickPhraseList", quickPhraseService.getAllQuickPhrase());
        responseResult.markSuccess();
        return responseResult;

    }

    @RequestMapping(value = "faq/get-all-showed-faq")
    @ResponseBody
    public ResponseResult getAllShowedFaqList() {
        ResponseResult responseResult = new ResponseResult();

        List<FAQ> faqList = faqService.getFAQListForMoumou();

        responseResult.addData("faq", faqList);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "get-embed-related-group")
    @ResponseBody
    //获取各布点页面相关的组信息
    public ResponseResult getEmbedRelatedGroupInfo() {
        ResponseResult responseResult = new ResponseResult();

        responseResult.addData("embedSet", groupInfoService.getEmbedRelatedGroupInfo());
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "get-touch-board-config-list")
    @ResponseBody
    public ResponseResult getTouchBoardConfigList() {
        ResponseResult responseResult = new ResponseResult();

        responseResult.addData("list", touchPortalBoardConfigService.getRuntimeData());
        responseResult.markSuccess();
        return responseResult;
    }
}
