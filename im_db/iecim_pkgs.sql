-----------------------------------------------
-- Export file for user IM                --
-- Created by Newton on 2011/02/10, 10:00:00 --
-----------------------------------------------

spool im_pkgs.log
set define off;

prompt
prompt Creating package MINI_FAMILY_PKG
prompt ================================
prompt
create or replace package im.Mini_Family_Pkg is

  -- Author  : LVWL
  -- Created : 2007-1-19 16:59:46
  -- Purpose : 用户家族信息处理包
  TYPE T_CURSOR IS REF CURSOR;
  procedure GetFamilyList                                   --获取用户的家族列表
  (
       t_userId in IM_CLUSTERMEMBER.Userid%type,
       curfamilylist out T_CURSOR,
       returncode out integer
  );

  procedure GetFamilyInfo                                    --获取家族详细信息
  (
       t_familyid in IM_CLUSTER.ClusterId%type,
       t_userid out IM_CLUSTER.Userid%type,
       t_famLevel out IM_CLUSTER.ClusterLevel%type,--等级 0 普通 1 荣誉
       t_familyclass out IM_CLUSTER.ClusterGroupId%type,--分类
       t_privType out IM_CLUSTER.ViewAuthFlag%type,    --隐私属性
       t_joinMethod out IM_CLUSTER.JoinMethod%type,
       t_familyname out IM_CLUSTER.ClusterName%type,
       t_familydesc out IM_CLUSTER.ClusterDesc%type,
       t_familydomain out IM_CLUSTER.ClusterDomain%type,
       t_logoUri out IM_CLUSTER.LogoUri%type,
       t_memberlimit out IM_CLUSTER.MemberLimit%type,
       t_familysign out IM_CLUSTER.ClusterSign%type,
       t_familynotice out IM_CLUSTER.ClusterNotice%type,
       returncode out integer
  );


  procedure SetFamilyInfo                                    --设置家族详细信息
  (
       t_familyid in IM_CLUSTER.ClusterId%type,
       t_familyclass in IM_CLUSTER.ClusterGroupId%type,
       t_joinMethod in IM_CLUSTER.JoinMethod%type,
       t_familyname in IM_CLUSTER.ClusterName%type,
       t_familydesc in IM_CLUSTER.ClusterDesc%type,
       t_familysign in IM_CLUSTER.ClusterSign%type,
       t_familynotice in IM_CLUSTER.ClusterNotice%type,
       returncode out integer
  );


  procedure GetFamilyMember                                    --获取家族成员
  (
       t_familyid in IM_CLUSTERMEMBER.ClusterId%type,
       curfamilymem out T_CURSOR,
       returncode out integer
  );

  procedure CreateFamily                                    --创建家族
  (
       t_familyid out IM_CLUSTER.ClusterId%type,
       t_userid in IM_CLUSTER.Userid%type,
       t_familyclass in IM_CLUSTER.ClusterGroupId%type,--分类
       t_privType in IM_CLUSTER.ViewAuthFlag%type,
       t_joinMethod in IM_CLUSTER.JoinMethod%type,
       t_familyname in IM_CLUSTER.ClusterName%type,
       t_familydesc in IM_CLUSTER.ClusterDesc%type,
       t_familydomain in IM_CLUSTER.ClusterDomain%type,
       returncode out integer
  );

  procedure AddFamilyMember                                   -- 添加成员
  (
       t_familyid in IM_CLUSTERMEMBER.ClusterId%type,
       t_struid in varchar2,
       retcount out integer,
       returncode out integer
  );

  procedure DelFamilyMember                                   --删除成员
  (
       t_familyid in IM_CLUSTERMEMBER.ClusterId%type,
       t_userid in IM_CLUSTERMEMBER.userid%type,
       returncode out integer
  );

  procedure FamilySetMemProp                                   --设置成员属性
  (
       t_familyid in IM_CLUSTERMEMBER.ClusterId%type,
       t_userid   in IM_CLUSTERMEMBER.Userid%type,
       t_usrLevel in IM_CLUSTERMEMBER.userLevel%type,
       returncode out integer
  );

  procedure SearchFamilyID                                      --按ID查找家族
  (
       t_familyid in IM_CLUSTER.ClusterId%type,
       t_familyname out IM_CLUSTER.ClusterName%type,
       t_familydomain out IM_CLUSTER.ClusterDomain%type,
       t_Joinmethod out IM_CLUSTER.Joinmethod%type,
       t_familyclass out IM_CLUSTER.ClusterGroupId%type,
       t_userid      out IM_CLUSTER.userid%type,
       t_membercount out IM_CLUSTER.Membercount%type,
       returncode out integer
  );

  procedure SearchFamilyName                                    --按名称查找家族
  (
	  t_familyname in IM_CLUSTER.ClusterName%type,
	  n_startpos in integer,
	  n_reqcount in integer,
	  cur_familyinfo out T_CURSOR,
	  returncode out integer
  );

   procedure SearchFamilyClass                                    --按类型查找家族
  (
       t_familyclass in IM_CLUSTER.ClusterGroupId%type,
       n_startpos in integer,
       n_reqcount in integer,
       cur_familyinfo out T_CURSOR,
       returncode out integer
  );

  procedure DeleteFamily
  (
      t_familyid in IM_CLUSTER.ClusterId%type,
      returncode out integer
  );

   procedure GetMemLevel
  (
      t_familyid in IM_CLUSTERMEMBER.ClusterId%type,
      t_userid in IM_CLUSTERMEMBER.Userid%type,
      t_userlevel out IM_CLUSTERMEMBER.Userlevel%type,
      returncode out integer
  );
   procedure GetBlackList
  (
      t_familyid in IM_CLUSTERMEMBER.ClusterId%type,
      t_userid in IM_CLUSTERMEMBER.Userid%type,
      returncode out integer
  );
  
  procedure GetMemProfile                                              --获取群名片
  (
      t_familyid in IM_CLUSTERMEMBER.ClusterId%type,
      t_userid in IM_CLUSTERMEMBER.Userid%type,
      t_profilename out IM_CLUSTERMEMBER.ProfileName%type,
      returncode out integer
  );
  
  procedure SetMemProfile                                              --设置群名片
  (
      t_familyid in IM_CLUSTERMEMBER.ClusterId%type,
      t_userid in IM_CLUSTERMEMBER.Userid%type,
      t_profilename in IM_CLUSTERMEMBER.ProfileName%type,
      returncode out integer
  );

  procedure GetAllProfile                                        
  (
      t_familyid in IM_CLUSTERMEMBER.ClusterId%type,
      cur_profile out T_CURSOR,
      returncode out integer
  );
  
	
end Mini_Family_Pkg;
/

prompt
prompt Creating package Mini_Local_PKG
prompt ================================
prompt

create or replace package im.Mini_Local_PKG is

  TYPE T_CURSOR IS REF CURSOR;
  procedure GetGroupInfo
  (
      t_userid in IM_CLUSTER.UserId%type,
      curGroupInfo out T_CURSOR,
      returncode out integer,
      t_flag in integer
  );
  procedure GetFamilyMsgID
  (
      t_familyid in IM_CLUSTERTALKMSG.ClusterId%type,
      t_maxfamilyid out IM_CLUSTERTALKMSG.MsgId%type,
      t_minfamilyid out IM_CLUSTERTALKMSG.MsgId%type,
      returncode out integer
  );
  procedure SaveFamilyRequestMsg
  (
      t_familyid in IM_CLUSTERSYSMSG.ClusterId%type,
      t_senderid in IM_CLUSTERSYSMSG.Sender%type,
      t_sendtime in IM_CLUSTERSYSMSG.SendTime%type,
      t_msgtype in IM_CLUSTERSYSMSG.MsgType%type,
      t_msg in BLOB default empty_blob(),
      t_msglen in IM_CLUSTERSYSMSG.MsgLen%type,
      returncode out integer
  );
  procedure DeleteFamilyMsg
  (
      t_familyid in IM_CLUSTERTALKMSG.ClusterId%type,
      returncode out integer
  );

  procedure DeleteFamilyMemRecvMsg
  (
       t_familyid in IM_CLUSTERMEMBER.CLUSTERID%type,
       t_recverid in IM_CLUSTERMEMBER.UserId%type,
       returncode out integer
  );

  procedure GetUserOnlineTime
  (
       t_userid in IM_USERSTAT.UserId%type,
       t_timelen out IM_USERSTAT.Totallinetime%type,
       returncode out integer
  );
  procedure SaveMerchantLogin
  (
       t_userid in IM_LOGINOUTTIME.UserId%type,
       t_logintime in IM_LOGINOUTTIME.LOGINTIME%type,
       returncode out integer
  );

  procedure SaveMerchantLogout
  (
       t_userid in IM_LOGINOUTTIME.UserId%type,
       t_logouttime in IM_LOGINOUTTIME.LOGOUTTIME%type,
       returncode out integer
  );

  procedure AddGroup
  (
      t_userid  in IM_FRIENDGROUP.UserId%type,
      t_groupid in out IM_FRIENDGROUP.FriendGroupId%type,
      t_groupname in IM_FRIENDGROUP.FriendGroupName%type,
      t_groupPid in IM_FRIENDGROUP.FriendGroupPid%type,
      returncode out integer
  );

  procedure ModifyCustInfo
  (
      t_id in IM_CURCLILIST.SELLERID%type,
      t_account in IM_CURCLILIST.USERNAME%type,
      t_time in varchar2,
      returncode out integer
  );
  procedure ModifyCustRemark
  (
      t_id in IM_CURCLILIST.SELLERID%type,
      t_account in IM_CURCLILIST.USERNAME%type,
      t_time in varchar2,
      t_remark in IM_CURCLILIST.REMARKNAME%type,
      returncode out integer
  );
  
  procedure GetCustRemark
  (
       t_id in IM_CURCLILIST.SELLERID%type,
      t_account in IM_CURCLILIST.USERNAME%type,
      t_time out varchar2,
      t_remark out IM_CURCLILIST.REMARKNAME%type,
      returncode out integer
  );

 
  procedure GetCustNum
  (
      t_id in IM_CURCLILIST.SELLERID%type,
      t_btime in varchar2,
      t_etime in varchar2,
      t_count out integer,
      returncode out integer
  );
 
  procedure GetCustInfo
  (
	  t_id in IM_CURCLILIST.SELLERID%type,
	  t_btime in varchar2,
	  t_etime in varchar2,
	  t_maxcount in integer,
	  t_pos in integer,
	  t_count in integer,
	  t_direct in integer,
	  curcliInfo out T_CURSOR,
	  returncode out integer
  );

  procedure ModifyGroup
  (
      t_userid in IM_FRIENDGROUP.UserId%type,
      t_groupid in IM_FRIENDGROUP.FriendGroupId%type,
      t_groupname in IM_FRIENDGROUP.FriendGroupName%type,
      returncode out integer
  );
  procedure DeleteGroup
  (
      t_uesrid in IM_FRIENDGROUP.UserId%type,
      t_groupid in IM_FRIENDGROUP.FriendGroupId%type,
      returncode out integer
  );

  procedure SaveUserLogin
  (
       t_userid in IM_LOGINSTAT.UserId%type,
       t_userip in IM_LOGINSTAT.UserIp%type,
       t_userport in IM_USERONLINE.Srcport%type,
       t_serverno in IM_USERONLINE.ServerNo%type,
       returncode out integer
  );

  procedure SaveUserLogout
  (
       t_userid in IM_USERONLINE.UserId%type,
       returncode out integer
  );
  
  procedure UpdateAllLogout
  (
       t_serverno in IM_USERONLINE.ServerNo%type,
       returncode out integer
  );
  
  procedure SaveUserAction
  (
       t_userid in IM_OPSTAT.UserId%type,
       t_actionid in IM_OPSTAT.OpId%type,
       t_actioncount in IM_OPSTAT.OpCount%type,
       returncode out integer
  );
  procedure SaveUserTimelen
  (
      t_userid in IM_TIMELEN.Userid%type,
      t_timelen in IM_TIMELEN.TIMELEN%type,
      returncode out integer
  );

  procedure SaveUserRecvFamilyMsg
  (
      t_familyid in IM_CLUSTERMEMBER.ClusterId%type,
      t_userid in IM_CLUSTERMEMBER.UserId%type,
      t_msgid in IM_CLUSTERMEMBER.MsgIdReceived%type,
      returncode out integer
  );
   procedure GetUserFamilyMsgID
  (
       t_userid in IM_CLUSTERMEMBER.UserId%type,
       t_familyid in IM_CLUSTERMEMBER.ClusterId%type,
       t_msgid out IM_CLUSTERMEMBER.MsgIdReceived%type,
       returncode out integer
  );

   procedure DeleteAllOfflineMsg
  (
       t_userid in IM_OFFLINETALKMSG.Receiver%type,
       returncode out integer
  );

   procedure SaveOnlineNum
  (
       t_onlinenum in IM_ONLINEUSER.Onlineuser%type,
       returncode out integer
  );
  procedure SetWokuMsgRecvID
  (
       t_userid in IM_USERSTAT.UserId%type,
       t_type   in integer,
       t_recid  in IM_USERSTAT.Usermessage%type,
       returncode out integer
  );
  procedure GetWokuMsgID
  (
       t_userid in IM_USERSTAT.UserId%type,
       t_nOperationID out IM_USERSTAT.Useroperation%type,
       t_nUserMsgID  out IM_USERSTAT.Usermessage%type,
       t_nSysMsgID  out IM_USERSTAT.Sysmessage%type,
       t_nBroadcastID  out IM_USERSTAT.Broadcastinfo%type,
       t_nUsercommentID  out number,
       t_nLogcommentID  out number,
       returncode out integer
  );
  
  procedure GetWebIMMsg
  (
       t_serverno in IM_USERONLINE.ServerNo%type,
       t_count in out integer,
       cur_webimmsg out T_CURSOR,
       returncode out integer
  );
  
  procedure SaveBuyerLogin
  (
       t_userid in IM_LOGINOUTTIME.UserId%type,
       t_logintime in IM_LOGINOUTTIME.LOGINTIME%type,
       returncode out integer
  );
  
  procedure SaveBuyerLogout
  (
       t_userid in IM_LOGINOUTTIME.UserId%type,
       t_logouttime in IM_LOGINOUTTIME.LOGOUTTIME%type,
       returncode out integer
  );
  
end Mini_Local_PKG;
/

prompt
prompt Creating package Mini_User_Pkg
prompt ================================
prompt

create or replace package im.Mini_User_Pkg is

  -- Author  : LVWL
  -- Created : 2007-1-19 16:59:46
  -- Purpose : 用户个人信息处理包
  TYPE T_CURSOR IS REF CURSOR;
  procedure GetUserPwd
  (
    t_username in IM_USER.Username%type,         --用户名
    t_userId out IM_USER.userId%type,            --用户ID
    t_userpwd out IM_USER.Password%type,     -- 密码
    ReturnCode out integer                               --返回参数,错误代码 0=正常
  );

  procedure GetUserPwdByID
  (
    t_userId in IM_USER.userId%type,            --用户ID
    t_userpwd out IM_USER.Password%type,     -- 密码
    t_username out IM_USER.Username%type,         --用户名
    t_mobileno out IM_USER.Mobile%type,
    ReturnCode out integer                               --返回参数,错误代码 0=正常 17 未激活
  );
  procedure GetUserPwdByMobile
  (
    t_mobileno in IM_USER.Mobile%type,
    t_username out IM_USER.Username%type,         --用户名
    t_userId out IM_USER.userId%type,            --用户ID
    t_userpwd out IM_USER.Password%type,     -- 密码
    ReturnCode out integer                               --返回参数,错误代码 0=正常 17 未激活
  );
  procedure AccountSave
  (
  	t_id in IM_USER.userId%type,
	t_account in IM_USER.Username%type,
	t_pwd in IM_USER.Password%type,
	t_truename in IM_USERINFO.NICKNAME%type,
	ReturnCode out integer
  );
   procedure AccountSave_20150916
  (
  	t_id in IM_USER.userId%type,
	t_account in IM_USER.Username%type,
	t_pwd in IM_USER.Password%type,
	t_truename in IM_USERINFO.NICKNAME%type,
	ReturnCode out integer
  );

    procedure AccountSaveWork
  (
	t_id out IM_USER.userid%type,
	t_account in IM_USER.Username%type,
	t_pwd in IM_USER.Password%type,
	t_mobileno in IM_USER.Mobile%type,
	t_emialaddress in IM_USER.Email%type,
	t_nickname in IM_USERINFO.NICKNAME%type,
	t_headImg in IM_USERINFO.USERPICURI%type,
	t_groupname in IM_USERINFO.GROUPNAME%type,
	ReturnCode out integer
);

procedure AccountSaveBuyer_20150907
  (
     t_id out IM_USER.userid%type,
	  t_account in IM_USER.Username%type,
	  t_pwd in IM_USER.Password%type,
	  t_guid in IM_USER.sizeds%type,
     t_mobileno in IM_USER.Mobile%type,
     t_emialaddress in IM_USER.Email%type,
     t_nickname in IM_USERINFO.NICKNAME%type,
     t_headImg in IM_USERINFO.USERPICURI%type,
	  ReturnCode out integer
  );
  
  
 procedure AccountSaveBuyer_20150728
  (
     t_id out IM_USER.userid%type,
	  t_account in IM_USER.Username%type,
	  t_pwd in IM_USER.Password%type,
	  t_guid in IM_USER.sizeds%type,
     t_mobileno in IM_USER.Mobile%type,
     t_emialaddress in IM_USER.Email%type,
     t_nickname in IM_USERINFO.NICKNAME%type,
	  ReturnCode out integer
  );
  
  
  procedure AccountSaveBuyer_20150623
  (
     t_id out IM_USER.userid%type,
	  t_account in IM_USER.Username%type,
	  t_pwd in IM_USER.Password%type,
	  t_guid in IM_USER.sizeds%type,
     t_mobileno in IM_USER.Mobile%type,
     t_emialaddress in IM_USER.Email%type, 
	  ReturnCode out integer
  );

   procedure AccountSaveBuyer_20150508
  (
     t_id out IM_USER.userid%type,
	  t_account in IM_USER.Username%type,
	  t_pwd in IM_USER.Password%type,
	  t_guid in IM_USER.sizeds%type,
     t_mobileno in IM_USER.Mobile%type,
     t_emialaddress in IM_USER.Email%type, 
	  ReturnCode out integer
  );  
  
  procedure AccountSaveBuyer
  (
     t_id out IM_USER.userid%type,
	  t_account in IM_USER.Username%type,
	  t_pwd in IM_USER.Password%type,
	  t_guid in IM_USER.sizeds%type,
     t_mobileno in IM_USER.Mobile%type,
     t_emialaddress in IM_USER.Email%type, 
	  ReturnCode out integer
  );
  
   procedure SubAccountSave
  (
	   t_uid in IM_USER.userId%type,
	   t_fid in IM_FRIEND.friendId%type,
	   t_gid in IM_FRIEND.friendGroupId%type,
	   t_nickname in IM_USERINFO.nickName%type,
	   t_pwd in IM_USER.passWord%type,
	   t_truename in IM_FRIEND.aliasName%type,
	   ReturnCode out integer
  );
  procedure GetUserInfo
  (
     t_userid in IM_USER.Userid%type,
     t_username out IM_USER.username%type,
     t_nickname out IM_USER.Company%type,
     t_usersex out IM_USER.Gender%type,
     t_usersign out IM_USERINFO.Usersign%type,
     t_userpicuri out IM_USERINFO.Userpicuri%type,
     t_email out IM_USER.Email%type,
     t_joinauthflag out IM_USERINFO.Joinauthflag%type,
     t_mphone out IM_USER.MOBILE%type,
     t_userdesc out IM_USERINFO.Userdesc%type,
     t_nation out IM_USERINFO.Nation%type,
     t_province out IM_USERINFO.Province%type,
     t_city out IM_USERINFO.City%type,
     t_birthday out IM_USERINFO.Birthday%type,
     t_join2family out IM_USERINFO.Join2family%type,
     t_useraddress out IM_USERINFO.Usraddress%type,
     t_linkpop out IM_USERINFO.linkpop%type,
     t_usermoney out number,
     t_avatoruri out varchar2,
     t_mobilestatus out number,
     t_integral     out IM_USERINFO.integral%type,
     t_career out IM_USERINFO.career%type,
     returncode out integer
  );
  procedure GetUserInfo_20160302
(
	 t_userid in IM_USER.Userid%type,
	 t_username out IM_USER.username%type,
	 t_nickname out IM_USER.Company%type,
	 t_usersex out IM_USER.Gender%type,
	 t_usersign out IM_USERINFO.Usersign%type,
	 t_userpicuri out IM_USERINFO.Userpicuri%type,
	 t_email out IM_USER.Email%type,
	 t_joinauthflag out IM_USERINFO.Joinauthflag%type,
	 t_mphone out IM_USER.MOBILE%type,
	 t_userdesc out IM_USERINFO.Userdesc%type,
	 t_nation out IM_USERINFO.Nation%type,
	 t_province out IM_USERINFO.Province%type,
	 t_city out IM_USERINFO.City%type,
	 t_birthday out IM_USERINFO.Birthday%type,
	 t_join2family out IM_USERINFO.Join2family%type,
	 t_useraddress out IM_USERINFO.Usraddress%type,
	 t_linkpop out IM_USERINFO.linkpop%type,
	 t_usermoney out number,
	 t_avatoruri out varchar2,
	 t_mobilestatus out number,
	 t_integral     out IM_USERINFO.integral%type,
	 t_career out IM_USERINFO.career%type,
	 t_groupname out IM_USERINFO.GROUPNAME%type,
	 returncode out integer
);
  procedure GetUserInfo_20151125
  (
     t_userid in IM_USER.Userid%type,
     t_username out IM_USER.username%type,
     t_nickname out IM_USER.Company%type,
     t_usersex out IM_USER.Gender%type,
     t_usersign out IM_USERINFO.Usersign%type,
     t_userpicuri out IM_USERINFO.Userpicuri%type,
     t_email out IM_USER.Email%type,
     t_joinauthflag out IM_USERINFO.Joinauthflag%type,
     t_mphone out IM_USER.MOBILE%type,
     t_userdesc out IM_USERINFO.Userdesc%type,
     t_nation out IM_USERINFO.Nation%type,
     t_province out IM_USERINFO.Province%type,
     t_city out IM_USERINFO.City%type,
     t_birthday out IM_USERINFO.Birthday%type,
     t_join2family out IM_USERINFO.Join2family%type,
     t_useraddress out IM_USERINFO.Usraddress%type,
     t_linkpop out IM_USERINFO.linkpop%type,
     t_usermoney out number,
     t_avatoruri out varchar2,
     t_mobilestatus out number,
     t_integral     out IM_USERINFO.integral%type,
     t_career out IM_USERINFO.career%type,
     t_realname out IM_CSINFO.realname%type,
     returncode out integer
  );
  procedure GetUserInfo_20160224
  (
     t_userid in IM_USER.Userid%type,
     t_username out IM_USER.username%type,
     t_nickname out IM_USER.Company%type,
     t_usersex out IM_USER.Gender%type,
     t_usersign out IM_USERINFO.Usersign%type,
     t_userpicuri out IM_USERINFO.Userpicuri%type,
     t_email out IM_USER.Email%type,
     t_joinauthflag out IM_USERINFO.Joinauthflag%type,
     t_mphone out IM_USER.MOBILE%type,
     t_userdesc out IM_USERINFO.Userdesc%type,
     t_nation out IM_USERINFO.Nation%type,
     t_province out IM_USERINFO.Province%type,
     t_city out IM_USERINFO.City%type,
     t_birthday out IM_USERINFO.Birthday%type,
     t_join2family out IM_USERINFO.Join2family%type,
     t_useraddress out IM_USERINFO.Usraddress%type,
     t_linkpop out IM_USERINFO.linkpop%type,
     t_usermoney out number,
     t_avatoruri out varchar2,
     t_mobilestatus out number,
     t_integral     out IM_USERINFO.integral%type,
     t_career out IM_USERINFO.career%type,
     t_realname out IM_CSINFO.realname%type,
     t_cslimit out IM_CSINFO.receptionlimit%type,
     returncode out integer
  );
  
   procedure Get_Agent_Merchant 
  (
     t_userid in  T_MERCHANT_DIVERSE_CONFIG.M_MERCHANT_ID%type,
     t_merchant out T_MERCHANT_DIVERSE_CONFIG.M_PROXY_SUBACCOUNT_ID%type,
      returncode out integer
  );
   procedure GetFriendsList
  (
       t_userid in IM_FRIEND.userid%type,
       t_startpos in integer,
       t_reqcount in integer,
       curfriends out T_CURSOR,
       returncode out integer,
       t_flag in integer
  );

   procedure GetSubAccount
  (
       t_groupid in T_SERVICE_GROUP_MAPPING.s_group_id%type,
       cursubaccount out T_CURSOR,
       returncode out integer
  );

   procedure GetFlowMode
  (
       t_id in T_MERCHANT_DIVERSE_CONFIG.m_merchant_id%type,
       t_mode out T_MERCHANT_DIVERSE_CONFIG.m_diverse_type%type,
       returncode out integer
  );

   procedure SearchConnAccount
  (
       t_sid in IM_WEBLIST.FROMID%type,
       t_did in varchar2,
       cur_accounts out T_CURSOR,
       returncode out integer
  );

   procedure GetNoReplyNum
  (
  	   t_id in IM_WEBLIST.FROMID%type,
  	   t_time in integer,
       t_NoReplyNum out integer,
	   returncode out integer
  );
 
  procedure GetJoinFlag
  (
      t_userid in IM_USER.userid%type,
      t_joinflag out IM_USERINFO.Joinauthflag%type,
      returncode out integer
  );

  procedure AddFriend
  (
      t_userid in IM_FRIEND.Userid%type,
      t_bloggerid in IM_FRIEND.FriendId%type,
      t_groupinfo in IM_FRIEND.FriendGroupId%type,
      returncode out integer
  );

  procedure DelFriend
  (
      t_userid in IM_FRIEND.Userid%type,
      t_bloggerid in IM_FRIEND.FriendId%type,
      returncode out integer
  );

  procedure SerarchUserID
  (
      t_userid in IM_USER.Userid%type,
      t_username out IM_USER.Username%type,
      t_nickname out IM_USERINFO.Nickname%type,
      t_usersex out IM_USER.Gender%type,
      t_joinauthflag out IM_USERINFO.Joinauthflag%type,
      returncode out integer
   );

  procedure SerarchUserName
  (
      t_username in IM_USER.Username%type,
      t_userid out IM_USER.Userid%type,
      t_nickname out IM_USERINFO.Nickname%type,
      t_usersex out IM_USER.Gender%type,
      t_joinauthflag out IM_USERINFO.Joinauthflag%type,
      returncode out integer
   );

  procedure SerarchNickName
  (
      t_nickname in IM_USERINFO.Nickname%type,
      t_startpos in integer,
      t_reqcount in integer,
      curFindNick out T_CURSOR,
      returncode out integer
   );

  procedure SerarchEmail
  (
      t_email in IM_USER.Email%type,
      t_startpos in integer,
      t_reqcount in integer,
      curFindEmail out T_CURSOR,
      returncode out integer
   );

   procedure ChangePwd
  (
      t_userid in IM_USER.Userid%type,
      returncode out integer
  );

  procedure ModifyUserInfo
  (
      t_userid in IM_USER.Userid%type,
      t_joinauthflag in IM_USERINFO.Joinauthflag%type,
      t_nickname in IM_USERINFO.Nickname%type,
      t_testnick in IM_USERINFO.Nickname%type,
      t_passwd in IM_USER.password%type,
      t_desc in IM_USERINFO.Userdesc%type,
      t_emal in IM_USER.Email%type,
      t_mphone in IM_USER.mobile%type,
      t_join2family in IM_USERINFO.Join2family%type,
      t_birthday in IM_USERINFO.Birthday%type,
      t_nation in IM_USERINFO.Nation%type,
      t_province in IM_USERINFO.Province%type,
      t_city in IM_USERINFO.City%type,
      t_sex in IM_USER.Gender%type,
      t_address in IM_USERINFO.usraddress%type,
      t_linkpop in IM_USERINFO.linkpop%type,
      t_career in IM_USERINFO.career%type,
      t_url in IM_USERINFO.userpicuri%type,
      returncode out integer     --14 昵称已经存在 15  昵称包含非法字符
  );

  procedure WebIMMsgSave_20150708
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
	t_logintime in IM_WEBDATA.LOGINTIME%type,
	t_fontname in IM_WEBDATA.FONTNAME%type,
	t_nickname in IM_WEBDATA.FROMNICKNAME%type,
	t_msg in IM_WEBDATA.MSG%type,
	returncode out integer
  );
  
  procedure WebIMMsgSave_20151222
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
	t_logintime in IM_WEBDATA.LOGINTIME%type,
	t_fontname in IM_WEBDATA.FONTNAME%type,
	t_nickname in IM_WEBDATA.FROMNICKNAME%type,
	t_msg in IM_WEBDATA.MSG%type,
  t_uuid in IM_WEBDATA.UUID%type,
	returncode out integer
  );
  
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
	t_fontname in IM_WEBDATA.FONTNAME%type,
	t_nickname in IM_WEBDATA.FROMNICKNAME%type,
	t_msg in IM_WEBDATA.MSG%type,
	returncode out integer
  );

  procedure WebRecommSave
  (
	  t_fid in IM_WEBRECOMMENT.FROMID%type,
	  t_did in IM_WEBRECOMMENT.TOID%type,
	  t_recomm in IM_WEBRECOMMENT.RECOMMENT%type,
	  t_time in IM_WEBRECOMMENT.SENDTIME%type,
	  t_logintime in IM_WEBRECOMMENT.I_SESSION_ID%type,
	  t_msg	in IM_WEBRECOMMENT.V_RECOMMENT_TEXT%type,
	  returncode out integer
  );	 

  procedure WebServiceSave
  (
	  t_fid in IM_WEBRECOMMENT.FROMID%type,
	  t_did in IM_WEBRECOMMENT.TOID%type,
	  t_recomm in IM_WEBRECOMMENT.RECOMMENT%type,
	  t_time in IM_WEBRECOMMENT.SENDTIME%type,
	  returncode out integer
  );	
  
  procedure WebMsgReq
  (
	  t_sid in IM_WEBDATA.FROMID%type,
	  t_did in IM_WEBDATA.TOID%type,
	  t_time in IM_WEBDATA.SENDTIME%type,
	  t_mid in IM_WEBDATA.MSGID%type,
	  t_num in integer,
	  curmsgInfo out T_CURSOR,
	  returncode out integer
  );
  
  	procedure WebLastMsgReq
	(
		t_sid in IM_WEBDATA.FROMID%type,
		t_num in integer,
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
  );

   procedure WebListReq_20150907
  (
	  t_sid in IM_WEBDATA.FROMID%type,
	  t_num in integer,
	  curlistInfo out T_CURSOR,
	  returncode out integer
  );
  
    procedure WebListReq_20151020
  (
	  t_sid in IM_WEBDATA.FROMID%type,
	  t_num in integer,
	  curlistInfo out T_CURSOR,
	  returncode out integer
  );
  
  procedure WebListReq
  (
	  t_sid in IM_WEBDATA.FROMID%type,
	  t_num in integer,
	  curlistInfo out T_CURSOR,
	  returncode out integer
  );	
  procedure  ModifyFriendAliasname
  (
       t_userid in IM_FRIEND.Userid%type,
       t_bloggerid in IM_FRIEND.FriendId%type,
       t_aliasname in IM_FRIEND.aliasname%type,
       returncode out integer
  );

  procedure  ModifyFriendAliasname20150824                   --修改用户别名
  (
       t_userid in IM_FRIEND.Userid%type,
       t_bloggerid in IM_FRIEND.FriendId%type,
       t_aliasname in IM_FRIEND.aliasname%type,
       returncode out integer
  );
  
  procedure FillUserMoney
  (
      t_userid in number,
      t_fillmoney in number,
      returncode out integer
  );

   procedure ModifyUserSign
  (
       t_userid in IM_USERINFO.Userid%type,
       t_usersign in IM_USERINFO.Usersign%type,
       returncode out integer
  );

  procedure ModifyFriendLoveFlag
  (
      t_userid in IM_FRIEND.Userid%type,
      t_bloggerid in IM_FRIEND.FriendId%type,
      t_loveflag in number,
      returncode out integer
  );

  procedure GetClientMsg
  (
       t_recid in out IM_CLIENTMSGINFO.Recid%type,
       t_count in out integer,
       cur_clientmsg out T_CURSOR,
       returncode out integer
  );

  procedure GetOfflineClientMsg
  (
       t_userid in IM_CLIENTMSGINFO.Userid%type,
       t_count in out integer,
       cur_offclientmsg out T_CURSOR,
       returncode out integer
  );

  procedure SetClientMsgStatus
  (
       t_recid in Im_Clientmsginfo.Recid%type,
       returncode out integer
   );

  procedure GetFriendGroup
  (
       t_userid in IM_FRIEND.userid%type,
       t_bloggerid in IM_FRIEND.FriendId%type,
       t_groupid out IM_FRIEND.FriendGroupId%type,
       returncode out integer
  );

  procedure ModifyFriendGroup
  (
       t_userid in IM_FRIEND.Userid%type,
       t_bloggerid in IM_FRIEND.FriendId%type,
       t_groupid in IM_FRIEND.FriendGroupId%type,
       returncode out integer
  );

  procedure GetFriendAliasName
  (
       t_userid in IM_FRIEND.Userid%type,
       t_bloggerid in IM_FRIEND.FriendId%type,
       t_aliasname out IM_FRIEND.Aliasname%type,
       returncode out integer
  );

  procedure GetUserBaseInfo
  (
      t_struid in varchar2,
      curuserbaseinfo out T_CURSOR,
      returncode out integer
  );

  procedure GetFriendsBaseInfo
  (
      t_userid in IM_FRIEND.Userid%type,
      t_struid in varchar2,
      curfriendsbaseinfo out T_CURSOR,
      returncode out integer
  );
  procedure GetFriendsBaseInfo_20150729
  (
      t_userid in IM_FRIEND.Userid%type,
      t_struid in varchar2,
      curfriendsbaseinfo out T_CURSOR,
      returncode out integer
  );
  procedure GetUserJoin2Family
  (
       t_struid in varchar2,
       curjoin2family out T_CURSOR,
       returncode out integer
  );
