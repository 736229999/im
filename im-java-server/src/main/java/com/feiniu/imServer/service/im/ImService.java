package com.feiniu.imServer.service.im;

import com.feiniu.imServer.base.FeiniuCommonRequest;
import com.feiniu.imServer.bean.CachedUserInfo;
import com.feiniu.imServer.bean.KVPair;
import com.feiniu.imServer.bean.Shunt.SessionInfoItem;
import com.feiniu.imServer.constants.BusinessConfig;
import com.feiniu.imServer.consts.ShuntConfig;
import com.feiniu.imServer.repository.CsInfoDao;
import com.feiniu.imServer.repository.UserInfoDao;
import com.feiniu.imServer.service.base.CacheClientService;
import com.feiniu.imServer.service.sessionInfo.SessionInfoService;
import com.feiniu.imServer.util.FormatUtils;
import com.feiniu.imServer.util.JsonUtil;
import com.feiniu.imServer.util.json.JsonNodeUtil;
import com.feiniu.imServer.util.web.HttpClientUtils;
import com.fn.cache.client.RedisCacheClient;
import org.codehaus.jackson.JsonNode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by wangzhen on 2016/2/19.
 */
@Service
public class ImService {
    @Autowired
    RedisCacheClient redisCacheClient;
    @Autowired
    CacheClientService cacheClientService;
    @Autowired
    BusinessConfig businessConfig;

    @Autowired
    CsInfoDao csInfoDao;
    @Autowired
    UserInfoDao userInfoDao;
    @Autowired
    SessionInfoService sessionInfoService;

    private static final Logger LOGGER = LoggerFactory.getLogger(ImService.class);

    public boolean nofityRecptionClosedByCustomer(Long customerId,Long serviceId,Long loginIP ){

        FeiniuCommonRequest feiniuCommonRequest = new FeiniuCommonRequest();
        Map<String,Long> params = new HashMap<>(1);

        params.put("buyerId",customerId);
        params.put("csId",serviceId);
        params.put("loginIp",loginIP);
        feiniuCommonRequest.setMethod("pushCloseReceptionByCustomer");
        feiniuCommonRequest.setParams(JsonUtil.toJsonString(params));

        try {
            String response = HttpClientUtils.executeGet(businessConfig.getImServerUrl(), "", JsonUtil.toMap(feiniuCommonRequest));
            JsonNode jsonNode = JsonUtil.toNode(response);
            if (jsonNode != null && JsonNodeUtil.getBoolean(jsonNode, "result")) {
                LOGGER.info("通知Imserver客户结束接待成功: " + customerId + ": serviceId : " +
                        serviceId.toString()  );
                return true;
            } else {
                LOGGER.info("通知Imserver客户结束接待失败: " + customerId+ ": serviceId : " +
                        serviceId.toString() );
                return false;
            }
        } catch (Exception e) {
            LOGGER.error("通知Imserver客户结束接待失败", e);
            return false;
        }

    }
    /*
    * 暂时不去考虑userinfo的redis的生命周期。。im server去维护
    * */
    public boolean notifyShuntDoneMessage(Long groupId,Long queueCount){
        FeiniuCommonRequest feiniuCommonRequest = new FeiniuCommonRequest();
        Map<String,Long> params = new HashMap<>(1);

        params.put("groupid", groupId);
        params.put("queueLength",queueCount);
        feiniuCommonRequest.setMethod("pushQueuetoCs");
        feiniuCommonRequest.setParams(JsonUtil.toJsonString(params));

        try {
            String response = HttpClientUtils.executeGet(businessConfig.getImServerUrl(), "", JsonUtil.toMap(feiniuCommonRequest));
            JsonNode jsonNode = JsonUtil.toNode(response);
            if (jsonNode != null && JsonNodeUtil.getBoolean(jsonNode, "result")) {
                LOGGER.info("通知Imserver排队成功 groudId : " + groupId + " queueCount + " + queueCount.toString());
                return true;
            } else {
                LOGGER.info("通知Imserver排队失败 groudId : " + groupId + " queueCount + " + queueCount.toString());
                return false;
            }
        } catch (Exception e) {
            LOGGER.error("通知Imserver分流成功失败", e);
            return false;
        }
    }

//    public void flushUserInfoFromImServerToRedis(Long userId){
//        CachedUserInfo cachedUserInfo = null;
//        //1.遍历所有的IMSERVER获取UserInfo信息。
//        String[] imServerIpList = businessConfig.getImServerIpList().split(";");
//        for (String imServerIpIndex :imServerIpList){
//            CachedUserInfo cachedUserInfoIndex = getUserInfoFromImserverByServiceIp(imServerIpIndex, userId);
//            if (cachedUserInfoIndex !=null){
//                cachedUserInfo= cachedUserInfoIndex;
//                break;
//            }
//        }
//        //2.如果所有的Imserver没有该userinfo信息，读取数据库获取后填充默认值，
//        if (cachedUserInfo == null){
//            cachedUserInfo = userInfoDao.getCachedUserInfoInDB(userId);
//        }
//        //3.刷新到redis
//        flushUserInfoToRedis(cachedUserInfo);
//        return ;
//    }
//
//    public void  flushUserInfoToRedis(CachedUserInfo redisUserInfoFull){
//
//        if (redisUserInfoFull == null)
//            return;
//
//        String infoKey = ShuntConfig.MEMCACHE_USER_INFO + "#" + businessConfig.getMagicNumber()
//                + "#" + redisUserInfoFull.getId();
//        try {
//            cacheClientService.delete(infoKey);
//            cacheClientService.putHValue(infoKey,redisUserInfoFull);
//        }catch (Exception e){
//            LOGGER.error("将Userinfo刷新到缓存失败" + infoKey , e);
//        }
//        return;
//    }

