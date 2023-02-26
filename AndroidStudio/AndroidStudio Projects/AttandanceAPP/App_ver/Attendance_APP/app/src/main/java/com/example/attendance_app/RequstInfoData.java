package com.example.attendance_app;

import static com.example.attendance_app.Act_LoginView.user_id;

import android.os.Build;

import androidx.annotation.RequiresApi;

import java.time.Duration;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.concurrent.ExecutionException;

@RequiresApi(api = Build.VERSION_CODES.O)
public class RequstInfoData {
    RequstInfoData(){}
    public void insertVacatonStatus(int type, String start_date, String end_date, String vacation_reason) throws ExecutionException, InterruptedException {
        LocalDate date = LocalDate.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
        String now = formatter.format(date);
        LocalDate start = LocalDate.parse(start_date, formatter);
        LocalDate end = LocalDate.parse(end_date, formatter);

        LocalDateTime stTime = start.atTime(0,0), endTime = end.atTime(0,0);

        Duration duration = Duration.between(stTime, endTime);
        long diffSec = duration.getSeconds();
        long diffDays = diffSec / (24*60*60);
        diffDays++;

        VacationInfo uv = Util_Vacation.getVacationInt(user_id);
        String url = BuildConfig.ec_ip + "/insertvacation/:" + user_id +"/:" + type + "/:" + now +
                "/:"+start_date+"/:"+end_date+"/:"+ Long.toString(diffDays) +"/:"+vacation_reason+"/:"+uv.vacation_year +
                "/:"+uv.vacation_use_year +"/:"+uv.vacation_month +"/:"+uv.vacation_use_month +
                "/:"+uv.vacation_replace +"/:"+uv.vacation_use_replace;
        String json = new GetDataJson.GetDataJSON().execute(url).get();
    }

    public void insertBusinessStatus(String start_date, String end_date, String business_reason, Double x, Double y, String address)
            throws ExecutionException, InterruptedException {
        LocalDate date = LocalDate.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
        String now = formatter.format(date);
        LocalDate start = LocalDate.parse(start_date, formatter);
        LocalDate end = LocalDate.parse(end_date, formatter);

        LocalDateTime stTime = start.atTime(0,0), endTime = end.atTime(0,0);

        Duration duration = Duration.between(stTime, endTime);
        long diffSec = duration.getSeconds();
        long diffDays = diffSec / (24*60*60);
        diffDays++;

        String url = BuildConfig.ec_ip + "/insertbusiness/:" + user_id +"/:" + now +
                "/:"+start_date+"/:"+end_date+"/:"+diffDays+"/:"+business_reason+"/:"+x +
                "/:"+y +"/:"+address;
        String json = new GetDataJson.GetDataJSON().execute(url).get();
    }
}
