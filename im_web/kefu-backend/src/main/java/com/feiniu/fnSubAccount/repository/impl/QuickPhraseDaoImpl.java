package com.feiniu.fnSubAccount.repository.impl;

import com.feiniu.common.core.SystemEnv;
import com.feiniu.fnSubAccount.bean.request.QuickPhraseGroupRequest;
import com.feiniu.fnSubAccount.bean.request.QuickPhraseRequest;
import com.feiniu.fnSubAccount.domain.QuickPhrase;
import com.feiniu.fnSubAccount.domain.QuickPhraseGroup;
import com.feiniu.fnSubAccount.repository.QuickPhraseDao;
import com.feiniu.fnSubAccount.service.QuickPhraseService;
import com.feiniu.fnSubAccount.util.TransformService;
import org.apache.commons.collections.CollectionUtils;
import org.apache.commons.lang.StringUtils;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import java.sql.Timestamp;
import java.util.*;

/**
 * Created by zhangmin on 2015/10/23.
 */
@Repository
@Transactional

public class QuickPhraseDaoImpl implements QuickPhraseDao {

    @Autowired
    private SessionFactory sessionFactory;


    @Override
    public List<QuickPhrase> getQuickPhraseList(QuickPhraseRequest quickPhraseRequest, boolean phraseIsLike) {

        StringBuilder stringBuilder = new StringBuilder("select * from (select rownum as rowIndex, i_phrase_id as phraseId, v_phrase_summary as phraseSummary , " +
                "v_phrase_group as phraseGroupName, v_quick_code as quickCode," +
                "v_last_operator as lastOperator, "+
                "i_font_size as fontSize, i_font_color as fontColor, i_font_style as fontStyle, v_font_name as fontName, " +
                "update_time as updateTime from t_quick_phrase where 1= 1");

        Map<String, Object> params = new HashMap<>(1);
        if (StringUtils.isNotEmpty(quickPhraseRequest.getQuickCode()))
        {
            stringBuilder.append(" and v_quick_code = :quickCode ");
            params.put("quickCode", quickPhraseRequest.getQuickCode());
        }

        if (StringUtils.isNotEmpty(quickPhraseRequest.getQuickPhrasePart()))
        {
            if(phraseIsLike){
                stringBuilder.append( " and v_phrase_summary like :phrasePart " );
            }
            else{
                stringBuilder.append( " and v_phrase_summary = :phrasePart " );
            }
            params.put("phrasePart", '%' + quickPhraseRequest.getQuickPhrasePart() + '%');
        }

        if(StringUtils.isNotEmpty(quickPhraseRequest.getPhraseGroupNamePart()))
        {
            stringBuilder.append(" and v_phrase_group like :phraseGroupNamePart");
            params.put("phraseGroupNamePart", '%' + quickPhraseRequest.getPhraseGroupNamePart() + '%');
        }

        stringBuilder.append( ") where rowIndex > :fromRowIndex and rowIndex < :toRowIndex");
        params.put("fromRowIndex", (quickPhraseRequest.getCurrentPageNo() - 1) * quickPhraseRequest.getPageSize());
        params.put("toRowIndex",quickPhraseRequest.getCurrentPageNo() * quickPhraseRequest.getPageSize() +  1) ;
        return TransformService.transformResult(
                sessionFactory.getCurrentSession().createSQLQuery(stringBuilder.toString()),
                params,
                QuickPhrase.class
        );
    }

    @Override
    @SuppressWarnings("unchecked")
    public List<Integer> phraseCount(QuickPhraseRequest quickPhraseRequest) {

        StringBuilder stringBuilder = new StringBuilder( "select  count(1) AS totalCount , " +
                " 1 AS noUse from t_quick_phrase where 1= 1");

        Map<String, Object> params = new HashMap<>(1);
        if (StringUtils.isNotEmpty(quickPhraseRequest.getQuickCode()))
        {
            stringBuilder.append(" and v_quick_code = :quickCode ");
            params.put("quickCode", quickPhraseRequest.getQuickCode());
        }

        if (StringUtils.isNotEmpty(quickPhraseRequest.getQuickPhrasePart()))
        {
            stringBuilder.append(" and v_phrase_summary like :phrasePart ");
            params.put("phrasePart", '%' + quickPhraseRequest.getQuickPhrasePart() + '%');
        }

        if(StringUtils.isNotEmpty(quickPhraseRequest.getPhraseGroupNamePart()))
        {
            stringBuilder.append(" and v_phrase_group like :phraseGroupNamePart");
            params.put("phraseGroupNamePart", '%' + quickPhraseRequest.getPhraseGroupNamePart() + '%');
        }

        List<Object[]> list = SqlHelper.createQueryWithParams(sessionFactory.getCurrentSession(),
                stringBuilder.toString(), params, 0).list();

        if (CollectionUtils.isNotEmpty(list)) {
            List<Integer> result = new ArrayList<>(2);
            Object[] row = list.get(0);
            result.add(Integer.parseInt(row[0].toString()));
            return result;
        }

        return Collections.emptyList();
    }


