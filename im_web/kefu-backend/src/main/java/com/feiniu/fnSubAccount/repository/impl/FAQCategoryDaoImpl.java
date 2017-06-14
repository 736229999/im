package com.feiniu.fnSubAccount.repository.impl;

import com.feiniu.fnSubAccount.bean.request.FAQRequest;
import com.feiniu.fnSubAccount.bean.vo.FAQCategoryConsultDetail;
import com.feiniu.fnSubAccount.domain.FAQCategory;
import com.feiniu.fnSubAccount.repository.FAQCategoryDao;
import com.feiniu.fnSubAccount.util.TransformService;
import org.apache.commons.collections.CollectionUtils;
import org.apache.commons.lang.StringUtils;
import org.hibernate.SQLQuery;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import java.sql.Timestamp;
import java.util.*;

/**
 * Author: morningking
 * Date: 2015/10/12
 * Contact: 243717042@qq.com
 */
@SuppressWarnings({"deprecation", "unchecked"})
@Repository
@Transactional
public class FAQCategoryDaoImpl implements FAQCategoryDao {
    @Autowired
    private SessionFactory sessionFactory;

    @Override
    public List<FAQCategory> getFAQCategoryList(FAQRequest faqRequest) {
        StringBuilder builder = new StringBuilder("" +
                " select f.i_id as categoryID, c_is_show as isShow, v_title as title, v_summary as remark," +
                " v_last_operator as lastOperator, update_time as updateTime, " +
                " nvl(b.consultCount,0) as consultCount from (select * from (select f0.*, rownum as rn from t_faq_category f0 where 1 = 1 ");

        Map<String, Object> params = new HashMap<>(1);

        if (StringUtils.isNotEmpty(faqRequest.getKeyWord())) {
            builder.append(" and v_title like :title");
            params.put("title", '%' + faqRequest.getKeyWord() + '%');
        }
        if (faqRequest.getIsShow()) {
            builder.append(" and c_is_show = '1'");
        }

        builder.append(") where rn > :fromNo and rn <= :endNo) f " +
                " left join " +
                " (select i_faq_category_id, sum(i_consult_count) as consultCount from t_faq_category_consult_info where 1=1 ");
        if (faqRequest.getStartTime() != null) {
            builder.append(" and i_consult_date >= :startTime");
            params.put("startTime", faqRequest.getStartTime());
        }
        if (faqRequest.getEndTime() != null) {
            builder.append(" and i_consult_date <= :endTime");
            params.put("endTime", faqRequest.getEndTime());
        }

        builder.append(" group by i_faq_category_id) b  on f.i_id = b.i_faq_category_id");
        if (faqRequest.getIsSortByViewCountAsc() == null) {
            builder.append(" order by f.i_id asc");
        } else {
            if (faqRequest.getIsSortByViewCountAsc()) {
                builder.append(" order by nvl(b.consultCount,0) asc");
            } else {
                builder.append(" order by nvl(b.consultCount,0) desc");
            }
        }

        params.put("fromNo", (faqRequest.getCurrentPageNo() - 1) * faqRequest.getPageSize());
        params.put("endNo", faqRequest.getCurrentPageNo() * faqRequest.getPageSize());

        if (faqRequest.isFetchAll()) {
            params.put("fromNo", 0);
            params.put("endNo", Integer.MAX_VALUE);
        }

        return TransformService.transformResult(
                sessionFactory.getCurrentSession().createSQLQuery(builder.toString()),
                params,
                FAQCategory.class
        );
    }

    @Override
    public void changeFAQCategoryShowState(int faqCategoryID, boolean isShow) {
        Session session = sessionFactory.getCurrentSession();
        session.createQuery("update FAQCategory set isShow = :isShow ,updateTime = systimestamp where categoryID=:id")
                .setParameter("isShow", isShow ? '1' : '0')
                .setParameter("id", faqCategoryID)
                .executeUpdate();
    }

