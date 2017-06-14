package com.feiniu.csWebIm.utils.encode;

/*
 --------------------------------------------**********--------------------------------------------

 璇ョ畻娉曚簬1977骞寸敱缇庡浗楹荤渷鐞嗗伐瀛﹂櫌MIT(Massachusetts Institute of Technology)鐨凴onal Rivest锛孉di Shamir鍜孡en Adleman涓変綅骞磋交鏁欐巿鎻愬嚭锛屽苟浠ヤ笁浜虹殑濮撴皬Rivest锛孲hamir鍜孉dlernan鍛藉悕涓篟SA绠楁硶锛屾槸涓�涓敮鎸佸彉闀垮瘑閽ョ殑鍏叡瀵嗛挜绠楁硶锛岄渶瑕佸姞瀵嗙殑鏂囦欢蹇殑闀垮害涔熸槸鍙彉鐨�!

 鎵�璋揜SA鍔犲瘑绠楁硶锛屾槸涓栫晫涓婄涓�涓潪瀵圭О鍔犲瘑绠楁硶锛屼篃鏄暟璁虹殑绗竴涓疄闄呭簲鐢ㄣ�傚畠鐨勭畻娉曞涓嬶細

 1.鎵句袱涓潪甯稿ぇ鐨勮川鏁皃鍜宷锛堥�氬父p鍜宷閮芥湁155鍗佽繘鍒朵綅鎴栭兘鏈�512鍗佽繘鍒朵綅锛夊苟璁＄畻n=pq锛宬=(p-1)(q-1)銆�

 2.灏嗘槑鏂囩紪鐮佹垚鏁存暟M锛屼繚璇丮涓嶅皬浜�0浣嗘槸灏忎簬n銆�

 3.浠诲彇涓�涓暣鏁癳锛屼繚璇乪鍜宬浜掕川锛岃�屼笖e涓嶅皬浜�0浣嗘槸灏忎簬k銆傚姞瀵嗛挜鍖欙紙绉颁綔鍏挜锛夋槸(e, n)銆�

 4.鎵惧埌涓�涓暣鏁癲锛屼娇寰梕d闄や互k鐨勪綑鏁版槸1锛堝彧瑕乪鍜宯婊¤冻涓婇潰鏉′欢锛宒鑲畾瀛樺湪锛夈�傝В瀵嗛挜鍖欙紙绉颁綔瀵嗛挜锛夋槸(d, n)銆�

 鍔犲瘑杩囩▼锛� 鍔犲瘑鍚庣殑缂栫爜C绛変簬M鐨別娆℃柟闄や互n鎵�寰楃殑浣欐暟銆�

 瑙ｅ瘑杩囩▼锛� 瑙ｅ瘑鍚庣殑缂栫爜N绛変簬C鐨刣娆℃柟闄や互n鎵�寰楃殑浣欐暟銆�

 鍙e銆乨鍜宯婊¤冻涓婇潰缁欏畾鐨勬潯浠躲�侻绛変簬N銆�

 --------------------------------------------**********--------------------------------------------
 */
import javax.crypto.Cipher;
import java.math.BigInteger;
import java.security.*;
import java.security.interfaces.RSAPublicKey;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;
import java.util.HashMap;
import java.util.Map;

public class RSA {
	/** 鎸囧畾key鐨勫ぇ灏� */
	private static int KEYSIZE = 1024;

	/**
	 * 鐢熸垚瀵嗛挜瀵�
	 */
	public static Map<String, String> generateKeyPair() throws Exception {
		/** RSA绠楁硶瑕佹眰鏈変竴涓彲淇′换鐨勯殢鏈烘暟婧� */
		SecureRandom sr = new SecureRandom();
		/** 涓篟SA绠楁硶鍒涘缓涓�涓狵eyPairGenerator瀵硅薄 */
		KeyPairGenerator kpg = KeyPairGenerator.getInstance("RSA");
		/** 鍒╃敤涓婇潰鐨勯殢鏈烘暟鎹簮鍒濆鍖栬繖涓狵eyPairGenerator瀵硅薄 */
		kpg.initialize(KEYSIZE, sr);
		/** 鐢熸垚瀵嗗寵瀵� */
		KeyPair kp = kpg.generateKeyPair();
		/** 寰楀埌鍏挜 */
		Key publicKey = kp.getPublic();
		byte[] publicKeyBytes = publicKey.getEncoded();
		String pub = new String(Base64.encodeBase64(publicKeyBytes),
				ConfigureEncryptAndDecrypt.CHAR_ENCODING);
		/** 寰楀埌绉侀挜 */
		Key privateKey = kp.getPrivate();
		byte[] privateKeyBytes = privateKey.getEncoded();
		String pri = new String(Base64.encodeBase64(privateKeyBytes),
				ConfigureEncryptAndDecrypt.CHAR_ENCODING);

		Map<String, String> map = new HashMap<String, String>();
		map.put("publicKey", pub);
		map.put("privateKey", pri);
		RSAPublicKey rsp = (RSAPublicKey) kp.getPublic();
		BigInteger bint = rsp.getModulus();
		byte[] b = bint.toByteArray();
		byte[] deBase64Value = Base64.encodeBase64(b);
		String retValue = new String(deBase64Value);
		map.put("modulus", retValue);
		return map;
	}

