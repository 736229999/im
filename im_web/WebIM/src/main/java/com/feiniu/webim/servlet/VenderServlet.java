package com.feiniu.webim.servlet;

import com.feiniu.webim.controller.VenderController;
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
 * Created by wangchen on 2015/1/13.
 */
@WebServlet("/vender")
public class VenderServlet extends HttpServlet {
    VenderController venderController = VenderController.instance;
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

        venderController.handleRequest(webAction);
    }
}
