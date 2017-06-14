package com.feiniu.webim.bean.domain;

/**
 * Created by wangchen on 2014/12/4.
 */
public class MessageFeatures {
    private int color = 0;
    private byte line = (byte)0;
    private byte weight = (byte)0;
    private byte style = (byte)0;
    private String family = "Microsoft YaHei";
    private byte size = (byte)10;

    public int getColor() {
        return color;
    }

    public void setColor(int color) {
        this.color = color;
    }

    public byte getLine() {
        return line;
    }

    public void setLine(byte line) {
        this.line = line;
    }

    public byte getWeight() {
        return weight;
    }

    public void setWeight(byte weight) {
        this.weight = weight;
    }

    public byte getStyle() {
        return style;
    }

    public void setStyle(byte style) {
        this.style = style;
    }

    public String getFamily() {
        return family;
    }

    public void setFamily(String family) {
        this.family = family;
    }

    public byte getSize() {
        return size;
    }

    public void setSize(byte size) {
        this.size = size;
    }
}
