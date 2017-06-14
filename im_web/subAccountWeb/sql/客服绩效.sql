/*
Navicat Oracle Data Transfer
Oracle Client Version : 10.2.0.5.0

Source Server         : te
Source Server Version : 110200
Source Host           : 10.211.64.108:1521
Source Schema         : IM

Target Server Type    : ORACLE
Target Server Version : 110200
File Encoding         : 65001

Date: 2015-08-06 09:32:35
*/


-- ----------------------------
-- Table structure for T_SESSION_DETAIL
-- ----------------------------
CREATE global temporary table  "IM"."T_SESSION_DETAIL" (
"I_FROMID" NUMBER(20) NOT NULL ,
"I_TOID" NUMBER(20) NOT NULL ,
"I_LOGINTIME" NUMBER(20) NOT NULL ,
"I_ASKTIME" NUMBER(20) NULL ,
"I_REPLYTIME" NUMBER(20) NULL ,
"I_DUR" NUMBER(20) NULL ,
"I_MESSAGE_COUNT" NUMBER(20) NULL ,
"I_START_TIME" NUMBER(20) NULL ,
"I_END_TIME" NUMBER(20) NULL ,
"I_SESSION_LEN" NUMBER(20) NULL ,
"I_REPLY_START" NUMBER(20) NULL ,
"I_REPLY_END" NUMBER(20) NULL ,
"I_REPLY_LEN" NUMBER(20) NULL 
)
On Commit Delete Rows ;

-- ----------------------------
-- Checks structure for table T_SESSION_DETAIL
-- ----------------------------
ALTER TABLE "IM"."T_SESSION_DETAIL" ADD CHECK ("I_FROMID" IS NOT NULL);
ALTER TABLE "IM"."T_SESSION_DETAIL" ADD CHECK ("I_TOID" IS NOT NULL);
ALTER TABLE "IM"."T_SESSION_DETAIL" ADD CHECK ("I_LOGINTIME" IS NOT NULL);


COMMENT ON TABLE "IM"."T_SESSION_DETAIL" IS '回复明细表:临时表，用于存放统计出来的会话的数据';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_FROMID" IS '会话中商家的ID';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_TOID" IS '会话中买家的ID';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_LOGINTIME" IS '会话标示';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_ASKTIME" IS '买家发送的第一条消息的时间';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_REPLYTIME" IS '卖家发送的第一条消息的时间';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_DUR" IS '卖家回复买家中间的时间差';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_MESSAGE_COUNT" IS '会话中卖家一共回复的消息数';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_START_TIME" IS '会话开始的时间';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_END_TIME" IS '会话结束的时间';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_SESSION_LEN" IS '会话的时长';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_REPLY_START" IS '商家一次回复的时间';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_REPLY_END" IS '商家最后一次回复的时间';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_REPLY_LEN" IS '商家第一次回复到最后一次回复的时间差';


/*
Navicat Oracle Data Transfer
Oracle Client Version : 10.2.0.5.0

Source Server         : te
Source Server Version : 110200
Source Host           : 10.211.64.108:1521
Source Schema         : IM

Target Server Type    : ORACLE
Target Server Version : 110200
File Encoding         : 65001

Date: 2015-08-06 09:32:08
*/


-- ----------------------------
-- Table structure for IM_SUBMERCHANT_ATTENDANCE
-- ----------------------------
CREATE TABLE "IM"."IM_SUBMERCHANT_ATTENDANCE" (
"I_SUBMERCHANT_ID" NUMBER(15) NOT NULL ,
"I_MERCHANT_ID" NUMBER(12) NOT NULL ,
"I_STATISTICS_TIME" NUMBER(11) NOT NULL ,
"I_LOGIN_COUNT" NUMBER(3) DEFAULT 0  NULL ,
"I_FIRST_ONLINE_TIME" NUMBER(11) NULL ,
"I_LAST_ONLINE_TIME" NUMBER(11) NULL ,
"I_TOTAL_ONLINE_DURATION" NUMBER(6) DEFAULT 0  NULL ,
"I_TOTAL_HANGING_DURATION" NUMBER(6) DEFAULT 0  NULL ,
"I_TOTAL_OFFLINE_DURATION" NUMBER(6) DEFAULT 0  NULL ,
"UPDATE_TIME" TIMESTAMP(6)  DEFAULT systimestamp  NOT NULL 
)
TABLESPACE ecsdata;

