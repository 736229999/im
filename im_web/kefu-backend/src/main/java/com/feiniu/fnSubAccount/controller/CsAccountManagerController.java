package com.feiniu.fnSubAccount.controller;

import com.feiniu.common.core.SystemEnv;
import com.feiniu.common.core.UserSession;
import com.feiniu.fnSubAccount.base.ResponseResult;
import com.feiniu.fnSubAccount.bean.*;
import com.feiniu.fnSubAccount.bean.response.CSModifyStatusRequest;
import com.feiniu.fnSubAccount.bean.response.CsModifyDetailRequest;
import com.feiniu.fnSubAccount.bean.response.CsModifyPasswordRequest;
import com.feiniu.fnSubAccount.service.CSAccountService;
import com.feiniu.fnSubAccount.service.external.BackendService;
import com.feiniu.fnSubAccount.util.ExcelReader;
import com.feiniu.fnSubAccount.util.Pager;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.util.StringUtils;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.multipart.MultipartFile;

import java.text.ParseException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by wangzhen on 2015/11/18.
 */
@Controller
@RequestMapping(value = "account")
public class CsAccountManagerController {
    @Autowired
    private CSAccountService csAccountService;
    @Autowired
    private BackendService backendService;

    //从excel批量导入用户数据请求
    @RequestMapping(value = "batch-add", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult batchAddEmployeeList(@RequestParam("file") MultipartFile photoFile) {

        ResponseResult responseResult = new ResponseResult();

        ExcelReader excelReader = null;
        try {
            excelReader = new ExcelReader(photoFile.getInputStream(), 0);
        } catch (Exception e) {
            responseResult.setMessage("excel文件格式存在问题");
            return responseResult;
        }

        /**表体数据**/
        List<List<String>> dataList = excelReader.getListData();
        /**表头数据**/
        List<String> headerList = dataList.get(0);
        List<CSAccountResponse> csExcelImportList = new ArrayList<>();

        //获取导入人员名称
        UserSession userSession = SystemEnv.getCurrentUser();
        CSModiAddUserReq csModiAddUserReq = new CSModiAddUserReq();
        csModiAddUserReq.setImportname(userSession.getTrueName());
        csModiAddUserReq.setImportCsId(userSession.getUserId());
        List<CSModiAddUserReq.CSModiAdduserinf> userinfolist = new ArrayList<>();
        csModiAddUserReq.setUserinfo(userinfolist);

        int addSuccessCount = 0;
        int rowIndex = 0;
        Map<Integer, String> errorDetail = new HashMap<>();

        for (List<String> rowList : dataList) {
            //跳过表头
            if (rowIndex++ == 0) {
                continue;
            }

            boolean isContinue = true;
            for (int i = 0; i < rowList.size(); i++) {
                String cell = rowList.get(i);

                if (StringUtils.isEmpty(cell)) {
                    responseResult.setMessage("第" + (i + 1) + "列(" + headerList.get(i) + ")为空：");
                    return responseResult;
                }
            }
            if (!isContinue) {
                continue;
            }

            CSModiAddUserReq.CSModiAdduserinf csModiAdduserinf = csModiAddUserReq.new CSModiAdduserinf();
            csModiAdduserinf.setPermission(1);
            csModiAdduserinf.setUsername(rowList.get(0));
            csModiAdduserinf.setReceptionlimit(Integer.parseInt(rowList.get(1)));
            csModiAddUserReq.getUserinfo().add(csModiAdduserinf);
            addSuccessCount++;
        }
        long currentTime = System.currentTimeMillis() / 1000;
        String resultStr= csAccountService.batchAddEmployeeList(csModiAddUserReq,currentTime);

        if (resultStr.equals("successed")) {
            responseResult.markSuccess();
        }else {
            responseResult.setMessage(resultStr);
            return responseResult;
        }

        Pager<CSAccountResponse> pager = csAccountService.getCSAccountListByTime(currentTime,userSession.getTrueName());
        responseResult.addData("pager",pager);
        return responseResult;
    }

    //查询+刷新请求
    @RequestMapping(value = "get-cs-account-manager", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getCSAccountResponse(CsAccountSearchRequest csAccountSearchRequest) throws ParseException {
        ResponseResult responseResult = new ResponseResult();
        Pager<CSAccountResponse> result = csAccountService.getCSAccountList( csAccountSearchRequest);

        if (result.getResultsForCurrentPage() != null && !result.getResultsForCurrentPage().isEmpty()){
            responseResult.addData("pager",result);
            responseResult.markSuccess();
        }else {
            responseResult.setErrorCode(-2);
            responseResult.setMessage("没有符合的数据");
        }
        return responseResult;

    }

    @RequestMapping(value = "modi-cs-account-status", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult modifyStatus(CSModifyStatusRequest csModifyStatusRequest){
        ResponseResult responseResult = new ResponseResult();

        csAccountService.setStatus(csModifyStatusRequest.getIds(),csModifyStatusRequest.getStatus());
        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "modi-cs-account-detail", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult modifyDetail(CsModifyDetailRequest csModifyDetailRequest){
        ResponseResult responseResult = new ResponseResult();

        csAccountService.setDetail(csModifyDetailRequest.getMoumouId(),csModifyDetailRequest.getMemberId(),
                csModifyDetailRequest.getMemberName(),csModifyDetailRequest.getLimited(),csModifyDetailRequest.getIssupervisor());
        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "modi-cs-account-password", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult modifyPassword(CsModifyPasswordRequest csModifyPasswordRequest){
        ResponseResult responseResult = new ResponseResult();

        csAccountService.setPassword(csModifyPasswordRequest.getMoumouId(), csModifyPasswordRequest.getPassword());
        responseResult.markSuccess();
        return responseResult;
    }


    @RequestMapping(value = "delete-account", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult deleteAccount(CSModifyStatusRequest csModifyStatusRequest){
        ResponseResult responseResult = new ResponseResult();

        csAccountService.deleteAccount(csModifyStatusRequest.getIds());
        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "get-member-List-in-org", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getCSAccountResponse(String orgId){
        ResponseResult responseResult = new ResponseResult();
        List<CSBackendInfo> memberList;
        if (!orgId.equals("0") ) {
             memberList = backendService.getAllMemberListInOrgan(Long.valueOf(orgId));
        }else {
            memberList=backendService.getAllMemberList();
        }
        if (memberList == null){
            responseResult.setMessage("获取成员列表错误");
        }

        responseResult.addData("memberList",memberList);
        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "get-moumou-name-by-part-name", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getMoumouName(String partName){
        ResponseResult responseResult = new ResponseResult();
        List<KVPair<Long,String>> moumouNameMap = csAccountService.getMoumouNameByPartName(partName);
        List< CSAccountResponse> resultList = new ArrayList<>();
        for (KVPair<Long,String> index: moumouNameMap){
            CSAccountResponse csAccountResponse = new CSAccountResponse();
            csAccountResponse.setMoumouUserName(index.getValue());
            csAccountResponse.setMoumouId(index.getKey().toString());
            resultList.add(csAccountResponse);
        }
        responseResult.addData("moumouNameList",resultList);
        responseResult.markSuccess();
        return responseResult;
    }
}
