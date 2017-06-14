package com.feiniu.csWebIm.utils.encode;

import java.security.MessageDigest;

public class EncryptionUtil {
	
	// 瀵瑰瓧绗︿覆杩涜md5鍔犲瘑
	public static String md5(String str) {
		try {
			MessageDigest md = MessageDigest.getInstance("MD5");
			md.update(str.getBytes());

			byte[] b = md.digest();
			StringBuffer sb = new StringBuffer();
			for (int i = 0; i < b.length; i++) {
				int v = (int) b[i];
				v = v < 0 ? 0x100 + v : v;
				String cc = Integer.toHexString(v);
				if (cc.length() == 1)
					sb.append('0');
				sb.append(cc);
			}

			return sb.toString();
		} catch (Exception e) {
		}
		return "";
	}
	
	// 瀵瑰瓧绗︿覆杩涜sha256鍔犲瘑
	public static String sha256(String str) {
		try {
			MessageDigest md = MessageDigest.getInstance("SHA-256");
			md.update(str.getBytes());

			byte[] b = md.digest();
			StringBuffer sb = new StringBuffer();
			for (int i = 0; i < b.length; i++) {
				int v = (int) b[i];
				v = v < 0 ? 0x100 + v : v;
				String cc = Integer.toHexString(v);
				if (cc.length() == 1)
					sb.append('0');
				sb.append(cc);
			}

			return sb.toString();
		} catch (Exception e) {
		}
		return "";
	}
	// 瀵瑰瓧绗︿覆杩涜sha1鍔犲瘑
	public static String sha1(String str) {
		try {
			MessageDigest md = MessageDigest.getInstance("SHA-1");
			md.update(str.getBytes());

			byte[] b = md.digest();
			StringBuffer sb = new StringBuffer();
			for (int i = 0; i < b.length; i++) {
				int v = (int) b[i];
				v = v < 0 ? 0x100 + v : v;
				String cc = Integer.toHexString(v);
				if (cc.length() == 1)
					sb.append('0');
				sb.append(cc);
			}

			return sb.toString();
		} catch (Exception e) {
		}
		return "";
	}
	
}
