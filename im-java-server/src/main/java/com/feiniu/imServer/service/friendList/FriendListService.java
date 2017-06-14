package com.feiniu.imServer.service.friendList;

import com.feiniu.imServer.bean.SubMerchantInfo;
import com.feiniu.imServer.bean.backend.CSBackendInfo;
import com.feiniu.imServer.bean.backend.OrganNameInfo;
import com.feiniu.imServer.bean.friendList.FriendItem;
import com.feiniu.imServer.constants.BackendSetting;
import com.feiniu.imServer.constants.BusinessConfig;
import com.feiniu.imServer.consts.BackendConfig;
import com.feiniu.imServer.consts.ShuntConfig;
import com.feiniu.imServer.consts.SystemConfig;
import com.feiniu.imServer.repository.CsInfoDao;
import com.feiniu.imServer.repository.FriendInfoDao;
import com.feiniu.imServer.service.base.CacheClientService;
import com.feiniu.imServer.service.external.BackendService;
import com.feiniu.imServer.service.external.MerchantService;
import com.feiniu.imServer.service.im.ImService;
import com.feiniu.imServer.util.JsonUtil;
import com.feiniu.imServer.util.json.JsonNodeUtil;
import com.fn.cache.client.RedisCacheClient;
import org.codehaus.jackson.JsonNode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * Created by wangzhen on 2016/3/14.
 */
@Service
public class FriendListService {

    private static final Logger LOGGER = LoggerFactory.getLogger(FriendListService.class);

    @Autowired
    FriendInfoDao friendInfoDao;
    @Autowired
    RedisCacheClient redisCacheClient;
    @Autowired
    CacheClientService cacheClientService;
    @Autowired
    BusinessConfig businessConfig;
    @Autowired
    ImService imService;
    @Autowired
    MerchantService merchantService;
    @Autowired
    BackendService backendService;
    @Autowired
    BackendSetting backendSetting;
    @Autowired
    CsInfoDao csInfoDao;

    private void  flushGroupItemsToRedis(Long userId, List<OrganNameInfo> organNameInfos){
        String infoKey = ShuntConfig.MEMCACHE_FRIEND_GROUP_LIST + "#" + businessConfig.getMagicNumber()
                + "#" + userId.toString();

        for (OrganNameInfo organNameInfo:organNameInfos){
            redisCacheClient.hput(infoKey, organNameInfo.getId().toString(),
                    JsonUtil.toJsonString(organNameInfo));
        }

        if (userId == SystemConfig.MIN_SELF_SERVICE_ID){
            redisCacheClient.expire(infoKey,ShuntConfig.FRIEND_SELF_LIST_TIMEOUT);
        }else {
            redisCacheClient.expire(infoKey,ShuntConfig.FRIEND_LIST_TIMEOUT);
        }
        return;
    }

    private void  flushFriendItemsToRedis(Long userId, List<FriendItem> friendItemList){
        String infoKey = ShuntConfig.MEMCACHE_FRIEND_LIST + "#" + businessConfig.getMagicNumber()
                + "#" + userId.toString();

        for (FriendItem friendItemIndex:friendItemList){
            if ( !friendItemIndex.getGroupId().equals(Long.valueOf(250)) ){//好友
                if (!redisCacheClient.hexists(infoKey,friendItemIndex.getFriendId().toString())) {
                    redisCacheClient.hput(infoKey, friendItemIndex.getFriendId().toString(),
                            friendItemIndex.getGroupId().toString());
                }
            } else {//250团队
                redisCacheClient.hput(infoKey, friendItemIndex.getFriendId().toString(),
                        friendItemIndex.getGroupId().toString());
            }
        }

        if (userId == SystemConfig.MIN_SELF_SERVICE_ID){
            redisCacheClient.expire(infoKey,ShuntConfig.FRIEND_SELF_LIST_TIMEOUT);
        }else {
            redisCacheClient.expire(infoKey,ShuntConfig.FRIEND_LIST_TIMEOUT);
        }
        return;
    }

    private void getStatusOfFriendList(List<FriendItem> friendItemList){
        for (FriendItem friendItem : friendItemList){
            friendItem.setStatus(imService.getServerOnlineStatus(friendItem.getFriendId()));
        }
        return;
    }
    private FriendItem  getFriendItem(Long friendId,Long groupId){
        Integer status = imService.getServerOnlineStatus(friendId);
        FriendItem friendItem = new FriendItem();
        friendItem.setFriendId(friendId);
        friendItem.setGroupId(groupId);
        if (status == -1)
            friendItem.setStatus(BackendConfig.OFF_LINE_STATUS);
        else
            friendItem.setStatus(status);
        return  friendItem;
    }

