package com.feiniu.fnSubAccount.bean;



/**
 * Created by wanbingwen on 2015/10/20.
 */
public class CSGroupInfo {
        private String groupName;    //组名
        private String groupDescrib;  //组描述
        private String lastModifyName; //最后修改人
        private float lastModifyTime;  //最后修改时间

        public String getGroupName() {
                return groupName;
        }

        public void setGroupName(String groupName) {
                this.groupName = groupName;
        }

        public String getGroupDescrib() {
                return groupDescrib;
        }

        public void setGroupDescrib(String groupDescrib) {
                this.groupDescrib = groupDescrib;
        }

        public String getLastModifyName() {
                return lastModifyName;
        }

        public void setLastModifyName(String lastModifyName) {
                this.lastModifyName = lastModifyName;
        }

        public float getLastModifyTime() {
                return lastModifyTime;
        }

        public void setLastModifyTime(float lastModifyTime) {
                this.lastModifyTime = lastModifyTime;
        }
}
