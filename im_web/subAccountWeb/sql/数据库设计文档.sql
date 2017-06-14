CREATE TABLE d_diverse_type(
  d_diverse_type_id INTEGER PRIMARY KEY NOT NULL ,
  d_diverse_type_name VARCHAR(20) NOT NULL ,
  d_is_recommend CHAR(1) DEFAULT '0' NOT NULL,
  d_diverse_description varchar(50) NOT NULL
);
Create sequence seq_diverse_type increment by 1 start with 1;

INSERT INTO d_diverse_type (d_diverse_type_id, d_diverse_type_name, d_is_recommend, d_diverse_description)
VALUES (seq_diverse_type.nextval, '平均分流', 0, '单位时间内所有账号接待量相同');

create table t_merchant_diverse_config(
	m_merchant_id integer primary key not null,
	m_diverse_type integer not null,
	m_proxy_subaccount_id integer default -1 not null,
  constraint fk_merchant_diverse_config foreign key (m_diverse_type) references d_diverse_type (d_diverse_type_id) on delete cascade
);

create table t_service_group(
	g_group_id integer primary key not null,
	g_group_name varchar(50) default '' not null,
  g_is_show CHAR(1) DEFAULT '0' NOT NULL ,
  g_show_order INTEGER DEFAULT 0 NOT NULL ,
  g_merchant_id INTEGER NOT NULL
);
Create sequence seq_service_group increment by 1 start with 1;

create table t_service_group_mapping(
  s_mapping_id INTEGER PRIMARY KEY NOT NULL ,
	s_group_id integer not null,
	s_subaccount_id integer unique not null,
  s_diverse_state CHAR(1) default '0' not null ,
	constraint fk_service_group_mapping_group foreign key (s_group_id) references t_service_group (g_group_id) on delete cascade
);

Create sequence seq_service_group_mapping increment by 1 start with 1;

create table t_store_light_config(
  s_merchant_id INTEGER NOT NULL PRIMARY KEY ,
	s_first_work_day_start char(1)  default '1' not null,
	s_first_work_day_end char(1)  default '1' not null,
	s_first_work_time_start char(5)  default '00:00' not null,
	s_first_work_time_end char(5)  default '00:00' not null,
	s_first_is_work_time_show char(1)  default '0' not null,

  s_second_work_day_start char(1)  default '1' not null,
  s_second_work_day_end char(1)  default '1' not null,
  s_second_work_time_start char(5)  default '00:00' not null,
  s_second_work_time_end char(5)  default '00:00' not null,
  s_second_is_work_time_show char(1)  default '0' not null,

  s_phone CHAR(11) NOT NULL ,
  s_is_phone_show CHAR(1) DEFAULT '0' NOT NULL,
	s_telphone char(11) not null,
	s_is_telphone_show char(1) default '0' not null
);

create table d_show_group_config(
	d_config_id integer primary key not null,
	d_config_name varchar(50) not null
);
Create sequence seq_show_group_config increment by 1 start with 1;

insert into d_show_group_config	(d_config_id, d_config_name	)
values(seq_service_group_config.nextval, '商家店铺名称亮灯');
insert into d_show_group_config	(d_config_id, d_config_name	)
values(seq_service_group_config.nextval, '购物车结算页');
insert into d_show_group_config	(d_config_id, d_config_name	)
values(seq_service_group_config.nextval, '已购买商品（未发货）');
insert into d_show_group_config	(d_config_id, d_config_name	)
values(seq_service_group_config.nextval, '已购买商品（已发货）');
insert into d_show_group_config	(d_config_id, d_config_name	)
values(seq_service_group_config.nextval, '其他');


create table t_group_show_mapping(
  g_mapping_id INTEGER NOT NULL PRIMARY KEY ,
	g_group_id integer not null ,
	g_config_id integer not null UNIQUE ,

	constraint fk_group_show_mapping_group foreign key (g_group_id) references t_service_group(g_group_id) on delete cascade,
	constraint fk_group_show_mapping_config foreign key (g_config_id) references d_show_group_config(d_config_id) on delete cascade,
  CONSTRAINT t_group_show_mapping_u1 UNIQUE (g_group_id, g_config_id)
);

Create sequence seq_group_show_mapping increment by 1 start with 1;
