package util.webim;

/**
 * Author: morningking
 * Date: 2016/3/4
 * Contact: 243717042@qq.com
 */
public interface Register<T> {
    void register(String topic, Subscriber<T> subscriber);
}
