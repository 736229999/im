package com.feiniu.webim.component;

import org.apache.commons.lang.StringUtils;

import java.util.List;
import java.util.regex.Pattern;

/**
 * Author: morningking
 * Date: 2015/11/10
 * Contact: 243717042@qq.com
 */
public class ConvertService {
    public static final ConvertService CONVERT_SERVICE = new ConvertService();

    private ConvertService() {
    }

    public String doConvert(String toReplace, List<ConvertRule> rules) {
        for (ConvertRule rule : rules) {
            toReplace = rule.replace(toReplace);
        }

        return toReplace;
    }

    public enum ConvertRule {
        //让图片地址变成相对地址
        MAKE_PHOTO_URL_RELATIVE(Pattern.compile("(<image:)(http://[^/]+)([^>]+>)")) {
            @Override
            String doReplace(String toReplace) {
                return getReplacePattern().matcher(toReplace).replaceAll("$1$3");
            }
        },
        //让图片地址变成绝对地址
        MAKE_PHOTO_URL_ABSOLUTE(Pattern.compile("(<image:)(/[^:/>]+[/][-=_\\w/]+\\.[a-zA-Z]{2,6}>)")) {
            @Override
            String doReplace(String toReplace) {
                return getReplacePattern().matcher(toReplace).replaceAll("$1" + ImageService.getMerchantRandomUrlBase() + "$2");
            }
        },
        //im返回的聊天数据中包含老商城图片，替换之
        TRANSFORM_OLD_MERCHANT_PHOTO_FROM_IM(Pattern.compile("(<image:)(http://[^/]+)(/group\\d+/[^.]+[.]\\w+>)")) {
            @Override
            String doReplace(String toReplace) {
                return getReplacePattern().matcher(toReplace).replaceAll("$1" + ImageService.getMerchantRandomUrlBase() + "$3");
            }
        },
        //自营客服 发送表情转换为相对地址
        MAKE_CS_CUSTOMER_EMOJI_RELATIVE(
                Pattern.compile("(<image:)(http://(?:(?:local|webim).(?:beta1|dev1).fn|merchant.feiniu.com)" +
                        "/webim/static/images/feiniu-emotion/+)(b\\d{2}\\.gif>)")) {
            @Override
            String doReplace(String toReplace) {
                return getReplacePattern().matcher(toReplace).replaceAll("$1$3");
            }
        },
        //最近联系人头像、店铺logo转换为绝对地址
        MEKE_HEAD_IMG_ABSOLUTE(Pattern.compile("^/[^:/>]+[/][-=_\\w/]+\\.[a-zA-Z]{2,6}$")) {
            @Override
            String doReplace(String toReplace) {
                return getReplacePattern().matcher(toReplace).replaceAll(ImageService.getMerchantRandomUrlBase() + "$0");
            }
        },
        //对于接口数据返回的老接口地址，替换域名
        TRANSFORM_OLD_MERCHANT_PHOTO_FROM_EXTERNAL(Pattern.compile("^(http://[^/]+)(/group\\d+/[^.]+[.]\\w+)$")) {
            @Override
            String doReplace(String toReplace) {
                return getReplacePattern().matcher(toReplace).replaceAll(ImageService.getMerchantRandomUrlBase() + "$2");
            }
        },;

        private Pattern replacePattern;

        ConvertRule(Pattern replacePattern) {
            this.replacePattern = replacePattern;
        }

        Pattern getReplacePattern() {
            return replacePattern;
        }

        abstract String doReplace(String toReplace);

        String replace(String toReplace) {
            if (StringUtils.isEmpty(toReplace)) {
                return "";
            } else {
                return doReplace(toReplace);
            }
        }
    }
}
