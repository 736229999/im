package com.feiniu.csWebIm.bean.domain;

/**
 * Created by Chenwuming on 2015/11/25.
 */
import java.util.List;

public class ServicelogInfo {
    private String contact_name;
    private String contact_phone;
    private String priority;
    private String rec_usr_seq;
    private String usr_seq;
    private String process_unit;
    private String csl_status;
    private String og_seq;
    private String csm_source;
    private String csm_value;
    private String service_type;
    private List<Cstvalue> cst_value;
    private String checked_item;
    private String checked_ds_type;
    private String csm_summary;
    private String csm_process;
    private String mem_guid;
    private String is_overseas;
    private String is_fresh_prod;
    public String getContact_name() {
        return contact_name;
    }

    public void setContact_name(String contact_name) {
        this.contact_name = contact_name;
    }

    public String getContact_phone() {
        return contact_phone;
    }

    public void setContact_phone(String contact_phone) {
        this.contact_phone = contact_phone;
    }

    public String getPriority() {
        return priority;
    }

    public void setPriority(String priority) {
        this.priority = priority;
    }

    public String getRec_usr_seq() {
        return rec_usr_seq;
    }

    public void setRec_usr_seq(String rec_usr_seq) {
        this.rec_usr_seq = rec_usr_seq;
    }

    public String getUsr_seq() {
        return usr_seq;
    }

    public void setUsr_seq(String usr_seq) {
        this.usr_seq = usr_seq;
    }

    public String getProcess_unit() {
        return process_unit;
    }

    public void setProcess_unit(String process_unit) {
        this.process_unit = process_unit;
    }

    public String getCsl_status() {
        return csl_status;
    }

    public void setCsl_status(String csl_status) {
        this.csl_status = csl_status;
    }

    public String getOg_seq() {
        return og_seq;
    }

    public void setOg_seq(String og_seq) {
        this.og_seq = og_seq;
    }

    public String getCsm_source() {
        return csm_source;
    }

    public void setCsm_source(String csm_source) {
        this.csm_source = csm_source;
    }

    public String getCsm_value() {
        return csm_value;
    }

    public void setCsm_value(String csm_value) {
        this.csm_value = csm_value;
    }

    public String getService_type() {
        return service_type;
    }

    public void setService_type(String service_type) {
        this.service_type = service_type;
    }

    public String getMem_guid() {
        return mem_guid;
    }

    public void setMem_guid(String mem_guid) {
        this.mem_guid = mem_guid;
    }

    public String getIs_overseas() {
        return is_overseas;
    }

    public void setIs_overseas(String is_overseas) {
        this.is_overseas = is_overseas;
    }

    public String getIs_fresh_prod() {
        return is_fresh_prod;
    }

    public void setIs_fresh_prod(String is_fresh_prod) {
        this.is_fresh_prod = is_fresh_prod;
    }

    public List<Cstvalue> getCst_value() {
        return cst_value;
    }

    public void setCst_value(List<Cstvalue> cst_value) {
        this.cst_value = cst_value;
    }

    public String getChecked_item() {
        return checked_item;
    }

    public void setChecked_item(String checked_item) {
        this.checked_item = checked_item;
    }

    public String getChecked_ds_type() {
        return checked_ds_type;
    }

    public void setChecked_ds_type(String checked_ds_type) {
        this.checked_ds_type = checked_ds_type;
    }

    public String getCsm_summary() {
        return csm_summary;
    }

    public void setCsm_summary(String csm_summary) {
        this.csm_summary = csm_summary;
    }

    public String getCsm_process() {
        return csm_process;
    }

    public void setCsm_process(String csm_process) {
        this.csm_process = csm_process;
    }
}