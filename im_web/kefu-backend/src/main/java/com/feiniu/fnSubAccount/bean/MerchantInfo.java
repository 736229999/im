package com.feiniu.fnSubAccount.bean;

import javax.persistence.Column;
import javax.persistence.Id;
import javax.persistence.SequenceGenerator;
import java.io.Serializable;

/**
 * Created by wangzhen on 2015/7/23.
 */
public class MerchantInfo implements Serializable {

    private static final long serialVersionUID = 3447774250907304863L;


    @Id
    @SequenceGenerator(name = "mall_merchant")
    @Column(name = "id")
    private Long id;
    
    @Column(name = "name")
    private String name;


    @Column(name = "shop_name")
    private String shopName;


    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getShopName() {
        return shopName;
    }

    public void setShopName(String shopName) {
        this.shopName = shopName;
    }

    static public boolean Merchant_equals(MerchantInfo a, MerchantInfo b) {
        if (b == null || a == null) return false;
        return a.id.equals(b.getId()) && a.name.equals(b.getName()) &&
                a.shopName.equals(b.getShopName());
    }
}