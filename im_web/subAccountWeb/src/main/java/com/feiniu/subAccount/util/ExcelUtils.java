package com.feiniu.subAccount.util;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Method;
import java.net.URLEncoder;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.StringTokenizer;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFDateUtil;
import org.apache.poi.hssf.usermodel.HSSFFont;
import org.apache.poi.hssf.usermodel.HSSFPalette;
import org.apache.poi.hssf.usermodel.HSSFRichTextString;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.hssf.util.HSSFColor;
import org.apache.poi.hssf.util.Region;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.feiniu.core.base.util.DateUtils;
import com.feiniu.subAccount.base.ResponseResult;
import com.feiniu.subAccount.exception.CustomGlobalExceptionResolver;

/**
 * Excel生成工具类
 * 生成一个普通表格样式Excel
 *
 * @author nyj
 */
public class ExcelUtils {
    private static final Logger logger = LoggerFactory.getLogger(CustomGlobalExceptionResolver.class);
    private String sheetName = null;            //工作表名称
    private String sheetDesc = null;            //描述信息
    private HSSFCellStyle headStyle = null;        //字段名显示样式
    private HSSFCellStyle fieldStyle = null;    //字段名显示样式
    private HSSFCellStyle dataStyle = null;        //数据显示
    private String[] fieldNames = null;        //字段名数组
    private String[][] datas = null;            //显示数据
    private List<String> stringFields = null;            //导出Excel时无需转换为数值的字符串列
    private HSSFWorkbook workbook = null;
    private String excelFileName = null;
    private List<String[]> headNames = null;                //表头字段数组
    private List<String[]> headValues = null;                //表头字段值数组

    public ExcelUtils(HSSFWorkbook workbook) {
        this.workbook = workbook;
        setDefaultHeadStyle();
        setDefaultFieldStyle();
        setDefaultDataStyle();
    }

    public void setDatas(String[][] datas) {
        this.datas = datas;
    }

    public void setDataStyle(HSSFCellStyle dataStyle) {
        this.dataStyle = dataStyle;
    }

    public void setFieldNames(String[] fieldNames) {
        this.fieldNames = fieldNames;
    }

    public void setFieldStyle(HSSFCellStyle fieldStyle) {
        this.fieldStyle = fieldStyle;
    }

    public void setSheetName(String sheetName) {
        this.sheetName = sheetName;
    }

    public void setStringFields(List<String> stringFields) {
        this.stringFields = stringFields;
    }

    public void setHeadStyle(HSSFCellStyle headStyle) {
        this.headStyle = headStyle;
    }

    public void setSheetDesc(String sheetDesc) {
        this.sheetDesc = sheetDesc;
    }

    public List<String[]> getHeadNames() {
        return this.headNames;
    }

    public void setHeadNames(List<String[]> headNames) {
        this.headNames = headNames;
    }

    public List<String[]> getHeadValues() {
        return this.headValues;
    }

    public void setHeadValues(List<String[]> headValues) {
        this.headValues = headValues;
    }

    /**
     * 设置缺省标题样式
     */
    public void setDefaultHeadStyle() {
        HSSFCellStyle style = workbook.createCellStyle();
        //style.setFillBackgroundColor(HSSFColor.GREEN.index);
        //设定单元个背景颜色 - customColor #D4D0C8(R=212  G=208  B=200)
        byte r = (byte) 150;
        byte g = (byte) 150;
        byte b = (byte) 150;

        // choose a seed color index
        short colorIndex = HSSFColor.GREEN.index + 1;
        // get the workbook custom pallete and set the color at the seed index to
        // the custom color
        HSSFPalette palette = workbook.getCustomPalette();
        palette.setColorAtIndex(colorIndex, r, g, b);
        style.setFillForegroundColor(colorIndex);
        style.setFillPattern(HSSFCellStyle.SOLID_FOREGROUND);

        //水平居中
        style.setAlignment(HSSFCellStyle.ALIGN_CENTER);
        //垂直居中
        style.setVerticalAlignment(HSSFCellStyle.VERTICAL_CENTER);

        style.setBorderBottom(HSSFCellStyle.BORDER_THIN);    //下边框
        style.setBorderLeft(HSSFCellStyle.BORDER_THIN);        //左边框
        style.setBorderRight(HSSFCellStyle.BORDER_THIN);    //右边框
        style.setBorderTop(HSSFCellStyle.BORDER_THIN);        //上边框
        style.setLeftBorderColor(HSSFColor.BLACK.index);
        style.setRightBorderColor(HSSFColor.BLACK.index);
        style.setTopBorderColor(HSSFColor.BLACK.index);
        style.setBottomBorderColor(HSSFColor.BLACK.index);

        //设置单元格字体显示颜色
        HSSFFont font = workbook.createFont();//创建一个Font
        font.setFontName(HSSFFont.FONT_ARIAL);
        font.setFontHeightInPoints((short) 12);
        font.setBoldweight(HSSFFont.BOLDWEIGHT_BOLD);
        font.setColor(HSSFColor.BLACK.index);
        style.setFont(font);

        this.headStyle = style;
    }

