package com.feiniu.webim.controller;

import com.feiniu.webim.bean.MessageType;
import com.feiniu.webim.bean.domain.*;
import com.feiniu.webim.bean.helper.RecentlyMobileSellerComparator;
import com.feiniu.webim.bean.request.*;
import com.feiniu.webim.bean.response.BaseResponseBean;
import com.feiniu.webim.bean.response.ChatResponseBean;
import com.feiniu.webim.bean.response.HistoryMessageListResponseBean;
import com.feiniu.webim.bean.web.request.*;
import com.feiniu.webim.bean.web.response.MerchantUnreadMsgListResponse;
import com.feiniu.webim.communication.CommunicationManager;
import com.feiniu.webim.component.*;
import com.feiniu.webim.config.DiverseType;
import com.feiniu.webim.config.EventType;
import com.feiniu.webim.config.RequestParams;
import com.feiniu.webim.constants.Constants;
import com.feiniu.webim.constants.ErrorCode;
import com.feiniu.webim.constants.MerchantDivserErrorState;
import com.feiniu.webim.core.*;
import com.feiniu.webim.exception.*;
import com.feiniu.webim.servlet.HttpAttributeKeys;
import com.feiniu.webim.util.general.StringUtil;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.web.ServletRequestUtil;
import com.feiniu.webim.util.web.WebUtil;
import org.apache.commons.collections4.CollectionUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.commons.lang.math.NumberUtils;
import org.apache.log4j.Logger;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import java.util.*;

import static com.feiniu.webim.component.ConvertService.ConvertRule;

public class ChatController {
    private final ChatService chatService;
    private final MerchantService merchantService = MerchantService.instance;
    private final StoreService storeService = StoreService.instance;
    private final UserService userService = UserService.instance;
    private final AppMemberService appMemberService = AppMemberService.instance;
    private final ImApiService imApiService = ImApiService.INSTANCE;
    private final RiskEliminateService RISK_DETECT_SERVICE = RiskEliminateService.INSTANCE;
    private final ConvertService CONVERT_SERVICE = ConvertService.CONVERT_SERVICE;
    private final EndecryptedManager ENDECRYPTED_MANAGER = EndecryptedManager.INSTANCE;

    //当保存消息时，使用的相关规则
    private static final List<ConvertRule> CONVERT_MSG_RULES_WHEN_SAVE =
            Arrays.asList(ConvertRule.MAKE_CS_CUSTOMER_EMOJI_RELATIVE, ConvertRule.MAKE_PHOTO_URL_RELATIVE);
    //当返回消息给端时，使用的相关规则
    private static final List<ConvertRule> CONVERT_MSG_RULES_WHEN_GIVE = Arrays.asList(ConvertRule.MAKE_PHOTO_URL_ABSOLUTE,
            ConvertRule.TRANSFORM_OLD_MERCHANT_PHOTO_FROM_IM);
    //与头像相关的转换
    private static final List<ConvertRule> CONVERT_HEAD_IMG_RELATIVE = Arrays.asList(ConvertRule.MEKE_HEAD_IMG_ABSOLUTE,
            ConvertRule.TRANSFORM_OLD_MERCHANT_PHOTO_FROM_EXTERNAL);

    private ChatController() {
        chatService = ChatService.getInstance();
    }

    private static final ChatController CHAT_CONTROLLER = new ChatController();

    public static ChatController getInstance() {
        return CHAT_CONTROLLER;
    }

    private static final Logger LOGGER = Logger.getLogger(ChatController.class);

    public void handleRequest(WebAction webAction) {
        try {
            webAction.startAction();
            doRequest(webAction);
            webAction.printResult();
        } catch (RuntimeException e) {
            Session session = webAction.getSession();
            /**对于没初始化成功[sessionID未正确设置]的session，移除，等待gc**/
            if (session != null) {
                if (!session.getId().replaceAll("\\d", "").equals("")) {
                    SessionManager.getInstance().removeSession(session);
                }
            }

            ResponseContentGenerator generator = webAction.getResponseContentGenerator();

            if (e.getClass().equals(RuntimeException.class)) {
                generator.printServerError(null);
            } else {
                ResultInfo resultInfo = new ResultInfo();
                resultInfo.setErrorMessage("服务器发生故障，请稍后再试～");
                resultInfo.setErrorCode(ErrorCode.ERRORCODE_CHAT_WEBIM_FAIL);

                if (e instanceof SendTimeOutException) {
                    resultInfo.setErrorMessage("发送消息出现超时，请刷新页面重试");
                    resultInfo.setErrorCode(ErrorCode.ERRORCODE_SEND_TIME_OUT);
                } else if (e instanceof ChatSessionNotExistException) {
                    resultInfo.setErrorMessage("您与卖家的会话已断开，请刷新页面重试");
                    resultInfo.setErrorCode(ErrorCode.ERRORCODE_CHAT_SESSION_NOT_EXIST);
                } else if (e instanceof LoginCheckException) {
                    resultInfo.setErrorMessage("您的帐号未登录，请重新登录～");
                    resultInfo.setNeedLogin(true);
                    resultInfo.setErrorCode(ErrorCode.ERRORCODE_CHECK_LOGIN_FAIL);
                } else if (e instanceof ImserverException) {
                    resultInfo.setErrorMessage("服务器响应超时，请稍后再试～");
                    resultInfo.setErrorCode(ErrorCode.ERRORCODE_SERVER_FAIL);
                } else if (e instanceof GetStoreInfoException) {
                    resultInfo.setErrorMessage("服务器响应超时，请稍后再试～");
                    resultInfo.setErrorCode(ErrorCode.ERRORCODE_GET_STORE_INFO_FAIL);
                } else if (e instanceof ClientIllegalArgumentException) {
                    resultInfo.setErrorMessage(e.getMessage());
                    resultInfo.setErrorCode(ErrorCode.CLIENT_ILLEGAL_ARGUMENT_FAIL);
                }

                generator.writeObject(resultInfo);
            }

            LOGGER.error("处理请求出错：", e);
        } finally {
            webAction.finishAction();
        }
    }

