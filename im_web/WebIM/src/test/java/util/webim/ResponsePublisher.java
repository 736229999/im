package util.webim;

import com.feiniu.webim.bean.response.BaseResponseBean;
import com.feiniu.webim.bean.response.LoginResponseBean;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Author: morningking
 * Date: 2016/3/4
 * Contact: 243717042@qq.com
 */
public class ResponsePublisher implements Publisher<BaseResponseBean>, Register<BaseResponseBean> {
    Map<String, Subscriber<BaseResponseBean>> map = new ConcurrentHashMap<>();

    @Override
    public void register(String topic, Subscriber<BaseResponseBean> subscriber) {
        map.put(topic, subscriber);
    }

    @Override
    public void publish(BaseResponseBean value) {
        if (value instanceof LoginResponseBean) {
            LoginResponseBean loginResponseBean = (LoginResponseBean) value;
            map.get(loginResponseBean.getSessionID()).dispatch(value);
        } else {
            map.get(value.getDid()).dispatch(value);
        }
    }
}
