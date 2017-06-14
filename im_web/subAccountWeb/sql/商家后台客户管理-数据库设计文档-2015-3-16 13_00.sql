/*
Author: wangchen
Date: 2015-03-16 09:06:37
*/


-- ----------------------------
-- Table structure for D_DIVERSE_TYPE
-- ----------------------------
CREATE TABLE "IM"."D_DIVERSE_TYPE" (
  "D_DIVERSE_TYPE_ID"     NUMBER               NOT NULL,
  "D_DIVERSE_TYPE_NAME"   VARCHAR2(20)         NOT NULL,
  "D_IS_RECOMMEND"        CHAR(1) DEFAULT '0'  NOT NULL,
  "D_DIVERSE_DESCRIPTION" VARCHAR2(50)         NOT NULL
) TABLESPACE ecsdata;

COMMENT ON TABLE "IM"."D_DIVERSE_TYPE"
IS '分流方式配置字典表';
COMMENT ON COLUMN IM.D_DIVERSE_TYPE.D_DIVERSE_TYPE_ID
IS '分流方式 ID[序列生成]';
COMMENT ON COLUMN IM.D_DIVERSE_TYPE.D_DIVERSE_TYPE_NAME
IS '分流方式 名称';
COMMENT ON COLUMN IM.D_DIVERSE_TYPE.D_IS_RECOMMEND
IS '是否为推荐 分流方式';
COMMENT ON COLUMN IM.D_DIVERSE_TYPE.D_DIVERSE_DESCRIPTION
IS '分流方式 描述';

-- ----------------------------
-- Table structure for D_SHOW_GROUP_CONFIG
-- ----------------------------
CREATE TABLE "IM"."D_SHOW_GROUP_CONFIG" (
  "D_CONFIG_ID"   NUMBER       NOT NULL,
  "D_CONFIG_NAME" VARCHAR2(50) NOT NULL
) TABLESPACE ecsdata;
COMMENT ON TABLE IM.D_SHOW_GROUP_CONFIG
IS '客服分组 显示页面 字典表';
COMMENT ON COLUMN IM.D_SHOW_GROUP_CONFIG.D_CONFIG_ID
IS '默认主键';
COMMENT ON COLUMN IM.D_SHOW_GROUP_CONFIG.D_CONFIG_NAME
IS '客服分组 显示页面 名称';

-- ----------------------------
-- Table structure for T_GROUP_SHOW_MAPPING
-- ----------------------------
CREATE TABLE "IM"."T_GROUP_SHOW_MAPPING" (
  "G_MAPPING_ID" NUMBER NOT NULL,
  "G_GROUP_ID"   NUMBER NOT NULL,
  "G_CONFIG_ID"  NUMBER NOT NULL
) TABLESPACE ecsdata;
COMMENT ON TABLE IM.T_GROUP_SHOW_MAPPING
IS '客服分组显示位置 映射表';
COMMENT ON COLUMN IM.T_GROUP_SHOW_MAPPING.G_MAPPING_ID
IS '映射主键';
COMMENT ON COLUMN IM.T_GROUP_SHOW_MAPPING.G_CONFIG_ID
IS '客服分组显示位置 外键';
COMMENT ON COLUMN IM.T_GROUP_SHOW_MAPPING.G_GROUP_ID
IS '客服分组 主键';

-- ----------------------------
-- Table structure for T_MERCHANT_DIVERSE_CONFIG
-- ----------------------------
CREATE TABLE "IM"."T_MERCHANT_DIVERSE_CONFIG" (
  "M_MERCHANT_ID"         NUMBER             NOT NULL,
  "M_DIVERSE_TYPE"        NUMBER             NOT NULL,
  "M_PROXY_SUBACCOUNT_ID" NUMBER DEFAULT -1  NOT NULL
) TABLESPACE ecsdata;
COMMENT ON TABLE IM.T_MERCHANT_DIVERSE_CONFIG
IS '商家 分流信息 配置表';
COMMENT ON COLUMN IM.T_MERCHANT_DIVERSE_CONFIG.M_MERCHANT_ID
IS '商家id';
COMMENT ON COLUMN IM.T_MERCHANT_DIVERSE_CONFIG.M_PROXY_SUBACCOUNT_ID
IS '商家分流 代理账号';
COMMENT ON COLUMN IM.T_MERCHANT_DIVERSE_CONFIG.M_DIVERSE_TYPE
IS '商家 分流类型id';

