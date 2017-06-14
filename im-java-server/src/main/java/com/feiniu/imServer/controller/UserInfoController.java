package com.feiniu.imServer.controller;

import com.feiniu.imServer.base.ResponseResult;
import com.feiniu.imServer.bean.*;
import com.feiniu.imServer.constants.BusinessConfig;
import com.feiniu.imServer.consts.ShuntConfig;
import com.feiniu.imServer.service.shunt.KefuQueueInfoService;
import com.feiniu.imServer.service.userinfo.PagingMessageService;
import com.feiniu.imServer.service.userinfo.UserHotInfoService;
import com.feiniu.imServer.service.im.ImService;
import com.feiniu.imServer.service.external.MerchantService;
import com.feiniu.imServer.util.FormatUtils;
import com.fn.cache.client.RedisCacheClient;
import org.apache.commons.lang.math.NumberUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by wangzhen on 2016/2/2.
 */
@Controller
@RequestMapping(value = "userInfo")
public class UserInfoController {
    private static final Logger LOGGER = LoggerFactory.getLogger(UserInfoController.class);

    @Autowired
    PagingMessageService pagingMessageService;
    @Autowired
    KefuQueueInfoService kefuQueueInfoService;
    @Autowired
    UserHotInfoService userHotInfoService;
    @Autowired
    MerchantService merchantService;
    @Autowired
    RedisCacheClient redisCacheClient;
    @Autowired
    BusinessConfig businessConfig;
    @Autowired
    ImService imServer;


    //获取历史消息记录的http接口
    @RequestMapping(value = "get-history-message-by-page")
    @ResponseBody
    public ResponseResult getHistroyMsgByPage(HistoryMessageListRequest historyMessageListRequest) {
        ResponseResult result = new ResponseResult();

        List<String> subMerchantIds= new ArrayList<>() ;

        if(historyMessageListRequest.getCustomerId() == null || historyMessageListRequest.getCustomerId()<=0
                ||historyMessageListRequest.getSellerId()==null|| historyMessageListRequest.getSellerId()<=0){

            result.setMessage("参数不全");
            return result;
        }else if(historyMessageListRequest.getPageCount()<=0 ){
            result.setMessage("每页记录数不正确");
            return result;
        }else{
            Integer sellerId = historyMessageListRequest.getSellerId();
            if (historyMessageListRequest.isMerchantId()){
                List<SubMerchantInfo> subMerchantList = merchantService.retrieveSubMerchantInfo(sellerId.toString());
                if(subMerchantList.size()==0){
                    result.setMessage("商家沒有下属");
                    return result;
                }
                for(SubMerchantInfo list:subMerchantList){
                    subMerchantIds.add(String.valueOf(list.getMemberId()));
                }
            }else{
                subMerchantIds.add(sellerId.toString());
            }
            historyMessageListRequest.setSubMerchantIds(subMerchantIds);
            List<HistoryMessage> messageList = pagingMessageService.queryHistoryPagingMsg(historyMessageListRequest);
            if(messageList.size()==0){
                result.setMessage("沒有聊天记录");
            }
            result.addData("histroyMessage", messageList);
            result.markSuccess();
        }

        return result;
    }

    //获取离线人员消息记录的http接口-分页查询
    @RequestMapping(value = "get-offline-message-by-page")
    @ResponseBody
    public ResponseResult getOfflineMsgByPage(OfflineMessageRequest offlineMessageRequest){
        ResponseResult result = new ResponseResult();
        if(offlineMessageRequest.getId()<=0||offlineMessageRequest.getId()==null||offlineMessageRequest.getPageCount()<=0
                || offlineMessageRequest.getStartTime()<=0||offlineMessageRequest.getEndTime()<=0){
            result.setMessage("参数不全或有误");
            return result;
        }

        List<OfflineMessage> messageList=pagingMessageService.queryOfflinePagingMsg(offlineMessageRequest);
        result.addData("offlineMessage", messageList);
        result.markSuccess();
        return result;
    }

