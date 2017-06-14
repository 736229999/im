package com.feiniu.imServer.component;


import com.feiniu.imServer.domain.ServiceGroupMapping;

import java.util.List;

/**
 * Created by wangchen on 2015/2/3.
 */
public interface ServiceGroupMappingManager {


    public List<ServiceGroupMapping> getSubAccountListUnderGroup(long groupID);

    public List<ServiceGroupMapping> getCategorizedSubAccounts(long merchantID);


}
