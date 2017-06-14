package com.cn.feiniu.logManagement.repository.impl;

import com.cn.feiniu.logManagement.bean.dto.SubMerchantAttendanceDetail;
import com.cn.feiniu.logManagement.repository.SubMerchantActionStatisticsDao;
import com.cn.feiniu.logManagement.util.db.DBUtil;
import org.hibernate.SQLQuery;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Author: morningking
 * Date: 2015/7/31
 * Contact: 243717042@qq.com
 */
@Repository
public class SubMerchantActionStatisticsDaoImpl implements SubMerchantActionStatisticsDao {
    @Autowired
    private SessionFactory sessionFactory;

    @Override
    public void saveActionStatisticsList(List<SubMerchantAttendanceDetail> subMerchantAttendanceDetailList) {
        Session session = sessionFactory.getCurrentSession();
        SQLQuery query = session.createSQLQuery("MERGE INTO IM_SUBMERCHANT_ATTENDANCE t_old" +
                " USING (SELECT" +
                "         :submerchantID AS i_submerchant_id, :merchantID AS i_merchant_id, :statisticsTime AS i_statistics_time, " +
                " :loginCount AS i_login_count, :firstOnlineTime AS i_first_online_time, :lastOnlineTime AS i_last_online_time, " +
                " :totalOnlineDuration AS i_total_online_duration, :totalHangDuration AS i_total_hanging_duration, " +
                " :totalOfflineDuration AS i_total_offline_duration, :totalLeaveNum AS i_total_leave_num," +
                " :totalLeaveDuration AS i_total_leave_duration FROM DUAL) t_new" +
                " ON (t_old.i_submerchant_id = t_new.i_submerchant_id AND t_old.i_merchant_id = t_new.i_merchant_id AND" +
                "    t_old.i_statistics_time = t_new.i_statistics_time)" +
                " WHEN MATCHED THEN UPDATE SET t_old.i_login_count = t_new.i_login_count, t_old.i_first_online_time = t_new.i_first_online_time," +
                " t_old.i_last_online_time = t_new.i_last_online_time, t_old.i_total_online_duration = t_new.i_total_online_duration, " +
                " t_old.i_total_hanging_duration = t_new.i_total_hanging_duration, t_old.i_total_offline_duration = t_new.i_total_offline_duration, " +
                " t_old.i_total_leave_duration = t_new.i_total_leave_duration, t_old.i_total_leave_num = t_new.i_total_leave_num," +
                " t_old.update_time = systimestamp" +
                " WHEN NOT MATCHED THEN INSERT (t_old.i_submerchant_id, t_old.i_merchant_id, t_old.i_statistics_time, t_old.i_login_count," +
                " t_old.i_first_online_time, t_old.i_last_online_time, t_old.i_total_online_duration, t_old.i_total_hanging_duration, t_old.i_total_offline_duration," +
                " t_old.i_total_leave_num, t_old.i_total_leave_duration)" +
                " VALUES (t_new.i_submerchant_id, t_new.i_merchant_id, t_new.i_statistics_time, t_new.i_login_count, t_new.i_first_online_time," +
                "  t_new.i_last_online_time, t_new.i_total_online_duration, t_new.i_total_hanging_duration, t_new.i_total_offline_duration," +
                "  t_new.i_total_leave_num, t_new.i_total_leave_duration)");

        Map<String, Object> params = new HashMap<>();
        for (SubMerchantAttendanceDetail subMerchantAttendanceDetail : subMerchantAttendanceDetailList) {
            params.put("submerchantID", subMerchantAttendanceDetail.getSubMerchantAttendanceDetailKey().getSubMerchantID());
            params.put("merchantID", subMerchantAttendanceDetail.getSubMerchantAttendanceDetailKey().getMerchantID());
            params.put("statisticsTime", subMerchantAttendanceDetail.getSubMerchantAttendanceDetailKey().getStatisticsTime());
            params.put("loginCount", subMerchantAttendanceDetail.getLoginCount());
            params.put("firstOnlineTime", subMerchantAttendanceDetail.getFirstOnlineTime());
            params.put("lastOnlineTime", subMerchantAttendanceDetail.getLastOnlineTime());
            params.put("totalOnlineDuration", subMerchantAttendanceDetail.getTotalOnlineDuration());
            params.put("totalHangDuration", subMerchantAttendanceDetail.getTotalHangingDuration());
            params.put("totalOfflineDuration", subMerchantAttendanceDetail.getTotalOfflineDuration());
            params.put("totalLeaveNum", subMerchantAttendanceDetail.getTotalLeaveNum());
            params.put("totalLeaveDuration", subMerchantAttendanceDetail.getTotalLeaveDuration());

            DBUtil.executeUpdate(query, params);
        }

        session.flush();
    }
}
