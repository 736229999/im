package com.feiniu.fnSubAccount.service;

import com.feiniu.fnSubAccount.bean.CSBackendInfo;
import com.feiniu.fnSubAccount.bean.OrganNameInfo;
import com.feiniu.fnSubAccount.bean.ServiceGroupDetail;
import com.feiniu.fnSubAccount.bean.ServiceGroupInfo;
import com.feiniu.fnSubAccount.constants.BusinessConfig;
import com.feiniu.fnSubAccount.constants.EmbedEnum;
import com.feiniu.fnSubAccount.repository.GroupInfoDao;
import com.feiniu.fnSubAccount.service.external.BackendService;
import com.feiniu.fnSubAccount.service.external.ImApiService;
import com.feiniu.fnSubAccount.util.Pager;
import com.feiniu.fnSubAccount.util.StringUtils;
import com.fn.cache.client.RedisCacheClient;
import org.apache.commons.collections4.MapUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.*;

/**
 * Created by wangzhen on 2015/10/19.
 */
@SuppressWarnings({"unused", "unchecked", "rawtypes"})
@Service
public class GroupInfoService {
    @Autowired
    GroupInfoDao groupInfoDao;

    @Autowired
    private CSAccountService csAccountService;

    @Autowired
    private BackendService backendService;

    @Autowired
    private CacheClientService cacheClientService;

    @Autowired
    RedisCacheClient redisCacheClient;

    @Autowired
    BusinessConfig businessConfig;
    @Autowired
    ImApiService imApiService;

    private static final String EMBED_TO_GROUP_CACHE_KEY = "GROUP_INFO#EMBED_TO_GROUP";
    private static final Logger LOGGER = LoggerFactory.getLogger(GroupInfoService.class);

    //创建组
    public Long addGroup(String groupName, String groupGescription, String importName, Long currentTime) {

        return groupInfoDao.createGroup(groupName, groupGescription, importName, currentTime.toString());

    }

    //修改组名组描述
    public Long changeDescription(String groupId, String groupName, String groupGescription, String importName) {
        Long currentTime = System.currentTimeMillis() / 1000;

        return groupInfoDao.modifyGroup(groupId, groupName, groupGescription, importName, currentTime.toString());
    }

    //新增修改组成员
    public Long addGroupMember(String groupId, String CSId, String importName) {
        Long currentTime = System.currentTimeMillis() / 1000;
        //更新redis。
        imApiService.modifyCsGroupMemInRedis(Long.valueOf(groupId),CSId);

        return groupInfoDao.addGroupMem(groupId, CSId, importName, currentTime.toString());
    }

    //模糊查找获取分组列表
    public List<ServiceGroupInfo> groupNameList(String groupNamePart) {


        return groupInfoDao.queryGroupNameList(groupNamePart);
    }

    //模糊查找获取最后修改人
    public List<ServiceGroupInfo> lastModifyedPepoleList(String lastModifiedPeopleNamePart) {

        return groupInfoDao.querylastModifyedPepoleList(lastModifiedPeopleNamePart);
    }

    //获取组织架构名
    public List<OrganNameInfo> organNameList(String orgnNamePart) {
        List<OrganNameInfo> resultList = backendService.organNameList();
        Iterator<OrganNameInfo> iter = resultList.iterator();
        while (iter.hasNext()) {
            OrganNameInfo s = iter.next();
            if (s.getOrgnName().indexOf(orgnNamePart) < 0) {
                iter.remove();
            }
        }
        return resultList;
    }

    public Long changeStatus(String userId, Long status, String groupId) {

        return groupInfoDao.modifyStatus(userId, status, groupId);
    }

    public Long deleteGroupInfo(String groupId) {
        imApiService.deleteGroupInRedis(Long.valueOf(groupId));
        return groupInfoDao.deleteGroup(groupId);
    }

    public Long getGroupListNum(String groupName, String lastModifyname, String beginTime, String endTime) {
        Long len = groupInfoDao.getGroupListNum(groupName, lastModifyname, beginTime, endTime);

        return len == null ? 0 : len;

    }

    //查找组
    public List<ServiceGroupInfo> searchGroup(String groupName, String lastModifyname, String beginTime, String endTime,
                                              int startRow, int maxResults) {

        return groupInfoDao.queryGroupList(groupName, lastModifyname, beginTime, endTime, startRow, maxResults);
    }

    public Long getGroupMemNmb() {
        Long len = groupInfoDao.getGroupMemNum();
        return len == null ? 0 : len;
    }

    public Long getGroupMemNmb(String groupId) {
        Long len = groupInfoDao.getGroupMemNum(groupId);
        return len == null ? 0 : len;
    }

