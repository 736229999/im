/************************************************************************ 
 �ļ��������������Ͷ���
 
 �����ˣ� zouyf, 2006-6-9 
 
 �޸ļ�¼�� 
************************************************************************/

#ifndef __ZPUBLICTYPE_H_
#define __ZPUBLICTYPE_H_

// beta������preview�汾��online�汾 Э���һ��
//�ͷ�����̨Э��ź��̼Ұ�Э�������
#ifdef _CS_PLAT_EX_
	#define CLIENT_VERSION 2400
#elif defined _WORK_EX_
	#define CLIENT_VERSION 2300
#else
	#define CLIENT_VERSION 2400
#endif


#ifndef HAS_COMMON_TYPEDEF
#define HAS_COMMON_TYPEDEF

typedef unsigned __int64	uint64;
typedef unsigned int		uint32;
typedef unsigned short		uint16;
typedef unsigned char		uint8;
typedef __int64				int64;
typedef int					int32;
typedef short				int16;
typedef char				int8;

#endif // !HAS_COMMON_TYPEDEF

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



#define PERM_FAMILY				1	//�̶�Ⱥ
#define TEMP_FAMILY				2	//��ʱȺ

#define FAMILY_COMMON				0	//��ͨȺ
#define FAMILY_VIP					1	//VIPȺ

//Ⱥ��Ա����Action
#define CMEMBER_ADD					0	//���
#define CMEMBER_DEL					1	//ɾ��
#define CMEMBER_SET_ADMIN			2	//��Ϊ����Ա
#define CMEMBER_CANCEL_ADMIN		3	//ȡ������Ա
#define CMEMBER_INVITE_REFUSE		4	//�����뷽�ܾ���Ⱥ
#define CMEMBER_CREATOR_REFUSE		5	//Ⱥ���ܾ������뷽��Ⱥ
#define CMEMBER_PROFILE_MOD		    6	//�޸�Ⱥ��Ƭ

#define FAMILY_REQ_JOIN				CMEMBER_ADD	//�������̶�Ⱥ
#define FAMILY_REQ_QUIT				CMEMBER_DEL	//�����˳�Ⱥ

//Ⱥ��֤��ʽ
#define FAMILY_JOIN_ANY				0	//�������
#define FAMILY_JOIN_VALIDATE		1	//��֤
#define FAMILY_JOIN_UNALLOW			2	//�ܾ�

//�������Ⱥ��Ϣ
#define FAMILY_JOIN_REQUEST			0	//�������
#define FAMILY_JOIN_AGREE			1	//ͬ�����
#define FAMILY_JOIN_REFUSE			2	//�ܾ�����

//Ⱥҳ�����Ȩ��
#define FAMILY_VIEW_MEM				1	//Ⱥ��Ա��˽�У�
#define FAMILY_VIEW_REG				2	//ע���û����빫����
#define FAMILY_VIEW_ALL				3	//���У�������

//Ⱥ��Ա����
#define FAMILY_CREATER				2	//Ⱥ������
#define FAMILY_MANAGER				1	//Ⱥ����Ա
#define FAMILY_NORMAL				0	//��ͨ��Ա



//#define MAX_NAME_LEN				24

//�û����ϻ�����Ϣ���ȶ���

#define MAX_USERNAME_LEN			120				//�û��� ����40
#define MAX_USERNAME_LEN_LAST       40              //�û���ԭ����40�����ڼ�����һ���汾��ȡ�û����ļ�
#define MAX_NICKNAME_LEN			40				//�س� ԭ����32��ԭ����30
#define MAX_HEADURL_LEN				100				//�Զ���ͷ��ͼƬURL
#define MAX_EMAIL_LEN				60				//Email
#define MAX_PASSWORD_LEN			40				//�� �ݿ�洢���ܺ�ĳ���
#define MAX_PASSWORD_LEN_LAST		40				//���ݿ�洢���ܺ��ԭ���ȣ����ڼ�����һ���汾��ȡ�û����ļ�
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
#define MAX_CLUSTER_MEMBER_PROFILE  10				//Ⱥ��Ƭ
#define MAX_GUID_LENGTH             100				//��ӪGUID����
#define MAX_JOB_LENGTH				40				//ְ��
#define MAX_DEPT_LENGTH				100				//����
#define MAX_UUID_LEN				36				//uuid
//���Ѽ�����֤��ʽ

