// THIS FILE WAS AUTOMATICALLY GENERATED.  DO NOT EDIT.
#ifndef LOGINTERFACE_UPLOADSUBMERCHANTACTIONSERVICE_H
#define LOGINTERFACE_UPLOADSUBMERCHANTACTIONSERVICE_H

#include "LogInterface/ResultInfo.h"
#include "LogInterface/SubMerchantAction.h"
#include "hessian/types.h"

namespace LogInterface {

class UploadSubMerchantActionService
{
public:
    virtual LogInterface::ResultInfo uploadAction(const LogInterface::SubMerchantAction& param1) = 0;
    
    virtual ~UploadSubMerchantActionService ()
    { }
};

}//namespace LogInterface
#endif
