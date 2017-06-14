#include "stdafx.h"
#include ".\msgobj.h"



CMsgObj::CMsgObj(void)
{
}

CMsgObj::~CMsgObj(void)
{
}


void CMsgObj::Load(const char * szMsg)
{
	m_arrMsgSeg.clear();
	
	if(szMsg == NULL)
		return;
	
	const char * p = szMsg;
	const char * pSeg = szMsg;
	MSGOBJ_SEG seg;
	string sMsg;
	seg.nType = MOT_COMMON_MSG;
	while(*p)
	{
		// �ҵ�[img]��ʼλ��
		if(*p=='[')
		{
			// ͼƬ���忪ʼ
			if(strncmp(p,"[img]",5) == 0)
			{
				// �������һ����Ϣ�����ȱ���
				if((p - pSeg) > 0)
				{
					sMsg.assign(pSeg,p);
					seg.sData = sMsg;
					// ����Ϣ��������ȷ��
					m_arrMsgSeg.push_back(seg);
				}
				seg.nType = MOT_WEB_PIC;
				p += 5;
				pSeg = p;
				// ��ʼ���������������[/img]
				while(*p)
				{
					if(*p == '[')
					{
						// ͼƬ�������
						if(strncmp(p,"[/img]",6) == 0)
						{
							// ����ͼƬ��Ϣ
							sMsg.assign(pSeg,p);
							seg.sData = sMsg;
							m_arrMsgSeg.push_back(seg);
							p += 6;
							pSeg = p;
							// Ĭ����һ��Ϊ��ͨ��Ϣ
							seg.nType = MOT_COMMON_MSG; 
							break;
						}
						else
						{
							p++;
						}
					}
					else
					{
						p++;
					}
				}
			}
			// flash���忪ʼ
			else if(strncmp(p,"[flash]",7) == 0)
			{
				p+=7;
				// .
				// .
				// .
			}
			// movie���忪ʼ
			else if(strncmp(p,"[mov]",5) ==0)
			{
				p+=5;
				// .
				// .
				// .
			}
			else
			{
				p++;
			}
		}
		else
		{
			p++;
		}
	}
	if(p != pSeg)
	{
		// ��ʱ������֤һ�¸���Ϣ���Ƿ���������
		if(seg.nType==MOT_COMMON_MSG)
			seg.sData = pSeg;
		else
		{
			// û�н����Ķ��壬Ŀǰ�������ϸ���֤������Ϸ�
			seg.sData = pSeg;
		}
		m_arrMsgSeg.push_back(seg);
	}
	// �����滻ת��հ�
	//size_t i ;
	//size_t nC = m_arrMsgSeg.size();
	//for(i = 0; i < nC; i++)
	//{
	//	ProcBlank(m_arrMsgSeg[i].sData);
	//}
}

void CMsgObj::GetWebImage(vector<string>& arrImg) const
{
	arrImg.clear();
	size_t nC = m_arrMsgSeg.size();
	for(size_t i=0;i<nC;i++)
	{
		const MSGOBJ_SEG & seg = m_arrMsgSeg[i];
		if(seg.nType==1)
			arrImg.push_back(seg.sData);
	}
}

bool CMsgObj::ReplaceWebImage(const vector<string> arrName)
{
	size_t nC = arrName.size();
	size_t nSegs = m_arrMsgSeg.size();
	size_t nImgs = 0;
	size_t i,j=0;
	// �ȼ������Ƿ�ƥ��
	for(i = 0; i < nSegs; i++)
	{
		if(m_arrMsgSeg[i].nType == MOT_WEB_PIC)
			nImgs++;
	}
	if(nImgs != nC)
		return false;
	for(i = 0; i < nSegs; i++)
	{
		if(m_arrMsgSeg[i].nType == MOT_WEB_PIC)
		{
			// �滻
			m_arrMsgSeg[i].sData = "<image:"+arrName[j]+">";
			j++;
		}
	}
	return true;
}

string CMsgObj::ToMsg() const
{
	string sRst;
	size_t nC = m_arrMsgSeg.size();
	size_t i;
	for( i = 0; i < nC; i++)
	{
		sRst+=m_arrMsgSeg[i].sData;
	}
	return sRst;
}


void CMsgObj::ProcBlank(string& sUrl)
{
	const char * pS = sUrl.c_str();
	char * szBuf = new char[sUrl.size() + 1];
	char * pD = szBuf;
	while(*pS)
	{
		if(*pS == '%')
		{
			// %20
			if(*(pS+1)=='2' && *(pS+2)=='0')
			{
				*pD = ' ';	// �滻�հ�
				pS += 2;
			}
			else
			{
				*pD = *pS;
			}
		}
		else
		{
			*pD = *pS;
		}
		pD++;
		pS++;
	}
	*pD = '\0';
	sUrl = szBuf;
	delete [] szBuf;
}

