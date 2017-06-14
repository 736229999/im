import org.zeromq.ZMQ;
import org.zeromq.ZMQException;

/**
 * Created by wangzhen on 2016/3/1.
 */
public class TestServer {
    public static void main(String[] argv) {
        ZMQ.Context context = ZMQ.context(1);
        ZMQ.Socket socket = context.socket(ZMQ.REP);
        String url = "tcp://*:9999";
        try {
            socket.bind(url);
        } catch (ZMQException e) {
            throw e;
        }
        boolean wait = true;
        while (wait) {
            byte[] request;
            try {
                request = socket.recv(0);

                System.out.println("server Received reply   [" + new String(request) + "]");
                /* TODO process request
                 * .....
                */
                socket.send("OK".getBytes(), 1);

            } catch (ZMQException e) {
                throw e;
            }
        } // while(wait)
    }
}
