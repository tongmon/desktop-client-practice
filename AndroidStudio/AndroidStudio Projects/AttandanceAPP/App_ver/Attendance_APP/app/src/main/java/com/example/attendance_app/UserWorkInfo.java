package com.example.attendance_app;

import static com.example.attendance_app.Act_LoginView.user_id;
import static com.example.attendance_app.Util_Date.getDateDay;

import android.os.Build;

import androidx.annotation.RequiresApi;

import org.json.JSONArray;
import org.json.JSONObject;

import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.HashMap;
import java.util.Vector;
import java.util.concurrent.ExecutionException;

// 테이블의 한 튜플이라고 보면 됨
class UserState {
    enum DayType {
        Normal, // 정상 근무
        Trip, // 출장
        Vacation, // 휴가
        Outside, // 외근
        Education, // 교육
        WeekEnd, // 주말
        Holiday, // 공휴일
        Sick, // 병가
        EndType // 미출근
    }
    String m_SettedAttendTime; // 설정된 유저의 출근 시간 ex) 8시, 9시
    String m_SettedOffTime; // 설정된 유저의 퇴근 시간 ex) 17시, 18시
    String m_AttendTime; // 출근 시각
    String m_OffTime; // 퇴근 시각
    int m_OverTime; // 연장 근로 시간
    float m_OverPercent; // 연장 근로 퍼센트
    int m_WorkTime; // 소정 근로 시간
    float m_WorkPercent; // 소정 근로 퍼센트
    String m_Day; // 출근 요일
    String m_Date; // 출근 날짜
    DayType m_DayState; // 외근인지 출장인지 이런거...
    String m_over_type; // 연장 근로 상태
    String m_OverStartTime; // 연장 근무 시작 시간
    String m_OverEndTime;   // 연장 근무 종료 시각
}

// 사용자 근무 상태에 대한 테이블
// 하루 근무 시간이 몇 시간인지 테이블에 따로 넣어두긴 해야댐, 밑에 왜 그런지 주석 있음
@RequiresApi(api = Build.VERSION_CODES.O)
public class UserWorkInfo {
    HashMap<String, UserState> m_StateInfo; // 해시 테이블, 날짜와 UserState 매칭, 곧 날짜로 유일한 튜플에 접근 가능해야 됨.
    Vector<UserState> m_StateInfoVec; // 사용자 근무 정보 집합
    Vector<UserState> m_StateWeekVec;
    Vector<Integer> m_TotalCntData; // 총 교육, 외근, 출장 일 등... 컨테이너
    int m_MustWorkTime; // 의무 근로 시간
    int m_TotalWorkTime; // 총 근무 시간
    int m_WorkTimePerDay; // 하루 근무 시간 (8시간이라고 가정)
    String json;

    // 유저 키로 근무 테이블 접근
    // DB 연동 부분
    UserWorkInfo() throws ExecutionException, InterruptedException {
        m_MustWorkTime = m_TotalWorkTime = 0;
        m_WorkTimePerDay = 8; // 하루 근무 시간에 대한 변수인데 DB에 넣어야 좋을 듯, 월별, 년도별 의무 근로 시간 체크할 때 사용됨
        m_StateInfo = new HashMap<>();
        m_StateInfoVec = new Vector<UserState>();
        m_StateWeekVec = new Vector<UserState>();
        m_TotalCntData = new Vector<Integer>();
        for (int i = 0; i < UserState.DayType.EndType.ordinal(); i++)
            m_TotalCntData.add(0);

        // DB 연동
        String url =BuildConfig.ec_ip + "/userstate/:" + user_id;
        json = new GetDataJson.GetDataJSON().execute(url).get();
        userWorkInfoJsonParsing(json);
        // DB 연동
    }

