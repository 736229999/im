----------------------------------------------
-- Export file for user IM               --
----------------------------------------------
spool im_tables.log
prompt
prompt Creating table im.IM_FRIENDGROUP
prompt ===============================
prompt
create table im.IM_FRIENDGROUP
(
    FriendGroupId  number(20),
    FriendGroupName    varchar2(80),
    ListOrder    number(4) default 0,
    FriendCount    number(10) default 0,
    UserId    number(20),
    CreateTime    date default sysdate,
    FriendGroupPid  number(20)
)tablespace ECSDATA
;

comment on column im.IM_FRIENDGROUP.FriendGroupId
	is '���ѷ���ID';
comment on column im.IM_FRIENDGROUP.FriendGroupName
	is '���ѷ�����';
comment on column im.IM_FRIENDGROUP.ListOrder
	is '��ʾ˳��';
comment on column im.IM_FRIENDGROUP.FriendCount
	is '��������';
comment on column im.IM_FRIENDGROUP.UserId
	is '������ԱID';
comment on column im.IM_FRIENDGROUP.CreateTime
	is '���ʱ��';
comment on column im.IM_FRIENDGROUP.FriendGroupPid	
	is '���ѷ��鸸��ID';

create index im.IM_FRIENDGROUP_IDX_USERID on im.IM_FRIENDGROUP (UserId) tablespace ECSINDEX;

prompt
prompt Creating table im.IM_FRIEND
prompt ===============================
prompt
create table im.IM_FRIEND
(
    ItemId    number(20),
    ListOrder    number(4) default 0,
    UserId    number(20) default 0,
    FriendId    number(20) default 0,
    FriendGroupId    number(20) default 0,
    UserName    varchar2(80),
    NickName    varchar2(100),
    TrueName    varchar2(20),
    Gender    number(1) default 1,
    Company    varchar2(100),
    Telephone    varchar2(20),
    Mobile    varchar2(20),
    HomePage    varchar2(255),
    Email    varchar2(50),
    AliasName    varchar2(50),
    IsBlankList    number(1) default 0,
    Note    varchar2(255),
    AddTime    date default sysdate,
    EditTime   date
)tablespace ECSDATA
;

comment on column im.IM_FRIEND.ItemId
	is '���к�';
comment on column im.IM_FRIEND.ListOrder
	is '��ʾ˳��';
comment on column im.IM_FRIEND.UserId
	is '��ԱID';
comment on column im.IM_FRIEND.FriendId
	is '���ѻ�ԱID';
comment on column im.IM_FRIEND.FriendGroupId
	is '���ѷ���ID';
comment on column im.IM_FRIEND.UserName
	is '��Ա��';
comment on column im.IM_FRIEND.NickName
	is '�ǳ�';
comment on column im.IM_FRIEND.TrueName
	is '��ʵ����';
comment on column im.IM_FRIEND.Gender
	is '�Ա�(0-Ů 1-��)';
comment on column im.IM_FRIEND.Company
	is '��˾����';
comment on column im.IM_FRIEND.Telephone
	is '�绰';
comment on column im.IM_FRIEND.Mobile
	is '�ֻ�';
comment on column im.IM_FRIEND.HomePage
	is '��ҳ';
comment on column im.IM_FRIEND.Email
	is 'Email';
comment on column im.IM_FRIEND.AliasName
	is '��ע����';
comment on column im.IM_FRIEND.IsBlankList
	is '�Ƿ���������(0-�� 1-��)';
comment on column im.IM_FRIEND.Note
	is '��ע';
comment on column im.IM_FRIEND.AddTime
	is '���ʱ��';
comment on column im.IM_FRIEND.EditTime
	is '�޸�ʱ��';

create index im.IM_FRIEND_IDX_USERID on im.IM_FRIEND (UserId) tablespace ECSINDEX;

prompt
prompt Creating table im.IM_USER
prompt ===============================
prompt
create table im.IM_USER
(
    UserId	number(20) primary key,
    UserType	number(1)	,
    UserClass	number(4)	,
    GroupId	number(4)	,
    UserName	varchar2(80) not null,
    Password	varchar2(60)	,
    Vip	number(2)	,
    Vipend	date	,
    Email	varchar2(50)	,
    Gender	number(1)	,
    TrueName	varchar2(20)	,
    Telephone	varchar2(50)	,
    Mobile	varchar2(50)	,
    IdCard	varchar2(30)	,
    Company	varchar2(100)	,
    CompanyType	varchar2(100)	,
    CatId	varchar2(100)	,
    AreaId	number(6)	,
    Modeds	varchar2(100)	,
    Capital	number(16,2)	,
    RegUnit	varchar2(15)	,
    Sizeds	varchar2(100)	,
    RegYear	varchar2(4)	,
    RegCity	varchar2(30)	,
    Business	varchar2(255)	,
    CompanyPhone	varchar2(50)	,
    Fax	varchar2(50)	,
    CompanyEmail	varchar2(50)	,
    Address	varchar2(255)	,
    PostCode	varchar2(20)	,
    HomePage	varchar2(255)	,
    Thumb	varchar2(255)	,
    Introduce	varchar2(255)	,
    TemplateDir	varchar2(30)	,
    MarketId	number(20)	,
    EpayId	varchar2(30)	,
    Regtime	date	,
    RegIp	varchar2(50)	,
    Edittime	date	,
    EditIp	varchar2(50)	,
    LoginTime	date	,
    LoginIp	varchar2(50)	,
    LoginTimes	number(10)	,
    Credit	number(10)	,
    Hits	number(10)	,
    AuthId	number(20)	,
    AuthTime	date	,
    Status	number(1)	,
    IsLocked	number(1)	,
    IdentifyStatus	number(1)	,
    IdentifyTime	date	,
    LockOpId	number(20)	,
    LockOpTime	date	,
    DealSearch	number(1)	
) tablespace ECSDATA
;

comment on column im.IM_USER.UserId
	is '��ԱID';
comment on column im.IM_USER.UserType
	is '��Ա����';
comment on column im.IM_USER.UserClass
	is '��Ա����';
comment on column im.IM_USER.GroupId
	is '��Ա����ID';
comment on column im.IM_USER.UserName
	is '��Ա��';
comment on column im.IM_USER.Password
	is '��¼����';
comment on column im.IM_USER.Vip
	is '���Ż�Ա';
comment on column im.IM_USER.Vipend
	is '�շѻ�Ա����ʱ��';
comment on column im.IM_USER.Email
	is '��������';
comment on column im.IM_USER.Gender
	is '�Ա�';
comment on column im.IM_USER.TrueName
	is '��ʵ����';
comment on column im.IM_USER.Telephone
	is '��ϵ�˵绰����';
comment on column im.IM_USER.Mobile
	is '��ϵ���ֻ�����';
comment on column im.IM_USER.IdCard
	is '���֤����';
comment on column im.IM_USER.Company
	is '��˾����';
comment on column im.IM_USER.CompanyType
	is '��˾����';
comment on column im.IM_USER.CatId
	is '��Ӫ��ҵϸ��';
comment on column im.IM_USER.AreaId
	is '���ڵ���';
comment on column im.IM_USER.Modeds
	is '��Ӫģʽ';
comment on column im.IM_USER.Capital
	is 'ע���ʱ�';
comment on column im.IM_USER.RegUnit
	is '���ҵ�λ';
comment on column im.IM_USER.Sizeds
	is '��ģ';
comment on column im.IM_USER.RegYear
	is 'ע�����';
comment on column im.IM_USER.RegCity
	is 'ע�����';
comment on column im.IM_USER.Business
	is '��Ӫ��Χ';
comment on column im.IM_USER.CompanyPhone
	is '��˾�绰����';
comment on column im.IM_USER.Fax
	is '����';
comment on column im.IM_USER.CompanyEmail
	is '����Email��ַ';
comment on column im.IM_USER.Address
	is '��ϵ��ַ';
comment on column im.IM_USER.PostCode
	is '��������';
comment on column im.IM_USER.HomePage
	is '��ַ';
comment on column im.IM_USER.Thumb
	is '����ͼƬURL';
comment on column im.IM_USER.Introduce
	is '���';
comment on column im.IM_USER.TemplateDir
	is '��ҳģ��Ŀ¼';
comment on column im.IM_USER.MarketId
	is 'רҵ�г�ID';
comment on column im.IM_USER.EpayId
	is 'E֧��ƽ̨�ͻ���';
comment on column im.IM_USER.Regtime
	is 'ע��ʱ��';
comment on column im.IM_USER.RegIp
	is 'ע����IP';
comment on column im.IM_USER.Edittime
	is '�޸�ʱ��';
comment on column im.IM_USER.EditIp
	is '�޸���IP';
comment on column im.IM_USER.LoginTime
	is '��¼ʱ��';
comment on column im.IM_USER.LoginIp
	is '��¼IP';
comment on column im.IM_USER.LoginTimes
	is '��¼����';
comment on column im.IM_USER.Credit
	is '����ָ��';
comment on column im.IM_USER.Hits
	is '�����';
comment on column im.IM_USER.AuthId
	is '�����ID';
comment on column im.IM_USER.AuthTime
	is '���ʱ��';
comment on column im.IM_USER.Status
	is '��Ա״̬';
comment on column im.IM_USER.IsLocked
	is '����״̬';
comment on column im.IM_USER.IdentifyStatus
	is 'ʵ����֤״̬';
comment on column im.IM_USER.IdentifyTime
	is 'ʵ����֤ʱ��';
comment on column im.IM_USER.LockOpId
	is '�������������ԱID';
comment on column im.IM_USER.LockOpTime
	is '���������ʱ��';
comment on column im.IM_USER.DealSearch
	is '�������洦����';

prompt
prompt Creating table IM_CLIENTMSGINFO
prompt ===============================
prompt
create table im.IM_CLIENTMSGINFO
(
  RECID      NUMBER(20) not null,
  USERID     NUMBER(20) default 0,
  MSGTYPE    NUMBER(5) not null,
  CONTENT    VARCHAR2(4000) not null,
  LOGID      NUMBER(10) default 0,
  STATUS     NUMBER(2) default 1,
  SENDERID   NUMBER(10) default 0,
  SENDERNAME VARCHAR2(20) not null,
  CREATEDATE DATE default sysdate
) tablespace ECSDATA
;

