#include "StdAfx.h"
#include ".\mmtalker.h"

//#define _LOCAL_TEST

#ifdef _DEBUG
//#define _WRITE_FILE
#ifdef _WRITE_FILE
#include <fstream>
#endif
using namespace std;
#endif


#define TALK_WND_SIZE	10
#define	TALK_SEND_BUF	(10 * 1024)


#ifdef _MY_UDP
DWORD UserNetCallback(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData)
{
	return 0;
};
#endif

struct MMTALK_PARAM
{
	CMMTalker *	pThis;
	uint32		fid;
	uint32		ip;			// 网络字节
	uint16		port;		// 网络字节
	uint32		tranIp;		// 中转服务器（网络字节）
	uint16		tranPort;	// 中转服务器port （网络字节）
};


DWORD WINAPI CMMTalker::ConnectProc(void * param)
{
	MMTALK_PARAM * pParam = (MMTALK_PARAM *)param;
	pParam->pThis->OnConnect(pParam->fid,pParam->ip,pParam->port,pParam->tranIp,pParam->tranPort);
	return 0;
}

DWORD WINAPI CMMTalker::AcceptProc(void * param)
{
	MMTALK_PARAM * pParam = (MMTALK_PARAM *)param;
	pParam->pThis->OnAccept(pParam->fid,pParam->ip,pParam->port,pParam->tranIp,pParam->tranPort);
	return 0;
}

void CMMTalker::OnConnect(uint32 fid,uint32 ip,uint16 port,uint32 tranIp,uint16 tranPort)
{
	if(m_pDataTran == NULL)
	{
		OnConnectTalkOk(false);
		return;
	}
	int nRt;
	if(m_netsoc)
	{
		if(m_pDataTran)
		{
			m_pDataTran->CloseSocket(m_netsoc);
			m_netsoc = 0;
		}
	}
	m_netsoc = m_pDataTran->Socket(TALK_WND_SIZE,TALK_SEND_BUF);
	// 连接到另一个用户
	nRt = m_pDataTran->Connect(m_netsoc,m_nMyId,fid,ip,port,tranIp,tranPort);
	if(nRt != 0)
	{
		try
		{
			m_pDataTran->CloseSocket(m_netsoc);
		}
		catch (...)
		{
			return ;
		}
		
		m_netsoc = 0;
		OnConnectTalkOk(false);
	}
	else
	{
		m_hThreadRecv = ::CreateThread(0,0,RecvData,this,0,0);
		OnConnectTalkOk(true);
	}
}


void CMMTalker::OnAccept(uint32 fid,uint32 ip,uint16 port,uint32 tranIp,uint16 tranPort)
{
	if(m_pDataTran == NULL)
	{
		OnAcceptTalkOk(false);
		return;
	}
	m_netsoc = m_pDataTran->Accept(m_nMyId,fid,ip,port,tranIp,tranPort);

	if(m_netsoc != -1)
	{
		m_hThreadRecv = ::CreateThread(0,0,RecvData,this,0,0);
		OnAcceptTalkOk(true);
	}
	else
	{
		m_netsoc = 0;
		try
		{
			OnAcceptTalkOk(false);
		}
		catch (...)
		{
			return ;
		}
		
	}
}

CMMTalker::CMMTalker(void)
{
	m_pDataSend = new BYTE[1024 * 8];
	m_pDataRecv = new BYTE[1024 * 8];
	m_nDataRecv = 1024 * 8;
	m_nDataSend = 1024 * 8;
	m_hThreadRecv = NULL;
	m_hThreadAccpet = NULL;
	m_hThreadConnect = NULL;
#ifndef _MY_UDP
	m_pDataTran = NULL;
	m_netsoc = NULL;
#endif
#ifdef _TCP_TEST
	m_socketConn = 0;
	m_socketListen = 0;
	m_socketAccept = 0;
#endif

#ifdef _TRACE_BITRATE
	m_nData = 0;
	m_nLastTime = 0;
	m_nTotData = 0;
	m_nTotTime = 0;
#endif

}

CMMTalker::~CMMTalker(void)
{
	Stop();
	if(m_pDataSend)
	{
		delete [] m_pDataSend;
		m_pDataSend = NULL;
	}
	if(m_pDataRecv)
	{
		delete [] m_pDataRecv;
		m_pDataRecv = NULL;
	}
	if(m_netsoc)
	{
		if(m_pDataTran)
		{
			m_pDataTran->CloseSocket(m_netsoc);
		}
	}

}

