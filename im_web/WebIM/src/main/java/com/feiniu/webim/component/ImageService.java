package com.feiniu.webim.component;

import com.fasterxml.jackson.databind.JsonNode;
import com.feiniu.webim.bean.helper.KVPair;
import com.feiniu.webim.config.ConfigManager;
import com.feiniu.webim.constants.Constants;
import com.feiniu.webim.exception.BusinessException;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.json.JsonNodeUtil;
import com.feiniu.webim.util.web.HttpClientUtils;
import org.apache.commons.io.IOUtils;
import org.apache.log4j.Logger;
import sun.misc.BASE64Encoder;

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
 * Author: wangchen
 * Date: 2015/6/29
 * Contact: 243717042@qq.com
 */
public class ImageService {
    public static final ImageService IMAGE_SERVICE = new ImageService();

    private ImageService() {
    }

    private static final String OLD_UPLOAD_PHOTO_URL = ConfigManager.getProperty("service_image") + "common/upload.do";
    private static final String UPLOAD_PHOTO_URL = ConfigManager.getProperty("service_image") + "/imageupload/upload";
    private static final Logger LOGGER = Logger.getLogger(ImageService.class);
    private static final Pattern BASE_URL_LIST_SPLIT_URL_PATTERN = Pattern.compile("^(http://[^\\[]+)\\[((?:\\d+,?)+)\\](.+)$");
    private static final String[] BASE_URL_LIST;

    static {
        //初始化图片下载根地址
        String baseUrlListString = ConfigManager.getProperty("image_access_url_list");
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

    //随机的获取商城图片下载服务器地址中的一个作为根地址
    public static final String getMerchantRandomUrlBase() {
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

        LOGGER.debug("url: " + UPLOAD_PHOTO_URL);

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
            dout.write(JSONUtil.toJsonString(params));
            dout.flush();
            dout.close();

            int responseCode = connection.getResponseCode();
            if (responseCode == HttpURLConnection.HTTP_OK) {
                String result = new String(IOUtils.toCharArray(connection.getInputStream(), Constants.WEB_MESSAGE_ENCODING));
                uploadedURL = JSONUtil.toMap(result).get("result").toString();

                LOGGER.debug("result: " + result);
            } else {
                LOGGER.error("上传图片http状态码：" + responseCode);
            }
        } catch (IOException exception) {
            LOGGER.error("上传图片出现错误", exception);
            throw new BusinessException(exception);
        }

        return getMerchantRandomUrlBase() + uploadedURL;
    }

    public String uploadPhotoOld(Map<String, Object> params, KVPair<String, InputStream> filePart) {
        String uploadedURL = "";

        try {
            String response = HttpClientUtils.executeMultiPartRequest(OLD_UPLOAD_PHOTO_URL, params, filePart);
            JsonNode responseNode = JSONUtil.toNode(response);

            if (responseNode.has("result") && "true".equals(JsonNodeUtil.getString(responseNode, "result"))
                    && responseNode.has("url")) {
                uploadedURL = JsonNodeUtil.getString(responseNode, "url");
            }
        } catch (IOException exception) {
            throw new BusinessException("上传图片出现错误", exception);
        }

        return uploadedURL;
    }
}