/*
   procedure GetOperationMsg
  (
       t_recid in number,
       t_count in  integer,
       cur_operationmsg out T_CURSOR,
       returncode out integer
  );
*/
/*amin
  procedure GetUserMsg   --收件箱消息
  (
       t_recid in out IM_MESSAGE_ALERT.MsgId%type,
       t_count in integer,
       cur_usermsg out T_CURSOR,
       returncode out integer        
  );
*/
/*
  procedure GetUserMsg
  (
        t_recid in out BLOG_USERINBOX.RECID%type,
       t_count in  integer,
       cur_usermsg out T_CURSOR,
       returncode out integer
  );
*/
  procedure GetSysMsg   --系统广播消息
  (
       t_msgid in out im_sysmessage.msgid%type,
       t_count in integer,
       cur_sysmsg out T_CURSOR,
       returncode out integer
  );
 
  procedure GetSysOffMsg   --离线系统消息
  (
       t_msgid in out im_sysmessage.msgid%type,
       t_uid in im_sysmessage.toid%type,
       t_count in integer,
       cur_sysmsg out T_CURSOR,
       returncode out integer
  );
/*
  procedure  Get_broadcastInfo   --用户广播消息
  (
       t_msgid in out  blog_broadcastInfo.MSGID%type,
       t_count in  integer,
       cur_broadcast out T_CURSOR,
       returncode out integer
  );

  procedure Get_UserComment  --个人主页评论
  (
        t_commentid in out blog_userComment.Commentid%type,
        t_count in out integer,
        cur_usercomment out T_CURSOR,
        returncode out integer
  );

  procedure Get_LogComment  --博客内容评论
  (
        t_commentid in out  blog_userLogComment.Commentid%type,
        t_count in out integer,
        cur_logcomment out T_CURSOR,
        returncode out integer
  );
*/
/*
 procedure GetOneOperationMsg  --用户更新消息
  (
       t_recid in out number,
       t_struid in varchar2,
       t_count in integer,
       cur_oneoperationmsg out T_CURSOR,
       returncode out integer
  );
*/

/*
  procedure GetOneUserMsg   --收件箱消息
  (
       t_recid in out BLOG_USERINBOX.RECID%type,
       t_userid in BLOG_USERINBOX.USERID%type,
       t_count in integer,
       cur_oneusermsg out T_CURSOR,
       returncode out integer
  );
*/
/*
  procedure Get_OneUserComment  --个人主页评论
  (
        t_commentid in out blog_userComment.Commentid%type,
        t_userid in blog_usercomment.userid%type,
        t_count in out integer,
        cur_oneusercomment out T_CURSOR,
        returncode out integer
  );

  procedure Get_OneLogComment  --博客内容评论
  (
        t_commentid in out  blog_userLogComment.Commentid%type,
        t_userid in blog_userlogcomment.userid%type,
        t_count in out integer,
        cur_onelogcomment out T_CURSOR,
        returncode out integer
  );

  procedure SetUserMessageStatus
  (
       t_recid  in BLOG_USERINBOX.Recid%type,
       returncode out integer
  );

   procedure GetSystemLog  --业务同步
  (
       t_recid in out Blog_Systemlog.Recid%type,
       t_count in integer,
       cur_systemlog out T_CURSOR,
       returncode out integer
  );
 */
  procedure GetNickName
  (
         t_userid in IM_USERINFO.Userid%type,
         t_nickname out  IM_USERINFO.Nickname%type,
         returncode out integer
  );

  procedure GetUserPicUri
  (
         t_userid in IM_USERINFO.Userid%type,
         t_userpicuri out  IM_USERINFO.Userpicuri%type,
         returncode out integer
  );
  
  PROCEDURE SplitStringSp(
  	P_INSTR IN VARCHAR2,--传入字符串，逗号分隔，输入参数
		P_STRS OUT VARCHAR2,--截取第1个值之后剩下的字符串，输出参数
		P_CURR OUT VARCHAR2, --截取到的第1个值，输出参数
		P_CHAR IN VARCHAR2 --以什么字符为分隔字符
	) ;
	
PROCEDURE statisticsWorkLoad
(
	startTime in NUMBER ,
	endTime in NUMBER,
	subMerchantId in VARCHAR2,
	merchantId in VARCHAR2,
	statisticsDate in NUMBER
);
	
PROCEDURE statisticsUpdateReplyDetail
(
 startTime in NUMBER ,
	endTime in NUMBER
);

procedure ModifyCSPwd
 (
      t_userid in IM_USER.Userid%type,
      t_passwd in IM_USER.password%type,
      returncode out integer
 );

procedure GetCSPWD
(
    t_username in IM_USER.Username%type,
    t_userId out IM_USER.userId%type,
    t_userpwd out IM_USER.Password%type,
    t_status out IM_USER.status%type,
    ReturnCode out integer
 ) ;

 procedure ImportCSReq
 (
     t_account in IM_USER.Username%type,
     t_permission in IM_CSINFO.permission%type,
     t_receptionlimit in IM_CSINFO.RECEPTIONLIMIT%type,
	   t_importname in IM_CSINFO.IMPORTNAME%type,
     t_id out IM_USER.userid%type,
     t_importtime in IM_CSINFO.IMPORTTIME%type,
     t_importcsid in im_CSINFO.Importcsid%type,
	   ReturnCode out integer
 );

 procedure ModifyCSReq
  (
	      t_receptionlimit in IM_CSINFO.RECEPTIONLIMIT%type,
	      t_username in IM_CSINFO.USERNAME%type,
      	t_realname in IM_CSINFO.REALNAME%type,
        t_password in IM_USER.PASSWORD%type,
        t_importtime out IM_CSINFO.IMPORTTIME%type,
        t_csid in IM_CSINFO.CSID%type,
        t_permission in IM_CSINFO.PERMISSION%type,
      	returncode out integer
  );

 procedure GetVaildCS
 (
       cur_CSlist out T_CURSOR,
       returncode out integer
 );

    procedure QureryCSInfo
  (
	cur_CSInfolist out T_CURSOR,
	t_username in  IM_CSINFO.USERNAME%type,
	t_importname in IM_CSINFO.IMPORTNAME%type,
	t_realname in IM_CSINFO.REALNAME%type,
	t_status in IM_CSINFO.STATUS%type,
	t_begintime in IM_CSINFO.IMPORTTIME%type,
	t_endtime in IM_CSINFO.IMPORTTIME%type,
       returncode out integer
  );
  
  procedure ModifyCSStatusReq
  (
      t_username in IM_CSINFO.USERNAME%type,
      t_status in IM_CSINFO.STATUS%type,
      returncode out integer
  );
   procedure CreateGroup
  (
	  t_account in IM_CSGROUP.GROUPNAME%type,
	  t_describ in IM_CSGROUP.GROUPDESCRIB%type,
	  t_importname in IM_CSGROUP.LASTMODIFYNAME%type,
	  t_importtime in IM_CSGROUP.LASTMODIFYTIME%type,
	  t_id out IM_CSGROUP.GROUPID%type,
	  ReturnCode out integer
  
  );
  
	  procedure ModifyGroup
  (
	  t_id in IM_CSGROUP.GROUPID%type,
	  t_account in IM_CSGROUP.GROUPNAME%type,
	  t_describ in IM_CSGROUP.GROUPDESCRIB%type,
	  t_importname in IM_CSGROUP.LASTMODIFYNAME%type,
	  t_importtime in IM_CSGROUP.LASTMODIFYTIME%type,
	  ReturnCode out integer
  );
  
  procedure AddGroupMem
   (
	   t_groupid in IM_CSGROUPMEM.GROUPID%type,
	   t_ids in VARCHAR2,
	   t_importname in IM_CSGROUP.LASTMODIFYNAME%type,
	   t_importtime in IM_CSGROUP.LASTMODIFYTIME%type,
		 ReturnCode out integer
   );

  
	procedure getUseridMapCSid
  (
	  t_id in IM_CSINFO.USERID%type,
	  t_csid out IM_CSINFO.CSID%type,
	  ReturnCode out integer
  
  );
  
procedure GetCSInfo
(
	t_userid in IM_CSINFO.userid%type,
	t_username out IM_CSINFO.username%type,
	t_status out IM_CSINFO.status%type,
	t_receptionlimit out IM_CSINFO.receptionlimit%type,
	t_permission out IM_CSINFO.permission%type,
	ReturnCode out integer
);

procedure GetCSFriend
(
	t_userid in IM_CSGROUPMEM.userid%type,
	cur_InGroupMem out T_CURSOR,
	ReturnCode out integer

);

procedure GetCSGroupMem
(
	t_groupid in IM_CSGROUPMEM.groupid%type,
	cur_InGroupMem out T_CURSOR,
	ReturnCode out integer
);

procedure GetCSGroupMemFlow
(
	t_groupid in IM_CSGROUPMEM.groupid%type,
	cur_InGroupMem out T_CURSOR,
	ReturnCode out integer
);

 procedure GetCSGroups
(
	t_userid in IM_CSGROUPMEM.userid%type,
	cur_InGroupMem out T_CURSOR,
	ReturnCode out integer
);

  procedure WebHistoryMsg
	(
	  t_sid in IM_WEBDATA.FROMID%type,
    t_did in IM_WEBDATA.FROMID%type,
    t_msgid_bgn in IM_WEBDATA.MSGID%type,
	  t_time_bgn in IM_WEBDATA.SENDTIME%type,
    t_time_end in IM_WEBDATA.SENDTIME%type,
	  t_maxnum in integer,
	  curmsgInfo out T_CURSOR,
	  returncode out integer
	);
	
	 procedure GetVaildCS_20160106
 (
       cur_CSlist out T_CURSOR,
       returncode out integer
 );
 
 procedure SetMsgRemind                                
  (
      t_userid in IM_MSGREMIND.USERID%type,
      t_neworder in IM_MSGREMIND.NEWORDER%type,
      t_payorder in IM_MSGREMIND.PAYORDER%type,
      t_goodscomment in IM_MSGREMIND.GOODSCOMMENT%type,
      t_returnorder in IM_MSGREMIND.RETURNORDER%type,
      returncode out integer                  
  );
  
  procedure GetMsgRemind
	(
	  t_userid in IM_USER.USERID%type,
	  curmsgremind out T_CURSOR,
	  returncode out integer
	);
	
	procedure GetRecentContact
	(
	  t_userguid in IM_USER.SIZEDS%type,
    t_starttime in IM_WEBLIST.SENDTIME%type,
	  recentcontact out T_CURSOR,
	  returncode out integer
	);
	
	procedure DeleteOneMsgNotify
	(
	  t_userid in IM_USER.USERID%type,
    t_uuid in IM_MSGNOTIFY.UUID%type,
	  returncode out integer
	);
	
	procedure GetMsgNotifyDataOnePerson
	(
	  t_userid in IM_USER.USERID%type,
    recentcontact out T_CURSOR,
	  returncode out integer
	);

/*
   procedure PayMoney
  (
        t_userid in number,
        t_consumetype in number,
        t_logid in number,
        t_money in  number,
        returncode out integer
  );
  procedure GetMobileMsg
  (
        t_transid in out mo_mini_smsmo.transId%type,
        t_count in out integer,
        cur_mobilemsg out T_CURSOR,
        returncode out integer
  );

  procedure GetOneMobileMsg
  (
        t_transid in out mo_mini_smsmo.transId%type,
        t_userid in mo_mini_smsmo.receiverid%type,
        t_count in out integer,
        cur_onemobilemsg out T_CURSOR,
        returncode out integer
  );

  procedure SendMobileMsg
  (
        t_userid in blog_userbaseinfo.userid%type,
        t_biztype in mo_mt_sms.biztype%type,
        t_bizid in mo_mt_sms.bizid%type,
        t_smstype in mo_mt_sms.smstype%type,
        t_linkid in mo_mt_sms.linkid%type,
        t_content in mo_mt_sms.content%type,
        t_nickname in blog_userbaseinfo.nickname%type,
        returncode out integer
  );
*/


end Mini_User_Pkg;
/

