package com.feiniu.fnSubAccount.bean;

import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by wangzhen on 2015/10/16.
 */
public class ServiceGroupInfo {

    private String groupId;
    private String groupName;
    private String groupDescription;
    private String lastModifiedName;
    private String lastModifiedTime;
    private String embedSet;

    public String getGroupId() {
        return groupId;
    }

    public void setGroupId(String groupId) {
        this.groupId = groupId;
    }

    public String getGroupName() {
        return groupName;
    }

    public void setGroupName(String groupName) {
        this.groupName = groupName;
    }

    public String getGroupDescription() {
        return groupDescription;
    }

    public void setGroupDescription(String groupDescription) {
        this.groupDescription = groupDescription;
    }

    public String getLastModifiedName() {
        return lastModifiedName;
    }

    public void setLastModifiedName(String lastModifiedName) {
        this.lastModifiedName = lastModifiedName;
    }

    public String getLastModifiedTime() {
        return lastModifiedTime;
    }

    //public void setLastModifiedTime(String lastModifiedTime) {
       // this.lastModifiedTime = lastModifiedTime;
   // }
    public void setLastModifiedTime(String lastModifiedTime) {
        SimpleDateFormat format =   new SimpleDateFormat( "yyyy-MM-dd HH:mm:ss" );
        String time=lastModifiedTime;
        //Date date = format.parse(time);
        String a = format.format(new Date(Long.parseLong(lastModifiedTime)*1000));
//        System.out.print("Format To times:"+date.getTime());
        this.lastModifiedTime = a;

    }

    public String getEmbedSet() {
        return embedSet;
    }

    public void setEmbedSet(String embedSet) {
        this.embedSet = embedSet;
    }
}
