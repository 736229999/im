package com.feiniu.imServer.controller;

import com.feiniu.imServer.base.ResponseResult;
import com.feiniu.imServer.bean.backend.OrganNameInfo;
import com.feiniu.imServer.bean.friendList.FriendItem;
import com.feiniu.imServer.service.friendList.FriendListService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import java.util.List;

/**
 * Created by wangzhen on 2016/3/15.
 */
@Controller
@RequestMapping(value = "friend")
public class FriendListController {

    @Autowired
    FriendListService friendListService;


    @RequestMapping(value = "get-all-group-list")
    @ResponseBody
    public ResponseResult getAllGroupList(Long serviceId){
        ResponseResult result = new ResponseResult();
        List<OrganNameInfo> organNameInfos = friendListService.getAllGroupItem(serviceId);
        result.addData("organNameInfos",organNameInfos);
        result.markSuccess();
        return result;
    }

    @RequestMapping(value = "add-group")
    @ResponseBody
    public ResponseResult addGroup(Long serviceId , OrganNameInfo organNameInfo){
        ResponseResult result = new ResponseResult();
        Integer returnCode = friendListService.addFriendGroup(serviceId, organNameInfo);
        if(returnCode == 0){
            result.setMessage("添加好友分组成功");
            result.markSuccess();
        }else {
            result.setMessage("添加好友分组失败");
        }
        result.addData("returnCode" , returnCode);
        return result;
    }

    @RequestMapping(value = "delete-group")
    @ResponseBody
    public ResponseResult deleteGroup(Long serviceId , Long groupId){
        ResponseResult result = new ResponseResult();
        Integer resultCode = friendListService.deleteFriendGroup(serviceId , groupId);
        if(resultCode == 0){
            result.setMessage("删除好友分组成功");
            result.markSuccess();
        }else {
            result.setMessage("删除好友分组失败");
        }
        result.addData("resultCode",resultCode);
        return result;
    }

    @RequestMapping(value = "modify-group")
    @ResponseBody
    public ResponseResult modifyGroup(Long serviceId , Long groupId ,String groupName){
        ResponseResult result = new ResponseResult();
        Integer resultCode = friendListService.modifyFriendGroup(serviceId , groupId , groupName ) ;
        if(resultCode == 0){
            result.setMessage("修改好友分组成功");
            result.markSuccess();
        }else {
            result.setMessage("修改好友分组失败");
        }
        result.addData("resultCode",resultCode);
        return result;
    }


    @RequestMapping(value = "get-all-friend-list")
    @ResponseBody
    public ResponseResult getAllFriendList(Long serviceId,Long merchantId){
        ResponseResult result = new ResponseResult();
        List<FriendItem> friendItemList = friendListService.getAllFriendItem(serviceId, merchantId);
        result.addData("friendItemList",friendItemList);
        result.markSuccess();
        return result;
    }

    @RequestMapping(value = "add-friend")
    @ResponseBody
    public ResponseResult addFriend(Long serviceId,Long friendId,Long groupId){
        ResponseResult result = new ResponseResult();
        Integer resultCode = friendListService.addFriendItem(serviceId,friendId,groupId);
        if(resultCode == 0){
            result.setMessage("添加好友成功");
            result.markSuccess();
        }else {
           result.setMessage("添加好友失败");
        }
        result.addData("resultCode",resultCode);

        return result;
    }

    @RequestMapping(value = "delete-friend")
    @ResponseBody
    public ResponseResult deleteFriend(Long serviceId,Long friendId){
        ResponseResult result = new ResponseResult();
        Integer resultCode = friendListService.deleteFriendItem(serviceId, friendId);
        if(resultCode == 0){
            result.setMessage("删除好友成功");
            result.markSuccess();
        }else {
            result.setMessage("删除好友失败");
        }
        result.addData("resultCode",resultCode);
        return result;
    }

    @RequestMapping(value = "modify-friend")
    @ResponseBody
    public ResponseResult modifyFriend(Long serviceId, Long friendId , Long groupId){
        ResponseResult result = new ResponseResult();
        Integer resultCode = friendListService.modifyFriendItem(serviceId, friendId , groupId);
        if(resultCode == 0){
            result.setMessage("修改好友成功");
            result.markSuccess();
        }else{
            result.setMessage("修改好友失败");
        }
        result.addData("resultCode",resultCode);
        return result;
    }
}
