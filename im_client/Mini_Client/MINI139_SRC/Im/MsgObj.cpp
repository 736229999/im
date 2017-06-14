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
		// 找到[img]开始位置
		if(*p=='[')
		{
			// 图片定义开始
			if(strncmp(p,"[img]",5) == 0)
			{
				// 如果有上一段消息，则先保存
				if((p - pSeg) > 0)
				{
					sMsg.assign(pSeg,p);
					seg.sData = sMsg;
					// 该消息的类型已确定
					m_arrMsgSeg.push_back(seg);
				}
				seg.nType = MOT_WEB_PIC;
				p += 5;
				pSeg = p;
				// 开始搜索定义结束符号[/img]
				while(*p)
				{
					if(*p == '[')
					{
						// 图片定义结束
						if(strncmp(p,"[/img]",6) == 0)
						{
							// 保存图片信息
							sMsg.assign(pSeg,p);
							seg.sData = sMsg;
							m_arrMsgSeg.push_back(seg);
							p += 6;
							pSeg = p;
							// 默认下一段为普通消息
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
			// flash定义开始
			else if(strncmp(p,"[flash]",7) == 0)
			{
				p+=7;
				// .
				// .
				// .
			}
			// movie定义开始
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
		// 此时可以验证一下该消息段是否正常结束
		if(seg.nType==MOT_COMMON_MSG)
			seg.sData = pSeg;
		else
		{
			// 没有结束的定义，目前不进行严格验证，令其合法
			seg.sData = pSeg;
		}
		m_arrMsgSeg.push_back(seg);
	}
	// 处理替换转义空白
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
	// 先检查个数是否匹配
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
			// 替换
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
				*pD = ' ';	// 替换空白
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