    /**
     * 设置缺省标题样式
     */
    public void setDefaultFieldStyle() {
        HSSFCellStyle style = this.workbook.createCellStyle();
        //style.setFillBackgroundColor(HSSFColor.GREEN.index);
        //设定单元个背景颜色 - customColor #D4D0C8(R=212  G=208  B=200)
        byte r = (byte) 212;
        byte g = (byte) 208;
        byte b = (byte) 200;

        // choose a seed color index
        short colorIndex = HSSFColor.RED.index + 1;
        // get the workbook custom pallete and set the color at the seed index to
        // the custom color
        HSSFPalette palette = this.workbook.getCustomPalette();
        palette.setColorAtIndex(colorIndex, r, g, b);
        style.setFillForegroundColor(colorIndex);
        style.setFillPattern(HSSFCellStyle.SOLID_FOREGROUND);

        //水平居中
        style.setAlignment(HSSFCellStyle.ALIGN_CENTER);
        //垂直居中
        style.setVerticalAlignment(HSSFCellStyle.VERTICAL_CENTER);

        style.setBorderBottom(HSSFCellStyle.BORDER_THIN);    //下边框
        style.setBorderLeft(HSSFCellStyle.BORDER_THIN);        //左边框
        style.setBorderRight(HSSFCellStyle.BORDER_THIN);    //右边框
        style.setBorderTop(HSSFCellStyle.BORDER_THIN);        //上边框
        style.setLeftBorderColor(HSSFColor.BLACK.index);
        style.setRightBorderColor(HSSFColor.BLACK.index);
        style.setTopBorderColor(HSSFColor.BLACK.index);
        style.setBottomBorderColor(HSSFColor.BLACK.index);

        //设置单元格字体显示颜色
        HSSFFont font = this.workbook.createFont();//创建一个Font
        font.setFontName(HSSFFont.FONT_ARIAL);
        font.setFontHeightInPoints((short) 10);
        font.setBoldweight(HSSFFont.BOLDWEIGHT_BOLD);
        font.setColor(HSSFColor.BLACK.index);
        style.setFont(font);

        fieldStyle = style;
    }

    /**
     * 设置缺省普通单元格样式
     */
    public void setDefaultDataStyle() {
        HSSFCellStyle style = this.workbook.createCellStyle();
        style.setFillForegroundColor(HSSFColor.WHITE.index);
        style.setFillPattern(HSSFCellStyle.SOLID_FOREGROUND);

        style.setBorderBottom(HSSFCellStyle.BORDER_THIN);    //下边框
        style.setBorderLeft(HSSFCellStyle.BORDER_THIN);        //左边框
        style.setBorderRight(HSSFCellStyle.BORDER_THIN);    //右边框
        style.setBorderTop(HSSFCellStyle.BORDER_THIN);        //上边框
        style.setLeftBorderColor(HSSFColor.BLACK.index);
        style.setRightBorderColor(HSSFColor.BLACK.index);
        style.setTopBorderColor(HSSFColor.BLACK.index);
        style.setBottomBorderColor(HSSFColor.BLACK.index);

        style.setAlignment(HSSFCellStyle.ALIGN_CENTER);//居中
        style.setAlignment(HSSFCellStyle.VERTICAL_CENTER);//竖直居中
        style.setDataFormat((short) 1);
        //设置单元格字体显示颜色
        HSSFFont font = this.workbook.createFont();//创建一个Font
        font.setFontName(HSSFFont.FONT_ARIAL);
        font.setFontHeightInPoints((short) 9);
        font.setColor(HSSFColor.BLACK.index);
        style.setFont(font);
        this.dataStyle = style;
    }

    /**
     * Convenient method to obtain the cell in the given sheet, row and column.
     * <p>Creates the row and the cell if they still doesn't already exist.
     * Thus, the column can be passed as an int, the method making the needed downcasts.
     *
     * @param sheet a sheet object. The first sheet is usually obtained by workbook.getSheetAt(0)
     * @param row   thr row number
     * @param col   the column number
     * @return the HSSFCell
     */
    protected HSSFCell getCell(HSSFSheet sheet, int row, int col) {
        HSSFRow sheetRow = sheet.getRow(row);
        if (sheetRow == null) {
            sheetRow = sheet.createRow(row);
        }
        HSSFCell cell = sheetRow.getCell((short) col);
        if (cell == null) {
            cell = sheetRow.createCell((short) col);
        }
        return cell;
    }

    /**
     * 生成空数据Excel
     */
    public void buildNullExcelDocument() {
        String sheetName = this.sheetName;
        if (sheetName == null) sheetName = "Sheet1";
        HSSFSheet sheet = this.workbook.createSheet(sheetName);
        HSSFCell cell = this.getCell(sheet, 0, 0);
        cell.setCellValue(new HSSFRichTextString("无数据"));
    }

