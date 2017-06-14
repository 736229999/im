package com.feiniu.csWebIm.utils;

import com.feiniu.csWebIm.bean.request.ServiceLogRequset;
import com.feiniu.csWebIm.consts.SystemConfig;
import com.feiniu.csWebIm.service.EndecryptedManager;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.web.bind.ServletRequestUtils;

import javax.servlet.http.HttpServletRequest;

/**
 * Created by wangzhen on 2015/12/11.
 */
public class UrlDecoderUtils {
    private static final Logger LOGGER = LoggerFactory.getLogger("UrlDecoderUtils");

    public static boolean decodeUrl(ServiceLogRequset serviceLogRequset,HttpServletRequest request){

        String customerToken = ServletRequestUtils.getStringParameter(request, "customerToken", "");
        String goodsAskingType = ServletRequestUtils.getStringParameter(request, "goodsAskingType", "");
        String orderToken = ServletRequestUtils.getStringParameter(request, "orderToken", "");

        if (customerToken == "" || goodsAskingType == "" ){
            LOGGER.error("解密参数失败:没找到参数 customerToken: "+ customerToken );
            LOGGER.error("解密参数失败:没找到参数 goodsAskingType: "+ goodsAskingType );
            return  false;
        }
        if (!decodeUrl(serviceLogRequset, customerToken, goodsAskingType, orderToken)){
            LOGGER.error("解密参数失败:参数解密失败  customerToken: "+ customerToken  +" goodsAskingType: "+ goodsAskingType
                        + " orderToken :"+orderToken);
            return false;
        }

        return  true;
    }
    public  static  boolean decodeUrl(ServiceLogRequset serviceLogRequset,String customerToken ,String goodsAskingType,String orderToken){
        serviceLogRequset.setGoodsAskingType(goodsAskingType);

        String decodedCustomerToken = EndecryptedManager.INSTANCE.decryptedWithURLDncode(customerToken);
        String encodedCustomerId = decodedCustomerToken.substring(0, SystemConfig.DEFAULT_ENCODE_SERVICE_ID);
        String encodedServiceId  = decodedCustomerToken.substring(SystemConfig.DEFAULT_ENCODE_SERVICE_ID);

        serviceLogRequset.setCustomerServiceId(encodedServiceId);
        serviceLogRequset.setCustomerId(encodedCustomerId);

        if (orderToken != ""){
            //serviceLogRequset.setOrId(orderToken);
            serviceLogRequset.setOrId(EndecryptedManager.INSTANCE.decryptedWithURLDncode(orderToken));
        }
        return true;
    }
}
