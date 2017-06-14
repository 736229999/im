/**
 * Created by wangchen on 2015/2/4.
 */
package com.feiniu.fnSubAccount.service;

import com.feiniu.common.core.SystemEnv;
import com.feiniu.common.core.UserSession;
import com.feiniu.fnSubAccount.bean.Merchant;
import com.feiniu.fnSubAccount.bean.StoreInfo;
import com.feiniu.fnSubAccount.bean.SubMerchantInfo;
import com.feiniu.fnSubAccount.bean.WorkMessageRequestParams;
import com.feiniu.fnSubAccount.bean.request.GetHistoryMessageListWithSessionRequest;
import com.feiniu.fnSubAccount.bean.request.HistoryMessageListRequest;
import com.feiniu.fnSubAccount.bean.request.WorkMessageListRequest;
import com.feiniu.fnSubAccount.bean.response.WorkMessageResponse;
import com.feiniu.fnSubAccount.bean.response.WorkMessageTotalInfoResponse;
import com.feiniu.fnSubAccount.consts.SystemConfig;
import com.feiniu.fnSubAccount.domain.ChatMessage;
import com.feiniu.fnSubAccount.domain.MerchantLogInOutInfo;
import com.feiniu.fnSubAccount.repository.ChatMessageDao;
import com.feiniu.fnSubAccount.repository.CustomerServiceAuthDao;
import com.feiniu.fnSubAccount.repository.WorkMessageDao;
import com.feiniu.fnSubAccount.service.external.InfoCenterApiClient;
import com.feiniu.fnSubAccount.service.external.MerchantService;
import com.feiniu.fnSubAccount.service.external.StoreService;
import com.feiniu.fnSubAccount.util.FormatUtils;
import com.feiniu.fnSubAccount.util.Pager;
import com.feiniu.fnSubAccount.util.web.HttpUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.*;

/**
 * Created by wangchen on 2015/2/4.
 */
@Service
@Transactional
public class MonitorService {
    @Autowired
    private ChatMessageDao chatMessageDao;
    @Autowired
    private WorkMessageDao workMessageDao;
    @Autowired
    MerchantService merchantService;
    @Autowired
    StoreService storeService;
    @Autowired
    CustomerServiceAuthDao customerServiceAuthDao;
    @Autowired
    DirectorService directorService;
    @Autowired
    InfoCenterApiClient infoCenterApiClient;

    private static final Logger LOGGER = LoggerFactory.getLogger(MonitorService.class);


    //查找会话内的聊天消息
    public List<ChatMessage> getHistoryMessageListWithinSession(GetHistoryMessageListWithSessionRequest request) {
        return chatMessageDao.getHistoryChatMessageListWithinSession(request);
    }

    /*
    * 获取聊天记录的相关函数
    * */
    public Pager<ChatMessage> getHistoryMessageList(HistoryMessageListRequest historyMessageListRequest) {
        Pager<ChatMessage> pager = new Pager<ChatMessage>();

        pager.setCurrentPageNo(historyMessageListRequest.getCurrentPageNo());
        pager.setPageSize(historyMessageListRequest.getPageSize());

        pager.setResultsForCurrentPage(chatMessageDao.getHistoryChatMessageListByIdList(historyMessageListRequest));
        pager.setTotalRecordSize(chatMessageDao.getTotalChatMessageNumberByIdList(historyMessageListRequest));

        long pageNum = pager.getTotalRecordSize() / pager.getPageSize();
        pager.setTotalPageNum(pager.getTotalRecordSize() % pager.getPageSize() == 0 ? pageNum : pageNum + 1);

        return pager;
    }

    /*
    * 定时任务相关函数
    * */

    public void insertBuildStatus() {
        //获取所有商家id
        Long currentTime = FormatUtils.getTodayTimeInSeconds();

        String ipAddress = "0.0.0.0";
        //判断是否已经更新过了，返回0表示没有更新，返回1表示已经更新过了，3表示正在被其他服务器更新。
        if (0 == workMessageDao.insertIntoStatus(currentTime, SystemConfig.STATISITC_STATUS_NOT_STATISITCED, ipAddress)) {
            LOGGER.info(currentTime.toString() + ": 统计项已经存在");
            return;
        }
    }