    //获取离线人员的消息记录的http接口-根据onlinetime
    @RequestMapping(value = "get-offline-message-list-by-onlinetime")
    @ResponseBody
    public ResponseResult getAllOfflineMsgList(Integer serviceId , Long onlineTime ,Long msgId,Integer pageCount){
        ResponseResult result = new ResponseResult();
        if(!NumberUtils.isNumber(serviceId.toString())|| !NumberUtils.isNumber(onlineTime.toString()) ||serviceId <= 0 || onlineTime <= 0){
            result.setMessage("参数不全或有误");
            return result;

        }
        //根据onlinetime得到上一次离线时间
        Long offlineTime = pagingMessageService.getLastOfflineTimeById(serviceId , onlineTime);
        if(offlineTime == 0){
            result.setMessage("没有离线登录记录，IM_LOGINOUTTIME没有数据");
            return result;
        }
        //只取七天内的离线消息
        Long validTime = onlineTime - 604800;
        if(offlineTime < validTime){
            offlineTime = validTime;
        }

        OfflineMessageRequest offlineMessageRequest = new OfflineMessageRequest();
        offlineMessageRequest.setStartTime(offlineTime);
        offlineMessageRequest.setEndTime(onlineTime);
        offlineMessageRequest.setId(serviceId);

        if(msgId != null && NumberUtils.isNumber(msgId.toString()) ){
            offlineMessageRequest.setMsgId(msgId);
        }
        if(pageCount != null && NumberUtils.isNumber(pageCount.toString())  ){
            offlineMessageRequest.setPageCount(pageCount);
        }

        List<OfflineMessage> messageList = pagingMessageService.queryOfflinePagingMsg(offlineMessageRequest);
        result.addData("offlineMessage", messageList);
        result.markSuccess();
        return result;
    }


    //查询商家状态
    @RequestMapping(value = "get-merchant-online-status")
    @ResponseBody
    public ResponseResult getMerchantOnlineStatus(String merchantIDs){
        ResponseResult result = new ResponseResult();
        List<Object> statuses = userHotInfoService.queryManyMerchantsOnlineStatus(merchantIDs);
        LOGGER.debug("查询商家状态：在线=1，不在线=0：" + statuses);
        result.addData("statuses", statuses);
        result.markSuccess();
        return result;
    }

    //查询客服子账号状态
    @RequestMapping(value = "get-subMerchant-online-status")
    @ResponseBody
    public ResponseResult getUserOnlineStatus(String subMerchantIDs){
        ResponseResult result = new ResponseResult();
        List<Object> statuses = userHotInfoService.querySubmerchantsOnlineStatus(subMerchantIDs);
        result.addData("statuses", statuses);
        LOGGER.debug("查询客服子账号状态,在线，返回1，不在线返回0：");
        result.markSuccess();
        return result;
    }

    //获取最近联系人
    @RequestMapping(value = "get-recent-subMerchant-info")
    @ResponseBody
    public ResponseResult getRecentSubMerchantInfo(Long customerId,Integer maxNumber){
        ResponseResult result = new ResponseResult();
        List<RecentConnectInfoShow> recentChatInfoList = userHotInfoService.getRecentSubMerchantInfo(customerId, maxNumber);
        result.addData("recentChatInfoList", recentChatInfoList);
        result.markSuccess();
        return result;
    }


    @RequestMapping(value = "get-recent-merchant-info")
    @ResponseBody
    public ResponseResult getRecentMerchantInfo(Long customerId,Integer maxNumber){
        ResponseResult result = new ResponseResult();
        List<RecentlyMobileSeller> recentlyMobileSellerList = userHotInfoService.getRecentMerchantInfo(customerId, maxNumber);
        result.addData("recentlyMobileSellerList",recentlyMobileSellerList);
        result.markSuccess();
        return result;
    }

    //获取自营客服的用户信息
    @RequestMapping(value = "get-cs-monitor-info")
    @ResponseBody
    public ResponseResult getCsMonitorInfo(Long cServiceId){
        ResponseResult result = new ResponseResult();
        CSMonitorInfo monitorInfo = kefuQueueInfoService.getMonitorInfoById(cServiceId, false);
        if (monitorInfo == null){
            result.setMessage("数据为空");
            return  result;
        }
        result.addData("monitorInfo", monitorInfo);
        result.markSuccess();
        return result;
    }