	/**
	 * 鍔犲瘑鏂规硶 source锛� 婧愭暟鎹�
	 */
	public static String encrypt(String source, String publicKey)
			throws Exception {
		Key key = getPublicKey(publicKey);
		/** 寰楀埌Cipher瀵硅薄鏉ュ疄鐜板婧愭暟鎹殑RSA鍔犲瘑 */
		Cipher cipher = Cipher.getInstance(ConfigureEncryptAndDecrypt.RSA_ALGORITHM);
		cipher.init(Cipher.ENCRYPT_MODE, key);
		byte[] b = source.getBytes();
		/** 鎵ц鍔犲瘑鎿嶄綔 */
		byte[] b1 = cipher.doFinal(b);
		return new String(Base64.encodeBase64(b1),
				ConfigureEncryptAndDecrypt.CHAR_ENCODING);
	}

	/**
	 * 瑙ｅ瘑绠楁硶 cryptograph:瀵嗘枃
	 */
	public static String decrypt(String cryptograph, String privateKey)
			throws Exception {
		Key key = getPrivateKey(privateKey);
		/** 寰楀埌Cipher瀵硅薄瀵瑰凡鐢ㄥ叕閽ュ姞瀵嗙殑鏁版嵁杩涜RSA瑙ｅ瘑 */
		Cipher cipher = Cipher.getInstance(ConfigureEncryptAndDecrypt.RSA_ALGORITHM);
		cipher.init(Cipher.DECRYPT_MODE, key);
		byte[] b1 = Base64.decodeBase64(cryptograph.getBytes());
		/** 鎵ц瑙ｅ瘑鎿嶄綔 */
		byte[] b = cipher.doFinal(b1);
		return new String(b);
	}

	/**
	 * 寰楀埌鍏挜
	 * 
	 * @param key
	 *            瀵嗛挜瀛楃涓诧紙缁忚繃base64缂栫爜锛�
	 * @throws Exception
	 */
	public static PublicKey getPublicKey(String key) throws Exception {
		X509EncodedKeySpec keySpec = new X509EncodedKeySpec(
				Base64.decodeBase64(key.getBytes()));
		KeyFactory keyFactory = KeyFactory.getInstance("RSA");
		PublicKey publicKey = keyFactory.generatePublic(keySpec);
		return publicKey;
	}

	/**
	 * 寰楀埌绉侀挜
	 * 
	 * @param key
	 *            瀵嗛挜瀛楃涓诧紙缁忚繃base64缂栫爜锛�
	 * @throws Exception
	 */
	public static PrivateKey getPrivateKey(String key) throws Exception {
		PKCS8EncodedKeySpec keySpec = new PKCS8EncodedKeySpec(
				Base64.decodeBase64(key.getBytes()));
		KeyFactory keyFactory = KeyFactory.getInstance("RSA");
		PrivateKey privateKey = keyFactory.generatePrivate(keySpec);
		return privateKey;
	}

	public static String sign(String content, String privateKey) {
		String charset = ConfigureEncryptAndDecrypt.CHAR_ENCODING;
		try {
			PKCS8EncodedKeySpec priPKCS8 = new PKCS8EncodedKeySpec(
					Base64.decodeBase64(privateKey.getBytes()));
			KeyFactory keyf = KeyFactory.getInstance("RSA");
			PrivateKey priKey = keyf.generatePrivate(priPKCS8);

			Signature signature = Signature.getInstance("SHA1WithRSA");

			signature.initSign(priKey);
			signature.update(content.getBytes(charset));

			byte[] signed = signature.sign();

			return new String(Base64.encodeBase64(signed));
		} catch (Exception e) {

		}

		return null;
	}
	
	public static boolean checkSign(String content, String sign, String publicKey)
	{
		try 
		{
			KeyFactory keyFactory = KeyFactory.getInstance("RSA");
	        byte[] encodedKey = Base64.decode2(publicKey);
	        PublicKey pubKey = keyFactory.generatePublic(new X509EncodedKeySpec(encodedKey));

		
			Signature signature = Signature
			.getInstance("SHA1WithRSA");
		
			signature.initVerify(pubKey);
			signature.update( content.getBytes("utf-8") );
		
			boolean bverify = signature.verify( Base64.decode2(sign) );
			return bverify;
			
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		
		return false;
	}	

}