comment on column im.IM_CLIENTMSGINFO.RECID
	is '��ˮID';
comment on column im.IM_CLIENTMSGINFO.USERID
	is '���շ��û�ID';
comment on column im.IM_CLIENTMSGINFO.MSGTYPE
	is '��Ϣ����';
comment on column im.IM_CLIENTMSGINFO.CONTENT
	is '��������';
comment on column im.IM_CLIENTMSGINFO.LOGID
	is '����Id';
comment on column im.IM_CLIENTMSGINFO.STATUS
	is '����״̬';
comment on column im.IM_CLIENTMSGINFO.SENDERID
	is '���ͷ��û�Id';
comment on column im.IM_CLIENTMSGINFO.SENDERNAME
	is '���ͷ��û���';
comment on column im.IM_CLIENTMSGINFO.CREATEDATE
	is '��������';

create unique index im.IM_CLIENTMSGINFO_IDX_RECID on im.IM_CLIENTMSGINFO(RECID) tablespace ECSINDEX;
alter table im.IM_CLIENTMSGINFO
  add constraint IM_CLIENTMSGINFO_IDX_RECID primary key (RECID) using index;
create index im.IM_CLIENTMSGINFO_IDX_STATUS on im.IM_CLIENTMSGINFO (STATUS) tablespace ECSINDEX;

prompt
prompt Creating table IM_CLUSTER
prompt =========================
prompt
create table im.IM_CLUSTER
(
  CLUSTERID      NUMBER(20) not null,
  CLUSTERDOMAIN  VARCHAR2(20),
  CLUSTERNAME    VARCHAR2(80) not null,
  CLUSTERDESC    VARCHAR2(500),
  CLUSTERGROUPID NUMBER(20) default 0 not null,
  CLUSTERLEVEL   NUMBER(2) default 0,
  LOGOURI        VARCHAR2(200),
  USERID         NUMBER(20) not null,
  JOINMETHOD     NUMBER(1) default 0,
  VIEWAUTHFLAG   NUMBER(2) default 3,
  VIEWPASSWD     VARCHAR2(40),
  MEMBERCOUNT    NUMBER(10) default 1,
  MEMBERLIMIT    NUMBER(10) default 99999,
  STATUS         NUMBER(1) default 0,
  CREATETIME     DATE default sysdate not null,
  EDITTIME       DATE,
  CLUSTERSIGN 	 VARCHAR2(101),
  CLUSTERNOTICE  VARCHAR2(101)
) tablespace ECSDATA
;

comment on column im.IM_CLUSTER.CLUSTERID
	is 'ȺID';
comment on column im.IM_CLUSTER.CLUSTERDOMAIN
	is 'Ⱥ����';
comment on column im.IM_CLUSTER.CLUSTERNAME
	is 'Ⱥ����';
comment on column im.IM_CLUSTER.CLUSTERDESC
	is 'Ⱥ����';
comment on column im.IM_CLUSTER.CLUSTERGROUPID
	is 'Ⱥ����ID';
comment on column im.IM_CLUSTER.CLUSTERLEVEL
	is 'Ⱥ�ȼ�(0-��ͨȺ��1-�߼�Ⱥ��2-����Ⱥ)';
comment on column im.IM_CLUSTER.LOGOURI
	is 'Ⱥͼ���ַ';
comment on column im.IM_CLUSTER.USERID
	is '����Ⱥ���û�Id';
comment on column im.IM_CLUSTER.JOINMETHOD
	is '��֤��ʽ(0-�����κ��˼��룬1-��Ҫ�����֤��2-�������κ��˼���)';
comment on column im.IM_CLUSTER.VIEWAUTHFLAG
	is 'Ⱥҳ�����Ȩ��(1-Ⱥ��Ա��2-ע���û���3-�����û���4-�������)';
comment on column im.IM_CLUSTER.VIEWPASSWD
	is '��������';
comment on column im.IM_CLUSTER.MEMBERCOUNT
	is '��Ա��';
comment on column im.IM_CLUSTER.MEMBERLIMIT
	is '����Ա��';
comment on column im.IM_CLUSTER.STATUS
	is 'Ⱥ״̬';
comment on column im.IM_CLUSTER.CREATETIME
	is '��������';
comment on column im.IM_CLUSTER.EDITTIME
	is '�޸�����';
comment on column im.IM_CLUSTER.CLUSTERSIGN
	is 'Ⱥǩ��';
comment on column im.IM_CLUSTER.CLUSTERNOTICE
	is 'Ⱥ����';

create unique index im.IM_CLUSTER_PK_CLUSTERID on im.IM_CLUSTER(CLUSTERID) tablespace ECSINDEX;
alter table im.IM_CLUSTER
  add constraint IM_CLUSTER_PK_CLUSTERID primary key (CLUSTERID) using index;
create unique index im.IM_CLUSTER_IDX_USERCLUSTER on im.IM_CLUSTER (USERID, CLUSTERNAME) tablespace ECSINDEX;

prompt
prompt Creating table IM_CLUSTERGROUP
prompt ==============================
prompt
create table im.IM_CLUSTERGROUP
(
  CLUSTERGROUPID   NUMBER(20) not null,
  CLUSTERGROUPNAME VARCHAR2(80) not null,
  LISTORDER        NUMBER(4) default 0,
  CLUSTERCOUNT     NUMBER(10) default 0 not null,
  USERID           NUMBER(20) not null,
  CREATETIME       DATE default sysdate,
  EDITTIME         DATE
) tablespace ECSDATA
;

comment on column im.IM_CLUSTERGROUP.CLUSTERGROUPID
	is 'Ⱥ����ID';
comment on column im.IM_CLUSTERGROUP.CLUSTERGROUPNAME
	is 'Ⱥ������';
comment on column im.IM_CLUSTERGROUP.LISTORDER
	is '��ʾ˳��';
comment on column im.IM_CLUSTERGROUP.CLUSTERCOUNT
	is 'Ⱥ����';
comment on column im.IM_CLUSTERGROUP.USERID
	is '������ԱID';
comment on column im.IM_CLUSTERGROUP.CREATETIME
	is '���ʱ��';
comment on column im.IM_CLUSTERGROUP.EDITTIME
	is '�޸�����';

create unique index im.IM_CLUSTERGROUP_PK_GROUPID on im.IM_CLUSTERGROUP(CLUSTERGROUPID) tablespace ECSINDEX;
alter table im.IM_CLUSTERGROUP
  add constraint IM_CLUSTERGROUP_PK_GROUPID primary key (CLUSTERGROUPID) using index;
create unique index im.IM_CLUSTERGROUP_IDX_USERGROUP on im.IM_CLUSTERGROUP (USERID, CLUSTERGROUPNAME) tablespace ECSINDEX;

prompt
prompt Creating table IM_CLUSTERMEMBER
prompt ===============================
prompt
create table im.IM_CLUSTERMEMBER
(
  CLUSTERID     NUMBER(20) not null,
  USERID        NUMBER(20) not null,
  USERNAME      VARCHAR2(80) not null,
  STATUS        NUMBER(2) default 0,
  USERLEVEL     NUMBER(3) default 0,
  JOINMETHOD    NUMBER(1) default 0,
  VERIFYUSERID  NUMBER(20),
  VERIFYTIME    DATE,
  MSGIDRECEIVED NUMBER(20) default 0 not null,
  CREATETIME    DATE default sysdate not null,
  EDITTIME      DATE,
  PROFILENAME   VARCHAR2(80)
) tablespace ECSDATA
;

comment on column im.IM_CLUSTERMEMBER.CLUSTERID
	is 'ȺID';
comment on column im.IM_CLUSTERMEMBER.USERID
	is '�û�ID';
comment on column im.IM_CLUSTERMEMBER.USERNAME
	is '�û���';
comment on column im.IM_CLUSTERMEMBER.STATUS
	is '�û�״̬(0-������1-�����)';
comment on column im.IM_CLUSTERMEMBER.USERLEVEL
	is '�û��ȼ�(0-��ͨ��Ա��1-����Ա)';
comment on column im.IM_CLUSTERMEMBER.JOINMETHOD
	is '���뷽ʽ(0-���ɼ��룬1-������룬2-������ʽ)';
comment on column im.IM_CLUSTERMEMBER.VERIFYUSERID
	is '�����ID';
comment on column im.IM_CLUSTERMEMBER.VERIFYTIME
	is '���ʱ��';
comment on column im.IM_CLUSTERMEMBER.MSGIDRECEIVED
	is '�����յ���Ⱥ��Ϣ��MsgId';
comment on column im.IM_CLUSTERMEMBER.CREATETIME
	is '��������';
comment on column im.IM_CLUSTERMEMBER.EDITTIME
	is '�޸�����';
comment on column im.IM_CLUSTERMEMBER.PROFILENAME
	is 'Ⱥ��Ƭ';

create unique index im.IM_CLUSTERMEMBER_IDX_CLUSTERID on im.IM_CLUSTERMEMBER (CLUSTERID, USERID) tablespace ECSINDEX;
create index im.IM_CLUSTERMEMBER_IDX_USERID on im.IM_CLUSTERMEMBER (USERID) tablespace ECSINDEX;

prompt
prompt Creating table IM_CLUSTERSYSMSG
prompt ===============================
prompt
create table im.IM_CLUSTERSYSMSG
(
  MSGID     NUMBER(20) default 0 not null,
  CLUSTERID NUMBER(20) default 0 not null,
  SRCIP     NUMBER(10) default 0 not null,
  SRCPORT   NUMBER(6) default 0 not null,
  SENDTIME  NUMBER(10) default 0 not null,
  MSGTYPE   NUMBER(6) default 0 not null,
  SENDER    NUMBER(20) default 0 not null,
  MSGLEN    NUMBER(10) default 0 not null,
  MSG       BLOB not null
) tablespace ECSDATA
;

comment on column im.IM_CLUSTERSYSMSG.MSGID
	is '��ϢID';
comment on column im.IM_CLUSTERSYSMSG.CLUSTERID
	is 'ȺID';
comment on column im.IM_CLUSTERSYSMSG.SRCIP
	is 'ԴIP';
comment on column im.IM_CLUSTERSYSMSG.SRCPORT
	is 'Դ�˿�';
