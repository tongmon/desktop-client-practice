package com.example.attendance_app;

import static com.example.attendance_app.Act_LoginView.user_id;
import static com.example.attendance_app.Util_Date.getDateDay;

import android.os.Build;

import androidx.annotation.RequiresApi;

import org.json.JSONArray;
import org.json.JSONObject;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.format.DateTimeFormatter;
import java.util.concurrent.ExecutionException;

// 현재 출근, 퇴근 정보
@RequiresApi(api = Build.VERSION_CODES.O)
class UserNow {
    int attend_state;       // 출근(0), 퇴근(1) 타입 아닐 경우 -1
    int outside_type;       // 외근(0), 교육(1) 타입 아닐 경우 -1
    int business_type;      // 출장(1) 타입 아닐 경우 -1
    int over_type;          // 연장근무(0), 주말근무(1), 공휴일근무(2) 타입 아닐 경우 -1
    int vacation_type;      // 연차(0),월차(1),병가(2) 타입 아닐 경우 -1
    int weekend_type;       // 주말(1) 타입 아닐 경우 -1
    int holiday_type;       // 공휴일(1) 타입 아닐 경우 -1
    int setted_attend_time; // 설정한 출근 시간
    int setted_off_time;    // 설정한 퇴근 시간
    int over_attend_time;   // 연장 근무 시작 시간
    int over_off_time;      // 연장 근무 끝나는 시간
    int must_work_time;     // 하루 근무 시간
    int break_time;         // 하루 쉬는 시간
    String attend_time;     // 출근 시간
    String off_time;        // 퇴근 시간
    Double gps_x;           // x 좌표
    Double gps_y;           // y좌표
    String device_id;       // 비콘 확인

    UserNow(){

    }
    // 현재 출근 퇴근 조회
    UserNow attendOffNow() throws Exception {
        LocalDate now = LocalDate.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
        String getDate = formatter.format(now);

        String url =BuildConfig.ec_ip + "/attendoffnow/:" + user_id + "/:" + getDate;
        String json = new GetDataJson.GetDataJSON().execute(url).get();
        if(json!=null) {
            return attendOffInfoJsonParsing(json, now);
        }
        return null;
    }

    UserNow attendOffInfoJsonParsing(String json, LocalDate now) throws Exception {
        JSONArray jsonObject = new JSONArray(json);
        JSONObject usersObject = jsonObject.getJSONObject(0);
        UserNow user_now = new UserNow();
        String temp = "";
        String weekend = getDateDay(now);
        if(weekend.equals("토요일") || weekend.equals("일요일")){
            user_now.weekend_type = 1;
        }else {
            user_now.weekend_type = -1;
        }
        user_now.device_id = usersObject.getString("device_id");
        user_now.attend_state = usersObject.getInt("attend_state");
        user_now.business_type = usersObject.getInt("business_type");
        user_now.outside_type = usersObject.getInt("outside_type");
        user_now.vacation_type = usersObject.getInt("vacation_type");
        user_now.over_type = usersObject.getInt("over_type");
        temp = usersObject.getString("holiday_name");
        if(!temp.equals("-1")) user_now.holiday_type = 1;
        else user_now.holiday_type = -1;
        user_now.must_work_time = usersObject.getInt("must_work_time");
        user_now.break_time = usersObject.getInt("break_time");
        if(user_now.outside_type != -1){
            temp = usersObject.getString("outside_start_time");
            user_now.setted_attend_time  = Integer.parseInt(temp.substring(temp.length()-8, temp.length()-6));
            temp = usersObject.getString("outside_end_time");
            user_now.setted_off_time  = Integer.parseInt(temp.substring(temp.length()-8, temp.length()-6));
            user_now.gps_x = usersObject.getDouble("outside_x");
            user_now.gps_y = usersObject.getDouble("outside_y");
        }else if(user_now.over_type != -1){
            temp = usersObject.getString("over_start_time");
            user_now.over_attend_time  = Integer.parseInt(temp.substring(temp.length()-8, temp.length()-6));
            temp = usersObject.getString("over_end_time");
            user_now.over_off_time  = Integer.parseInt(temp.substring(temp.length()-8, temp.length()-6));
            if(user_now.over_type == 0){
                user_now.setted_attend_time = usersObject.getInt("setted_attend_time");
                user_now.setted_off_time = usersObject.getInt("setted_off_time");
            } else{
                user_now.setted_attend_time = user_now.over_attend_time;
                user_now.setted_off_time = user_now.over_off_time;
            }
            user_now.gps_x = usersObject.getDouble("over_x");
            user_now.gps_y = usersObject.getDouble("over_y");
        }else {
            user_now.setted_attend_time = usersObject.getInt("setted_attend_time");
            user_now.setted_off_time = usersObject.getInt("setted_off_time");
            if(user_now.business_type == 1){
                user_now.gps_x = usersObject.getDouble("business_x");
                user_now.gps_y = usersObject.getDouble("business_y");
            }else{
                user_now.gps_x = usersObject.getDouble("gps_x");
                user_now.gps_y = usersObject.getDouble("gps_y");
            }
        }
        if(user_now.attend_state == 0){
            temp = usersObject.getString("attend_time");
            user_now.attend_time  = temp.substring(temp.length()-8);
            user_now.off_time = null;
        }else if(user_now.attend_state == 1){
            temp = usersObject.getString("attend_time");
            user_now.attend_time  = temp.substring(temp.length()-8);
            temp = usersObject.getString("off_time");
            user_now.off_time  = temp.substring(temp.length()-8);
        }else{
            user_now.attend_time = null;
            user_now.off_time = null;
        }
        return user_now;
    }
}

