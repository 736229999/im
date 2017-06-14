package com.feiniu.subAccount.domain;

import org.hibernate.annotations.GenericGenerator;

import javax.persistence.*;

/**
 * Created by wangchen on 2015/2/3.
 */
@Entity
@Table(name = "t_group_show_mapping")
public class MappingForGroupServicePlace {
    @Id
    @GenericGenerator(name = "groupShowMappingIdGenerator", strategy = "sequence",
            parameters = {@org.hibernate.annotations.Parameter(name = "sequence", value = "seq_group_show_mapping")})
    @GeneratedValue(generator = "groupShowMappingIdGenerator")
    @Column(name = "g_mapping_id")
    private long mappingID;

    @ManyToOne
    @JoinColumn(name = "g_group_id", nullable = false)
    private ServiceGroup serviceGroup;

    @OneToOne
    @JoinColumn(name = "g_config_id")
    private GroupServicePlaceConfig groupServicePlaceConfig;

    public long getMappingID() {
        return mappingID;
    }

    public void setMappingID(long mappingID) {
        this.mappingID = mappingID;
    }

    public ServiceGroup getServiceGroup() {
        return serviceGroup;
    }

    public void setServiceGroup(ServiceGroup serviceGroup) {
        this.serviceGroup = serviceGroup;
    }

    public GroupServicePlaceConfig getGroupServicePlaceConfig() {
        return groupServicePlaceConfig;
    }

    public void setGroupServicePlaceConfig(GroupServicePlaceConfig groupServicePlaceConfig) {
        this.groupServicePlaceConfig = groupServicePlaceConfig;
    }
}
