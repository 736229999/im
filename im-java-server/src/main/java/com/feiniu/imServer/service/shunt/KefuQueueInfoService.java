package com.feiniu.imServer.service.shunt;

import com.feiniu.imServer.bean.CSMonitorInfo;
import com.feiniu.imServer.bean.CachedUserInfo;
import com.feiniu.imServer.bean.ImServer.ImServerQueueInfo;
import com.feiniu.imServer.bean.ImServer.ImServerSubAccountInfo;
import com.feiniu.imServer.bean.Shunt.QueueItem;
import com.feiniu.imServer.constants.BusinessConfig;
import com.feiniu.imServer.consts.ShuntConfig;
import com.feiniu.imServer.repository.CsInfoDao;
import com.feiniu.imServer.service.base.CacheClientService;
import com.feiniu.imServer.service.im.ImService;
import com.feiniu.imServer.util.FormatUtils;
import com.feiniu.imServer.util.JsonUtil;
import com.feiniu.imServer.util.json.JsonNodeUtil;
import com.fn.cache.client.RedisCacheClient;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.*;

/**
 * Created by wangzhen on 2016/1/6.
 */
@Service
public class KefuQueueInfoService {
    private static final Logger LOGGER = LoggerFactory.getLogger(KefuQueueInfoService.class);
    @Autowired
    CacheClientService redisCacheClientService;
    @Autowired
    BusinessConfig businessConfig;

    @Autowired
    CsInfoDao csInfoDao;
    @Autowired
    private RedisCacheClient redisCacheClient;
    @Autowired
    private ImService imService;
    @Autowired
    private ShuntService shuntService;


        /*
        *
        * CSQUEUE的操作,只有读写互斥，也就是只有分流任务进行的时候不允许操作队列
        * */

    //锁住一个组进行分流。
    public boolean lockQueueInfoIndexForShunt(Long groupId) {
        try {
            String infoKey = ShuntConfig.MEMCACHE_CS_QUEUE + "#" + businessConfig.getMagicNumber()
                    + "#" + groupId.toString();

            boolean success = redisCacheClientService.tryLock(infoKey, ShuntConfig.LOCK_EXPIRED, ShuntConfig.LOCK_TIMEOUT);

            return success;
        } catch (Exception e) {
            LOGGER.error(e.toString());
            return false;
        }
    }

    //释放这个组的排队信息
    public void releaseQueueInfoIndexForShunt(Long groupId) {
        try {
            String infoKey = ShuntConfig.MEMCACHE_CS_QUEUE + "#" + businessConfig.getMagicNumber()
                    + "#" + groupId.toString();

            redisCacheClientService.releaseLock(infoKey);
            return;
        } catch (Exception e) {
            LOGGER.info("释放锁失败，锁不存在 :"+ groupId);
            return;
        }
    }

    //判断是否已经被锁了
    public boolean isQueueLocked(Long groupId) {
        try {
            String infoLockKey = ShuntConfig.MEMCACHE_CS_QUEUE + "#" + businessConfig.getMagicNumber()
                    + "#" + groupId.toString() + ShuntConfig.LOCK_NAME;

            String lastLockTimeExpire = redisCacheClient.get(infoLockKey);
            return !(lastLockTimeExpire == null || Long.valueOf(lastLockTimeExpire) < System.currentTimeMillis());
        } catch (Exception e) {
            LOGGER.error(e.toString());
            return false;
        }
    }

    public QueueItem getQueueFirstItem(Long groupId){
        String itemKey = ShuntConfig.MEMCACHE_CS_QUEUE_ITEM + "#" + businessConfig.getMagicNumber();
        String infoKey = ShuntConfig.MEMCACHE_CS_QUEUE + "#" + businessConfig.getMagicNumber()
                + "#" + groupId.toString();

        while (true) {
            String customerId = getQueueFirstId(groupId);
            if (customerId == null)
                return null;

            String queueItemStr = redisCacheClient.hget(itemKey, customerId);
            if (queueItemStr == null)
                redisCacheClient.zrem(infoKey, customerId);
            else
                return JsonNodeUtil.injectBean(queueItemStr, QueueItem.class);
        }
    }