    private List<FriendItem> getSubMerchantAsFriendList(Long merchantId){
        List<SubMerchantInfo> subMerchantInfoList = merchantService.retrieveSubMerchantInfo(merchantId.toString());
        List<FriendItem> resultItems = new ArrayList<>();
        for (SubMerchantInfo subMerchantInfo : subMerchantInfoList){
            FriendItem friendItemIndex = new FriendItem();
            friendItemIndex.setFriendId(subMerchantInfo.getMemberId());
            friendItemIndex.setGroupId(ShuntConfig.MEMCACHE_FRIEND_GROUP_ID);
            friendItemIndex.setStatus(-1);
            resultItems.add(friendItemIndex);
        }
        return  resultItems;
    }

    private List<FriendItem> getCsServerAsFriendList(){
        Map<Integer,CSBackendInfo> backendInfoMap = backendService.getAllMemberMapInOrgan(
                Long.valueOf(backendSetting.getCustomerServiceOrganId()));

        Map<Long,Integer> MoumouIdToCsIdMap = csInfoDao.getAllUserIdToCsIdMap();

        List<FriendItem> resultList = new ArrayList<>();
        for (Map.Entry<Long,Integer> csMapIndex : MoumouIdToCsIdMap.entrySet()){
            CSBackendInfo  backendInfo = backendInfoMap.get(csMapIndex.getValue());
            if (backendInfo == null)
                continue;
            FriendItem item = getFriendItem(csMapIndex.getKey(),
                    Long.valueOf(backendInfo.getOrganid()) + BackendConfig.ORGAN_ID_TO_GROUP_ID);
            resultList.add(item);
        }

        return  resultList;
    }

    public List<FriendItem> getAllFriendItem(Long userId,Long merchantId){
        String infoKey ;
        Integer friendType = 0;
        if(userId <  SystemConfig.MAX_SERVICE_ID ){//商城
            infoKey = ShuntConfig.MEMCACHE_FRIEND_LIST + "#" + businessConfig.getMagicNumber()
                    + "#" + userId.toString();
            friendType =1;
        }else if(userId >= SystemConfig.MAX_SERVICE_ID && userId < SystemConfig.MIN_SELF_SERVICE_ID){//买家
            infoKey = ShuntConfig.MEMCACHE_FRIEND_LIST + "#" + businessConfig.getMagicNumber()
                    + "#" + userId.toString();
            friendType =2;
        }else if(userId >=SystemConfig.MIN_SELF_SERVICE_ID && userId < SystemConfig.MIN_Work_SERVICE_ID){//自营
            infoKey = ShuntConfig.MEMCACHE_FRIEND_LIST + "#" + businessConfig.getMagicNumber()
                    + "#" + SystemConfig.MIN_SELF_SERVICE_ID;
            friendType =3;
        }else{//工作版
            infoKey = ShuntConfig.MEMCACHE_FRIEND_LIST + "#" + businessConfig.getMagicNumber()
                    + "#" + userId.toString();
            friendType =4;
        }

        Map<String, String> resultMap = redisCacheClient.hgetAll(infoKey);
        List<FriendItem> resultList;

        if (resultMap == null || resultMap.values().size() == 0){
            if (friendType == 1) {//商城
                resultList =  friendInfoDao.getAllFriendItemInDB(userId);
                resultList.addAll(getSubMerchantAsFriendList(merchantId));
                //保存到redis
                flushFriendItemsToRedis(userId, resultList);
                resultMap = redisCacheClient.hgetAll(infoKey);
            } else if(friendType == 2){//买家
                resultList =  friendInfoDao.getAllFriendItemInDB(userId);
                flushFriendItemsToRedis(userId, resultList);
                resultMap = redisCacheClient.hgetAll(infoKey);
            }else if(friendType == 3){//自营
                resultList = getCsServerAsFriendList();
                flushFriendItemsToRedis(SystemConfig.MIN_SELF_SERVICE_ID, resultList);
                return  resultList;
            }else {//工作版
                resultList =  friendInfoDao.getAllFriendItemInDB(userId);
                flushFriendItemsToRedis(userId, resultList);
                resultMap = redisCacheClient.hgetAll(infoKey);
            }

        }


        resultList = new ArrayList<>();
        for (Map.Entry<String, String> entry : resultMap.entrySet()){
            FriendItem friendItemIndex = getFriendItem(Long.valueOf(entry.getKey())
                    , Long.valueOf(entry.getValue()));
            resultList.add(friendItemIndex);
        }

        return  resultList;
    }