-- ----------------------------
-- Table structure for T_SERVICE_GROUP
-- ----------------------------
CREATE TABLE "IM"."T_SERVICE_GROUP" (
  "G_GROUP_ID"    NUMBER                   NOT NULL,
  "G_GROUP_NAME"  VARCHAR2(50) DEFAULT ''  NOT NULL,
  "G_MERCHANT_ID" NUMBER                   NOT NULL,
  "G_IS_SHOW"     CHAR(1)                  NOT NULL,
  "G_SHOW_ORDER"  NUMBER                   NOT NULL
) TABLESPACE ecsdata;
COMMENT ON TABLE IM.T_SERVICE_GROUP
IS '客服分组表';
COMMENT ON COLUMN IM.T_SERVICE_GROUP.G_GROUP_ID
IS '客服分组id';
COMMENT ON COLUMN IM.T_SERVICE_GROUP.G_GROUP_NAME
IS '客服分组名称';
COMMENT ON COLUMN IM.T_SERVICE_GROUP.G_MERCHANT_ID
IS '当前客服分组所属商家id';
COMMENT ON COLUMN IM.T_SERVICE_GROUP.G_IS_SHOW
IS '当前客服分组是否在店铺页面显示';
COMMENT ON COLUMN IM.T_SERVICE_GROUP.G_SHOW_ORDER
IS '当前客服分组在店铺页显示顺序[值越大，显示越靠前]';

-- ----------------------------
-- Table structure for T_SERVICE_GROUP_MAPPING
-- ----------------------------
CREATE TABLE "IM"."T_SERVICE_GROUP_MAPPING" (
  "S_MAPPING_ID"    NUMBER             NOT NULL,
  "S_GROUP_ID"      NUMBER             NOT NULL,
  "S_SUBACCOUNT_ID" NUMBER             NOT NULL,
  "S_DIVERSE_STATE" CHAR(1) DEFAULT 0  NOT NULL
) TABLESPACE ecsdata;
COMMENT ON TABLE IM.T_SERVICE_GROUP_MAPPING
IS '子账号与客服分组映射表';
COMMENT ON COLUMN IM.T_SERVICE_GROUP_MAPPING.S_MAPPING_ID
IS '表主键[序列生成]';
COMMENT ON COLUMN IM.T_SERVICE_GROUP_MAPPING.S_GROUP_ID
IS '客服分组id';
COMMENT ON COLUMN IM.T_SERVICE_GROUP_MAPPING.S_SUBACCOUNT_ID
IS '子账号id';
COMMENT ON COLUMN IM.T_SERVICE_GROUP_MAPPING.S_DIVERSE_STATE
IS '当前子账号是否参与分流';

