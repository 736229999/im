create table t_merchant_diverse_config(
	m_merchant_id integer primary key not null,
	m_diverse_type integer not null,
	m_proxy_subaccount_id integer default -1 not null
);

create table t_service_group(
	g_group_id integer primary key not null,
	g_group_name varchar(50) default '' not null
);
Create sequence seq_service_group increment by 1 start with 1;

create table t_service_group_mapping(
	s_mapping_id integer primary key not null,
	s_group_id integer not null,
	s_subaccount_id integer unique not null,
	s_is_diverse_on char(1) default '0' not null ,
	constraint fk_service_group_mapping_group foreign key (s_group_id) references t_service_group (g_group_id) on delete cascade
);
Create sequence seq_service_group_mapping increment by 1 start with 1;

create table t_service_group_config(
	s_config_id integer primary key not null,
	s_group_id integer unique not null,
	s_is_group_name_show char(1) default '0' not null,

	s_work_day_start char(1)  default '1' not null,
	s_work_day_end char(1)  default '1' not null,
	s_work_time_start char(5)  default '00:00' not null,
	s_work_time_end char(5)  default '00:00' not null,
	s_is_work_time_show char(1)  default '0' not null,

	s_telphone char(11) not null,
	s_is_telphone_show char(1) default '0' not null,
	constraint fk_service_group_config_group foreign key (s_group_id) references t_service_group(g_group_id)
on delete cascade
);

Create sequence seq_service_group_config increment by 1 start with 1;

create table d_show_group_config(
	d_config_id integer primary key not null,
	d_config_name varchar(50) not null
);
Create sequence seq_show_group_config increment by 1 start with 1;

insert into d_show_group_config	(d_config_id, d_config_name	)
values(seq_service_group_config.nextval, '商品搜索列表页');

create table t_group_show_mapping(
	g_mapping_id integer primary key not null,
	g_group_id integer not null,
	g_config_id integer not null,
	
	constraint fk_group_show_mapping_group foreign key (g_group_id) references t_service_group(g_group_id)
on delete cascade,
	constraint fk_group_show_mapping_config foreign key (g_config_id) references d_show_group_config(d_config_id)
on delete cascade,
	unique (g_group_id, g_config_id)
);

