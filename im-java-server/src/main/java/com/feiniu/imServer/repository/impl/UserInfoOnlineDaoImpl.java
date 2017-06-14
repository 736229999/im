package com.feiniu.imServer.repository.impl;

import com.feiniu.imServer.repository.UserInfoOnlineDao;
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

/**
 * Created by chenqing on 2016/3/18.
 */
@SuppressWarnings({ " unused" ," unchecked" , " rawtypes"  })
@Repository
@Transactional
public class UserInfoOnlineDaoImpl implements UserInfoOnlineDao {

    private static final Logger LOGGER = LoggerFactory.getLogger(UserInfoOnlineDaoImpl.class);

    @Autowired
    SessionFactory sessionFactory;


    @Override
    public Integer getUserOnlineTime(final Long userId){

        Integer onlineTime = sessionFactory.getCurrentSession().doReturningWork(new ReturningWork<Integer>() {
            @Override
            public Integer execute(Connection connection) throws SQLException {
                CallableStatement callableStatement = connection.prepareCall(
                        "{call Mini_Local_Pkg.GetUserOnlineTime(?,?,?)}"
                );
                callableStatement.setLong(1, userId);
                callableStatement.registerOutParameter(2, Types.BIGINT);
                callableStatement.registerOutParameter(3, Types.BIGINT);
                callableStatement.execute();
                Integer onlineTime = callableStatement.getInt(2);
                Integer returnCode = callableStatement.getInt(3);
                callableStatement.close();
                if (returnCode == -1) {
                    LOGGER.error("调用存储过程存储Mini_Local_Pkg.GetUserOnlineTime过程失败：" +userId.toString());
                }
                return onlineTime;
            }
        });

        return onlineTime;
    }
}