int CMMTalker::CmpAudioFrame(MMAudioFrame * pFrame)
{
	if( pFrame == NULL)
		return 0;
	int nLen = pFrame->dwLen;
	BYTE * pCmpData = m_acodec.CmpWaveData((BYTE*)pFrame->data,nLen);
	if(pCmpData)
	{	
		memcpy(pFrame->data,pCmpData,nLen);
		pFrame->dwLen = nLen;
		nLen += sizeof(MMAudioFrame);
		return nLen;
	}
	else
	{
		return 0;
	}
}

int CMMTalker::CmpVideoFrame(MMVideoFrame * pFrame)
{
	static int nSeq = 0;
	if(pFrame == NULL)
		return false;
	int nLen = pFrame->dwLen;
	BYTE * pCmpData = m_vcodec.CmpFrame((BYTE*)pFrame->data,nLen);
	if(pCmpData)
	{
		memcpy(pFrame->data,pCmpData,nLen);
		pFrame->dwLen = nLen;
		nLen += sizeof(MMVideoFrame);
		pFrame->wRes = nSeq++;
		return nLen;
	}
	else
	{
		return 0;
	}
}

int CMMTalker::EncodeData(const BYTE * pData,int nLen)
{
	MMFrame * pFrame = (MMFrame *)pData;
	switch(pFrame->dwFlag)
	{
	case MMAudioFrame::FCC_AUDIO:
		return CmpAudioFrame((MMAudioFrame*)pData);
	case MMVideoFrame::FCC_VIDEO:
		return CmpVideoFrame((MMVideoFrame*)pData);
	case MMControlFrame::FCC_CONTROL:
		break;
	default:
		break;
	}
	return nLen;
}



void CMMTalker::SendData(XDataPoolEx * pPool,DWORD dwUser)
{
	CMMTalker * pThis = (CMMTalker *)dwUser;
	pThis->OnSendData(pPool);
}

// 将采集的数据发送到接收方,可以阻塞

void CMMTalker::OnSendData(XDataPoolEx * pPool)
{
	int nLen = pPool->GetFrontLen();
	if(nLen > m_nDataSend)
	{
		delete [] m_pDataSend;
		m_pDataSend = new BYTE[nLen];
		m_nDataSend = nLen;
	}
	int nRet = pPool->Pop((char*)m_pDataSend,nLen);
	if(nRet <= 0)
	{
		return;
	}
	assert(nLen == nRet);
	nLen = nRet;
	// 对数据进行编码
	nLen = EncodeData(m_pDataSend,nLen);

	if(nLen <= 0)
	{
		// 编码失败
		TRACE("编码失败 nLen = %d\n",nLen);
		return;
	}
	
#ifdef _WRITE_FILE

	static bool bFile = false;
	static ofstream ofs;
	if(bFile == false)
	{
		ofs.open("c:\\video.dat",ios::binary);
		bFile = true;
	}
	ofs.write((char*)m_pDataSend,nLen);
#endif

#ifdef _TRACE_BITRATE
	//static int nC = 0;
	m_nData += nLen;
	m_nTotData += nLen;
	if(m_nLastTime == 0)
	{
		m_nLastTime = GetTickCount();
	}
	int nT2 = GetTickCount();
	int nTime = nT2 - m_nLastTime;
	if(nTime > 1000)
	{
		m_nLastTime = nT2;
		m_nTotTime += nTime;
		TRACE("%4.2fKB/秒,平均%4.2fKB/秒\n",
			(double)m_nData / (1.024 * nTime),
			(double)m_nTotData / (1.024 * m_nTotTime)
			);
		m_nData = 0;
	}
#endif 

	int nSend;
	if(nLen >= 0)
	{
	/*	ParseData(m_pDataSend,nLen);
		return;*/
#ifdef _LOCAL_TEST
		ParseData(m_pDataSend,nLen);
		return;
#endif
		// 发送数据
		// ...........
		//TRACE("send data len=%d\n",nLen);
#ifndef _TCP_TEST
#ifndef _MY_UDP
		if(m_netsoc && m_pDataTran && m_pDataSend)
		{
			char * pData = (char *)m_pDataSend;
			while(nLen > 0)
			{
				nSend = m_pDataTran->Send(m_netsoc,pData,nLen);
				if(nSend <= 0)
				{
					TRACE("err:视频聊天数据发送失败!\n");
					m_netsoc = 0;
					break;
				}
				//TRACE("send = %d\n",nSend);
				nLen -= nSend;
				pData += nSend;
			}
		}
#else
		if(m_session.IsValid())
		{
			while(nLen > 0)
			{
				nSend = m_netTran.Send(m_session,(char*)m_pDataSend,nLen);
				if(nSend <= 0)
					break;
				nLen -= nSend;
			}
			
		}
#endif

#else
		while(nLen > 0)
		{
			if(m_socketConn)
				nSend = send(m_socketConn,(char*)m_pDataSend,nLen,0);
			else if(m_socketAccept)
				nSend = send(m_socketAccept,(char*)m_pDataSend,nLen,0);
			else
				break;
			if(nSend <= 0)
			{
				TRACE("网络传输错误\n");
				break;
			}
			nLen -= nSend;
		}
#endif


	}
}

