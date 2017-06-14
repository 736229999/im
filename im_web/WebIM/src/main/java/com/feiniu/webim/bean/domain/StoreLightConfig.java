package com.feiniu.webim.bean.domain;

/**
 * Created by wangchen on 2015/2/25.
 */
public class StoreLightConfig {
    private long merchantID;

    private char firstWorkDayStart = '1';

    private char firstWorkDayEnd = '1';

    private String firstWorkTimeStart = "00:00";

    private String firstWorkTimeEnd = "00:00";

    private char isFirstWorkTimeShow = '0';

    private char secondWorkDayStart = '1';

    private char secondWorkDayEnd = '1';

    private String secondWorkTimeStart = "00:00";

    private String secondWorkTimeEnd = "00:00";

    private char isSecondWorkTimeShow = '0';

    private String telphone;

    private char isTelphoneShow = '0';

    private String phone;

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
