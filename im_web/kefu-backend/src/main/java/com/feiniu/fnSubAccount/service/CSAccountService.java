package com.feiniu.fnSubAccount.service;

import com.feiniu.fnSubAccount.bean.*;
import com.feiniu.fnSubAccount.constants.BusinessConfig;
import com.feiniu.fnSubAccount.repository.CSAccountDao;
import com.feiniu.fnSubAccount.service.external.BackendService;
import com.feiniu.fnSubAccount.service.external.ImApiService;
import com.feiniu.fnSubAccount.service.external.ImService;
import com.feiniu.fnSubAccount.util.AccountUtil.AccountFormatUtil;
import com.feiniu.fnSubAccount.util.FormatUtils;
import com.feiniu.fnSubAccount.util.Pager;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.ArrayList;
import java.util.List;

/**
 * Author: duyong
 * Date: 2015/9/29
 * Contact:
 */
@SuppressWarnings({"unused", "unchecked", "rawtypes"})
@Service
@Transactional
public class CSAccountService {

    @Autowired
    private BusinessConfig businessConfig;

    @Autowired
    private ImService imService;
    @Autowired
    private BackendService backendService;
    @Autowired
    private CSAccountDao csAccountDao;
    @Autowired
    private ImApiService imApiService;

    private static final Logger LOGGER = LoggerFactory.getLogger(CSAccountService.class);

    //将查询之后的整合数据提交到js
    public Pager<CSAccountResponse> getCSAccountList(CsAccountSearchRequest csAccountSearchRequest) {
        Pager<CSAccountResponse> pager = new Pager<CSAccountResponse>();

        CSAccountQuery csAccountQuery = new CSAccountQuery();
        String moumouName = csAccountSearchRequest.getMoumouName();
        int status = csAccountSearchRequest.getStatus();
        long startTime = FormatUtils.getMinStartTime(csAccountSearchRequest.getStartTime());
        long endTime = FormatUtils.getMaxEndTime(csAccountSearchRequest.getEndTime());
        int fromIndex = (csAccountSearchRequest.getPageNo() - 1) * csAccountSearchRequest.getPageSize();
        int toIndex = csAccountSearchRequest.getPageNo() * csAccountSearchRequest.getPageSize() + 1;
        Integer resultCount = csAccountDao.searchCsAccountInfoCount(moumouName, csAccountSearchRequest.getCsIdList(),
                csAccountSearchRequest.getImportCsIdList(), status, startTime, endTime);
        if (resultCount == null || resultCount == 0) {
            pager.setCurrentPageNo(csAccountSearchRequest.getPageNo());
            pager.setPageSize(csAccountSearchRequest.getPageSize());
            pager.setResultsForCurrentPage(new ArrayList<CSAccountResponse>());
            pager.setTotalRecordSize(0);
            return pager;
        }

        List<CSAccountResponse> result = csAccountDao.searchCsAccountInfo(moumouName, csAccountSearchRequest.getCsIdList(),
                csAccountSearchRequest.getImportCsIdList(), status, startTime, endTime, fromIndex, toIndex);

        for (CSAccountResponse index : result) {
            AccountFormatUtil.formatAccountInfo(index);
        }
        pager.setCurrentPageNo(csAccountSearchRequest.getPageNo());
        pager.setPageSize(csAccountSearchRequest.getPageSize());
        pager.setResultsForCurrentPage(result);
        pager.setTotalRecordSize(resultCount);
        return pager;
    }

    public Pager<CSAccountResponse> getCSAccountListByTime(Long importTime, String importName) {
        Pager<CSAccountResponse> pager = new Pager<CSAccountResponse>();

        List<CSAccountResponse> result = csAccountDao.searchCsAccountInfoByTime(importTime, importName);
        for (CSAccountResponse index : result) {
            AccountFormatUtil.formatAccountInfo(index);
        }
        //不用过滤组织，在js中过滤，因为没有批量调用接口，暂时先这样做
        pager.setCurrentPageNo(1);
        pager.setPageSize(result.size());
        pager.setResultsForCurrentPage(result);
        pager.setTotalRecordSize(result.size());
        return pager;
    }

    public String batchAddEmployeeList(CSModiAddUserReq csModiAddUserReq, long currentTime) {
        List<CSModiAddUserReq.CSModiAdduserinf> csModiAdduserinf = csModiAddUserReq.getUserinfo();
        String errorString = "successed";
        boolean insertResult = false;
        for (CSModiAddUserReq.CSModiAdduserinf index : csModiAdduserinf) {
            insertResult = csAccountDao.insertToCSInfo(index.getUsername(), index.getPermission(), csModiAddUserReq.getImportname(),
                    currentTime, index.getReceptionlimit(), 1, csModiAddUserReq.getImportCsId());
            if (!insertResult)
                return index.getUsername() + "导入失败";
        }
        return errorString;
    }

    public boolean setStatus(String ids, Integer status) {
        if (ids == null || status == null)
            return false;
        return csAccountDao.setStatus(ids, status);
    }

    public boolean deleteAccount(String ids) {
        if (ids == null)
            return false;
        return csAccountDao.deleteAccount(ids);
    }

    public boolean setDetail(long moumouId, long csId, String csName, int limitNumber, int issupervisor) {

        imApiService.modifyReceptionLimitInRedis(moumouId, limitNumber);
        return csAccountDao.setDetail(moumouId, csId, csName, limitNumber, issupervisor);
    }

    public boolean setPassword(long moumouId, String password) {
        return csAccountDao.setPassword(moumouId, password);
    }

    public List<KVPair<Long, String>> getMoumouNameByPartName(String partName) {
        return csAccountDao.getMoumouNameByPartName(partName);
    }

    public Long getUserIDInMouMou(Long userIDInBackend) {
        return csAccountDao.getUserIDInMouMou(userIDInBackend);
    }
}
