#ifndef _PROTOCAL_
#define _PROTOCAL_

#include "PublicType.h"

/************************************************************************
*
* �ļ�����protocol.h
*
* �ļ�����: �¿��ͨѶЭ����ص�������
*
* �����ˣ� ѦС��,2005��8��25��
*
* �汾�ţ�2.0
*
* �޸ļ�¼�� �����¿��ϵͳ���������ͨѶЭ�������ֶ��� by zouyf 2006-06-15
*
************************************************************************/




#define SENDER_XTREAM_SERVER		0x0102
#define XTREAM_CLIENT_VERSION		0x011D





//Ⱥ�������Ա���
#define CMEMBER_CREATOR             2
#define CMEMBER_ADMIN               1
#define CMEMBER_NORMAL              0

//��Э��涨 0Ϊ�ɹ���־,��0Ϊ������
#define	V_SUCCESS					0
#define V_FAIL						1

//������Ϣ
#define CMD_PUBLIC_MSG				0x00d


//�û�ע��
#define CMD_REGIST_REQ				0x201
#define CMD_REGIST_ACK				0x202

#define CMD_LOGOUT					0x203

//��¼
#define CMD_LOGIN_REQ				0x211
#define CMD_LOGIN_ACK				0x212

//����,Directory
#define CMD_DIRECTORY_REQ			0x301
#define CMD_DIRECTORY_ACK			0x302

//�����û�״̬
#define CMD_STATUS_MOD_REQ				0x303
#define CMD_STATUS_MOD_ACK				0x304

//��ȡ�ͷ���������Ϣ
#define CMD_CS_TEAM_INFO_REQ			0x305
#define CMD_CS_TEAM_INFO_ACK			0x306

//���Ϳͷ��������Ŷ�����Ϣ
#define CMD_CS_TEAM_QINFO_REQ			0x307
#define CMD_CS_TEAM_QINFO_ACK			0x308

#define CMD_USERFAMILYFLAG_GET_REQ		0x413
#define CMD_USERFAMILYFLAG_GET_ACK		0x414

//��ѯ�û�����
#define CMD_USERINFO_GET_REQ		0x415
#define CMD_USERINFO_GET_ACK		0x416

//�޸��û�����
#define CMD_USERINFO_MOD_REQ		0x417
#define CMD_USERINFO_MOD_ACK		0x418

//����޸�ɾ����
#define CMD_GROUPINFO_MOD_REQ		0x501
#define CMD_GROUPINFO_MOD_ACK		0x502

//��ȡ����Ϣ
#define CMD_GROUPINFO_GET_REQ		0x503
#define CMD_GROUPINFO_GET_ACK		0x504

//��ȡ�����嵥
#define CMD_FRIENDLIST_REQ			0x601
#define CMD_FRIENDLIST_ACK			0x602

//��ѯxtream�û�
#define CMD_XTREAM_SEARCH_REQ		0x603
#define CMD_XTREAM_SEARCH_ACK		0x604

//���,�޸�,ɾ�����ѷ���
#define CMD_FRIENDGROUP_MOD_REQ		0x605
#define CMD_FRIENDGROUP_MOD_ACK		0x606

//������֤����Ӧ��
#define CMD_USERAUTH_REQ			0x607
#define CMD_USERAUTH_IND			0x607
#define CMD_USERAUTH_ACK			0x608

//P2P������Ϣ
#define CMD_P2P_TEST_REQ			0x609
#define CMD_P2P_TEST_ACK			0x60a
#define CMD_P2PMSG_SEND				0x60b
#define CMD_P2PMSG_RECV				CMD_P2PMSG_SEND
#define CMD_P2PMSG_SEND_OFFLINE		0x160b
#define CMD_P2PMSG_ACK				0x60c

//���������͸��ͻ��˵�ϵͳ��Ϣ��ת����Ϣ
#define CMD_SERVERMSG_IND			0x60d
#define CMD_SERVERMSG_IND_ACK		0x60e



