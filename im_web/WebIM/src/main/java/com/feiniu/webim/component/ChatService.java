package com.feiniu.webim.component;

import com.feiniu.webim.bean.domain.*;
import com.feiniu.webim.bean.request.*;
import com.feiniu.webim.bean.response.*;
import com.feiniu.webim.bean.web.request.*;
import com.feiniu.webim.communication.CommunicationManager;
import com.feiniu.webim.core.SeqGenerator;
import com.feiniu.webim.core.Session;
import com.feiniu.webim.core.SessionMessageCenter;
import com.feiniu.webim.exception.ImserverException;
import com.feiniu.webim.exception.SendTimeOutException;
import com.feiniu.webim.exception.WrongRequestParamException;
import com.feiniu.webim.statistics.RequestStatisticsBean;
import com.feiniu.webim.statistics.Statistics;
import com.feiniu.webim.util.json.JSONUtil;
import org.apache.commons.collections4.CollectionUtils;
import org.apache.commons.collections4.MapUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;

import java.util.*;

public class ChatService {
    private ChatService() {
    }

    private static final KefuService KEFU_SERVICE = KefuService.instance;
    private static final ChatService CHAT_SERVICE = new ChatService();
    private static final MerchantService MERCHANT_SERVICE = MerchantService.instance;
    private static final ImService IM_SERVICE = ImService.IM_SERVICE;
    private static final ImApiService IM_API_SERVICE = ImApiService.INSTANCE;
    private SeqGenerator seqGenerator = new SeqGenerator();

    private static final Logger LOGGER = Logger.getLogger(ChatService.class);

    public static ChatService getInstance() {
        return CHAT_SERVICE;
    }

    public int login(LoginWebRequest loginWebRequest) {
        RequestStatisticsBean requestStatisticsBean = new RequestStatisticsBean();

        JoinRequestBean joinRequestBean = new JoinRequestBean();
        joinRequestBean.setSeq(seqGenerator.getNextSeq());
        joinRequestBean.setSid(0);
        joinRequestBean.setLogin_flag((byte) 5);
        joinRequestBean.setUsername(loginWebRequest.getSname());
        joinRequestBean.setRandom_key("11111");
        joinRequestBean.setPassword("123456");
        joinRequestBean.setSessionID(loginWebRequest.getSession().getId() + loginWebRequest.getGuid());
        joinRequestBean.setStatus(0);
        joinRequestBean.setMobilephoneno(loginWebRequest.getTelephone());
        joinRequestBean.setEmail(loginWebRequest.getEmail());

        requestStatisticsBean.setCmd(joinRequestBean.getCmd());

        sendRequest(joinRequestBean);

        SessionMessageCenter sessionMessageCenter = loginWebRequest.getSession().getMessageManagerCenter();
        LoginResponseBean message = sessionMessageCenter.retrieveLoginMessage();

        requestStatisticsBean.setReceivedTime(System.currentTimeMillis());
        if (message != null) {
            Statistics.putStaticsSource(RequestStatisticsBean.class, requestStatisticsBean, new Object[0]);
            return message.getUser_id();
        } else {
            requestStatisticsBean.setTimedOut(true);
            Statistics.putStaticsSource(RequestStatisticsBean.class, requestStatisticsBean, new Object[0]);
            throw new ImserverException("登录失败, 服务器响应超时：" + JSONUtil.toJsonString(joinRequestBean));
        }
    }

    public UserInfo getUserInfo(BaseWebRequest baseWebRequest) {
        RequestStatisticsBean requestStatisticsBean = new RequestStatisticsBean();
        Session session = baseWebRequest.getSession();

        int userID = Integer.parseInt(session.getId());
        UserInfoRequestBean userInfoRequestBean = UserInfoRequestBean.getMessage();

        userInfoRequestBean.setSeq(seqGenerator.getNextSeq());
        userInfoRequestBean.setSid(userID);
        userInfoRequestBean.setFid(userID);
        requestStatisticsBean.setCmd(userInfoRequestBean.getCmd());

        sendRequest(userInfoRequestBean);

        SessionMessageCenter sessionMessageCenter = session.getMessageManagerCenter();
        UserInfoResponseBean message = sessionMessageCenter.retrieveUserInfoMessage();

        requestStatisticsBean.setReceivedTime(System.currentTimeMillis());
        if (message != null) {
            Statistics.putStaticsSource(RequestStatisticsBean.class, requestStatisticsBean, new Object[0]);
            return message.getUserInfo();
        } else {
            requestStatisticsBean.setTimedOut(true);
            Statistics.putStaticsSource(RequestStatisticsBean.class, requestStatisticsBean, new Object[0]);
            throw new ImserverException("获取用户个人信息失败, 服务器响应超时" + JSONUtil.toJsonString(userInfoRequestBean));
        }
    }

