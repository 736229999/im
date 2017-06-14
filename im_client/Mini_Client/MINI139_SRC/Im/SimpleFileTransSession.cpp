#include "StdAfx.h"
#include "simplefiletranssession.h"
#include "localdata.h"
#include "xtpacket.h"
#include "improtocol.h"
#include "RichEditCtrlEx.h"


// 发送消息
void SEND_FILE_STRUCT_NEW::SendMsg()
{
	if(pMsg == NULL)
	{
		return;
	}

	g_pProtocol->SendP2PMsg(*pMsg, nType);
}

CSimpleFileTransSession::CSimpleFileTransSession(void)
{
	m_nType = CIMProtocol::SMT_SERVER;
}

CSimpleFileTransSession::~CSimpleFileTransSession(void)
{
	CString strImageCachePath = g_LocalData.GetImageFileCachePath();

	int i;
	for ( i=0; i<m_vecRecvFiles.size(); i++ )
	{
		RECV_FILE_STRUCT *p = m_vecRecvFiles[i];
		if ( p->fileObject )
		{
			try
			{
			
				if(p->fileObject->m_hFile != (HANDLE)0xFFFFFFFF)//zhangmin
					p->fileObject->Close();
				delete p->fileObject;
			}
			catch(...)
			{
			}
			
			CString strFile;
			strFile.Format("%s%s.tmp",strImageCachePath,p->file_name);
			DeleteFile(strFile);
		}

		delete p;
	}

	m_vecRecvFiles.clear();

	for ( i=0; i<m_vecSendFiles.size(); i++ )
	{
		SEND_FILE_STRUCT_NEW *p = m_vecSendFiles[i];

		if ( p->fileObject )
		{
			try
			{
				if(p->fileObject->m_hFile != (HANDLE)0xFFFFFFFF)//zhangmin
				{
					p->fileObject->Close();
				}
				delete p->fileObject;
			}
			catch(...)
			{
			}

			p->fileObject=NULL;
		}

		delete p;
	}

	m_vecSendFiles.clear();
}

void CSimpleFileTransSession::SF_AbortPfileTransmit()
{
	XT_MSG msg;
	SF_InitXtMsg(msg);

	msg.data_type = 5;

	int i;
	for ( i=0; i<m_vecSendFiles.size(); i++ )
	{
		CSendPacket spMsg(msg.data);
		spMsg<<m_vecSendFiles[i]->file_id<<g_ImUserInfo.GetId();
		msg.data_len = spMsg.GetSize();

		g_pProtocol->SendP2PMsg(msg,m_nType);
		Sleep(10);
	}

	for ( i=0; i<m_vecRecvFiles.size(); i++ )
	{
		CSendPacket spMsg(msg.data);
		spMsg<<m_vecRecvFiles[i]->file_id<<m_vecRecvFiles[i]->from_id;
		msg.data_len = spMsg.GetSize();

		g_pProtocol->SendP2PMsg(msg,m_nType);
		Sleep(10);
	}

}

// 请求开始上传图片
void CSimpleFileTransSession::SF_StartUploadCfileReq(const IMAGE_FILE_STRUCT &is,uint32 cid, int nPackets)
{
	XT_MSG file_packet;
	SF_InitXtMsg(file_packet);
	SEND_FILE_STRUCT_NEW *pSend;
	pSend = new SEND_FILE_STRUCT_NEW();
	pSend->file_id   = is.file_session_id;
	pSend->file_size = is.file_size;
	pSend->send_size = 0;
	pSend->window_size = nPackets;
	pSend->cid = cid;
	strcpy(pSend->file_name,is.file_name);
	// 计算文件总包数
	int nPacketCount = pSend->file_size/CPIC_PACKET_LEN;
	if(pSend->file_size%CPIC_PACKET_LEN)
		nPacketCount++;
	pSend->nPacketCount = nPacketCount;
	m_vecSendFiles.push_back(pSend);

#ifdef _DEBUG
// 	CString sPrompt;
// 	sPrompt.Format("%s(%d)%dbytes分为%d包",
// 		is.file_name,
// 		is.file_session_id,
// 		is.file_size,
// 		pSend->nPacketCount);
// 	ShowMessage(sPrompt);
#endif

	CString strSendFile ;
	strSendFile.Format("%s%s",g_LocalData.GetImageFileCachePath(),pSend->file_name);
	pSend->fileObject=new CStdioFile();

	int nRead  = 0;
	int nCount = nPackets;
	if ( pSend->fileObject->Open(strSendFile,CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone) )
	{
		// 向服务器提出上传图片请求
		file_packet.data_type  = 8;			// 类型设置为8表示上传图片请求
		CSendPacket spReq(file_packet.data);
		spReq	<< pSend->file_id			// 文件id
				<< g_ImUserInfo.GetId()			// 文件拥有者
				<< pSend->file_size			// 文件大小
				<< (uint16)CPIC_PACKET_LEN	// 文件包大小
				<< pSend->file_name			// 文件名
				;
		file_packet.data_len=spReq.GetSize();
		g_pProtocol->SendClusterMsg(file_packet);

		/*file_packet.data_type  = 6;
		while( pSend->send_size<pSend->file_size )
		{				
			CSendPacket spMsg(file_packet.data);
			spMsg<<pSend->file_id
				 <<pSend->file_size
				 <<pSend->file_name
				 <<pSend->last_packet_id
				 <<pSend->last_file_pointer;

			nRead=pSend->fileObject->Read(buffer,sizeof(buffer));

			spMsg.WriteData(buffer,nRead);

			pSend->send_size+=nRead;
			pSend->last_packet_id++;
			pSend->last_file_pointer+=nRead ;
			file_packet.data_len=spMsg.GetSize();

			g_pProtocol->SendClusterMsg(file_packet);

			nCount--;

			if ( nCount==0 )
			{
				break;
			}
		}*/
	}
	else
	{
		SF_RemoveSendFile(pSend->file_id);
	}

	/*if ( nCount>0 )
	{
		
	}*/
}