    /**
     * 根据event,处理请求
     */
    public void doRequest(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        String event = request.getParameter(RequestParams.EVENT.toString());
        EventType eventType = EventType.getEventType(event);

        switch (eventType) {
            case INIT_CHAT_ENV:
                doInitChatEnvironment(webAction);
                webAction.setNeedRefreshSession(true);
                break;
            case INIT_EMBED_ENV:
                doInitEmebedEnvironment(webAction);
                webAction.setNeedRefreshSession(true);
                break;
            case GET_RECENTYLE_CONTACTS_FOR_BUYER:
                doGetRecentlyContactList(webAction);
                webAction.setNeedRefreshSession(true);
                break;
            case GET_RECENTYLE_MOBILE_CONTACTS_FOR_BUYER:
                doGetRecentlyMobileContactList(webAction);
                webAction.setNeedRefreshSession(true);
                break;
            case GET_MERCHANT_ONLINE_INFO:
                doGetMerchantOnlineInfo(webAction);
                break;
            case GET_SUB_MERCHANT_ONLINE_INFO:
                doGetSubMerchantOnlineInfo(webAction);
                break;
            case GET_SERVICE_GROUP_ONLINE_INFO:
                doGetServiceGroupOnlineInfo(webAction);
                break;
            case GET_CHAT_MESSAGE_LIST:
                doGetList(webAction);
                break;
            case GET_HISTORY_MESSAGE_LIST:
                doGetHistoryList(webAction);
                break;
            case PUT_CHAT_MESSAGE:
                doPutMsg(webAction);
                webAction.setNeedRefreshSession(true);
                break;
            case PULL_QUEUE_INFO:
                doGetQueueLength(webAction);
                webAction.setNeedRefreshSession(true);
                break;
            case GET_CHAT_UNREAD_COUNT_LIST:
                doGetUnreadCountList(webAction);
                webAction.setNeedRefreshSession(true);
                break;
            case EXIT:
                doExit(webAction);
                webAction.setNeedRefreshToken(false);
                break;
            default:
                ResultInfo resultInfo = new ResultInfo();
                resultInfo.setErrorMessage("未知的事件类型：" + event);
                webAction.setResultInfo(resultInfo);
        }
    }

    /**
     * 初始化聊天环境
     */
    protected void doInitChatEnvironment(WebAction webAction) {
        // Step1 --> 登录
        doLogin(webAction);

        //Step2 --> 获取个人信息
        doGetUserInfo(webAction);

        //Step3 --> 分流
        MerchantSubAccountInfo merchantSubAccount = doGetMerchantSubAccount(webAction);

        ResultInfo resultInfo = new ResultInfo();
        Session session = webAction.getSession();
        UserInfo userInfo = session.getUserInfo();
        if (StringUtils.isNotEmpty(userInfo.getNickname())) {
            userInfo.setNickname(StringUtil.makeTelephoneSecure(userInfo.getNickname()));
        }

        int userID = Integer.parseInt(webAction.getSession().getId());

        resultInfo.addData("userID", userID);
        resultInfo.addData("userName", userInfo.getUsername());
        resultInfo.addData("nickName", userInfo.getNickname());
        resultInfo.addData("uuid", webAction.getRequest().getAttribute(HttpAttributeKeys.ATTRIBUTE_UUID));
        resultInfo.addData("imgUrl", userInfo.getFace_url());
        resultInfo.addData("maxNonSpeakTolerantTime", Constants.MAX_NON_SPEAK_TOLERANT_TIME);
        resultInfo.addData("authTokenPair", session.getGlobalToken());

        //对于来自自营客服系统买家端的请求，特殊处理
        boolean isFromCS = (boolean) (webAction.getRequest().getAttribute(HttpAttributeKeys.IS_FROM_CS));
        boolean isDiverseFailed = merchantSubAccount.getQueueLength() != 0 || merchantSubAccount.getId() <= 0;

        if (isDiverseFailed) {
            if (merchantSubAccount.getQueueLength() == -1) {
                resultInfo.setErrorMessage(Constants.TIP_FOR_NO_SUBACCOUNT_ONLINE);
            }
            //如果商家ID为0返回结果success为true
            if (merchantSubAccount.getId() == 0) {
                resultInfo.markSuccess();
            }
            if (isFromCS) {
                resultInfo.addData("needWait", true);
                resultInfo.addData("queuedSeq", merchantSubAccount.getQueueLength());
                resultInfo.markSuccess();
            }
        } else {
            if (isFromCS) {
                resultInfo.addData("needWait", false);
                resultInfo.addData("queuedSeq", merchantSubAccount.getQueueLength());
            }

            //Step4 --> 给客户端发送初始化消息
            sendInitWindowMessage(webAction, merchantSubAccount);
            //Step5 --> 初始化会话内部参数
            initSessionMappingInfo(webAction, merchantSubAccount);
            //Step6 --> 获取离线消息
            sendRetrieveOfflineMessages(userID);

            resultInfo.addData("subAccount", merchantSubAccount);
            resultInfo.markSuccess();
        }

        webAction.setResultInfo(resultInfo);
    }

    //获取离线消息
    private void sendRetrieveOfflineMessages(int userID) {
        RetrieveOfflineMessageRequest request = new RetrieveOfflineMessageRequest();
        request.setSid(userID);

        chatService.retrieveOfflineMessage(request);
    }

    /**
     * 给商家客户端发送一条消息，让他知道他被分流到当前客户了
     */
    private void sendInitWindowMessage(WebAction webAction, MerchantSubAccountInfo merchantSubAccount) {
        HttpServletRequest request = webAction.getRequest();
        Map<String, Object> initMessage = new HashMap<>(2);
        Session session = webAction.getSession();
        String orderSeq = ServletRequestUtil.getString(request, "orderSeq", "");

        initMessage.put("productID", ServletRequestUtil.getString(request, "productID", ""));
        initMessage.put("orderSeq", orderSeq.length() > 0 ? ENDECRYPTED_MANAGER.encrypted(orderSeq) : orderSeq);

        PutMessageRequestBean putMessageRequestBean = new PutMessageRequestBean();
        putMessageRequestBean.setSid(Integer.parseInt(webAction.getSession().getId()));
        putMessageRequestBean.setDid(merchantSubAccount.getId());
        putMessageRequestBean.setSender(putMessageRequestBean.getSid());
        putMessageRequestBean.setRecver(putMessageRequestBean.getDid());
        putMessageRequestBean.setType(MessageType.INIT_CHAT_ENV_MESSAGE_TYPE.getType());
        putMessageRequestBean.setMsg(JSONUtil.toJsonString(initMessage));
        putMessageRequestBean.setTime(System.currentTimeMillis());
        putMessageRequestBean.setFeatures(new MessageFeatures());
        putMessageRequestBean.setNickname(webAction.getSession().getUserInfo().getNickname());
        putMessageRequestBean.setVersion(-1);

        chatService.putMsg(putMessageRequestBean);
    }