//��ȡ���ѣ��Լ���������������Ӧ��,֪ͨ
#define CMD_GET_FRIEND_BASICINFO_REQ	0x0611
#define CMD_GET_FRIEND_BASICINFO_ACK	0x0612
#define CMD_GET_FRIEND_BASICINFO_NOTIFY 0x0613


//��ȡ��ʷ��Ϣ
#define CMD_WEB_MORE_MSG_REQ	0x0616
#define CMD_WEB_MORE_MSG_ACK	0x0617

//web�˻�ȡ���
#define CMD_WEB_BUSI_LIST_REQ	0x0618
#define CMD_WEB_BUSI_LIST_ACK	0x0619

//��������������ĺ���״̬����֪ͨ
#define CMD_FRIEND_STATUS_IND		0x701
#define CMD_FRIEND_STATUS_RET		0x702


//�ͻ��˵�¼�ɹ��������������������Ϣ
#define CMD_OFFLINEMSG_REQ			0x703
#define CMD_OFFLINEMSG_ACK			0x704

#define CMD_STATUS_GET_REQ			0x705
#define CMD_STATUS_GET_ACK			0x706

// �޸ĺ��ѱ�ע
#define CMD_MOD_FCNAME_REQ			0x707
#define CMD_MOD_FCNAME_ACK			0x708
// �����ѷ����ʼ�����
#define CMD_FRIENDINVITE_REQ		0x709
#define CMD_FRIENDINVITE_ACK		0x70a

//�޸ĸ���ǩ��
#define CMD_MODIFYUNDERWRITE_REQ	0x070b
#define CMD_MODIFYUNDERWRITE_ACK	0x070c

//�޸ĺ��ѹ�ע���
#define CMD_MODIFYFRIENDFLAG_REQ	0x070d
#define CMD_MODIFYFRIENDFLAG_ACK	0x070e

//���֧��
#define CMD_PAYMONEY_REQ			0x0711
#define CMD_PAYMONEY_ACK			0x0712

#define CMD_FRIEND_INFO_IND			0x0713

#define CMD_CREATE_CLUSTER_REQ      0x901
#define CMD_CREATE_CLUSTER_ACK      0x902

#define CMD_SET_CMEMBERS_REQ        0x903
#define CMD_SET_CMEMBERS_ACK        0x904

#define CMD_SET_CINFO_REQ           0x905
#define CMD_SET_CINFO_IND           CMD_SET_CINFO_REQ
#define CMD_SET_CINFO_ACK           0x906

#define CMD_GET_CINFO_REQ           0x907
#define CMD_GET_CINFO_ACK           0x908

//Ⱥ��Ա���˳�����ӣ�֪ͨ
#define CMD_CMEMBERS_IND          CMD_SET_CMEMBERS_REQ

#define CMD_CLUSTER_SEARCH_REQ      0x909
#define CMD_CLUSTER_SEARCH_ACK      0x90a

#define CMD_CLUSTER_APPLY_REQ       0x90b
#define CMD_CLUSTER_APPLY_ACK       0x90c

#define CMD_CLUSTER_AUTH_REQ        0x90d
#define CMD_CLUSTER_AUTH_ACK        0x90e
#define CMD_CLUSTER_AUTH_IND        CMD_CLUSTER_AUTH_REQ

#define CMD_CLUSTERMSG_SEND         0x911
#define CMD_CLUSTERMSG_ACK          0x912
#define CMD_CLUSTERMSG_SEND_OFFLINE 0x1911

#define CMD_CLUSTERMSG_RECV         0x917
#define CMD_CLUSTERMSG_RET          0x918

#define CMD_GET_CMEMBERS_REQ        0x913
#define CMD_GET_CMEMBERS_ACK        0x914

#define CMD_CDISMISS_REQ			0x915
#define CMD_CDISMISS_ACK            0x916
#define CMD_CDISMISS_IND			CMD_CDISMISS_REQ

#define CMD_COFFLINE_MSG_REQ        0x919
#define CMD_COFFLINE_MSG_ACK        0x91a