// 请求开始上传图片应答
void CSimpleFileTransSession::SF_StartUploadCfileAck(uint32 fid)
{
	SEND_FILE_STRUCT_NEW * pSend = SF_FindSendFile(fid);

	if(pSend == NULL)
	{
		TRACE("error:服务器返回上传图片应答中的文件id(%d)错误\n",fid);
		return;
	}
	static XT_CPIC_DATA picData;
	// 设置同时发送包个数
	int nWindow = pSend->window_size;
	// 文件总包数
	int nPackets = pSend->nPacketCount;
	assert(nPackets > 0);
	int nC = (nPackets > nWindow)?nWindow:nPackets;
	int i;
	int nRead = 0;
	pSend->last_packet_id = 0;
	for(i = 0;i < nC;i++)
	{
		TRACE("\n发群图片(%d), 包%d(pos=%d)\n",
			pSend->file_id,
			pSend->nPacketCount,
			pSend->last_packet_id
		);

		picData.fid = pSend->file_id;
		picData.packetId = pSend->last_packet_id ;
		picData.cid = pSend->cid;
		picData.sid = g_ImUserInfo.GetId();
		// 从文件读一个包大小的数据块
		nRead = pSend->fileObject->Read(picData.data,CPIC_PACKET_LEN);
		if(nRead == 0)
			break;
		picData.dataLen = nRead;
		pSend->send_size += nRead;
		pSend->last_packet_id++;
		g_pProtocol->SendCPicData(picData);
	}
	// 如果图片包已经传完，则删除发送队列中的文件
	if(pSend->last_packet_id >= pSend->nPacketCount)
	{
		SF_RemoveSendFile(pSend->file_id);
	}
}

bool CSimpleFileTransSession::SF_InitRecvFile(RECV_FILE_STRUCT *pRecv)
{
	CString strCacheFileName;
	CString strCache = g_LocalData.GetImageFileCachePath();
	strCacheFileName.Format("%s%s.tmp",strCache,pRecv->file_name);

	CreateDirectory(strCache,NULL);

	pRecv->fileObject= new CStdioFile();

	if ( pRecv->fileObject->Open(strCacheFileName,CFile::modeCreate|CFile::typeBinary|CFile::modeWrite|CFile::shareDenyNone) )
	{
		char zero_buffer[1024]={0};

		int n1=pRecv->file_size / 1024;
		int n2=pRecv->file_size % 1024;

		while(n1>0)
		{
			pRecv->fileObject->Write(zero_buffer,1024);
			n1--;
		}

		pRecv->fileObject->Write(zero_buffer,n2);
		pRecv->fileObject->SeekToBegin();

		return true;
	}
	else
	{
		delete pRecv->fileObject;
		pRecv->fileObject =NULL;
			
		return false;
	}
}

//void CSimpleFileTransSession::OnRecvClusterImageFileReq(const XT_MSG &msg)
//{
//	XT_DOWNLOAD_CPIC_NOTIFY notify;
//	
//	CRecvPacket rp((char *)msg.data,sizeof(msg.data));
//	rp	>>notify.nFileId
//		>>notify.nFileSize
//		>>notify.szFileName;
//	SF_OnRecvFileReq(notify.szFileName);
//}

void CSimpleFileTransSession::SF_RecvPfileReq(const XT_MSG & msg)
{
	uint32 file_id        ;
	uint32 file_size      ;
	char   file_name[256] ;

	CRecvPacket rp((char *)msg.data,sizeof(msg.data));

	rp>>file_id>>file_size>>file_name;

	if ( SF_FindRecvFile(file_id,msg.from_id) )
	{
		return ;//重复请求,丢掉
	}

	RECV_FILE_STRUCT *pRecv = new RECV_FILE_STRUCT();
	pRecv->file_id   = file_id     ;
	pRecv->file_size = file_size   ;
	pRecv->recv_size = 0;
	pRecv->from_id   = msg.from_id ;
	pRecv->recv_tick = GetTickCount();
	strcpy(pRecv->file_name,file_name);

	//判断cache里又没有文件
	
	CString strCacheFileName, strImageFile;
	CString strCache = g_LocalData.GetImageFileCachePath();
	strCacheFileName.Format("%s%s.tmp",strCache,pRecv->file_name);
	strImageFile.Format("%s%s",strCache,pRecv->file_name);
	
	uint8 ret;

	if (  _access((LPCSTR)strImageFile,0)==-1)
	{
		if ( SF_InitRecvFile(pRecv) )
		{
			m_vecRecvFiles.push_back(pRecv);
			ret = 0;
		}
		else 
		{
			ret = 1;
		}
	}
	else
	{
		ret=2;//cache 里已经有相同的文件了，不必再接收
	}
	
	if ( ret )
	{
		delete pRecv;
	}
	else
	{
		SF_OnRecvFileReq(file_name);
	}

	XT_MSG msg_ack;
	SF_InitXtMsg(msg_ack);

	msg_ack.data_type   = 2;//recv file ack

	CSendPacket spMsg(msg_ack.data);
	spMsg<<ret<<file_id;

	msg_ack.data_len = spMsg.GetSize();

	g_pProtocol->SendP2PMsg(msg_ack,msg.recv_flag);
}

