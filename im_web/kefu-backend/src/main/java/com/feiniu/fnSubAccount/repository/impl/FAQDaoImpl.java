package com.feiniu.fnSubAccount.repository.impl;

import com.feiniu.fnSubAccount.bean.request.FAQRequest;
import com.feiniu.fnSubAccount.bean.vo.FAQViewDetail;
import com.feiniu.fnSubAccount.domain.FAQ;
import com.feiniu.fnSubAccount.repository.FAQDao;
import com.feiniu.fnSubAccount.util.TransformService;
import org.apache.commons.collections.CollectionUtils;
import org.apache.commons.lang.StringUtils;
import org.hibernate.Query;
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
 * Date: 2015/9/28
 * Contact: 243717042@qq.com
 */
@SuppressWarnings({"deprecation", "unchecked"})
@Repository
@Transactional
public class FAQDaoImpl implements FAQDao {
    @Autowired
    private SessionFactory sessionFactory;

    @Override
    public List<FAQ> getFAQList(FAQRequest faqRequest) {
        StringBuilder builder = new StringBuilder("" +
                " select f.i_faq_id as faqID, c_is_show as isShow, v_title as title, v_summary as faqSummary," +
                " v_last_operator as lastOperator, i_order as showOrder, update_time as updateTime, " +
                " nvl(b.viewCount,0) as viewCount from (select * from (select f0.*, rownum as rn from t_faq f0 where 1 = 1 ");

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
                " (select i_faq_id, sum(i_view_count) as viewCount from t_faq_browse_info where 1=1 ");
        if (faqRequest.getStartTime() != null) {
            builder.append(" and i_view_date >= :startTime");
            params.put("startTime", faqRequest.getStartTime());
        }
        if (faqRequest.getEndTime() != null) {
            builder.append(" and i_view_date <= :endTime");
            params.put("endTime", faqRequest.getEndTime());
        }

        builder.append(" group by i_faq_id) b  on f.i_faq_id = b.i_faq_id");
        if (faqRequest.getIsSortByViewCountAsc() == null) {
            builder.append(" order by i_order asc");
        } else {
            if (faqRequest.getIsSortByViewCountAsc()) {
                builder.append(" order by nvl(b.viewCount,0) asc");
            } else {
                builder.append(" order by nvl(b.viewCount,0) desc");
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
                FAQ.class
        );
    }

    @Override
    public void changeFAQShowState(int faqID, boolean isShow) {
        Session session = sessionFactory.getCurrentSession();
        session.createQuery("update FAQ set isShow = :isShow ,updateTime = systimestamp where faqID=:id")
                .setParameter("isShow", isShow ? '1' : '0')
                .setParameter("id", faqID)
                .executeUpdate();

        if (isShow) {
            session.createSQLQuery("UPDATE t_faq f0 SET f0.i_order = (SELECT max(f1.i_order) + 1 FROM t_faq f1), " +
                    " f0.update_time = systimestamp WHERE f0.i_faq_id = :id")
                    .setParameter("id", faqID)
                    .executeUpdate();
        } else {
            session.createSQLQuery("UPDATE t_faq f0 SET f0.i_order = f0.i_order - 1 WHERE f0.i_order > " +
                    "(SELECT f1.i_order FROM t_faq f1 WHERE f1.i_faq_id = :id)")
                    .setParameter("id", faqID)
                    .executeUpdate();
            session.createQuery("update FAQ set showOrder = -1, updateTime = systimestamp where faqID = :id")
                    .setParameter("id", faqID)
                    .executeUpdate();
        }
    }

    @Override
    public void deleteFAQ(int faqID) {
        Session session = sessionFactory.getCurrentSession();

        session.createSQLQuery("UPDATE t_faq f0 SET f0.i_order = f0.i_order - 1 WHERE f0.i_order > " +
                "(SELECT f1.i_order FROM t_faq f1 WHERE f1.i_faq_id = :id)")
                .setParameter("id", faqID)
                .executeUpdate();
        session.createQuery("delete from FAQ where faqID = :id")
                .setParameter("id", faqID)
                .executeUpdate();
    }

