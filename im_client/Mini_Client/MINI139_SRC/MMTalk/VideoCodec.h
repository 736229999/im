/******************************************************************
** 文件名:	videoCompress.h
** 版  权:	深圳华亚和讯
** 创建人:	Tony
** 日  期:	2007-7-19 15:30
** 描  述:  使用xvid进行视频压缩解压类

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
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
	bool init(int width,int height);    //初始化xvid
	bool destory(); //销毁xvid
	//视频压缩函数
	bool compress(char* iBuf,const int& iSize,char* oBuf,int& oSize);
	//视频解压函数
	bool deCompress(char* iBuf,const int& iSize,char* oBuf,int& oSize);
protected:
	static int globalInit();  //全局初始化函数，使用xvid_gbl_init初始化
private:
	static xvid_gbl_init_t m_xvid_gbl_init;   //全局结构，每个进程初始化一次
	void *m_hEncoder,*m_hDecoder;  //压缩，解压指针
	xvid_enc_create_t m_enc_create;  //压缩函数使用
	xvid_dec_create_t m_dec_create;  //解压缩函数使用
	xvid_enc_frame_t  m_enc_frame;   //压缩帧结构
	xvid_dec_frame_t  m_dec_frame;   //解压帧结构
	static bool m_bInit;   //是否已经初始化过
	int m_height,m_width;  //视频的高度和宽度
};

#endif/*__VIDEO_ENCODER_DECODER__*/