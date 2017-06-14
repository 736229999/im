package com.feiniu.imServer.service.base;

import com.feiniu.imServer.bean.config.ImageServerPathPool;
import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Author: morningking
 * Date: 2015/11/10
 * Contact: 243717042@qq.com
 */
@Service
public class ImageConvertService {
    @Autowired
    private ImageServerPathPool imageServerPathPool;

    //商城图片 相对地址
    private static final Pattern MERCHANT_RELATIVE_IMAGE_PATH = Pattern.compile("^/[^:/>]+[/][-=_\\w/]+\\.[a-zA-Z]{2,6}$");
    //商城图片 绝对地址
    private static final Pattern MERCHANT_ABSOLUTE_IMAGE_PATH = Pattern.compile("^(http://[^/]+)(/group\\d+/[^.]+[.]\\w+)$");

    /**
     * 获取转换后的商城图片
     *
     * @param imagePath
     * @return 转换后的商城图片，如果不符合规则，返回入参
     */
    public String getConvertedMerchantImage(String imagePath) {
        if (StringUtils.isNotEmpty(imagePath)) {
            String randomServerPath = imageServerPathPool.getRandomMerchantImageServerUrlBase();
            Matcher matcher = MERCHANT_ABSOLUTE_IMAGE_PATH.matcher(imagePath);

            if (matcher.matches()) {
                return matcher.replaceAll(randomServerPath + "$2");
            } else if ((matcher = MERCHANT_RELATIVE_IMAGE_PATH.matcher(imagePath)).matches()) {
                return matcher.replaceAll(randomServerPath + "$0");
            }
        }

        return "";
    }

    /**
     * 获取转换后的自营图片
     *
     * @param imagePath
     * @return 转换后的商城图片，如果不符合规则，返回入参
     */
    public String getSelfSupportImage(String imagePath) {
        if (StringUtils.isNotEmpty(imagePath)) {
            return imageServerPathPool.getSelfSupportImgAccessUrl() + imagePath;
        }

        return "";
    }
}