    //获取排队队列中score最小的
    public String getQueueFirstId(Long groupId) {
        String infoKey = ShuntConfig.MEMCACHE_CS_QUEUE + "#" + businessConfig.getMagicNumber()
                + "#" + groupId.toString();

        Set<String> result = redisCacheClient.zrange(infoKey, 0, 1);
        if (result == null || result.isEmpty())
            return null;

        for (Iterator it = result.iterator(); it.hasNext(); ) {
            return it.next().toString();
        }
        return null;
    }

    //获取队列的个数
    public Long getQueueNumberInGroupId(Long groupId) {
        try {
            String infoKey = ShuntConfig.MEMCACHE_CS_QUEUE + "#" + businessConfig.getMagicNumber()
                    + "#" + groupId.toString();

            return redisCacheClient.zcard(infoKey);

        } catch (Exception e) {
            LOGGER.error(e.toString());
            return Long.valueOf(0);
        }
    }

    //插入到排队队列
    public Long addToQueue(Long groupId, QueueItem queueItem, double score) {
        try {
            String infoKey = ShuntConfig.MEMCACHE_CS_QUEUE + "#" + businessConfig.getMagicNumber()
                    + "#" + groupId.toString();

            String queueItemKey = ShuntConfig.MEMCACHE_CS_QUEUE_ITEM + "#" + businessConfig.getMagicNumber();

            Double scoreOld = redisCacheClient.zscore(infoKey, queueItem.getCustomerId().toString());

            //如果已经在排队了，而且优先级比要插入的优先级低，才进行插入
            if (scoreOld == null || scoreOld > score) {
                redisCacheClient.zadd(infoKey, score, queueItem.getCustomerId().toString());
                redisCacheClient.hput(queueItemKey,queueItem.getCustomerId().toString(),
                        JsonUtil.toJsonString(queueItem));
            }

            Long index = redisCacheClient.zrank(infoKey, queueItem.getCustomerId().toString());

            return index == null ? Long.valueOf(-3) : index+1;
        } catch (Exception e) {
            LOGGER.error(e.toString());

            return Long.valueOf(-3);
        }
    }

    //删除队列中的一个
    public boolean removeMembernInQueue(Long groupId, Long customerId) {
        try {
            String infoKey = ShuntConfig.MEMCACHE_CS_QUEUE + "#" + businessConfig.getMagicNumber()
                    + "#" + groupId.toString();

            String queueItemKey = ShuntConfig.MEMCACHE_CS_QUEUE_ITEM + "#" + businessConfig.getMagicNumber();

            redisCacheClient.zrem(infoKey, customerId.toString());

            String[] customerIdList = {customerId.toString()};
            redisCacheClient.hremove(queueItemKey, customerIdList);

            return true;
        } catch (Exception e) {
            LOGGER.error(e.toString());

            return false;
        }
    }

    //删除所有的队列
    public boolean removeAllQueues() {
        try {
            List<String> groupIdList = csInfoDao.getAllGroups();
            for (String groupIdIndex : groupIdList) {
                String infoKey = ShuntConfig.MEMCACHE_CS_QUEUE + "#" + businessConfig.getMagicNumber()
                        + "#" + groupIdIndex.toString();
                String queueItemKey = ShuntConfig.MEMCACHE_CS_QUEUE_ITEM + "#" + businessConfig.getMagicNumber();

                redisCacheClient.remove(infoKey);
                redisCacheClient.remove(queueItemKey);
            }
            return true;
        } catch (Exception e) {
            LOGGER.error(e.toString());

            return false;
        }
    }

    //获取排队列表
    public List<ImServerQueueInfo> getQueueInfoListInGroupId(Long groupId) {
        List<ImServerQueueInfo> resultQueueInfoList = new ArrayList<>();
        Set<String> reusltSet = getQueueInfoInGroupId(groupId);
        int indexPos = 1;
        for (String idIndex : reusltSet) {
            ImServerQueueInfo indexInfo = new ImServerQueueInfo();
            indexInfo.setId(Long.valueOf(idIndex));
            indexInfo.setPos(indexPos++);
            resultQueueInfoList.add(indexInfo);
        }
        return resultQueueInfoList;
    }

