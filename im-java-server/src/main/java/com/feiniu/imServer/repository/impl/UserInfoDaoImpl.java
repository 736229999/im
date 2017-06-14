package com.feiniu.imServer.repository.impl;

import com.feiniu.imServer.bean.CachedUserInfo;
import com.feiniu.imServer.bean.base.UserInfoInDB;
import com.feiniu.imServer.bean.request.SaveBuyerRequest;
import com.feiniu.imServer.bean.request.SaveSellerRequest;
import com.feiniu.imServer.repository.UserInfoDao;
import com.feiniu.imServer.util.JsonUtil;
import com.feiniu.imServer.util.TransformService;
import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.jdbc.ReturningWork;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Types;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by wangzhen on 2016/2/19.
 */
@SuppressWarnings({" unused", " unchecked", " rawtypes"})
@Repository
public class UserInfoDaoImpl implements UserInfoDao{

    @Autowired
    private SessionFactory sessionFactory;
    private static final Logger LOGGER = LoggerFactory.getLogger(UserInfoDaoImpl.class);

    @Override
    @Transactional
    public CachedUserInfo getCachedUserInfoInDB(final Long userId) {
        Map<String, Object> params = new HashMap<>(1);

        StringBuilder stringBuilder = new StringBuilder(" " +
                "SELECT  A.*,b.EMAIL,b.USERNAME,b.PHONE，C.receptionLimit AS  receptionLimit" +
                " FROM " +
                " ( " +
                "  SELECT " +
                "   USERID AS ID, " +
                "   '' AS lanip, " +
                "   NICKNAME AS NICKNAME, " +
                "   USERPICURI AS headurl, " +
                "   1 AS flow, " +
                "   USERSIGN AS USERSIGN, " +
                "   '' AS csUserID, " +
                "   '' AS csTeamID, " +
                "   '-1' AS status " +
                "  FROM " +
                "   IM.IM_USERINFO " +
                "  WHERE " +
                "   USERID = :userId " +
                " ) A " +
                " LEFT JOIN ( " +
                " SELECT  " +
                "   USERID AS USERID, " +
                "  USERNAME AS USERNAME, " +
                "  EMAIL AS EMAIL, " +
                "  MOBILE AS phone " +
                " FROM " +
                "  IM.IM_USER " +
                " ) b ON A.ID = b.Userid" +
                " LEFT JOIN ( " +
                " SELECT  " +
                "  USERID,RECEPTIONLIMIT FROM im.IM_CSINFO  " +
                " )c on A.ID = C.USERID");

        params.put("userId", userId);

        List<CachedUserInfo> cachedUserInfoList =  TransformService.transformResult(
                sessionFactory.getCurrentSession().createSQLQuery(stringBuilder.toString()),
                params,
                CachedUserInfo.class);

        if (cachedUserInfoList == null || cachedUserInfoList.size() == 0)
            return null;
        else
            return cachedUserInfoList.get(0);
    }

    @Override
    @Transactional
    public String getUserIdListInGroup(final Long groupId) {
        Query query;
        Session session = sessionFactory.getCurrentSession();
        StringBuilder stringBuilder = new StringBuilder(" " +
                "SELECT listagg( S_SUBACCOUNT_ID,',')" +
                " within GROUP (order by S_GROUP_ID)  " +
                "FROM IM.T_SERVICE_GROUP_MAPPING WHERE S_GROUP_ID="+groupId.toString()
        );

        query = session.createSQLQuery(stringBuilder.toString());

        for (Object obj : query.list()) {
            if (obj == null) {
                return null;
            } else {
                return obj.toString();
            }
        }
        return null;
    }

    @Override
    @Transactional
    public Integer saveBuyer(final SaveBuyerRequest saveBuyerRequest) {

        int userId = sessionFactory.getCurrentSession().doReturningWork(new ReturningWork<Integer>() {
            @Override
            public Integer execute(Connection connection) throws SQLException {
                CallableStatement callableStatement = connection.prepareCall(
                        "{call Mini_User_Pkg.AccountSaveBuyer_20150907(?,?,?,?,?,?,?,?,?)}");
                callableStatement.registerOutParameter(1, Types.BIGINT);
                callableStatement.setString(2, saveBuyerRequest.getUserName());
                callableStatement.setString(3, saveBuyerRequest.getPassword());
                callableStatement.setString(4, saveBuyerRequest.getMemGuid());
                callableStatement.setString(5, saveBuyerRequest.getMobileNo());
                callableStatement.setString(6, saveBuyerRequest.getEmail());
                callableStatement.setString(7, saveBuyerRequest.getNickName());
                callableStatement.setString(8, saveBuyerRequest.getHeadImgUrl());

                callableStatement.registerOutParameter(9, Types.BIGINT);
                callableStatement.execute();
                Integer returnCode = callableStatement.getInt(9);
                Integer userId = 0;
                if(returnCode == 0)
                {
                    userId = callableStatement.getInt(1);
                }
                callableStatement.close();

                return userId;
            }
        });

        if (userId == 0) {
            LOGGER.error("调用存储过程存储过程失败：" +  JsonUtil.toJsonString(saveBuyerRequest));
        }
        return userId;
    }

    @Override
    @Transactional
    public Integer saveSeller(final SaveSellerRequest saveSellerRequest) {
        Integer returnCode = sessionFactory.getCurrentSession().doReturningWork(new ReturningWork<Integer>() {
            @Override
            public Integer execute(Connection connection) throws SQLException {
                CallableStatement callableStatement = connection.prepareCall(
                        "{call Mini_User_Pkg.AccountSave_20150916(?,?,?,?,?)}");
                callableStatement.setInt(1, saveSellerRequest.getUserId());
                callableStatement.setString(2, saveSellerRequest.getUserName());
                callableStatement.setString(3, saveSellerRequest.getPassword());
                callableStatement.setString(4, saveSellerRequest.getTrueName());

                callableStatement.registerOutParameter(5, Types.BIGINT);
                callableStatement.execute();
                Integer returnCode = callableStatement.getInt(5);
                callableStatement.close();

                return returnCode;
            }
        });

        if (returnCode != 0) {
            LOGGER.error("调用存储过程存储过程失败：" +  JsonUtil.toJsonString(saveSellerRequest));
        }
        return returnCode;

    }

    @Override
    @Transactional
    public UserInfoInDB getUserInfoInDB(Long userId) {
        StringBuilder stringBuilder = new StringBuilder(
                "call Mini_User_Pkg.GetUserInfo_20160302("+userId.toString() +")");

        List<UserInfoInDB> resultList = TransformService.transformResult(
                sessionFactory.getCurrentSession().createSQLQuery(stringBuilder.toString()),
                Collections.EMPTY_MAP,
                UserInfoInDB.class
        );

        if (resultList == null || resultList.size() == 0 )
            return null;
        else
            return resultList.get(0);
    }

    @Override
    public void  updateImUserInfoFeild(Long userId,String feild, String value) {
        Session session = sessionFactory.getCurrentSession();
        session.createQuery("update IM.IM_USER set "+feild +" = :value where userId = :userId")
                .setParameter("value", value)
                .setParameter("userId",userId)
                .executeUpdate();
        return ;
    }

    @Override
    public void  updateImUserFeild(Long userId,String feild, String value) {
        Session session = sessionFactory.getCurrentSession();
        session.createQuery("update IM.IM_USERINFO set "+feild + "= :value where userId = :userId")
                .setParameter("value",value)
                .setParameter("userId",userId)
                .executeUpdate();
        return ;
    }
}
