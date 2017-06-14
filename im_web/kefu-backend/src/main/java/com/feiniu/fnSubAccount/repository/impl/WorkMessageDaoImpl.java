package com.feiniu.fnSubAccount.repository.impl;

import com.feiniu.fnSubAccount.bean.WorkMessageRequestParams;
import com.feiniu.fnSubAccount.bean.response.WorkMessageResponse;
import com.feiniu.fnSubAccount.bean.response.WorkMessageTotalInfoResponse;
import com.feiniu.fnSubAccount.consts.SystemConfig;
import com.feiniu.fnSubAccount.domain.MerchantLogInOutInfo;
import com.feiniu.fnSubAccount.repository.WorkMessageDao;
import com.feiniu.fnSubAccount.util.FormatUtils;
import com.feiniu.fnSubAccount.util.TransformService;
import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by wangchen on 2015/2/4.
 */

@Repository
public class WorkMessageDaoImpl implements WorkMessageDao {
    @Autowired
    private SessionFactory sessionFactory;

    private static final Logger LOGGER= LoggerFactory.getLogger(WorkMessageDaoImpl.class);

    private Query createQueryWithParams(String sql, Map<String, Object> params, int type) {
        Session session = sessionFactory.getCurrentSession();
        Query query;

        if (type == 0)
            query = session.createSQLQuery(sql);
        else
            query = session.createQuery(sql);

        for (Map.Entry<String, Object> entry : params.entrySet()) {
            if (entry.getValue() instanceof List) {
                query.setParameterList(entry.getKey(), (List) entry.getValue());
            } else {
                query.setParameter(entry.getKey(), entry.getValue());
            }
        }
        return query;
    }

    /*
    * 定时任务相关函数
    * */
    /**
     * 更新统计状态表中今天的统计状态为正在统计，实现互斥执行跑批任务
     */
    @Override
    public Map<String, Object> lockUpdateStatus(Long currentTime) {
        String sql;
        Query query;
        Map<String, Object> params = new HashMap<String, Object>();
        Integer status = SystemConfig.STATISITC_STATUS_NODATA;
        Integer retrytime = 0;

        params.put("startTime", currentTime);
        params.put("type", SystemConfig.STATISITC_TYPE_BACKEND);

        sql = "SELECT STATISITC_STATUS ,RETRY_TIME FROM " +
                "IM_STATISITC_STATUS WHERE " +
                "STATISITC_TIME = (:startTime) " +
                "AND STATISITC_TYPE=(:type)" +
                "FOR UPDATE ";

        try {
            query = createQueryWithParams(sql, params, 0);

            for (Object obj : query.list()) {
                if (obj == null) {
                    status = SystemConfig.STATISITC_STATUS_NODATA;
                    retrytime = 0;
                } else {
                    Object[] res = (Object[]) obj;
                    status = Integer.valueOf(res[0].toString());
                    retrytime = Integer.valueOf(res[1].toString());
                }
            }
        }catch (Exception e){
            LOGGER.error("锁定失败");
            return  null;
        }

        params.put("status", status);
        params.put("retrytime", retrytime);
        return params;
    }



    @Override
    public  int insertIntoStatus(Long currentTime,int statisitcStatus, String ipAddress){
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();

        sql = "INSERT INTO IM.IM_STATISITC_STATUS(" +
                "\"STATISITC_TIME\",\"STATISITC_TYPE\",\"STATISITC_STATUS\",\"RETRY_TIME\",\"TRY_IP\")" +
                "SELECT" +
                " " + String.valueOf(currentTime) + "," +
                " " + String.valueOf(SystemConfig.STATISITC_TYPE_BACKEND) + ", " +
                " " + String.valueOf(statisitcStatus) + ", " +
                " " + String.valueOf(0) + ", " +
                "'" + ipAddress + "'" +
                " from dual where not exists (select * from im.IM_STATISITC_STATUS where STATISITC_TYPE = " + SystemConfig.STATISITC_TYPE_BACKEND
                + " and STATISITC_TIME = " + String.valueOf(currentTime)+")"
        ;
        query = session.createSQLQuery(sql);
        return query.executeUpdate();
    }