    private void initSessionMappingInfo(WebAction webAction, MerchantSubAccountInfo merchantSubAccount) {
        HttpServletRequest request = webAction.getRequest();
        Session session = webAction.getSession();
        SessionMessageCenter sessionMessageCenter = session.getMessageManagerCenter();
        String token = TokenManager.getToken(request);
        String uuid = request.getAttribute(HttpAttributeKeys.ATTRIBUTE_UUID).toString();
        boolean isFromCS = (Boolean) request.getAttribute(HttpAttributeKeys.IS_FROM_CS);

        //给之前的窗口 投放 准备退出消息
        ChatResponseBean chatResponseBean = ChatResponseBean.getFakeResponseBean();
        chatResponseBean.setType(MessageType.PREPARE_TO_CLOSE.getType());
        chatResponseBean.setToken(token);
        chatResponseBean.setMsg(uuid);
        chatResponseBean.setUuid(uuid);
        chatResponseBean.setFromCS(isFromCS);
        session.postMessage(chatResponseBean);

        RecentlySeller recentlySeller = new RecentlySeller();
        recentlySeller.setId(merchantSubAccount.getId());
        recentlySeller.setNickname(merchantSubAccount.getUsername());
        recentlySeller.setUsersign(merchantSubAccount.getUsersign());
        recentlySeller.setState(merchantSubAccount.isOnline() ? 0 : 1);
        recentlySeller.setTime((int) (System.currentTimeMillis() / Constants.MSG_TIME_TRANSFORM));
        recentlySeller.setTag(RecentlySeller.IS_CURRENT_CHATTING_SELLER);
        recentlySeller.setSkuID(ServletRequestUtil.getLong(request, "productID", 0l));
        recentlySeller.setHeadImgUrl(merchantSubAccount.getHeadImgUrl());

        session.addSubMerchantListInChatting(recentlySeller);

        //初始化后清空未读消息数
        GetSubMerchantWebRequest getSubMerchantWebRequest = ServletRequestUtil.getObject(request, GetSubMerchantWebRequest.class);
        Integer merchantId = getSubMerchantWebRequest.getPid();
        sessionMessageCenter.removeUnreadMap(TokenManager.getToken(request), merchantId);
    }

    /**
     * 初始化布点时环境
     */
    @SuppressWarnings({"unchecked", "rawtypes"})
    protected void doInitEmebedEnvironment(WebAction webAction) {
        ResultInfo resultInfo = new ResultInfo();

        doLogin(webAction);
        doGetUserInfo(webAction);

        HttpServletRequest request = webAction.getRequest();
        Session session = webAction.getSession();

        List<RecentlySeller> recentlySellerList = chatService.getRecentlyContactsForBuyer(parseRecentlyContactRequest(request, session));
        List<Integer> subMerchantIDList = new ArrayList<>();
        for (RecentlySeller seller : recentlySellerList) {
            subMerchantIDList.add(seller.getId());
        }

        Map<Integer, StoreInfo> storeInfoMap = new HashMap<>();
        List<Map> recentlySellerInfo = new ArrayList<>();
        Map<String, Integer> merchantAccountMap = new HashMap<>();

        try {
            if (CollectionUtils.isNotEmpty(subMerchantIDList)) {
                merchantAccountMap = merchantService.retriveMerchantIDsByMemberIDs(subMerchantIDList);
                List<StoreInfo> storeInfoList = storeService.batchRetriveStoreInfo(merchantAccountMap.values());
                for (StoreInfo storeInfo : storeInfoList) {
                    storeInfo.setStoreLogoUrl(CONVERT_SERVICE.doConvert(storeInfo.getStoreLogoUrl(), CONVERT_HEAD_IMG_RELATIVE));
                    storeInfoMap.put(storeInfo.getMerchantId(), storeInfo);
                }
            }
        } catch (RuntimeException e) {
            LOGGER.error("获取商家相关信息失败", e);
        }

        for (RecentlySeller seller : recentlySellerList) {
            Map contactInfo = new HashMap();
            if (merchantAccountMap.containsKey(seller.getId() + "")) {
                StoreInfo storeInfo = storeInfoMap.get(merchantAccountMap.get(seller.getId() + ""));
                if (storeInfo != null) {
                    contactInfo.putAll(JSONUtil.toMap(storeInfo));
                }
            }

            contactInfo.putAll(JSONUtil.toMap(seller));
            recentlySellerInfo.add(contactInfo);
        }

        resultInfo.addData("sid", webAction.getSession().getId());
        resultInfo.addData("contactList", recentlySellerInfo);

        resultInfo.markSuccess();

        webAction.setResultInfo(resultInfo);
    }

