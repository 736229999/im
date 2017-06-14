package com.feiniu.csWebIm.service;

import com.feiniu.csWebIm.bean.domain.*;
import com.feiniu.csWebIm.bean.request.FAQRequest;
import com.feiniu.csWebIm.bean.response.WebRightResponse;
import com.feiniu.csWebIm.consts.BusinessConfig;
import com.feiniu.csWebIm.consts.SystemConfig;
import com.feiniu.csWebIm.repository.FAQDao;
import com.feiniu.csWebIm.service.external.FAQService;
import com.feiniu.csWebIm.service.external.LogisticsDetailService;
import com.feiniu.csWebIm.service.external.OrderConsultService;
import com.feiniu.csWebIm.service.external.RecentDealsService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by wangzhen on 2015/11/5.
 */
@Service
public class WebRightService {

    private static final Logger LOGGER = LoggerFactory.getLogger(WebRightService.class);

    @Autowired
    private RecentDealsService recentDealsService;//最近一笔订单

    @Autowired
    private LogisticsDetailService logisticsDetailService;//物流信息查询

    @Autowired
    FAQDao faqDao;

    @Autowired
    private BusinessConfig businessConfig;

    @Autowired
    private FAQService faqService;

    private boolean formatPackageListToPackageDetail(PackageDetail packageDetail,PackageList packageList){
        List<ItemList> itemLists = packageList.getItemList();
        List<GoodPicInfo> goodPicInfoList = new ArrayList<>();
        if (itemLists == null)
            return  false;

        for ( ItemList itemIndx: itemLists){
            GoodPicInfo goodPicInfo = new GoodPicInfo();
            goodPicInfo.setPicURL(itemIndx.getPicUrl());//图片
            if (itemIndx.getSourceUrl().indexOf("http") < 0)
                goodPicInfo.setHrefURL(businessConfig.getGoodsItemURL() +itemIndx.getSourceUrl());//详情页
            else
                goodPicInfo.setHrefURL(itemIndx.getSourceUrl());//详情页
            goodPicInfoList.add(goodPicInfo);
        }

        packageDetail.setGoodPicInfoList(goodPicInfoList);
        return true;
    }

    private boolean formatDealsToPackageDetail(RecentDeals recentDeals,WebRightResponse webRightResponse){

        if(recentDeals == null) {
            webRightResponse.setOrderNo("--");
            webRightResponse.setOrderStatus("--");
            webRightResponse.setOrderTime("--");
            webRightResponse.setOrderUrl("--");
            webRightResponse.setPayType("--");
            webRightResponse.setPrice(0.0);
            return false;
        }

        List<PackageList> packDetailList = recentDeals.getPackageList();
        OrderDetail orderDetail = recentDeals.getOrderDetail();
        //容错
        if (orderDetail == null) {
            orderDetail = new OrderDetail();
        }
        webRightResponse.setOrderNo(orderDetail.getOgNo());//订单号
        webRightResponse.setPayType(orderDetail.getPayType());//支付方式
        webRightResponse.setPrice(orderDetail.getTotalPay());//支付金额
        webRightResponse.setOrderTime(orderDetail.getInsertDate());//下单时间
        webRightResponse.setPackageDetailList(new ArrayList<PackageDetail>());

        if(packDetailList == null ){
            webRightResponse.setPackageDetailList(null);
        }else {
            for (PackageList packageDetailIndx: packDetailList){
                PackageDetail packageDetail = new PackageDetail();
                formatPackageListToPackageDetail(packageDetail, packageDetailIndx);
                LogisticsDetail logisticsDetail= logisticsDetailService.getLogisticsInfo(orderDetail.getMemGuid(),
                        packageDetailIndx.getPackQueryNo(), orderDetail.getOgSeq(), packageDetailIndx.getType());
                packageDetail.setLogisticsDetailList(logisticsDetail);
                webRightResponse.getPackageDetailList().add(packageDetail);
            }
        }

        return true;
    }

    public boolean getFAQToWebRightResponse(WebRightResponse webRightResponse){
        ////常见问题
        List<FAQ> resultList = faqService.getFAQList();
        List<CommenQustion> commonQustionsList = new ArrayList<>();
        for (FAQ index:resultList) {
            CommenQustion commenQustion = new CommenQustion();
            commenQustion.setQuestionId(index.getFaqID());
            commenQustion.setQustion(index.getTitle());
            commenQustion.setAnswer(index.getFaqSummary());
            commonQustionsList.add(commenQustion);
        }
        webRightResponse.setCommonQustionsList(commonQustionsList);

        return true;
    }

    public WebRightResponse getPageDetail(String customerId,String askingType,String orderNo){
        WebRightResponse webRightResponse = new WebRightResponse();
        int askingTypeInt = Integer.valueOf(askingType);////区分最近一笔订单和咨询订单的消息

        List<PackageDetail> packageDetailList = new ArrayList<>();
        RecentDeals recentDeals = null;

        if(askingTypeInt == 0)//最近一笔订单
        {
            webRightResponse.setTitle(SystemConfig.WEB_RIGHT_TITILES[askingTypeInt]);
            webRightResponse.setOrderUrl(businessConfig.getMemberServerURL() + SystemConfig.ORDER_DETAIL_URL + orderNo);
            recentDeals = recentDealsService.getRecentDeals(customerId);
        }
        else if(askingTypeInt == 1)//咨询订单信息
        {
            webRightResponse.setTitle(SystemConfig.WEB_RIGHT_TITILES[askingTypeInt]);
            webRightResponse.setOrderUrl(businessConfig.getMemberServerURL() + SystemConfig.ORDER_DETAIL_URL + orderNo);
            ///咨询订单信息
            String  guid =  customerId;
            String  orId =  orderNo;
            recentDeals = recentDealsService.getOrderDeals(orId, guid);
        }

        formatDealsToPackageDetail(recentDeals ,webRightResponse);

        getFAQToWebRightResponse(webRightResponse);

        return webRightResponse;
    }
}
