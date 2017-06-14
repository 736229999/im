package com.feiniu.imServer.repository.impl;

import com.feiniu.imServer.bean.RecentConnectInfo;
import com.feiniu.imServer.consts.SystemConfig;
import com.feiniu.imServer.repository.RecentInfoDao;
import com.feiniu.imServer.util.TransformService;
import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by wangzhen on 2016/2/1.
 */

@SuppressWarnings({" unused", " unchecked", " rawtypes"})
@Repository
@Transactional
public class RecentInfoDaoImpl implements RecentInfoDao {
    @Autowired
    private SessionFactory sessionFactory;

    @Override
    public List<RecentConnectInfo> getRecentServiceInfo(long sid, int maxNumber) {
        Map<String, Object> params = new HashMap<>(1);

        StringBuilder stringBuilder = new StringBuilder(" " +
                " SELECT serviceId AS id ,sendTime AS time,nickName AS nickName," +
                "   userPicUrl AS headImgUrl  ,lastMsg AS lastMsg ,userSign AS userSign FROM ( " +
                " SELECT rownum as rowIndex,AA.*,BB.MSG AS lastMsg FROM( " +
                " SELECT A.TOID AS serviceId,A.SENDTIME AS sendTime ,B.NICKNAME AS nickName,B.USERPICURI AS userPicUrl,B.USERSIGN AS userSign " +
                " FROM " +
                " IM.IM_WEBLIST A , IM.IM_USERINFO B " +
                " WHERE A.TOID=B.USERID AND A.FROMID=:sid AND A.TOID <" + SystemConfig.MAX_SERVICE_ID.toString()+
                " ORDER BY A.SENDTIME DESC) AA " +
                " LEFT JOIN IM.IM_WEBDATA BB " +
                " ON AA.SENDTIME=BB.SENDTIME AND AA.SERVICEID=BB.TOID) " +
                " where  rowIndex < :toRowIndex ");

        params.put("sid", sid);
        params.put("toRowIndex", maxNumber);

        return TransformService.transformResult(
                sessionFactory.getCurrentSession().createSQLQuery(stringBuilder.toString()),
                params,
                RecentConnectInfo.class
        );
    }

    @Override
    public Long getRecentServiceInfoBySubMerchantList(long sid, String subMerchantIdList) {
        Map<String, Object> params = new HashMap<>();
        Session session = sessionFactory.getCurrentSession();
        Query query;

        StringBuilder stringBuilder = new StringBuilder(" SELECT * FROM (SELECT A.FROMID  AS serviceId " +
                " FROM " +
                " IM.IM_WEBLIST A , IM.IM_USERINFO B " +
                " WHERE A.TOID=B.USERID AND A.TOID= " + String.valueOf(sid)+
                "  AND " +
                "  A.FROMID IN ( " + subMerchantIdList +
                " )" +
                " ORDER BY A.SENDTIME DESC ) WHERE ROWNUM=1");


        query = session.createSQLQuery(stringBuilder.toString());

        List resultIdList =query.list();
        return resultIdList == null || resultIdList.isEmpty() ? null :  Long.valueOf(resultIdList.get(0).toString()) ;
    }
}
