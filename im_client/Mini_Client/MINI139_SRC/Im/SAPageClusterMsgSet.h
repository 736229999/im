#pragma once


#include "sabasepage.h"

// CSAPageClusterMsgSet �Ի���

class CSAPageClusterMsgSet : public CSABasePage
{
	DECLARE_DYNAMIC(CSAPageClusterMsgSet)

public:

	enum RECVMSG_TYPE{
		RECV_AND_SHOW,
		RECV_NO_SHOW,
		//REFUSE_RECV,
	};
	
	CSAPageClusterMsgSet(CWnd* pParent = NULL);   // ��׼���캯��
	
	virtual ~CSAPageClusterMsgSet();

	enum { IDD = IDD_SA_CLUSTERMSGSET };

	void		LoadConfig();

	void		SaveConfig();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:

	RECVMSG_TYPE	m_typeRecvMsg;


};