comment on column im.IM_CLUSTERSYSMSG.SENDTIME
	is '����ʱ��';
comment on column im.IM_CLUSTERSYSMSG.MSGTYPE
	is '��Ϣ����';
comment on column im.IM_CLUSTERSYSMSG.SENDER
	is '������ID';
comment on column im.IM_CLUSTERSYSMSG.MSGLEN
	is '��Ϣ����';
comment on column im.IM_CLUSTERSYSMSG.MSG
	is '��Ϣ����';

create unique index im.IM_CLUSTERSYSMSG_IDX_MSGID on im.IM_CLUSTERSYSMSG(MSGID) tablespace ECSINDEX;
alter table im.IM_CLUSTERSYSMSG
  add constraint IM_CLUSTERSYSMSG_IDX_MSGID primary key (MSGID) using index;
create index im.IM_CLUSTERSYSMSG_IDX_CLUSTERID on im.IM_CLUSTERSYSMSG (CLUSTERID) tablespace ECSINDEX;
create index im.IM_CLUSTERSYSMSG_IDX_SENDER on im.IM_CLUSTERSYSMSG (SENDER) tablespace ECSINDEX;

prompt
prompt Creating table IM_CLUSTERTALKMSG
prompt ================================
prompt
create table im.IM_CLUSTERTALKMSG
(
  MSGID     NUMBER(20) default 0 not null,
  CLUSTERID NUMBER(20) default 0 not null,
  SRCIP     NUMBER(10) default 0 not null,
  SRCPORT   NUMBER(6) default 0 not null,
  SENDTIME  NUMBER(10) default 0 not null,
  MSGTYPE   NUMBER(6) default 0 not null,
  SENDER    NUMBER(20) default 0 not null,
  MSGLEN    NUMBER(10) default 0 not null,
  MSG       BLOB not null
) tablespace ECSDATA
;

comment on column im.IM_CLUSTERTALKMSG.MSGID
	is '��ϢID';
comment on column im.IM_CLUSTERTALKMSG.CLUSTERID
	is 'ȺID';
comment on column im.IM_CLUSTERTALKMSG.SRCIP
	is 'ԴIP';
comment on column im.IM_CLUSTERTALKMSG.SRCPORT
	is 'Դ�˿�';
comment on column im.IM_CLUSTERTALKMSG.SENDTIME
	is '����ʱ��';
comment on column im.IM_CLUSTERTALKMSG.MSGTYPE
	is '��Ϣ����';
comment on column im.IM_CLUSTERTALKMSG.SENDER
	is '������ID';
comment on column im.IM_CLUSTERTALKMSG.MSGLEN
	is '��Ϣ����';
comment on column im.IM_CLUSTERTALKMSG.MSG
	is '��Ϣ����';

create unique index im.IM_CLUSTERTALKMSG_I_CLUSTERMSG on im.IM_CLUSTERTALKMSG(CLUSTERID, MSGID) tablespace ECSINDEX;
alter table im.IM_CLUSTERTALKMSG
  add constraint IM_CLUSTERTALKMSG_I_CLUSTERMSG primary key (CLUSTERID, MSGID) using index;
create index im.IM_CLUSTERTALKMSG_IDX_SENDER on im.IM_CLUSTERTALKMSG (SENDER) tablespace ECSINDEX;
create index im.IM_CLUSTERTALKMSG_I_CLUSTERID on im.IM_CLUSTERTALKMSG (CLUSTERID) tablespace ECSINDEX;

prompt
prompt Creating table IM_LOGINSTAT
prompt ===========================
prompt
create table im.IM_LOGINSTAT
(
  USERID     NUMBER(20),
  USERIP     NUMBER(10),
  LOGINDATE  DATE default sysdate,
  LOGINTIMES NUMBER(5) default 1
) tablespace ECSDATA
;

comment on column im.IM_LOGINSTAT.USERID
	is '�û�ID';
comment on column im.IM_LOGINSTAT.USERIP
	is '��¼IP';
comment on column im.IM_LOGINSTAT.LOGINDATE
	is '��¼����';
comment on column im.IM_LOGINSTAT.LOGINTIMES
	is '��¼����';

create index im.IM_LOGINSTAT_IDX_LOGINDATE on im.IM_LOGINSTAT (LOGINDATE) tablespace ECSINDEX;
create index im.IM_LOGINSTAT_IDX_USERID on im.IM_LOGINSTAT (USERID) tablespace ECSINDEX;
create index im.IM_LOGINSTAT_IDX_USERIP on im.IM_LOGINSTAT (USERIP) tablespace ECSINDEX;

prompt
prompt Creating table IM_OFFLINESYSMSG
prompt ===============================
prompt
create table im.IM_OFFLINESYSMSG
(
  MSGID    NUMBER(20) default 0 not null,
  RECEIVER NUMBER(20) default 0 not null,
  SENDER   NUMBER(20) default 0 not null,
  SRCIP    NUMBER(10) default 0 not null,
  SRCPORT  NUMBER(6) default 0 not null,
  SENDTIME NUMBER(10) default 0 not null,
  MSGTYPE  NUMBER(6) default 0 not null,
  MSGLEN   NUMBER(10) default 0 not null,
  MSG      BLOB not null
) tablespace ECSDATA
;

comment on column im.IM_OFFLINESYSMSG.MSGID
	is '��ϢID';
comment on column im.IM_OFFLINESYSMSG.RECEIVER
	is '������ID';
comment on column im.IM_OFFLINESYSMSG.SENDER
	is '������ID';
comment on column im.IM_OFFLINESYSMSG.SRCIP
	is 'ԴIP';
comment on column im.IM_OFFLINESYSMSG.SRCPORT
	is 'Դ�˿�';
comment on column im.IM_OFFLINESYSMSG.SENDTIME
	is '����ʱ��';
comment on column im.IM_OFFLINESYSMSG.MSGTYPE
	is '��Ϣ����';
comment on column im.IM_OFFLINESYSMSG.MSGLEN
	is '��Ϣ����';
comment on column im.IM_OFFLINESYSMSG.MSG
	is '��Ϣ����';

create unique index im.IM_OFFLINESYSMSG_IDX_MSGID on im.IM_OFFLINESYSMSG(MSGID) tablespace ECSINDEX;
alter table im.IM_OFFLINESYSMSG
  add constraint IM_OFFLINESYSMSG_IDX_MSGID primary key (MSGID) using index;
create index im.IM_OFFLINESYSMSG_IDX_RECEIVER on im.IM_OFFLINESYSMSG (RECEIVER) tablespace ECSINDEX;

prompt
prompt Creating table IM_OFFLINETALKMSG
prompt ================================
prompt
create table im.IM_OFFLINETALKMSG
(
  MSGID    NUMBER(20) default 0 not null,
  RECEIVER NUMBER(20) default 0 not null,
  SENDER   NUMBER(20) default 0 not null,
  SRCIP    NUMBER(10) default 0 not null,
  SRCPORT  NUMBER(6) default 0 not null,
  SENDTIME date default sysdate not null,
  MSGTYPE  NUMBER(6) default 0 not null,
  MSGLEN   NUMBER(10) default 0 not null,
  MSG      BLOB,
  ReceiverName varchar2(100),
  SenderName   varchar2(100),
  ReadTime     date,
  Status       number(2) default 0 not null,
  Text         varchar2(1024) not null,
  uuid		varchar(41)
) tablespace ECSDATA
;

comment on column im.IM_OFFLINETALKMSG.MSGID
	is '��ϢID';
comment on column im.IM_OFFLINETALKMSG.RECEIVER
	is '������ID';
comment on column im.IM_OFFLINETALKMSG.SENDER
	is '������ID';
comment on column im.IM_OFFLINETALKMSG.SRCIP
	is 'ԴIP';
comment on column im.IM_OFFLINETALKMSG.SRCPORT
	is 'Դ�˿�';
comment on column im.IM_OFFLINETALKMSG.SENDTIME
	is '����ʱ��';
comment on column im.IM_OFFLINETALKMSG.MSGTYPE
	is '��Ϣ����';
comment on column im.IM_OFFLINETALKMSG.MSGLEN
	is '��Ϣ����';
comment on column im.IM_OFFLINETALKMSG.MSG
	is '��Ϣ����';
comment on column im.IM_OFFLINETALKMSG.ReceiverName
	is '�������û�';
comment on column im.IM_OFFLINETALKMSG.SenderName
	is '�������û�';
comment on column im.IM_OFFLINETALKMSG.ReadTime
	is '��ȡʱ��';
comment on column im.IM_OFFLINETALKMSG.Status
	is '��ȡ״̬';
comment on column im.IM_OFFLINETALKMSG.Text
	is '��Ϣ����';

create unique index im.IM_OFFLINETALKMSG_IDX_MSGID on im.IM_OFFLINETALKMSG(MSGID) tablespace ECSINDEX;
alter table im.IM_OFFLINETALKMSG
  add constraint IM_OFFLINETALKMSG_IDX_MSGID primary key (MSGID) using index;
create index im.IM_OFFLINETALKMSG_IDX_RECEIVER on im.IM_OFFLINETALKMSG (RECEIVER) tablespace ECSINDEX;
create index im.IM_OFFLINETALKMSG_IDX_SENDTIME on im.IM_OFFLINETALKMSG (SENDTIME) tablespace ECSINDEX;

prompt
prompt Creating table IM_ONLINEUSER
prompt ============================
prompt
create table im.IM_ONLINEUSER
(
  ONLINEUSER NUMBER(20) not null,
  CREATETIME DATE default sysdate not null
) tablespace ECSDATA
;

comment on column im.IM_ONLINEUSER.ONLINEUSER
	is '��������';
comment on column im.IM_ONLINEUSER.CREATETIME
	is 'ʱ��';

create index im.IM_ONLINEUSER_IDX_CREATETIME on im.IM_ONLINEUSER (CREATETIME) tablespace ECSINDEX;
create index im.IM_ONLINEUSER_IDX_ONLINEUSER on im.IM_ONLINEUSER (ONLINEUSER) tablespace ECSINDEX;

prompt
prompt Creating table IM_OPDICT
prompt ========================
prompt
create table im.IM_OPDICT
(
  OPID   NUMBER(10) not null,
  OPDESC VARCHAR2(200) not null
) tablespace ECSDATA
;