void CSimpleFileTransSession::SF_RecvAbortPfileTransmit(const XT_MSG &msg)
{
	CRecvPacket rp((char *)msg.data,sizeof(msg.data));
	uint32 file_id;
	uint32 sender_id;
	rp>>file_id>>sender_id;

	if ( sender_id!=g_ImUserInfo.GetId() )
	{
		for ( int i=0; i<m_vecRecvFiles.size(); i++ )
		{
			RECV_FILE_STRUCT *pRecv=m_vecRecvFiles[i];

			if ( pRecv->file_id==file_id )
			{
				SF_OnAbortFile(pRecv->file_name);

				if ( pRecv->fileObject )
				{
					
					if(pRecv->fileObject->m_hFile != (HANDLE)0xFFFFFFFF)//zhangmin
						pRecv->fileObject->Close();
					delete pRecv->fileObject;
				}

				pRecv->fileObject=NULL;
				TRACE("OnAbortP2PFileTransmit:删除待下载文件%s(%d)\n",pRecv->file_name,pRecv->file_id);
				delete pRecv;
				m_vecRecvFiles.erase(m_vecRecvFiles.begin()+i);
				break;
			}
		}
	}
	else
	{
		SF_RemoveSendFile(file_id);
	}
}

void CSimpleFileTransSession::SF_RemoveSendFile(int file_id)
{
	for ( int i=0; i<m_vecSendFiles.size(); i++ )
	{
		if(m_vecSendFiles[i]->file_id==file_id)
		{
			SEND_FILE_STRUCT_NEW *pSend = m_vecSendFiles[i];

			if ( pSend->fileObject )
			{
				try
				{
					if(pSend->fileObject->m_hFile != (HANDLE)0xFFFFFFFF)//zhangmin
					{
						pSend->fileObject->Close() ;

					}
					delete pSend->fileObject;

				}
				catch(...)
				{
				}

				pSend->fileObject=NULL;
			}

			delete m_vecSendFiles[i];
			m_vecSendFiles.erase(m_vecSendFiles.begin()+i);
			break;
		}
	}
}

RECV_FILE_STRUCT * CSimpleFileTransSession::SF_FindRecvFile(int file_id, uint32 sender )
{
	for ( int i=0; i<m_vecRecvFiles.size(); i++ )
	{
		RECV_FILE_STRUCT *pRecv=m_vecRecvFiles[i];

		if ( pRecv->file_id==file_id && pRecv->from_id==sender )
		{
			return pRecv;
		}
	}

	return NULL;
}

SEND_FILE_STRUCT_NEW * CSimpleFileTransSession::SF_FindSendFile(int file_id )
{
	int count = m_vecSendFiles.size();

	for ( int i=0; i<count; i++ )
	{
		SEND_FILE_STRUCT_NEW *pSend=m_vecSendFiles[i];

		if ( pSend->file_id==file_id )
		{
			return pSend;
		}
	}

	return NULL;
}


