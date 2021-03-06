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

Date: 2015-08-21 14:40:45
*/


-- ----------------------------
-- Table structure for IM_RECEPTION_MAP
-- ----------------------------
CREATE TABLE "IM"."IM_RECEPTION_MAP" (
"I_SUBMERCHANT_ID" NUMBER(10) NOT NULL ,
"I_MERCHANT_ID" NUMBER(10) NOT NULL ,
"I_STATISITC_TIME" NUMBER(20) NOT NULL ,
"I_COUSTOMER_ID" NUMBER(10) NOT NULL ,
"I_REPLY" NUMBER(2) NOT NULL ,
"I_NOT_REPLY" NUMBER(2) NOT NULL ,
"UPDATE_TIME" TIMESTAMP(6)  DEFAULT systimestamp  NOT NULL 
)
TABLESPACE ecsdata;

COMMENT ON TABLE "IM"."IM_RECEPTION_MAP" IS '客服接待情况统计表';
COMMENT ON COLUMN "IM"."IM_RECEPTION_MAP"."I_SUBMERCHANT_ID" IS '子账号的ID';
COMMENT ON COLUMN "IM"."IM_RECEPTION_MAP"."I_MERCHANT_ID" IS '商家的ID';
COMMENT ON COLUMN "IM"."IM_RECEPTION_MAP"."I_STATISITC_TIME" IS '统计时间：当天的0点0分0秒';
COMMENT ON COLUMN "IM"."IM_RECEPTION_MAP"."I_COUSTOMER_ID" IS '接待的客户ID';
COMMENT ON COLUMN "IM"."IM_RECEPTION_MAP"."I_REPLY" IS '接待的次数';
COMMENT ON COLUMN "IM"."IM_RECEPTION_MAP"."I_NOT_REPLY" IS '没接待的次数';
COMMENT ON COLUMN "IM"."IM_RECEPTION_MAP"."UPDATE_TIME" IS '数据更新时间';

-- ----------------------------
-- Indexes structure for table IM_RECEPTION_MAP
-- ----------------------------
CREATE INDEX "IM"."subMerchantIDToid"
ON "IM"."IM_RECEPTION_MAP" ("I_SUBMERCHANT_ID" ASC, "I_MERCHANT_ID" ASC)
LOGGING
VISIBLE;

-- ----------------------------
-- Checks structure for table IM_RECEPTION_MAP
-- ----------------------------
ALTER TABLE "IM"."IM_RECEPTION_MAP" ADD CHECK ("I_SUBMERCHANT_ID" IS NOT NULL);
ALTER TABLE "IM"."IM_RECEPTION_MAP" ADD CHECK ("I_MERCHANT_ID" IS NOT NULL);
ALTER TABLE "IM"."IM_RECEPTION_MAP" ADD CHECK ("I_STATISITC_TIME" IS NOT NULL);
ALTER TABLE "IM"."IM_RECEPTION_MAP" ADD CHECK ("I_COUSTOMER_ID" IS NOT NULL);
ALTER TABLE "IM"."IM_RECEPTION_MAP" ADD CHECK ("I_REPLY" IS NOT NULL);
ALTER TABLE "IM"."IM_RECEPTION_MAP" ADD CHECK ("I_NOT_REPLY" IS NOT NULL);
ALTER TABLE "IM"."IM_RECEPTION_MAP" ADD CHECK ("UPDATE_TIME" IS NOT NULL);
ALTER TABLE "IM"."IM_RECEPTION_MAP" ADD CHECK ("I_SUBMERCHANT_ID" IS NOT NULL);
ALTER TABLE "IM"."IM_RECEPTION_MAP" ADD CHECK ("I_MERCHANT_ID" IS NOT NULL);
ALTER TABLE "IM"."IM_RECEPTION_MAP" ADD CHECK ("I_STATISITC_TIME" IS NOT NULL);
ALTER TABLE "IM"."IM_RECEPTION_MAP" ADD CHECK ("I_COUSTOMER_ID" IS NOT NULL);
ALTER TABLE "IM"."IM_RECEPTION_MAP" ADD CHECK ("I_REPLY" IS NOT NULL);
ALTER TABLE "IM"."IM_RECEPTION_MAP" ADD CHECK ("I_NOT_REPLY" IS NOT NULL);

-- ----------------------------
-- Primary Key structure for table IM_RECEPTION_MAP
-- ----------------------------
ALTER TABLE "IM"."IM_RECEPTION_MAP" ADD PRIMARY KEY ("I_SUBMERCHANT_ID", "I_MERCHANT_ID", "I_STATISITC_TIME", "I_COUSTOMER_ID");

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

Date: 2015-09-06 10:08:01
*/


-- ----------------------------
-- Table structure for IM_MERCHANT_LANDING_DETAIL
-- ----------------------------
CREATE TABLE "IM"."IM_MERCHANT_LANDING_DETAIL" (
"MERCHANT_ID" NUMBER(20) NOT NULL ,
"STATISTICS_DATE" NUMBER(20) NOT NULL ,
"LANDING_TIME" NUMBER(20) NOT NULL ,
"UPDATE_TIME" TIMESTAMP(6)  DEFAULT systimestamp  NOT NULL 
)
TABLESPACE ecsdata;

COMMENT ON TABLE "IM"."IM_MERCHANT_LANDING_DETAIL" IS '商家工作量统计报表';
COMMENT ON COLUMN "IM"."IM_MERCHANT_LANDING_DETAIL"."MERCHANT_ID" IS '商家ID';
COMMENT ON COLUMN "IM"."IM_MERCHANT_LANDING_DETAIL"."STATISTICS_DATE" IS '统计的时间，为当天的0点0分0秒';
COMMENT ON COLUMN "IM"."IM_MERCHANT_LANDING_DETAIL"."LANDING_TIME" IS '商家所有子账号登录时长';
COMMENT ON COLUMN "IM"."IM_MERCHANT_LANDING_DETAIL"."UPDATE_TIME" IS '数据更新时间';

-- ----------------------------
-- Indexes structure for table IM_MERCHANT_LANDING_DETAIL
-- ----------------------------
CREATE INDEX "IM"."merchantid"
ON "IM"."IM_MERCHANT_LANDING_DETAIL" ("MERCHANT_ID" ASC)
LOGGING
VISIBLE;

-- ----------------------------
-- Checks structure for table IM_MERCHANT_LANDING_DETAIL
-- ----------------------------
ALTER TABLE "IM"."IM_MERCHANT_LANDING_DETAIL" ADD CHECK ("MERCHANT_ID" IS NOT NULL);
ALTER TABLE "IM"."IM_MERCHANT_LANDING_DETAIL" ADD CHECK ("STATISTICS_DATE" IS NOT NULL);
ALTER TABLE "IM"."IM_MERCHANT_LANDING_DETAIL" ADD CHECK ("LANDING_TIME" IS NOT NULL);
ALTER TABLE "IM"."IM_MERCHANT_LANDING_DETAIL" ADD CHECK ("UPDATE_TIME" IS NOT NULL);
-- ----------------------------
-- Primary Key structure for table IM_MERCHANT_LANDING_DETAIL
-- ----------------------------
ALTER TABLE "IM"."IM_MERCHANT_LANDING_DETAIL" ADD PRIMARY KEY ("MERCHANT_ID", "STATISTICS_DATE");