comment on column im.IM_OPDICT.OPID
	is '��������';
comment on column im.IM_OPDICT.OPDESC
	is '��������';

create unique index im.IM_OPDICT_IDX_OPID on im.IM_OPDICT (OPID) tablespace ECSINDEX;

prompt
prompt Creating table IM_OPSTAT
prompt ========================
prompt
create table im.IM_OPSTAT
(
  USERID  NUMBER(20) not null,
  OPID    NUMBER(10) not null,
  OPCOUNT NUMBER(10) default 0,
  OPTIME  DATE default sysdate
) tablespace ECSDATA
;

comment on column im.IM_OPSTAT.USERID
	is '�û�ID';
comment on column im.IM_OPSTAT.OPID
	is '��������';
comment on column im.IM_OPSTAT.OPCOUNT
	is '��������';
comment on column im.IM_OPSTAT.OPTIME
	is '��¼ʱ��';

create index im.IM_OPSTAT_IDX_OPID on im.IM_OPSTAT (OPID) tablespace ECSINDEX;
create index im.IM_OPSTAT_IDX_USERID on im.IM_OPSTAT (USERID) tablespace ECSINDEX;

prompt
prompt Creating table IM_SYSMESSAGE
prompt ============================
prompt
create table im.IM_SYSMESSAGE
(
  MSGID      NUMBER(20) not null,
  TITLE      VARCHAR2(64),
  CONTENT    VARCHAR2(512) not null,
  CREATEDATE DATE default sysdate,
  TOID VARCHAR2(512) not null
) tablespace ECSDATA
;

comment on column im.IM_SYSMESSAGE.MSGID
	is '��ϢID';
comment on column im.IM_SYSMESSAGE.TITLE
	is '����';
comment on column im.IM_SYSMESSAGE.CONTENT
	is '����';
comment on column im.IM_SYSMESSAGE.CREATEDATE
	is '����ʱ��';
comment on column im.IM_SYSMESSAGE.TOID
	is '���շ�ID';

create unique index im.IM_SYSMESSAGE_IDX_MSGID on im.IM_SYSMESSAGE(MSGID) tablespace ECSINDEX;
alter table im.IM_SYSMESSAGE
  add constraint IM_SYSMESSAGE_IDX_MSGID primary key (MSGID) using index;

prompt
prompt Creating table IM_SYSTEMMSG
prompt ===========================
prompt
create table im.IM_SYSTEMMSG
(
  MSGID      NUMBER(10) not null,
  MSG        VARCHAR2(1200) not null,
  EXPIRETIME DATE not null,
  PUBTIME    DATE default sysdate,
  CONTENTS   CLOB
) tablespace ECSDATA
;

comment on column im.IM_SYSTEMMSG.MSGID
	is '��ϢID';
comment on column im.IM_SYSTEMMSG.MSG
	is '��ϢժҪ';
comment on column im.IM_SYSTEMMSG.EXPIRETIME
	is '����ʱ��';
comment on column im.IM_SYSTEMMSG.PUBTIME
	is '����ʱ��';
comment on column im.IM_SYSTEMMSG.CONTENTS
	is '��ϸ����';

create unique index im.IM_SYSTEMMSG_IDX_MSGID on im.IM_SYSTEMMSG(MSGID) tablespace ECSINDEX;
alter table im.IM_SYSTEMMSG
  add constraint IM_SYSTEMMSG_IDX_MSGID primary key (MSGID) using index;
create index im.IM_SYSTEMMSG_IDX_EXPIRETIME on im.IM_SYSTEMMSG (EXPIRETIME) tablespace ECSINDEX;

prompt
prompt Creating table IM_TIMELEN
prompt =========================
prompt
create table im.IM_TIMELEN
(
  USERID     NUMBER(20) not null,
  TIMELEN    NUMBER(10) default 0 not null,
  CREATETIME DATE default sysdate not null
) tablespace ECSDATA
;

comment on column im.IM_TIMELEN.USERID
	is '�û�ID';
comment on column im.IM_TIMELEN.TIMELEN
	is '����ʱ��';
comment on column im.IM_TIMELEN.CREATETIME
	is '��¼ʱ��';

create index im.IM_TIMELEN_IDX_CREATEDATE on im.IM_TIMELEN (CREATETIME) tablespace ECSINDEX;
create index im.IM_TIMELEN_IDX_USERID on im.IM_TIMELEN (USERID) tablespace ECSINDEX;

prompt
prompt Creating table IM_USERINFO
prompt ==========================
prompt
create table im.IM_USERINFO
(
  USERID       NUMBER(20) not null,
  NICKNAME     VARCHAR2(100) not null,
  USERSIGN     VARCHAR2(250),
  USERPICURI   VARCHAR2(100),
  CITY         NUMBER(5),
  NATION       NUMBER(5) default 0,
  PROVINCE     NUMBER(5),
  JOINAUTHFLAG NUMBER(2) default 0,
  USERDESC     VARCHAR2(500),
  BIRTHDAY     VARCHAR2(8),
  JOIN2FAMILY  NUMBER(1) default 0,
  USRADDRESS   VARCHAR2(80),
  LINKPOP      NUMBER(1) default 1,
  INTEGRAL     NUMBER(10) default 0,
  CAREER       NUMBER(3),
  GROUPNAME	   VARCHAR2(100)
) tablespace ECSDATA
;

comment on column im.IM_USERINFO.USERID
	is '�û�ID';
comment on column im.IM_USERINFO.NICKNAME
	is '�ǳ�';
comment on column im.IM_USERINFO.USERSIGN
	is 'ǩ��';
comment on column im.IM_USERINFO.USERPICURI
	is 'ͷ��URI';
comment on column im.IM_USERINFO.CITY
	is '����';
comment on column im.IM_USERINFO.NATION
	is '���Ҵ���';
comment on column im.IM_USERINFO.PROVINCE
	is 'ʡ����';
comment on column im.IM_USERINFO.JOINAUTHFLAG
	is '��Ϊ������֤��־(0������Ҫ��֤��1����Ҫ��֤��2��ֻ���ձ������û�����)';
comment on column im.IM_USERINFO.USERDESC
	is '�û����ҽ���';
comment on column im.IM_USERINFO.BIRTHDAY
	is '��������(YYYYMMDD)';
comment on column im.IM_USERINFO.JOIN2FAMILY
	is '�Ƿ�������';
comment on column im.IM_USERINFO.USRADDRESS
	is '��ϵ��ַ';
comment on column im.IM_USERINFO.LINKPOP
	is '�Ƿ񹫿���ϵ��ʽ(0-�Լ�(������)��1-���ѣ�2-ע���û���3-����)';
comment on column im.IM_USERINFO.INTEGRAL
	is '���׵Ļ���';
comment on column im.IM_USERINFO.CAREER
	is 'ְҵ';

create index im.IM_USERINFO_IDX_ADDRESS on im.IM_USERINFO (NATION, PROVINCE, CITY) tablespace ECSINDEX;
create index im.IM_USERINFO_IDX_CITY on im.IM_USERINFO (CITY) tablespace ECSINDEX;
create index im.IM_USERINFO_IDX_NICKNAME on im.IM_USERINFO (NICKNAME) tablespace ECSINDEX;
create unique index im.IM_USERINFO_IDX_USERID on im.IM_USERINFO (USERID) tablespace ECSINDEX;
create index im.IM_USERINFO_IDX_USERPICURI on im.IM_USERINFO (USERPICURI) tablespace ECSINDEX;

prompt
prompt Creating table IM_USERSTAT
prompt ==========================
prompt
create table im.IM_USERSTAT
(
  USERID        NUMBER(20) not null,
  MSGID         NUMBER(20) default 0,
  LSTLOGINTIME  NUMBER(10) default 0,
  LSTLOGOUTTIME NUMBER(10) default 0,
  TOTALLINETIME NUMBER(10,1) default 0,
  TOTALMONEY    NUMBER(10) default 0,
  LOGINCOUNT    NUMBER(10) default 0,
  TODAYLINETIME NUMBER(10,1) default 0,
  USERMESSAGE   NUMBER(10) default 0,
  SYSMESSAGE    NUMBER(10) default 0,
  BROADCASTINFO NUMBER(10) default 0,
  USEROPERATION NUMBER(10) default 0,
  COUNTFLAG     NUMBER(2) default 0,
  CREATETIME    DATE default sysdate
) tablespace ECSDATA
;

comment on column im.IM_USERSTAT.USERID
	is '�û�ID';
comment on column im.IM_USERSTAT.MSGID
	is '��ϢID';
comment on column im.IM_USERSTAT.LSTLOGINTIME
	is '����¼ʱ��';
comment on column im.IM_USERSTAT.LSTLOGOUTTIME
	is '���ǳ�ʱ��';
comment on column im.IM_USERSTAT.TOTALLINETIME
	is '������ʱ��';
comment on column im.IM_USERSTAT.TOTALMONEY
	is '�������';
comment on column im.IM_USERSTAT.LOGINCOUNT
	is '��¼����';
comment on column im.IM_USERSTAT.TODAYLINETIME
	is '��������ʱ��';
comment on column im.IM_USERSTAT.USERMESSAGE
	is '�û���Ϣ��';
comment on column im.IM_USERSTAT.SYSMESSAGE
	is 'ϵͳ��Ϣ��';
comment on column im.IM_USERSTAT.BROADCASTINFO
	is '������Ϣ��';
comment on column im.IM_USERSTAT.USEROPERATION
	is '�û�������Ϣ';
comment on column im.IM_USERSTAT.COUNTFLAG
	is '�Ƿ���ͳ�ƽ��(0-δͳ�ƣ�1-��ͳ��)';
comment on column im.IM_USERSTAT.CREATETIME
	is '����ʱ��';

create index im.IM_USERSTAT_IDX_CREATETIME on im.IM_USERSTAT (CREATETIME) tablespace ECSINDEX;
create unique index im.IM_USERSTAT_IDX_USERID on im.IM_USERSTAT (USERID) tablespace ECSINDEX;

prompt
prompt Creating table IM_USERONLINE
prompt ============================
prompt
create table im.IM_USERONLINE
(
  UserId             number(20) not null,                    --�û�ID
  SrcIp              number(10) default 0 not null,          --ԴIP
  SrcPort            number(6) default 0 not null,           --Դ�˿�
  ServerNo           number(10) default 0 not null,          --���������
  Status             number(2) default 0,                    --����״̬��0-���� 1-����
  CreateTime         date default sysdate not null           --��¼ʱ��
) tablespace ECSDATA;

