prompt
prompt Creating table IM_CSGROUP
prompt =========================
prompt
create table IM.IM_CSGROUP
(
  groupid        NUMBER(10) not null,
  groupname      VARCHAR2(100),
  groupdescrib   VARCHAR2(400),
  lastmodifyname VARCHAR2(80),
  lastmodifytime NUMBER(20),
  insert_time    DATE default sysdate,
  update_time    TIMESTAMP(6) default systimestamp
)
tablespace ECSDATA
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 8K
    minextents 1
    maxextents unlimited
  );
comment on column IM.IM_CSGROUP.groupid
  is '客服组id';
comment on column IM.IM_CSGROUP.groupname
  is '客服组名称';
comment on column IM.IM_CSGROUP.groupdescrib
  is '组描述';
comment on column IM.IM_CSGROUP.lastmodifyname
  is '最后修改人名称';
comment on column IM.IM_CSGROUP.lastmodifytime
  is '最后修改时间';
comment on column IM.IM_CSGROUP.insert_time
  is '插入时间';
comment on column IM.IM_CSGROUP.update_time
  is '更新时间';

prompt
prompt Creating table IM_CSGROUPMEM
prompt ============================
prompt
create table IM.IM_CSGROUPMEM
(
  userid      NUMBER(20) not null,
  groupid     NUMBER(20),
  flowstatus  NUMBER(20),
  insert_time DATE default sysdate,
  update_time TIMESTAMP(6) default systimestamp
)
tablespace ECSDATA
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 16K
    next 8K
    minextents 1
    maxextents unlimited
  );
comment on column IM.IM_CSGROUPMEM.userid
  is '客服id';
comment on column IM.IM_CSGROUPMEM.groupid
  is '客服组id';
comment on column IM.IM_CSGROUPMEM.flowstatus
  is '客服接待状态';
comment on column IM.IM_CSGROUPMEM.insert_time
  is '客服插入时间';
comment on column IM.IM_CSGROUPMEM.update_time
  is '客服更新时间';

prompt
prompt Creating table IM_CSINFO
prompt ========================
prompt
create table IM.IM_CSINFO
(
  userid         NUMBER(20) not null,
  csid           NUMBER(20),
  username       VARCHAR2(80),
  permission     NUMBER(20),
  importname     VARCHAR2(80),
  importtime     NUMBER(20),
  receptionlimit NUMBER(20),
  realname       VARCHAR2(80),
  status         NUMBER(20),
  groupname      VARCHAR2(80),
  insert_time    DATE default sysdate,
  update_time    TIMESTAMP(6) default systimestamp,
  importcsid     NUMBER(20)
)
tablespace ECSDATA
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initial 64K
    next 8K
    minextents 1
    maxextents unlimited
  );
comment on column IM.IM_CSINFO.userid
  is '客服id';
comment on column IM.IM_CSINFO.csid
  is '客服csid';
comment on column IM.IM_CSINFO.username
  is '客服用户名';
comment on column IM.IM_CSINFO.permission
  is '客服权限';
comment on column IM.IM_CSINFO.importname
  is '客服导入人员名称';
comment on column IM.IM_CSINFO.importtime
  is '客服导入时间';
comment on column IM.IM_CSINFO.receptionlimit
  is '客服接待上限';
comment on column IM.IM_CSINFO.realname
  is '客服真实名称';
comment on column IM.IM_CSINFO.status
  is '客服状态';
comment on column IM.IM_CSINFO.groupname
  is '客服所在组名称';
comment on column IM.IM_CSINFO.insert_time
  is '插入时间';
comment on column IM.IM_CSINFO.update_time
  is '更新时间';


prompt
prompt Creating sequence IM_CSGROUP_SEQ_GROUPID
prompt ========================================
prompt
create sequence IM.IM_CSGROUP_SEQ_GROUPID
minvalue 1
maxvalue 1000000000
start with 184
increment by 1
cache 20
order;

