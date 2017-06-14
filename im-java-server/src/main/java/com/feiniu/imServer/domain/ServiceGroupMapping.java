package com.feiniu.imServer.domain;

import org.codehaus.jackson.annotate.JsonIgnore;
import org.hibernate.annotations.GenericGenerator;

import javax.persistence.*;

/**
 * Created by wangchen on 2015/2/3.
 */
@Entity
@Table(name = "t_service_group_mapping")
public class ServiceGroupMapping {
    @Id
    @Column(name = "s_mapping_id")
    private long mappingID;

    @Column(name = "s_subaccount_id", nullable = false, unique = true)
    private long subAccountID;

    @Column(name = "s_diverse_state", columnDefinition = "char(1)", nullable = false)
    private char diverseState = '0';

    @ManyToOne
    @JoinColumn(name = "s_group_id", nullable = false)
    private ServiceGroup serviceGroup;

    @JsonIgnore
    public long getMappingID() {
        return mappingID;
    }

    public void setMappingID(long mappingID) {
        this.mappingID = mappingID;
    }

    public char getDiverseState() {
        return diverseState;
    }

    public void setDiverseState(char diverseState) {
        this.diverseState = diverseState;
    }

    public ServiceGroup getServiceGroup() {
        return serviceGroup;
    }

    public void setServiceGroup(ServiceGroup serviceGroup) {
        this.serviceGroup = serviceGroup;
    }

    public long getSubAccountID() {
        return subAccountID;
    }

    public void setSubAccountID(long subAccountID) {
        this.subAccountID = subAccountID;
    }
}
