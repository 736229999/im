import org.zeromq.ZMQ;

/**
 * Created by wangzhen on 2016/3/1.
 */
public class TestZMqueue {
    public static void main(String args[]) {
//        for (int j = 0; j < 100; j++) {
            new Thread(new Runnable(){

                public void run() {
                    // TODO Auto-generated method stub
                    ZMQ.Context context = ZMQ.context(1);  //创建1个I/O线程的上下文
                    ZMQ.Socket subscriber = context.socket(ZMQ.SUB);     //创建一个sub类型，也就是subscriber类型的socket
                    subscriber.connect("tcp://kefu-merchant.dev.fn.com:9999");    //与在9999端口监听的publisher建立连接
                    subscriber.subscribe("queueinfo".getBytes());     //订阅fjs这个channel


                    byte[] message = subscriber.recv();  //接收publisher 发送过来的消息
                    System.out.println("receive : " + new String(message));
                    subscriber.close();
                    context.term();
                }

            }).start();
        }

  //  }
}
