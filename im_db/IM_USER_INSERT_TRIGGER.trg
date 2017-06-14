prompt
prompt Creating trigger
prompt ================================
prompt

create or replace trigger IM_USER_INSERT_TRIGGER
  before insert on im_user  
  for each row
WHEN (new.userid IS NULL)
declare
  -- local variables here
  
begin
  SELECT IM_USER_SEQ_USERID.NEXTVAL
  INTO :new.userid
  FROM dual;
end IM_USER_INSERT_TRIGGER;
/
