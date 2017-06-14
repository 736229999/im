package com.feiniu.fnSubAccount.controller;

import com.feiniu.common.core.SystemEnv;
import com.feiniu.common.core.UserSession;
import com.feiniu.fnSubAccount.base.ResponseResult;
import com.feiniu.fnSubAccount.bean.OrganNameInfo;
import com.feiniu.fnSubAccount.bean.ServiceGroupDetail;
import com.feiniu.fnSubAccount.bean.ServiceGroupInfo;
import com.feiniu.fnSubAccount.bean.request.GroupModifyStatusRequest;
import com.feiniu.fnSubAccount.bean.request.ServiceGroupDetailRequest;
import com.feiniu.fnSubAccount.bean.request.ServiceGroupRequest;
import com.feiniu.fnSubAccount.bean.request.ServiceGroupSaveRequest;
import com.feiniu.fnSubAccount.constants.EmbedEnum;
import com.feiniu.fnSubAccount.service.AfterSaleBoardService;
import com.feiniu.fnSubAccount.service.GroupInfoService;
import com.feiniu.fnSubAccount.service.external.BackendService;
import com.feiniu.fnSubAccount.util.FormatUtils;
import com.feiniu.fnSubAccount.util.Pager;
import com.feiniu.fnSubAccount.util.json.JsonUtil;
import org.apache.commons.collections4.CollectionUtils;
import org.apache.commons.lang.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import java.text.ParseException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by wangzhen on 2015/10/16.
 */
@Controller
@RequestMapping(value = "serviceGroup")
public class ServiceGroupController {
    @Autowired
    private GroupInfoService groupInfoService;
    @Autowired
    private BackendService backendService;
    @Autowired
    private AfterSaleBoardService afterSaleBoardService;

    private static final Logger LOGGER = LoggerFactory.getLogger(ServiceGroupController.class);