void CSimpleFileTransSession::SF_RemoveRecvFile(int file_id, uint32 sender)
{
	for ( int i=0; i<m_vecRecvFiles.size(); i++ )
	{
		RECV_FILE_STRUCT *pRecv=m_vecRecvFiles[i];

		if ( pRecv->file_id==file_id && pRecv->from_id==sender )
		{
			if ( pRecv->fileObject )
			{
				try
				{
				
					if(pRecv->fileObject->m_hFile != (HANDLE)0xFFFFFFFF)//zhangmin
						pRecv->fileObject->Close();
					delete pRecv->fileObject;
				}
				catch(...)
				{
				}

				pRecv->fileObject=NULL;
			}

			delete pRecv;
			m_vecRecvFiles.erase(m_vecRecvFiles.begin()+i);
			break;
		}
	}
}
/*
void CSimpleFileTransSession::OnRecvClusterImageFilePacket(const XT_MSG &msg)
{
	CRecvPacket rp((char *)msg.data,sizeof(msg.data));

	uint32  file_id   ;
	uint32  file_size ;
	char    file_name[255];
	uint32  packet_id ;
	uint32  file_pointer ;

	char data[FILE_PACKET_LEN];
	int  data_len;

	rp>>file_id>>file_size>>file_name>>packet_id>>file_pointer;
	data_len = rp.ReadData(data,sizeof(data));

	


	OnRecvFilePacket(file_name,file_id);

	ASSERT(data_len<=FILE_PACKET_LEN);

	RECV_FILE_STRUCT *pRecv = SF_FindRecvFile(file_id,msg.from_id);

	if ( pRecv==NULL )
	{
		pRecv = new RECV_FILE_STRUCT();

		pRecv->file_id   = file_id     ;
		pRecv->file_size = file_size   ;
		pRecv->recv_size = 0;
		pRecv->from_id   = msg.from_id ;

		sprintf(pRecv->file_name,file_name);

		if (SF_InitRecvFile(pRecv) )
		{
			m_vecRecvFiles.push_back(pRecv);
		}
		else
		{
			delete pRecv;
			return ;
		}
	}

	map<int,int>::iterator it;
	it = pRecv->mapPacketRecv.find((int)packet_id);

	

	if ( it==pRecv->mapPacketRecv.end() )
	{
		pRecv->fileObject->Seek(file_pointer,CFile::begin);
		pRecv->fileObject->Write(data,data_len);
		pRecv->recv_size+=data_len;

		pRecv->mapPacketRecv.insert(map<int,int>::value_type((int)packet_id,(int)file_pointer));
		TRACE("收图片%s(%d),已接收%d包(%d)\n",file_name,file_id,pRecv->mapPacketRecv.size(),packet_id);
		if ( pRecv->recv_size==pRecv->file_size )
		{
			try
			{
				pRecv->fileObject->Close();
				Sleep(50);
				delete pRecv->fileObject;
			}
			catch(...)
			{
			}
			
			pRecv->fileObject=NULL;


			CString strCacheFileName, strImageFile;
			CString strCache = g_LocalData.GetImageFileCachePath();
			strCacheFileName.Format("%s%s.tmp",strCache,pRecv->file_name);
			strImageFile.Format("%s%s",strCache,pRecv->file_name);

			CopyFile(strCacheFileName,strImageFile,false);
			DeleteFile(strCacheFileName);

			OnRecvImageFileComplete(pRecv->file_name);
			
			SF_RemoveRecvFile(file_id,msg.from_id);
		}
	}	
}


*/
void CSimpleFileTransSession::SF_RecvPfilePacket(const XT_MSG & msg)
{
	CRecvPacket rp((char *)msg.data,sizeof(msg.data));

	uint32  file_id;
	uint32  packet_id;
	uint32  file_pointer;

	char data[FILE_PACKET_LEN];
	int  data_len;

	rp>>file_id>>packet_id>>file_pointer;
	data_len = rp.ReadData(data,sizeof(data));

	ASSERT(data_len <= FILE_PACKET_LEN);

	RECV_FILE_STRUCT *pRecv = SF_FindRecvFile(file_id,msg.from_id);

	if ( pRecv )
	{
		map<int,int>::iterator it;
		it = pRecv->mapPacketRecv.find((int)packet_id);


		SF_OnRecvFilePacket(pRecv->file_name,file_id);

		if ( it==pRecv->mapPacketRecv.end() )
		{
			pRecv->fileObject->Seek(file_pointer,CFile::begin);

			DWORD dwWrite=0;

			bool bWriteFlag = false;
			
			bWriteFlag = WriteFile( pRecv->fileObject->m_hFile,
				data,data_len,&dwWrite,NULL);

			int count=0;

			while ( bWriteFlag==false && dwWrite!=data_len )
			{
				ShowMessage("write file error!");
				bWriteFlag = WriteFile( pRecv->fileObject->m_hFile,
					data,data_len,&dwWrite,NULL);

				if ( count++>5 )
					break;
			}

			pRecv->recv_size+=data_len;

			pRecv->mapPacketRecv.insert(map<int,int>::value_type((int)packet_id,(int)file_pointer));
			TRACE("收图片%s(%d),已接收%d包(%d)\n",pRecv->file_name,packet_id,pRecv->mapPacketRecv.size(),packet_id);
			if ( pRecv->recv_size==pRecv->file_size )
			{
				// 文件接收完毕
				try
				{
					
					if(pRecv->fileObject->m_hFile != (HANDLE)0xFFFFFFFF)//zhangmin
						pRecv->fileObject->Close();
					delete pRecv->fileObject;
				}
				catch(...)
				{
				}
				
				pRecv->fileObject=NULL;

				CString strCacheFileName, strImageFile;
				CString strCache = g_LocalData.GetImageFileCachePath();
				strCacheFileName.Format("%s%s.tmp",strCache,pRecv->file_name);
				strImageFile.Format("%s%s",strCache,pRecv->file_name);

				CopyFile(strCacheFileName,strImageFile,false);
				DeleteFile(strCacheFileName);
                CString sFileName;
				uint32 file_size = pRecv->file_size;
				sFileName = pRecv->file_name;

				SF_OnRecvFileComplete(pRecv->file_name);
				SF_RemoveRecvFile(pRecv->file_id,msg.from_id);				

				XT_MSG msg_ack;
				SF_InitXtMsg(msg_ack);
				msg_ack.data_type = 4;//file packet ack
				CSendPacket spMsg(msg_ack.data);
				spMsg<<file_id<<file_size<<sFileName<<packet_id<<file_pointer;
				msg_ack.data_len = spMsg.GetSize();
				g_pProtocol->SendP2PMsg(msg_ack,msg.recv_flag);	
				return;
			}		

		}

		XT_MSG msg_ack;
		SF_InitXtMsg(msg_ack);

		msg_ack.data_type = 4;//file packet ack

		CSendPacket spMsg(msg_ack.data);
		spMsg<<file_id<<pRecv->file_size<<pRecv->file_name<<packet_id<<file_pointer;

		msg_ack.data_len = spMsg.GetSize();
		g_pProtocol->SendP2PMsg(msg_ack,msg.recv_flag);	
	}
}

