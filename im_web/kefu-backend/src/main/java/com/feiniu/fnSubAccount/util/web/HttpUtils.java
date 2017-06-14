package com.feiniu.fnSubAccount.util.web;

import com.feiniu.fnSubAccount.constants.Constants;
import com.feiniu.fnSubAccount.exception.HttpGetException;
import org.apache.http.Consts;
import org.apache.http.HttpEntity;
import org.apache.http.NameValuePair;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.utils.URIBuilder;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.util.EntityUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.net.InetAddress;
import java.net.URISyntaxException;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * Created by morningking on 2014/12/23.
 */
public class HttpUtils {
    private static final Logger LOGGER = LoggerFactory.getLogger(HttpUtils.class);

    public static String excutePost(String url, Map<String, Object> params) throws IOException {
        CloseableHttpClient client = HttpClients.createDefault();
        CloseableHttpResponse response = null;
        String result = "";

        try {
            HttpPost post = new HttpPost(url);
            List<NameValuePair> formparams = new ArrayList<NameValuePair>();

            for (Map.Entry<String, Object> entry : params.entrySet()) {
                if (entry.getValue() != null) {
                    formparams.add(new BasicNameValuePair(entry.getKey(), entry.getValue().toString()));
                }
            }
            UrlEncodedFormEntity entity = new UrlEncodedFormEntity(formparams, Consts.UTF_8);
            post.setEntity(entity);

            LOGGER.info("params:" + formparams);
            LOGGER.info("url-method:" + post);
            response = client.execute(post);
            HttpEntity responseEntity = response.getEntity();

            if (responseEntity != null) {
                result = EntityUtils.toString(responseEntity, Constants.WEB_MESSAGE_ENCODING);
                LOGGER.info("response: " + response);
                LOGGER.info("result: " + result);
            }
        } finally {
            if (response != null) {
                response.close();
            }

            if (client != null) {
                client.close();
            }
        }

        return result;
    }

    public static String executeGet(String host, String path, Map<String, Object> params) throws HttpGetException {
        CloseableHttpClient client = HttpClients.createDefault();
        String result = "";

        URIBuilder uriBuilder = new URIBuilder();
        uriBuilder.setScheme("http");
        uriBuilder = uriBuilder.setHost(host).setPath(path);

        for (Map.Entry<String, Object> entry : params.entrySet()) {
            uriBuilder = uriBuilder.setParameter(entry.getKey(), entry.getValue().toString());
        }

        try {
            HttpGet httpget = new HttpGet(uriBuilder.build());
            LOGGER.info("get-request:" + httpget);
            HttpEntity responseEntity = client.execute(httpget).getEntity();

            if (responseEntity != null) {
                result = EntityUtils.toString(responseEntity, Constants.WEB_MESSAGE_ENCODING);
                LOGGER.info("result: " + result);
            }
        } catch (IOException e) {
            throw new HttpGetException(e);
        } catch (URISyntaxException e) {
            throw new HttpGetException(e);
        } finally {
            if (client != null) {
                try {
                    client.close();
                } catch (IOException e) {
                    throw new HttpGetException(e);
                }
            }
        }

        return result;
    }

    public static  String getIPAddress(){
        try {
            return InetAddress.getLocalHost().getHostAddress();
        } catch (UnknownHostException e) {
            LOGGER.error("Get Ip Address Failed!",e);
        }
        return  null;
    }
}
