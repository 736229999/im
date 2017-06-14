package com.feiniu.imServer.domain;

import org.hibernate.annotations.GenericGenerator;

import javax.persistence.*;

/**
 * Created by wangchen on 2015/2/3.
 */
@Entity
@Table(name = "t_service_group")
public class ServiceGroup {
    @Id
    @Column(name = "g_group_id")
    private long groupID;

    @Column(name = "g_group_name", length = 50, nullable = false)
    private String groupName;

    @Column(name = "g_merchant_id", nullable = false)
    private long merchantID;

    @Column(name = "g_is_show", nullable = false)
    private char isShowOnStore = '0';

    @Column(name = "g_show_order", nullable = false)
    private int showOrder = 0;

    public long getGroupID() {
        return groupID;
    }

    public void setGroupID(long groupID) {
        this.groupID = groupID;
    }

    public String getGroupName() {
        return groupName;
    }

    public void setGroupName(String groupName) {
        this.groupName = groupName;
    }

    public long getMerchantID() {
        return merchantID;
    }

    public void setMerchantID(long merchantID) {
        this.merchantID = merchantID;
    }

    public char getIsShowOnStore() {
        return isShowOnStore;
    }

    public void setIsShowOnStore(char isShowOnStore) {
        this.isShowOnStore = isShowOnStore;
    }

    public int getShowOrder() {
        return showOrder;
    }

    public void setShowOrder(int showOrder) {
        this.showOrder = showOrder;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        ServiceGroup that = (ServiceGroup) o;
        if (groupID != that.groupID) return false;

        return true;
    }

}

