package com.feiniu.fnSubAccount.controller;

import com.feiniu.fnSubAccount.base.ResponseResult;
import com.feiniu.fnSubAccount.domain.TouchPortalBoardConfig;
import com.feiniu.fnSubAccount.repository.GroupInfoDao;
import com.feiniu.fnSubAccount.service.TouchPortalBoardConfigService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import java.sql.Timestamp;
import java.util.List;

/**
 * Author: morningking
 * Date: 2016/3/8
 * Contact: 243717042@qq.com
 */
@Controller
@RequestMapping(value = "relative-settings/touch-portal-board-config")
public class TouchPortalBoardConfigController {
    @Autowired
    private TouchPortalBoardConfigService touchPortalBoardConfigService;
    @Autowired
    private GroupInfoDao groupInfoDao;

    @RequestMapping(value = "config", method = RequestMethod.GET)
    public String getConfigIndex(ModelMap modelMap, Boolean isRuntime, Boolean isPreview) {
        List<TouchPortalBoardConfig> touchPortalBoardConfigList;

        if (isRuntime != null && isRuntime) {
            touchPortalBoardConfigList = touchPortalBoardConfigService.getRuntimeData();
        } else {
            touchPortalBoardConfigList = touchPortalBoardConfigService.getDesignTimeData();
        }

        modelMap.put("configList", touchPortalBoardConfigList);
        modelMap.put("isPreview", (isPreview != null && isPreview) || (isRuntime != null && isRuntime));
        modelMap.put("groupList", groupInfoDao.getAllGroup());

        return "relative-settings/touch-portal-board-config";
    }

    @RequestMapping(value = "add")
    @ResponseBody
    public ResponseResult add(TouchPortalBoardConfig config) {
        ResponseResult responseResult = new ResponseResult();

        config.setUpdateTime(new Timestamp(System.currentTimeMillis()));

        touchPortalBoardConfigService.add(config);
        responseResult.addData("config", config);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "modify")
    @ResponseBody
    public ResponseResult modify(TouchPortalBoardConfig touchPortalBoardConfig) {
        ResponseResult responseResult = new ResponseResult();

        touchPortalBoardConfig.setUpdateTime(new Timestamp(System.currentTimeMillis()));
        touchPortalBoardConfigService.modify(touchPortalBoardConfig);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "delete")
    @ResponseBody
    public ResponseResult delete(int id) {
        ResponseResult responseResult = new ResponseResult();

        touchPortalBoardConfigService.delete(id);
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "change-order")
    @ResponseBody
    public ResponseResult changeOrder(int srcID, int destID) {
        ResponseResult responseResult = new ResponseResult();

        touchPortalBoardConfigService.changeOrder(srcID, destID);

        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "get-list")
    @ResponseBody
    public ResponseResult getList() {
        ResponseResult responseResult = new ResponseResult();

        responseResult.addData("list", touchPortalBoardConfigService.getBoardList());

        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "publish")
    @ResponseBody
    public ResponseResult publish() {
        ResponseResult responseResult = new ResponseResult();

        touchPortalBoardConfigService.syncEditToRuntime();

        responseResult.markSuccess();
        return responseResult;
    }
}
