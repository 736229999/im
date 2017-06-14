// THIS FILE WAS AUTOMATICALLY GENERATED.  DO NOT EDIT.
#ifndef LOGINTERFACE_SUBMERCHANTACTION_H
#define LOGINTERFACE_SUBMERCHANTACTION_H

#include "hessian/types.h"
#include "hessian/HessianInputStream.h"
#include "hessian/HessianOutputStream.h"

namespace LogInterface {

struct SubMerchantAction
{
    hessian::Int merchantID;
    hessian::Long subMerchantID;
    hessian::Int actionTime;
    hessian::Int actionType;
    
    SubMerchantAction():merchantID(0),subMerchantID(0),actionTime(0),actionType(0) {}
    
};

hessian::HessianInputStream& operator>>(hessian::HessianInputStream& in, SubMerchantAction& object);
hessian::HessianOutputStream& operator<<(hessian::HessianOutputStream& out, const SubMerchantAction& object);

}//namespace LogInterface
#endif
