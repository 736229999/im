package com.cn.feiniu.logManagement.web.controller;

import com.cn.feiniu.logManagement.bean.base.WebResultInfo;
import com.cn.feiniu.logManagement.bean.dto.SubMerchantAttendanceDetail;
import com.cn.feiniu.logManagement.service.internal.SubMerchantActionStatisticsService;
import com.cn.feiniu.logManagement.task.SubMerchantActionStatisticsTask;
import com.cn.feiniu.logManagement.util.general.FormatUtils;
import com.cn.feiniu.logManagement.util.json.JsonUtil;
import org.apache.commons.lang.time.DateUtils;
import org.apache.commons.logging.LogFactory;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.WebDataBinder;
import org.springframework.web.bind.annotation.InitBinder;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import java.beans.PropertyEditorSupport;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.*;

/**
 * Author: morningking
 * Date: 2015/8/5
 * Contact: 243717042@qq.com
 * 手动跑批 对外接口
 */
@Controller
@RequestMapping(value = "mannual-count")
public class MannualCountController {
    @Autowired
    private SubMerchantActionStatisticsTask subMerchantActionStatisticsTask;
    private static final Logger LOGGER = LoggerFactory.getLogger(MannualCountController.class);

    @InitBinder
    public void initBinder(WebDataBinder binder) {
        binder.registerCustomEditor(Date.class, new PropertyEditorSupport() {
            public void setAsText(String value) {
                try {
                    setValue(new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").parse(value));
                } catch (ParseException e) {
                    try {
                        setValue(new SimpleDateFormat("yyyy-MM-dd").parse(value));
                    } catch (ParseException e1) {
                        try {
                            setValue(new SimpleDateFormat("HH:mm:ss").parse(value));
                        } catch (ParseException e2) {
                            setValue(null);
                        }
                    }
                }
            }
        });
    }

    @RequestMapping(value = "submerchant-action")
    @ResponseBody
    public WebResultInfo countSubmerchantAction(Date startDate, Date endDate) {
        WebResultInfo resultInfo = new WebResultInfo();

        if (startDate == null) {
            resultInfo.setErrorMessage("开始时间为空");
        } else {
            startDate = DateUtils.truncate(startDate, Calendar.DATE);
            endDate = endDate == null ? DateUtils.addDays(startDate, 1) : DateUtils.truncate(endDate, Calendar.DATE);
            Map<String, Object> params = new HashMap<>(1);

            while (startDate.before(endDate)) {
                params.put("date", startDate);
                try {
                    subMerchantActionStatisticsTask.runTask(params);
                    resultInfo.addData(FormatUtils.formatDate(startDate), "成功");
                } catch (Exception e) {
                    LOGGER.warn("手动跑批发生错误，跑批任务参数为：" + JsonUtil.toJsonString(params), e);
                }

                startDate = DateUtils.addDays(startDate, 1);
            }

            resultInfo.markSuccess();
        }

        return resultInfo;
    }
}
