package com.feiniu.fnSubAccount.service;

import com.feiniu.fnSubAccount.bean.request.QuickPhraseGroupRequest;
import com.feiniu.fnSubAccount.bean.request.QuickPhraseRequest;
import com.feiniu.fnSubAccount.domain.QuickPhrase;
import com.feiniu.fnSubAccount.domain.QuickPhraseGroup;
import com.feiniu.fnSubAccount.repository.QuickPhraseDao;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * Created by zhangmin on 2015/11/26.
 */
@Service
public class QuickPhraseService {

    private static final String QUICK_PHRASE_LIST_TAB  = "quickPhrase#AllList";
    public static final String QUICK_PHRASE_GROUP_LIST_TAB  = "quickPhraseGroup#AllList";
    private static final Logger LOGGER = LoggerFactory.getLogger(QuickPhraseService.class);
    @Autowired
    private CacheClientService cacheClientService;
    @Autowired
    private QuickPhraseDao quickPhraseDao;

    //依quickPhraseRequst获取list
    public List<QuickPhrase> getQuickPhraseList(QuickPhraseRequest quickPhraseRequest, boolean phraseIsLike)
    {
        return quickPhraseDao.getQuickPhraseList(quickPhraseRequest, phraseIsLike);
    }

    //依quickPhraseRequst获取count
    public int getQuickPhraseCount(QuickPhraseRequest quickPhraseRequest)
    {
        return quickPhraseDao.phraseCount(quickPhraseRequest).get(0);
    }

    //依quickPhraseGroupRequest获取groupNameList
    public List<QuickPhraseGroup> getQuickPhraseGroupList(QuickPhraseGroupRequest quickPhraseGroupRequest)
    {
        return quickPhraseDao.getPhraseGroupList(quickPhraseGroupRequest);
    }

    public int phraseGroupCount(QuickPhraseGroupRequest quickPhraseGroupRequest){
        return quickPhraseDao.phraseGroupCount(quickPhraseGroupRequest).get(0);
    }
    //批量更新组名
    public void updateGroupName(QuickPhraseGroupRequest quickPhraseGroupRequest)
    {
        QuickPhraseRequest quickPhraseRequest = new QuickPhraseRequest();
        quickPhraseRequest.setPageSize(10000);
        quickPhraseRequest.setPhraseGroupNamePart(quickPhraseGroupRequest.getOldGroupName());

        List<QuickPhrase> quickPhraseList = getQuickPhraseList(quickPhraseRequest, false);
        for(QuickPhrase quickPhrase : quickPhraseList)
        {
            quickPhrase.setPhraseGroupName(quickPhraseGroupRequest.getNewGroupName());
            updateQuickPhrase(quickPhrase);
        }
    }
    public void addQuickPhrase(QuickPhrase quickPhrase){
        quickPhraseDao.addQuickPhrase(quickPhrase);
        updateCacheQuickPhrase();
    }

    public void updateQuickPhrase(QuickPhrase quickPhrase){
        quickPhraseDao.updateQuickPhrase(quickPhrase);
        updateCacheQuickPhrase();
    }

    public void deleteQuickPhrase(List<Integer> quickPhraseId){
        quickPhraseDao.deleteQuickPhrase(quickPhraseId);
        updateCacheQuickPhrase();
    }

    public void addQuickPhraseList(List<QuickPhrase> quickPhraseList)
    {
        quickPhraseDao.saveAll(quickPhraseList);
        updateCacheQuickPhrase();
    }
    //获取全量的quickPhrase
    public List<QuickPhrase> getAllQuickPhrase()
    {
        //优先从memcached中获取
        List<QuickPhrase> quickPhraseList = cacheClientService.getListValue(QUICK_PHRASE_LIST_TAB, QuickPhrase.class);
        if (quickPhraseList == null) {
            quickPhraseList = updateCacheQuickPhrase();
            LOGGER.debug("Read quickPhraseList from db");
        } else {
            LOGGER.debug("Read quickPhraseList from cache");
        }
        return quickPhraseList;
    }
    //从db中更新全量quickPhrase到memcached
    public List<QuickPhrase> updateCacheQuickPhrase()
    {
        QuickPhraseRequest quickPhraseRequest = new QuickPhraseRequest();
        quickPhraseRequest.setCurrentPageNo(1);
        quickPhraseRequest.setPageSize(10000);

        List<QuickPhrase> quickPhraseList = quickPhraseDao.getQuickPhraseList(quickPhraseRequest,true);
        cacheClientService.put(QUICK_PHRASE_LIST_TAB, quickPhraseList);
        return quickPhraseList;
    }




}