    //更新定时任务执行状态表
    public int updateBuildStatus(Long currentTime, int statisitcStatus, String ipAddress) {
        Integer status = SystemConfig.STATISITC_STATUS_NODATA;
        Integer retrytime = 0;
        //锁住一行
        Map<String, Object> returnParams = workMessageDao.lockUpdateStatus(currentTime);
        if (returnParams == null) {
            LOGGER.error("已经有其他用户在进行统计");
            return -1;
        }
        status = Integer.valueOf(returnParams.get("status").toString());
        retrytime = Integer.valueOf(returnParams.get("retrytime").toString());

        if (status == SystemConfig.STATISITC_STATUS_NODATA) {
            //没有数据，插入统计项
            workMessageDao.insertIntoStatus(currentTime, statisitcStatus, ipAddress);
            return 0;
        } else if (status == SystemConfig.STATISITC_STATUS_UPDATED) {
            //已经update成功过了,
            return -1;
        } else if (status == SystemConfig.STATISITC_STATUS_UPDATING) {
            //update结束，返回统计是否成功，写入数据库
            if (statisitcStatus != SystemConfig.STATISITC_STATUS_UPDATED
                    && statisitcStatus != SystemConfig.STATISITC_STATUS_FAILED)
                return -1;
        } else if (status == SystemConfig.STATISITC_STATUS_FAILED || status == SystemConfig.STATISITC_STATUS_NOT_STATISITCED) {
            //跟新过，但是跟新失败了,继续
        } else {
            //报错
            return -1;
        }

        retrytime++;
        return workMessageDao.updateStatus(currentTime, statisitcStatus, ipAddress, retrytime.toString());
    }

    //每天定时任务的入口函数
    public void updateDataBaseBuild() {
        //获取所有商家id

        Long currentTime = FormatUtils.getTodayTimeInSeconds();
        Long tomorrowTime = FormatUtils.getTomorrowTimeInSeconds();

        String ipAddress = HttpUtils.getIPAddress();

        LOGGER.info((new Date()).toString() + "=====>开始进行工作量统计");
        //判断是否已经更新过了，返回0表示没有更新，负数表示已经更新过或者正在更新数据
        if (0 > updateBuildStatus(currentTime, SystemConfig.STATISITC_STATUS_UPDATING, ipAddress)) {
            LOGGER.info((new Date()).toString() + "<=====:不需要去进行统计，因为已经跟新过了");
            return;
        }
        //统计数据，
        try {
            doUpdateImWorkDetail(currentTime, tomorrowTime);
        } catch (Exception e) {
            //捕获到异常后，更新失败信息到表。
            updateBuildStatus(currentTime, SystemConfig.STATISITC_STATUS_FAILED, ipAddress);
            LOGGER.error((new Date()).toString() + "<=====:更新失败", e);
            return;
        }
        //统计成功，更新数据到表。
        updateBuildStatus(currentTime, SystemConfig.STATISITC_STATUS_UPDATED, ipAddress);
        LOGGER.info((new Date()).toString() + "<=====当天的数据统计结束");
    }

    public void doUpdateImWorkDetailByMap(Map<Long, List<SubMerchantInfo>> idToListMap, Map<Long, Long> subMerchantIdToMerchantIdMap
            , long currentTime, long tomorrowTime) {
        //获取这些子账号登入登出的记录进行分析。
        for (Map.Entry<Long, List<SubMerchantInfo>> entry : idToListMap.entrySet()) {

            LOGGER.info("正在统计,商家ID" + entry.getKey().toString());
            List<SubMerchantInfo> subMerchantInfoList = entry.getValue();
            String subMerchantIdlist = "";
            for (SubMerchantInfo indexInfo : subMerchantInfoList) {
                subMerchantIdlist += String.valueOf(indexInfo.getMemberId()) + ",";
                subMerchantIdToMerchantIdMap.put(indexInfo.getMemberId(), entry.getKey());
            }
            subMerchantIdlist = subMerchantIdlist.substring(0, subMerchantIdlist.length() - 1);
            int onlineDur = workMessageDao.getMerchantOnlineDur(subMerchantIdlist, currentTime, tomorrowTime);

            //如果当天该账号所有子账号都没有登入登出的动作，不记录
            if (onlineDur != 0) {
                workMessageDao.updateImWorkDetail(entry.getKey(), currentTime, onlineDur);
            }
        }
        return;
    }

    public void doUpdateImWorkDetailByMap(String merchantIdList, Map<Long, Long> subMerchantIdToMerchantIdMap
            , long currentTime, long tomorrowTime) {
        //获取商家的子账号信息,
        Map<Long, List<SubMerchantInfo>> idToListMap = merchantService.retrieveSubMerchantInfos(merchantIdList);

        doUpdateImWorkDetailByMap(idToListMap, subMerchantIdToMerchantIdMap, currentTime, tomorrowTime);

    }

