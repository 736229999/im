package util;

/**
 * Author: morningking
 * Date: 2015/11/9
 * Contact: 243717042@qq.com
 */
public class RandomTest {
    public static void main(String[] args) {
        int[] result = new int[2];

        for (int i = 0; i < 1000; i++) {
            int random = getRandom();

            result[random] = result[random] + 1;
        }

        System.out.println(result[0]);
    }

    public static int getRandom() {
        return (int) Math.floor(Math.random() * 2);
    }
}
