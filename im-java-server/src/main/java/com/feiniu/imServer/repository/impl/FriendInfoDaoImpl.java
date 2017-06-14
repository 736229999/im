package com.feiniu.imServer.repository.impl;

import com.feiniu.imServer.bean.backend.OrganNameInfo;
import com.feiniu.imServer.bean.friendList.FriendItem;
import com.feiniu.imServer.repository.FriendInfoDao;
import com.feiniu.imServer.util.TransformService;
import org.hibernate.SessionFactory;
import org.hibernate.jdbc.ReturningWork;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;
import java.sql.*;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by wangzhen on 2016/3/14.
 */
@SuppressWarnings({" unused", " unchecked", " rawtypes"})
@Repository
@Transactional
public class FriendInfoDaoImpl implements FriendInfoDao{
    @Autowired
    private SessionFactory sessionFactory;

    private static final Logger LOGGER = LoggerFactory.getLogger(FriendInfoDaoImpl.class);

    @Override
    public List<FriendItem> getAllFriendItemInDB(Long userId) {
        Map<String, Object> params = new HashMap<>(1);

        StringBuilder stringBuilder = new StringBuilder(
                "SELECT FRIENDID  as friendId,FRIENDGROUPID AS groupId," +
                        " -1 AS status FROM IM.IM_FRIEND WHERE USERID = :userId");

        params.put("userId", userId);

        return TransformService.transformResult(
                sessionFactory.getCurrentSession().createSQLQuery(stringBuilder.toString()),
                params,
                FriendItem.class
        );
    }

    @Override
    public Integer addFriendItem(final Long userId,final Long friendId,final Long groupId) {
        int returnCode = sessionFactory.getCurrentSession().doReturningWork(new ReturningWork<Integer>() {
            @Override
            public Integer execute(Connection connection) throws SQLException {
                CallableStatement callableStatement = connection.prepareCall(
                        "{call Mini_User_Pkg.AddFriend(?,?,?,?)}");
                callableStatement.setLong(1, userId);
                callableStatement.setLong(2, friendId);
                callableStatement.setLong(3, groupId);
                callableStatement.registerOutParameter(4, Types.BIGINT);
                callableStatement.execute();
                Integer returnCode = callableStatement.getInt(4);
                callableStatement.close();
                return returnCode;
            }
        });

        if (returnCode == -1) {
            LOGGER.error("调用存储过程存储Mini_User_Pkg.AddFriend过程失败：" +userId.toString());
        }
        return returnCode;
    }

    @Override
    public Integer deleteFriendItem(final Long userId,final  Long friendId) {
        int returnCode = sessionFactory.getCurrentSession().doReturningWork(new ReturningWork<Integer>() {
            @Override
            public Integer execute(Connection connection) throws SQLException {
                CallableStatement callableStatement = connection.prepareCall(
                        "{call Mini_User_Pkg.DelFriend(?,?,?)}");
                callableStatement.setLong(1, userId);
                callableStatement.setLong(2, friendId);
                callableStatement.registerOutParameter(3, Types.BIGINT);
                callableStatement.execute();
                Integer returnCode = callableStatement.getInt(3);
                callableStatement.close();
                return returnCode;
            }
        });

        if (returnCode == -1) {
            LOGGER.error("调用存储过程存储Mini_User_Pkg.DelFriend过程失败：" +userId.toString());
        }
        return returnCode;
    }

    @Override
    public Integer modifyFriendItem(final  Long userId, final Long friendId,final  Long groupId) {
        int returnCode = sessionFactory.getCurrentSession().doReturningWork(new ReturningWork<Integer>() {
            @Override
            public Integer execute(Connection connection) throws SQLException {
                CallableStatement callableStatement = connection.prepareCall(
                        "{call Mini_User_Pkg.ModifyFriendGroup(?,?,?,?)}");
                callableStatement.setLong(1, userId);
                callableStatement.setLong(2, friendId);
                callableStatement.setLong(3, groupId);
                callableStatement.registerOutParameter(4, Types.BIGINT);
                callableStatement.execute();
                Integer returnCode = callableStatement.getInt(4);
                callableStatement.close();
                return returnCode;
            }
        });

        if (returnCode == -1) {
            LOGGER.error("调用存储过程存储Mini_User_Pkg.ModifyFriendGroup过程失败：" +userId.toString());
        }
        return returnCode;
    }

