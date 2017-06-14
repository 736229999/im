package com.feiniu.fnSubAccount.controller;

import com.feiniu.common.core.SystemEnv;
import com.feiniu.fnSubAccount.base.ResponseResult;
import com.feiniu.fnSubAccount.bean.request.QuickPhraseGroupRequest;
import com.feiniu.fnSubAccount.bean.request.QuickPhraseRequest;
import com.feiniu.fnSubAccount.consts.SystemConfig;
import com.feiniu.fnSubAccount.domain.QuickPhrase;
import com.feiniu.fnSubAccount.service.QuickPhraseService;
import com.feiniu.fnSubAccount.util.ExcelUtils;
import com.feiniu.fnSubAccount.util.FormatUtils;
import com.feiniu.fnSubAccount.util.ExcelReader;
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
 * Created by zhangmin on 2015/10/23.
 */
@Controller
@RequestMapping(value = "relative-settings/quick-phrase")
public class QuickPhraseController {
    @Autowired
    private QuickPhraseService quickPhraseService;

    private static int maxGroupNameLen = 10;
    private static int maxQuickCodeLen = 4;
    private static int maxQuickPhraseLen = 430;
    private static final String[] exportExcelHeaders = new String[]{
            "快捷编码", "快捷短语", "快捷短语分组"
    };
    private static String noGroupName = "未分组";

    private static String getLimitLenString(String initString, int maxLen) {
        String retString = "";
        if (StringUtils.isNotEmpty(initString)) {
            retString = initString.substring(0, (initString.length() < maxLen ? initString.length() : maxLen));
        }
        return retString;
    }

    @RequestMapping(value = "get-quick-phrase")
    @ResponseBody
    public ResponseResult getPartQuickPhrase(QuickPhraseRequest quickPhraseRequest) {
        ResponseResult responseResult = new ResponseResult();

        responseResult.addData("quickPhraseList", quickPhraseService.getQuickPhraseList(quickPhraseRequest, true));
        responseResult.addData("totalNum", quickPhraseService.getQuickPhraseCount(quickPhraseRequest));
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "add-quick-phrase")
    @ResponseBody
    public ResponseResult addQuickPhrase(QuickPhrase quickPhrase) {
        ResponseResult responseResult = new ResponseResult();
        if(StringUtils.isEmpty(quickPhrase.getPhraseGroupName()))
        {
            quickPhrase.setPhraseGroupName(noGroupName);
        }
        quickPhraseService.addQuickPhrase(quickPhrase);
        responseResult.addData("quickPhrase", quickPhrase);
        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "update-quick-phrase")
    @ResponseBody
    public ResponseResult updateQuickPhrase(QuickPhrase quickPhrase) {
        ResponseResult responseResult = new ResponseResult();
        quickPhraseService.updateQuickPhrase(quickPhrase);
        responseResult.addData("quickPhrase", quickPhrase);
        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "del-quick-phrase")
    @ResponseBody
    public ResponseResult delQuickPhrase(String quickPhraseIds) {
        ResponseResult responseResult = new ResponseResult();
        quickPhraseService.deleteQuickPhrase(com.feiniu.fnSubAccount.util.StringUtils.stringToIntegerList(quickPhraseIds, ","));
        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "config", method = RequestMethod.GET)
    public String getQuickReplyIndex(ModelMap modelMap) {
        return "relative-settings/phrase";
    }



    @RequestMapping(value = "import-quick-phrase-list", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult importQuickPhraseList(@RequestParam(value = "file") MultipartFile multipartFile) {
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
        //去除第一行表头
        if(dataList.isEmpty())
        {
            return responseResult;
        }
        dataList.remove(0);

        List<QuickPhrase> quickPhraseList = new ArrayList<>(dataList.size());

        for (List<String> row : dataList) {
            if (StringUtils.isEmpty(row.get(1)) || row.get(0).equals(SystemConfig.QUICK_PHRASE_TITILE))
                continue;
            QuickPhrase quickPhrase = new QuickPhrase();
            String phraseGroupName = StringUtils.isEmpty(row.get(2)) ? noGroupName : row.get(2);

            quickPhrase.setQuickCode(getLimitLenString(row.get(0), maxQuickCodeLen));
            quickPhrase.setPhraseSummary(getLimitLenString(row.get(1), maxQuickPhraseLen));
            quickPhrase.setPhraseGroupName(getLimitLenString(phraseGroupName, maxGroupNameLen));
            quickPhrase.setLastOperator(SystemEnv.getCurrentUser().getUserName());
            quickPhraseList.add(quickPhrase);
        }
        quickPhraseService.addQuickPhraseList(quickPhraseList);
        responseResult.addData("importCount", quickPhraseList.size());
        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "export-quick-phrase-excel")
    public void exportQuickPhraseList(QuickPhraseRequest quickPhraseRequest, HttpServletResponse response, HttpServletRequest httpServletRequest) {

        List<QuickPhrase> quickPhraseList = quickPhraseService.getQuickPhraseList(quickPhraseRequest,true);
        String[][] content = new String[quickPhraseList.size()][exportExcelHeaders.length];
        for (int i = 0; i < quickPhraseList.size(); i++) {
            QuickPhrase quickPhrase = quickPhraseList.get(i);
            content[i][0] = quickPhrase.getQuickCode();
            content[i][1] = quickPhrase.getPhraseSummary();
            content[i][2] = quickPhrase.getPhraseGroupName();
        }

        ExcelUtils.generateExcelStream(
                "快捷短语" + FormatUtils.formatTime(new Date()) + ".xlsx",
                "默认表单", exportExcelHeaders, content, httpServletRequest, response);
    }

    @RequestMapping(value = "get-group-list")
    @ResponseBody
    public ResponseResult getPhraseGroupList(QuickPhraseGroupRequest quickPhraseGroupRequest) {
        ResponseResult responseResult = new ResponseResult();
        responseResult.addData("phraseGroup", quickPhraseService.getQuickPhraseGroupList(quickPhraseGroupRequest));
        responseResult.addData("totalNum", quickPhraseService.phraseGroupCount(quickPhraseGroupRequest));
        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "update-group")
    @ResponseBody
    public ResponseResult updateGroupName(QuickPhraseGroupRequest quickPhraseGroupRequest) {
        ResponseResult responseResult = new ResponseResult();
        quickPhraseService.updateGroupName(quickPhraseGroupRequest);
        responseResult.markSuccess();
        return responseResult;
    }

}
