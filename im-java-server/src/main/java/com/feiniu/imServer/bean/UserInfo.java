package com.feiniu.imServer.bean;

/**
 * Author: morningking
 * Date: 2016/3/1
 * Contact: 243717042@qq.com
 */
public class UserInfo {
    int id;              // 用户id
    String username;     // 用户名
    String nickname;     // 呢称
    String email;        // 电子邮件
    String birthday;     // 出生日期
    byte sex;            // 性别
    byte province;       // 省
    byte country;        // 国家
    byte city;           // 城市
    String face_url;     // 头像URL
    byte career;         // 职业
    byte rule;           // 加为好友的认证方式
    byte familyflag;     // 列为家族成员认证方式
    int gold_money;      // 金币数目
    byte online_time;    // 在线时长
    byte linkpop;        // 联系方式隐私属性（0 私有;1 好友可见；3 公开）
    String address;      // 联系地址
    String mobile;       // 手机
    String description;  //个人说明
    String usersign;     //个人签名
    String avatoruri;    //闪秀地址
    byte mobilestatus;   //用户手机状态 0未绑定，1已绑定 2已绑定并且订制了业务
    int integral;        //用户积分
    String guid;
    String memLevel;     //用户等级

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getNickname() {
        return nickname;
    }

    public void setNickname(String nickname) {
        this.nickname = nickname;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getBirthday() {
        return birthday;
    }

    public void setBirthday(String birthday) {
        this.birthday = birthday;
    }

    public byte getSex() {
        return sex;
    }

    public void setSex(byte sex) {
        this.sex = sex;
    }

    public byte getProvince() {
        return province;
    }

    public void setProvince(byte province) {
        this.province = province;
    }

    public byte getCountry() {
        return country;
    }

    public void setCountry(byte country) {
        this.country = country;
    }

    public byte getCity() {
        return city;
    }

    public void setCity(byte city) {
        this.city = city;
    }

    public String getFace_url() {
        return face_url;
    }

    public void setFace_url(String face_url) {
        this.face_url = face_url;
    }

    public byte getCareer() {
        return career;
    }

    public void setCareer(byte career) {
        this.career = career;
    }

    public byte getRule() {
        return rule;
    }

    public void setRule(byte rule) {
        this.rule = rule;
    }

    public byte getFamilyflag() {
        return familyflag;
    }

    public void setFamilyflag(byte familyflag) {
        this.familyflag = familyflag;
    }

    public int getGold_money() {
        return gold_money;
    }

    public void setGold_money(int gold_money) {
        this.gold_money = gold_money;
    }

    public byte getOnline_time() {
        return online_time;
    }

    public void setOnline_time(byte online_time) {
        this.online_time = online_time;
    }

    public byte getLinkpop() {
        return linkpop;
    }

    public void setLinkpop(byte linkpop) {
        this.linkpop = linkpop;
    }

    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public String getMobile() {
        return mobile;
    }

    public void setMobile(String mobile) {
        this.mobile = mobile;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public String getUsersign() {
        return usersign;
    }

    public void setUsersign(String usersign) {
        this.usersign = usersign;
    }

    public String getAvatoruri() {
        return avatoruri;
    }

    public void setAvatoruri(String avatoruri) {
        this.avatoruri = avatoruri;
    }

    public byte getMobilestatus() {
        return mobilestatus;
    }

    public void setMobilestatus(byte mobilestatus) {
        this.mobilestatus = mobilestatus;
    }

    public int getIntegral() {
        return integral;
    }

    public void setIntegral(int integral) {
        this.integral = integral;
    }

    public String getGuid() {
        return guid;
    }

    public void setGuid(String guid) {
        this.guid = guid;
    }

    public String getMemLevel() {
        return memLevel;
    }

    public void setMemLevel(String memLevel) {
        this.memLevel = memLevel;
    }
}
