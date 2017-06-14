package com.feiniu.imServer.service.shunt;

import com.feiniu.imServer.bean.CSMonitorInfo;
import com.feiniu.imServer.bean.CachedUserInfo;
import com.feiniu.imServer.bean.KVPair;
import com.feiniu.imServer.bean.Shunt.QueueItem;
import com.feiniu.imServer.bean.Shunt.SessionInfoItem;
import com.feiniu.imServer.component.DBServiceGroupMappingManager;
import com.feiniu.imServer.component.MerchantDiverseConfigManager;
import com.feiniu.imServer.constants.BusinessConfig;
import com.feiniu.imServer.consts.ShuntConfig;
import com.feiniu.imServer.consts.SystemConfig;
import com.feiniu.imServer.domain.MerchantDiverseConfig;
import com.feiniu.imServer.repository.RecentInfoDao;
import com.feiniu.imServer.service.base.CacheClientService;
import com.feiniu.imServer.service.base.ImMessageHandleSyncService;
import com.feiniu.imServer.service.external.MerchantService;
import com.feiniu.imServer.service.im.ImService;
import com.feiniu.imServer.service.sessionInfo.SessionInfoService;
import com.feiniu.imServer.service.userinfo.UserHotInfoService;
import com.feiniu.imServer.service.webIm.NotifyService;
import com.feiniu.imServer.util.FormatUtils;
import com.fn.cache.client.RedisCacheClient;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import javax.annotation.PreDestroy;
import java.io.IOException;
import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;


/**
 * Created by wangzhen on 2016/1/7.
 */
@Service
public class ShuntService {
    private static final Logger LOGGER = LoggerFactory.getLogger(ShuntService.class);
    private static ThreadPoolExecutor executor = new ThreadPoolExecutor(ShuntConfig.THREAD_POOL_SIZE,
            ShuntConfig.THREAD_MAX_POOL_SIZE, ShuntConfig.THREAD_KEEP_ALIVE_TIME, TimeUnit.MILLISECONDS,
            new ArrayBlockingQueue<Runnable>(ShuntConfig.THREAD_POOL_SIZE));
    @Autowired
    KefuQueueInfoService kefuQueueInfoService;
    @Autowired
    ImMessageHandleSyncService imMessageHandleSyncService;
    @Autowired
    BusinessConfig businessConfig;
    @Autowired
    CacheClientService redisCacheClientService;
    @Autowired
    RecentInfoDao recentInfoDao;
    @Autowired
    private RedisCacheClient redisCacheClient;
    @Autowired
    private MerchantDiverseConfigManager merchantDiverseConfigManager;
    @Autowired
    private DBServiceGroupMappingManager dbServiceGroupMappingManager;
    @Autowired
    private ImService imService;
    @Autowired
    private NotifyService notifyService;
    @Autowired
    private UserHotInfoService userHotInfoService;
    @Autowired
    private MerchantService merchantService;
    @Autowired
    private SessionInfoService sessionInfoService;

    @PreDestroy
    public void destroy() throws IOException {
        executor.shutdown();
    }

    //加入排队逻辑
    public Long addToQueue(QueueItem queueItem, Long groupId,
                           Integer userLevel, Long requestTime) {
        Long score = (ShuntConfig.MAX_USER_LEVEL - userLevel) * ShuntConfig.MAX_REQUEST_TIME + requestTime;

        //redis 是单线程的，应该是不需要进行锁操作，也就是说分流的时候进行排队插入也没有问题
//        boolean locksuccessed = kefuQueueInfoService.lockQueueInfoIndexForShunt(groupId);
//
//        if (!locksuccessed){
//            LOGGER.info("加入到排队队列失败，队列一直被锁");
//
//            return false;
//        }

        boolean isOnline = kefuQueueInfoService.isCanShuntInGroup(groupId);

        if (!isOnline) {
            LOGGER.info("没有可以分流的客服" + queueItem.getCustomerId());
            return Long.valueOf(-1);
        }

        boolean isReception = imService.isCustomerReceptioning(queueItem.getCustomerId());

        if (isReception) {
            LOGGER.info("需要排队的客户已经在被接待" + queueItem.getCustomerId());
            return Long.valueOf(-2);
        }

        Long rank = kefuQueueInfoService.addToQueue(groupId, queueItem, Double.valueOf(score));

        imService.notifyShuntDoneMessage(groupId,kefuQueueInfoService.
                getQueueNumberInGroupId(groupId));
//        kefuQueueInfoService.releaseQueueInfoIndexForShunt(groupId);

        //加入排队后，启动线程进行分流
        startShuntTask(groupId);

        return rank;
    }