BOOL CMMTalker::Init(VideoPlayerDrawFnc pFnc,DWORD dwUser,uint32 nMyId,CUdpNetTrans * pNetTran,int nBufCount)
{
#ifndef _MY_UDP
	m_nMyId = nMyId;
	m_pDataTran = pNetTran;
	//m_pDataTran = new CDataTrans(nMyId,pNetTran);
#else
	m_netTran.SetUserCallback(0,UserNetCallback);
	m_netTran.StartListen("0.0.0.0",12345);
#endif
	m_dataMaker.Init(&m_acodec,&m_vcodec,nBufCount,SendData,(DWORD)this);
	m_vplayer.SetCallBackOnDraw(pFnc,dwUser);
#ifdef _TCP_TEST
	int	iResult; 
	WSADATA ws; 
	iResult = WSAStartup(0x0202,&ws); 
	int IP = inet_addr("0.0.0.0");
	int Port = htons(12345);
	int nAddrLen = 0;

	SOCKADDR_IN addrBnd;
	addrBnd.sin_family = AF_INET;
	addrBnd.sin_addr.s_addr = IP;
	addrBnd.sin_port = Port;
	nAddrLen = sizeof(addrBnd);


	m_socketListen = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);

	int nRst = bind(m_socketListen,(sockaddr *)&addrBnd, sizeof(addrBnd));
	nRst = listen(m_socketListen,2);
	if(m_socketListen == 0)
	{
		TRACE("建立socket失败\n");
	}
#endif
	return true;
}


BOOL CMMTalker::InitAudio(HWND hWnd,WAVEFORMATEX * pWfxIn/* = NULL */)
{
	if(m_dataMaker.IsInit() == false)
	{
		m_dataMaker.Init(&m_acodec,&m_vcodec,32,SendData,(DWORD)this);
	}
	/*
		8000的采样频率下
		WAVE_FORMAT_DSPGROUP_TRUESPEECH 0x0022 = 34 压缩比为8.000 解压比为7.500 对比1.066667	
		WAVE_FORMAT_DVI_ADPCM 0x11 = 17	压缩比为2 解压比为1.973 对比1.013685
		WAVE_FORMAT_ADPCM 0x2 = 2	压缩比为1.932 解压比为1.921 对比1.005726
		WAVE_FORMAT_GSM610 0x31 = 49 ,	压缩比为3.938  解压比为4.923 对比0.79992
		WAVE_FORMAT_MPEGLAYER3 0x55 = 85 压缩比为8.463 解压比为0.000 对比?
	*/

	int nHZ = 8000;
	BOOL bRt;
	WAVEFORMATEX * pIn = pWfxIn;
	// 默认使用最差音质
	WAVEFORMATEX wfx;
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 1;
	wfx.nSamplesPerSec = nHZ;
	wfx.nAvgBytesPerSec = nHZ;
	wfx.nBlockAlign = 1;
	wfx.wBitsPerSample = 8;
	wfx.cbSize = 0;

	WAVEFORMATEX wfxCmp;
	wfxCmp.wFormatTag = WAVE_FORMAT_DSPGROUP_TRUESPEECH;//WAVE_FORMAT_ADPCM;//WAVE_FORMAT_DSPGROUP_TRUESPEECH;//WAVE_FORMAT_MPEGLAYER3;//WAVE_FORMAT_DSPGROUP_TRUESPEECH;
	wfxCmp.nChannels = 1;
	wfxCmp.nSamplesPerSec = nHZ;
	wfxCmp.nAvgBytesPerSec = 0;
	wfxCmp.nBlockAlign = 1;
	wfxCmp.wBitsPerSample = 8;
	wfxCmp.cbSize = 0;


	if(pWfxIn == NULL)
	{
		pIn = &wfx;
	}
	
	bRt = m_acodec.Init(pIn,&wfxCmp);
	if(bRt == false)
		return false;
//#ifdef _DEBUG
//	bRt = m_aplayer.Init(hWnd,m_acodec.GetSrcWaveFormat(),&m_acodec);
//#endif
	bRt = m_aplayer.Init(hWnd,m_acodec.GetPcmWaveFormat(),&m_acodec);
	if(bRt == false)
		return false;

	bRt = m_acap.Init(pIn,CMMDataMaker::OnAudioData,(DWORD)&m_dataMaker);
	if(bRt == false)
		return false;

	return true;
}