    public void doUpdateCustomerWorkDetail(Map<Long, Long> subMerchantIdToMerchantIdMap
            , long currentTime, long tomorrowTime) {

        //获取所有客服列表
        List<SubMerchantInfo> customerServiceList = directorService.getAllSubMerchantInfoList();

        Map<Long, List<SubMerchantInfo>> idToListMap = new HashMap<>();
        idToListMap.put(Long.valueOf(-1), customerServiceList);

        doUpdateImWorkDetailByMap(idToListMap, subMerchantIdToMerchantIdMap, currentTime, tomorrowTime);

    }

    //具体的定时任务实现函数
    public void doUpdateImWorkDetail(long currentTime, long tomorrowTime) {
        //获取所有商家
        List<Merchant> alllist = merchantService.getMerchantInfoListByMerchantName("");

        int merchantIndex = 0;
        String merchantIdList = "";
        Map<Long, Long> subMerchantIdToMerchantIdMap = new HashMap<>();
        //遍历所有的商家，
        for (Merchant merchaninfo : alllist) {
            merchantIndex++;
            merchantIdList += String.valueOf(merchaninfo.getId()) + ",";
            if (merchantIndex < 10) {
                continue;
            }
            merchantIdList = merchantIdList.substring(0, merchantIdList.length() - 1);
            LOGGER.info("正在统计,商家ID列表" + merchantIdList);

            doUpdateImWorkDetailByMap(merchantIdList, subMerchantIdToMerchantIdMap, currentTime, tomorrowTime);

            merchantIndex = 0;
            merchantIdList = "";
        }
        doUpdateImWorkDetailByMap(merchantIdList, subMerchantIdToMerchantIdMap, currentTime, tomorrowTime);
        //客服登陆时长统计
        doUpdateCustomerWorkDetail(subMerchantIdToMerchantIdMap, currentTime, tomorrowTime);
        //更新I_RECEPTION_MAP表
        workMessageDao.updateLoginMap(subMerchantIdToMerchantIdMap, currentTime, tomorrowTime);
    }

    public Integer getMerchantLogInOutCount(Long startTime,Long endTime,
                                            List<Long> listUserId){
        return workMessageDao.getMerchantLogInOutCount(startTime,endTime,listUserId);
    }

    public List<MerchantLogInOutInfo> getMerchantLogInOut(Long startTime,Long endTime,
                                                          Integer pageNo,Integer pageSize,
                                                          List<Long> listUserId) {
        return workMessageDao.getMerchantLogInOut(startTime,endTime,pageNo,pageSize,listUserId);
    }
    /*
    * 数据查询相关函数
    * */
    public long checkParams(WorkMessageListRequest workMessageListRequest) {
        //三种条件都没限制的情况是查找所有的商家
        if (workMessageListRequest.getSellerID().isEmpty() && workMessageListRequest.getSellerName().isEmpty() &&
                workMessageListRequest.getStoreName().isEmpty()) {
            return 0;
        } else {
            //只要限制了一个条件，那么查找的是一个商家的信息，不需要汇总和均值
            Merchant finalbySellerID = merchantService.getMerchantInfoByMerchantId(workMessageListRequest.getSellerID());
            Merchant finalbyStoreName = merchantService.getMerchantInfoByShopName(workMessageListRequest.getStoreName());
            Merchant finalbySellerName = merchantService.getMerchantInfoByMerchantName(workMessageListRequest.getSellerName());
            Merchant finalFindMerchant = null;

            Set<Long> findIdSet = new HashSet<>();
            if (finalbySellerID != null) {
                finalFindMerchant = finalbySellerID;
                findIdSet.add(finalbySellerID.getId());
            }
            if (finalbyStoreName != null) {
                finalFindMerchant = finalbyStoreName;
                findIdSet.add(finalbyStoreName.getId());
            }
            if (finalbySellerName != null) {
                finalFindMerchant = finalbySellerName;
                findIdSet.add(finalbySellerName.getId());
            }

            if (findIdSet.size() > 1) {
                return -2;
            }

            if (findIdSet.size() == 0) {
                return -3;
            }
            //由于是模糊查找，这里过滤掉模糊查找的内容
            if ((workMessageListRequest.getSellerID().isEmpty() || finalFindMerchant.getId().toString().equals(workMessageListRequest.getSellerID()))
                    && (workMessageListRequest.getSellerName().isEmpty() || finalFindMerchant.getName().equals(workMessageListRequest.getSellerName()))
                    && (workMessageListRequest.getStoreName().isEmpty() || finalFindMerchant.getShopName().equals(workMessageListRequest.getStoreName()))) {

            } else {
                return -4;
            }

            return finalFindMerchant.getId();
        }

    }