//�����κ��˼�Ϊ����
#define FRIEND_JOIN_ANYONE	0
//��Ҫ��֤
#define FRIEND_JOIN_VERIFY	1
//�ܾ��κ��˼�Ϊ����
#define	FRIEND_JOIN_REJECT	2


//���ݰ�������,�����ֽ�
#define MAX_RECV_BUFFERS			1440
#define MAX_SEND_BUFFERS			1440

// �ļ��������ݰ�buf��С
#define MAX_FILE_TRAN_RECV_BUF MAX_RECV_BUFFERS*40
#define MAX_FILE_TRAN_SEND_BUF MAX_SEND_BUFFERS*40

#define MAX_NETWORK_PACKET		(MAX_RECV_BUFFERS + sizeof(XT_HEAD))

// �ļ�����ʱ����С
#define MAX_FILE_TRAN_NETWORK_PACKET		(MAX_FILE_TRAN_RECV_BUF + sizeof(XT_HEAD))

#define MAX_ERROR_BUF				128				//������������

//����
#define FAMILE_MAX_MSGNUM			100         //Ⱥ������Ϣ��������
#define MAX_PERMFAMILY_NUM				100			//������̶�Ⱥ��,�����ģ�������������Ⱥ
#ifdef _BUSINESS_VESION
#define MAX_PERMFAMILYMEM_NUM			1000		//���Ұ���ͨ�̶�Ⱥ��Ա��
#else
#define MAX_PERMFAMILYMEM_NUM			500			//��ͨ�̶�Ⱥ��Ա��
#endif
#define MAX_VIPFAMILYMEM_NUM			300			//VIPȺ��Ա������
#define MAX_PERMFAMILYMAN_NUM			3			//��ͨ�̶�Ⱥ����Ա������
#define MAX_VIPFAMILYMAN_NUM			4			//VIPȺ����Ա��

#define MAX_TEMPFAMILY_NUM				10			//���������ʱȺ��
#define MAX_TEMPFAMILYMEM_NUM			30			//��ʱȺ��Ա��

#define MAX_FAMILEMSG_NUM				100         //Ⱥ������Ϣ��������
#define MAX_OFFLINEMSG_NUM				20			//����������Ϣ��������
#define MAX_GROUP_NUM					5			//�û���
#define MAX_FRIENDS_NUM					300         //���������� 
#define MAX_CREATEFAMILY_NUM			100			//�û���ഴ���Ĺ̶�Ⱥ��

//Ⱥ��������
#define MAX_FAMILYNAME_LEN				100			//Ⱥ������
#define MAX_FAMILYDOMAIN_LEN			20			//Ⱥ��������
#define MAX_FAMILYDESC_LEN				500			//Ⱥ��������
#define MAX_FAMILYFACEURL_LEN			10			//Ⱥ��Ƭ����

#define OFFLINE_MAX_MSGNUM				20			//����������Ϣ��������
#define MANAGER_ID						260953		//����ԱID������������	

#define DAY_SEND_MONEY					1			//ÿ�쳬����Сʱ�͵Ľ����

#define MIN_PSW_LEN		                6           //��������С����
#define MAX_PSW_LEN		                18          //��������󳤶�

#pragma pack(push,1)

typedef unsigned __int64	uint64;
typedef unsigned int		uint32;
typedef unsigned short		uint16;
typedef unsigned char		uint8;
typedef __int64				int64;
typedef int					int32;
typedef short				int16;
typedef char				int8;

