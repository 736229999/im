package com.feiniu.fnSubAccount.repository.impl;

import com.feiniu.fnSubAccount.bean.CSAccountInfo;
import com.feiniu.fnSubAccount.bean.CSAccountResponse;
import com.feiniu.fnSubAccount.bean.KVPair;
import com.feiniu.fnSubAccount.repository.CSAccountDao;
import com.feiniu.fnSubAccount.util.MD5;
import com.feiniu.fnSubAccount.util.TransformService;
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
import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Types;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

/**
 * Created by wangzhen on 2015/11/19.
 */
@SuppressWarnings({" unused", " unchecked", " rawtypes"})
@Repository
@Transactional
public class CSAccountDaoImpl implements CSAccountDao {
    @Autowired
    private SessionFactory sessionFactory;

    private static final Logger LOGGER = LoggerFactory.getLogger(CSAccountDaoImpl.class);

    @Override
    public Integer searchCsAccountInfoCount(String moumouName, String csIdList, String improtCsIdList,
                                            int status, long startTime, long endTime) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        HashMap<String, Object> params = new HashMap<>(1);
        sql = "select COUNT(*) " +
                " FROM IM.IM_CSINFO " +
                " WHERE  IMPORTTIME > :startTime AND IMPORTTIME < :endTime ";

        params.put("startTime", startTime);
        params.put("endTime", endTime);

        if (status == 0 || status == 1) {
            sql += " AND STATUS = :STATUS ";
            params.put("STATUS", status);
        }
        if (moumouName != null && !moumouName.equals("")) {
            sql += " AND USERNAME like " + "\'%" + moumouName + "%\'";
        }
        if (csIdList != null && !csIdList.isEmpty()) {
            sql += " AND CSID IN ( " + csIdList + ") ";
        }
        if (improtCsIdList != null && !improtCsIdList.isEmpty()) {
            sql += " AND IMPORTCSID IN ( " + improtCsIdList + ") ";
        }

