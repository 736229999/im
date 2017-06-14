// THIS FILE WAS AUTOMATICALLY GENERATED.  DO NOT EDIT.
#ifndef LOGINTERFACE_RESULTINFO_H
#define LOGINTERFACE_RESULTINFO_H

#include "hessian/types.h"
#include <string>
#include "hessian/HessianInputStream.h"
#include "hessian/HessianOutputStream.h"

namespace LogInterface {

struct ResultInfo
{
    bool success;
    std::string errorMessage;
    
    ResultInfo() {}
    
};

hessian::HessianInputStream& operator>>(hessian::HessianInputStream& in, ResultInfo& object);
hessian::HessianOutputStream& operator<<(hessian::HessianOutputStream& out, const ResultInfo& object);

}//namespace LogInterface
#endif
