package com.feiniu.imServer.service.userinfo;

import com.feiniu.imServer.bean.*;
import com.feiniu.imServer.bean.dto.ChatMessage;
import com.feiniu.imServer.constants.BusinessConfig;
import com.feiniu.imServer.consts.ShuntConfig;
import com.feiniu.imServer.repository.CsInfoDao;
import com.feiniu.imServer.repository.RecentInfoDao;
import com.feiniu.imServer.service.external.MerchantService;
import com.feiniu.imServer.service.im.ImService;
import com.feiniu.imServer.service.shunt.KefuQueueInfoService;
import com.feiniu.imServer.util.FormatUtils;
import com.feiniu.imServer.util.JsonUtil;
import com.feiniu.imServer.util.json.JsonNodeUtil;
import com.fn.cache.client.RedisCacheClient;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.*;

/**
 * Created by wangzhen on 2016/2/3.
 */
@Service
public class UserHotInfoService {
    @Autowired
    MerchantService merchantService;
    @Autowired
    RecentInfoDao recentInfoDao;
    @Autowired
    KefuQueueInfoService kefuQueueInfoService;
    @Autowired
    ImService imService;
    @Autowired
    CsInfoDao csInfoDao;
    @Autowired
    private RedisCacheClient redisCacheClient;
    @Autowired
    private BusinessConfig businessConfig;


    public boolean setConsultToReception(Long customerId,Long  serviceId){
        String receptionListKey = ShuntConfig.MEMCACHE_RECPTION_LIST + "#" + businessConfig.getMagicNumber()
                + "#" + serviceId;

        if (redisCacheClient.exists(receptionListKey)){
            if (imService.getCustomerOnlineStats(customerId)){
                redisCacheClient.hset(receptionListKey,customerId.toString(), ShuntConfig.REDIS_RECPTION);
                return true;
            }else {
                String fileds[] = {customerId.toString()};
                redisCacheClient.hremove(receptionListKey,fileds);
                return false;
            }
        }

        return false;
    }


    public List<RecentConnectInfo> flushRecentInfoToRedis(Long customerId){
        String recentListKey = ShuntConfig.RECENT_INFO_CUSTOMER + "#" + businessConfig.getMagicNumber()
                + "#" + customerId;
        List<RecentConnectInfo> resultInfos = recentInfoDao.getRecentServiceInfo(customerId, 20);
        for (RecentConnectInfo recentConnectInfo : resultInfos) {
            redisCacheClient.hput(recentListKey,recentConnectInfo.getId().toString(),
                    JsonUtil.toJsonString(recentConnectInfo));
        }

        redisCacheClient.expire(recentListKey,ShuntConfig.RECENT_INFO_EXPIRE_TIME);
        return resultInfos;
    }

    public boolean putRecentInfo(Long customerId,Long serviceId,ChatMessage chatMessage){
        RecentConnectInfo recentConnectInfo = new RecentConnectInfo();
        recentConnectInfo.setId(serviceId);
        recentConnectInfo.setLastMsg(chatMessage.getData());
        recentConnectInfo.setTime(Long.valueOf(chatMessage.getSend_time()));
        CachedUserInfo cachedUserInfo = imService.getUserInfo(serviceId, false);
        recentConnectInfo.setHeadImgUrl(cachedUserInfo.getHeadURL());
        recentConnectInfo.setNickname(cachedUserInfo.getNickname());
        recentConnectInfo.setUserSign(cachedUserInfo.getUserSign());
        return putRecentInfo(customerId, recentConnectInfo);
    }
    //redis缓存最近联系人
    public boolean putRecentInfo(Long customerId,RecentConnectInfo recentConnectInfo){
        String recentListKey = ShuntConfig.RECENT_INFO_CUSTOMER + "#" + businessConfig.getMagicNumber()
                + "#" + customerId;

        if (!redisCacheClient.exists(recentListKey)){
            flushRecentInfoToRedis(customerId);
        }

        redisCacheClient.hput(recentListKey, recentConnectInfo.getId().toString(),
                JsonUtil.toJsonString(recentConnectInfo));

        redisCacheClient.expire(recentListKey, ShuntConfig.RECENT_INFO_EXPIRE_TIME);
        return false;
    }

    private Long getSendTimeOfRecentService(Long customerId,Long serviceId){
        String recentListKey = ShuntConfig.RECENT_INFO_CUSTOMER + "#" + businessConfig.getMagicNumber()
                + "#" + customerId.toString();

        String recentInfoStr = redisCacheClient.hget(recentListKey, serviceId.toString());

        if (recentInfoStr == null){
            return  null;
        }

        RecentConnectInfo recentConnectInfo =JsonNodeUtil.injectBean(recentInfoStr, RecentConnectInfo.class);
        return recentConnectInfo.getTime();
    }