    //增加接待关系到redis
    public boolean addReceptionRelationShip(Long groupId, Long serviceId, Long customerId,boolean needCustomerTag){
        String customerinfoKey = ShuntConfig.MEMCACHE_USER_INFO + "#" + businessConfig.getMagicNumber()
                + "#" + customerId.toString();

        String receptionListKey = ShuntConfig.MEMCACHE_RECPTION_LIST + "#" + businessConfig.getMagicNumber()
                + "#" + serviceId.toString();


        boolean success = cacheClientService.tryLock(customerinfoKey, ShuntConfig.LOCK_EXPIRED, ShuntConfig.LOCK_TIMEOUT);

        if (!success) {
            LOGGER.error("锁住用户信息失败，增加接待关系失败");
            return false;
        }

        try {
            if (redisCacheClient.exists(customerinfoKey) && needCustomerTag) {
                redisCacheClient.hset(customerinfoKey, ShuntConfig.MEMCACHE_FEILD_CS_USERID, serviceId.toString());
                redisCacheClient.hset(customerinfoKey, ShuntConfig.MEMCACHE_FEILD_GROUP_USERID, groupId.toString());
            }
            redisCacheClient.hput(receptionListKey, customerId.toString(), "0");
        } catch (Exception e) {
            cacheClientService.releaseLock(customerinfoKey);
            e.printStackTrace();
            return false;
        }
        cacheClientService.releaseLock(customerinfoKey);

        return true;
    }
    //修改用户接待上限
    public void modifyCsReceptionLimit(Long id,Integer receptionLimit){
        String infoKey = ShuntConfig.MEMCACHE_USER_INFO + "#" + businessConfig.getMagicNumber()
                + "#" + id.toString();

        if (!redisCacheClient.exists(infoKey)){
            return;
        }

        redisCacheClient.hput(infoKey,"receptionLimit",receptionLimit.toString());

        return;
    }
    //删除用户上下文
    public void  removeUserInfo(Long id){
        String serverInfoKey = ShuntConfig.MEMCACHE_USER_INFO + "#" + businessConfig.getMagicNumber()
                + "#" + id.toString();

        redisCacheClient.remove(serverInfoKey);
    }