    List<WorkMessageResponse> doGetWorkMessage(WorkMessageRequestParams workMessageRequestParams, long merchantId) {

        List<WorkMessageResponse> workMessageResponseList = workMessageDao.getWorkMessageResponseList(workMessageRequestParams, merchantId);

        if (workMessageResponseList == null || workMessageResponseList.size() == 0)
            return null;

        FormatUtils.formatWorkMessageResponse(workMessageResponseList.get(0));

        Merchant merchantInfo = merchantService.getMerchantInfoByMerchantId(String.valueOf(merchantId));
        //设置主营经营大类
        merchantInfo.setMainCategoryName(merchantService.getMainCategoryNameByMerchantId(String.valueOf(merchantId)));

        StoreInfo storeInfo = storeService.retrieveStoreInfo(String.valueOf(merchantId));

        FormatUtils.formatWorkMessageResponse(workMessageResponseList.get(0), merchantInfo, storeInfo.getOpenType());

        return workMessageResponseList;
    }

    //查询单个商家的数据
    //将获取的List<WorkMessageResponse>转为pager，具体的查询逻辑是在doGetWorkMessage中
    public Pager<WorkMessageResponse> getWorkMessage(WorkMessageRequestParams workMessageRequestParams, long merchantId) {
        Pager<WorkMessageResponse> pager = new Pager<WorkMessageResponse>();

        List<WorkMessageResponse> workMessageResponseList = doGetWorkMessage(workMessageRequestParams, merchantId);

        pager.setCurrentPageNo(1);
        pager.setPageSize(workMessageRequestParams.getPageSize());
        pager.setResultsForCurrentPage(workMessageResponseList);
        pager.setTotalRecordSize(1);
        return pager;
    }


    public List<WorkMessageResponse> doGetWorkMessageResponseList(WorkMessageRequestParams workMessageRequestParams, WorkMessageTotalInfoResponse totalInfo) {

        if (totalInfo == null || totalInfo.getTotalCount() <= 2)
            return null;

        List<WorkMessageResponse> workMessageResponseList = workMessageDao.getWorkMessageResponseList(workMessageRequestParams, 0);

        if (workMessageResponseList == null || workMessageResponseList.size() == 0)
            return null;

        String merchantIds = "";
        for (WorkMessageResponse workMessageResponse : workMessageResponseList) {
            FormatUtils.formatWorkMessageResponse(workMessageResponse);
            merchantIds += workMessageResponse.getMerchantIDStr() + ",";
        }

        List<Merchant> merchantInfoList = merchantService.getMerchantInfos(merchantIds);
        //主营经营大类
        Map<String,String> mainCategoryNameMap = merchantService.getMainCategoryNameByMerchantIds(merchantIds);
//        for(Merchant index : merchantInfoList){
//            Object key = index.getId();
//            index.setMainCategoryName(mainCategoryNameMap.get(key));
//        }

        Set<String> mapSet =  mainCategoryNameMap.keySet();	//获取所有的key值 为set的集合
        Iterator<String> itor =  mapSet.iterator();
        while(itor.hasNext()){
            String key = itor.next();//当前key值 merchantID
            for(Merchant index : merchantInfoList){
                if(key.equals(index.getId().toString())){//获取key 与 Merchant的ID比较
                    index.setMainCategoryName(mainCategoryNameMap.get(key));
                }
            }
        }

        Map<Long, Merchant> idToMerchantInfo = FormatUtils.merchantListToMap(merchantInfoList);
        Map<Integer, Integer> idTostatusMap = storeService.getMerchantStatusMap(merchantIds);

        FormatUtils.formatWorkMessageResponseListWithMap(workMessageResponseList, idToMerchantInfo, idTostatusMap);

        if (workMessageRequestParams.getCurrentPageNo() == 1) {
            WorkMessageResponse totalWorkMessageResponse = new WorkMessageResponse();
            WorkMessageResponse avgWworkMessageResponse = new WorkMessageResponse();

            FormatUtils.formatTotalInfo(totalInfo, totalWorkMessageResponse, avgWworkMessageResponse);

            workMessageResponseList.add(0 , totalWorkMessageResponse);
            workMessageResponseList.add(1 , avgWworkMessageResponse);
        }

        return workMessageResponseList;

    }