-- ----------------------------
-- Table structure for T_STORE_LIGHT_CONFIG
-- ----------------------------
CREATE TABLE "IM"."T_STORE_LIGHT_CONFIG" (
  "S_MERCHANT_ID"              NUMBER                   NOT NULL,
  "S_FIRST_WORK_DAY_START"     CHAR(1) DEFAULT '1'      NOT NULL,
  "S_FIRST_WORK_DAY_END"       CHAR(1) DEFAULT '1'      NOT NULL,
  "S_FIRST_WORK_TIME_START"    CHAR(5) DEFAULT '00:00'  NOT NULL,
  "S_FIRST_WORK_TIME_END"      CHAR(5) DEFAULT '00:00'  NOT NULL,
  "S_FIRST_IS_WORK_TIME_SHOW"  CHAR(1) DEFAULT '0'      NOT NULL,
  "S_SECOND_WORK_DAY_START"    CHAR(1) DEFAULT '1'      NOT NULL,
  "S_SECOND_WORK_DAY_END"      CHAR(1) DEFAULT '1'      NOT NULL,
  "S_SECOND_WORK_TIME_START"   CHAR(5) DEFAULT '00:00'  NOT NULL,
  "S_SECOND_WORK_TIME_END"     CHAR(5) DEFAULT '00:00'  NOT NULL,
  "S_SECOND_IS_WORK_TIME_SHOW" CHAR(1) DEFAULT '0'      NOT NULL,
  "S_PHONE"                    CHAR(11)                 NOT NULL,
  "S_IS_PHONE_SHOW"            CHAR(1) DEFAULT '0'      NOT NULL,
  "S_TELPHONE"                 CHAR(11)                 NOT NULL,
  "S_IS_TELPHONE_SHOW"         CHAR(1) DEFAULT '0'      NOT NULL
) TABLESPACE ecsdata;
COMMENT ON TABLE IM.T_STORE_LIGHT_CONFIG
IS '店铺亮灯配置表';
COMMENT ON COLUMN IM.T_STORE_LIGHT_CONFIG.S_MERCHANT_ID
IS '当前店铺对应的商家id';
COMMENT ON COLUMN IM.T_STORE_LIGHT_CONFIG.S_FIRST_WORK_DAY_START
IS '一期开始工作时间 星期几';
COMMENT ON COLUMN IM.T_STORE_LIGHT_CONFIG.S_FIRST_WORK_TIME_START
IS '一期开始工作时间 几点钟';
COMMENT ON COLUMN IM.T_STORE_LIGHT_CONFIG.S_FIRST_WORK_DAY_END
IS '一期结束工作时间 星期几';
COMMENT ON COLUMN IM.T_STORE_LIGHT_CONFIG.S_FIRST_WORK_TIME_END
IS '一期结束工作时间 几点钟';
COMMENT ON COLUMN IM.T_STORE_LIGHT_CONFIG.S_FIRST_IS_WORK_TIME_SHOW
IS '是否显示 一期工作时间区间';
COMMENT ON COLUMN IM.T_STORE_LIGHT_CONFIG.S_SECOND_WORK_DAY_START
IS '二期开始工作时间 星期几';
COMMENT ON COLUMN IM.T_STORE_LIGHT_CONFIG.S_SECOND_WORK_TIME_START
IS '二期开始工作时间 几点钟';
COMMENT ON COLUMN IM.T_STORE_LIGHT_CONFIG.S_SECOND_WORK_DAY_END
IS '二期结束工作时间 星期几';
COMMENT ON COLUMN IM.T_STORE_LIGHT_CONFIG.S_SECOND_WORK_TIME_END
IS '二期结束工作时间 几点钟';
COMMENT ON COLUMN IM.T_STORE_LIGHT_CONFIG.S_SECOND_IS_WORK_TIME_SHOW
IS '是否显示 二期工作时间区间';
COMMENT ON COLUMN IM.T_STORE_LIGHT_CONFIG.S_PHONE
IS '固话号码';
COMMENT ON COLUMN IM.T_STORE_LIGHT_CONFIG.S_IS_PHONE_SHOW
IS '是否显示固话号码';
COMMENT ON COLUMN IM.T_STORE_LIGHT_CONFIG.S_TELPHONE
IS '手机号码';
COMMENT ON COLUMN IM.T_STORE_LIGHT_CONFIG.S_IS_TELPHONE_SHOW
IS '是否显示 手机号码';

/**序列定义**/
CREATE SEQUENCE im.seq_diverse_type INCREMENT BY 1 START WITH 1;
---分流类型表 专用序列
CREATE SEQUENCE im.seq_service_group INCREMENT BY 1 START WITH 1;
---客服分组表 专用序列
CREATE SEQUENCE im.seq_service_group_mapping INCREMENT BY 1 START WITH 1;
---客服分组与子账号映射表 专用序列
CREATE SEQUENCE im.seq_show_group_config INCREMENT BY 1 START WITH 1;
---客服分组显示位置配置 专用序列
CREATE SEQUENCE im.seq_group_show_mapping INCREMENT BY 1 START WITH 1;

---客服分组显示页面 专用序列