comment on column im.IM_USERONLINE.UserId
	is '�û�ID';
comment on column im.IM_USERONLINE.SrcIp
	is 'ԴIP';
comment on column im.IM_USERONLINE.SrcPort
	is 'Դ�˿�';
comment on column im.IM_USERONLINE.ServerNo
	is '���������';
comment on column im.IM_USERONLINE.Status
	is '����״̬(0-���ߣ�1-����)';
comment on column im.IM_USERONLINE.CreateTime
	is '��¼ʱ��';

create unique index im.IM_USERONLINE_IDX_USERID on im.IM_USERONLINE (UserId) tablespace ECSINDEX;
create index im.IM_USERONLINE_IDX_CREATETIME on im.IM_USERONLINE (CreateTime) tablespace ECSINDEX;
create index im.IM_USERONLINE_IDX_SERVERNO on im.IM_USERONLINE (ServerNo,Status) tablespace ECSINDEX;

prompt
prompt Creating table IM_OFFLINETALKMSG_ARC
prompt ====================================
prompt
create table im.IM_OFFLINETALKMSG_ARC
(
  MsgId              number(20) default 0 not null,          --��ϢID
  Receiver           number(20) default 0 not null,          --������ID
  Sender             number(20) default 0 not null,          --������ID
  SrcIp              number(10) default 0 not null,          --ԴIP
  SrcPort            number(6) default 0 not null,           --Դ�˿�
  SendTime           date default sysdate not null,          --����ʱ��
  MsgType            number(6) default 0 not null,           --��Ϣ����
  MsgLen             number(10) default 0 not null,          --��Ϣ����
  Msg                blob,                                   --��Ϣ����
  ReceiverName       varchar2(100),                          --�������û�NickName
  SenderName         varchar2(100),                          --�������û�NickName
  ReadTime           date,                                   --��ȡʱ��
  Status             number(2) default 0 not null,           --��ȡ״̬��0-δ�� 1-�Ѷ�
  Text               varchar2(1024) not null                 --��Ϣ����
)tablespace ECSDATA;

comment on column im.IM_OFFLINETALKMSG_ARC.MsgId
	is '��ϢID';
comment on column im.IM_OFFLINETALKMSG_ARC.Receiver
	is '������ID';
comment on column im.IM_OFFLINETALKMSG_ARC.Sender
	is '������ID';
comment on column im.IM_OFFLINETALKMSG_ARC.SrcIp
	is 'ԴIP';
comment on column im.IM_OFFLINETALKMSG_ARC.SrcPort
	is 'Դ�˿�';
comment on column im.IM_OFFLINETALKMSG_ARC.SendTime
	is '����ʱ��';
comment on column im.IM_OFFLINETALKMSG_ARC.MsgType
	is '��Ϣ����';
comment on column im.IM_OFFLINETALKMSG_ARC.MsgLen
	is '��Ϣ����';
comment on column im.IM_OFFLINETALKMSG_ARC.Msg
	is '��Ϣ����';
comment on column im.IM_OFFLINETALKMSG_ARC.ReceiverName
	is '�������û�';
comment on column im.IM_OFFLINETALKMSG_ARC.SenderName
	is '�������û�';
comment on column im.IM_OFFLINETALKMSG_ARC.ReadTime
	is '��ȡʱ��';
comment on column im.IM_OFFLINETALKMSG_ARC.Status
	is '��ȡ״̬(0-δ����1-�Ѷ�)';
comment on column im.IM_OFFLINETALKMSG_ARC.Text
	is '��Ϣ����';

prompt
prompt Creating table IM_CURCLILIST
prompt ====================================
prompt
create table im.IM_CURCLILIST
(
  SELLERID     number(20) not null,  
  USERNAME     VARCHAR2(80) not null,
  LASTTIME     DATE not null,
  ORDERID      VARCHAR2(64) default '',
  REMARKNAME   VARCHAR2(128) default ''
) tablespace ECSDATA;

comment on column im.IM_CURCLILIST.SELLERID
	is '����ID';
comment on column im.IM_CURCLILIST.USERNAME
	is '����û���';
comment on column im.IM_CURCLILIST.LASTTIME
	is '�����ϵʱ��';
comment on column im.IM_CURCLILIST.ORDERID
	is '����ID';
comment on column im.IM_CURCLILIST.REMARKNAME
	is '��ע';

create index im.IM_CURCLILIST_IDX_ID on im.IM_CURCLILIST (SELLERID) tablespace ECSINDEX;

prompt
prompt Creating table IM_WEBDATA
prompt ====================================
prompt
create table im.IM_WEBDATA
(
	FROMID number(20) not null,
	TOID number(20) not null,
	DATATYPE number(3) not null,
	VERSION number(10) not null,
	SENDTIME number(20) not null,
	RECVFLAG number(3) not null,
	FONTSIZE number(3) not null,
	FONTCOLOR number(20) not null,
	FONTSTYLE number(10) not null,
	FONTNAME VARCHAR2(50) not null,
	FROMNICKNAME VARCHAR2(80) not null,
	MSG VARCHAR2(1315) not null,
	MSGID number(20) not null,
	LOGINTIME number(20) 
)tablespace ECSDATA;

comment on column im.IM_WEBDATA.FROMID
	is '���ͷ�ID';
comment on column im.IM_WEBDATA.TOID
	is '���շ�ID';
comment on column im.IM_WEBDATA.DATATYPE
	is '��������';
comment on column im.IM_WEBDATA.VERSION
	is '���ݰ汾';
comment on column im.IM_WEBDATA.SENDTIME
	is '����ʱ��';
comment on column im.IM_WEBDATA.RECVFLAG
	is '���ձ�־';
comment on column im.IM_WEBDATA.FONTSIZE
	is '�����С';
comment on column im.IM_WEBDATA.FONTCOLOR
	is '������ɫ';
comment on column im.IM_WEBDATA.FONTSTYLE
	is '������ʽ';
comment on column im.IM_WEBDATA.FONTNAME
	is '��������';
comment on column im.IM_WEBDATA.FROMNICKNAME
	is '���ͷ��ǳ�';
comment on column im.IM_WEBDATA.MSG
	is '��������';
comment on column im.IM_WEBDATA.MSGID
	is '��Ϣ����';
comment on column im.IM_WEBDATA.LOGINTIME
	is '��ҵ�¼ʱ��';

create index im.IM_WEBDATA_IDX_ID on im.IM_WEBDATA (FROMID,TOID) tablespace ECSINDEX;

prompt
prompt Creating table IM_WEBLIST
prompt ====================================
prompt
create table im.IM_WEBLIST
(
	FROMID number(20) not null,
	TOID number(20) not null,
	SENDTIME number(20) not null
)tablespace ECSDATA;

comment on column im.IM_WEBLIST.FROMID
	is '���ͷ�ID';
comment on column im.IM_WEBLIST.TOID
	is '���շ�ID';
comment on column im.IM_WEBLIST.SENDTIME
	is '�����ϵʱ��';

create index im.IM_WEBLIST_IDX_ID on im.IM_WEBLIST (FROMID,TOID) tablespace ECSINDEX;


-- ----------------------------
-- 2015-06-26 :����IM_REPLAY_DETAIL��
-- ----------------------------

CREATE TABLE "IM"."IM_REPLAY_DETAIL" (
  "FROMID" NUMBER(20) NOT NULL ,
  "TOID" NUMBER(20) NOT NULL ,
  "ASKTIME" NUMBER NULL ,
  "REPLYTIME" NUMBER NULL ,
  "DUR" NUMBER NULL
) TABLESPACE ecsdata;

COMMENT ON TABLE "IM"."IM_REPLAY_DETAIL"
IS '�̼ҽӴ��ͻ���ͳ�Ʊ�';
COMMENT ON COLUMN IM.IM_REPLAY_DETAIL.FROMID
IS '�ͻ�ID';
COMMENT ON COLUMN IM.IM_REPLAY_DETAIL.TOID
IS '�̼����˺�ID';
COMMENT ON COLUMN IM.IM_REPLAY_DETAIL.ASKTIME
IS '�ͻ�ѯ��ʱ��';
COMMENT ON COLUMN IM.IM_REPLAY_DETAIL.REPLYTIME
IS '�̼����˺Żظ�ʱ��';
COMMENT ON COLUMN IM.IM_REPLAY_DETAIL.DUR
IS '�̼���Ӧʱ��';
-- ----------------------------
-- Indexes structure for table IM_REPLAY_DETAIL
-- ----------------------------
CREATE INDEX "IM"."REPLY_FROM_TO"
ON "IM"."IM_REPLAY_DETAIL" ("FROMID" ASC, "TOID" ASC)
 TABLESPACE ecsdata;
-- ----------------------------
-- Checks structure for table IM_REPLAY_DETAIL
-- ----------------------------
ALTER TABLE "IM"."IM_REPLAY_DETAIL" ADD CHECK ("FROMID" IS NOT NULL);
ALTER TABLE "IM"."IM_REPLAY_DETAIL" ADD CHECK ("TOID" IS NOT NULL);


-- ----------------------------
-- Table structure for IM_WORK_DETAIL
-- ----------------------------

CREATE TABLE "IM"."IM_WORK_DETAIL" (
"MERCHANT_ID" NUMBER(20) NOT NULL ,
"STATISTICS_DATE" NUMBER(20) NOT NULL ,
"LANDING_TIME" FLOAT(10) NOT NULL ,
"SERVE_COUNT" NUMBER(10) NOT NULL ,
"RESPONSE_TIME" NUMBER(10) NOT NULL ,
"RESPONSE_IN_90S_COUNT" NUMBER(10) NOT NULL ,
"RESPONSE_COUNT" NUMBER(10) NOT NULL ,
"SUB_MERCHANT_IDS" VARCHAR2(800 BYTE) NULL ,
"CUSTOMER_IDS" VARCHAR2(1600 BYTE) NULL 
) TABLESPACE ecsdata;

