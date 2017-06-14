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


select IM_WORK_SEQ_USERID.nextval from dual;
alter table im_userinfo add GROUPNAME varchar2(100);
