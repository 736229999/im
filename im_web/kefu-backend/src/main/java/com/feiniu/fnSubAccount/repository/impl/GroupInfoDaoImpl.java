package com.feiniu.fnSubAccount.repository.impl;

import com.feiniu.fnSubAccount.bean.ServiceGroupDetail;
import com.feiniu.fnSubAccount.bean.ServiceGroupInfo;
import com.feiniu.fnSubAccount.repository.GroupInfoDao;
import com.feiniu.fnSubAccount.util.FormatUtils;
import com.feiniu.fnSubAccount.util.TransformService;
import org.apache.commons.collections4.CollectionUtils;
import org.hibernate.Query;
import org.hibernate.SQLQuery;
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
import java.util.Map;

/**
 * Created by wangzhen on 2015/10/19.
 */
@SuppressWarnings({"unused", "unchecked", "rawtypes"})
@Repository
public class GroupInfoDaoImpl implements GroupInfoDao {
    @Autowired
    protected SessionFactory sessionFactory;

    protected Logger LOGGER = LoggerFactory.getLogger(this.getClass());

    @Override
    @Transactional
    public Long createGroup(final String groupName, final String groupGescription, final String importName, final String currentTime) {
        return sessionFactory.getCurrentSession().doReturningWork(new ReturningWork<Long>() {
            @Override
            public Long execute(Connection connection) throws SQLException {
                CallableStatement callableStatement = connection.prepareCall("{call mini_user_pkg.CreateGroup(?,?,?,?,?,?)}");
                callableStatement.setString(1, groupName);
                callableStatement.setString(2, groupGescription);
                callableStatement.setString(3, importName);
                callableStatement.setLong(4, Long.valueOf(currentTime));
                callableStatement.registerOutParameter(5, Types.BIGINT);
                callableStatement.registerOutParameter(6, Types.INTEGER);
                callableStatement.execute();
                Long groupId = callableStatement.getLong(5);
                Integer returnCode = callableStatement.getInt(6);
                callableStatement.close();
                if (returnCode != null && ((BigDecimal) new BigDecimal(returnCode)).equals(new BigDecimal(0)))
                    return groupId;
                else
                    return Long.valueOf(-1);
            }
        });
    }

    @Override
    @Transactional
    public Long modifyGroup(final String groupId, final String groupName, final String groupGescription, final String importName, final String currentTime) {
        return sessionFactory.getCurrentSession().doReturningWork(new ReturningWork<Long>() {
            @Override
            public Long execute(Connection connection) throws SQLException {
                CallableStatement callableStatement = connection.prepareCall("{call mini_user_pkg.ModifyGroup(?,?,?,?,?,?)}");
                callableStatement.setFloat(1, Float.valueOf(groupId));
                callableStatement.setString(2, groupName);
                callableStatement.setString(3, groupGescription);
                callableStatement.setString(4, importName);
                callableStatement.setLong(5, Long.valueOf(currentTime));
                callableStatement.registerOutParameter(6, Types.INTEGER);
                callableStatement.execute();
                Integer returnCode = callableStatement.getInt(6);
                callableStatement.close();

                if (returnCode != null && ((BigDecimal) new BigDecimal(returnCode)).equals(new BigDecimal(0)))
                    return Long.valueOf(1);
                else
                    return Long.valueOf(-1);
            }
        });
    }

    @Override
    @Transactional
    public Long addGroupMem(final String groupId, final String CSId, final String importName, final String currentTime) {
        return sessionFactory.getCurrentSession().doReturningWork(new ReturningWork<Long>() {
            @Override
            public Long execute(Connection connection) throws SQLException {
                CallableStatement callableStatement = connection.prepareCall("{call mini_user_pkg.AddGroupMem(?,?,?,?,?)}");
                callableStatement.setFloat(1, Float.valueOf(groupId));
                callableStatement.setString(2, CSId);
                callableStatement.setString(3, importName);
                callableStatement.setLong(4, Long.valueOf(currentTime));
                callableStatement.registerOutParameter(5, Types.INTEGER);
                callableStatement.execute();
                Integer returnCode = callableStatement.getInt(5);
                callableStatement.close();

                if (returnCode != null && ((BigDecimal) new BigDecimal(returnCode)).equals(new BigDecimal(0)))
                    return Long.valueOf(1);
                else
                    return Long.valueOf(-1);
            }
        });
    }

