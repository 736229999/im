package com.feiniu.webim.core;

import com.feiniu.webim.bean.response.BaseResponseBean;
import com.feiniu.webim.bean.response.HistoryMessageListResponseBean;
import com.feiniu.webim.constants.Constants;
import com.feiniu.webim.util.general.MathUtil;
import org.apache.commons.collections4.CollectionUtils;

import java.util.ArrayList;
import java.util.List;

public class IndividualGeneralMessageManager implements IndividualMessageManager {
    private MessageQueue messageQueue;

    public IndividualGeneralMessageManager() {
        messageQueue = new MessageQueue();
    }

    @Override
    public BaseResponseBean retrieveGeneralMessage() {
        return messageQueue.pollGeneralMessage();
    }

    @Override
    public List<BaseResponseBean> retrieveAllChatMessage(String token) {
        throw new UnsupportedOperationException();
    }

    @Override
    public List<BaseResponseBean> retrieveAllHistoryMessage(String uuid) {
        List<BaseResponseBean> received = new ArrayList<>(1);
        List<BaseResponseBean> temp = messageQueue.pollAllHistoryMessage();
        int remainingNum = Constants.NUM_OF_MESSAGE_PER_PAGE;
        int waitTime = Constants.NUM_OF_MESSAGE_PER_PAGE - 1; /**剩余尝试次数**/

        /**如果第一次返回了消息，那么就继续等，否则就返回超时**/
        if (CollectionUtils.isNotEmpty(temp)) {
            while (waitTime > 0) {
                for (BaseResponseBean baseResponseBean : temp) {
                    HistoryMessageListResponseBean historyMessageListResponseBean = (HistoryMessageListResponseBean) baseResponseBean;
                    if (historyMessageListResponseBean.getUuid().equals(uuid)) {
                        if (remainingNum == Constants.NUM_OF_MESSAGE_PER_PAGE) {
                            /**如果是第一次进入这个条件判断，取im服务端给的msgnum**/
                            remainingNum = historyMessageListResponseBean.getMsgnum();
                        }
                        remainingNum -= historyMessageListResponseBean.getReturnnum();

                        received.add(historyMessageListResponseBean);
                    }
                }
                /**waitTime 总是比 remainingNum 小**/
                waitTime = MathUtil.getLittle(waitTime, remainingNum);

                if (waitTime <= 0) {
                    break;
                } else {
                    temp = messageQueue.pollAllHistoryMessage();
                    waitTime--;
                }
            }
        }

        return received;
    }

    @Override
    public int putMessage(BaseResponseBean responseMessage) {
        if (responseMessage == null) {
            return -1;
        } else {
            boolean isPutSuccess = messageQueue.add(responseMessage);

            return isPutSuccess ? 1 : 0;
        }
    }

    @Override
    public void destroy() {
        messageQueue.destroy();
    }
}
