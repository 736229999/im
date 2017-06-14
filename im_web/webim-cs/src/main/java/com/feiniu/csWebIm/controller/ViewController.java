package com.feiniu.csWebIm.controller;

import com.feiniu.csWebIm.bean.domain.EmployeeInfo;
import com.feiniu.csWebIm.bean.request.ServiceLogRequset;
import com.feiniu.csWebIm.bean.response.WebRightResponse;
import com.feiniu.csWebIm.repository.CsService;
import com.feiniu.csWebIm.service.WebRightService;
import com.feiniu.csWebIm.service.external.EmployeeService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.ServletRequestUtils;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

/**
 * Created by wangzhen on 2015/10/20.
 */
@Controller
public class ViewController {

    @Autowired
    private EmployeeService employeeService;//哞哞客服对象声明

    @Autowired
    private WebRightService webRightService;//WEB人工服务台

    @Autowired
    private CsService csService;

    private static final Logger LOGGER = LoggerFactory.getLogger(ViewController.class);

    @RequestMapping(value = "checksrv")
    public String getCheckSrv(ModelMap modelMap) {

        return "checkSrv/checkSrv";
    }

    //服务小记页面，客服对客户时，客服显示的右侧页
    @RequestMapping(value = "webRight/service-log")
    public ModelAndView getCsServiceLog(HttpServletRequest request) {
        ModelAndView modelAndView = new ModelAndView("servicelog/service-log");

        ServiceLogRequset serviceLogRequset = (ServiceLogRequset)request.getAttribute("serviceLogRequset");

        if (serviceLogRequset.getCustomerId() == null || serviceLogRequset.getCustomerServiceId() == null
                 || serviceLogRequset.getGoodsAskingType() == null) {

            return null;
        }

        String moumouId = serviceLogRequset.getCustomerServiceId();
        String realName = csService.getTureNameByCsMoumouId(moumouId);
        serviceLogRequset.setCustomerServiceName(realName);

        String customerToken = ServletRequestUtils.getStringParameter(request, "customerToken", "");

        //将id用token和orderid用token代替,删除ID
        serviceLogRequset.setCustomerId(customerToken);
        serviceLogRequset.setCustomerServiceId("");

        modelAndView.addObject("serviceLogRequest", serviceLogRequset);

        return modelAndView;
    }

    //客服对客服右侧页
    @RequestMapping(value = "serviceProfile/right_profile")//客服对客服右侧页
    public ModelAndView getServiceProfile(HttpServletRequest request) {

        ModelAndView modelAndView = new ModelAndView("serviceProfile/right_profile");

        String serviceId = ServletRequestUtils.getStringParameter(request, "serviceId",null);

        if (serviceId == null) {
            LOGGER.error("没有参数 serviceId");
            return null;
        }
        String csId = csService.getCsIdByCsMoumouId(serviceId);

        EmployeeInfo employee = new EmployeeInfo();
        if (csId == null) {
            employee.setEmployeeName("--");
            employee.setEmailInfo("--");
            employee.setCellPhone("--");
            employee.setDepartmentInfo("--");
        }
        else
        {
            employee = employeeService.getEmployInfoById(csId);
            if(employee == null)
            {
                employee.setEmployeeName("--");
                employee.setEmailInfo("--");
                employee.setCellPhone("--");
                employee.setDepartmentInfo("--");
            }
        }

        modelAndView.addObject("employee", employee);

        return modelAndView;
    }

    //web右侧页，客户显示的右侧页
    @RequestMapping(value = "buyerRightPage/right")//WEB人工客服
    public ModelAndView getWebRight(HttpServletRequest request) {

        ModelAndView modelAndView = new ModelAndView("webRight/right");

        String orderNo = ServletRequestUtils.getStringParameter(request, "orderNo", "");
        String askingType = ServletRequestUtils.getStringParameter(request, "goodsAskingType", "");

        HttpSession session = request.getSession();
        Object customerId = session.getAttribute("customerId");

        if ( customerId== null || askingType=="" ){
            return  null;
        }
        if (askingType.equals("1") && orderNo ==""){
            return  null;
        }

        WebRightResponse webRightResponse = webRightService.getPageDetail(customerId.toString(),askingType,orderNo);

        modelAndView.addObject("webRightResponse", webRightResponse);
        return modelAndView;
    }

}