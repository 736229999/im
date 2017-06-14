package com.feiniu.csWebIm.controller;

import com.feiniu.csWebIm.base.ResponseResult;
import com.feiniu.csWebIm.bean.domain.*;
import com.feiniu.csWebIm.bean.request.SaveServiceLogRequest;
import com.feiniu.csWebIm.bean.request.ServiceLogRequset;
import com.feiniu.csWebIm.bean.request.getServiceLogInfoRequest;
import com.feiniu.csWebIm.bean.response.ServiceLogInfoResponse;
import com.feiniu.csWebIm.consts.BusinessConfig;
import com.feiniu.csWebIm.consts.SystemConfig;
import com.feiniu.csWebIm.repository.ServiceLogDao;
import com.feiniu.csWebIm.service.EndecryptedManager;
import com.feiniu.csWebIm.service.ServiceLogInfoService;
import com.feiniu.csWebIm.service.external.*;
import com.feiniu.csWebIm.utils.FormatUtils;
import com.feiniu.csWebIm.utils.json.JsonNodeUtil;
import com.feiniu.csWebIm.utils.json.JsonUtil;
import com.feiniu.csWebIm.utils.web.HttpUtils;
import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import org.codehaus.jackson.JsonNode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.ServletRequestUtils;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.servlet.http.HttpServletRequest;
import java.io.IOException;
import java.util.*;

/**
 * Created by wangzhen on 2015/10/22.
 */
@Controller
public class ServiceLogInfoController {

    @Autowired
    private ServiceLogInfoService serviceLogInfoService;
    @Autowired
    private UserInfoService userInfoService;

    @Autowired
    private ContactService contactService;//声明

    @Autowired
    private TotalConsumptService totalConsumptService;//累积消费

    @Autowired
    private RecentDealsService recentDealsService;//最近订单

    @Autowired
    private OrderConsultService orderConsultService;//订单咨询

    @Autowired
    private AskingGoodsService askingGoodsService;

    @Autowired
    private  ServiceLogDao serviceLogDao;

    @Autowired
    private BusinessConfig businessConfig;

    @Autowired
    private ProductDetailService productDetailService;//台北ERP获取商品咨询问题

    @Autowired
    private AskingQuestionsService askingQuestionsService;//咨询问题分类

    @Autowired
    private  SaveQuestionsToBackendService saveQuestionsToBackendService;//保存咨询问题到运营后台

    private static final Logger LOGGER = LoggerFactory.getLogger(ServiceLogInfoController.class);
    //获取常用联系人
    @RequestMapping(value = "serviceLogFunction/get-CustomerInfo")
    @ResponseBody
    public ResponseResult getCustomerInfo(HttpServletRequest httpRequest){
        ServiceLogRequset request = (ServiceLogRequset)httpRequest.getAttribute("ServiceLogRequest");

        ResponseResult responseResult = new ResponseResult();
        //获取收货人信息
        CustomerInfo customer = contactService.getContactListById(request.getCustomerId());
        //吞掉异常，显示为默认值
        if (customer == null){
            customer = new CustomerInfo();
            customer.SetDefaultAddr(true);
            customer.setCustomerName("--");
            customer.setCustomerTel("--");
        }
        else
        {
            if(customer.getDefaultAddr() != true) //非空但不是默认地址,从上一笔订单中取
            {
                RecentDeals recentDeals = recentDealsService.getRecentDeals(request.getCustomerId());
                if (recentDeals  == null){
                    customer.setCustomerName("--");
                    customer.setCustomerTel("--");
                }else {
                    OrderDetail orderDetail = recentDeals.getOrderDetail();
                    customer.setCustomerName(orderDetail.getMemberName());
                    customer.setCustomerTel(orderDetail.getMemberCellphone());
                }
            }
        }
        responseResult.addData("connectInfo", customer);
        responseResult.markSuccess();
        return responseResult;
    }