    /**
     * 处理三种情况：
     * 1. 直接传子账号id：那么从商家接口取子账号信息返回即可；
     * 2. 传主账号id：那么走全局分流接口；
     * 3. 传组账号id：那么走组分流接口
     */
    @Deprecated
    public MerchantSubAccountInfo getMerchantSubAccount(GetSubMerchantWebRequest getSubMerchantWebRequest) {
        RequestStatisticsBean requestStatisticsBean = new RequestStatisticsBean();
        if (getSubMerchantWebRequest.getSub_merchant_id() != -1) {
            MerchantSubAccountInfo merchantSubAccountInfo =
                    MERCHANT_SERVICE.retrieveMemberInfo(getSubMerchantWebRequest.getSub_merchant_id() + "");

            if (merchantSubAccountInfo != null && StringUtils.isNotEmpty(getSubMerchantWebRequest.getNickName())) {
                merchantSubAccountInfo.setTruename(getSubMerchantWebRequest.getNickName());
                merchantSubAccountInfo.setUsername(getSubMerchantWebRequest.getNickName());
            }

            return merchantSubAccountInfo;
        }

        MerchantSubAccountRequestBean merchantSubAccountRequestBean = MerchantSubAccountRequestBean.getMessage();
        merchantSubAccountRequestBean.setSeq(seqGenerator.getNextSeq());
        merchantSubAccountRequestBean.setSid(getSubMerchantWebRequest.getSid());
        merchantSubAccountRequestBean.setMerchant_id(getSubMerchantWebRequest.getPid());

        /**如果是分组分流**/
        if (getSubMerchantWebRequest.getDiverse_type() > 0) {
            if (getSubMerchantWebRequest.getGroup_id() != null) {
                merchantSubAccountRequestBean.setGroup_id(getSubMerchantWebRequest.getGroup_id());
            }
        }
        merchantSubAccountRequestBean.setType(getSubMerchantWebRequest.getDiverse_type());
        requestStatisticsBean.setCmd(merchantSubAccountRequestBean.getCmd());

        sendRequest(merchantSubAccountRequestBean);

        Session session = getSubMerchantWebRequest.getSession();
        SessionMessageCenter sessionMessageCenter = session.getMessageManagerCenter();
        MerchantSubAccountResponseBean message = sessionMessageCenter.retrieveMerchantSubAccountMessage();

        requestStatisticsBean.setReceivedTime(System.currentTimeMillis());
        if (message != null) {
            Statistics.putStaticsSource(RequestStatisticsBean.class, requestStatisticsBean, new Object[0]);
            MerchantSubAccountInfo info = new MerchantSubAccountInfo();
            info.setId(message.getId());
            info.setTruename(message.getTruename());
            info.setUsername(message.getUsername());
            info.setUsersign(message.getUsersign());
            info.setHeadImgUrl(message.getHeadImgUrl());
            info.setQueueLength(message.getQueueLength());

            return info;
        } else {
            requestStatisticsBean.setTimedOut(true);
            Statistics.putStaticsSource(RequestStatisticsBean.class, requestStatisticsBean, new Object[0]);
            throw new ImserverException("获取卖家子账号出错, 服务器响应超时: " + JSONUtil.toJsonString(merchantSubAccountRequestBean));
        }
    }

