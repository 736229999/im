package com.feiniu.imServer.service.userinfo;

import com.feiniu.imServer.bean.base.UserInfoFull;
import com.feiniu.imServer.bean.base.UserInfoInDB;
import com.feiniu.imServer.consts.ShuntConfig;
import com.feiniu.imServer.repository.UserInfoDao;
import com.feiniu.imServer.service.base.CacheClientService;
import com.fn.cache.client.RedisCacheClient;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.transaction.annotation.Transactional;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by wangzhen on 2016/4/5.
 */
public class UserInfoService {
    @Autowired
    UserInfoDao userInfoDao;
    @Autowired
    RedisCacheClient redisCacheClient;
    @Autowired
    CacheClientService cacheClientService;
    private static final Logger LOGGER = LoggerFactory.getLogger(UserInfoService.class);

    private static Map<String,String> imUserFeildMap = new HashMap<>();
    private static Map<String,String> imUserInfoFeildMap = new HashMap<>();
    private static Map<String,String> imUserHotInfo = new HashMap<>();
    static {
        imUserFeildMap.put("USERNAME","username");
        imUserFeildMap.put("GENDER","Gender");
        imUserFeildMap.put("EMAIL","EMAIL");
        imUserFeildMap.put("MOBILE","MOBILE");
        imUserFeildMap.put("COMPANY","Company");

        imUserInfoFeildMap.put("NICKNAME","nickname");
        imUserInfoFeildMap.put("USERSIGN","usersign");
        imUserInfoFeildMap.put("USERPICURI","USERPICURI");
        imUserInfoFeildMap.put("CITY","CITY");
        imUserInfoFeildMap.put("NATION","NATION");
        imUserInfoFeildMap.put("PROVINCE","PROVINCE");
        imUserInfoFeildMap.put("JOINAUTHFLAG","JOINAUTHFLAG");
        imUserInfoFeildMap.put("USERDESC","USERDESC");
        imUserInfoFeildMap.put("BIRTHDAY","BIRTHDAY");
        imUserInfoFeildMap.put("JOIN2FAMILY","JOIN2FAMILY");
        imUserInfoFeildMap.put("USRADDRESS","USRADDRESS");
        imUserInfoFeildMap.put("LINKPOP","LINKPOP");
        imUserInfoFeildMap.put("INTEGRAL","Integral");
        imUserInfoFeildMap.put("CAREER","CAREER");
        imUserInfoFeildMap.put("GROUPNAME","GROUPNAME");

        imUserHotInfo.put("isMornitored","isMornitored");//
        imUserHotInfo.put("chattingBuyerID","chattingBuyerID");
        imUserHotInfo.put("directorID","directorID");//
        imUserHotInfo.put("merchantID","merchantID");
        imUserHotInfo.put("status","status");	//
        imUserHotInfo.put("usertype","usertype");
        imUserHotInfo.put("csUserID","csUserID");
        imUserHotInfo.put("csTeamID","csTeamID");
        imUserHotInfo.put("loginType","loginType");
        imUserHotInfo.put("lanPort","lanPort");
        imUserHotInfo.put("u_GUID","u_GUID");
        imUserHotInfo.put("onlineTime","onlineTime");
        imUserHotInfo.put("serverip","serverip");
        imUserHotInfo.put("lanip","lanip");
        imUserHotInfo.put("flag","flag");
        imUserHotInfo.put("logintime","logintime");
        imUserHotInfo.put("sessionkey","sessionkey");
        imUserHotInfo.put("p2psessionkey","p2psessionkey");
        imUserHotInfo.put("flow","flow");
        imUserHotInfo.put("id","id");
        imUserHotInfo.put("version","version");
        imUserHotInfo.put("clientip","clientip");
        imUserHotInfo.put("clientport","clientport");
    }