    //获取排队id列表
    public Set<String> getQueueInfoInGroupId(Long groupId) {
        try {
            String infoKey = ShuntConfig.MEMCACHE_CS_QUEUE + "#" + businessConfig.getMagicNumber()
                    + "#" + groupId.toString();

            return redisCacheClient.zrange(infoKey, 0, -1);

        } catch (Exception e) {
            LOGGER.error(e.toString());
            return null;
        }
    }

    /**
     * 获取组内所有客服id列表
     */
    public List<String> getServiceIdListInGroupId(Long groupId) {
        try {
            String infoKey = ShuntConfig.MEMCACHE_SERVICE_IN_GROUP + "#" + businessConfig.getMagicNumber()
                    + "#" + groupId.toString();

            List<String> idList =  redisCacheClient.lrange(infoKey, 0, -1);

            //如果没有数据，读数据库获取组内成员列表更新到redis里面
            if (idList == null|| idList.isEmpty()) {
                idList = flushGroupMemToRedis(groupId);
            }

            return  idList;
        } catch (Exception e) {
            LOGGER.error("获取组内成员失败:" + groupId.toString());
            LOGGER.error(e.toString());
            return null;
        }
    }

    /**
     * 获取单个客服上下文信息
     */
    public CSMonitorInfo getMonitorInfoById(Long id,boolean needOnline) {
        try {

            CSMonitorInfo memcheCSInfo = new CSMonitorInfo();

            CachedUserInfo userInfo = imService.getUserInfo(id,needOnline);

            if (userInfo == null )
                return  null;

            memcheCSInfo.setCachedUserInfo(userInfo);
            memcheCSInfo.setcServiceId(id);

            //如果客服不处于在线状态，直接将咨询量和接待量设置为0
            if (!FormatUtils.isStatusOnline(Integer.valueOf(userInfo.getStatus()))) {
                memcheCSInfo.setConsultCount(0);
                memcheCSInfo.setReceptionCount(0);
                return memcheCSInfo;
            }

            String receptionListKey = ShuntConfig.MEMCACHE_RECPTION_LIST + "#" + businessConfig.getMagicNumber()
                    + "#" + id;

            Long consultCount = redisCacheClient.hlen(receptionListKey);
            Map<String, String> resultMap = redisCacheClient.hgetAll(receptionListKey);

            if (resultMap == null) {
                memcheCSInfo.setConsultCount(0);
                memcheCSInfo.setReceptionCount(0);
            } else {
                Integer receptionCount = 0;

                for (Map.Entry<String, String> entry : resultMap.entrySet()) {
                    if (entry.getValue().equals("1"))
                        receptionCount++;
                }

                memcheCSInfo.setConsultCount(consultCount.intValue());
                memcheCSInfo.setReceptionCount(receptionCount);
            }

            return memcheCSInfo;
        } catch (Exception e) {
            LOGGER.error(e.toString());
            return null;
        }
    }

    //将数据库里面所有的组成员信息更新到redis。
    public void flushAllGroupMemToRedis() {
        List<String> groupList = csInfoDao.getAllGroups();
        for (String groupIndex : groupList) {
            flushGroupMemToRedis(Long.valueOf(groupIndex));
        }
        //一个单独组，保留所有客服
        flushGroupMemToRedis(Long.valueOf(0));
    }

    //更新数据库里面的一个组成员到redis
    public List<String> flushGroupMemToRedis(Long groupId) {
        List<String> idList;
        if (groupId == 0)
            idList = csInfoDao.getAllCsIdInGroup(null);
        else
            idList = csInfoDao.getAllCsIdInGroup(groupId.toString());

        String infoKey = ShuntConfig.MEMCACHE_SERVICE_IN_GROUP + "#" + businessConfig.getMagicNumber()
                + "#" + groupId.toString();

        redisCacheClient.del(infoKey);

        for (String indexId : idList) {
            redisCacheClient.lpush(infoKey, indexId);
        }
        return idList;
    }

