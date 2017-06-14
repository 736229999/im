package com.cn.feiniu.logManagement.service.internal;

import com.cn.feiniu.logManagement.bean.base.ResultInfo;
import com.cn.feiniu.logManagement.bean.web.hessian.req.SubMerchantAction;

/**
 * Author: morningking
 * Date: 2015/7/17
 * Contact: 243717042@qq.com
 */
public interface UploadSubMerchantActionService {
    ResultInfo uploadAction(SubMerchantAction subMerchantAction);
}