    @Deprecated
    public List<RecentlySeller> getRecentlyContactsForBuyer(GetRecentlyContactWebRequest getRecentlyContactWebRequest) {
        RequestStatisticsBean requestStatisticsBean = new RequestStatisticsBean();
        Session session = getRecentlyContactWebRequest.getSession();
        SessionMessageCenter sessionMessageCenter = session.getMessageManagerCenter();

        RecentylContactsForBuyerRequestBean recentylContactsForBuyerRequestBean = RecentylContactsForBuyerRequestBean.getMessage();
        recentylContactsForBuyerRequestBean.setSid(getRecentlyContactWebRequest.getSenderID());
        recentylContactsForBuyerRequestBean.setId(getRecentlyContactWebRequest.getSenderID());
        recentylContactsForBuyerRequestBean.setDid(getRecentlyContactWebRequest.getSenderID());
        recentylContactsForBuyerRequestBean.setMsgnum(getRecentlyContactWebRequest.getMsgNum());
        recentylContactsForBuyerRequestBean.setSeq(seqGenerator.getNextSeq());
        requestStatisticsBean.setCmd(recentylContactsForBuyerRequestBean.getCmd());

        sendRequest(recentylContactsForBuyerRequestBean);

        RecentlyContactsForBuyerResponseBean message = sessionMessageCenter.retrieveRecentlySellerListMessage();

        requestStatisticsBean.setReceivedTime(System.currentTimeMillis());
        if (message == null) {
            requestStatisticsBean.setTimedOut(true);
            Statistics.putStaticsSource(RequestStatisticsBean.class, requestStatisticsBean, new Object[0]);
            throw new ImserverException("获取最近联系人失败, 服务器响应超时" + JSONUtil.toJsonString(recentylContactsForBuyerRequestBean));
        } else {
            Statistics.putStaticsSource(RequestStatisticsBean.class, requestStatisticsBean, new Object[0]);
            if (message.getList() != null && message.getList().size() > 0) {
                Collections.sort(message.getList(), Collections.reverseOrder());
            }

            return message.getList();
        }
    }

    public List<MerchantOnlineInfo> getMerchantOnlineInfo(MerchantOnlineInfoWebRequest merchantOnlineInfoWebRequest) {
        if (CollectionUtils.isEmpty(merchantOnlineInfoWebRequest.getMerchantIDList())) {
            throw new WrongRequestParamException("输入参数不合法");
        }

        List<MerchantOnlineInfo> merchantOnlineInfoList;
        try {
            merchantOnlineInfoList = IM_API_SERVICE.getMerchantOnlineInfo(merchantOnlineInfoWebRequest.getMerchantIDList());
        } catch (Exception e) {
            LOGGER.error("获取商家在线情况发生错误", e);

            merchantOnlineInfoList = new ArrayList<>(merchantOnlineInfoWebRequest.getMerchantIDList().size());
            for (Integer merchantID : merchantOnlineInfoWebRequest.getMerchantIDList()) {
                MerchantOnlineInfo merchantOnlineInfo = new MerchantOnlineInfo();
                merchantOnlineInfo.setId(merchantID);
                merchantOnlineInfo.setOnline(true);

                merchantOnlineInfoList.add(merchantOnlineInfo);
            }
        }

        return merchantOnlineInfoList;
    }

    public List<MerchantOnlineInfo> getSubMerchantOnlineInfo(SubMerchantOnlineInfoWebRequest subMerchantOnlineInfoWebRequest) {
        if (CollectionUtils.isEmpty(subMerchantOnlineInfoWebRequest.getSubMerchantIDList())) {
            throw new WrongRequestParamException("输入参数不合法");
        }

        List<MerchantOnlineInfo> merchantOnlineInfoList;
        try {
            merchantOnlineInfoList = IM_SERVICE.getSubMerchantOnlineInfo(subMerchantOnlineInfoWebRequest.getSubMerchantIDList());
        } catch (Exception e) {
            LOGGER.error("获取商家子账号在线情况发生错误", e);

            merchantOnlineInfoList = new ArrayList<>(subMerchantOnlineInfoWebRequest.getSubMerchantIDList().size());
            for (Long merchantID : subMerchantOnlineInfoWebRequest.getSubMerchantIDList()) {
                MerchantOnlineInfo merchantOnlineInfo = new MerchantOnlineInfo();
                merchantOnlineInfo.setId(merchantID);
                merchantOnlineInfo.setOnline(true);

                merchantOnlineInfoList.add(merchantOnlineInfo);
            }
        }

        return merchantOnlineInfoList;
    }

