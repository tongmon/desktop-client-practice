package com.example.attendance_app;

import static com.example.attendance_app.Util_Date.getDateDay;
import static com.example.attendance_app.Util_Date.weekDate;
import static com.example.attendance_app.Util_Date.weekStartSundayDate;

import android.graphics.Typeface;
import android.os.Build;

import androidx.annotation.RequiresApi;

import java.time.LocalDate;
import java.time.LocalTime;
import java.time.format.DateTimeFormatter;
import java.util.HashMap;
import java.util.Objects;
import java.util.Vector;

class WorkMonth{
    int day_type;       // 0 - 공휴일, 1 - 주말, 2 - etc, 3 - 현재 날짜 이후 / 이전
    int type;           // 0 - 정상, 1 - 지각, 2 - 결근, 3 - 휴가, 4 - null
    String status;
    String on_time;
    String off_time;
}

class Statistics{
    int duty_work;              // 의무근로
    int predetermined_work;     // 소정근로
    int extension_work;         // 연장근로
    int vacation;               // 휴가
}

@RequiresApi(api = Build.VERSION_CODES.O)
public class WorkCalendarData {
    HashMap<String, Holiday> holidays = new HashMap<>();
    UserWorkInfo userWorkInfo;
    String join_date;

    WorkCalendarData(){
        try {
            /*공휴일 불러오는 예시 소스*/
            new HolidayCalendar();
            userWorkInfo = new UserWorkInfo();
            for(Holiday hl : HolidayCalendar.holiday_list){
                holidays.put(hl.holiday_date, hl);
            }
            join_date = null;
            if(userWorkInfo != null && userWorkInfo.m_StateInfoVec != null){
                join_date = userWorkInfo.m_StateInfoVec.get(0).m_Date;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public Vector<Vector<WorkMonth>> workScheduleManagement(String start_date){
        Vector<String> start_end_date;
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
        LocalDate date = LocalDate.parse(start_date, formatter);
        LocalDate end = date.withDayOfMonth(date.lengthOfMonth());
        Vector<Vector<String>> week_date = new Vector<>();
        int i=0;
        while(i < 42) {
            start_end_date = weekStartSundayDate(formatter.format(date));
            week_date.add(start_end_date);
            date = date.plusDays(7);
            i = i + 7;
        }
        Vector<Vector<WorkMonth>> month_data = weekWorkData(week_date);
        return month_data;
    }
    @RequiresApi(api = Build.VERSION_CODES.O)
    public Vector<Vector<WorkMonth>> weekWorkData(Vector<Vector<String>> week_date){
        Vector<Vector<WorkMonth>> month_data = new Vector<>();
        LocalDate localDate = LocalDate.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
        LocalDate joinLocalDate;
        if(join_date == null){
            joinLocalDate = LocalDate.now();
        }else{
            joinLocalDate = LocalDate.parse(join_date, formatter);
        }
        for(int i=0; i<week_date.size(); i++){
            Vector<WorkMonth> row = new Vector<>();
            Vector<String> start_end_date =week_date.get(i);
            for(int j=0; j<start_end_date.size(); j++) {
                WorkMonth wm = new WorkMonth();
                formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
                LocalDate date = LocalDate.parse(start_end_date.get(j), formatter);
                String day = getDateDay(date);
                if(holidays.containsKey(start_end_date.get(j))){
                    wm.day_type = 0;
                    wm.type = 4;
                    wm.status = Objects.requireNonNull(holidays.get(start_end_date.get(j))).Holiday_name;
                }else if(day.equals("토요일") || day.equals("일요일")){
                    wm.day_type = 1;
                    wm.type = 4;
                    wm.status = null;
                }else if(date.isAfter(localDate)){
                    wm.day_type = 3;
                    wm.type = 4;
                    wm.status = null;
                }else if(localDate.isEqual(joinLocalDate) || date.isBefore(joinLocalDate)){
                    wm.day_type = 3;
                    wm.type = 4;
                    wm.status = null;
                }else{
                    wm.day_type = 2;
                    wm.type = 2;
                    wm.status = "결근";
                }
                wm.on_time = null;
                wm.off_time = null;
                if (userWorkInfo.m_StateInfo.containsKey(start_end_date.get(j))) {
                    UserState uw = userWorkInfo.m_StateInfo.get(start_end_date.get(j));
                    if(uw.m_AttendTime != null){
                        wm.on_time = uw.m_AttendTime.substring(0, 5);
                    }
                    if(uw.m_OffTime != null){
                        wm.off_time = uw.m_OffTime.substring(0, 5);
                    }
                    if(uw.m_DayState.equals(UserState.DayType.Vacation) || uw.m_DayState.equals(UserState.DayType.Sick)){
                        wm.type = 3;
                        wm.status = "휴가";
                    }else if(!(uw.m_AttendTime == null)){
                        formatter = DateTimeFormatter.ofPattern("HH:mm:ss");

                        LocalTime on_time= LocalTime.parse(uw.m_AttendTime, formatter);
                        LocalTime setting_time= LocalTime.parse(uw.m_SettedAttendTime, formatter);
                        if(on_time.isAfter(setting_time)) {
                            if(!(uw.m_DayState.equals(UserState.DayType.Holiday))) {
                                wm.status = "지각";
                                wm.type = 1;
                            }
                        }else{
                            if(!(uw.m_DayState.equals(UserState.DayType.Holiday))) {
                                wm.status = "정상출근";
                                wm.type = 0;
                            }
                        }
                    }
                }
                row.add(wm);
            }
            month_data.add(row);
        }
        return month_data;
    }

    public Statistics workManage(String start_date){
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
        LocalDate date = LocalDate.parse(start_date, formatter);
        LocalDate end = date.withDayOfMonth(date.lengthOfMonth());
        Vector<String> week_date = new Vector<>();

        while(date.isBefore(end) || date.isEqual(end)) {
            week_date.add(formatter.format(date));
            date = date.plusDays(1);
        }

        Statistics data = new Statistics();
        int per_day = 8;
        data.duty_work = 0;
        data.extension_work = 0;
        data.predetermined_work = 0;
        data.vacation = 0;

        for(int i=0; i<week_date.size(); i++){
            date = LocalDate.parse(week_date.get(i), formatter);
            String day = getDateDay(date);
            if(!(day.equals("토요일") || day.equals("일요일") || holidays.containsKey(week_date.get(i)))){
                data.duty_work += per_day;
            }
            if (userWorkInfo.m_StateInfo.containsKey(week_date.get(i))) {
                UserState uw = userWorkInfo.m_StateInfo.get(week_date.get(i));
                assert uw != null;
                if(uw.m_DayState.equals(UserState.DayType.Vacation)){
                    data.vacation += uw.m_WorkTime;
                }else if(!(day.equals("토요일") || day.equals("일요일") || holidays.containsKey(week_date.get(i)))){
                    data.extension_work += uw.m_WorkTime;
                }
                data.predetermined_work += uw.m_OverTime;
            }
        }
        return data;
    }
}
