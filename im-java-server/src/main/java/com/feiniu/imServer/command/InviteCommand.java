package com.feiniu.imServer.command;

import com.feiniu.imServer.bean.ImServer.ImServerEvent;
import com.feiniu.imServer.bean.KVPair;
import com.feiniu.imServer.constants.ImServerEventTypeEnum;

import java.util.List;

/**
 * Created by wangzhen on 2016/2/25.
 */
public class InviteCommand  extends BaseCommand{
    /*
       * 客服主动邀约申请
       * */
    public KVPair<Long,Long> handleServiceInviteRequestSync(ImServerEvent message) {

        if ( message.getServiceId()== null){
            LOGGER.error("参数不全，客服主动邀约 " );
            return null;
        }

        List<Long> groupIdList =  csInfoDao.getAllGroupsOfCsId(message.getServiceId().toString());
        //主动邀约增加全局的排队数据
        groupIdList.add(Long.valueOf(0));

        if (groupIdList == null)
            return null;
        for (Long groupIdIndex : groupIdList){
            Long customerId = shuntService.invite(groupIdIndex, message.getServiceId());

            if (customerId != null) {
                KVPair<Long,Long> result = new KVPair<>();
                result.setKey(customerId);
                result.setValue(groupIdIndex);
                return result;
            }
        }
        return null;
    }

    @Override
    public KVPair<Boolean, Object> execute(ImServerEvent message) {
        KVPair<Boolean,Object> resultPair = new KVPair<>();
        resultPair.setKey(true);
        resultPair.setValue(null);

        KVPair<Long,Long> result =handleServiceInviteRequestSync(message);
        if (resultPair == null){
            resultPair.setKey(false);
            resultPair.setValue("主动邀约失败");
        }else{
            resultPair.setValue(result);
        }
        return resultPair;
    }

    @Override
    public ImServerEventTypeEnum getEventType() {
        return ImServerEventTypeEnum.EVENT_INVITE_SESSION_REQUSET;
    }
}
