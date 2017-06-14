package com.feiniu.fnSubAccount.domain;

import com.feiniu.fnSubAccount.util.TimeStampSerializer;
import org.codehaus.jackson.map.annotate.JsonSerialize;
import org.hibernate.annotations.GenericGenerator;

import javax.persistence.*;
import java.sql.Timestamp;

/**
 * Created by zhangmin on 2015/10/22.
 */
@Table(name = "T_QUICK_PHRASE")
@Entity
public class QuickPhrase {

    @Id
    @GenericGenerator(name = "QuickPharseIdGenerator", strategy = "sequence",
            parameters = {@org.hibernate.annotations.Parameter(name = "sequence", value = "seq_quick_phrase")})
    @GeneratedValue(generator = "QuickPharseIdGenerator")

    @Column(name = "i_phrase_id")
    private int phraseId;

    @Column(name ="v_phrase_summary")
    private String phraseSummary;

    @Column(name = "v_phrase_group")
    private String phraseGroupName;

    @Column(name = "v_quick_code")
    private String quickCode;

    @Column(name = "v_last_operator")
    private String lastOperator;

    @Column(name = "i_font_size")
    private int fontSize;

    @Column(name = "i_font_color")
    private int fontColor;

    @Column(name = "i_font_style")
    private int fontStyle;

    @Column(name = "v_font_name")
    private String fontName;

    @Column(name = "update_time")
    private Timestamp updateTime;


    public int getPhraseId() {
        return phraseId;
    }

    public void setPhraseId(int phraseId) {
        this.phraseId = phraseId;
    }

    public String getPhraseSummary() {
        return phraseSummary;
    }

    public void setPhraseSummary(String phraseSummary) {
        this.phraseSummary = phraseSummary;
    }

    public String getPhraseGroupName() {
        return phraseGroupName;
    }

    public void setPhraseGroupName(String phraseGroupName) {
        this.phraseGroupName = phraseGroupName;
    }

    public String getQuickCode() {
        return quickCode;
    }

    public void setQuickCode(String quickCode) {
        this.quickCode = quickCode;
    }

    public String getLastOperator() {
        return lastOperator;
    }

    public void setLastOperator(String lastOperator) {
        this.lastOperator = lastOperator;
    }

    public int getFontSize() {
        return fontSize;
    }

    public void setFontSize(int fontSize) {
        this.fontSize = fontSize;
    }

    public int getFontColor() {
        return fontColor;
    }

    public void setFontColor(int fontColor) {
        this.fontColor = fontColor;
    }

    public int getFontStyle() {
        return fontStyle;
    }

    public void setFontStyle(int fontStyle) {
        this.fontStyle = fontStyle;
    }

    public String getFontName() {
        return fontName;
    }

    public void setFontName(String fontName) {
        this.fontName = fontName;
    }

    @JsonSerialize(using = TimeStampSerializer.class)
    public Timestamp getUpdateTime() {
        return updateTime;
    }

    public void setUpdateTime(Timestamp updateTime) {
        this.updateTime = updateTime;
    }
}