    /**
     * 这个地方开放public，仅仅是为了给常驻dummy-session用来给未登录的账号刷新商户在线客服状态，
     * 平时，只能给初始化聊天环境和初始化布点环境时使用
     */
    protected void doLogin(WebAction webAction) {
        Session session;
        SessionManager manager = SessionManager.getInstance();
        LoginWebRequest loginWebRequest = ServletRequestUtil.getObject(webAction.getRequest(), LoginWebRequest.class);
        String resolveGUID;
        UserCheckInInfo userCheckInInfo;
        HttpServletRequest request = webAction.getRequest();

        if (TokenManager.isMobile(request)) {/**如果是无线请求**/
            userCheckInInfo = appMemberService.getUserInfo(TokenManager.getToken(request));
            resolveGUID = userCheckInInfo.getGuid();
        } else {/**如果是pc请求**/
            Cookie[] cookies = webAction.getRequest().getCookies();

            if (!UserService.checkSessionCookieLegality(cookies)) {
                throw new LoginCheckException("cookie项缺失");
            }

            userCheckInInfo = userService.checkUserInfoFromMember(cookies);
            resolveGUID = userCheckInInfo.getGuid();
        }

        loginWebRequest.setGuid(resolveGUID);
        loginWebRequest.setTelephone(userCheckInInfo.getTelephone());
        loginWebRequest.setEmail(userCheckInInfo.getEmail());
        loginWebRequest.setSname(userCheckInInfo.getUserName());

        Session testSession = manager.getSessionByName(loginWebRequest.getSname());
        if (testSession != null) {
            session = testSession;
            testSession.updateTime();
        } else {
            session = new Session(WebUtil.uuid());
            session.setUserName(session.getId());
            SessionManager.getInstance().addSession(session);
        }

        loginWebRequest.setSession(session);

        String sid;
        if (session.isLogin()) {
            sid = session.getId();
        } else {
            sid = chatService.login(loginWebRequest) + "";
            manager.removeSession(session);
        }

        if (!NumberUtils.isDigits(session.getId())) {/**该session未登录**/
            session.setId(sid);
            session.setUserName(loginWebRequest.getSname());
            session = manager.addSession(session);
            session.setLogin(true);
        }
        webAction.setSession(session);

        request = webAction.getRequest();
        String uuid = WebUtil.uuid();
        boolean isFromCS = ServletRequestUtil.getInteger(request, "diverse_type", 0) == DiverseType.FROM_CS_DIVERSE_AMONG_GROUP.getType();
        boolean isMobile = TokenManager.isMobile(webAction.getRequest());
        String authToken = TokenManager.getToken(request);

        //如果是来自pc自营客服，使用uuid加密串作为token
        if (isFromCS && !isMobile) {
            authToken = ENDECRYPTED_MANAGER.encryptedWithURLEncode(uuid);
            request.setAttribute(HttpAttributeKeys.WEBIM_AUTH_TOKEN_NAME, authToken);
        }

        request.setAttribute(HttpAttributeKeys.ATTRIBUTE_UUID, uuid);
        request.setAttribute(HttpAttributeKeys.ATTRIBUTE_GUID, resolveGUID);
        request.setAttribute(HttpAttributeKeys.IS_FROM_CS, isFromCS);

        ChatWindowHandle chatWindowHandle = new ChatWindowHandle();
        chatWindowHandle.setAuthToken(authToken);
        chatWindowHandle.setUuid(uuid);
        chatWindowHandle.setIsFromCS(isFromCS);
        chatWindowHandle.setLastUpdateTime(WebUtil.now());
        //聊天窗口设置正在聊天的商家
        chatWindowHandle.setFromMobile(isMobile);
        //如果来自自营客服，记录组信息
        if (chatWindowHandle.isFromCS()) {
            String groupID = ServletRequestUtil.getString(request, RequestParams.GROUP_ID.toString());
            if (StringUtils.isEmpty(groupID)) {
                groupID = "0";
            }

            chatWindowHandle.setGroupID(Integer.parseInt(groupID));
            chatWindowHandle.setQueueLength(Integer.MAX_VALUE);
        }

        Integer pid = ServletRequestUtil.getInteger(request, "pid", 0);
        chatWindowHandle.setMerchantID(pid);

        session.addChatLink(chatWindowHandle);
    }

    private void doGetUserInfo(WebAction webAction) {
        HttpServletRequest httpServletRequest = webAction.getRequest();
        Session session = webAction.getSession();
        boolean isFromCS = (boolean) (webAction.getRequest().getAttribute(HttpAttributeKeys.IS_FROM_CS));
        String guid = httpServletRequest.getAttribute(HttpAttributeKeys.ATTRIBUTE_GUID).toString();

        if (session.getUserInfo() == null) {
            UserInfo userInfo = userService.getUserInfo(guid);
            if (isFromCS) {
                userInfo.setMemLevel(userService.getUserLevel(guid));
            }

            session.setUserInfo(userInfo);
            session.generateGlobalToken();
        } else {
            UserInfo userInfo = userService.getUserInfo(guid);
            if (isFromCS && userInfo.getMemLevel() == null) {
                userInfo.setMemLevel(userService.getUserLevel(guid));
                session.setUserInfo(userInfo);
            }
        }
    }

    protected MerchantSubAccountInfo doGetMerchantSubAccount(WebAction webAction) {
        Session session = webAction.getSession();
        HttpServletRequest request = webAction.getRequest();
        int sid = Integer.parseInt(session.getId());

        /**分流客户 */
        GetSubMerchantWebRequest getSubMerchantWebRequest = ServletRequestUtil.getObject(request, GetSubMerchantWebRequest.class);
        getSubMerchantWebRequest.setSid(sid);
        getSubMerchantWebRequest.setSession(session);
        getSubMerchantWebRequest.setUserLevel(session.getUserInfo().getMemLevel());
        getSubMerchantWebRequest.setFromDeviceType(TokenManager.isMobile(request) ? 1 : 0);
        getSubMerchantWebRequest.setFromPageType(ServletRequestUtil.getInteger(request, "fromPageType", -1));

        //如果请求是商城全局分流且merchantid非法，不向服务器请求分流
        if (getSubMerchantWebRequest.getPid() <= 0 && getSubMerchantWebRequest.getDiverse_type() < 1) {
            MerchantSubAccountInfo merchantSubAccountInfo = new MerchantSubAccountInfo();
            merchantSubAccountInfo.setId(0);
            merchantSubAccountInfo.setQueueLength(0);

            return merchantSubAccountInfo;
        }

        MerchantSubAccountInfo merchantSubAccount = imApiService.getSubAccount(getSubMerchantWebRequest);

        /**获取分流子账号的 在线信息，因为可能是通过最近联系人进来的[这种情况在分流时获取不了在线状态]*/
        if (merchantSubAccount.getId() > 0) {
            //设置当前正在聊天的客服
            session.setCurrentSubAccountIdMap(TokenManager.getToken(request), merchantSubAccount.getId());
        }

        String uuid = request.getAttribute(HttpAttributeKeys.ATTRIBUTE_UUID).toString();
        int lastQueueLength = session.getLastQueueLength(uuid);

        if (lastQueueLength > 0) {
            if (lastQueueLength > merchantSubAccount.getQueueLength()) {
                session.setLastQueueLength(uuid, merchantSubAccount.getQueueLength());
            } else {
                merchantSubAccount.setQueueLength(lastQueueLength);
            }
        }

        //如果当前客户的接待状态为正在被接待，删掉当前窗口句柄
        if (merchantSubAccount.getQueueLength() == MerchantDivserErrorState.SERVICE_ON_GOING.getState()) {
            session.removeChatLink(uuid);
            webAction.setNeedRefreshToken(false);
        }

        return merchantSubAccount;
    }

    protected void doGetMerchantOnlineInfo(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        Session session = webAction.getSession();

        ResultInfo resultInfo = new ResultInfo();
        List<MerchantOnlineInfo> onlineInfo = chatService.getMerchantOnlineInfo(parseMerchantOnlineRequest(request, session));

        resultInfo.addData("merchantOnlineInfo", onlineInfo);
        resultInfo.markSuccess();

        webAction.setResultInfo(resultInfo);
    }

