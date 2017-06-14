package com.feiniu.fnSubAccount.service.external;

import com.feiniu.common.util.TreeDto;
import com.feiniu.fnSubAccount.bean.CSBackendInfo;
import com.feiniu.fnSubAccount.bean.CSOrganInfo;
import com.feiniu.fnSubAccount.bean.OrganNameInfo;
import com.feiniu.fnSubAccount.bean.response.BackOperatersByOrganIdsResponse;
import com.feiniu.fnSubAccount.constants.BusinessConfig;
import com.feiniu.fnSubAccount.constants.Constants;
import com.feiniu.fnSubAccount.consts.SystemConfig;
import com.feiniu.fnSubAccount.exception.BusinessException;
import com.feiniu.fnSubAccount.util.StringUtils;
import com.feiniu.fnSubAccount.util.json.JsonNodeUtil;
import com.feiniu.fnSubAccount.util.json.JsonUtil;
import com.feiniu.fnSubAccount.util.web.HttpUtils;
import org.apache.commons.collections.map.HashedMap;
import org.codehaus.jackson.JsonNode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.*;

import static com.feiniu.fnSubAccount.util.json.JsonNodeUtil.injectBean;

/**
 * Created by wangzhen on 2015/11/17.
 */
@SuppressWarnings({ "deprecation","unchecked" })
@Service
public class BackendService {
    @Autowired
    private BusinessConfig businessConfig;

    private static final Logger LOGGER = LoggerFactory.getLogger(BackendService.class);

    //递归获取运营后台组织架构
    private void addAllChildrenToList(TreeDto info, List<OrganNameInfo> organlist,List<String> orgnNameList) {
        List<TreeDto> allChildren = info.getChildren();

        if (allChildren == null) {
            return;
        }

        for (TreeDto index : allChildren) {
            OrganNameInfo root = new OrganNameInfo();

            String finalName = "";
            for (int i =0 ;i< orgnNameList.size();i++){
                finalName += "\\"+orgnNameList.get(i);
            }
            finalName += "\\"+index.getText();
            root.setOrgnName(finalName);
            root.setId(String.valueOf(index.getId()));
            organlist.add(root);
            orgnNameList.add(index.getText());
            addAllChildrenToList(index, organlist, orgnNameList);
            orgnNameList.remove(orgnNameList.size() -1);
        }
        return;
    }

    //获取组织架构名
    public List<OrganNameInfo> organNameList() {
        CSOrganInfo csOrganInfo = retriveOrganInfo();
        List<OrganNameInfo> groupinfoList = new ArrayList<>();
        List<TreeDto> infoList = csOrganInfo.getData();

        for (TreeDto index : infoList) {
            OrganNameInfo root = new OrganNameInfo();
            root.setOrgnName("\\"+index.getText());
            root.setId(String.valueOf(index.getId()));
            List<String> orgnNameList = new ArrayList<>();
            orgnNameList.add(index.getText());
            groupinfoList.add(root);
            addAllChildrenToList(index, groupinfoList,orgnNameList);
        }

        Iterator<OrganNameInfo> iter = groupinfoList.iterator();

        while(iter.hasNext()) {
            if ( iter.next().getOrgnName().indexOf(businessConfig.getCustomerServiceOrganName()) < 0 ) {
                iter.remove();
            }
        }

        return groupinfoList;
    }
    //暂时没有批量的接口，循环调用,等刘宏伟接口
    public Map<Integer,CSBackendInfo> retriveUserInfoByIdList(String idList){
        if (idList == "") return  new HashMap<>();
        List<Integer> splitId = StringUtils.stringToIntegerList(idList,",");
        Map<Integer,CSBackendInfo> resultHashMap = new HashedMap();
        for (Integer idIndex :splitId){
            CSBackendInfo resultIndex = retriveInfoByCsId(idIndex.toString());
            resultHashMap.put(idIndex,resultIndex);
        }
        return resultHashMap;
    }
    public CSBackendInfo retriveUserInfo(int key ,String value){
        String method = SystemConfig.BACKEND_GET_OPERATOR_METHOD[key] + value;
        String response = Constants.NIL_RESPONSE;
        String backendHost = businessConfig.getBackendServiceURL().replace("http://", "");
        CSBackendInfo csBackendInfo = null;
        try {
            if (value == null || value.length() <= 0) {
                csBackendInfo = new CSBackendInfo();
                csBackendInfo.setOrganName("--");
            } else {
                response = HttpUtils.executeGet(backendHost, method,
                        Collections.EMPTY_MAP);
                JsonNode jsonNode = JsonUtil.toNode(response);
                if (jsonNode != null && jsonNode.has("code") && "200".equals(JsonNodeUtil.getString(jsonNode, "code"))) {
                    csBackendInfo = injectBean(jsonNode.get("data").toString(), CSBackendInfo.class);
                    return csBackendInfo;
                } else {
                    LOGGER.error("客服-查找客服接口相关信息失败 :"+method);
                }
            }
        } catch (Exception e) {
            LOGGER.error("客服-查找客服接口相关信息失败  ", e);
            throw new BusinessException("客服-查找客服接口返回错误数据");
        }
        csBackendInfo = new CSBackendInfo();
        csBackendInfo.setOrganName("--");
        csBackendInfo.setOperatorid(-1);
        return csBackendInfo;
    }
    //通过用户名去查找用户数据
    public CSBackendInfo retriveInfoByCsId(String csId) {
        return retriveUserInfo(0,csId);
    }
    //通过用户真实姓名去查找用户数据
    public CSBackendInfo retriveUserInfoByRealName(String realName) {
        return retriveUserInfo(1,realName);
    }

