package com.feiniu.fnSubAccount.service;

import com.feiniu.fnSubAccount.bean.*;
import com.feiniu.fnSubAccount.bean.ImApi.CSMonitorInfoNew;
import com.feiniu.fnSubAccount.bean.shunt.QueueItem;
import com.feiniu.fnSubAccount.bean.shunt.QueueItemShow;
import com.feiniu.fnSubAccount.constants.BusinessConfig;
import com.feiniu.fnSubAccount.consts.FromPortTypeEnum;
import com.feiniu.fnSubAccount.consts.SessionInfoConfig;
import com.feiniu.fnSubAccount.consts.SystemConfig;
import com.feiniu.fnSubAccount.domain.UserInfo;
import com.feiniu.fnSubAccount.repository.CSAccountDao;
import com.feiniu.fnSubAccount.repository.GroupInfoDao;
import com.feiniu.fnSubAccount.repository.SessionInfoDao;
import com.feiniu.fnSubAccount.service.external.BackendService;
import com.feiniu.fnSubAccount.service.external.ImApiService;
import com.feiniu.fnSubAccount.util.AccountUtil.AccountFormatUtil;
import com.feiniu.fnSubAccount.util.FormatUtils;
import com.feiniu.fnSubAccount.util.JsonUtil;
import com.feiniu.fnSubAccount.util.json.JsonNodeUtil;
import com.fn.cache.client.RedisCacheClient;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.*;


/**
 * Created by wangzhen on 2015/11/24.
 */
@SuppressWarnings({ "unused","unchecked", "rawtypes" })
@Service
public class CsMonitorService {
    @Autowired
    RedisCacheClient redisCacheClient;
    @Autowired
    CacheClientService cacheClientService;
    @Autowired
    BusinessConfig businessConfig;
    @Autowired
    BackendService backendService;
    @Autowired
    private GroupInfoDao groupInfoDao;
    @Autowired
    CSAccountDao csAccountDao;
    @Autowired
    ImApiService imApiService;
    @Autowired
    SessionInfoDao sessionInfoDao;

    protected Logger LOGGER = LoggerFactory.getLogger(this.getClass());

    public static long ipToLong(String strIp){
        long[] ip = new long[4];
        //先找到IP地址字符串中.的位置
        int position1 = strIp.indexOf(".");
        int position2 = strIp.indexOf(".", position1 + 1);
        int position3 = strIp.indexOf(".", position2 + 1);
        //将每个.之间的字符串转换成整型
        ip[0] = Long.parseLong(strIp.substring(0, position1));
        ip[1] = Long.parseLong(strIp.substring(position1+1, position2));
        ip[2] = Long.parseLong(strIp.substring(position2+1, position3));
        ip[3] = Long.parseLong(strIp.substring(position3+1));
        return (ip[3] << 24) + (ip[2] << 16) + (ip[1] << 8) + ip[0];
    }

    public void  addInfoToProfile(CSMonitorProfile csMonitorProfile,CSMonitorInfoNew indexInfo){
        csMonitorProfile.setOnlineCSCount(csMonitorProfile.getOnlineCSCount() + 1);
        if (indexInfo.getConsultCount() >=
                Integer.valueOf(indexInfo.getCachedUserInfo().getReceptionLimit())) {
            csMonitorProfile.setArriveRecpLimitCount(csMonitorProfile.getArriveRecpLimitCount() + 1);
        }
        csMonitorProfile.setConsultCount(csMonitorProfile.getConsultCount() + indexInfo.getConsultCount());
        csMonitorProfile.setCSRecpCount(csMonitorProfile.getCSRecpCount()+ indexInfo.getReceptionCount());
    }

    public void  addInfoToProfile(CSMonitorProfile csMonitorProfile,CSMonitorInfo indexInfo){
        csMonitorProfile.setOnlineCSCount(csMonitorProfile.getOnlineCSCount() + 1);
        if (indexInfo.getCustomerlist()!=null &&
            indexInfo.getCustomerlist().size() >= indexInfo.getRecpLimit()) {
            csMonitorProfile.setArriveRecpLimitCount(csMonitorProfile.getArriveRecpLimitCount() + 1);
        }
        csMonitorProfile.setConsultCount(csMonitorProfile.getConsultCount() + indexInfo.getConsult());
        csMonitorProfile.setCSRecpCount(csMonitorProfile.getCSRecpCount()+ indexInfo.getRecption());
    }

