package com.example.attendance_app;

import static com.example.attendance_app.Act_LoginView.user_id;
import static com.example.attendance_app.Act_LoginView.user_type;

import android.os.Build;

import androidx.annotation.RequiresApi;

import org.json.JSONArray;
import org.json.JSONObject;

import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.Vector;
import java.util.concurrent.ExecutionException;

class Accept{
    int num;
    String user_id;
    String type;
    String user_nm;
    String start_date;
    String draft_date;
    String permit_nm;
    String permit_date;
    String permit;
    String address;
    String reason;
    int period;

    Accept(){}

    public Vector<Accept> acceptVacationJson(String json){
        Vector<Accept> accepts = new Vector<>();
        try {
            JSONArray jsonObject = new JSONArray(json);
            for (int i = 0; i < jsonObject.length(); i++) {
                Accept accept = new Accept();
                JSONObject vacationObject = jsonObject.getJSONObject(i);
                accept.num = vacationObject.getInt("num");
                accept.period = vacationObject.getInt("vacation_period");
                accept.user_id = vacationObject.getString("user_id");
                int t = vacationObject.getInt("vacation_type");
                if(t == 0) accept.type = "연차";
                else if(t == 1) accept.type = "월차";
                else if(t == 2) accept.type = "병가";
                else accept.type = "대체";
                accept.user_nm = vacationObject.getString("user_nm");
                accept.start_date = vacationObject.getString("vacation_start_date");
                accept.start_date = accept.start_date.substring(0,10);
                accept.draft_date = vacationObject.getString("draft_date");
                accept.draft_date = accept.draft_date.substring(0,10);
                accept.permit_nm = vacationObject.optString("permit_nm", null);
                if(accept.permit_nm.equals("null")) accept.permit_nm = "-";
                accept.permit_date = vacationObject.optString("permit_date", null);
                if(accept.permit_date.equals("null")) accept.permit_date = "-";
                else accept.permit_date = accept.permit_date.substring(0,10);
                t = vacationObject.getInt("vacation_permit");
                if(t == 0) accept.permit = "신청";
                else if(t == 1) accept.permit = "허용";
                else if(t == 2) accept.permit = "반려";
                accept.address = "-";
                accept.reason = vacationObject.optString("vacation_reason", null);
                if(accept.reason.equals("null")) accept.reason = "-";
                accepts.add(accept);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return accepts;
    }

    public Vector<Accept> acceptOverJson(String json){
        Vector<Accept> accepts = new Vector<>();
        try {
            JSONArray jsonObject = new JSONArray(json);
            for (int i = 0; i < jsonObject.length(); i++) {
                Accept accept = new Accept();
                JSONObject overObject = jsonObject.getJSONObject(i);
                accept.num = overObject.getInt("num");
                accept.period = 1;
                accept.user_id = overObject.getString("user_id");
                int t = overObject.getInt("over_type");
                if(t == 0) accept.type = "연장근무";
                else if(t == 1) accept.type = "주말근무";
                else if(t == 2) accept.type = "공휴일근무";
                accept.user_nm = overObject.getString("user_nm");
                accept.start_date = overObject.getString("attend_date");
                accept.start_date = accept.start_date.substring(0,10);
                accept.draft_date = overObject.getString("draft_date");
                accept.draft_date = accept.draft_date.substring(0,10);
                accept.permit_nm = overObject.optString("permit_nm", null);
                if(accept.permit_nm.equals("null")) accept.permit_nm = "-";
                accept.permit_date = overObject.optString("permit_date", null);
                if(accept.permit_date.equals("null")) accept.permit_date = "-";
                else accept.permit_date = accept.permit_date.substring(0,10);
                t = overObject.getInt("over_permit");
                if(t == 0) accept.permit = "신청";
                else if(t == 1) accept.permit = "허용";
                else if(t == 2) accept.permit = "반려";
                accept.address = overObject.optString("address", null);
                if(accept.address.equals("null")) accept.address = "-";
                accept.reason = overObject.optString("over_reason", null);
                if(accept.reason.equals("null")) accept.reason = "-";
                accepts.add(accept);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return accepts;
    }

    public Vector<Accept> acceptOutJson(String json){
        Vector<Accept> accepts = new Vector<>();
        try {
            JSONArray jsonObject = new JSONArray(json);
            for (int i = 0; i < jsonObject.length(); i++) {
                Accept accept = new Accept();
                JSONObject outObject = jsonObject.getJSONObject(i);
                accept.num = outObject.getInt("num");
                accept.period = 1;
                accept.user_id = outObject.getString("user_id");
                int t = outObject.getInt("outside_type");
                if(t == 0) accept.type = "외근";
                else if(t == 1) accept.type = "교육";
                accept.user_nm = outObject.getString("user_nm");
                accept.start_date = outObject.getString("outside_date");
                accept.start_date = accept.start_date.substring(0,10);
                accept.draft_date = outObject.getString("draft_date");
                accept.draft_date = accept.draft_date.substring(0,10);
                accept.permit_nm = outObject.optString("permit_nm", null);
                if(accept.permit_nm.equals("null")) accept.permit_nm = "-";
                accept.permit_date = outObject.optString("permit_date", null);
                if(accept.permit_date.equals("null")) accept.permit_date = "-";
                else accept.permit_date = accept.permit_date.substring(0,10);
                t = outObject.getInt("outside_permit");
                if(t == 0) accept.permit = "신청";
                else if(t == 1) accept.permit = "허용";
                else if(t == 2) accept.permit = "반려";
                accept.address = outObject.optString("address", null);
                if(accept.address.equals("null")) accept.address = "-";
                accept.reason = outObject.optString("outside_reason", null);
                if(accept.reason.equals("null")) accept.reason = "-";
                accepts.add(accept);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return accepts;
    }

    public Vector<Accept> acceptBusinessJson(String json){
        Vector<Accept> accepts = new Vector<>();
        try {
            JSONArray jsonObject = new JSONArray(json);
            for (int i = 0; i < jsonObject.length(); i++) {
                Accept accept = new Accept();
                JSONObject businessObject = jsonObject.getJSONObject(i);
                accept.num = businessObject.getInt("num");
                accept.period = businessObject.getInt("business_period");
                accept.user_id = businessObject.getString("user_id");
                accept.type = "출장";
                accept.user_nm = businessObject.getString("user_nm");
                accept.start_date = businessObject.getString("business_start_date");
                accept.start_date = accept.start_date.substring(0,10);
                accept.draft_date = businessObject.getString("draft_date");
                accept.draft_date = accept.draft_date.substring(0,10);
                accept.permit_nm = businessObject.optString("permit_nm", null);
                if(accept.permit_nm.equals("null")) accept.permit_nm = "-";
                accept.permit_date = businessObject.optString("permit_date", null);
                if(accept.permit_date.equals("null")) accept.permit_date = "-";
                else accept.permit_date = accept.permit_date.substring(0,10);
                int t = businessObject.getInt("business_permit");
                if(t == 0) accept.permit = "신청";
                else if(t == 1) accept.permit = "허용";
                else if(t == 2) accept.permit = "반려";
                accept.address = businessObject.optString("address", null);
                if(accept.address.equals("null")) accept.address = "-";
                accept.reason = businessObject.optString("business_reason", null);
                if(accept.reason.equals("null")) accept.reason = "-";
                accepts.add(accept);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return accepts;
    }
}
@RequiresApi(api = Build.VERSION_CODES.O)
public class AcceptData {
    AcceptData(){}

    public Vector<Accept> vacationAcceptList() throws ExecutionException, InterruptedException {
        String url =BuildConfig.ec_ip + "/recognizeselectvacation";
        String json = new GetDataJson.GetDataJSON().execute(url).get();

        return new Accept().acceptVacationJson(json);
    }

    public Vector<Accept> overAcceptList() throws ExecutionException, InterruptedException {
        String url =BuildConfig.ec_ip + "/recognizeselectover";
        String json = new GetDataJson.GetDataJSON().execute(url).get();

        return new Accept().acceptOverJson(json);
    }

    public Vector<Accept> outAcceptList() throws ExecutionException, InterruptedException {
        String url =BuildConfig.ec_ip + "/recognizeselectoutside";
        String json = new GetDataJson.GetDataJSON().execute(url).get();

        return new Accept().acceptOutJson(json);
    }

    public Vector<Accept> businessAcceptList() throws ExecutionException, InterruptedException {
        String url =BuildConfig.ec_ip + "/recognizeselectbusiness";
        String json = new GetDataJson.GetDataJSON().execute(url).get();

        return new Accept().acceptBusinessJson(json);
    }

    public void vacationAcceptUpdate(int num, int permit, int period, String user, String type) throws ExecutionException, InterruptedException {
        LocalDate now = LocalDate.now();
        DateTimeFormatter formatter;
        String date;

        VacationInfo data = Util_Vacation.getVacationInt(user);
        if(permit == 1) {
            if (type.equals("연차")) data.vacation_use_year =data.vacation_use_year + period;
            else if (type.equals("월차")) data.vacation_use_month = data.vacation_use_month + period;
            else data.vacation_use_replace = data.vacation_use_replace + period;
            data.remaining_day = data.remaining_day - period;
            formatter = DateTimeFormatter.ofPattern("yyyy");
            date = formatter.format(now);
            String url =BuildConfig.ec_ip + "/vacationupdate/:" + user + "/:" + date + "/:" +
                    data.vacation_use_year + "/:" + data.vacation_use_month + "/:" +
                    data.vacation_use_replace + "/:" + data.remaining_day;
            String json = new GetDataJson.GetDataJSON().execute(url).get();
            System.out.println(url);
        }
        formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
        date = formatter.format(now);
        String url =BuildConfig.ec_ip + "/recognizeupdatevacation/:" + num + "/:" + permit + "/:" + date + "/:" +
                user_id + "/:" + data.vacation_use_year + "/:" + data.vacation_use_month + "/:" + data.vacation_use_replace;
        String json = new GetDataJson.GetDataJSON().execute(url).get();
    }

    public void overAcceptUpdate(int num, int permit) throws ExecutionException, InterruptedException {
        LocalDate now = LocalDate.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
        String date = formatter.format(now);
        String url =BuildConfig.ec_ip + "/recognizeupdateover/:" + num + "/:" + permit + "/:" + date + "/:" + user_id;
        String json = new GetDataJson.GetDataJSON().execute(url).get();
    }

    public void outAcceptUpdate(int num, int permit) throws ExecutionException, InterruptedException {
        LocalDate now = LocalDate.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
        String date = formatter.format(now);
        String url =BuildConfig.ec_ip + "/recognizeupdateoutside/:" + num + "/:" + permit + "/:" + date + "/:" + user_id;
        String json = new GetDataJson.GetDataJSON().execute(url).get();
    }

    public void businessAcceptUpdate(int num, int permit) throws ExecutionException, InterruptedException {
        LocalDate now = LocalDate.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
        String date = formatter.format(now);
        String url =BuildConfig.ec_ip + "/recognizeupdatebusiness/:" + num + "/:" + permit + "/:" + date + "/:" + user_id;
        String json = new GetDataJson.GetDataJSON().execute(url).get();
    }
}
