// ----------------------------------------------------------------
// 文件:VideoCap.h
// 版本: 
// 作者: xy
// 日期: 2007-5-15
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
// 
// ----------------------------------------------------------------


#pragma once
#include <Vfw.h>
#pragma comment(lib,"vfw32.lib")
#include "VideoCodec.h"

#include "../im/P2PFile.h "

typedef  void (*VideoCapDataFnc)(BYTE * pCmpFrame,int nLen,int nTime,DWORD dwUser);


//------------------------------------------------------
// 视频编码解码器
//------------------------------------------------------

class CMMVideoCodec
{
public:

	CMMVideoCodec();

	~CMMVideoCodec();

	static const DWORD DEFAULT_CODEC = mmioFOURCC('I','V','5','0');

public:

	//------------------------------------------------------
	// 初始化压缩解码器
	//------------------------------------------------------
	BOOL			Init(BITMAPINFO * pRawFormat,double dbQuality = 50,DWORD fcc = DEFAULT_CODEC);

	//------------------------------------------------------
	// 设置压缩质量0~100
	//------------------------------------------------------
	void			SetCmpQuality(double fQuality);

	//------------------------------------------------------
	// 压缩视频帧，要求源数据为摄像设备当前使用的格式
	//------------------------------------------------------
	BYTE *			CmpFrame(/*IN*/BYTE * pData,/*OUT*/int & nSize);

	//------------------------------------------------------
	// 解压视频帧，要求待加压数据为摄像设备当前压缩格式
	//------------------------------------------------------
	BYTE *			DeCmpFrame(/*IN*/BYTE * pData,int nLen);

	//------------------------------------------------------
	// 关闭视频编码解码器
	//------------------------------------------------------
	void			Close();

protected:

	Cxvid			m_xvid;

	COMPVARS		m_CV;				// 压缩参数

	BYTE *			m_pBmpDeCmp;		// 解压缓冲

	BYTE *			m_pCmpData;			// 压缩缓冲

	BITMAPINFO *	m_pBmpInfoRaw;		// 未压缩的数据格式

	BITMAPINFO *	m_pBmpInfoCmp;		// 压缩后的图形格式

	BOOL			m_bInit;			// 是否初始化成功

	CSyncCritical	m_sc;

};


//------------------------------------------------------
// 视频采集设备
//------------------------------------------------------

class CMMVideoCap
{
public:
	
	CMMVideoCap();
	
	~CMMVideoCap();

public:

	//------------------------------------------------------
	// 初始化视频捕捉
	//------------------------------------------------------
	BOOL			InitCap(const CRect & rcCapWnd,HWND hWndParent,int nCapIdx);

	//------------------------------------------------------
	// 处理源视频数据
	//------------------------------------------------------
	void			ProcRawData(LPVIDEOHDR pVHdr);

	//------------------------------------------------------
	// 设置数据捕获回调 
	//------------------------------------------------------
	void			SetCallBackOnData(VideoCapDataFnc pFnc,DWORD dwUser);
	
	//------------------------------------------------------
	// 压缩视频帧，要求源数据为摄像设备当前使用的格式
	//------------------------------------------------------
	//BYTE *			CmpFrame(/*IN*/BYTE * pData,/*OUT*/int & nSize);

	//------------------------------------------------------
	// 解压视频帧，要求待加压数据为摄像设备当前压缩格式
	//------------------------------------------------------
	//BYTE *			DeCmpFrame(/*IN*/BYTE * pData);

	//------------------------------------------------------
	// 获取图片格式 
	//------------------------------------------------------
	BITMAPINFO *	GetBmpInfo();

	//------------------------------------------------------
	// 设置视频每秒采集帧数
	//------------------------------------------------------
	//BOOL			SetFramePerSec(DWORD nC);

	//------------------------------------------------------
	// 设置压缩质量0~100
	//------------------------------------------------------
	//void			SetCmpQuality(double fQuality);

	//------------------------------------------------------
	// 显示视频格式设置对话框
	//------------------------------------------------------
	void			ShowVideoFormatDlg();

	//------------------------------------------------------
	// 关闭视频采集
	//------------------------------------------------------
	void			Close();


	//------------------------------------------------------
	// 预览
	//------------------------------------------------------
	void			Preview();

	//------------------------------------------------------
	// 启动视频采集
	//------------------------------------------------------
	void			Start(int nFramePerSec =3 );


protected:

	//------------------------------------------------------
	// 将非标准格式转化为标准输出格式
	//------------------------------------------------------
	void			Convert2RGB24(BITMAPINFO * pBmi,BYTE * pIn,BYTE * pOut);



private:

	//------------------------------------------------------
	// 转化平面YUV格式
	//------------------------------------------------------
	void			ConvertPlanarYUV12(BITMAPINFO * pBmi,BYTE * pIn,BYTE * pOut,bool bUV);

	//------------------------------------------------------
	// 转化紧凑YUV格式
	//------------------------------------------------------
	void			ConvertPackedYUV16(BITMAPINFO * pBmi,BYTE * pIn,BYTE * pOut);

protected:

	HWND			m_hWndCap;			// 捕获窗的句柄 

	CAPDRIVERCAPS	m_capDriverCaps ;	// 视频驱动器的能力 

	CAPSTATUS		m_capStatus ;		// 捕获窗的状态 

	BITMAPINFO *	m_pBmpInfoSrc;		// 源数据格式

	BITMAPINFO *	m_pBmpInfoCmp;		// 压缩图形格式

	BITMAPINFO		m_bmpInfoRGB24;		// 标准输出格式

	BYTE *			m_pBmpRGB24;		// 标准输出数据
	
	VideoCapDataFnc	m_pCallBackData;	// 数据回调通知函数

	DWORD			m_dwUser;			// 用户数据

	int				m_nTimeSpan;		// 画面采集的间隔时间

	int				m_nFramePerSec;		// 每秒采集的帧数

	int				m_nTimeLeft;

	int				m_nLastTick;

	//CMMVideoCodec	m_codec;
	//COMPVARS		m_CV;				// 压缩参数
	//BYTE *			m_pBmpDeCmp;		// 解压数据
};




