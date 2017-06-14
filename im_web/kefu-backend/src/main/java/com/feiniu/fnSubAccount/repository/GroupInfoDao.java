package com.feiniu.fnSubAccount.repository;

import com.feiniu.fnSubAccount.bean.ServiceGroupDetail;
import com.feiniu.fnSubAccount.bean.ServiceGroupInfo;

import java.util.List;
import java.util.Map;

/**
 * Created by wangzhen on 2015/10/19.
 */
public interface GroupInfoDao {
    //查询组信息
    Long getGroupListNum(String groupName, String lastModifyName, String beginTime, String endTime);
    List<ServiceGroupInfo> queryGroupList(String groupName, String lastModifyName, String beginTime, String endTime,int startRow,int maxResults);
    //3.或者所有组信息
    //创建分组
    Long createGroup(String groupName, String groupGescription, String importName, String currentTime) ;
    //修改分组信息
    Long modifyGroup(String groupId, String groupName, String groupGescription, String importName,String currentTime);
    //添加成员到组
    Long addGroupMem(String groupId,String CSId, String importName, String currentTime);
    //模糊搜索组名
    List<ServiceGroupInfo> queryGroupNameList(String groupNamePart);
    //模糊搜索最后修改人
    List<ServiceGroupInfo> querylastModifyedPepoleList(String lastModifiedPeopleNamePart);
    //获取组织名
    List<ServiceGroupDetail> organNameList();
    //修改分流状态
    Long modifyStatus(String userId,Long status,String groupId);
    //删除组
    Long deleteGroup(String groupId);
    //查询组成员
    //1.根据参数查询组成员
    Long getGroupMemNum( String ServiceName, String MoumouName,String idList);
    List<ServiceGroupDetail> queryGroupMemInfo(String idList,String ServiceName, String MoumouName,int startRow,int maxResults);
    //2.获取所有组的组成员
    Long getGroupMemNum();
    List<ServiceGroupDetail> queryGroupMemInfo(int startRow,int maxResults);
    //3.根据groupId查询
    Long getGroupMemNum(String groupId);
    List<ServiceGroupDetail> queryGroupMemInfo(String groupId,int startRow,int maxResults);
    //获取所有组信息
    List<ServiceGroupInfo> getAllGroup();
    //获取与组相关的布点信息
    Map<Integer, String> getGroupToEmbeds();
    //设置布点信息
    void saveEmbedSets(Map<Integer, String> groupToEmbedsMap);
}

