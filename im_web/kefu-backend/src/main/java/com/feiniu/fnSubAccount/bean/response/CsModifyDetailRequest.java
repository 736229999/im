package com.feiniu.fnSubAccount.bean.response;

/**
 * Created by wangzhen on 2015/11/23.
 */
public class CsModifyDetailRequest {
    Long moumouId;
    Long memberId;
    String memberName;
    Integer limited;
    Integer issupervisor;

    public Integer getIssupervisor() {
        return issupervisor;
    }

    public void setIssupervisor(Integer issupervisor) {
        this.issupervisor = issupervisor;
    }

    public Long getMoumouId() {
        return moumouId;
    }

    public void setMoumouId(Long moumouId) {
        this.moumouId = moumouId;
    }

    public Long getMemberId() {
        return memberId;
    }

    public void setMemberId(Long memberId) {
        this.memberId = memberId;
    }

    public String getMemberName() {
        return memberName;
    }

    public void setMemberName(String memberName) {
        this.memberName = memberName;
    }

    public Integer getLimited() {
        return limited;
    }

    public void setLimited(Integer limited) {
        this.limited = limited;
    }

}
