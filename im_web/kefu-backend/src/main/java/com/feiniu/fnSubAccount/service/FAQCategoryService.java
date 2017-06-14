package com.feiniu.fnSubAccount.service;

import com.feiniu.fnSubAccount.bean.request.FAQRequest;
import com.feiniu.fnSubAccount.bean.vo.FAQCategoryConsultDetail;
import com.feiniu.fnSubAccount.domain.FAQCategory;
import com.feiniu.fnSubAccount.repository.FAQCategoryDao;
import org.apache.commons.lang.time.DateUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * Author: morningking
 * Date: 2015/11/25
 * Contact: 243717042@qq.com
 */
@Service
public class FAQCategoryService {
    @Autowired
    private FAQCategoryDao faqCategoryDao;
    @Autowired
    private CacheClientService cacheClientService;

    private static final FAQRequest GET_ALL_SHOWED_FAQ_CATEGORY_LIST = new FAQRequest();
    private static final String GET_ALL_SHOWD_FAQ_CATEGORY_LIST_KEY = "FAQCategory#allList";
    private static final Logger LOGGER = LoggerFactory.getLogger(FAQCategoryService.class);

    //咨询次数
    private ConcurrentHashMap<Integer, ConcurrentHashMap<Long, AtomicInteger>> dayToConsultNumMap = new ConcurrentHashMap<>(1);

    static {
        GET_ALL_SHOWED_FAQ_CATEGORY_LIST.setIsShow(true);
        GET_ALL_SHOWED_FAQ_CATEGORY_LIST.setPageSize(Integer.MAX_VALUE);
    }

    //获取所有已显示的常见分类
    public List<FAQCategory> getFAQCategoryListForMoumou() {
        List<FAQCategory> faqCategoryList = cacheClientService.getListValue(GET_ALL_SHOWD_FAQ_CATEGORY_LIST_KEY, FAQCategory.class);

        if (faqCategoryList == null) {
            faqCategoryList = faqCategoryDao.getFAQCategoryList(GET_ALL_SHOWED_FAQ_CATEGORY_LIST);
            cacheClientService.put(GET_ALL_SHOWD_FAQ_CATEGORY_LIST_KEY, faqCategoryList);

            LOGGER.debug("read faqCategoryList from db");
        } else {
            LOGGER.debug("read faqCategoryList from cache");
        }

        return faqCategoryList;
    }

    //获取问题分类列表
    public List<FAQCategory> getFAQCategoryList(FAQRequest faqRequest) {
        return faqCategoryDao.getFAQCategoryList(faqRequest);
    }

    //改变问题分类显示状态
    public void changeFAQCategoryShowState(int faqCategoryID, boolean isShow) {
        faqCategoryDao.changeFAQCategoryShowState(faqCategoryID, isShow);

        updateCache();
    }

    //删除问题分类项
    public void deleteFAQCategory(int faqCategoryID) {
        faqCategoryDao.deleteFAQCategory(faqCategoryID);

        updateCache();
    }

    //更新问题分类项
    public void updateFAQCategory(FAQCategory faqCategory) {
        faqCategoryDao.updateFAQCategory(faqCategory);

        updateCache();
    }

    //插入问题分类项
    public void addFAQCategory(FAQCategory faqCategory) {
        faqCategoryDao.addFAQCategory(faqCategory);
    }

    //按照统计时间，获取各个faq统计次数
    public List<FAQCategoryConsultDetail> getFAQCategoryConsultDetailList(Long startTime, Long endTime, String titleFilter) {
        return faqCategoryDao.getFAQCategoryConsultDetailList(startTime, endTime, titleFilter);
    }

    //获取问题分类数量
    public List<Integer> faqCount() {
        return faqCategoryDao.faqCount();
    }

    //批量保存 问题分类项
    public void saveAll(List<FAQCategory> faqList) {
        faqCategoryDao.saveAll(faqList);

        updateCache();
    }

    private void updateCache() {
        cacheClientService.delete(GET_ALL_SHOWD_FAQ_CATEGORY_LIST_KEY);
    }

    //更新咨询次数
    public void updateConsultInfo(List<Integer> faqCategoryIDList) {
        for (Integer faqCategoryID : faqCategoryIDList) {
            ConcurrentHashMap<Long, AtomicInteger> tempBrowseInfoUnderFaqID = new ConcurrentHashMap<>(1);
            ConcurrentHashMap<Long, AtomicInteger> consultInfoUnderFaqCategoryID = dayToConsultNumMap.putIfAbsent(faqCategoryID, tempBrowseInfoUnderFaqID);
            if (consultInfoUnderFaqCategoryID == null) {
                consultInfoUnderFaqCategoryID = tempBrowseInfoUnderFaqID;
            }

            long currentTime = DateUtils.truncate(new Date(), Calendar.DATE).getTime();

            AtomicInteger temp = new AtomicInteger(0);
            AtomicInteger todayConsultInfo = consultInfoUnderFaqCategoryID.putIfAbsent(currentTime, temp);
            if (todayConsultInfo == null) {
                todayConsultInfo = temp;
            }

            todayConsultInfo.incrementAndGet();
        }
    }

    public void updateAllConsultInfo(long currentTime) {
        Map<Integer, Map<Long, Integer>> map = new HashMap<>();

        for (Map.Entry<Integer, ConcurrentHashMap<Long, AtomicInteger>> entry : dayToConsultNumMap.entrySet()) {
            int faqCategoryID = entry.getKey();
            ConcurrentHashMap<Long, AtomicInteger> browseInfoUnderFaqCategoryID = entry.getValue();

            Map<Long, Integer> consultNum = new HashMap<>();
            map.put(faqCategoryID, consultNum);
            for (Map.Entry<Long, AtomicInteger> consultInfoEntry : browseInfoUnderFaqCategoryID.entrySet()) {
                if (consultInfoEntry.getKey() < currentTime) {
                    consultNum.put(consultInfoEntry.getKey(), consultInfoEntry.getValue().intValue());
                }
            }
        }

        buildToDB(map);

        for (Map.Entry<Integer, Map<Long, Integer>> entry : map.entrySet()) {
            ConcurrentHashMap<Long, AtomicInteger> consultInfoNum = dayToConsultNumMap.get(entry.getKey());

            for (Map.Entry<Long, Integer> consultInfoEntry : entry.getValue().entrySet()) {
                consultInfoNum.remove(consultInfoEntry.getKey());
            }
        }
    }

    @Transactional
    private void buildToDB(Map<Integer, Map<Long, Integer>> map) {
        for (Map.Entry<Integer, Map<Long, Integer>> entry : map.entrySet()) {
            for (Map.Entry<Long, Integer> browseInfo : entry.getValue().entrySet()) {
                try {
                    faqCategoryDao.updateConsultInfo(entry.getKey(), browseInfo.getKey(), browseInfo.getValue());
                } catch (RuntimeException e) {
                    LOGGER.error("存在错误数据", e);
                }
            }
        }
    }

}
