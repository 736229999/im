package com.feiniu.csWebIm.bean.domain;

/**
 * Created by wangzhen on 2015/10/21.
 */
public class AskingOpt {

    private String text ;
    private boolean checked ;
    private String id;
    private String realID;//保存到台北ID

    public String getText() {
        return text;
    }

    public void setText(String text) {
        this.text = text;
    }

    public boolean isChecked() {
        return checked;
    }

    public void setChecked(boolean checked) {
        this.checked = checked;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getRealID() {
        return realID;
    }

    public void setRealID(String realID) {
        this.realID = realID;
    }
}
