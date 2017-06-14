package com.feiniu.webim.servlet;


import com.feiniu.webim.bean.domain.ResultInfo;
import com.feiniu.webim.bean.helper.KVPair;
import com.feiniu.webim.component.ImageService;
import com.feiniu.webim.core.ResponseContentGenerator;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;

/**
 * Servlet implementation class uploadImageSvlt
 */
@WebServlet("/uploadImageSvlt")
public class UploadImageServlet extends HttpServlet {
    private static final long serialVersionUID = 1L;

    /**
     * @see HttpServlet#HttpServlet()
     */
    public UploadImageServlet() {
        super();
        // TODO Auto-generated constructor stub
    }

    /**
     * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
     */
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        uploadPhoto(request, response);
    }

    private void uploadPhotoOld(HttpServletRequest request, HttpServletResponse response) {
        try {
            ImageService imageService = ImageService.IMAGE_SERVICE;
            KVPair<String, InputStream> filePartPair = new KVPair<>();
            Map<String, Object> params = new HashMap<>();
            ResponseContentGenerator responseContentGenerator = new ResponseContentGenerator(response);
            responseContentGenerator.start();

            //set params for update pic
            params.put("imageName", "text");
            params.put("uploadType", 3);
            params.put("img", "null.png");
            params.put("extName", "png");

            BufferedInputStream fileIn = new BufferedInputStream(request.getInputStream());

            filePartPair.setKey("img");
            filePartPair.setValue(fileIn);

            ResultInfo resultInfo = new ResultInfo();

            //set the result url to web
            resultInfo.markSuccess();
            resultInfo.addData("imageURL", imageService.uploadPhotoOld(params, filePartPair));
            responseContentGenerator.writeObject(resultInfo);

            fileIn.close();

        } catch(Exception e){
            System.out.println(e.toString());
        }
    }

    private void uploadPhoto(HttpServletRequest request, HttpServletResponse response) {
        try {
            ImageService imageService = ImageService.IMAGE_SERVICE;
            ResponseContentGenerator responseContentGenerator = new ResponseContentGenerator(response);
            responseContentGenerator.start();

            ResultInfo resultInfo = new ResultInfo();

            resultInfo.addData("imageURL", imageService.uploadPhoto("random.png", request.getInputStream()));
            resultInfo.markSuccess();

            responseContentGenerator.writeObject(resultInfo);
        } catch (Exception e) {
            System.out.println(e.toString());
        }
    }
}
