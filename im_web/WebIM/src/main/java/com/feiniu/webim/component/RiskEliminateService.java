package com.feiniu.webim.component;

import org.apache.commons.lang.StringUtils;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Author: morningking
 * Date: 2015/10/23
 * Contact: 243717042@qq.com
 */
public class RiskEliminateService {
    public static final RiskEliminateService INSTANCE = new RiskEliminateService();

    private RiskEliminateService() {
    }

    private static final Pattern INNER_LINK_PATTERN =
            Pattern.compile("(?:https?[:]//(?:www\\.)?|www\\.)" +
                    "(?:(?:[-\\w_]+\\.)+(?:com|cn|net|org|fn|edu)|(?:(?:\\d{1,3}[.]){3}\\d{1,3}(?:[:]\\d{2,5})?))" +
                    "(?:[-\\w./\\?%&#_=;,]+)?");
    private static final String[] WHITE_LIST_PATTERN_LITERAL = new String[]{
            "(?:https?[:]//)?(?:[-\\w_]+\\.){1,3}(?:feiniu\\.com|beta1\\.fn|dev1\\.fn|idc1\\.fn|fn\\.com).*",
            "(?:https?[:]//)?wh-image01\\.fn\\.com.*",
            "(?:https?[:]//)?10.202.249.218.*",
            "(?:https?[:]//)?img\\d+\\.fn-mart\\.com.*",
            "(?:https?[:]//)?(?:www\\.)?feiniu\\.com.*"
    };

    //屏蔽外链
    public String filterOutterLink(String message) {
        if (StringUtils.isEmpty(message)) {
            return "";
        }

        Matcher matcher = INNER_LINK_PATTERN.matcher(message);
        StringBuilder resultBuilder = new StringBuilder();
        int previousStart = 0;

        while (matcher.find()) {
            int start = matcher.start();
            if (start > previousStart) {
                resultBuilder.append(message.substring(previousStart, start));
            }

            String currentUrlPart = matcher.group();
            if (isInWhiteList(currentUrlPart)) {
                resultBuilder.append(currentUrlPart);
            } else {
                resultBuilder.append("");
            }

            previousStart = matcher.end();
        }

        if (previousStart < message.length()) {
            resultBuilder.append(message.substring(previousStart, message.length()));
        }

        return resultBuilder.toString();
    }

    private static boolean isInWhiteList(String url) {
        boolean result = false;

        for (String whiteListPattern : WHITE_LIST_PATTERN_LITERAL) {
            if (url.matches(whiteListPattern)) {
                result = true;
                break;
            }
        }

        return result;
    }

    public static void main(String[] args) {
        String message = "aaahttps://feiniu.index.com?aa=bb哈哈哈aahttp://www.feiniu.com?hahahah=999-恩恩呢000http://baidu.com?texgunx 9900嗯嗯嗯";
        message = "aaa";

        System.out.println(INSTANCE.filterOutterLink(message));
    }
}