COMMENT ON TABLE "IM"."IM_WORK_DETAIL" IS '�̼ҹ�����ͳ�Ʊ���';
COMMENT ON COLUMN "IM"."IM_WORK_DETAIL"."MERCHANT_ID" IS '�̼�ID';
COMMENT ON COLUMN "IM"."IM_WORK_DETAIL"."STATISTICS_DATE" IS 'ͳ�Ƶ�ʱ�䣬Ϊ�����0��0��0��';
COMMENT ON COLUMN "IM"."IM_WORK_DETAIL"."LANDING_TIME" IS '�̼��������˺ŵ�¼ʱ��';
COMMENT ON COLUMN "IM"."IM_WORK_DETAIL"."SERVE_COUNT" IS '�̼ҿͷ�����';
COMMENT ON COLUMN "IM"."IM_WORK_DETAIL"."RESPONSE_TIME" IS '������̼����пͷ���������Ӧʱ���ܺ�';
COMMENT ON COLUMN "IM"."IM_WORK_DETAIL"."RESPONSE_IN_90S_COUNT" IS '90������Ӧ�Ĵ���';
COMMENT ON COLUMN "IM"."IM_WORK_DETAIL"."RESPONSE_COUNT" IS '������̼����пͷ��Ļظ�����';
COMMENT ON COLUMN "IM"."IM_WORK_DETAIL"."SUB_MERCHANT_IDS" IS '������̼����пͷ���ID�б�';
COMMENT ON COLUMN "IM"."IM_WORK_DETAIL"."CUSTOMER_IDS" IS '������̼����пͻ���ID�б�';

-- ----------------------------
-- Indexes structure for table IM_WORK_DETAIL
-- ----------------------------
CREATE INDEX "IM"."merchantid"
ON "IM"."IM_WORK_DETAIL" ("MERCHANT_ID" ASC)
 TABLESPACE ecsdata;
-- ----------------------------
-- Checks structure for table IM_WORK_DETAIL
-- ----------------------------
ALTER TABLE "IM"."IM_WORK_DETAIL" ADD CHECK ("MERCHANT_ID" IS NOT NULL);
ALTER TABLE "IM"."IM_WORK_DETAIL" ADD CHECK ("STATISTICS_DATE" IS NOT NULL);
ALTER TABLE "IM"."IM_WORK_DETAIL" ADD CHECK ("LANDING_TIME" IS NOT NULL);
ALTER TABLE "IM"."IM_WORK_DETAIL" ADD CHECK ("SERVE_COUNT" IS NOT NULL);
ALTER TABLE "IM"."IM_WORK_DETAIL" ADD CHECK ("RESPONSE_TIME" IS NOT NULL);
ALTER TABLE "IM"."IM_WORK_DETAIL" ADD CHECK ("RESPONSE_IN_90S_COUNT" IS NOT NULL);
ALTER TABLE "IM"."IM_WORK_DETAIL" ADD CHECK ("RESPONSE_COUNT" IS NOT NULL);
ALTER TABLE "IM"."IM_WORK_DETAIL" ADD CHECK ("MERCHANT_ID" IS NOT NULL);
ALTER TABLE "IM"."IM_WORK_DETAIL" ADD CHECK ("STATISTICS_DATE" IS NOT NULL);
ALTER TABLE "IM"."IM_WORK_DETAIL" ADD CHECK ("LANDING_TIME" IS NOT NULL);
ALTER TABLE "IM"."IM_WORK_DETAIL" ADD CHECK ("SERVE_COUNT" IS NOT NULL);
ALTER TABLE "IM"."IM_WORK_DETAIL" ADD CHECK ("RESPONSE_TIME" IS NOT NULL);
ALTER TABLE "IM"."IM_WORK_DETAIL" ADD CHECK ("RESPONSE_IN_90S_COUNT" IS NOT NULL);
ALTER TABLE "IM"."IM_WORK_DETAIL" ADD CHECK ("RESPONSE_COUNT" IS NOT NULL);

-- ----------------------------
-- Primary Key structure for table IM_WORK_DETAIL
-- ----------------------------
ALTER TABLE "IM"."IM_WORK_DETAIL" ADD PRIMARY KEY ("MERCHANT_ID", "STATISTICS_DATE");



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



COMMENT ON TABLE "IM"."IM_STATISITC_STATUS" IS 'ͳ��״̬�����е�ͳ��������Ҫ�������ű�����ݣ������������';
COMMENT ON COLUMN "IM"."IM_STATISITC_STATUS"."STATISITC_TIME" IS 'ͳ�Ƶ�ʱ�䣬Ϊ�����0��0��0��';
COMMENT ON COLUMN "IM"."IM_STATISITC_STATUS"."STATISITC_TYPE" IS 'ͳ�Ƶ����ͣ�Ŀǰ����Ӫ��̨ͳ�ƺͿͷ���Чͳ��';
COMMENT ON COLUMN "IM"."IM_STATISITC_STATUS"."STATISITC_STATUS" IS 'ͳ�Ƶ�״̬��δͳ�ƣ�ͳ��ʧ�ܣ��Ѿ�ͳ�ƣ�����ͳ����';
COMMENT ON COLUMN "IM"."IM_STATISITC_STATUS"."RETRY_TIME" IS '���ԵĴ���';
COMMENT ON COLUMN "IM"."IM_STATISITC_STATUS"."TRY_IP" IS '��һ��ͳ��ִ�е�IP��ַ';



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


COMMENT ON TABLE "IM"."T_SESSION_DETAIL" IS '�ظ���ϸ��:��ʱ�����ڴ��ͳ�Ƴ����ĻỰ������';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_FROMID" IS '�Ự���̼ҵ�ID';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_TOID" IS '�Ự����ҵ�ID';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_LOGINTIME" IS '�Ự��ʾ';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_ASKTIME" IS '��ҷ��͵ĵ�һ����Ϣ��ʱ��';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_REPLYTIME" IS '���ҷ��͵ĵ�һ����Ϣ��ʱ��';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_DUR" IS '���һظ�����м��ʱ���';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_MESSAGE_COUNT" IS '�Ự������һ���ظ�����Ϣ��';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_START_TIME" IS '�Ự��ʼ��ʱ��';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_END_TIME" IS '�Ự������ʱ��';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_SESSION_LEN" IS '�Ự��ʱ��';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_REPLY_START" IS '�̼�һ�λظ���ʱ��';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_REPLY_END" IS '�̼����һ�λظ���ʱ��';
COMMENT ON COLUMN "IM"."T_SESSION_DETAIL"."I_REPLY_LEN" IS '�̼ҵ�һ�λظ������һ�λظ���ʱ���';


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
"I_TOTAL_LEAVE_NUM" NUMBER(6) DEFAULT 0  NULL ,
"I_TOTAL_LEAVE_DURATION" NUMBER(6) DEFAULT 0  NULL ,
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



COMMENT ON TABLE "IM"."IM_SUBMERCHANT_ATTENDANCE" IS '������ͳ��';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_ATTENDANCE"."I_SUBMERCHANT_ID" IS '���˺ŵ�ID';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_ATTENDANCE"."I_MERCHANT_ID" IS '�̼ҵ�ID';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_ATTENDANCE"."I_STATISTICS_TIME" IS 'ͳ��ʱ�䣺�����0��0��0��';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_ATTENDANCE"."I_LOGIN_COUNT" IS '���˺ŵ����½�Ĵ���';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_ATTENDANCE"."I_FIRST_ONLINE_TIME" IS '���˺ŵ����һ�ε�½��ʱ��';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_ATTENDANCE"."I_LAST_ONLINE_TIME" IS '���˺ŵ������һ�ε�½��ʱ��';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_ATTENDANCE"."I_TOTAL_ONLINE_DURATION" IS '���˺ŵ������ߵ���ʱ��';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_ATTENDANCE"."I_TOTAL_HANGING_DURATION" IS '���˺ŵ���������ʱ��';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_ATTENDANCE"."I_TOTAL_OFFLINE_DURATION" IS '���˺ŵ������ߵ���ʱ��';


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



COMMENT ON TABLE "IM"."IM_SUBMERCHANT_WORKLOAD" IS '������ͳ��';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_WORKLOAD"."I_SUBMERCHANT_ID" IS '���˺ŵ�ID';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_WORKLOAD"."I_MERCHANT_ID" IS '�̼ҵ�ID';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_WORKLOAD"."I_STATISTICS_TIME" IS 'ͳ��ʱ�䣺�����0��0��0��';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_WORKLOAD"."I_RESPONSE_DURATION" IS '���˺���Ӧ�ͻ���ʱ���ܺ�';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_WORKLOAD"."I_SESSION_DURATION" IS '���˺����лỰ��ʱ���ܺ�';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_WORKLOAD"."I_SESSION_COUNT" IS '���˺����лỰ�ĸ���';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_WORKLOAD"."I_RESPONSE_MESSAGE_COUNT" IS '���˺����лظ�����Ϣ����';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_WORKLOAD"."I_RECEPTION_DURATION" IS '���˺ŽӴ���ʱ��';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_WORKLOAD"."V_RESPONSE_IDS" IS '���˺Żظ��������пͻ���ID';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_WORKLOAD"."V_ASKING_IDS" IS '������ѯ�������˺ŵĿͻ���ID';
COMMENT ON COLUMN "IM"."IM_SUBMERCHANT_WORKLOAD"."I_RESPONSE_COUNT" IS '���˺Żظ����Ĵ���';


prompt
prompt Creating table im.IM_LOGINOUTTIME
prompt ===============================
prompt
create table im.IM_LOGINOUTTIME
(
    USERID         NUMBER(20) not null,
    LOGINTIME      NUMBER(20) default 0,
    LOGOUTTIME     NUMBER(20) default 0
)tablespace ECSDATA
;

comment on column im.IM_LOGINOUTTIME.UserId 
	is '�û�ID';
comment on column im.IM_LOGINOUTTIME.LOGINTIME 
	is '��¼ʱ��';
comment on column im.IM_LOGINOUTTIME.LOGOUTTIME 
	is '�ǳ�ʱ��';

create index im.IM_LOGINOUTTIME_IDX_USERID on im.IM_LOGINOUTTIME (UserId) tablespace ECSINDEX;