-- ----------------------------
-- Indexes structure for table IM_SUBMERCHANT_ATTENDANCE
-- ----------------------------
CREATE INDEX "IM"."ATTENDACNCE_STATISTICS_TIME"
ON "IM"."IM_SUBMERCHANT_ATTENDANCE" ("I_STATISTICS_TIME" ASC)
LOGGING
VISIBLE;

-- ----------------------------
-- Checks structure for table IM_SUBMERCHANT_ATTENDANCE
-- ----------------------------
ALTER TABLE "IM"."IM_SUBMERCHANT_ATTENDANCE" ADD CHECK ("I_SUBMERCHANT_ID" IS NOT NULL);
ALTER TABLE "IM"."IM_SUBMERCHANT_ATTENDANCE" ADD CHECK ("I_MERCHANT_ID" IS NOT NULL);
ALTER TABLE "IM"."IM_SUBMERCHANT_ATTENDANCE" ADD CHECK ("I_STATISTICS_TIME" IS NOT NULL);

-- ----------------------------
-- Primary Key structure for table IM_SUBMERCHANT_ATTENDANCE
-- ----------------------------
ALTER TABLE "IM"."IM_SUBMERCHANT_ATTENDANCE" ADD PRIMARY KEY ("I_SUBMERCHANT_ID", "I_MERCHANT_ID", "I_STATISTICS_TIME");



COMMENT ON TABLE "IM"."IM_SUBMERCHANT_ATTENDANCE" IS '出勤率统计';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_ATTENDANCE"."I_SUBMERCHANT_ID" IS '子账号的ID';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_ATTENDANCE"."I_MERCHANT_ID" IS '商家的ID';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_ATTENDANCE"."I_STATISTICS_TIME" IS '统计时间：当天的0点0分0秒';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_ATTENDANCE"."I_LOGIN_COUNT" IS '子账号当天登陆的次数';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_ATTENDANCE"."I_FIRST_ONLINE_TIME" IS '子账号当天第一次登陆的时间';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_ATTENDANCE"."I_LAST_ONLINE_TIME" IS '子账号当天最后一次登陆的时间';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_ATTENDANCE"."I_TOTAL_ONLINE_DURATION" IS '子账号当天在线的总时长';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_ATTENDANCE"."I_TOTAL_HANGING_DURATION" IS '子账号当天挂起的总时长';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_ATTENDANCE"."I_TOTAL_OFFLINE_DURATION" IS '子账号当天离线的总时长';


/*
Navicat Oracle Data Transfer
Oracle Client Version : 10.2.0.5.0

Source Server         : te
Source Server Version : 110200
Source Host           : 10.211.64.108:1521
Source Schema         : IM

Target Server Type    : ORACLE
Target Server Version : 110200
File Encoding         : 65001

Date: 2015-08-06 09:31:58
*/


-- ----------------------------
-- Table structure for IM_SUBMERCHANT_WORKLOAD
-- ----------------------------
CREATE TABLE "IM"."IM_SUBMERCHANT_WORKLOAD" (
"I_SUBMERCHANT_ID" NUMBER(10) NOT NULL ,
"I_MERCHANT_ID" NUMBER(10) NOT NULL ,
"I_STATISTICS_TIME" NUMBER(20) NOT NULL ,
"I_RESPONSE_DURATION" NUMBER(10) NOT NULL ,
"I_SESSION_DURATION" NUMBER(10) NOT NULL ,
"I_SESSION_COUNT" NUMBER(10) NOT NULL ,
"I_RESPONSE_MESSAGE_COUNT" NUMBER(10) NOT NULL ,
"I_RECEPTION_DURATION" NUMBER(10) NOT NULL ,
"V_RESPONSE_IDS" CLOB NULL ,
"V_ASKING_IDS" CLOB NULL ,
"I_RESPONSE_COUNT" NUMBER(10) NOT NULL ,
"I_RESPONSE_IN_90S_COUNT" NUMBER(10) NOT NULL ,
"UPDATE_TIME" TIMESTAMP(6)  DEFAULT systimestamp  NOT NULL 
)
TABLESPACE ecsdata;
-- ----------------------------
-- Indexes structure for table IM_SUBMERCHANT_WORKLOAD
-- ----------------------------
CREATE INDEX "IM"."WORKLOAD_STATISTICS_TIME"
ON "IM"."IM_SUBMERCHANT_WORKLOAD" ("I_STATISTICS_TIME" ASC)
LOGGING
VISIBLE;