    //获取用户信息(用户名，昵称，会员等级，累计消费信息)
    @RequestMapping(value = "serviceLogFunction/get-UserInfo")
    @ResponseBody
    public ResponseResult getUserInfo(HttpServletRequest httpRequest){
        ResponseResult responseResult = new ResponseResult();
        ServiceLogRequset request = (ServiceLogRequset)httpRequest.getAttribute("ServiceLogRequest");

        //获取用户信息
        UserInfo userInfo = userInfoService.getUserInfoById(request.getCustomerId());
        UserLevel userLevel = userInfoService.getUserLevelInfoById(request.getCustomerId());
        if (userInfo == null){
            userInfo = new UserInfo();
        }
        if (userLevel == null){
            userLevel = new UserLevel();
        }

        userInfo.formatToshow();
        userLevel.formatToshow();
        responseResult.addData("userInfo", userInfo);
        responseResult.addData("userLevel", userLevel);
        responseResult.markSuccess();
        return responseResult;
    }


    @RequestMapping(value = "serviceLogFunction/get-OrderDetail")
    @ResponseBody
    public ResponseResult getOrderDetail(HttpServletRequest httpRequest) {
        ResponseResult responseResult = new ResponseResult();
        ServiceLogRequset request = (ServiceLogRequset)httpRequest.getAttribute("ServiceLogRequest");

        ServiceLogInfoResponse response = new ServiceLogInfoResponse();
        List<AskingPic> askingPictures =new ArrayList<AskingPic>();
        //获取最近一笔订单
        //获取咨询商品信息
        if (request.getGoodsAskingType().equals("0") ){
            //购物车,不需要订单信息
            List<AskingGoodsCart> askingGoodsCartList =askingGoodsService.getCartForCustomer(request.getCustomerId());
            //图片改造没完成
            if(askingGoodsCartList != null) {
                for (AskingGoodsCart index : askingGoodsCartList) {
                    AskingPic picIndex = new AskingPic();
                    picIndex.setUrl(SystemConfig.HTTP_HEAD + businessConfig.getGoodsItemURL() + SystemConfig.ITEM_HEAD + index.getSmSeq());

                    String picSubURL = index.getPic();
                    picSubURL = picSubURL.substring(0, picSubURL.length() - 4);
                    picIndex.setPicUrl(SystemConfig.HTTPS_HEAD + businessConfig.getGoodsImgURL() + picSubURL + SystemConfig.PIC_SIZE);
                    askingPictures.add(picIndex);
                }
            }
        }else if(request.getGoodsAskingType().equals("1")){
            //订单，需要获取订单编号和状态，还需要获取咨询列表
            ConsultOrderInfo consultOrderInfo = orderConsultService.getOrderConsult(request.getOrId(),request.getCustomerId());
            if (consultOrderInfo != null){
                for (ConsultPackageList consultPackageListIndex : consultOrderInfo.getPackageList()){
                    for (ConsultItemList consultItemListIndex:consultPackageListIndex.getItemList()){
                        AskingPic picIndex = new AskingPic();

                        //图片地址
                        picIndex.setPicUrl(consultItemListIndex.getPicUrl());

                        //商品详情页
                        String strPicSource= consultItemListIndex.getSourceUrl();
                        if(strPicSource.indexOf(SystemConfig.HTTP_HEAD) !=-1)
                        {
                            picIndex.setUrl(strPicSource);
                        }
                        else
                        {
                            picIndex.setUrl(businessConfig.getGoodsItemURL() +consultItemListIndex.getSourceUrl());
                        }
                        askingPictures.add(picIndex);
                    }
                }
                responseResult.addData("ogSeq",consultOrderInfo.getOrderDetail().getOgSeq());
            }
        }else{
            //否则不显示
        }

        responseResult.addData("askingPictures", askingPictures);
        responseResult.markSuccess();
        return responseResult;
    }

    //获取咨询问题选择 --运营后台接口
    @RequestMapping(value = "serviceLogFunction/get-AskingOpts")
    @ResponseBody
    public ResponseResult getAskingOpts(HttpServletRequest httpRequest) {
        ServiceLogRequset request = (ServiceLogRequset)httpRequest.getAttribute("ServiceLogRequest");
        ResponseResult responseResult = new ResponseResult();

        AskingQuestion askingQuestion= askingQuestionsService.getAskingQuestion();
        List<FaqCategoryList> faqCategoryList= askingQuestionsService.getQuestionList(askingQuestion);

        List<AskingOpt> AskingOpts =new ArrayList<AskingOpt>();
        //for(FaqCategoryList index : faqCategoryList)
        for (int i = 0; i < faqCategoryList.size(); i++)
        {
            String strRealID = String.valueOf(i + 1);     //台北ERP存储真实ID（1，2，3...13)
            String strCatgoryID = faqCategoryList.get(i).getCategoryID();//运营后台DB存储ID
            String strTitle = faqCategoryList.get(i).getTitle();//运营后台存储标题

            AskingOpt askingOpt = new AskingOpt();
            askingOpt.setText(strTitle);
            askingOpt.setChecked(false);
            askingOpt.setId(strCatgoryID);
            askingOpt.setRealID(strRealID);
            AskingOpts.add(askingOpt);
        }

        responseResult.addData("askingOpts", AskingOpts);
        responseResult.markSuccess();
        return responseResult;
    }