    //转接和接手会话逻辑
    public boolean transfer(Long groupId, Long serviceId, Long customerId, Long toServiceId, Long toServiceGroup) {
        boolean result = imService.transRecptionRelation(groupId, serviceId, customerId, toServiceId, toServiceGroup);
        startShuntTask(groupId);
        startShuntTask(Long.valueOf(0));
        return result;
    }

    //主动邀约逻辑
    public Long invite(Long groupId, Long serviceId) {
        boolean locksuccessed = kefuQueueInfoService.lockQueueInfoIndexForShunt(groupId);

        if (!locksuccessed) {
            LOGGER.info("主动邀约失败，队列一直被锁");
            return null;
        }

        try {
            QueueItem queueItem = kefuQueueInfoService.getQueueFirstItem(groupId);

            if (queueItem == null) {
                kefuQueueInfoService.releaseQueueInfoIndexForShunt(groupId);
                return null;
            }

            if (serviceId != 0) {
                //删除排队队列中该成员
                kefuQueueInfoService.removeMembernInQueue(groupId, queueItem.getCustomerId());
                //更新redis里面用户上下文的接待信息。
                imService.addReceptionRelationShip(groupId, serviceId, queueItem.getCustomerId(),true);
            }

            kefuQueueInfoService.releaseQueueInfoIndexForShunt(groupId);


            //分流成功，填充会话信息
            SessionInfoItem sessionInfoItem = new SessionInfoItem();
            sessionInfoItem.setCustomerId(queueItem.getCustomerId());
            sessionInfoItem.setEventType(0);
            sessionInfoItem.setGroupId(Long.valueOf(groupId));
            sessionInfoItem.setServiceId(Long.valueOf(0));
            sessionInfoItem.setFromPage(queueItem.getFromPage());
            sessionInfoItem.setFromDevice(queueItem.getFromDevice());
            sessionInfoItem.setWaitingTime(
                    (System.currentTimeMillis()/1000 - queueItem.getWaitingTime()));
            sessionInfoItem.setSessionStartTime(System.currentTimeMillis()/1000);
            sessionInfoItem.setUserLevel(queueItem.getCustomerLevel());
            sessionInfoItem.setServiceId(serviceId);
            sessionInfoService.putEventMessage(sessionInfoItem);

            imService.notifyShuntDoneMessage(groupId,kefuQueueInfoService.
                    getQueueNumberInGroupId(groupId));

            notifyService.notifyQueueInfoMessage(groupId, queueItem.getCustomerId(), serviceId
                    , System.currentTimeMillis());

            return queueItem.getCustomerId();
        } catch (Exception e) {
            LOGGER.error("主动邀约失败");
            e.printStackTrace();
            kefuQueueInfoService.releaseQueueInfoIndexForShunt(groupId);
            return null;
        }
    }


    //接待结束
    public boolean closeRecption(Long serviceId, Long customerId,boolean needRelationed) {
        String customerinfoKey = ShuntConfig.MEMCACHE_USER_INFO + "#" + businessConfig.getMagicNumber()
                + "#" + customerId.toString();
        String groupId = redisCacheClient.hget(customerinfoKey, ShuntConfig.MEMCACHE_FEILD_GROUP_USERID);
        boolean result = false;

        if (!needRelationed) {
            result = imService.removeReceptionRelation(serviceId, customerId);
        }else{
            result = imService.removeReceptionRelationIfRelated(serviceId, customerId);
        }

        if (groupId != null && !groupId.isEmpty()){
            startShuntTask(Long.valueOf(groupId));
            startShuntTask(Long.valueOf(0));
        }

        return result;
    }

