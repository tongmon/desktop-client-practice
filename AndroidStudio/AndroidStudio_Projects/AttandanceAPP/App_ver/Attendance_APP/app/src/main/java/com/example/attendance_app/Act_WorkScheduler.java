package com.example.attendance_app;

import android.graphics.Typeface;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import java.text.SimpleDateFormat;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Date;
import java.util.Vector;
import java.util.concurrent.ExecutionException;

@RequiresApi(api = Build.VERSION_CODES.O)
public class Act_WorkScheduler extends AppCompatActivity {

    int curYear, curMonth; // 현재 연도, 월
    DynamicCalendar calendar; // 캘린더
    TextView monthTitle, monthRange;
    WorkCalendarData workCalendarData;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_act_work_scheduler);
        workCalendarData = new WorkCalendarData();
        // 메뉴 이정표 텍스트
        TextView menuText = (TextView) findViewById(R.id.text_scheduler_menu_milestone);
        menuText.setText("홈 >> 근로관리 >> 근로일정관리");

        // 캘린더 선언
        calendar = new DynamicCalendar(R.id.table_calander, Act_WorkScheduler.this);

        // 현재 날짜로 달력 생성
        Date now = new Date();
        SimpleDateFormat getYear = new SimpleDateFormat("yyyy"), getMonth= new SimpleDateFormat("M");
        curYear = Integer.parseInt(getYear.format(now));
        curMonth = Integer.parseInt(getMonth.format(now));
        SchedulChanger(curYear, curMonth);

        // 리프레시 버튼
        ImageButton ibtnRefresh = findViewById(R.id.imgbtn_scheduler_month_refresh);
        ibtnRefresh.setOnClickListener(new View.OnClickListener() {
            @Override
            // 클릭
            public void onClick(View view) {
                LocalDateTime now = LocalDateTime.now();
                curYear = Integer.parseInt(now.format(DateTimeFormatter.ofPattern("yyyy")));
                curMonth = Integer.parseInt(now.format(DateTimeFormatter.ofPattern("M")));
                SchedulChanger(curYear, curMonth);
            }
        });

        // 좌 이동 버튼
        ImageButton ibtnGoLeft = findViewById(R.id.imgbtn_scheduler_month_goleft);
        ibtnGoLeft.setOnClickListener(new View.OnClickListener() {
            @Override
            // 클릭
            public void onClick(View view) {
                if(curMonth == 1) {curMonth = 12; curYear--;}
                else curMonth--;
                SchedulChanger(curYear, curMonth);
            }
        });

        // 우측 이동 버튼
        ImageButton ibtnGoRight = findViewById(R.id.imgbtn_scheduler_month_goright);
        ibtnGoRight.setOnClickListener(new View.OnClickListener() {
            @Override
            // 클릭
            public void onClick(View view) {
                LocalDateTime now = LocalDateTime.now();
                String nowDay = now.format(DateTimeFormatter.ofPattern("yyyy-M")), curDay = Integer.toString(curYear) + '-' + Integer.toString(curMonth);
                if(nowDay.equals(curDay)){
                    Toast.makeText(Act_WorkScheduler.this, "달력의 마지막입니다.", Toast.LENGTH_LONG).show();
                    return;
                }

                if(curMonth == 12) {curMonth = 1; curYear++;}
                else curMonth++;
                SchedulChanger(curYear, curMonth);
            }
        });
        // 종알림 버튼
        ImageButton btnAlert = findViewById(R.id.imageButton01);

        // DB 연동해서 icon 이미지를 어떻게 할 것인지 결정
        // Alert 테이블에 해당 유저에 대한 알림이 없다면
        Vector<AlertStatus> status = new Vector<>();
        try {
            status = new AlertData().selectAlert();
        } catch (ExecutionException | InterruptedException e) {
            e.printStackTrace();
        }
        if(status == null || status.size() == 0){
            btnAlert.setImageResource(R.drawable.btn_notibell_notnoti_img);
        }
        else{ // Alert 테이블에 해당 유저에 대한 알림이 없다면
            btnAlert.setImageResource(R.drawable.btn_notibell_noti_img);
        }

        btnAlert.setOnClickListener(new Button.OnClickListener() {
            @Override
            // 클릭
            public void onClick(View view) {
                ImageButton alertBtn = (ImageButton) view;

                // 버튼 클릭시 Dialog_Alert 호출
                Dialog_Alert dlg = new Dialog_Alert(Act_WorkScheduler.this);
                dlg.show();

                // 알림을 모두 본 상태이니 알림 다 본 아이콘으로 바꾼다.
                alertBtn.setImageResource(R.drawable.btn_notibell_notnoti_img); // 아이콘 넣어야됨 아직 못 구함
            }
        });
        // 사용자 정보 버튼
        ImageButton userInfo = findViewById(R.id.imageButton02);
        userInfo.setOnClickListener(new Button.OnClickListener() {
            @Override
            // 클릭
            public void onClick(View view) {
                // 버튼 클릭시 Custom Dialog 호출
                CustomDialog dlg = new CustomDialog(Act_WorkScheduler.this);
                dlg.show();
            }
        });
        // 로그아웃 버튼
        ImageButton logout = findViewById(R.id.imageButton03);
        logout.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View view) {
                LogoutDialog dlg = new LogoutDialog(Act_WorkScheduler.this, new LogoutDialog.LogoutDialogListener(){
                    @RequiresApi(api = Build.VERSION_CODES.O)
                    @Override
                    public void clickBtn() {
                        Act_MainMenu.isLogoutState = true;
                        finish();
                        /*
                        g_PrefEditor.putString("id", "");
                        g_PrefEditor.putString("pw", "");
                        g_PrefEditor.putBoolean("autoLogin", false);
                        chbox_loginMem.setChecked(false);
                        */
                    }
                });
                dlg.show();
            }
        });
    }

    // DB랑 연동해서 바꿔야함
    // 년도랑 월 넣으면 그에 해당하는 달력 출력해주는 함수
    void SchedulChanger(int Year, int Month)
    {
        String strMonth = Integer.toString(Month);
        if(strMonth.length() < 2) strMonth = '0' + strMonth;

        // 월 타이틀 변경
        String monthTitleText = Integer.toString(Year) + "년 " + strMonth + "월";
        monthTitle = (TextView) findViewById(R.id.text_scheduler_month_title);
        monthTitle.setText(monthTitleText);

        String start_date = Year + "-" + strMonth + "-01";
        Vector<Vector<CellInfo>> content = new Vector<>();
        Vector<CellInfo> calRow = new Vector<>();
        CellInfo cellInfo = new CellInfo();
        Vector<Vector<WorkMonth>> fullData = workCalendarData.workScheduleManagement(start_date);
        for(int i=0; i<fullData.size(); i++) {
            calRow = new Vector<>();
            Vector<WorkMonth> workMonths = fullData.get(i);
            for(int j=0; j<workMonths.size(); j++) {
                cellInfo = new CellInfo();

                if(workMonths.get(j).type == 4) {
                    if(workMonths.get(j).day_type == 0) {
                        cellInfo.Add(workMonths.get(j).status, R.drawable.text_normal_cell, "#fa3832", Typeface.BOLD);
                    }else{
                        cellInfo.Add(null, R.drawable.text_normal_cell, "", Typeface.NORMAL);
                    }
                    cellInfo.Add(null, R.drawable.text_normal_cell, "", Typeface.NORMAL);
                } else if(workMonths.get(j).type == 3){
                    cellInfo.Add(workMonths.get(j).status, R.drawable.text_normal_cell, "", Typeface.NORMAL);
                }else if(workMonths.get(j).type == 2) {
                    cellInfo.Add(workMonths.get(j).status, R.drawable.text_normal_cell, "", Typeface.NORMAL);
                    cellInfo.Add("00:00~00:00", R.drawable.text_absent_cell, "", Typeface.NORMAL);
                }else if(workMonths.get(j).type == 1 || workMonths.get(j).type == 0) {
                    if (workMonths.get(j).day_type == 0) {
                        cellInfo.Add(workMonths.get(j).status, R.drawable.text_normal_cell, "#fa3832", Typeface.BOLD);
                    } else {
                        cellInfo.Add(workMonths.get(j).status, R.drawable.text_normal_cell, "", Typeface.NORMAL);
                    }
                    String str = workMonths.get(j).on_time + "~00:00";
                    if(workMonths.get(j).off_time != null){
                        str = workMonths.get(j).on_time + "~" + workMonths.get(j).off_time;
                    }
                    if(workMonths.get(j).type == 0){
                        cellInfo.Add(str, R.drawable.text_attend_cell, "", Typeface.NORMAL);
                    }else{
                        cellInfo.Add(str, R.drawable.text_absent_cell, "", Typeface.NORMAL);
                    }
                }
                calRow.add(cellInfo);
            }
            // 모든 일주일의 완성마다 실제 content에 넣어줘야댐
            content.add(calRow);
        }
        calendar.Display(Year, Month, content);
        insertStatusText(start_date);
        // 월 범위 변경
        monthRange = (TextView) findViewById(R.id.text_scheduler_month_range);
        String dayRange = Integer.toString(Year) + '-' + strMonth + "-01~" +
                Integer.toString(Year) + '-' + strMonth + '-' + Integer.toString(calendar.m_LastDay);
        monthRange.setText(dayRange);

        // 위의 방식을 뺑뺑이 돌려서 총 42개의 캘린더 칸을 채워넣어야댐
        // 캘린더 칸이 왜 42개인지 궁금하면 윈도우 달력을 참고해보면 됨
    }
    void insertStatusText(String start_date){
        Statistics workStatus = workCalendarData.workManage(start_date);
        TextView duty = (TextView) findViewById(R.id.text_scheduler_must_time);
        duty.setText(workStatus.duty_work + "시간");
        TextView predetermined = (TextView) findViewById(R.id.text_scheduler_do_time);
        predetermined.setText(workStatus.extension_work + "시간");
        TextView extension = (TextView) findViewById(R.id.text_scheduler_extend_time);
        extension.setText(workStatus.predetermined_work + "시간");
        TextView vacation = (TextView) findViewById(R.id.text_scheduler_vacation_time);
        vacation.setText(workStatus.vacation + "시간");
    }
}
