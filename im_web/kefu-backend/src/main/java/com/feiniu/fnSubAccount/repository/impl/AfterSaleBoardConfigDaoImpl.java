package com.feiniu.fnSubAccount.repository.impl;

import com.feiniu.fnSubAccount.consts.SystemConfig;
import com.feiniu.fnSubAccount.domain.AfterSaleBoardConfig;
import com.feiniu.fnSubAccount.repository.AfterSaleBoardConfigDao;
import kafka.controller.OfflineReplica;
import org.apache.commons.collections.ListUtils;
import org.apache.commons.collections4.CollectionUtils;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Author: morningking
 * Date: 2015/10/14
 * Contact: 243717042@qq.com
 */
@SuppressWarnings({"deprecation", "unchecked"})
@Transactional
@Repository
public class AfterSaleBoardConfigDaoImpl implements AfterSaleBoardConfigDao {
    @Autowired
    private SessionFactory sessionFactory;

    @Override
    public List<AfterSaleBoardConfig> getDesignTimeData() {
        return sessionFactory.getCurrentSession().
                createQuery("from AfterSaleBoardConfig where dataType = " + SystemConfig.DESIGN_TIME_INDICATOR + " order by boardID asc ")
                .list();
    }

    @Override
    public List<AfterSaleBoardConfig> getRuntimeData() {
        return sessionFactory.getCurrentSession().
                createQuery("from AfterSaleBoardConfig where dataType = " + SystemConfig.RUNTIME_INDICATOR + " order by boardID asc ")
                .list();
    }

    @Override
    public void update(AfterSaleBoardConfig afterSaleBoardConfig) {
        sessionFactory.getCurrentSession().update(afterSaleBoardConfig);
    }

    @Override
    public List<AfterSaleBoardConfig> syncEditToRuntime() {
        Session session = sessionFactory.getCurrentSession();
        List<AfterSaleBoardConfig> designTimeData = getDesignTimeData();
        List<AfterSaleBoardConfig> runtimeData = getRuntimeData();
        List<AfterSaleBoardConfig> toUpdate = new ArrayList<>(designTimeData.size());

        for (int i = 0; i < runtimeData.size(); i++) {
            AfterSaleBoardConfig currentRuntime = runtimeData.get(i);
            AfterSaleBoardConfig currentDesign = designTimeData.get(i);

            try {
                currentDesign = (AfterSaleBoardConfig) (currentDesign.clone());
            } catch (Exception e) {
                throw new RuntimeException(e);
            }

            currentDesign.setBoardID(currentRuntime.getBoardID());
            currentDesign.setUpdateTime(new Timestamp(System.currentTimeMillis()));
            currentDesign.setDataType(SystemConfig.RUNTIME_INDICATOR);

            toUpdate.add(currentDesign);
        }

        for (AfterSaleBoardConfig toUpdateItem : toUpdate) {
            session.merge(toUpdateItem);
        }

        session.flush();

        return toUpdate;
    }

    @Override
    public boolean isGroupBinded(int groupID) {
        Long bindNum = (Long) (sessionFactory.getCurrentSession().createQuery("select count(*) from AfterSaleBoardConfig " +
                "where dataType = :dataType and isBindGroup = '1' and groupID = :groupID")
                .setParameter("dataType", SystemConfig.RUNTIME_INDICATOR)
                .setParameter("groupID", groupID)
                .uniqueResult());

        return bindNum > 0l;
    }

    @Override
    @SuppressWarnings("unchecked")
    public Map<Integer, Boolean> checkGroupsIfBind(List<Integer> groupIDList) {
        List<Object[]> result = sessionFactory.getCurrentSession().createQuery("select groupID," +
                " (case when count(*) > 0 then true else false end) as isBind" +
                " from AfterSaleBoardConfig " +
                " where dataType = :dataType and isBindGroup = '1' and groupID in :groupID" +
                " group by groupID")
                .setParameter("dataType", SystemConfig.RUNTIME_INDICATOR)
                .setParameterList("groupID", groupIDList)
                .list();

        if (CollectionUtils.isNotEmpty(result)) {
            Map<Integer, Boolean> groupBindResult = new HashMap<>(1);
            for (Object[] row : result) {
                groupBindResult.put((int) row[0], (boolean) row[1]);
            }

            return groupBindResult;
        } else {
            return null;
        }
    }
}