prompt
prompt Creating sequence IM_CS_USER_SEQ_USERID
prompt =======================================
prompt
create sequence IM.IM_CS_USER_SEQ_USERID
minvalue 2000000000
maxvalue 2100000000
start with 2000000110
increment by 1
cache 20
order;



-- ----------------------------
-- Table structure for T_AFTER_SALE_BOARD_CONFIG
-- ----------------------------
CREATE TABLE "IM"."T_AFTER_SALE_BOARD_CONFIG" (
  "I_BOARD_ID"           NUMBER(10)                         NOT NULL,
  "V_TITLE"              VARCHAR2(24 BYTE)                  NULL,
  "V_DESCRIPTION_LIST"   VARCHAR2(250 BYTE)                 NULL,
  "C_IS_BIND_GROUP"      CHAR(1 BYTE)                       NOT NULL,
  "I_GROUP_ID"           NUMBER(10)                         NULL,
  "C_IS_BIND_USER_ORDER" CHAR(1 BYTE)                       NOT NULL,
  "C_DATA_TYPE"          CHAR(1 BYTE)                       NOT NULL,
  "INSERT_TIME"          DATE DEFAULT sysdate               NULL,
  "UPDATE_TIME"          TIMESTAMP(6) DEFAULT systimestamp  NULL
)
TABLESPACE ecsdata;
-- ----------------------------
-- Indexes structure for table T_AFTER_SALE_BOARD_CONFIG
-- ----------------------------

-- ----------------------------
-- Checks structure for table T_AFTER_SALE_BOARD_CONFIG
-- ----------------------------
ALTER TABLE "IM"."T_AFTER_SALE_BOARD_CONFIG" ADD CHECK ("I_BOARD_ID" IS NOT NULL);
ALTER TABLE "IM"."T_AFTER_SALE_BOARD_CONFIG" ADD CHECK ("C_IS_BIND_GROUP" IS NOT NULL);
ALTER TABLE "IM"."T_AFTER_SALE_BOARD_CONFIG" ADD CHECK ("C_IS_BIND_USER_ORDER" IS NOT NULL);
ALTER TABLE "IM"."T_AFTER_SALE_BOARD_CONFIG" ADD CHECK ("C_DATA_TYPE" IS NOT NULL);
ALTER TABLE "IM"."T_AFTER_SALE_BOARD_CONFIG" ADD CHECK (c_data_type IN ('0', '1'));
ALTER TABLE "IM"."T_AFTER_SALE_BOARD_CONFIG" ADD CHECK (c_is_bind_group IN ('0', '1'));
ALTER TABLE "IM"."T_AFTER_SALE_BOARD_CONFIG" ADD CHECK (c_is_bind_user_order IN ('0', '1'));

-- ----------------------------
-- Primary Key structure for table T_AFTER_SALE_BOARD_CONFIG
-- ----------------------------
ALTER TABLE "IM"."T_AFTER_SALE_BOARD_CONFIG" ADD PRIMARY KEY ("I_BOARD_ID");










-- ----------------------------
-- Table structure for T_FAQ
-- ----------------------------
CREATE TABLE "IM"."T_FAQ" (
  "I_FAQ_ID"        NUMBER(10)                    NOT NULL,
  "C_IS_SHOW"       CHAR(1 BYTE) DEFAULT '0'      NOT NULL,
  "V_TITLE"         VARCHAR2(90 BYTE)             NOT NULL,
  "V_SUMMARY"       VARCHAR2(600 BYTE)            NOT NULL,
  "V_LAST_OPERATOR" VARCHAR2(100 BYTE)            NOT NULL,
  "I_ORDER"         NUMBER(4) DEFAULT -1          NOT NULL,
  "INSERT_TIME"     DATE DEFAULT sysdate          NOT NULL,
  "UPDATE_TIME"     TIMESTAMP(6) DEFAULT sysdate  NOT NULL
)
TABLESPACE ecsdata;


