package com.feiniu.subAccount.domain;

import org.hibernate.annotations.GenericGenerator;

import javax.persistence.*;

/**
 * Created by wangchen on 2015/2/3.
 */
@Entity
@Table(name = "d_show_group_config")
public class GroupServicePlaceConfig {
    @Id
    @GenericGenerator(name = "groupShowConfigGenerator", strategy = "sequence",
            parameters = {@org.hibernate.annotations.Parameter(name = "sequence", value = "seq_show_group_config")})
    @GeneratedValue(generator = "groupShowConfigGenerator")
    @Column(name = "d_config_id")
    private long configID;

    @Column(name = "d_config_name", nullable = false)
    private String configName;

    public long getConfigID() {
        return configID;
    }

    public void setConfigID(long configID) {
        this.configID = configID;
    }

    public String getConfigName() {
        return configName;
    }

    public void setConfigName(String configName) {
        this.configName = configName;
    }
}