    public Long getGroupMemNmb(String orgnName, String ServiceName, String MoumouName, String idList) {
        Long len = groupInfoDao.getGroupMemNum(ServiceName, MoumouName, idList);

        return len == null ? 0 : len;
    }

    private void formatServiceGroupDetail(List<ServiceGroupDetail> serviceGroupDetailList) {
        String idList = "";
        for (ServiceGroupDetail indexGroup : serviceGroupDetailList) {
            if (indexGroup.getCsId() != null)
                idList += indexGroup.getCsId() + ",";
        }

        Map<Integer, CSBackendInfo> allmember = backendService.retriveUserInfoByIdList(idList);

        for (int i = 0; i < serviceGroupDetailList.size(); i++) {
            String csid = serviceGroupDetailList.get(i).getCsId();
            if (csid == null || csid == "") {
                serviceGroupDetailList.get(i).setOrgnName("--");
                continue;
            }
            CSBackendInfo csBackendInfo = allmember.get(Integer.valueOf(csid));
            if (csBackendInfo != null) {
                serviceGroupDetailList.get(i).setOrgnName(csBackendInfo.getOrganName());
            }
        }
        return;
    }

    //查询信息：组id
    public Pager<ServiceGroupDetail> queryGroupMemInfo(String groupId, int startRow, int maxResults) {
        Long totalNum = getGroupMemNmb(groupId);

        if (totalNum == 0) {
            return null;
        }

        Pager<ServiceGroupDetail> pager = new Pager<ServiceGroupDetail>();
        List<ServiceGroupDetail> infoList = new ArrayList<>();
        infoList = groupInfoDao.queryGroupMemInfo(groupId, startRow, maxResults);
        formatServiceGroupDetail(infoList);

        pager.setTotalRecordSize(totalNum);
        pager.setResultsForCurrentPage(infoList);
        return pager;
    }

    //查询信息：组织，真实姓名，用户名
    public Pager<ServiceGroupDetail> queryGroupMemInfo(String orgnName, String serviceName, String moumouName,
                                                       int startRow, int maxResults) {
        String idLists = "";
        if (!orgnName.equals("0")) {
            idLists = backendService.getAllMemberInOrgan(Long.valueOf(orgnName));
            if (idLists == "")
                idLists = "-1";
        }

        Long totalNum = getGroupMemNmb(orgnName, serviceName, moumouName, idLists);

        if (totalNum == 0) {
            return null;
        }

        List<ServiceGroupDetail> infoListorcl = new ArrayList<>();
        infoListorcl = groupInfoDao.queryGroupMemInfo(idLists, serviceName, moumouName, startRow, maxResults);
        formatServiceGroupDetail(infoListorcl);

        Pager<ServiceGroupDetail> pager = new Pager<ServiceGroupDetail>();
        pager.setTotalRecordSize(totalNum);
        pager.setResultsForCurrentPage(infoListorcl);
        return pager;
    }

    //获取组信息
    public List<ServiceGroupInfo> getAllGroup() {
        return groupInfoDao.getAllGroup();
    }

    //获取布点与布点相关组信息 的映射
    @SuppressWarnings("unchecked")
    public Map<String, Integer> getEmbedRelatedGroupInfo() {
        Map<String, Integer> embedRelatedGroupInfo = cacheClientService.getValue(EMBED_TO_GROUP_CACHE_KEY, Map.class);

        if (embedRelatedGroupInfo == null) {
            Map<Integer, String> groupToEmbeds = groupInfoDao.getGroupToEmbeds();
            LOGGER.debug("Read From DB");

            if (MapUtils.isNotEmpty(groupToEmbeds)) {
                embedRelatedGroupInfo = new HashMap<>(1);

                for (Map.Entry<Integer, String> entry : groupToEmbeds.entrySet()) {
                    int groupID = entry.getKey();
                    String embeds = entry.getValue();
                    List<Integer> embedArray = StringUtils.stringToIntegerList(embeds, ",");

                    for (Integer embedID : embedArray) {
                        embedRelatedGroupInfo.put(EmbedEnum.getName(embedID), groupID);
                    }
                }

                cacheClientService.put(EMBED_TO_GROUP_CACHE_KEY, embedRelatedGroupInfo);
                return embedRelatedGroupInfo;
            } else {
                cacheClientService.put(EMBED_TO_GROUP_CACHE_KEY, Collections.emptyMap());
                return Collections.emptyMap();
            }
        } else {
            return embedRelatedGroupInfo;
        }
    }

    public void saveEmbedSets(Map<Integer, String> groupToEmbedsMap) {
        groupInfoDao.saveEmbedSets(groupToEmbedsMap);
        cacheClientService.delete(EMBED_TO_GROUP_CACHE_KEY);
    }
}