// 当含有图片时，消息发送成功后返回消息体，在群发功能中使用到
void CSimpleFileTransSession::SF_RecvPfilePacketAck(const XT_MSG &msg)
{
	CRecvPacket rp((char *)msg.data,sizeof(msg.data));
	uint32  file_id;
	rp>>file_id;
	SEND_FILE_STRUCT_NEW *pSend=SF_FindSendFile(file_id);

	if ( pSend && pSend->fileObject )
	{
		char buffer[FILE_PACKET_LEN];
		
		pSend->ack_count++;

		DWORD dwTick = GetTickCount();

		int nPackets=0;

		if ( pSend->last_ack_tick == 0 )
		{
			nPackets = 1;
		}
		else
		{
			if ( (dwTick-pSend->last_ack_tick)>1000 )
			{
				pSend->window_size--;
				pSend->ack_count=0;
				nPackets=0;
			}
			else
			{
				if ( pSend->ack_count>=pSend->window_size )
				{
					nPackets=5;
					pSend->window_size+=5;
					pSend->ack_count=0;
				}
				else
				{
					nPackets=1;
				}
			}
		}

		pSend->last_ack_tick=dwTick;
		nPackets = 1;

		for ( int n=0; n<nPackets; n++ )
		{
			int nRead=pSend->fileObject->Read(buffer,FILE_PACKET_LEN);
			
			if ( nRead>0 )
			{
				XT_MSG file_packet;
				SF_InitXtMsg(file_packet);
				file_packet.data_type=3;
				// 通讯方式
				file_packet.recv_flag = m_nType;

				CSendPacket spMsg(file_packet.data);
				spMsg<<file_id<<pSend->last_packet_id<<pSend->last_file_pointer;
				spMsg.WriteData(buffer,nRead);

				pSend->last_packet_id++;
				pSend->last_file_pointer+=nRead ;
				pSend->send_size+=nRead;
			
				file_packet.data_len=spMsg.GetSize();

				g_pProtocol->SendP2PMsg(file_packet,m_nType);
			}

			char		file_name[255] = {0};
			uint32		file_size = 0;
			uint32		last_packet_id = 0;
			char		szUrl[128] = {0};
			uint32		last_file_pointer = 0;
			rp>>file_size>>file_name>>last_packet_id>>last_file_pointer>>szUrl;

			if (/*pSend->send_size==pSend->file_size*/strcmp(szUrl, "") != 0)
			{
				CString strUrl = szUrl;
				CString strLocalPath = strUrl.Right(strUrl.GetLength() - strUrl.ReverseFind('/') - 1);
				CString strDownloadFile = g_LocalData.GetImageFileCachePath()+strLocalPath;

				//图片改造
				if (strUrl.Find("http://") == -1)
				{
					strUrl = GetPicRandomUrl() + strUrl;
				}

				DownLoadWebFile(strUrl,strDownloadFile);

				CString strSendFile ;
				strSendFile.Format("%s%s",g_LocalData.GetImageFileCachePath(),pSend->file_name);

				//判断上传的图片和原图片比较，如果失真了就重新发送
				BOOL bIsSave = ComparePicture(strSendFile, strDownloadFile);
				if(!bIsSave)
				{
					//SF_RemoveSendFile(pSend->file_id);
					pSend->nFailCount++;
					if(pSend->nFailCount >= 4)//连续5次失真则发送一个叉
					{
						CString strData = pSend->pMsg->data;
						strData.Replace(pSend->file_name, "noimage");
						strcpy(pSend->pMsg->data, strData);
						pSend->SendMsg();
						SF_RemoveSendFile(pSend->file_id);
						break;
					}
					else//重新发送图片
					{
						pSend->fileObject->Close();
						delete pSend->fileObject;
						pSend->fileObject = NULL;

						XT_MSG msg;
						SF_InitXtMsg(msg);

						msg.data_type  = 1;//

						CSendPacket spMsg(msg.data);

						pSend->file_id = CRichEditCtrlEx::m_dwFileSessionId++;

						CString strCacheFileName;
						g_LocalData.SaveImageFileToCache(strSendFile,strCacheFileName);

						CString strData = pSend->pMsg->data;
						strData.Replace(pSend->file_name, strCacheFileName);
						strcpy(pSend->pMsg->data, strData);

						strcpy(pSend->file_name, strCacheFileName);
						spMsg<<pSend->file_id<<pSend->file_size<<pSend->file_name;

						msg.data_len = spMsg.GetSize();
						g_pProtocol->SendP2PMsg(msg,m_nType);
						break;
					}
				}
				else//没有失真则直接发送图片
				{
					pSend->ReplaceUrl(szUrl);
					if(pSend->IsReplaceAll())
					{
						pSend->SendMsg();
					}
					SF_RemoveSendFile(pSend->file_id);
				}
				break;
			}
		}
	}
}

void CSimpleFileTransSession::SF_SendPfileReq(const IMAGE_FILE_STRUCT &is,int nPackets, std::tr1::shared_ptr<XT_MSG> &msgO, int &nType)
{
	XT_MSG msg;
	SF_InitXtMsg(msg);

	msg.data_type  = 1;//

	CSendPacket spMsg(msg.data);
	spMsg<<is.file_session_id<<is.file_size<<is.file_name;

#ifdef _DEBUG
// 	CString sPrompt;
// 	int nP = is.file_size/FILE_PACKET_LEN;
// 	if(is.file_size%FILE_PACKET_LEN)
// 		nP++;
// 	sPrompt.Format("%s(%d)%dbytes分为%d包",is.file_name,is.file_session_id,is.file_size,nP);
// 	ShowMessage(sPrompt);
#endif
	msg.data_len = spMsg.GetSize();
	
	SEND_FILE_STRUCT_NEW *pSend = new SEND_FILE_STRUCT_NEW();

	pSend->file_id   = is.file_session_id;
	pSend->file_size = is.file_size;
	pSend->window_size = nPackets;

	pSend->send_size = 0;
	sprintf(pSend->file_name,is.file_name);

	// 保存原有消息体，当所有图片都上传到图片空间后，消息体发送出去
	pSend->pMsg = msgO;
	pSend->nType = nType;

	m_vecSendFiles.push_back(pSend);

	g_pProtocol->SendP2PMsg(msg,m_nType);
}