#define CMD_GET_CLIST_REQ           0x91d
#define CMD_GET_CLIST_ACK           0x91e

#define CMD_CLUSTER_NOTIFY_REQ		0x920	//�û���Ⱥ����״̬�ı�֪ͨ
#define CMD_CLUSTER_NOTIFY_ACK		0x921

#define CMD_FAMILYOFFLINEMSG_REQ	0x922	//��ȡ����������Ϣ(�ɷ��������������������)
#define CMD_FAMILYOFFLINEMSG_ACK	0x923

//��ȡȺ��Ƭ all
#define CMD_GET_PROFILE_NAME_REQ	0x924
#define CMD_GET_PROFILE_NAME_ACK	0x925

//����Ⱥ��Ƭ
#define CMD_SET_PROFILE_NAME_REQ	0x926
#define CMD_SET_PROFILE_NAME_ACK	0x927

//��ȡȺ��Ƭ byid
#define CMD_GET_PROFILE_NAME_BYID_REQ	0x928
#define CMD_GET_PROFILE_NAME_BYID_ACK	0x929


//Ⱥ���ϴ�ͼƬ
#define CMD_SEND_CPIC_REQ			0x931
#define CMD_SEND_CPIC_ACK			0x932
//Ⱥ������ͼƬ
#define CMD_GET_CPIC_REQ			0x933
#define CMD_GET_CPIC_ACK			0x934

#define CMD_CLUSTER_INVITE_REQ      0x935	// ���������Ⱥ����
#define CMD_CLUSTER_INVITE_ACK      0x936	// ���������ȺӦ��

#define CMD_CLUSTER_AGREE_REQ      0x937	// ���������Ƿ���Ⱥ ����
#define CMD_CLUSTER_AGREE_ACK      0x938	// ���������Ƿ���Ⱥ Ӧ��

//#define CMD_START_SEND_CPIC_REQ		0x941
#define CMD_START_SEND_CPIC_ACK		0x942	// ����ʼ����ȺͼƬӦ��

//��ȡ����������˺�
#define CMD_GET_SUB_ACCOUNT_REQ		0x943
#define CMD_GET_SUB_ACCOUNT_ACK		0x944

//�ж��Ƿ������˺�����
#define CMD_GET_SUB_ONLINE_REQ		0x945
#define CMD_GET_SUB_ONLINE_ACK		0x946

//��ѯ�û�����״̬
#define CMD_GET_USER_STATUS_REQ		0x947
#define CMD_GET_USER_STATUS_ACK		0x948


#define CMD_SEND_MOBILE_REQ		0x1f10
#define CMD_SEND_MOBILE_ACK		0x1f11
#define CMD_MOBILE_IND			0x1f12		//�������ŵ��ͻ���
#define CMD_MOBILE_ACK			0x1f13

#define CMD_WEBSYSLOG_IND		0x1f1a		//����վͬ��ҵ��
#define CMD_WEBSYSLOG_ACK		0x1f1b


#define CMD_WOKU_IND			0x1f1c
#define CMD_WOKU_ACK			0x1f1d
#define CMD_WEB_IND                 0x1f1e
#define CMD_WEB_ACK                 0x1f1f

#define CMD_TRANSFILE_REQ			0x0c01
#define CMD_TRANSFILE_ACK			0x0c02

#define CMD_ROUTERUSERPACKET_REQ			0x8009
#define CMD_ROUTERUSERPACKET_ACK			0x800a
#define CMD_ROUTERPACKET_REQ			0x800b
#define CMD_ROUTERPACKET_ACK			0x800c

#define CMD_FILE_TRANS_REQ   	0x8010
#define CMD_FILE_TRANS_ACK   	0x8011

#define CMD_FILE_TRANS_IND   	0x8012
#define CMD_FILE_TRANS_IND_ACK  0x8013

#define CMD_FILE_TRANS_NOTIFY   0x8019
#define CMD_FILE_TRANS_NOTIFY_ACK	0x801a

