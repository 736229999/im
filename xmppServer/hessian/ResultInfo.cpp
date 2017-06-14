// THIS FILE WAS AUTOMATICALLY GENERATED.  DO NOT EDIT.
#include "LogInterface/ResultInfo.h"

namespace LogInterface {

const std::string KEY_success("success");
const std::string KEY_errorMessage("errorMessage");

hessian::HessianInputStream&
operator>> (hessian::HessianInputStream& in, ResultInfo& object)
{
    if (in.beginObject()) {    
    	while (in.peek() != 'z') {
        	std::string key;
        	in >> key;
        
        	if (key == KEY_success) {
        	    in >> object.success;
        	} else if (key == KEY_errorMessage) {
        	    in >> object.errorMessage;
        	} else {
           	 in.throwUnknownPropertyException(key);
        	}
    	}
    	in.endObject();
    }
    return in;
}

hessian::HessianOutputStream&
operator<< (hessian::HessianOutputStream& out, const ResultInfo& object)
{
    static char TYPE_NAME[] = "com.cn.feiniu.logManagement.bean.base.ResultInfo";
    out.beginObject(TYPE_NAME, sizeof(TYPE_NAME) - 1);
    out << KEY_success << object.success;
    out << KEY_errorMessage << object.errorMessage;
    out.endObject();
    return out;
}

}//namespace LogInterface