-- ----------------------------
-- Indexes structure for table T_FAQ
-- ----------------------------
CREATE INDEX "IM"."I_FAQ_SHOW_INDEX"
ON "IM"."T_FAQ" ("C_IS_SHOW" ASC)
TABLESPACE ecsdata;

-- ----------------------------
-- Checks structure for table T_FAQ
-- ----------------------------
ALTER TABLE "IM"."T_FAQ" ADD CHECK ("I_FAQ_ID" IS NOT NULL);
ALTER TABLE "IM"."T_FAQ" ADD CHECK ("C_IS_SHOW" IS NOT NULL);
ALTER TABLE "IM"."T_FAQ" ADD CHECK ("V_TITLE" IS NOT NULL);
ALTER TABLE "IM"."T_FAQ" ADD CHECK ("V_SUMMARY" IS NOT NULL);
ALTER TABLE "IM"."T_FAQ" ADD CHECK ("V_LAST_OPERATOR" IS NOT NULL);
ALTER TABLE "IM"."T_FAQ" ADD CHECK ("I_ORDER" IS NOT NULL);
ALTER TABLE "IM"."T_FAQ" ADD CHECK ("INSERT_TIME" IS NOT NULL);
ALTER TABLE "IM"."T_FAQ" ADD CHECK ("UPDATE_TIME" IS NOT NULL);
ALTER TABLE "IM"."T_FAQ" ADD CHECK (c_is_show IN ('0', '1'));

-- ----------------------------
-- Primary Key structure for table T_FAQ
-- ----------------------------
ALTER TABLE "IM"."T_FAQ" ADD PRIMARY KEY ("I_FAQ_ID");


-- ----------------------------
-- Table structure for T_FAQ_BROWSE_INFO
-- ----------------------------
CREATE TABLE "IM"."T_FAQ_BROWSE_INFO" (
  "I_FAQ_ID"     NUMBER(10)                    NOT NULL,
  "I_VIEW_DATE"  NUMBER(20)                    NOT NULL,
  "I_VIEW_COUNT" NUMBER(20)                    NOT NULL,
  "INSERT_TIME"  DATE DEFAULT sysdate          NOT NULL,
  "UPDATE_TIME"  TIMESTAMP(7) DEFAULT sysdate  NOT NULL
)
TABLESPACE ecsdata;

-- ----------------------------
-- Indexes structure for table T_FAQ_BROWSE_INFO
-- ----------------------------

-- ----------------------------
-- Checks structure for table T_FAQ_BROWSE_INFO
-- ----------------------------
ALTER TABLE "IM"."T_FAQ_BROWSE_INFO" ADD CHECK ("I_FAQ_ID" IS NOT NULL);
ALTER TABLE "IM"."T_FAQ_BROWSE_INFO" ADD CHECK ("I_VIEW_DATE" IS NOT NULL);
ALTER TABLE "IM"."T_FAQ_BROWSE_INFO" ADD CHECK ("I_VIEW_COUNT" IS NOT NULL);
ALTER TABLE "IM"."T_FAQ_BROWSE_INFO" ADD CHECK ("INSERT_TIME" IS NOT NULL);
ALTER TABLE "IM"."T_FAQ_BROWSE_INFO" ADD CHECK ("UPDATE_TIME" IS NOT NULL);

-- ----------------------------
-- Primary Key structure for table T_FAQ_BROWSE_INFO
-- ----------------------------
ALTER TABLE "IM"."T_FAQ_BROWSE_INFO" ADD PRIMARY KEY ("I_FAQ_ID", "I_VIEW_DATE");













