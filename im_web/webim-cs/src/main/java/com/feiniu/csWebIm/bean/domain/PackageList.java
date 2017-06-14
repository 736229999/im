package com.feiniu.csWebIm.bean.domain;
import java.util.List;
/**
 * Created by chenwuming on 2015/11/16.
 */
public class PackageList{
    private String packNo;
    private Integer packState;
    private String packStateStr;
    private Integer type;
    private String packQueryNo;
    private Integer freight;
    private List<ItemList> itemList;

    public String getPackNo() {
        return packNo;
    }

    public void setPackNo(String packNo) {
        this.packNo = packNo;
    }

    public Integer getPackState() {
        return packState;
    }

    public void setPackState(Integer packState) {
        this.packState = packState;
    }

    public String getPackStateStr() {
        return packStateStr;
    }

    public void setPackStateStr(String packStateStr) {
        this.packStateStr = packStateStr;
    }

    public Integer getType() {
        return type;
    }

    public void setType(Integer type) {
        this.type = type;
    }

    public String getPackQueryNo() {
        return packQueryNo;
    }

    public void setPackQueryNo(String packQueryNo) {
        this.packQueryNo = packQueryNo;
    }

    public Integer getFreight() {
        return freight;
    }

    public void setFreight(Integer freight) {
        this.freight = freight;
    }

    public List<ItemList> getItemList() {
        return itemList;
    }

    public void setItemList(List<ItemList> itemList) {
        this.itemList = itemList;
    }
}
