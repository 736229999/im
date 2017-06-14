package com.feiniu.csWebIm.utils.encode;


import org.apache.commons.codec.digest.DigestUtils;

import java.io.UnsupportedEncodingException;
import java.security.MessageDigest;

public class MD5 {
	
	public final static String getMD5(String s) {
		char hexDigits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
				'a', 'b', 'c', 'd', 'e', 'f' };
		try {
			byte[] strTemp = s.getBytes();
			MessageDigest mdTemp = MessageDigest.getInstance("MD5");
			
			mdTemp.update(strTemp);
			
			byte[] md = mdTemp.digest();
			int j = md.length;
			char str[] = new char[j * 2];
			int k = 0;
			for (int i = 0; i < j; i++) {
				byte byte0 = md[i];
				str[k++] = hexDigits[byte0 >>> 4 & 0xf];
				str[k++] = hexDigits[byte0 & 0xf];
			}
			return new String(str);
		} catch (Exception e) {
			return null;
		}
	}

	/**
	 * 绛惧悕瀛楃涓?
	 * @param text 闇?瑕佺鍚嶇殑瀛楃涓?
	 * @param key 瀵嗛挜
	 * @param input_charset 缂栫爜鏍煎紡
	 * @return 绛惧悕缁撴灉
	 */
	public static String sign(String text, String key, String input_charset) {
		text = text + key;
		return DigestUtils.md5Hex(getContentBytes(text, input_charset));
	}

	/**
	 * 绛惧悕瀛楃涓?
	 * @param text 闇?瑕佺鍚嶇殑瀛楃涓?
	 * @param sign 绛惧悕缁撴灉
	 * @param key 瀵嗛挜
	 * @param input_charset 缂栫爜鏍煎紡
	 * @return 绛惧悕缁撴灉
	 */
	public static boolean verify(String text, String sign, String key, String input_charset) {
		text = text + key;
		String mysign = DigestUtils.md5Hex(getContentBytes(text, input_charset));
		if(mysign.equals(sign)) {
			return true;
		}
		else {
			return false;
		}
	}

	/**
	 * @param content
	 * @param charset
	 * @return
	 * @throws java.security.SignatureException
	 * @throws UnsupportedEncodingException
	 */
	private static byte[] getContentBytes(String content, String charset) {
		if (charset == null || "".equals(charset)) {
			return content.getBytes();
		}
		try {
			return content.getBytes(charset);
		} catch (UnsupportedEncodingException e) {
			throw new RuntimeException("MD5绛惧悕杩囩▼涓嚭鐜伴敊璇?,鎸囧畾鐨勭紪鐮侀泦涓嶅,鎮ㄧ洰鍓嶆寚瀹氱殑缂栫爜闆嗘槸:" + charset);
		}
	}

	public static void main(String[] args) {
		String pwd="111111";
		System.out.println("鏄庢枃锛?"+pwd+"瀵嗘枃锛?"+MD5.getMD5(pwd));
	}

}
