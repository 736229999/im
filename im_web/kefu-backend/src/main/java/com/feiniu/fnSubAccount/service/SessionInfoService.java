package com.feiniu.fnSubAccount.service;

import com.feiniu.fnSubAccount.bean.CSAccountInfo;
import com.feiniu.fnSubAccount.bean.SessionInfo.*;
import com.feiniu.fnSubAccount.consts.FromPortTypeEnum;
import com.feiniu.fnSubAccount.consts.SessionInfoConfig;
import com.feiniu.fnSubAccount.consts.SystemConfig;
import com.feiniu.fnSubAccount.domain.ChatMessage;
import com.feiniu.fnSubAccount.domain.UserInfo;
import com.feiniu.fnSubAccount.repository.CSAccountDao;
import com.feiniu.fnSubAccount.repository.ChatMessageDao;
import com.feiniu.fnSubAccount.repository.SessionInfoDao;
import com.feiniu.fnSubAccount.util.FormatUtils;
import com.feiniu.fnSubAccount.util.Pager;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.math.BigDecimal;
import java.math.RoundingMode;
import java.text.SimpleDateFormat;
import java.util.*;

/**
 * Created by wangzhen on 2016/4/6.
 */
@Service
public class SessionInfoService {

    @Autowired
    SessionInfoDao sessionInfoDao;
    @Autowired
    ChatMessageDao chatMessageDao;
    @Autowired
    CSAccountDao csAccountDao;

    private List<SessionCountInfoShow> covertSessionCountInfoToShow(List<SessionCountInfo> resultList ){
        List<SessionCountInfoShow> result = new ArrayList<>();
        Map<Integer,BigDecimal> resultMap = new HashMap<>();
        BigDecimal totalCount = new BigDecimal(0);
        if (resultList != null){
            for (SessionCountInfo index: resultList){
                resultMap.put(index.getSessionTime().intValue(),index.getSessionCount());
                totalCount = totalCount.add(index.getSessionCount());
            }
        }
        boolean isAllZero = totalCount.intValue() == 0;

        Integer beginTime = 0;
        for (;beginTime<24;beginTime++ ){
            SessionCountInfoShow indexShow = new SessionCountInfoShow();
            indexShow.setTime(beginTime.toString()+":00:00-"+beginTime.toString()+":59:59");
            BigDecimal sessionCount = resultMap.get(beginTime);
            if (sessionCount == null) {
                indexShow.setSessionCount(0);
                indexShow.setSessionRate("0");
            }
            else {
                indexShow.setSessionCount(sessionCount.intValue());
                if (isAllZero)
                    indexShow.setSessionRate("0");
                else
                    indexShow.setSessionRate(
                            sessionCount.multiply(new BigDecimal(100))
                                    .divide(totalCount, SystemConfig.DEFAULT_SCALE, RoundingMode.HALF_UP).toString()
                    );
            }
            result.add(indexShow);
        }
        return  result;
    }

    public List<SessionCountInfoShow> getSessionCountInfo(Long startTime,Long endTime){

        List<SessionCountInfo> resultList = sessionInfoDao.getSessionCountInfo(startTime, endTime);

        return covertSessionCountInfoToShow(resultList);
    }

    private SessionInfoItemShow convertSessionItemToShow(SessionInfoItem sessionInfoItem){
        SessionInfoItemShow result = new SessionInfoItemShow();

        SimpleDateFormat format =   new SimpleDateFormat( "yyyy-MM-dd HH:mm:ss" );

        result.setCustomerNameStr(sessionInfoItem.getCustomerId().toString());
        result.setServiceNameStr(sessionInfoItem.getServiceId().toString());

        result.setCustomerWaitingTimeStr(
                FormatUtils.formatNumberToSecTime(sessionInfoItem.getCustomerWaitingTimeSql()));
        if (sessionInfoItem.getFromDeviceSql() == SessionInfoConfig.PC_TYPE){
            //pc
            result.setMobilePortStr("--");
            result.setPcPortStr(FromPortTypeEnum.getInfo(sessionInfoItem.getFromPageSql()).getShowInfo());
        }else {
            //Mobile
            result.setPcPortStr("--");
            result.setMobilePortStr(FromPortTypeEnum.getInfo(sessionInfoItem.getFromPageSql()).getShowInfo());
        }

        result.setSessionEndTimeStr(format.format(new Date(Long.parseLong(sessionInfoItem.getSessionEndTimeSql().toString())*1000)));
        result.setSessionLengthStr(FormatUtils.formatNumberToSecTime(sessionInfoItem.getSessionLengthSql()));


        result.setSessionStartTimeStr(format.format(new Date(Long.parseLong(sessionInfoItem.getSessionStartTimeSql().toString())*1000)));

        result.setSessionID(sessionInfoItem.getSessionID());
        return result;
    }

