package com.feiniu.imServer.domain;

import org.hibernate.annotations.GenericGenerator;

import javax.persistence.*;

/**
 * Created by wangchen on 2015/2/3.
 */
@Entity
@Table(name = "d_diverse_type")
public class DiverseType {
    @Id
    @Column(name = "d_diverse_type_id")
    private int diverseTypeID;

    @Column(name = "d_diverse_type_name", length = 20, nullable = false)
    private String diverseTypeName;

    @Column(name = "d_is_recommend", columnDefinition = "char(1)", nullable = false)
    private char isRecommend = '0';

    @Column(name = "d_diverse_description", length = 50, nullable = false)
    private String diverseDescription;

    public int getDiverseTypeID() {
        return diverseTypeID;
    }

    public void setDiverseTypeID(int diverseTypeID) {
        this.diverseTypeID = diverseTypeID;
    }

    public String getDiverseTypeName() {
        return diverseTypeName;
    }

    public void setDiverseTypeName(String diverseTypeName) {
        this.diverseTypeName = diverseTypeName;
    }

    public String getDiverseDescription() {
        return diverseDescription;
    }

    public void setDiverseDescription(String diverseDescription) {
        this.diverseDescription = diverseDescription;
    }

    public char getIsRecommend() {
        return isRecommend;
    }

    public void setIsRecommend(char isRecommend) {
        this.isRecommend = isRecommend;
    }
}