    @Override
    public  int updateStatus(Long currentTime,int statisitcStatus, String ipAddress,String retrytime){
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();

        sql = "UPDATE  IM_STATISITC_STATUS SET " +
                "STATISITC_STATUS =" + String.valueOf(statisitcStatus) + ", " +
                " RETRY_TIME =" + String.valueOf(retrytime) + " ," +
                " TRY_IP = '" + ipAddress + "' " +
                " WHERE STATISITC_TIME =" + String.valueOf(currentTime) + " " +
                "AND STATISITC_TYPE =" + SystemConfig.STATISITC_TYPE_BACKEND + " ";

        query = session.createSQLQuery(sql);
        return  query.executeUpdate();
    }

    @Override
    public  int updateImWorkDetail(long merchantId,long statisticsDate,int landingDur){
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        //删除重复项
        sql = "DELETE FROM IM.IM_MERCHANT_LANDING_DETAIL " +
                "WHERE MERCHANT_ID=" + String.valueOf(merchantId) + " " +
                "AND STATISTICS_DATE=" + String.valueOf(statisticsDate) + " ";

        query = session.createSQLQuery(sql.toString());
        query.executeUpdate();
        //插入新数据
        sql = "INSERT INTO IM_MERCHANT_LANDING_DETAIL(MERCHANT_ID,STATISTICS_DATE,LANDING_TIME) VALUES(" +
                String.valueOf(merchantId) +","+
                String.valueOf(statisticsDate)+ ","+
                String.valueOf(landingDur)+")";

        query = session.createSQLQuery(sql.toString());
        query.executeUpdate();

        return 0;
    }

    /**
     * 插入数据到IM_RECEPTION_MAP表，
     */
    public  int insertLandingInfo(long merchantId,long statisticsDate,long subMerchantId){
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        //删除重复项
        sql = "DELETE FROM IM.IM_RECEPTION_MAP " +
                "WHERE I_MERCHANT_ID=" + String.valueOf(merchantId) + " " +
                "AND I_STATISITC_TIME=" + String.valueOf(statisticsDate) + " "+
                "AND I_SUBMERCHANT_ID=" + String.valueOf(subMerchantId) + " "+
                "AND I_COUSTOMER_ID = 0  ";

        query = session.createSQLQuery(sql.toString());
        query.executeUpdate();
        //插入新数据
        sql = "INSERT INTO IM.IM_RECEPTION_MAP(I_SUBMERCHANT_ID,I_MERCHANT_ID,I_STATISITC_TIME,I_COUSTOMER_ID,I_REPLY,I_NOT_REPLY)" +
                "  VALUES(" +
                String.valueOf(subMerchantId) +","+
                String.valueOf(merchantId)+ ","+
                String.valueOf(statisticsDate)+","+
                "0,"+
                "0,"+
                "0"+
                ")";

        query = session.createSQLQuery(sql.toString());
        query.executeUpdate();

        return 0;
    }

    @Override
    public  void updateLoginMap(Map<Long,Long> subMerchantIdToMerchantIdMap,long startTime,long endTime){
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        Map<String, Object> params = new HashMap<String, Object>();

        params.put("startTime", startTime);
        params.put("endTime", endTime);

        sql = "SELECT DISTINCT IM.IM_LOGINOUTTIME.USERID AS dID " +
                " FROM IM.IM_LOGINOUTTIME " +
                " WHERE  ((IM.IM_LOGINOUTTIME.LOGOUTTIME = NULL) OR " +
                " (IM.IM_LOGINOUTTIME.LOGOUTTIME = 0) OR " +
                " ( " + "   IM.IM_LOGINOUTTIME.LOGOUTTIME > " +String.valueOf(startTime)+")" +
                " ) AND IM.IM_LOGINOUTTIME.LOGINTIME < " +String.valueOf(endTime) +
                " ORDER BY IM.IM_LOGINOUTTIME.USERID ";

        query = session.createSQLQuery(sql);

        for (Object obj : query.list()) {
            if (obj == null)
                return ;
            Long subMerchantId = ((BigDecimal)obj).longValue();
            Long merchantId = subMerchantIdToMerchantIdMap.get(subMerchantId);
            if (merchantId == null)
                merchantId = Long.valueOf(0);
            insertLandingInfo(merchantId,startTime,subMerchantId);
        }
    }