    @Override
    public List<QuickPhraseGroup> getPhraseGroupList(QuickPhraseGroupRequest quickPhraseGroupRequest){
        StringBuilder stringBuilder  = new StringBuilder("select * from (select rownum as rowIndex, v_phrase_group as groupName from " +
                "(select distinct v_phrase_group from t_quick_phrase where 1=1");

        Map<String, Object> params = new HashMap<>(1);
        if (StringUtils.isNotEmpty(quickPhraseGroupRequest.getGroupNamePart())) {
            stringBuilder.append(" and v_phrase_group like :groupNamePart ");
            params.put("groupNamePart", '%' + quickPhraseGroupRequest.getGroupNamePart() + '%');
        }

        stringBuilder.append(")) where rowIndex > :fromRowIndex and rowIndex < :toRowIndex");
        params.put("fromRowIndex", (quickPhraseGroupRequest.getCurrentPageNo() - 1) * quickPhraseGroupRequest.getPageSize());
        params.put("toRowIndex", quickPhraseGroupRequest.getCurrentPageNo() * quickPhraseGroupRequest.getPageSize() + 1) ;


        return TransformService.transformResult(
                sessionFactory.getCurrentSession().createSQLQuery(stringBuilder.toString()),
                params,
                QuickPhraseGroup.class
        );
    }

    @Override
    public List<Integer> phraseGroupCount(QuickPhraseGroupRequest quickPhraseGroupRequest)
    {
        StringBuilder stringBuilder = new StringBuilder("select  count(1) AS totalCount ,  1 AS noUse from " +
                "(select distinct v_phrase_group from t_quick_phrase where 1= 1");

        Map<String, Object> params = new HashMap<>(1);
        if (StringUtils.isNotEmpty(quickPhraseGroupRequest.getGroupNamePart())) {
            stringBuilder.append(" and v_phrase_group like :groupNamePart ");
            params.put("groupNamePart", '%' + quickPhraseGroupRequest.getGroupNamePart() + '%');
        }
        stringBuilder.append(")");

        List<Object[]> list = SqlHelper.createQueryWithParams(sessionFactory.getCurrentSession(),
                stringBuilder.toString(), params, 0).list();

        if (CollectionUtils.isNotEmpty(list)) {
            List<Integer> result = new ArrayList<>(2);
            Object[] row = list.get(0);
            result.add(Integer.parseInt(row[0].toString()));
            return result;
        }
        return Collections.EMPTY_LIST;
    }

    @Override
    public void deleteQuickPhrase(List<Integer> phraseId) {
        Session session = sessionFactory.getCurrentSession();
        session.createQuery("delete from QuickPhrase where phraseId in :id")
                .setParameterList("id", phraseId)
                .executeUpdate();
    }

    @Override
    public void updateQuickPhrase(QuickPhrase quickPhrase) {
        quickPhrase.setLastOperator(SystemEnv.getCurrentUser().getUserName());
        quickPhrase.setUpdateTime(new Timestamp(System.currentTimeMillis()));
        sessionFactory.getCurrentSession().update(quickPhrase);
    }

    @Override
    public void addQuickPhrase(QuickPhrase quickPhrase) {
        quickPhrase.setLastOperator(SystemEnv.getCurrentUser().getUserName());
        quickPhrase.setUpdateTime(new Timestamp(System.currentTimeMillis()));
        sessionFactory.getCurrentSession().save(quickPhrase);
    }

    @Override
    public void saveAll(List<QuickPhrase> quickPhraseList) {
        for (QuickPhrase quickPhrase : quickPhraseList) {
            addQuickPhrase(quickPhrase);
        }
    }
}