    //获取台北商品咨询详情
    @RequestMapping(value = "serviceLogFunction/get-AskingInfos")
    @ResponseBody
    public ResponseResult getAskingInfos(HttpServletRequest httpRequest){
        ResponseResult responseResult = new ResponseResult();
        ServiceLogRequset request = (ServiceLogRequset)httpRequest.getAttribute("ServiceLogRequest");

        List<ProductData> productDataList = productDetailService.getProductDetail(request.getOrId());
        List<AskingInfo> askingInfosShow = new ArrayList<>();
        if (productDataList == null){
            responseResult.addData("askingInfos", askingInfosShow);
            responseResult.markSuccess();
            return responseResult;
        }
        for (ProductData index : productDataList){
            askingInfosShow.add(FormatUtils.convertToAskingInfoShow(index));
        }
        responseResult.addData("askingInfos", askingInfosShow);
        responseResult.markSuccess();
        return responseResult;
    }

    //获取历史小记列表
    @RequestMapping(value = "serviceLogFunction/get-HistoryLogs")
    @ResponseBody
    public ResponseResult getHistoryLogs(HttpServletRequest httpRequest){
        ResponseResult responseResult = new ResponseResult();
        ServiceLogRequset request = (ServiceLogRequset)httpRequest.getAttribute("ServiceLogRequest");

        List<HistoryServiceLog> HistoryServiceLogs = serviceLogDao.getLastTenHistoryLog(request.getCustomerId());

        responseResult.addData("historyServiceLog", HistoryServiceLogs);
        responseResult.markSuccess();
        return responseResult;
    }