    private Map<Long,UserInfo> getCustomerIdMap(List<SessionInfoItem> sessionInfoItemList){
        if (sessionInfoItemList.size() == 0)
            return Collections.EMPTY_MAP;

        StringBuilder customerIdList = new StringBuilder("");

        for (SessionInfoItem sessionInfoItem : sessionInfoItemList){
            customerIdList.append(sessionInfoItem.getCustomerId()+",");
        }
        List<UserInfo> allUsers = sessionInfoDao.getCustomerListByIds(
                customerIdList.substring(0,customerIdList.length()-1));

        Map<Long,UserInfo> allUserMap = new HashMap<>();
        for (UserInfo userIndex : allUsers){
            allUserMap.put(Long.valueOf(userIndex.getUserID()),userIndex);
        }

        return allUserMap;
    }
    private Map<Long,CSAccountInfo> getServiceIdMap(){
        //填充客服的真实姓名和用户名
        List<CSAccountInfo> allIdList = csAccountDao.getAccountIdList(0, SystemConfig.MAX_PAGE_ITEM_SIZE);
        Map<Long,CSAccountInfo> csAccountInfoMap = new HashMap<>();
        for (CSAccountInfo accountInfo : allIdList){
            csAccountInfoMap.put(accountInfo.getUserid(),accountInfo);
        }
        return  csAccountInfoMap;
    }

    private SessionInfoItemShow getTotalInfoItem(SessionTotalInfo sessionTotalInfo){
        SessionInfoItemShow sessionInfoItemShow = new SessionInfoItemShow();
        sessionInfoItemShow.setSessionStartTimeStr("均值/详情");
        sessionInfoItemShow.setSessionEndTimeStr("--");
        sessionInfoItemShow.setCustomerWaitingTimeStr("--");
        sessionInfoItemShow.setServiceNameStr("--");
        sessionInfoItemShow.setCustomerNameStr("--");
        sessionInfoItemShow.setSessionLengthStr(
                FormatUtils.formatNumberToSecTime(sessionTotalInfo.getAvgLen()));
        sessionInfoItemShow.setCustomerWaitingTimeStr(
                FormatUtils.formatNumberToSecTime(sessionTotalInfo.getAvgWaitingTime()));
        sessionInfoItemShow.setMobilePortStr("-1");
        sessionInfoItemShow.setPcPortStr("-1");
        sessionInfoItemShow.setSessionID("-1");
        return sessionInfoItemShow;
    }
    @Transactional
    public Pager<SessionInfoItemShow> getSessionInfo(Long startTime ,Long endTime ,
                                                 String customerName, Long serviceId,
                                                 Integer pageNo,Integer pageSize){
        Pager<SessionInfoItemShow> pager = new Pager<SessionInfoItemShow>();
        Integer startRow = (pageNo-1) * pageSize - 1;
        Integer endRow = pageNo * pageSize -1 ;
        Long customerId = null;

        //非法性判断
        if (customerName == null || customerName.isEmpty()){
            customerId = null;
        }else{
            customerId = sessionInfoDao.getCustomerIdByName(customerName);
            if (customerId == null)
                return null;
        }
        //获取当前页要显示的内容
        List<SessionInfoItem> sessionInfoItemList = sessionInfoDao.getSessionInfoList(
                startTime,endTime,customerId,serviceId,startRow,endRow);

        //获取汇总信息
        SessionTotalInfo sessionTotalInfo = sessionInfoDao.getSessionInfoCount(startTime,endTime,customerId,
                serviceId);


        //第一页加入汇总信息
        List<SessionInfoItemShow> sessionInfoItemListShow = new ArrayList<>();
        if (pageNo == 1&& sessionInfoItemList.size() !=0 ){
            SessionInfoItemShow sessionInfoItemShow = getTotalInfoItem(sessionTotalInfo);
            sessionInfoItemListShow.add(sessionInfoItemShow);
        }
        //获取客服和客户的基本信息
        Map<Long,UserInfo> allUserMap = getCustomerIdMap(sessionInfoItemList);
        Map<Long,CSAccountInfo> csAccountInfoMap= getServiceIdMap();


        for (SessionInfoItem sessionInfoItem : sessionInfoItemList){
            SessionInfoItemShow sessionInfoItemShow = convertSessionItemToShow(sessionInfoItem);
            UserInfo user = allUserMap.get(Long.valueOf(sessionInfoItem.getCustomerId()));
            CSAccountInfo csAccountInfo=csAccountInfoMap.get(Long.valueOf(sessionInfoItem.getServiceId()));
            if (csAccountInfo == null)
                sessionInfoItemShow.setServiceNameStr("--");
            else
                sessionInfoItemShow.setServiceNameStr(csAccountInfo.getUserName()+
                        "("+csAccountInfo.getRealName()+")");

            sessionInfoItemShow.setCustomerNameStr(user.getNickName()+
                    "("+SystemConfig.USER_LEVEL_DESC[sessionInfoItem.getCustomerLevel()]+")");
            sessionInfoItemListShow.add(sessionInfoItemShow);
        }

        Integer pageCount = (sessionTotalInfo.getRecodeSize() +1 )/ pageSize +
                (sessionTotalInfo.getRecodeSize() +1 )%pageSize ==0 ?0:1;
        pager.setResultsForCurrentPage(sessionInfoItemListShow);
        pager.setCurrentPageNo(pageNo);
        pager.setPageSize(pageSize);
        pager.setTotalPageNum(pageCount);
        pager.setTotalRecordSize(sessionTotalInfo.getRecodeSize() +1 );

        return pager;

    }

