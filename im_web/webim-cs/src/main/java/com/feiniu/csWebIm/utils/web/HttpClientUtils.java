package com.feiniu.csWebIm.utils.web;
import com.feiniu.csWebIm.bean.domain.KVPair;
import com.feiniu.csWebIm.consts.Constants;
import com.feiniu.csWebIm.exception.HttpGetException;
import org.apache.http.Consts;
import org.apache.http.HttpEntity;
import org.apache.http.NameValuePair;
import org.apache.http.client.config.RequestConfig;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.protocol.HttpClientContext;
import org.apache.http.client.utils.URIBuilder;
import org.apache.http.conn.scheme.Scheme;
import org.apache.http.conn.ssl.AllowAllHostnameVerifier;
import org.apache.http.conn.ssl.SSLSocketFactory;
import org.apache.http.entity.mime.MultipartEntity;
import org.apache.http.entity.mime.content.InputStreamBody;
import org.apache.http.entity.mime.content.StringBody;
import org.apache.http.impl.client.BasicCookieStore;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.impl.cookie.BasicClientCookie;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.protocol.BasicHttpContext;
import org.apache.http.protocol.HttpContext;
import org.apache.http.util.EntityUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;
import javax.servlet.http.Cookie;
import java.io.IOException;
import java.io.InputStream;
import java.net.URISyntaxException;
import java.security.cert.X509Certificate;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by morningking on 2014/12/23.
 */
public class HttpClientUtils {
    private static final Logger LOGGER = LoggerFactory.getLogger(HttpClientUtils.class);
    private static final int SOCKET_TIME_OUT = 30000;
    private static final int CONNECT_TIME_OUT = 5000;
    private static final int CONNECT_POOL_WAIT_TIME_OUT = 10000;

    //设置请求和传输超时时间
    private static final RequestConfig HTTP_CLIENT_REQUEST_DEFAULT_CONFIG =
            RequestConfig.custom()
                    .setSocketTimeout(SOCKET_TIME_OUT).setConnectTimeout(CONNECT_TIME_OUT)
                    .setConnectionRequestTimeout(CONNECT_POOL_WAIT_TIME_OUT)
                    .build();