    public Long getMostRecentServiceIdInList(Long customerId,String[] subMerrchantIdList){
        Long idResultInRedis = null;
        Long sendTime = Long.MIN_VALUE;

        for (String idIndex: subMerrchantIdList){
            Long sendTImeIndex= getSendTimeOfRecentService(customerId, Long.valueOf(idIndex));
            if (sendTImeIndex !=null && sendTImeIndex > sendTime){
                sendTime = sendTImeIndex;
                idResultInRedis = Long.valueOf(idIndex);
            }
        }
        return idResultInRedis;

    }
    public List<RecentConnectInfo> getRecentInfo(Long customerId, Integer maxNumber){
        String recentListKey = ShuntConfig.RECENT_INFO_CUSTOMER + "#" + businessConfig.getMagicNumber()
                + "#" + customerId.toString();

        List<RecentConnectInfo> resultList = new ArrayList<>();
        if (!redisCacheClient.exists(recentListKey)){
            return  flushRecentInfoToRedis(customerId);
        }
        Map<String, String> idToRecentMap = redisCacheClient.hgetAll(recentListKey);
        for(Map.Entry<String, String> entry : idToRecentMap.entrySet()){
            resultList.add(JsonNodeUtil.injectBean(entry.getValue(),RecentConnectInfo.class));
            if (resultList.size()  >= maxNumber){
                return resultList;
            }
        }
        return resultList;
    }



    //获取最近联系人
    public List<RecentConnectInfoShow> getRecentSubMerchantInfo(Long customerId, Integer maxNumber) {
        List<RecentConnectInfo> resultInfos = getRecentInfo(customerId, maxNumber);
        List<RecentConnectInfoShow> resultShowInfos = new ArrayList<>();
        for (RecentConnectInfo recentConnectInfo : resultInfos) {
            RecentConnectInfoShow resultIndex = new RecentConnectInfoShow(recentConnectInfo);
            Integer stat = imService.getServerOnlineStatus(resultIndex.getId());
            if (resultIndex.getUserSign() == null){
                resultIndex.setUserSign("");
            }
            resultIndex.setState(FormatUtils.isStatusOnline(stat) ? 0: 1);
            resultShowInfos.add(resultIndex);
        }
        return resultShowInfos;
    }

    public List<RecentlyMobileSeller> getRecentMerchantInfo(Long customerId, Integer maxNumber) {
        //1.获取最近联系人
        List<RecentConnectInfo> connectInfoList = getRecentInfo(customerId, Integer.MAX_VALUE);
        List<Long> memberIdList = new ArrayList<>();
        for (RecentConnectInfo connectInfo : connectInfoList) {
            memberIdList.add(connectInfo.getId());
        }
        //2.获取所有店铺ID
        Map<String, Integer> merchantIdList = merchantService.retriveMerchantIDsByMemberIDs(memberIdList);

        Map<Integer, RecentlyMobileSeller> merchantIdToRecentMap = new HashMap<>();

        for (RecentConnectInfo connectInfo : connectInfoList) {
            Integer merchantID =merchantIdList.get(connectInfo.getId().toString());
            if (merchantID == null)
                continue;

            RecentlyMobileSeller lastResut = merchantIdToRecentMap.get(merchantID);

            if (lastResut != null && lastResut.getLastMsgTime() > connectInfo.getTime()) {
                continue;
            }
            RecentlyMobileSeller indexResult = new RecentlyMobileSeller();
            indexResult.setLastMsg(connectInfo.getLastMsg());
            indexResult.setLastMsgTime(connectInfo.getTime());
            indexResult.setSubAccountId(connectInfo.getId());
            indexResult.setSubAccountNickname(connectInfo.getNickname());
            indexResult.setMerchantId(merchantID);
            merchantIdToRecentMap.put(merchantID, indexResult);
        }

        return new ArrayList(merchantIdToRecentMap.values());
    }

    //查询多个商家的状态
    public List<Object> queryManyMerchantsOnlineStatus(String merchantIDs){

        List<String> merchantIDsList= Arrays.asList(merchantIDs.split(","));
        List<Object> allStatus = new ArrayList<>();

        for(String merchantID:merchantIDsList){
            Map<String, Object> data = new HashMap<String, Object>();

            Integer status = queryMerchantOnlineStatus(merchantID);
            data.put("status", status);
            data.put("merchantID", merchantID);

            allStatus.add(data);
        }
        return allStatus;
    }

    //查询一个商家的状态
    public Integer queryMerchantOnlineStatus(String merchantID){
        List<Long> subMerchantIds= new ArrayList<>() ;
        Integer status=0;

        List<SubMerchantInfo> subMerchantList = merchantService.retrieveSubMerchantInfo(merchantID);
        for(SubMerchantInfo list:subMerchantList){
            subMerchantIds.add(list.getMemberId());
        }

        for(Long sid :subMerchantIds){
            status = imService.getServerOnlineStatus(sid);
            if (FormatUtils.isStatusOnline(status)){//在线 =0
                return 1;
            }
        }
        return 0;
    }

    //查询子账号状态
    public List<Object> querySubmerchantsOnlineStatus(String subMerchantIDs){

        List<String> subMerchantIDsList= Arrays.asList(subMerchantIDs.split(","));
        List<Object> allStatus = new ArrayList<>();

        for (String subMerchantID:subMerchantIDsList){

            Integer status = imService.getServerOnlineStatus(Long.valueOf(subMerchantID));

            Map<String,Object> data = new HashMap<String,Object>();
            data.put("subMerchantID", subMerchantID);
            if(FormatUtils.isStatusOnline(status)){
                data.put("status",1);//在线
            }else{
                data.put("status",0);
            }

            allStatus.add(data);
        }
        return allStatus;
    }

    //获取客服信息
    public CSInfo getCSInfoById(Long userId){

        return csInfoDao.getCSInfoById(userId);
    }

    //获取客服分组账号
    public List<Long> getCSGroupCountList(Long groupId){

        return csInfoDao.getCSGroupCountList(groupId);
    }
}
