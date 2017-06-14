_________________________________
Data:20150706

Function:获取商家所有子账户离线消息
_________________________________

存储过程:
 procedure WebMsgMoreReq
  (
	  t_sid in IM_WEBDATA.FROMID%type,
	  t_time in IM_WEBDATA.SENDTIME%type,
	  t_mid in IM_WEBDATA.MSGID%type,
	  t_num in integer,
	  t_merchant in varchar2,
	  curmsgInfo out T_CURSOR,
	  returncode out integer
  );
  
 	procedure WebMsgMoreReq
	(
	  t_sid in IM_WEBDATA.FROMID%type,
	  t_time in IM_WEBDATA.SENDTIME%type,
	  t_mid in IM_WEBDATA.MSGID%type,
	  t_num in integer,
	  t_merchant in varchar2,
	  curmsgInfo out T_CURSOR,
	  returncode out integer
	)
	is
	  v_sql varchar2(4000); 
	  begin
	     
	  	returncode := 0;
		IF t_mid = 0 THEN
		BEGIN
	  		v_sql := 'SELECT FROMID,SENDTIME,FONTSIZE,FONTCOLOR,FONTSTYLE,FONTNAME,FROMNICKNAME,MSG,MSGID '
	           || 'FROM (SELECT * FROM IM_WEBDATA WHERE SENDTIME<  '||t_time||' AND ((FROMID IN ('
	           || t_merchant
	           || ') AND TOID= '||t_sid||' ) OR (FROMID='|| t_sid ||' AND TOID IN ('
	           || t_merchant
	           || '))) ORDER BY MSGID DESC) WHERE ROWNUM>0 AND ROWNUM<='||t_num||'';
	           
			OPEN curmsgInfo FOR v_sql;  
		END;
		ELSE 
		BEGIN	
	  v_sql :=  'SELECT FROMID,SENDTIME,FONTSIZE,FONTCOLOR,FONTSTYLE,FONTNAME,FROMNICKNAME,MSG,MSGID '
	        || 'FROM (SELECT * FROM IM_WEBDATA WHERE MSGID<'||t_mid||' AND ((FROMID IN ('
	        || t_merchant
	        || ') AND TOID='||t_sid||') OR (FROMID='||t_sid||' AND TOID IN ('
	        || t_merchant
	        || '))) ORDER BY MSGID DESC) WHERE ROWNUM>0 AND ROWNUM<='||t_num||'';
	        
			OPEN curmsgInfo FOR v_sql;		
		END;
		END IF;
		EXCEPTION
		when OTHERS then
		begin
			returncode := 1;
			raise;
			return;
		end;
	  end WebMsgMoreReq;
_________________________________
Data:20150630

Function:存储商家的上下线时间信息
_________________________________

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

comment on column im.IM_LOGINOUTTIME.USERID 
	is '用户ID';
comment on column im.IM_LOGINOUTTIME.LOGINTIME 
	is '登录时间';
comment on column im.IM_LOGINOUTTIME.LOGOUTTIME 
	is '登出时间';

create index im.IM_LOGINOUTTIME_IDX_USERID on im.IM_LOGINOUTTIME (USERID) tablespace ECSINDEX;

存储过程:
Mini_Local_PKG:

 procedure SaveMerchantLogin
  (
       t_userid in IM_LOGINOUTTIME.UserId%type,
       t_logintime in IM_LOGINOUTTIME.LOGINTIME%type,
       returncode out integer
  )
  is
    begin
         returncode := 0;
 	INSERT INTO IM_LOGINOUTTIME(UserId,LOGINTIME) VALUES (t_userid,t_logintime);
 
         COMMIT;
         EXCEPTION
         when OTHERS then
         begin
            returncode := 1;
            return;
         end;
    end SaveMerchantLogin;



procedure SaveMerchantLogout
  (
       t_userid in IM_LOGINOUTTIME.UserId%type,
       t_logouttime in IM_LOGINOUTTIME.LOGOUTTIME%type,
       returncode out integer
  )
  is
    begin
         returncode := 0;

         update IM_LOGINOUTTIME set LOGOUTTIME = t_logouttime 
           where UserId=t_userid and LOGOUTTIME = 0;
         COMMIT;
         EXCEPTION
         when OTHERS then
         begin
            returncode := 1;
            return;
         end;
    end SaveMerchantLogout;   





____________________________________________________________
Data:20150702
Function:在IM_WEBDATA里增加LOGINTIME字段来标记买家会话标识
_____________________________________________________________
alter table IM_WEBDATA add LOGINTIME NUMBER(20) 


存储过程:
Mini_User_Pkg:
 procedure WebIMMsgSave
  (
	t_sid in IM_WEBDATA.FROMID%type,
	t_did in IM_WEBDATA.TOID%type,
	t_type in IM_WEBDATA.DATATYPE%type,
	t_ver in IM_WEBDATA.VERSION%type,
	t_time in IM_WEBDATA.SENDTIME%type,
	t_flag in IM_WEBDATA.RECVFLAG%type,
	t_fontsize in IM_WEBDATA.FONTSIZE%type,
	t_fontcolor in IM_WEBDATA.FONTCOLOR%type,
	t_fontstyle in IM_WEBDATA.FONTSTYLE%type,
  	t_logintime in IM_WEBDATA.LOGINTIME%type,//增加的LOGINTIME字段
	t_fontname in IM_WEBDATA.FONTNAME%type,
	t_nickname in IM_WEBDATA.FROMNICKNAME%type,
	t_msg in IM_WEBDATA.MSG%type,
	returncode out integer
  )
  is
  	min_time integer;
    c_time integer;
	t_num integer;
	t_msgid integer;
  	begin
	  returncode := 0;
	  SELECT IM_WEBDATA_SEQ_MSGID.NEXTVAL into t_msgid FROM DUAL;
	  INSERT INTO IM_WEBDATA
	  	(FROMID,TOID,DATATYPE,VERSION,SENDTIME,RECVFLAG,FONTSIZE,FONTCOLOR,FONTSTYLE,LOGINTIME,FONTNAME,FROMNICKNAME,MSG,MSGID)
	  VALUES
	  	(t_sid,t_did,t_type,t_ver,t_time,t_flag,t_fontsize,t_fontcolor,t_fontstyle,t_logintime,t_fontname,t_nickname,t_msg,t_msgid);
	  commit;

	  SELECT COUNT(1) INTO t_num FROM IM_WEBLIST WHERE FROMID=t_sid AND TOID=t_did;
	  if t_num = 0 then
	  begin
	  	INSERT INTO IM_WEBLIST(FROMID,TOID,SENDTIME) VALUES(t_sid,t_did,t_time);
	  	commit;
	  end;
	  elsif t_num = 1 then
	  begin
	  	UPDATE IM_WEBLIST SET SENDTIME=t_time WHERE FROMID=t_sid AND TOID=t_did;
	  	commit;
	  end;
	  else
	  begin
		returncode := 1;
		return;
	  end;
	  end if;

	  EXCEPTION
	  when OTHERS then
	  begin
		  returncode := 1;
		  rollback;
		  return;
	  end;
  	end WebIMMsgSave;