@RequiresApi(api = Build.VERSION_CODES.O)
public class AttendOff {
    UserNow userNow;
    int workType;

    AttendOff() throws Exception {
        userNow = new UserNow().attendOffNow();    // 출근, 퇴근 조회
        workType = 1;
        if(userNow.business_type != -1){ workType = 2; }
        else if(userNow.outside_type != -1){
            if(userNow.outside_type == 0) { workType = 4; }
            else if(userNow.outside_type == 1){ workType = 5; }
        }
        else if(userNow.over_type != -1){
            userNow.holiday_type = -1;
            if(userNow.weekend_type == 1) {
                workType = 6;
                userNow.weekend_type = -1;
            }
        }
    }
    public Boolean attendClick(Boolean ret, Double x, Double y, String device_id) throws ExecutionException, InterruptedException {
        if(!(ret) && !userNow.device_id.equals(device_id)) {
            System.out.println(device_id);
            System.out.println(userNow.device_id);
            return false;
        }
        LocalDateTime now = LocalDateTime.now(ZoneId.of("Asia/Seoul"));
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        String getTime = formatter.format(now);
        LocalDate nowDate = LocalDate.now();
        formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
        String getDate = formatter.format(nowDate);
        String day = getDateDay(nowDate);
        userNow.attend_time = getTime.substring(11);
        String url =BuildConfig.ec_ip + "/insertattend/:" + user_id;
        url += "/:0/:" + getTime + "/:" + day + "/:" + getDate + "/:" + workType + "/:" + x + "/:" + y;
        String json = new GetDataJson.GetDataJSON().execute(url).get();
        if((json != null) && !json.equals("undefined")) {
            return true;
        }
        return false;
    }
    public Boolean offClick(Boolean ret, Double x, Double y, String device_id) throws ExecutionException, InterruptedException {
        if(!(ret) && !userNow.device_id.equals(device_id))
            return false;
        LocalDateTime now = LocalDateTime.now(ZoneId.of("Asia/Seoul"));
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        String getTime = formatter.format(now);
        LocalDate nowDate = LocalDate.now();
        formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
        String getDate = formatter.format(nowDate);
        formatter = DateTimeFormatter.ofPattern("HH");
        int tmp = Integer.parseInt(formatter.format(now));
        int attend = Integer.parseInt(userNow.attend_time.substring(0, 2));
        if(userNow.setted_attend_time > attend)
            attend = userNow.setted_attend_time;
        int per_day = tmp - attend - userNow.break_time;
        System.out.println(per_day);
        if(per_day < 0) per_day = 0;
        int over_time = 0;
        if(userNow.over_type != -1){
            over_time = tmp - userNow.over_off_time;
            per_day = per_day - over_time;
            if(over_time < 0)
                over_time = 0;
        }
        userNow.off_time = getTime.substring(11);
        String url =BuildConfig.ec_ip + "/updateoff/:" + user_id;
        url += "/:1/:" + getTime + "/:" + getDate + "/:" + per_day + "/:" + over_time + "/:" + workType + "/:" + x + "/:" + y;
        String json = new GetDataJson.GetDataJSON().execute(url).get();
        if((json != null) && !json.equals("undefined")) {
            return true;
        }
        return false;
    }
}