-- ----------------------------
-- Checks structure for table IM_SUBMERCHANT_WORKLOAD
-- ----------------------------
ALTER TABLE "IM"."IM_SUBMERCHANT_WORKLOAD" ADD CHECK ("I_SUBMERCHANT_ID" IS NOT NULL);
ALTER TABLE "IM"."IM_SUBMERCHANT_WORKLOAD" ADD CHECK ("I_MERCHANT_ID" IS NOT NULL);
ALTER TABLE "IM"."IM_SUBMERCHANT_WORKLOAD" ADD CHECK ("I_STATISTICS_TIME" IS NOT NULL);
ALTER TABLE "IM"."IM_SUBMERCHANT_WORKLOAD" ADD CHECK ("I_RESPONSE_DURATION" IS NOT NULL);
ALTER TABLE "IM"."IM_SUBMERCHANT_WORKLOAD" ADD CHECK ("I_SESSION_DURATION" IS NOT NULL);
ALTER TABLE "IM"."IM_SUBMERCHANT_WORKLOAD" ADD CHECK ("I_SESSION_COUNT" IS NOT NULL);
ALTER TABLE "IM"."IM_SUBMERCHANT_WORKLOAD" ADD CHECK ("I_RESPONSE_MESSAGE_COUNT" IS NOT NULL);
ALTER TABLE "IM"."IM_SUBMERCHANT_WORKLOAD" ADD CHECK ("I_RECEPTION_DURATION" IS NOT NULL);
ALTER TABLE "IM"."IM_SUBMERCHANT_WORKLOAD" ADD CHECK ("I_RESPONSE_COUNT" IS NOT NULL);
ALTER TABLE "IM"."IM_SUBMERCHANT_WORKLOAD" ADD CHECK ("I_SUBMERCHANT_ID" IS NOT NULL);
ALTER TABLE "IM"."IM_SUBMERCHANT_WORKLOAD" ADD CHECK ("I_MERCHANT_ID" IS NOT NULL);
ALTER TABLE "IM"."IM_SUBMERCHANT_WORKLOAD" ADD CHECK ("I_STATISTICS_TIME" IS NOT NULL);
ALTER TABLE "IM"."IM_SUBMERCHANT_WORKLOAD" ADD CHECK ("I_RESPONSE_DURATION" IS NOT NULL);
ALTER TABLE "IM"."IM_SUBMERCHANT_WORKLOAD" ADD CHECK ("I_SESSION_DURATION" IS NOT NULL);
ALTER TABLE "IM"."IM_SUBMERCHANT_WORKLOAD" ADD CHECK ("I_SESSION_COUNT" IS NOT NULL);
ALTER TABLE "IM"."IM_SUBMERCHANT_WORKLOAD" ADD CHECK ("I_RESPONSE_MESSAGE_COUNT" IS NOT NULL);
ALTER TABLE "IM"."IM_SUBMERCHANT_WORKLOAD" ADD CHECK ("I_RECEPTION_DURATION" IS NOT NULL);
ALTER TABLE "IM"."IM_SUBMERCHANT_WORKLOAD" ADD CHECK ("I_RESPONSE_COUNT" IS NOT NULL);

-- ----------------------------
-- Primary Key structure for table IM_SUBMERCHANT_WORKLOAD
-- ----------------------------
ALTER TABLE "IM"."IM_SUBMERCHANT_WORKLOAD" ADD PRIMARY KEY ("I_SUBMERCHANT_ID", "I_STATISTICS_TIME");



