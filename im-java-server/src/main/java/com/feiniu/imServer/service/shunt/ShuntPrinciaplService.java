package com.feiniu.imServer.service.shunt;

import com.feiniu.imServer.base.ResponseResult;
import com.feiniu.imServer.bean.*;
import com.feiniu.imServer.service.external.ImLoggerService;
import com.feiniu.imServer.service.im.ImService;
import com.feiniu.imServer.service.external.MerchantService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * Created by wangzhen on 2016/2/29.
 */
@Service
public class ShuntPrinciaplService {
    private static final Logger LOGGER = LoggerFactory.getLogger(ShuntPrinciaplService.class);

    @Autowired
    ShuntService shuntService;
    @Autowired
    MerchantService merchantService;
    @Autowired
    ImService imservice;
    @Autowired
    ImLoggerService imLoggerService;

    private QueueResponse getEmptyQueueResponse(Integer id,String trueName,String userName){
        QueueResponse queueResponse =new QueueResponse();
        queueResponse.setId(id);
        queueResponse.setTruename(trueName);
        queueResponse.setUsername(userName);
        queueResponse.setHeadImgUrl("");
        queueResponse.setIsOnline(false);
        queueResponse.setQueueLength(0);
        queueResponse.setUsersign("");
        return queueResponse;
    }

    private QueueResponse convertUserInfoToQueueResponse(CachedUserInfo resultServiceInfo,boolean isOnline,boolean isProxy){
        QueueResponse queueResponse = new QueueResponse();
        queueResponse.setHeadImgUrl(resultServiceInfo.getHeadURL());
        queueResponse.setId(Integer.valueOf(resultServiceInfo.getId()));
        queueResponse.setTruename(resultServiceInfo.getNickname());
        queueResponse.setUsername(resultServiceInfo.getUsername());
        if (resultServiceInfo.getUserSign() == null)
            queueResponse.setUsersign("");
        else
            queueResponse.setUsersign(resultServiceInfo.getUserSign());
        queueResponse.setIsOnline(isOnline);
        queueResponse.setIsProxySubAccount(isProxy);
        return queueResponse;
    }

    private QueueResponse getProxyQueueResponse(Long proxySubAccountId){
        LOGGER.info("商城客服分流 : 获取代理账号" + proxySubAccountId.toString() );

        CachedUserInfo resultServiceInfo = shuntService.getServicePrinciaplById(proxySubAccountId, false);

        //如果商城客服的代理账号或者主账号没有登陆过哞哞，需要去加入这个账号到哞哞数据库
        if (resultServiceInfo == null){
            MerchantSubAccountInfo subAccountInfo = merchantService.retrieveMemberInfo(proxySubAccountId.toString());
            if (subAccountInfo == null){
                LOGGER.error("获取代理账号失败，该账号既不在哞哞数据库里也不在商城数据库里");
                return null;
            }

            imLoggerService.saveSeller(proxySubAccountId,
                    subAccountInfo.getUsername(),subAccountInfo.getTruename());

            return  getEmptyQueueResponse(proxySubAccountId.intValue()
                    ,subAccountInfo.getTruename(), subAccountInfo.getUsername());

        }

        return convertUserInfoToQueueResponse(resultServiceInfo,
                !resultServiceInfo.getStatus().equals("-1"),true);
    }

    public QueueResponse doShuntRequestPrinciapl(Long customerId,Long proxySubAccountId,
                                                 List<CachedUserInfo> subMerchantInfoList,Integer searchType){
        //如果在线客服为空，直接返回代理账号
        if (subMerchantInfoList.isEmpty())
            return getProxyQueueResponse(proxySubAccountId);

        QueueResponse queueResponse = null;
        CachedUserInfo resultServiceInfo;

        resultServiceInfo = shuntService.getShuntServicePrinciapl(searchType,
                customerId,subMerchantInfoList);

        //step 6 分流失败，取出代理账号
        if (resultServiceInfo == null) {
            queueResponse = getProxyQueueResponse(proxySubAccountId);
        }else{
            queueResponse = convertUserInfoToQueueResponse(resultServiceInfo, true,false);
        }

        imservice.addReceptionRelationShip(Long.valueOf(0),Long.valueOf(
                queueResponse.getId()),customerId,false);

        return queueResponse;
    }