    // 일간 근무 현황 일간 근무 전체 현황(정상근무, 근무한 날짜, 근무 요일), 상태, 출근, 근무시작시각, 퇴근, 퇴근시작시각, 연장근로상태, 소정근로, 연장근로
    protected void userWorkInfoJsonParsing(String json) throws ExecutionException, InterruptedException {
        try {
            JSONArray jsonObject = new JSONArray(json);
            for (int i = 0; i < jsonObject.length(); i++) {
                UserState user = new UserState();
                JSONObject usersObject = jsonObject.getJSONObject(i);
                // 상태
                int work_type = usersObject.getInt("work_type");
                if (work_type == 1) user.m_DayState = UserState.DayType.Normal;
                else if (work_type == 2) user.m_DayState = UserState.DayType.Trip;
                else if (work_type == 3) user.m_DayState = UserState.DayType.Vacation;
                else if (work_type == 4) user.m_DayState = UserState.DayType.Outside;
                else if (work_type == 5) user.m_DayState = UserState.DayType.Education;
                else if (work_type == 6) user.m_DayState = UserState.DayType.WeekEnd;
                else if (work_type == 7) user.m_DayState = UserState.DayType.Holiday;
                else if (work_type == 8) user.m_DayState = UserState.DayType.Sick;
                else user.m_DayState = UserState.DayType.EndType;
                // 근무한 날짜
                String temp = usersObject.getString("attend_date");
                user.m_Date = temp.substring(0, 10);
                // 근무한 요일
                user.m_Day = usersObject.getString("attend_day");
                // 출근 시각
                temp = usersObject.getString("attend_time");
                user.m_AttendTime = temp.substring(temp.length()-13, temp.length()-5);
                // 근무 시작 시각
                int set;
                if(work_type == 4 || work_type == 5){
                    temp = usersObject.getString("outside_start_time");
                    user.m_SettedAttendTime = temp.substring(temp.length()-8);
                } else if(work_type == 6){
                    temp = usersObject.getString("over_start_time");
                    user.m_SettedAttendTime = temp.substring(temp.length()-8);
                } else {
                    set = usersObject.getInt("setted_attend_time");
                    if (set < 10) {
                        user.m_SettedAttendTime = "0" + set + ":00:00";
                    } else {
                        user.m_SettedAttendTime = set + ":00:00";
                    }
                }
                // 퇴근 시각
                temp = usersObject.optString("off_time", null);
                if(temp == "null"){
                    user.m_OffTime = null;
                }else {
                    user.m_OffTime = temp.substring(temp.length() - 13, temp.length() - 5);
                }
                // 근무 퇴근 시각
                if(work_type == 4 || work_type == 5){
                    temp = usersObject.getString("outside_end_time");
                    user.m_SettedOffTime = temp.substring(temp.length()-8);
                } else if(work_type == 6){
                    temp = usersObject.getString("over_end_time");
                    user.m_SettedOffTime = temp.substring(temp.length()-8);
                } else{
                    set = usersObject.getInt("setted_off_time");
                    if (set < 10) {
                        user.m_SettedOffTime = "0" + set + ":00:00";
                    } else {
                        user.m_SettedOffTime = set + ":00:00";
                    }
                }
                // 연장 근로 상태
                if(user.m_DayState == UserState.DayType.Normal){
                    set = usersObject.getInt("over_type");
                    if (set == 1) {
                        user.m_over_type = "신청";
                        temp = usersObject.getString("over_start_time");
                        user.m_OverStartTime = temp.substring(temp.length()-8);
                        temp = usersObject.getString("over_end_time");
                        user.m_OverEndTime = temp.substring(temp.length()-8);
                    }
                    else {
                        user.m_over_type = "미신청";
                        user.m_OverStartTime = "-";
                        user.m_OverEndTime = "-";
                    }
                }else{
                    user.m_over_type = "미신청";
                    user.m_OverStartTime = "-";
                    user.m_OverEndTime = "-";
                }
                // 소정 근로
                user.m_WorkTime = usersObject.optInt("work_time_per_day", 0);
                if(user.m_WorkTime != 0){
                    user.m_WorkPercent = (float)user.m_WorkTime /(float)m_WorkTimePerDay * 100;
                }else{
                    user.m_WorkPercent = 0;
                }
                if(user.m_WorkTime > 100)
                    user.m_WorkPercent = 100.0F;
                user.m_WorkTime = m_WorkTimePerDay;
                // 연장 근로
                user.m_OverTime = usersObject.optInt("over_time", 0);
                if(user.m_OverTime != 0){
                    user.m_OverPercent = (float)user.m_OverTime / (float)m_WorkTimePerDay * 100;
                }else{
                    user.m_OverPercent = 0;
                }
                if(user.m_OverTime > 100)
                    user.m_OverPercent = 100.0F;
                m_StateInfo.put(user.m_Date, user);
            }
            LocalDate now = LocalDate.now();
            DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
            String getTime = formatter.format(now);
        } catch (Exception e) {
            e.printStackTrace();
        }
        allDay();
    }

