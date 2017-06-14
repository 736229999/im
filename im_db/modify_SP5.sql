

---------------------20150717-----------------------

alter table IM_FRIEND modify UserName varchar2(80);

alter table IM_WEBDATA modify FROMNICKNAME varchar2(80);



---------------------20150729-----------------------
---修改用户名和群名称长度
alter table IM_CLUSTER modify CLUSTERNAME varchar2(80);
alter table IM_CLUSTERGROUP modify CLUSTERGROUPNAME varchar2(80);
alter table IM_ClUSTERMEMBER modify USERNAME varchar2(80);
alter table IM_CLUSTERMEMBER modify PROFILENAME varchar2(80);
alter table IM_CURCLILIST modify USERNAME varchar2(80);
alter table IM_FRIENDGROUP modify FRIENDGROUPNAME varchar2(80);

---------------------20150806-----------------------



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

Date: 2015-08-06 09:26:00
*/


-- ----------------------------
-- Table structure for IM_STATISITC_STATUS
-- ----------------------------
CREATE TABLE "IM"."IM_STATISITC_STATUS" (
"STATISITC_TIME" NUMBER(20) NOT NULL ,
"STATISITC_TYPE" NUMBER(10) NOT NULL ,
"STATISITC_STATUS" NUMBER(10) NOT NULL ,
"RETRY_TIME" NUMBER(10) NOT NULL ,
"TRY_IP" VARCHAR2(50 BYTE) NOT NULL,
"UPDATE_TIME" TIMESTAMP(6)  DEFAULT systimestamp  NOT NULL 
)
TABLESPACE ecsdata;

-- ----------------------------
-- Indexes structure for table IM_STATISITC_STATUS
-- ----------------------------

-- ----------------------------
-- Checks structure for table IM_STATISITC_STATUS
-- ----------------------------
ALTER TABLE "IM"."IM_STATISITC_STATUS" ADD CHECK ("STATISITC_TIME" IS NOT NULL);
ALTER TABLE "IM"."IM_STATISITC_STATUS" ADD CHECK ("STATISITC_TYPE" IS NOT NULL);
ALTER TABLE "IM"."IM_STATISITC_STATUS" ADD CHECK ("STATISITC_STATUS" IS NOT NULL);
ALTER TABLE "IM"."IM_STATISITC_STATUS" ADD CHECK ("RETRY_TIME" IS NOT NULL);
ALTER TABLE "IM"."IM_STATISITC_STATUS" ADD CHECK ("TRY_IP" IS NOT NULL);
ALTER TABLE "IM"."IM_STATISITC_STATUS" ADD CHECK ("UPDATE_TIME" IS NOT NULL);

-- ----------------------------
-- Primary Key structure for table IM_STATISITC_STATUS
-- ----------------------------
ALTER TABLE "IM"."IM_STATISITC_STATUS" ADD PRIMARY KEY ("STATISITC_TIME", "STATISITC_TYPE");



COMMENT ON TABLE "IM"."IM_STATISITC_STATUS" IS '统计状态表，所有的统计任务需要更新这张表的内容，互斥进行跑批';
COMMENT ON COLUMN "IM"."IM_STATISITC_STATUS"."STATISITC_TIME" IS '统计的时间，为当天的0点0分0秒';
COMMENT ON COLUMN "IM"."IM_STATISITC_STATUS"."STATISITC_TYPE" IS '统计的类型，目前有运营后台统计和客服绩效统计';
COMMENT ON COLUMN "IM"."IM_STATISITC_STATUS"."STATISITC_STATUS" IS '统计的状态：未统计，统计失败，已经统计，正在统计中';
COMMENT ON COLUMN "IM"."IM_STATISITC_STATUS"."RETRY_TIME" IS '重试的次数';
COMMENT ON COLUMN "IM"."IM_STATISITC_STATUS"."TRY_IP" IS '上一次统计执行的IP地址';