    @Override
    public Integer getMerchantLogInOutCount(long startTime, long endTime, List<Long> listUserId) {
        Map<String, Object> params = new HashMap<>(1);
        String sql = "SELECT COUNT(*) from (" +
                " SELECT ROWNUM AS rowNumber ,pc.* FROM (" +
                " SELECT PA.USERID AS USERID,PB.USERNAME AS username,pa.LOGINOUTTIME AS logInOutTime ,PA.LOGINTYPE AS logInOutType FROM ( " +
                " (SELECT " +
                " USERID,LOGINTIME AS logInOuttime,0 AS loginType " +
                " FROM " +
                " IM.IM_LOGINOUTTIME " +
                " WHERE " +
                " USERID IN (:useridList) " +
                " ) " +
                " UNION  " +
                " ( " +
                " SELECT " +
                " USERID,LOGOUTTIME AS logInOuttime,1 AS loginType " +
                " FROM " +
                " IM.IM_LOGINOUTTIME " +
                " WHERE " +
                " USERID IN (:useridList)  " +
                " AND LOGOUTTIME <>0 " +
                " ) ) PA " +
                " LEFT JOIN ( " +
                " SELECT USERID,USERNAME FROM IM.IM_USER " +
                " ) PB  ON PA.USERID = PB.USERID " +
                " WHERE PA.logInOuttime > :startTime AND PA.logInOuttime < :endTime " +
                " ORDER BY PA.logInOuttime DESC) pc" +
                " )pd ";

        List resultList= sessionFactory.getCurrentSession().createSQLQuery(sql)
                .setParameterList("useridList",listUserId)
                .setParameter("startTime",startTime)
                .setParameter("endTime",endTime)
                .list();

        if(resultList == null)
            return 0;
        else
            return ((BigDecimal)resultList.get(0)).intValue();
    }

    @Override
    public List<MerchantLogInOutInfo> getMerchantLogInOut(long startTime,long endTime ,int pageNo, int pageSize,
                                                          List<Long> listUserId) {

        Map<String, Object> params = new HashMap<>(1);
        String sql = "SELECT * from (" +
                " SELECT ROWNUM AS rowNumber ,pc.* FROM (" +
                " SELECT PA.USERID AS USERID,PB.USERNAME AS username,pa.LOGINOUTTIME AS logInOutTime ,PA.LOGINTYPE AS logInOutType FROM ( " +
                " (SELECT " +
                " USERID,LOGINTIME AS logInOuttime,0 AS loginType " +
                " FROM " +
                " IM.IM_LOGINOUTTIME " +
                " WHERE " +
                " USERID IN (:useridList) " +
                " ) " +
                " UNION  " +
                " ( " +
                " SELECT " +
                " USERID,LOGOUTTIME AS logInOuttime,1 AS loginType " +
                " FROM " +
                " IM.IM_LOGINOUTTIME " +
                " WHERE " +
                " USERID IN (:useridList)  " +
                " AND LOGOUTTIME <>0 " +
                " ) ) PA " +
                " LEFT JOIN ( " +
                " SELECT USERID,USERNAME FROM IM.IM_USER " +
                " ) PB  ON PA.USERID = PB.USERID " +
                " WHERE PA.logInOuttime > :startTime AND PA.logInOuttime < :endTime " +
                " ORDER BY PA.logInOuttime ) pc" +
                " )pd WHERE pd.rowNumber > :startRow and pd.rowNumber <= :endRow";

        params.put("useridList",listUserId);
        params.put("startTime",startTime);
        params.put("endTime",endTime);
        params.put("startRow",(pageNo -1)* pageSize);
        params.put("endRow",pageNo * pageSize);
        return TransformService.transformResult(
                sessionFactory.getCurrentSession().createSQLQuery(sql),
                params, MerchantLogInOutInfo.class);
    }

