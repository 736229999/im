package com.feiniu.fnSubAccount.controller;

import com.feiniu.fnSubAccount.base.ResponseResult;
import com.feiniu.fnSubAccount.bean.*;
import com.feiniu.fnSubAccount.bean.request.*;
import com.feiniu.fnSubAccount.bean.response.WorkMessageResponse;
import com.feiniu.fnSubAccount.consts.SystemConfig;
import com.feiniu.fnSubAccount.domain.ChatMessage;
import com.feiniu.fnSubAccount.service.DirectorService;
import com.feiniu.fnSubAccount.service.MonitorService;
import com.feiniu.fnSubAccount.service.external.MerchantService;
import com.feiniu.fnSubAccount.util.ExcelUtils;
import com.feiniu.fnSubAccount.util.FormatUtils;
import com.feiniu.fnSubAccount.util.Pager;
import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.logging.Logger;

/**
 * Created by wangchen on 2015/2/4.
 */
@Controller
@RequestMapping(value = "monitor")
public class MonitorController {
    @Autowired
    private MonitorService monitorService;
    @Autowired
    private MerchantService merchantService;
    @Autowired
    private DirectorService directorService;

    @RequestMapping(value = "get-history-message-list", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getHistoryMessageListPerPage(HistoryMessageListRequest historyMessageListRequest) {
        ResponseResult responseResult = new ResponseResult();

        if (StringUtils.isEmpty(historyMessageListRequest.getDirctorID())
                || historyMessageListRequest.getStartTime() == null || historyMessageListRequest.getEndTime() == null
                || historyMessageListRequest.getIsCustomerService() == null) {
            responseResult.setMessage("参数不全");
        } else {
            if (historyMessageListRequest.getIsCustomerService() != 0) {
                List<SubMerchantInfo> subMerchantInfoList =
                        directorService.getAllSubMerchantInfoList();

                if (subMerchantInfoList.size() == 0) {
                    responseResult.setMessage("没有下属");
                    return responseResult;
                }
                //如果输入了员工号，将其他员工删除
                String subMerchantNamePart = historyMessageListRequest.getSubMerchantNamePart();
                if (subMerchantNamePart != null && subMerchantNamePart != "") {
                    Iterator<SubMerchantInfo> iter = subMerchantInfoList.iterator();
                    while (iter.hasNext()) {
                        SubMerchantInfo s = iter.next();
                        if (0 > s.getUsername().indexOf(subMerchantNamePart) &&
                                (s.getTruename() == null || 0 > s.getTruename().indexOf(subMerchantNamePart))) {
                            iter.remove();
                        }
                    }
                }

                if (subMerchantInfoList.size() == 0) {
                    responseResult.setMessage("指定的员工不存在！");
                    return responseResult;
                }

                Map<Long, SubMerchantInfo> subMerchantInfoMap = FormatUtils.convertSubMerchantInfListToMap(subMerchantInfoList);

                String subMerchantIDlist = FormatUtils.convertSubMerchantInfListToIdList(subMerchantInfoMap);

                historyMessageListRequest.setSubMerchantIDList(subMerchantIDlist);
            } else {
                if (historyMessageListRequest.getStoreName().isEmpty()) {
                    responseResult.setMessage("店铺名称不能为空");
                    return responseResult;
                }

                String merchantId = merchantService.getMerchantIDByStoreName(historyMessageListRequest.getStoreName());

                if (merchantId == null || merchantId == "") {
                    responseResult.setMessage("没找到指定的店铺");
                    return responseResult;
                }
                List<SubMerchantInfo> subMerchantInfoList = merchantService.retrieveSubMerchantInfo(merchantId);

                if (subMerchantInfoList == null || subMerchantInfoList.size() == 0) {
                    responseResult.setMessage("店铺没有客服！");
                    return responseResult;
                }
                String idList = "";
                for (SubMerchantInfo index : subMerchantInfoList) {
                    idList += index.getMemberId() + ",";
                }
                idList = idList.substring(0, idList.length() - 1);
                historyMessageListRequest.setSubMerchantIDList(idList);
            }

            Pager<ChatMessage> result = monitorService.getHistoryMessageList(historyMessageListRequest);

            if (result.getTotalRecordSize() == 0) {
                responseResult.setMessage("没有聊天记录");
                return responseResult;
            }

            responseResult.addData("pager", result);
            //responseResult.addData("servicerIDList", subMerchantIDList);
            responseResult.markSuccess();

        }

        return responseResult;
    }

    @RequestMapping(value = "get-history-message-list-within-session", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getHistoryMessageListWithinSessionPerPage(GetHistoryMessageListWithSessionRequest request) {
        ResponseResult responseResult = new ResponseResult();

        responseResult.addData("list", monitorService.getHistoryMessageListWithinSession(request));
        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "get-work-message-list", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getWorkMessageResponse(WorkMessageListRequest workMessageListRequest) {
        ResponseResult responseResult = new ResponseResult();
        WorkMessageRequestParams params = new WorkMessageRequestParams();

        FormatUtils.convertRequestToParams(workMessageListRequest, params);

        Pager<WorkMessageResponse> result = null;

        long checkResult = monitorService.checkParams(workMessageListRequest);

        if (checkResult == 0) {
            result = monitorService.getWorkMessageList(params);
            if (null == result)
                return FormatUtils.formatErrorResponse(responseResult, -3);
        } else if (checkResult > 0)
            result = monitorService.getWorkMessage(params, checkResult);
        else {
            return FormatUtils.formatErrorResponse(responseResult, checkResult);
        }

        responseResult.addData("pager", result);
        responseResult.addData("servicerIDList", workMessageListRequest.getSubMerchantIDList());
        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "get-customer-service-info")
    @ResponseBody
    public ResponseResult getSubMerchantInfoByPartName(String merchantId, String subMerchantNamePart) {
        ResponseResult responseResult = new ResponseResult();

        List<SubMerchantInfo> subMerchantInfoPairList =
                merchantService.retrieveSubMerchantInfoByPartName(merchantId, subMerchantNamePart);
        responseResult.addData("subMerchantInfoPairList", subMerchantInfoPairList);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "get-store-info-pair")
    @ResponseBody
    public ResponseResult getStoreInfoPairByStorePartName(String storeNamePart) {
        ResponseResult responseResult = new ResponseResult();

        List<StoreInfoPair> storeInfoPairList =
                merchantService.getStoreInfoPairListByStoreNamePart(storeNamePart);
        responseResult.addData("storeInfoPairList", storeInfoPairList);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "get-merchant-info")
    @ResponseBody
    public ResponseResult getMerchantInfoByMerchantName(String MerchantName) {
        ResponseResult responseResult = new ResponseResult();

        List<Merchant> merchantInfoPairList =
                merchantService.getMerchantInfoListByMerchantName(MerchantName);
        responseResult.addData("merchantInfoList", merchantInfoPairList);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "update-work-detail", method = RequestMethod.GET)
    public void getWorkMessageListPerPage(UpdateWorkDetailRequest updateWorkDetailRequest, HttpServletResponse response) {
        Long startTime = updateWorkDetailRequest.getStartTime();
        Long endTime = updateWorkDetailRequest.getEndTime();

        while (startTime < endTime) {
            monitorService.doUpdateImWorkDetail(startTime, startTime + SystemConfig.STATISITC_ONE_DAY_SECOUNDS);
            startTime += SystemConfig.STATISITC_ONE_DAY_SECOUNDS;
        }

    }


    @RequestMapping(value = "get-excel-file", method = RequestMethod.GET)
    public void exportExcelHandler(WorkMessageListRequest workMessageListRequest, HttpServletResponse response, HttpServletRequest httpServletRequest) {
        String[] fieldNameList = {"商家名称", "商家编码", "客服个数（个）", "店铺名称", "主营经营大类", "累计登录总时长（时）", "咨询量(人)", "接待量（人）",
                "咨询次数", "接待次数", "回复率", "平均首次回复时间（秒）", "90s在线响应率", "店铺状态"};
        WorkMessageRequestParams params = new WorkMessageRequestParams();

        FormatUtils.convertRequestToParams(workMessageListRequest, params);

        String[][] values = null;

        long checkResult = monitorService.checkParams(workMessageListRequest);

        if (checkResult == 0)
            values = monitorService.getWorkMessageListExcelExport(params);
        else if (checkResult > 0)
            values = monitorService.getWorkMessageListExport(params, checkResult);

        if (values == null)
            ExcelUtils.generateEmptyExcelStream("商家工时统计报表.xlsx", "默认表单", httpServletRequest, response);
        else
            ExcelUtils.generateExcelStream("商家工时统计报表.xlsx", "默认表单", fieldNameList, values, httpServletRequest, response);
    }
    @RequestMapping(value = "send-Inner-Mail")
    @ResponseBody
    public ResponseResult onlineTimeWarn(String infoTitle,String messageContent){
        ResponseResult result = new ResponseResult();
        monitorService.sendOnlineTimeWarn(infoTitle,messageContent);
        result.addData("aa","aaa");
        result.markSuccess();
        return result;
    }
}
