// THIS FILE WAS AUTOMATICALLY GENERATED.  DO NOT EDIT.
#include "LogInterface/ResultInfo.h"
#include "LogInterface/SubMerchantAction.h"
#include "LogInterface/UploadSubMerchantActionServiceProxy.h"
#include "hessian/Call.h"

namespace LogInterface {
    
LogInterface::ResultInfo
UploadSubMerchantActionServiceProxy::uploadAction (const LogInterface::SubMerchantAction& param1)
{
    static char METHOD_NAME[] = "uploadAction";
    hessian::Call<LogInterface::ResultInfo > call(METHOD_NAME, sizeof(METHOD_NAME) - 1);
    call << param1;
    return call.invoke(connection_);
}

}//namespace LogInterface