prompt
prompt Creating package body MINI_FAMILY_PKG
prompt =====================================
prompt
create or replace package body im.Mini_Family_Pkg is

  -- Author  : LVWL
  -- Created : 2007-4-4
  -- Purpose : Mini用户家族信息处理包
  procedure GetFamilyList
  (
        t_userId in IM_CLUSTERMEMBER.Userid%type,
        curfamilylist out T_CURSOR,
        returncode out integer                               --返回参数,错误代码 0=正常
  )
  is

    begin
         returncode := 0;
         OPEN curfamilylist FOR
         select ClusterId
         from IM_CLUSTERMEMBER
         where userid = t_userId AND STATUS = 0;
         EXCEPTION
         when OTHERS then
         begin
              returncode := 1;
              raise;
              return;
         end;
    end GetFamilyList;


  procedure GetFamilyInfo
  (
       t_familyid in IM_CLUSTER.ClusterId%type,
       t_userid out IM_CLUSTER.Userid%type,
       t_famLevel out IM_CLUSTER.ClusterLevel%type,--等级 0 普通 1 荣誉
       t_familyclass out IM_CLUSTER.ClusterGroupId%type,--分类
       t_privType out IM_CLUSTER.ViewAuthFlag%type,    --隐私属性
       t_joinMethod out IM_CLUSTER.JoinMethod%type,
       t_familyname out IM_CLUSTER.ClusterName%type,
       t_familydesc out IM_CLUSTER.ClusterDesc%type,
       t_familydomain out IM_CLUSTER.ClusterDomain%type,
       t_logoUri out IM_CLUSTER.LogoUri%type,
       t_memberlimit out IM_CLUSTER.MemberLimit%type,
       t_familysign out IM_CLUSTER.ClusterSign%type,
       t_familynotice out IM_CLUSTER.ClusterNotice%type,
       returncode out integer
  )
  is
     begin
       returncode := 0;

       begin
          SELECT userid,ClusterLevel, ClusterGroupId, ViewAuthFlag, joinMethod, ClusterName, ClusterDesc, ClusterDomain,memberlimit,logoUri,ClusterSign,ClusterNotice
          INTO t_userid, t_famLevel,t_familyclass,t_privType,t_joinMethod,t_familyname,t_familydesc, t_familydomain,t_memberlimit,t_logoUri,t_familysign,t_familynotice
          FROM IM_CLUSTER
          WHERE ClusterId = t_familyid;

          EXCEPTION
          when NO_DATA_FOUND THEN
          begin
               returncode := 1403;
               return;
          end;
          when OTHERS then
          begin
               returncode := 1;
               raise;
               return;
          end;
       end;

    end Getfamilyinfo;


  procedure SetFamilyInfo                                    --获取家族详细信息
  (
       t_familyid in IM_CLUSTER.ClusterId%type,
       t_familyclass in IM_CLUSTER.ClusterGroupId%type,
       t_joinMethod in IM_CLUSTER.JoinMethod%type,
       t_familyname in IM_CLUSTER.ClusterName%type,
       t_familydesc in IM_CLUSTER.ClusterDesc%type,
       t_familysign in IM_CLUSTER.ClusterSign%type,
       t_familynotice in IM_CLUSTER.ClusterNotice%type,
       returncode out integer                                --返回码：1403 家族不存在 5名称已经存在 16名称非法
  )
  is
     t_count integer;
     begin
          returncode := 0;

          SELECT count(1) INTO t_count
          FROM IM_CLUSTER
          WHERE ClusterId = t_familyid;
          if t_count = 0 then
          begin
                returnCode := 1403;
                return;
          end;
          end if;

          SELECT count(1) INTO t_count
          FROM IM_CLUSTER
          WHERE ClusterName=t_familyname and ClusterId != t_familyid;
          if t_count > 0 then
          begin
                returnCode := 5;
                return;
          end;
          end if;

          UPDATE IM_CLUSTER
          SET ClusterGroupId=t_familyclass,joinMethod=t_joinMethod,
              ClusterName =t_familyname,ClusterDesc=t_familydesc,
              ClusterSign=t_familysign,ClusterNotice=t_familynotice
          WHERE ClusterId=t_familyid;
          commit;

          EXCEPTION
          WHEN NO_DATA_FOUND THEN
          begin
               returncode := 1403;
               return;
          end;
          when OTHERS then
          begin
               returncode := 1;
               rollback;
               raise;
               return;
          end;
     end Setfamilyinfo;



   procedure GetFamilyMember
  (
       t_familyid in IM_CLUSTERMEMBER.ClusterId%type,
       curfamilymem out T_CURSOR,
       returncode out integer
  )
  is
    begin
         returncode := 0;
         OPEN curfamilymem FOR
         SELECT userId,UserLevel
         FROM IM_CLUSTERMEMBER
         WHERE ClusterId=t_familyid AND STATUS = 0;
         EXCEPTION
         when OTHERS then
         begin
              returncode := 1;
              raise;
              return;
         end;
    end GetFamilyMember;

  procedure CreateFamily                                    --创建家族
  (
       t_familyid out IM_CLUSTER.ClusterId%type,
       t_userid in IM_CLUSTER.Userid%type,
       t_familyclass in IM_CLUSTER.ClusterGroupId%type,--分类
       t_privType in IM_CLUSTER.ViewAuthFlag%type,
       t_joinMethod in IM_CLUSTER.JoinMethod%type,
       t_familyname in IM_CLUSTER.ClusterName%type,
       t_familydesc in IM_CLUSTER.ClusterDesc%type,
       t_familydomain in IM_CLUSTER.ClusterDomain%type,
       returncode out integer  -- 7 创建数满 5 家族名存在 16 非法名称 6 域名存在
  )
  is
     t_count integer;
     begin
          returncode := 0;
          t_familyid := 0;
          t_count := 0;
          SELECT COUNT(1) INTO t_count
          FROM IM_CLUSTER WHERE USERId=t_userid and status = 0;
          if t_count >= 100 then        --最多创建100家族
          begin
                returnCode := 7;
                return;
          end;
          end if;

          SELECT COUNT(1) INTO t_count FROM IM_CLUSTER WHERE ClusterName=t_familyname;
          if t_count > 0 then
          begin
                returnCode := 5;
                return;
          end;
          end if;

          SELECT COUNT(1) INTO t_count FROM IM_CLUSTER WHERE ClusterDomain=t_familydomain;
          if t_count > 0 then
          begin
                returnCode := 6;
                return;
          end;
          end if;

          select IM_CLUSTER_SEQ_CLUSTERID.nextval into t_familyid from dual;

          INSERT INTO IM_CLUSTER
                 (ClusterId,ClusterName,ClusterGroupId,ClusterDesc,ClusterDomain,ViewAuthFlag,JoinMethod, USERID,MemberCount)
           VALUES
                 (t_familyid,t_familyname,t_familyclass,t_familydesc,t_familydomain,t_privType,t_joinMethod,t_userid,1);

          INSERT INTO IM_CLUSTERMEMBER
                 (ClusterId, userId,username,status,UserLevel,joinMethod)
          VALUES
                 (t_familyid, t_userid,(select username from im_user where userid=t_userid),0,2,2);

          commit;
          EXCEPTION
          when OTHERS then
          begin
               returncode := 1;
               rollback;
               raise;
               return;
          end;
     end CreateFamily;

  procedure AddFamilyMember
  (
       t_familyid in IM_CLUSTERMEMBER.ClusterId%type,
       t_struid in varchar2,
       retcount out integer,
       returncode out integer
  )
  is
  l_list varchar2(4000) := t_struid;
  p_del varchar2(2) := ',';
  l_userid varchar2(10);
  l_idx  pls_integer;
  l_count integer;
  t_count integer;
  t_status integer;
  t_username IM_CLUSTERMEMBER.Username%type;
     begin
          returncode := 0;
          l_count := 0;
          retcount := 0;
          t_status := 0;
          loop
             l_idx := instr(l_list,p_del);
             if l_idx > 0 then
                l_userid := substr(l_list,1,l_idx-1);
                l_list := substr(l_list,l_idx+length(p_del));
             else
                l_userid := l_list;
             end if;

             if l_userid is not null then
                 SELECT count(1) into t_count
                 from IM_CLUSTERMEMBER
                 where ClusterId = t_familyid and userid = to_number(l_userid);
                 if t_count <> 1 then
                 begin
                     select username into t_username from IM_USER where userid = to_number(l_userid);
                     INSERT INTO IM_CLUSTERMEMBER (ClusterId,userid,status,userLevel,username)
                     VALUES  (t_familyid,to_number(l_userid),0,0,t_username);
                     l_count  := l_count + 1;
                 end;
                 else
                 if t_count = 1 then
                 begin
                       SELECT STATUS into t_status                          --黑名单的不通过。
                       from IM_CLUSTERMEMBER
                       where ClusterId = t_familyid and userid = to_number(l_userid);
                       if t_status <> 9 and t_status <> 0 then
                       begin
                          UPDATE IM_CLUSTERMEMBER set userlevel = 0,status=0 where ClusterId = t_familyid and userid = to_number(l_userid);
                          l_count  := l_count + 1;
                       end;
                       end if;
                 end;
                 end if;
                 end if;
             end if;

             if l_idx <= 0 then
                exit;
             end if;
          end loop;
          if l_count > 0 then
               UPDATE IM_CLUSTER SET MemberCount = MemberCount + l_count WHERE ClusterId = t_familyid;
          end if;
          COMMIT;
          retcount := l_count;
          EXCEPTION
          when OTHERS then
          begin
               returncode := 1;
               retcount := l_count;
               rollback;
               raise;
               return;
          end;


     end AddFamilyMember;

  procedure DelFamilyMember
  (
       t_familyid in IM_CLUSTERMEMBER.ClusterId%type,
       t_userid in IM_CLUSTERMEMBER.userid%type,
       returncode out integer
  )
  is
     n_tmp integer;
     begin
          returncode := 0;
          select count(1) into n_tmp from IM_CLUSTERMEMBER where ClusterId=t_familyid AND userid=t_userid;
          if n_tmp > 0 then
             DELETE from IM_CLUSTERMEMBER WHERE ClusterId=t_familyid AND userid=t_userid;
             UPDATE IM_CLUSTER SET MemberCount=MemberCount-1 WHERE ClusterId = t_familyid and MemberCount > 0;
             COMMIT;
          end if;
          EXCEPTION
          when OTHERS then
          begin
               returncode := 1;
               rollback;
               raise;
               return;
          end;
     end DelFamilyMember;


  procedure FamilySetMemProp                                  --设置成员属性
  (
       t_familyid in IM_CLUSTERMEMBER.ClusterId%type,
       t_userid   in IM_CLUSTERMEMBER.Userid%type,
       t_usrLevel in IM_CLUSTERMEMBER.userLevel%type,
       returncode out integer
  )
  is
     begin
          returncode := 0;
           Update IM_CLUSTERMEMBER set userLevel=t_usrLevel WHERE ClusterId=t_familyid and userId = t_userid;
           commit;
           EXCEPTION
           when OTHERS then
           begin
                  returncode := 1;
                  rollback;
                  raise;
                  return;
           end;
     end FamilySetMemProp;

  procedure SearchFamilyID                                    --按ID查找家族
  (
       t_familyid in IM_CLUSTER.ClusterId%type,
       t_familyname out IM_CLUSTER.ClusterName%type,
       t_familydomain out IM_CLUSTER.ClusterDomain%type,
       t_Joinmethod out IM_CLUSTER.Joinmethod%type,
       t_familyclass out IM_CLUSTER.ClusterGroupId%type,
       t_userid      out IM_CLUSTER.userid%type,
       t_membercount out IM_CLUSTER.Membercount%type,
       returncode out integer
  )
  is
     begin
          returncode := 0;
          SELECT ClusterName,ClusterDomain, Joinmethod,ClusterGroupId,USERID,Membercount
          INTO t_familyname, t_familydomain, t_Joinmethod, t_familyclass, t_userid,t_membercount
          FROM IM_CLUSTER
          WHERE ClusterId=t_familyid and status=0;
          EXCEPTION
          when NO_DATA_FOUND THEN
          begin
               returncode := 1403;
               return;
          end;
          when OTHERS then
          begin
               rollback;
               returncode := 1;
               raise;
               return;
          end;
      end SearchFamilyID;

  procedure SearchFamilyName                                         --按名称查找家族
  (
	  t_familyname in IM_CLUSTER.ClusterName%type,
	  n_startpos in integer,
	  n_reqcount in integer,
	  cur_familyinfo out T_CURSOR,
	  returncode out integer
  )
  is
	  n_endpos integer;
	  begin
			n_endpos := n_startpos + n_reqcount;
			returncode := 0;
			OPEN cur_familyinfo FOR
			SELECT ClusterId, ClusterName,ClusterDomain, Joinmethod,ClusterGroupId,USERID,Membercount
			FROM
				(
				   SELECT rownum as RdNo, ClusterId, ClusterName,ClusterDomain, Joinmethod,ClusterGroupId,USERID,Membercount
				   FROM IM_CLUSTER
				   where ClusterName like '%'||t_familyname||'%' and status=0 and rownum <=n_endpos
				)
			WHERE RdNo > n_startpos and RdNo <= n_endpos;
			EXCEPTION
			when OTHERS then
			begin
				 returncode := 1;
				 raise;
				 return;
			end;
     end SearchFamilyName;

  procedure SearchFamilyClass                                           --按类型查找家族
  (
       t_familyclass in IM_CLUSTER.ClusterGroupId%type,
       n_startpos in integer,
       n_reqcount in integer,
       cur_familyinfo out T_CURSOR,
       returncode out integer
  )
  is
     n_endpos integer;
     begin
           n_endpos := n_startpos + n_reqcount;
           returncode := 0;
           OPEN cur_familyinfo FOR
           SELECT ClusterId, ClusterName,ClusterDomain, Joinmethod,ClusterGroupId,USERID
           FROM
               (
                  SELECT rownum as RdNo, ClusterId, ClusterName,ClusterDomain, Joinmethod,ClusterGroupId,USERID
                  FROM IM_CLUSTER
                  where ClusterGroupId = t_familyclass and status=0 and rownum <=n_endpos
               )
           WHERE RdNo > n_startpos and RdNo <= n_endpos;
           EXCEPTION
           when OTHERS then
           begin
                returncode := 1;
                raise;
                return;
           end;
     end Searchfamilyclass;

  procedure DeleteFamily
  (
      t_familyid in IM_CLUSTER.ClusterId%type,
      returncode out integer
  )
  is
    begin
          returncode := 0;
          UPDATE IM_CLUSTERMEMBER SET STATUS = 99 WHERE ClusterId=t_familyid;
          UPDATE IM_CLUSTER SET STATUS = 1 WHERE ClusterId=t_familyid;
          COMMIT;
          EXCEPTION
          when OTHERS then
          begin
               returncode := 1;
               rollback;
               raise;
               return;
          end;
    end DeleteFamily;

  procedure GetMemLevel
  (
      t_familyid in IM_CLUSTERMEMBER.ClusterId%type,
      t_userid in IM_CLUSTERMEMBER.Userid%type,
      t_userlevel out IM_CLUSTERMEMBER.Userlevel%type,
      returncode out integer
  )
  is
    begin
         returncode :=0;
         SELECT Userlevel INTO t_userlevel
         FROM IM_CLUSTERMEMBER WHERE ClusterId = t_familyid and userId=t_userid;
         EXCEPTION
         when NO_DATA_FOUND THEN
         begin
              returncode := 1403;
              return;
         end;
         when OTHERS then
         begin
               returncode := 1;
               return;
         end;
   end GetMemLevel;
  procedure GetBlackList
  (
      t_familyid in IM_CLUSTERMEMBER.ClusterId%type,
      t_userid in IM_CLUSTERMEMBER.Userid%type,
      returncode out integer
  )
  is
  n_status integer;
    begin
         returncode :=0;
         SELECT STATUS INTO n_status
         FROM IM_CLUSTERMEMBER WHERE ClusterId = t_familyid and userId=t_userid;
          returncode := n_status;
          EXCEPTION
         when NO_DATA_FOUND THEN
         begin
              returncode := 1403;
              return;
         end;
         when OTHERS then
         begin
               returncode := 1;
               return;
         end;

   end GetBlackList;
   
  procedure GetMemProfile                                    --获取成员群名片
  (
      t_familyid in IM_CLUSTERMEMBER.ClusterId%type,
      t_userid in IM_CLUSTERMEMBER.Userid%type,
      t_profilename out IM_CLUSTERMEMBER.ProfileName%type,
      returncode out integer
  )
  is
    begin
         returncode :=0;
         SELECT ProfileName INTO t_profilename
         FROM IM_CLUSTERMEMBER WHERE ClusterId = t_familyid and userId=t_userid;
         EXCEPTION
         when NO_DATA_FOUND THEN
         begin
              returncode := 1403;
              return;
         end;
         when OTHERS then
         begin
               returncode := 1;
               return;
         end;
    end GetMemProfile;
   
  procedure SetMemProfile                                  --设置成员群名片
  (
       t_familyid in IM_CLUSTERMEMBER.ClusterId%type,
       t_userid   in IM_CLUSTERMEMBER.Userid%type,
       t_profilename in IM_CLUSTERMEMBER.ProfileName%type,
       returncode out integer
  )
  is
     begin
          returncode := 0;
           Update IM_CLUSTERMEMBER set ProfileName=t_profilename WHERE ClusterId=t_familyid and userId = t_userid;
           commit;
           EXCEPTION
           when OTHERS then
           begin
                  returncode := 1;
                  rollback;
                  raise;
                  return;
           end;
     end SetMemProfile;

  procedure GetAllProfile                                              --获取群名片
  (
      t_familyid in IM_CLUSTERMEMBER.ClusterId%type,
      cur_profile out T_CURSOR,
      returncode out integer
  )
  is
  	begin
  		returncode := 0;
  		 OPEN cur_profile FOR
		 SELECT USERID, PROFILENAME FROM IM_CLUSTERMEMBER WHERE CLUSTERID = t_familyid and PROFILENAME IS NOT NULL;
		 EXCEPTION
		 when OTHERS then
		 begin
			  returncode := 1;
			  raise;
			  return;
		 end;
  	end GetAllProfile;
     
end Mini_Family_Pkg;
/

prompt
prompt Creating package body Mini_Local_PKG
prompt =====================================
prompt

create or replace package body im.Mini_Local_PKG is

  procedure GetGroupInfo
  (
      t_userid in IM_CLUSTER.UserId%type,
      curGroupInfo out T_CURSOR,
      returncode out integer,
      t_flag in integer
  )
  is
    begin
      returncode := 0;
      OPEN curGroupInfo FOR
      SELECT FriendGroupPid,FriendGroupId,FriendGroupName  FROM  IM_FRIENDGROUP WHERE UserId = t_userid;
      EXCEPTION
      when OTHERS then
      begin
          returncode := 1;
          raise;
          return;
      end;
    end GetGroupInfo;


  procedure GetFamilyMsgID
  (
      t_familyid in IM_CLUSTERTALKMSG.ClusterId%type,
      t_maxfamilyid out IM_CLUSTERTALKMSG.MsgId%type,
      t_minfamilyid out IM_CLUSTERTALKMSG.MsgId%type,
      returncode out integer
  )
  is
    begin
         returncode := 0;
         t_maxfamilyid := 1;
         t_minfamilyid := 0;
         SELECT MAX(MsgId)
         INTO t_maxfamilyid
         FROM IM_CLUSTERTALKMSG
         WHERE ClusterId=t_familyid;
         SELECT MIN(MsgId)
         INTO t_minfamilyid
         FROM IM_CLUSTERTALKMSG
         WHERE ClusterId=t_familyid;
         EXCEPTION
/*         when NO_DATA_FOUND THEN
         begin
              returncode := 1403;
              return;
         end; */
         when OTHERS then
         begin
            returncode := 1;
            return;
         end;

    end Getfamilymsgid;

  procedure SaveFamilyRequestMsg
  (
      t_familyid in IM_CLUSTERSYSMSG.ClusterId%type,
      t_senderid in IM_CLUSTERSYSMSG.Sender%type,
      t_sendtime in IM_CLUSTERSYSMSG.SendTime%type,
      t_msgtype in IM_CLUSTERSYSMSG.MsgType%type,
      t_msg in BLOB default empty_blob(),
      t_msglen in IM_CLUSTERSYSMSG.MsgLen%type,
      returncode out integer
  )
  is
    begin
         returncode := 0;
         INSERT INTO IM_CLUSTERSYSMSG(MsgId, CLUSTERID,Sender,SRCIP,SRCPORT,SENDTIME,MSGTYPE,MSGLEN,MSG)
         VALUES(IM_CLUSTERSYSMSG_SEQ_MSGID.nextval, t_familyid,t_senderid,0,0,t_sendtime,t_msgtype,t_msglen,t_msg);
         commit;
         EXCEPTION
         when OTHERS then
         begin
            returncode := 1;
            return;
         end;

    end Savefamilyrequestmsg;

  procedure DeleteFamilyMsg
  (
      t_familyid in IM_CLUSTERTALKMSG.ClusterId%type,
      returncode out integer
  )
  is
    begin
         returncode := 0;
         DELETE FROM IM_CLUSTERTALKMSG WHERE CLUSTERID=t_familyid;
         update IM_CLUSTERMEMBER set MsgIdReceived=0 WHERE CLUSTERID=t_familyid;
         commit;
         EXCEPTION
         when OTHERS then
         begin
            returncode := 1;
            return;
         end;
    end Deletefamilymsg;

  procedure DeleteFamilyMemRecvMsg
  (
       t_familyid in IM_CLUSTERMEMBER.CLUSTERID%type,
       t_recverid in IM_CLUSTERMEMBER.UserId%type,
       returncode out integer
  )
  is
    begin
         returncode := 0;
         update IM_CLUSTERMEMBER set MsgIdReceived=0 WHERE CLUSTERID=t_familyid and UserId = t_recverid;
         COMMIT;
         EXCEPTION
         when OTHERS then
         begin
            returncode := 1;
            return;
         end;
    end Deletefamilymemrecvmsg;

  procedure GetUserOnlineTime
  (
       t_userid in IM_USERSTAT.UserId%type,
       t_timelen out IM_USERSTAT.Totallinetime%type,
       returncode out integer
  )
  is
    begin
         returncode := 0;
         SELECT TotalLineTime INTO t_timelen FROM IM_USERSTAT WHERE UserId=t_userid;
         EXCEPTION
         when NO_DATA_FOUND THEN
         begin
              returncode := 1403;
              return;
         end;
         when OTHERS then
         begin
            returncode := 1;
            return;
         end;
    end Getuseronlinetime;

 procedure SaveMerchantLogin
  (
       t_userid in IM_LOGINOUTTIME.UserId%type,
       t_logintime in IM_LOGINOUTTIME.LOGINTIME%type,
       returncode out integer
  )
  is
    begin
         returncode := 0;
         update IM_LOGINOUTTIME set LOGOUTTIME = t_logintime 
         where UserId=t_userid and LOGOUTTIME = 0;
         COMMIT;
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

  procedure AddGroup
  (
      t_userid  in IM_FRIENDGROUP.UserId%type,
      t_groupid in out IM_FRIENDGROUP.FriendGroupId%type,
      t_groupname in IM_FRIENDGROUP.FriendGroupName%type,
      t_groupPid in IM_FRIENDGROUP.FriendGroupPid%type,
      returncode out integer
  )
  is
    tmpcount integer;
    begin
         returncode := 0;
         SELECT COUNT(FriendGroupId) INTO tmpcount FROM IM_FRIENDGROUP WHERE UserId=t_userid;
         if tmpcount > 50 then
         begin
              returncode := 10;--超出最大限制
              return;
         end;
         end if;
         SELECT COUNT(FriendGroupId) INTO tmpcount FROM IM_FRIENDGROUP WHERE UserId=t_userid AND FriendGroupName=t_groupname;
         if tmpcount > 0 then
         begin
              returncode := 9;--分组已经存在
              return;
         end;
         end if;
         SELECT count(FriendGroupId) INTO tmpcount FROM IM_FRIENDGROUP WHERE UserId=t_userid AND FriendGroupId=t_groupid;
         if tmpcount > 0 then
         begin
              SELECT MAX(FriendGroupId) INTO t_groupid FROM IM_FRIENDGROUP WHERE UserId=t_userid;
              t_groupid := t_groupid + 1;
         end;
         end if;
         INSERT INTO IM_FRIENDGROUP(UserId,FriendGroupId,FriendGroupName,FriendGroupPid)  VALUES(t_userid,t_groupid,t_groupname,t_groupPid);
         COMMIT;
         EXCEPTION
         when OTHERS then
         begin
            returncode := 1;
            return;
         end;
    end AddGroup;

  procedure ModifyCustInfo
  (
      t_id in IM_CURCLILIST.SELLERID%type,
      t_account in IM_CURCLILIST.USERNAME%type,
      t_time in varchar2,
      returncode out integer
  )
  is
  	tempcount integer;
  	begin
		returncode := 0;
  		SELECT COUNT(SELLERID) INTO tempcount FROM IM_CURCLILIST WHERE SELLERID=t_id AND USERNAME = t_account;
		if tempcount = 0 then
		begin
			INSERT INTO IM_CURCLILIST(SELLERID,USERNAME,LASTTIME) VALUES(t_id,t_account,to_date(t_time,'yyyy-mm-dd hh24:mi:ss'));
			COMMIT;
		end;
		elsif tempcount = 1 then
		begin
			UPDATE IM_CURCLILIST SET LASTTIME = to_date(t_time,'yyyy-mm-dd hh24:mi:ss') WHERE SELLERID = t_id AND USERNAME = t_account;
			COMMIT;
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
		   return;
		end;
  	end ModifyCustInfo;
	
procedure ModifyCustRemark
  (
      t_id in IM_CURCLILIST.SELLERID%type,
      t_account in IM_CURCLILIST.USERNAME%type,
      t_time in varchar2,
      t_remark in IM_CURCLILIST.REMARKNAME%type,
      returncode out integer
  )
  is
  	tempcount integer;
  	begin
		returncode := 0;
  		SELECT COUNT(SELLERID) INTO tempcount FROM IM_CURCLILIST WHERE SELLERID=t_id AND USERNAME = t_account;
		if tempcount = 0 then
		begin
			INSERT INTO IM_CURCLILIST(SELLERID,USERNAME,LASTTIME,REMARKNAME) VALUES(t_id,t_account,to_date(t_time,'yyyy-mm-dd hh24:mi:ss'),t_remark);
			COMMIT;
		end;
		elsif tempcount = 1 then
		begin
			UPDATE IM_CURCLILIST SET REMARKNAME= t_remark WHERE SELLERID = t_id AND USERNAME = t_account;
			COMMIT;
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
		   return;
		end;
  	end ModifyCustRemark;
    
    procedure GetCustRemark
  (
     t_id in IM_CURCLILIST.SELLERID%type,
      t_account in IM_CURCLILIST.USERNAME%type,
      t_time out varchar2,
      t_remark out IM_CURCLILIST.REMARKNAME%type,
      returncode out integer
  )
   is
    begin
          returncode := 0;
          SELECT to_char(LASTTIME,'yyyy-mm-dd hh24:mi:ss'),REMARKNAME
          into t_time ,t_remark
          FROM IM_CURCLILIST
          where  SELLERID=t_id AND USERNAME = t_account;
          EXCEPTION
          when NO_DATA_FOUND THEN
          begin
                returncode := 1403;
                return;
          end;
          when OTHERS then
          begin
              returncode := 1;
              return;
          end;
  	end GetCustRemark;
    
	procedure GetCustNum
	(
		t_id in IM_CURCLILIST.SELLERID%type,
		t_btime in varchar2,
		t_etime in varchar2,
		t_count out integer,
		returncode out integer
	)
	is
	 begin
		returncode := 0;
  		SELECT COUNT(1) INTO t_count FROM IM_CURCLILIST WHERE SELLERID=t_id AND LASTTIME BETWEEN to_date(t_btime,'yyyy-mm-dd hh24:mi:ss') AND to_date(t_etime,'yyyy-mm-dd hh24:mi:ss');
		EXCEPTION
		when OTHERS then
		begin
			returncode := 1;
			raise;
			return;
		end;
	 end GetCustNum;

	procedure GetCustInfo
	(
		t_id in IM_CURCLILIST.SELLERID%type,
		t_btime in varchar2,
		t_etime in varchar2,
		t_maxcount in integer,
		t_pos in integer,
		t_count in integer,
		t_direct in integer,
		curcliInfo out T_CURSOR,
		returncode out integer
	)
	is
	  t_endpos integer;
	  begin
	  	t_endpos := t_count+t_pos;
		returncode := 0;
		if t_direct=0 then
		begin
			OPEN curcliInfo FOR
			SELECT USERNAME,to_char(LASTTIME,'yyyy-mm-dd hh24:mi:ss'),ORDERID,REMARKNAME FROM 
			(SELECT ROWNUM as RdNo,USERNAME,LASTTIME,ORDERID,REMARKNAME FROM 
			(SELECT * FROM IM_CURCLILIST WHERE SELLERID=t_id AND LASTTIME BETWEEN to_date(t_btime,'yyyy-mm-dd hh24:mi:ss') AND to_date(t_etime,'yyyy-mm-dd hh24:mi:ss') 
			ORDER BY LASTTIME DESC)WHERE ROWNUM<=t_maxcount)
			WHERE RdNo>t_pos AND RdNo<=t_endpos;
		end;
		elsif t_direct=1 then
		begin
			OPEN curcliInfo FOR
			SELECT USERNAME,to_char(LASTTIME,'yyyy-mm-dd hh24:mi:ss'),ORDERID,REMARKNAME FROM 
			(SELECT ROWNUM as RdNo,USERNAME,LASTTIME,ORDERID,REMARKNAME FROM 
			(SELECT * FROM IM_CURCLILIST WHERE SELLERID=t_id AND LASTTIME BETWEEN to_date(t_btime,'yyyy-mm-dd hh24:mi:ss') AND to_date(t_etime,'yyyy-mm-dd hh24:mi:ss') 
			ORDER BY LASTTIME)WHERE ROWNUM<=t_maxcount)
			WHERE RdNo>t_pos AND RdNo<=t_endpos;			
		end;
		end if;
		EXCEPTION
		when OTHERS then
		begin
			returncode := 1;
			raise;
			return;
		end;
	  end GetCustInfo;

  procedure ModifyGroup
  (
      t_userid in IM_FRIENDGROUP.UserId%type,
      t_groupid in IM_FRIENDGROUP.FriendGroupId%type,
      t_groupname in IM_FRIENDGROUP.FriendGroupName%type,
      returncode out integer
  )
  is
    tmpcount integer;
    begin
         returncode := 0;
         SELECT COUNT(UserId) INTO tmpcount FROM IM_FRIENDGROUP WHERE UserId=t_userid AND FriendGroupId=t_groupid;
         if tmpcount = 1 then
         begin
              SELECT COUNT(UserId) INTO tmpcount FROM IM_FRIENDGROUP WHERE UserId=t_userid AND FriendGroupName=t_groupname;
              if tmpcount = 0 then
              begin
                   UPDATE IM_FRIENDGROUP SET FriendGroupName=t_groupname WHERE UserId=t_userid AND FriendGroupId=t_groupid;
                   COMMIT;
              end;
              else
              begin
                   returncode := 9;
                   return;
              end;
              end if;
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
            return;
         end;

    end ModifyGroup;
  procedure DeleteGroup
  (
      t_uesrid in IM_FRIENDGROUP.UserId%type,
      t_groupid in IM_FRIENDGROUP.FriendGroupId%type,
      returncode out integer
  )
  is
    begin
         returncode := 0;
         DELETE FROM IM_FRIENDGROUP WHERE UserId=t_uesrid AND FriendGroupId=t_groupid;
         COMMIT;
         EXCEPTION
         when OTHERS then
         begin
            returncode := 1;
            return;
         end;
    end DeleteGroup;

  procedure SaveUserLogin
  (
       t_userid in IM_LOGINSTAT.UserId%type,
       t_userip in IM_LOGINSTAT.UserIp%type,
       t_userport in IM_USERONLINE.Srcport%type,
       t_serverno in IM_USERONLINE.ServerNo%type,
       returncode out integer
  )
  is
    begin
         returncode := 0;
         merge into IM_LOGINSTAT t1 using 
           (select t_userid as Userid,t_userip as UserIp, trunc(sysdate) as LoginDate from dual) t2
           on (t1.UserId=t2.UserId and t1.UserIp=t2.UserIp and t1.LoginDate>=t2.LoginDate)
           when matched then
           update set t1.LoginTimes=t1.LoginTimes+1
           when not matched then
           insert (UserId,UserIp) values (t_userid,t_userip);

         merge into IM_USERONLINE t1 using 
           (select t_userid as Userid from dual) t2
           on (t1.UserId=t2.UserId)
           when matched then
           update set t1.Status=1,t1.SrcIp=t_userip,t1.SrcPort=t_userport,t1.Serverno=t_serverno,t1.CreateTime=sysdate
           when not matched then
           insert (UserId,SrcIp,SrcPort,ServerNo,Status) values (t_userid,t_userip,t_userport,t_serverno,1);   
           
         COMMIT;
         EXCEPTION
         when OTHERS then
         begin
            returncode := 1;
            return;
         end;
    end SaveUserLogin;

  procedure SaveUserLogout
  (
       t_userid in IM_USERONLINE.UserId%type,
       returncode out integer
  )
  is
    begin
         returncode := 0;
         update IM_USERONLINE set Status=0,CreateTime=sysdate
           where UserId=t_userid;
         COMMIT;
         EXCEPTION
         when OTHERS then
         begin
            returncode := 1;
            return; 
         end;
    end SaveUserLogout;
 
  procedure UpdateAllLogout
  (
       t_serverno in IM_USERONLINE.ServerNo%type,
       returncode out integer
  )
  is
    begin
         returncode := 0;
         update IM_USERONLINE set Status=0,CreateTime=sysdate
           where ServerNo=t_serverno;
         COMMIT;
         EXCEPTION
         when OTHERS then
         begin
            returncode := 1;
            return; 
         end;
    end UpdateAllLogout;
       
  procedure SaveUserAction
  (
       t_userid in IM_OPSTAT.UserId%type,
       t_actionid in IM_OPSTAT.OpId%type,
       t_actioncount in IM_OPSTAT.OpCount%type,
       returncode out integer
  )
  is
    tmpcount integer;
    begin
         returncode := 0;
         SELECT COUNT(USERID) into tmpcount FROM IM_OPSTAT WHERE USERID = t_userid AND OpId = t_actionid AND OpTime > trunc(sysdate);
         if tmpcount > 0 then
             UPDATE IM_OPSTAT SET OpCount = OpCount + t_actioncount, OpTime = sysdate  WHERE USERID = t_userid AND OpId = t_actionid AND OpTime > trunc(sysdate);
         else
             INSERT INTO IM_OPSTAT (USERID,OpId,OpCount) VALUES(t_userid,t_actionid,t_actioncount);
         end if;
         COMMIT;
         EXCEPTION
         when OTHERS then
         begin
            returncode := 1;
            return;
         end;


    end Saveuseraction;

  procedure SaveUserTimelen
  (
      t_userid in IM_TIMELEN.Userid%type,
      t_timelen in IM_TIMELEN.TIMELEN%type,
      returncode out integer
  )
  is
    --tmpcount integer;
    begin
         returncode := 0;
