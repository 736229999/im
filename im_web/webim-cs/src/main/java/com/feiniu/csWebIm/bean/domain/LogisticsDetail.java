package com.feiniu.csWebIm.bean.domain;
import java.util.List;
/**
 * Modified by wangzhen on 2015/11/19.
 */
/////物流信息
public class LogisticsDetail {
    private String query_code;
    private String hd_no;
    private String spv_seq;
    private String post_code;
    private List<ExpInfo> exp_info;
    private String spv_name;

    public String getQuery_code() {
        return query_code;
    }

    public void setQuery_code(String query_code) {
        this.query_code = query_code;
    }

    public String getHd_no() {
        return hd_no;
    }

    public void setHd_no(String hd_no) {
        this.hd_no = hd_no;
    }

    public String getSpv_seq() {
        return spv_seq;
    }

    public void setSpv_seq(String spv_seq) {
        this.spv_seq = spv_seq;
    }

    public String getPost_code() {
        return post_code;
    }

    public void setPost_code(String post_code) {
        this.post_code = post_code;
    }

    public List<ExpInfo> getExp_info() {
        return exp_info;
    }

    public void setExp_info(List<ExpInfo> exp_info) {
        this.exp_info = exp_info;
    }

    public String getSpv_name() {
        return spv_name;
    }

    public void setSpv_name(String spv_name) {
        this.spv_name = spv_name;
    }
}