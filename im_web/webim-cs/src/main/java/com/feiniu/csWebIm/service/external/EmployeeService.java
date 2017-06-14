package com.feiniu.csWebIm.service.external;
import com.feiniu.csWebIm.bean.domain.BaseEmployeeInfo;
import com.feiniu.csWebIm.bean.domain.EmployeeInfo;
import com.feiniu.csWebIm.consts.BusinessConfig;
import com.feiniu.csWebIm.consts.Constants;
import com.feiniu.csWebIm.consts.SystemConfig;
import com.feiniu.csWebIm.utils.json.JsonNodeUtil;
import com.feiniu.csWebIm.utils.json.JsonUtil;
import com.feiniu.csWebIm.utils.web.HttpUtils;
import org.codehaus.jackson.JsonNode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.HashMap;
import java.util.Map;

/**
 * Author: chenwuming
 * Date: 2015/10/30
 */

@Service
public class EmployeeService {

    private static final String SUCCSS_CODE = "200";
    private static final Logger LOGGER = LoggerFactory.getLogger(EmployeeService.class);

    @Autowired
    private BusinessConfig businessConfig;

    public EmployeeInfo getEmployInfoById(String userId)
    {
        String serviceUrl = businessConfig.getEmployInfoURL();
        String method = SystemConfig.EMPLOYEE_INFO;
        String response = Constants.NIL_RESPONSE;
        String data = userId;
        method = method + data;
        BaseEmployeeInfo userInfo;
        EmployeeInfo employee;
        Map<String, Object> map = new HashMap<String, Object>();
        try
        {
            response = HttpUtils.executeGet(serviceUrl, method, map);//GET请求
            JsonNode jsonNode = JsonUtil.toNode(response);
            if (jsonNode != null && SUCCSS_CODE.equals(JsonNodeUtil.getString(jsonNode, "code")) && jsonNode.has("data")) {
                userInfo = JsonNodeUtil.injectBean(JsonNodeUtil.getString(jsonNode, "data"), BaseEmployeeInfo.class);
                employee = getEmployInfo(userInfo);
            }
            else
            {
                LOGGER.error("查询当前客服信息失败，数据为空" );
                return  null;
            }
        }
        catch (Exception e) {
            LOGGER.error("查询当前客服信息失败", e);
            return  null;
        }
        return employee;//返回处理后的结构
    }

    public EmployeeInfo getEmployInfo(BaseEmployeeInfo employeeInfp) {
        EmployeeInfo employInfo = new EmployeeInfo();
        ////员工真实姓名
        if (employeeInfp.getOperatorrealname() != null&& !employeeInfp.getOperatorrealname().equals("")) {
            employInfo.setEmployeeName(employeeInfp.getOperatorrealname());
        }
        else
        {
            employInfo.setEmployeeName("--");
        }

        ////员工邮箱
        if (employeeInfp.getEmail() != null&&!employeeInfp.getEmail().equals("")) {
            employInfo.setEmailInfo(employeeInfp.getEmail());
        }
        else
        {
            employInfo.setEmailInfo("--");
        }
        ////电话
        if (employeeInfp.getMobilenumber() != null && !employeeInfp.getMobilenumber().equals("")) {
            employInfo.setCellPhone(employeeInfp.getMobilenumber());
        }
        else
        {
            employInfo.setCellPhone("--");
        }
        ////员工部门
        if (employeeInfp.getOrganName() != null && !employeeInfp.getOrganName().equals("")) {
            employInfo.setDepartmentInfo(employeeInfp.getOrganName());
        }
        else
        {
            employInfo.setDepartmentInfo("--");
        }
        return employInfo;//返回处理后的结构
    }
}