/*         SELECT COUNT(USERID) INTO tmpcount FROM CLIENT_TIMELEN WHERE USERID =t_userid  AND CREATEDATE > trunc(sysdate) AND CREATEDATE < trunc(sysdate+1);
         if tmpcount > 0 then
            UPDATE CLIENT_TIMELEN SET TIMELEN=TIMELEN + t_timelen,CREATEDATE=sysdate WHERE USERID =t_userid
                   AND CREATEDATE > trunc(sysdate)
                   AND CREATEDATE < trunc(sysdate + 1); --TO_DATE(TO_CHAR(sysdate+1, 'YYYY-MM-DD'),'YYYY-MM-DD');
         else

         end if;*/
         INSERT INTO  IM_TIMELEN (USERID,TIMELEN) VALUES(t_userid, t_timelen);
         COMMIT;
          EXCEPTION
         when OTHERS then
         begin
            returncode := 1;
            return;
         end;
    end SaveUserTimelen;

  procedure SaveUserRecvFamilyMsg
  (
      t_familyid in IM_CLUSTERMEMBER.ClusterId%type,
      t_userid in IM_CLUSTERMEMBER.UserId%type,
      t_msgid in IM_CLUSTERMEMBER.MsgIdReceived%type,
      returncode out integer
  )
  is
    tmpcount integer;
    begin
         returncode := 0;
         SELECT COUNT(UserId) INTO tmpcount FROM IM_CLUSTERMEMBER WHERE ClusterId=t_familyid  AND  UserId=t_userid;
         if tmpcount > 0 then
             UPDATE IM_CLUSTERMEMBER SET MsgIdReceived=t_msgid WHERE ClusterId=t_familyid  AND  UserId=t_userid;
         else
             INSERT INTO IM_CLUSTERMEMBER (ClusterId,UserId,MsgIdReceived) VALUES (t_familyid,t_userid,t_msgid);
         end if;
         COMMIT;
          EXCEPTION
         when OTHERS then
         begin
            returncode := 1;
            return;
         end;
    end Saveuserrecvfamilymsg;

  procedure GetUserFamilyMsgID
  (
       t_userid in IM_CLUSTERMEMBER.UserId%type,
       t_familyid in IM_CLUSTERMEMBER.ClusterId%type,
       t_msgid out IM_CLUSTERMEMBER.MsgIdReceived%type,
       returncode out integer
  )
  is
    begin
          returncode := 0;
          SELECT MsgIdReceived INTO  t_msgid FROM IM_CLUSTERMEMBER WHERE ClusterId=t_familyid AND UserId=t_userid;
          EXCEPTION
          when NO_DATA_FOUND THEN
          begin
              returncode := 1403;
              return;
          end;
          when OTHERS then
          begin
             returncode := 1;
             return;
         end;
    end Getuserfamilymsgid;

  procedure DeleteAllOfflineMsg
  (
       t_userid in IM_OFFLINETALKMSG.Receiver%type,
       returncode out integer
  )
  is
    begin
         DELETE FROM IM_OFFLINESYSMSG WHERE Receiver=t_userid;
         --DELETE FROM IM_OFFLINETALKMSG WHERE Receiver=t_userid;
         COMMIT;
         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             return;
         end;
    end Deleteallofflinemsg;

  procedure SaveOnlineNum
  (
       t_onlinenum in IM_ONLINEUSER.Onlineuser%type,
       returncode out integer
  )
  is
    begin
         returncode := 0;
         INSERT INTO IM_ONLINEUSER(ONLINEUSER)VALUES(t_onlinenum);
         COMMIT;
         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             return;
         end;
    end SaveOnlineNum;

  procedure SetWokuMsgRecvID
  (
       t_userid in IM_USERSTAT.UserId%type,
       t_type   in integer,
       t_recid  in IM_USERSTAT.Usermessage%type,
       returncode out integer
  )
  is
    tmp integer;
    begin
         returncode := 0;
         tmp := 0;
         SELECT COUNT(1) into tmp
         FROM IM_USERSTAT where UserId = t_userid;
         if tmp = 0 then
         begin
              INSERT INTO IM_USERSTAT(UserId) VALUES(t_userid);
              commit;
         end;
         end if;
         if t_type = 0 then
            UPDATE IM_USERSTAT set useroperation=t_recid where UserId = t_userid;
         ELSIF  t_type = 1 then
            UPDATE IM_USERSTAT set usermessage=t_recid where UserId = t_userid;
         ELSIF t_type = 2 then
            UPDATE IM_USERSTAT set sysmessage=t_recid where UserId = t_userid;
         ELSIF t_type = 3 then
            UPDATE IM_USERSTAT set BROADCASTINFO=t_recid where UserId = t_userid;
         ELSIF t_type = 4 then
            --UPDATE IM_USERSTAT set USERCOMMENT=t_recid where UserId = t_userid;
            null;
         ELSIF t_type = 5 then
            --UPDATE IM_USERSTAT set USERLOGCOMMENT=t_recid where UserId = t_userid;
            null;
         end if;
         COMMIT;
         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             return;
         end;
    end SetWokuMsgRecvID;

  procedure GetWokuMsgID
  (
       t_userid in IM_USERSTAT.UserId%type,
       t_nOperationID out IM_USERSTAT.Useroperation%type,
       t_nUserMsgID  out IM_USERSTAT.Usermessage%type,
       t_nSysMsgID  out IM_USERSTAT.Sysmessage%type,
       t_nBroadcastID  out IM_USERSTAT.Broadcastinfo%type,
       t_nUsercommentID  out number,
       t_nLogcommentID  out number,
       returncode out integer
  )
  is
    begin
          returncode := 0;
          SELECT Useroperation,Usermessage,Sysmessage,Broadcastinfo,0,0
          into   t_nOperationID,t_nUserMsgID,t_nSysMsgID,t_nBroadcastID,t_nUsercommentID,t_nLogcommentID
          FROM IM_USERSTAT
          where UserId = t_userid;
          EXCEPTION
          when NO_DATA_FOUND THEN
          begin
                returncode := 1403;
                return;
          end;
          when OTHERS then
          begin
              returncode := 1;
              return;
          end;

    end GetWokuMsgID;

  procedure GetWebIMMsg
  (
       t_serverno in IM_USERONLINE.ServerNo%type,
       t_count in out integer,
       cur_webimmsg out T_CURSOR,
       returncode out integer
  )
  is
    begin
         returncode := 0;
         OPEN cur_webimmsg FOR
         	select t1.MsgId,t1.Receiver,t1.Sender,
         	(cast(cast(t1.SendTime as timestamp) at time zone 'gmt' as date)-to_date('19700101 00:00:00','yyyymmdd hh24:mi:ss'))*24*60*60,
         	t1.MsgType,t1.MsgLen,t1.Msg,t1.ReceiverName,t1.SenderName,t1.Text
         	from IM_OFFLINETALKMSG t1, IM_USERONLINE t2
         	where t2.ServerNo=t_serverno and t2.status=0 and t1.Receiver=t2.UserId
            and rownum <= t_count
         	order by t1.Receiver,t1.MsgId;
         exception
         when others then
         begin
             returncode := 1;
             raise;
             return;
         end;
    end GetWebIMMsg;
    
    
   procedure SaveBuyerLogin
  (
       t_userid in IM_LOGINOUTTIME.UserId%type,
       t_logintime in IM_LOGINOUTTIME.LOGINTIME%type,
       returncode out integer
  )
  is
       tmp integer;
    begin
         returncode := 0;
         tmp := 0;
         
         SELECT COUNT(1)  into tmp
          FROM IM_LOGINOUTTIME
          where UserId = t_userid;
          
          if tmp = 0 then
          begin
              INSERT INTO IM_LOGINOUTTIME(UserId,LOGINTIME) VALUES (t_userid,t_logintime);
              commit;
          end;
          else
          begin
              update IM_LOGINOUTTIME set LOGINTIME = t_logintime,LOGOUTTIME = 0 where UserId=t_userid;
              commit;
          end;  
          end if;
          
          returncode := 1;
    end SaveBuyerLogin;



  procedure SaveBuyerLogout
  (
       t_userid in IM_LOGINOUTTIME.UserId%type,
       t_logouttime in IM_LOGINOUTTIME.LOGOUTTIME%type,
       returncode out integer
  )
  is
       tmp integer;
    begin
         returncode := 0;
         tmp := 0;
         
         SELECT COUNT(1)  into tmp
          FROM IM_LOGINOUTTIME
          where UserId = t_userid;
          if tmp = 0 then
          begin
              INSERT INTO IM_LOGINOUTTIME(UserId,LOGINTIME,LOGOUTTIME) VALUES (t_userid,t_logouttime,t_logouttime);
              commit;
          end;
          else
          begin
              update IM_LOGINOUTTIME set LOGINTIME = t_logouttime, LOGOUTTIME = t_logouttime where UserId=t_userid;
              commit;
          end;  
          end if;
          
          returncode := 1;
    end SaveBuyerLogout; 
    
end Mini_Local_PKG;
/

prompt
prompt Creating package body Mini_User_Pkg
prompt =====================================
prompt

create or replace package body im.Mini_User_Pkg is

  -- Author  : LVWL
  -- Created : 2007-1-19 16:59:46
  -- Purpose : 用户个人信息处理包
  procedure GetUserPwd
  (
    t_username in IM_USER.Username%type,         --用户名
    t_userId out IM_USER.userId%type,            --用户ID
    t_userpwd out IM_USER.Password%type,     -- 密码
    ReturnCode out integer                               --返回参数,错误代码 0=正常 17 未激活
  )
  is
    t_active IM_USER.status%type;
    begin
         ReturnCode := 0;
         select userid, Password,Status into t_userId, t_userpwd,t_active
         from IM_USER
         where username = t_username;
         if  t_active <> 3 then
         begin
              returncode := 17;
              return;
         end;
         end if;
         EXCEPTION
         when NO_DATA_FOUND THEN
         begin
              returncode := 1403;
              return;
         end;
         when OTHERS then
         begin
             ReturnCode := 2;
             return;
         end;

    end GetUserPwd;

   procedure GetUserPwdByID
  (
    t_userId in IM_USER.userId%type,            --用户ID
    t_userpwd out IM_USER.Password%type,     -- 密码
    t_username out IM_USER.Username%type,         --用户名
    t_mobileno out IM_USER.Mobile%type,
    ReturnCode out integer                               --返回参数,错误代码 0=正常 17 未激活
  )
  is
    t_active IM_USER.status%type;
    begin
         ReturnCode := 0;
         select username,password,mobile,status into t_username,t_userpwd,t_mobileno,t_active
         from IM_USER
         where userid = t_userid;
         if  t_active <> 3 then
         begin
              returncode := 17;
              return;
         end;
         end if;
         EXCEPTION
         when NO_DATA_FOUND THEN
         begin
              returncode := 1403;
              return;
         end;
         when OTHERS then
         begin
             ReturnCode := 2;
             return;
         end;

    end GetUserPwdByID;

  procedure GetUserPwdByMobile
  (
    t_mobileno in IM_USER.Mobile%type,
    t_username out IM_USER.Username%type,         --用户名
    t_userId out IM_USER.userId%type,            --用户ID
    t_userpwd out IM_USER.Password%type,     -- 密码
    ReturnCode out integer                               --返回参数,错误代码 0=正常 17 未激活
  )
  is
    t_active IM_USER.status%type;
    begin
         ReturnCode := 0;
         select userid, username,password,status into t_userId,t_username,t_userpwd,t_active
         from IM_USER
         where mobile = t_mobileno;
         if  t_active <> 3 then
         begin
              returncode := 17;
              return;
         end;
         end if;
         EXCEPTION
         when NO_DATA_FOUND THEN
         begin
              returncode := 1403;
              return;
         end;
         when OTHERS then
         begin
             ReturnCode := 2;
             return;
         end;

    end GetUserPwdByMobile;

  procedure AccountSave
  (
  	t_id in IM_USER.userId%type,
	t_account in IM_USER.Username%type,
	t_pwd in IM_USER.Password%type,
	t_truename in IM_USERINFO.NICKNAME%type,
	ReturnCode out integer
  )	
  is
  	t_num integer;
  	begin
  		returncode := 0;
  		SELECT COUNT(1) INTO t_num FROM IM_USER WHERE USERID=t_id;
		IF t_num=0 THEN
		begin
			INSERT INTO IM_USER(USERID,USERNAME,PASSWORD) VALUES(t_id,t_account,t_pwd);
			INSERT INTO IM_USERINFO(USERID,NICKNAME) VALUES(t_id,t_truename);
			COMMIT;
		end;
		ELSIF t_num>1 THEN
		begin
			returncode := 1;
			return;
		end;
		end IF;
		EXCEPTION
		when OTHERS then
		begin
			returncode := 1;
			rollback;
			return;
		end;
  	end AccountSave;

  procedure AccountSave_20150916
  (
  	t_id in IM_USER.userId%type,
	t_account in IM_USER.Username%type,
	t_pwd in IM_USER.Password%type,
	t_truename in IM_USERINFO.NICKNAME%type,
	ReturnCode out integer
  )	
  is
  	t_num integer;
  	begin
  		returncode := 0;
  		SELECT COUNT(1) INTO t_num FROM IM_USER WHERE USERID=t_id;
		IF t_num=0 THEN
		begin
			INSERT INTO IM_USER(USERID,USERNAME,PASSWORD) VALUES(t_id,t_account,t_pwd);
			INSERT INTO IM_USERINFO(USERID,NICKNAME) VALUES(t_id,t_truename);
			COMMIT;
		end;
		ELSIF t_num=1 THEN
		begin
			UPDATE IM_USER SET USERNAME=t_account,PASSWORD=t_pwd WHERE USERID=t_id;
			UPDATE IM_USERINFO SET NICKNAME = t_truename WHERE USERID = t_id;
			COMMIT;
		end;
		ELSIF t_num>1 THEN
		begin
			returncode := 1;
			return;
		end;
		end IF;
		EXCEPTION
		when OTHERS then
		begin
			returncode := 1;
			rollback;
			return;
		end;
  	end AccountSave_20150916;



 procedure AccountSaveWork
  (
	t_id out IM_USER.userid%type,
	t_account in IM_USER.Username%type,
	t_pwd in IM_USER.Password%type,
	t_mobileno in IM_USER.Mobile%type,
	t_emialaddress in IM_USER.Email%type,
	t_nickname in IM_USERINFO.NICKNAME%type,
	t_headImg in IM_USERINFO.USERPICURI%type,
	t_groupname in IM_USERINFO.GROUPNAME%type,
	ReturnCode out integer

  )
  is
    t_cnt integer;
    t_sized IM_USER.Sizeds%type;
    t_mobile IM_USER.Mobile%type;
    t_email IM_USER.Email%type;
    t_nick IM_USERINFO.NICKNAME%type;
    t_headurl IM_USERINFO.USERPICURI%type;
  	begin
      t_id := 0;
      returncode := 0;
      SELECT COUNT(userid) INTO t_cnt FROM IM_USER WHERE USERNAME=t_account and USERID>=2000000000;
      IF t_cnt=0 THEN
        begin
          SELECT IM_WORK_SEQ_USERID.NEXTVAL into t_id FROM DUAL;
          INSERT INTO IM_USER(USERID,USERNAME,PASSWORD,MOBILE,EMAIL) VALUES(t_id,t_account,t_pwd,t_mobileno,t_emialaddress);
          INSERT INTO IM_USERINFO(USERID,NICKNAME,USERPICURI,GROUPNAME) VALUES(t_id,t_nickname,t_headImg,t_groupname);
          COMMIT;
        end;
      ELSIF t_cnt=1 THEN
        begin
          SELECT userid,Sizeds,Mobile,Email INTO t_id,t_sized,t_mobile,t_email FROM IM_USER WHERE USERNAME=t_account and USERID>= 2000000000;
			IF t_id >= 2000000000 and t_id < 2050000000 THEN
				begin
					returncode := 2;
					return;
				end;
			END IF;
          SELECT NICKNAME,USERPICURI INTO t_nick,t_headurl FROM IM_USERINFO WHERE USERID=t_id;
          UPDATE IM_USER SET Mobile=t_mobileno,Email= t_emialaddress where USERID=t_id;
          UPDATE IM_USERINFO SET NICKNAME=t_nickname, USERPICURI=t_headImg,GROUPNAME=t_groupname where USERID=t_id;
          COMMIT;
        end;
      ELSE
        begin
          returncode := 1;
          return;
        end;
      END IF;
      EXCEPTION
      when OTHERS then
      begin
        returncode := 1;
        rollback;
        return;
      end;
  	end AccountSaveWork;

  	procedure AccountSaveBuyer_20150907
  (
		t_id out IM_USER.userid%type,
		t_account in IM_USER.Username%type,
		t_pwd in IM_USER.Password%type,
		t_guid in IM_USER.Sizeds%type,
		t_mobileno in IM_USER.Mobile%type,
		t_emialaddress in IM_USER.Email%type,
		t_nickname in IM_USERINFO.NICKNAME%type,
		t_headImg in IM_USERINFO.USERPICURI%type,
	ReturnCode out integer

  )
  is
    t_cnt integer;
    t_sized IM_USER.Sizeds%type;
    t_mobile IM_USER.Mobile%type;
    t_email IM_USER.Email%type;
    t_nick IM_USERINFO.NICKNAME%type;
    t_headurl IM_USERINFO.USERPICURI%type;
  	begin
      t_id := 0;
      returncode := 0;
      SELECT COUNT(userid) INTO t_cnt FROM IM_USER WHERE USERNAME=t_account and USERID>=5000000 and USERID < 2000000000;
      IF t_cnt=0 THEN
        begin
          INSERT INTO IM_USER(USERNAME,PASSWORD,SIZEDS,MOBILE,EMAIL) VALUES(t_account,t_pwd,t_guid,t_mobileno,t_emialaddress);
          SELECT IM_USER_SEQ_USERID.CURRVAL into t_id FROM DUAL;
          INSERT INTO IM_USERINFO(USERID,NICKNAME,USERPICURI) VALUES(t_id,t_nickname,t_headImg);
          COMMIT;
        end;
      ELSIF t_cnt=1 THEN
        begin
          SELECT userid,Sizeds,Mobile,Email INTO t_id,t_sized,t_mobile,t_email FROM IM_USER WHERE USERNAME=t_account and USERID>=5000000 and USERID< 2000000000;
          SELECT NICKNAME,USERPICURI INTO t_nick,t_headurl FROM IM_USERINFO WHERE USERID=t_id;
          IF t_sized is NULL  or t_sized != t_guid  THEN
            begin
              UPDATE IM_USER SET Sizeds=t_guid where USERID=t_id;
              end;
            END IF;
             UPDATE IM_USER SET Mobile=t_mobileno,Email= t_emialaddress where USERID=t_id;
             UPDATE IM_USERINFO SET NICKNAME=t_nickname, USERPICURI=t_headImg where USERID=t_id;
          COMMIT;
        end;
      ELSE
        begin
          returncode := 1;
          return;
        end;
      END IF;
      EXCEPTION
      when OTHERS then
      begin
        returncode := 1;
        rollback;
        return;
      end;
  	end AccountSaveBuyer_20150907;

  	

procedure AccountSaveBuyer_20150728
  (
		t_id out IM_USER.userid%type,
		t_account in IM_USER.Username%type,
		t_pwd in IM_USER.Password%type,
		t_guid in IM_USER.Sizeds%type,
		t_mobileno in IM_USER.Mobile%type,
		t_emialaddress in IM_USER.Email%type,
		t_nickname in IM_USERINFO.NICKNAME%type,
		
	ReturnCode out integer

  )
  is
    t_cnt integer;
    t_sized IM_USER.Sizeds%type;
    t_mobile IM_USER.Mobile%type;
    t_email IM_USER.Email%type;
    t_nick IM_USERINFO.NICKNAME%type;
  	begin
      t_id := 0;
      returncode := 0;
      SELECT COUNT(userid) INTO t_cnt FROM IM_USER WHERE USERNAME=t_account and USERID>=5000000;
      IF t_cnt=0 THEN
        begin
          INSERT INTO IM_USER(USERNAME,PASSWORD,SIZEDS,MOBILE,EMAIL) VALUES(t_account,t_pwd,t_guid,t_mobileno,t_emialaddress);
          SELECT IM_USER_SEQ_USERID.CURRVAL into t_id FROM DUAL;
          INSERT INTO IM_USERINFO(USERID,NICKNAME) VALUES(t_id,t_nickname);
          COMMIT;
        end;
      ELSIF t_cnt=1 THEN
        begin
          SELECT userid,Sizeds,Mobile,Email INTO t_id,t_sized,t_mobile,t_email FROM IM_USER WHERE USERNAME=t_account and USERID>=5000000;
          SELECT NICKNAME INTO t_nick FROM IM_USERINFO WHERE USERID=t_id;
          IF t_sized is NULL  or t_sized != t_guid  THEN
            begin
              UPDATE IM_USER SET Sizeds=t_guid where USERID=t_id;
              end;
            END IF;
             UPDATE IM_USER SET Mobile=t_mobileno,Email= t_emialaddress where USERID=t_id;
             UPDATE IM_USERINFO SET NICKNAME=t_nickname where USERID=t_id;
          COMMIT;
        end;
      ELSE
        begin
          returncode := 1;
          return;
        end;
      END IF;
      EXCEPTION
      when OTHERS then
      begin
        returncode := 1;
        rollback;
        return;
      end;
  	end AccountSaveBuyer_20150728;
  	
procedure AccountSaveBuyer_20150623
  (
    	t_id out IM_USER.userid%type,
	t_account in IM_USER.Username%type,
	t_pwd in IM_USER.Password%type,
    	t_guid in IM_USER.Sizeds%type,
        t_mobileno in IM_USER.Mobile%type,
        t_emialaddress in IM_USER.Email%type, 
	ReturnCode out integer

  )
  is
    t_cnt integer;
    t_sized IM_USER.Sizeds%type;
    t_mobile IM_USER.Mobile%type;
    t_email IM_USER.Email%type;
  	begin
      t_id := 0;
      returncode := 0;
      SELECT COUNT(userid) INTO t_cnt FROM IM_USER WHERE USERNAME=t_account and USERID>=5000000;
      IF t_cnt=0 THEN
        begin
          INSERT INTO IM_USER(USERNAME,PASSWORD,SIZEDS,MOBILE,EMAIL) VALUES(t_account,t_pwd,t_guid,t_mobileno,t_emialaddress);
          SELECT IM_USER_SEQ_USERID.CURRVAL into t_id FROM DUAL;
          INSERT INTO IM_USERINFO(USERID,NICKNAME) VALUES(t_id,t_account);
          COMMIT;
        end;
      ELSIF t_cnt=1 THEN
        begin
          SELECT userid,Sizeds,Mobile,Email INTO t_id,t_sized,t_mobile,t_email FROM IM_USER WHERE USERNAME=t_account and USERID>=5000000;
          IF t_sized is NULL  or t_sized != t_guid  THEN
            begin
              UPDATE IM_USER SET Sizeds=t_guid where USERID=t_id;
              end;
            END IF;
             UPDATE IM_USER SET Mobile=t_mobileno,Email= t_emialaddress where USERID=t_id;
          COMMIT;
        end;
      ELSE
        begin
          returncode := 1;
          return;
        end;
      END IF;
      EXCEPTION
      when OTHERS then
      begin
        returncode := 1;
        rollback;
        return;
      end;
  	end AccountSaveBuyer_20150623;

  procedure AccountSaveBuyer_20150508
  (
    	t_id out IM_USER.userid%type,
	t_account in IM_USER.Username%type,
	t_pwd in IM_USER.Password%type,
    	t_guid in IM_USER.Sizeds%type,
     t_mobileno in IM_USER.Mobile%type,
     t_emialaddress in IM_USER.Email%type, 
	ReturnCode out integer

  )
  is
    t_cnt integer;
    t_sized IM_USER.Sizeds%type;
    t_mobile IM_USER.Mobile%type;
    t_email IM_USER.Email%type;
  	begin
      t_id := 0;
  		returncode := 0;
      SELECT COUNT(userid) INTO t_cnt FROM IM_USER WHERE USERNAME=t_account and USERID>=5000000;
      IF t_cnt=0 THEN
        begin
          INSERT INTO IM_USER(USERNAME,PASSWORD,SIZEDS,MOBILE,EMAIL) VALUES(t_account,t_pwd,t_guid,t_mobileno,t_emialaddress);
          SELECT IM_USER_SEQ_USERID.CURRVAL into t_id FROM DUAL;
          INSERT INTO IM_USERINFO(USERID,NICKNAME) VALUES(t_id,t_account);
          COMMIT;
        end;
      ELSIF t_cnt=1 THEN
        begin
          SELECT userid,Sizeds,Mobile,Email INTO t_id,t_sized,t_mobile,t_email FROM IM_USER WHERE USERNAME=t_account and USERID>=5000000;
          IF t_sized is NULL  or t_sized != t_guid  THEN
            begin
              UPDATE IM_USER SET Sizeds=t_guid where USERID=t_id;
              end;
            END IF;
            IF t_mobile is NULL  or t_mobile != t_mobileno  THEN
            begin
              UPDATE IM_USER SET Mobile=t_mobileno where USERID=t_id;
              end;
            END IF;
               IF t_email is NULL  or t_email != t_emialaddress  THEN
            begin
              UPDATE IM_USER SET Email= t_emialaddress where USERID=t_id;
              end;
            END IF;
          COMMIT;
        end;
      ELSE
        begin
          returncode := 1;
          return;
        end;
      END IF;
      EXCEPTION
      when OTHERS then
      begin
        returncode := 1;
        rollback;
        return;
      end;
  	end AccountSaveBuyer_20150508;
	
  procedure AccountSaveBuyer
  (
    	t_id out IM_USER.userid%type,
	t_account in IM_USER.Username%type,
	t_pwd in IM_USER.Password%type,
    	t_guid in IM_USER.Sizeds%type,
        t_mobileno in IM_USER.Mobile%type,
        t_emialaddress in IM_USER.Email%type, 
	ReturnCode out integer

  )
  is
    t_cnt integer;
    t_sized IM_USER.Sizeds%type;
    t_mobile IM_USER.Mobile%type;
    t_email IM_USER.Email%type;
  	begin
      t_id := 0;
      returncode := 0;
      SELECT COUNT(userid) INTO t_cnt FROM IM_USER WHERE USERNAME=t_account and USERID>=5000000;
      IF t_cnt=0 THEN
        begin
          INSERT INTO IM_USER(USERNAME,PASSWORD,SIZEDS,MOBILE,EMAIL) VALUES(t_account,t_pwd,t_guid,t_mobileno,t_emialaddress);
          SELECT IM_USER_SEQ_USERID.CURRVAL into t_id FROM DUAL;
          INSERT INTO IM_USERINFO(USERID,NICKNAME) VALUES(t_id,t_account);
          COMMIT;
        end;
      ELSIF t_cnt=1 THEN
        begin
          SELECT userid,Sizeds,Mobile,Email INTO t_id,t_sized,t_mobile,t_email FROM IM_USER WHERE USERNAME=t_account and USERID>=5000000;
          IF t_sized is NULL  or t_sized != t_guid  THEN
            begin
              UPDATE IM_USER SET Sizeds=t_guid where USERID=t_id;
              end;
            END IF;
             UPDATE IM_USER SET Mobile=t_mobileno,Email= t_emialaddress where USERID=t_id;
          COMMIT;
        end;
      ELSE
        begin
          returncode := 1;
          return;
        end;
      END IF;
      EXCEPTION
      when OTHERS then
      begin
        returncode := 1;
        rollback;
        return;
      end;
  	end AccountSaveBuyer;
    
	  procedure SubAccountSave
	(
		t_uid in IM_USER.userId%type,
		t_fid in IM_FRIEND.friendId%type,
		t_gid in IM_FRIEND.friendGroupId%type,
		t_nickname in IM_USERINFO.nickName%type,
		t_pwd in IM_USER.passWord%type,
		t_truename in IM_FRIEND.aliasName%type,
		ReturnCode out integer
	)
	is
	  t_num integer;
	begin
	  ReturnCode := 0;
	  SELECT COUNT(1) INTO t_num FROM IM_USER WHERE USERID=t_fid;
	  IF t_num=0 THEN
	  begin
		  INSERT INTO IM_USER(USERID,USERNAME,PASSWORD) VALUES(t_fid,t_nickname,t_pwd);
		  INSERT INTO IM_USERINFO(USERID,NICKNAME) VALUES(t_fid,t_truename);
		  COMMIT;
	  end;
	  ELSIF t_num>1 THEN
	  begin
		  ReturnCode := 1;
		  return;
	  end;
	  end IF;
	  SELECT COUNT(1) INTO t_num FROM IM_FRIEND WHERE USERID=t_uid AND FRIENDID=t_fid;
	  IF t_num=0 THEN
	  begin
		  INSERT INTO IM_FRIEND(USERID,FRIENDID) VALUES(t_uid,t_fid);
		  COMMIT;
	  end;
	  ELSIF t_num>1 THEN
	  begin
		  ReturnCode := 1;
		  return;
	  end;
	  end IF;
	  EXCEPTION
	  when OTHERS then
	  begin
		  ReturnCode := 1;
		  rollback;
		  return;
	  end;
	end SubAccountSave;

  procedure GetUserInfo_20151125
  (
     t_userid in IM_USER.Userid%type,
     t_username out IM_USER.username%type,
     t_nickname out IM_USER.Company%type,
     t_usersex out IM_USER.Gender%type,
     t_usersign out IM_USERINFO.Usersign%type,
     t_userpicuri out IM_USERINFO.Userpicuri%type,
     t_email out IM_USER.Email%type,
     t_joinauthflag out IM_USERINFO.Joinauthflag%type,
     t_mphone out IM_USER.MOBILE%type,
     t_userdesc out IM_USERINFO.Userdesc%type,
     t_nation out IM_USERINFO.Nation%type,
     t_province out IM_USERINFO.Province%type,
     t_city out IM_USERINFO.City%type,
     t_birthday out IM_USERINFO.Birthday%type,
     t_join2family out IM_USERINFO.Join2family%type,
     t_useraddress out IM_USERINFO.Usraddress%type,
     t_linkpop out IM_USERINFO.linkpop%type,
     t_usermoney out number,
     t_avatoruri out varchar2,
     t_mobilestatus out number,
     t_integral     out IM_USERINFO.integral%type,
     t_career out IM_USERINFO.career%type,
     t_realname out IM_CSINFO.realname%type,
     returncode out integer
  )
  is
     begin
          returncode := 0;

          select username,Gender,EMAIL,MOBILE,Company
            into t_username,t_usersex,t_email,t_mphone,t_avatoruri
            from IM_USER where userid=t_userid;

          select nickname,usersign,USERPICURI,CITY,NATION,PROVINCE,
              JOINAUTHFLAG,USERDESC,BIRTHDAY,JOIN2FAMILY,USRADDRESS,LINKPOP,0,Integral,0,CAREER
            into t_nickname,t_usersign,t_userpicuri,t_city,t_nation,t_province,
               t_joinauthflag,t_userdesc,t_birthday,t_join2family,t_useraddress,
               t_linkpop,t_mobilestatus,t_integral,t_usermoney,t_career
            from IM_USERINFO where userid=t_userid;
            
          select realname into t_realname from IM_CSINFO where userid=t_userid;  

          EXCEPTION
          when NO_DATA_FOUND THEN
          begin
               returncode := 1403;
               return;
          end;
          when OTHERS then
          begin
               returncode := 1;
               raise;
               return;
          end;
          
     end GetUserInfo_20151125;