    public List<OrganNameInfo> getAllGroupItem(Long userId){
        String infoKey ;
        Integer friendType = 0 ;

        if (userId <  SystemConfig.MAX_SERVICE_ID ){//商城
            infoKey = ShuntConfig.MEMCACHE_FRIEND_GROUP_LIST + "#" + businessConfig.getMagicNumber()
                    + "#" + userId.toString();
            friendType = 1;
        }else if(userId >= SystemConfig.MAX_SERVICE_ID && userId < SystemConfig.MIN_SELF_SERVICE_ID){//买家
            infoKey = ShuntConfig.MEMCACHE_FRIEND_GROUP_LIST + "#" + businessConfig.getMagicNumber()
                    + "#" + userId.toString();
            friendType = 2;
        }else if(userId >=SystemConfig.MIN_SELF_SERVICE_ID && userId < SystemConfig.MIN_Work_SERVICE_ID){//自营
            infoKey = ShuntConfig.MEMCACHE_FRIEND_GROUP_LIST + "#" + businessConfig.getMagicNumber()
                    + "#" + SystemConfig.MIN_SELF_SERVICE_ID;
            friendType = 3;
        }else {//工作版
            infoKey = ShuntConfig.MEMCACHE_FRIEND_GROUP_LIST + "#" +businessConfig.getMagicNumber()
                    + "#" + userId.toString();
            friendType = 4;

        }

        Map<String, String> resultMap = redisCacheClient.hgetAll(infoKey);
        List<OrganNameInfo> resultList;

        if (resultMap == null || resultMap.values().size() == 0){
            if (friendType == 1) {//商家
                resultList =  friendInfoDao.getAllGroupItemInDB(userId);
                flushGroupItemsToRedis(userId, resultList);
            } else if(friendType == 2 ){//买家
                resultList =  friendInfoDao.getAllGroupItemInDB(userId);
                flushGroupItemsToRedis(userId, resultList);
            } else if(friendType == 3){//自营
                resultList = backendService.organNameList();
                for (OrganNameInfo index : resultList){
                    index.setId(index.getId()+100);
                    index.setParentId(index.getParentId() +100);
                }
                flushGroupItemsToRedis(SystemConfig.MIN_SELF_SERVICE_ID, resultList);
            } else {//工作版
                resultList =  friendInfoDao.getAllGroupItemInDB(userId);
                flushGroupItemsToRedis(userId, resultList);
            }
            return  resultList;
        }

        resultList = new ArrayList<>();
        for (Map.Entry<String, String> entry : resultMap.entrySet()){
            JsonNode jsonNode = JsonUtil.toNode(entry.getValue());
            if(jsonNode != null){
                OrganNameInfo organNameInfoIndex = new OrganNameInfo();
                organNameInfoIndex.setId(Long.valueOf(entry.getKey()));
                organNameInfoIndex.setParentId(JsonNodeUtil.getLong(jsonNode, "parentId", 0l));
                organNameInfoIndex.setOrganName(JsonNodeUtil.getString(jsonNode, "organName",""));
                resultList.add(organNameInfoIndex);
            }
        }
        return  resultList;
    }

    //添加好友
    public Integer  addFriendItem(Long userId,Long friendId, Long groupId){
        String infoKey = ShuntConfig.MEMCACHE_FRIEND_LIST + "#" + businessConfig.getMagicNumber()
                + "#" + userId.toString();

        Integer returnCode = friendInfoDao.addFriendItem(userId, friendId, groupId);
        if(returnCode == 0){
            if (redisCacheClient.exists(infoKey)) {
                redisCacheClient.hput(infoKey, friendId.toString(), groupId.toString());
            }
        }
        return returnCode;
    }

    //添加好友分组
    public Integer addFriendGroup(Long userId ,OrganNameInfo organNameInfo){
        String infoKey = ShuntConfig.MEMCACHE_FRIEND_GROUP_LIST + "#" + businessConfig.getMagicNumber()
                + "#" + userId;

        Integer returnCode = friendInfoDao.addFriendGroup(userId, organNameInfo);
        //如果数据库更新成功，修改redis
        if(returnCode == 0){
            if(redisCacheClient.exists(infoKey)){
                redisCacheClient.hput(infoKey, organNameInfo.getId().toString(), JsonUtil.toJsonString(organNameInfo));
            }
        }
        return returnCode;
    }

