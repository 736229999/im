package com.feiniu.imServer.consts;

/**
 * Created by wangzhen on 2016/3/15.
 */
public class BackendConfig {
    public static final String GET_REAL_ID="/rest/backOperator/id/";
    public static final String GET_REAL_NAME="/rest/backOperator/realname/";
    public static final String GET_USER_NAME="/rest/backOperator/username/";
    public static final String ORGAN_ID = "$organId";
    public static final String GET_ORGAN_URL = "/rest/organ/$organId/ids";
    public static final String HTTP_URL= "http://";
    public static final String GET_BACKOPERATERS_URL= "/rest/backOperator/getBackOperatersByOrganIds";

    public static final Integer OFF_LINE_STATUS =19;
    public static final Integer ORGAN_ID_TO_GROUP_ID =100;
    public static final String BACKEND_GET_OPERATOR_METHOD[]={
            GET_REAL_ID,GET_REAL_NAME,GET_USER_NAME
    };
}
