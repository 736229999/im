CREATE OR REPLACE 
PROCEDURE statisticsWorkLoad
(startTime in NUMBER ,
endTime in NUMBER,
subMerchantId in VARCHAR2,
merchantId in VARCHAR2,
statisticsDate in NUMBER
)AS
responseTime integer:= 0;
responseCount integer:= 0;
responseIn90sCount integer:= 0;
sessionTime integer:= 0;
sessionCount integer:= 0;
responseMessageCount integer:= 0;
receptionTime integer:= 0;
responseIds CLOB:='';
askingIds CLOB:='';
updateTime VARCHAR2(50);
v_str_merchantid VARCHAR2(4000):='';
v_res_str_merchantid VARCHAR2(4000):='';
v_current_merchantid VARCHAR2(50):='';
v_str_submerchantid VARCHAR2(4000):='';
v_res_str_submerchantid VARCHAR2(4000):='';
v_current_submerchantid VARCHAR2(50):='';
resultCount integer:=0;
BEGIN
--��������
	select to_char(CURRENT_TIMESTAMP,'YYYYMMDD HH24:MI:SS.FF9') INTO updateTime from dual;

	v_str_merchantid := merchantId;
	v_str_submerchantid := subMerchantId;
	
	STATISTICSUPDATEREPLYDETAIL(startTime, endTime);

loop
	EXIT WHEN v_str_submerchantid IS NULL;
	responseTime := 0;
	responseCount := 0;
	sessionTime :=0;
	sessionCount := 0;
  responseIn90sCount :=0;
	responseMessageCount:= 0;
	receptionTime := 0;
	responseIds :='';
	askingIds :='';

	SPLIT_STRING_SP(v_str_submerchantid, v_res_str_submerchantid, v_current_submerchantid, ';');
	SPLIT_STRING_SP(v_str_merchantid, v_res_str_merchantid, v_current_merchantid, ';');
	dbms_output.put_line('�������� v_current_submerchantid');
	dbms_output.put_line(v_current_submerchantid);

--������������������
	SELECT (CASE WHEN COUNT(*) = NULL THEN 0 ELSE COUNT(*) END )INTO resultCount 	FROM T_SESSION_DETAIL p
	WHERE p.I_start_Time < endTime AND p.I_start_Time > startTime AND p.I_FROMID = v_current_submerchantid;

	dbms_output.put_line('�������� count��');
	dbms_output.put_line(resultCount);
IF ( resultCount <>0 ) THEN
--����responseTime,responseCount,sessionTime,sessionCount,responseMessageCount,receptionTime
	SELECT
	SUM(CASE WHEN p.I_DUR >0 OR p.I_DUR =0  THEN p.I_DUR ELSE 0 END) AS response_time,
	SUM(CASE WHEN p.I_DUR >0 OR p.I_DUR =0 OR p.I_DUR = -1 THEN 1 ELSE 0 END) AS response_count,
	SUM(CASE WHEN (p.I_DUR >0 OR p.I_DUR =0) AND p.I_DUR<90 THEN 1 ELSE 0 END) AS response_In90s_Count,
	SUM(CASE WHEN p.I_DUR >0 OR p.I_DUR =0 OR p.I_DUR = -1 THEN p.I_session_len ELSE 0 END) AS SESSION_time,
	SUM(CASE WHEN p.I_DUR >0 OR p.I_DUR =0 OR p.I_DUR = -1 THEN 1 ELSE 0 END) AS SESSION_count,
	SUM(CASE WHEN p.I_DUR >0 OR p.I_DUR =0 OR p.I_DUR = -1 THEN p.I_message_count ELSE 0 END) AS message_count,
	SUM(CASE WHEN p.I_DUR >0 OR p.I_DUR =0 OR p.I_DUR = -1 THEN p.I_reply_len ELSE 0 END) AS reception_time
	INTO responseTime,responseCount,responseIn90sCount,sessionTime,sessionCount,responseMessageCount,receptionTime
	FROM T_SESSION_DETAIL p
	WHERE p.I_start_Time < endTime AND p.I_start_Time > startTime AND p.I_FROMID = v_current_submerchantid
	GROUP BY p.I_FROMID;