    @Transactional
    public List<SessionProfileItem> getSessionProfile(Long startTime ,Long endTime ,
                                                       String customerName, Long serviceId,Integer type){

        Long customerId= null;
        boolean needCustomer=true;
        if (customerName == null || customerName.isEmpty()){
            customerId = null;
            needCustomer =false;
        }else{
            customerId = sessionInfoDao.getCustomerIdByName(customerName);
            needCustomer =true;
        }
        List<SessionProfileItem> sessionInfoItemList ;
        List<SessionProfileItem> sessionInfoItemListResult  = new ArrayList<>();
        if (customerId == null && needCustomer){
            sessionInfoItemList = Collections.EMPTY_LIST;
        }else {
            sessionInfoItemList = sessionInfoDao.getSessionProfile(type, startTime, endTime, customerId,
                    serviceId);
        }

        Map<Integer,SessionProfileItem> sessionProfileItemMap = new HashMap<>();
        for (SessionProfileItem sessionProfileItem : sessionInfoItemList){
            sessionProfileItemMap.put(sessionProfileItem.getTypeSql(),sessionProfileItem);
        }

        Map<Integer, FromPortTypeEnum> idToInfoMap = FromPortTypeEnum.getIdToInfo();
        for (Map.Entry<Integer, FromPortTypeEnum> mapIndex:idToInfoMap.entrySet()){
            if (sessionProfileItemMap.get(mapIndex.getKey()) == null){
                SessionProfileItem sessionProfileItem = new SessionProfileItem();
                sessionProfileItem.setTypeSql(mapIndex.getKey());
                sessionProfileItem.setTitle(mapIndex.getValue().getShowInfo());
                sessionProfileItem.setCount(0);
                sessionInfoItemListResult.add(sessionProfileItem);
            }else {
                sessionProfileItemMap.get(mapIndex.getKey()).setTitle(mapIndex.getValue().getShowInfo());
                sessionInfoItemListResult.add(sessionProfileItemMap.get(mapIndex.getKey()));
            }
        }
        return sessionInfoItemListResult;

    }

    @Transactional
    public Pager<ChatMessage> getHistoryMessageListById(Long sessionId,Integer pageNo, Integer pageSize) {
        Pager<ChatMessage> pager = new Pager<ChatMessage>();

        int startRow = (pageNo-1) *pageSize;
        long resultSize= chatMessageDao.getCsgetCsHistoryChatMessageLenBySession(sessionId);

        pager.setCurrentPageNo(pageNo);
        pager.setPageSize(pageSize);
        if (resultSize != 0) {
            pager.setResultsForCurrentPage(chatMessageDao.getCsHistoryChatMessageListBySession(sessionId, startRow, pageSize));
        }

        pager.setTotalRecordSize(resultSize);

        long pageNum = pager.getTotalRecordSize() / pager.getPageSize();
        pager.setTotalPageNum(pager.getTotalRecordSize() % pager.getPageSize() == 0 ? pageNum : pageNum + 1);

        return pager;
    }

    @Transactional
   public  List<CsAccountName> getCsAccountInfoListByPartName(String partName){
       return sessionInfoDao.getCsIdListByPartName(partName);
    }
}
