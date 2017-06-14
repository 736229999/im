package com.feiniu.fnSubAccount.repository;

import com.feiniu.fnSubAccount.bean.request.FAQRequest;
import com.feiniu.fnSubAccount.bean.vo.FAQCategoryConsultDetail;
import com.feiniu.fnSubAccount.domain.FAQCategory;

import java.util.List;

/**
 * Author: morningking
 * Date: 2015/10/12
 * Contact: 243717042@qq.com
 */
public interface FAQCategoryDao {
    //获取问题分类列表
    List<FAQCategory> getFAQCategoryList(FAQRequest faqRequest);

    //改变问题分类显示状态
    void changeFAQCategoryShowState(int faqCategoryID, boolean isShow);

    //删除问题分类项
    void deleteFAQCategory(int faqCategoryID);

    //更新问题分类项
    void updateFAQCategory(FAQCategory faqCategory);

    //插入问题分类项
    void addFAQCategory(FAQCategory faqCategory);

    //按照统计时间，获取各个faq统计次数
    List<FAQCategoryConsultDetail> getFAQCategoryConsultDetailList(Long startTime, Long endTime, String titleFilter);

    //获取问题分类数量
    List<Integer> faqCount();

    //批量保存 问题分类项
    void saveAll(List<FAQCategory> faqList);

    //更新咨询次数
    void updateConsultInfo(int faqCategoryID, long consultDate, int delta);
}
