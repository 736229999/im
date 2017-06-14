#include "StdAfx.h"
#include ".\mmdata.h"
#include <assert.h>

#ifdef _DEBUG
//#define _WRITE_FILE
#ifdef _WRITE_FILE
#include <fstream>
using namespace std;
#endif
#endif

CMMDataParse::CMMDataParse(int nBufLen)
{
	// 缓冲区初始大小至少为一个帧头的大小
	assert(nBufLen >= 256);
	m_nBufLen = nBufLen;
	m_pBuf = new BYTE[nBufLen];
	m_nDataLen = 0;
}

CMMDataParse::~CMMDataParse(void)
{
	if(m_pBuf)
	{
		delete [] m_pBuf;
	}
}

void CMMDataParse::Clear()
{
	m_nDataLen = 0;
}

int CMMDataParse::ParseData(const BYTE * pData,int nLen)
{

#ifdef _WRITE_FILE

	static bool bFile = false;
	static ofstream ofs;
	if(bFile == false)
	{
		ofs.open("c:\\video2.dat",ios::binary);
		bFile = true;
	}
	//ofs.write((char*)pData,nLen);
#endif


	if(pData == NULL)
		return 0;
	assert(nLen > 0);
	int nRt = 0;
	DWORD * pFlag;
	BYTE * pCursor;
	bool bExit = false;
	// 计算剩余空间
	int nSpace = m_nBufLen - m_nDataLen;
	// 计算可拷贝的长度
	int nCopy = nLen > nSpace ? nSpace : nLen; 
	// 如果当前缓冲区没有满，则先将数据拷贝入缓冲区
	if(nSpace <= 0 || nSpace < nLen)
	{	
		assert(0 && "解析缓冲区已满");
		return 0;
	}


	
	memcpy(m_pBuf + m_nDataLen,pData,nCopy);
	// 当前缓冲区中的数据长度增加
	m_nDataLen += nCopy;
	pCursor = m_pBuf;
	//int nCursor = 0;
	int nFrameSize;


	while(m_nDataLen > 0)
	{
		pFlag = (DWORD *)pCursor;
		if(m_nDataLen >= sizeof(DWORD))
		{
			switch(*pFlag)
			{
			case MMVideoFrame::FCC_VIDEO:
				if(m_nDataLen >= sizeof(MMVideoFrame))
				{
					MMVideoFrame * pVideo = (MMVideoFrame*)pCursor;
					// 得到该帧的总长度
					nFrameSize = pVideo->dwLen + sizeof(MMVideoFrame);
					// 重分配缓冲的大小
					if(nFrameSize > m_nBufLen)
					{
						if(nFrameSize > 1024 *100 || nFrameSize < 0)
						{
							// 错误的帧
							pCursor += 1;
							m_nDataLen -= 1;
						}
						else
						{
							BYTE * pNew = new BYTE[nFrameSize];
							memcpy(pNew,pCursor,m_nDataLen);
							if(m_pBuf)
								delete [] m_pBuf;
							m_pBuf = pNew;
							m_nBufLen = nFrameSize;
							bExit = true;
						}
					
					}
					// 当前已经出现完整帧数据
					else if(nFrameSize <= m_nDataLen)
					{	
#ifdef _WRITE_FILE
						ofs.write((char*)pCursor,nFrameSize);
#endif
						// 取出该帧数据
						OnVideFrame(pCursor,nFrameSize);
						m_nDataLen -= nFrameSize;
						pCursor += nFrameSize;
					}
					else
					{
						bExit = true;
					}
				}
				else
				{
					bExit = true;
				}
				break;
			case MMAudioFrame::FCC_AUDIO:
				if(m_nDataLen >= sizeof(MMAudioFrame))
				{
					MMAudioFrame * pAudio = (MMAudioFrame *)pCursor;
					// 得到该帧的总长度
					nFrameSize = pAudio->dwLen + sizeof(MMAudioFrame);
					// 重分配缓冲的大小
					if(nFrameSize > m_nBufLen)
					{
						if(nFrameSize > 1024*100 || nFrameSize < 0)
						{
							// 错误的帧
							pCursor += 1;
							m_nDataLen -= 1;
						}
						else
						{
							BYTE * pNew = new BYTE[nFrameSize];
							memcpy(pNew,pCursor,m_nDataLen);
							if(m_pBuf)
								delete [] m_pBuf;
							m_pBuf = pNew;
							m_nBufLen = nFrameSize;
							bExit = true;
						}
						
					}
					// 当前已经出现完整帧数据
					else if(nFrameSize <= m_nDataLen)
					{			

#ifdef _WRITE_FILE
						ofs.write((char*)pCursor,nFrameSize);
#endif
						// 取出该帧数据
						OnAudioFrame(pCursor,nFrameSize);
						m_nDataLen -= nFrameSize;
						//nCursor += nFrameSize;
						pCursor += nFrameSize;
					}
					else
					{
						bExit = true;
					}
				}
				else
				{
					bExit = true;
				}
				break;
			case MMControlFrame::FCC_CONTROL:
				if(m_nDataLen >= sizeof(MMControlFrame))
				{
					MMControlFrame * pControl = (MMControlFrame *)pCursor;
					// 得到该帧的总长度
					nFrameSize = pControl->dwLen + sizeof(MMControlFrame);
					// 重分配缓冲的大小
					if(nFrameSize > m_nBufLen)
					{
						BYTE * pNew = new BYTE[nFrameSize];
						memcpy(pNew,pCursor,m_nDataLen);
						if(m_pBuf)
							delete [] m_pBuf;
						m_pBuf = pNew;
						m_nBufLen = nFrameSize;
						bExit = true;
					}
					// 当前已经出现完整帧数据
					else if(nFrameSize <= m_nDataLen)
					{			
						// 取出该帧数据
						OnControlFrame(pCursor,nFrameSize);
						m_nDataLen -= nFrameSize;
						//nCursor += nFrameSize;
						pCursor += nFrameSize;
					}
					// 当前帧数据不完整
					else
					{
						bExit = true;
					}
				}
				else
				{
					bExit = true;
				}
				break;
			default:
				//bExit = true;
				//m_nDataLen = 0;
				pCursor += 1;
				m_nDataLen -= 1;
				TRACE("err:出现无法识别的帧类型!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				assert(0 && "出现无法识别的帧类型！！");
				break;
			};

		}
		else
		{
			break;
		}
		if(bExit)
		{
			break;
		};
	}
	// 移动剩下的数据到缓冲前
	if( (m_pBuf != pCursor) && m_nDataLen)
	{
		memcpy(m_pBuf,pCursor,m_nDataLen);
	}
	
	return nCopy;
}


