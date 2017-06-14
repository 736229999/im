package com.feiniu.webim.servlet;

import com.feiniu.webim.bean.domain.ResultInfo;
import com.feiniu.webim.bean.helper.KVPair;
import com.feiniu.webim.component.ImageService;
import com.feiniu.webim.config.RequestParams;
import com.feiniu.webim.core.ResponseContentGenerator;
import com.feiniu.webim.core.Session;
import com.feiniu.webim.core.SessionManager;
import com.feiniu.webim.core.TokenManager;
import com.feiniu.webim.util.web.ServletRequestUtil;
import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.fileupload.servlet.ServletFileUpload;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

/**
 * Author: wangchen
 * Date: 2015/6/29
 * Contact: 243717042@qq.com
 */
@WebServlet("/upload-photo.do")
public class ImageServlet extends HttpServlet {
    private static final Logger LOGGER = Logger.getLogger(ImageServlet.class);
    ImageService imageService = ImageService.IMAGE_SERVICE;
    private static final ResultInfo ILLEGAL_ACCESS_RESULT = new ResultInfo();

    @Override
    public void init() throws ServletException {
        ILLEGAL_ACCESS_RESULT.setErrorMessage("非法访问");
        super.init();
    }

    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        super.doPost(req, resp);
    }

    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        uploadPhoto(req, resp);
    }

    @SuppressWarnings("unchecked")
    private void uploadPhotoOld(HttpServletRequest req, HttpServletResponse resp) {
        ResponseContentGenerator responseContentGenerator = new ResponseContentGenerator(resp);
        responseContentGenerator.start();

        Integer userID = null;
        String callback = req.getParameter(RequestParams.CALLBACK.toString());
        String token = TokenManager.parseToken(req);
        Session session;
        DiskFileItemFactory factory = new DiskFileItemFactory();

        // 创建一个ServletFileUpload对象
        ServletFileUpload uploader = new ServletFileUpload(factory);
        try {
            ArrayList<FileItem> list = (ArrayList<FileItem>) uploader.parseRequest(req);
            Map<String, Object> params = new HashMap<>();
            KVPair<String, InputStream> filePartPair = new KVPair<>();

            params.put("imageName", "text");
            params.put("uploadType", 3);

            for (FileItem fileItem : list) {
                if (!fileItem.isFormField()) {      // 如果是文件form field
                    String fileName = fileItem.getName();

                    params.put(fileItem.getFieldName(), fileName);
                    params.put("extName", fileName.substring(fileName.lastIndexOf(".") + 1));

                    filePartPair.setKey(fileItem.getFieldName());
                    filePartPair.setValue(fileItem.getInputStream());
                } else {
                    String fieldName = fileItem.getFieldName();
                    if ("userID".equals(fieldName) && StringUtils.isNumeric(fileItem.getString())) {
                        userID = Integer.parseInt(fileItem.getString());
                    }
                }
            }

            LOGGER.info("req-url:" + req.getRequestURL());
            boolean isFromIM = ServletRequestUtil.getInteger(req, "isFromIM", 0) > 0;
            if (!isFromIM && (userID == null
                    || (session = SessionManager.getInstance().getSessionByID(userID + "")) == null
                    || !session.isTokenLegal(token))) {
                responseContentGenerator.printClientError(ILLEGAL_ACCESS_RESULT, callback);
            } else {
                ResultInfo resultInfo = new ResultInfo();
                resultInfo.markSuccess();
                resultInfo.addData("imageURL", imageService.uploadPhotoOld(params, filePartPair));

                responseContentGenerator.writeObject(resultInfo);
            }
        } catch (Exception e) {
            LOGGER.error(e);
            responseContentGenerator.printServerError(null);
        }

        responseContentGenerator.close();
    }

    private void uploadPhoto(HttpServletRequest req, HttpServletResponse resp) {
        ResponseContentGenerator responseContentGenerator = new ResponseContentGenerator(resp);
        responseContentGenerator.start();

        Integer userID = null;
        String callback = req.getParameter(RequestParams.CALLBACK.toString());
        String token = TokenManager.parseToken(req);
        Session session;
        DiskFileItemFactory factory = new DiskFileItemFactory();

        // 创建一个ServletFileUpload对象
        ServletFileUpload uploader = new ServletFileUpload(factory);
        try {
            ArrayList<FileItem> list = (ArrayList<FileItem>) uploader.parseRequest(req);
            String fileName = "";
            InputStream inputStream = null;

            for (FileItem fileItem : list) {
                if (!fileItem.isFormField()) {      // 如果是文件form field
                    fileName = fileItem.getName();
                    inputStream = fileItem.getInputStream();
                } else {
                    String fieldName = fileItem.getFieldName();
                    if ("userID".equals(fieldName) && StringUtils.isNumeric(fileItem.getString())) {
                        userID = Integer.parseInt(fileItem.getString());
                    }
                }
            }

            boolean isFromIM = ServletRequestUtil.getInteger(req, "isFromIM", 0) > 0;

            if (!isFromIM && (userID == null
                    || (session = SessionManager.getInstance().getSessionByID(userID + "")) == null
                    || !session.isTokenLegal(token))) {
                responseContentGenerator.printClientError(ILLEGAL_ACCESS_RESULT, callback);
            } else {
                ResultInfo resultInfo = new ResultInfo();
                resultInfo.markSuccess();
                resultInfo.addData("imageURL", imageService.uploadPhoto(fileName, inputStream));

                responseContentGenerator.writeObject(resultInfo);
            }
        } catch (Exception e) {
            LOGGER.error(e);
            responseContentGenerator.printServerError(null);
        }

        responseContentGenerator.close();
    }
}