    //  根据分组名称，最后修改人，修改时间查询组信息
    @RequestMapping(value = "get-group-list", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getGroupListResponse(ServiceGroupRequest serviceGroupRequest) throws ParseException {
        ResponseResult responseResult = new ResponseResult();
        //根据组名，最后修改人，最后修改时间去查找组信息。
        //对应页面上第一个查询按钮
        Pager<ServiceGroupInfo> pager = new Pager<ServiceGroupInfo>();

        List<ServiceGroupInfo> infoList = new ArrayList<>();
        int startRow = serviceGroupRequest.getPageSize() * (serviceGroupRequest.getCurrentPageNo() - 1);
        int maxResults = serviceGroupRequest.getPageSize();

        Long totalNum = groupInfoService.getGroupListNum(serviceGroupRequest.getGroupName(),
                serviceGroupRequest.getLastModifiedName(), serviceGroupRequest.getStartTime(), serviceGroupRequest.getEndTime());

        if (totalNum == 0) {
            responseResult.setErrorCode(-2);
            responseResult.setMessage("没有符合的数据");
            return responseResult;
        }

        infoList = groupInfoService.searchGroup(serviceGroupRequest.getGroupName(),
                serviceGroupRequest.getLastModifiedName(), serviceGroupRequest.getStartTime(), serviceGroupRequest.getEndTime(),
                startRow, maxResults);

        List<Integer> groupIDList = new ArrayList<>(1);
        if (CollectionUtils.isNotEmpty(infoList)) {
            for (ServiceGroupInfo serviceGroupInfo : infoList) {
                groupIDList.add(Integer.parseInt(serviceGroupInfo.getGroupId()));
            }

            Map<Integer, Boolean> groupIsBindPortalMap = afterSaleBoardService.checkGroupsIfBind(groupIDList);

            for (ServiceGroupInfo serviceGroupInfo : infoList) {
                String embedSets = serviceGroupInfo.getEmbedSet();
                int groupID = Integer.parseInt(serviceGroupInfo.getGroupId());
                List<Integer> embedIDs = new ArrayList<>(1);

                //portal页是否被绑定
                Boolean isGroupBind = groupIsBindPortalMap.get(groupID);
                if (isGroupBind != null && isGroupBind) {
                    embedIDs.add(0);
                }

                //其他布点页是否被绑定
                if (StringUtils.isNotEmpty(embedSets)) {
                    embedIDs.addAll(com.feiniu.fnSubAccount.util.StringUtils.stringToIntegerList(embedSets, ","));
                }

                //设置显示名称
                if (CollectionUtils.isNotEmpty(embedIDs)) {
                    List<String> embedShowNames = new ArrayList<>(embedIDs.size());

                    for (Integer embedID : embedIDs) {
                        embedShowNames.add(EmbedEnum.getShowName(embedID));
                    }

                    serviceGroupInfo.setEmbedSet(StringUtils.join(embedShowNames, ","));
                }
            }
        }

        pager.setCurrentPageNo(serviceGroupRequest.getCurrentPageNo());
        pager.setPageSize(serviceGroupRequest.getPageSize());
        pager.setTotalRecordSize(totalNum);
        pager.setResultsForCurrentPage(infoList);

        responseResult.addData("pager", pager);

        responseResult.markSuccess();
        return responseResult;

    }

    @RequestMapping(value = "get-group-detail-list", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getGroupListDetailResponse(ServiceGroupDetailRequest serviceGroupDetailRequest) throws ParseException {
        ResponseResult responseResult = new ResponseResult();
        // 1.一个是csAccountRequest中的组ID为空时，需要根据组织，员工，账号名去做查询组的成员信息
        // 2.另外一个查询逻辑是当csAccountRequest中的组ID不为空，根据组ID去做查询组的成员信息
        Pager<ServiceGroupDetail> pager = new Pager<ServiceGroupDetail>();

        int startRow = serviceGroupDetailRequest.getPageSize() * (serviceGroupDetailRequest.getCurrentPageNo() - 1);
        int maxResults = serviceGroupDetailRequest.getPageSize();

        Long totalNum = Long.valueOf(0);

        String groupId = serviceGroupDetailRequest.getGroupId();
        String orgnName = serviceGroupDetailRequest.getOrgnName();
        String serviceName = serviceGroupDetailRequest.getServiceName();
        String moumouName = serviceGroupDetailRequest.getMoumouName();
        //当有id时，为查找组内客服,当id为空时，客服分组设置的查询
        if (groupId != null) {
            pager = groupInfoService.queryGroupMemInfo(groupId, startRow, maxResults);
        } else {//根据组织，员工，账号名查询组成员信息
            pager = groupInfoService.queryGroupMemInfo(orgnName, serviceName, moumouName, startRow, maxResults);
        }

        if (pager == null) {
            responseResult.setErrorCode(-2);
            responseResult.setMessage("没有符合的数据");
            return responseResult;
        }

        pager.setCurrentPageNo(serviceGroupDetailRequest.getCurrentPageNo());
        pager.setPageSize(serviceGroupDetailRequest.getPageSize());
        responseResult.addData("pager", pager);
        responseResult.markSuccess();
        return responseResult;

    }

    //保存组以及组成员
    @RequestMapping(value = "save-group-change", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult saveGroup(ServiceGroupSaveRequest serviceGroupSaveRequest) throws ParseException {
        ResponseResult responseResult = new ResponseResult();
        //获取用户名
        //对应界面上的保存修改按钮
        String userName = "";
        UserSession userSession = SystemEnv.getCurrentUser();
        if (userSession == null || userSession.getUserId() == null) {
            return responseResult;
        }
        userName = userSession.getUserName();
        Long currentTime = System.currentTimeMillis() / 1000;

        //创建组，如果需要
        if (serviceGroupSaveRequest.getNeedAddGroup()) {
            Long addResult = Long.valueOf("0");
            addResult = groupInfoService.addGroup(serviceGroupSaveRequest.getGroupName(),
                    serviceGroupSaveRequest.getGroupDetail(), userName, currentTime);

            if (addResult <= 0) {
                responseResult.setErrorCode(-3);
                responseResult.setMessage("创建组失败");
                return responseResult;
            }
            serviceGroupSaveRequest.setGroupId(String.valueOf(addResult));
            ServiceGroupInfo addedServiceGroupInfo = new ServiceGroupInfo();
            addedServiceGroupInfo.setGroupDescription(serviceGroupSaveRequest.getGroupDetail());
            addedServiceGroupInfo.setGroupId(addResult.toString());
            addedServiceGroupInfo.setGroupName(serviceGroupSaveRequest.getGroupName());
            addedServiceGroupInfo.setLastModifiedName(userName);
            addedServiceGroupInfo.setLastModifiedTime(currentTime.toString());
            responseResult.addData("addedServiceGroupInfo", addedServiceGroupInfo);
        }
        //如果需要修改组的名字或者描述
        if (!serviceGroupSaveRequest.getNeedAddGroup() && serviceGroupSaveRequest.getNeedUpdateDescription()) {
            Long addResult = groupInfoService.changeDescription(serviceGroupSaveRequest.getGroupId(), serviceGroupSaveRequest.getGroupName(),
                    serviceGroupSaveRequest.getGroupDetail(), userName);
            if (addResult <= 0) {
                responseResult.setErrorCode(-2);
                responseResult.setMessage("更新组的描述失败");
                return responseResult;
            }
        }
        //更新组成员
        Long addResult = groupInfoService.addGroupMember(serviceGroupSaveRequest.getGroupId(), serviceGroupSaveRequest.getIdList(), userName);
        if (addResult <= 0) {
            responseResult.setErrorCode(-1);
            responseResult.setMessage("添加组成员失败");
            return responseResult;
        }
        responseResult.addData("moditiedTime", FormatUtils.convertNumberToDateAndTime(currentTime));
        responseResult.addData("moditiedName", userName);
        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "get-group-name-list", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getGroupNameList(String groupNamePart) {
        ResponseResult responseResult = new ResponseResult();
        List<ServiceGroupInfo> infoList = new ArrayList<>();
        infoList = groupInfoService.groupNameList(groupNamePart);
        responseResult.addData("groupInfoList", infoList);
        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "get-last-modified-people-list", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getLastModifiedPeopleList(String lastModifiedPeopleNamePart) {
        ResponseResult responseResult = new ResponseResult();
        List<ServiceGroupInfo> infoList = new ArrayList<>();
        infoList = groupInfoService.lastModifyedPepoleList(lastModifiedPeopleNamePart);
        responseResult.addData("groupInfoList", infoList);
        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "get-orgn-name-list", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult getOrgnNameList(String orgnNamePart) {
        ResponseResult responseResult = new ResponseResult();
        List<OrganNameInfo> infoList = new ArrayList<>();
        infoList = groupInfoService.organNameList(orgnNamePart);
        responseResult.addData("groupInfoList", infoList);
        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "modifyStatus", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult modifyStatus(GroupModifyStatusRequest groupModifyStatusRequest) {
        ResponseResult responseResult = new ResponseResult();
        Long addResult = groupInfoService.changeStatus(groupModifyStatusRequest.getUserId(), groupModifyStatusRequest.getSetStatus(), groupModifyStatusRequest.getGroupId());

        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "deleteGroup", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult deleteGroup(String groupId) {
        ResponseResult responseResult = new ResponseResult();

        boolean isBind = afterSaleBoardService.isGroupBinded(Integer.valueOf(groupId));
        Long memberCount = groupInfoService.getGroupMemNmb(groupId);
        if (isBind || memberCount != 0) {
            responseResult.setMessage("改分组内存在客服或已被布点至页面，无法删除");
            responseResult.setErrorCode(-2);
            return responseResult;
        }
        Long addResult = groupInfoService.deleteGroupInfo(groupId);

        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "save-embed-sets")
    @ResponseBody
    @SuppressWarnings("unchecked")
    public ResponseResult saveEmbedSets(String embedSetJson) {
        ResponseResult responseResult = new ResponseResult();
        Map<Integer, String> groupToEmbedsMap = new HashMap<>(1);
        Map<String, Integer> map = JsonUtil.toMap(embedSetJson);

        for (Map.Entry<String, Integer> entry : map.entrySet()) {
            String embedID = entry.getKey();
            Integer groupID = entry.getValue();

            String value = groupToEmbedsMap.get(groupID);
            if (value == null) {
                value = embedID;
            } else {
                value = value + "," + embedID;
            }

            groupToEmbedsMap.put(groupID, value);
        }

        groupInfoService.saveEmbedSets(groupToEmbedsMap);

        responseResult.markSuccess();
        return responseResult;
    }
}
