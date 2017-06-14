package com.feiniu.fnSubAccount.bean;

import java.io.Serializable;

/**
 * 描述:
 *
 * @author fanbeibei
 * @since 2014年12月23日
 */

public class Merchant  {

    /**
     * 主键
     */
    private Long id;
    /**
     * 公司名称
     */
    private String name;
    /**
     * 申请店铺名称
     */
    private String shopName;
    /**
     * 主营类目名称
     */
    private String mainCategoryName;


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

    public String getMainCategoryName() {
        return mainCategoryName;
    }

    public void setMainCategoryName(String mainCategoryName) {
        this.mainCategoryName = mainCategoryName;
    }

    static public boolean Merchant_equals(Merchant a, Merchant b) {
        if (b == null || a == null) return false;
        return a.id.equals(b.getId()) && a.name.equals(b.getName()) &&
                a.shopName.equals(b.getShopName());
    }
}