prompt
prompt Creating table IM_WEBRECOMMENT
prompt ====================================
prompt
create table im.IM_WEBRECOMMENT
(
	SEQID  number(20) primary key,
	FROMID number(20) not null,
	TOID number(20) not null,
	SENDTIME number(20) not null,
	RECOMMENT number(2) default 0 not null,
	V_RECOMMENT_TEXT VARCHAR2(1315) null,
	I_SESSION_ID number(20) null
)tablespace ECSDATA;

comment on column im.IM_WEBRECOMMENT.SEQID
	is '����';
comment on column im.IM_WEBRECOMMENT.FROMID
	is '���շ�ID';
comment on column im.IM_WEBRECOMMENT.TOID
	is '���ͷ�ID';
comment on column im.IM_WEBRECOMMENT.SENDTIME
	is '����ʱ��';
comment on column im.IM_WEBRECOMMENT.RECOMMENT
	is '�����';

prompt
prompt Creating sequence IM_CLIENTMSGINFO_SEQ_RECID
prompt ============================================
prompt
create sequence im.IM_CLIENTMSGINFO_SEQ_RECID
minvalue 1
maxvalue 99999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence IM_CLUSTERGROUP_SEQ_GROUPID
prompt =============================================
prompt
create sequence im.IM_CLUSTERGROUP_SEQ_GROUPID
minvalue 1
maxvalue 99999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence IM_CLUSTERSYSMSG_SEQ_MSGID
prompt ============================================
prompt
create sequence im.IM_CLUSTERSYSMSG_SEQ_MSGID
minvalue 1
maxvalue 99999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence IM_CLUSTER_SEQ_CLUSTERID
prompt ==========================================
prompt
create sequence im.IM_CLUSTER_SEQ_CLUSTERID
minvalue 1
maxvalue 99999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence IM_OFFLINESYSMSG_SEQ_MSGID
prompt ============================================
prompt
create sequence im.IM_OFFLINESYSMSG_SEQ_MSGID
minvalue 1
maxvalue 99999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence IM_OFFLINETALKMSG_SEQ_MSGID
prompt =============================================
prompt
create sequence im.IM_OFFLINETALKMSG_SEQ_MSGID
minvalue 1
maxvalue 99999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence IM_SYSMESSAGE_SEQ_MSGID
prompt =========================================
prompt
create sequence im.IM_SYSMESSAGE_SEQ_MSGID
minvalue 1
maxvalue 99999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence IM_SYSTEMMSG_SEQ_MSGID
prompt ========================================
prompt
create sequence im.IM_SYSTEMMSG_SEQ_MSGID
minvalue 1
maxvalue 99999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence IM_USER_SEQ_USERID
prompt ========================================
prompt
CREATE SEQUENCE  im.IM_USER_SEQ_USERID  
MINVALUE 5000000 
MAXVALUE 9999999999999999999999999999 
INCREMENT BY 1 
START WITH 5000001 
CACHE 20 
ORDER  NOCYCLE;

prompt
prompt Creating sequence IM_WORK_SEQ_USERID
prompt ========================================
prompt
CREATE SEQUENCE  im.IM_WORK_SEQ_USERID  
MINVALUE 2050000000 
MAXVALUE 9999999999999999999999999999 
INCREMENT BY 1 
START WITH 2050000001 
CACHE 20 
ORDER  NOCYCLE;

prompt
prompt Creating sequence IM_WEBDATA_SEQ_MSGID
prompt ========================================
prompt
CREATE SEQUENCE  im.IM_WEBDATA_SEQ_MSGID  
MINVALUE 1 
MAXVALUE 9999999999999999999999999999 
INCREMENT BY 1 
START WITH 1 
CACHE 20;

prompt
prompt Creating sequence IM_WEBRECOMMENT_SEQ_MSGID
prompt ========================================
prompt
CREATE SEQUENCE  im.IM_WEBRECOMMENT_SEQ_MSGID  
MINVALUE 1 
MAXVALUE 9999999999999999999999999999 
INCREMENT BY 1 
START WITH 1 
CACHE 20;

spool off



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

COMMENT ON TABLE "IM"."IM_RECEPTION_MAP" IS '�ͷ��Ӵ����ͳ�Ʊ�';
COMMENT ON COLUMN "IM"."IM_RECEPTION_MAP"."I_SUBMERCHANT_ID" IS '���˺ŵ�ID';
COMMENT ON COLUMN "IM"."IM_RECEPTION_MAP"."I_MERCHANT_ID" IS '�̼ҵ�ID';
COMMENT ON COLUMN "IM"."IM_RECEPTION_MAP"."I_STATISITC_TIME" IS 'ͳ��ʱ�䣺�����0��0��0��';
COMMENT ON COLUMN "IM"."IM_RECEPTION_MAP"."I_COUSTOMER_ID" IS '�Ӵ��Ŀͻ�ID';
COMMENT ON COLUMN "IM"."IM_RECEPTION_MAP"."I_REPLY" IS '�Ӵ��Ĵ���';
COMMENT ON COLUMN "IM"."IM_RECEPTION_MAP"."I_NOT_REPLY" IS 'û�Ӵ��Ĵ���';
COMMENT ON COLUMN "IM"."IM_RECEPTION_MAP"."UPDATE_TIME" IS '���ݸ���ʱ��';

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

COMMENT ON TABLE "IM"."IM_MERCHANT_LANDING_DETAIL" IS '�̼ҹ�����ͳ�Ʊ���';
COMMENT ON COLUMN "IM"."IM_MERCHANT_LANDING_DETAIL"."MERCHANT_ID" IS '�̼�ID';
COMMENT ON COLUMN "IM"."IM_MERCHANT_LANDING_DETAIL"."STATISTICS_DATE" IS 'ͳ�Ƶ�ʱ�䣬Ϊ�����0��0��0��';
COMMENT ON COLUMN "IM"."IM_MERCHANT_LANDING_DETAIL"."LANDING_TIME" IS '�̼��������˺ŵ�¼ʱ��';
COMMENT ON COLUMN "IM"."IM_MERCHANT_LANDING_DETAIL"."UPDATE_TIME" IS '���ݸ���ʱ��';

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

-- Create table
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
  update_time    TIMESTAMP(6) default systimestamp,
  EMBED_SET 		 VARCHAR(40)
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
  is '�ͷ���id';
comment on column IM.IM_CSGROUP.groupname
  is '�ͷ�������';
comment on column IM.IM_CSGROUP.groupdescrib
  is '������';
comment on column IM.IM_CSGROUP.lastmodifyname
  is '����޸�������';
comment on column IM.IM_CSGROUP.lastmodifytime
  is '����޸�ʱ��';
comment on column IM.IM_CSGROUP.insert_time
  is '����ʱ��';
comment on column IM.IM_CSGROUP.update_time
  is '����ʱ��';
comment on column IM.IM_CSGROUP.EMBED_SET
  is '����ҳ������';
  

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
  is '�ͷ�id';
comment on column IM.IM_CSGROUPMEM.groupid
  is '�ͷ���id';
comment on column IM.IM_CSGROUPMEM.flowstatus
  is '�ͷ��Ӵ�״̬';
comment on column IM.IM_CSGROUPMEM.insert_time
  is '�ͷ�����ʱ��';
comment on column IM.IM_CSGROUPMEM.update_time
  is '�ͷ�����ʱ��';

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
  is '�ͷ�id';
comment on column IM.IM_CSINFO.csid
  is '�ͷ�csid';
comment on column IM.IM_CSINFO.username
  is '�ͷ��û���';
comment on column IM.IM_CSINFO.permission
  is '�ͷ�Ȩ��';
comment on column IM.IM_CSINFO.importname
  is '�ͷ�������Ա����';
comment on column IM.IM_CSINFO.importtime
  is '�ͷ�����ʱ��';
comment on column IM.IM_CSINFO.receptionlimit
  is '�ͷ��Ӵ�����';
comment on column IM.IM_CSINFO.realname
  is '�ͷ���ʵ����';
comment on column IM.IM_CSINFO.status
  is '�ͷ�״̬';
comment on column IM.IM_CSINFO.groupname
  is '�ͷ�����������';
comment on column IM.IM_CSINFO.insert_time
  is '����ʱ��';
comment on column IM.IM_CSINFO.update_time
  is '����ʱ��';




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
  is '�ͷ���id';
comment on column IM.IM_CSGROUP.groupname
  is '�ͷ�������';
comment on column IM.IM_CSGROUP.groupdescrib
  is '������';
comment on column IM.IM_CSGROUP.lastmodifyname
  is '����޸�������';
comment on column IM.IM_CSGROUP.lastmodifytime
  is '����޸�ʱ��';
comment on column IM.IM_CSGROUP.insert_time
  is '����ʱ��';
comment on column IM.IM_CSGROUP.update_time
  is '����ʱ��';

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
  is '�ͷ�id';
comment on column IM.IM_CSGROUPMEM.groupid
  is '�ͷ���id';
comment on column IM.IM_CSGROUPMEM.flowstatus
  is '�ͷ��Ӵ�״̬';
comment on column IM.IM_CSGROUPMEM.insert_time
  is '�ͷ�����ʱ��';
comment on column IM.IM_CSGROUPMEM.update_time
  is '�ͷ�����ʱ��';

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
  is '�ͷ�id';
comment on column IM.IM_CSINFO.csid
  is '�ͷ�csid';
comment on column IM.IM_CSINFO.username
  is '�ͷ��û���';
comment on column IM.IM_CSINFO.permission
  is '�ͷ�Ȩ��';
comment on column IM.IM_CSINFO.importname
  is '�ͷ�������Ա����';
comment on column IM.IM_CSINFO.importtime
  is '�ͷ�����ʱ��';
comment on column IM.IM_CSINFO.receptionlimit
  is '�ͷ��Ӵ�����';
comment on column IM.IM_CSINFO.realname
  is '�ͷ���ʵ����';
comment on column IM.IM_CSINFO.status
  is '�ͷ�״̬';
comment on column IM.IM_CSINFO.groupname
  is '�ͷ�����������';
comment on column IM.IM_CSINFO.insert_time
  is '����ʱ��';
comment on column IM.IM_CSINFO.update_time
  is '����ʱ��';
  
  
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

Date: 2016-04-07 14:00:35
*/


-- ----------------------------
-- Table structure for IM_CSSESSION_INFO
-- ----------------------------