    /**
     * 一个客户正在被接待，删除接待关系
     */
    public boolean  removeReceptionRelationIfRelated( Long serviceId, Long customerId){
        String customerinfoKey = ShuntConfig.MEMCACHE_USER_INFO + "#" + businessConfig.getMagicNumber()
                + "#" + customerId.toString();

        String receptionListKey = ShuntConfig.MEMCACHE_RECPTION_LIST + "#" + businessConfig.getMagicNumber()
                + "#" + serviceId.toString();

        boolean success = cacheClientService.tryLock(customerinfoKey, ShuntConfig.LOCK_EXPIRED, ShuntConfig.LOCK_TIMEOUT);

        if (!success) {
            LOGGER.error("锁住用户信息失败，删除接待关系失败");
            return false;
        }

        try {
            if (redisCacheClient.exists(customerinfoKey)) {
                String csUserId = redisCacheClient.hget(customerinfoKey,ShuntConfig.MEMCACHE_FEILD_CS_USERID);
                if (csUserId != null && csUserId.equals(serviceId.toString())) {
                    redisCacheClient.hset(customerinfoKey, ShuntConfig.MEMCACHE_FEILD_CS_USERID, "");
                    redisCacheClient.hset(customerinfoKey, ShuntConfig.MEMCACHE_FEILD_GROUP_USERID, "");
                }
            }
            String fileds[] = {customerId.toString()};
            redisCacheClient.hremove(receptionListKey, fileds);
            //结束会话的时候去更新数据库中的sessionInfo

            SessionInfoItem sessionInfoItem = new SessionInfoItem();
            sessionInfoItem.setCustomerId(customerId);
            sessionInfoItem.setServiceId(serviceId);
            sessionInfoItem.setSessionEndTime(System.currentTimeMillis()/1000);
            sessionInfoItem.setEventType(1);
            sessionInfoService.putEventMessage(sessionInfoItem);
        } catch (Exception e) {
            cacheClientService.releaseLock(customerinfoKey);
            e.printStackTrace();
            return false;
        }
        cacheClientService.releaseLock(customerinfoKey);
        return true;
    }
    /**
     * 一个客户正在被接待，删除接待关系
     */
    public boolean  removeReceptionRelation( Long serviceId, Long customerId){
        String customerinfoKey = ShuntConfig.MEMCACHE_USER_INFO + "#" + businessConfig.getMagicNumber()
                + "#" + customerId.toString();

        String receptionListKey = ShuntConfig.MEMCACHE_RECPTION_LIST + "#" + businessConfig.getMagicNumber()
                + "#" + serviceId.toString();

        boolean success = cacheClientService.tryLock(customerinfoKey, ShuntConfig.LOCK_EXPIRED, ShuntConfig.LOCK_TIMEOUT);

        if (!success) {
            LOGGER.error("锁住用户信息失败，删除接待关系失败");
            return false;
        }

        try {
            if (redisCacheClient.exists(customerinfoKey)) {
                redisCacheClient.hset(customerinfoKey, ShuntConfig.MEMCACHE_FEILD_CS_USERID, "");
                redisCacheClient.hset(customerinfoKey, ShuntConfig.MEMCACHE_FEILD_GROUP_USERID, "");
            }
            String fileds[] = {customerId.toString()};
            redisCacheClient.hremove(receptionListKey, fileds);
            //结束会话的时候去更新数据库中的sessionInfo

            SessionInfoItem sessionInfoItem = new SessionInfoItem();
            sessionInfoItem.setCustomerId(customerId);
            sessionInfoItem.setServiceId(serviceId);
            sessionInfoItem.setSessionEndTime(System.currentTimeMillis()/1000);
            sessionInfoItem.setEventType(1);
            sessionInfoService.putEventMessage(sessionInfoItem);
        } catch (Exception e) {
            cacheClientService.releaseLock(customerinfoKey);
            e.printStackTrace();
            return false;
        }
        cacheClientService.releaseLock(customerinfoKey);
        return true;
    }
    /**
     * 一个客户正在被接待，转接给另外一个客服
     */
    public boolean transRecptionRelation(Long groupId, Long serviceId, Long customerId,
                                         Long toServiceId, Long toGroupId) {

        String customerinfoKey = ShuntConfig.MEMCACHE_USER_INFO + "#" + businessConfig.getMagicNumber()
                + "#" + customerId.toString();

        String receptionListKey = ShuntConfig.MEMCACHE_RECPTION_LIST + "#" + businessConfig.getMagicNumber()
                + "#" + serviceId.toString();

        String toReceptionListKey = ShuntConfig.MEMCACHE_RECPTION_LIST + "#" + businessConfig.getMagicNumber()
                + "#" + toServiceId.toString();


        boolean success = cacheClientService.tryLock(customerinfoKey, ShuntConfig.LOCK_EXPIRED, ShuntConfig.LOCK_TIMEOUT);

        if (!success) {
            LOGGER.error("锁住用户信息失败，删除接待关系失败");
            return false;
        }

        try {
            if (redisCacheClient.exists(customerinfoKey)) {
                LOGGER.info("转接会话成功,只更新csuserid字段");
                redisCacheClient.hset(customerinfoKey, ShuntConfig.MEMCACHE_FEILD_CS_USERID, toServiceId.toString());
                //redisCacheClient.hset(customerinfoKey, ShuntConfig.MEMCACHE_FEILD_GROUP_USERID, toGroupId.toString());
            }
            String fileds[] = {customerId.toString()};
            redisCacheClient.hremove(receptionListKey, fileds);
            redisCacheClient.hput(toReceptionListKey, customerId.toString(), "0");
            //通知转接成功，修改会话
            SessionInfoItem sessionInfoItem = new SessionInfoItem();
            sessionInfoItem.setCustomerId(customerId);
            sessionInfoItem.setServiceId(serviceId);
            sessionInfoItem.setTransId(toServiceId);
            sessionInfoItem.setEventType(2);
            sessionInfoService.putEventMessage(sessionInfoItem);
        } catch (Exception e) {
            cacheClientService.releaseLock(customerinfoKey);
            e.printStackTrace();
            return false;
        }
        cacheClientService.releaseLock(customerinfoKey);
        return true;
    }

