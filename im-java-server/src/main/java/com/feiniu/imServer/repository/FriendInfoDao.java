package com.feiniu.imServer.repository;

import com.feiniu.imServer.bean.backend.OrganNameInfo;
import com.feiniu.imServer.bean.friendList.FriendItem;

import java.util.List;

/**
 * Created by wangzhen on 2016/3/14.
 */
public interface FriendInfoDao {
    List<FriendItem> getAllFriendItemInDB(Long userId);
    List<OrganNameInfo> getAllGroupItemInDB(Long userId);
    Integer addFriendItem(Long userId, Long friendId,Long groupId);
    Integer deleteFriendItem(Long userId, Long friendId);
    Integer modifyFriendItem(Long userId, Long friendId,Long groupId);
    Integer addFriendGroup(Long userId ,OrganNameInfo organNameInfo);
    Integer deleteFriendGroup(Long userId ,Long groupId);
    Integer modifyFriendGroup(Long userId ,Long groupId ,String groupName);
}