    //保存服务小记到台北ERP存储
    @RequestMapping(value = "serviceLogSave/save-serviceLog")
    @ResponseBody
    public ResponseResult saveResponse( SaveServiceLogRequest request) {
        ResponseResult responseResult = new ResponseResult();

        String decodedCustomerToken = EndecryptedManager.INSTANCE.decryptedWithURLDncode(request.getCustomerUUID());
        String encodedCustomerId = decodedCustomerToken.substring(0, SystemConfig.DEFAULT_ENCODE_SERVICE_ID);
        request.setCustomerUUID(encodedCustomerId);

        ////从运营后台获取咨询问题，然后回传勾选的项到运营后台
        Gson gson = new Gson();
        List<AskingOpt> askingOpts = gson.fromJson(request.getAskingOptions(), new TypeToken<List<AskingOpt>>() {
        }.getType());
        if(askingOpts == null) {
            responseResult.setErrorCode(-2);
            responseResult.setMessage("获取咨询问题失败");
            return responseResult;
        }

        request.setAskingOptionsList(askingOpts);

        ////获取页面选择的咨询问题
        String categoryIDs = "";
        List<AskingOpt> askingOptsFilter = new ArrayList<AskingOpt>();//保存选中项

        LOGGER.info("保存服务小计：质询问题个数 "+askingOpts.size());

        for(AskingOpt index :askingOpts)
        {
            boolean bChecked = index.isChecked();
            AskingOpt askingOpt = new AskingOpt();
            if (bChecked)
            {
                askingOpt.setId(index.getId());
                askingOptsFilter.add(askingOpt);
            }
        }

        for (int i = 0; i < askingOptsFilter.size(); i++)
        {
            AskingOpt askingOpt = askingOptsFilter.get(i);
            String categoryID = "";
            if(i!=askingOptsFilter.size()-1)
            {
                categoryID = askingOpt.getId() + ",";
            }
            else
            {
                categoryID = askingOpt.getId();
            }
            categoryIDs += categoryID;
        }

        ////保存咨询问题分类到运营后台
        if (!saveQuestionsToBackendService.SaveQuestionToBackend(categoryIDs)){
            responseResult.setErrorCode(-2);
            responseResult.setMessage("保存咨询问题到商家后台失败");
            return responseResult;
        }

        ///////解析服务小记参数
        ////存储咨询商品信息
        List<AskingInfo> askingInfos = gson.fromJson(request.getAskingGoodsInfo(), new TypeToken<List<AskingInfo>>() {
        }.getType());
        if(askingInfos == null) {
            responseResult.setErrorCode(-2);
            responseResult.setMessage("获取咨询的商品信息失败");
            return responseResult;
        }
        request.setAskingGoodsInfoList(askingInfos);
        LOGGER.info("保存服务小计：质询信息个数 " + askingOpts.size());
        ///
        if (!serviceLogInfoService.formatRequstJsonToList(request)){
            responseResult.setErrorCode(-2);
            responseResult.setMessage("解析传入参数失败");
            return responseResult;
        }

        //保存服务小记
        ServicelogInfo servicelogInfo = new ServicelogInfo();
        servicelogInfo.setContact_name(request.getSavedName());//收件人姓名
        servicelogInfo.setContact_phone(request.getSavedTel());//收件人电话
        //类型
        if(request.getSavedType().equals("服务查询"))
        {
            servicelogInfo.setService_type("1");
        }
        else
        {
            servicelogInfo.setService_type("2");
        }

        //优先级
        if(request.getSavedLevel().equals("普通"))
        {
            servicelogInfo.setPriority("0");
        }
        else    if(request.getSavedLevel().equals("急件"))
        {
            servicelogInfo.setPriority("1");
        }

        //状态
        if(request.getSavedStatus().equals("处理中"))
        {
            servicelogInfo.setCsl_status("1");
        }
        else  if(request.getSavedStatus().equals("已结案"))
        {
            servicelogInfo.setCsl_status("2");
        }
        else
        {
            servicelogInfo.setCsl_status("3");
        }

        servicelogInfo.setCsm_summary(request.getServiceLog());//联系小记

        servicelogInfo.setRec_usr_seq(request.getServiceName());//客服真实姓名(哞哞ID获取)
        servicelogInfo.setUsr_seq(request.getServiceName());    //客服真实姓名(哞哞ID获取)
        servicelogInfo.setOg_seq(request.getOgSeq());//订单流水号(可以不填)
        //默认字段
        servicelogInfo.setProcess_unit("1");//客服
        servicelogInfo.setCsm_source("3");//在线客服
        /////////
        //咨询问题
        List<Cstvalue> cstvalueList = new ArrayList<Cstvalue>();
        for(AskingOpt index :askingOpts )
        {
            Cstvalue cstvalue = new Cstvalue();
            boolean bChecked = index.isChecked();
            if (bChecked) {
                cstvalue.setType(index.getRealID());///真实的分类问题ID
                cstvalue.setValue(index.getText());
                cstvalueList.add(cstvalue);
            }
        }
        servicelogInfo.setCst_value(cstvalueList);

        /////////
        //咨询商品
        String ogSnos = "";////选择项的OG_SNO
        String SellerTypes = "";
        if (request.getOgSeq() != null) {
            List<AskingInfo> askingInFilter = new ArrayList<AskingInfo>();//保存选中项
            for (AskingInfo index : askingInfos) {
                boolean bChecked = index.isChecked();
                AskingInfo askingInfo = new AskingInfo();
                if (bChecked) {
                    askingInfo.setOgsno(index.getOgsno());
                    askingInfo.setSellType(index.getSellType());
                    askingInFilter.add(askingInfo);
                }
            }
            for (int j = 0; j < askingInFilter.size() && j < 14; j++) {
                String ogSno = "";
                String SellerType = "";//出货者类型
                AskingInfo askingInfo = new AskingInfo();
                askingInfo = askingInFilter.get(j);
                if (j != askingInFilter.size() - 1) {
                    ogSno = askingInfo.getOgsno() + ",";
                    SellerType = askingInfo.getSellType() + "/";
                } else {
                    ogSno = askingInfo.getOgsno();
                    SellerType = askingInfo.getSellType();
                }
                ogSnos += ogSno;//ID列表
                SellerTypes += SellerType;//出货者类型
            }
            servicelogInfo.setChecked_item(ogSnos);//选中商品ID列表
            servicelogInfo.setChecked_ds_type(SellerTypes);//出货者类型
        }

        ////保存服务小记到台北ERP
        if (!serviceLogInfoService.getSaveServicelogService(servicelogInfo)){
            responseResult.setErrorCode(-2);
            responseResult.setMessage("解析传入参数失败");
            return responseResult;
        }
        ////
        Calendar calendar = Calendar.getInstance();
        Long saveTime= calendar.getTimeInMillis()/ 1000;

        int result = serviceLogDao.saveServiceLog(String.valueOf(saveTime),request.getCustomerUUID(),
                request.getServiceName(),request.getServiceLog());

        if (result <0){
            responseResult.setMessage("写入哞哞数据库失败");
            return  responseResult;
        }
        responseResult.markSuccess();
        return responseResult;
    }