    //获取用户上下文
    public CachedUserInfo getUserInfo(Long id,Boolean needOnline){
        String infoKey = ShuntConfig.MEMCACHE_USER_INFO + "#" + businessConfig.getMagicNumber()
                + "#" + id.toString();
        try {
            CachedUserInfo userInfo = cacheClientService.getHValue(infoKey, CachedUserInfo.class);
            if (userInfo == null || userInfo.getId() == null){
                if (needOnline)
                    return null;
                else
                    userInfo = userInfoDao.getCachedUserInfoInDB(id);
            }
            return userInfo;
        }catch (Exception e){
            LOGGER.error("获取用户上下文失败 :"+e.toString());
        }
        return null;
    }


    public CachedUserInfo getUserInfo(Long id){

        String infoKey = ShuntConfig.MEMCACHE_USER_INFO + "#" + businessConfig.getMagicNumber()
                + "#" + id.toString();

        try {
            CachedUserInfo userInfo = cacheClientService.getHValue(infoKey, CachedUserInfo.class);
            if (userInfo == null || userInfo.getId() == null){
                userInfo = userInfoDao.getCachedUserInfoInDB(id);
            }
            return userInfo;
        }catch (Exception e){
            LOGGER.error("获取用户上下文失败 :"+e.toString());
        }
        return null;
    }