/*
void CSimpleFileTransSession::OnClusterImageFilePacketAck(const XT_MSG &msg)
{
	CRecvPacket rp((char *)msg.data,sizeof(msg.data));

	uint32 file_id;
	uint32 file_size;
	char   file_name[255];
	uint32 last_packet_id;
	uint32 last_file_pointer;

	rp>>file_id>>file_size>>file_name>>last_packet_id>>last_file_pointer;

	SEND_FILE_STRUCT *pSend = FindSendFile(file_id);

	if ( pSend )
	{
		char buffer[FILE_PACKET_LEN];
		int nRead=pSend->fileObject->Read(buffer,sizeof(buffer));

		TRACE("\n发送群图片(%d), 包(%d+滑动大小), %d/%d\n", file_id,last_packet_id, pSend->last_file_pointer,file_size);
	
		XT_MSG file_packet;
		SF_InitXtMsg(file_packet);
		file_packet.data_type=6;

		if ( nRead>0 )
		{
			CSendPacket spMsg(file_packet.data);
			spMsg<<pSend->file_id
				 <<pSend->file_size
				 <<pSend->file_name
				 <<pSend->last_packet_id
				 <<pSend->last_file_pointer;

			spMsg.WriteData(buffer,nRead);

			pSend->last_packet_id++;
			pSend->last_file_pointer+=nRead ;
			pSend->send_size+=nRead;

			file_packet.data_len=spMsg.GetSize();

			g_pProtocol->SendClusterMsg(file_packet);
			// 
			Sleep(5);
		}
	
		if ( pSend->send_size==pSend->file_size)
		{
			SF_RemoveSendFile(pSend->file_id);
		}
	}
}

*/
void CSimpleFileTransSession::SF_RecvPfileAck(const XT_MSG & msg)
{
	CRecvPacket rp((char *)msg.data,sizeof(msg.data));
	uint32 file_id;
	uint8  ret;
	char szUrl[1024] = {0};		// 当此张图片已经传输过时，会收到图片的url，未传输过时，url为空
	rp>>ret>>file_id>>szUrl;

	if ( ret ) 
	{
		if(ret == 2)
		{
			TRACE("接收方已经有该图片，放弃接收该图片\n");
			// 判断图片是否已传输过
			SEND_FILE_STRUCT_NEW *pSend = SF_FindSendFile(file_id);
			if(pSend != NULL && strcmp(szUrl, "") != 0)
			{
				pSend->ReplaceUrl(szUrl);
				if(pSend->IsReplaceAll())
				{
					pSend->SendMsg();
				}
			}
		}
		else if(ret == 1)
			TRACE("接收方在初始化图片文件时发生错误,放弃接受图片\n");
		SF_RemoveSendFile(file_id);
		return;//接收失败，对方不能接收此文件
	}

	XT_MSG file_packet;
	SF_InitXtMsg(file_packet);
	
	file_packet.data_type   = 3;//

	SEND_FILE_STRUCT_NEW *pSend = SF_FindSendFile(file_id);

	if ( pSend && pSend->file_id==file_id && pSend->fileObject==NULL )
	{
		static char buffer[FILE_PACKET_LEN];

		CString strSendFile ;
		strSendFile.Format("%s%s",g_LocalData.GetImageFileCachePath(),pSend->file_name);

		pSend->fileObject=new CStdioFile();

		int nRead  = 0;
		int nCount = pSend->window_size;

		if ( pSend->fileObject->Open(strSendFile,CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone) )
		{
			while( (nRead=pSend->fileObject->Read(buffer,sizeof(buffer)))>0 )
			{					
				CSendPacket spMsg(file_packet.data);

				spMsg<<file_id<<pSend->last_packet_id<<pSend->last_file_pointer;
				spMsg.WriteData(buffer,nRead);
				
				pSend->last_packet_id++;
				pSend->last_file_pointer+=nRead ;
				pSend->send_size+=nRead;
				file_packet.data_len=spMsg.GetSize();
				file_packet.recv_flag = m_nType;
				
				file_packet.recv_flag = m_nType;
				g_pProtocol->SendP2PMsg(file_packet,m_nType);

				nCount--;

				if ( nCount==0 )
				{
					break;
				}
			}
				
			/*if ( nCount>0 )	// 如果大于0，说明图片太小，在一个窗口周期内，图片就发送完毕了
			{
			SF_RemoveSendFile(file_id);
			}*/
		}
	}
}