    @Transactional
    private Map<Integer,UserInfo> getCustomerIdMap(Map<String, String> resultMap){

        if (resultMap.keySet().size() == 0)
            return Collections.EMPTY_MAP;

        StringBuilder customerIdList = new StringBuilder("");

        for (String id :  resultMap.keySet()){
            customerIdList.append(id+",");
        }
        List<UserInfo> allUsers = sessionInfoDao.getCustomerListByIds(
                customerIdList.substring(0,customerIdList.length()-1));

        Map<Integer,UserInfo> allUserMap = new HashMap<>();
        for (UserInfo userIndex : allUsers){
            allUserMap.put(userIndex.getUserID(),userIndex);
        }

        return allUserMap;
    }

    public  List<QueueItemShow> getQueueItemList(){
        List<QueueItemShow> resultList = new ArrayList<>();
        String itemKey = SessionInfoConfig.MEMCACHE_CS_QUEUE_ITEM + "#" + businessConfig.getMemcacheMagicNumber();
        Map<String, String> resultMap =  redisCacheClient.hgetAll(itemKey);
        Map<Integer,UserInfo> idMap = getCustomerIdMap(resultMap);

        for (Map.Entry<String, String> index: resultMap.entrySet()){
            QueueItem queueItem = JsonNodeUtil.injectBean(index.getValue(), QueueItem.class);
            resultList.add(convertQueueItemToShow(queueItem, idMap));
        }

        return resultList;
    }
    public QueueItemShow convertQueueItemToShow(QueueItem queueItem,Map<Integer,UserInfo> idMap){
        QueueItemShow queueItemShow = new QueueItemShow();
        UserInfo user= idMap.get(queueItem.getCustomerId().intValue());
        if (user == null)
            queueItemShow.setCustomerName("--");
        else
            queueItemShow.setCustomerName(user.getNickName()+
                    "("+SystemConfig.USER_LEVEL_DESC[queueItem.getCustomerLevel()]+")");

        String fromDevice=SessionInfoConfig.FROM_DEVICE_TYPE_NAME[queueItem.getFromDevice()]
                +"-"+ FromPortTypeEnum.getInfo(queueItem.getFromPage()).getShowInfo();
        queueItemShow.setCustomerSource(fromDevice);

        Long waitingTime = (System.currentTimeMillis()/1000 - queueItem.getWaitingTime());
        queueItemShow.setCustomerWaitingTime(
                FormatUtils.formatNumberToSecTime(waitingTime.intValue()));

        return queueItemShow;
    }

    public Integer getQueueCountByGroupId(String groupId){
        try {
            String infoKey = SystemConfig.MEMCACHE_CS_QUEUE+"#"+businessConfig.getMemcacheMagicNumber()
                    +"#"+groupId;
            CSMonitorInfo infos = new CSMonitorInfo();
            Long infoResult = redisCacheClient.zcard(infoKey);

            if (infoResult == null)
                return  0;

            return infoResult.intValue();

        } catch (Exception e) {
            LOGGER.error(e.toString());
            return  0;
        }
    }

    public Integer getQueueCountTotal(){
        List<ServiceGroupInfo> groupIdArrayList =  groupInfoDao.getAllGroup();
        Integer result = 0;
        for (ServiceGroupInfo groupIndex: groupIdArrayList){
            result += getQueueCountByGroupId(groupIndex.getGroupId());
        }
        result += getQueueCountByGroupId("0");
        return  result;
    }

    public CSMonitorProfile getProfile(Map<Long,CSMonitorInfo> csMonitorInfoList){
        if (csMonitorInfoList == null)
            return new CSMonitorProfile();

        CSMonitorProfile result = new CSMonitorProfile();

        for (Map.Entry<Long,CSMonitorInfo> entry : csMonitorInfoList.entrySet()) {
            addInfoToProfile(result,entry.getValue());
        }

        result.setQueuingCount(getQueueCountTotal());
        result.setLastrefreshTime(FormatUtils.formatTime(new Date()));
        return  result;
    }

    public CSMonitorProfile getProfileNew(Map<Long,CSMonitorInfoNew> csMonitorInfoList){
        if (csMonitorInfoList == null)
            return new CSMonitorProfile();

        CSMonitorProfile result = new CSMonitorProfile();

        for (Map.Entry<Long,CSMonitorInfoNew> entry : csMonitorInfoList.entrySet()) {
            addInfoToProfile(result,entry.getValue());
        }

        result.setQueuingCount(getQueueCountTotal());
        result.setLastrefreshTime(FormatUtils.formatTime(new Date()));
        return  result;
    }

    private CSMonitorInfo getMonitorInfoById(String id){
        try {

            String infoKey = SystemConfig.MEMCACHE_SERVICE_INFO+"#"+businessConfig.getMemcacheMagicNumber()
                    +"#"+id;
            CSMonitorInfo infos = new CSMonitorInfo();
            String infoResult = (String) cacheClientService.getValueAsString(infoKey);
            CSMonitorInfo  memcheCSInfo = JsonNodeUtil.injectBeanWithoutCaseParse(infoResult, CSMonitorInfo.class, false);

            return memcheCSInfo;
        } catch (Exception e) {
            LOGGER.error(e.toString());
            return  null;
        }
    }

