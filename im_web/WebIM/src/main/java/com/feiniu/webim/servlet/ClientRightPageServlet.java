package com.feiniu.webim.servlet;

import com.feiniu.webim.controller.ClientRightPageController;
import com.feiniu.webim.core.ResponseContentGenerator;
import com.feiniu.webim.core.Session;
import com.feiniu.webim.core.WebAction;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;

/**
 * Created by zhangmin on 2016/1/8.
 */

@WebServlet("/crp")
public class ClientRightPageServlet extends HttpServlet {
    ClientRightPageController clientRightPageController = ClientRightPageController.instance;
    Session NIL_SESSION = null;

    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        process(req, resp);
    }

    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        process(req, resp);
    }

    private void process(HttpServletRequest request, HttpServletResponse response) {
        WebAction webAction = new WebAction();
        webAction.setRequest(request);
        webAction.setResponse(response);
        webAction.setResponseContentGenerator(new ResponseContentGenerator(response));
        webAction.setSession(NIL_SESSION);

        clientRightPageController.handleRequest(webAction);
    }
}
