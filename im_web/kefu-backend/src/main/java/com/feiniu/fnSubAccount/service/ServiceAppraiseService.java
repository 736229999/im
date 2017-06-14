package com.feiniu.fnSubAccount.service;


import com.feiniu.fnSubAccount.bean.AppraiseStat;
import com.feiniu.fnSubAccount.bean.ServiceAppraiseDetailStat;
import com.feiniu.fnSubAccount.bean.ServiceAppraiseStat;
import com.feiniu.fnSubAccount.bean.SubMerchantInfo;
import com.feiniu.fnSubAccount.bean.request.ServiceAppraiseDetailRequest;
import com.feiniu.fnSubAccount.bean.request.ServiceAppraiseRequest;
import com.feiniu.fnSubAccount.domain.ChatMessage;
import com.feiniu.fnSubAccount.repository.ChatMessageDao;
import com.feiniu.fnSubAccount.repository.ServiceAppraiseManager;
import com.feiniu.fnSubAccount.util.FormatUtils;
import com.feiniu.fnSubAccount.util.Pager;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by wangchen on 2015/2/12.
 */
@Service
@Transactional
public class ServiceAppraiseService {
    @Autowired
    private ServiceAppraiseManager serviceAppraiseManager;
    @Autowired
    private ChatMessageDao chatMessageDao;
    @Autowired
    private DirectorService directorService;

    public Map<Long ,AppraiseStat> getServiceAppraiseList(ServiceAppraiseRequest serviceAppraiseRequest) {

        if(serviceAppraiseRequest.getSubAccountIDListStr() == null)
            return new HashMap<>();

        List<ServiceAppraiseStat> result  = serviceAppraiseManager.getServiceAppraiseList(serviceAppraiseRequest);

        Map<Long ,AppraiseStat> idToAppraiseStatMap =  new HashMap<Long, AppraiseStat>();

        List<SubMerchantInfo> subMerchantInfoList = directorService.getSubMerchantInfoListByDirectorId();

        Map<Long, SubMerchantInfo> subMerchantInfoMap = FormatUtils.convertSubMerchantInfListToMap(subMerchantInfoList);

        FormatUtils.convertServiceAppraiseStatToAppraiseStatMap(idToAppraiseStatMap,result,serviceAppraiseRequest.getSubAccountIDList());

        for (Map.Entry<Long ,AppraiseStat> entry:idToAppraiseStatMap.entrySet()){
            AppraiseStat index = entry.getValue();
            index.setGoodRate(FormatUtils.formatAppraiseRate(index.getAppraiseCount(),0));
            SubMerchantInfo indexInfo = subMerchantInfoMap.get(Long.valueOf(index.getSubMerchantId()));
            if (indexInfo != null)
                index.setSubMerchantName(indexInfo.getUsername());
        }

        return idToAppraiseStatMap;
    }

    public List<ServiceAppraiseDetailStat> getServiceAppraiseList(ServiceAppraiseDetailRequest serviceAppraiseDetailRequest) {

        serviceAppraiseDetailRequest.getPageSize();
        int startRow = (serviceAppraiseDetailRequest.getCurrentPageNo()-1) *
                serviceAppraiseDetailRequest.getPageSize() +1;

        int endRow = startRow + serviceAppraiseDetailRequest.getPageSize() -1;

        List<ServiceAppraiseDetailStat> result =  serviceAppraiseManager.getServiceAppraiseList(serviceAppraiseDetailRequest.getStartTime(),serviceAppraiseDetailRequest.getEndTime(),
                serviceAppraiseDetailRequest.getSubMerchantID(),serviceAppraiseDetailRequest.getSearchType(),startRow,endRow);

        for (ServiceAppraiseDetailStat index : result){
            FormatUtils.formatServiceAppraiseDetailStat(index);
        }

        return  result;
    }


    public Pager<ChatMessage> getHistoryMessageListById(ServiceAppraiseDetailRequest serviceAppraiseDetailRequest) {
        Pager<ChatMessage> pager = new Pager<ChatMessage>();

        int startRow = (serviceAppraiseDetailRequest.getCurrentPageNo()-1) *
                serviceAppraiseDetailRequest.getPageSize() ;

        int endRow = startRow + serviceAppraiseDetailRequest.getPageSize() -1;

        long resultSize= chatMessageDao.getHistoryChatMessageCountByid(serviceAppraiseDetailRequest.getHistorySessionId(),
                Long.valueOf(serviceAppraiseDetailRequest.getSubMerchantID()));

        pager.setCurrentPageNo(serviceAppraiseDetailRequest.getHistoryChatCurrentPageNo());
        pager.setPageSize(serviceAppraiseDetailRequest.getHistoryChatPageSize());
        if (resultSize != 0) {
            pager.setResultsForCurrentPage(chatMessageDao.getHistoryChatMessageListByid(serviceAppraiseDetailRequest.getHistorySessionId(),
                    Long.valueOf(serviceAppraiseDetailRequest.getSubMerchantID()), startRow, endRow));
        }

        pager.setTotalRecordSize(resultSize);

        long pageNum = pager.getTotalRecordSize() / pager.getPageSize();
        pager.setTotalPageNum(pager.getTotalRecordSize() % pager.getPageSize() == 0 ? pageNum : pageNum + 1);

        return pager;
    }
}
