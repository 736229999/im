package util.webim;

import com.feiniu.webim.bean.response.BaseResponseBean;
import com.feiniu.webim.bean.response.LoginResponseBean;

import java.util.concurrent.CountDownLatch;

/**
 * Author: morningking
 * Date: 2016/3/4
 * Contact: 243717042@qq.com
 */
public class WebIMProcess implements Runnable, Subscriber<BaseResponseBean> {
    private String userName;
    private int userID;
    private int pid;
    private boolean needLogin;
    private volatile Register<BaseResponseBean> register;
    private CountDownLatch loginLatch = new CountDownLatch(1);
    private CountDownLatch chatLatch = new CountDownLatch(100);
    private volatile BaseResponseBean baseResponseBean;

    public WebIMProcess(String userName, int userID, int pid) {
        this.userName = userName;
        this.userID = userID;
        this.pid = pid;
    }

    public void setRegister(Register<BaseResponseBean> baseResponseBeanRegister) {
        this.register = baseResponseBeanRegister;
    }

    @Override
    public void run() {
        sendLogin();

    }

    public void sendLogin() {

    }

    public void doQueue() {

    }

    public void sendMessage() {

    }

    public void exit() {

    }

    @Override
    public void dispatch(BaseResponseBean object) {
        if (object instanceof LoginResponseBean) {
            baseResponseBean = object;
            loginLatch.countDown();
        }
    }
}
