package com.cn.feiniu.logManagement.repository.impl;

import com.cn.feiniu.logManagement.bean.web.req.SaveBuyerRequest;
import com.cn.feiniu.logManagement.bean.web.req.SaveSellerRequest;
import com.cn.feiniu.logManagement.repository.UserInfoDao;
import com.cn.feiniu.logManagement.util.json.JsonUtil;
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
 * Created by zhangmin on 2016/2/29.
 */
@Repository
@Transactional
public class UserInfoDaoImpl implements UserInfoDao{

    @Autowired
    private SessionFactory sessionFactory;
    private static final Logger LOGGER = LoggerFactory.getLogger(UserInfoDaoImpl.class);

    @Override
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
    public void saveCS() {

    }
}
