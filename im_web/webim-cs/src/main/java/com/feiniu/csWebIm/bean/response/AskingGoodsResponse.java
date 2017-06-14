package com.feiniu.csWebIm.bean.response;

import com.feiniu.csWebIm.bean.domain.AskingGoodsCart;

import java.util.List;

/**
 * Created by wangzhen on 2015/10/29.
 */
public class AskingGoodsResponse {
    private String code;
    private String msg;
    private List<AskingGoodsCart> data;
    private String miniCartSum;
    private String resultList;

    public String getMiniCartSum() {
        return miniCartSum;
    }

    public void setMiniCartSum(String miniCartSum) {
        this.miniCartSum = miniCartSum;
    }

    public String getResultList() {
        return resultList;
    }

    public void setResultList(String resultList) {
        this.resultList = resultList;
    }

    public String getCode() {
        return code;
    }

    public void setCode(String code) {
        this.code = code;
    }

    public String getMsg() {
        return msg;
    }

    public void setMsg(String msg) {
        this.msg = msg;
    }

    public List<AskingGoodsCart> getData() {
        return data;
    }

    public void setData(List<AskingGoodsCart> data) {
        this.data = data;
    }
}
