package com.cn.feiniu.imindex.web.controller;

import com.cn.feiniu.imindex.bean.ResultInfo;
import com.cn.feiniu.imindex.checkSvr.SystemEnv;
import com.cn.feiniu.imindex.config.DumpConfig;
import com.cn.feiniu.imindex.util.FormatUtil;
import org.apache.commons.io.FileUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.util.FileCopyUtils;
import org.springframework.web.bind.ServletRequestUtils;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.multipart.MultipartFile;

import javax.annotation.PostConstruct;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.*;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * Author: morningking
 * Date: 2016/2/5
 * Contact: 243717042@qq.com
 */
@Controller
@RequestMapping(value = "dump-tracker")
public class DumpUploadController {
    @Autowired
    private DumpConfig dumpConfig;

    private File imServerDumpDir;
    private List<String> imFileList = new ArrayList<>(1);

    private File pcMerchantDumpDir;
    private List<String> pcMerchantFileList = new ArrayList<>(1);

    private static final Logger LOGGER = LoggerFactory.getLogger(DumpUploadController.class);

    @PostConstruct
    private void init() {
        imServerDumpDir = new File(dumpConfig.getImServerDumpPath());
        pcMerchantDumpDir = new File(dumpConfig.getPcMerchantDumpPath());

        initFileList(imServerDumpDir, imFileList);
        initFileList(pcMerchantDumpDir, pcMerchantFileList);
    }

    private static void initFileList(File dir, List<String> fileList) {
        if (dir != null || dir.isDirectory()) {
            File[] fileArray = dir.listFiles();
            if (fileArray != null && fileArray.length > 0) {
                for (File file : fileArray) {
                    fileList.add(file.getName());
                }

                Collections.sort(fileList, Collections.reverseOrder());
            }
        }
    }

    @RequestMapping(value = "upload-for-server")
    @ResponseBody
    public ResultInfo uploadServerDump(@RequestParam(value = "file") MultipartFile multipartFile) {
        ResultInfo resultInfo = new ResultInfo();

        try {
            File dumpFile = new File(imServerDumpDir,
                    FormatUtil.formatChinaTime(new Timestamp(System.currentTimeMillis()))
                            + "_" + multipartFile.getOriginalFilename());
            FileUtils.copyInputStreamToFile(multipartFile.getInputStream(), dumpFile);

            resultInfo.markSuccess();
            imFileList.add(0, dumpFile.getName());
        } catch (IOException e) {
            LOGGER.error("写dump文件时发生错误", e);
        }

        return resultInfo;
    }

    @RequestMapping(value = "upload-for-pc-merchant")
    @ResponseBody
    public ResultInfo uploadServerPcMerchant(@RequestParam(value = "file") MultipartFile multipartFile) {
        ResultInfo resultInfo = new ResultInfo();

        try {
            File dumpFile = new File(pcMerchantDumpDir,
                    FormatUtil.formatChinaTime(new Timestamp(System.currentTimeMillis()))
                            + "_" + multipartFile.getOriginalFilename());
            FileUtils.copyInputStreamToFile(multipartFile.getInputStream(), dumpFile);

            resultInfo.markSuccess();
            pcMerchantFileList.add(0, dumpFile.getName());
        } catch (IOException e) {
            LOGGER.error("写dump文件时发生错误", e);
        }

        return resultInfo;
    }

    @RequestMapping(value = "dump-list-page")
    public String getDumpListPage(ModelMap modelMap) {
        modelMap.put("fileList", pagination("im-server", 1, 10));
        return "dump-list";
    }

    @RequestMapping(value = "download")
    public void downloadDumpFile(HttpServletRequest request, HttpServletResponse response) throws Exception {
        String fileName = ServletRequestUtils.getRequiredStringParameter(request, "fileName");
        String type = ServletRequestUtils.getRequiredStringParameter(request, "type");
        File dir;

        if ("im-server".equals(type)) {
            dir = imServerDumpDir;
        } else if ("pc-merchant".equals(type)) {
            dir = pcMerchantDumpDir;
        } else {
            return;
        }

        OutputStream outputStream = response.getOutputStream();

        try {
            response.reset();
            outputStream = new BufferedOutputStream(outputStream);
            FileCopyUtils.copy(new FileInputStream(new File(dir, fileName)), outputStream);

            String userAgent = request.getHeader("User-Agent");
            byte[] bytes = userAgent.contains("MSIE") ? fileName.getBytes() : fileName.getBytes("UTF-8");
            fileName = new String(bytes, "ISO-8859-1");

            response.setContentType("application/octet-stream");
            response.setHeader("Content-Disposition", String.format("attachment; filename=\"%s\"", fileName));
        } finally {
            if (outputStream != null) {
                outputStream.close();
            }
        }
    }

    @RequestMapping(value = "dump-list")
    @ResponseBody
    public ResultInfo getDumpList(HttpServletRequest request) {
        ResultInfo resultInfo = new ResultInfo();

        int pageNo = ServletRequestUtils.getIntParameter(request, "pageNo", 1);
        int pageSize = ServletRequestUtils.getIntParameter(request, "pageSize", 10);
        String type = ServletRequestUtils.getStringParameter(request, "type", "im-server");

        resultInfo.addData("dumpList", pagination(type, pageNo, pageSize));
        resultInfo.markSuccess();

        return resultInfo;
    }

    private List<String> pagination(String type, int pageNo, int pageSize) {
        List<String> fileList = Collections.emptyList();

        if ("im-server".equals(type)) {
            fileList = imFileList;
        } else if ("pc-merchant".equals(type)) {
            fileList = pcMerchantFileList;
        }

        pageNo = pageNo < 1 ? 1 : pageNo;
        pageSize = pageSize < 1 ? 10 : pageSize;

        int startIndex = (pageNo - 1) * pageSize;
        int endIndex = pageNo * pageSize;

        startIndex = startIndex > fileList.size() ? fileList.size() : startIndex;
        endIndex = endIndex > fileList.size() ? fileList.size() : endIndex;

        return fileList.subList(startIndex, endIndex);
    }
}