    private UserInfoFull convertUserInfoInDBToUserInfoFull(UserInfoInDB userInfoInDB){
        UserInfoFull userInfoFull = new UserInfoFull();
        userInfoFull.setAddress(userInfoInDB.getT_useraddress());
        userInfoFull.setBirthday(userInfoInDB.getT_birthday());
        userInfoFull.setCareer(userInfoInDB.getT_career());
        userInfoFull.setCity(userInfoInDB.getT_city());
        userInfoFull.setCountry(userInfoInDB.getT_nation());
        userInfoFull.setDescription(userInfoInDB.getT_userdesc());
        userInfoFull.seteMail(userInfoInDB.getT_email());
        userInfoFull.setFamilyFlag(userInfoInDB.getT_join2family());
        userInfoFull.setGroupname(userInfoInDB.getT_groupname());
        userInfoFull.setHeadURL(userInfoInDB.getT_userpicuri());
        userInfoFull.setId(userInfoInDB.getT_userid());
        userInfoFull.setIntegral(userInfoInDB.getT_integral());
        userInfoFull.setUsername(userInfoInDB.getT_username());
        userInfoFull.setUserSign(userInfoInDB.getT_usersign());
        userInfoFull.setSexy(userInfoInDB.getT_usersex());
        userInfoFull.setMobileStatus(userInfoInDB.getT_mobilestatus());
        userInfoFull.setLinkpop(userInfoInDB.getT_linkpop());
        userInfoFull.setNickname(userInfoInDB.getT_nickname());
        userInfoFull.setPhone(userInfoInDB.getT_mphone());
        userInfoFull.setProvince(userInfoInDB.getT_province());

        //应该是数据库里面查到的，但是存储过程没有
        userInfoFull.setIsFullInfoValid(Integer.valueOf(0));
        userInfoFull.setReceptionLimit(Integer.valueOf(0));
        userInfoFull.setRule(Integer.valueOf(0));
        userInfoFull.setLevel(Integer.valueOf(2));

        //动态字段
        userInfoFull.setU_GUID("");
        userInfoFull.setSessionkey("");
        userInfoFull.setP2psessionkey("");
        userInfoFull.setIsMornitored(Integer.valueOf(0));
        userInfoFull.setLanip(Long.valueOf(0));
        userInfoFull.setLogintime(Long.valueOf(0));
        userInfoFull.setLoginType(Integer.valueOf(0));
        userInfoFull.setMerchantID(Long.valueOf(0));
        userInfoFull.setMoney(Integer.valueOf(0));
        userInfoFull.setNewlyLiveTime(Long.valueOf(0));
        userInfoFull.setOnlineTime(Long.valueOf(0));
        userInfoFull.setFlag(Integer.valueOf(0));
        userInfoFull.setFlow(Integer.valueOf(0));
        userInfoFull.setDirectorID(Long.valueOf(0));
        userInfoFull.setServerip(Long.valueOf(0));
        userInfoFull.setStatus(Integer.valueOf(-1));
        userInfoFull.setUsertype(Integer.valueOf(0));
        userInfoFull.setVersion(Integer.valueOf(0));
        userInfoFull.setChattingBuyerID(Long.valueOf(0));
        userInfoFull.setClientip(Long.valueOf(0));
        userInfoFull.setClientport(Long.valueOf(0));
        userInfoFull.setCsTeamID(Long.valueOf(0));
        userInfoFull.setCsUserID(Long.valueOf(0));
        return userInfoFull;
    }

    @Transactional
    public boolean modifyUserInfoFull(Long userId, String feilds,String objects){
        String userInfoKey = ShuntConfig.MEMCACHE_USER_INFO + "#test#"+ userId.toString();
        String[] feildList = feilds.split(",");
        String[] valueList = objects.split(",");
        boolean isSuccess= true;
        if (feildList.length != valueList.length)
            return false;
        
        for (int i = 0 ;i < feildList.length ;i++){
            if (imUserFeildMap.get(feildList[i]) != null){
                userInfoDao.updateImUserFeild(userId,feildList[i],valueList[i]);
                if (redisCacheClient.exists(userInfoKey))
                    redisCacheClient.hput(userInfoKey,feildList[i],valueList[i]);
            }else if(imUserFeildMap.get(feildList[i]) != null){
                userInfoDao.updateImUserInfoFeild(userId,feildList[i],valueList[i]);
                if (redisCacheClient.exists(userInfoKey))
                    redisCacheClient.hput(userInfoKey,feildList[i],valueList[i]);
            }else if (imUserInfoFeildMap.get(feildList[i]) !=null){
                if (redisCacheClient.exists(userInfoKey))
                    redisCacheClient.hput(userInfoKey,feildList[i],valueList[i]);
            }else{
                isSuccess =false;
            }
        }
        return isSuccess;
    }

    public UserInfoFull getUserInfoFull(Long userId){
        String userInfoKey = ShuntConfig.MEMCACHE_USER_INFO + "#test#"+ userId.toString();
        try {
            UserInfoFull userInfoFull = cacheClientService.getHValue(userInfoKey,UserInfoFull.class);
            if (userInfoFull != null)
                redisCacheClient.expire(userInfoKey,ShuntConfig.USER_INFO_TIMEOUT);
            return  userInfoFull;
        }catch (Exception e){
            LOGGER.error("从memcache中获取userinfo失败");
            e.printStackTrace();
        }

        UserInfoInDB userInfoInDB = userInfoDao.getUserInfoInDB(userId);
        if (userInfoInDB == null)
            return null;

        UserInfoFull userInfoFull = convertUserInfoInDBToUserInfoFull(userInfoInDB);

        try {
            cacheClientService.putHValue(userInfoKey,userInfoFull);
        }catch (Exception e){
            LOGGER.error("刷新userinfo到redis失败");
            e.printStackTrace();
        }
        return  userInfoFull;
    }
}
