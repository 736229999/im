package com.feiniu.imServer.bean.request;

/**
 * Author: morningking
 * Date: 2016/2/25
 * Contact: 243717042@qq.com
 */
public class GetReturnOrderRequest {
    private String ogSeq;
    private String memGuid;
    private String fdlSeq;
    private String isMall;

    public String getOgSeq() {
        return ogSeq;
    }

    public void setOgSeq(String ogSeq) {
        this.ogSeq = ogSeq;
    }

    public String getMemGuid() {
        return memGuid;
    }

    public void setMemGuid(String memGuid) {
        this.memGuid = memGuid;
    }

    public String getFdlSeq() {
        return fdlSeq;
    }

    public void setFdlSeq(String fdlSeq) {
        this.fdlSeq = fdlSeq;
    }

    public String getIsMall() {
        return isMall;
    }

    public void setIsMall(String isMall) {
        this.isMall = isMall;
    }
}