procedure GetUserInfo_20160224
  (
     t_userid in IM_USER.Userid%type,
     t_username out IM_USER.username%type,
     t_nickname out IM_USER.Company%type,
     t_usersex out IM_USER.Gender%type,
     t_usersign out IM_USERINFO.Usersign%type,
     t_userpicuri out IM_USERINFO.Userpicuri%type,
     t_email out IM_USER.Email%type,
     t_joinauthflag out IM_USERINFO.Joinauthflag%type,
     t_mphone out IM_USER.MOBILE%type,
     t_userdesc out IM_USERINFO.Userdesc%type,
     t_nation out IM_USERINFO.Nation%type,
     t_province out IM_USERINFO.Province%type,
     t_city out IM_USERINFO.City%type,
     t_birthday out IM_USERINFO.Birthday%type,
     t_join2family out IM_USERINFO.Join2family%type,
     t_useraddress out IM_USERINFO.Usraddress%type,
     t_linkpop out IM_USERINFO.linkpop%type,
     t_usermoney out number,
     t_avatoruri out varchar2,
     t_mobilestatus out number,
     t_integral     out IM_USERINFO.integral%type,
     t_career out IM_USERINFO.career%type,
     t_realname out IM_CSINFO.realname%type,
     t_cslimit out IM_CSINFO.receptionlimit%type,
     returncode out integer
  )
  is
     begin
          returncode := 0;

          select username,Gender,EMAIL,MOBILE,Company
            into t_username,t_usersex,t_email,t_mphone,t_avatoruri
            from IM_USER where userid=t_userid;

          select nickname,usersign,USERPICURI,CITY,NATION,PROVINCE,
              JOINAUTHFLAG,USERDESC,BIRTHDAY,JOIN2FAMILY,USRADDRESS,LINKPOP,0,Integral,0,CAREER
            into t_nickname,t_usersign,t_userpicuri,t_city,t_nation,t_province,
               t_joinauthflag,t_userdesc,t_birthday,t_join2family,t_useraddress,
               t_linkpop,t_mobilestatus,t_integral,t_usermoney,t_career
            from IM_USERINFO where userid=t_userid;
            
          select realname,receptionlimit into t_realname,t_cslimit from IM_CSINFO where userid=t_userid;  

          EXCEPTION
          when NO_DATA_FOUND THEN
          begin
               returncode := 1403;
               return;
          end;
          when OTHERS then
          begin
               returncode := 1;
               raise;
               return;
          end;
          
     end GetUserInfo_20160224;
     
  procedure GetUserInfo
  (
     t_userid in IM_USER.Userid%type,
     t_username out IM_USER.username%type,
     t_nickname out IM_USER.Company%type,
     t_usersex out IM_USER.Gender%type,
     t_usersign out IM_USERINFO.Usersign%type,
     t_userpicuri out IM_USERINFO.Userpicuri%type,
     t_email out IM_USER.Email%type,
     t_joinauthflag out IM_USERINFO.Joinauthflag%type,
     t_mphone out IM_USER.MOBILE%type,
     t_userdesc out IM_USERINFO.Userdesc%type,
     t_nation out IM_USERINFO.Nation%type,
     t_province out IM_USERINFO.Province%type,
     t_city out IM_USERINFO.City%type,
     t_birthday out IM_USERINFO.Birthday%type,
     t_join2family out IM_USERINFO.Join2family%type,
     t_useraddress out IM_USERINFO.Usraddress%type,
     t_linkpop out IM_USERINFO.linkpop%type,
     t_usermoney out number,
     t_avatoruri out varchar2,
     t_mobilestatus out number,
     t_integral     out IM_USERINFO.integral%type,
     t_career out IM_USERINFO.career%type,
     returncode out integer
  )
  is
     begin
          returncode := 0;

          select username,Gender,EMAIL,MOBILE,Company
            into t_username,t_usersex,t_email,t_mphone,t_avatoruri
            from IM_USER where userid=t_userid;

          select nickname,usersign,USERPICURI,CITY,NATION,PROVINCE,
              JOINAUTHFLAG,USERDESC,BIRTHDAY,JOIN2FAMILY,USRADDRESS,LINKPOP,0,Integral,0,CAREER
            into t_nickname,t_usersign,t_userpicuri,t_city,t_nation,t_province,
               t_joinauthflag,t_userdesc,t_birthday,t_join2family,t_useraddress,
               t_linkpop,t_mobilestatus,t_integral,t_usermoney,t_career
            from IM_USERINFO where userid=t_userid;

          EXCEPTION
          when NO_DATA_FOUND THEN
          begin
               returncode := 1403;
               return;
          end;
          when OTHERS then
          begin
               returncode := 1;
               raise;
               return;
          end;
      end GetUserInfo;
      

procedure GetUserInfo_20160302
(
	 t_userid in IM_USER.Userid%type,
	 t_username out IM_USER.username%type,
	 t_nickname out IM_USER.Company%type,
	 t_usersex out IM_USER.Gender%type,
	 t_usersign out IM_USERINFO.Usersign%type,
	 t_userpicuri out IM_USERINFO.Userpicuri%type,
	 t_email out IM_USER.Email%type,
	 t_joinauthflag out IM_USERINFO.Joinauthflag%type,
	 t_mphone out IM_USER.MOBILE%type,
	 t_userdesc out IM_USERINFO.Userdesc%type,
	 t_nation out IM_USERINFO.Nation%type,
	 t_province out IM_USERINFO.Province%type,
	 t_city out IM_USERINFO.City%type,
	 t_birthday out IM_USERINFO.Birthday%type,
	 t_join2family out IM_USERINFO.Join2family%type,
	 t_useraddress out IM_USERINFO.Usraddress%type,
	 t_linkpop out IM_USERINFO.linkpop%type,
	 t_usermoney out number,
	 t_avatoruri out varchar2,
	 t_mobilestatus out number,
	 t_integral     out IM_USERINFO.integral%type,
	 t_career out IM_USERINFO.career%type,
	 t_groupname out IM_USERINFO.GROUPNAME%type,
	 returncode out integer
)
  is
     begin
          returncode := 0;

          select username,Gender,EMAIL,MOBILE,Company
            into t_username,t_usersex,t_email,t_mphone,t_avatoruri
            from IM_USER where userid=t_userid;

          select nickname,usersign,USERPICURI,CITY,NATION,PROVINCE,
              JOINAUTHFLAG,USERDESC,BIRTHDAY,JOIN2FAMILY,USRADDRESS,LINKPOP,0,Integral,0,CAREER,GROUPNAME
            into t_nickname,t_usersign,t_userpicuri,t_city,t_nation,t_province,
               t_joinauthflag,t_userdesc,t_birthday,t_join2family,t_useraddress,
               t_linkpop,t_mobilestatus,t_integral,t_usermoney,t_career,t_groupname
            from IM_USERINFO where userid=t_userid;

          EXCEPTION
          when NO_DATA_FOUND THEN
          begin
               returncode := 1403;
               return;
          end;
          when OTHERS then
          begin
               returncode := 1;
               raise;
               return;
          end;
      end GetUserInfo_20160302;
      
  procedure Get_Agent_Merchant 
  (
     t_userid in  T_MERCHANT_DIVERSE_CONFIG.M_MERCHANT_ID%type,
     t_merchant out T_MERCHANT_DIVERSE_CONFIG.M_PROXY_SUBACCOUNT_ID%type,
      returncode out integer
  )  
   is
     begin
          returncode := 0;

          select m_proxy_subaccount_id
            into t_merchant
            from T_MERCHANT_DIVERSE_CONFIG where m_merchant_id = t_userid;          

          EXCEPTION
          when NO_DATA_FOUND THEN
          begin
               returncode := 1403;
               return;
          end;
          when OTHERS then
          begin
               returncode := 1;
               raise;
               return;
          end;
      end Get_Agent_Merchant;  
  
/*
  procedure GetFriendsList
  (
       t_userid in IM_FRIEND.userid%type,
       t_startpos in integer,
       t_reqcount in integer,
       curfriends out T_CURSOR,
       returncode out integer,
       t_flag in integer
  )
  is
     t_endpos integer;
     begin
          returncode := 0;
          t_endpos := t_startpos + t_reqcount;
		  if t_flag = 253 then
		  begin
          	OPEN curfriends FOR
          	SELECT FriendId,FriendGroupId
          	FROM
              ( select rownum as RdNo, FriendId,FriendGroupId FROM IM_FRIEND WHERE USERID=t_userid and rownum <= t_endpos)
          	WHERE RdNo > t_startpos and RdNo <= t_endpos;
		  end;
		  else
		  begin	  	
		  	OPEN curfriends FOR
		  	SELECT FriendId,FriendGroupId
		  	FROM
				( select rownum as RdNo, FriendId,FriendGroupId FROM IM_FRIEND WHERE USERID=t_userid and FriendGroupId<>250 and rownum <= t_endpos)
		  	WHERE RdNo > t_startpos and RdNo <= t_endpos;
		  end;
		  end if;
          EXCEPTION
          when OTHERS then
          begin
              returncode := 1;
              raise;
              return;
          end;

     end GetFriendsList;*/
    procedure GetFriendsList
  (
       t_userid in IM_FRIEND.userid%type,
       t_startpos in integer,
       t_reqcount in integer,
       curfriends out T_CURSOR,
       returncode out integer,
       t_flag in integer
  )
  is
     t_endpos integer;
     begin
          returncode := 0;
          t_endpos := t_startpos + t_reqcount;
          OPEN curfriends FOR
          SELECT FriendId,FriendGroupId
          FROM
             ( select rownum as RdNo, FriendId,FriendGroupId FROM IM_FRIEND WHERE USERID=t_userid and rownum <= t_endpos)
          WHERE RdNo > t_startpos and RdNo <= t_endpos;
          EXCEPTION
          when OTHERS then
          begin
              returncode := 1;
              raise;
              return;
          end;
     end GetFriendsList;

  
   procedure GetSubAccount
  (
       t_groupid in T_SERVICE_GROUP_MAPPING.s_group_id%type,
       cursubaccount out T_CURSOR,
       returncode out integer
  )
  is
  	begin
  		returncode := 0;
  		OPEN cursubaccount FOR
  		SELECT S_SUBACCOUNT_ID FROM T_SERVICE_GROUP_MAPPING WHERE S_GROUP_ID=t_groupid AND S_DIVERSE_STATE=1;
		EXCEPTION
		when OTHERS then
		begin
			returncode := 1;
			raise;
			return;
		end;
   	end GetSubAccount;

   procedure GetFlowMode
  (
       t_id in T_MERCHANT_DIVERSE_CONFIG.m_merchant_id%type,
       t_mode out T_MERCHANT_DIVERSE_CONFIG.m_diverse_type%type,
       returncode out integer
  )
  is
  	begin
  		returncode := 0;
   		SELECT M_DIVERSE_TYPE into t_mode FROM T_MERCHANT_DIVERSE_CONFIG WHERE M_MERCHANT_ID=t_id;
		EXCEPTION
		when NO_DATA_FOUND THEN
        begin
             returncode := 1403;
             return;
        end;
		when OTHERS then
		begin
			returncode := 1;
			raise;
			return;
		end;
  	end GetFlowMode;

   procedure SearchConnAccount
  (
       t_sid in IM_WEBLIST.FROMID%type,
       t_did in varchar2,
       cur_accounts out T_CURSOR,
       returncode out integer
  )
  is
  	strsql varchar2(4000);
  	begin
	   returncode := 0;
	   strsql := 'SELECT TOID,SENDTIME FROM IM_WEBLIST WHERE FROMID=('|| t_sid ||') AND TOID in ('
	   				|| t_did
	   				|| ')';
	   OPEN cur_accounts FOR
               strsql;
	   EXCEPTION
	   when OTHERS then
	   begin
		   returncode := 1;
		   raise;
		   return;
	   end;
  	end SearchConnAccount;

   procedure GetNoReplyNum
  (
  	   t_id in IM_WEBLIST.FROMID%type,
  	   t_time in integer,
       t_NoReplyNum out integer,
	   returncode out integer
  )
  is
  	begin
  		returncode := 0;
   		SELECT count(*) into t_NoReplyNum FROM IM_WEBLIST A,IM_WEBLIST B 
		WHERE A.FROMID=t_id AND B.TOID=t_id AND A.TOID=B.FROMID AND 
		(A.SENDTIME > (t_time - 600) or B.SENDTIME > (t_time - 600)) AND B.SENDTIME>A.SENDTIME;
		EXCEPTION
		when OTHERS then
		begin
			returncode := 1;
			raise;
			return;
		end;
  	end GetNoReplyNum;

  procedure GetJoinFlag
  (
      t_userid in IM_USER.userid%type,
      t_joinflag out IM_USERINFO.Joinauthflag%type,
      returncode out integer
  )
  is
     begin
          returncode := 0;
          SELECT JOINAUTHFLAG INTO t_joinflag
          FROM IM_USERINFO
          WHERE USERID = t_userid;
          EXCEPTION
          when NO_DATA_FOUND THEN
          begin
               returncode := 1403;
               return;
          end;
          when OTHERS then
          begin
               returncode := 1;
               raise;
               return;
          end;
     end GetJoinFlag;

  procedure AddFriend                                  --添加好友
  (
      t_userid in IM_FRIEND.Userid%type,
      t_bloggerid in IM_FRIEND.FriendId%type,
      t_groupinfo in IM_FRIEND.FriendGroupId%type,
      returncode out integer                       --11 好友数已满
  )
  is
     t_tmpcount integer;
     begin
          returncode := 0;
          SELECT COUNT(1) INTO t_tmpcount FROM IM_FRIEND WHERE USERID=t_userid;
          if t_tmpcount > 500 then     --最多加500个好友？？
            begin
             returncode := 11;
             return;
            end;
          end if;
          SELECT COUNT(1) INTO t_tmpcount FROM IM_FRIEND WHERE USERID=t_userid AND FriendId=t_bloggerid;
          if t_tmpcount > 0 then                  --修改分组
            begin
              UPDATE IM_FRIEND SET FriendGroupId = t_groupinfo
              WHERE USERID = t_userid AND FriendId = t_bloggerid;
              COMMIT;
              return;
            end;
          end if;
          INSERT INTO IM_FRIEND(ItemId,USERID,FriendId,FriendGroupId,
              USERNAME,NICKNAME,truename,gender,company,Telephone,Mobile,HOMEPAGE,Email)
            select 1,t_userid,t_bloggerid,t_groupinfo,
              t1.USERNAME,t2.NICKNAME,t1.truename,t1.gender,t1.company,t1.Telephone,t1.Mobile,t1.HOMEPAGE,Email
            from im_user t1,im_userinfo t2 where t1.USERID = t_bloggerid and t1.userid=t2.userid;

          COMMIT;
          EXCEPTION
          when OTHERS then
          begin
               returncode := 1;
               rollback;
               raise;
               return;
          end;

     end AddFriend;
  procedure DelFriend                                        --删除好友
  (
      t_userid in IM_FRIEND.Userid%type,
      t_bloggerid in IM_FRIEND.FriendId%type,
      returncode out integer
  )
  is
     tmp integer;
     begin
          returncode := 0;
          select count(1) into tmp from IM_FRIEND where USERID=t_userid AND FriendId=t_bloggerid;
          if  tmp = 0 then
              return;
          end if;
          DELETE FROM IM_FRIEND WHERE USERID=t_userid AND FriendId=t_bloggerid;

          COMMIT;

          EXCEPTION
          when OTHERS then
          begin
               returncode := 1;
               rollback;
               raise;
               return;
          end;
     end DelFriend;
  procedure SerarchUserID      --按ID查找用户
  (
      t_userid in IM_USER.Userid%type,
      t_username out IM_USER.Username%type,
      t_nickname out IM_USERINFO.Nickname%type,
      t_usersex out IM_USER.Gender%type,
      t_joinauthflag out IM_USERINFO.Joinauthflag%type,
      returncode out integer
   )
   is
      begin
           returncode := 0;
           SELECT nvl(t2.NICKNAME,t1.company),t1.USERNAME,t1.Gender,t2.JOINAUTHFLAG
             INTO t_nickname,t_username,t_usersex,t_joinauthflag
             FROM im_user t1,im_userinfo t2
             WHERE t1.userid = t_userid and t1.userid=t2.userid;


           EXCEPTION
           when NO_DATA_FOUND THEN
           begin
               returncode := 1403;--用户不存在。
               return;
           end;
           when OTHERS then
           begin
               ReturnCode := 1;
               raise;
               return;
            end;

      end SerarchUserID;

   procedure SerarchUserName     --按用户名查找用户
  (
      t_username in IM_USER.Username%type,
      t_userid out IM_USER.Userid%type,
      t_nickname out IM_USERINFO.Nickname%type,
      t_usersex out IM_USER.Gender%type,
      t_joinauthflag out IM_USERINFO.Joinauthflag%type,
      returncode out integer
   )
   is
      begin
           returncode := 0;
           SELECT * 
           INTO t_userid,t_nickname,t_usersex,t_joinauthflag 
           FROM (
             SELECT t1.USERID, nvl(t2.NICKNAME,t1.company),t1.Gender,t2.JOINAUTHFLAG
             FROM im_user t1,im_userinfo t2
             WHERE t1.username=t_username and t1.userid=t2.userid)
           WHERE ROWNUM <= 1;
           EXCEPTION
           when NO_DATA_FOUND THEN
           begin
               returncode := 1403;--用户不存在。
               return;
           end;
           when OTHERS then
           begin
               ReturnCode := 1;
               raise;
               return;
            end;
       end  SerarchUserName;

   procedure SerarchNickName     --按昵称查找用户
  (
      t_nickname in IM_USERINFO.Nickname%type,
      t_startpos in integer,
      t_reqcount in integer,
      curFindNick out T_CURSOR,
      returncode out integer
   )
   is
      t_endpos integer;
     begin
          returncode := 0;
          t_endpos := t_startpos + t_reqcount;
          OPEN curFindNick FOR
          SELECT USERID,USERNAME,NICKNAME,Gender,JOINAUTHFLAG
           FROM
             ( SELECT rownum as RdNo, t1.USERID,t1.USERNAME,t2.NICKNAME,t1.Gender,t2.JOINAUTHFLAG
               FROM IM_USER t1,im_userinfo t2
               WHERE t2.NICKNAME =t_nickname and t1.userid=t2.userid and rownum <= t_endpos
              )
            WHERE RdNo > t_startpos and RdNo <= t_endpos;
          EXCEPTION
          when OTHERS then
          begin
              returncode := 1;
              raise;
              return;
          end;
       end  SerarchNickName;

  procedure SerarchEmail     --按Email查找用户
  (
      t_email in IM_USER.Email%type,
      t_startpos in integer,
      t_reqcount in integer,
      curFindEmail out T_CURSOR,
      returncode out integer
   )
   is
     t_endpos integer;
     begin
          returncode := 0;
          t_endpos := t_startpos + t_reqcount;
          OPEN curFindEmail FOR
          SELECT USERID,USERNAME,NICKNAME,Gender,JOINAUTHFLAG
           FROM
             ( SELECT rownum as RdNo, t1.USERID,t1.USERNAME,t2.NICKNAME,t1.Gender,t2.JOINAUTHFLAG
               FROM IM_USER t1,im_userinfo t2
               WHERE t1.EMAIL =t_email and t1.userid=t2.userid and rownum <= t_endpos
              )
            WHERE RdNo > t_startpos and RdNo <= t_endpos;

          EXCEPTION
          when OTHERS then
          begin
              returncode := 1;
              raise;
              return;
          end;
     end  SerarchEmail;
  procedure ChangePwd       --修改骗子账号的密码
  (
      t_userid in IM_USER.Userid%type,
      returncode out integer
  )
  is
    t_userpwd IM_USER.password%type;
    begin
         returncode := 0;
         t_userpwd := 'baa4be07728065c0cbf4527effb6c87d';
         UPDATE IM_USER SET  PASSWORD=t_userpwd   WHERE USERID=t_userid;
         commit;
         EXCEPTION
         when OTHERS then
         begin
              returncode := 1;
              rollback;
              raise;
              return;
          end;
    end ChangePwd;

  procedure ModifyUserInfo  --修改用户信息
  (
      t_userid in IM_USER.Userid%type,
      t_joinauthflag in IM_USERINFO.Joinauthflag%type,
      t_nickname in IM_USERINFO.Nickname%type,
      t_testnick in IM_USERINFO.Nickname%type,
      t_passwd in IM_USER.password%type,
      t_desc in IM_USERINFO.Userdesc%type,
      t_emal in IM_USER.Email%type,
      t_mphone in IM_USER.Mobile%type,
      t_join2family in IM_USERINFO.Join2family%type,
      t_birthday in IM_USERINFO.Birthday%type,
      t_nation in IM_USERINFO.Nation%type,
      t_province in IM_USERINFO.Province%type,
      t_city in IM_USERINFO.City%type,
      t_sex in IM_USER.Gender%type,
      t_address in IM_USERINFO.usraddress%type,
      t_linkpop in IM_USERINFO.linkpop%type,
      t_career in IM_USERINFO.career%type,
      t_url in IM_USERINFO.userpicuri%type,
      returncode out integer     --14 昵称已经存在 15  昵称包含非法字符
  )
  is
    begin
          returncode := 0;

          UPDATE IM_USER SET
              Gender=nvl(t_sex,Gender),
              EMAIL=nvl(t_emal,EMAIL),
              Password=nvl(t_passwd,Password),
              Mobile=t_mphone
          WHERE USERID=t_userid;

          UPDATE IM_USERINFO SET
              CITY=t_city,
              NATION=t_nation,
              PROVINCE=t_province,
              Nickname=nvl(t_nickname,Nickname),
              USERDESC=t_desc,
              JOINAUTHFLAG=t_joinauthflag,
              JOIN2FAMILY=t_join2family,
              BIRTHDAY=t_birthday,
              USRADDRESS=t_address,
              LINKPOP=t_linkpop,
              CAREER=nvl(t_career,CAREER),
              USERPICURI=t_url
          WHERE USERID=t_userid;

          COMMIT;
          EXCEPTION
          when OTHERS then
          begin
              returncode := 1;
              rollback;
              raise;
              return;
          end;
    end Modifyuserinfo;

  
  procedure WebIMMsgSave_20150708
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
	t_logintime in IM_WEBDATA.LOGINTIME%type,
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

  	end WebIMMsgSave_20150708;	
  	
 procedure WebIMMsgSave_20151222
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
	t_logintime in IM_WEBDATA.LOGINTIME%type,
	t_fontname in IM_WEBDATA.FONTNAME%type,
	t_nickname in IM_WEBDATA.FROMNICKNAME%type,
	t_msg in IM_WEBDATA.MSG%type,
  t_uuid in IM_WEBDATA.UUID%type,
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
	  	(FROMID,TOID,DATATYPE,VERSION,SENDTIME,RECVFLAG,FONTSIZE,FONTCOLOR,FONTSTYLE,LOGINTIME,FONTNAME,FROMNICKNAME,MSG,MSGID,UUID)
	  VALUES
	  	(t_sid,t_did,t_type,t_ver,t_time,t_flag,t_fontsize,t_fontcolor,t_fontstyle,t_logintime,t_fontname,t_nickname,t_msg,t_msgid,t_uuid);
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

  	end WebIMMsgSave_20151222;   
  		
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
	  	(FROMID,TOID,DATATYPE,VERSION,SENDTIME,RECVFLAG,FONTSIZE,FONTCOLOR,FONTSTYLE,FONTNAME,FROMNICKNAME,MSG,MSGID) 
	  VALUES
	  	(t_sid,t_did,t_type,t_ver,t_time,t_flag,t_fontsize,t_fontcolor,t_fontstyle,t_fontname,t_nickname,t_msg,t_msgid); 
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
		
	procedure WebRecommSave
	(
		t_fid in IM_WEBRECOMMENT.FROMID%type,
		t_did in IM_WEBRECOMMENT.TOID%type,
		t_recomm in IM_WEBRECOMMENT.RECOMMENT%type,
		t_time in IM_WEBRECOMMENT.SENDTIME%type,
		t_logintime in IM_WEBRECOMMENT.I_SESSION_ID%type,
	 	t_msg	in IM_WEBRECOMMENT.V_RECOMMENT_TEXT%type,
		returncode out integer
	)
	is 
	  time integer;
	  num integer;
	  t_id integer;
	  begin
	  	returncode := 0;
		SELECT COUNT(SENDTIME),MAX(SENDTIME) INTO num,time 
		FROM IM_WEBRECOMMENT WHERE FROMID=t_fid AND TOID=t_did AND RECOMMENT=0 AND I_SESSION_ID=0;
		IF num > 0 THEN	
		BEGIN
			UPDATE IM_WEBRECOMMENT SET RECOMMENT=t_recomm,SENDTIME=t_time,V_RECOMMENT_TEXT=t_msg,I_SESSION_ID=t_logintime
			WHERE FROMID=t_fid AND TOID=t_did AND RECOMMENT=0 AND SENDTIME=time;
			COMMIT;			
			EXCEPTION
			when OTHERS then
			begin
				returncode := 1;
				rollback;
				return;
			end;
		END;
		ELSIF num=0 THEN
		BEGIN
		SELECT IM_WEBRECOMMENT_SEQ_MSGID.NEXTVAL into t_id FROM DUAL;
			INSERT INTO IM_WEBRECOMMENT(SEQID,FROMID,TOID,RECOMMENT,SENDTIME,I_SESSION_ID,V_RECOMMENT_TEXT) VALUES(t_id,t_fid,t_did,t_recomm,t_time,t_logintime,t_msg);
			COMMIT;
			EXCEPTION
			when OTHERS then
			begin
				returncode := 1;
				rollback;
				return;
			end;
		END;
		ELSE
		BEGIN
			returncode := 1;
			return;
		END;
		END IF;
	  end WebRecommSave;

	procedure WebServiceSave
	(
		t_fid in IM_WEBRECOMMENT.FROMID%type,
		t_did in IM_WEBRECOMMENT.TOID%type,
		t_recomm in IM_WEBRECOMMENT.RECOMMENT%type,
		t_time in IM_WEBRECOMMENT.SENDTIME%type,
		returncode out integer
	)
	is 
	  t_id integer;
	  begin
	  	returncode := 0;
		SELECT IM_WEBRECOMMENT_SEQ_MSGID.NEXTVAL into t_id FROM DUAL;
		INSERT INTO IM_WEBRECOMMENT(SEQID,FROMID,TOID,RECOMMENT,SENDTIME,I_SESSION_ID) VALUES(t_id,t_fid,t_did,t_recomm,t_time,0);
		COMMIT;
		EXCEPTION
		when OTHERS then
		begin
			returncode := 1;
			rollback;
			return;
		end;
	  end WebServiceSave;

	procedure WebMsgReq
	(
		t_sid in IM_WEBDATA.FROMID%type,
		t_did in IM_WEBDATA.TOID%type,
		t_time in IM_WEBDATA.SENDTIME%type,
		t_mid in IM_WEBDATA.MSGID%type,
		t_num in integer,
		curmsgInfo out T_CURSOR,
		returncode out integer
	)
	is
	  begin
	  	returncode := 0;
		IF t_mid = 0 THEN
		BEGIN
			OPEN curmsgInfo FOR
			SELECT FROMID,SENDTIME,FONTSIZE,FONTCOLOR,FONTSTYLE,FONTNAME,MSG,MSGID FROM 
			(SELECT * FROM IM_WEBDATA WHERE SENDTIME<t_time AND ((FROMID=t_sid AND TOID=t_did) OR (FROMID=t_did AND TOID=t_sid)) ORDER BY MSGID DESC)
			WHERE ROWNUM>0 AND ROWNUM<=t_num;
		END;
		ELSE 
		BEGIN			
			OPEN curmsgInfo FOR
			SELECT FROMID,SENDTIME,FONTSIZE,FONTCOLOR,FONTSTYLE,FONTNAME,MSG,MSGID FROM 
			(SELECT * FROM IM_WEBDATA WHERE MSGID<t_mid AND ((FROMID=t_sid AND TOID=t_did) OR (FROMID=t_did AND TOID=t_sid)) ORDER BY MSGID DESC)
			WHERE ROWNUM>0 AND ROWNUM<=t_num;
		END;
		END IF;
		EXCEPTION
		when OTHERS then
		begin
			returncode := 1;
			raise;
			return;
		end;
	  end WebMsgReq;
	  
