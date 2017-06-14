/******************************************************************
** 文件名:	videoCompress.cpp
** 版  权:	深圳华亚和讯
** 创建人:	Tony
** 日  期:	2007-7-19 15:30
** 描  述:  使用xvid进行视频压缩解压类

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
*******************************************************************/
#include "stdafx.h"
#include "videocodec.h"


static const int motion_presets[] = {
		 /* quality 0 */
		0,     
         /* quality 1 */
         XVID_ME_ADVANCEDDIAMOND16,
         
         /* quality 2 */
         XVID_ME_ADVANCEDDIAMOND16 | XVID_ME_HALFPELREFINE16,
         
         /* quality 3 */
         XVID_ME_ADVANCEDDIAMOND16 | XVID_ME_HALFPELREFINE16 |
         XVID_ME_ADVANCEDDIAMOND8 | XVID_ME_HALFPELREFINE8,
         
         /* quality 4 */
         XVID_ME_ADVANCEDDIAMOND16 | XVID_ME_HALFPELREFINE16 |
         XVID_ME_ADVANCEDDIAMOND8 | XVID_ME_HALFPELREFINE8 |
         XVID_ME_CHROMA_PVOP | XVID_ME_CHROMA_BVOP,
         
         /* quality 5 */
         XVID_ME_ADVANCEDDIAMOND16 | XVID_ME_HALFPELREFINE16 |
         XVID_ME_ADVANCEDDIAMOND8 | XVID_ME_HALFPELREFINE8 |
         XVID_ME_CHROMA_PVOP | XVID_ME_CHROMA_BVOP,
         
         /* quality 6 */
         XVID_ME_ADVANCEDDIAMOND16 | XVID_ME_HALFPELREFINE16 | XVID_ME_EXTSEARCH16 |
         XVID_ME_ADVANCEDDIAMOND8 | XVID_ME_HALFPELREFINE8 | XVID_ME_EXTSEARCH8 |
         XVID_ME_CHROMA_PVOP | XVID_ME_CHROMA_BVOP,
         
 };


 bool Cxvid::m_bInit = false;
 
 xvid_gbl_init_t Cxvid::m_xvid_gbl_init;
 
 Cxvid::Cxvid()
 {
	 m_hDecoder = NULL;
	 m_hEncoder = NULL;
	 m_width = 320;
     m_height = 240;
 }

 Cxvid::~Cxvid()
 {

 }
bool Cxvid::init(int width,int height)
{
	globalInit();
	m_width = width;
	m_height = height;
	//创建压缩函数指针
	memset(&m_enc_create,0,sizeof(m_enc_create));
	m_enc_create.version = XVID_VERSION;
	m_enc_create.width = m_width;
	m_enc_create.height = m_height;
	m_enc_create.profile = XVID_PROFILE_ARTS_L1;//XVID_PROFILE_AS_L4;


    /* Frame rate - Do some quick float fps = fincr/fbase hack */    
    m_enc_create.fincr = 1;
    m_enc_create.fbase = (int) 10;
    /* Maximum key frame interval */
    m_enc_create.max_key_interval = (int)-1;    //--default 10s
    ///* Bframes settings */
    //m_enc_create.max_bframes = 0;
    //m_enc_create.bquant_ratio = 150;
    //m_enc_create.bquant_offset = 100;

    ///* Dropping ratio frame -- we don't need that */
    //m_enc_create.frame_drop_ratio = 0;
    ///* Global encoder options */
    //m_enc_create.global = 0;





	if(xvid_encore(NULL, XVID_ENC_CREATE, &m_enc_create,NULL) != 0)
	{
		printf("Create encore handle error!\n");
		return false;
	}
	m_hEncoder = m_enc_create.handle;    //取出handle指针 
	//创建解压函数指针
	memset(&m_dec_create,0,sizeof(m_dec_create));
	m_dec_create.version = XVID_VERSION;
	//高度和宽度，解压函数会自动调整
	//m_dec_create.width =  320;//320;
	//m_dec_create.height = 240;//240;
	
	if(xvid_decore(NULL,XVID_DEC_CREATE,&m_dec_create,NULL) != 0)
	{
		printf("Create decore handle error!\n");
		return false;
	}
	m_hDecoder = m_dec_create.handle;
	return true;
}

bool Cxvid::destory()
{//待完善
	m_bInit = false;
	if(m_hEncoder)
	{
		xvid_encore(m_hEncoder,XVID_ENC_DESTROY,NULL,NULL);
		m_hEncoder = NULL;
	}
	if(m_hDecoder)
	{
		xvid_decore(m_hDecoder,XVID_DEC_DESTROY,NULL,NULL);
		m_hDecoder = NULL;
	}
	return false;
}

int Cxvid::globalInit()
{
	if(m_bInit)  //只调用一次
		return 0;
	else
		m_bInit = true;

	memset(&m_xvid_gbl_init,0,sizeof(m_xvid_gbl_init));
	m_xvid_gbl_init.version = XVID_VERSION;
	m_xvid_gbl_init.debug = NULL;
	m_xvid_gbl_init.cpu_flags = XVID_CPU_FORCE;
	return xvid_global(NULL,XVID_GBL_INIT,&m_xvid_gbl_init,NULL);
}

bool Cxvid::compress(char* iBuf,const int& iSize,char* oBuf,int& oSize)
{
	memset(&m_enc_frame,0,sizeof(m_enc_frame));
	m_enc_frame.version = XVID_VERSION;
	m_enc_frame.vol_flags = XVID_VOL_MPEGQUANT;
	m_enc_frame.vop_flags = 0;//XVID_VOP_DEBUG;
	m_enc_frame.quant = 8;
	// 初始化输出缓冲
	m_enc_frame.bitstream = oBuf; //输出缓冲区
	m_enc_frame.length = -1;
	// 初始化输入图像
	m_enc_frame.type = XVID_TYPE_AUTO;
	m_enc_frame.input.csp = XVID_CSP_BGR;  //RGB24位色
	m_enc_frame.input.plane[0] = iBuf;    //待压缩图片缓冲区
	m_enc_frame.input.stride[0]   = m_width * 3; 
	
	m_enc_frame.motion = motion_presets[2];
	

	oSize = xvid_encore(m_hEncoder,XVID_ENC_ENCODE,&m_enc_frame,NULL);
	return oSize <= 0?false:true;
}
bool Cxvid::deCompress(char* iBuf,const int& iSize,char* oBuf,int& oSize)
{
	memset(&m_dec_frame,0,sizeof(m_dec_frame));
	m_dec_frame.version = XVID_VERSION;
	m_dec_frame.bitstream = iBuf;
	m_dec_frame.length = iSize;
	m_dec_frame.output.plane[0] = oBuf;
	m_dec_frame.output.stride[0] = m_width * 3;
	m_dec_frame.output.csp = XVID_CSP_BGR;
	oSize = xvid_decore(m_hDecoder,XVID_DEC_DECODE,&m_dec_frame,NULL);
	return oSize <= 0?false:true;
}