package com.feiniu.imServer.repository;


import com.feiniu.imServer.bean.FAQ.FAQRequest;
import com.feiniu.imServer.bean.FAQ.FAQViewDetail;
import com.feiniu.imServer.domain.FAQ;

import java.util.List;

/**
 * Author: morningking
 * Date: 2015/9/28
 * Contact: 243717042@qq.com
 */
public interface FAQDao {
    //获取faq列表
    List<FAQ> getFAQList(FAQRequest faqRequest);

    //改变faq显示状态
    void changeFAQShowState(int faqID, boolean isShow);

    //删除faq
    void deleteFAQ(int faqID);

    //更新faq
    void updateFAQ(FAQ faq);

    //插入faq
    void addFAQ(FAQ faq);

    //改变faq位置
    void changeFAQOrder(int srcFAQID, int srcOrder, int destFAQID, int destOrder);

    //按照统计时间，获取各个faq统计次数
    List<FAQViewDetail> getFAQViewDetailList(Long startTime, Long endTime, String titleFilter);

    List<Integer> faqCount();

    void saveAll(List<FAQ> faqList);

    void updateBrowseInfo(int faqID);
}
