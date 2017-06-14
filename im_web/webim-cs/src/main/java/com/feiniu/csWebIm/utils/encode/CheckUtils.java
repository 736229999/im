package com.feiniu.csWebIm.utils.encode;

import java.lang.reflect.Array;
import java.util.Collection;
import java.util.Map;


public class CheckUtils {

	public static final String COMMON_FIELD = "flowID,initiator,";


	/**
	 * 楠岃瘉瀵硅薄鏄惁涓篘ULL,绌哄瓧绗︿覆锛岀┖鏁扮粍锛岀┖鐨凜ollection鎴朚ap(鍙湁绌烘牸鐨勫瓧绗︿覆涔熻涓烘槸绌轰覆)
	 * @param obj 琚獙璇佺殑瀵硅薄
	 * @param message 寮傚父淇℃伅
	 */
	@SuppressWarnings("rawtypes")
	public static void notEmpty(Object obj, String message) {
		if (obj == null){
			throw new IllegalArgumentException(message + " must be specified");
		}
		if (obj instanceof String && obj.toString().trim().length()==0){
			throw new IllegalArgumentException(message + " must be specified");
		}
		if (obj.getClass().isArray() && Array.getLength(obj)==0){
			throw new IllegalArgumentException(message + " must be specified");
		}
		if (obj instanceof Collection && ((Collection)obj).isEmpty()){
			throw new IllegalArgumentException(message + " must be specified");
		}
		if (obj instanceof Map && ((Map)obj).isEmpty()){
			throw new IllegalArgumentException(message + " must be specified");
		}
	}
	

}