BOOL CMMTalker::InitVideo(const CRect & rc,HWND hParent,double dbQuality,int nCapIdx)
{
	if(m_dataMaker.IsInit() == false)
	{
		m_dataMaker.Init(&m_acodec,&m_vcodec,32,SendData,(DWORD)this);
	}
	BOOL bRt ;
	bRt = m_vcodec.Init(m_vcap.GetBmpInfo(),dbQuality);
	if(bRt == false)
		return false;
	bRt = m_vplayer.Init(&m_vcodec);
	if(bRt == false)
		return false;
	bRt = m_vcap.InitCap(rc,hParent,nCapIdx);
	m_vcap.SetCallBackOnData(CMMDataMaker::OnVideoData,(DWORD)&m_dataMaker);
	if(bRt == false)
		return false;
	
	return true;
}

bool CMMTalker::OnAudioFrame(const BYTE * pData,int nLen)
{
	m_aplayer.PlayAudio((MMAudioFrame*)pData);
	return true;
}

bool CMMTalker::OnVideFrame(const BYTE * pData,int nLen)
{
	m_vplayer.AddVideoFrame((MMVideoFrame*)pData,nLen);
	return true;
}

bool CMMTalker::OnControlFrame(const BYTE * pData,int nLen)
{
	MMControlFrame * pCtrl = (MMControlFrame *)pData;
	switch(pCtrl->dwCmd)
	{
	case CMD_NOTIFY_FRAME_PER_SEC:
		{
			MM_NOTIFY_FRAME_PER_SEC * pCmd = (MM_NOTIFY_FRAME_PER_SEC*)pCtrl->data;
			m_vplayer.SetFramePerSec(pCmd->nFramePerSec);
		}
		break;
	default:
	    break;
	};
	return true;
}



void CMMTalker::StartTalk(uint32 nUserID,uint32 nIP,uint16 nPort,uint32 nTranIP,uint16 nTranPort)
{

#ifdef _LOCAL_TEST
	return ;
#endif


#ifndef _MY_UDP

#ifndef _TCP_TEST
	static MMTALK_PARAM param;
	param.pThis = this;
	param.fid = nUserID;
	param.ip = nIP;
	param.port = nPort;
	param.tranIp = nTranIP;
	param.tranPort = nTranPort;
	
	::CreateThread(0,0,ConnectProc,&param,0,0);
	
	/*m_netsoc = m_pDataTran->socket(64);
	nRt = m_pDataTran->connect(m_netsoc,nUserID,nIP,nPort,nTranIP,nTranPort);*/

#else
	int nRt;
	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_addr.s_addr = nIP;
	addrSrv.sin_port = htons(12345);
	m_socketConn = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
	nRt = connect(m_socketConn,(sockaddr*)&addrSrv,sizeof(addrSrv));
	if(nRt != 0)
		m_socketConn = 0;
	if(nRt != 0)
	{
		m_netsoc = 0;
		OnConnectTalkOk(false);
		//return false;
	}
	else
	{
		m_hThreadRecv = ::CreateThread(0,0,RecvData,this,0,0);
		OnConnectTalkOk(true);
		//return true;
	}
#endif

#else	// _MY_UDP
	
	m_session = m_netTran.Connect(nIP,12345,1,0);
	if(m_session.IsValid())
	{
		//m_hThreadRecv = ::CreateThread(0,0,RecvData,this,0,0);
		//return true;
	}
	else
	{
		//return false;
	};
#endif	// _MY_UDP

}

