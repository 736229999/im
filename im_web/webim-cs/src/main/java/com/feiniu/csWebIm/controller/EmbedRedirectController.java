package com.feiniu.csWebIm.controller;

import com.feiniu.csWebIm.bean.response.UserCheckInInfo;
import com.feiniu.csWebIm.constants.FromPageType;
import com.feiniu.csWebIm.consts.BusinessConfig;
import com.feiniu.csWebIm.exception.NonLoginFromMemberException;
import com.feiniu.csWebIm.service.external.KefuBackendService;
import com.feiniu.csWebIm.service.external.UserInfoService;
import org.apache.commons.codec.binary.Base64;
import org.apache.commons.lang.StringUtils;
import org.apache.commons.lang.math.NumberUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.ServletRequestUtils;
import org.springframework.web.bind.annotation.RequestMapping;

import javax.servlet.http.HttpServletRequest;
import java.util.Map;

/**
 * Author: morningking
 * Date: 2016/1/18
 * Contact: 243717042@qq.com
 */
@Controller
@RequestMapping(value = "router")
public class EmbedRedirectController {
    @Autowired
    private BusinessConfig businessConfig;
    @Autowired
    private UserInfoService userInfoService;
    @Autowired
    private KefuBackendService kefuBackendService;

    private static final Logger LOGGER = LoggerFactory.getLogger(EmbedRedirectController.class);

    /**
     * 对 来自布点页的请求做处理：
     * 1. redirec到聊天页；
     * 2. redirect至live800；
     *
     * @param request
     * @return
     */
    @RequestMapping(value = "embed-dispatch")
    public String embedPageRouter(HttpServletRequest request) throws Exception {
        String type = ServletRequestUtils.getStringParameter(request, "type", "");
        String redirectUrl = ServletRequestUtils.getRequiredStringParameter(request, "redirectUrl");
        String orderSeq = ServletRequestUtils.getStringParameter(request, "orderSeq", "");
        redirectUrl = new String(Base64.decodeBase64(redirectUrl));
        boolean isRedirect;

        try {
            UserCheckInInfo userCheckInInfo = userInfoService.checkUserInfoFromMember(request.getCookies());
            String userName = userCheckInInfo.getUserName();
            userName = userName.replaceAll("\\D", "");
            Long userNameHashCode = userName.length() == 0 ? 0l : Long.parseLong(userName);

            isRedirect = userNameHashCode % businessConfig.getGrayFactor() == businessConfig.getGrayLuckyNumber();
        } catch (NonLoginFromMemberException nonLoginException) {
            isRedirect = true;
            LOGGER.info("redirect-controller capture non-login request");
        }

        if (isRedirect) {
            Map<String, Integer> embedSetMap = kefuBackendService.getEmbedToGroupMap();
            int groupID = 0;
            if (embedSetMap.containsKey(type)) {
                groupID = embedSetMap.get(type);
            }

            redirectUrl = "/index.jsp?groupID=" + groupID;
            if (StringUtils.isNotEmpty(orderSeq)) {
                redirectUrl += "&orderSeq=" + orderSeq;
            }
            redirectUrl += "&type=" + type + "&fromPageType=" + FromPageType.getType(type);
        }

        return "redirect:" + redirectUrl;
    }
}