    @Override
    public void updateFAQ(FAQ faq) {
        faq.setUpdateTime(new Timestamp(System.currentTimeMillis()));
        sessionFactory.getCurrentSession().update(faq);
    }

    @Override
    public void addFAQ(FAQ faq) {
        Session session = sessionFactory.getCurrentSession();

        faq.setUpdateTime(new Timestamp(System.currentTimeMillis()));
        session.save(faq);
    }

    @Override
    public void changeFAQOrder(int srcFAQID, int srcOrder, int destFAQID, int destOrder) {
        Session session = sessionFactory.getCurrentSession();
        Query query = session.createQuery("update FAQ set showOrder = :order, updateTime = systimestamp  where faqID = :id");

        query.setParameter("order", srcOrder).setParameter("id", destFAQID).executeUpdate();
        query.setParameter("order", destOrder).setParameter("id", srcFAQID).executeUpdate();
    }

    @Override
    public List<FAQViewDetail> getFAQViewDetailList(Long startTime, Long endTime, String titleFilter) {
        StringBuilder builder = new StringBuilder(
                " select b.i_faq_id as faqID, f.v_title, nvl(b.viewCount,0) as viewCount" +
                        " from (select * from t_faq where 1 = 1 "
        );
        HashMap<String, Object> params = new HashMap<>(1);

        if (StringUtils.isNotEmpty(titleFilter)) {
            builder.append(" and v_title like :title");
            params.put("title", titleFilter);
        }

        builder.append(" ) f left join ( select i_faq_id, sum(i_view_count) as viewCount from t_faq_browse_info where 1 = 1");
        if (startTime != null) {
            builder.append(" and i_view_date >= :startTime");
            params.put("startTime", startTime);
        }
        if (endTime != null) {
            builder.append(" and i_view_date <= :endTime");
            params.put("endTime", endTime);
        }

        builder.append(" ) b on f.i_faq_id = b.i_faq_id");

        return TransformService.transformResult(
                sessionFactory.getCurrentSession().createSQLQuery(builder.toString()),
                params,
                FAQViewDetail.class
        );
    }

    @Override
    @SuppressWarnings("unchecked")
    public List<Integer> faqCount() {
        List<Object[]> list = sessionFactory.getCurrentSession().createSQLQuery("" +
                " SELECT count(*) AS totalCount, " +
                " nvl(sum(CASE WHEN c_is_show = '1' THEN 1 ELSE 0 END),0) AS showedCount FROM t_faq").list();

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
    public void saveAll(List<FAQ> faqList) {
        for (FAQ faq : faqList) {
            addFAQ(faq);
        }
    }

    @Override
    public void updateBrowseInfo(int faqID, int delta, long viewDate) {
        SQLQuery query = sessionFactory.getCurrentSession().createSQLQuery("MERGE INTO t_faq_browse_info t_old" +
                " USING (SELECT :faqID AS i_faq_id, :viewDate AS i_view_date, :viewCount AS i_view_count, " +
                " systimestamp AS update_time FROM DUAL) t_new" +
                " ON (t_old.i_faq_id = t_new.i_faq_id AND t_old.i_view_date = t_new.i_view_date)" +
                " WHEN MATCHED THEN UPDATE SET t_old.i_view_count = t_old.i_view_count + t_new.i_view_count, t_old.update_time = systimestamp" +
                " WHEN NOT MATCHED THEN INSERT (t_old.i_faq_id, t_old.i_view_date, t_old.i_view_count, t_old.update_time)" +
                " VALUES (t_new.i_faq_id, t_new.i_view_date, t_new.i_view_count,systimestamp)");

        query.setParameter("faqID", faqID)
                .setParameter("viewDate", viewDate)
                .setParameter("viewCount", delta).executeUpdate();
    }
}
