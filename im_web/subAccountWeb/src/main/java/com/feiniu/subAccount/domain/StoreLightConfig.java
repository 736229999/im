package com.feiniu.subAccount.domain;

import javax.persistence.*;

/**
 * Created by wangchen on 2015/2/3.
 */
@Entity
@Table(name = "t_store_light_config")
public class StoreLightConfig {
    @Id
    @Column(name = "s_merchant_id", unique = true, nullable = false)
    private long merchantID;

    @Column(name = "s_first_work_day_start", columnDefinition = "char(1)", nullable = false)
    private char firstWorkDayStart = '1';

    @Column(name = "s_first_work_day_end", columnDefinition = "char(1)", nullable = false)
    private char firstWorkDayEnd = '1';

    @Column(name = "s_first_work_time_start", columnDefinition = "char(5)", nullable = false)
    private String firstWorkTimeStart = "00:00";

    @Column(name = "s_first_work_time_end", columnDefinition = "char(5)", nullable = false)
    private String firstWorkTimeEnd = "00:00";

    @Column(name = "s_first_is_work_time_show", columnDefinition = "char(1)", nullable = false)
    private char isFirstWorkTimeShow = '0';

    @Column(name = "s_second_work_day_start", columnDefinition = "char(1)", nullable = false)
    private char secondWorkDayStart = '1';

    @Column(name = "s_second_work_day_end", columnDefinition = "char(1)", nullable = false)
    private char secondWorkDayEnd = '1';

    @Column(name = "s_second_work_time_start", columnDefinition = "char(5)", nullable = false)
    private String secondWorkTimeStart = "00:00";

    @Column(name = "s_second_work_time_end", columnDefinition = "char(5)", nullable = false)
    private String secondWorkTimeEnd = "00:00";

    @Column(name = "s_second_is_work_time_show", columnDefinition = "char(1)", nullable = false)
    private char isSecondWorkTimeShow = '0';

    @Column(name = "s_telphone", columnDefinition = "char(11)", nullable = false)
    private String telphone;

    @Column(name = "s_is_telphone_show", columnDefinition = "char(1)", nullable = false)
    private char isTelphoneShow = '0';

    @Column(name = "s_phone", columnDefinition = "char(11)", nullable = false)
    private String phone;

    @Column(name = "s_is_phone_show", columnDefinition = "char(1)", nullable = false)
    private char isPhoneShow = '0';

    public long getMerchantID() {
        return merchantID;
    }

    public void setMerchantID(long merchantID) {
        this.merchantID = merchantID;
    }

    public char getFirstWorkDayStart() {
        return firstWorkDayStart;
    }

    public void setFirstWorkDayStart(char firstWorkDayStart) {
        this.firstWorkDayStart = firstWorkDayStart;
    }

    public char getFirstWorkDayEnd() {
        return firstWorkDayEnd;
    }

    public void setFirstWorkDayEnd(char firstWorkDayEnd) {
        this.firstWorkDayEnd = firstWorkDayEnd;
    }

    public String getFirstWorkTimeStart() {
        return firstWorkTimeStart;
    }

    public void setFirstWorkTimeStart(String firstWorkTimeStart) {
        this.firstWorkTimeStart = firstWorkTimeStart;
    }

    public String getFirstWorkTimeEnd() {
        return firstWorkTimeEnd;
    }

    public void setFirstWorkTimeEnd(String firstWorkTimeEnd) {
        this.firstWorkTimeEnd = firstWorkTimeEnd;
    }

    public char getIsFirstWorkTimeShow() {
        return isFirstWorkTimeShow;
    }

    public void setIsFirstWorkTimeShow(char isFirstWorkTimeShow) {
        this.isFirstWorkTimeShow = isFirstWorkTimeShow;
    }

    public char getSecondWorkDayStart() {
        return secondWorkDayStart;
    }

    public void setSecondWorkDayStart(char secondWorkDayStart) {
        this.secondWorkDayStart = secondWorkDayStart;
    }

    public char getSecondWorkDayEnd() {
        return secondWorkDayEnd;
    }

    public void setSecondWorkDayEnd(char secondWorkDayEnd) {
        this.secondWorkDayEnd = secondWorkDayEnd;
    }

    public String getSecondWorkTimeStart() {
        return secondWorkTimeStart;
    }

    public void setSecondWorkTimeStart(String secondWorkTimeStart) {
        this.secondWorkTimeStart = secondWorkTimeStart;
    }

    public String getSecondWorkTimeEnd() {
        return secondWorkTimeEnd;
    }

    public void setSecondWorkTimeEnd(String secondWorkTimeEnd) {
        this.secondWorkTimeEnd = secondWorkTimeEnd;
    }

    public char getIsSecondWorkTimeShow() {
        return isSecondWorkTimeShow;
    }

    public void setIsSecondWorkTimeShow(char isSecondWorkTimeShow) {
        this.isSecondWorkTimeShow = isSecondWorkTimeShow;
    }

    public String getTelphone() {
        return telphone;
    }

    public void setTelphone(String telphone) {
        this.telphone = telphone;
    }

    public char getIsTelphoneShow() {
        return isTelphoneShow;
    }

    public void setIsTelphoneShow(char isTelphoneShow) {
        this.isTelphoneShow = isTelphoneShow;
    }

    public String getPhone() {
        return phone;
    }

    public void setPhone(String phone) {
        this.phone = phone;
    }

    public char getIsPhoneShow() {
        return isPhoneShow;
    }

    public void setIsPhoneShow(char isPhoneShow) {
        this.isPhoneShow = isPhoneShow;
    }
}
