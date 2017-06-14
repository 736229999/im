package com.feiniu.fnSubAccount.controller;

import com.feiniu.common.core.SystemEnv;
import com.feiniu.fnSubAccount.base.ResponseResult;
import com.feiniu.fnSubAccount.bean.request.FAQRequest;
import com.feiniu.fnSubAccount.bean.vo.FAQViewDetail;
import com.feiniu.fnSubAccount.domain.FAQ;
import com.feiniu.fnSubAccount.service.FAQService;
import com.feiniu.fnSubAccount.util.ExcelUtils;
import com.feiniu.fnSubAccount.util.FormatUtils;
import com.feiniu.fnSubAccount.util.text.ExcelReader;
import org.apache.commons.collections.CollectionUtils;
import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.multipart.MultipartFile;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * Author: morningking
 * Date: 2015/9/28
 * Contact: 243717042@qq.com
 */
@Controller
@RequestMapping(value = "relative-settings/faq")
public class FAQController {
    @Autowired
    private FAQService faqService;
    private static final String[] exportExcelHeaders = new String[]{
            "问题", "解释", "累计浏览次数", "是否已显示"
    };

    @RequestMapping(value = "config", method = RequestMethod.GET)
    public String getFAQIndex(ModelMap modelMap) {
        List<Integer> countInfo = faqService.faqCount();
        if (CollectionUtils.isNotEmpty(countInfo)) {
            modelMap.put("totalCount", countInfo.get(0));
            modelMap.put("showedCount", countInfo.get(1));
        }

        return "relative-settings/faq-index";
    }

    @RequestMapping(value = "get-faq-list")
    @ResponseBody
    public ResponseResult getFAQList(FAQRequest faqRequest) {
        ResponseResult responseResult = new ResponseResult();

        List<FAQ> faqList = faqService.getFAQList(faqRequest);
        List<Integer> countInfo = faqService.faqCount();

        responseResult.addData("faqList", faqList);
        responseResult.addData("totalFaqNum", countInfo.get(0));
        responseResult.addData("totalShowedFaqNum", countInfo.get(1));

        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "delete-faq", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult deleteFAQ(int faqID) {
        ResponseResult responseResult = new ResponseResult();

        faqService.deleteFAQ(faqID);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "update-faq", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult updateFAQ(FAQ faq) {
        ResponseResult responseResult = new ResponseResult();

        faq.setLastOperator(SystemEnv.getCurrentUser().getUserName());
        faqService.updateFAQ(faq);

        responseResult.addData("faq", faq);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "change-faq-show-state", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult changeFAQShowState(int faqID, boolean isShow) {
        ResponseResult responseResult = new ResponseResult();

        faqService.changeFAQShowState(faqID, isShow);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "add-faq")
    @ResponseBody
    public ResponseResult addFAQ(FAQ faq) {
        ResponseResult responseResult = new ResponseResult();

        faq.setLastOperator(SystemEnv.getCurrentUser().getUserName());
        faqService.addFAQ(faq);

        responseResult.addData("faq", faq);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "change-faq-order", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult changeFAQOrder(int srcFAQID, int srcOrder, int destFAQID, int destOrder) {
        ResponseResult responseResult = new ResponseResult();

        faqService.changeFAQOrder(srcFAQID, srcOrder, destFAQID, destOrder);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "get-faq-view-detail-list", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getFAQViewDetailList(Long startTime, Long endTime, String titleFilter) {
        ResponseResult responseResult = new ResponseResult();

        List<FAQViewDetail> faqViewDetailList = faqService.getFAQViewDetailList(startTime, endTime, titleFilter);

        responseResult.addData("faqViewDetailList", faqViewDetailList);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "import-faq-list", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult importFAQList(@RequestParam(value = "file") MultipartFile multipartFile) {
        ResponseResult responseResult = new ResponseResult();

        ExcelReader excelReader;
        try {
            excelReader = new ExcelReader(multipartFile.getInputStream(), 0);
        } catch (Exception e) {
            responseResult.setMessage("excel文件格式存在问题");
            return responseResult;
        }

        /**表体数据**/
        List<List<String>> dataList = excelReader.getListData();
        //去除表头
        dataList.remove(0);

        List<FAQ> faqList = new ArrayList<>(dataList.size());

        for (List<String> row : dataList) {
            if (StringUtils.isEmpty(row.get(0))) {
                continue;
            }

            FAQ faq = new FAQ();
            faq.setTitle(row.get(0));
            faq.setFaqSummary(row.get(1));
            faq.setLastOperator(SystemEnv.getCurrentUser().getUserName());

            faqList.add(faq);
        }

        faqService.saveAll(faqList);

        responseResult.addData("importCount", faqList.size());
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "export-faq-excel")
    public void exportFAQList(FAQRequest faqRequest, HttpServletResponse response, HttpServletRequest httpServletRequest) {
        List<FAQ> faqList = faqService.getFAQList(faqRequest);
        String[][] content = new String[faqList.size()][exportExcelHeaders.length];
        for (int i = 0; i < faqList.size(); i++) {
            FAQ faq = faqList.get(i);
            content[i][0] = faq.getTitle();
            content[i][1] = faq.getFaqSummary();
            content[i][2] = faq.getViewCount() + "";
            content[i][3] = '1' == faq.getIsShow() ? "是" : "否";
        }

        ExcelUtils.generateExcelStream(
                "常见问题-" + FormatUtils.formatTime(new Date()) + ".xlsx",
                "默认表单", exportExcelHeaders, content, httpServletRequest, response);
    }

    @RequestMapping(value = "update-browse-info")
    @ResponseBody
    public ResponseResult updateBrowserInfo(int faqID) {
        ResponseResult responseResult = new ResponseResult();

        faqService.updateBrowseInfo(faqID);

        responseResult.markSuccess();
        return responseResult;
    }

}
