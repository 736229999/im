/************************************************************************ 
 �ļ��������������Ͷ���
 
 �����ˣ� zouyf, 2006-6-9 
 
 �޸ļ�¼�� 
************************************************************************/

#ifndef __ZPUBLICTYPE_H_
#define __ZPUBLICTYPE_H_

#include <string.h>

typedef unsigned char    uint8;
typedef unsigned short   uint16;
typedef unsigned int     uint32;
typedef unsigned long long uint64;

#define MAX_MSG_LEN					1400	  //������Ϣ��󳤶�

#define XTREAM_ONLINE				0x00  //����
#define XTREAM_LEAVE				0x01  //�뿪
#define XTREAM_BUSY					0x02  //æµ
#define XTREAM_HANGUP				0x03  //����
#define XTREAM_HIDE					0x12  //����
#define XTREAM_OFFLINE				0x13  //����
#define XTREAM_EXIT					XTREAM_OFFLINE


//������ID����
#define GROUPID_MYFRIEND			0	//�ҵĺ���
#define GROUPID_STRANGER			1	//İ����
#define GROUPID_BLACKLIST			2	//������
//������ID���Զ�����



#define PERM_FAMILY				1	//�̶�����
#define TEMP_FAMILY				2	//��ʱ����

#define FAMILY_COMMON				0	//��ͨ����
#define FAMILY_VIP					1	//VIP����

//�����Ա����Action
#define CMEMBER_ADD					0	//���
#define CMEMBER_DEL					1	//ɾ��
#define CMEMBER_SET_ADMIN			2	//��Ϊ����Ա
#define CMEMBER_CANCEL_ADMIN		3	//ȡ������Ա
//#define CMEMBER_PROFILE_MOD		    4	//����Ⱥ��Ƭ
#define CMEMBER_REFUSE_IN_1			4	//�������˾ܾ���Ⱥ
#define CMEMBER_REFUSE_IN_2			5	//Ⱥ���ܾ���Ⱥ

#define FAMILY_REQ_JOIN				CMEMBER_ADD	//�������̶�����
#define FAMILY_REQ_QUIT				CMEMBER_DEL	//�����˳�����

//������֤��ʽ
#define FAMILY_JOIN_ANY				0	//�������
#define FAMILY_JOIN_VALIDATE		1	//��֤
#define FAMILY_JOIN_UNALLOW			2	//�ܾ�

//������������Ϣ
#define FAMILY_JOIN_REQUEST			0	//�������
#define FAMILY_JOIN_AGREE			1	//ͬ�����
#define FAMILY_JOIN_REFUSE			2	//�ܾ�����

//����ҳ�����Ȩ��
#define FAMILY_VIEW_MEM				1	//�����Ա��˽�У�
#define FAMILY_VIEW_REG				2	//ע���û����빫����
#define FAMILY_VIEW_ALL				3	//���У�������

//�����Ա����
#define FAMILY_CREATER				2	//���崴����
#define FAMILY_MANAGER				1	//�������Ա
#define FAMILY_NORMAL				0	//��ͨ��Ա



//#define MAX_NAME_LEN				24

//�û����ϻ�����Ϣ���ȶ���

#define MAX_USERNAME_LEN			120				//�û��� ԭ����20
#define MAX_NICKNAME_LEN			40				//�س� ԭ����32
#define MAX_HEADURL_LEN				100				//�Զ���ͷ��ͼƬURL
#define MAX_EMAIL_LEN				60				//Email
#define MAX_PASSWORD_LEN			60				//���ݿ�洢���ܺ�ĳ���
#define MAX_BIRTHDAY_LEN			8				//����
#define MAX_ADDRESS_LEN				80				//��ַ
#define MAX_PHONE_LEN				20				//ͨѶ����
#define MAX_DESCRIPTION_LEN			100				//����˵��
#define MAX_VERSION_URL             100				//�汾����URL
#define MAX_UPDATECONFIG_URL		100				//�汾�����ļ�URL
#define MAX_UPDATE_INFO             512				//������Ϣ
#define MAX_WEB_LOCATION            100				//web location
#define MAX_GROUPNAME_LEN			20				//���ѷ������������
#define MAX_SESSIONKEY_LEN			16				//Session key
#define MAX_COMMENTNAME_LEN			16				//���ѱ�ע
#define MAX_USERSIGN_LEN			100				//����ǩ��
#define MAX_AVATOR_LEN				100				//�����ַ
#define MAX_GUID_LEN				100				//GUID
#define MAX_MSG_LENGTH				1315			//���������Ϣ����
#define MAX_WORKGROUP_LEN			100				//��������Ϣ

//���Ѽ�����֤��ʽ

//�����κ��˼�Ϊ����
#define FRIEND_JOIN_ANYONE	0
//��Ҫ��֤
#define FRIEND_JOIN_VERIFY	1
//�ܾ��κ��˼�Ϊ����
#define	FRIEND_JOIN_REJECT	2