--  ----------------------------
-- Indexes structure for table D_DIVERSE_TYPE
-- ----------------------------
CREATE UNIQUE INDEX im.UI_PK_DIVERSE_TYPE_ID ON im.d_diverse_type (d_diverse_type_id)
TABLESPACE ecsdata;
-- ----------------------------
-- Checks structure for table D_DIVERSE_TYPE
-- ----------------------------

ALTER TABLE "IM"."D_DIVERSE_TYPE" ADD CHECK ("D_DIVERSE_TYPE_ID" IS NOT NULL);
ALTER TABLE "IM"."D_DIVERSE_TYPE" ADD CHECK ("D_DIVERSE_TYPE_NAME" IS NOT NULL);
ALTER TABLE "IM"."D_DIVERSE_TYPE" ADD CHECK ("D_IS_RECOMMEND" IS NOT NULL);
ALTER TABLE "IM"."D_DIVERSE_TYPE" ADD CHECK ("D_DIVERSE_DESCRIPTION" IS NOT NULL);

-- ----------------------------
-- Primary Key structure for table D_DIVERSE_TYPE
-- ----------------------------
ALTER TABLE "IM"."D_DIVERSE_TYPE" ADD PRIMARY KEY ("D_DIVERSE_TYPE_ID");

-- ----------------------------
-- Indexes structure for table D_SHOW_GROUP_CONFIG
-- ----------------------------
CREATE UNIQUE INDEX im.UI_PK_SHOW_GROUP_CONFIG_ID ON im.D_SHOW_GROUP_CONFIG (D_CONFIG_ID)
TABLESPACE ecsdata;
-- ----------------------------
-- Checks structure for table D_SHOW_GROUP_CONFIG
-- ----------------------------
ALTER TABLE "IM"."D_SHOW_GROUP_CONFIG" ADD CHECK ("D_CONFIG_ID" IS NOT NULL);
ALTER TABLE "IM"."D_SHOW_GROUP_CONFIG" ADD CHECK ("D_CONFIG_NAME" IS NOT NULL);
-- ----------------------------
-- Primary Key structure for table D_SHOW_GROUP_CONFIG
-- ----------------------------
ALTER TABLE "IM"."D_SHOW_GROUP_CONFIG" ADD PRIMARY KEY ("D_CONFIG_ID");

-- ----------------------------
-- Indexes structure for table T_GROUP_SHOW_MAPPING
-- ----------------------------
CREATE UNIQUE INDEX im.UI_PK_GROUP_SHOW_MAPPING_ID ON T_GROUP_SHOW_MAPPING (G_MAPPING_ID)
TABLESPACE ecsdata;

-- ----------------------------
-- Uniques structure for table T_GROUP_SHOW_MAPPING
-- ----------------------------
ALTER TABLE "IM"."T_GROUP_SHOW_MAPPING" ADD UNIQUE ("G_GROUP_ID", "G_CONFIG_ID");
-- ----------------------------
-- Checks structure for table T_GROUP_SHOW_MAPPING
-- ----------------------------
ALTER TABLE "IM"."T_GROUP_SHOW_MAPPING" ADD CHECK ("G_MAPPING_ID" IS NOT NULL);
ALTER TABLE "IM"."T_GROUP_SHOW_MAPPING" ADD CHECK ("G_GROUP_ID" IS NOT NULL);
ALTER TABLE "IM"."T_GROUP_SHOW_MAPPING" ADD CHECK ("G_CONFIG_ID" IS NOT NULL);
-- ----------------------------
-- Primary Key structure for table T_GROUP_SHOW_MAPPING
-- ----------------------------
ALTER TABLE "IM"."T_GROUP_SHOW_MAPPING" ADD PRIMARY KEY ("G_MAPPING_ID");

-- ----------------------------
-- Indexes structure for table T_MERCHANT_DIVERSE_CONFIG
-- ----------------------------
CREATE UNIQUE INDEX im.UI_PK_DIVERSE_CONF_MERCHANT_ID ON T_MERCHANT_DIVERSE_CONFIG (M_MERCHANT_ID)
TABLESPACE ecsdata;

