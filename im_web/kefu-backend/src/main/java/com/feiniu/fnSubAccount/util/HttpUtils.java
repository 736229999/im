package com.feiniu.fnSubAccount.util;

import com.feiniu.fnSubAccount.consts.SystemConfig;
import org.apache.http.Consts;
import org.apache.http.HttpEntity;
import org.apache.http.NameValuePair;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.util.EntityUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * Created by morningking on 2014/12/23.
 */
public class HttpUtils {
    private static final Logger logger = LoggerFactory.getLogger(HttpUtils.class);

    public static String excutePost(String url, Map<String, Object> params) {
        CloseableHttpClient client = HttpClients.createDefault();
        CloseableHttpResponse response = null;
        String result = "";

        try {
            HttpPost post = new HttpPost(url);
            List<NameValuePair> formparams = new ArrayList<NameValuePair>();

            for (Map.Entry<String, Object> entry : params.entrySet()) {
                formparams.add(new BasicNameValuePair(entry.getKey(), entry.getValue().toString()));
            }
            UrlEncodedFormEntity entity = new UrlEncodedFormEntity(formparams, Consts.UTF_8);
            post.setEntity(entity);

            logger.info("params:" + formparams);
            logger.info("url-method:" + post);
            response = client.execute(post);
            HttpEntity responseEntity = response.getEntity();

            if (responseEntity != null) {
                result = EntityUtils.toString(responseEntity, SystemConfig.DEFAULT_ENCODING);
                logger.info("result: " + response);
            }
        } catch (Exception e) {
            throw new RuntimeException(e);
        } finally {
            try {
                if (response != null) {
                    response.close();
                }

                if (client != null) {
                    client.close();
                }

            } catch (Exception e) {
                throw new RuntimeException(e);
            }
        }

        return result;
    }

}