    //启动分流线程
    public void startShuntTask(Long groupId) {
        //如果已经有分流线程在处理这个组的等待队列,不启动线程
        //也就是说不允许2个分流线程对同一个组进行分流
        if (kefuQueueInfoService.isQueueLocked(groupId)) {
            return;
        }

        ShuntTask shuntTask = new ShuntTask(groupId);

        executor.execute(shuntTask);
    }

    //获取所有在线并且能接待的的自营客服的信息
    public List<CachedUserInfo> getOnlineUserInfoBySubMerchantIdList(String subMerchantIDList,Long groupId) {
        String[] subMerchantIdList = subMerchantIDList.split(",");
        List<CachedUserInfo> cachedUserInfoList = new ArrayList<>();
        for (String subMerchantId : subMerchantIdList) {
            CachedUserInfo resultIndex = getServicePrinciaplById(Long.valueOf(subMerchantId), true);
            if (resultIndex != null )
                cachedUserInfoList.add(resultIndex);
        }
        return cachedUserInfoList;
    }

    //获取客户最近联系的客服ID
    public Long getRecentServiceIdBySubMerantIdList(Long customerId, String subMerchantIDList) {
        String[] subMerrchantIdArray = subMerchantIDList.split(",");
        Long idResultInRedis = userHotInfoService.getMostRecentServiceIdInList(customerId,subMerrchantIdArray);

        if (idResultInRedis == null){
            //如果redis里面没有数据，刷新数据库的数据到redis
            Long subMerchantId = recentInfoDao.getRecentServiceInfoBySubMerchantList(customerId, subMerchantIDList);
            return subMerchantId;
        }else {
            return idResultInRedis;
        }
    }