COMMENT ON TABLE "IM"."IM_SUBMERCHANT_WORKLOAD" IS '工作量统计';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_WORKLOAD"."I_SUBMERCHANT_ID" IS '子账号的ID';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_WORKLOAD"."I_MERCHANT_ID" IS '商家的ID';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_WORKLOAD"."I_STATISTICS_TIME" IS '统计时间：当天的0点0分0秒';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_WORKLOAD"."I_RESPONSE_DURATION" IS '子账号响应客户的时间总和';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_WORKLOAD"."I_SESSION_DURATION" IS '子账号所有会话的时间总和';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_WORKLOAD"."I_SESSION_COUNT" IS '子账号所有会话的个数';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_WORKLOAD"."I_RESPONSE_MESSAGE_COUNT" IS '子账号所有回复的消息条数';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_WORKLOAD"."I_RECEPTION_DURATION" IS '子账号接待总时长';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_WORKLOAD"."V_RESPONSE_IDS" IS '子账号回复过的所有客户的ID';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_WORKLOAD"."V_ASKING_IDS" IS '所有咨询过该子账号的客户的ID';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_WORKLOAD"."I_RESPONSE_COUNT" IS '子账号回复过的次数';


/*
Navicat Oracle Data Transfer
Oracle Client Version : 10.2.0.5.0

Source Server         : te
Source Server Version : 110200
Source Host           : 10.211.64.108:1521
Source Schema         : IM

Target Server Type    : ORACLE
Target Server Version : 110200
File Encoding         : 65001

Date: 2015-08-06 09:31:58
*/

CREATE OR REPLACE 
PROCEDURE SplitStringSp(P_INSTR IN VARCHAR2,--传入字符串，逗号分隔，输入参数
P_STRS OUT VARCHAR2,--截取第1个值之后剩下的字符串，输出参数
P_CURR OUT VARCHAR2, --截取到的第1个值，输出参数
P_CHAR IN VARCHAR2 --以什么字符为分隔字符
) 
IS
CURR_LOCA NUMBER;
BEGIN
    CURR_LOCA := INSTR(P_INSTR, P_CHAR, 1, 1);
    P_CURR  := SUBSTR(P_INSTR, 0, CURR_LOCA - 1);
    IF P_CURR IS NULL THEN
			P_CURR := P_INSTR;
			P_STRS := NULL;
    ELSE
			P_STRS := SUBSTR(P_INSTR, CURR_LOCA + 1);
    END IF;
END;

/*
Navicat Oracle Data Transfer
Oracle Client Version : 10.2.0.5.0

Source Server         : te
Source Server Version : 110200
Source Host           : 10.211.64.108:1521
Source Schema         : IM

Target Server Type    : ORACLE
Target Server Version : 110200
File Encoding         : 65001

Date: 2015-08-06 09:31:58
*/

CREATE OR REPLACE 
PROCEDURE statisticsUpdateReplyDetail
 (startTime in NUMBER ,
endTime in NUMBER)AS
BEGIN
--回复情况分5种：
------客户先发消息：（1）客服回了   DUR>0
---------------------(2) 客服没回   dur=-2
------主动接待：
-------客服先发消息：(3) 客户在线 并回复了消息  dur=-1
-------客服先发消息：(4) 客户在线 但没有回复了消息  dur=0
-------客服先发消息：(5) 客户不在线   dur=-1

