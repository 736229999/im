prompt
prompt Creating table IM_MSGNOTIFY
prompt =========================
prompt
create table IM.IM_MSGNOTIFY
(
	UUID 					VARCHAR2(80) PRIMARY KEY, 
	INSERTTIME 		TIMESTAMP DEFAULT SYSDATE, 
	BUF 					BLOB, 
	BUFFTYPE 			NUMBER(2) NOT NULL
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
comment on column IM.IM_MSGNOTIFY.UUID
  is '推送消息唯一标识';
comment on column IM.IM_MSGNOTIFY.INSERTTIME
  is '推送消息写入时间';
comment on column IM.IM_MSGNOTIFY.BUF
  is '推送消息内容';
comment on column IM.IM_MSGNOTIFY.BUFFTYPE
  is '推送消息类型';
  
prompt
prompt Creating table IM_USER_WITH_MSGNOTIFY
prompt =========================
prompt
create table IM.IM_USER_WITH_MSGNOTIFY
(
	USERID 			NUMBER(30), 
	UUID 				VARCHAR2(80), 
	FOREIGN KEY(USERID) REFERENCES IM.IM_USER(USERID), 
	FOREIGN KEY(UUID) REFERENCES IM.IM_MSGNOTIFY(UUID) ON DELETE CASCADE
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
comment on column IM.IM_USER_WITH_MSGNOTIFY.USERID
  is '推送消息关联的用户ID';
comment on column IM.IM_USER_WITH_MSGNOTIFY.UUID
  is '推送消息唯一标识';
  
prompt
prompt Creating table IM_MSGREMIND
prompt =========================
prompt
create table IM.IM_MSGREMIND
(
	USERID 			NUMBER(30), 
	NEWORDER 		NUMBER(2) DEFAULT 1, 
	PAYORDER 		NUMBER(2) DEFAULT 1, 
	GOODSCOMMENT NUMBER(2) DEFAULT 1, 
	RETURNORDER NUMBER(2) DEFAULT 1, 
	FOREIGN KEY(USERID) REFERENCES IM.IM_USER(USERID)
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
comment on column IM.IM_MSGREMIND.USERID
  is '消息订阅配置-用户ID';
comment on column IM.IM_MSGREMIND.NEWORDER
  is '消息订阅配置-新订单配置';
comment on column IM.IM_MSGREMIND.PAYORDER
  is '消息订阅配置-已付款订单配置';
comment on column IM.IM_MSGREMIND.GOODSCOMMENT
  is '消息订阅配置-商品评价配置';
comment on column IM.IM_MSGREMIND.RETURNORDER
  is '消息订阅配置-退货配置';