package com.feiniu.subAccount.web;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import com.feiniu.core.web.annotation.LoginRequired;
import com.feiniu.subAccount.base.ResponseResult;
import com.feiniu.subAccount.domain.ServiceGroup;
import com.feiniu.subAccount.service.ServiceGroupService;

/**
 * Created by wangchen on 2015/2/7.
 */
@Controller
@RequestMapping(value = "service-group")
@LoginRequired
public class ServiceGroupController {
    @Autowired
    private ServiceGroupService serviceGroupService;

    @RequestMapping(value = "add-group", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult addServiceGroup(ServiceGroup serviceGroup) {
        ResponseResult responseResult = new ResponseResult();

        ServiceGroup addedGroup = serviceGroupService.addServiceGroup(serviceGroup);

        responseResult.addData("groupID", addedGroup.getGroupID());
        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "remove-group", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult removeServiceGroup(ServiceGroup serviceGroup) {
        ResponseResult responseResult = new ResponseResult();

        serviceGroupService.removeServiceGroup(serviceGroup);

        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "rename-group", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult renameServiceGroup(ServiceGroup serviceGroup) {
        ResponseResult responseResult = new ResponseResult();

        serviceGroupService.renameServiceGroup(serviceGroup);

        responseResult.markSuccess();
        return responseResult;
    }
}
