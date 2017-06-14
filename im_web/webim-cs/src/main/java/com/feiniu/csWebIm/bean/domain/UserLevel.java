package com.feiniu.csWebIm.bean.domain;

/**
 * Created by wangzhen on 2015/10/26.
 */
public class UserLevel {
    String memLevel;
    String memLevelDesc;

    public String getMemLevel() {
        return memLevel;
    }

    public void setMemLevel(String memLevel) {
        this.memLevel = memLevel;
    }

    public String getMemLevelDesc() {
        return memLevelDesc;
    }

    public void setMemLevelDesc(String memLevelDesc) {
        this.memLevelDesc = memLevelDesc;
    }


    public void formatToshow() {
        if (memLevel == null || memLevel == "") {
            memLevel = "普通会员";
        }
        if (memLevelDesc == null || memLevelDesc == "") {
            memLevelDesc = "普通会员";
        }
    }

}
