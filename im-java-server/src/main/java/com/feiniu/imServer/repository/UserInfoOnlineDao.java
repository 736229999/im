package com.feiniu.imServer.repository;

import org.springframework.beans.factory.annotation.Autowired;

/**
 * Created by chenqing on 2016/3/18.
 */
public interface UserInfoOnlineDao {

    Integer getUserOnlineTime(Long userId);


}
