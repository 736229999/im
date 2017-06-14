package com.feiniu.subAccount.service;

import com.feiniu.subAccount.component.repository.DBStoreLightConfigManager;
import com.feiniu.subAccount.domain.MerchantConnectInfo;
import com.feiniu.subAccount.domain.StoreLightConfig;
import com.feiniu.subAccount.service.external.MerchantService;
import com.feiniu.subAccount.util.FormatUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

/**
 * Created by wangzhen on 2015/11/27.
 */
@Service
public class PhoneNumberMemcacheService {
    @Autowired
    CacheClientService cacheClientService;

    public static final String CONNETC_INFO = "MerchantConnectInfo#ALLINFO#";
    @Autowired
    DBStoreLightConfigManager dbStoreLightConfigManager;

    @Autowired
    MerchantService merchantService;

    private MerchantConnectInfo queryConnectNumberFromDB(Long merchantID){
        MerchantConnectInfo merchantConnectInfo = new MerchantConnectInfo();
        StoreLightConfig result =dbStoreLightConfigManager.getStoreLightConfigUnderMerchant(merchantID);
        //哞哞数据库中没有
        if (result ==null ){
            return null;
        }

        String telphone = FormatUtils.formatPhoneStr(result.getTelphone());
        String phone = FormatUtils.formatPhoneStr(result.getPhone());

        //如果都是默认值,或者都为空
        if (telphone ==null && phone ==null){
            return null;
        }

        merchantConnectInfo.setPhone(phone);
        merchantConnectInfo.setTelphone(telphone);
        return  merchantConnectInfo;
    }

    public MerchantConnectInfo doGetConnectNumber(Long merchantID){
        MerchantConnectInfo  merchantConnectInfo = getMerchantConnectInfo(merchantID);
        //memcache里面没有
        if (merchantConnectInfo == null){
            //数据库里面查找
            merchantConnectInfo = queryConnectNumberFromDB(merchantID);
            //数据库里面有，加入到memcache后返回
            if (merchantConnectInfo != null){
                //写入到memcache
                putMerchantConnectInfo(merchantID, merchantConnectInfo);
            }else{
                //数据库里面没有,调用商城的接口,商城的数据不用缓存，不然会导致数据不一致
                merchantConnectInfo = merchantService.doGetMerchantPhoneInfoById(String.valueOf(merchantID));
            }
        }
        return merchantConnectInfo;
    }
    public MerchantConnectInfo getMerchantConnectInfo(Long merchantId){
        if (merchantId == null)
            return null;

        String key = CONNETC_INFO + merchantId.toString();
        MerchantConnectInfo merchantConnectInfo = cacheClientService.getValue(key,MerchantConnectInfo.class);

        return merchantConnectInfo;
    }

    public boolean putMerchantConnectInfo(Long merchantId,MerchantConnectInfo merchantConnectInfo){
        if (merchantId == null || merchantConnectInfo ==null )
            return false;

        String key = CONNETC_INFO + merchantId.toString();
         cacheClientService.put(key,merchantConnectInfo);

        return true;
    }

    public boolean putMerchantConnectInfo(Long merchantId,String phone,String telphone){
        telphone = FormatUtils.formatPhoneStr(telphone);
        phone = FormatUtils.formatPhoneStr(phone);
        if(telphone == null && phone == null)
            return false;
        MerchantConnectInfo merchantConnectInfo = new MerchantConnectInfo();
        merchantConnectInfo.setPhone(phone);
        merchantConnectInfo.setTelphone(telphone);

        return putMerchantConnectInfo(merchantId,merchantConnectInfo);
    }
}
