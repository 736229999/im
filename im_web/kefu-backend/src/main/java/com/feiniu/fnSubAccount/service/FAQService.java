package com.feiniu.fnSubAccount.service;

import com.feiniu.fnSubAccount.bean.request.FAQRequest;
import com.feiniu.fnSubAccount.bean.vo.FAQViewDetail;
import com.feiniu.fnSubAccount.domain.FAQ;
import com.feiniu.fnSubAccount.repository.FAQDao;
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
 * Date: 2015/11/20
 * Contact: 243717042@qq.com
 */
@Service
public class FAQService {
    @Autowired
    private CacheClientService cacheClientService;
    @Autowired
    private FAQDao faqDao;

    private static final FAQRequest GET_ALL_SHOWED_FAQ_LIST = new FAQRequest();
    private static final String GET_ALL_SHOWD_FAQ_LIST_KEY = "FAQ#allList";
    private static final Logger LOGGER = LoggerFactory.getLogger(FAQService.class);
    //浏览次数
    private ConcurrentHashMap<Integer, ConcurrentHashMap<Long, AtomicInteger>> dayToBrowseNumMap = new ConcurrentHashMap<>(1);

    static {
        GET_ALL_SHOWED_FAQ_LIST.setIsShow(true);
        GET_ALL_SHOWED_FAQ_LIST.setPageSize(Integer.MAX_VALUE);
    }

    //获取所有已显示的常见问题
    public List<FAQ> getFAQListForMoumou() {
        List<FAQ> faqList = cacheClientService.getListValue(GET_ALL_SHOWD_FAQ_LIST_KEY, FAQ.class);

        if (faqList == null) {
            faqList = faqDao.getFAQList(GET_ALL_SHOWED_FAQ_LIST);
            cacheClientService.put(GET_ALL_SHOWD_FAQ_LIST_KEY, faqList);

            LOGGER.debug("read faqList from db");
        } else {
            LOGGER.debug("read faqList from cache");
        }

        return faqList;
    }

    //获取faq列表
    public List<FAQ> getFAQList(FAQRequest faqRequest) {
        return faqDao.getFAQList(faqRequest);
    }

    //改变faq显示状态
    public void changeFAQShowState(int faqID, boolean isShow) {
        faqDao.changeFAQShowState(faqID, isShow);

        updateCache();
    }

    //删除faq
    public void deleteFAQ(int faqID) {
        faqDao.deleteFAQ(faqID);

        updateCache();
    }

    //更新faq
    public void updateFAQ(FAQ faq) {
        faqDao.updateFAQ(faq);

        updateCache();
    }

    //插入faq
    public void addFAQ(FAQ faq) {
        faqDao.addFAQ(faq);

        updateCache();
    }

    //改变faq位置
    public void changeFAQOrder(int srcFAQID, int srcOrder, int destFAQID, int destOrder) {
        faqDao.changeFAQOrder(srcFAQID, srcOrder, destFAQID, destOrder);

        updateCache();
    }

    //按照统计时间，获取各个faq统计次数
    public List<FAQViewDetail> getFAQViewDetailList(Long startTime, Long endTime, String titleFilter) {
        return faqDao.getFAQViewDetailList(startTime, endTime, titleFilter);
    }

    public List<Integer> faqCount() {
        return faqDao.faqCount();
    }

    public void saveAll(List<FAQ> faqList) {
        faqDao.saveAll(faqList);

        updateCache();
    }

    public void updateBrowseInfo(int faqID) {
        ConcurrentHashMap<Long, AtomicInteger> tempBrowseInfoUnderFaqID = new ConcurrentHashMap<>(1);
        ConcurrentHashMap<Long, AtomicInteger> browseInfoUnderFaqID = dayToBrowseNumMap.putIfAbsent(faqID, tempBrowseInfoUnderFaqID);
        if (browseInfoUnderFaqID == null) {
            browseInfoUnderFaqID = tempBrowseInfoUnderFaqID;
        }

        long currentTime = DateUtils.truncate(new Date(), Calendar.DATE).getTime();

        AtomicInteger temp = new AtomicInteger(0);
        AtomicInteger todayBrowseInfo = browseInfoUnderFaqID.putIfAbsent(currentTime, temp);
        if (todayBrowseInfo == null) {
            todayBrowseInfo = temp;
        }

        todayBrowseInfo.incrementAndGet();
    }

    public void updateAllBrowseInfo(long currentTime) {
        Map<Integer, Map<Long, Integer>> map = new HashMap<>();

        for (Map.Entry<Integer, ConcurrentHashMap<Long, AtomicInteger>> entry : dayToBrowseNumMap.entrySet()) {
            int faqID = entry.getKey();
            ConcurrentHashMap<Long, AtomicInteger> browseInfoUnderFaqID = entry.getValue();

            Map<Long, Integer> browseNum = new HashMap<>();
            map.put(faqID, browseNum);
            for (Map.Entry<Long, AtomicInteger> browseInfoEntry : browseInfoUnderFaqID.entrySet()) {
                if (browseInfoEntry.getKey() < currentTime) {
                    browseNum.put(browseInfoEntry.getKey(), browseInfoEntry.getValue().intValue());
                }
            }
        }

        buildToDB(map);

        for (Map.Entry<Integer, Map<Long, Integer>> entry : map.entrySet()) {
            ConcurrentHashMap<Long, AtomicInteger> browseInfoNum = dayToBrowseNumMap.get(entry.getKey());

            for (Map.Entry<Long, Integer> browseInfoEntry : entry.getValue().entrySet()) {
                browseInfoNum.remove(browseInfoEntry.getKey());
            }
        }
    }

    @Transactional
    private void buildToDB(Map<Integer, Map<Long, Integer>> map) {
        for (Map.Entry<Integer, Map<Long, Integer>> entry : map.entrySet()) {
            for (Map.Entry<Long, Integer> browseInfo : entry.getValue().entrySet()) {
                faqDao.updateBrowseInfo(entry.getKey(), browseInfo.getValue(), browseInfo.getKey());
            }
        }
    }

    private void updateCache() {
        cacheClientService.delete(GET_ALL_SHOWD_FAQ_LIST_KEY);
    }
}