//Ӧ�ò����ݰ�ͷ
typedef struct XT_HEAD
{
	uint8  flag;  //1  MINI139����ʶ 0xF0����������������0xF1������Ӧ��  0xF2�ͻ������� 0xF3�ͻ���Ӧ�� 0xFD���Ұ��־����ֻ�ڻ�ȡ�����б��Լ�����Ϣʱ�õ�
	enum  { FLAG_SRVACTION = 0xF0, FLAG_SRVACK = 0xF1, FLAG_CNTREQUEST = 0xF2, FLAG_CNTACK = 0xF3, FLAG_SELLCNTREQ = 0xFD };


	uint16 from;  //2  ��Դ,�Կͻ�����˵�ǰ汾��
	enum { FROM_SERVER = 0x0100, FROM_CLIENT = CLIENT_VERSION, FROM_TRANSFER = 0x0200 };

	uint16 cmd;   //3 �����

	uint32 seq;   //4 �������к�

	uint32 sid;   //5 ԴID (��ϸ˵����Э���ĵ�)
	
	enum 
	{ 
		SID_SERVER = 0, //ͨ�õķ�����ID,������ʹ��
	 };
	
	uint8  dtype;// 6 Ŀ������ (0x01 �û���0x02 Ⱥ��0x03 ��ת�Ự��Ա)
	enum { DEST_TYPE_SERVER = 0x00, DEST_TYPE_USER = 0x01, DEST_TYPE_FAMILY = 0x02 , DEST_TYPE_TRANSMIT = 0x03 };

	uint32 did;   //7 Ŀ��ID(��ϸ˵����Э���ĵ�)

#ifdef _IM_USE_MULTIDEVICE_
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
	char	data[MAX_FILE_TRAN_NETWORK_PACKET];

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
	uint8	src_type;	// P2P_ENDPOINT_TYPE  
	uint16  data_len;
	uint32  vlink;		// virtual link
	char	data[MAX_FILE_TRAN_NETWORK_PACKET];

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

typedef struct _SESSION_ID
{
	uint32		uid;
	uint32		fid;

	_SESSION_ID()
	{
		uid = -1;
		fid = -1;
	}

	bool	IsValid() const
	{
		return uid != -1 && fid != -1;
	}

	bool operator < (_SESSION_ID  o) const
	{
		return (uid < o.uid) || ((uid == o.uid) && (fid < o.fid));
	}

	bool operator == (_SESSION_ID  o) const
	{
		return (uid == o.uid) && (fid == o.fid);
	}

} SESSION_ID;

class CIStream
{
public:
	virtual const char * Read( /*IN-OUT*/int & nLen) = 0;
};


#pragma pack(pop)

//��������������ļ����˿�

//ȫ�������û�����
#define PORT_ONLINE_USER_MANAGER	10000
//ȫ������Ⱥ����
#define PORT_ONLINE_CLUSTER_MANAGER 10001
//IMҵ�������
#define PORT_IM_BUSINESS			10002
//�ļ�ת��������
#define PORT_TRANSPORT_SERVER		10003
//���������
#define PORT_CONN_SERVER            10004

//Ӧ���������ȫ��ͨѶ�˿�
#define PORT_APPNET2GLOBAL          11000  


//��������ID����
#define SERVERID_CONNECT		1 //���������
#define SERVERID_ONLINE_USER	2 //ȫ���û����߹���
#define SERVERID_ONLINE_CLUSTER 3 //ȫ��Ⱥ���߹���
#define SERVERID_TRANSFER		-4 //��ת������





//�������Դ���
#define SEND_RETRY_TIME				4		
#define PIC_RETRY_TIME				10	







//����139�û���
#define MAX_139_ACCOUNT_NUM  10000000

//����ֵ
#define RESULT_SUCCESS 0
#define RESULT_FAILED  1

#define MAX_PROTOLPACKET_LEN ( MAX_SEND_BUFFERS - sizeof(XT_HEAD)) 




#endif
