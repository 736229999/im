package com.feiniu.fnSubAccount.repository.impl;

import com.feiniu.fnSubAccount.consts.SystemConfig;
import com.feiniu.fnSubAccount.domain.AfterSaleBoardConfig;
import com.feiniu.fnSubAccount.domain.TouchPortalBoardConfig;
import com.feiniu.fnSubAccount.repository.TouchPortalBoardConfigDao;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.List;

/**
 * Author: morningking
 * Date: 2016/3/8
 * Contact: 243717042@qq.com
 */
@Repository
public class TouchPortalBoardConfigDaoImpl implements TouchPortalBoardConfigDao {
    @Autowired
    private SessionFactory sessionFactory;

    @Override
    public void add(TouchPortalBoardConfig config) {
        sessionFactory.getCurrentSession().save(config);
    }

    @Override
    public List<TouchPortalBoardConfig> getRuntimeBoardList() {
        return sessionFactory.getCurrentSession().createQuery("from TouchPortalBoardConfig where dataType = "
                + SystemConfig.RUNTIME_INDICATOR + " order by showOrder asc").list();
    }

    @Override
    public List<TouchPortalBoardConfig> getDesignTimeBoardList() {
        return sessionFactory.getCurrentSession().createQuery("from TouchPortalBoardConfig where dataType = "
                + SystemConfig.DESIGN_TIME_INDICATOR + " order by showOrder asc").list();
    }

    @Override
    public List<TouchPortalBoardConfig> syncEditToRuntime() {
        Session session = sessionFactory.getCurrentSession();
        List<TouchPortalBoardConfig> designTimeData = getDesignTimeBoardList();
        List<TouchPortalBoardConfig> toUpdate = new ArrayList<>(designTimeData.size());

        //先将运行时数据全部删除
        session.createQuery("delete from TouchPortalBoardConfig where dataType = " + SystemConfig.RUNTIME_INDICATOR).executeUpdate();

        for (TouchPortalBoardConfig touchPortalBoardConfig : designTimeData) {
            try {
                TouchPortalBoardConfig cloned = touchPortalBoardConfig.clone();
                cloned.setDataType(SystemConfig.RUNTIME_INDICATOR);
                toUpdate.add(cloned);
            } catch (CloneNotSupportedException e) {
                //omit
            }
        }

        //同步设计时数据
        for (TouchPortalBoardConfig touchPortalBoardConfig : toUpdate) {
            session.save(touchPortalBoardConfig);
        }

        session.flush();

        return toUpdate;
    }

    @Override
    public void delete(int id) {
        Session session = sessionFactory.getCurrentSession();

        session.createQuery("UPDATE TouchPortalBoardConfig f0 SET f0.showOrder = f0.showOrder - 1 WHERE f0.dataType = :dataType " +
                "and f0.showOrder> " +
                "(SELECT f1.showOrder FROM TouchPortalBoardConfig f1 WHERE f1.id= :id and f1.dataType = :dataType)")
                .setParameter("id", id)
                .setParameter("dataType", SystemConfig.DESIGN_TIME_INDICATOR)
                .executeUpdate();
        session.createQuery("delete from TouchPortalBoardConfig where id = :id")
                .setParameter("id", id)
                .executeUpdate();
    }

    @Override
    public void modify(TouchPortalBoardConfig touchPortalBoardConfig) {
        sessionFactory.getCurrentSession().update(touchPortalBoardConfig);
    }

    @Override
    public void changeOrder(int srcID, int destID) {
        Session session = sessionFactory.getCurrentSession();
        TouchPortalBoardConfig srcBoard = (TouchPortalBoardConfig) session.get(TouchPortalBoardConfig.class, srcID);
        TouchPortalBoardConfig destBoard = (TouchPortalBoardConfig) session.get(TouchPortalBoardConfig.class, destID);

        if (srcBoard != null && destBoard != null) {
            int srcOrder = srcBoard.getShowOrder();
            srcBoard.setShowOrder(destBoard.getShowOrder());
            destBoard.setShowOrder(srcOrder);

            srcBoard.setUpdateTime(new Timestamp(System.currentTimeMillis()));
            destBoard.setUpdateTime(new Timestamp(System.currentTimeMillis()));

            session.merge(srcBoard);
            session.merge(destBoard);
        }
    }
}
