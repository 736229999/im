package com.feiniu.imServer.repository.impl;

import com.feiniu.imServer.bean.CSInfo;
import com.feiniu.imServer.repository.CsInfoDao;
import oracle.jdbc.OracleTypes;
import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.jdbc.ReturningWork;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import java.math.BigDecimal;
import java.sql.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by wangzhen on 2016/1/6.
 */
@SuppressWarnings({" unused", " unchecked", " rawtypes"})
@Repository
@Transactional
public class CsInfoDaoImpl implements CsInfoDao {
    @Autowired
    private SessionFactory sessionFactory;
    private static final Logger LOGGER = LoggerFactory.getLogger(CsInfoDaoImpl.class);

    @Override
    public List<String> getAllCsIdInGroup(String groupid) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        List<String> moumouIdList = new ArrayList<>();
        sql = "SELECT DISTINCT USERID FROM IM.IM_CSGROUPMEM WHERE 1=1";

        if (groupid != null && groupid.length() != 0)
            sql = sql + " and GROUPID = " + groupid;

        query = session.createSQLQuery(sql);

        for (Object obj : query.list()) {
            if (obj == null) {
                return moumouIdList;
            } else {
                moumouIdList.add(obj.toString());
            }
        }
        return moumouIdList;
    }

    @Override
    public Map<Long,Integer> getAllUserIdToCsIdMap() {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        Map<Long,Integer> resultMap = new HashMap<>();
        sql = "SELECT CSID,USERID FROM  IM.IM_CSINFO WHERE  CSID IS NOT NULL ";

        query = session.createSQLQuery(sql);

        for (Object obj : query.list()) {
            if (obj == null) {
                return resultMap;
            } else {
                Object[] res = (Object[]) obj;
                resultMap.put(((BigDecimal) res[1]).longValue(),
                        ((BigDecimal) res[0]).intValue());
            }
        }
        return resultMap;
    }

    @Override
    public List<String> getAllGroups() {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        List<String> groupIdList = new ArrayList<>();
        sql = "SELECT DISTINCT GROUPID FROM IM.IM_CSGROUP WHERE 1=1";

        query = session.createSQLQuery(sql);

        for (Object obj : query.list()) {
            if (obj == null) {
                return groupIdList;
            } else {
                groupIdList.add(obj.toString());
            }
        }
        return groupIdList;
    }

    @Override
    public List<Long> getAllGroupsOfCsId(String csId) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        List<Long> groupIdList = new ArrayList<>();
        sql = "SELECT DISTINCT groupid FROM IM.IM_CSGROUPMEM WHERE USERID = " +csId;

        query = session.createSQLQuery(sql);

        for (Object obj : query.list()) {
            if (obj == null) {
                return groupIdList;
            } else {
                groupIdList.add(((BigDecimal)obj).longValue());
            }
        }
        return groupIdList;
    }

    @Override
    public Integer getReceptionLimit(String csid) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        List<String> groupIdList = new ArrayList<>();
        sql = "SELECT RECEPTIONLIMIT FROM IM.IM_CSINFO WHERE USERID="+ csid;
        BigDecimal result= (BigDecimal) session.createSQLQuery(sql).list().get(0);
        return result.intValue();
    }

    @Override
    public CSInfo getCSInfoById(final  Long userId) {
        final CSInfo csInfo = new CSInfo();
        Integer returnCode = sessionFactory.getCurrentSession().doReturningWork(new ReturningWork<Integer>() {
            @Override
            public Integer execute(Connection connection) throws SQLException {
                CallableStatement callableStatement = connection.prepareCall(
                        "{call Mini_User_Pkg.GetCSInfo(?,?,?,?,?,?)}"
                );
                callableStatement.setLong(1, userId);
                callableStatement.registerOutParameter(2, Types.VARCHAR);
                callableStatement.registerOutParameter(3,Types.BIGINT);
                callableStatement.registerOutParameter(4,Types.BIGINT);
                callableStatement.registerOutParameter(5, Types.BIGINT);
                callableStatement.registerOutParameter(6, Types.BIGINT);

                callableStatement.execute();
                csInfo.setUserId(userId);
                csInfo.setUserName(callableStatement.getString(2));
                csInfo.setStatus(callableStatement.getLong(3));
                csInfo.setReceptionLimit(callableStatement.getLong(4));
                csInfo.setPermission(callableStatement.getLong(5));
                Integer returnCode = callableStatement.getInt(6);
                callableStatement.close();

                return returnCode;
            }
        });

        if (returnCode == -1) {
            LOGGER.error("调用存储过程存储Mini_User_Pkg.GetCSInfo过程失败：" +userId.toString());
        }
        return csInfo;
    }

    @Override
    public List<Long> getCSGroupCountList(final Long groupId) {

        final List<Long> ids = new ArrayList<Long>();

        Integer returnCode = sessionFactory.getCurrentSession().doReturningWork(new ReturningWork<Integer>() {
            @Override
            public Integer execute(Connection connection) throws SQLException {
                CallableStatement callableStatement = connection.prepareCall(
                        "{call Mini_User_Pkg.GetCSGroupMemFlow(?,?,?)}"
                );
                callableStatement.setLong(1, groupId);
                callableStatement.registerOutParameter(2, OracleTypes.CURSOR);
                callableStatement.registerOutParameter(3, Types.BIGINT);

                callableStatement.execute();

                ResultSet rs = (ResultSet)callableStatement.getObject(2);
                //游标
                while(rs.next())
                {
                    LOGGER.debug("游标：" + rs.getString(1) + "  ");
                    ids.add(rs.getLong(1));
                }
                Integer returnCode = callableStatement.getInt(3);
                callableStatement.close();

                return returnCode;
            }
        });

        if (returnCode == -1) {
            LOGGER.error("调用存储过程存储Mini_User_Pkg.GetCSGroupMemFlow过程失败：" +groupId.toString());
        }
        return ids;
    }
}