CREATE TABLE "IM"."IM_CSSESSION_INFO" (
"I_SESSION_ID" NUMBER(20) NOT NULL PRIMARY KEY, 
"I_CUSTOMER_ID" NUMBER(20) NOT NULL ,
"I_CUSTOMER_LEVEL" NUMBER(20) NOT NULL ,
"I_SERVICE_ID" NUMBER(20) NOT NULL ,
"I_SESSION_START_TIME" NUMBER(20) NOT NULL ,
"I_SESSION_END_TIME" NUMBER(20) NULL ,
"I_WAITING_DURATION" NUMBER(20) NULL ,
"I_FROM_DEVICE" NUMBER(3) NULL ,
"I_FROM_PAGE" NUMBER(5) NULL ,
"UPDATE_TIME" TIMESTAMP(6)  DEFAULT systimestamp  NOT NULL 
)
TABLESPACE ecsdata;
-- ----------------------------
-- Checks structure for table IM_CSSESSION_INFO
-- ----------------------------
ALTER TABLE "IM"."IM_CSSESSION_INFO" ADD CHECK ("I_SESSION_ID" IS NOT NULL);
ALTER TABLE "IM"."IM_CSSESSION_INFO" ADD CHECK ("I_CUSTOMER_ID" IS NOT NULL);
ALTER TABLE "IM"."IM_CSSESSION_INFO" ADD CHECK ("I_SERVICE_ID" IS NOT NULL);
ALTER TABLE "IM"."IM_CSSESSION_INFO" ADD CHECK ("I_SESSION_START_TIME" IS NOT NULL);



CREATE SEQUENCE IM.SEQ_IM_CSSESSION_INFO 
INCREMENT BY 1 
START WITH 1 
NOMAXVALUE 
NOCYCLE
NOCACHE ;

COMMENT ON TABLE "IM"."IM_CSSESSION_INFO" IS '�ͷ��Ự��Ϣ��';
COMMENT ON COLUMN "IM"."IM_CSSESSION_INFO"."I_SESSION_ID" IS 'session ID';
COMMENT ON COLUMN "IM"."IM_CSSESSION_INFO"."I_CUSTOMER_ID" IS '�ͻ�ID';
COMMENT ON COLUMN "IM"."IM_CSSESSION_INFO"."I_CUSTOMER_LEVEL" IS '�ͻ��ȼ�';
COMMENT ON COLUMN "IM"."IM_CSSESSION_INFO"."I_SERVICE_ID" IS '�ͷ�ID';
COMMENT ON COLUMN "IM"."IM_CSSESSION_INFO"."I_SESSION_START_TIME" IS '�Ự��ʼʱ��';
COMMENT ON COLUMN "IM"."IM_CSSESSION_INFO"."I_SESSION_END_TIME" IS '�Ự����ʱ��';
COMMENT ON COLUMN "IM"."IM_CSSESSION_INFO"."I_WAITING_DURATION" IS '�ȴ�ʱ��';
COMMENT ON COLUMN "IM"."IM_CSSESSION_INFO"."I_FROM_DEVICE" IS '�豸����';
COMMENT ON COLUMN "IM"."IM_CSSESSION_INFO"."I_FROM_PAGE" IS '�������';


/*
Navicat Oracle Data Transfer
Oracle Client Version : 10.2.0.5.0

Source Server         : wh-oracle01.fn.com_1521_ORCL
Source Server Version : 110200
Source Host           : wh-oracle01.fn.com:1521
Source Schema         : IM

Target Server Type    : ORACLE
Target Server Version : 110200
File Encoding         : 65001

Date: 2016-04-22 09:57:50
*/


-- ----------------------------
-- Table structure for T_TOUCH_PORTAL_BOARD_CONFIG
-- ----------------------------
CREATE TABLE "IM"."T_TOUCH_PORTAL_BOARD_CONFIG" (
"I_ID" NUMBER(10) NOT NULL ,
"V_TITLE" VARCHAR2(60 BYTE) NOT NULL ,
"V_DESCRIPTION" VARCHAR2(120 BYTE) NOT NULL ,
"V_ICON" VARCHAR2(300 BYTE) NOT NULL ,
"I_SHOW_ORDER" NUMBER(4) NOT NULL ,
"C_IS_BIND_GROUP" CHAR(1 BYTE) NOT NULL ,
"I_GROUP_ID" NUMBER(10) NULL ,
"C_IS_BIND_USER_ORDER" CHAR(1 BYTE) NOT NULL ,
"C_DATA_TYPE" CHAR(1 BYTE) NOT NULL ,
"UPDATE_TIME" TIMESTAMP(6)  DEFAULT systimestamp  NULL 
)
TABLESPACE ecsdata;

COMMENT ON TABLE "IM"."T_TOUCH_PORTAL_BOARD_CONFIG" IS '����portalҳ��������';
COMMENT ON COLUMN "IM"."T_TOUCH_PORTAL_BOARD_CONFIG"."I_ID" IS '����������';
COMMENT ON COLUMN "IM"."T_TOUCH_PORTAL_BOARD_CONFIG"."V_TITLE" IS '�������';
COMMENT ON COLUMN "IM"."T_TOUCH_PORTAL_BOARD_CONFIG"."V_DESCRIPTION" IS '�������';
COMMENT ON COLUMN "IM"."T_TOUCH_PORTAL_BOARD_CONFIG"."V_ICON" IS '���ͼ��';
COMMENT ON COLUMN "IM"."T_TOUCH_PORTAL_BOARD_CONFIG"."I_SHOW_ORDER" IS '��ʾ˳��';
COMMENT ON COLUMN "IM"."T_TOUCH_PORTAL_BOARD_CONFIG"."C_IS_BIND_GROUP" IS '�Ƿ�󶨿ͷ�����';
COMMENT ON COLUMN "IM"."T_TOUCH_PORTAL_BOARD_CONFIG"."I_GROUP_ID" IS '�ͷ�����id';
COMMENT ON COLUMN "IM"."T_TOUCH_PORTAL_BOARD_CONFIG"."C_IS_BIND_USER_ORDER" IS '�Ƿ�󶨶���';
COMMENT ON COLUMN "IM"."T_TOUCH_PORTAL_BOARD_CONFIG"."C_DATA_TYPE" IS '����������ͣ����ʱ������ʱ';
COMMENT ON COLUMN "IM"."T_TOUCH_PORTAL_BOARD_CONFIG"."UPDATE_TIME" IS '������ʱ��';

-- ----------------------------
-- Indexes structure for table T_TOUCH_PORTAL_BOARD_CONFIG
-- ----------------------------

-- ----------------------------
-- Checks structure for table T_TOUCH_PORTAL_BOARD_CONFIG
-- ----------------------------
ALTER TABLE "IM"."T_TOUCH_PORTAL_BOARD_CONFIG" ADD CHECK ("I_ID" IS NOT NULL);
ALTER TABLE "IM"."T_TOUCH_PORTAL_BOARD_CONFIG" ADD CHECK ("V_TITLE" IS NOT NULL);
ALTER TABLE "IM"."T_TOUCH_PORTAL_BOARD_CONFIG" ADD CHECK ("V_DESCRIPTION" IS NOT NULL);
ALTER TABLE "IM"."T_TOUCH_PORTAL_BOARD_CONFIG" ADD CHECK ("V_ICON" IS NOT NULL);
ALTER TABLE "IM"."T_TOUCH_PORTAL_BOARD_CONFIG" ADD CHECK ("I_SHOW_ORDER" IS NOT NULL);
ALTER TABLE "IM"."T_TOUCH_PORTAL_BOARD_CONFIG" ADD CHECK ("C_IS_BIND_GROUP" IS NOT NULL);
ALTER TABLE "IM"."T_TOUCH_PORTAL_BOARD_CONFIG" ADD CHECK ("C_IS_BIND_USER_ORDER" IS NOT NULL);
ALTER TABLE "IM"."T_TOUCH_PORTAL_BOARD_CONFIG" ADD CHECK ("C_DATA_TYPE" IS NOT NULL);
ALTER TABLE "IM"."T_TOUCH_PORTAL_BOARD_CONFIG" ADD CHECK ("I_ID" IS NOT NULL);
ALTER TABLE "IM"."T_TOUCH_PORTAL_BOARD_CONFIG" ADD CHECK ("V_TITLE" IS NOT NULL);
ALTER TABLE "IM"."T_TOUCH_PORTAL_BOARD_CONFIG" ADD CHECK ("V_DESCRIPTION" IS NOT NULL);
ALTER TABLE "IM"."T_TOUCH_PORTAL_BOARD_CONFIG" ADD CHECK ("V_ICON" IS NOT NULL);
ALTER TABLE "IM"."T_TOUCH_PORTAL_BOARD_CONFIG" ADD CHECK ("I_SHOW_ORDER" IS NOT NULL);
ALTER TABLE "IM"."T_TOUCH_PORTAL_BOARD_CONFIG" ADD CHECK ("C_IS_BIND_GROUP" IS NOT NULL);
ALTER TABLE "IM"."T_TOUCH_PORTAL_BOARD_CONFIG" ADD CHECK ("C_IS_BIND_USER_ORDER" IS NOT NULL);
ALTER TABLE "IM"."T_TOUCH_PORTAL_BOARD_CONFIG" ADD CHECK ("C_DATA_TYPE" IS NOT NULL);
ALTER TABLE "IM"."T_TOUCH_PORTAL_BOARD_CONFIG" ADD CHECK (c_data_type IN ('0', '1'));
ALTER TABLE "IM"."T_TOUCH_PORTAL_BOARD_CONFIG" ADD CHECK (c_is_bind_group IN ('0', '1'));
ALTER TABLE "IM"."T_TOUCH_PORTAL_BOARD_CONFIG" ADD CHECK (c_is_bind_user_order IN ('0', '1'));

-- ----------------------------
-- Primary Key structure for table T_TOUCH_PORTAL_BOARD_CONFIG
-- ----------------------------
ALTER TABLE "IM"."T_TOUCH_PORTAL_BOARD_CONFIG" ADD PRIMARY KEY ("I_ID");

CREATE SEQUENCE "IM"."SEQ_TOUCH_PORTAL_BOARD_CONFIG" MINVALUE 1 MAXVALUE 1000000 INCREMENT BY 1 NOCYCLE;
