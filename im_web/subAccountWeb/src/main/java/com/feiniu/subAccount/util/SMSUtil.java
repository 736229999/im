package com.feiniu.subAccount.util;

import java.util.Random;

/**
 * Created by morningking on 2014/12/23.
 */
public class SMSUtil {
    private static final Random RANDOM = new Random(System.currentTimeMillis());

    public static String retrieveSMSVerifyCode(int length) {
        if (length < 1) {
            throw new IllegalArgumentException("长度应该大于0");
        }

        StringBuilder builder = new StringBuilder();

        for (int i = 0; i < length; i++) {
            builder.append(RANDOM.nextInt(10));
        }

        System.out.println("Generate code" + builder);
        return builder.toString();
    }
}
