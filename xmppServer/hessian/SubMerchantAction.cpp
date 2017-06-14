// THIS FILE WAS AUTOMATICALLY GENERATED.  DO NOT EDIT.
#include "LogInterface/SubMerchantAction.h"

namespace LogInterface {

const std::string KEY_merchantID("merchantID");
const std::string KEY_subMerchantID("subMerchantID");
const std::string KEY_actionTime("actionTime");
const std::string KEY_actionType("actionType");

hessian::HessianInputStream&
operator>> (hessian::HessianInputStream& in, SubMerchantAction& object)
{
    if (in.beginObject()) {    
    	while (in.peek() != 'z') {
        	std::string key;
        	in >> key;
        
        	if (key == KEY_merchantID) {
        	    in >> object.merchantID;
        	} else if (key == KEY_subMerchantID) {
        	    in >> object.subMerchantID;
        	} else if (key == KEY_actionTime) {
        	    in >> object.actionTime;
        	} else if (key == KEY_actionType) {
        	    in >> object.actionType;
        	} else {
           	 in.throwUnknownPropertyException(key);
        	}
    	}
    	in.endObject();
    }
    return in;
}

hessian::HessianOutputStream&
operator<< (hessian::HessianOutputStream& out, const SubMerchantAction& object)
{
    static char TYPE_NAME[] = "com.cn.feiniu.logManagement.bean.web.hessian.req.SubMerchantAction";
    out.beginObject(TYPE_NAME, sizeof(TYPE_NAME) - 1);
    out << KEY_merchantID << object.merchantID;
    out << KEY_subMerchantID << object.subMerchantID;
    out << KEY_actionTime << object.actionTime;
    out << KEY_actionType << object.actionType;
    out.endObject();
    return out;
}

}//namespace LogInterface