    /**
     * 生成Excel
     */
    public void buildExcelDocument() {
        buildExcelDocument(sheetName, 0);
    }


    /**
     * 生成Excel
     *
     * @param sheetName sheet名称
     * @param index     sheet索引
     */
    public void buildExcelDocument(String sheetName, int index) {
        HSSFSheet sheet = null;
        HSSFCell cell = null;
        sheet = this.workbook.createSheet(sheetName);
        this.workbook.setSheetName(index, sheetName);
        if (this.datas.length == 0) {
            return;
        }

        int fieldNameLen = this.fieldNames.length;
        int[] colWidths = new int[fieldNameLen];

        int rowIndex = 0;
        //表头
        //指定合并区域 rowFrom, colFrom, rowTo, colTo
        sheet.addMergedRegion(new Region(0, (short) 0, 0, (short) (fieldNameLen - 1)));
        cell = getCell(sheet, rowIndex, 0);
        cell.setCellValue(new HSSFRichTextString(sheetName));
        for (int i = 0; i < fieldNameLen; i++) {
            cell = getCell(sheet, rowIndex, i);
            cell.setCellStyle(this.headStyle);
        }
        rowIndex++;

        //统计描述信息
        //指定合并区域 rowFrom, colFrom, rowTo, colTo
        sheet.addMergedRegion(new Region(1, (short) 0, 1, (short) (fieldNameLen - 1)));
        cell = getCell(sheet, rowIndex, 0);
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String desc = "导出时间：" + sdf.format(new Date());
        if (this.sheetDesc != null && !this.sheetDesc.trim().equals("")) {
            desc += "    " + this.sheetDesc;
        }
        cell.setCellValue(new HSSFRichTextString(desc));
        for (int i = 0; i < fieldNameLen; i++) {
            cell = getCell(sheet, rowIndex, i);
            cell.setCellStyle(this.fieldStyle);
        }
        rowIndex++;

        //字段名
        for (int j = 0; j < fieldNameLen; j++) {
            cell = getCell(sheet, rowIndex, j);
            cell.setCellValue(new HSSFRichTextString(this.fieldNames[j]));
            cell.setCellStyle(this.fieldStyle);
            colWidths[j] = getStrLength(this.fieldNames[j]);
        }
        rowIndex++;

        //数据
        int dateLen = this.datas.length;
        for (int i = 0; i < dateLen; i++) {
            int dLen = this.datas[i].length;
            for (int j = 0; j < dLen; j++) {
                cell = getCell(sheet, i + rowIndex, j);
                String data = this.datas[i][j];
                double dData = -1;
                try {
                    if (this.stringFields.contains(this.fieldNames[j])) {
                        //if(this.stringFields != null && this.stringFields.contains(this.fieldNames[j])) {
                        cell.setCellValue(new HSSFRichTextString(this.datas[i][j]));
                    } else {
                        dData = Double.parseDouble(data);
                        cell.setCellValue(dData);
                    }
                } catch (Exception e) {
                    cell.setCellValue(new HSSFRichTextString(this.datas[i][j]));
                }
                cell.setCellStyle(this.dataStyle);
                if (this.datas[i][j] != null) {
                    //计算表格数据的宽度
                    colWidths[j] = Math.max(colWidths[j], getStrLength(this.datas[i][j]));
                }
            }
        }
        int len = this.datas[0].length;
        for (int j = 0; j < len; j++) {
            double t = (colWidths[j] + 2) * 256;
            sheet.setColumnWidth((short) j, (short) t);
        }

        //冻结头三行
        sheet.createFreezePane(0, 3);
    }


    /**
     * 生成Excel --巡更统计导出
     */
    public void buildExcelDocumentByNameHead(String headName, String dateTitle) {
        buildExcelDocumentByNameHead(sheetName, 0, headName, dateTitle);
    }