    private List<CSMonitorInfo> getAllMonitorInfoListByIp(String ipAddress){
        Long ipInLong = ipToLong(ipAddress);

        List<CSMonitorInfo> result = new ArrayList<>();
        try {
            String idKey = SystemConfig.MEMCACHE_SERVICE_LIST+"#"+businessConfig.getMemcacheMagicNumber()
                    +"#"+ipInLong.toString();

            String idResult =(String) cacheClientService.getValueAsString(idKey);

            if (idResult == null){
                return  result;
            }
            String idsStr = JsonNodeUtil.getString(JsonUtil.toNode(idResult), "ids");

            if (idsStr == null){
                return result;
            }
            List<String> list = JsonUtil.toList(idsStr, String.class);

            for (String idIndex: list){
                CSMonitorInfo memcheCSInfoNum = getMonitorInfoById(idIndex);
                if (memcheCSInfoNum != null){
                    result.add(memcheCSInfoNum);
                }
            }
            return result;
        } catch (Exception e) {
            LOGGER.error(e.toString());
        }
        return result;

    }

    public Map<Long,CSMonitorInfo> getAllMonitorInfoList(String serverIpList){
        List<String> serverIpArrayList = Arrays.asList(serverIpList.split(";"));

        Map<Long,CSMonitorInfo> totoalReust = new HashMap<>();

        for (String ipAddressIndex: serverIpArrayList){
            List<CSMonitorInfo> resultInServer = getAllMonitorInfoListByIp(ipAddressIndex);
            for (CSMonitorInfo indexInfo : resultInServer){
                totoalReust.put(indexInfo.getCServiceId(),indexInfo);
            }
        }
        return totoalReust;
    }


    public CSMonitorProfile  doGetMonitorReuslt(int startRow,int maxResults,List<CSMoitorInfoShow> csMonitorInfoList){

        CSMonitorProfile csMonitorProfile = null;
        List<CSAccountInfo> allIdList = csAccountDao.getAccountIdList(startRow,maxResults);
        Integer onlineUserCount = 0;
        //通过Im-api服务器获取客服信息
        if (businessConfig.isUseImApi()){
            Map<Long,CSMonitorInfoNew> resultMap = getAllMonitorInfoListNew();
            csMonitorProfile = getProfileNew(resultMap);
            for (CSAccountInfo idIndex:allIdList){
                CSMonitorInfoNew indexResult = resultMap.get(idIndex.getUserid());
                csMonitorInfoList.add(AccountFormatUtil.fromatCSMoitorInfoNewToshow(indexResult,idIndex.getUserid(),
                        idIndex.getUserName() + "(" + idIndex.getRealName() + ")"));
            }
            onlineUserCount = resultMap.values().size();
        }else {
            Map<Long,CSMonitorInfo> resultMap = getAllMonitorInfoList(businessConfig.getMemcacheServerIp());
            csMonitorProfile = getProfile(resultMap);
            for (CSAccountInfo idIndex:allIdList){
                CSMonitorInfo indexResult = resultMap.get(idIndex.getUserid());
                csMonitorInfoList.add(AccountFormatUtil.fromatCSMoitorInfoToshow(indexResult,idIndex.getUserid(),
                        idIndex.getUserName() + "(" + idIndex.getRealName() + ")"));
            }
            onlineUserCount = resultMap.size();
        }
        Integer idCount = csAccountDao.getAccountIdListCount();
        csMonitorProfile.setCSCount(idCount);
        csMonitorProfile.setOfflineCSCount(idCount - onlineUserCount);
        return  csMonitorProfile;
    }


    public Map<Long,CSMonitorInfoNew> getAllMonitorInfoListNew(){
        List<CSAccountInfo> allIdList = csAccountDao.getAccountIdList(0, SystemConfig.MAX_PAGE_ITEM_SIZE);
        StringBuffer idListStrBuffer = new StringBuffer();
        for (CSAccountInfo infoIndex : allIdList){
            idListStrBuffer.append(String.valueOf(infoIndex.getUserid())+",");
        }
        List<CSMonitorInfoNew> allMonitorInfo = imApiService.getMonitorInfoList(
                idListStrBuffer.substring(0, idListStrBuffer.length() - 1));

        Map<Long,CSMonitorInfoNew> resultMap = new HashMap<>();

        for (CSMonitorInfoNew csMonitorInfoIndex : allMonitorInfo){
            resultMap.put(csMonitorInfoIndex.getcServiceId(),csMonitorInfoIndex);
        }

        return  resultMap;
    }


}
