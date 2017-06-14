package com.feiniu.fnSubAccount.repository;

import com.feiniu.fnSubAccount.bean.SubMerchantInfo;

import java.util.List;

/**
 * Created by wangzhen on 2015/10/10.
 */
public interface CustomerServiceAuthDao {
    boolean isIdDirector(long id);
    Long convertIDToMoumouId(long directorId);
    List<SubMerchantInfo> getAllSubMerchantInfoList();
    List<SubMerchantInfo> getSubMerchantInfoListByIdList(String idList);
}
