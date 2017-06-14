package com.cn.feiniu.logManagement.core;

import com.cn.feiniu.logManagement.bean.base.BusinessConfig;
import com.cn.feiniu.logManagement.bean.web.hessian.req.SubMerchantAction;
import com.cn.feiniu.logManagement.constants.SystemConfig;
import org.apache.commons.io.FileUtils;
import org.apache.commons.lang.time.DateUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import javax.annotation.PostConstruct;
import java.io.File;
import java.util.Date;
import java.util.HashMap;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

/**
 * Author: morningking
 * Date: 2015/7/22
 * Contact: 243717042@qq.com
 */
@Service
public class SubMerchantLogManager {
    @Autowired
    private BusinessConfig businessConfig;
    @Autowired
    private LogFileNamePatternManager logFileNamePatternManager;
    private static final Logger LOGGER = LoggerFactory.getLogger(SubMerchantLogManager.class);

    private BlockingQueue<SubMerchantAction> blockingQueue = null;

    @PostConstruct
    private void startWorker() {
        LOGGER.info("开启工作者线程");
        blockingQueue = new ArrayBlockingQueue<>(businessConfig.getMaxRequestInMemory());

        LogTransfer logTransfer = new LogTransfer();
        logTransfer.init();

        Thread thread = new Thread(logTransfer, "submerchant-action-log-writer");
        thread.start();
    }

    public void addLog(SubMerchantAction subMerchantAction) {
        blockingQueue.add(subMerchantAction);
    }

    private SubMerchantAction getLog() {
        try {
            return blockingQueue.take();
        } catch (InterruptedException e) {
            LOGGER.error("获取消息时被打断", e);
            throw new RuntimeException(e);
        }
    }

    /**
     * 写日志 工作者
     */
    private class LogTransfer implements Runnable {
        private String fileName;
        private File currentDayLogFile;
        private Date currentFileBaseTime = new Date();
        private HashMap<String, Object> params = new HashMap<>(1);
        private boolean isStatusNormal = true;

        void init() {
            checkAndCreateLogDirectory();
            createLogFile(new Date());
        }

        /**
         * 检查并创建日志文件存放目录
         */
        void checkAndCreateLogDirectory() {
            File logDirectory = new File(businessConfig.getLogFilePath());

            if (!logDirectory.exists()) {
                if (!logDirectory.mkdirs()) {
                    throw new Error("初始化日志工作者失败，创建目录失败：" + businessConfig.getLogFilePath());
                } else {
                    LOGGER.info("创建日志路径成功：" + businessConfig.getLogFilePath());
                }
            } else {
                LOGGER.info("日志路径已存在：" + businessConfig.getLogFilePath());
            }
        }

        /**
         * 创建日志文件
         *
         * @param date 需要创建日志的日期
         */
        void createLogFile(Date date) {
            currentFileBaseTime = date;
            params.put("date", currentFileBaseTime);

            fileName = businessConfig.getLogFilePath() +
                    logFileNamePatternManager.getLogFileName(LogFileNamePatternManager.LogTypeEnum.SUB_MERCHANT_ACTION_LOG_PATTERN,
                            params);

            currentDayLogFile = new File(fileName);
            if (!currentDayLogFile.exists()) {
                try {
                    currentDayLogFile.createNewFile();
                    markStatusNormal();
                    LOGGER.info("创建日志文件成功：" + fileName);
                } catch (Exception e) {
                    markStatusAbNormal();
                    LOGGER.error("创建日志文件失败", e);
                }
            } else {
                markStatusNormal();
                LOGGER.info("日志文件已存在：" + fileName);
            }
        }

        /**
         * 检查日期，如果是新的一天，就创建新的日志文件
         *
         * @param date 需要检查的日期
         */
        void checkDateAndModifyIfNeccessary(Date date) {
            boolean isSameDay = DateUtils.isSameDay(date, currentFileBaseTime);

            if (!isSameDay) {
                createLogFile(date);
            }
        }

        void markStatusNormal() {
            this.isStatusNormal = true;
        }

        void markStatusAbNormal() {
            this.isStatusNormal = false;
        }

        boolean isStatusNormal() {
            return isStatusNormal;
        }

        void appendLine(SubMerchantAction subMerchantAction) {
            StringBuilder builder = new StringBuilder();
            String fieldSeparator = SystemConfig.SUB_MERCHANT_ACTION_LOG_FIELD_SEPARATOR;

            builder.append(subMerchantAction.getMerchantID()).append(fieldSeparator);
            builder.append(subMerchantAction.getSubMerchantID()).append(fieldSeparator);
            builder.append(subMerchantAction.getActionType()).append(fieldSeparator);
            builder.append(subMerchantAction.getActionTime());
            builder.append(SystemConfig.SYSTEM_LINE_SEPARATOR);

            try {
                Date actionDate = new Date(subMerchantAction.getActionTime() * SystemConfig.UNIX_TIME_TRANSFORM_FACTOR);
                checkDateAndModifyIfNeccessary(actionDate);

                FileUtils.write(currentDayLogFile, builder.toString(), true);
            } catch (Exception e) {
                markStatusAbNormal();
                addLog(subMerchantAction);
                throw new RuntimeException(e);
            }
        }

        boolean recoverFSProblem() {
            while (currentDayLogFile != null && currentDayLogFile.exists())
            while (currentDayLogFile.canWrite()) {}

            markStatusNormal();
            return true;
        }

        @Override
        public void run() {
            LOGGER.debug("写日志工作者-开启工作模式");

            while (isStatusNormal() || recoverFSProblem()) {
                try {
                    SubMerchantAction subMerchantAction = getLog();
                    appendLine(subMerchantAction);
                } catch (Exception exception) {
                    LOGGER.error("写日志工作者 写日志时 发生错误", exception);
                }
            }
        }
    }
}