    protected void doGetSubMerchantOnlineInfo(WebAction webAction) {
        ResultInfo resultInfo = new ResultInfo();

        String merchantIDs = ServletRequestUtil.getString(webAction.getRequest(), RequestParams.SUB_MERCHANT_IDS.toString());
        List<Long> subMerchantIDList = new ArrayList<>();
        for (String subMerchantID : merchantIDs.split(",")) {
            long currentSubMerchantID = Long.parseLong(subMerchantID);
            if (currentSubMerchantID > 0l) {
                subMerchantIDList.add(currentSubMerchantID);
            }
        }

        SubMerchantOnlineInfoWebRequest subMerchantOnlineInfoWebRequest = new SubMerchantOnlineInfoWebRequest(webAction.getSession());
        subMerchantOnlineInfoWebRequest.setSubMerchantIDList(subMerchantIDList);

        List<MerchantOnlineInfo> onlineInfo = chatService.getSubMerchantOnlineInfo(subMerchantOnlineInfoWebRequest);

        resultInfo.addData("subMerchantOnlineInfo", onlineInfo);
        resultInfo.markSuccess();

        webAction.setResultInfo(resultInfo);
    }

    protected void doGetServiceGroupOnlineInfo(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        Session session = webAction.getSession();
        int id = Integer.parseInt(webAction.getSession().getId());
        String groupIDs = ServletRequestUtil.getString(request, RequestParams.GROUP_IDS.toString());
        List<Long> groupIDList = new ArrayList<>();

        for (String subGroupID : groupIDs.split(",")) {
            long currentGroupID = Long.parseLong(subGroupID);
            if (currentGroupID > 0l) {
                groupIDList.add(currentGroupID);
            }
        }

        GroupOnlineInfoWebRequest groupOnlineInfoWebRequest = new GroupOnlineInfoWebRequest(session);
        groupOnlineInfoWebRequest.setSenderID(id);
        groupOnlineInfoWebRequest.setGroupIDList(groupIDList);

        ResultInfo resultInfo = new ResultInfo();
        List<MerchantOnlineInfo> onlineInfo = chatService.getServiceGroupOnlineInfo(groupOnlineInfoWebRequest);

        resultInfo.addData("serviceGroupOnlineInfo", onlineInfo);
        resultInfo.markSuccess();

        webAction.setResultInfo(resultInfo);
    }

    @SuppressWarnings({"unchecked", "rawtypes"})
    protected void doGetRecentlyContactList(WebAction webAction) {
        ResultInfo resultInfo = new ResultInfo();
        Session session = webAction.getSession();
        HttpServletRequest request = webAction.getRequest();

        int id = Integer.parseInt(session.getId());
        int msgnum = ServletRequestUtil.getInteger(request, RequestParams.MSGNUM.toString(), Constants.NUMBER_OF_RECENTLY_SELLER_FETCH);
        GetRecentMerchantInfoRequest getRecentMerchantInfoRequest = new GetRecentMerchantInfoRequest();
        getRecentMerchantInfoRequest.setCustomerId(id);
        getRecentMerchantInfoRequest.setMaxNumber(msgnum);

        List<RecentlySeller> recentlySellerList = imApiService.getRecentSubAccountList(getRecentMerchantInfoRequest);

        //添加正在联系人[去重，正在联系人优先级更高]
        RecentlySeller[] sellerInChatting = webAction.getSession().getAllSubMerchantListInChatting();
        for (RecentlySeller recentlySeller : sellerInChatting) {
            if (Constants.isKefuFromCS(recentlySeller.getId())) {
                continue;
            }

            int index = recentlySellerList.indexOf(recentlySeller);
            if (index == -1) {
                recentlySellerList.add(recentlySeller);
            } else {
                RecentlySeller current = recentlySellerList.get(index);
                current.setTime(recentlySeller.getTime());
                current.setSkuID(recentlySeller.getSkuID());
                current.setTag(recentlySeller.getTag());
            }
        }

        List<Integer> subMerchantIDList = new ArrayList<>();
        for (RecentlySeller seller : recentlySellerList) {
            seller.setHeadImgUrl(CONVERT_SERVICE.doConvert(seller.getHeadImgUrl(), CONVERT_HEAD_IMG_RELATIVE));
            subMerchantIDList.add(seller.getId());
        }

        //merchantID-->店铺信息
        Map<Integer, StoreInfo> storeInfoMap = new HashMap<>();
        List<Map> recentlySellerInfo = new ArrayList<>();
        //子账号id对商家id
        Map<String, Integer> merchantAccountMap = new HashMap<>();
        //商家id对店铺名称
        Map<Integer, String> merchantIDToShopNameMap = Collections.emptyMap();

        try {
            if (CollectionUtils.isNotEmpty(subMerchantIDList)) {
                merchantAccountMap = merchantService.retriveMerchantIDsByMemberIDs(subMerchantIDList);
                merchantIDToShopNameMap = merchantService.retrieveStoreNameMap(merchantAccountMap.values());
                List<StoreInfo> storeInfoList = storeService.batchRetriveStoreInfo(merchantAccountMap.values());

                for (StoreInfo storeInfo : storeInfoList) {
                    storeInfo.setStoreName(merchantIDToShopNameMap.get(storeInfo.getMerchantId()));
                    storeInfo.setStoreLogoUrl(CONVERT_SERVICE.doConvert(storeInfo.getStoreLogoUrl(), CONVERT_HEAD_IMG_RELATIVE));
                    storeInfoMap.put(storeInfo.getMerchantId(), storeInfo);
                    merchantIDToShopNameMap.remove(storeInfo.getMerchantId());
                }
            }
        } catch (RuntimeException e) {
            LOGGER.error("获取商家相关信息失败", e);
        }

        for (RecentlySeller seller : recentlySellerList) {
            Map<String, Object> contactInfo = new HashMap<>();
            if (merchantAccountMap.containsKey(seller.getId() + "")) {
                Integer merchantID = merchantAccountMap.get(seller.getId() + "");

                StoreInfo storeInfo = storeInfoMap.get(merchantID);
                if (storeInfo != null) {
                    contactInfo.putAll(JSONUtil.toMap(storeInfo));
                } else {
                    contactInfo.put("storeName", merchantIDToShopNameMap.get(merchantID));
                    contactInfo.put("merchantId", merchantID);
                    contactInfo.put("storeDesc", "");
                }
            }

            contactInfo.putAll(JSONUtil.toMap(seller));
            recentlySellerInfo.add(contactInfo);
        }

        resultInfo.addData("contactList", recentlySellerInfo);
        resultInfo.markSuccess();

        webAction.setResultInfo(resultInfo);
    }

