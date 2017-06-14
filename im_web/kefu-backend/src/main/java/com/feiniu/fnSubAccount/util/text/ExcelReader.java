package com.feiniu.fnSubAccount.util.text;

import org.apache.commons.lang.StringUtils;
import org.apache.poi.ss.usermodel.*;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Author: wangchen
 * Date: 2015/7/1
 * Contact: 243717042@qq.com
 */
public class ExcelReader {
    private String filePath;
    private String sheetName;
    private int index;
    private Workbook workBook;
    private Sheet sheet;
    private List<String> columnHeaderList;
    private List<List<String>> listData;
    private List<Map<String,String>> mapData;
    private boolean flag;
    private InputStream inputStream;

    public ExcelReader(String filePath, String sheetName) {
        this.filePath = filePath;
        this.sheetName = sheetName;
        this.flag = false;
        this.loadFile();
    }

    public ExcelReader(InputStream fileInputStream, String sheetName){
        this.inputStream = fileInputStream;
        this.sheetName = sheetName;
        this.flag = false;
        this.loadStream();

    }

    public ExcelReader(InputStream fileInputStream, int index){
        this.inputStream = fileInputStream;
        this.index = index;
        this.flag = false;
        this.loadStream();
    }

    private void loadStream(){
        try {
            this.workBook = WorkbookFactory.create(inputStream);
            if (StringUtils.isNotEmpty(sheetName)) {
                this.sheet = workBook.getSheet(sheetName);
            } else {
                this.sheet = workBook.getSheetAt(index);
            }

            this.loadSheetData();
        }catch (Exception e){
            e.printStackTrace();
        }finally{
            try {
                if(inputStream!=null){
                    inputStream.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private void loadFile() {
        FileInputStream inStream = null;
        try {
            inStream = new FileInputStream(new File(filePath));
            workBook = WorkbookFactory.create(inStream);
            sheet = workBook.getSheet(sheetName);
        } catch (Exception e) {
            e.printStackTrace();
        }finally{
            try {
                if(inStream!=null){
                    inStream.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private String getCellValue(Cell cell) {
        String cellValue = "";
        DataFormatter formatter = new DataFormatter();
        if (cell != null) {
            switch (cell.getCellType()) {
                case Cell.CELL_TYPE_NUMERIC:
                    if (DateUtil.isCellDateFormatted(cell)) {
                        cellValue = formatter.formatCellValue(cell);
                    } else {
                        double value = cell.getNumericCellValue();
                        int intValue = (int) value;
                        cellValue = value - intValue == 0 ? String.valueOf(intValue) : String.valueOf(value);
                    }
                    break;
                case Cell.CELL_TYPE_STRING:
                    cellValue = cell.getStringCellValue();
                    break;
                case Cell.CELL_TYPE_BOOLEAN:
                    cellValue = String.valueOf(cell.getBooleanCellValue());
                    break;
                case Cell.CELL_TYPE_FORMULA:
                    cellValue = String.valueOf(cell.getCellFormula());
                    break;
                case Cell.CELL_TYPE_BLANK:
                    cellValue = "";
                    break;
                case Cell.CELL_TYPE_ERROR:
                    cellValue = "";
                    break;
                default:
                    cellValue = cell.toString().trim();
                    break;
            }
        }
        return cellValue.trim();
    }

    private void loadSheetData() {
        listData = new ArrayList<>();
        mapData = new ArrayList<>();
        columnHeaderList = new ArrayList<>();
        int numOfRows = sheet.getLastRowNum() + 1;
        for (int i = 0; i < numOfRows; i++) {
            Row row = sheet.getRow(i);
            Map<String, String> map = new HashMap<>();
            List<String> list = new ArrayList<>();
            if (row != null) {
                for (int j = 0; j < row.getLastCellNum(); j++) {
                    Cell cell = row.getCell(j);
                    if (i == 0){
                        columnHeaderList.add(getCellValue(cell));
                    }
                    else{
                        map.put(columnHeaderList.get(j), this.getCellValue(cell));
                    }
                    list.add(this.getCellValue(cell));
                }
            }
            if (i > 0){
                mapData.add(map);
            }
            listData.add(list);
        }
        flag = true;
    }

    public List<String> getColumnHeaderList() {
        return columnHeaderList;
    }

    public void setColumnHeaderList(List<String> columnHeaderList) {
        this.columnHeaderList = columnHeaderList;
    }

    public List<List<String>> getListData() {
        return listData;
    }

    public void setListData(List<List<String>> listData) {
        this.listData = listData;
    }

    public List<Map<String, String>> getMapData() {
        return mapData;
    }

    public void setMapData(List<Map<String, String>> mapData) {
        this.mapData = mapData;
    }
}
