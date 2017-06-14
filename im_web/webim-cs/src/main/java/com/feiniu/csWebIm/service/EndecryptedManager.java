package com.feiniu.csWebIm.service;


import com.feiniu.csWebIm.utils.encode.AES;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.net.URLDecoder;
import java.net.URLEncoder;

/**
 * Author: morningking
 * Date: 2015/11/26
 * Contact: 243717042@qq.com
 */
public class EndecryptedManager {
    public static final EndecryptedManager INSTANCE = new EndecryptedManager();

    private EndecryptedManager() {
    }

    public static final String AES_KEY = "feiniu-webim2015";
    private static final String CODEC_CHARASET = "utf-8";
    private static final Logger LOGGER = LoggerFactory.getLogger(EndecryptedManager.class);

    public String encrypted(String data) {
        try {
            String encrypted = AES.encryptToBase64(data, AES_KEY);

            return encrypted;
        } catch (Exception e) {
            LOGGER.error("encode error", e);
        }

        return null;
    }

    public String encryptedWithURLEncode(String data) {
        try {
            String encrypted = AES.encryptToBase64(data, AES_KEY);
            encrypted = URLEncoder.encode(encrypted, CODEC_CHARASET);

            return encrypted;
        } catch (Exception e) {
            LOGGER.error("encode error", e);
        }

        return null;
    }

    public String decryptedWithURLDncode(String data) {
        try {
            data = URLDecoder.decode(data, CODEC_CHARASET);
            data =  data.replaceAll("\\s","+");
            return AES.decryptFromBase64(data, AES_KEY);
        } catch (Exception e) {
            LOGGER.error("decode Error", e);
        }

        return null;
    }

    public String decrypted(String data) {
        try {
            return AES.decryptFromBase64(data, AES_KEY);
        } catch (Exception e) {
            LOGGER.error("decode Error", e);
        }

        return null;
    }
}