    /**
     * 添加消息
     */
    protected void doPutMsg(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        PutMessageRequestBean putMessageRequestBean = ServletRequestUtil.getObject(request, PutMessageRequestBean.class);
        putMessageRequestBean.setNickname(webAction.getSession().getUserInfo().getNickname());
        putMessageRequestBean.setFeatures(JSONUtil.fromJson(ServletRequestUtil.getString(request, "features"), MessageFeatures.class));
        putMessageRequestBean.setTime(System.currentTimeMillis() + Constants.TIME_DELTA_BETWEEN_WEBIM_AND_IM * Constants.MSG_TIME_TRANSFORM);
        putMessageRequestBean.setSid(putMessageRequestBean.getSender());
        putMessageRequestBean.setDid(putMessageRequestBean.getRecver());
        if (putMessageRequestBean.getFeatures() == null) {
            putMessageRequestBean.setFeatures(new MessageFeatures());
        }

        if (StringUtils.isEmpty(putMessageRequestBean.getMsg())) {
            putMessageRequestBean.setMsg("");
        } else if (putMessageRequestBean.getMsg().length() > Constants.MAX_MESSAGE_CONTENT_LENGTH) {
            putMessageRequestBean.setMsg(putMessageRequestBean.getMsg().substring(0, Constants.MAX_MESSAGE_CONTENT_LENGTH));
        }

        putMessageRequestBean.setMsg(RISK_DETECT_SERVICE.filterOutterLink(putMessageRequestBean.getMsg()));
        putMessageRequestBean.setMsg(CONVERT_SERVICE.doConvert(putMessageRequestBean.getMsg(), CONVERT_MSG_RULES_WHEN_SAVE));

        ResultInfo resultInfo = new ResultInfo();

        if (StringUtils.isNotEmpty(putMessageRequestBean.getMsg())) {
            chatService.putMsg(putMessageRequestBean);

            if (putMessageRequestBean.getType() == 0 && !Constants.isKefuFromCS(putMessageRequestBean.getRecver())) {/**如果是商家聊天消息才转发**/
                ChatResponseBean chatResponseBean = ChatResponseBean.getFakeResponseBean();
                chatResponseBean.setRecver(putMessageRequestBean.getRecver());
                chatResponseBean.setSender(putMessageRequestBean.getSender());
                chatResponseBean.setTime(putMessageRequestBean.getTime());
                chatResponseBean.setMsg(putMessageRequestBean.getMsg());
                chatResponseBean.setFeatures(putMessageRequestBean.getFeatures());
                chatResponseBean.setType(putMessageRequestBean.getType());
                chatResponseBean.setToken(TokenManager.getToken(request));
                chatResponseBean.setUuid(ServletRequestUtil.getString(request, "uuid"));

                Session session = webAction.getSession();
                //发送消息时根据窗口的uuid获取商家ID，并设置发送的商家ID，用于触屏版过滤不同商家的消息
                SessionMessageCenter sessionMessageCenter = session.getMessageManagerCenter();
                chatResponseBean.setMerchantId(sessionMessageCenter.getUuidToMerchantID(chatResponseBean.getUuid()));
                session.getMessageManagerCenter().postMessage(
                        JSONUtil.toJsonString(chatResponseBean).replaceFirst("(\"time\":\\d+)000,", "$1,"));
            }
        }

        resultInfo.addData("messageTime", putMessageRequestBean.getTime() * Constants.MSG_TIME_TRANSFORM);
        resultInfo.markSuccess();
        webAction.setResultInfo(resultInfo);
    }

    /**
     * 获取消息
     */
    protected void doGetList(WebAction webAction) {
        ResultInfo resultInfo = new ResultInfo();
        GetMessageWebRequest getMessageWebRequest = ServletRequestUtil.getObject(webAction.getRequest(), GetMessageWebRequest.class);
        getMessageWebRequest.setToken(TokenManager.getToken(webAction.getRequest()));
        getMessageWebRequest.setSession(webAction.getSession());

        List<BaseResponseBean> messageList = chatService.retrieveMessageList(getMessageWebRequest);
        if (CollectionUtils.isNotEmpty(messageList)) {
            for (BaseResponseBean baseResponseBean : messageList) {
                ChatResponseBean chatResponseBean = (ChatResponseBean) baseResponseBean;
                chatResponseBean.setMsg(RISK_DETECT_SERVICE.filterOutterLink(chatResponseBean.getMsg()));
                chatResponseBean.setMsg(CONVERT_SERVICE.doConvert(chatResponseBean.getMsg(), CONVERT_MSG_RULES_WHEN_GIVE));
            }
        }

        resultInfo.addData("messageList", messageList);

        //判断是否有未读消息
        boolean isHasUnreadMsg = false;
        Session session = webAction.getSession();
        SessionMessageCenter sessionMessageCenter = session.getMessageManagerCenter();
        String token = TokenManager.getToken(webAction.getRequest());
        try {
            Integer merchantID = sessionMessageCenter.getUuidToMerchantID(getMessageWebRequest.getUuid());
            if (merchantID != 0) {
                isHasUnreadMsg = sessionMessageCenter.getHasUnreadMsg(token, merchantID);
            }
        } catch (RuntimeException e) {
            //获取sessionMessageCenter为空的异常暂时忽略，isHasUnreadMsg仍设置为false
        }
        resultInfo.addData("isHasUnreadMsg", isHasUnreadMsg);

        resultInfo.markSuccess();
        webAction.setResultInfo(resultInfo);
    }