    //判断客服是否能分流
    public boolean isServiceCanShunt(Long serciceId) {
        try {
            CachedUserInfo userInfo = imService.getUserInfo(serciceId,true);
            if (userInfo == null)
                return false;
            return FormatUtils.isServiceCanShunt(userInfo);
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    //商城客服分流的逻辑
    public CachedUserInfo getShuntServicePrinciapl(int shuntType, long customerId, List<CachedUserInfo> subMerchantInfos) {
        CachedUserInfo result = null;
//        if (shuntType == 2) {
            //均分
            Long receptionCountMin = Long.valueOf(10000000);
            for (CachedUserInfo cachedUserInfo: subMerchantInfos){
                Long receptionCountIndex = kefuQueueInfoService.getReceptionCountById(Long.valueOf(cachedUserInfo.getId()));
                if (receptionCountMin > receptionCountIndex){
                    receptionCountMin = receptionCountIndex;
                    result = cachedUserInfo;
                }
            }
//        } else {
//            //接待能力分
//            Integer minCondition = ShuntConfig.MAX_RECEPTION_RATE;
//            for (CachedUserInfo cachedUserInfo: subMerchantInfos){
//                Long receptionCountIndex = kefuQueueInfoService.getReceptionCountById(Long.valueOf(cachedUserInfo.getId()));
//                Integer recptionLimit = cachedUserInfo.getReceptionLimit() == null ?
//                        1:Integer.valueOf(cachedUserInfo.getReceptionLimit());
//
//                Integer conditionIndex = receptionCountIndex.intValue() * 100 / recptionLimit;
//
//                if (minCondition > conditionIndex){
//                    minCondition = conditionIndex;
//                    result = cachedUserInfo;
//                }
//            }
//        }
        return result;
    }

    public CachedUserInfo getServicePrinciaplById(Long subAccountId, boolean needFlow) {
        try {

            CachedUserInfo userInfo = imService.getUserInfo(subAccountId,false);
            //在线而且能够分流
            if (needFlow && userInfo != null && userInfo.getId() != null && userInfo.getFlow().equals("0") &&
                    userInfo.getStatus().equals( "0"))
                return userInfo;
            else if (!needFlow)
                return userInfo;
            else
                return null;
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    //获取商家的分流方式，读缓存，没有就从数据库中刷
    //需要商家后台配合修改，商家修改分流方法后，同时修改缓存
    public KVPair<Integer, Long> getShuntModeByMerchantId(long merchantId) {
        String infoKey = ShuntConfig.MEMCACHE_INFO + "#" + businessConfig.getMagicNumber()
                + "#" + String.valueOf(merchantId);
        String shuntTypeStr = redisCacheClient.hget(infoKey, "shuntType");
        String proxySubAccountIdStr = redisCacheClient.hget(infoKey, "proxy_subAcount_id");
        KVPair<Integer, Long> resultPair;

        if (StringUtils.isEmpty(shuntTypeStr) || StringUtils.isEmpty(proxySubAccountIdStr)) {
            MerchantDiverseConfig merchantDiverseConfig = merchantDiverseConfigManager.getDiverseConfigForMerchant(merchantId);
            int shuntTypeInt = 0;
            long proxy_subAcount_id = 0;
            if (merchantDiverseConfig != null) {
                shuntTypeInt = merchantDiverseConfig.getDiverseType().getDiverseTypeID();
                proxy_subAcount_id = merchantDiverseConfig.getProxySubAccountID();
            }else {
                shuntTypeInt = 1;
                proxy_subAcount_id = merchantService.retrieveMainSubMerchntId(merchantId);
            }
            redisCacheClient.hput(infoKey, "shuntType", String.valueOf(shuntTypeInt));
            redisCacheClient.hput(infoKey, "proxy_subAcount_id", String.valueOf(proxy_subAcount_id));
            resultPair = new KVPair<>(shuntTypeInt, proxy_subAcount_id);
            return resultPair;
        }

        return new KVPair<>(Integer.valueOf(shuntTypeStr), Long.valueOf(proxySubAccountIdStr));
    }

    public class ShuntTask implements Runnable {
        private Long groupId;

        public ShuntTask(Long groupId) {
            this.groupId = groupId;
        }

        @Override
        public void run() {
            LOGGER.info("分流线程开始" + groupId);
            shunt(groupId);
            LOGGER.info("分流线程结束" + groupId);
        }

        //分流逻辑
        public void shunt(Long groupId) {

            if (kefuQueueInfoService.getQueueNumberInGroupId(groupId) == Long.valueOf(0)) {
                LOGGER.info("分流失败，队列为空");
                return;
            }

            boolean locksuccessed = kefuQueueInfoService.lockQueueInfoIndexForShunt(groupId);

            if (!locksuccessed) {
                LOGGER.info("分流失败，队列一直被锁");
                return;
            }

            try {
                SessionInfoItem sessionInfoItem = null;
                do {
                    //分流成功，通知到webim
                    sessionInfoItem = doShunt(groupId);
                    if (sessionInfoItem != null) {
                        //发送分流成功信息
                        notifyService.notifyQueueInfoMessage(sessionInfoItem.getGroupId(),
                                sessionInfoItem.getCustomerId(), sessionInfoItem.getServiceId()
                                , sessionInfoItem.getWaitingTime());

                        //将session信息写入数据库
                        sessionInfoService.putEventMessage(sessionInfoItem);
//                        //发送分流成功的事件提醒
//                        imMessageHandleService.notifyEventMessage(resultMessage,
//                                ShuntConfig.SHUNT_EVENT_TYPE.EVENT_SHUNT_SUCCESSED.ordinal(),
//                                "分流成功");

                        //通知imserver，分流成功的事件
                        imService.notifyShuntDoneMessage(groupId,
                                kefuQueueInfoService.getQueueNumberInGroupId(groupId));
                    }
                } while (sessionInfoItem != null);
            } catch (Exception e) {
                kefuQueueInfoService.releaseQueueInfoIndexForShunt(groupId);
                LOGGER.info("分流失败: " + groupId + " msg:" + e.getMessage());
            }

            kefuQueueInfoService.releaseQueueInfoIndexForShunt(groupId);
        }

        //分流逻辑的具体实现
        private SessionInfoItem doShunt(Long groupId) throws Exception{
            //如果列表中没有数据，直接返回
            if (groupId == null){
                Exception e = new Exception("分流失败，组ID为空");
                throw e;
            }
            //获取队列中优先级最高的
            QueueItem queueItem = kefuQueueInfoService.getQueueFirstItem(groupId);
            //队列里面没有数据，分流线程结束
            if (queueItem == null){
                Exception e = new Exception("分流失败，队列为空");
                throw e;
            }
            //如果排队的客户已经下线，直接删除该用户的排队，继续下个分流任务
            Long customerid = queueItem.getCustomerId();
            if (!imService.getCustomerOnlineStats(customerid)){
                kefuQueueInfoService.removeMembernInQueue(groupId, customerid);
                return null;
            }

            List<CSMonitorInfo> serviceList = kefuQueueInfoService.getServiceInfoListInGroupId(groupId, true);

            //分流给满足条件：shunCondition最小的客服
            Long minID = Long.valueOf(0);
            Integer minCondition = ShuntConfig.MAX_RECEPTION_RATE;
            for (CSMonitorInfo infoIndex : serviceList) {
                Long serviceId = infoIndex.getcServiceId();
                if (serviceId == null){
                    Exception e = new Exception("获取客服信息异常 ");
                    throw e;
                }

                Integer receptionCount = infoIndex.getConsultCount();
                Long receptionLimit = Long.valueOf(infoIndex.getCachedUserInfo().getReceptionLimit());

                if (receptionCount == null || receptionLimit > receptionCount) {
                    BigDecimal shunCondition = new BigDecimal(receptionCount.toString()).multiply(new BigDecimal(100)).divide(new BigDecimal(receptionLimit.toString()), BigDecimal.ROUND_HALF_EVEN).setScale(SystemConfig.DEFAULT_SCALE, BigDecimal.ROUND_HALF_EVEN);
                    if (shunCondition.intValue() < minCondition) {
                        minCondition = shunCondition.intValue();
                        minID = infoIndex.getcServiceId();
                    }
                }
            }
            if (minCondition != ShuntConfig.MAX_RECEPTION_RATE) {
                //分流成功，填充会话信息
                SessionInfoItem sessionInfoItem = new SessionInfoItem();
                sessionInfoItem.setCustomerId(Long.valueOf(customerid));
                sessionInfoItem.setEventType(0);
                sessionInfoItem.setGroupId(Long.valueOf(groupId));
                sessionInfoItem.setServiceId(Long.valueOf(0));
                sessionInfoItem.setFromPage(queueItem.getFromPage());
                sessionInfoItem.setFromDevice(queueItem.getFromDevice());
                sessionInfoItem.setWaitingTime(
                        (System.currentTimeMillis()/1000 - queueItem.getWaitingTime()));
                sessionInfoItem.setSessionStartTime(System.currentTimeMillis()/1000);
                sessionInfoItem.setUserLevel(queueItem.getCustomerLevel());
                sessionInfoItem.setServiceId(minID);
                //删除排队队列中该成员
                kefuQueueInfoService.removeMembernInQueue(groupId, customerid);
                //更新redis里面用户上下文的接待信息。
                imService.addReceptionRelationShip(groupId, sessionInfoItem.getServiceId(), customerid,true);
                return sessionInfoItem;
            } else {
                //没找到可以给分流的客服
                Exception e = new Exception("没找到可以分流的客服 ");
                throw e;
            }
        }
    }
}