    /**
     * 生成Excel
     *
     * @param sheetName sheet名称
     * @param index     sheet索引
     */
    public void buildExcelDocumentByNameHead(String sheetName, int index, String headName, String dateTitle) {
        HSSFSheet sheet = null;
        HSSFCell cell = null;
        sheet = this.workbook.createSheet(sheetName);
        this.workbook.setSheetName(index, sheetName);
        if (this.datas.length == 0) {
            return;
        }

        int fieldNameLen = this.fieldNames.length;
        int[] colWidths = new int[fieldNameLen];

        int rowIndex = 0;
        //表头
        //指定合并区域 rowFrom, colFrom, rowTo, colTo
        sheet.addMergedRegion(new Region(0, (short) 0, 0, (short) (fieldNameLen - 1)));
        cell = getCell(sheet, rowIndex, 0);
        if (headName == null || "".equals(headName)) {
            cell.setCellValue(new HSSFRichTextString(sheetName));
        } else {
            cell.setCellValue(new HSSFRichTextString(headName));
        }
        for (int i = 0; i < fieldNameLen; i++) {
            cell = getCell(sheet, rowIndex, i);
            cell.setCellStyle(this.headStyle);
        }
        rowIndex++;

        //统计描述信息
        //指定合并区域 rowFrom, colFrom, rowTo, colTo
        sheet.addMergedRegion(new Region(1, (short) 0, 1, (short) (fieldNameLen - 1)));
        cell = getCell(sheet, rowIndex, 0);
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String desc = sdf.format(new Date());
        if (dateTitle == null || "".equals(dateTitle)) {
            if (this.sheetDesc != null && !this.sheetDesc.trim().equals("")) {
                desc += "    " + this.sheetDesc;
            }
        } else {
            desc = dateTitle;
        }
        cell.setCellValue(new HSSFRichTextString(desc));
        for (int i = 0; i < fieldNameLen; i++) {
            cell = getCell(sheet, rowIndex, i);
            cell.setCellStyle(this.fieldStyle);
        }
        rowIndex++;

        //字段名
        for (int j = 0; j < fieldNameLen; j++) {
            cell = getCell(sheet, rowIndex, j);
            cell.setCellValue(new HSSFRichTextString(this.fieldNames[j]));
            cell.setCellStyle(this.fieldStyle);
            colWidths[j] = getStrLength(this.fieldNames[j]);
        }
        rowIndex++;

        //数据
        int dateLen = this.datas.length;
        for (int i = 0; i < dateLen; i++) {
            int dLen = this.datas[i].length;
            for (int j = 0; j < dLen; j++) {
                cell = getCell(sheet, i + rowIndex, j);
                String data = this.datas[i][j];
                double dData = -1;
                try {
                    if (this.stringFields != null && this.stringFields.contains(this.fieldNames[j])) {
                        cell.setCellValue(new HSSFRichTextString(this.datas[i][j]));
                    } else {
                        dData = Double.parseDouble(data);
                        cell.setCellValue(dData);
                    }
                } catch (Exception e) {
                    cell.setCellValue(new HSSFRichTextString(this.datas[i][j]));
                }
                cell.setCellStyle(this.dataStyle);
                if (this.datas[i][j] != null) {
                    //计算表格数据的宽度
                    colWidths[j] = Math.max(colWidths[j], getStrLength(this.datas[i][j]));
                }
            }
        }
        int len = this.datas[0].length;
        for (int j = 0; j < len; j++) {
            double t = (colWidths[j] + 2) * 256;
            sheet.setColumnWidth((short) j, (short) t);
        }

        //冻结头三行
        sheet.createFreezePane(0, 3);
    }

    /**
     * 生成Excel  --巡更统计明细导出
     */
    public void buildExcelDocumentByhead(String[] strHead, int sheetHeadCount) {
        buildExcelDocumentByHead(sheetName, 0, strHead, sheetHeadCount);
    }