-- ----------------------------
-- Table structure for T_FAQ_CATEGORY
-- ----------------------------
CREATE TABLE "IM"."T_FAQ_CATEGORY" (
  "I_ID"            NUMBER(10)                    NOT NULL,
  "C_IS_SHOW"       CHAR(1 BYTE) DEFAULT '0'      NOT NULL,
  "V_TITLE"         VARCHAR2(90 BYTE)             NOT NULL,
  "V_SUMMARY"       VARCHAR2(150 BYTE),
  "V_LAST_OPERATOR" VARCHAR2(100 BYTE)            NOT NULL,
  "INSERT_TIME"     DATE DEFAULT sysdate          NOT NULL,
  "UPDATE_TIME"     TIMESTAMP(6) DEFAULT sysdate  NOT NULL
)
TABLESPACE ecsdata;


-- ----------------------------
-- Indexes structure for table T_FAQ_CATEGORY
-- ----------------------------
CREATE INDEX "IM"."I_FAQ_CATEGORY_SHOW_INDEX"
ON "IM"."T_FAQ_CATEGORY" ("C_IS_SHOW" ASC)
TABLESPACE ecsdata;

-- ----------------------------
-- Checks structure for table T_FAQ_CATEGORY
-- ----------------------------
ALTER TABLE "IM"."T_FAQ_CATEGORY" ADD CHECK ("I_ID" IS NOT NULL);
ALTER TABLE "IM"."T_FAQ_CATEGORY" ADD CHECK ("C_IS_SHOW" IS NOT NULL);
ALTER TABLE "IM"."T_FAQ_CATEGORY" ADD CHECK ("V_TITLE" IS NOT NULL);
ALTER TABLE "IM"."T_FAQ_CATEGORY" ADD CHECK ("V_SUMMARY" IS NOT NULL);
ALTER TABLE "IM"."T_FAQ_CATEGORY" ADD CHECK ("V_LAST_OPERATOR" IS NOT NULL);
ALTER TABLE "IM"."T_FAQ_CATEGORY" ADD CHECK ("INSERT_TIME" IS NOT NULL);
ALTER TABLE "IM"."T_FAQ_CATEGORY" ADD CHECK ("UPDATE_TIME" IS NOT NULL);
ALTER TABLE "IM"."T_FAQ_CATEGORY" ADD CHECK (c_is_show IN ('0', '1'));

-- ----------------------------
-- Primary Key structure for table T_FAQ_CATEGORY
-- ----------------------------
ALTER TABLE "IM"."T_FAQ_CATEGORY" ADD PRIMARY KEY ("I_ID");

























-- ----------------------------
-- Table structure for T_FAQ_CATEGORY_CONSULT_INFO
-- ----------------------------
CREATE TABLE "IM"."T_FAQ_CATEGORY_CONSULT_INFO" (
  "I_FAQ_CATEGORY_ID" NUMBER(10)                    NOT NULL,
  "I_CONSULT_DATE"    NUMBER(20)                    NOT NULL,
  "I_CONSULT_COUNT"   NUMBER(20)                    NOT NULL,
  "INSERT_TIME"       DATE DEFAULT sysdate          NOT NULL,
  "UPDATE_TIME"       TIMESTAMP(7) DEFAULT sysdate  NOT NULL
)
TABLESPACE ecsdata;


-- ----------------------------
-- Indexes structure for table T_FAQ_CATEGORY_CONSULT_INFO
-- ----------------------------
CREATE INDEX "IM"."I_FAQ_CATEGORY_CONSULT_INDEX"
ON "IM"."T_FAQ_CATEGORY_CONSULT_INFO" ("I_FAQ_CATEGORY_ID" ASC, "I_CONSULT_COUNT" ASC)
TABLESPACE ecsdata;

-- ----------------------------
-- Checks structure for table T_FAQ_CATEGORY_CONSULT_INFO
-- ----------------------------
ALTER TABLE "IM"."T_FAQ_CATEGORY_CONSULT_INFO" ADD CHECK ("I_FAQ_CATEGORY_ID" IS NOT NULL);
ALTER TABLE "IM"."T_FAQ_CATEGORY_CONSULT_INFO" ADD CHECK ("I_CONSULT_DATE" IS NOT NULL);
ALTER TABLE "IM"."T_FAQ_CATEGORY_CONSULT_INFO" ADD CHECK ("I_CONSULT_COUNT" IS NOT NULL);
ALTER TABLE "IM"."T_FAQ_CATEGORY_CONSULT_INFO" ADD CHECK ("INSERT_TIME" IS NOT NULL);
ALTER TABLE "IM"."T_FAQ_CATEGORY_CONSULT_INFO" ADD CHECK ("UPDATE_TIME" IS NOT NULL);

