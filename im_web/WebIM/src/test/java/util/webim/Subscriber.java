package util.webim;

/**
 * Author: morningking
 * Date: 2016/3/4
 * Contact: 243717042@qq.com
 */
public interface Subscriber<T> {
    void dispatch(T object);
}