#define CMD_TRANS_LOGIN_REQ			0x8101
#define CMD_TRANS_LOGIN_ACK			0x8102

#define CMD_TRANS_SETRECV_REQ		0x8103
#define CMD_TRANS_SETRECV_ACK		0x8104

#define CMD_TRANS_RECVLOGIN_REQ		0x8107
#define CMD_TRANS_RECVLOGIN_ACK		0x8108

#define CMD_TRANS_NOTIFYSENDER_IND	0x8109
#define CMD_TRANS_NOTIFYSENDER_ACK	0x810a
#define CMD_CUSTSER_TRAN_REQ	0x801B		// ת��ת������
#define CMD_CUSTSER_TRAN_ACK	0x801C		// ת��ת��Ӧ��

#define CMD_TRANS_SENDERCLOSE_REQ	0x8111
#define CMD_TRANS_SENDERCLOSE_ACK	0x8112

#define CMD_TRANS_NOTIFYCLOSE_IND	0x8113
#define CMD_TRANS_NOTIFYCLOSE_ACK	0x8114

// ��ȡ�̻������ϵ�Ŀͻ���Ϣ
#define CMD_GET_NEAR_COMM_CUST_REQ         0x0A07
#define CMD_GET_NEAR_COMM_CUST_ACK         0x0A08

// �����̻�������ϵ�ͻ���Ϣ
#define CMD_UPDATE_NEAR_COMM_CUST_INFO_REQ		0x0A09
#define CMD_UPDATE_NEAR_COMM_CUST_INFO_ACK		0x0A10

//���������ϵ�ͻ���ע����̨
#define CMD_UPDATE_NEAR_COMM_CUST_REMARK_REQ		0x0A15
#define CMD_UPDATE_NEAR_COMM_CUST_REMARK_ACK		0x0A16

//�Ӻ�̨��ȡ�����ϵ�ͻ���ע
#define CMD_GET_NEAR_COMM_CUST_REMARK_REQ		0x0A17
#define CMD_GET_NEAR_COMM_CUST_REMARK_ACK		0x0A18


#define CMD_NETWORK_ERR				0x2000



//#define CMD_FACEURL_REQ             0x0A03
//#define CMD_FACEURL_ACK             0x0A04


/***
ȫ�ַ�����ҵ��Э��������
***/
#define CMDGSRV_BASE  0x8200

//��ѯ�����û�ȫ������
#define CMDGSRV_GET_USER_OTHER_QUERY			0x8887
//��ѯ�����û�ȫ������Ӧ��
#define CMDGSRV_GET_USER_OTHER_ACK			0x8888

//��ѯ�����û�ȫ��״̬
#define CMDGSRV_USERSTATUS_OTHER_QUERY			0x888A
//��ѯ�����û�ȫ��״̬Ӧ��
#define CMDGSRV_USERSTATUS_OTHER_ACK			0x888B

//ɾ�������û�ȫ������
#define CMDGSRV_DEL_USER_OTHER_QUERY			0x888C
//ɾ�������û�ȫ������Ӧ��
#define CMDGSRV_DEL_USER_OTHER_ACK			0x888D

//��ѯ���߼���ȫ������
#define CMDGSRV_FAMILYINFO_OTHER_QUERY			0x888E
//��ѯ���߼���ȫ������Ӧ��
#define CMDGSRV_FAMILYINFO_OTHER_ACK		0x888F

//������߼���ȫ������
#define CMDGSRV_ADD_FAMILY_OTHER_QUERY			0x8890
//������߼���ȫ������Ӧ��
#define CMDGSRV_ADD_FAMILY_OTHER_ACK			0x8891

//ɾ�����߼���ȫ������
#define CMDGSRV_DEL_FAMILY_OTHER_QUERY			0x8892
//ɾ�����߼���ȫ������Ӧ��
#define CMDGSRV_DEL_FAMILY_OTHER_ACK			0x8893