    /*
    * 数据查询相关函数
    * */
    /**
     * 获取汇总信息的函数
     */
    @Override
    public WorkMessageTotalInfoResponse getWorkTotalResponse(WorkMessageRequestParams workMessageRequestParams){
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        WorkMessageTotalInfoResponse result= new WorkMessageTotalInfoResponse();

        sql ="  SELECT SUM(landing_time) AS SUM_landing_time," +
                " SUM(ASKCOUNT) AS SUM_asking_count, " +
                " SUM(replyCount) AS SUM_reply_count, " +
                " AVG(landing_time) AS AVG_landing_time, " +
                " AVG(ASKCOUNT) AS AVG_asking_count, " +
                " AVG(replyCount) AS AVG_reply_count, " +
                " AVG(RESPONSE_DUR) AS AVG_RESPONSE_TIME, " +
                " AVG(RATE) AS AVG_RATE ," +
                " SUM(1) AS SUM_COUNT," +
                " SUM(customerCount) AS SUM_ONLINE_CUSTOMER_COUNT ," +
                " AVG(customerCount) AS AVG_ONLINE_CUSTOMER_COUNT, " +
                " SUM(askingTimes) AS SUM_ASKING_TIMES," +
                " SUM(replyTimes) AS SUM_REPLY_TIMES," +
                " AVG(askingTimes) AS AVG_ASKING_TIMES," +
                " AVG(replyTimes) AS AVG_REPLY_TIMES" +
                " FROM( "+ FormatUtils.getWorkMessageSql(workMessageRequestParams,false,0) +
                " ) ";

        query = session.createSQLQuery(sql);

        for (Object obj : query.list()) {
            if (obj == null) {
                return result;
            } else {
                Object[] res = (Object[]) obj;
                if (res[0] == null){
                    return result;
                }
                result.setSumLandingDuration(((BigDecimal) res[0]).intValue());
                result.setSumAskingCount(((BigDecimal) res[1]).intValue());
                result.setSumReplyCount(((BigDecimal) res[2]).intValue());
                result.setAvgLandingDuration(((BigDecimal) res[3]).intValue());
                result.setAvgAskingCount(((BigDecimal) res[4]).intValue());
                result.setAvgReplyCount(((BigDecimal) res[5]).intValue());

                if (res[6] == null){
                    result.setAvgResponseDuration(-1);
                }else {
                    result.setAvgResponseDuration(((BigDecimal) res[6]).floatValue());
                }

                if (res[7] == null){
                    result.setAvgRate(-1);
                }else {
                    result.setAvgRate(((BigDecimal) res[7]).floatValue());
                }

                result.setTotalCount(((BigDecimal) res[8]).intValue() + 2 );
                result.setSumOnlineCustomerCount(((BigDecimal) res[9]).intValue());
                result.setAvgOnlineCustomerCount(((BigDecimal) res[10]).floatValue());
                result.setSumAskingTimes(((BigDecimal) res[11]).intValue());
                result.setSumReplyTimes(((BigDecimal) res[12]).intValue());
                result.setAvgAskingTimes(((BigDecimal) res[13]).floatValue());
                result.setAvgReplyTimes(((BigDecimal) res[14]).floatValue());
            }
        }

        return result;
    }