    //通过用户名去查找用户数据
    public CSBackendInfo retriveInfoByUserName(String userName) {
        return retriveUserInfo(2,userName);
    }

    //获取组织架构
    public CSOrganInfo retriveOrganInfo() {
        String response = Constants.NIL_RESPONSE;
        CSOrganInfo csOrganInfo = null;
        String backendHost = businessConfig.getBackendServiceURL().replace("http://", "");
        try {
            //Map<String, Object> map = new HashMap<String, Object>();
            //map.put("merchantIds", StringUtils.join(merchantIDList, ","));
            response = HttpUtils.executeGet(backendHost, "/rest/organ/tree",
                    Collections.EMPTY_MAP);
            JsonNode jsonNode = JsonUtil.toNode(response);
            if (jsonNode != null && jsonNode.has("code") && "200".equals(JsonNodeUtil.getString(jsonNode, "code"))) {
                csOrganInfo = injectBean(response, CSOrganInfo.class, true);
            } else {
                throw new BusinessException("客服-查找客服接口返回错误数据");
            }

        } catch (Exception e) {
            LOGGER.error("客服-查找客服接口相关信息失败", e);
        }
        return csOrganInfo;
    }

    public Map<Integer,CSBackendInfo> getAllMemberListMap(){
        List<CSBackendInfo>  result = getAllMemberList();
        Map<Integer,CSBackendInfo> resultMap = new HashMap<>();
        for (CSBackendInfo index: result){
            resultMap.put(index.getOperatorid(),index);
        }
        return resultMap;
    }

    public List<CSBackendInfo> getAllMemberList(){
//        CSOrganInfo csOrganInfo = retriveOrganInfo();
//        List<TreeDto> infoList = csOrganInfo.getData();
//        List<CSBackendInfo> allList = new ArrayList<>();
//        for (TreeDto index : infoList) {
//            List<CSBackendInfo> listInOrg = getAllMemberListInOrgan(index.getId());
//            allList.addAll(listInOrg);
//        }
//        return allList;
        return getAllMemberListInOrgan(Long.valueOf(businessConfig.getCustomerServiceOrganId()));
    }

    public Map<Integer,CSBackendInfo> getAllMemberMapInOrgan(long orgId){
        List<CSBackendInfo>  result = getAllMemberListInOrgan(orgId);
        Map<Integer,CSBackendInfo> resultMap = new HashMap<>();
        for (CSBackendInfo index: result){
            resultMap.put(index.getOperatorid(),index);
        }
        return resultMap;
    }

    public List<CSBackendInfo> getAllMemberListInOrgan(long orgId) {
        String response = Constants.NIL_RESPONSE;
        String orgUrl = SystemConfig.GET_ORGAN_URL;
        orgUrl = orgUrl.replace(SystemConfig.ORGAN_ID, String.valueOf(orgId));

        String organIds = "";

        String backendHost = businessConfig.getBackendServiceURL().replace("http://", "");
        BackOperatersByOrganIdsResponse backOperatersByOrganIdsResponse;
        Map<String, Object> map = new HashedMap();
        try {
            response = HttpUtils.executeGet(backendHost,
                    orgUrl, map);
            JsonNode jsonNode = com.feiniu.fnSubAccount.util.JsonUtil.toNode(response);
            if (jsonNode != null && jsonNode.has("code") && "200".equals(JsonNodeUtil.getString(jsonNode, "code"))) {
                organIds = JsonNodeUtil.getString(jsonNode, "data");
            } else {
                throw new BusinessException("根据组织ID获取账号ID失败");
            }

            Map<String, Object> params = new HashMap<>(1);
            params.put("organIds", organIds);

            String responseGetOrganIds = HttpUtils.excutePost(SystemConfig.HTTP_URL + backendHost + SystemConfig.GET_BACKOPERATERS_URL,
                    params);

            backOperatersByOrganIdsResponse = JsonNodeUtil.injectBean(responseGetOrganIds, BackOperatersByOrganIdsResponse.class);

            return  backOperatersByOrganIdsResponse.getData();
        } catch (Exception e) {
            LOGGER.error("根据组织ID获取账号ID失败", e);
            return null;
        }
    }

    //根据组织查找所有的成员id
    public String getAllMemberInOrgan(long orgId){
        List<CSBackendInfo> csBackendInfoList = getAllMemberListInOrgan(orgId);
        String subMerchantIDlist = "";
        if (csBackendInfoList ==null)
            return subMerchantIDlist;

        for (CSBackendInfo index: csBackendInfoList){
            subMerchantIDlist += String.valueOf(index.getOperatorid()) + ",";
        }

        if (subMerchantIDlist.length() != 0){
            subMerchantIDlist = subMerchantIDlist.substring(0,subMerchantIDlist.length() -1);
        }

        return  subMerchantIDlist;
    }
}