    //删除好友分组
    public Integer deleteFriendGroup(Long userId ,Long groupId){
        String infoKey = ShuntConfig.MEMCACHE_FRIEND_GROUP_LIST + "#" + businessConfig.getMagicNumber()
                + "#" + userId;

        Integer returnCode = friendInfoDao.deleteFriendGroup(userId, groupId);
        if(returnCode == 0){
            if (redisCacheClient.hexists(infoKey, groupId.toString())) {
                //删除redis里面的好友
                String[] fields = {groupId.toString()};
                redisCacheClient.hremove(infoKey, fields);
            }
        }
        return returnCode;
    }

    //修改好友分组-分组名
    public Integer modifyFriendGroup(Long userId ,Long groupId ,String groupName){
        String infoKey = ShuntConfig.MEMCACHE_FRIEND_GROUP_LIST + "#" + businessConfig.getMagicNumber()
                + "#" + userId.toString();

        Integer returnCode = friendInfoDao.modifyFriendGroup(userId,groupId,groupName);
        if(returnCode == 0){
            if (redisCacheClient.hexists(infoKey,groupId.toString())) {
                String value = redisCacheClient.hget(infoKey, groupId.toString());
                OrganNameInfo organNameInfo = JsonUtil.fromJson(value, OrganNameInfo.class);
                organNameInfo.setOrganName(groupName);

                redisCacheClient.hput(infoKey,groupId.toString(),JsonUtil.toJsonString(organNameInfo));
            }
        }
        return returnCode;
    }

    private  void  deleteGroupIfNeed(Long userId,Long deleteFriendGroupId){

        String infoKey = ShuntConfig.MEMCACHE_FRIEND_LIST + "#" + businessConfig.getMagicNumber()
                + "#" + userId.toString();
        Map<String, String> resultMap = redisCacheClient.hgetAll(infoKey);

        //判断是否要删除group
        Boolean groupIsDelete = true;
        for (Map.Entry<String, String> entry : resultMap.entrySet()){
            if (entry.getValue().equals(deleteFriendGroupId.toString())){
                groupIsDelete = false;
                break;
            }
        }
        //如果需要删除分组，删除group
        if (groupIsDelete) {
            String groupInfoKey = ShuntConfig.MEMCACHE_FRIEND_GROUP_LIST + "#" + businessConfig.getMagicNumber()
                    + "#" + userId.toString();
            String[] groupfields = {deleteFriendGroupId.toString()};
            redisCacheClient.hdel(groupInfoKey, groupfields);
        }
    }

    private void addGroupIfNeed(Long userId, Long addFriendGroupId , String organName){
        String infoKey = ShuntConfig.MEMCACHE_FRIEND_LIST + "#" + businessConfig.getMagicNumber()
                + "#" + userId.toString();
        Map<String,String> resultMap = redisCacheClient.hgetAll(infoKey);
        //判断是否要添加group
        Boolean groupIsAdd = true;
        for (Map.Entry<String, String> entry : resultMap.entrySet()){
            if (entry.getValue().equals(addFriendGroupId.toString())){
                groupIsAdd = false;
                break;
            }
        }
        //如果需要添加分组，添加group
        if (groupIsAdd) {
            String groupInfoKey = ShuntConfig.MEMCACHE_FRIEND_GROUP_LIST + "#" + businessConfig.getMagicNumber()
                    + "#" + userId.toString();
            String groupfields = addFriendGroupId.toString();
            redisCacheClient.hput(groupInfoKey,groupfields,organName);

        }


    }

    //删除好友
    public Integer deleteFriendItem(Long userId,Long friendId){
        String infoKey = ShuntConfig.MEMCACHE_FRIEND_LIST + "#" + businessConfig.getMagicNumber()
                + "#" + userId.toString();

        Integer returnCode = friendInfoDao.deleteFriendItem(userId, friendId);
        if(returnCode == 0){
            if (redisCacheClient.hexists(infoKey,friendId.toString())) {
                //删除redis里面的好友
                String[] fields = {friendId.toString()};
                redisCacheClient.hremove(infoKey, fields);
            }
        }
        return returnCode;
    }

    //修改好友
    public Integer modifyFriendItem(Long userId , Long friendId , Long groupId){
        String infoKey = ShuntConfig.MEMCACHE_FRIEND_LIST + "#" + businessConfig.getMagicNumber()
                + "#" + userId.toString();

        Integer returnCode = friendInfoDao.modifyFriendItem(userId,friendId,groupId);
        if(returnCode == 0){
            if (redisCacheClient.hexists(infoKey,friendId.toString())) {
                redisCacheClient.hput(infoKey,friendId.toString(),groupId.toString());
            }
        }
        return returnCode;
    }
}

