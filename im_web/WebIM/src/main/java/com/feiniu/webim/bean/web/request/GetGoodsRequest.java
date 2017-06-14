package com.feiniu.webim.bean.web.request;

/**
 * Created by zhangmin on 2015/12/9.
 */
public class GetGoodsRequest {

    //定义：关键词和SM_SEQ。SM_SEQ支持多值，它们是OR关系，以","分割默认（不指定）：不指定关键词;
    // q=袜子,xxxxx,xxxxx
    private String keyWord;
    //定义：每次返回商品数量; 默认值（不指定）：36; resnum=20
    private Integer pageRows = 10;
    //定义：第几页。根据主流电商做法，最大值100，也就是最多返回前100页;
    // 默认值（不指定）：1; pn=2
    private Integer curPage = 1;
   //"定义：店铺搜索，值是店铺id ;默认值（不指定）：全站搜索"; shopid=xxxx
    private Long merchantId;
    //定义：排序方式，recommend表示综合，sm_soldqty表示销量，sm_price表示价格(有促销价时按促销价)，sm_seq表示上架时间。
    // GB_START_TIME表示团购开始时间。GB_SOLDQTY表示团购销量。ALL_GB_POPULAR表示团购人气。favorite 表示收藏
    //默认值（不指定）：recommend
    String sort = "recommend";
    //"定义：升序或降序，对综合、销量和新品排序不生效。1表示升序，0表示降序
    //默认值（不指定）：0，降序。"
    Integer asc = 0;


    public String getKeyWord() {
        return keyWord;
    }

    public void setKeyWord(String keyWord) {
        this.keyWord = keyWord;
    }

    public Integer getPageRows() {
        return pageRows;
    }

    public void setPageRows(Integer pageRows) {
        this.pageRows = pageRows;
    }

    public Integer getCurPage() {
        return curPage;
    }

    public void setCurPage(Integer curPage) {
        this.curPage = curPage;
    }

    public Long getMerchantId() {
        return merchantId;
    }

    public void setMerchantId(Long merchantId) {
        this.merchantId = merchantId;
    }

    public String getSort() {
        return sort;
    }

    public void setSort(String sort) {
        this.sort = sort;
    }

    public Integer getAsc() {
        return asc;
    }

    public void setAsc(Integer asc) {
        this.asc = asc;
    }
}