--合并表，而不是直接插入
MERGE INTO IM.T_SESSION_DETAIL p USING(
--将统计出来的基础数据转换为reply信息
		SELECT r.fromid,r.toid,r.logintime,r.askTime,r.replytime,
		(CASE WHEN r.asktime IS NULL THEN (CASE WHEN r.logintime = 0 THEN -3 ELSE 0 END) ELSE 
			(CASE WHEN r.replytime is NULL THEN -2 ELSE 
				(CASE WHEN r.replytime - r.asktime < 0 THEN -1 ELSE r.replytime - r.asktime END)
			END) 
		END) AS DUR ,

		r.start_time AS start_time,
		r.end_time AS end_time,
		(r.END_TIME - r.START_TIME) AS session_len,
		r.MESSAGE_COUNT as message_count,
		r.reply_start AS reply_start,
		r.reply_end AS reply_end,
		(r.reply_end - r.reply_start) AS reply_len
		FROM(
--根据LOGINTIME统计这段时间内所有会话的基础数据。
					SELECT 
					(CASE WHEN a.fromid is NULL THEN b.TOID ELSE a.FROMID END) AS toid,
					(CASE WHEN a.toid is NULL THEN b.fromid ELSE a.toid END) AS fromid,
					(CASE WHEN a.LOGINTIME is NULL THEN b.LOGINTIME ELSE a.LOGINTIME END) AS logintime,
					a.time AS askTime,
					b.time AS replytime,
					(CASE WHEN a.time IS NULL THEN b.time ELSE 
						(CASE WHEN b.TIME IS NULL THEN a.time ELSE 
						(CASE WHEN b.TIME < a.time THEN b.time ELSE a.time END ) END) END) AS  start_time,

					(CASE WHEN a.END_TIME IS NULL THEN b.END_TIME ELSE 
						(CASE WHEN b.END_TIME IS NULL THEN a.END_TIME ELSE 
						(CASE WHEN b.END_TIME > a.END_TIME THEN b.END_TIME ELSE a.END_TIME END ) END) END) AS  end_time,

					b.message_count as message_count,

					(CASE WHEN b.time IS NULL THEN 0 ELSE b.time  END) AS reply_start,
					(CASE WHEN b.END_TIME IS NULL THEN 0 ELSE b.END_TIME  END) AS reply_end

					 FROM(
--根据LOGINTIME统计这段时间内所有聊天记录拼接成会话。
									SELECT IM.IM_WEBDATA.FROMID,IM.IM_WEBDATA.TOID,IM.IM_WEBDATA.LOGINTIME AS LOGINTIME,MIN (IM.IM_WEBDATA.SENDTIME) AS TIME
									,MAX(IM.IM_WEBDATA.SENDTIME) AS END_TIME
											FROM IM.IM_WEBDATA 
											WHERE IM.IM_WEBDATA.FROMID > 5000000
											AND IM.IM_WEBDATA.TOID < 5000000
											AND IM.IM_WEBDATA.DATATYPE <> 21
											AND IM.IM_WEBDATA.SENDTIME > startTime
											AND IM.IM_WEBDATA.SENDTIME < endTime
											AND IM.IM_WEBDATA.LOGINTIME <> 1
											GROUP BY
												IM.IM_WEBDATA.FROMID,
												IM.IM_WEBDATA.TOID,
												IM.IM_WEBDATA.LOGINTIME
											ORDER BY
												IM.IM_WEBDATA.FROMID ASC,
												IM.IM_WEBDATA.TOID ASC
									) a FULL JOIN (
									SELECT IM.IM_WEBDATA.FROMID,IM.IM_WEBDATA.TOID,IM.IM_WEBDATA.LOGINTIME AS LOGINTIME,MIN (IM.IM_WEBDATA.SENDTIME) AS TIME
									,MAX(IM.IM_WEBDATA.SENDTIME) AS END_TIME,COUNT(*) AS MESSAGE_COUNT
											FROM IM.IM_WEBDATA 
											WHERE IM.IM_WEBDATA.FROMID < 5000000
											AND IM.IM_WEBDATA.TOID > 5000000
											AND IM.IM_WEBDATA.DATATYPE <> 21
											AND IM.IM_WEBDATA.SENDTIME > startTime
											AND IM.IM_WEBDATA.SENDTIME < endTime
											AND IM.IM_WEBDATA.LOGINTIME <> 1
											GROUP BY
												IM.IM_WEBDATA.FROMID,
												IM.IM_WEBDATA.TOID,
												IM.IM_WEBDATA.LOGINTIME
											ORDER BY
												IM.IM_WEBDATA.FROMID ASC,
												IM.IM_WEBDATA.TOID ASC
									)b
									ON(
										a.FROMID = b.TOID AND
										a.TOID = b.FROMID AND
										a.LOGINTIME = b.LOGINTIME
									)
						)r

)pn
ON(   
		p.I_FROMID   = pn.FROMID    AND p.I_TOID   = pn.TOID AND p.I_logintime = pn.logintime
 ) 
