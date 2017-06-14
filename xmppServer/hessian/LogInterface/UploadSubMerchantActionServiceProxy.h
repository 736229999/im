// THIS FILE WAS AUTOMATICALLY GENERATED.  DO NOT EDIT.
#ifndef LOGINTERFACE_UPLOADSUBMERCHANTACTIONSERVICEPROXY_H
#define LOGINTERFACE_UPLOADSUBMERCHANTACTIONSERVICEPROXY_H

#include "LogInterface/ResultInfo.h"
#include "LogInterface/SubMerchantAction.h"
#include "LogInterface/UploadSubMerchantActionService.h"
#include "hessian/types.h"
#include "hessian/Connection.h"

namespace LogInterface {

class UploadSubMerchantActionServiceProxy: public UploadSubMerchantActionService
{
protected:
    hessian::Connection& connection_;
    
public:
    UploadSubMerchantActionServiceProxy (hessian::Connection& connection):
        connection_(connection)
    { }
    
    LogInterface::ResultInfo uploadAction(const LogInterface::SubMerchantAction& param1);
};

}//namespace LogInterface
#endif
