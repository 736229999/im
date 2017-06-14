package com.feiniu.webim.bean.domain;

/**
 * Created by chenqing on 2016/3/2.
 */
public class FeiniuStaffInfo {
    private long workId;
    private String groupName;
    private String nickName;
    private String userName;

    public String getGroupName() {
        return groupName;
    }

    public void setGroupName(String groupName) {
        this.groupName = groupName;
    }

    public String getNickName() {
        return nickName;
    }

    public void setNickName(String nickName) {
        this.nickName = nickName;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public long getWorkId() {
        return workId;
    }

    public void setWorkId(long workId) {
        this.workId = workId;
    }

  }