-- ----------------------------
-- Checks structure for table T_MERCHANT_DIVERSE_CONFIG
-- ----------------------------
ALTER TABLE "IM"."T_MERCHANT_DIVERSE_CONFIG" ADD CHECK ("M_MERCHANT_ID" IS NOT NULL);
ALTER TABLE "IM"."T_MERCHANT_DIVERSE_CONFIG" ADD CHECK ("M_DIVERSE_TYPE" IS NOT NULL);
ALTER TABLE "IM"."T_MERCHANT_DIVERSE_CONFIG" ADD CHECK ("M_PROXY_SUBACCOUNT_ID" IS NOT NULL);

-- ----------------------------
-- Primary Key structure for table T_MERCHANT_DIVERSE_CONFIG
-- ----------------------------
ALTER TABLE "IM"."T_MERCHANT_DIVERSE_CONFIG" ADD PRIMARY KEY ("M_MERCHANT_ID");

-- ----------------------------
-- Indexes structure for table T_SERVICE_GROUP
-- ----------------------------
CREATE UNIQUE INDEX im.UI_PK_SERVICE_GROUP_ID ON im.T_SERVICE_GROUP(G_GROUP_ID)
  TABLESPACE ecsdata;
-- ----------------------------
-- Checks structure for table T_SERVICE_GROUP
-- ----------------------------
ALTER TABLE "IM"."T_SERVICE_GROUP" ADD CHECK ("G_GROUP_ID" IS NOT NULL);
ALTER TABLE "IM"."T_SERVICE_GROUP" ADD CHECK ("G_GROUP_NAME" IS NOT NULL);
ALTER TABLE "IM"."T_SERVICE_GROUP" ADD CHECK ("G_MERCHANT_ID" IS NOT NULL);
ALTER TABLE "IM"."T_SERVICE_GROUP" ADD CHECK ("G_IS_SHOW" IS NOT NULL);
ALTER TABLE "IM"."T_SERVICE_GROUP" ADD CHECK ("G_SHOW_ORDER" IS NOT NULL);

-- ----------------------------
-- Primary Key structure for table T_SERVICE_GROUP
-- ----------------------------
ALTER TABLE "IM"."T_SERVICE_GROUP" ADD PRIMARY KEY ("G_GROUP_ID");

-- ----------------------------
-- Indexes structure for table T_SERVICE_GROUP_MAPPING
-- ----------------------------
CREATE UNIQUE INDEX im.UI_PK_GROUP_MAPPING_ID ON im.T_SERVICE_GROUP_MAPPING(S_MAPPING_ID)
  TABLESPACE ecsdata;
-- ----------------------------
-- Uniques structure for table T_SERVICE_GROUP_MAPPING
-- ----------------------------
ALTER TABLE "IM"."T_SERVICE_GROUP_MAPPING" ADD UNIQUE ("S_SUBACCOUNT_ID");
-- ----------------------------
-- Checks structure for table T_SERVICE_GROUP_MAPPING
-- ----------------------------
ALTER TABLE "IM"."T_SERVICE_GROUP_MAPPING" ADD CHECK ("S_MAPPING_ID" IS NOT NULL);
ALTER TABLE "IM"."T_SERVICE_GROUP_MAPPING" ADD CHECK ("S_GROUP_ID" IS NOT NULL);
ALTER TABLE "IM"."T_SERVICE_GROUP_MAPPING" ADD CHECK ("S_SUBACCOUNT_ID" IS NOT NULL);
ALTER TABLE "IM"."T_SERVICE_GROUP_MAPPING" ADD CHECK ("S_DIVERSE_STATE" IS NOT NULL);
-- ----------------------------
-- Primary Key structure for table T_SERVICE_GROUP_MAPPING
-- ----------------------------
ALTER TABLE "IM"."T_SERVICE_GROUP_MAPPING" ADD PRIMARY KEY ("S_MAPPING_ID");

