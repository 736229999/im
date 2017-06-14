package com.feiniu.imServer.bean.backend;


import com.feiniu.imServer.bean.dto.TreeDto;

import java.util.List;

/**
 * Created by wanbingwen on 2015/10/23.
 */
public class CSOrganInfo {


    private String code;
    private List<TreeDto> data;


    public String getCode() {
        return code;
    }

    public void setCode(String code) {
        this.code = code;
    }

    public List<TreeDto> getData() {
        return data;
    }

    public void setData(List<TreeDto> data) {
        this.data = data;
    }



}
