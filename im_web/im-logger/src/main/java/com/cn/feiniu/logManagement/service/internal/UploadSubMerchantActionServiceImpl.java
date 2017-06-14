package com.cn.feiniu.logManagement.service.internal;

import com.cn.feiniu.logManagement.bean.base.ResultInfo;
import com.cn.feiniu.logManagement.bean.web.hessian.req.SubMerchantAction;
import com.cn.feiniu.logManagement.constants.SubMerchantActionType;
import com.cn.feiniu.logManagement.core.SubMerchantLogManager;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

/**
 * Author: morningking
 * Date: 2015/7/17
 * Contact: 243717042@qq.com
 */
@Service("com.cn.feiniu.logManagement.service.internal.UploadSubMerchantActionService")
public class UploadSubMerchantActionServiceImpl implements UploadSubMerchantActionService {
    @Autowired
    private SubMerchantLogManager subMerchantLogManager;

    @Override
    public ResultInfo uploadAction(SubMerchantAction subMerchantAction) {
        ResultInfo resultInfo = new ResultInfo();

        if (subMerchantAction == null) {
            resultInfo.setErrorMessage("对象为空");
        } else if (SubMerchantActionType.getType(subMerchantAction.getActionType()) != null) {
            subMerchantLogManager.addLog(subMerchantAction);

            resultInfo.markSuccess();
        } else {
            throw new RuntimeException();
        }

        return resultInfo;
    }
}
