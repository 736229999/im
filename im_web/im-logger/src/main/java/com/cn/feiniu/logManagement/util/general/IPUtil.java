package com.cn.feiniu.logManagement.util.general;


import javax.servlet.http.HttpServletRequest;

/**
 * IP工具类
 *
 * @author zb
 * @date 2014-11-12 下午1:43:59
 */
public class IPUtil {
    /**
     * 将字符串型ip转成int型ip
     *
     * @param strIp
     * @return
     */
    public static long ipToLong(String strIp) {
        String[] ss = strIp.split("\\.");
        if (ss.length != 4) {
            return 0;
        }

        byte[] parts = new byte[ss.length];
        for (int i = 0; i < parts.length; i++) {
            parts[i] = (byte) Integer.parseInt(ss[i]);
        }

        int intResult = byteToInt(parts);
        long result = Long.valueOf("0" + Integer.toBinaryString(intResult), 2);

        return result;
    }

    /**
     * 将int型ip转成String型ip
     *
     * @param longIp
     * @return
     */
    public static String longToIP(long longIp) {
        byte[] bytes = longToByte(longIp);
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < 4; i++) {
            sb.append(bytes[i] & 0xFF);
            if (i < 3) {
                sb.append(".");
            }
        }
        return sb.toString();
    }

    private static byte[] longToByte(long i) {
        byte[] bytes = new byte[4];

        bytes[0] = (byte) (0xff & i);
        bytes[1] = (byte) ((0xff00 & i) >> 8);
        bytes[2] = (byte) ((0xff0000 & i) >> 16);
        bytes[3] = (byte) ((0xff000000 & i) >> 24);

        return bytes;
    }

    public static byte[] intToByteArray(int i) {
        byte[] targets = new byte[4];

        targets[3] = (byte) (i & 0xff);// 最低位
        targets[2] = (byte) ((i >> 8) & 0xff);// 次低位
        targets[1] = (byte) ((i >> 16) & 0xff);// 次高位
        targets[0] = (byte) (i >>> 24);// 最高位,无符号右移。
        return targets;
    }

    private static int byteToInt(byte[] parts) {
        int n = parts[0] & 0xFF;

        n |= ((parts[1] << 8) & 0xFF00);
        n |= ((parts[2] << 16) & 0xFF0000);
        n |= ((parts[3] << 24) & 0xFF000000);

        return n;
    }

    public static int byteArrayToInt(byte[] buffer) {
        int value = 0;

        for (int i = 0; i < 4; i++) {
            int shift = (4 - 1 - i) * 8;
            value += (buffer[i] & 0x000000FF) << shift;
        }

        return value;
    }

    public static String getIpAddr(HttpServletRequest request) {
        String ip = request.getHeader("x-forwarded-for");
        if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {
            ip = request.getHeader("Proxy-Client-IP");
        }
        if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {
            ip = request.getHeader("WL-Proxy-Client-IP");
        }
        if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {
            ip = request.getHeader("HTTP_CLIENT_IP");
        }
        if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {
            ip = request.getHeader("HTTP_X_FORWARDED_FOR");
        }
        if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {
            ip = request.getRemoteAddr();
        }
        return ip;
    }

    public static void main(String[] args) {
        String ip1 = "10.211.64.180";
        long intIp = ipToLong(ip1);
        System.out.println(intIp);
        String ip2 = longToIP(intIp);
        System.out.println(ip2.equals(ip1));
    }
}
