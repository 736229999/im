package com.feiniu.webim.util.encrypt;

import com.feiniu.webim.constants.Constants;
import sun.misc.BASE64Decoder;
import sun.misc.BASE64Encoder;

import javax.crypto.Cipher;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;
import java.io.IOException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.security.Security;

/**
 * 
 * 加密解密工具
 * 
 * @author pengxiaodong
 * 
 */
public class EncryptDecryptUtils {

	private static final byte[] keyBytes = { 0x11, 0x22, 0x4F, 0x58, (byte) 0x88, 0x10, 0x40, 0x38, 0x28, 0x25, 0x79, 0x51, (byte) 0xCB, (byte) 0xDD, 0x55, 0x66, 0x77, 0x29, 0x74, (byte) 0x98, 0x30, 0x40, 0x36, (byte) 0xE2 }; // 24字节的密钥

	private static final String Algorithm = "DESede"; // 定义 加密算法,可用 DES,DESede,Blowfish
	
	/**
	 * MD5加密
	 * 
	 * @param str
	 * @return
	 */
	public static String md5Encrypt(String str) {
		return md5Encrypt(str, Constants.MD5_KEY);
	}
	
	/**
	 * MD5加密
	 * 
	 * @param str
	 * @return
	 */
	public static String md5Encrypt(String str, String key) {
		if (str == null || str.trim().length() == 0) {
			return null;
		}

		StringBuffer hexString = new StringBuffer();
		try {
			MessageDigest md = MessageDigest.getInstance("MD5");
			str = key + str;
			md.update(str.getBytes());
			byte[] hash = md.digest();

			for (int i = 0; i < hash.length; i++) {
				if ((0xFF & hash[i]) < 16) {
					hexString.append("0" + Integer.toHexString(0xFF & hash[i]));
				} else {
					hexString.append(Integer.toHexString(0xFF & hash[i]));
				}
			}
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
		}

		return hexString.toString();
	}

	/**
	 * base64加密
	 * 
	 * @param str
	 * @return
	 */
	public static String base64Encrypt(String str) {
		if (str == null || str.trim().length() == 0) {
			return null;
		}

		return new BASE64Encoder().encode(str.getBytes());
	}

	/**
	 * base64解密
	 * 
	 * @param str
	 * @return
	 */
	public static String base64Decrypt(String str) {
		if (str == null || str.trim().length() == 0) {
			return null;
		}

		try {
			return new String(new BASE64Decoder().decodeBuffer(str));
		} catch (IOException e) {
			e.printStackTrace();
		}

		return null;
	}
	
	/**
	 * 3des加密
	 * 
	 * @param src
	 * @return
	 */
	public static String des3Crypt(String src) {
		if (src == null || src.trim().length() == 0) {
			return null;
		}
		
		try {
			// 生成密钥
			SecretKey deskey = new SecretKeySpec(keyBytes, Algorithm);

			// 加密
			Cipher c1 = Cipher.getInstance(Algorithm);
			c1.init(Cipher.ENCRYPT_MODE, deskey);
			return new String(c1.doFinal(src.getBytes()));
		} catch (NoSuchAlgorithmException e1) {
			e1.printStackTrace();
		} catch (javax.crypto.NoSuchPaddingException e2) {
			e2.printStackTrace();
		} catch (Exception e3) {
			e3.printStackTrace();
		}
		
		return null;
	}

	/**
	 * 3des解密
	 * 
	 * @param src
	 * @return
	 */
	public static String des3Decrypt(String src) {
		if (src == null || src.trim().length() == 0) {
			return null;
		}
		
		try {
			// 生成密钥
			SecretKey deskey = new SecretKeySpec(keyBytes, Algorithm);

			// 解密
			Cipher c1 = Cipher.getInstance(Algorithm);
			c1.init(Cipher.DECRYPT_MODE, deskey);
			return new String(c1.doFinal(src.getBytes()));
		} catch (NoSuchAlgorithmException e1) {
			e1.printStackTrace();
		} catch (javax.crypto.NoSuchPaddingException e2) {
			e2.printStackTrace();
		} catch (Exception e3) {
			e3.printStackTrace();
		}
		
		return null;
	}

	// 转换成十六进制字符串
	public static String byte2hex(byte[] b) {
		String hs = "";
		String stmp = "";

		for (int n = 0; n < b.length; n++) {
			stmp = (Integer.toHexString(b[n] & 0XFF));
			if (stmp.length() == 1)
				hs = hs + "0" + stmp;
			else
				hs = hs + stmp;
			if (n < b.length - 1)
				hs = hs + ":";
		}
		
		return hs.toUpperCase();
	}
	
	public static void main(String[] args) {
		Security.addProvider(new com.sun.crypto.provider.SunJCE());
		String ss = "er234923i0rjkfl(*0(*we012I`1";
		String s22 = des3Crypt(ss);
		String s33 = des3Decrypt(s22);
		
		System.out.println(ss);
		System.out.println(s22);
		System.out.println(s33);
		
	}
}
