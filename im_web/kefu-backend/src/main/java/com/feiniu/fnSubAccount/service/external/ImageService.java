package com.feiniu.fnSubAccount.service.external;

import com.feiniu.fnSubAccount.constants.BusinessConfig;
import com.feiniu.fnSubAccount.constants.Constants;
import com.feiniu.fnSubAccount.exception.BusinessException;
import com.feiniu.fnSubAccount.util.json.JsonUtil;
import org.apache.commons.io.IOUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import sun.misc.BASE64Encoder;

import javax.annotation.PostConstruct;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Author: morningking
 * Date: 2015/6/29
 * Contact: 243717042@qq.com
 */
@Component
public class ImageService {
    @Autowired
    private BusinessConfig businessConfig;

    private static final Logger LOGGER = LoggerFactory.getLogger(ImageService.class);
    private Pattern BASE_URL_LIST_SPLIT_URL_PATTERN = Pattern.compile("^(http://[^]]+)\\[((?:\\d+,?)+)\\](.+)$");
    private String[] BASE_URL_LIST;
    private String UPLOAD_PHOTO_URL;

    @PostConstruct
    public void initConfig() {
        UPLOAD_PHOTO_URL = businessConfig.getImageServiceURL() + "/imageupload/upload";

        //初始化图片下载根地址
        String baseUrlListString = businessConfig.getImageAccessUrlList();
        Matcher matcher = BASE_URL_LIST_SPLIT_URL_PATTERN.matcher(baseUrlListString);

        //如果是beta以上环境
        if (matcher.matches()) {
            String[] numberArray = matcher.group(2).split(",");
            BASE_URL_LIST = new String[numberArray.length];

            for (int i = 0; i < numberArray.length; i++) {
                BASE_URL_LIST[i] = matcher.group(1) + numberArray[i] + matcher.group(3);
            }
        } else {
            BASE_URL_LIST = new String[]{baseUrlListString};
        }
    }

    //随机的获取图片下载服务器地址中的一个作为根地址
    public String getRandomUrlBase() {
        int index = 0;
        if (BASE_URL_LIST.length > 1) {
            index = (int) Math.floor(Math.random() * BASE_URL_LIST.length);
        }

        return BASE_URL_LIST[index];
    }

    public String uploadPhoto(String fileName, InputStream inputStream) throws IOException {
        Map<String, Object> params = new HashMap<>(3);
        String uploadedURL = "";

        //设置参数
        params.put("fileName", fileName);
        params.put("category", Constants.UPLOAD_PHOTO_CATEGORY);
        params.put("content", new BASE64Encoder().encode(IOUtils.toByteArray(inputStream)));

        try {
            URL url = new URL(UPLOAD_PHOTO_URL);
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            connection.setDoOutput(true);
            connection.setDoInput(true);
            connection.setUseCaches(false);
            connection.setRequestMethod("POST");
            connection.setConnectTimeout(1000 * 5);
            connection.setRequestProperty("Connection", "Keep-Alive");
            connection.setRequestProperty("Content-type", "multipart/form-data;boundary=---------------------------7d318fd100112");
            PrintWriter dout = new PrintWriter(new OutputStreamWriter(connection.getOutputStream(), "utf-8"));
            dout.write(JsonUtil.toJsonString(params));
            dout.flush();
            dout.close();

            int responseCode = connection.getResponseCode();
            if (responseCode == HttpURLConnection.HTTP_OK) {
                String result = new String(IOUtils.toCharArray(connection.getInputStream(), Constants.WEB_MESSAGE_ENCODING));
                uploadedURL = JsonUtil.toMap(result).get("result").toString();

                LOGGER.debug("上传图片返回地址：" + uploadedURL);
            } else {
                LOGGER.error("上传图片http状态码：" + responseCode);
            }
        } catch (IOException exception) {
            throw new BusinessException("上传图片出现错误", exception);
        }

        return getRandomUrlBase() + uploadedURL;
    }

}
