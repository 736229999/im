package com.feiniu.fnSubAccount.util.io;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

/**
 * Author: morningking
 * Date: 2015/10/20
 * Contact: 243717042@qq.com
 */
public class ImageUtil {
    /**
     * 获取图片文件的高和宽
     * @param file
     * @return [宽度，高度]
     * @throws IOException
     */
    public static Integer[] getImageWidthHeight(File file)throws IOException{
        BufferedImage bufferedImage = ImageIO.read(file);
        Integer[] info = new Integer[2];

        info[0] = bufferedImage.getWidth();
        info[1] = bufferedImage.getHeight();

        return info;
    }
}
