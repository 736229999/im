/**创建客服出勤明细表*/
CREATE TABLE IM_SUBMERCHANT_ATTENDANCE (
  i_submerchant_id         NUMBER(15) NOT NULL,
  i_merchant_id            NUMBER(12) NOT NULL,
  i_statistics_time        NUMBER(11) NOT NULL,
  i_login_count            NUMBER(3) DEFAULT 0,
  i_first_online_time      NUMBER(11),
  i_last_online_time       NUMBER(11),
  i_total_online_duration  NUMBER(6) DEFAULT 0,
  i_total_hanging_duration NUMBER(6) DEFAULT 0,
  i_total_offline_duration NUMBER(6) DEFAULT 0,
  update_time              TIMESTAMP DEFAULT systimestamp,

  PRIMARY KEY (i_submerchant_id, i_merchant_id, i_statistics_time)
);

/** 2015/12/07 增加离开事件的统计**/
ALTER TABLE IM_SUBMERCHANT_ATTENDANCE ADD i_total_leave_num NUMBER(6) DEFAULT 0;
ALTER TABLE IM_SUBMERCHANT_ATTENDANCE ADD i_total_leave_duration NUMBER(6) DEFAULT 0;
UPDATE IM_SUBMERCHANT_ATTENDANCE
SET i_total_leave_duration = 0, i_total_leave_num = 0;