--已经有了该条统计信息，更新数据
WHEN MATCHED THEN  UPDATE  SET p.I_ASKTIME = pn.ASKTIME,p.I_DUR = pn.DUR ,p.I_REPLYTIME = pn.REPLYTIME,
p.I_start_Time = pn.start_time,p.I_end_time = pn.end_time,p.I_session_len=pn.session_len,p.I_message_count=pn.message_count,
p.I_reply_start = pn.reply_start,p.I_reply_end = pn.reply_end,p.I_reply_len = pn.reply_len
--没有，插入数据
WHEN NOT MATCHED THEN  INSERT VALUES 
	(    pn.FROMID ,    pn.TOID , pn.logintime,   pn.ASKTIME ,    pn.REPLYTIME,  pn.DUR,pn.message_count,pn.start_time,pn.end_time,pn.session_len,
	pn.reply_start,pn.reply_end,pn.reply_len);

END;

/*
Navicat Oracle Data Transfer
Oracle Client Version : 10.2.0.5.0

Source Server         : te
Source Server Version : 110200
Source Host           : 10.211.64.108:1521
Source Schema         : IM

Target Server Type    : ORACLE
Target Server Version : 110200
File Encoding         : 65001

Date: 2015-08-06 09:31:58
*/

CREATE OR REPLACE 
PROCEDURE statisticsWorkLoad
	(startTime in NUMBER ,
	endTime in NUMBER,
	subMerchantId in VARCHAR2,
	merchantId in VARCHAR2,
	statisticsDate in NUMBER
	)AS
	responseTime integer:= 0;
	responseCount integer:= 0;
	responseIn90sCount integer:= 0;
	sessionTime integer:= 0;
	sessionCount integer:= 0;
	responseMessageCount integer:= 0;
	receptionTime integer:= 0;
	responseIds CLOB:='';
	askingIds CLOB:='';
	updateTime VARCHAR2(50);
	v_str_merchantid VARCHAR2(4000):='';
	v_res_str_merchantid VARCHAR2(4000):='';
	v_current_merchantid VARCHAR2(50):='';
	v_str_submerchantid VARCHAR2(4000):='';
	v_res_str_submerchantid VARCHAR2(4000):='';
	v_current_submerchantid VARCHAR2(50):='';
	resultCount integer:=0;
	BEGIN
	--获取时间
		select to_char(CURRENT_TIMESTAMP,'YYYYMMDD HH24:MI:SS.FF9') INTO updateTime from dual;
		
		v_str_merchantid := merchantId;
		v_str_submerchantid := subMerchantId;
		
	loop
		EXIT WHEN v_str_submerchantid IS NULL;
		responseTime := 0;
		responseCount := 0;
		sessionTime :=0;
		sessionCount := 0;
		responseMessageCount:= 0;
		receptionTime := 0;
		responseIds :='';
		askingIds :='';
		responseIn90sCount := 0;
		
		SplitStringSp(v_str_submerchantid, v_res_str_submerchantid, v_current_submerchantid, ';');
		SplitStringSp(v_str_merchantid, v_res_str_merchantid, v_current_merchantid, ';');
		dbms_output.put_line('开始统计 v_current_submerchantid');
		dbms_output.put_line(v_current_submerchantid);
		
	--判断是否有统计数据
		SELECT (CASE WHEN COUNT(*) = NULL THEN 0 ELSE COUNT(*) END )INTO resultCount 	FROM T_SESSION_DETAIL p 
		WHERE p.I_start_Time < endTime AND p.I_start_Time > startTime AND p.I_FROMID = v_current_submerchantid;
		
		dbms_output.put_line('开始统计 count：');
		dbms_output.put_line(resultCount);
	IF ( resultCount <>0 ) THEN
	--统计responseTime,responseCount,sessionTime,sessionCount,responseMessageCount,receptionTime
		SELECT 
			SUM(CASE WHEN p.I_DUR >0 OR p.I_DUR =0  THEN p.I_DUR ELSE 0 END) AS response_time,
			SUM(CASE WHEN p.I_DUR >0 OR p.I_DUR =0 OR p.I_DUR = -1 THEN 1 ELSE 0 END) AS response_count,
			SUM(CASE WHEN (p.I_DUR >0 OR p.I_DUR =0) AND p.I_DUR<90 THEN 1 ELSE 0 END) AS response_In90s_Count,
			SUM(CASE WHEN p.I_DUR >0 OR p.I_DUR =0 OR p.I_DUR = -1 THEN p.I_session_len ELSE 0 END) AS SESSION_time,
			SUM(CASE WHEN p.I_DUR >0 OR p.I_DUR =0 OR p.I_DUR = -1 THEN 1 ELSE 0 END) AS SESSION_count,
			SUM(CASE WHEN p.I_DUR >0 OR p.I_DUR =0 OR p.I_DUR = -1 THEN p.I_message_count ELSE 0 END) AS message_count,
			SUM(CASE WHEN p.I_DUR >0 OR p.I_DUR =0 OR p.I_DUR = -1 THEN p.I_reply_len ELSE 0 END) AS reception_time
		INTO responseTime,responseCount,responseIn90sCount,sessionTime,sessionCount,responseMessageCount,receptionTime
		FROM T_SESSION_DETAIL p 
		WHERE p.I_start_Time < endTime AND p.I_start_Time > startTime AND p.I_FROMID = v_current_submerchantid
		GROUP BY p.I_FROMID;
	--统计所有的咨询过的ID列表
		SELECT listagg( p.I_toid,';') within GROUP (order by p.I_toid) AS ASKING_IDS INTO askingIds
		FROM T_SESSION_DETAIL p 
		WHERE p.I_start_Time < endTime AND p.I_start_Time > startTime AND p.I_FROMID = v_current_submerchantid ;
		
		
	--统计所有客服回复过的客户的ID列表
		SELECT  COUNT(*) INTO resultCount 	FROM T_SESSION_DETAIL p 
		WHERE p.I_start_Time < endTime AND p.I_start_Time > startTime AND p.I_FROMID = v_current_submerchantid AND  p.I_dur <> -2;
		
		IF ( resultCount <>0 ) THEN
			SELECT listagg( p.I_toid,';') within GROUP (order by p.I_toid) AS responseIds INTO responseIds
			FROM T_SESSION_DETAIL p 
			WHERE p.I_start_Time < endTime AND p.I_start_Time > startTime AND p.I_FROMID = v_current_submerchantid AND  p.I_dur <> -2
			GROUP BY p.I_FROMID;
		END IF;
		
	END IF;
	--插入数据到统计表，先删再插入
		DELETE FROM im.im_submerchant_workload p WHERE p.I_SUBMERCHANT_ID = v_current_submerchantid AND p.I_MERCHANT_ID = v_current_merchantid AND p.I_STATISTICS_TIME = statisticsDate;
		
 		insert into im.im_submerchant_workload(
  	I_SUBMERCHANT_ID,I_MERCHANT_ID,I_STATISTICS_TIME,I_RESPONSE_DURATION,I_SESSION_DURATION,I_SESSION_COUNT,I_RESPONSE_MESSAGE_COUNT,I_RECEPTION_DURATION,V_RESPONSE_IDS,V_ASKING_IDS,I_RESPONSE_COUNT,I_RESPONSE_IN_90S_COUNT)
  	values(v_current_submerchantid,v_current_merchantid,statisticsDate,responseTime,sessionTime,sessionCount,responseMessageCount,receptionTime,responseIds,askingIds,responseCount,responseIn90sCount);

	--
		v_str_submerchantid := v_res_str_submerchantid;
		v_str_merchantid := v_res_str_merchantid;
	end loop;
	END ;
