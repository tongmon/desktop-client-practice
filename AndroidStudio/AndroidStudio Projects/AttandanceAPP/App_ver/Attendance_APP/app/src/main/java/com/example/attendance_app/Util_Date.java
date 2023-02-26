package com.example.attendance_app;

import android.os.Build;

import androidx.annotation.RequiresApi;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.format.DateTimeFormatter;
import java.util.Locale;
import java.util.Vector;

@RequiresApi(api = Build.VERSION_CODES.O)
public class Util_Date {
    Util_Date(){

    }
    //월요일 ~ 일요일 일자 구하기
    public static Vector<String> weekDate(String date) {
        Vector<String> start_end_date = new Vector<>();
        LocalDate localDate = LocalDate.parse(date, DateTimeFormatter.ISO_DATE);
        String day = getDateDay(localDate);
        switch (day) {
            case "화요일":
                localDate = localDate.minusDays(1);
                break;
            case "수요일":
                localDate = localDate.minusDays(2);
                break;
            case "목요일":
                localDate = localDate.minusDays(3);
                break;
            case "금요일":
                localDate = localDate.minusDays(4);
                break;
            case "토요일":
                localDate = localDate.minusDays(5);
                break;
            case "일요일":
                localDate = localDate.plusDays(1);
                break;
        }
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
        for(int i=0; i<7; i++){
            LocalDate tmp = localDate.plusDays(i);
            start_end_date.add(formatter.format(tmp));
        }
        return start_end_date;
    }
    //일요일 ~ 월요일 일자 구하기
    public static Vector<String> weekStartSundayDate(String date) {
        Vector<String> start_end_date = new Vector<>();
        LocalDate localDate = LocalDate.parse(date, DateTimeFormatter.ISO_DATE);
        String day = getDateDay(localDate);
        switch (day) {
            case "월요일":
                localDate = localDate.minusDays(1);
                break;
            case "화요일":
                localDate = localDate.minusDays(2);
                break;
            case "수요일":
                localDate = localDate.minusDays(3);
                break;
            case "목요일":
                localDate = localDate.minusDays(4);
                break;
            case "금요일":
                localDate = localDate.minusDays(5);
                break;
            case "토요일":
                localDate = localDate.minusDays(6);
                break;
        }
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
        for(int i=0; i<7; i++){
            LocalDate tmp = localDate.plusDays(i);
            start_end_date.add(formatter.format(tmp));
        }
        return start_end_date;
    }
    // 요일 구하기
    public static String getDateDay(LocalDate now) {
        String day = null;
        int dayOfWeekValue = now.getDayOfWeek().getValue();

        switch (dayOfWeekValue) {
            case 1:
                day = "월요일";
                break;
            case 2:
                day = "화요일";
                break;
            case 3:
                day = "수요일";
                break;
            case 4:
                day = "목요일";
                break;
            case 5:
                day = "금요일";
                break;
            case 6:
                day = "토요일";
                break;
            case 7:
                day = "일요일";
                break;
        }
        return day;
    }

    // 날짜 형식
    public static String getDate(String date, String day) {
        String result_date;
        // 오늘 날짜
        LocalDateTime now = LocalDateTime.now(ZoneId.of("Asia/Seoul"));
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
        String getTime = formatter.format(now);

        // 형식 변환
        formatter = DateTimeFormatter.ofPattern("yyyy년 MM월 dd일");
        LocalDate comp = LocalDate.parse(date);
        String nowDate = formatter.format(comp);
        result_date = nowDate+" " + day;
        // 날짜 비교
        if(date.equals(getTime)){
            formatter = DateTimeFormatter.ofPattern("a HH시 mm분");
            result_date += " " + now.format(formatter.withLocale(Locale.forLanguageTag("ko")));
        }

        return result_date;
    }
}
