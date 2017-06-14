/******************************************************************
** �ļ���:	videoCompress.h
** ��  Ȩ:	���ڻ��Ǻ�Ѷ
** ������:	Tony
** ��  ��:	2007-7-19 15:30
** ��  ��:  ʹ��xvid������Ƶѹ����ѹ��

**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
*******************************************************************/

#ifndef __VIDEO_ENCODER_DECODER__
#define __VIDEO_ENCODER_DECODER__
#include "xvid.h"
#pragma comment(lib,"videocodec.lib")

class Cxvid
{
public:
	Cxvid();
	~Cxvid();
	bool init(int width,int height);    //��ʼ��xvid
	bool destory(); //����xvid
	//��Ƶѹ������
	bool compress(char* iBuf,const int& iSize,char* oBuf,int& oSize);
	//��Ƶ��ѹ����
	bool deCompress(char* iBuf,const int& iSize,char* oBuf,int& oSize);
protected:
	static int globalInit();  //ȫ�ֳ�ʼ��������ʹ��xvid_gbl_init��ʼ��
private:
	static xvid_gbl_init_t m_xvid_gbl_init;   //ȫ�ֽṹ��ÿ�����̳�ʼ��һ��
	void *m_hEncoder,*m_hDecoder;  //ѹ������ѹָ��
	xvid_enc_create_t m_enc_create;  //ѹ������ʹ��
	xvid_dec_create_t m_dec_create;  //��ѹ������ʹ��
	xvid_enc_frame_t  m_enc_frame;   //ѹ��֡�ṹ
	xvid_dec_frame_t  m_dec_frame;   //��ѹ֡�ṹ
	static bool m_bInit;   //�Ƿ��Ѿ���ʼ����
	int m_height,m_width;  //��Ƶ�ĸ߶ȺͿ��
};

#endif/*__VIDEO_ENCODER_DECODER__*/