-- ----------------------------
-- Primary Key structure for table T_FAQ_CATEGORY_CONSULT_INFO
-- ----------------------------
ALTER TABLE "IM"."T_FAQ_CATEGORY_CONSULT_INFO" ADD PRIMARY KEY ("I_FAQ_CATEGORY_ID", "I_CONSULT_DATE");



















-- ----------------------------
-- Table structure for T_KEFU_INDEX_CONFIG
-- ----------------------------
CREATE TABLE "IM"."T_KEFU_INDEX_CONFIG" (
  "I_CONFIG_ID"                  NUMBER(10)                         NOT NULL,
  "V_NOTICE"                     VARCHAR2(300 BYTE)                 NULL,
  "V_AD_BANNER_LEFT_IMAGE_URL"   VARCHAR2(200 BYTE)                 NULL,
  "V_AD_BANNER_LEFT_IMAGE_HREF"  VARCHAR2(200 BYTE)                 NULL,
  "V_AD_BANNER_RIGHT_IMAGE_URL"  VARCHAR2(200 BYTE)                 NULL,
  "V_AD_BANNER_RIGHT_IMAGE_HREF" VARCHAR2(200 BYTE)                 NULL,
  "V_JSON_OF_FEINIU_INDEX"       CLOB DEFAULT '{}'                  NULL,
  "C_DATA_TYPE"                  CHAR(1 BYTE)                       NOT NULL,
  "INSERT_TIME"                  DATE DEFAULT sysdate               NULL,
  "UPDATE_TIME"                  TIMESTAMP(6) DEFAULT systimestamp  NULL
)
TABLESPACE ecsdata;
-- ----------------------------
-- Indexes structure for table T_KEFU_INDEX_CONFIG
-- ----------------------------

-- ----------------------------
-- Checks structure for table T_KEFU_INDEX_CONFIG
-- ----------------------------
ALTER TABLE "IM"."T_KEFU_INDEX_CONFIG" ADD CHECK ("I_CONFIG_ID" IS NOT NULL);
ALTER TABLE "IM"."T_KEFU_INDEX_CONFIG" ADD CHECK ("C_DATA_TYPE" IS NOT NULL);
ALTER TABLE "IM"."T_KEFU_INDEX_CONFIG" ADD CHECK (c_data_type IN ('0', '1'));

-- ----------------------------
-- Primary Key structure for table T_KEFU_INDEX_CONFIG
-- ----------------------------
ALTER TABLE "IM"."T_KEFU_INDEX_CONFIG" ADD PRIMARY KEY ("I_CONFIG_ID");

-- ----------------------------
-- Foreign Key structure for table "IM"."T_FAQ_BROWSE_INFO"
-- ----------------------------
ALTER TABLE "IM"."T_FAQ_BROWSE_INFO" ADD FOREIGN KEY ("I_FAQ_ID") REFERENCES "IM"."T_FAQ" ("I_FAQ_ID") ON DELETE CASCADE;

-- ----------------------------
-- Foreign Key structure for table "IM"."T_FAQ_CATEGORY_CONSULT_INFO"
-- ----------------------------
ALTER TABLE "IM"."T_FAQ_CATEGORY_CONSULT_INFO" ADD FOREIGN KEY ("I_FAQ_CATEGORY_ID") REFERENCES "IM"."T_FAQ_CATEGORY" ("I_ID") ON DELETE CASCADE;
























