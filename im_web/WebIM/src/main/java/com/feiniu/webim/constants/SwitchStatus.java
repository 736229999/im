package com.feiniu.webim.constants;

/**
 * Author: morningking
 * Date: 2016/4/19 9:17
 * Contact: 243717042@qq.com
 */
public enum SwitchStatus {
    GENERAL_SWITCH_START(1, "普通转接开始"),
    GENERAL_SWITCH_SUCC(0, "普通转接结束"),
    MONITOR_SWITCH_SUCC(2, "主管接入"),;

    private int status;
    private String description;

    SwitchStatus(int status, String description) {
        this.status = status;
        this.description = description;
    }

    public int getStatus() {
        return status;
    }
}