        return ((BigDecimal) SqlHelper.createQueryWithParams(sessionFactory.getCurrentSession(), sql, params, 0).list().get(0)).intValue();
    }

    @Override
    public List<CSAccountResponse> searchCsAccountInfo(String moumouName, String csIdList, String improtCsIdList,
                                                       int status, long startTime, long endTime, int fromRowIndex, int toRowIndex) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        HashMap<String, Object> params = new HashMap<>(1);
        sql = "select * from ( " +
                " SELECT rownum as rowIndex,USERID AS moumouId , " +
                " CSID AS opratorId, " +
                " USERNAME AS moumouUserName, " +
                " IMPORTCSID AS importCsId, " +
                " IMPORTNAME AS importuserName,  " +
                " PERMISSION  AS moumouUserLevel, " +
                " IMPORTNAME,IMPORTTIME,'--' AS ImportUserTeam , " +
                " REALNAME AS operatorRealName , " +
                " '--' AS operatorOrganName , " +
                " STATUS AS AccountStates, " +
                " RECEPTIONLIMIT AS AccountLimits " +
                " FROM IM.IM_CSINFO " +
                " WHERE  IMPORTTIME > :startTime AND IMPORTTIME < :endTime ";

        params.put("startTime", startTime);
        params.put("endTime", endTime);

        if (status == 0 || status == 1) {
            sql += " AND STATUS = :STATUS ";
            params.put("STATUS", status);
        }
        if (moumouName != null && !moumouName.equals("")) {
            sql += " AND USERNAME like " + "\'%" + moumouName + "%\'";
        }
        if (csIdList != null && !csIdList.isEmpty()) {
            sql += " AND CSID IN ( " + csIdList + ") ";
        }
        if (improtCsIdList != null && !improtCsIdList.isEmpty()) {
            sql += " AND IMPORTCSID IN ( " + improtCsIdList + ") ";
        }

        sql += ") where rowIndex > :fromRowIndex and rowIndex < :toRowIndex";
        params.put("fromRowIndex", fromRowIndex);
        params.put("toRowIndex", toRowIndex);

        return TransformService.transformResult(
                sessionFactory.getCurrentSession().createSQLQuery(sql),
                params,
                CSAccountResponse.class
        );
    }

    @Override
    public List<CSAccountResponse> searchCsAccountInfoByTime(long importTime, String importName) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        HashMap<String, Object> params = new HashMap<>(1);
        sql = " SELECT USERID AS moumouId , " +
                " CSID AS opratorId, " +
                " USERNAME AS moumouUserName, " +
                " IMPORTCSID AS importCsId, " +
                " IMPORTNAME AS importName,  " +
                " PERMISSION  AS moumouUserLevel, " +
                " IMPORTTIME,'--' AS ImportUserTeam , " +
                " REALNAME AS operatorRealName , " +
                " '--' AS operatorOrganName , " +
                " STATUS AS AccountStates, " +
                " RECEPTIONLIMIT AS AccountLimits " +
                " FROM IM.IM_CSINFO " +
                " WHERE  IMPORTTIME = :importTime AND IMPORTNAME =  :importName ";

        params.put("importTime", importTime);
        params.put("importName", importName);

        return TransformService.transformResult(
                sessionFactory.getCurrentSession().createSQLQuery(sql),
                params,
                CSAccountResponse.class
        );
    }

    @Override
    public Boolean insertToCSInfo(final String moumouUserName, final int permission, final String importName, final long importTime, final int receptionLimit, final int status, final long importCsId) {
        return sessionFactory.getCurrentSession().doReturningWork(new ReturningWork<Boolean>() {
            @Override
            public Boolean execute(Connection connection) throws SQLException {
                CallableStatement callableStatement = connection.prepareCall("{call mini_user_pkg.ImportCSReq(?,?,?,?,?,?,?,?)}");
                callableStatement.setString(1, moumouUserName);
                callableStatement.setInt(2, permission);
                callableStatement.setInt(3, receptionLimit);
                callableStatement.setString(4, importName);
                callableStatement.registerOutParameter(5, Types.BIGINT);
                callableStatement.setLong(6, importTime);
                callableStatement.setLong(7, importCsId);
                callableStatement.registerOutParameter(8, Types.INTEGER);
                callableStatement.execute();
                Long userId = callableStatement.getLong(5);
                Integer returnCode = callableStatement.getInt(8);

                callableStatement.close();

                if (null == returnCode || returnCode == 1 || userId.equals(0)) {
                    return false;
                }
                return true;
            }
        });
    }

    @Override
    public boolean setStatus(String ids, int status) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        HashMap<String, Object> params = new HashMap<>(1);
        sql = "UPDATE IM.IM_CSINFO SET STATUS = " + String.valueOf(status) +
                " WHERE USERID IN " +
                " ( " + ids + ") ";

        session.createSQLQuery(sql).executeUpdate();

        return true;
    }

    @Override
    public boolean setDetail(long moumouId, long csId, String csName, int limitNumber, int issupervisor) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        HashMap<String, Object> params = new HashMap<>(1);
        issupervisor = issupervisor == 0 ? 1 : 0;
        sql = "UPDATE IM.IM_CSINFO SET CSID = " + String.valueOf(csId) +
                ", REALNAME = '" + csName + "' " +
                ", RECEPTIONLIMIT = " + String.valueOf(limitNumber) +
                ", PERMISSION= " + String.valueOf(issupervisor) +
                " WHERE USERID = " +
                "  " + String.valueOf(moumouId) + " ";

        session.createSQLQuery(sql).executeUpdate();

        return true;
    }

    @Override
    public boolean setPassword(long moumouId, String password) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        HashMap<String, Object> params = new HashMap<>(1);
        String passwrodInMd5 = MD5.getMD5(password);
        sql = "UPDATE IM_USER SET PASSWORD='" + String.valueOf(passwrodInMd5) + "' " +
                " WHERE USERID = " +
                "  " + String.valueOf(moumouId) + " ";

        session.createSQLQuery(sql).executeUpdate();

        return true;
    }

    @Override
    public boolean deleteAccount(String ids) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        HashMap<String, Object> params = new HashMap<>(1);
        sql = "DELETE FROM IM.IM_CSINFO " +
                " WHERE USERID IN " +
                " ( " + ids + ") ";

        session.createSQLQuery(sql).executeUpdate();

        sql = "DELETE FROM IM.IM_USER " +
                " WHERE USERID IN " +
                " ( " + ids + ") ";

        session.createSQLQuery(sql).executeUpdate();

        sql = "DELETE FROM IM.IM_USERINFO " +
                " WHERE USERID IN " +
                " ( " + ids + ") ";

        session.createSQLQuery(sql).executeUpdate();

        sql = "DELETE FROM IM.IM_CSGROUPMEM " +
                " WHERE USERID IN " +
                " ( " + ids + ") ";

        session.createSQLQuery(sql).executeUpdate();
        return true;
    }

    @Override
    public List<CSAccountInfo> getAccountIdList(int startRow, int maxReuslt) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        HashMap<String, Object> params = new HashMap<>(1);
        List<CSAccountInfo> resultList = new ArrayList<>();
        sql = " SELECT USERID,userName,REALNAME FROM (SELECT  USERID ,userName,REALNAME ,ROWNUM AS ROWINDEX FROM IM.IM_CSINFO ORDER BY USERID )WHERE " +
                " ROWINDEX > " + String.valueOf(startRow) + " " +
                " AND ROWINDEX <= " + String.valueOf(maxReuslt + startRow) + " ";

        query = sessionFactory.getCurrentSession().createSQLQuery(sql);

        for (Object obj : query.list()) {
            if (obj == null) {
                return resultList;
            } else {
                Object[] res = (Object[]) obj;
                CSAccountInfo index = new CSAccountInfo();
                index.setUserid(((BigDecimal) res[0]).longValue());
                index.setUserName(res[1].toString());
                if (null != res[2])
                    index.setRealName(res[2].toString());
                else
                    index.setRealName("--");
                resultList.add(index);
            }
        }
        return resultList;
    }

    @Override
    public Integer getAccountIdListCount() {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        HashMap<String, Object> params = new HashMap<>(1);
        sql = " SELECT COUNT(*) AS moumouId  " +
                " FROM IM.IM_CSINFO ";

        return ((BigDecimal) sessionFactory.getCurrentSession().createSQLQuery(sql).list().get(0)).intValue();
    }

    @Override
    public List<KVPair<Long, String>> getMoumouNameByPartName(String partName) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        List<KVPair<Long, String>> moumouNameList = new ArrayList<>();
        sql = "SELECT USERID, USERNAME FROM IM_CSINFO WHERE 1=1";

        if (partName != null && partName.length() != 0)
            sql = sql + " and USERNAME like " + "\'%" + partName + "%\'";

        query = session.createSQLQuery(sql);

        for (Object obj : query.list()) {
            if (obj == null) {
                return moumouNameList;
            } else {
                Object[] res = (Object[]) obj;
                KVPair<Long, String> idToNamePair = new KVPair<>();
                idToNamePair.setKey(((BigDecimal) res[0]).longValue());
                idToNamePair.setValue(res[1].toString());
                moumouNameList.add(idToNamePair);
            }
        }
        return moumouNameList;
    }

    @Override
    public Long getUserIDInMouMou(Long userIDInBackend) {
        Number result = ((Number) sessionFactory.getCurrentSession().createSQLQuery("SELECT USERID FROM IM_CSINFO WHERE CSID = :userIDInBackend")
                .setParameter("userIDInBackend", userIDInBackend)
                .setMaxResults(1)
                .uniqueResult());

        if (result != null) {
            return result.longValue();
        } else {
            return null;
        }
    }
}