void CMMTalker::WaitTalk(int nUserID,uint32 ip,uint16 port,uint32 nTranIP,uint16 nTranPort)
{
#ifdef _LOCAL_TEST
	return ;
#endif

#ifndef _MY_UDP

#ifndef _TCP_TEST
	static MMTALK_PARAM param;
	param.pThis = this;
	param.fid = nUserID;
	param.ip = ip;
	param.port = port;
	param.tranIp = nTranIP;
	param.tranPort = nTranPort;
	::CreateThread(0,0,AcceptProc,&param,0,0);
#else
	
	SOCKADDR_IN  addrClient;
	int nAddrLen = sizeof(SOCKADDR_IN);
	m_socketAccept = accept(m_socketListen,(sockaddr *)&addrClient,&nAddrLen);;
	if(m_socketAccept == 0)
	{
		TRACE("监听失败");
	}
	m_netsoc = 1;

	if(m_netsoc > 0)
	{
		m_hThreadRecv = ::CreateThread(0,0,RecvData,this,0,0);
		OnAcceptTalkOk(true);
	}
	else
	{
		m_netsoc = 0;
		OnAcceptTalkOk(false);
	}
#endif
#else // _MY_UDP
	m_session = m_netTran.Accept(0,1);
	if(m_session.IsValid())
	{
		m_hThreadRecv = ::CreateThread(0,0,RecvData,this,0,0);
	}
	else
	{
	}
#endif// _MY_UDP
}

DWORD WINAPI CMMTalker::RecvData(void * pParam)
{
	CMMTalker * pThis = (CMMTalker *)pParam;
	pThis->OnRecvData();
	return 0;
}

void CMMTalker::OnRecvData()
{
	int nLen;
	while(true)
	{
#ifndef _MY_UDP
#ifndef _TCP_TEST
		if(m_netsoc == 0)
			break;
		if(m_pDataRecv)
			nLen = m_pDataTran->Recv(m_netsoc,(char*)m_pDataRecv,m_nDataRecv);
		else
			nLen = 0;
		//TRACE("recv = %d\n",nLen);
#else
		if(m_socketAccept == 0 && m_socketConn == 0)
			break;
		if(m_socketAccept)
		{
			nLen = recv(m_socketAccept,(char*)m_pDataRecv,m_nDataRecv,0);

		}
		else if(m_socketConn)
		{
			nLen = recv(m_socketConn,(char*)m_pDataRecv,m_nDataRecv,0);
		}
#endif
		if(nLen > 0)
		{
			ParseData(m_pDataRecv,nLen);
		}
		else
		{
			m_netsoc = 0;
			break;
		}
#else	// _MY_UDP
		if(m_session.IsValid() == false)
			break;
		else
			nLen = m_netTran.Recv(m_session,(char*)m_pDataRecv,m_nDataRecv);
		if(nLen > 0)
			ParseData(m_pDataRecv,nLen);
		else
			break;
#endif	// _MY_UDP
		
	}
}

void CMMTalker::StartAudioCap()
{
	m_acap.Start();
}

void CMMTalker::StopAudioCap()
{
	m_acap.Stop();
}

void CMMTalker::Stop()
{
	if(m_netsoc && m_pDataTran)
	{
		m_pDataTran->CloseSocket(m_netsoc);
		m_netsoc = 0;
	}
	StopAudioCap();
	CloseVideoCap();
	m_vplayer.Stop();
	m_dataMaker.Clear();
	CMMDataParse::Clear();
	m_vcodec.Close();
	
	m_aplayer.Stop();
}


void CMMTalker::CloseVideoCap()
{
	m_vcap.Close();
}

void CMMTalker::StartVideoCap()
{
	m_vcap.Start();
}


void CMMTalker::SetPlayVolume(int n)
{
	m_aplayer.SetAudioVolume(n);
}

void CMMTalker::SetRecordVolume(int n)
{
	m_acap.SetCapVolume(n);
}

void CMMTalker::SetVideoQuality(int n)
{
	m_vcodec.SetCmpQuality(n);
}

BITMAPINFO * CMMTalker::GetVideoFormat()
{
	return m_vcap.GetBmpInfo();
}

//
//void CMMTalker::SetFramePerSec(WORD nC)
//{
//	if(m_vcap.SetFramePerSec(nC))
//	{
//		MM_NOTIFY_FRAME_PER_SEC cmd;
//		cmd.nFramePerSec = nC;
//		m_dataMaker.AddControlData(CMD_NOTIFY_FRAME_PER_SEC,(BYTE*)&cmd,sizeof(cmd));
//	};
//	
//}