#ifndef IM_TRANSFER
//���ݰ�������,�����ֽ�
#define MAX_RECV_BUFFERS			1440
#define MAX_SEND_BUFFERS			1440
#else
#define MAX_RECV_BUFFERS			1440*50
#define MAX_SEND_BUFFERS			1440*50
#endif

#define MAX_NETWORK_PACKET		(MAX_RECV_BUFFERS + sizeof(XT_HEAD))
#define MAX_REVBUF_LENGTH		MAX_NETWORK_PACKET + sizeof(uint32)
#define MAX_JSON_BUFFER			14400

#define MAX_ERROR_BUF				128				//������������

//����
#define FAMILE_MAX_MSGNUM			100         //����������Ϣ��������
#define MAX_PERMFAMILY_NUM				100			//������̶�������,�����ģ����������������
#define MAX_PERMFAMILYMEM_NUM			100			//��ͨ�̶������Ա��
#define MAX_VIPFAMILYMEM_NUM			500		//VIP�����Ա������
#define MAX_PERMFAMILYMAN_NUM			10			//��ͨ�̶��������Ա������
#define MAX_VIPFAMILYMAN_NUM			10			//VIP�������Ա��

#define MAX_TEMPFAMILY_NUM				10			//���������ʱȺ��
#define MAX_TEMPFAMILYMEM_NUM			30			//��ʱȺ��Ա��

#define MAX_FAMILEMSG_NUM				100         //Ⱥ������Ϣ��������
#define MAX_OFFLINEMSG_NUM				20			//����������Ϣ��������
#define MAX_GROUP_NUM					50			//�û���
#define MAX_FRIENDS_NUM					500         //���������� 
#define MAX_CREATEFAMILY_NUM			2			//�û���ഴ���Ĺ̶�������

//�����������
#define MAX_FAMILYNAME_LEN				100			//����������
#define MAX_FAMILYDOMAIN_LEN			20			//������������
#define MAX_FAMILYDESC_LEN				500			//������������
#define MAX_FAMILYFACEURL_LEN			100			//���������ַ����

#define OFFLINE_MAX_MSGNUM				20			//����������Ϣ��������
#define MANAGER_ID						260953		//����ԱID������������	

#define DAY_SEND_MONEY					1			//ÿ�쳬����Сʱ�͵Ľ����


//Ⱥ��Ƭ
#define CPIC_PACKET_LEN				1400
#define MAX_CLUSTER_MEMBER_PROFILE  10				//Ⱥ��Ƭ
#define MAX_MEM_CNT 				30				//Ⱥ��Ƭ�������


#pragma pack(push,1)

//Ӧ�ò����ݰ�ͷ
typedef struct XT_HEAD
{

	/*MINI139����ʶ 
	0xF0����������������
	0xF1������Ӧ��  
	0xF2�ͻ������� 
	0xF3�ͻ���Ӧ��
	0xF4Web������
	0xF5Web��Ӧ��
	0xF6Web�˷������������� 
	0xFD���ҿͻ�������
	*/
	uint8  flag;  
	enum  { 
		FLAG_SRVACTION = 0xF0,
		FLAG_SRVACK = 0xF1, 
		FLAG_CNTREQUEST = 0xF2, 
		FLAG_CNTACK = 0xF3 , 
		FLAG_MOBREQ = 0xF4, 
		FLAG_MOBSRVACK = 0xF5, 
		FLAG_MOBSRVACTION = 0xF6, 
		FLAG_SELLCNTREQ = 0xFD
	};


	uint16 from;  //2  ��Դ,�Կͻ�����˵�ǰ汾��
	enum { 
		FROM_SERVER = 0x0100, 
		FROM_CLIENT = 0x203, 
		FROM_TRANSFER = 0x0200 
	};

	uint16 cmd;   //3 �����

	uint32 seq;   //4 �������к�

	uint32 sid;   //5 ԴID (��ϸ˵����Э���ĵ�)
	
	enum 
	{ 
		SID_SERVER = 0, //ͨ�õķ�����ID,������ʹ��
	 };
	
	uint8  dtype;// 6 Ŀ������ (0x01 �û���0x02 ���壬0x03 ��ת�Ự��Ա)
	enum { DEST_TYPE_SERVER = 0x00, DEST_TYPE_USER = 0x01, DEST_TYPE_FAMILY = 0x02 , DEST_TYPE_TRANSMIT = 0x03 };

	uint32 did;   //7 Ŀ��ID(��ϸ˵����Э���ĵ�)
#ifdef IM_USE_MULTIDEVICE
	uint32 deviceId;
#endif
	uint16 len;  //8 ���峤��
	
	XT_HEAD()
	{
		memset(this,0,sizeof(XT_HEAD));
	}



} XT_HEAD, * LPXT_HEAD;