-- ----------------------------
-- Table structure for T_QUICK_PHRASE
-- ----------------------------
CREATE TABLE "IM"."T_QUICK_PHRASE" (
"I_PHRASE_ID" NUMBER(10) NOT NULL ,
"V_PHRASE_SUMMARY" VARCHAR2(1315 BYTE) NOT NULL ,
"V_PHRASE_GROUP" VARCHAR2(40) NOT NULL ,
"V_QUICK_CODE" VARCHAR2(20 BYTE) NULL ,
"V_LAST_OPERATOR" VARCHAR2(100 BYTE) NOT NULL ,
"I_FONT_SIZE" NUMBER(3) DEFAULT 12  NULL ,
"I_FONT_COLOR" NUMBER(20) DEFAULT 0  NULL ,
"I_FONT_STYLE" NUMBER(10) DEFAULT 0  NULL ,
"V_FONT_NAME" VARCHAR2(80 BYTE) DEFAULT 'Microsoft YaHei'  NULL ,
"INSERT_TIME" DATE DEFAULT sysdate  NULL ,
"UPDATE_TIME" TIMESTAMP(6)  DEFAULT systimestamp  NULL 
)
TABLESPACE ecsdata;



-- ----------------------------
-- Indexes structure for table T_QUICK_PHRASE
-- ----------------------------
CREATE UNIQUE INDEX "IM"."T_QUICK_PHRASE_INDEX"
ON "IM"."T_QUICK_PHRASE" ("I_PHRASE_ID" DESC, "V_PHRASE_GROUP" DESC)
TABLESPACE ecsdata;

-- ----------------------------
-- Checks structure for table T_QUICK_PHRASE
-- ----------------------------
ALTER TABLE "IM"."T_QUICK_PHRASE" ADD CHECK ("I_PHRASE_ID" IS NOT NULL);
ALTER TABLE "IM"."T_QUICK_PHRASE" ADD CHECK ("V_PHRASE_SUMMARY" IS NOT NULL);
ALTER TABLE "IM"."T_QUICK_PHRASE" ADD CHECK ("V_PHRASE_GROUP" IS NOT NULL);
ALTER TABLE "IM"."T_QUICK_PHRASE" ADD CHECK ("V_LAST_OPERATOR" IS NOT NULL);

-- ----------------------------
-- Primary Key structure for table T_QUICK_PHRASE
-- ----------------------------
ALTER TABLE "IM"."T_QUICK_PHRASE" ADD PRIMARY KEY ("I_PHRASE_ID");































-- ----------------------------
-- Table structure for IM_SERVICE_LOG
-- ----------------------------
CREATE TABLE "IM"."IM_SERVICE_LOG" (
  "SAVE_TIME"     NUMBER(20)                         NOT NULL,
  "CUSTOMER_UUID" NVARCHAR2(60)                      NOT NULL,
  "SERVICE_NAME"  VARCHAR2(40 BYTE)                  NOT NULL,
  "LOG_DETAIL"    VARCHAR2(4000 BYTE)                NOT NULL,
  "UPDATE_TIME"   TIMESTAMP(6) DEFAULT systimestamp  NOT NULL
)
TABLESPACE ecsdata;

-- ----------------------------
-- Indexes structure for table IM_SERVICE_LOG
-- ----------------------------
CREATE UNIQUE INDEX "IM"."IM_SERVICE_LOG_TIME"
ON "IM"."IM_SERVICE_LOG" ("SAVE_TIME" DESC, "CUSTOMER_UUID" DESC)
LOGGING
VISIBLE;

-- ----------------------------
-- Uniques structure for table IM_SERVICE_LOG
-- ----------------------------
ALTER TABLE "IM"."IM_SERVICE_LOG" ADD UNIQUE ("SAVE_TIME", "CUSTOMER_UUID", "SERVICE_NAME");

