package com.feiniu.imServer.service.userinfoOnline;

import com.feiniu.imServer.repository.UserInfoOnlineDao;
import com.feiniu.imServer.service.friendList.FriendListService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

/**
 * Created by chenqing on 2016/3/18.
 */
@Service
public class UserInfoOnlineService {

    private static final Logger LOGGER = LoggerFactory.getLogger(UserInfoOnlineService.class);

    @Autowired
    UserInfoOnlineDao userInfoOnlineDao;

    public Integer getUserOnlineTime(Long userId){
        Integer onlineTime = userInfoOnlineDao.getUserOnlineTime(userId);
        return onlineTime;
    }
}
