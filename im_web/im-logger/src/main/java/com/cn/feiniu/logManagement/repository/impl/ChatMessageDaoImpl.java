package com.cn.feiniu.logManagement.repository.impl;

import com.cn.feiniu.logManagement.bean.dto.ChatMessage;
import com.cn.feiniu.logManagement.repository.ChatMessageDao;
import com.cn.feiniu.logManagement.util.json.JsonUtil;
import org.apache.commons.collections4.CollectionUtils;
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
import java.util.List;

/**
 * Author: morningking
 * Date: 2016/1/25
 * Contact: 243717042@qq.com
 */
@Repository
@Transactional
public class ChatMessageDaoImpl implements ChatMessageDao {
    @Autowired
    private SessionFactory sessionFactory;
    private static final Logger LOGGER = LoggerFactory.getLogger(ChatMessageDaoImpl.class);

    @Override
    public void saveMessageList(List<ChatMessage> chatMessages) {
        if (CollectionUtils.isNotEmpty(chatMessages)) {
            for (final ChatMessage chatMessage : chatMessages) {
                saveMessage(chatMessage);
            }
        }
    }

    @Override
    public void saveMessage(final ChatMessage chatMessage) {
        int returnCode = sessionFactory.getCurrentSession().doReturningWork(new ReturningWork<Integer>() {
            @Override
            public Integer execute(Connection connection) throws SQLException {
                CallableStatement callableStatement = connection.prepareCall(
                        "{call Mini_User_Pkg.WebIMMsgSave_20151222(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}");
                callableStatement.setInt(1, chatMessage.getFrom_id());
                callableStatement.setInt(2, chatMessage.getTo_id());
                callableStatement.setInt(3, chatMessage.getData_type());
                callableStatement.setInt(4, chatMessage.getVer());
                callableStatement.setInt(5, chatMessage.getSend_time());
                callableStatement.setInt(6, chatMessage.getRecv_flag());
                callableStatement.setInt(7, chatMessage.getFontSize());
                callableStatement.setInt(8, chatMessage.getFontColor());
                callableStatement.setInt(9, chatMessage.getFontStyle());
                callableStatement.setInt(10, chatMessage.getLogintime());
                callableStatement.setString(11, chatMessage.getFontName());
                callableStatement.setString(12, chatMessage.getFrom_nickname());
                callableStatement.setString(13, chatMessage.getData());
                callableStatement.setString(14, chatMessage.getUuid());

                callableStatement.registerOutParameter(15, Types.BIGINT);
                callableStatement.execute();
                Integer returnCode = callableStatement.getInt(15);
                callableStatement.close();

                return returnCode;
            }
        });

        if (returnCode == -1) {
            LOGGER.error("调用存储过程存储过程失败：" + JsonUtil.toJsonString(chatMessage));
        }
    }
}