    /**
     * 获取组内所有在线客服上下文信息
     */
    public List<CSMonitorInfo> getServiceInfoListInGroupId(Long groupId, boolean needOnline) {
        try {
            List<CSMonitorInfo> resultList = new ArrayList<>();
            List<String> idList = getServiceIdListInGroupId(groupId);

            //获取在线用户的上下文
            for (String idIndex : idList) {
                CSMonitorInfo indexReuslt = getMonitorInfoById(Long.valueOf(idIndex),needOnline);
                if (indexReuslt != null && (!needOnline || FormatUtils.isServiceCanShunt(indexReuslt.getCachedUserInfo())))
                    resultList.add(indexReuslt);
            }
            return resultList;
        } catch (Exception e) {
            LOGGER.error(e.toString());
            return null;
        }
    }

    /*
    *
    * addRecptionRelation  remRecptionRelation transRecptionRelation 3个函数互斥。
    *
    * 也就是说不能允许3个函数操作同一个customer的接待关系
    * */


    /**
     * 清除所有的redis数据
     */
    public boolean clearAllRecptionRelation() {
        List<String> allIds = csInfoDao.getAllCsIdInGroup(null);
        for (String idIndex : allIds) {
            remAllRecptionRelationByServiceId(Long.valueOf(idIndex), true);
        }
        return true;
    }

    /**
     * 删除一个客服所有的接待关系，主要是为客服下线来调用
     */
    public boolean remAllRecptionRelationByServiceId(Long serviceId, boolean isOffLine) {
        String receptionListKey = ShuntConfig.MEMCACHE_RECPTION_LIST + "#" + businessConfig.getMagicNumber()
                + "#" + serviceId.toString();

        Map<String, String> resultMap = redisCacheClient.hgetAll(receptionListKey);

        for (Map.Entry<String, String> entry : resultMap.entrySet()) {
            imService.removeReceptionRelation(serviceId, Long.valueOf(entry.getKey()));
        }

        //离线了删除userinfo字段
        if (isOffLine)
            imService.removeUserInfo(serviceId);

        return true;
    }

    //获取队列的个数
    public Long getReceptionCountById(Long serviceId) {
        try {
            String receptionListKey = ShuntConfig.MEMCACHE_RECPTION_LIST + "#" + businessConfig.getMagicNumber()
                    + "#" + serviceId.toString();

            return redisCacheClient.hlen(receptionListKey);

        } catch (Exception e) {
            LOGGER.error(e.toString());
            return null;
        }
    }

    /**
     * 获取客户上下文信息
     */
    public ImServerSubAccountInfo getSubAccountInfo(Long customerId, Long serviceId) {

        ImServerSubAccountInfo testSubAccount = new ImServerSubAccountInfo();
        try {
            CachedUserInfo cachedUserInfo = imService.getUserInfo(serviceId,false);

            if (cachedUserInfo == null) {
                LOGGER.error("获取用户信息为空 : " + cachedUserInfo);
                return null;
            }
            testSubAccount.setBuyerId(customerId);
            testSubAccount.setEmail(cachedUserInfo.geteMail());
            testSubAccount.setFaceUrl(cachedUserInfo.getHeadURL());
            testSubAccount.setMerchantId(serviceId);
            testSubAccount.setMobile(cachedUserInfo.getPhone());
            testSubAccount.setNickName(cachedUserInfo.getNickname());
            testSubAccount.setUserName(cachedUserInfo.getUsername());
            testSubAccount.setUserSign(cachedUserInfo.getUserSign());

            return testSubAccount;
        } catch (Exception e) {
            LOGGER.error("获取用户信息失败  " +e.toString());
            return null;
        }
    }


    /**
     * 获取组内成员是否有在线状态并可以分流的
     */
    public boolean isCanShuntInGroup(Long groupId){
        List<String > idList = getServiceIdListInGroupId(groupId);
        if (idList == null || idList.isEmpty())
            return false;

        for (String idIndex : idList){
            if (shuntService.isServiceCanShunt(Long.valueOf(idIndex)))
                return true;
        }
        return false;
    }
}