    //批量获取自营客服的用户信息
    @RequestMapping(value = "get-cs-monitor-info-list")
    @ResponseBody
    public ResponseResult getCsMonitorInfo(String cServiceIds){
        ResponseResult result = new ResponseResult();
        String[] cServiceIdList = cServiceIds.split(",");
        List<CSMonitorInfo> csMonitorInfoList = new ArrayList<>();
        for (String idIndex :cServiceIdList){
            CSMonitorInfo monitorInfo =kefuQueueInfoService.getMonitorInfoById(Long.valueOf(idIndex),false);
            if (monitorInfo != null &&
                    FormatUtils.isStatusOnline(
                            Integer.valueOf(monitorInfo.getCachedUserInfo().getStatus())))
                csMonitorInfoList.add(monitorInfo);
        }
        if (csMonitorInfoList.isEmpty()){
            result.setMessage("数据为空");
        }else {
            result.addData("csMonitorInfoList", csMonitorInfoList);
            result.markSuccess();
        }
        return result;
    }


    //修改客服组成员redis,kefu-backend数据异动时候调用该接口
    @RequestMapping(value = "modify-cs-group-member")
    @ResponseBody
    public ResponseResult modifyGroupMem(Long groupId,String CSIds){
        ResponseResult result = new ResponseResult();
        //更新redis。
        String infoKey = ShuntConfig.MEMCACHE_SERVICE_IN_GROUP + "#" + businessConfig.getMagicNumber()
                + "#" + groupId.toString();
        redisCacheClient.remove(infoKey);

        String[] idList = CSIds.split(",");
        for (String idIndex : idList){
            if(!idIndex.isEmpty())
                redisCacheClient.lpush(infoKey, idIndex);
        }

        result.markSuccess();
        return result;
    }

    //删除客服组redis,kefu-backend数据异动时候调用该接口
    @RequestMapping(value = "delete-cs-group")
    @ResponseBody
    public ResponseResult deleteGroup(Long groupId){
        ResponseResult result = new ResponseResult();
        //更新redis。
        String infoKey = ShuntConfig.MEMCACHE_SERVICE_IN_GROUP + "#" + businessConfig.getMagicNumber()
                + "#" + groupId.toString();
        redisCacheClient.remove(infoKey);
        result.markSuccess();
        return result;
    }

    //删除客服接待上限发生变化，kefu-backend数据异动时候调用该接口
    @RequestMapping(value = "modify-cs-reception-limit")
    @ResponseBody
    public ResponseResult modifyCsReceptionLimit(Long id,Integer receptionLimit){
        ResponseResult result = new ResponseResult();

        imServer.modifyCsReceptionLimit(id, receptionLimit);

        result.markSuccess();
        return result;
    }


    //删除商家分流信息发生变化，kefu-merchant数据异动时候调用该接口
    @RequestMapping(value = "modify-merchant-shunt-info")
    @ResponseBody
    public ResponseResult modifyMerchantShuntInfo(Integer merchantId,Integer shuntType, Integer proxySubAcountId){
        ResponseResult result = new ResponseResult();

        String infoKey = ShuntConfig.MEMCACHE_INFO + "#" + businessConfig.getMagicNumber()
                + "#" + String.valueOf(merchantId);

        redisCacheClient.hput(infoKey, "shuntType", String.valueOf(shuntType));
        redisCacheClient.hput(infoKey, "proxy_subAcount_id", String.valueOf(proxySubAcountId));

        result.markSuccess();
        return result;
    }

    //获取客服信息
    @RequestMapping(value = "get-CS-info-by-id")
    @ResponseBody
    public ResponseResult getCSInfoById(Long userId){
        ResponseResult result = new ResponseResult();
        CSInfo csInfo = userHotInfoService.getCSInfoById(userId);
        result.addData("CSInfo",csInfo);
        result.markSuccess();
        result.markSuccess();
        return  result;
    }

    //获取客服分组帐号
    @RequestMapping(value = "get-cs-group-count-list")
    @ResponseBody
    public ResponseResult getCSGroupCountList(Long groupId){
        ResponseResult result = new ResponseResult();
        List<Long> teamUserIds = userHotInfoService.getCSGroupCountList(groupId);
        result.addData("TeamUser",teamUserIds);
        result.markSuccess();
        result.markSuccess();
        return  result;
    }


}
