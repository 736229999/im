package com.feiniu.fnSubAccount.bean;

/**
 * Created by wanbingwen on 2015/11/2.
 */
public class CSInfoCountMemcache {
    private Integer WaitCustomer;
    private Integer CServiceNum;
    private Integer OnlineCService;
    private Integer OffLineCService;
    private Integer ConsultCService;
    private Integer RecptionCService;

    public Integer getWaitCustomer() {
        return WaitCustomer;
    }

    public void setWaitCustomer(Integer waitCustomer) {
        WaitCustomer = waitCustomer;
    }

    public  Integer getCServiceNum() {
        return CServiceNum;
    }

    public void setCServiceNum(Integer CServiceNum) {
        this.CServiceNum = CServiceNum;
    }

    public Integer getOnlineCService() {
        return OnlineCService;
    }

    public void setOnlineCService(Integer onlineCService) {
        OnlineCService = onlineCService;
    }

    public Integer getOffLineCService() {
        return OffLineCService;
    }

    public void setOffLineCService(Integer offLineCService) {
        OffLineCService = offLineCService;
    }

    public Integer getConsultCService() {
        return ConsultCService;
    }

    public void setConsultCService(Integer consultCService) {
        ConsultCService = consultCService;
    }

    public Integer getRecptionCService() {
        return RecptionCService;
    }

    public void setRecptionCService(Integer recptionCService) {
        RecptionCService = recptionCService;
    }

    public void addToSelf(CSInfoCountMemcache csInfoCountMemcache){
        this.WaitCustomer = this.WaitCustomer + csInfoCountMemcache.getWaitCustomer();
        this.CServiceNum = this.CServiceNum + csInfoCountMemcache.getCServiceNum();
        this.OnlineCService = this.OnlineCService+csInfoCountMemcache.getOnlineCService();
        this.OffLineCService= this.OffLineCService + csInfoCountMemcache.getOffLineCService();
        this.ConsultCService = this.ConsultCService + csInfoCountMemcache.getConsultCService();
        this.RecptionCService = this.RecptionCService + csInfoCountMemcache.getRecptionCService();
    }
}
