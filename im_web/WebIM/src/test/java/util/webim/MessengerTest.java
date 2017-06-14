package util.webim;

import com.fasterxml.jackson.databind.JsonNode;
import com.feiniu.webim.bean.request.ExitRequestBean;
import com.feiniu.webim.bean.request.JoinRequestBean;
import com.feiniu.webim.util.json.JSONUtil;
import com.feiniu.webim.util.json.JsonNodeUtil;
import org.apache.commons.lang.StringUtils;

import java.util.Map;
import java.util.UUID;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * Author: morningking
 * Date: 2016/3/4
 * Contact: 243717042@qq.com
 */
public class MessengerTest {
    private static final Messenger messenger = new Messenger("10.211.240.177", 10006);
    private static boolean isContinue = true;

    public static void main(String[] args) throws Exception {
        AtomicInteger seqGenerator = new AtomicInteger(1);
        messenger.initNetWorkEnv();

        initListening();

        int num = 10000;
        String baseName = "morningking";

        while (isContinue) {
            for (int i = 0; i < num; i++) {
                JoinRequestBean joinRequestBean = new JoinRequestBean();
                joinRequestBean.setSeq(seqGenerator.addAndGet(1));
                joinRequestBean.setSid(0);
                joinRequestBean.setLogin_flag((byte) 5);
                joinRequestBean.setUsername(baseName + i);
                joinRequestBean.setRandom_key("11111");
                joinRequestBean.setPassword("123456");
                joinRequestBean.setSessionID(UUID.randomUUID().toString() + UUID.randomUUID().toString());
                joinRequestBean.setStatus(0);
                joinRequestBean.setMobilephoneno("");
                joinRequestBean.setEmail("");

                messenger.send(joinRequestBean);




            }

            TimeUnit.SECONDS.sleep(1);
        }

        isContinue = false;
    }

    public static void initListening() {
        Thread listeningThread = new Thread(new Runnable() {
            @Override
            public void run() {
                String messageJSON;

                /**获取im-时间**/
                while (true) {
                    messageJSON = messenger.receive();
                    Map map = JSONUtil.toMap(messageJSON);

                    if (map != null && map.containsKey("serverTime")) {
                        int imTime = Integer.parseInt(map.get("serverTime").toString());
                        int webimTime = (int) (System.currentTimeMillis() / 1000);

                        System.out.println("SYNC_TIME[同步im时间]: webim-now:" + webimTime + "s, im-now:" + imTime + 's');
                        break;
                    }
                }

                while (isContinue) {
                    try {

                        messageJSON = messenger.receive();
                        JsonNode jsonNode = JSONUtil.toNode(messageJSON);
                        int id = JsonNodeUtil.getInteger(jsonNode,"user_id",0);
                        ExitRequestBean exitRequestBean = ExitRequestBean.getMessage();
                        exitRequestBean.setId(id);
                        exitRequestBean.setSid(id);
                        exitRequestBean.setAuthData((byte) 0);
                        messenger.send(exitRequestBean);

                        if (StringUtils.isNotEmpty(messageJSON)) {
                            System.out.println("{监听线程}服务器返回数据：" + messageJSON);
                        } else {
                            System.out.println("{监听线程}服务器返回空数据：" + messageJSON);
                        }


                    } catch (Exception e) {
                        System.out.println("{监听线程}将服务器返回数据解析为Json对象失败：" + messageJSON);
                        e.printStackTrace();
                    }
                }
            }
        }, "接收线程");

        listeningThread.start();
        System.out.println("初始化接收者线程成功");
    }
}
