package com.feiniu.csWebIm.bean.domain;

/**
 * Created by Chenwuming on 2015/10/26.
 */


public class ContactionInfo{

    private String consignee_id ;
    private String con_is_default ;//是否为默认收货地址1-默认，0-非默认
    private String con_name ;//联系人姓名
    private String con_province ;
    private String con_city ;
    private String con_address ;
    private String con_zip;
    private String con_area;
    private String con_telete;
    private String con_call_phone; //联系人电话
    private String con_town;
    private String con_town_code;
    private String con_area_code;
    private String con_city_code;
    private String con_province_code;
    private String longitude;
    private String latitude;
    private String con_upd_memdt;

    public String getCon_upd_memdt() {
        return con_upd_memdt;
    }

    public void setCon_upd_memdt(String con_upd_memdt) {
        this.con_upd_memdt = con_upd_memdt;
    }

    public String getLatitude() {
        return latitude;
    }

    public void setLatitude(String latitude) {
        this.latitude = latitude;
    }

    public String getLongitude() {
        return longitude;
    }

    public void setLongitude(String longitude) {
        this.longitude = longitude;
    }

    public boolean CheckDefaultAddr()
    {
        if(con_is_default.equals("1") == true)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    public String getConsignee_id() {
        return consignee_id;
    }

    public void setConsignee_id(String consignee_id) {
        this.consignee_id = consignee_id;
    }

    public String getCon_is_default() {
        return con_is_default;
    }

    public void setCon_is_default(String con_is_default) {
        this.con_is_default = con_is_default;
    }

    public String getCon_name() {
        return con_name;
    }

    public void setCon_name(String con_name) {
        this.con_name = con_name;
    }

    public String getCon_province() {
        return con_province;
    }

    public void setCon_province(String con_province) {
        this.con_province = con_province;
    }

    public String getCon_city() {
        return con_city;
    }

    public void setCon_city(String con_city) {
        this.con_city = con_city;
    }

    public String getCon_address() {
        return con_address;
    }

    public void setCon_address(String con_address) {
        this.con_address = con_address;
    }

    public String getCon_zip() {
        return con_zip;
    }

    public void setCon_zip(String con_zip) {
        this.con_zip = con_zip;
    }

    public String getCon_area() {
        return con_area;
    }

    public void setCon_area(String con_area) {
        this.con_area = con_area;
    }

    public String getCon_telete() {
        return con_telete;
    }

    public void setCon_telete(String con_telete) {
        this.con_telete = con_telete;
    }

    public String getCon_call_phone() {
        return con_call_phone;
    }

    public void setCon_call_phone(String con_call_phone) {
        this.con_call_phone = con_call_phone;
    }

    public String getCon_town() {
        return con_town;
    }

    public void setCon_town(String con_town) {
        this.con_town = con_town;
    }

    public String getCon_town_code() {
        return con_town_code;
    }

    public void setCon_town_code(String con_town_code) {
        this.con_town_code = con_town_code;
    }

    public String getCon_area_code() {
        return con_area_code;
    }

    public void setCon_area_code(String con_area_code) {
        this.con_area_code = con_area_code;
    }

    public String getCon_city_code() {
        return con_city_code;
    }

    public void setCon_city_code(String con_city_code) {
        this.con_city_code = con_city_code;
    }

    public String getCon_province_code() {
        return con_province_code;
    }

    public void setCon_province_code(String con_province_code) {
        this.con_province_code = con_province_code;
    }
}
