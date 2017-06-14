package util;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Author: morningking
 * Date: 2015/11/9
 * Contact: 243717042@qq.com
 */
public class RegexTest {
    public static void main(String[] args) {
        String baseUrlListString = "http://img[17,18,19].fn-mart.com/imageservice/";
        Pattern BASE_URL_LIST_SPLIT_URL_PATTERN = Pattern.compile("^(http://[^]]+)\\[((?:\\d+,?)+)\\](.+)$");
        String[] BASE_URL_LIST;
        Matcher matcher = BASE_URL_LIST_SPLIT_URL_PATTERN.matcher(baseUrlListString);

        //如果是beta以上环境
        if (matcher.matches()) {
            String[] numberArray = matcher.group(2).split(",");
            BASE_URL_LIST = new String[numberArray.length];

            for (int i = 0; i < numberArray.length; i++) {
                BASE_URL_LIST[i] = matcher.group(1) + numberArray[i] + matcher.group(3);
            }
        } else {
            BASE_URL_LIST = new String[]{baseUrlListString};
        }

        System.out.println(BASE_URL_LIST.length);
    }
}
