package com.feiniu.fnSubAccount.controller;

import com.feiniu.fnSubAccount.base.ResponseResult;
import com.feiniu.fnSubAccount.consts.SystemConfig;
import com.feiniu.fnSubAccount.domain.AfterSaleBoardConfig;
import com.feiniu.fnSubAccount.domain.KefuIndexConfig;
import com.feiniu.fnSubAccount.repository.GroupInfoDao;
import com.feiniu.fnSubAccount.service.AfterSaleBoardService;
import com.feiniu.fnSubAccount.service.HomePageService;
import com.feiniu.fnSubAccount.service.KefuIndexConfigService;
import com.feiniu.fnSubAccount.service.external.ImageService;
import com.feiniu.fnSubAccount.util.io.ImageUtil;
import org.apache.commons.io.FileUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.multipart.MultipartFile;

import java.io.File;
import java.io.FileInputStream;
import java.sql.Timestamp;
import java.util.List;

/**
 * Author: morningking
 * Date: 2015/10/14
 * Contact: 243717042@qq.com
 */
@Controller
@RequestMapping(value = "relative-settings/kefu-home-page-config/")
public class HomePageConfigController {
    @Autowired
    private AfterSaleBoardService afterSaleBoardCofigService;
    @Autowired
    private KefuIndexConfigService kefuIndexConfigService;
    @Autowired
    private ImageService imageService;
    @Autowired
    private HomePageService homePageService;
    @Autowired
    private GroupInfoDao groupInfoDao;

    private static final Logger LOGGER = LoggerFactory.getLogger(HomePageConfigController.class);

    private static final AfterSaleBoardConfig[] dummyArray = new AfterSaleBoardConfig[0];

    @RequestMapping(value = "index")
    public String getIndex(ModelMap modelMap, boolean isRuntime, boolean isPreview, String selector) {
        List<AfterSaleBoardConfig> configList;
        KefuIndexConfig kefuIndexConfig;

        if (!isRuntime) {
            configList = afterSaleBoardCofigService.getDesignTimeData();
            kefuIndexConfig = kefuIndexConfigService.getDesignTimeData();
        } else {
            configList = afterSaleBoardCofigService.getRuntimeData();
            kefuIndexConfig = kefuIndexConfigService.getRuntimeData();
        }

        if (isPreview) {
            homePageService.doFetchFooterOfFeiniuIndex(null);
        }

        modelMap.put("afterSaleBoardConfigList", configList.toArray(dummyArray));
        modelMap.put("kefuIndexConfig", kefuIndexConfig);
        modelMap.put("groupList", groupInfoDao.getAllGroup());

        return "relative-settings/kefu-home-page-config";
    }

    @RequestMapping(value = "update-after-sale-board", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult updateAfterSaleBoard(AfterSaleBoardConfig afterSaleBoardConfig) {
        ResponseResult responseResult = new ResponseResult();

        afterSaleBoardConfig.setDataType(SystemConfig.DESIGN_TIME_INDICATOR);
        afterSaleBoardConfig.setUpdateTime(new Timestamp(System.currentTimeMillis()));
        afterSaleBoardCofigService.update(afterSaleBoardConfig);

        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "update-index-config", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult updateIndexConfig(KefuIndexConfig kefuIndexConfig) {
        ResponseResult responseResult = new ResponseResult();

        kefuIndexConfig.setDataType(SystemConfig.DESIGN_TIME_INDICATOR);
        kefuIndexConfig.setUpdateTime(new Timestamp(System.currentTimeMillis()));

        kefuIndexConfigService.update(kefuIndexConfig);

        responseResult.markSuccess();
        return responseResult;
    }

    @RequestMapping(value = "upload-baner-image", method = RequestMethod.POST)
    @ResponseBody
    public ResponseResult uploadBannerPhoto(@RequestParam(value = "file") MultipartFile multipartFile) {
        ResponseResult responseResult = new ResponseResult();
        String fileName = multipartFile.getOriginalFilename();
        String extName = fileName.substring(fileName.lastIndexOf(".") + 1);


        try {
            File file = FileUtils.getFile(FileUtils.getTempDirectory(), System.currentTimeMillis() + "." + extName);
            multipartFile.transferTo(file);

            Integer[] imageInfo = ImageUtil.getImageWidthHeight(file);
            responseResult.addData("imageWidth", imageInfo[0]);
            responseResult.addData("imageHeight", imageInfo[1]);

            String photoURL = imageService.uploadPhoto(fileName, new FileInputStream(file));

            responseResult.addData("uploadedURL", photoURL);
            responseResult.markSuccess();
        } catch (Exception e) {
            LOGGER.error("上传图片发生失败", e);
        }

        return responseResult;
    }

    @RequestMapping(value = "publish")
    @ResponseBody
    public ResponseResult publish() {
        ResponseResult responseResult = new ResponseResult();

        homePageService.publish();
        responseResult.markSuccess();

        return responseResult;
    }

    @RequestMapping(value = "is-group-bind")
    @ResponseBody
    public ResponseResult isGroupBind(int groupID) {
        ResponseResult responseResult = new ResponseResult();

        responseResult.addData("isBind", afterSaleBoardCofigService.isGroupBinded(groupID));
        responseResult.markSuccess();

        return responseResult;
    }
}
