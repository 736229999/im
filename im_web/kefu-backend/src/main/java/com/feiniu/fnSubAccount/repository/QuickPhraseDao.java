package com.feiniu.fnSubAccount.repository;

import com.feiniu.fnSubAccount.bean.request.QuickPhraseGroupRequest;
import com.feiniu.fnSubAccount.bean.request.QuickPhraseRequest;
import com.feiniu.fnSubAccount.domain.QuickPhrase;
import com.feiniu.fnSubAccount.domain.QuickPhraseGroup;

import java.util.List;

/**
 * Created by zhangmin on 2015/10/22.
 */
public interface QuickPhraseDao {
    //按条件获取quickPhrase列表
    List<QuickPhrase> getQuickPhraseList(QuickPhraseRequest quickPhraseRequest, boolean phraseIsLike);
    //获取总数
    public List<Integer> phraseCount(QuickPhraseRequest quickPhraseRequest);
    //删除quickPhrase
    void deleteQuickPhrase(List<Integer> phraseId);
    //更新quickPhrase
    void updateQuickPhrase(QuickPhrase quickPhrase);
    //插入quickPhrase
    void addQuickPhrase(QuickPhrase quickPhrase);
    //保存全部quickPhrase
    void saveAll(List<QuickPhrase> quickPhraseList);
    //获取分组列表
    List<QuickPhraseGroup> getPhraseGroupList(QuickPhraseGroupRequest quickPhraseGroupRequest);
    //获取分组count
    List<Integer> phraseGroupCount(QuickPhraseGroupRequest quickPhraseGroupRequest);
}