procedure WebLastMsgReq
	(
		t_sid in IM_WEBDATA.FROMID%type,
		t_num in integer,
		curmsgInfo out T_CURSOR,
		returncode out integer
	)
	is

	  begin
	  	returncode := 0;
			OPEN curmsgInfo FOR
 ---    SELECT  E.TOID,F.SENDTIME,E.NICKNAME,F.MSG from
 ---   (SELECT C.FROMID,C.TOID,C.NICKNAME,MAX(D.MSGID) MAXID FROM
 ---    (SELECT A.FROMID,A.TOID,B.NICKNAME FROM IM_WEBLIST A,IM_USERINFO B WHERE A.FROMID=t_sid AND B.USERID=A.TOID group by (A.FROMID,A.TOID,B.NICKNAME) ) C, IM_WEBDATA D
 ---   WHERE (D.FROMID=C.FROMID AND D.TOID=C.TOID) OR (D.FROMID=C.TOID AND D.TOID=C.FROMID) GROUP BY (C.FROMID,C.TOID,C.NICKNAME)) E,IM_WEBDATA F
 ---   WHERE E.MAXID=F.MSGID and ((E.FROMID=F.FROMID AND E.TOID=F.TOID) OR (E.FROMID=F.TOID AND E.TOID=F.FROMID))
 ---    AND ROWNUM > 0 AND ROWNUM <=t_num;
 ---       select   a.fromid ,a.sndtime,b.nickname,a.msgs from
 ---        (select fromid,toid,max(sendtime) as sndtime,max(msg) as msgs,max(msgid) from im_webdata where toid=t_sid  group by (fromid,toid ))A,IM_USERINFO b
 ---        WHERE B.USERID = A.FROMID and rownum > 0 and rownum <=t_num ORDER BY SNDTIME DESC ;


		
  ---      with to_id_list as (select DISTINCT(toid) as to_id from im.IM_WEBDATA WHERE FROMID = t_sid),
---  	recent_id_list as (select to_id from to_id_list l where EXISTS (select 1 from im.IM_WEBDATA d where( fromid = l.to_id and d.TOID = t_sid) or (toid = l.to_id and d.fromid = t_sid))),
---        max_msg_id as (select * from (
---        select rank() over(partition by FROMID order by MSGID DESC) my_rank, MSGID, FROMID, TOID,MSG,FROMNICKNAME,SENDTIME from IM.IM_WEBDATA
---        WHERE (TOID = t_sid and FROMID in (select to_id from recent_id_list)) or (fromid= t_sid and toid in (select to_id from recent_id_list))) where my_rank=1)

---       select (case when r.fromid > r.toid then r.toid else r.fromid END)  as user_id, r.sendtime, (CASE WHEN r.fromid < 5000000 then r.fromnickname else ui.nickname end) as nick_name,r.msg
---       from
---      (select * from (select rank() over(partition by dummyid order by msgid DESC) my_rank, fromid, toid, msgid,msg,fromnickname,sendtime FROM
---      ((select  a.*, a.toid as dummyid from (select * from max_msg_id where fromid > 5000000) a) UNION (select a.*, a.FROMid as dummyid from (select * from max_msg_id where toid> 5000000) a))
---       u) where my_rank =1) r , IM_USERINFO ui WHERE UI.USERID = r.toid  and rownum > 0 and rownum <=t_num ORDER BY SENDTIME DESC ;

   WITH F as (select a.fromid,A.toid,B.sendtime,A.NICKNAME, B.MSG  from 
          ( select D.fromid,D.toid,C.NICKNAME ,max(D.msgid) as msgid from 
                   ( SELECT TOID,FROMID,SENDTIME,NICKNAME,USERPICURI,USERSIGN FROM
                            (SELECT A.TOID,A.FROMID,A.SENDTIME,B.NICKNAME,B.USERPICURI,B.USERSIGN FROM IM_WEBLIST A,IM_USERINFO B WHERE A.FROMID=t_sid AND B.USERID=A.TOID ORDER BY A.SENDTIME DESC)
    ) C, im_webdata D where (D.fromid=t_sid and D.toid =C.toid) OR (d.toid=t_sid and D.fromid =C.toid )  group by(c.nickname,D.fromid,D.toid)) A,im_webdata B where b.msgid=a.msgid order by sendtime desc)
    select ( case when fromid < toid then fromid else toid end ) as fromid,sendtime,nickname,msg from (select * from (select  rank()  OVER (partition by nickname order by sendtime desc ) my_rank ,fromid,toid,sendtime,nickname,msg FROM f ) where my_rank=1)
    where rownum > 0 and rownum <=t_num;

		EXCEPTION
		when OTHERS then
		begin
			returncode := 1;
			raise;
			return;
		end;
	  end WebLastMsgReq;


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

  procedure WebListReq_20150907
  (
	  t_sid in IM_WEBDATA.FROMID%type,
	  t_num in integer,
	  curlistInfo out T_CURSOR,
	  returncode out integer
  )
  is
  	t_count integer;
  	begin	
	  returncode := 0;
	  OPEN curlistInfo FOR
	  SELECT TOID,SENDTIME,NICKNAME,USERPICURI FROM 
	  (SELECT A.TOID,A.SENDTIME,B.NICKNAME,B.USERPICURI FROM IM_WEBLIST A,IM_USERINFO B WHERE A.TOID < 200000000 AND A.FROMID=t_sid AND B.USERID=A.TOID ORDER BY A.SENDTIME DESC)
	  WHERE ROWNUM>0 AND ROWNUM<=t_num;
	  EXCEPTION
	  when OTHERS then
	  begin
		  returncode := 1;
		  raise;
		  return;
	  end;
  	end WebListReq_20150907;
  
  
   procedure WebListReq_20151020
  (
	  t_sid in IM_WEBDATA.FROMID%type,
	  t_num in integer,
	  curlistInfo out T_CURSOR,
	  returncode out integer
  )
  is
  	t_count integer;
  	begin
	  returncode := 0;
	  OPEN curlistInfo FOR
	  SELECT TOID,SENDTIME,NICKNAME,USERPICURI,USERSIGN FROM
	  (SELECT A.TOID,A.SENDTIME,B.NICKNAME,B.USERPICURI,B.USERSIGN FROM IM_WEBLIST A,IM_USERINFO B WHERE A.TOID < 200000000 AND A.FROMID=t_sid AND B.USERID=A.TOID ORDER BY A.SENDTIME DESC)
	  WHERE ROWNUM>0 AND ROWNUM<=t_num;
	  EXCEPTION
	  when OTHERS then
	  begin
		  returncode := 1;
		  raise;
		  return;
	  end;
  	end WebListReq_20151020;
  	
  procedure WebListReq
  (
	  t_sid in IM_WEBDATA.FROMID%type,
	  t_num in integer,
	  curlistInfo out T_CURSOR,
	  returncode out integer
  )
  is
  	t_count integer;
  	begin	
	  returncode := 0;
	  OPEN curlistInfo FOR
	  SELECT TOID,SENDTIME,NICKNAME FROM 
	  (SELECT A.TOID,A.SENDTIME,B.NICKNAME FROM IM_WEBLIST A,IM_USERINFO B WHERE A.FROMID=t_sid AND B.USERID=A.TOID ORDER BY A.SENDTIME DESC)
	  WHERE ROWNUM>0 AND ROWNUM<=t_num;
	  EXCEPTION
	  when OTHERS then
	  begin
		  returncode := 1;
		  raise;
		  return;
	  end;
  	end WebListReq;

 procedure  ModifyFriendAliasname20150824                   --修改用户别名
  (
       t_userid in IM_FRIEND.Userid%type,
       t_bloggerid in IM_FRIEND.FriendId%type,
       t_aliasname in IM_FRIEND.aliasname%type,
       returncode out integer
  )
  is
    begin
          returncode := 0;

		  MERGE INTO IM_FRIEND t1 USING
		  ( SELECT t_userid AS Userid ,t_bloggerid AS FriendId from dual ) t2
		  on(t1.userid = t2.Userid and t1.friendid = t2.FriendId)
		  WHEN MATCHED THEN
		  UPDATE  SET T1.ALIASNAME = t_aliasname
		  WHEN NOT MATCHED THEN
		  INSERT  (userid,friendid,aliasname) values (t_userid,t_bloggerid,t_aliasname);		  
          commit;
          EXCEPTION
          when OTHERS then
          begin
              returncode := 1;
              rollback;
              return;
          end;
    end ModifyFriendAliasname20150824;

  procedure  ModifyFriendAliasname                   --修改用户别名
  (
       t_userid in IM_FRIEND.Userid%type,
       t_bloggerid in IM_FRIEND.FriendId%type,
       t_aliasname in IM_FRIEND.aliasname%type,
       returncode out integer
  )
  is
    begin
          returncode := 0;
          UPDATE IM_FRIEND SET ALIASNAME = t_aliasname
          WHERE USERID = t_userid AND FriendId = t_bloggerid;
          commit;
          EXCEPTION
          when OTHERS then
          begin
              returncode := 1;
              rollback;
              return;
          end;
    end Modifyfriendaliasname;

  procedure FillUserMoney
  (
      t_userid in number,
      t_fillmoney in number,
      returncode out integer
  )
  is
    --score NUMBER(10);
    --vErr varchar2(300);
    --vSql varchar2(300);
    BEGIN
         --score := t_fillmoney*5;
         returncode := 0;
         --pkg_wsv10_money.fillMoney(t_userid,3001,t_fillmoney,ret);   --赠送金币
         --execute immediate 'begin Pkg_WSV10_Integral.AddIntegral(t_userid,1,10,t_userid,score,ret); end;';

         --Pkg_WSV10_Integral.AddIntegral(t_userid,1,10,t_userid,score,returncode);  --赠送积分
         if returncode <> 0  then  --重试
            returncode := 2;
         end if;
/*         vSql := 'declare ret   number;  begin Pkg_WSV10_Integral.AddIntegral(' ||
         substr(t_userid,1,10) ||
         ',1,10,' ||
         substr(t_userid,1,10) ||
         ',' ||
         substr(score,1,10) ||
         ',ret); end;';
         execute immediate vSql;*/
         EXCEPTION
         when OTHERS then
         begin
              --vErr := SQLCODE || substr(SQLERRM,1,200);
              --insert into mini_errorlog(procid,procname,errdesc) values(1,'fillusermoney',vErr);
              --commit;
             returncode := 1;
             return;
         end;
    end Fillusermoney;


  procedure ModifyUserSign
  (
       t_userid in IM_USERINFO.Userid%type,
       t_usersign in IM_USERINFO.Usersign%type,
       returncode out integer
  )
  is
    begin
         returncode := 0;
         UPDATE IM_USERINFO SET USERSIGN = t_usersign  WHERE USERID = t_userid;
         commit;
         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             rollback;
             return;
         end;
    end ModifyUserSign;

  procedure ModifyFriendLoveFlag
  (
      t_userid in IM_FRIEND.Userid%type,
      t_bloggerid in IM_FRIEND.FriendId%type,
      t_loveflag in number,
      returncode out integer
  )
  is
    begin
         returncode := 0;
         /*
         UPDATE BLOG_USERFRIENDS SET LOVEFLAG = t_loveflag
                WHERE USERID = t_userid AND BLOGGERID = t_bloggerid;
         commit;
         */
         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             rollback;
             return;
         end;
    end ModifyFriendLoveFlag;

  procedure GetClientMsg
  (
       t_recid in out IM_CLIENTMSGINFO.Recid%type,
       t_count in out integer,
       cur_clientmsg out T_CURSOR,
       returncode out integer
  )
  is

    begin
         returncode := 0;
         if t_recid = 0 then
            SELECT MIN(RECID) INTO t_recid
            FROM  IM_CLIENTMSGINFO
            WHERE CREATEDATE > trunc(sysdate);--TO_DATE(TO_CHAR(sysdate,'YYYY-MM-DD'),'YYYY-MM-DD');
         end if;
         OPEN cur_clientmsg FOR
         SELECT recId,userId, SenderId,msgType,content,TO_CHAR(createDate,'YYYY/MM/DD HH24:MI:SS')
         FROM im_clientMsgInfo
         WHERE RECID > t_recid AND status=1 AND rownum <= t_count;
         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             raise;
             return;
         end;
    end GetClientMsg;

   procedure GetOfflineClientMsg
  (
       t_userid in IM_CLIENTMSGINFO.Userid%type,
       t_count in out integer,
       cur_offclientmsg out T_CURSOR,
       returncode out integer
  )
  is
    begin
          returncode := 0;
          OPEN cur_offclientmsg FOR
          SELECT recId,senderId,msgType,content,TO_CHAR(createDate,'YYYY/MM/DD HH24:MI:SS')
          FROM
            ( SELECT * FROM IM_CLIENTMSGINFO WHERE userID = t_userid AND status=1 ORDER BY CREATEDATE DESC)
          WHERE rownum <= t_count;
         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             raise;
             return;
         end;
    end GetOfflineClientMsg;

   procedure SetClientMsgStatus
   (
       t_recid in Im_Clientmsginfo.Recid%type,
       returncode out integer
   )
   is
     begin
          returncode := 0;
          UPDATE Im_clientMsgInfo SET  status=0 WHERE recid = t_recid;
          commit;
          EXCEPTION
          when OTHERS then
          begin
              returncode := 1;
              raise;
              return;
          end;
      end SetClientMsgStatus;

  procedure GetFriendGroup
  (
       t_userid in IM_FRIEND.userid%type,
       t_bloggerid in IM_FRIEND.FriendId%type,
       t_groupid out IM_FRIEND.FriendGroupId%type,
       returncode out integer
  )
  is
    begin
         returncode := 0;
         SELECT FriendGroupId INTO t_groupid FROM IM_FRIEND
         WHERE USERID = t_userid AND FriendId = t_bloggerid;
         EXCEPTION
         when NO_DATA_FOUND then
         begin
              returncode := 1403;
              t_groupid := 2; --黑名单
         end;
         when OTHERS then
         begin
              returncode := 1;
              t_groupid := 2;
              raise;
              return;
         end;
    end GetFriendGroup;

  procedure ModifyFriendGroup
  (
       t_userid in IM_FRIEND.Userid%type,
       t_bloggerid in IM_FRIEND.FriendId%type,
       t_groupid in IM_FRIEND.FriendGroupId%type,
       returncode out integer
  )
  is
    itmp integer;
    begin
         returncode := 0;
         itmp := 0;
         select count(1) into itmp from IM_FRIEND  WHERE USERID = t_userid AND FriendId = t_bloggerid;
         if itmp = 0 then
         begin
              AddFriend(t_userid,t_bloggerid,t_groupid,returncode );
              return;
         end;
         end if;

         UPDATE IM_FRIEND SET FriendGroupId = t_groupid
         WHERE USERID = t_userid AND FriendId = t_bloggerid;
         Commit;
         EXCEPTION
         when OTHERS then
         begin
            returncode := 1;
            raise;
            return;
         end;
    end Modifyfriendgroup;

  procedure GetFriendAliasName
  (
       t_userid in IM_FRIEND.Userid%type,
       t_bloggerid in IM_FRIEND.FriendId%type,
       t_aliasname out IM_FRIEND.Aliasname%type,
       returncode out integer
  )
  is
    begin
          returncode := 0;
          SELECT ALIASNAME
           INTO t_aliasname
          FROM IM_FRIEND
          WHERE USERID = t_userid and FriendId = t_bloggerid;
          EXCEPTION
          when NO_DATA_FOUND THEN
          begin
               returncode := 1403;
               return;
          end;
          when OTHERS then
          begin
               returncode := 1;
               raise;
               return;
          end;
    end Getfriendaliasname;

  procedure GetUserBaseInfo
  (
      t_struid in varchar2,
      curuserbaseinfo out T_CURSOR,
      returncode out integer
  )
  is
     strsql varchar2(4000);
     begin
          returncode := 0;
          strsql := 'SELECT A.*,B.REALNAME FROM (SELECT t1.USERID,t1.USERNAME, t2.NICKNAME,t1.GENDER,t2.USERPICURI,0 as USERMOBILESTATUS,t1.SIZEDS FROM  IM_USER t1,IM_USERINFO t2 WHERE t1.USERID in ('
                    || t_struid
                    || ') and t1.userid=t2.userid) A LEFT JOIN IM_CSINFO B ON A.USERID=B.USERID' ;
          OPEN curuserbaseinfo FOR
               strsql;
          EXCEPTION
          when OTHERS then
          begin
              returncode := 1;
              raise;
              return;
          end;
     end GetUserBaseInfo;


  procedure GetFriendsBaseInfo
  (
      t_userid in IM_FRIEND.Userid%type,
      t_struid in varchar2,
      curfriendsbaseinfo out T_CURSOR,
      returncode out integer
  )
  is
     strsql varchar2(4000);
     begin
          returncode := 0;
          strsql := 'SELECT A.USERID,A.USERNAME, A.NICKNAME,A.GENDER,A.USERPICURI,A.USERSIGN, B.ALIASNAME, 0 as LOVEFLAG,0 as USERMOBILESTATUS,A.SIZEDS '||
                    'FROM  (SELECT t1.USERID,t1.USERNAME,t2.NICKNAME,t1.GENDER,t2.USERPICURI,t2.USERSIGN,t1.SIZEDS FROM IM_USER t1, IM_USERINFO t2 WHERE t1.USERID in ('
                    || t_struid
                    || ') and t1.userid=t2.userid) A, IM_FRIEND B WHERE B.USERID=' || t_userid || ' AND B.FriendId = A.USERID';
          OPEN curfriendsbaseinfo FOR
               strsql;
          EXCEPTION
          when OTHERS then
          begin
              returncode := 1;
              raise;
              return;
          end;
     end GetFriendsBaseInfo;

  procedure GetFriendsBaseInfo_20150729
  (
      t_userid in IM_FRIEND.Userid%type,
      t_struid in varchar2,
      curfriendsbaseinfo out T_CURSOR,
      returncode out integer
  )
  is
     strsql varchar2(4000);
     begin
          returncode := 0;
          strsql := 'SELECT TT1.*,TT2.REALNAME FROM (SELECT A.USERID,A.USERNAME, A.NICKNAME,A.GENDER,A.USERPICURI,A.USERSIGN, B.ALIASNAME, 0 as LOVEFLAG,0 as USERMOBILESTATUS,A.SIZEDS '
					|| ' FROM  (SELECT t1.USERID,t1.USERNAME,t2.NICKNAME,t1.GENDER,t2.USERPICURI,t2.USERSIGN,t1.SIZEDS '
					|| ' FROM IM_USER t1 '
					|| ' LEFT JOIN IM_USERINFO t2 '
					|| ' ON t1.USERID=t2.USERID '
					|| 'WHERE t1.USERID in ('|| t_struid || ') ) A '
					|| 'LEFT JOIN  (SELECT * FROM im_friend WHERE userid=' || t_userid || ' ) B '
					|| 'ON B.FriendId = A.USERID) TT1 LEFT JOIN IM_CSINFO TT2 ON TT1.USERID=TT2.USERID';
          OPEN curfriendsbaseinfo FOR
               strsql;
          EXCEPTION
          when OTHERS then
          begin
              returncode := 1;
              raise;
              return;
          end;
     end GetFriendsBaseInfo_20150729;

  procedure GetUserJoin2Family
  (
       t_struid in varchar2,
       curjoin2family out T_CURSOR,
       returncode out integer
  )
  is
    strsql varchar2(4000);
    begin
         returncode := 0;
         strsql := 'SELECT USERID,JOIN2FAMILY FROM IM_USERINFO WHERE USERID in ('
                   || t_struid
                   || ')';
         OPEN curjoin2family FOR
              strsql;
          EXCEPTION
          when OTHERS then
          begin
              returncode := 1;
              raise;
              return;
          end;
    end GetUserJoin2Family;
/*
  procedure GetUserMsg   --收件箱消息
  (
       t_recid in out IM_MESSAGE_ALERT.MsgId%type,
       t_count in integer,
       cur_usermsg out T_CURSOR,
       returncode out integer        
  )
  is 
    begin
         returncode := 0;
         if t_recid = 0 then
            SELECT MIN(MSGID)-1 INTO t_recid
            FROM  IM_MESSAGE_ALERT 
            WHERE CREATETIME > trunc(sysdate) and IsReaded<>1;--TO_DATE(TO_CHAR(sysdate,'YYYY-MM-DD'),'YYYY-MM-DD');
         end if;
         if t_recid is NULL then
            SELECT MAX(MSGID) INTO t_recid
            FROM IM_MESSAGE_ALERT;
         end if;
         if t_recid is NULL then
            t_recid := 0;
         end if;
         
         OPEN cur_usermsg FOR
         select MsgId, ReceiverId,0,0,title,content,0,TO_CHAR(CreateTime,'YYYY/MM/DD HH24:MI:SS'),
                'Web','Web'
         from IM_MESSAGE_ALERT where MSGID > t_recid and IsReaded<>1 and  rownum < t_count; 

         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             raise;
             return; 
         end;
    end GetUserMsg;    
amin */
/*
  procedure GetOperationMsg  --用户更新消息
  (
       t_recid in out number,
       t_count in integer,
       cur_operationmsg out T_CURSOR,
       returncode out integer
  )
  is
    begin
         returncode := 0;

         if t_recid = 0 then
            SELECT MIN(RECID) INTO t_recid
            FROM  blog_userOperation
            WHERE CREATEDATE > trunc(sysdate);--TO_DATE(TO_CHAR(sysdate,'YYYY-MM-DD'),'YYYY-MM-DD');
         end if;

         if t_recid is NULL then
            SELECT MAX(RECID) INTO t_recid
            FROM blog_userOperation;
         end if;
         if t_recid is NULL then
            t_recid := 0;
         end if;
         OPEN cur_operationmsg FOR

         SELECT a.recId,a.userId,b.nickname,b.username,a.classid,a.logid,a.srcuri,a.title,a.outlines,TO_CHAR(a.createDate,'YYYY/MM/DD HH24:MI:SS')
         FROM
             (SELECT recId,userId,classid,logid,srcuri,title,outlines,createDate
             FROM blog_userOperation
             WHERE RECID > t_recid AND rownum <= t_count ) a,
             blog_userbaseinfo b
         where b.userId=a.userId;

         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             raise;
             return;
         end;

    end GetOperationMsg;
*/
 /*
 procedure GetUserMsg   --收件箱消息
  (
       t_recid in out BLOG_USERINBOX.RECID%type,
       t_count in integer,
       cur_usermsg out T_CURSOR,
       returncode out integer
  )
  is
    t_msgid integer;
    begin
         returncode := 0;
         if t_recid = 0 then
            SELECT MIN(MSGID) INTO t_msgid
            FROM  BLOG_USERMESSAGECONTENT
            WHERE CREATEDATE > trunc(sysdate);--TO_DATE(TO_CHAR(sysdate,'YYYY-MM-DD'),'YYYY-MM-DD');
            SELECT MIN(RECID) INTO t_recid
            FROM BLOG_USERINBOX
            WHERE MSGID = t_msgid and STATUS=0;
         end if;
         if t_recid is NULL then
            SELECT MAX(RECID) INTO t_recid
            FROM BLOG_USERINBOX;
         end if;
         if t_recid is NULL then
            t_recid := 0;
         end if;

         OPEN cur_usermsg FOR
         select a.RECID,a.USERID,a.MSGID,b.userid,b.title,b.content,b.msgtype,TO_CHAR(b.createDate,'YYYY/MM/DD HH24:MI:SS'),c.nickname,c.username
         from
              (
                 select RECID,USERID,MSGID FROM BLOG_USERINBOX where RECID > t_recid and status=0 and rownum < t_count
              ) a,
              BLOG_USERMESSAGECONTENT b,
              blog_userbaseinfo c
         where a.msgid = b.msgid  and c.userid=b.userid;
         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             raise;
             return;
         end;
    end GetUserMsg;
 */
  procedure GetSysMsg   --系统广播消息
  (
       t_msgid in out im_sysmessage.msgid%type,
       t_count in integer,
       cur_sysmsg out T_CURSOR,
       returncode out integer
  )
  is
    begin
         returncode := 0;
         OPEN cur_sysmsg FOR
              SELECT MSGID,TITLE,CONTENT,TO_CHAR(CREATEDATE,'YYYY/MM/DD HH24:MI:SS'),TOID FROM
              (SELECT * FROM im_sysmessage where MSGID > t_msgid ORDER BY MSGID)
			  WHERE rownum < t_count;
         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             raise;
             return;
         end;
    end GetSysMsg;

  procedure GetSysOffMsg	 --离线系统消息
  (
	 t_msgid in out im_sysmessage.msgid%type,
	 t_uid in im_sysmessage.toid%type,
	 t_count in integer,
	 cur_sysmsg out T_CURSOR,
	 returncode out integer
  )
  is
  	begin  		
	  returncode := 0;
	  OPEN cur_sysmsg FOR
		   SELECT MSGID,TITLE,CONTENT,TO_CHAR(CREATEDATE,'YYYY/MM/DD HH24:MI:SS') FROM
		   (SELECT * FROM im_sysmessage where TOID = t_uid AND MSGID > t_msgid ORDER BY MSGID DESC)
		   where rownum < t_count;
	  EXCEPTION
	  when OTHERS then
	  begin
		  returncode := 1;
		  raise;
		  return;
	  end;
  	end GetSysOffMsg;