    @Override
    public List<OrganNameInfo> getAllGroupItemInDB(Long userId) {

        Map<String, Object> params = new HashMap<>(1);

        StringBuilder stringBuilder = new StringBuilder(
                "SELECT FRIENDGROUPID AS id,FRIENDGROUPNAME AS organName  ,FRIENDGROUPPID AS parentId FROM  " +
                        "IM.IM_FRIENDGROUP WHERE USERID =  :userId");

        params.put("userId", userId);

        return TransformService.transformResult(
                sessionFactory.getCurrentSession().createSQLQuery(stringBuilder.toString()),
                params,
                OrganNameInfo.class
        );

    }

    @Override
    public Integer addFriendGroup(final Long userId, final OrganNameInfo organNameInfo) {
        int returnCode = sessionFactory.getCurrentSession().doReturningWork(new ReturningWork<Integer>() {
            @Override
            public Integer execute(Connection connection) throws SQLException {
                CallableStatement callableStatement = connection.prepareCall(
                        "{call Mini_Local_Pkg.AddGroup(?,?,?,?,?)}");
                callableStatement.setLong(1,userId);
                callableStatement.setLong(2, organNameInfo.getId());
                callableStatement.setString(3, organNameInfo.getOrganName());
                callableStatement.setLong(4, organNameInfo.getParentId());
                callableStatement.registerOutParameter(5, Types.BIGINT);
                callableStatement.execute();
                Integer returnCode = callableStatement.getInt(5);
                callableStatement.close();
                return returnCode;
            }
        });

        if (returnCode == -1) {
            LOGGER.error("调用存储过程存储Mini_Local_Pkg.AddGroup过程失败：" +userId.toString());
            return  returnCode;
        }
        return returnCode;
    }

    @Override
    public Integer deleteFriendGroup(final Long userId, final Long groupId) {
        int returnCode = sessionFactory.getCurrentSession().doReturningWork(new ReturningWork<Integer>() {
            @Override
            public Integer execute(Connection connection) throws SQLException {
                CallableStatement callableStatement = connection.prepareCall(
                        "{call Mini_Local_Pkg.DeleteGroup(?,?,?)}");
                callableStatement.setLong(1, userId);
                callableStatement.setLong(2, groupId);
                callableStatement.registerOutParameter(3, Types.BIGINT);
                callableStatement.execute();
                Integer returnCode = callableStatement.getInt(3);
                callableStatement.close();
                return returnCode;
            }
        });

        if (returnCode == -1) {
            LOGGER.error("调用存储过程存储Mini_Local_Pkg.DeleteGroup过程失败：" +userId.toString());
            return  returnCode;
        }
        return returnCode;
    }

    @Override
    public Integer modifyFriendGroup(final Long userId, final Long groupId, final String groupName) {
        int returnCode = sessionFactory.getCurrentSession().doReturningWork(new ReturningWork<Integer>() {
            @Override
            public Integer execute(Connection connection) throws SQLException {
                CallableStatement callableStatement = connection.prepareCall(
                        "{call Mini_Local_Pkg.ModifyGroup(?,?,?,?)}");
                callableStatement.setLong(1, userId);
                callableStatement.setLong(2, groupId);
                callableStatement.setString(3, groupName);
                callableStatement.registerOutParameter(4, Types.BIGINT);
                callableStatement.execute();
                Integer returnCode = callableStatement.getInt(4);
                callableStatement.close();
                return returnCode;
            }
        });

        if (returnCode != 0) {
            LOGGER.error("调用存储过程存储Mini_Local_Pkg.AddGroup过程失败：" +userId.toString());
            return  returnCode;
        }
        return returnCode;
    }
}
