prompt
prompt Creating job
prompt ================================
prompt

conn im/moumou;

create or replace procedure DeleteWebData
is
begin
delete from im_webdata where TO_DATE('1970-1-1','yyyy-mm-dd hh24:mi:ss')+(sendtime/(3600*24)) < TO_DATE(sysdate-180);
delete from im_weblist where TO_DATE('1970-1-1','yyyy-mm-dd hh24:mi:ss')+(sendtime/(3600*24)) < TO_DATE(sysdate-180);
end;
/
variable job number;
begin
dbms_job.submit(:job,'DeleteWebData;',sysdate,'sysdate+1');
end;
/
begin
dbms_job.run(:job);
end;
/

create or replace procedure DeleteMsgNotifySevenDaysAgo as
begin
       execute immediate 'delete from IM_MSGNOTIFY where INSERTTIME+0<sysdate-7';
end;
/
variable jobno number;
begin
dbms_job.submit(:jobno,'DeleteMsgNotifySevenDaysAgo;',sysdate,'trunc(sysdate)+25/24');
end;  
/
begin
dbms_job.run(:jobno);
end;
/