    /**
     * 获取手机端未读消息列表
     */
    protected void doGetUnreadCountList(WebAction webAction) {
        Session session = webAction.getSession();
        String token = TokenManager.getToken(webAction.getRequest());
        SessionMessageCenter sessionMessageCenter = session.getMessageManagerCenter();
        List<MerchantUnreadMsgListResponse> unreadMsgList = sessionMessageCenter.getUnreadMsgList(token);
        Map<Integer, MerchantUnreadMsgListResponse> unreadMsgListResponseMap = new HashMap<>(unreadMsgList.size());

        //获取店铺信息名称和店铺logo
        Collection<Integer> merchantIDs = new ArrayList<>();
        for (MerchantUnreadMsgListResponse tmp : unreadMsgList) {
            merchantIDs.add(tmp.getMerchantId());
            unreadMsgListResponseMap.put(tmp.getMerchantId(), tmp);
        }

        if (merchantIDs.size() > 0) {
            List<StoreInfo> listStoreInfo = storeService.batchRetriveStoreInfo(merchantIDs);
            Map<Integer, String> merchantIDToShopNameMap = merchantService.retrieveStoreNameMap(merchantIDs);

            for (StoreInfo storeInfo : listStoreInfo) {
                MerchantUnreadMsgListResponse response = unreadMsgListResponseMap.get(storeInfo.getMerchantId());

                if (response != null) {
                    response.setMerchantLogoUrl(storeInfo.getStoreLogoUrl());
                    response.setMerchantName(storeInfo.getStoreName());
                    unreadMsgListResponseMap.remove(storeInfo.getMerchantId());
                }
            }

            for (Map.Entry<Integer, MerchantUnreadMsgListResponse> entry : unreadMsgListResponseMap.entrySet()) {
                MerchantUnreadMsgListResponse response = entry.getValue();
                response.setMerchantName(merchantIDToShopNameMap.get(entry.getKey()));
            }
        }

        ResultInfo resultInfo = new ResultInfo();
        if (unreadMsgList.size() == 0)
            resultInfo.addData("unreadMsgList", null);
        else
            resultInfo.addData("unreadMsgList", unreadMsgList);
        resultInfo.markSuccess();
        webAction.setResultInfo(resultInfo);
    }

    protected void doGetQueueLength(WebAction webAction) {
        //分流，获取排队人数
        Session session = webAction.getSession();
        String uuid = (String) webAction.getRequest().getAttribute(HttpAttributeKeys.ATTRIBUTE_UUID);
        int queueLength = session.getLastQueueLength(uuid);
        webAction.getRequest().setAttribute(HttpAttributeKeys.IS_FROM_CS, true);

        ResultInfo resultInfo = new ResultInfo();
        resultInfo.markSuccess();
        resultInfo.addData("queuedSeq", queueLength);

        if (queueLength == 0) {
            MerchantSubAccountInfo merchantSubAccount = session.getCurrentKefu(uuid);
            sendInitWindowMessage(webAction, merchantSubAccount);
            initSessionMappingInfo(webAction, merchantSubAccount);
            sendRetrieveOfflineMessages(session.getUserInfo().getId());

            resultInfo.addData("needWait", false);
            resultInfo.addData("subAccount", merchantSubAccount);
        } else {
            resultInfo.addData("needWait", true);
            resultInfo.markSuccess();
        }

        webAction.setResultInfo(resultInfo);
    }

    /**
     * 获取手机端最近联系人列表
     */
    protected void doGetRecentlyMobileContactList(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();

        //判断needClearCurrentMerchantId如果为true则清空当前聊天的商家ID
        String needClearCurrentMerchantId = ServletRequestUtil.getString(webAction.getRequest(), "needClearCurrentMerchantId");
        if (needClearCurrentMerchantId != null && needClearCurrentMerchantId.equals("true")) {
            Session session = webAction.getSession();
            String token = TokenManager.getToken(webAction.getRequest());
            session.setCurrentSubAccountIdMap(token, 0);
        }
        //获取最近联系商家
        GetRecentMerchantInfoRequest getRecentMerchantInfoRequest = new GetRecentMerchantInfoRequest();
        getRecentMerchantInfoRequest.setCustomerId(ServletRequestUtil.getInteger(request, "sid"));
        getRecentMerchantInfoRequest.setMaxNumber(20);
        List<RecentlyMobileSeller> recentlyMobileSellerList =
                chatService.getRecentMerchantInfo(getRecentMerchantInfoRequest);

        //获取未读消息
        Session session = webAction.getSession();
        SessionMessageCenter sessionMessageCenter = session.getMessageManagerCenter();
        List<MerchantUnreadMsgListResponse> unreadMsgList = sessionMessageCenter.getUnreadMsgList(TokenManager.getToken(webAction.getRequest()));

        //设置最近联系店铺
        Collection merchantIDs = new ArrayList();
        List<Integer> merchantIDList = new ArrayList();
        if (recentlyMobileSellerList != null) {
            //设置店铺最后一条消息
            for (int i = 0; i < recentlyMobileSellerList.size(); ) {
                RecentlyMobileSeller tmp = recentlyMobileSellerList.get(i);
                tmp.setLastMsg(RISK_DETECT_SERVICE.filterOutterLink(tmp.getLastMsg()));
                tmp.setLastMsg(CONVERT_SERVICE.doConvert(tmp.getLastMsg(), CONVERT_MSG_RULES_WHEN_GIVE));

                if (tmp.getMerchantId() == 0) {
                    recentlyMobileSellerList.remove(tmp);
                } else {
                    i++;
                }
            }

            //商家id到最近联系店铺信息的映射
            Map<Integer, RecentlyMobileSeller> merchantIDToRecentSellerMap = new HashMap<>(recentlyMobileSellerList.size());
            //最近联系店铺设置未读消息数
            for (RecentlyMobileSeller tmp : recentlyMobileSellerList) {
                for (MerchantUnreadMsgListResponse unread : unreadMsgList) {
                    if (unread.getMerchantId() == tmp.getMerchantId()) {
                        tmp.setUnreadMsgCount(unread.getUnreadMsgCount());
                    }
                }
                merchantIDs.add(tmp.getMerchantId());
                merchantIDList.add(tmp.getMerchantId());
                merchantIDToRecentSellerMap.put(tmp.getMerchantId(), tmp);
            }

            //获取店铺名称和logo
            if (merchantIDs.size() > 0) {
                List<StoreInfo> listStoreInfo = storeService.batchRetriveStoreInfo(merchantIDs);
                Map<Integer, String> merchantIDToShopNameMap = merchantService.retrieveStoreNameMap(merchantIDs);

                for (StoreInfo storeInfo : listStoreInfo) {
                    Integer merchantID = storeInfo.getMerchantId();
                    RecentlyMobileSeller seller = merchantIDToRecentSellerMap.get(merchantID);

                    seller.setMerchantName(storeInfo.getStoreName());
                    seller.setMerchantLogoUrl(CONVERT_SERVICE.doConvert(storeInfo.getStoreLogoUrl(), CONVERT_HEAD_IMG_RELATIVE));
                    merchantIDToRecentSellerMap.remove(merchantID);
                }

                for (Map.Entry<Integer, RecentlyMobileSeller> entry : merchantIDToRecentSellerMap.entrySet()) {
                    entry.getValue().setMerchantName(merchantIDToShopNameMap.get(entry.getKey()));
                }
            }
            //获取店铺是否在线
            ImApiService imApiService = ImApiService.INSTANCE;
            if (merchantIDs.size() > 0) {
                List<MerchantOnlineInfo> merchantOnlineInfos = imApiService.getMerchantOnlineInfo(merchantIDList);
                if (merchantOnlineInfos != null && merchantOnlineInfos.size() == recentlyMobileSellerList.size()) {
                    for (int i = 0; i < recentlyMobileSellerList.size(); i++) {
                        for (int j = 0; j < merchantOnlineInfos.size(); j++) {
                            if (recentlyMobileSellerList.get(i).getMerchantId() == merchantOnlineInfos.get(j).getId()) {
                                recentlyMobileSellerList.get(i).setOnline(merchantOnlineInfos.get(j).isOnline());
                                break;
                            }
                        }
                    }
                }
            }
        }
        if (recentlyMobileSellerList == null) {
            recentlyMobileSellerList = new ArrayList<>();
        } else {
            Collections.sort(recentlyMobileSellerList, new RecentlyMobileSellerComparator());
        }
        ResultInfo resultInfo = new ResultInfo();
        resultInfo.addData("contactList", recentlyMobileSellerList);
        resultInfo.markSuccess();

        webAction.setResultInfo(resultInfo);
    }