    public static String excutePost(String url, Map<String, Object> params) throws IOException {
        CloseableHttpClient client = HttpClients.createDefault();


        CloseableHttpResponse response = null;
        String result = "";

        try {
            HttpPost post = new HttpPost(url);
            post.setConfig(HTTP_CLIENT_REQUEST_DEFAULT_CONFIG);
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
        return executeGet(host, path, params, null, null);
    }

    public static String executeGet(String host, String path, Map<String, Object> params, Cookie[] cookies, String cookieDomain)
            throws HttpGetException {
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
            httpget.setConfig(HTTP_CLIENT_REQUEST_DEFAULT_CONFIG);
            BasicCookieStore cookieStore = new BasicCookieStore();

            if (cookies != null) {
                for (Cookie cookie : cookies) {
                    BasicClientCookie basicCookie = new BasicClientCookie(cookie.getName(), cookie.getValue());
                    basicCookie.setDomain(cookieDomain);
                    basicCookie.setPath("/");
                    cookieStore.addCookie(basicCookie);
                }
            }

            HttpContext localContext = new BasicHttpContext();
            localContext.setAttribute(HttpClientContext.COOKIE_STORE, cookieStore);

            LOGGER.info("url:" + uriBuilder.toString());
            LOGGER.info("params:" + params);
            LOGGER.info("cookies:" + cookieStore);

            HttpEntity responseEntity = client.execute(httpget, localContext).getEntity();

            if (responseEntity != null) {
                result = EntityUtils.toString(responseEntity, Constants.WEB_MESSAGE_ENCODING);
                LOGGER.info("result:" + result);
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

    public static String executeHttpsGet(String host, String path, Map<String, Object> params, Cookie[] cookies, String cookieDomain)
            throws HttpGetException {
        DefaultHttpClient client = httpClientTrustingAllSSLCerts();
        String result = "";

        URIBuilder uriBuilder = new URIBuilder();
        uriBuilder.setScheme("https");
        uriBuilder = uriBuilder.setHost(host).setPath(path);

        for (Map.Entry<String, Object> entry : params.entrySet()) {
            uriBuilder = uriBuilder.setParameter(entry.getKey(), entry.getValue().toString());
        }

        try {
            HttpGet httpget = new HttpGet(uriBuilder.build());
            httpget.setConfig(HTTP_CLIENT_REQUEST_DEFAULT_CONFIG);
            BasicCookieStore cookieStore = new BasicCookieStore();

            if (cookies != null) {
                for (Cookie cookie : cookies) {
                    BasicClientCookie basicCookie = new BasicClientCookie(cookie.getName(), cookie.getValue());
                    basicCookie.setDomain(cookieDomain);
                    basicCookie.setPath("/");
                    cookieStore.addCookie(basicCookie);
                }
            }

            HttpContext localContext = new BasicHttpContext();
            localContext.setAttribute(HttpClientContext.COOKIE_STORE, cookieStore);

            LOGGER.info("url:" + uriBuilder.toString());
            LOGGER.info("params:" + params);
            LOGGER.info("cookies:" + cookieStore);

            HttpEntity responseEntity = client.execute(httpget, localContext).getEntity();

            if (responseEntity != null) {
                result = EntityUtils.toString(responseEntity, Constants.WEB_MESSAGE_ENCODING);
                LOGGER.info("result:" + result);
            }
        } catch (IOException e) {
            throw new HttpGetException(e);
        } catch (URISyntaxException e) {
            throw new HttpGetException(e);
        } finally {
            if (client != null) {
                client.close();
            }
        }

        return result;
    }

    public static String executeHttpsPost(String url, Map<String, Object> params) throws IOException{
        CloseableHttpClient client = httpClientTrustingAllSSLCerts();

        CloseableHttpResponse response = null;
        String result = "";

        try {
            HttpPost post = new HttpPost(url);
            post.setConfig(HTTP_CLIENT_REQUEST_DEFAULT_CONFIG);
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

    private static DefaultHttpClient httpClientTrustingAllSSLCerts() {
        try {
            DefaultHttpClient httpclient = new DefaultHttpClient();

            SSLContext sc = SSLContext.getInstance("SSL");
            sc.init(null, getTrustingManager(), new java.security.SecureRandom());

            SSLSocketFactory socketFactory = new SSLSocketFactory(sc);
            socketFactory.setHostnameVerifier(new AllowAllHostnameVerifier());
            Scheme sch = new Scheme("https", 443, socketFactory);
            httpclient.getConnectionManager().getSchemeRegistry().register(sch);
            return httpclient;
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    private static TrustManager[] getTrustingManager() {
        TrustManager[] trustAllCerts = new TrustManager[]{new X509TrustManager() {
            @Override
            public X509Certificate[] getAcceptedIssuers() {
                return null;
            }

            @Override
            public void checkClientTrusted(X509Certificate[] certs, String authType) {
                // Do nothing
            }

            @Override
            public void checkServerTrusted(X509Certificate[] certs, String authType) {
                // Do nothing
            }

        }};
        return trustAllCerts;
    }

    /**
     * 上传文件接口
     * @param params 表单参数
     * @param inputStreamEntry {文件流,文件名称}
     * @return 上传后得到的文件名
     */
    public static String executeMultiPartRequest(String url, Map<String, Object> params,
                                                 KVPair<String,InputStream> inputStreamEntry) throws IOException{
        CloseableHttpClient client = HttpClients.createDefault();

        CloseableHttpResponse response = null;
        String result = "";

        try {
            HttpPost post = new HttpPost(url);

            MultipartEntity multiPartEntity = new MultipartEntity();
            for (Map.Entry<String, Object> entry : params.entrySet()) {
                if (entry.getValue() != null) {
                    multiPartEntity.addPart(entry.getKey(), new StringBody(entry.getValue().toString()));
                }
            }
            multiPartEntity.addPart("attachment", new InputStreamBody(inputStreamEntry.getValue(), inputStreamEntry.getKey()));

            post.setEntity(multiPartEntity);

            LOGGER.info("params:" + multiPartEntity);
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

    public static void main(String[] args) throws Exception {
        Map<String, Object> map = new HashMap<>();
        map.put("sid", -1);
        map.put("event", "PUT_CHAT_MESSAGE");
        map.put("features", "{\"weight\":0,\"style\":0,\"line\":0,\"color\":0,\"family\":\"Microsoft YaHei\",\"size\":\"14\"}");
        map.put("did", 71);
        map.put("type", 0);
        map.put("msg", "aaa");

    }
}