-- ----------------------------
-- Indexes structure for table T_STORE_LIGHT_CONFIG
-- ----------------------------
CREATE UNIQUE INDEX im.UI_PK_LIGHT_CONF_MERCHANT_ID ON im.T_STORE_LIGHT_CONFIG (S_MERCHANT_ID)
TABLESPACE ecsdata;
-- ----------------------------
-- Checks structure for table T_STORE_LIGHT_CONFIG
-- ----------------------------
ALTER TABLE "IM"."T_STORE_LIGHT_CONFIG" ADD CHECK ("S_MERCHANT_ID" IS NOT NULL);
ALTER TABLE "IM"."T_STORE_LIGHT_CONFIG" ADD CHECK ("S_FIRST_WORK_DAY_START" IS NOT NULL);
ALTER TABLE "IM"."T_STORE_LIGHT_CONFIG" ADD CHECK ("S_FIRST_WORK_DAY_END" IS NOT NULL);
ALTER TABLE "IM"."T_STORE_LIGHT_CONFIG" ADD CHECK ("S_FIRST_WORK_TIME_START" IS NOT NULL);
ALTER TABLE "IM"."T_STORE_LIGHT_CONFIG" ADD CHECK ("S_FIRST_WORK_TIME_END" IS NOT NULL);
ALTER TABLE "IM"."T_STORE_LIGHT_CONFIG" ADD CHECK ("S_FIRST_IS_WORK_TIME_SHOW" IS NOT NULL);
ALTER TABLE "IM"."T_STORE_LIGHT_CONFIG" ADD CHECK ("S_SECOND_WORK_DAY_START" IS NOT NULL);
ALTER TABLE "IM"."T_STORE_LIGHT_CONFIG" ADD CHECK ("S_SECOND_WORK_DAY_END" IS NOT NULL);
ALTER TABLE "IM"."T_STORE_LIGHT_CONFIG" ADD CHECK ("S_SECOND_WORK_TIME_START" IS NOT NULL);
ALTER TABLE "IM"."T_STORE_LIGHT_CONFIG" ADD CHECK ("S_SECOND_WORK_TIME_END" IS NOT NULL);
ALTER TABLE "IM"."T_STORE_LIGHT_CONFIG" ADD CHECK ("S_SECOND_IS_WORK_TIME_SHOW" IS NOT NULL);
ALTER TABLE "IM"."T_STORE_LIGHT_CONFIG" ADD CHECK ("S_PHONE" IS NOT NULL);
ALTER TABLE "IM"."T_STORE_LIGHT_CONFIG" ADD CHECK ("S_IS_PHONE_SHOW" IS NOT NULL);
ALTER TABLE "IM"."T_STORE_LIGHT_CONFIG" ADD CHECK ("S_TELPHONE" IS NOT NULL);
ALTER TABLE "IM"."T_STORE_LIGHT_CONFIG" ADD CHECK ("S_IS_TELPHONE_SHOW" IS NOT NULL);

-- ----------------------------
-- Primary Key structure for table T_STORE_LIGHT_CONFIG
-- ----------------------------
ALTER TABLE "IM"."T_STORE_LIGHT_CONFIG" ADD PRIMARY KEY ("S_MERCHANT_ID");

-- ----------------------------
-- Foreign Key structure for table "IM"."T_GROUP_SHOW_MAPPING"
-- ----------------------------
CREATE INDEX IM.UI_FK_GROUP_SHOW_GROUP_ID ON IM.T_GROUP_SHOW_MAPPING(G_GROUP_ID) TABLESPACE ecsdata;
ALTER TABLE "IM"."T_GROUP_SHOW_MAPPING" ADD FOREIGN KEY ("G_CONFIG_ID") REFERENCES "IM"."D_SHOW_GROUP_CONFIG" ("D_CONFIG_ID") ON DELETE CASCADE;
ALTER TABLE "IM"."T_GROUP_SHOW_MAPPING" ADD FOREIGN KEY ("G_GROUP_ID") REFERENCES "IM"."T_SERVICE_GROUP" ("G_GROUP_ID") ON DELETE CASCADE;

-- ----------------------------
-- Foreign Key structure for table "IM"."T_MERCHANT_DIVERSE_CONFIG"
-- ----------------------------
CREATE INDEX im.UI_FK_DIVERSE_CONF_TYPE_ID ON im.T_MERCHANT_DIVERSE_CONFIG(M_DIVERSE_TYPE) TABLESPACE ecsdata;
ALTER TABLE "IM"."T_MERCHANT_DIVERSE_CONFIG" ADD FOREIGN KEY ("M_DIVERSE_TYPE") REFERENCES "IM"."D_DIVERSE_TYPE" ("D_DIVERSE_TYPE_ID") ON DELETE CASCADE;