/*
  procedure  Get_broadcastInfo   --用户广播消息
  (
       t_msgid in out  blog_broadcastInfo.MSGID%type,
       t_count in integer,
       cur_broadcast out T_CURSOR,
       returncode out integer
  )
  is
    begin
         returncode := 0;
         if t_msgid = 0 then
            SELECT MIN(MSGID) INTO t_msgid
            FROM   blog_broadcastInfo
            WHERE CREATEDATE > trunc(sysdate);--TO_DATE(TO_CHAR(sysdate,'YYYY-MM-DD'),'YYYY-MM-DD');
         end if;
         if t_msgid is NULL then
            SELECT MAX(MSGID) INTO t_msgid
            FROM   blog_broadcastInfo;
         end if;
         if t_msgid is NULL then
            t_msgid := 0;
         end if;
         OPEN cur_broadcast FOR
              select a.msgid, a.USERID,a.content,TO_CHAR(a.createDate,'YYYY/MM/DD HH24:MI:SS'),b.nickname,b.username
              from
              (
                     select MSGID,USERID,content,createDate FROM  blog_broadcastInfo where MSGID > t_msgid and rownum < t_count
              ) a,
              blog_userbaseinfo b
              where b.userid = a.userid;
         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             raise;
             return;
         end;
    end Get_broadcastInfo;

  procedure Get_UserComment  --个人主页评论
  (
        t_commentid in out blog_userComment.Commentid%type,
        t_count in out integer,
        cur_usercomment out T_CURSOR,
        returncode out integer
  )
  is
    begin

         returncode := 0;
         if t_commentid = 0 then
            SELECT MIN(COMMENTID) INTO t_commentid
            FROM   blog_userComment
            WHERE CREATEDATE > trunc(sysdate);
         end if;
         if t_commentid is NULL then
            SELECT MAX(COMMENTID) INTO t_commentid
            FROM   blog_userComment;
         end if;
         if t_commentid is NULL then
            t_commentid := 0;
         end if;

         OPEN cur_usercomment FOR
              select a.COMMENTID,a.USERID,a.title,a.content,a.authorid,b.nickname,b.username,TO_CHAR(a.createDate,'YYYY/MM/DD HH24:MI:SS')
              from
              (
                   select COMMENTID,USERID,title,content,authorid,createDate FROM  blog_userComment where COMMENTID > t_commentid and floornum = 0 and rownum < t_count
              ) a,
              blog_userbaseinfo b
              where b.userid(+) = a.authorid;
         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             raise;
             return;
         end;
    end Get_Usercomment;

  procedure Get_LogComment  --博客内容评论
  (
        t_commentid in out  blog_userLogComment.Commentid%type,
        t_count in out integer,
        cur_logcomment out T_CURSOR,
        returncode out integer
  )
  is
    begin

         returncode := 0;
         if t_commentid = 0 then
            SELECT MIN(COMMENTID) INTO t_commentid
            FROM   blog_userLogComment
            WHERE CREATEDATE > trunc(sysdate);
         end if;
         if t_commentid is NULL then
            SELECT MAX(COMMENTID) INTO t_commentid
            FROM   blog_userLogComment;
         end if;
         if t_commentid is NULL then
            t_commentid := 0;
         end if;
         OPEN cur_logcomment FOR
              select a.COMMENTID,a.USERID,a.logtype,a.logid,a.content,a.authorid,b.nickname,b.username,TO_CHAR(a.createDate,'YYYY/MM/DD HH24:MI:SS')
              from
              (
                   select COMMENTID,USERID,logtype,logid,content,authorid,createDate FROM   blog_userLogComment where COMMENTID > t_commentid and floornum = 0 and rownum < t_count
              ) a,
              blog_userbaseinfo b
              where b.userid(+) = a.authorid;
         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             raise;
             return;
         end;
    end Get_LogComment;
*/
/*
 procedure GetOneOperationMsg  --用户更新消息
  (
       t_recid in out BLOG_USEROPERATION.Recid%type,
       t_struid in varchar2,
       t_count in integer,
       cur_oneoperationmsg out T_CURSOR,
       returncode out integer
  )
  is
   strsql varchar2(4000);
     begin
          returncode := 0;

         if t_recid = 0 then
            SELECT MIN(RECID) INTO t_recid
            FROM  blog_userOperation
            WHERE CREATEDATE > trunc(sysdate);--TO_DATE(TO_CHAR(sysdate,'YYYY-MM-DD'),'YYYY-MM-DD');
         end if;

        if t_recid is NULL then
            SELECT MAX(RECID) INTO t_recid
            FROM blog_userOperation;
         end if;
         if t_recid is NULL then
            t_recid := 0;
         end if;
         strsql := 'SELECT a.recId,a.userId,b.nickname,b.username,a.classid,a.logid,a.srcuri,a.title,a.outlines,TO_CHAR(a.createDate,''YYYY/MM/DD HH24:MI:SS'')'
          || ' FROM'
          || ' (SELECT recId,userId,classid,logid,srcuri,title,outlines,createDate '
          || ' FROM blog_userOperation '
          || ' WHERE RECID > '
          || t_recid
          || ' AND USERID in ('
          || t_struid
          || ') AND rownum <= '
          || t_count
          || ' ) a,   blog_userbaseinfo b '
          || ' where b.userId=a.userId';

         OPEN cur_oneoperationmsg FOR
               strsql;

         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             raise;
             return;
         end;
    end GetOneOperationMsg;
*/
/*
  procedure GetOneUserMsg   --收件箱消息
  (
       t_recid in out BLOG_USERINBOX.RECID%type,
       t_userid in BLOG_USERINBOX.USERID%type,
       t_count in integer,
       cur_oneusermsg out T_CURSOR,
       returncode out integer
  )
  is
     t_msgid integer;
    begin
         returncode := 0;
         if t_recid = 0 then
            SELECT MIN(MSGID) INTO t_msgid
            FROM  BLOG_USERMESSAGECONTENT
            WHERE CREATEDATE > trunc(sysdate) - 3;
            SELECT MIN(RECID) INTO t_recid
            FROM BLOG_USERINBOX
            WHERE MSGID=t_msgid;
         end if;

        if t_recid is NULL then
            SELECT MAX(RECID) INTO t_recid
            FROM BLOG_USERINBOX;
         end if;
         if t_recid is NULL then
            t_recid := 0;
         end if;

         OPEN cur_oneusermsg FOR
         select a.RECID,a.USERID,a.MSGID,b.userid,b.title,b.content,b.msgtype,TO_CHAR(b.createDate,'YYYY/MM/DD HH24:MI:SS'),c.nickname,c.username
         from
              (
                  select RECID,USERID,MSGID
                  from
                  (
                       select RECID,USERID,MSGID FROM BLOG_USERINBOX where RECID > t_recid and status=0 and userid = t_userid
                  )
                  where rownum < t_count
                 --select RECID,USERID,MSGID FROM BLOG_USERINBOX where RECID > t_recid and status=0 and userid = t_userid and rownum < t_count
              ) a,
              BLOG_USERMESSAGECONTENT b,
              blog_userbaseinfo c
         where a.msgid = b.msgid  and c.userid=b.userid;
         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             raise;
             return;
         end;
    end GetOneUserMsg;
 */
 /*
  procedure Get_OneUserComment  --个人主页评论
  (
        t_commentid in out blog_userComment.Commentid%type,
        t_userid in blog_usercomment.userid%type,
        t_count in out integer,
        cur_oneusercomment out T_CURSOR,
        returncode out integer
  )
  is
    begin
         returncode := 0;
         if t_commentid = 0 then
            SELECT MIN(COMMENTID) INTO t_commentid
            FROM   blog_userComment
            WHERE CREATEDATE > trunc(sysdate);
         end if;
         if t_commentid IS NULL then
            SELECT MAX(COMMENTID) INTO t_commentid
            FROM   blog_userComment;
         end if;
         if t_commentid is NULL then
            t_commentid := 0;
          end if;
         OPEN cur_oneusercomment FOR
              select a.COMMENTID,a.USERID,a.title,a.content,a.authorid,b.nickname,b.username,TO_CHAR(a.createDate,'YYYY/MM/DD HH24:MI:SS')
              from
              (
                   select COMMENTID,USERID,title,content,authorid,createDate
                   from
                   (
                       select COMMENTID,USERID,title,content,authorid,createDate FROM  blog_userComment where COMMENTID > t_commentid and userid = t_userid and authorid != t_userid and floornum = 0
                   )
                   where rownum < t_count
              ) a,
              blog_userbaseinfo b
              where b.userid(+) = a.authorid;
         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             raise;
             return;
         end;
    end Get_OneUsercomment;

  procedure Get_OneLogComment  --博客内容评论
  (
        t_commentid in out  blog_userLogComment.Commentid%type,
        t_userid in blog_userlogcomment.userid%type,
        t_count in out integer,
        cur_onelogcomment out T_CURSOR,
        returncode out integer
  )
  is
    begin

         returncode := 0;
         if t_commentid = 0 then
            SELECT MIN(COMMENTID) INTO t_commentid
            FROM   blog_userComment
            WHERE CREATEDATE > trunc(sysdate);
         end if;

         if t_commentid is NULL then
            t_commentid := 0;
          end if;
         OPEN cur_onelogcomment FOR
              select a.COMMENTID,a.USERID,a.logtype,a.logid,a.content,a.authorid,b.nickname,b.username,TO_CHAR(a.createDate,'YYYY/MM/DD HH24:MI:SS')
              from
              (
                   select COMMENTID,USERID,logtype,logid,content,authorid,createDate
                   from
                   (
                         select COMMENTID,USERID,logtype,logid,content,authorid,createDate FROM   blog_userLogComment where COMMENTID > t_commentid and userid = t_userid and authorid != t_userid and floornum = 0
                   )
                   where rownum < t_count
              ) a,
              blog_userbaseinfo b
              where b.userid(+) = a.authorid;
         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             raise;
             return;
         end;
    end Get_OneLogComment;

  procedure SetUserMessageStatus
  (
       t_recid  in BLOG_USERINBOX.Recid%type,
       returncode out integer
  )
  is
    begin
         returncode := 0;

         UPDATE BLOG_USERINBOX set Status = 1 where RecID = t_recid;

         COMMIT;
         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             return;
         end;
    end SetUserMessageStatus;

 procedure GetSystemLog  --业务同步
  (
       t_recid in out Blog_Systemlog.Recid%type,
       t_count in integer,
       cur_systemlog out T_CURSOR,
       returncode out integer
  )
  is

    begin
         returncode := 0;
         if t_recid = 0 then
            SELECT MIN(RECID) INTO t_recid
            FROM  Blog_Systemlog
            WHERE CREATEDATE > trunc(sysdate);
         end if;

         if t_recid is NULL then
            SELECT MAX(RECID) INTO t_recid
            FROM Blog_Systemlog;
         end if;
         if t_recid is NULL then
            t_recid := 0;
         end if;
         OPEN cur_systemlog FOR
              SELECT RECID,PROJID,FLATID,OPERID,USERID,LOGID
              FROM Blog_Systemlog
              WHERE RECID > t_recid and PROJID in(110501,110502,120201,120202,120203,120204) and rownum <= t_count;
              --  110501---个人主页好友添加 110502---个人主页好友删除 120201---成员加入（无需验证）
              --    120202---成员加入（管理员验证通过）120203---成员退出（自行退出） 120204---成员退出（管理员fire）

         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             raise;
             return;
         end;
    end GetSystemLog;
 */
   procedure GetNickName
   (
         t_userid in IM_USERINFO.Userid%type,
         t_nickname out  IM_USERINFO.Nickname%type,
         returncode out integer
   )
   is
     begin
          SELECT NICKNAME INTO t_nickname
          FROM IM_USERINFO
          WHERE USERID = t_userid;
          EXCEPTION
          when NO_DATA_FOUND THEN
          begin
               returncode := 1403;
               return;
          end;
          when OTHERS then
          begin
               returncode := 1;
               raise;
               return;
          end;

     end GetNickName;
   procedure GetUserPicUri
  (
         t_userid in IM_USERINFO.Userid%type,
         t_userpicuri out  IM_USERINFO.Userpicuri%type,
         returncode out integer
  )
  is
    begin
          SELECT USERPICURI INTO t_userpicuri
          FROM IM_USERINFO
          WHERE USERID = t_userid;
          EXCEPTION
          when NO_DATA_FOUND THEN
          begin
               returncode := 1403;
               return;
          end;
          when OTHERS then
          begin
               returncode := 1;
               raise;
               return;
          end;
    end GetUserPicUri;
    
 PROCEDURE SplitStringSp(
			P_INSTR IN VARCHAR2,--传入字符串，逗号分隔，输入参数
			P_STRS OUT VARCHAR2,--截取第1个值之后剩下的字符串，输出参数
			P_CURR OUT VARCHAR2, --截取到的第1个值，输出参数
			P_CHAR IN VARCHAR2 --以什么字符为分隔字符
) 
IS
CURR_LOCA NUMBER;
BEGIN
    CURR_LOCA := INSTR(P_INSTR, P_CHAR, 1, 1);
    P_CURR  := SUBSTR(P_INSTR, 0, CURR_LOCA - 1);
    IF P_CURR IS NULL THEN
			P_CURR := P_INSTR;
			P_STRS := NULL;
    ELSE
			P_STRS := SUBSTR(P_INSTR, CURR_LOCA + 1);
    END IF;
END SplitStringSp;


PROCEDURE statisticsUpdateReplyDetail
 (startTime in NUMBER ,
endTime in NUMBER)AS
BEGIN
--回复情况分5种：
------客户先发消息：（1）客服回了   DUR>0
---------------------(2) 客服没回   dur=-2
------主动接待：
-------客服先发消息：(3) 客户在线 并回复了消息  dur=-1
-------客服先发消息：(4) 客户在线 但没有回复了消息  dur=0
-------客服先发消息：(5) 客户不在线   dur=-1
	
--合并表，而不是直接插入
MERGE INTO IM.T_SESSION_DETAIL p USING(
--将统计出来的基础数据转换为reply信息
		SELECT r.fromid,r.toid,r.logintime,r.askTime,r.replytime,
		(CASE WHEN r.asktime IS NULL THEN (CASE WHEN r.logintime = 0 THEN -3 ELSE 0 END) ELSE 
			(CASE WHEN r.replytime is NULL THEN -2 ELSE 
				(CASE WHEN r.replytime - r.asktime < 0 THEN -1 ELSE r.replytime - r.asktime END)
			END) 
		END) AS DUR ,
	
		r.start_time AS start_time,
		r.end_time AS end_time,
		(r.END_TIME - r.START_TIME) AS session_len,
		r.MESSAGE_COUNT as message_count,
		r.reply_start AS reply_start,
		r.reply_end AS reply_end,
		(r.reply_end - r.reply_start) AS reply_len
		FROM(
--根据LOGINTIME统计这段时间内所有会话的基础数据。
					SELECT 
					(CASE WHEN a.fromid is NULL THEN b.TOID ELSE a.FROMID END) AS toid,
					(CASE WHEN a.toid is NULL THEN b.fromid ELSE a.toid END) AS fromid,
					(CASE WHEN a.LOGINTIME is NULL THEN b.LOGINTIME ELSE a.LOGINTIME END) AS logintime,
					a.time AS askTime,
					b.time AS replytime,
					(CASE WHEN a.time IS NULL THEN b.time ELSE 
						(CASE WHEN b.TIME IS NULL THEN a.time ELSE 
						(CASE WHEN b.TIME < a.time THEN b.time ELSE a.time END ) END) END) AS  start_time,
	
					(CASE WHEN a.END_TIME IS NULL THEN b.END_TIME ELSE 
						(CASE WHEN b.END_TIME IS NULL THEN a.END_TIME ELSE 
						(CASE WHEN b.END_TIME > a.END_TIME THEN b.END_TIME ELSE a.END_TIME END ) END) END) AS  end_time,
	
					b.message_count as message_count,
	
					(CASE WHEN b.time IS NULL THEN 0 ELSE b.time  END) AS reply_start,
					(CASE WHEN b.END_TIME IS NULL THEN 0 ELSE b.END_TIME  END) AS reply_end
	
					 FROM(
--根据LOGINTIME统计这段时间内所有聊天记录拼接成会话。
									SELECT IM.IM_WEBDATA.FROMID,IM.IM_WEBDATA.TOID,IM.IM_WEBDATA.LOGINTIME AS LOGINTIME,MIN (IM.IM_WEBDATA.SENDTIME) AS TIME
									,MAX(IM.IM_WEBDATA.SENDTIME) AS END_TIME
											FROM IM.IM_WEBDATA 
											WHERE (IM.IM_WEBDATA.FROMID > 5000000 AND IM.IM_WEBDATA.FROMID < 2000000000)
											AND (IM.IM_WEBDATA.TOID < 5000000 OR IM.IM_WEBDATA.TOID > 2000000000 )
											AND IM.IM_WEBDATA.DATATYPE <> 21
											AND IM.IM_WEBDATA.SENDTIME > startTime
											AND IM.IM_WEBDATA.SENDTIME < endTime
											AND IM.IM_WEBDATA.LOGINTIME <> 1
											GROUP BY
												IM.IM_WEBDATA.FROMID,
												IM.IM_WEBDATA.TOID,
												IM.IM_WEBDATA.LOGINTIME
											ORDER BY
												IM.IM_WEBDATA.FROMID ASC,
												IM.IM_WEBDATA.TOID ASC
									) a FULL JOIN (
									SELECT IM.IM_WEBDATA.FROMID,IM.IM_WEBDATA.TOID,IM.IM_WEBDATA.LOGINTIME AS LOGINTIME,MIN (IM.IM_WEBDATA.SENDTIME) AS TIME
									,MAX(IM.IM_WEBDATA.SENDTIME) AS END_TIME,COUNT(*) AS MESSAGE_COUNT
											FROM IM.IM_WEBDATA 
											WHERE (IM.IM_WEBDATA.FROMID < 5000000 OR IM.IM_WEBDATA.FROMID > 2000000000)
											AND (IM.IM_WEBDATA.TOID > 5000000 AND IM.IM_WEBDATA.TOID < 2000000000 )
											AND IM.IM_WEBDATA.DATATYPE <> 21
											AND IM.IM_WEBDATA.SENDTIME > startTime
											AND IM.IM_WEBDATA.SENDTIME < endTime
											AND IM.IM_WEBDATA.LOGINTIME <> 1
											GROUP BY
												IM.IM_WEBDATA.FROMID,
												IM.IM_WEBDATA.TOID,
												IM.IM_WEBDATA.LOGINTIME
											ORDER BY
												IM.IM_WEBDATA.FROMID ASC,
												IM.IM_WEBDATA.TOID ASC
									)b
									ON(
										a.FROMID = b.TOID AND
										a.TOID = b.FROMID AND
										a.LOGINTIME = b.LOGINTIME
									)
						)r
	
)pn
ON(   
		p.I_FROMID   = pn.FROMID    AND p.I_TOID   = pn.TOID AND p.I_logintime = pn.logintime
 ) 
--已经有了该条统计信息，更新数据
WHEN MATCHED THEN  UPDATE  SET p.I_ASKTIME = pn.ASKTIME,p.I_DUR = pn.DUR ,p.I_REPLYTIME = pn.REPLYTIME,
p.I_start_Time = pn.start_time,p.I_end_time = pn.end_time,p.I_session_len=pn.session_len,p.I_message_count=pn.message_count,
p.I_reply_start = pn.reply_start,p.I_reply_end = pn.reply_end,p.I_reply_len = pn.reply_len
--没有，插入数据
WHEN NOT MATCHED THEN  INSERT VALUES 
	(    pn.FROMID ,    pn.TOID , pn.logintime,   pn.ASKTIME ,    pn.REPLYTIME,  pn.DUR,pn.message_count,pn.start_time,pn.end_time,pn.session_len,
	pn.reply_start,pn.reply_end,pn.reply_len);
	
END STATISTICSUPDATEREPLYDETAIL;
	
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
	--获取时间
		select to_char(CURRENT_TIMESTAMP,'YYYYMMDD HH24:MI:SS.FF9') INTO updateTime from dual;
		
		v_str_merchantid := merchantId;
		v_str_submerchantid := subMerchantId;
		
	loop
		EXIT WHEN v_str_submerchantid IS NULL;
		responseTime := 0;
		responseCount := 0;
		sessionTime :=0;
		sessionCount := 0;
		responseMessageCount:= 0;
		receptionTime := 0;
		responseIds :='';
		askingIds :='';
		responseIn90sCount :=0;
		
		SplitStringSp(v_str_submerchantid, v_res_str_submerchantid, v_current_submerchantid, ';');
		SplitStringSp(v_str_merchantid, v_res_str_merchantid, v_current_merchantid, ';');
		dbms_output.put_line('开始统计 v_current_submerchantid');
		dbms_output.put_line(v_current_submerchantid);
		
	--判断是否有统计数据
		SELECT (CASE WHEN COUNT(*) = NULL THEN 0 ELSE COUNT(*) END )INTO resultCount 	FROM T_SESSION_DETAIL p 
		WHERE p.I_start_Time < endTime AND p.I_start_Time > startTime AND p.I_FROMID = v_current_submerchantid;
		
		dbms_output.put_line('开始统计 count：');
		dbms_output.put_line(resultCount);
	IF ( resultCount <>0 ) THEN
	--统计responseTime,responseCount,sessionTime,sessionCount,responseMessageCount,receptionTime
		SELECT 
			SUM(CASE WHEN p.I_DUR >0 OR p.I_DUR =0  THEN p.I_DUR ELSE 0 END) AS response_time,
			SUM(CASE WHEN p.I_DUR >0 OR p.I_DUR =0 OR p.I_DUR = -1 THEN 1 ELSE 0 END) AS response_count,
			SUM(CASE WHEN (p.I_DUR >0  AND p.I_DUR<90 ) OR p.I_DUR =0 OR p.I_DUR = -1  THEN 1 ELSE 0 END) AS response_In90s_Count,
			SUM(CASE WHEN p.I_DUR >0 OR p.I_DUR =0 OR p.I_DUR = -1 THEN p.I_session_len ELSE 0 END) AS SESSION_time,
			SUM(CASE WHEN p.I_DUR >0 OR p.I_DUR =0 OR p.I_DUR = -1 THEN 1 ELSE 0 END) AS SESSION_count,
			SUM(CASE WHEN p.I_DUR >0 OR p.I_DUR =0 OR p.I_DUR = -1 THEN p.I_message_count ELSE 0 END) AS message_count,
			SUM(CASE WHEN p.I_DUR >0 OR p.I_DUR =0 OR p.I_DUR = -1 THEN p.I_reply_len ELSE 0 END) AS reception_time
		INTO responseTime,responseCount,responseIn90sCount,sessionTime,sessionCount,responseMessageCount,receptionTime
		FROM T_SESSION_DETAIL p 
		WHERE p.I_start_Time < endTime AND p.I_start_Time > startTime AND p.I_FROMID = v_current_submerchantid
		GROUP BY p.I_FROMID;
	--统计所有的咨询过的ID列表
		SELECT listagg( p.I_toid,';') within GROUP (order by p.I_toid) AS ASKING_IDS INTO askingIds
		FROM T_SESSION_DETAIL p 
		WHERE p.I_start_Time < endTime AND p.I_start_Time > startTime AND p.I_FROMID = v_current_submerchantid ;
		
		
	--统计所有客服回复过的客户的ID列表
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
	--插入数据到统计表，先删再插入
		DELETE FROM im.im_submerchant_workload p WHERE p.I_SUBMERCHANT_ID = v_current_submerchantid AND p.I_MERCHANT_ID = v_current_merchantid AND p.I_STATISTICS_TIME = statisticsDate;
		
 		insert into im.im_submerchant_workload(
  	I_SUBMERCHANT_ID,I_MERCHANT_ID,I_STATISTICS_TIME,I_RESPONSE_DURATION,I_SESSION_DURATION,I_SESSION_COUNT,I_RESPONSE_MESSAGE_COUNT,I_RECEPTION_DURATION,V_RESPONSE_IDS,V_ASKING_IDS,I_RESPONSE_COUNT,I_RESPONSE_IN_90S_COUNT)
  	values(v_current_submerchantid,v_current_merchantid,statisticsDate,responseTime,sessionTime,sessionCount,responseMessageCount,receptionTime,responseIds,askingIds,responseCount,responseIn90sCount);

	--
		v_str_submerchantid := v_res_str_submerchantid;
		v_str_merchantid := v_res_str_merchantid;
	end loop;
	END statisticsWorkLoad;


