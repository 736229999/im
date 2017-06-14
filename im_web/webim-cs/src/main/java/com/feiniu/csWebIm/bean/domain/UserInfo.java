package com.feiniu.csWebIm.bean.domain;

/**
 * Created by wangzhen on 2015/10/23.
 */
public class UserInfo {
    private String MEM_GUID ;
    private String NICKNAME ;
    private String MEM_USERNAME ;

    public String getMEM_GUID() {
        return MEM_GUID;
    }

    public void setMEM_GUID(String MEM_GUID) {
        this.MEM_GUID = MEM_GUID;
    }

    public String getNICKNAME() {
        return NICKNAME;
    }

    public void setNICKNAME(String NICKNAME) {
        this.NICKNAME = NICKNAME;
    }

    public String getMEM_USERNAME() {
        return MEM_USERNAME;
    }

    public void setMEM_USERNAME(String MEM_USERNAME) {
        this.MEM_USERNAME = MEM_USERNAME;
    }

    public void formatToshow() {
        if (MEM_GUID == null || MEM_GUID == "") {
            MEM_GUID = "--";
        }
        if (NICKNAME == null || NICKNAME == "") {
            NICKNAME = "--";
        }
        if (MEM_USERNAME == null || MEM_USERNAME == "") {
            MEM_USERNAME = "--";
        }
    }
}