-- ----------------------------
-- Checks structure for table IM_SERVICE_LOG
-- ----------------------------
ALTER TABLE "IM"."IM_SERVICE_LOG" ADD CHECK ("SAVE_TIME" IS NOT NULL);
ALTER TABLE "IM"."IM_SERVICE_LOG" ADD CHECK ("CUSTOMER_UUID" IS NOT NULL);
ALTER TABLE "IM"."IM_SERVICE_LOG" ADD CHECK ("SERVICE_NAME" IS NOT NULL);
ALTER TABLE "IM"."IM_SERVICE_LOG" ADD CHECK ("LOG_DETAIL" IS NOT NULL);
ALTER TABLE "IM"."IM_SERVICE_LOG" ADD CHECK ("UPDATE_TIME" IS NOT NULL);




























-- ----------------------------
-- add clum for table IM_WEBRECOMMENT
-- ----------------------------
ALTER TABLE IM_WEBRECOMMENT ADD (V_RECOMMENT_TEXT VARCHAR2(1315));
ALTER TABLE IM_WEBRECOMMENT ADD (I_SESSION_ID NUMBER(20));




CREATE SEQUENCE seq_faq MINVALUE 1 MAXVALUE 1000000 INCREMENT BY 1 NOCYCLE;
CREATE SEQUENCE seq_faq_category MINVALUE 1 MAXVALUE 1000000 INCREMENT BY 1 NOCYCLE;
CREATE SEQUENCE seq_after_sale_board_config MINVALUE 1 MAXVALUE 1000000 INCREMENT BY 1 NOCYCLE;


CREATE SEQUENCE seq_quick_phrase MINVALUE 1 MAXVALUE 1000000 INCREMENT BY 1 NOCYCLE;



-- ----------------------------
---初始化插入数据
-- ----------------------------

-- ----------------------------
-- T_FAQ_CATEGORY
-- ----------------------------
INSERT INTO "T_FAQ_CATEGORY" VALUES (1, '1', '售前资料', '咨询售前相关问题', 'admin', sysdate, systimestamp);
INSERT INTO "T_FAQ_CATEGORY" VALUES (2, '1', '订购问题', '咨询订购相关问题忘记密码怎么.', 'admin', sysdate, systimestamp);
INSERT INTO "T_FAQ_CATEGORY" VALUES (3, '1', '付款问题', '咨询付款相关问题 ', 'admin', sysdate, systimestamp);
INSERT INTO "T_FAQ_CATEGORY" VALUES (4, '1', '出货问题', '咨询相关出货问题', 'admin', sysdate, systimestamp);
INSERT INTO "T_FAQ_CATEGORY" VALUES (5, '1', '更改资料', '客人需更改相关资料', 'admin', sysdate, systimestamp);
INSERT INTO "T_FAQ_CATEGORY" VALUES (6, '1', '物流问题', '客人咨询订单物流信息  ', 'admin', sysdate, systimestamp);
INSERT INTO "T_FAQ_CATEGORY" VALUES (7, '1', '系统BUG', '客人反馈问题属于系统BUG', 'admin', sysdate, systimestamp);
INSERT INTO "T_FAQ_CATEGORY" VALUES (8, '1', '商品问题', '客人反馈商品存在质量等问题', 'admin', sysdate, systimestamp);
INSERT INTO "T_FAQ_CATEGORY" VALUES (9, '1', '发票问题', '客人咨询发票开具相关问题', 'admin', sysdate, systimestamp);
INSERT INTO "T_FAQ_CATEGORY" VALUES (10, '1', '退货问题', '客人咨询退货', 'admin', sysdate, systimestamp);
INSERT INTO "T_FAQ_CATEGORY" VALUES (11, '1', '优惠劵问题', '客人咨询优惠劵相关问题', 'admin', sysdate, systimestamp);
INSERT INTO "T_FAQ_CATEGORY" VALUES (12, '1', '法务案件', '客人投诉涉及欺诈等问题', 'admin', sysdate, systimestamp);
INSERT INTO "T_FAQ_CATEGORY" VALUES (13, '1', '其它', '其他问题', 'admin', sysdate, systimestamp);
INSERT INTO "T_FAQ_CATEGORY" VALUES (14, '1', '忘记密码怎么办？', '您可以登录会员安全中心自肋找回密码。', 'admin', sysdate, systimestamp);