    //获取客户累计消费信息
    @RequestMapping(value = "serviceLogFunction/get-TotalCostInfo")
    @ResponseBody
    public ResponseResult getTotalConsumptService(HttpServletRequest httpRequest){
        ResponseResult responseResult = new ResponseResult();
        ServiceLogRequset request = (ServiceLogRequset)httpRequest.getAttribute("ServiceLogRequest");

        ServiceLogInfoResponse response = new ServiceLogInfoResponse();
        CustomerCost totalCost = totalConsumptService.getTotalCost(request.getCustomerId());
        if (totalCost == null)
        {
            response.setAllDealCount("--");//订单笔数
            response.setTotalGoodsNum("--");//商品件数
            response.setMeanPrice("--");//客单价
        }
        else
        {
            response.setAllDealCount(totalCost.getOrderTotal().toString());//订单笔数
            response.setTotalGoodsNum(totalCost.getCommodityTotal().toString());//商品件数
            response.setMeanPrice(totalCost.getMeanPrice());//客单价
        }
        responseResult.addData("totalCost", response);
        responseResult.markSuccess();
        return responseResult;
    }

    //获取客户最近一笔订单信息
    @RequestMapping(value = "serviceLogFunction/get-RecentDealsService")
    @ResponseBody
    public ResponseResult getRecentDealsService(HttpServletRequest httpRequest){
        ResponseResult responseResult = new ResponseResult();
        ServiceLogRequset request = (ServiceLogRequset)httpRequest.getAttribute("ServiceLogRequest");

        RecentDeals recentDeals = recentDealsService.getRecentDeals(request.getCustomerId());
        if (recentDeals == null){
            OrderDetail recentDetail = new  OrderDetail();
            recentDetail.setInsertDate("--");
            recentDetail.setOgNo("--");
            recentDetail.setOrderPrice(0.0);//客户端显示订单金额
            responseResult.addData("recentDetail", recentDetail);
            responseResult.markSuccess();
            return responseResult;
        }
        OrderDetail recentDetail = recentDeals.getOrderDetail();
        if (recentDetail == null)
        {
            recentDetail.setInsertDate("--");
            recentDetail.setOgNo("--");
            recentDetail.setOrderPrice(0.0);
        }
        responseResult.addData("recentDetail", recentDetail);
        responseResult.markSuccess();
        return responseResult;
    }

    //咨询订单信息(哞哞右侧只显示主图和详情页)

    //快捷短语
    @RequestMapping(value = "serviceProfile/quick-phrase")
    @ResponseBody
    public ResponseResult getQuickPhrase(HttpServletRequest httpRequest){
        ResponseResult responseResult = new ResponseResult();
        Map<String, Object> map = new HashMap<String, Object>();
        try {
            String response = HttpUtils.excutePost(businessConfig.getKefuBackendURL() + "get-quick-phrase",map);
            JsonNode jsonNode = JsonUtil.toNode(response);
            if(jsonNode != null) {
                if("true".equals(JsonNodeUtil.getString(jsonNode,"success")))
                {
                    JsonNode jsonQuickPhrase = jsonNode.get("data").get("quickPhraseList");
                    responseResult.addData("quickPhraseList", jsonQuickPhrase);
                    responseResult.markSuccess();
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        return responseResult;
    }
}
