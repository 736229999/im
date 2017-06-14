package com.feiniu.fnSubAccount.bean.request;

/**
 * Created by zhangmin on 2015/10/30.
 */
public class QuickPhraseRequest {

    private String quickCode;
    private String quickPhrasePart;
    private String phraseGroupNamePart;
    private int currentPageNo = 1;
    private int pageSize = 10;

    public String getPhraseGroupNamePart() {
        return phraseGroupNamePart;
    }

    public void setPhraseGroupNamePart(String phraseGroupNamePart) {
        this.phraseGroupNamePart = phraseGroupNamePart;
    }

    public String getQuickPhrasePart() {
        return quickPhrasePart;
    }

    public void setQuickPhrasePart(String quickPhrasePart) {
        this.quickPhrasePart = quickPhrasePart;
    }

    public String getQuickCode() {
        return quickCode;
    }

    public void setQuickCode(String quickCode) {
        this.quickCode = quickCode;
    }

    public int getCurrentPageNo() {
        return currentPageNo;
    }

    public void setCurrentPageNo(int currentPageNo) {
        this.currentPageNo = currentPageNo;
    }

    public int getPageSize() {
        return pageSize;
    }

    public void setPageSize(int pageSize) {
        this.pageSize = pageSize;
    }
}