    @Override
    public void deleteFAQCategory(int faqCategoryID) {
        sessionFactory.getCurrentSession().createQuery("delete from FAQCategory where categoryID = :id")
                .setParameter("id", faqCategoryID)
                .executeUpdate();
    }

    @Override
    public void updateFAQCategory(FAQCategory faqCategory) {
        faqCategory.setUpdateTime(new Timestamp(System.currentTimeMillis()));
        sessionFactory.getCurrentSession().update(faqCategory);
    }

    @Override
    public void addFAQCategory(FAQCategory faqCategory) {
        Session session = sessionFactory.getCurrentSession();
        faqCategory.setUpdateTime(new Timestamp(System.currentTimeMillis()));
        session.save(faqCategory);
    }

    @Override
    public List<FAQCategoryConsultDetail> getFAQCategoryConsultDetailList(Long startTime, Long endTime, String titleFilter) {
        StringBuilder builder = new StringBuilder(
                " select b.i_id as faqCategoryID, f.v_title, nvl(b.consultCount,0) as consultCount" +
                        " from (select * from t_faq where 1 = 1 "
        );
        HashMap<String, Object> params = new HashMap<>(1);

        if (StringUtils.isNotEmpty(titleFilter)) {
            builder.append(" and v_title like :title");
            params.put("title", titleFilter);
        }

        builder.append(" ) f left join ( select i_faq_category_id, sum(i_consult_count) as consultCount " +
                " from t_faq_category_consult_info where 1 = 1");
        if (startTime != null) {
            builder.append(" and i_consult_date >= :startTime");
            params.put("startTime", startTime);
        }
        if (endTime != null) {
            builder.append(" and i_consult_date <= :endTime");
            params.put("endTime", endTime);
        }

        builder.append(" ) b on f.i_id = b.i_faq_category_id");

        return TransformService.transformResult(
                sessionFactory.getCurrentSession().createSQLQuery(builder.toString()),
                params,
                FAQCategoryConsultDetail.class
        );
    }

    @Override
    public List<Integer> faqCount() {
        List<Object[]> list = sessionFactory.getCurrentSession().createSQLQuery("" +
                " SELECT count(*) AS totalCount, " +
                " nvl(sum(CASE WHEN c_is_show = '1' THEN 1 ELSE 0 END),0) AS showedCount FROM t_faq_category").list();

        if (CollectionUtils.isNotEmpty(list)) {
            List<Integer> result = new ArrayList<>(2);
            Object[] row = list.get(0);

            result.add(Integer.parseInt(row[0].toString()));
            result.add(Integer.parseInt(row[1].toString()));

            return result;
        }

        return Collections.emptyList();
    }

    @Override
    public void saveAll(List<FAQCategory> faqList) {
        for (FAQCategory faqCategory : faqList) {
            addFAQCategory(faqCategory);
        }
    }

    @Override
    public void updateConsultInfo(int faqCategoryID, long consultDate, int delta) {
        SQLQuery query = sessionFactory.getCurrentSession().createSQLQuery("MERGE INTO t_faq_category_consult_info t_old" +
                " USING (SELECT :faqCategoryID AS i_faq_category_id, :consultDate AS i_consult_date, :consultCount AS i_consult_count, " +
                " systimestamp AS update_time FROM DUAL) t_new" +
                " ON (t_old.i_faq_category_id = t_new.i_faq_category_id AND t_old.i_consult_date = t_new.i_consult_date)" +
                " WHEN MATCHED THEN UPDATE SET t_old.i_consult_count = t_old.i_consult_count + t_new.i_consult_count, " +
                " t_old.update_time = systimestamp" +
                " WHEN NOT MATCHED THEN INSERT (t_old.i_faq_category_id, t_old.i_consult_date, t_old.i_consult_count, t_old.update_time)" +
                " VALUES (t_new.i_faq_category_id, t_new.i_consult_date, t_new.i_consult_count,systimestamp)");

        query.setParameter("faqCategoryID", faqCategoryID)
                .setParameter("consultDate", consultDate)
                .setParameter("consultCount", delta).executeUpdate();
    }

}
