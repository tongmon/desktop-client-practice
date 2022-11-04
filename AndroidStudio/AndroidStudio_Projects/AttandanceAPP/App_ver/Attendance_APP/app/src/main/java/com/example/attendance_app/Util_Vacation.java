package com.example.attendance_app;


import static com.example.attendance_app.Act_LoginView.user_id;
import static com.example.attendance_app.Act_LoginView.user_type;
import static com.example.attendance_app.Act_SubWorkManager.select;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.Vector;
import java.util.concurrent.ExecutionException;

// 휴가 현황
class VacationInfo{
    String name;                 // 사원명
    String position;             // 직급
    String department;           // 부서
    String join_date;             // 입사일
    int vacation_year;           // 연차
    int vacation_use_year;       // 사용 연차
    int vacation_month;          // 월차
    int vacation_use_month;      // 사용 월차
    int vacation_replace;        // 대체
    int vacation_use_replace;    // 사용 대체
    int remaining_day;          // 잔여일수
    String state_select_year;   // 연도 추출

    VacationInfo() {
    }
    protected Vector<VacationInfo> vacationInfoJsonParsing(String json) {
        Vector<VacationInfo> list = new Vector<VacationInfo>();
        try {
            JSONArray jsonObject = new JSONArray(json);
            for (int i = 0; i < jsonObject.length(); i++) {
                VacationInfo info = new VacationInfo();
                JSONObject infoObject = jsonObject.getJSONObject(i);

                info.name = infoObject.getString("user_nm");
                int employee = infoObject.getInt("employee");
                if(employee == 1) info.position = "주임연구원";
                else if(employee == 2) info.position = "선임연구원";
                else if(employee == 3) info.position = "책임연구원";
                else if(employee == 4) info.position = "수석연구원";
                else if(employee == 5) info.position = "연구위원";
                else if(employee == 6) info.position = "전문연구위원";
                else if(employee == 7) info.position = "상무";
                else if(employee == 8) info.position = "대표이사";
                else info.position = "연구원";
                info.department = infoObject.getString("department_nm");
                info.join_date = infoObject.getString("join_date");
                info.join_date = info.join_date.substring(0, 10);
                info.vacation_year = infoObject.getInt("vacation_year");
                info.vacation_use_year = infoObject.getInt("use_vacation_year");
                info.vacation_month = infoObject.getInt("vacation_month");
                info.vacation_use_month = infoObject.getInt("use_vacation_month");
                info.vacation_replace = infoObject.getInt("vacation_replace");
                info.vacation_use_replace = infoObject.getInt("use_vacation_replace");
                info.remaining_day = infoObject.getInt("remaining_day");
                info.state_select_year = infoObject.getString("get_year");

                list.add(info);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return list;
    }

}

// 휴가 사용 내역
class VacationStateList{
    int row;                     // 번호
    String section;              // 구분
    String application_date;     // 적용일
    String start_date;           // 시작일
    String end_date;             // 종료일
    int vacation_year;           // 연차
    int vacation_use_year;       // 사용 연차
    int vacation_month;          // 월차
    int vacation_use_month;      // 사용 월차
    int vacation_replace;        // 대체
    int vacation_use_replace;    // 사용 대체
    String memo;                 // 내용
    int permit;                  // 휴가 허용 여부
    String vacation_reason;      // 휴가 사유
    String use_select_year;      // 연도 추출

    VacationStateList() {
    }

    protected Vector<VacationStateList> vacationStateListJsonParsing(String json) {
        Vector<VacationStateList> list = new Vector<VacationStateList>();
        try {
            JSONArray jsonObject = new JSONArray(json);
            for (int i = 0; i < jsonObject.length(); i++) {
                VacationStateList vacation = new VacationStateList();
                JSONObject vacationObject = jsonObject.getJSONObject(i);

                vacation.row = vacationObject.getInt("row_num");
                int type = vacationObject.getInt("vacation_type");
                if(type == 0) vacation.section = "연차";
                else if(type == 1) vacation.section = "월차";
                else if(type == 2) vacation.section = "병가";
                else vacation.section = "대체";
                vacation.application_date = vacationObject.getString("permit_date");
                if(!vacation.application_date.equals("null")){
                    vacation.application_date = vacation.application_date.substring(0, 10);
                }else{
                    vacation.application_date = null;
                }
                vacation.start_date = vacationObject.getString("vacation_start_date");
                vacation.start_date = vacation.start_date.substring(0, 10);
                vacation.end_date = vacationObject.getString("vacation_end_date");
                vacation.end_date = vacation.end_date.substring(0, 10);
                vacation.vacation_year = vacationObject.getInt("now_year");
                vacation.vacation_use_year = vacationObject.getInt("now_use_year");
                vacation.vacation_month = vacationObject.getInt("now_month");
                vacation.vacation_use_month = vacationObject.getInt("now_use_month");
                vacation.vacation_replace = vacationObject.getInt("now_replace");
                vacation.vacation_use_replace = vacationObject.getInt("now_use_replace");
                vacation.memo = vacationObject.getString("memo");
                if(vacation.memo.equals("null")){
                    vacation.memo = null;
                }
                vacation.permit = vacationObject.getInt("vacation_permit");
                vacation.vacation_reason = vacationObject.getString("vacation_reason");
                if(vacation.vacation_reason.equals("null")){
                    vacation.vacation_reason = null;
                }
                vacation.use_select_year = vacation.start_date.substring(0,4);

                list.add(vacation);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        return list;
    }
}

public class Util_Vacation {
    static Vector<VacationInfo> vacation_info = new Vector<>();
    static Vector<VacationStateList> vacation_state_list = new Vector<>();

    Util_Vacation() throws ExecutionException, InterruptedException {
        if(select == "state"){
            vacation_info = getVacationInfo();
            vacation_state_list = getVacationStateList();
        }
    }

    Vector<VacationInfo> getVacationInfo() throws ExecutionException, InterruptedException {
        String url;
        if(user_type == 1){
            url =BuildConfig.ec_ip + "/vacationall";
        }else{
            url =BuildConfig.ec_ip + "/vacationinfo/:" + user_id;
        }
        String json = new GetDataJson.GetDataJSON().execute(url).get();
        Vector<VacationInfo> list = new VacationInfo().vacationInfoJsonParsing(json);

        return list;
    }

    public static VacationInfo getVacationInt(String user) throws ExecutionException, InterruptedException {
        String url =BuildConfig.ec_ip + "/vacationinfo/:" + user;

        String json = new GetDataJson.GetDataJSON().execute(url).get();
        Vector<VacationInfo> list = new VacationInfo().vacationInfoJsonParsing(json);
        VacationInfo last = null;
        if(list.size() > 0) {
            last = list.get(0);
        }
        return last;
    }

    Vector<VacationStateList> getVacationStateList() throws ExecutionException, InterruptedException {
        String url;
        if(select.equals("state")) {
            url = BuildConfig.ec_ip + "/vacationstate/:" + user_id;
        }else{
            url = BuildConfig.ec_ip + "/vacationsign/:" + user_id;
        }
        String json = new GetDataJson.GetDataJSON().execute(url).get();
        Vector<VacationStateList> vacation_state = new VacationStateList().vacationStateListJsonParsing(json);

        return vacation_state;
    }
}
