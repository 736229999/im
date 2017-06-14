package com.feiniu.imServer.bean.friendList;

/**
 * Created by wangzhen on 2016/3/14.
 */
public class FriendItem {
    private Long friendId;
    private Integer status;
    private Long groupId;

    public Long getFriendId() {
        return friendId;
    }

    public void setFriendId(Long friendId) {
        this.friendId = friendId;
    }

    public Integer getStatus() {
        return status;
    }

    public void setStatus(Integer status) {
        this.status = status;
    }

    public Long getGroupId() {
        return groupId;
    }

    public void setGroupId(Long groupId) {
        this.groupId = groupId;
    }
}
