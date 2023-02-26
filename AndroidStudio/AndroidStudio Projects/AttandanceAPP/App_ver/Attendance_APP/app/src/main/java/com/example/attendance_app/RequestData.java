package com.example.attendance_app;

import static com.example.attendance_app.Act_LoginView.user_id;
import static com.example.attendance_app.Act_LoginView.user_type;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.Vector;
import java.util.concurrent.ExecutionException;

class Vacation{
    int vacation_type;      // 0 - 연차, 1 - 월차, 2 - 병가
    String name;            // 연차, 월차, 병가
    String vacation_start_date;
    String vacation_end_date;
    int vacation_period;
    String vacation_reason;
    int vacation_permit;    // 0 - 신청, 1 - 허용, 2 - 반려
    String permit_name;     // 신청, 허용, 반려
    String permit_date;
    String user_nm;         // 승인자

    protected Vector<Vacation> vacationListJsonParsing(String json) {
        Vector<Vacation> list = new Vector<Vacation>();
        try {
            JSONArray jsonObject = new JSONArray(json);
            for (int i = 0; i < jsonObject.length(); i++) {
                Vacation vacation = new Vacation();
                JSONObject vacationObject = jsonObject.getJSONObject(i);

                vacation.vacation_type = vacationObject.getInt("vacation_type");
                if(vacation.vacation_type == 0) vacation.name = "연차";
                else if(vacation.vacation_type == 1) vacation.name = "월차";
                else vacation.name = "병가";
                vacation.vacation_start_date = vacationObject.getString("vacation_start_date");
                vacation.vacation_start_date = vacation.vacation_start_date.substring(0,10);
                vacation.vacation_end_date = vacationObject.getString("vacation_end_date");
                vacation.vacation_end_date = vacation.vacation_end_date.substring(0,10);
                vacation.vacation_period = vacationObject.getInt("vacation_period");
                vacation.vacation_reason = vacationObject.optString("vacation_reason", null);
                if(vacation.vacation_reason.equals("null")) vacation.vacation_reason = "-";
                vacation.vacation_permit = vacationObject.getInt("vacation_permit");
                if(vacation.vacation_permit == 0) vacation.permit_name = "신청";
                else if(vacation.vacation_permit == 1) vacation.permit_name = "허용";
                else vacation.permit_name = "반려";
                vacation.permit_date = vacationObject.optString("permit_date", null);
                if(vacation.permit_date.equals("null")) vacation.permit_date = "-";
                else vacation.permit_date = vacation.permit_date.substring(0,10);
                vacation.user_nm = vacationObject.optString("permit_nm", null);
                if(vacation.user_nm.equals("null")) vacation.user_nm = "-";
                list.add(vacation);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        return list;
    }
}

class OverWork{
    int over_type;
    String name;
    String over_start_time;
    String over_end_time;
    String over_reason;
    int over_permit;
    String permit_name;
    String attend_date;
    String permit_date;
    String user_nm;

    protected Vector<OverWork> overworkListJsonParsing(String json) {
        Vector<OverWork> list = new Vector<OverWork>();
        try {
            JSONArray jsonObject = new JSONArray(json);
            for (int i = 0; i < jsonObject.length(); i++) {
                OverWork overWork = new OverWork();
                JSONObject overworkObject = jsonObject.getJSONObject(i);

                overWork.over_type = overworkObject.getInt("over_type");
                if(overWork.over_type == 0) overWork.name = "연장 근무";
                else if(overWork.over_type == 1) overWork.name = "주말 근무";
                else overWork.name = "공휴일 근무";
                overWork.over_start_time = overworkObject.getString("over_start_time");
                overWork.over_start_time = overWork.over_start_time.substring(11, 19);
                overWork.over_end_time = overworkObject.getString("over_end_time");
                overWork.over_end_time = overWork.over_end_time.substring(11, 19);
                overWork.over_reason = overworkObject.optString("over_reason", null);
                if(overWork.over_reason.equals("null")) overWork.over_reason = "-";
                overWork.over_permit = overworkObject.getInt("over_permit");
                if(overWork.over_permit == 0) overWork.permit_name = "신청";
                else if(overWork.over_permit == 1) overWork.permit_name = "허용";
                else overWork.permit_name = "반려";
                overWork.attend_date = overworkObject.getString("attend_date");
                overWork.attend_date = overWork.attend_date.substring(0,10);
                overWork.permit_date = overworkObject.optString("permit_date", null);
                if(overWork.permit_date.equals("null")) overWork.permit_date = "-";
                else overWork.permit_date = overWork.permit_date.substring(0,10);
                overWork.user_nm = overworkObject.optString("permit_nm", null);
                if(overWork.user_nm.equals("null")) overWork.user_nm = "-";
                list.add(overWork);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return list;
    }
}

class Outside{
    int outside_type;
    String name;
    String outside_start_time;
    String outside_end_time;
    String outside_reason;
    int outside_permit;
    String permit_name;
    String outside_date;
    String permit_date;
    String user_nm;

    protected Vector<Outside> outsideListJsonParsing(String json) {
        Vector<Outside> list = new Vector<Outside>();
        try {
            JSONArray jsonObject = new JSONArray(json);
            for (int i = 0; i < jsonObject.length(); i++) {
                Outside outside = new Outside();
                JSONObject outsideObject = jsonObject.getJSONObject(i);

                outside.outside_type = outsideObject.getInt("outside_type");
                if(outside.outside_type == 0) outside.name = "외근";
                else outside.name = "교육";
                outside.outside_start_time = outsideObject.getString("outside_start_time");
                outside.outside_start_time = outside.outside_start_time.substring(11, 19);
                outside.outside_end_time = outsideObject.getString("outside_end_time");
                outside.outside_end_time = outside.outside_end_time.substring(11, 19);
                outside.outside_reason = outsideObject.optString("outside_reason", null);
                if(outside.outside_reason.equals("null")) outside.outside_reason = "-";
                outside.outside_permit = outsideObject.getInt("outside_permit");
                if(outside.outside_permit == 0) outside.permit_name = "신청";
                else if(outside.outside_permit == 1) outside.permit_name = "허용";
                else outside.permit_name = "반려";
                outside.outside_date = outsideObject.getString("outside_date");
                outside.outside_date = outside.outside_date.substring(0,10);
                outside.permit_date = outsideObject.optString("permit_date", null);
                if(outside.permit_date.equals("null")) outside.permit_date = "-";
                else outside.permit_date = outside.permit_date.substring(0,10);
                outside.user_nm = outsideObject.optString("permit_nm", null);
                if(outside.user_nm.equals("null")) outside.user_nm = "-";
                list.add(outside);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return list;
    }
}

class Business{
    String business_start_date;
    String business_end_date;
    String business_reason;
    int business_period;
    int business_permit;
    String permit_date;
    String permit_name;
    String user_nm;

    protected Vector<Business> businessListJsonParsing(String json) {
        Vector<Business> list = new Vector<Business>();
        try {
            JSONArray jsonObject = new JSONArray(json);
            for (int i = 0; i < jsonObject.length(); i++) {
                Business business = new Business();
                JSONObject businessObject = jsonObject.getJSONObject(i);

                business.business_start_date = businessObject.getString("business_start_date");
                business.business_start_date = business.business_start_date.substring(0,10);
                business.business_end_date = businessObject.getString("business_end_date");
                business.business_end_date = business.business_end_date.substring(0,10);
                business.business_reason = businessObject.optString("business_reason", null);
                if(business.business_reason.equals("null")) business.business_reason = "-";
                business.business_period = businessObject.getInt("business_period");
                business.business_permit = businessObject.getInt("business_permit");
                if(business.business_permit == 0) business.permit_name = "신청";
                else if(business.business_permit == 1) business.permit_name = "허용";
                else business.permit_name = "반려";
                business.permit_date = businessObject.optString("permit_date", null);
                if(business.permit_date.equals("null")) business.permit_date = "-";
                else business.permit_date = business.permit_date.substring(0,10);
                business.user_nm = businessObject.optString("permit_nm", null);
                if(business.user_nm.equals("null")) business.user_nm = "-";
                list.add(business);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return list;
    }


}

public class RequestData {
    RequestData(){}

    public Vector<Vacation> vacationList() throws ExecutionException, InterruptedException {
        String url =BuildConfig.ec_ip + "/vacationselect/:" + user_id;
        String json = new GetDataJson.GetDataJSON().execute(url).get();
        Vector<Vacation> list = new Vacation().vacationListJsonParsing(json);

        return list;
    }

    public Vector<OverWork> overWorkList() throws ExecutionException, InterruptedException {
        String url = BuildConfig.ec_ip + "/overworkselect/:" + user_id;

        String json = new GetDataJson.GetDataJSON().execute(url).get();
        Vector<OverWork> list = new OverWork().overworkListJsonParsing(json);

        return list;
    }

    public Vector<Outside> outsideList() throws ExecutionException, InterruptedException {
        String url = BuildConfig.ec_ip + "/outsideselect/:" + user_id;

        String json = new GetDataJson.GetDataJSON().execute(url).get();
        Vector<Outside> list = new Outside().outsideListJsonParsing(json);

        return list;
    }

    public Vector<Business> businessList() throws ExecutionException, InterruptedException {
        String url = BuildConfig.ec_ip + "/businessselect/:" + user_id;

        String json = new GetDataJson.GetDataJSON().execute(url).get();
        Vector<Business> list = new Business().businessListJsonParsing(json);

        return list;
    }
}