    public void allDay() throws ExecutionException, InterruptedException {
        UserInfo us = new UserInfo();
        Vector<String> start_end_date = new Vector<>();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
        LocalDate date = LocalDate.parse(us.join_date, formatter);
        LocalDate end = LocalDate.now();
        HashMap<String, Holiday> holidays = new HashMap<>();
        for(Holiday hl : HolidayCalendar.holiday_list){
            holidays.put(hl.holiday_date, hl);
        }

        while(date.isBefore(end) || date.isEqual(end)) {
            String result = formatter.format(date);
            start_end_date.add(result);
            date = date.plusDays(1);
        }
        if(m_StateInfo.size() > 0) {
            for (int i = 0; i < start_end_date.size(); i++) {
                formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
                date = LocalDate.parse(start_end_date.get(i), formatter);
                String day = getDateDay(date);
                if (m_StateInfo.containsKey(start_end_date.get(i))) {
                    UserState uw = m_StateInfo.get(start_end_date.get(i));
                    m_StateInfoVec.add(uw);
                }else{
                    UserState user = new UserState();
                    if(holidays.containsKey(start_end_date.get(i))) {
                        user.m_DayState = UserState.DayType.Holiday;
                    }else if(day.equals("토요일") || day.equals("일요일")){
                        user.m_DayState = UserState.DayType.WeekEnd;
                    }else{
                        user.m_DayState = UserState.DayType.EndType;
                    }
                    user.m_over_type = "미신청";
                    user.m_Date = start_end_date.get(i);
                    user.m_Day = day;
                    user.m_SettedAttendTime = null;
                    user.m_SettedOffTime = null;
                    user.m_AttendTime = null;
                    user.m_OffTime = null;
                    user.m_OverTime = 0;
                    user.m_OverPercent = 0;
                    user.m_WorkTime = 0;
                    user.m_WorkPercent = 0;
                    m_StateInfoVec.add(user);
                }
            }
        }else{
            for (int i = 0; i < start_end_date.size(); i++) {
                formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
                date = LocalDate.parse(start_end_date.get(i), formatter);
                String day = getDateDay(date);
                UserState user = new UserState();
                if(holidays.containsKey(start_end_date.get(i))) {
                    user.m_DayState = UserState.DayType.Holiday;
                }else if(day.equals("토요일") || day.equals("일요일")){
                    user.m_DayState = UserState.DayType.WeekEnd;
                }else{
                    user.m_DayState = UserState.DayType.EndType;
                }
                user.m_over_type = "미신청";
                user.m_Date = start_end_date.get(i);
                user.m_Day = day;
                user.m_SettedAttendTime = null;
                user.m_SettedOffTime = null;
                user.m_AttendTime = null;
                user.m_OffTime = null;
                user.m_OverTime = 0;
                user.m_OverPercent = 0;
                user.m_WorkTime = 0;user.m_WorkPercent = 0;
                m_StateInfoVec.add(user);
            }
        }

    }
}
