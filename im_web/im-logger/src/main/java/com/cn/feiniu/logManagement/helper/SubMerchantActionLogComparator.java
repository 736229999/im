package com.cn.feiniu.logManagement.helper;

import com.cn.feiniu.logManagement.bean.web.hessian.req.SubMerchantAction;

import java.util.Comparator;

/**
 * Author: morningking
 * Date: 2015/8/3
 * Contact: 243717042@qq.com
 */
public class SubMerchantActionLogComparator implements Comparator<SubMerchantAction> {
    @Override
    public int compare(SubMerchantAction o1, SubMerchantAction o2) {
        if (o1.getMerchantID() == o2.getMerchantID()) {
            if (o1.getSubMerchantID() == o2.getSubMerchantID()) {
                return o1.getActionTime() - o2.getActionTime();
            } else {
                return (int) (o1.getSubMerchantID() - o2.getSubMerchantID());
            }
        } else {
            return o1.getMerchantID() - o2.getMerchantID();
        }
    }
}
