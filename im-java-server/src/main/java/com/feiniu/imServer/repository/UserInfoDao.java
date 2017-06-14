package com.feiniu.imServer.repository;

import com.feiniu.imServer.bean.CachedUserInfo;
import com.feiniu.imServer.bean.base.UserInfoInDB;
import com.feiniu.imServer.bean.request.SaveBuyerRequest;
import com.feiniu.imServer.bean.request.SaveSellerRequest;

/**
 * Created by wangzhen on 2016/2/19.
 */
public interface UserInfoDao {
    CachedUserInfo getCachedUserInfoInDB(Long userId);
    String getUserIdListInGroup(Long groupId);
    Integer saveBuyer(SaveBuyerRequest saveBuyerRequest);
    Integer saveSeller(SaveSellerRequest saveSellerRequest);
    UserInfoInDB getUserInfoInDB(Long userId);
    void updateImUserFeild(Long userId,String feild,String value);
    void updateImUserInfoFeild(Long userId,String feild,String value);
}
