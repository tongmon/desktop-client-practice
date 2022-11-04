package com.example.attendance_app;

import static com.example.attendance_app.Act_LoginView.user_id;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.Vector;
import java.util.concurrent.ExecutionException;


class AlertStatus{
    String type;
    String start_date;
    int period;
    String status;

    AlertStatus(){}

    public Vector<AlertStatus> alertStatusesJson(String json){
        Vector<AlertStatus> alertStatuses = new Vector<>();
        try {
            JSONArray jsonObject = new JSONArray(json);
            for (int i = 0; i < jsonObject.length(); i++) {
                AlertStatus alert = new AlertStatus();
                JSONObject alertObject = jsonObject.getJSONObject(i);
                alert.type = alertObject.getString("type");
                alert.start_date = alertObject.getString("start_date");
                alert.start_date = alert.start_date.substring(0,10);
                alert.period = alertObject.getInt("period");
                alert.status = alertObject.getString("status");
                alertStatuses.add(alert);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return alertStatuses;
    }
}
public class AlertData {

    AlertData(){}

    public void insertAlert(String user, String type, String start_date, int period, String status) throws ExecutionException, InterruptedException {
        String url =BuildConfig.ec_ip + "/insertalert/:" + user + "/:" + type + "/:"
                + start_date + "/:" + period + "/:" + status;
        String json = new GetDataJson.GetDataJSON().execute(url).get();
    }

    public Vector<AlertStatus> selectAlert() throws ExecutionException, InterruptedException {
        String url =BuildConfig.ec_ip + "/selectalert/:" + user_id;
        String json = new GetDataJson.GetDataJSON().execute(url).get();

        return new AlertStatus().alertStatusesJson(json);
    }

    public void deleteAlert() throws ExecutionException, InterruptedException {
        String url =BuildConfig.ec_ip + "/deletealert/:" + user_id;
        String json = new GetDataJson.GetDataJSON().execute(url).get();
    }
}
