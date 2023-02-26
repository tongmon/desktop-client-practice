package com.example.attendance_app;

import android.content.Context;
import android.os.Build;
import android.os.Environment;

import androidx.annotation.RequiresApi;

import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.apache.poi.ss.util.CellRangeAddress;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Vector;

public class DownloadExcel {
    DownloadExcel(){}

    @RequiresApi(api = Build.VERSION_CODES.O)
    String saveWorkExcel(Context context, Vector<Vector<String>> month, Vector<Vector<String>> week, Vector<Vector<String>> day){
        Workbook workbook = new HSSFWorkbook();
        Sheet sheet = workbook.createSheet("1"); // 새로운 시트 생성
        Row row = ((Sheet) sheet).createRow(0); // 새로운 행 생성
        Cell cell;

        // 월간 근태 현황
        int i = 0;
        cell = row.createCell(0);
        cell.setCellValue("월간 근태 현황");
        sheet.addMergedRegion(new CellRangeAddress(i, i, 0,  month.get(0).size() - 1));
        for(int k = 0; k < month.size() ; k++){ // 데이터 엑셀에 입력
            i++;
            row = ((Sheet) sheet).createRow(i);
            for(int j=0; j< month.get(k).size(); j++){
                cell = row.createCell(j);
                cell.setCellValue(month.get(k).get(j));
            }
        }

        // 주간 근태 현황
        i = i +2;
        row = ((Sheet) sheet).createRow(i); // 새로운 행 생성
        cell = row.createCell(0);
        cell.setCellValue("주간 근태 현황");
        sheet.addMergedRegion(new CellRangeAddress(i, i, 0,  week.get(0).size() - 1));
        for(int k = 0; k < week.size(); k++){ // 데이터 엑셀에 입력
            i++;
            row = ((Sheet) sheet).createRow(i);
            for(int j=0; j< week.get(k).size(); j++){
                cell = row.createCell(j);
                cell.setCellValue(week.get(k).get(j));
            }
        }

        // 일간 근태 현황
        i = i +2;
        row = ((Sheet) sheet).createRow(i); // 새로운 행 생성
        cell = row.createCell(0);
        cell.setCellValue("일간 근태 현황");
        sheet.addMergedRegion(new CellRangeAddress(i, i, 0,  day.get(0).size() - 1));
        for(int k = 0; k < day.size(); k++){ // 데이터 엑셀에 입력
            i++;
            row = ((Sheet) sheet).createRow(i);
            for(int j=0; j< day.get(k).size(); j++){
                cell = row.createCell(j);
                cell.setCellValue(day.get(k).get(j));
            }
        }
        File path = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS);
        String folder_path = path.getPath();
        String folder_nm = "status";
        LocalDateTime date = LocalDateTime.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyyMMddHHSS");
        String name = formatter.format(date);
        folder_nm += name + ".xls";
        File excelFile = new File(folder_path,folder_nm);
        try{
            FileOutputStream os = new FileOutputStream(excelFile);
            workbook.write(os);
        }catch (IOException e){
            e.printStackTrace();
        }
        System.out.println(excelFile.getAbsolutePath());
        return folder_path.substring(folder_path.lastIndexOf("/")+1) + "/"+folder_nm;
    }

}
