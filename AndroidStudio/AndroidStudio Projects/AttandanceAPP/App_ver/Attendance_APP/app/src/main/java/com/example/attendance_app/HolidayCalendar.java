package com.example.attendance_app;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.Vector;
import java.util.concurrent.ExecutionException;

class Holiday{
    String holiday_date;    // 날짜
    String holiday_year;    // 해당 연도
    String Holiday_name;    // 공휴일명
    String Holiday_type;    // 대체공휴일을 참조하는 공휴일(ex: 한글날), 연휴, 없으면 NULL

    Holiday() {
    }

    protected Vector<Holiday> holidayJsonParsing(String json) {
        Vector<Holiday> list = new Vector<Holiday>();
        try {
            JSONArray jsonObject = new JSONArray(json);
            for (int i = 0; i < jsonObject.length(); i++) {
                Holiday holidays = new Holiday();
                JSONObject holidayObject = jsonObject.getJSONObject(i);

                holidays.holiday_date = holidayObject.getString("holiday");
                holidays.holiday_date = holidays.holiday_date.substring(0, 10);
                holidays.holiday_year = holidayObject.getString("holiday_year");
                holidays.Holiday_type = holidayObject.getString("type");
                if(holidays.Holiday_type.equals("null")){
                    holidays.Holiday_type = null;
                }
                holidays.Holiday_name = holidayObject.getString("name");

                list.add(holidays);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        return list;
    }
}

public class HolidayCalendar {
    static Vector<Holiday> holiday_list = new Vector<>();

    HolidayCalendar() throws ExecutionException, InterruptedException {
        String url = BuildConfig.ec_ip + "/holiday";
        String json = new GetDataJson.GetDataJSON().execute(url).get();
        holiday_list = new Holiday().holidayJsonParsing(json);
    }
}
