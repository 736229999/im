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
	// ��������ʼ��С����Ϊһ��֡ͷ�Ĵ�С
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
	// ����ʣ��ռ�
	int nSpace = m_nBufLen - m_nDataLen;
	// ����ɿ����ĳ���
	int nCopy = nLen > nSpace ? nSpace : nLen; 
	// �����ǰ������û���������Ƚ����ݿ����뻺����
	if(nSpace <= 0 || nSpace < nLen)
	{	
		assert(0 && "��������������");
		return 0;
	}


	
	memcpy(m_pBuf + m_nDataLen,pData,nCopy);
	// ��ǰ�������е����ݳ�������
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
					// �õ���֡���ܳ���
					nFrameSize = pVideo->dwLen + sizeof(MMVideoFrame);
					// �ط��仺��Ĵ�С
					if(nFrameSize > m_nBufLen)
					{
						if(nFrameSize > 1024 *100 || nFrameSize < 0)
						{
							// �����֡
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
					// ��ǰ�Ѿ���������֡����
					else if(nFrameSize <= m_nDataLen)
					{	
#ifdef _WRITE_FILE
						ofs.write((char*)pCursor,nFrameSize);
#endif
						// ȡ����֡����
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
					// �õ���֡���ܳ���
					nFrameSize = pAudio->dwLen + sizeof(MMAudioFrame);
					// �ط��仺��Ĵ�С
					if(nFrameSize > m_nBufLen)
					{
						if(nFrameSize > 1024*100 || nFrameSize < 0)
						{
							// �����֡
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
					// ��ǰ�Ѿ���������֡����
					else if(nFrameSize <= m_nDataLen)
					{			

#ifdef _WRITE_FILE
						ofs.write((char*)pCursor,nFrameSize);
#endif
						// ȡ����֡����
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
					// �õ���֡���ܳ���
					nFrameSize = pControl->dwLen + sizeof(MMControlFrame);
					// �ط��仺��Ĵ�С
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
					// ��ǰ�Ѿ���������֡����
					else if(nFrameSize <= m_nDataLen)
					{			
						// ȡ����֡����
						OnControlFrame(pCursor,nFrameSize);
						m_nDataLen -= nFrameSize;
						//nCursor += nFrameSize;
						pCursor += nFrameSize;
					}
					// ��ǰ֡���ݲ�����
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
				TRACE("err:�����޷�ʶ���֡����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				assert(0 && "�����޷�ʶ���֡���ͣ���");
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
	// �ƶ�ʣ�µ����ݵ�����ǰ
	if( (m_pBuf != pCursor) && m_nDataLen)
	{
		memcpy(m_pBuf,pCursor,m_nDataLen);
	}
	
	return nCopy;
}