    //查询所有商家的数据
    //将获取的List<WorkMessageResponse>转为pager，具体的查询逻辑是在doGetWorkMessageResponseList中
    public Pager<WorkMessageResponse> getWorkMessageList(WorkMessageRequestParams workMessageRequestParams) {
        Pager<WorkMessageResponse> pager = new Pager<WorkMessageResponse>();

        WorkMessageTotalInfoResponse totalInfo = workMessageDao.getWorkTotalResponse(workMessageRequestParams);

        if (null == totalInfo) return null;

        List<WorkMessageResponse> workMessageResponseList = doGetWorkMessageResponseList(workMessageRequestParams, totalInfo);

        pager.setCurrentPageNo(workMessageRequestParams.getCurrentPageNo());
        pager.setPageSize(workMessageRequestParams.getPageSize());
        pager.setResultsForCurrentPage(workMessageResponseList);
        pager.setTotalRecordSize(totalInfo.getTotalCount());

        return pager;
    }


    /*
    * excel导出相关函数
    * */
    //export only one result
    public String[][] getWorkMessageListExport(WorkMessageRequestParams workMessageRequestParams, long merchantId) {
        String[][] result = new String[1][SystemConfig.STATISITC_ITEM_COUNT];
        workMessageRequestParams.setPageSize(SystemConfig.STATISITC_ITEM_COUNT);
        workMessageRequestParams.setCurrentPageNo(1);
        workMessageRequestParams.setStartCount(0);
        workMessageRequestParams.setEndCount(1);

        List<WorkMessageResponse> workMessageResponseList = doGetWorkMessage(workMessageRequestParams, merchantId);

        if (workMessageResponseList != null) {
            FormatUtils.formatWorkMessageResponseToArray(result, 0, workMessageResponseList.get(0));
        }
        return result;
    }

    //export all results
    public String[][] getWorkMessageListExcelExport(WorkMessageRequestParams workMessageRequestParams) {
        WorkMessageTotalInfoResponse totalInfo = workMessageDao.getWorkTotalResponse(workMessageRequestParams);
        int totalCount = totalInfo.getTotalCount();

        if (totalCount == 0)
            return null;

        int pageSize = SystemConfig.STATISITC_MAX_ITEM_PER_PAGE;
        int pageCount = totalCount / pageSize + (totalCount % pageSize == 0 ? 0 : 1);

        String[][] result = new String[totalInfo.getTotalCount()][SystemConfig.STATISITC_ITEM_COUNT];

        int indexPageCount = 1;
        while (indexPageCount <= pageCount) {
            workMessageRequestParams.setPageSize(pageSize);
            workMessageRequestParams.setCurrentPageNo(indexPageCount);

            if (indexPageCount == 1) {
                workMessageRequestParams.setStartCount(0);
            } else {
                workMessageRequestParams.setStartCount((indexPageCount - 1) * pageSize - 2);
            }
            workMessageRequestParams.setEndCount(pageSize * indexPageCount - 2);

            List<WorkMessageResponse> workMessageResponseList = doGetWorkMessageResponseList(workMessageRequestParams, totalInfo);

            if (workMessageResponseList == null) {
                continue;
            }

            int startIndex = (indexPageCount - 1) * pageSize;
            for (WorkMessageResponse indexWorkMessage : workMessageResponseList) {
                FormatUtils.formatWorkMessageResponseToArray(result, startIndex, indexWorkMessage);
                startIndex++;
            }

            indexPageCount++;
        }

        return result;
    }

    //发送警告 站内信
    public void sendOnlineTimeWarn(String infoTitle,String messageContent){


        InfoCenterApiClient.Message message = new InfoCenterApiClient.Message();
        message.setInfoTitle(infoTitle);
        message.setInfoTemplate(messageContent);
        message.setModule("moumou");
        UserSession userSession = SystemEnv.getCurrentUser();
        message.setAuthorId(SystemEnv.getProperty("message.authorId", userSession.getUserId().toString()));
        message.setAuthorName(SystemEnv.getProperty("message.authorName", userSession.getUserName()));
        List<InfoCenterApiClient.MessageItem> items = new ArrayList<InfoCenterApiClient.MessageItem>();
        items.add(new InfoCenterApiClient.MessageItem("71", new String[]{}));
        message.setItems(items);
        InfoCenterApiClient.sendInnerMail(message);
    }

}