//�޸������û�״̬ȫ������
#define CMDGSRV_MOD_USERSTATUS_OTHER_QUERY			0x8894
//�޸������û�״̬ȫ������Ӧ��
#define CMDGSRV_MOD_USERSTATUS_OTHER_ACK			0x8895

//�޸������û�����ȫ������
#define CMDGSRV_MOD_USERFLOW_OTHER_QUERY			0x8896
//�޸������û�����ȫ������Ӧ��
#define CMDGSRV_MOD_USERFLOW_OTHER_ACK			0x8897

//��ȡ��ѷ������˺�
#define CMDGSRV_GET_BEST_OTHER_ACCOUNT_QUERY		0x8898
//��ȡ��ѷ������˺�Ӧ��
#define CMDGSRV_GET_BEST_OTHER_ACCOUNT_ACK		0x8899

//�ļ������������ѯ�û���Ϣ
#define CMDGSRV_TUSERINFO_OTHER_QUERY				0x889A
#define CMDGSRV_TUSERINFO_OTHER_ACK				0x889B

//��ѯ�����û�������Ϣ
#define CMDGSRV_USERCONTROLINFO_OTHER_QUERY		0x889C
//��ѯ�����û�������ϢӦ��
#define CMDGSRV_USERCONTROLINFO_OTHER_ACK	0x889D








//�����û�ȫ�ֲ�ѯ
#define CMDGSRV_SEARCH_USER_QUERY				0x8201
//�����û�ȫ�ֲ�ѯӦ��
#define CMDGSRV_SEARCH_USER_ACK					0x8202

//��ѯ�����û�״̬
#define CMDGSRV_GET_USERSTATUS_QUERY			0x8203
//��ѯ�����û�״̬Ӧ��
#define CMDGSRV_GET_USERSTATUS_ACK				0x8204

//�޸������û�״̬
#define CMDGSRV_MODIFY_USERSTATUS_QUERY			0x8205
//�޸������û�״̬Ӧ��
#define CMDGSRV_MODIFY_USERSTATUS_ACK			0x8206

//��ѯ�����û�ȫ������
#define CMDGSRV_GET_USER_FULLINFO_QUERY			0x8207
//��ѯ�����û�ȫ������Ӧ��
#define CMDGSRV_GET_USER_FULLINFO_ACK			0x8208

//���/�޸�ȫ���û�����
#define CMDGSRV_ADDMODIFY_USER_FULLINFO_QUERY	0x8209
//���/�޸�ȫ���û�����Ӧ��
#define CMDGSRV_ADDMODIFY_USER_FULLINFO_ACK		0x8210

//ɾ���û�ȫ������
#define CMDGSRV_DEL_USER_FULLINFO_QUERY			0x8211
//ɾ���û�ȫ������Ӧ��
#define CMDGSRV_DEL_USER_FULLINFO_ACK			0x8212

//��ѯ���߼���ȫ������
#define CMDGSRV_FAMILYINFO_QUERY			0x8213
//��ѯ���߼���ȫ������Ӧ��
#define CMDGSRV_FAMILYINFO_QUERY_ACK		0x8214
//���ȫ�ּ�������
#define CMDGSRV_FAMILY_ADD_QUERY			0x8215
//���ȫ�ּ�������Ӧ��
#define CMDGSRV_FAMILY_ADD_ACK				0x8216
//ɾ��ȫ�ּ�������
#define CMDGSRV_FAMILY_DEL_QUERY			0x8217
//ɾ��ȫ�ּ�������Ӧ��
#define CMDGSRV_FAMILY_DEL_ACK				0x8218

//��ѯ�����û�������Ϣ
#define CMDGSRV_USERCONTROLINFO_QUERY		0x8219
//��ѯ�����û�������ϢӦ��
#define CMDGSRV_USERCONTROLINFO_QUERY_ACK	0x8220


//֪ͨҵ��������û����ص�¼
#define CMDGSRV_USERRELOGIN_NOTIFY			0x8221
//֪ͨҵ��������û����ص�¼Ӧ��
#define CMDGSRV_USERRELOGIN_ACK				0x8222