    public ResponseResult shuntRequestPrinciapl(ShuntRequestPrincipal shuntRequestPrincipal) {
        ResponseResult result = new ResponseResult();
        CachedUserInfo resultServiceInfo;
        List<CachedUserInfo> subMerchantInfoList;
        QueueResponse queueResponse = null;
        int searchType = 0;
        //A.获取最近联系的子账号，判断能否直接分流给该子账号
        //1.根据merchantID获取商家所有的ID列表
        String subMerchantIdList = merchantService.retrieveSubMerchantIdList(shuntRequestPrincipal.getMerchantId(),
                shuntRequestPrincipal.getGroupId());
        Long subMerchantId = null;
        LOGGER.info("商城客服分流 : merchantId" +shuntRequestPrincipal.getMerchantId()
                        +"customerId : " + shuntRequestPrincipal.getCustomerId());

        if (subMerchantIdList != null) {
            //step 2 查找最近联系的客服账号。
            subMerchantId = shuntService.getRecentServiceIdBySubMerantIdList(shuntRequestPrincipal.getCustomerId(),
                    subMerchantIdList);
        } else {
            LOGGER.info("商城客服分流结果 : 该商家没有客服 merchantId:" +  +shuntRequestPrincipal.getMerchantId()
                    +"customerId : " + shuntRequestPrincipal.getCustomerId() );
            result.setMessage("该商家没有客服");
            result.setErrorCode(400);
            return result;
        }

        //2.判断最新联系的账号是否可以分流，如果能分流，直接分流给该账号，返回
        if (subMerchantId != null && shuntService.isServiceCanShunt(subMerchantId)) {
            LOGGER.info("商城客服分流结果 :最近联系客服分流成功 customer "+
                    shuntRequestPrincipal.getCustomerId()+ "分流给"+subMerchantId,toString() );

            resultServiceInfo = shuntService.getServicePrinciaplById(subMerchantId, false);
            queueResponse = convertUserInfoToQueueResponse(resultServiceInfo, true,false);
            result.addData("queueResponse", queueResponse);
            result.markSuccess();
            return result;
        }

        //B.不能直接分流给最近联系的子账号，进行随机分流
        Long proxySubAccountId = Long.valueOf(0);
        //step1 获取商家的分流方式和代理账号
        KVPair<Integer, Long> resultPair = shuntService.getShuntModeByMerchantId(shuntRequestPrincipal.getMerchantId());

        if (resultPair == null){
            LOGGER.info("商城客服分流结果 : 该商家没有客服 merchantId:" +  +shuntRequestPrincipal.getMerchantId()
                    +"customerId : " + shuntRequestPrincipal.getCustomerId() );
            result.setMessage("该商家没有客服");
            result.setErrorCode(400);
            return result;
        }

        searchType = resultPair.getKey();
        proxySubAccountId = resultPair.getValue();

        //获取所有在线并且能接待的子账号，加入到subMerchantInfoList
        subMerchantInfoList = shuntService.getOnlineUserInfoBySubMerchantIdList(subMerchantIdList,shuntRequestPrincipal.getGroupId());

//        如果组内一个在线客服都没有，获取该商家所有的在线客服
        if (subMerchantInfoList.isEmpty() && !shuntRequestPrincipal.getGroupId().
                equals(Long.valueOf(0))) {
            subMerchantIdList = merchantService.retrieveSubMerchantIdList(
                    shuntRequestPrincipal.getMerchantId(),
                    Long.valueOf(0));

            subMerchantInfoList = shuntService.getOnlineUserInfoBySubMerchantIdList(subMerchantIdList,shuntRequestPrincipal.getGroupId());
        }

        queueResponse = doShuntRequestPrinciapl(shuntRequestPrincipal.getCustomerId(),
                proxySubAccountId,subMerchantInfoList,searchType);
        //返回
        if (queueResponse != null) {
            LOGGER.info("商城客服分流结果 : customer " +
                    shuntRequestPrincipal.getCustomerId() + "分流给" +
                    queueResponse.getId());

            result.addData("queueResponse", queueResponse);
            result.markSuccess();
        }else {
            result.setMessage("分流失败");
            result.setErrorCode(500);
        }
        return result;
    }
}
