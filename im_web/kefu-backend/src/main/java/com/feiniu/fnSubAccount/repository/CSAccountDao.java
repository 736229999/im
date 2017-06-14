package com.feiniu.fnSubAccount.repository;

import com.feiniu.fnSubAccount.bean.CSAccountInfo;
import com.feiniu.fnSubAccount.bean.CSAccountResponse;
import com.feiniu.fnSubAccount.bean.KVPair;

import java.util.List;

/**
 * Created by wangzhen on 2015/11/19.
 */
public interface CSAccountDao {
    Integer searchCsAccountInfoCount(String moumouName,String csIdList,String improtCsIdList,
                                                int status,long startTime,long endTime);

    List<CSAccountResponse> searchCsAccountInfo(String moumouName,String csIdList,String improtCsIdList,
                                                       int status,long startTime,long endTime,int fromRowIndex ,int toRowIndex);

    List<CSAccountResponse> searchCsAccountInfoByTime(long importTime,String importName);

    Boolean insertToCSInfo(String moumouUserName,int permission,String importName,
                               long importTime,int receptionLimit,int status,long importCsId);

    boolean setStatus(String ids,int status);

    boolean deleteAccount(String ids);

    boolean setDetail(long moumouId,long csId,String csName,int limitNumber,int issupervisor);

    boolean setPassword(long moumouId,String password);

    List<CSAccountInfo> getAccountIdList(int startRow,int maxReuslt);

    Integer getAccountIdListCount();

    List<KVPair<Long,String>> getMoumouNameByPartName(String partName);

    public Long getUserIDInMouMou(Long userIDInBackend);
}
