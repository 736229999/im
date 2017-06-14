spool im_create.log
create user im identified by moumou default tablespace ecsdata;
grant connect,resource to im;
grant select any table,debug any procedure,unlimited tablespace,debug connect session,select any dictionary to im;
grant create job to im;

set define off;

@/home/oracle/im_db/iecim_tables.sql

@/home/oracle/im_db/iecim_pkgs.sql

@/home/oracle/im_db/iecim_job.sql

@/home/oracle/im_db/IM_USER_INSERT_TRIGGER.trg

spool off