procedure ModifyCSPwd
  (
      t_userid in IM_USER.Userid%type,
      t_passwd in IM_USER.password%type,
      returncode out integer
  )
  is
    begin
          returncode := 0;

          UPDATE IM_USER SET PASSWORD=t_passwd WHERE USERID=t_userid;


          COMMIT;
          EXCEPTION
          when OTHERS then
          begin
              returncode := 1;
              rollback;
              raise;
              return;
          end;
    end ModifyCSPwd;

    procedure GetCSPWD
  (
    t_username in IM_USER.Username%type,
    t_userId out IM_USER.userId%type,
    t_userpwd out IM_USER.Password%type,
    t_status out IM_USER.status%type,
    ReturnCode out integer
   )
  is
    begin
         returnCode := 0;
         select USERID,PASSWORD,STATUS into t_userId, t_userpwd ,t_status from IM_USER where USERNAME = t_username;
         EXCEPTION
         when NO_DATA_FOUND THEN
         begin
              returncode := 1403;
              return;
         end;
         when OTHERS then
         begin
             returnCode := 2;
             return;
         end;

    end GetCSPWD;


    procedure GetVaildCS
 (
       cur_CSlist out T_CURSOR,
       returncode out integer
  )
  is
     begin
          returncode := 0;
          OPEN cur_CSlist FOR
          SELECT REALNAME,USERID FROM IM_CSINFO where REALNAME is not null;

          EXCEPTION
          when OTHERS then
          begin
              returncode := 1;
              raise;
              return;
          end;
     end GetVaildCS;

     procedure ImportCSReq
 (
     t_account in IM_USER.Username%type,
     t_permission in IM_CSINFO.permission%type,
     t_receptionlimit in IM_CSINFO.RECEPTIONLIMIT%type,
	   t_importname in IM_CSINFO.IMPORTNAME%type,
     t_id out IM_USER.userid%type,
     t_importtime in IM_CSINFO.IMPORTTIME%type,
     t_importcsid in im_CSINFO.Importcsid%type,
	   ReturnCode out integer
 )
  is
  t_num integer;
  	begin
	t_id := 0;
	returncode := 0;
  SELECT COUNT(1) INTO t_num FROM IM_CSINFO WHERE Username=t_account;
  IF t_num=0 THEN
    begin
  SELECT IM_CS_USER_SEQ_USERID.nextval into t_id FROM DUAL;
	INSERT INTO IM_USER(USERID,USERNAME,PASSWORD,STATUS) VALUES(t_id,t_account,'96e79218965eb72c92a549dd5a330112',1);
  INSERT INTO IM_USERINFO(USERID,NICKNAME)VALUES(t_id,t_account);
	INSERT INTO IM_CSINFO(USERID,USERNAME,PERMISSION,RECEPTIONLIMIT,IMPORTNAME,IMPORTTIME,STATUS,IMPORTCSID) VALUES (t_id,t_account,t_permission,t_receptionlimit,t_importname,t_importtime,1,t_importcsid);
	 COMMIT;
   end;
   ELSIF t_num= 1 THEN
     begin
      UPDATE IM_CSINFO SET PERMISSION=t_permission, RECEPTIONLIMIT=t_receptionlimit,IMPORTNAME=t_importname,IMPORTTIME=t_importtime,STATUS=1  WHERE Username=t_account;
      COMMIT;
      end;
    ELSE
      begin
        returncode := 1;
        return;
      end;
    END IF;
	EXCEPTION
	when OTHERS then
      	begin
        	returncode := 1;
        	rollback;
        	return;
      	end;
  end ImportCSReq;

  procedure ModifyCSReq
  (
        t_receptionlimit in IM_CSINFO.RECEPTIONLIMIT%type,
	      t_username in IM_CSINFO.USERNAME%type,
      	t_realname in IM_CSINFO.REALNAME%type,
        t_password in IM_USER.PASSWORD%type,
        t_importtime out IM_CSINFO.IMPORTTIME%type,
        t_csid in IM_CSINFO.CSID%type,
        t_permission in IM_CSINFO.PERMISSION%type,
      	returncode out integer
  )
  is
        t_id integer;
    begin
         returncode := 0;
         UPDATE IM_CSINFO SET REALNAME= t_realname,RECEPTIONLIMIT=t_receptionlimit,CSID = t_csid,PERMISSION = t_permission WHERE USERNAME= t_username;
         SELECT IMPORTTIME,USERID into t_importtime,t_id FROM IM_CSINFO WHERE USERNAME= t_username;
         UPDATE IM_USERINFO SET NICKNAME = t_realname WHERE USERID = t_id;
         if t_password is not null then
           UPDATE IM_USER SET PASSWORD= t_password WHERE USERNAME= t_username;
           end if;
         commit;
         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             rollback;
             return;
         end;
    end ModifyCSReq;

     procedure QureryCSInfo
  (
  cur_CSInfolist out T_CURSOR,
	t_username in  IM_CSINFO.USERNAME%type,
	t_importname in IM_CSINFO.IMPORTNAME%type,
	t_realname in IM_CSINFO.REALNAME%type,
	t_status in IM_CSINFO.STATUS%type,
	t_begintime in IM_CSINFO.IMPORTTIME%type,
	t_endtime in IM_CSINFO.IMPORTTIME%type,
       returncode out integer
  )
is
   strsql varchar2(4000);
     begin
          returncode := 0;

	strsql :='SELECT USERNAME,PERMISSION,IMPORTNAME,IMPORTTIME,REALNAME,STATUS,RECEPTIONLIMIT from IM_CSINFO where 1=1';
	IF t_username is not NULL then
    strsql := strsql ||'and USERNAME = ''' || t_username || '''';

	 END IF;
	IF t_importname is not NULL then
		strsql := strsql || 'and IMPORTNAME= '''|| t_importname || '''';
	END IF;
	IF t_realname is not NULL then
		strsql := strsql ||  'and REALNAME= '''|| t_realname || '''';
	END IF;
	IF t_status !=888 then
		strsql := strsql || 'and STATUS=' || t_status;
	END IF;
	IF t_begintime !=0 then
		strsql := strsql || 'and IMPORTTIME >=' || t_begintime;
	END IF;

	IF t_endtime !=0  then
		strsql := strsql || 'and IMPORTTIME<=' || t_endtime;
	END IF;
  dbms_output.put_line(strsql);
	OPEN cur_CSInfolist FOR strsql;

	    EXCEPTION
       when OTHERS then
       begin
           returncode := 1;
           raise;
           return;
        end;
  end QureryCSInfo;


  procedure ModifyCSStatusReq
  (
      t_username in IM_CSINFO.USERNAME%type,
      t_status in IM_CSINFO.STATUS%type,
      returncode out integer
  )
  is
   t_id integer;
    begin
          returncode := 0;
	  IF t_status = 0 or t_status = 1  then
	begin
     UPDATE IM_CSINFO SET STATUS=t_status  WHERE USERNAME=t_username;
    commit;
	end;
    elsif t_status =2 then
   begin
     select USERID into t_id FROM IM_CSINFO WHERE USERNAME=t_username;
     delete from IM_CSINFO where USERNAME=t_username;
     delete from IM_USER where USERID=t_id;
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
    raise;
      return;
    end;
    end ModifyCSStatusReq;

 procedure CreateGroup
(
	t_account in IM_CSGROUP.GROUPNAME%type,
	t_describ in IM_CSGROUP.GROUPDESCRIB%type,
	t_importname in IM_CSGROUP.LASTMODIFYNAME%type,
	t_importtime in IM_CSGROUP.LASTMODIFYTIME%type,
	t_id out IM_CSGROUP.GROUPID%type,
	ReturnCode out integer

)
  is
  begin
      t_id := 0;
      returncode := 0;
      begin
	 SELECT IM_CSGROUP_SEQ_GROUPID.nextval into t_id FROM DUAL;
   INSERT INTO IM_CSGROUP(GROUPID,GROUPNAME,GROUPDESCRIB,LASTMODIFYNAME,LASTMODIFYTIME) VALUES(t_id ,t_account,t_describ ,t_importname ,t_importtime);   
   	COMMIT;
    end;
      EXCEPTION
      when OTHERS then
      begin
        returncode := 1;
        rollback;
        return;
      end;
  	end CreateGroup;   
    
    procedure ModifyGroup
(
	t_id in IM_CSGROUP.GROUPID%type,
	t_account in IM_CSGROUP.GROUPNAME%type,
	t_describ in IM_CSGROUP.GROUPDESCRIB%type,
	t_importname in IM_CSGROUP.LASTMODIFYNAME%type,
	t_importtime in IM_CSGROUP.LASTMODIFYTIME%type,
	ReturnCode out integer

)
  is
  begin
      returncode := 0;
      begin
   	UPDATE IM_CSGROUP SET GROUPNAME=t_account,GROUPDESCRIB=t_describ,LASTMODIFYNAME=t_importname,LASTMODIFYTIME=t_importtime WHERE GROUPID= t_id;  
   	COMMIT;
    end;
      EXCEPTION
      when OTHERS then
      begin
        returncode := 1;
        rollback;
        return;
      end;
  	end ModifyGroup;

    
procedure AddGroupMem
 (
     t_groupid in IM_CSGROUPMEM.GROUPID%type,
     t_ids in VARCHAR2,
     t_importname in IM_CSGROUP.LASTMODIFYNAME%type,
     t_importtime in IM_CSGROUP.LASTMODIFYTIME%type,
	   ReturnCode out integer
 )
  is
  t_num integer;
  t_userid integer;
  v_str_merchantid VARCHAR2(4000):=t_ids;
  v_res_str_merchantid VARCHAR2(4000):='';
  v_current_merchantid VARCHAR2(4000):='';
  current_time_stamp_p TIMESTAMP(6) := systimestamp;
  current_date_p DATE := sysdate;
begin
  returncode := 0;
loop
	EXIT WHEN v_str_merchantid IS NULL;
	
	SplitStringSp(v_str_merchantid, v_res_str_merchantid, v_current_merchantid, ',');
	t_userid := TO_NUMBER(v_current_merchantid);

	MERGE INTO IM.IM_CSGROUPMEM groupTable   
	USING ( SELECT t_userid AS userid ,t_groupid AS groupid ,1 AS FLOWSTATUS ,current_date_p AS INSERT_TIME ,current_time_stamp_p AS UPDATE_TIME FROM dual) tableList  
	ON (tableList.userid = groupTable.USERID AND groupTable.GROUPID = t_groupid)   
	WHEN MATCHED THEN   
			UPDATE SET groupTable.INSERT_TIME=current_date_p , groupTable.UPDATE_TIME=current_time_stamp_p
	WHEN NOT MATCHED THEN   
			INSERT (USERID,GROUPID,FLOWSTATUS,INSERT_TIME,UPDATE_TIME) VALUES (t_userid ,t_groupid,1,current_date_p,current_time_stamp_p);

	v_str_merchantid := v_res_str_merchantid;
  COMMIT;
 end loop;
 

	DELETE FROM IM.IM_CSGROUPMEM WHERE (to_char(UPDATE_TIME,'yyyy-mm-dd HH24:mi:ss')<>to_char(current_time_stamp_p,'yyyy-mm-dd HH24:mi:ss') )AND GROUPID=t_groupid;
	EXCEPTION

	when OTHERS then
    begin
       returncode := 1;
       rollback;
       return;
     end;
end AddGroupMem;    


  procedure getUseridMapCSid
(
	t_id in IM_CSINFO.USERID%type,
	t_csid out IM_CSINFO.CSID%type,
	ReturnCode out integer

)
  is 
  begin
      returncode := 0;
        begin
	SELECT CSID into t_csid FROM IM_CSINFO WHERE USERID =t_id ;
          COMMIT;
        end;
      EXCEPTION
      when OTHERS then
      begin
        returncode := 1;
        rollback;
        return;
      end;
  	end getUseridMapCSid;    
  
 
procedure GetCSInfo
(
	t_userid in IM_CSINFO.userid%type,
	t_username out IM_CSINFO.username%type,
	t_status out IM_CSINFO.status%type,
	t_receptionlimit out IM_CSINFO.receptionlimit%type,
	t_permission out IM_CSINFO.permission%type,
	ReturnCode out integer
)
  is 
  begin
      returncode := 0;
   begin

  select username,status,receptionlimit,permission into t_username,t_status,
   t_receptionlimit,t_permission from IM_CSINFO  where userid = t_userid;
       COMMIT;
   end;
      EXCEPTION
      when OTHERS then
      begin
        returncode := 1;
        rollback;
        return;
      end;
  	end GetCSInfo;

procedure GetCSFriend
(
	t_userid in IM_CSGROUPMEM.userid%type,
	cur_InGroupMem out T_CURSOR,
	ReturnCode out integer

)
  is 
  begin
      returncode := 0;
   begin
	OPEN cur_InGroupMem FOR
    select B.userid,B.groupid,C.permission from  (select userid,groupid from IM_CSGROUPMEM  where groupid in (select GROUPID from IM_CSGROUPMEM t where USERID=t_userid) and USERID != t_userid )B,IM_CSINFO C where C.userid=B.userid;
    COMMIT;
   end;
      EXCEPTION
      when OTHERS then
      begin
        returncode := 1;
        rollback;
        return;
      end;
  	end GetCSFriend;
  
 procedure GetCSGroupMem
(
	t_groupid in IM_CSGROUPMEM.groupid%type,
	cur_InGroupMem out T_CURSOR,
	ReturnCode out integer
)
  is
  begin
      returncode := 0;
   begin
     IF t_groupid=0 THEN
  	OPEN cur_InGroupMem FOR 
             select distinct userid from IM_CSGROUPMEM
        COMMIT;
    ELSE
   	OPEN cur_InGroupMem FOR
      		select userid from IM_CSGROUPMEM where groupid = t_groupid;
    	COMMIT;    
    END IF;
  end;
      EXCEPTION
      when OTHERS then
      begin
        returncode := 1;
        rollback;
        return;
      end;   	
end GetCSGroupMem; 

procedure GetCSGroupMemFlow
(
	t_groupid in IM_CSGROUPMEM.groupid%type,
	cur_InGroupMem out T_CURSOR,
	ReturnCode out integer
)
  is
  begin
      returncode := 0;
   begin
     IF t_groupid=0 THEN
  	OPEN cur_InGroupMem FOR 
             select distinct userid from IM_CSGROUPMEM
        COMMIT;
    ELSE
   	OPEN cur_InGroupMem FOR
      		select userid from IM_CSGROUPMEM where (groupid = t_groupid and flowstatus=1);
    	COMMIT;    
    END IF;
  end;
      EXCEPTION
      when OTHERS then
      begin
        returncode := 1;
        rollback;
        return;
      end;   	
end GetCSGroupMemFlow; 

 procedure GetCSGroups
(
	t_userid in IM_CSGROUPMEM.userid%type,
	cur_InGroupMem out T_CURSOR,
	ReturnCode out integer
)
  is
  begin
      returncode := 0;
  	begin
   	OPEN cur_InGroupMem FOR
      		select groupid from IM_CSGROUPMEM where userid = t_userid;
    	COMMIT;

  	end;
      EXCEPTION
      when OTHERS then
      begin
        returncode := 1;
        rollback;
        return;
      end;
end GetCSGroups;
    
procedure WebHistoryMsg
	(
	  t_sid in IM_WEBDATA.FROMID%type,
    t_did in IM_WEBDATA.FROMID%type,
    t_msgid_bgn in IM_WEBDATA.MSGID%type,
	  t_time_bgn in IM_WEBDATA.SENDTIME%type,
    t_time_end in IM_WEBDATA.SENDTIME%type,
	  t_maxnum in integer,
	  curmsgInfo out T_CURSOR,
	  returncode out integer
	)
	is
	  v_sql varchar2(4000);
	  begin

	  	returncode := 0;

    IF t_msgid_bgn = 0 THEN  
		BEGIN
	  		v_sql := 'SELECT FROMID,TOID,SENDTIME,FONTSIZE,FONTCOLOR,FONTSTYLE,FONTNAME,FROMNICKNAME,MSG,MSGID,UUID,DATATYPE '
	           || 'FROM (SELECT * FROM IM_WEBDATA WHERE SENDTIME>  '||t_time_bgn||' AND SENDTIME< '||t_time_end||'' 
             || 'AND ((FROMID=  '||t_sid||' AND TOID= '||t_did||') OR (FROMID=  '||t_did||' AND TOID= '||t_sid||'))'
             || 'ORDER BY MSGID ASC) WHERE ROWNUM>0 AND ROWNUM<='||t_maxnum||'';

			OPEN curmsgInfo FOR v_sql;
		END;
    ELSE
    BEGIN
	  		v_sql := 'SELECT FROMID,TOID,SENDTIME,FONTSIZE,FONTCOLOR,FONTSTYLE,FONTNAME,FROMNICKNAME,MSG,MSGID,UUID,DATATYPE '
	           || 'FROM (SELECT * FROM IM_WEBDATA WHERE MSGID>  '||t_msgid_bgn||' AND SENDTIME< '||t_time_end||'' 
             || 'AND ((FROMID=  '||t_sid||' AND TOID= '||t_did||') OR (FROMID=  '||t_did||' AND TOID= '||t_sid||'))'
             || 'ORDER BY MSGID ASC) WHERE ROWNUM>0 AND ROWNUM<='||t_maxnum||'';

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
	  end WebHistoryMsg; 	
	  
procedure GetVaildCS_20160106
 	(
       cur_CSlist out T_CURSOR,
       returncode out integer
  )
  is
     begin
          returncode := 0;
          OPEN cur_CSlist FOR
          SELECT REALNAME,USERID,CSID FROM IM_CSINFO where REALNAME is not null;

          EXCEPTION
          when OTHERS then
          begin
              returncode := 1;
              raise;
              return;
          end;
     end GetVaildCS_20160106;    	 
     
 procedure SetMsgRemind                                
  (
      t_userid in IM_MSGREMIND.USERID%type,
      t_neworder in IM_MSGREMIND.NEWORDER%type,
      t_payorder in IM_MSGREMIND.PAYORDER%type,
      t_goodscomment in IM_MSGREMIND.GOODSCOMMENT%type,
      t_returnorder in IM_MSGREMIND.RETURNORDER%type,
      returncode out integer                  
  )
  is
     t_count integer;
     begin
          returncode := 0;
          SELECT COUNT(1) INTO t_count FROM IM_MSGREMIND WHERE USERID=t_userid;
          if t_count > 0 then    
            begin
             UPDATE IM_MSGREMIND SET NEWORDER=t_neworder,PAYORDER=PAYORDER,GOODSCOMMENT=t_goodscomment,RETURNORDER=t_returnorder WHERE USERID=t_userid;
             COMMIT;
             return;
            end;
          end if;
          
          INSERT INTO IM_MSGREMIND(USERID,NEWORDER,PAYORDER,GOODSCOMMENT,RETURNORDER) VALUES (t_userid,t_neworder,t_payorder,t_goodscomment,t_returnorder);
          COMMIT;
          
          EXCEPTION
          when OTHERS then
          begin
               returncode := 1;
               rollback;
               raise;
               return;
          end;

     end SetMsgRemind; 
     
 procedure GetMsgRemind
	(
	  t_userid in IM_USER.USERID%type,
	  curmsgremind out T_CURSOR,
	  returncode out integer
	)    
  is
     begin
          returncode := 0;
          OPEN curmsgremind FOR
          SELECT NEWORDER,PAYORDER,GOODSCOMMENT,RETURNORDER FROM IM_MSGREMIND where USERID=t_userid;

          EXCEPTION
          when OTHERS then
          begin
              returncode := 1;
              raise;
              return;
          end;
     end GetMsgRemind; 
     
 procedure GetRecentContact
	(
	  t_userguid in IM_USER.SIZEDS%type,
    t_starttime in IM_WEBLIST.SENDTIME%type,
	  recentcontact out T_CURSOR,
	  returncode out integer
	)
  is
     t_userid integer;
     begin
          returncode := 0;
          t_userid := 0;
          SELECT USERID INTO t_userid FROM IM_USER WHERE SIZEDS=t_userguid;
          if t_userid <> 0 then    
            begin
             OPEN recentcontact FOR
             SELECT TOID FROM IM_WEBLIST where FROMID=t_userid and SENDTIME>=t_starttime
             UNION
             SELECT FROMID FROM IM_WEBLIST where TOID=t_userid and SENDTIME>=t_starttime;
            end;
          end if;    

          EXCEPTION
          when OTHERS then
          begin
              returncode := 1;
              return;
          end;
     end GetRecentContact;
     
 procedure DeleteOneMsgNotify
	(
	  t_userid in IM_USER.USERID%type,
    t_uuid in IM_MSGNOTIFY.UUID%type,
	  returncode out integer
	)
  is
     t_count integer;
     begin
          returncode := 0;
          t_count := 0;
          DELETE FROM IM_USER_WITH_MSGNOTIFY WHERE USERID=t_userid and UUID=t_uuid;
          SELECT count(0) into t_count FROM IM_USER_WITH_MSGNOTIFY WHERE UUID=t_uuid;
          if t_count <> 0 then    
            begin
             DELETE FROM IM_MSGNOTIFY WHERE UUID=t_uuid;
            end;
          end if;    
          COMMIT;    
          EXCEPTION
          when OTHERS then
          begin
              returncode := 1;
              rollback;
              raise;
              return;
          end;
     end DeleteOneMsgNotify; 
     
     
  procedure GetMsgNotifyDataOnePerson
	(
	  t_userid in IM_USER.USERID%type,
    recentcontact out T_CURSOR,
	  returncode out integer
	)
  is
     begin
          returncode := 0;
          OPEN recentcontact FOR
          SELECT UUID FROM IM_USER_WITH_MSGNOTIFY WHERE USERID=t_userid;       

          EXCEPTION
          when OTHERS then
          begin
              returncode := 1;
              return;
          end;
     end GetMsgNotifyDataOnePerson;                    
	
/*
   procedure PayMoney
  (
         t_userid in number,
         t_consumetype in number,
         t_logid in number,
         t_money in  number,
         returncode out integer
  )
  is
    --tmp_consumetype number(10);
    begin
   --SELECT currmoney INTO usergold
   --       FROM blog_usermoney
   --       WHERE USERID = t_userid;
   --       if usergold > t_money then
   --         begin
   --              UPDATE blog_usermoney SET currmoney=currmoney-t_money WHERE USERID = t_userid;
   --              COMMIT;
   --         end;
   --       else
   --           returncode := 2;
   --       end if;

          --pkg_wsv10_money.consumeMoney(t_userid,130,t_money,'Mini大头贴消费','0.0.0.0',returncode);
          --tmp_consumetype := 130;
          --pkg_wsv10_money.consumeMoneyByEsp(t_userid,t_consumetype,t_money,'Mini大头贴消费','0.0.0.0',t_logid,returncode);
          if returncode = 11102 then
             returncode := 2;
          end if;
          EXCEPTION
          when NO_DATA_FOUND THEN
          begin
               returncode := 1403;
               return;
          end;
          when OTHERS then
          begin
               returncode := 1;
               raise;
               return;
          end;
    end PayMoney;
  procedure GetMobileMsg
  (
        t_transid in out mo_mini_smsmo.transId%type,
        t_count in out integer,
        cur_mobilemsg out T_CURSOR,
        returncode out integer
  )
  is
    begin
         returncode := 0;
         if t_transid = 0 then
            SELECT MIN(transId) INTO t_transid
            FROM  mo_mini_smsmo
            WHERE CREATETIME > trunc(sysdate);--TO_DATE(TO_CHAR(sysdate,'YYYY-MM-DD'),'YYYY-MM-DD');
         end if;
         if t_transid is NULL then
            SELECT MAX(transId) INTO t_transid
            FROM  mo_mini_smsmo;
         end if;
         if t_transid is NULL then
            t_transid := 0;
         end if;
         OPEN cur_mobilemsg FOR
              select a.TRANSID,a.RECEIVERID,a.SENDERID,a.MOBILENO,a.CONTENT,a.LINKID,a.BIZTYPE,b.nickname,b.username,TO_CHAR(a.createtime,'YYYY/MM/DD HH24:MI:SS')
              FROM
              (
                   select TRANSID,RECEIVERID,SENDERID,MOBILENO,CONTENT,LINKID,BIZTYPE,createtime FROM mo_mini_smsmo where transId > t_transid and rownum < t_count
              ) a,
              blog_userbaseinfo b
              where b.userid(+) = a.RECEIVERID;
         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             raise;
             return;
         end;
    end GetMobileMsg;

  procedure GetOneMobileMsg
  (
        t_transid in out mo_mini_smsmo.transId%type,
        t_userid in mo_mini_smsmo.receiverid%type,
        t_count in out integer,
        cur_onemobilemsg out T_CURSOR,
        returncode out integer
  )
  is
    begin
         returncode := 0;
         if t_transid = 0 then
            SELECT MIN(transId) INTO t_transid
            FROM  mo_mini_smsmo
            WHERE CREATETIME > trunc(sysdate);--TO_DATE(TO_CHAR(sysdate,'YYYY-MM-DD'),'YYYY-MM-DD');
         end if;
         if t_transid is NULL then
            SELECT MAX(transId) INTO t_transid
            FROM  mo_mini_smsmo;
         end if;
         if t_transid is NULL then
            t_transid := 0;
         end if;
         OPEN cur_onemobilemsg FOR
              select a.TRANSID,a.RECEIVERID,a.SENDERID,a.MOBILENO,a.CONTENT,a.LINKID,a.BIZTYPE,b.nickname,b.username,TO_CHAR(a.createtime,'YYYY/MM/DD HH24:MI:SS')
              FROM
              (
                   select TRANSID,RECEIVERID,SENDERID,MOBILENO,CONTENT,LINKID,BIZTYPE,createtime FROM mo_mini_smsmo where transId > t_transid and RECEIVERID = t_userid and rownum < t_count
              ) a,
              blog_userbaseinfo b
              where b.userid(+) = a.RECEIVERID;
         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             raise;
             return;
         end;
    end GetOneMobileMsg;

  procedure SendMobileMsg
  (
        t_userid in blog_userbaseinfo.userid%type,
        t_biztype in mo_mt_sms.biztype%type,
        t_bizid in mo_mt_sms.bizid%type,
        t_smstype in mo_mt_sms.smstype%type,
        t_linkid in mo_mt_sms.linkid%type,
        t_content in mo_mt_sms.content%type,
        t_nickname in blog_userbaseinfo.nickname%type,
        returncode out integer
  )
  is
    var_mobileno varchar2(16);
    var_sendcontent varchar2(140);
    var_bdcontent varchar2(140);
    hasservice integer;
    hasshow integer;
    mobilestatus integer;
    smstime integer;
    begin
         --该业务bizid = 发送者userid,t_userid表示接收者ID
         returncode := 0;
         select usermobilestatus,MOBILENO into mobilestatus,var_mobileno
         from blog_userbaseinfo
         where userid = t_userid;
         if mobilestatus = 0 then
            returncode := 2; --未绑定手机
            return;
         end if;
         if var_mobileno is NULL then
            returncode := 3; --手机号错误
            return;
         end if;

         --本月可以发送的短信数
         select smsCnt into smstime from sms_userInfo where userid = t_bizid;
         if smstime <= 0 then
            returncode := 5;
            return;
         end if;
         var_sendcontent := t_nickname || ' 内容:' || t_content || '[我酷网]';

         insert into mo_mt_sms(transid,mobileno,content,biztype,bizid,smstype)
         values(SEQ_MO_MTSMS_TRANSID.Nextval,var_mobileno,var_sendcontent,t_biztype,t_bizid,0);
         --发送条数加1
         update sms_userInfo set smsCnt=smsCnt-1,sendCnt=sendCnt+1,totalSendCnt=totalSendCnt+1 where userid = t_bizid;


         --是否定制业务
         select count(*) into hasservice from mo_user_service t1 where t1.mobileno=var_mobileno and t1.status=0 and t1.serviceid in ( select sid from mo_serviceinfo where feeType=1 and servicetype=0 and OperatorCode=0 and status=0 );
         if hasservice = 0 then --未定制
           begin
                --判断是否已经发送过定制广告提示
                Select count(*) into hasshow from mo_mt_sms where mobileno = var_mobileno and bizid=105;
                if hasshow = 0 then
                begin
                     var_bdcontent :='想和'|| t_nickname ||'一样可以发免费的自写短信吗？只要发J到10669658开通免费的空间加油站即可，同时免费拥有超酷的个人空间[我酷网]';
                     insert into mo_mt_sms(transid,mobileno,content,biztype,bizid,smstype)
                     values(SEQ_MO_MTSMS_TRANSID.Nextval,var_mobileno,var_bdcontent,401,105,0);
                end;
                end if;
           end;
         end if;
         commit;
         EXCEPTION
         when OTHERS then
         begin
             returncode := 1;
             raise;
             return;
         end;
    end  SendMobileMsg;
   */


end Mini_User_Pkg;
/


spool off