    /**
     * 获取商家工作情况的列表
     */
    @Override
    public List<WorkMessageResponse> getWorkMessageResponseList(WorkMessageRequestParams workMessageRequestParams,long isNeedmerchantId){
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        List<WorkMessageResponse> resultList = new ArrayList<>();

        sql = FormatUtils.getWorkMessageSql(workMessageRequestParams,true,isNeedmerchantId);

        query = session.createSQLQuery(sql);

        for (Object obj : query.list()) {
            if (obj == null) {
                return resultList;
            } else {
                Object[] res = (Object[]) obj;
                WorkMessageResponse workMessageResponse = new WorkMessageResponse();
                workMessageResponse.setMerchantID(((BigDecimal) res[0]).longValue());
                if (res[1] == null){
                    workMessageResponse.setSqlAvgResponseDuration(-1);
                }else {
                    workMessageResponse.setSqlAvgResponseDuration(((BigDecimal) res[1]).floatValue());
                }

                workMessageResponse.setSqlSumLandingTime(((BigDecimal) res[2]).intValue());

                if (res[3] == null){
                    workMessageResponse.setSqlResponseIn90sRate(-1);
                }else {
                    workMessageResponse.setSqlResponseIn90sRate(((BigDecimal) res[3]).floatValue());
                }
                workMessageResponse.setSqlSubMerchantOnlineCount(((BigDecimal) res[4]).intValue());
                workMessageResponse.setSqlAskingCount(((BigDecimal) res[5]).intValue());
                workMessageResponse.setSqlReplyCount(((BigDecimal) res[6]).intValue());
                workMessageResponse.setSqlAskingTimes(((BigDecimal) res[7]).intValue());
                workMessageResponse.setSqlReplyTimes(((BigDecimal) res[8]).intValue());
                resultList.add(workMessageResponse);
            }
        }

        return resultList;
    }

    /**
     * 获取商家客服的在线时间，因为需要去重，单独写成一个函数
     */
    @Override
    public  int getMerchantOnlineDur(String subMerchantIds,long startTime,long endTime){
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        Map<String, Object> params = new HashMap<String, Object>();

        params.put("TOID", subMerchantIds);
        params.put("startTime", startTime);
        params.put("endTime", endTime);

        //获取所有客服登陆的信息，进行统计，算出登陆时长
        sql = "SELECT IM.IM_LOGINOUTTIME.USERID AS dID," +
                "(CASE WHEN LOGOUTTIME=NULL OR LOGOUTTIME=0 OR LOGOUTTIME > "+ String.valueOf(endTime)+
                " THEN  " +String.valueOf(endTime)+
                " ELSE LOGOUTTIME END) AS logout," +
                " (CASE WHEN LOGINTIME <  "  +String.valueOf(startTime)+
                " THEN  " + String.valueOf(startTime)+
                " ELSE LOGINTIME END) AS login " +
                " FROM IM.IM_LOGINOUTTIME " +
                " WHERE IM.IM_LOGINOUTTIME.USERID IN ( " + subMerchantIds + ") " +
                " AND ((IM.IM_LOGINOUTTIME.LOGOUTTIME = NULL) OR " +
                " (IM.IM_LOGINOUTTIME.LOGOUTTIME = 0) OR " +
                " ( " + "   IM.IM_LOGINOUTTIME.LOGOUTTIME > " +String.valueOf(startTime)+")" +
                " ) AND IM.IM_LOGINOUTTIME.LOGINTIME < " +String.valueOf(endTime) +
                " ORDER BY IM.IM_LOGINOUTTIME.USERID ";

        query = session.createSQLQuery(sql);

        //着色器，间隔10秒钟，一天一共有24*60*6个间隔
        byte CorlorMap[] = new byte[SystemConfig.STATISITC_ONE_DAY_TEN_SECOUNDS];

        for (Object obj : query.list()) {
            Object[] res = (Object[]) obj;
            if (res[0] == null)
                return -1;
            long logoutTime = ((BigDecimal) res[1]).longValue();
            long loginTime = ((BigDecimal) res[2]).longValue();;
            int logintime = (int)(loginTime - startTime) / 10;
            int logouttime = (int)(logoutTime - startTime) / 10;

            while (logintime < logouttime)
                CorlorMap[logintime++] = 1;
        }
        int totalDur = 0;
        for (int i = 0; i < CorlorMap.length; i++)
            totalDur += CorlorMap[i];

        return totalDur*10;
    }
}