    /**
     * 生成Excel
     *
     * @param sheetName sheet名称
     * @param index     sheet索引
     * @param strHead   表头中标题和字段名称直接的部分 strHead[0] - type  ,strHead[1]~strHead[n] - 参数
     */
    public void buildExcelDocumentByHead(String sheetName, int index, String[] strHead, int sheetHeadCount) {

        HSSFSheet sheet = null;
        HSSFCell cell = null;
        sheet = this.workbook.createSheet(sheetName);
        this.workbook.setSheetName(index, sheetName);
        if (this.datas.length == 0) {
            return;
        }

        int fieldNameLen = this.fieldNames.length;
        int[] colWidths = new int[fieldNameLen];

        int rowIndex = 0;
        //表头
        //指定合并区域 rowFrom, colFrom, rowTo, colTo
        sheet.addMergedRegion(new Region(0, (short) 0, 0, (short) (fieldNameLen - 1)));
        cell = getCell(sheet, rowIndex, 0);
        cell.setCellValue(new HSSFRichTextString(sheetName));
        for (int i = 0; i < fieldNameLen; i++) {
            cell = getCell(sheet, rowIndex, i);
            cell.setCellStyle(this.headStyle);
        }
        rowIndex++;
        if ("inspectStatDetail".equals(strHead[0])) {
            //统计描述信息
            //指定合并区域 rowFrom, colFrom, rowTo, colTo
            sheet.addMergedRegion(new Region(1, (short) 0, 1, (short) (fieldNameLen - 1)));
            cell = getCell(sheet, rowIndex, 0);
            String descHead = strHead[1];
            if (this.sheetDesc != null && !this.sheetDesc.trim().equals("")) {
                descHead += "    " + this.sheetDesc;
            }
            cell.setCellValue(new HSSFRichTextString(descHead));
            for (int i = 0; i < fieldNameLen; i++) {
                cell = getCell(sheet, rowIndex, i);
                cell.setCellStyle(this.fieldStyle);
            }
            rowIndex++;

            //轮数
            //指定合并区域 rowFrom, colFrom, rowTo, colTo
            int circleNum = Integer.parseInt(strHead[2]);
            int perPointNum = Integer.parseInt(strHead[3]);
            String desc = "轮数";
            cell = getCell(sheet, rowIndex, 0);
            cell.setCellValue(new HSSFRichTextString(desc));
            cell.setCellStyle(this.fieldStyle);
            for (int i = 0; i < circleNum; i++) {
                sheet.addMergedRegion(new Region(2, (short) (i * perPointNum + 1), 2, (short) ((i + 1) * perPointNum)));
                cell = getCell(sheet, rowIndex, i * perPointNum + 1);
                cell.setCellValue(new HSSFRichTextString(String.valueOf(i + 1)));
                cell.setCellStyle(this.fieldStyle);
            }

            rowIndex++;

        } else {
            //统计描述信息
            //指定合并区域 rowFrom, colFrom, rowTo, colTo
            sheet.addMergedRegion(new Region(1, (short) 0, 1, (short) (fieldNameLen - 1)));
            cell = getCell(sheet, rowIndex, 0);
            SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
            String desc = sdf.format(new Date());
            if (this.sheetDesc != null && !this.sheetDesc.trim().equals("")) {
                desc += "    " + this.sheetDesc;
            }
            cell.setCellValue(new HSSFRichTextString(desc));
            for (int i = 0; i < fieldNameLen; i++) {
                cell = getCell(sheet, rowIndex, i);
                cell.setCellStyle(this.fieldStyle);
            }
            rowIndex++;
        }


        //字段名
        for (int j = 0; j < fieldNameLen; j++) {
            cell = getCell(sheet, rowIndex, j);
            cell.setCellValue(new HSSFRichTextString(this.fieldNames[j]));
            cell.setCellStyle(this.fieldStyle);
            colWidths[j] = getStrLength(this.fieldNames[j]);
        }
        rowIndex++;

        //数据
        int dateLen = this.datas.length;
        for (int i = 0; i < dateLen; i++) {
            int dLen = this.datas[i].length;
            for (int j = 0; j < dLen; j++) {
                cell = getCell(sheet, i + rowIndex, j);
                String data = this.datas[i][j];
                double dData = -1;
                try {
                    if (this.stringFields != null && this.stringFields.contains(this.fieldNames[j])) {
                        cell.setCellValue(new HSSFRichTextString(this.datas[i][j]));
                    } else {
                        dData = Double.parseDouble(data);
                        cell.setCellValue(dData);
                    }
                } catch (Exception e) {
                    cell.setCellValue(new HSSFRichTextString(this.datas[i][j]));
                }
                cell.setCellStyle(this.dataStyle);
                if (this.datas[i][j] != null) {
                    //计算表格数据的宽度
                    colWidths[j] = Math.max(colWidths[j], getStrLength(this.datas[i][j]));
                }
            }
        }
        int len = this.datas[0].length;
        for (int j = 0; j < len; j++) {
            double t = (colWidths[j] + 2) * 256;
            sheet.setColumnWidth((short) j, (short) t);
        }

        //冻结头三行
        if (sheetHeadCount > 3) {
            sheet.createFreezePane(0, sheetHeadCount);
        } else {
            sheet.createFreezePane(0, 3);
        }
    }

