package com.cn.feiniu.logManagement.repository;

import com.cn.feiniu.logManagement.bean.web.req.SaveBuyerRequest;
import com.cn.feiniu.logManagement.bean.web.req.SaveSellerRequest;

/**
 * Created by zhangmin on 2016/2/29.
 */
public interface UserInfoDao {

    Integer saveBuyer(SaveBuyerRequest saveBuyerRequest);

    Integer saveSeller(SaveSellerRequest saveSellerRequest);

    void saveCS();

}