void CSimpleFileTransSession::SF_UploadCfileAck(uint32 fid)
{
	SEND_FILE_STRUCT_NEW *pSend = SF_FindSendFile(fid);
	static XT_CPIC_DATA picData;
	if ( pSend )
	{
		picData.cid = pSend->cid;
		picData.sid = g_ImUserInfo.GetId();
		picData.packetId = pSend->last_packet_id;
		int nRead = pSend->fileObject->Read(picData.data,sizeof(picData.data));
		if ( nRead>0 )
		{
			pSend->last_packet_id++;
			//pSend->last_file_pointer+=nRead ;
			pSend->send_size += nRead;
			picData.dataLen = nRead;
			picData.fid = pSend->file_id;
			g_pProtocol->SendCPicData(picData);
			TRACE("\n发群图片(%d), 共%d包(pos=%d)\n",
				pSend->file_id,
				pSend->nPacketCount,
				pSend->last_packet_id
			);
			Sleep(0);
		}
		// 判断是否上传完毕
		if ( pSend->last_packet_id >= pSend->nPacketCount)
		{
			TRACE("上传完毕\n");
			SF_RemoveSendFile(pSend->file_id);
		}
	}
}


bool CSimpleFileTransSession::SF_RecvStartDownloadCfileNotify(const XT_MSG & msg)
{
	bool bRt = true;
	XT_DOWNLOAD_CPIC_NOTIFY notify;
	CRecvPacket rp((char *)msg.data,sizeof(msg.data));
	rp	>>notify.nFileId
		>>notify.nSrcId
		>>notify.nFileSize
		>>notify.nPacketSize
		>>notify.szFileName;
	int nCount = notify.nFileSize/notify.nPacketSize;
	if(notify.nFileSize%notify.nPacketSize)
		nCount++;
	//
	// 初始化待下载的文件
	// 先查找该文件是否正在下载中，如果能查找到则有异常
	//
	RECV_FILE_STRUCT *pRecv = SF_FindRecvFile(notify.nFileId,notify.nSrcId);
	assert(pRecv == NULL);
	if ( pRecv == NULL )
	{
		pRecv = new RECV_FILE_STRUCT();
		pRecv->file_id   = notify.nFileId;
		pRecv->file_size = notify.nFileSize  ;
		pRecv->recv_size = 0;
		pRecv->from_id   = notify .nSrcId;
		pRecv->nNextPacketId = 0;
		pRecv->nPacketCount = nCount;
		pRecv->nClusterId = msg.cluster_id;
#ifdef _DEBUG
		pRecv->nWaitTimes = 0;
#else
		pRecv->nWaitTimes = 2;	// 不要立刻下载,先等n次
#endif
		sprintf(pRecv->file_name,notify.szFileName);

		if (SF_InitRecvFile(pRecv) )
		{
			// 需要下载的文件
			m_vecRecvFiles.push_back(pRecv);
			TRACE("需要下载的文件(%s)(%d),所有者(%d)\n",pRecv->file_name,pRecv->file_id,pRecv->from_id);
		}
		else
		{
			TRACE("初始化下载图片文件失败！");
			delete pRecv;
			bRt = false;
		}
	}
	if(bRt == false)
	{
		SF_OnAbortFile(pRecv->file_name);
	}

	SF_OnRecvFileReq(notify.szFileName);

	return bRt;	
}

void CSimpleFileTransSession::SF_DownloadCfile(const RECV_FILE_STRUCT * pDwnFile)
{
	XT_DOWNLOAD_CPIC_REQ req;
	req.nFileId = pDwnFile->file_id;
	req.nFileSid = pDwnFile->from_id;
	req.nPacketId = pDwnFile->nNextPacketId;
	req.nCId = pDwnFile->nClusterId;
	//TRACE("准备下载文件(%d),所有者(%d)\n",pDwnFile->file_id,req.nFileSid);
	g_pProtocol->SendDownloadCPicReq(req);
}