    /**
     * 获取字符串的长度,考虑中文字符
     *
     * @param Str
     * @return
     */
    private int getStrLength(String str) {
        int len = 0;
        int strLen;
        if (str == null || str.length() <= 0) {
            len = 0;
        }
        char c;
        if (str != null) {
            strLen = str.length();
            for (int i = strLen - 1; i >= 0; i--) {
                c = str.charAt(i);
                if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z')
                        || (c >= 'A' && c <= 'Z')) {
                    // 字母, 数字
                    len++;
                } else {
                    if (Character.isLetter(c)) { // 中文
                        len += 2;
                    } else { // 符号或控制字符
                        len++;
                    }
                }
            }
        }
        return len;
    }

    public HSSFWorkbook getWorkbook() {
        return workbook;
    }

    public void setWorkbook(HSSFWorkbook workbook) {
        this.workbook = workbook;
    }

    public String getExcelFileName() {
        return excelFileName;
    }

    public void setExcelFileName(String excelFileName) {
        this.excelFileName = excelFileName;
    }

    /**
     * 生成单个文件
     *
     * @param path
     * @param fileName
     * @return
     */
    public static File createFile(String path, String fileName) {
        // 创建目录
        File file = null;
        StringTokenizer st = new StringTokenizer(path, "/");
        String path1 = st.nextToken() + "/";
        String path2 = path1;
        while (st.hasMoreTokens()) {
            path1 = st.nextToken() + "/";
            path2 += path1;
            File inbox = new File(path2);
            if (!inbox.exists())
                inbox.mkdir();
        }
        // 创建文件
        try {
            file = new File(path, fileName);
            file.createNewFile();
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
        return file;
    }

    // 删除指定目录下的文件
    public static void del(String filepath) throws IOException {
        File f = new File(filepath);// 定义文件路径
        if (f.exists() && f.isDirectory()) {// 判断是文件还是目录
            if (f.listFiles().length == 0) {// 若目录下没有文件则直接删除
                f.delete();
            } else {
                File delFile[] = f.listFiles();
                int i = f.listFiles().length;
                for (int j = 0; j < i; j++) {
                    if (delFile[j].isDirectory()) {
                        del(delFile[j].getAbsolutePath());
                    }
                    delFile[j].delete();
                }
            }
        }
        f.delete();
    }


    /**
     * 生成下载流
     *
     * @param fileName
     * @param filePathName
     * @param response
     */
    public static void genExportExcelStream(String fileName, String filePathName, HttpServletResponse response) {
        //生成excel文件流
        FileInputStream fileInputStream = null;
        File file = null;
        try {
            response.reset();
            response.setContentType("application/octet-stream");
            response.setHeader("Content-Disposition", "attachment; filename=".concat(URLEncoder.encode(fileName, "UTF-8")));
            OutputStream responseOutputStream = response.getOutputStream();
            file = new File(filePathName);
            fileInputStream = new FileInputStream(file);
            byte[] buf = new byte[64 * 1024];
            int nextRead = -1;
            while ((nextRead = fileInputStream.read(buf)) > -1) {
                responseOutputStream.write(buf, 0, nextRead);
            }
        } catch (Exception e) {
            logger.error("导出excel发生异常");
            throw new RuntimeException(e);
        } finally {
            try {
                if (fileInputStream != null) fileInputStream.close();
            } catch (Exception e) {
                throw new RuntimeException(e);
            }
            try {
                if (file != null && file.exists()) file.delete();
            } catch (Exception e) {
                throw new RuntimeException(e);
            }
        }
    }

    /**
     * 生成xls文件
     */
    public static void createXlsFile(String sheetName, String filePathName, String[] fieldNames, String[][] fieldValues) {
        HSSFWorkbook workbook = new HSSFWorkbook();
        ExcelUtils excelTool = new ExcelUtils(workbook);
        excelTool.setSheetName(sheetName);
        if (fieldValues.length < 1) {
            excelTool.buildNullExcelDocument();
        } else {
            excelTool.setFieldNames(fieldNames);
            excelTool.setDatas(fieldValues);
            excelTool.buildExcelDocument();
        }
        OutputStream xlsOutputStream = null;
        try {
            FileOutputStream xlsFileOutputStream = new FileOutputStream(filePathName);
            xlsOutputStream = new BufferedOutputStream(xlsFileOutputStream);
            workbook.write(xlsOutputStream);
        } catch (IOException e) {
        } finally {
            try {
                if (xlsOutputStream != null) xlsOutputStream.close();
            } catch (Exception e) {
            }
        }
    }

    public static void generateExcelStream(String fileName, String sheetName, String[] fieldNames,
                                           String[][] fieldValues, HttpServletRequest httpServletRequest, HttpServletResponse response) {
        HSSFWorkbook workbook = new HSSFWorkbook();
        ExcelUtils excelTool = new ExcelUtils(workbook);
        excelTool.setSheetName(sheetName);

        if (fieldValues.length < 1) {
            excelTool.buildNullExcelDocument();
        } else {
            excelTool.setFieldNames(fieldNames);
            excelTool.setDatas(fieldValues);
            excelTool.buildExcelDocument();
        }

        OutputStream xlsOutputStream = null;
        try {
            String userAgent = httpServletRequest.getHeader("User-Agent");
            byte[] bytes = userAgent.contains("MSIE") ? fileName.getBytes() : fileName.getBytes("UTF-8");
            fileName = new String(bytes, "ISO-8859-1");

            response.reset();
            response.setContentType("application/octet-stream");
            response.setHeader("Content-Disposition", String.format("attachment; filename=\"%s\"", fileName));

            OutputStream xlsFileOutputStream = response.getOutputStream();
            xlsOutputStream = new BufferedOutputStream(xlsFileOutputStream);
            workbook.write(xlsOutputStream);
        } catch (IOException e) {
            throw new RuntimeException(e);
        } finally {
            try {
                if (xlsOutputStream != null) xlsOutputStream.close();
            } catch (Exception e) {
                throw new RuntimeException(e);
            }
        }
    }

    /**
     * 导出用户列表
     *
     * @param filePathName
     * @param sheetName1
     * @param fields1
     * @param values1
     * @param sheetName2
     * @param fields2
     * @param values2
     */
    public static void createUserXlsFile(String filePathName,
                                         String sheetName1, String[] fields1, String[][] fieldValues1,
                                         String sheetName2, String[] fields2, String[][] fieldValues2) {
        HSSFWorkbook workbook = new HSSFWorkbook();
        ExcelUtils excelTool = new ExcelUtils(workbook);
        if (fieldValues1 == null || fieldValues1.length < 1) {
            excelTool.setSheetName(sheetName1);
            excelTool.buildNullExcelDocument();
        } else {
            excelTool.setFieldNames(fields1);
            excelTool.setDatas(fieldValues1);
            excelTool.buildExcelDocument(sheetName1, 0);
        }
        if (fieldValues2 == null || fieldValues2.length < 1) {
            excelTool.setSheetName(sheetName2);
            excelTool.buildNullExcelDocument();
        } else {
            excelTool.setFieldNames(fields2);
            excelTool.setDatas(fieldValues2);
            excelTool.buildExcelDocument(sheetName2, 1);
        }
        OutputStream xlsOutputStream = null;
        try {
            FileOutputStream xlsFileOutputStream = new FileOutputStream(filePathName);
            xlsOutputStream = new BufferedOutputStream(xlsFileOutputStream);
            workbook.write(xlsOutputStream);
        } catch (IOException e) {
        } finally {
            try {
                if (xlsOutputStream != null) xlsOutputStream.close();
            } catch (Exception e) {
            }
        }

    }

    /**
     * ==========================================================
     **/


    public static ResponseResult checkColName(InputStream ins, List<String> colNames, int sheetNum, int rowNum) {
        ResponseResult resultInfo = new ResponseResult();
        HSSFWorkbook wk = null;
        HSSFSheet sheet = null;
        HSSFRow row = null;
        HSSFCell cell = null;
        String content = null;
        Map<String, Integer> fMap = new HashMap<String, Integer>();
        try {
            wk = new HSSFWorkbook(ins);
            sheet = wk.getSheetAt(sheetNum - 1);
            row = sheet.getRow(rowNum - 1);
            for (int i = 0; i < row.getLastCellNum(); i++) {
                cell = row.getCell((short) i);
                cell.setCellType(HSSFCell.CELL_TYPE_STRING);
                content = cell.getRichStringCellValue().getString();
                if (!(content == null || "".equals(content))) {
                    fMap.put(content, i);
                }
            }
            for (String colName : colNames) {
                if (!fMap.containsKey(colName)) {
                    resultInfo.setMessage(colName);
                    return resultInfo;
                }
            }

        } catch (Exception e) {
            logger.error(e.getMessage());
        }
        resultInfo.markSuccess();
        return resultInfo;
    }


    /**
     * read excel data to java list object
     *
     * @param ins
     * @param fieldMapping   <Title, FieldName>
     * @param clazz
     * @param sheetNum
     * @param dataFromRowNum
     * @return
     */
    public static <T> List<T> readExcelData(InputStream ins, Map<String, String> fieldMapping, Class<T> clazz, int sheetNum, int dataFromRowNum) {

        if (ins == null || fieldMapping == null || clazz == null || sheetNum < 1) {
            logger.error(" please check your para in cn.com.egova.tools.ExcelUtils.readExcelData ");
        }

        List<T> list = new ArrayList<T>();
        HSSFWorkbook wk = null;
        HSSFSheet sheet = null;
        HSSFRow row = null;
        HSSFCell cell = null;
        String content = null;
        String fieldName = null;
        Method getMethod = null;
        Method setMethod = null;
        Class<?> paraType = null;
        Object value = null;
        try {

            wk = new HSSFWorkbook(ins);
            sheet = wk.getSheetAt(sheetNum - 1);
            if (isMergedRegion(sheet, dataFromRowNum - 1, 0)) {//适用于系统导出的记录，数据从第三行开始
                dataFromRowNum += 1;
            }
            row = sheet.getRow(dataFromRowNum - 1);
            if (row == null) {
                return null;
            }
            Map<Integer, String> columnMap = new HashMap<Integer, String>();
            for (int i = 0; i < row.getLastCellNum(); i++) {
                cell = row.getCell((short) i);
                cell.setCellType(HSSFCell.CELL_TYPE_STRING);
                content = cell.getRichStringCellValue().getString();
                // if title is null, get the first row content as title
                if (content == null || "".equals(content)) {
                    if (isMergedRegion(sheet, row.getRowNum(), i)) {
                        content = getMergedRegionValue(sheet, row.getRowNum(), i);
                    }
                }
                fieldName = fieldMapping.get(content);
                if (fieldName != null) {
                    columnMap.put(i, fieldName);
                }
            }

            if (columnMap == null || columnMap.size() == 0) {
                return null;
            }
            for (int i = dataFromRowNum; i <= sheet.getLastRowNum(); i++) {
                row = sheet.getRow(i);
                if (row.getLastCellNum() == 0) {
                    continue;
                }
                T obj = clazz.newInstance();
                for (int j = 0; j < row.getLastCellNum(); j++) {
                    cell = row.getCell((short) j);
                    if (isMergedRegion(sheet, row.getRowNum(), j)) {
                        content = getMergedRegionValue(sheet, row.getRowNum(), j);
                    } else {
                        content = getCellValue(cell);
                    }
                    fieldName = columnMap.get(j);
                    if (fieldName == null) {
                        if (fieldName == null) {
                            continue;
                        }
                    } else {
                        getMethod = clazz.getMethod("get" + fieldName);
                        paraType = getMethod.getReturnType();
                        setMethod = clazz.getMethod("set" + fieldName, paraType);
                        if (content != null) {
                            value = transfType(paraType, content.trim());
                            if (value != null) {
                                setMethod.invoke(obj, value);
                            }
                        }
                    }
                }
                list.add(obj);
            }
        } catch (Exception e) {
//        	logger.error(e.getMessage());
            e.printStackTrace();
        }
        return list;
    }

    private static Object transfType(Class<?> paraType, String value) {
        Object result = null;
        if (paraType == String.class) {
            result = value;
        } else if (paraType == Double.class) {
            result = new Double(Double.parseDouble(value));
        } else if (paraType == double.class) {
            result = Double.parseDouble(value);
        } else if (paraType == boolean.class) {
            result = Boolean.parseBoolean(value);
        } else if (paraType == Boolean.class) {
            result = Boolean.valueOf(value);
        } else if (paraType == int.class) {
            if ("".equals(value)) {
                result = 0;
            } else {
                result = (int) Double.parseDouble(value);//防止从excel读出带.0的数字
            }
        } else if (paraType == Integer.class) {
            result = Integer.decode(value);
        } else if (paraType == long.class) {
            result = Long.parseLong(value);
        } else if (paraType == Long.class) {
            result = Long.decode(value);
        } else if (paraType == float.class) {
            result = Float.parseFloat(value);
        } else if (paraType == Float.class) {
            result = Float.valueOf(value);
        } else if (paraType == Date.class) {
            try {
                result = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").parse(value);
            } catch (Exception e) {
                try {
                    result = new SimpleDateFormat("yyyy-MM-dd").parse(value);
                } catch (Exception e1) {
                }
            }
        }
        return result;
    }

    /**
     * 获取合并单元格的值
     *
     * @param sheet
     * @param row
     * @param column
     * @return
     */
    private static String getMergedRegionValue(HSSFSheet sheet, int row, int column) {
        int sheetMergeCount = sheet.getNumMergedRegions();

        for (int i = 0; i < sheetMergeCount; i++) {
            Region ca = sheet.getMergedRegionAt(i);
            int firstColumn = ca.getColumnFrom();
            int lastColumn = ca.getColumnTo();
            int firstRow = ca.getRowFrom();
            int lastRow = ca.getRowTo();

            if (row >= firstRow && row <= lastRow) {

                if (column >= firstColumn && column <= lastColumn) {
                    HSSFRow fRow = sheet.getRow(firstRow);
                    HSSFCell fCell = fRow.getCell((short) firstColumn);
                    return getCellValue(fCell);
                }
            }
        }

        return null;
    }

    /**
     * 判断指定的单元格是否是合并单元格
     *
     * @param sheet
     * @param row
     * @param column
     * @return
     */
    private static boolean isMergedRegion(HSSFSheet sheet, int row, int column) {
        int sheetMergeCount = sheet.getNumMergedRegions();
        for (int i = 0; i < sheetMergeCount; i++) {
            Region ca = sheet.getMergedRegionAt(i);
            int firstColumn = ca.getColumnFrom();
            int lastColumn = ca.getColumnTo();
            int firstRow = ca.getRowFrom();
            int lastRow = ca.getRowTo();

            if (row >= firstRow && row <= lastRow) {
                if (column >= firstColumn && column <= lastColumn) {
                    return true;
                }
            }
        }

        return false;
    }

    /**
     * 获取单元格的值
     *
     * @param cell
     * @return
     */
    private static String getCellValue(HSSFCell cell) {
        if (cell == null) return "";
        if (cell.getCellType() == HSSFCell.CELL_TYPE_STRING) {
            return cell.getRichStringCellValue().getString();
        } else if (cell.getCellType() == HSSFCell.CELL_TYPE_BOOLEAN) {
            return String.valueOf(cell.getBooleanCellValue());
        } else if (cell.getCellType() == HSSFCell.CELL_TYPE_FORMULA) {
            return cell.getCellFormula();
        } else if (cell.getCellType() == HSSFCell.CELL_TYPE_BLANK) {
            return "";
        } else if (cell.getCellType() == HSSFCell.CELL_TYPE_NUMERIC) {
            if (HSSFDateUtil.isCellDateFormatted(cell)) {//日期
                Date date = cell.getDateCellValue();
                return String.valueOf(DateUtils.convertDateToStr(date));
            } else {
                DecimalFormat decimalFormat = new DecimalFormat("#.#");//修正电话号码被格式化成1.2E10的形式
                String val = decimalFormat.format(cell.getNumericCellValue());
                if (val.endsWith(".0")) {//省略浮点数后的.0
                    val = val.replace(".0", "");
                }
                return val;
            }
        }
        return "";
    }

    public static void main(String[] args) {
        //HSSFWorkbook workbook = new HSSFWorkbook();
        //ExcelUtils excelUtils = new ExcelUtils(workbook);
    }

}
