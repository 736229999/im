package com.feiniu.fnSubAccount.controller;

import com.feiniu.common.core.SystemEnv;
import com.feiniu.fnSubAccount.base.ResponseResult;
import com.feiniu.fnSubAccount.bean.request.FAQRequest;
import com.feiniu.fnSubAccount.bean.vo.FAQCategoryConsultDetail;
import com.feiniu.fnSubAccount.domain.FAQCategory;
import com.feiniu.fnSubAccount.service.FAQCategoryService;
import com.feiniu.fnSubAccount.util.ExcelUtils;
import com.feiniu.fnSubAccount.util.FormatUtils;
import com.feiniu.fnSubAccount.util.text.ExcelReader;
import org.apache.commons.collections4.CollectionUtils;
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
 * Date: 2015/10/12
 * Contact: 243717042@qq.com
 */
@Controller
@RequestMapping(value = "relative-settings/faq-category")
public class FAQCategoryController {
    @Autowired
    private FAQCategoryService faqCategoryService;
    private static final String[] exportExcelHeaders = new String[]{
            "问题分类名称", "备注", "累计咨询次数", "是否已显示"
    };

    @RequestMapping(value = "config", method = RequestMethod.GET)
    public String getFAQIndex(ModelMap modelMap) {
        List<Integer> countInfo = faqCategoryService.faqCount();
        if (CollectionUtils.isNotEmpty(countInfo)) {
            modelMap.put("totalCount", countInfo.get(0));
            modelMap.put("showedCount", countInfo.get(1));
        }

        return "relative-settings/faq-category-index";
    }

    @RequestMapping(value = "get-faq-category-list")
    @ResponseBody
    public ResponseResult getFAQCategoryList(FAQRequest faqRequest) {
        ResponseResult responseResult = new ResponseResult();

        List<FAQCategory> faqList = faqCategoryService.getFAQCategoryList(faqRequest);
        List<Integer> countInfo = faqCategoryService.faqCount();

        responseResult.addData("faqList", faqList);
        responseResult.addData("totalFaqNum", countInfo.get(0));
        responseResult.addData("totalShowedFaqNum", countInfo.get(1));

        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "delete", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult deleteFAQCategory(int categoryID) {
        ResponseResult responseResult = new ResponseResult();

        faqCategoryService.deleteFAQCategory(categoryID);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "update", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult updateFAQCategory(FAQCategory faqCategory) {
        ResponseResult responseResult = new ResponseResult();

        faqCategory.setLastOperator(SystemEnv.getCurrentUser().getUserName());
        faqCategoryService.updateFAQCategory(faqCategory);

        responseResult.addData("faqCategory", faqCategory);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "change-show-state", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult changeFAQShowState(int categoryID, boolean isShow) {
        ResponseResult responseResult = new ResponseResult();

        faqCategoryService.changeFAQCategoryShowState(categoryID, isShow);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "add")
    @ResponseBody
    public ResponseResult addFAQCategory(FAQCategory faqCategory) {
        ResponseResult responseResult = new ResponseResult();

        faqCategory.setLastOperator(SystemEnv.getCurrentUser().getUserName());

        faqCategoryService.addFAQCategory(faqCategory);

        responseResult.addData("faqCategory", faqCategory);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "get-consult-detail-list", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getConsultDetailList(Long startTime, Long endTime, String titleFilter) {
        ResponseResult responseResult = new ResponseResult();

        List<FAQCategoryConsultDetail> consultDetailList = faqCategoryService.getFAQCategoryConsultDetailList(startTime, endTime, titleFilter);

        responseResult.addData("consultDetailList", consultDetailList);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "import", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult importFAQCategoryList(@RequestParam(value = "file") MultipartFile multipartFile) {
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
        List<FAQCategory> faqCategoryList = new ArrayList<>(dataList.size());

        for (List<String> row : dataList) {
            if (StringUtils.isEmpty(row.get(0))) {
                continue;
            }

            FAQCategory faq = new FAQCategory();
            faq.setTitle(row.get(0));
            faq.setRemark(row.get(1));
            faq.setLastOperator(SystemEnv.getCurrentUser().getUserName());

            faqCategoryList.add(faq);
        }

        faqCategoryService.saveAll(faqCategoryList);

        responseResult.addData("importCount", faqCategoryList.size());
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "export")
    public void exportFAQList(FAQRequest faqRequest, HttpServletResponse response, HttpServletRequest httpServletRequest) {
        List<FAQCategory> faqList = faqCategoryService.getFAQCategoryList(faqRequest);
        String[][] content = new String[faqList.size()][exportExcelHeaders.length];
        for (int i = 0; i < faqList.size(); i++) {
            FAQCategory faq = faqList.get(i);
            content[i][0] = faq.getTitle();
            content[i][1] = faq.getRemark();
            content[i][2] = faq.getConsultCount() + "";
            content[i][3] = '1' == faq.getIsShow() ? "是" : "否";
        }

        ExcelUtils.generateExcelStream(
                "客户问题分类设置-" + FormatUtils.formatTime(new Date()) + ".xlsx",
                "默认表单", exportExcelHeaders, content, httpServletRequest, response);
    }
}