--������������������ID����
	SELECT listagg( p.I_toid,';') within GROUP (order by p.I_toid) AS ASKING_IDS INTO askingIds
	FROM T_SESSION_DETAIL p
	WHERE p.I_start_Time < endTime AND p.I_start_Time > startTime AND p.I_FROMID = v_current_submerchantid ;


--��������������������������ID����
	SELECT  COUNT(*) INTO resultCount 	FROM T_SESSION_DETAIL p
	WHERE p.I_start_Time < endTime AND p.I_start_Time > startTime AND p.I_FROMID = v_current_submerchantid AND  p.I_dur <> -2;

	IF ( resultCount <>0 ) THEN
		SELECT listagg( p.I_toid,';') within GROUP (order by p.I_toid) AS responseIds INTO responseIds
		FROM T_SESSION_DETAIL p
		WHERE p.I_start_Time < endTime AND p.I_start_Time > startTime AND p.I_FROMID = v_current_submerchantid AND  p.I_dur <> -2
		GROUP BY p.I_FROMID;
	END IF;

	merge INTO IM_RECEPTION_MAP PO USING (
      SELECT p.I_FROMID AS SUBMERCHANTID, p.I_toid AS CUSTOMERID,
            SUM(CASE  WHEN p.I_DUR = -2 THEN 1 ELSE 0 END) as I_NOT_REPLY,
						SUM(CASE  WHEN p.I_DUR <> -2 THEN 1 ELSE 0 END) as I_REPLY
      FROM T_SESSION_DETAIL p
      WHERE p.I_start_Time < endTime AND p.I_start_Time > startTime AND p.I_FROMID =v_current_submerchantid
      GROUP BY p.I_FROMID,p.I_toid
    )PP
    ON (PO.I_SUBMERCHANT_ID =PP.SUBMERCHANTID AND PO.I_MERCHANT_ID = v_current_merchantid AND PO.I_STATISITC_TIME=statisticsDate AND PO.I_COUSTOMER_ID = PP.CUSTOMERID )
    		WHEN MATCHED THEN UPDATE SET PO.I_REPLY = PO.I_REPLY + PP.I_REPLY , PO.I_NOT_REPLY = PO.I_NOT_REPLY +PP.I_NOT_REPLY 
    WHEN NOT MATCHED THEN INSERT(I_SUBMERCHANT_ID,I_MERCHANT_ID,I_STATISITC_TIME,I_COUSTOMER_ID,I_REPLY,I_NOT_REPLY) VALUES(PP.SUBMERCHANTID,v_current_merchantid ,statisticsDate,PP.CUSTOMERID,PP.I_REPLY ,PP.I_NOT_REPLY );

END IF;
--����������������������������
	DELETE FROM im.im_submerchant_workload p WHERE p.I_SUBMERCHANT_ID = v_current_submerchantid AND p.I_MERCHANT_ID = v_current_merchantid AND p.I_STATISTICS_TIME = statisticsDate;

  insert into im.im_submerchant_workload(
  I_SUBMERCHANT_ID,I_MERCHANT_ID,I_STATISTICS_TIME,I_RESPONSE_DURATION,I_SESSION_DURATION,I_SESSION_COUNT,I_RESPONSE_MESSAGE_COUNT,I_RECEPTION_DURATION,V_RESPONSE_IDS,V_ASKING_IDS,I_RESPONSE_COUNT,I_RESPONSE_IN_90S_COUNT)
  values(v_current_submerchantid,v_current_merchantid,statisticsDate,responseTime,sessionTime,sessionCount,responseMessageCount,receptionTime,responseIds,askingIds,responseCount,responseIn90sCount);

--
	v_str_submerchantid := v_res_str_submerchantid;
  v_str_merchantid := v_res_str_merchantid;
end loop;

END;