//IMҵ�������ͣ��֪ͨ
#define	CMDCSRV_IMSERVER_SHUTDOWN_NOTIFY	0x8231
//IMҵ�������ͣ��֪ͨӦ��
#define CMDCSRV_IMSERVER_SHUTDOWN_ACK		0x8232

//��ѯ����
#define CMDGSRV_GET_USERFLOW_QUERY			0x8233
//��ѯ����Ӧ��
#define CMDGSRV_GET_USERFLOW_ACK			0x8234

//�޸ķ���
#define CMDGSRV_MODIFY_USERFLOW_QUERY		0x8235
//�޸ķ���Ӧ��
#define CMDGSRV_MODIFY_USERFLOW_ACK			0x8236

//��ȡ��ѷ������˺�
#define CMDGSRV_GET_BEST_ACCOUNT_QUERY		0x8237
//��ȡ��ѷ������˺�Ӧ��
#define CMDGSRV_GET_BEST_ACCOUNT_ACK		0x8238

//֪ͨ�ļ�����������û���¼
#define CMDGSRV_NOTIFY_TRANSFER_LOGIN		0x8800
#define CMDGSRV_NOTIFY_TRANSFER_LOGIN_ACK	0x8801

//֪ͨ�ļ�����������û��ǳ�
#define CMDGSRV_NOTIFY_TRANSFER_LOGOUT		0x8802
#define CMDGSRV_NOTIFY_TRANSFER_LOGOUT_ACK	0x8803

//�ļ������������ѯ�û���Ϣ
#define CMDGSRV_TUSERINFO_QUERY				0x8804
#define CMDGSRV_TUSERINFO_ACK				0x8805


#define NTF_HEART_TIMEOUT			0xE001

/***
��Ⱥ����ҵ��������
***/
#define CMDCSERVICE_BASE 0x8300

//�����û�/���嶨λ��Ϣ֪ͨ
#define CMDCS_UPDATELOCATION_NOTIFY (CMDCSERVICE_BASE)

/***
�������������̬����ҵ��������
***/
//Ӧ��ACK
#define CMD_ACK                         0x8000

//������ע��
#define CMD_SERVER_REGISTER             0x8001
#define CMD_SERVER_REGISTER_ACK         0x8002

//������ע��
#define CMD_SERVER_UNREGISTER           0x8003

//������״̬����
#define CMD_SERVER_STATUS_REPORT        0x8005
//������״̬����Ӧ��
#define CMD_SERVER_STATUS_ACK			0x8006

//������״̬�б�����
#define CMD_SERVER_STATUSLIST_REQ       0x8007
#define CMD_SERVER_STATUSLIST_REQ_ACK   0x8008

//������͸��ת���ͻ�������
#define CMD_SERVER_CLARITY_TRANSMIT     0x8009



//�û���Ϊ����
#define CMD_USER_ACTION_AUDIT_REQ			0x0A05
//�û���Ϊ����Ӧ��
#define CMD_USER_ACTION_AUDIT_ACK			0x0A06


#define CMD_MOD_HANGUP_STATE_REQ			0x0A11
#define CMD_MOD_HANGUP_STATE_ACK			0x0A12

#define CDM_HEAD_FILE_UPLOAD_REQ 0x951 //�ϴ�ͷ���ļ�����
#define CDM_HEAD_FILE_UPLOAD_ACK 0x952 //�ϴ�ͷ���ļ�Ӧ��

#define CMD_MOD_PSW_REQ         0x810D      //�û��޸ĵ�¼��������
#define CMD_MOD_PSW_ACK         0x810E      //�û��޸��������˷���ȷ?
// �Ŷ����Э�飨��������͸��ͻ��˵ģ�����ϵͳ��ϢCMD_SERVERMSG_IND 0x60d֮�£�
#define CMD_WAITQUEU_CHANGE		0x810F		// �ȴ��������ӳɹ�����ٳ�Ա