    @SuppressWarnings(value = "unchecked")
    public List<MerchantOnlineInfo> getServiceGroupOnlineInfo(GroupOnlineInfoWebRequest groupOnlineInfoWebRequest) {
        if (CollectionUtils.isEmpty(groupOnlineInfoWebRequest.getGroupIDList())) {
            return Collections.EMPTY_LIST;
        }

        Map<Long, Set<Long>> groupToSubAccountList = KEFU_SERVICE.getSubAccountListUnderGroupList(groupOnlineInfoWebRequest.getGroupIDList());
        List<Long> subMerchantIDList = new ArrayList<>();
        Map<Long, Long> subAccountToGroup = new HashMap<>();

        if (MapUtils.isNotEmpty(groupToSubAccountList)) {
            for (Map.Entry<Long, Set<Long>> entry : groupToSubAccountList.entrySet()) {
                for (Long subAccountID : entry.getValue()) {
                    subAccountToGroup.put(subAccountID, entry.getKey());
                }
                subMerchantIDList.addAll(entry.getValue());
            }
        } else {
            return Collections.EMPTY_LIST;
        }

        if (CollectionUtils.isEmpty(subMerchantIDList)) {
            throw new WrongRequestParamException("输入参数不合法");
        }

        List<MerchantOnlineInfo> subMerchantOnlineInfoList =
                IM_SERVICE.getSubMerchantOnlineInfo(subMerchantIDList);

        Map<Long, MerchantOnlineInfo> serviceGroupOnlineInfoMap = new HashMap<>();

        for (Long groupID : groupOnlineInfoWebRequest.getGroupIDList()) {
            MerchantOnlineInfo groupOnlineInfo = new MerchantOnlineInfo();
            groupOnlineInfo.setId(groupID);
            serviceGroupOnlineInfoMap.put(groupID, groupOnlineInfo);
        }
        if (CollectionUtils.isNotEmpty(subMerchantOnlineInfoList)) {
            for (MerchantOnlineInfo groupOnlineInfo : subMerchantOnlineInfoList) {
                Long groupID = subAccountToGroup.get(groupOnlineInfo.getId());
                MerchantOnlineInfo serviceGroupOnlineInfo;

                if (groupID != null && (serviceGroupOnlineInfo = serviceGroupOnlineInfoMap.get(groupID)) != null) {
                    if (!serviceGroupOnlineInfo.isOnline()) {
                        serviceGroupOnlineInfo.setOnline(groupOnlineInfo.isOnline());
                    }
                }
            }
        }

        return subMerchantOnlineInfoList;
    }

    public void putMsg(PutMessageRequestBean putMessageRequestBean) {
        putMessageRequestBean.setSeq(seqGenerator.getNextSeq());
        sendRequest(putMessageRequestBean);
    }

    public void retrieveOfflineMessage(RetrieveOfflineMessageRequest request) {
        sendRequest(request);
    }

    /**
     * 将保持长时间的连接
     */
    public List<BaseResponseBean> retrieveMessageList(GetMessageWebRequest getMessageWebRequest) {
        Session session = getMessageWebRequest.getSession();
        SessionMessageCenter sessionMessageCenter = session.getMessageManagerCenter();

        ChatWindowHandle chatWindowHandle = new ChatWindowHandle();
        chatWindowHandle.setAuthToken(getMessageWebRequest.getToken());
        chatWindowHandle.setUuid(getMessageWebRequest.getUuid());

        return sessionMessageCenter.retrieveAllChatMessage(chatWindowHandle);
    }

    /**
     * 取历史记录信息
     */
    public List<HistoryMessage> retrieveHistoryMessageList(GetHistoryMessageListRequest getHistoryMessageListRequest) {
        return IM_API_SERVICE.getHistoryMessageList(getHistoryMessageListRequest);
    }

    /**
     * 获取最近联系店铺
     */
    public List<RecentlyMobileSeller> getRecentMerchantInfo(GetRecentMerchantInfoRequest getRecentMerchantInfoRequest) {
        return IM_API_SERVICE.getRecentMerchantInfo(getRecentMerchantInfoRequest);
    }


    /**
     * 发送请求给业务服务器，可能因为同一时间段竞争过于激烈，导致放入发送消息队列失败，
     * 此时，应该返回给客户发送超时提示
     *
     * @throws com.feiniu.webim.exception.SendTimeOutException
     */
    private void sendRequest(BaseRequestBean baseRequestBean) {
        if (!CommunicationManager.instance.sendMessage(baseRequestBean)) {
            throw new SendTimeOutException("发送消息时出现超时，请重试");
        }
    }

}