-- ----------------------------
-- T_KEFU_INDEX_CONFIG
-- ----------------------------
INSERT INTO t_kefu_index_config (i_config_id, v_notice, v_ad_banner_left_image_url, v_ad_banner_left_image_href, v_ad_banner_right_image_url, v_ad_banner_right_image_href, c_data_type, insert_time, update_time)
VALUES (1, '欢迎使用飞牛哞哞在线客服！<br/>如果您对商品规格、详情、使用有疑问', '', '', '', '', '0', sysdate, systimestamp);
INSERT INTO t_kefu_index_config (i_config_id, v_notice, v_ad_banner_left_image_url, v_ad_banner_left_image_href, v_ad_banner_right_image_url, v_ad_banner_right_image_href, c_data_type, insert_time, update_time)
VALUES (2, '欢迎使用飞牛哞哞在线客服！<br/>如果您对商品规格、详情、使用有疑问', '', '', '', '', '1', sysdate, systimestamp);

-- ----------------------------
-- T_AFTER_SALE_BOARD_CONFIG
-- ----------------------------
INSERT INTO t_after_sale_board_config (i_board_id, v_title,  v_description_list, c_is_bind_group, i_group_id, c_is_bind_user_order, c_data_type, insert_time, update_time)
VALUES (seq_after_sale_board_config.nextval, '订单服务', '',  '0', -1, '0', '0', sysdate, systimestamp);
INSERT INTO t_after_sale_board_config (i_board_id, v_title,  v_description_list, c_is_bind_group, i_group_id, c_is_bind_user_order, c_data_type, insert_time, update_time)
VALUES (seq_after_sale_board_config.nextval, '售后服务', '',  '0', -1, '0', '0', sysdate, systimestamp);
INSERT INTO t_after_sale_board_config (i_board_id, v_title,  v_description_list, c_is_bind_group, i_group_id, c_is_bind_user_order, c_data_type, insert_time, update_time)
VALUES (seq_after_sale_board_config.nextval, '账户服务', '',  '0', -1, '0', '0', sysdate, systimestamp);
INSERT INTO t_after_sale_board_config (i_board_id, v_title,  v_description_list, c_is_bind_group, i_group_id, c_is_bind_user_order, c_data_type, insert_time, update_time)
VALUES (seq_after_sale_board_config.nextval, '意见反馈', '',  '0', -1, '0', '0', sysdate, systimestamp);

INSERT INTO t_after_sale_board_config (i_board_id, v_title,  v_description_list, c_is_bind_group, i_group_id, c_is_bind_user_order, c_data_type, insert_time, update_time)
VALUES (seq_after_sale_board_config.nextval, '订单服务', '',  '0', -1, '0', '1', sysdate, systimestamp);
INSERT INTO t_after_sale_board_config (i_board_id, v_title,  v_description_list, c_is_bind_group, i_group_id, c_is_bind_user_order, c_data_type, insert_time, update_time)
VALUES (seq_after_sale_board_config.nextval, '售后服务', '',  '0', -1, '0', '1', sysdate, systimestamp);
INSERT INTO t_after_sale_board_config (i_board_id, v_title,  v_description_list, c_is_bind_group, i_group_id, c_is_bind_user_order, c_data_type, insert_time, update_time)
VALUES (seq_after_sale_board_config.nextval, '账户服务', '',  '0', -1, '0', '1', sysdate, systimestamp);
INSERT INTO t_after_sale_board_config (i_board_id, v_title,  v_description_list, c_is_bind_group, i_group_id, c_is_bind_user_order, c_data_type, insert_time, update_time)
VALUES (seq_after_sale_board_config.nextval, '意见反馈', '',  '0', -1, '0', '1', sysdate, systimestamp);