    protected void doGetHistoryList(WebAction webAction) {
        HttpServletRequest request = webAction.getRequest();
        Session session = webAction.getSession();
        ResultInfo resultInfo = new ResultInfo();

        GetHistoryMessageListRequest getHistoryMessageListRequest = new GetHistoryMessageListRequest();
        getHistoryMessageListRequest.setCustomerId(ServletRequestUtil.getInteger(request, "sid"));
        getHistoryMessageListRequest.setSellerId(ServletRequestUtil.getInteger(request, "fid"));
        getHistoryMessageListRequest.setIsMerchantId(ServletRequestUtil.getInteger(request, "isMerchant", 0) == 1);
        getHistoryMessageListRequest.setSendTime((int) (ServletRequestUtil.getLong(request, "time", 0l) / 1000));
        getHistoryMessageListRequest.setPageCount(ServletRequestUtil.getInteger(request, "msgnum", 10));

        List<HistoryMessage> historyMessageList = chatService.retrieveHistoryMessageList(getHistoryMessageListRequest);
        HistoryMessageListResponseBean responseBean = new HistoryMessageListResponseBean();
        responseBean.setMsgInfo(historyMessageList);

        if (CollectionUtils.isNotEmpty(historyMessageList)) {
            for (HistoryMessage historyMessage : historyMessageList) {
                historyMessage.setMsg(RISK_DETECT_SERVICE.filterOutterLink(historyMessage.getMsg()));
                historyMessage.setMsg(CONVERT_SERVICE.doConvert(historyMessage.getMsg(), CONVERT_MSG_RULES_WHEN_GIVE));
            }

            responseBean.setHasMore(historyMessageList.size() == getHistoryMessageListRequest.getPageCount() ? 1 : 0);
        }

        //店铺维度获取历史消息后清空未读消息数
        if (getHistoryMessageListRequest.getIsMerchantId()) {
            SessionMessageCenter sessionMessageCenter = session.getMessageManagerCenter();
            sessionMessageCenter.removeUnreadMap(TokenManager.getToken(request), getHistoryMessageListRequest.getSellerId());
        }

        resultInfo.addData("historyRequestInfo", responseBean);
        resultInfo.markSuccess();

        webAction.setResultInfo(resultInfo);
    }

    /**
     * 退出
     * {"vlink":0,"flag":244,"from":1002,"cmd":515,"seq":103,"sid":0,"dtype":1,"did":0,"len":0,"id":125,"authData":0}
     */
    protected void doExit(WebAction webAction) {
        Session session = webAction.getSession();
        HttpServletRequest request = webAction.getRequest();
        String uuid = (String) request.getAttribute(HttpAttributeKeys.ATTRIBUTE_UUID);
        ChatWindowHandle chatWindowHandle = session.getCurrentChatWindow(uuid);

        //如果是自营客服且占用 排队/接待 名额，发送消息给服务器
        if (chatWindowHandle != null && chatWindowHandle.isFromCS() && chatWindowHandle.getQueueLength() >= 0) {
            MerchantSubAccountInfo subAccountInfo = chatWindowHandle.getMerchantSubAccountInfo();
            int userID = Integer.parseInt(session.getId());
            ExitRequestBean exitRequestBean = ExitRequestBean.getMessage();
            exitRequestBean.setSeq(0);
            exitRequestBean.setSid(userID);
            exitRequestBean.setId(userID);
            exitRequestBean.setAuthData((byte) 1);

            CommunicationManager.instance.sendMessage(exitRequestBean);
            imApiService.notifyWindowCloseFromBuyer(userID, chatWindowHandle.getGroupID(),
                    subAccountInfo != null ? subAccountInfo.getId() : 0);
        }

        if (session != null) {
            session.removeChatLink(uuid);
            session.destroyIfNecessary();
        }
    }

    private MerchantOnlineInfoWebRequest parseMerchantOnlineRequest(HttpServletRequest request, Session session) {
        String merchantIDs = ServletRequestUtil.getString(request, RequestParams.MERCHANTIDS.toString());
        List<Integer> merchantIDList = new ArrayList<>();
        for (String merchantID : merchantIDs.split(",")) {
            int currentID = Integer.parseInt(merchantID);
            if (currentID > 0) {
                merchantIDList.add(currentID);
            }
        }

        MerchantOnlineInfoWebRequest merchantOnlineInfoWebRequest = new MerchantOnlineInfoWebRequest(session);
        merchantOnlineInfoWebRequest.setMerchantIDList(merchantIDList);

        return merchantOnlineInfoWebRequest;
    }

    private GetRecentlyContactWebRequest parseRecentlyContactRequest(HttpServletRequest request, Session session) {
        int id = Integer.parseInt(session.getId());
        int msgnum = ServletRequestUtil.getInteger(request, RequestParams.MSGNUM.toString(), Constants.NUMBER_OF_RECENTLY_SELLER_FETCH);
        GetRecentlyContactWebRequest getRecentlyContactWebRequest = new GetRecentlyContactWebRequest(session);

        getRecentlyContactWebRequest.setSenderID(id);
        getRecentlyContactWebRequest.setMsgNum(msgnum);

        return getRecentlyContactWebRequest;
    }
}