void CSimpleFileTransSession::SF_DownloadCfileAck(const XT_DOWNLOAD_CPIC_ACK & ack)
{
	if(ack.nPacketId == (uint16)0xffff)
		return;
	RECV_FILE_STRUCT *pRecv = SF_FindRecvFile(ack.nFileId,ack.nSrcId);
	//ASSERT(pRecv);
	if(pRecv == NULL)
	{
		TRACE("下载图片包指定的文件%d,pos:%d,不存在:源%d\n",ack.nFileId,ack.nPacketId,ack.nSrcId);
		return;
	}
	if(XT_DOWNLOAD_CPIC_ACK::RET_WAIT == ack.ret)
	{
		pRecv->nWaitTimes = 6; // 等待n次约n秒
		TRACE("下载图片需等待%d次",pRecv->nWaitTimes);
		return;
	}
	else if(XT_DOWNLOAD_CPIC_ACK::RET_FAILED == ack.ret)
	{
		// 该文件下载失败
		TRACE("图片文件(%s)下载失败\n",pRecv->file_name);
		SF_OnAbortFile(pRecv->file_name);
		SF_RemoveRecvFile(ack.nFileId,ack.nSrcId);
		return;
	}
	else if(ack.ret == (uint8)SERVER_TIMEOUT)
	{
		// 该文件下载失败
		TRACE("图片文件(%s)下载失败\n",pRecv->file_name);
		SF_OnAbortFile(pRecv->file_name);
		SF_RemoveRecvFile(ack.nFileId,ack.nSrcId);
		return;
	}
	if(ack.ret != 0)
	{
		TRACE("下载图片应答，无法识别的错误\n");
		return;
	}
	if(ack.nPacketId != pRecv->nNextPacketId)
	{
		TRACE("ERROR:当前接收群图片块位置不正确\n");
		return;
	}
	// 写图片
	pRecv->fileObject->Write(ack.data,ack.nDataLen);
	pRecv->recv_size += ack.nDataLen ;
	pRecv->nNextPacketId ++;

	TRACE("收图片%s(%d),已接收%d包,共%d\n",
		pRecv->file_name,
		pRecv->file_id,
		pRecv->nNextPacketId,
		pRecv->nPacketCount);
	
	XT_DOWNLOAD_CPIC_REQ req;

	// 图片接收完毕
	if ( pRecv->nNextPacketId >= pRecv->nPacketCount )
	{
		TRACE("图片(%d)下载完毕\n",pRecv->file_id);
		// 通知服务器下载完毕
		req.nFileId = pRecv->file_id;
		req.nFileSid = pRecv->from_id;
		req.nPacketId = pRecv->nNextPacketId;
		req.nPacketId = (uint16)0xffff;
		g_pProtocol->SendDownloadCPicReq(req,0);
		try
		{
			
			if(pRecv->fileObject->m_hFile != (HANDLE)0xFFFFFFFF)//zhangmin
				pRecv->fileObject->Close();
			Sleep(50);
			delete pRecv->fileObject;
		}
		catch(...)
		{
		}
		pRecv->fileObject=NULL;
		CString strCacheFileName, strImageFile;
		CString strCache = g_LocalData.GetImageFileCachePath();
		strCacheFileName.Format("%s%s.tmp",strCache,pRecv->file_name);
		strImageFile.Format("%s%s",strCache,pRecv->file_name);
		CopyFile(strCacheFileName,strImageFile,false);
		DeleteFile(strCacheFileName);

		SF_OnRecvFileComplete(pRecv->file_name);
		SF_RemoveRecvFile(pRecv->file_id,pRecv->from_id);
	}	
	else
	{
		// 继续下载下一个图片
		req.nFileId = pRecv->file_id;
		req.nFileSid = pRecv->from_id;
		req.nPacketId = pRecv->nNextPacketId;
		req.nCId = pRecv->nClusterId;
		g_pProtocol->SendDownloadCPicReq(req);
	}
}

//比较两张图片的所有像素，超过5%不一样则判断为失真
BOOL CSimpleFileTransSession::ComparePicture(CString strFile1, CString strFile2)
{
	CImage img1;
	img1.Load(strFile1);
	HBITMAP hBmp1 = (HBITMAP)img1.operator HBITMAP();//(HBITMAP)LoadImage( NULL , strFile1 , IMAGE_BITMAP , 0 , 0 , LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	CImage img2;
	img2.Load(strFile2);
	HBITMAP hBmp2 = (HBITMAP)img2.operator HBITMAP();//(HBITMAP)LoadImage( NULL , strFile2 , IMAGE_BITMAP , 0 , 0 , LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	if (hBmp1 != NULL && hBmp2 != NULL)
	{
		BITMAP bmp1 = {0};
		int ret1 = GetObject( hBmp1 , sizeof(BITMAP) , &bmp1 );

		BITMAP bmp2 = {0};
		int ret2 = GetObject( hBmp2 , sizeof(BITMAP) , &bmp2 );

		if (ret1 && ret2)
		{
			if(bmp1.bmWidth == bmp2.bmWidth && bmp1.bmHeight == bmp2.bmHeight )
			{
				int nSize = bmp1.bmWidth * bmp1.bmHeight;
				int nDiff = 0;
				for(int nWidth = 0; nWidth < bmp1.bmWidth; nWidth++)
				{
					for (int nHeight = 0; nHeight < bmp1.bmHeight; nHeight++)
					{
						BYTE* pPixel1 = (BYTE*)bmp1.bmBits;  //指向BMP像素的指针

						BYTE *pOffset1 = pPixel1 + (bmp1.bmHeight - 1) * bmp1.bmWidthBytes;  //指向最后一行像素数据

						pOffset1 =  (pOffset1 -  nHeight * bmp1.bmWidthBytes) + nWidth * bmp1.bmBitsPixel / 8; //指向指定像素位置

						DWORD rgb1 = 0;

						memcpy( &rgb1 , pOffset1 , bmp1.bmBitsPixel / 8 );

						BYTE* pPixel2 = (BYTE*)bmp2.bmBits;  //指向BMP像素的指针

						BYTE *pOffset2 = pPixel2 + (bmp2.bmHeight - 1) * bmp2.bmWidthBytes;  //指向最后一行像素数据

						pOffset2 =  (pOffset2 -  nHeight * bmp2.bmWidthBytes) + nWidth * bmp2.bmBitsPixel / 8; //指向指定像素位置

						DWORD rgb2 = 0;

						memcpy( &rgb2 , pOffset2 , bmp2.bmBitsPixel / 8 );

						if(rgb1 != rgb2)
						{
							nDiff++;
							if(nDiff / (double)nSize > 0.05)
							{
								DeleteObject( hBmp1 );
								hBmp1 = NULL;
								ZeroMemory( &bmp1 , sizeof(bmp1) );

								DeleteObject( hBmp2 );
								hBmp2 = NULL;
								ZeroMemory( &bmp2 , sizeof(bmp2) );
								return FALSE;
							}
						}
					}
				}
			}
		}

		DeleteObject( hBmp1 );
		hBmp1 = NULL;
		ZeroMemory( &bmp1 , sizeof(bmp1) );

		DeleteObject( hBmp2 );
		hBmp2 = NULL;
		ZeroMemory( &bmp2 , sizeof(bmp2) );
	}
	return TRUE;
}