// �Ŷ����Э�飨�ͻ��˷��͸�����˵�Э�飩
#define CMD_RECEPTE_CUST_REQ	0x8110		// �Ӵ��ͻ�����
#define CMD_RECEPTE_CUST_ACK	0x8111		// �Ӵ��ͻ�Ӧ��


//web�Ӵ���ѯ���Э�飨web���͸�����˵�Э�飩
#define CMD_RECEPTE_CHECK_REQ	0x9110		// �Ӵ��ͻ���ѯ����
#define CMD_RECEPTE_CHECK_ACK	0x9111		// �Ӵ��ͻ���ѯӦ��

// ���ÿͷ�����������ҵ�Э��
#define CMD_MONITOR_SET_BUYER_REQ 0x955
#define CMD_MONITOR_SET_BUYER_ACK 0x956
#define CMD_MONITOR_SET_BUYER_IND CMD_MONITOR_SET_BUYER_REQ

// ��ʼ���Э��
#define CMD_MONITOR_START_REQ 0x957
#define CMD_MONITOR_START_ACK 0x958
#define CMD_MONITOR_START_IND CMD_MONITOR_START_REQ

// �������Э��
#define CMD_MONITOR_END_REQ 0x959
#define CMD_MONITOR_END_ACK 0x960
#define CMD_MONITOR_END_IND CMD_MONITOR_END_REQ

// ����Ự
#define CMD_MONITOR_INSERT_CHAT_REQ 0x961
#define CMD_MONITOR_INSERT_CHAT_ACK 0x962
#define CMD_MONITOR_INSERT_CHAT_IND CMD_MONITOR_INSERT_CHAT_REQ

// ���ֻỰ
#define CMD_MONITOR_TRAN_CHAT_REQ 0x963
#define CMD_MONITOR_TRAN_CHAT_ACK 0x964
#define CMD_MONITOR_TRAN_CHAT_IND CMD_MONITOR_TRAN_CHAT_REQ


//��ѯ�û������ϵ��Ϣ
#define CMD_GET_LAST_MSG_REQ		0x965
#define CMD_GET_LAST_MSG_ACK		0x966

//�ͷ�����̨��ȡCSID
#define CMD_GET_USERID_MAP_CSID_REQ  0x967
#define CMD_GET_USERID_MAP_CSID_ACK  0x968

//�ͷ�����̨֪ͨweb�˳��û�
#define CMD_LOGOUT_CS_WEB	0x969

//�ͷ�����̨֪ͨweb�û����ڷ���������û�
#define CMD_SET_WEBUSERINFO_REQ  0x970

//�ͷ�����̨֪ͨ�ͷ����ڷ���������Ӵ��û�
#define CMD_SET_CS_CONSULT_REQ  0x971


//ͬ���ͷ���Ϣ����
#define CMD_UPDATE_CSINFO_REQ  0x972
#define CMD_UPDATE_CSINFO_ACK  0x973

//ͬ��ȫ���Ŷ�����
#define CMD_UPDATE_CSQUEUE_REQ  0x974
#define CMD_UPDATE_CSQUEUE_ACK  0x975

//֪ͨweb�������Ŷ��û���
#define CMD_QUEUE_LENGTH_IND 0x976

// ֪ͨ�Ӵ��ͷ��������
#define CMD_BUYER_OFFLINE_IND 0x978

// ����buf
#define CMD_SEND_BUF_IND 0x984
// ����δ�������buf
#define CMD_SEND_BUF_REQ 0x985

// ��Ϣ��������
#define CMD_GETMSGREMIND_REQ 0x986
#define CMD_GETMSGREMIND_ACK 0x987
#define CMD_SETMSGREMIND_REQ 0x988
#define CMD_SETMSGREMIND_ACK 0x989

// ��ȡ������Ϣ��������
#define CMD_GET_OFFLINE_MSGONTIFY 0x98A

// ������֮�䷢��buf
#define CMD_SERVER_SEND_BUF_IND 0x98B


#endif

