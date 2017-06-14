#pragma  once

#include "BusinessExIMSrv.h"
#include "ClientProtocol.h"
#include "PublicType.h"

//群聊时文件（图片）数据上传
class CBusinessFileUpload : public CBusinessExIMSrv
{
public:
	CBusinessFileUpload(CRouterBusiness* pRouter);

	virtual int DoWork();

private:


};

//群聊时文件（图片）数据下载
class CBusinessFileDownload : public CBusinessExIMSrv
{
public:
	CBusinessFileDownload(CRouterBusiness* pRouter);

	virtual int DoWork();

private:


};

