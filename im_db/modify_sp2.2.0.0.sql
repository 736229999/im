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
  is '������ϢΨһ��ʶ';
comment on column IM.IM_MSGNOTIFY.INSERTTIME
  is '������Ϣд��ʱ��';
comment on column IM.IM_MSGNOTIFY.BUF
  is '������Ϣ����';
comment on column IM.IM_MSGNOTIFY.BUFFTYPE
  is '������Ϣ����';
  
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
  is '������Ϣ�������û�ID';
comment on column IM.IM_USER_WITH_MSGNOTIFY.UUID
  is '������ϢΨһ��ʶ';
  
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
  is '��Ϣ��������-�û�ID';
comment on column IM.IM_MSGREMIND.NEWORDER
  is '��Ϣ��������-�¶�������';
comment on column IM.IM_MSGREMIND.PAYORDER
  is '��Ϣ��������-�Ѹ��������';
comment on column IM.IM_MSGREMIND.GOODSCOMMENT
  is '��Ϣ��������-��Ʒ��������';
comment on column IM.IM_MSGREMIND.RETURNORDER
  is '��Ϣ��������-�˻�����';