-- ----------------------------
-- Foreign Key structure for table "IM"."T_SERVICE_GROUP_MAPPING"
-- ----------------------------
CREATE INDEX im.UI_FK_GROUP_MAPPING_GROUP_ID ON im.T_SERVICE_GROUP_MAPPING(S_GROUP_ID) TABLESPACE ecsdata;
ALTER TABLE "IM"."T_SERVICE_GROUP_MAPPING" ADD FOREIGN KEY ("S_GROUP_ID") REFERENCES "IM"."T_SERVICE_GROUP" ("G_GROUP_ID") ON DELETE CASCADE;

/**字典表 初始值插入**/
INSERT INTO im.d_diverse_type (d_diverse_type_id, d_diverse_type_name, d_is_recommend, d_diverse_description)
VALUES (seq_diverse_type.nextval, '平均分流', 0, '单位时间内所有账号接待量相同');

INSERT INTO im.d_show_group_config (d_config_id, d_config_name)
VALUES (seq_show_group_config.nextval, '商家店铺名称亮灯');
INSERT INTO im.d_show_group_config (d_config_id, d_config_name)
VALUES (seq_show_group_config.nextval, '购物车结算页');
INSERT INTO im.d_show_group_config (d_config_id, d_config_name)
VALUES (seq_show_group_config.nextval, '已购买商品（未发货）');
INSERT INTO im.d_show_group_config (d_config_id, d_config_name)
VALUES (seq_show_group_config.nextval, '已购买商品（已发货）');
INSERT INTO im.d_show_group_config (d_config_id, d_config_name)
VALUES (seq_show_group_config.nextval, '其他');


ALTER TABLE im.d_diverse_type MODIFY d_diverse_type_name varchar(50);

INSERT INTO im.d_diverse_type (d_diverse_type_id, d_diverse_type_name, d_is_recommend, d_diverse_description)
VALUES (3, '接待能力分流', 1, '系统会自动根据客服接待能力调整分流策略。即接待能力越强的客服会被分流到更多的买家。');

DROP TABLE "IM"."T_GUIDE_CONFIG";
CREATE TABLE "IM"."T_GUIDE_CONFIG" (
  "D_MERCHANT_ID" NUMBER NOT NULL ,
  "S_VERSION" VARCHAR2(20 BYTE) NOT NULL ,
  "B_HAS_GUIDED" CHAR(1 BYTE) DEFAULT 'N'  NOT NULL
) TABLESPACE ecsdata;

COMMENT ON TABLE "IM"."T_GUIDE_CONFIG"
IS '商家后台 新特性功能 是否引导表';
COMMENT ON COLUMN IM.T_GUIDE_CONFIG.D_MERCHANT_ID
IS '商家id';
COMMENT ON COLUMN IM.T_GUIDE_CONFIG.S_VERSION
IS '当前新特性引导说明的版本号';
COMMENT ON COLUMN IM.T_GUIDE_CONFIG.B_HAS_GUIDED
IS '该商家是否已经被引导过当前版本的新特性';

-- ----------------------------
-- Indexes structure for table T_GUIDE_CONFIG
-- ----------------------------

-- ----------------------------
-- Checks structure for table T_GUIDE_CONFIG
-- ----------------------------
ALTER TABLE "IM"."T_GUIDE_CONFIG" ADD CHECK ("D_MERCHANT_ID" IS NOT NULL);
ALTER TABLE "IM"."T_GUIDE_CONFIG" ADD CHECK ("S_VERSION" IS NOT NULL);
ALTER TABLE "IM"."T_GUIDE_CONFIG" ADD CHECK ("B_HAS_GUIDED" IS NOT NULL);

-- ----------------------------
-- Primary Key structure for table T_GUIDE_CONFIG
-- ----------------------------
ALTER TABLE "IM"."T_GUIDE_CONFIG" ADD PRIMARY KEY ("D_MERCHANT_ID", "S_VERSION");



