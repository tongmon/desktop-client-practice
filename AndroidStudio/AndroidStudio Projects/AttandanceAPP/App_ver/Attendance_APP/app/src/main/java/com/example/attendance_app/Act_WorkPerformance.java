package com.example.attendance_app;

import static com.example.attendance_app.Act_LoginView.user_id;
import static com.example.attendance_app.Util_Date.*;

import android.graphics.Typeface;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Vector;
import java.util.concurrent.ExecutionException;
@RequiresApi(api = Build.VERSION_CODES.O)
public class Act_WorkPerformance extends AppCompatActivity {
    UserWorkInfo userWorkInfo;
    LocalDate start_Date;
    LocalDate now_Date;
    DynamicTable tableMonth;
    DynamicTable tableDay;
    DynamicTable tableWeek;
    Vector<Vector<String>> month_excel;
    Vector<Vector<String>> week_excel;
    Vector<Vector<String>> day_excel;
    int dayWorkTableIndex;
    int dayWorkTableBatchSize;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_act_work_performance);

        dayWorkTableIndex = 0;
        dayWorkTableBatchSize = 5;

        // 메뉴 이정표 텍스트
        TextView menuText = (TextView) findViewById(R.id.text_perform_menu_milestone);
        menuText.setText("홈 >> 근로관리 >> 근로실적조회");

        // 월 선택 스피너
        Spinner spi_MonthSelector = findViewById(R.id.spinner_perform_month_selector);

        // DB 연동
        // 스피너에 들어갈 년도 itemsInSpinner에 삽입해야 함
        Vector<String> itemsInSpinner = new Vector<>();
        try {
            userWorkInfo = new UserWorkInfo();
            if(userWorkInfo.m_StateInfoVec.size() > 0){
                String start = userWorkInfo.m_StateInfoVec.get(0).m_Date;
                start_Date = LocalDate.parse(start, DateTimeFormatter.ISO_DATE);
                now_Date = LocalDate.now();
                while(!((start_Date.getMonthValue() == now_Date.getMonthValue()) && (start_Date.getYear() == now_Date.getYear()))) {
                    itemsInSpinner.add(start_Date.getYear() + "년 " + start_Date.getMonthValue() + "월");
                    start_Date = start_Date.plusMonths(1);
                }
                itemsInSpinner.add(now_Date.getYear() + "년 " + now_Date.getMonthValue() + "월");
            }
        } catch (ExecutionException | InterruptedException e) {
            e.printStackTrace();
        }
        spi_MonthSelector.setSelection(0);
        // DB 연동 끝

        ArrayAdapter<String> spi_Adapter = new ArrayAdapter<String>(this, R.layout.spinner_title_text, itemsInSpinner.toArray(new String[itemsInSpinner.size()]));
        spi_Adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        // 스피너 객체에다가 어댑터를 넣어줌
        spi_MonthSelector.setAdapter(spi_Adapter);
        spi_MonthSelector.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            // 선택
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                 String select_month = itemsInSpinner.get(position);
                 select_month = select_month.replaceAll("[^0-9]", "").replace(" ", "");
                 String year = select_month.substring(0,4);
                 String month =select_month.substring(4);
                 if(month.length() == 1) month = "0"+month;
                 select_month = year+"-"+month+"-01";
                try {
                    DateSelect(select_month, 1, 0, 0);
                    DateSelect(select_month, 0, 1, 0);
                } catch (Exception e) {
                    e.printStackTrace();
                }
                dayWorkTableIndex=0;
                if(!ShowDailyTable(dayWorkTableIndex,dayWorkTableBatchSize))
                    Toast.makeText(Act_WorkPerformance.this, "일일 근무 현황이 존재하지 않습니다.", Toast.LENGTH_LONG).show();
            }

            // 아무것도 선택 안됨
            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });

        // 일일 현황 좌측 이동
        ImageButton dayWorkgoLeft = (ImageButton) findViewById(R.id.imgbtn_daywork_goleft);
        dayWorkgoLeft.setOnClickListener(new View.OnClickListener() {
            @Override
            // 클릭
            public void onClick(View view) {
                if(ShowDailyTable(--dayWorkTableIndex, dayWorkTableBatchSize)) {
                    TextView textView = (TextView) findViewById(R.id.imgbtn_daywork_index);
                    textView.setText(Integer.toString(dayWorkTableIndex + 1));
                }
                else{
                    dayWorkTableIndex++;
                    Toast.makeText(Act_WorkPerformance.this, "테이블의 처음입니다.", Toast.LENGTH_LONG).show();
                }
            }
        });

        // 일일 현황 우측 이동
        ImageButton dayWorkgoRight = (ImageButton) findViewById(R.id.imgbtn_daywork_goright);
        dayWorkgoRight.setOnClickListener(new View.OnClickListener() {
            @Override
            // 클릭
            public void onClick(View view) {
                if(ShowDailyTable(++dayWorkTableIndex, dayWorkTableBatchSize)){
                    TextView textView = (TextView) findViewById(R.id.imgbtn_daywork_index);
                    textView.setText(Integer.toString(dayWorkTableIndex + 1));
                }
                else{
                    dayWorkTableIndex--;
                    Toast.makeText(Act_WorkPerformance.this, "테이블의 마지막입니다.", Toast.LENGTH_LONG).show();
                }
            }
        });

        // 근로 실적 다운로드 버튼
        TextView btnDownload = (TextView) findViewById(R.id.btn_perform_download);
        btnDownload.setOnClickListener(new View.OnClickListener() {
            @Override
            // 클릭
            public void onClick(View view) {
                DownloadExcel excel = new DownloadExcel();
                String message = excel.saveWorkExcel(Act_WorkPerformance.this, month_excel, week_excel, day_excel);
                message += " 경로에 저장했습니다.";
                Toast.makeText(Act_WorkPerformance.this, message, Toast.LENGTH_LONG).show();
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
                Dialog_Alert dlg = new Dialog_Alert(Act_WorkPerformance.this);
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
                CustomDialog dlg = new CustomDialog(Act_WorkPerformance.this);
                dlg.show();
            }
        });

        // 로그아웃 버튼
        ImageButton logout = findViewById(R.id.imageButton03);
        logout.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View view) {
                LogoutDialog dlg = new LogoutDialog(Act_WorkPerformance.this, new LogoutDialog.LogoutDialogListener(){
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

        // 월간 근태 현황 테이블
        tableMonth = new DynamicTable(R.id.table_monthly_attendance, Act_WorkPerformance.this);

        // 주간 근태 현황 테이블
        tableWeek = new DynamicTable(R.id.table_weekly_attendance, Act_WorkPerformance.this);

        // 일간 근태 현황 테이블
        tableDay = new DynamicTable(R.id.table_daily_attendance, Act_WorkPerformance.this);

    }

    public void DateSelect(String select_month, int month, int week, int day) throws Exception {
        Vector<String> start_end_date = new Vector<>();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
        LocalDate date = LocalDate.parse(select_month, formatter);
        LocalDate end = date.withDayOfMonth(date.lengthOfMonth());
        if(month == 1 || day == 1){
            int no_weekend = 0;
            while(date.isBefore(end) || date.isEqual(end)) {
                String result = formatter.format(date);
                start_end_date.add(result);
                date = date.plusDays(1);
                if(!(getDateDay(date).equals("토요일") || getDateDay(date).equals("일요일")))
                    no_weekend++;
            }
            month_excel = MonthStatus(no_weekend, start_end_date);
            day_excel = DayStatus(start_end_date);
        }else if(week == 1){
            Vector<Vector<String>> week_date = new Vector<>();

            while(date.isBefore(end) || date.isEqual(end)) {
                start_end_date = weekDate(formatter.format(date));
                week_date.add(start_end_date);
                date = date.plusDays(7);

            }
            week_excel = WeekStatus(week_date);
        }
    }

    public Vector<Vector<String>> MonthStatus(int no_weekend, Vector<String> start_end_date) {
        ArrayList<Integer> al = new ArrayList<Integer>(Arrays.asList(0, 0, 0, 0, 0, 0, 0));
        Vector<String> row = new Vector<>();
        Vector<Vector<String>> excel = new Vector<>();
        WorkCalendarData wd = new WorkCalendarData();
        int workTime = 0;
        int time = 0;
        if(userWorkInfo.m_StateInfoVec.size() > 0) {
            for (int i = 0; i < start_end_date.size(); i++) {
                DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
                LocalDate date = LocalDate.parse(start_end_date.get(i), formatter);
                String day = getDateDay(date);
                if(!(day.equals("토요일") || day.equals("일요일") || wd.holidays.containsKey(start_end_date.get(i)))){
                    time += userWorkInfo.m_WorkTimePerDay;
                }
                if (userWorkInfo.m_StateInfo.containsKey(start_end_date.get(i))) {
                    UserState uw = userWorkInfo.m_StateInfo.get(start_end_date.get(i));
                    if (!(uw.m_DayState.equals(UserState.DayType.EndType) || uw.m_DayState.equals(UserState.DayType.Sick))) {
                        if(!(day.equals("토요일") || day.equals("일요일") || wd.holidays.containsKey(start_end_date.get(i)))) {
                            workTime += userWorkInfo.m_WorkTimePerDay;
                        }
                    }
                    al.set(2, al.get(2)+uw.m_OverTime);
                    if(uw.m_DayState.equals(UserState.DayType.Vacation)){
                        al.set(3, al.get(3)+1);
                    }else if(uw.m_DayState.equals(UserState.DayType.Education)){
                        al.set(4, al.get(4)+1);
                    }else if(uw.m_DayState.equals(UserState.DayType.Outside)){
                        al.set(5, al.get(5)+1);
                    }else if(uw.m_DayState.equals(UserState.DayType.Trip)) {
                        al.set(6, al.get(6) + 1);
                    }
                }
            }
            al.set(0, time);
            al.set(1, workTime);
        }
        row.add(start_end_date.get(0));
        row.add(start_end_date.get(start_end_date.size()-1));
        for(int i=0; i<3; i++)
            row.add(al.get(i) + "시간");
        for(int i=3; i<7; i++)
            row.add(al.get(i) + "일");
        tableMonth.ClearTable();
        Vector<String> Row = new Vector<>(Arrays.asList("월 시작일", "월 종료일", "의무근로", "소정근로"
                , "연장근로", "휴가사용일수", "교육/훈련", "외근일수", "출장일수"));
        tableMonth.AddRow(Row,70,55,15,R.drawable.table_title_cell,"#020202", Typeface.BOLD);
        tableMonth.AddRow(row, 70, -1, 15, R.drawable.table_normal_cell, "#848484", Typeface.NORMAL);
        excel.add(Row);
        excel.add(row);

        return excel;
    }

    public Vector<Vector<String>> WeekStatus(Vector<Vector<String>> week_date) {
        Vector<Vector<String>> excel = new Vector<>();
        tableWeek.ClearTable();
        Vector<String> Row = new Vector<>(Arrays.asList("주 시작일", "주 종료일", "의무근로", "소정근로"
                , "연장근로", "휴가사용일수", "교육/훈련", "외근일수", "출장일수"));
        tableWeek.AddRow(Row,70,55,15,R.drawable.table_title_cell,"#020202", Typeface.BOLD);
        excel.add(Row);
        for(int i=0; i<week_date.size(); i++){
            Vector<String> row = new Vector<>();
            Vector<String> start_end_date =week_date.get(i);
            ArrayList<Integer> al = new ArrayList<Integer>(Arrays.asList(0, 0, 0, 0, 0, 0, 0));
            int status = 0;
            int workTime = 0;
            for(int j=0; j<start_end_date.size(); j++) {
                al.set(0, userWorkInfo.m_WorkTimePerDay*5);
                if (userWorkInfo.m_StateInfo.containsKey(start_end_date.get(j))) {
                    UserState uw = userWorkInfo.m_StateInfo.get(start_end_date.get(j));
                    workTime += uw.m_WorkTime;
                    al.set(2, al.get(2)+uw.m_OverTime);
                    if(uw.m_DayState.equals(UserState.DayType.Vacation)){
                        al.set(3, al.get(3)+1);
                    }else if(uw.m_DayState.equals(UserState.DayType.Education)){
                        al.set(4, al.get(4)+1);
                    }else if(uw.m_DayState.equals(UserState.DayType.Outside)){
                        al.set(5, al.get(5)+1);
                    }else if(uw.m_DayState.equals(UserState.DayType.Trip)) {
                        al.set(6, al.get(6) + 1);
                    }
                }
            }
            al.set(1, workTime);
            row.add(start_end_date.get(0));
            row.add(start_end_date.get(start_end_date.size()-1));
            for(int k=0; k<3; k++)
                row.add(al.get(k) + "시간");
            for(int k=3; k<7; k++)
                row.add(al.get(k) + "일");
            excel.add(row);
            tableWeek.AddRow(row, 70, -1, 15, R.drawable.table_normal_cell, "#848484", Typeface.NORMAL);
        }
        return excel;
    }

    public Vector<Vector<String>> DayStatus(Vector<String> start_end_date) {
        Vector<String> row = new Vector<>();
        Vector<Vector<String>> excel = new Vector<>();
        tableDay.ClearTable();
        Vector<String> Row = new Vector<>(Arrays.asList("근무일자", "요일", "상태", "출근시간"
                , "퇴근시간", "근무시작시간", "근무종료시간", "연장근무시작시각", "연장근무종료시각", "근무시간", "연장근무시간"));
        // tableDay.AddRow(Row,70,55,15,R.drawable.table_title_cell,"#020202", Typeface.BOLD);
        excel.add(Row);
        if(userWorkInfo.m_StateInfoVec.size() > 0) {
            for (int i = 0; i < start_end_date.size(); i++) {
                row = new Vector<>();
                if (userWorkInfo.m_StateInfo.containsKey(start_end_date.get(i))) {
                    UserState uw = userWorkInfo.m_StateInfo.get(start_end_date.get(i));
                    if (uw.m_DayState.equals(UserState.DayType.Normal) || uw.m_DayState.equals(UserState.DayType.Outside) ||
                            uw.m_DayState.equals(UserState.DayType.Education) || uw.m_DayState.equals(UserState.DayType.WeekEnd) || uw.m_DayState.equals(UserState.DayType.Holiday)) {
                        row.add(start_end_date.get(i));
                        row.add(uw.m_Day);
                        if(uw.m_DayState == UserState.DayType.Normal){
                            row.add("정상근무");
                        }else if(uw.m_DayState == UserState.DayType.Outside){
                            row.add("외근");
                        }else if(uw.m_DayState == UserState.DayType.Education){
                            row.add("교육");
                        }else if(uw.m_DayState == UserState.DayType.WeekEnd){
                            row.add("주말");
                        }else if(uw.m_DayState == UserState.DayType.Holiday){
                            row.add("공휴일");
                        }
                        row.add(uw.m_SettedAttendTime);
                        row.add(uw.m_SettedOffTime);
                        row.add(uw.m_AttendTime);
                        row.add(uw.m_OffTime);
                        row.add(uw.m_OverStartTime);
                        row.add(uw.m_OverEndTime);
                        row.add(uw.m_WorkTime + "시간");
                        row.add(uw.m_OverTime + "시간");
                        excel.add(row);
                    }
                }
                // tableDay.AddRow(row, 70, -1, 15, R.drawable.table_normal_cell, "#848484", Typeface.NORMAL);
            }
        }
        return excel;
    }

    public boolean ShowDailyTable(int index, int batchSize) {
        int stIndex = batchSize * index + 1, endIndex = stIndex + batchSize;
        if(stIndex >= day_excel.size() || stIndex < 1)
            return false;
        tableDay.ClearTable();
        tableDay.AddRow(day_excel.get(0),70,55,15,R.drawable.table_title_cell,"#020202", Typeface.BOLD);
        for (int i =stIndex;i < endIndex && i < day_excel.size();i++) {
            tableDay.AddRow(day_excel.get(i), 70, -1, 15, R.drawable.table_normal_cell, "#848484", Typeface.NORMAL);
        }
        return true;
    }
}