    @Override
    @Transactional
    public Long getGroupListNum(String groupName, String lastModifyName, String beginTime, String endTime) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        sql = "SELECT COUNT(*) FROM IM_CSGROUP WHERE 1= 1 ";
        if (groupName != null && groupName.length() != 0)
            sql = sql + " and GROUPNAME=" + "\'" + groupName + "\'";

        if (lastModifyName != null && lastModifyName.length() != 0)
            sql = sql + " and LASTMODIFYNAME=" + "\'" + lastModifyName + "\'";

        long startTimeLong = FormatUtils.getMinStartTime(beginTime);
        long endTimeLong = FormatUtils.getMaxEndTime(endTime);

        sql = sql + " and LASTMODIFYTIME>=" + String.valueOf(startTimeLong);
        sql = sql + " and LASTMODIFYTIME<=" + String.valueOf(endTimeLong);
        query = session.createSQLQuery(sql);
        return ((BigDecimal) (query.list().get(0))).longValue();
    }

    @Override
    @Transactional
    public List<ServiceGroupInfo> queryGroupList(String groupName, String lastModifyName, String beginTime, String endTime,
                                                 int startRow, int maxResults) {
        String sql;
        String strsql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        List<ServiceGroupInfo> groupinfoList = new ArrayList<>();

        sql = "SELECT GROUPID AS groupId, GROUPNAME AS groupName,GROUPDESCRIB AS groupDescription,LASTMODIFYNAME AS lastModifiedName,LASTMODIFYTIME AS lastModifiedTime," +
                " EMBED_SET as embedSet from IM_CSGROUP where 1=1";
        if (groupName != null && groupName.length() != 0)
            sql = sql + " and GROUPNAME=" + "\'" + groupName + "\'";

        if (lastModifyName != null && lastModifyName.length() != 0)
            sql = sql + " and LASTMODIFYNAME=" + "\'" + lastModifyName + "\'";

        long startTimeLong = FormatUtils.getMinStartTime(beginTime);
        long endTimeLong = FormatUtils.getMaxEndTime(endTime);

        sql = sql + " and LASTMODIFYTIME>=" + String.valueOf(startTimeLong);
        sql = sql + " and LASTMODIFYTIME<=" + String.valueOf(endTimeLong);

        query = session.createSQLQuery(sql);

        for (Object obj : query.setFirstResult(startRow).setMaxResults(maxResults).list()) {
            if (obj == null) {
                return groupinfoList;
            } else {
                Object[] res = (Object[]) obj;
                ServiceGroupInfo index = new ServiceGroupInfo();
                if (res[0] == null) {
                    return groupinfoList;
                }
                index.setGroupId((res[0]).toString());
                index.setGroupName((res[1]).toString());
                index.setGroupDescription((res[2]).toString());
                index.setLastModifiedName((res[3]).toString());
                index.setLastModifiedTime((res[4]).toString());
                index.setEmbedSet((String)res[5]);
                groupinfoList.add(index);
            }
        }

        return groupinfoList;
    }

    @Override
    @Transactional
    public List<ServiceGroupDetail> queryGroupMemInfo(int startRow, int maxResults) {

        return queryGroupMemInfo("*", startRow, maxResults);

    }

    @Override
    @Transactional
    public List<ServiceGroupDetail> queryGroupMemInfo(String groupId, int startRow, int maxResults) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        List<ServiceGroupDetail> groupinfoList = new ArrayList<>();

        sql = "SELECT a.USERID AS userId,a.USERNAME AS MoumouName,a.realname AS ServiceName,b.FLOWSTATUS AS Status ,a.CSID AS CSID FROM IM_CSINFO a,IM_CSGROUPMEM b WHERE a.USERID =b.USERID and b.GROUPID=" + groupId;
        query = session.createSQLQuery(sql);

        for (Object obj : query.setFirstResult(startRow).setMaxResults(maxResults).list()) {
            if (obj == null) {
                return groupinfoList;
            } else {
                Object[] res = (Object[]) obj;
                ServiceGroupDetail index = new ServiceGroupDetail();
                if (res[0] == null) {
                    return groupinfoList;
                }
                index.setUserId((res[0]).toString());
                index.setMoumouName((res[1]).toString());
                if ((res[2]) != null)
                    index.setServiceName((res[2]).toString());
                else
                    index.setServiceName("--");
                index.setStatus((res[3]).toString());

                if ((res[4]) == null)
                    index.setCsId(null);
                else
                    index.setCsId((res[4]).toString());
                groupinfoList.add(index);
            }
        }
        return groupinfoList;
    }

    @Override
    @Transactional
    public List<ServiceGroupDetail> queryGroupMemInfo(String idList, String serviceName, String moumouName,
                                                      int startRow, int maxResults) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        List<ServiceGroupDetail> groupinfoList = new ArrayList<>();
        sql = "SELECT aa.USERID AS USERID ,AA.username as MoumouName ,AA.REALNAME,bb.groupname AS groupname,AA.CSID as csid FROM IM_CSINFO aa LEFT JOIN ( " +
                " SELECT DISTINCT USERID,listagg( groupName,',')WITHIN GROUP (ORDER BY groupName) AS groupname FROM ( " +
                " SELECT a.USERID AS USERID ,a.GROUPID AS GROUPID,b.GROUPNAME as groupname FROM IM_CSGROUPMEM a LEFT JOIN IM_CSGROUP b ON a.GROUPID = b.GROUPID " +
                " )GROUP BY USERID)bb " +
                " ON aa.userid = bb.userid " +
                " WHERE (1=1) ";
        if (serviceName != null && serviceName.length() != 0)
            sql = sql + " and REALNAME like " + "\'%" + serviceName + "%\'";

        if (moumouName != null && moumouName.length() != 0)
            sql = sql + " and username=" + "\'" + moumouName + "\'";

        if (idList != null && idList.length() != 0)
            sql = sql + " and CSID IN( " + idList + ")";

        query = session.createSQLQuery(sql);

        for (Object obj : query.setFirstResult(startRow).setMaxResults(maxResults).list()) {
            if (obj == null) {
                return groupinfoList;
            } else {
                Object[] res = (Object[]) obj;
                ServiceGroupDetail index = new ServiceGroupDetail();
                if (res[0] == null) {
                    return groupinfoList;
                }
                index.setUserId((res[0]).toString());

                if (res[1] == null) {
                    index.setMoumouName("");
                } else {
                    index.setMoumouName((res[1]).toString());
                }

                if (res[2] == null) {
                    index.setServiceName("");
                } else {
                    index.setServiceName((res[2]).toString());
                }

                if (res[3] == null) {
                    index.setGroupName("");
                } else {
                    index.setGroupName((res[3]).toString());
                }
                if ((res[4]) == null)
                    index.setCsId(null);
                else
                    index.setCsId((res[4]).toString());
                groupinfoList.add(index);
            }
        }

        return groupinfoList;
    }

    @Override
    @Transactional
    public List<ServiceGroupInfo> queryGroupNameList(String groupNamePart) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        List<ServiceGroupInfo> groupinfoList = new ArrayList<>();
        sql = "SELECT DISTINCT GROUPNAME FROM IM_CSGROUP WHERE 1=1";

        if (groupNamePart != null && groupNamePart.length() != 0)
            sql = sql + " and GROUPNAME like " + "\'%" + groupNamePart + "%\'";

        query = session.createSQLQuery(sql);

        for (Object obj : query.list()) {
            if (obj == null) {
                return groupinfoList;
            } else {
                ServiceGroupInfo index = new ServiceGroupInfo();
                index.setGroupName(obj.toString());
                groupinfoList.add(index);
            }
        }
        return groupinfoList;
    }

    @Override
    @Transactional
    public List<ServiceGroupInfo> querylastModifyedPepoleList(String lastModifiedPeopleNamePart) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        List<ServiceGroupInfo> groupinfoList = new ArrayList<>();
        sql = "SELECT DISTINCT LASTMODIFYNAME FROM IM_CSGROUP WHERE 1=1";

        if (lastModifiedPeopleNamePart != null && lastModifiedPeopleNamePart.length() != 0)
            sql = sql + " and LASTMODIFYNAME like " + "\'%" + lastModifiedPeopleNamePart + "%\'";

        query = session.createSQLQuery(sql);

        for (Object obj : query.list()) {
            if (obj == null) {
                return groupinfoList;
            } else {
                ServiceGroupInfo index = new ServiceGroupInfo();
                index.setLastModifiedName(obj.toString());
                groupinfoList.add(index);
            }
        }

        return groupinfoList;
    }


    @Transactional
    @Override
    public List<ServiceGroupDetail> organNameList() {
        List<ServiceGroupDetail> groupinfoList = new ArrayList<>();
        return groupinfoList;
    }


    @Transactional
    @Override
    public Long modifyStatus(String userId, Long status, String groupId) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();

        sql = "UPDATE IM_CSGROUPMEM SET FLOWSTATUS =" + status + " WHERE GROUPID =" + groupId + " and USERID =" + userId;
        query = session.createSQLQuery(sql);
        query.executeUpdate();
        return Long.valueOf(1);
    }


    @Transactional
    @Override
    public Long deleteGroup(String groupId) {
        String sql;
        Query query;
        Session session = sessionFactory.getCurrentSession();
        sql = "DELETE FROM IM.IM_CSGROUP WHERE GROUPID = " + groupId;
        query = session.createSQLQuery(sql);
        query.executeUpdate();

        sql = "DELETE FROM IM.IM_CSGROUPMEM WHERE GROUPID = " + groupId;
        query = session.createSQLQuery(sql);
        query.executeUpdate();
        return Long.valueOf(1);

    }

    @Transactional
    @Override
    public Long getGroupMemNum() {
        return Long.valueOf(10);
    }

    @Transactional
    @Override
    public Long getGroupMemNum(String serviceName, String moumouName, String idList) {
        String sql;
        Query query;
        String groupMemCount = "";
        Session session = sessionFactory.getCurrentSession();
        sql = "SELECT COUNT(*)  FROM IM_CSINFO WHERE 1= 1 ";
        if (serviceName != null && serviceName.length() != 0)
            sql = sql + " and REALNAME like " + "\'%" + serviceName + "%\'";

        if (moumouName != null && moumouName.length() != 0)
            sql = sql + " and USERNAME=" + "\'" + moumouName + "\'";

        if (idList != null && idList.length() != 0)
            sql = sql + " and CSID IN( " + idList + ")";

        query = session.createSQLQuery(sql);
        return ((BigDecimal) (query.list().get(0))).longValue();
    }

    @Transactional
    @Override
    public Long getGroupMemNum(String groupId) {
        String sql;
        Query query;
        String groupMemCount = "";
        Session session = sessionFactory.getCurrentSession();
        sql = "SELECT COUNT(*) AS groupMemCount FROM IM_CSGROUPMEM WHERE GROUPID = " + groupId;
        query = session.createSQLQuery(sql);
        return ((BigDecimal) (query.list().get(0))).longValue();
    }

    @Transactional
    public List<ServiceGroupInfo> getAllGroup() {
        SQLQuery sqlQuery = sessionFactory.getCurrentSession()
                .createSQLQuery("SELECT GROUPID AS groupId, GROUPNAME AS groupName, GROUPDESCRIB AS groupDescription," +
                        "LASTMODIFYNAME AS lastModifiedName, LASTMODIFYTIME AS lastModifiedTime ,EMBED_SET as embedSet FROM im_csgroup");

        return TransformService.transformResult(sqlQuery, null, ServiceGroupInfo.class);
    }

    @Transactional
    @Override
    public Map<Integer, String> getGroupToEmbeds() {
        String sql = "SELECT GROUPID AS groupId,EMBED_SET AS embedSet FROM IM_CSGROUP WHERE EMBED_SET IS NOT NULL";
        List<Object[]> result = sessionFactory.getCurrentSession().createSQLQuery(sql).list();

        if (CollectionUtils.isNotEmpty(result)) {
            Map<Integer, String> groupToEmbeds = new HashMap<>(1);
            for (Object[] row : result) {
                groupToEmbeds.put(((BigDecimal) row[0]).intValue(), (String) row[1]);
            }

            return groupToEmbeds;
        } else {
            return null;
        }

    }

    @Override
    @Transactional
    public void saveEmbedSets(Map<Integer, String> groupToEmbedsMap) {
        Session session = sessionFactory.getCurrentSession();

        //1. 先将之前布点设置的置空
        session.createSQLQuery("UPDATE IM_CSGROUP SET EMBED_SET = NULL").executeUpdate();
        //2.保存本次布点设置
        SQLQuery savePair = session.createSQLQuery("UPDATE IM_CSGROUP SET EMBED_SET = :embedSets WHERE GROUPID = :groupID");
        for (Map.Entry<Integer, String> groupToEmbed : groupToEmbedsMap.entrySet()) {
            savePair.setParameter("embedSets", groupToEmbed.getValue())
                    .setParameter("groupID", groupToEmbed.getKey())
                    .executeUpdate();
        }
    }
}
