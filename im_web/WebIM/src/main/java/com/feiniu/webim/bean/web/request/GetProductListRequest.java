package com.feiniu.webim.bean.web.request;

/**
 * Created by wangchen on 2015/3/12.
 */
public class GetProductListRequest {
    private Boolean isPaginator = true;
    private int pageRows;
    private int curPage;
    private String title = null;
    private Boolean isIn = true;
    private Long idStr = null;
    private long merchantId;

    public GetProductListRequest() {}

    public Boolean getIsPaginator() {
        return isPaginator;
    }

    public void setIsPaginator(Boolean isPaginator) {
        this.isPaginator = isPaginator;
    }

    public int getPageRows() {
        return pageRows;
    }

    public void setPageRows(int pageRows) {
        this.pageRows = pageRows;
    }

    public int getCurPage() {
        return curPage;
    }

    public void setCurPage(int curPage) {
        this.curPage = curPage;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public boolean isIsIn() {
        return isIn;
    }

    public void setIsIn(boolean isIn) {
        this.isIn = isIn;
    }

    public Long getIdStr() {
        return idStr;
    }

    public void setIdStr(Long idStr) {
        this.idStr = idStr;
    }

    public long getMerchantId() {
        return merchantId;
    }

    public void setMerchantId(long merchantId) {
        this.merchantId = merchantId;
    }
}
