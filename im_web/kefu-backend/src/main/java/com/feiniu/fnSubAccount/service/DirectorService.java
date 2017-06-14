package com.feiniu.fnSubAccount.service;

import com.feiniu.fnSubAccount.bean.SubMerchantInfo;
import com.feiniu.fnSubAccount.constants.BusinessConfig;
import com.feiniu.fnSubAccount.repository.CustomerServiceAuthDao;
import com.feiniu.fnSubAccount.service.external.BackendService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

/**
 * Created by wangzhen on 2015/10/12.
 */

@SuppressWarnings({ "unused","unchecked", "rawtypes" })
@Service
@Transactional
public class DirectorService {
    @Autowired
    private CustomerServiceAuthDao customerServiceAuthDao;

    @Autowired
    private BusinessConfig businessConfig;

    @Autowired
    private BackendService backendService;

    private static final Logger LOGGER = LoggerFactory.getLogger(DirectorService.class);
    /*
   * 判断id是否是主管
   * */
    public boolean isIdDirector(long directorId){
        return  customerServiceAuthDao.isIdDirector(directorId);
    }
    /*
    * 根据director id查找哞哞对应的id
    * null 代表没有对应的哞哞ID
    * */
    public Long convertIDToMoumouId(long directorId){
        return  customerServiceAuthDao.convertIDToMoumouId(directorId);
    }
    /*
    * 获取所有客服的信息
    * */
    public List<SubMerchantInfo> getAllSubMerchantInfoList(){
        return  customerServiceAuthDao.getAllSubMerchantInfoList();
    }
    /*
    * 根据主管ID获取客服id列表
    * */
    public List<SubMerchantInfo> getSubMerchantInfoListByDirectorId(){

        return  customerServiceAuthDao.getAllSubMerchantInfoList();
        //暂时不需要权限划分，，代码保留，备用
//        UserSession userSession = SystemEnv.getCurrentUser();
//        List<SubMerchantInfo> result = customerServiceAuthDao.getAllSubMerchantInfoList();
//
//        if (userSession == null)
//            return result;
//
//        long orgId = userSession.getOrgId();
//
//        String subMerchantIDlist = backendService.getAllMemberInOrgan(orgId);
//
//        if (null == subMerchantIDlist)
//            return result;
//
//        result = customerServiceAuthDao.getSubMerchantInfoListByIdList(subMerchantIDlist);
//        for (SubMerchantInfo index : result){
//            if (index.getTruename() != null)
//                index.setUsername(index.getUsername()+"("+index.getTruename()+")");
//            else
//                index.setUsername(index.getUsername()+"(--)");
//        }
//
//        return result;
    }
}