    //获取客服状态
    public Integer getServerOnlineStatus(Long serviceId) {
        String customerinfoKey = ShuntConfig.MEMCACHE_USER_INFO + "#" + businessConfig.getMagicNumber()
                + "#" + serviceId;

        try {
            String status = redisCacheClient.hget(customerinfoKey,
                    ShuntConfig.MEMCACHE_FEILD_ONLINE_STATUS);
            if (status == null)
                return -1;
            else
                return Integer.valueOf(status);
        } catch (Exception e) {
            LOGGER.error("获取用户在线状态失败 : " + customerinfoKey);
            return -1;
        }
    }

    public boolean getCustomerOnlineStats(Long customerId){

        String customerinfoKey = ShuntConfig.MEMCACHE_USER_INFO + "#" + businessConfig.getMagicNumber()
                + "#" + customerId;

        try {
            String lanIp = redisCacheClient.hget(customerinfoKey,
                    ShuntConfig.MEMCACHE_FEILD_LAN_IP);

            if (lanIp == null)
                return  false;

            String webServerIp = FormatUtils.longToIP(Long.valueOf(lanIp));

            Map<String,Object> params = new HashMap<>(1);
            params.put("userIDList",customerId);
            params.put("VENDER_INFO_TYPE","QUERY_USER_LOGIN_INFO");

            String response = HttpClientUtils.executeGet(webServerIp,
                    ":8080/webim/vender",
                    params);

            JsonNode jsonNode = JsonUtil.toNode(response);
            if (jsonNode != null && JsonNodeUtil.getBoolean(jsonNode, "success")) {
                return  JsonNodeUtil.getBoolean(jsonNode.get("data").get("userLoginInfo"), customerId.toString());
            } else {
                return  false;
            }
        } catch (Exception e) {
            LOGGER.error("获取用户在线状态失败 : " + customerinfoKey);
            return  false;
        }
    }

    //设置用户在线状态
    public void setUserOnlineStatus(Long serviceId, Integer status) {
        String customerinfoKey = ShuntConfig.MEMCACHE_USER_INFO + "#" + businessConfig.getMagicNumber()
                + "#" + serviceId;

        try {
            redisCacheClient.hset(customerinfoKey,
                    ShuntConfig.MEMCACHE_FEILD_ONLINE_STATUS, status.toString());
            return;
        } catch (Exception e) {
            LOGGER.error("设置用户在线状态失败 : " + customerinfoKey);
            return;
        }
    }

    public KVPair getRecptionServiceIds(Long customerId){
        String customerinfoKey = ShuntConfig.MEMCACHE_USER_INFO + "#" + businessConfig.getMagicNumber()
                + "#" + customerId;
        KVPair<Long,Long> resultPair = new KVPair<>();
        try {
            String csid = redisCacheClient.hget(customerinfoKey, ShuntConfig.MEMCACHE_FEILD_CS_USERID);
            String groupId = redisCacheClient.hget(customerinfoKey, ShuntConfig.MEMCACHE_FEILD_CSTEAMID);
            if (csid != null && !csid.isEmpty() && !csid.equals("0")) {
                resultPair.setKey(Long.valueOf(csid));
                resultPair.setValue(Long.valueOf(groupId));
                return resultPair;
            }
            return null;
        } catch (Exception e) {
            LOGGER.error("获取用户是否被接待失败 : " + customerinfoKey);
            return null;
        }
    }

    //判断一个客户是否正在被接待
    public boolean isCustomerReceptioning(Long customerId) {
        String customerinfoKey = ShuntConfig.MEMCACHE_USER_INFO + "#" + businessConfig.getMagicNumber()
                + "#" + customerId;

        try {
            String csid = redisCacheClient.hget(customerinfoKey, ShuntConfig.MEMCACHE_FEILD_CS_USERID);
            if (csid != null && !csid.isEmpty()&& !csid.equals("0")) {
                return true;
            }
            return false;
        } catch (Exception e) {
            LOGGER.error("获取用户是否被接待失败 : " + customerinfoKey);
            return false;
        }
    }


}
