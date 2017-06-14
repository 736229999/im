prompt
prompt Drop Constraint
prompt ================================
prompt

conn im/moumou;

declare
 	cname varchar2(30);
 	ddl_sql_str varchar2(300);
begin
	select a.constraint_name into :cname from user_constraints a,user_cons_columns b WHERE a.constraint_name=b.constraint_name and a.table_name ='IM_USER_WITH_MSGNOTIFY' and a.CONSTRAINT_TYPE='R' and b.COLUMN_NAME='USERID';
	:ddl_sql_str := 'ALTER TABLE IM_USER_WITH_MSGNOTIFY DROP CONSTRAINT '||:cname;
	DBMS_UTILITY.EXEC_DDL_STATEMENT(:ddl_sql_str);
end;
/