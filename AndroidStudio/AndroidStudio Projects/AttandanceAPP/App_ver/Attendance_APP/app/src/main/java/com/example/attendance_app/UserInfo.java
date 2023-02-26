package com.example.attendance_app;

import static com.example.attendance_app.Act_LoginView.user_id;
import static com.example.attendance_app.Act_LoginView.user_type;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.Vector;
import java.util.concurrent.ExecutionException;

public class UserInfo {
    String name;
    String department;
    String position;
    String join_date;

    UserInfo() throws ExecutionException, InterruptedException {
        String url = BuildConfig.ec_ip + "/userallinfo/:" + user_id;
        String json = new GetDataJson.GetDataJSON().execute(url).get();
        userInfoJsonParsing(json);
    }
    protected void userInfoJsonParsing(String json) {
        Vector<Outside> list = new Vector<Outside>();
        try {
            JSONArray jsonObject = new JSONArray(json);
            JSONObject userObject = jsonObject.getJSONObject(0);
            name = userObject.getString("user_nm");
            department = userObject.getString("department_nm");
            int tmp = userObject.getInt("employee");
            switch (tmp){
                case 0:
                    position = "연구원"; break;
                case 1:
                    position = "주임연구원"; break;
                case 2:
                    position = "선임연구원"; break;
                case 3:
                    position = "책임연구원"; break;
                case 4:
                    position = "수석연구원"; break;
                case 5:
                    position = "연구위원"; break;
                case 6:
                    position = "전문연구위원"; break;
                case 7:
                    position = "이사"; break;
                case 8:
                    position = "대표이사"; break;
                default:
                    position = "관리자"; break;
            }
            join_date = userObject.getString("join_date");
            join_date = join_date.substring(0,10);

        } catch (JSONException e) {
            e.printStackTrace();
        }
    }
}