struct SendDataStruct
{
	uint32	dest_ip;//Destination peer IP
	uint16	port;   //host order
	int		retry_times;	//�����ط�����������3�Ͷ���
	uint32	tick_count;     //�ط���ʱ��
	uint16	time_interval;	//�ط�   ��� 
	uint16	data_len;//total data bytes will be sent,include first lead byte
	uint32  vlink;  //virtual link
	char	data[MAX_NETWORK_PACKET];

	SendDataStruct()
	{
		memset(this, 0, sizeof(SendDataStruct));
		retry_times = -1;
	}
};

struct RecvDataStruct
{
	uint32	src_ip;
	uint16  port;
	uint8	src_type;
	uint16  data_len;
	uint32  vlink;  //virtual link
	char	data[MAX_NETWORK_PACKET];

	RecvDataStruct()
	{
		memset(this, 0, sizeof(RecvDataStruct));
	}
};

struct tagClientReply
{
	uint32 sid;
	uint32 seq;
	uint32 vlink;
	tagClientReply()
	{
		sid = 0;
		seq = 0;
		vlink = 0;
	}
};

struct tagServerReply
{
	tagClientReply  requestId;  // Ψһȷ�����û���������Ϣ
	SendDataStruct  replyMsg; // �Ը�������Ϣ�Ļظ�
};

#pragma pack(pop)

//��������������ļ����˿�

//SSDB
#define PORT_SSDB					8888
//ȫ�������û�����
#define PORT_ONLINE_USER_MANAGER	10000
//ȫ�����߼������
#define PORT_ONLINE_CLUSTER_MANAGER 10001
//IMҵ�������
#define PORT_IM_BUSINESS			10002
//�ļ�ת��������
#define PORT_TRANSPORT_SERVER		10003
//���������
#define PORT_CONN_SERVER            10004
//վ����Ϣ֪ͨ������
#define PORT_NOTIFY_BUSINESS		10005
//TCP
#define PORT_TCP_BUSINESS		    10006

//memcached
#define PORT_MEMCACHED_ONLINE	    10007

//Ӧ���������ȫ��ͨѶ�˿�
#define PORT_APPNET2GLOBAL          11000  
#define PORT_NOTIFY2GLOBAL          11001

//ҵ�������֮��ͨѶ�˿�
#define PORT_IM_SVRP2P              11002
//WEB NOTIFYͨѶ�ӿ�
#define PORT_WEB_NOTIFY             11003

//�ļ������������ȫ�ֽӿ�
#define PORT_TRANS2GLOBAL           11004

//��������ID����
#define SERVERID_CONNECT		1 //���������
#define SERVERID_ONLINE_USER	2 //ȫ���û����߹���
#define SERVERID_ONLINE_CLUSTER 3 //ȫ�ּ������߹���
#define SERVERID_TRANSFER		-4 //��ת������





//�������Դ���
#define SEND_RETRY_TIME				4		
#define PIC_RETRY_TIME				10	







//����139�û���
#define MAX_139_ACCOUNT_NUM  10000000
#define MAX_SubMerchantID    10000000000

//����ֵ
#define RESULT_SUCCESS 0
#define RESULT_FAILED  1

/*define MEMCACHED_ERROR_TYPE*/

#define MEMCACHED_GET_INFO_SUCCESS 0

#define MEMCACHED_SERVER_DOWN 1

#define MEMCACHED_NO_INFO 2

#define MEMCACHED_JASON_ERROR 3

#define CS_SUCCESS   0
#define NO_USABLE_CS 1
#define NO_CS_ONLINE 2

#define MEMCACHED_USER_LOCK 1
#define MEMCACHED_CS_LOCK 2
#define MEMCACHED_QUEUE_LOCK 3

/*end define MEMCACHED_ERROR_TYPE*/

#define CLIENT_FORCE_RELOGIN 0xAA

#define MAX_PROTOLPACKET_LEN ( MAX_SEND_BUFFERS - sizeof(XT_HEAD)) 

//����û�����
#define TYPE_CLIENT 0
#define TYPE_WEB 1
#define TYPE_NULL 2

#define MAX_MSG_BUFFER_NUM 	512
#define MAX_MSG_PTHREAD_NUM 1
#define FILE_PACKET_LEN 	1240

//IDС��500000Ϊ����id,����5000000Ϊ���
#define Id_Boundary 5000000
#define cs_Boundary 2000000000
#define work_Boundary 2050000000	//������

#define seller_Timeout 210

#define buyer_First_Timeout 86400
#define buyer_Timeout 700
#define buyer_Quit_Timeout 60
#define LOGOUT_TIMEOUT 10
//��ȡ������Ϣ����
#define GET_FRIEND_NUMBER 150

//���Ҷ���
#define ACTION_ERROR     0
#define ACTION_LOGIN     1
#define ACTION_LOGOUT    2
#define ACTION_HANGUP    3
#define ACTION_RECEPTION 4
#define ACTION_LEAVE 	 5
#define ACTION_NOT_LEAVE 6


#define MYGROUP_ID 250
 
#define MAX_QueryUserStatus_NUM  50

#endif

