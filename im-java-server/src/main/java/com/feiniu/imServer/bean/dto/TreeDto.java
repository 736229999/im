package com.feiniu.imServer.bean.dto;

/**
 * Created by wangzhen on 2016/3/16.
 */

import java.io.Serializable;
import java.util.List;
import java.util.Map;

public class TreeDto implements Serializable {
    private static final long serialVersionUID = 1L;
    private Long id;
    private String text;
    private String state;
    private Boolean checked;
    private List<TreeDto> children;
    private String parentId;
    private Map<String, String> attributes;

    public TreeDto() {
    }

    public Map<String, String> getAttributes() {
        return this.attributes;
    }

    public void setAttributes(Map<String, String> attributes) {
        this.attributes = attributes;
    }

    public Long getId() {
        return this.id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getText() {
        return this.text;
    }

    public void setText(String text) {
        this.text = text;
    }

    public String getState() {
        return this.state;
    }

    public void setState(String state) {
        this.state = state;
    }

    public Boolean getChecked() {
        return this.checked;
    }

    public void setChecked(Boolean checked) {
        this.checked = checked;
    }

    public List<TreeDto> getChildren() {
        return this.children;
    }

    public void setChildren(List<TreeDto> children) {
        this.children = children;
    }

    public String getParentId() {
        return this.parentId;
    }

    public void setParentId(String parentId) {
        this.parentId = parentId;
    }
}