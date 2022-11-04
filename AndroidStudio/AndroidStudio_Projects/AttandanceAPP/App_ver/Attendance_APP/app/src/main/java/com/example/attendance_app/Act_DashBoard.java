package com.example.attendance_app;

import static com.example.attendance_app.Util_Date.getDate;
import static com.example.attendance_app.Util_Date.weekDate;

import android.annotation.SuppressLint;
import android.graphics.Typeface;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.Arrays;
import java.util.Objects;
import java.util.Vector;
import java.util.concurrent.ExecutionException;


// 대시 보드
@RequiresApi(api = Build.VERSION_CODES.O)
public class Act_DashBoard extends AppCompatActivity {
    UserWorkInfo userWorkInfo;
    DynamicTable tableMonth;

    LocalDate now = LocalDate.now();
    int index = -1;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_act_dash_board);

        try {
            userWorkInfo = new UserWorkInfo();
            tableMonth = new DynamicTable(R.id.table_whole_attendState, Act_DashBoard.this);
            index = userWorkInfo.m_StateInfoVec.size()-1;
            TextInsert();
            DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
            String getDate = formatter.format(now);
            Vector<String> start_end_date = MakeStateInfo(getDate);
            weekTextInsert(start_end_date);
        } catch (Exception e) {
            e.printStackTrace();
        }

        // 메뉴 이정표 텍스트
        TextView menuText = (TextView) findViewById(R.id.text_dashboard_menu_milestone);
        menuText.setText("홈 >> 개인 대시보드");

        // 종알림 버튼
        ImageButton btnAlert = findViewById(R.id.imgbtn_dash_alert);

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
                Dialog_Alert dlg = new Dialog_Alert(Act_DashBoard.this);
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
                CustomDialog dlg = new CustomDialog(Act_DashBoard.this);
                dlg.show();
            }
        });

        // 로그아웃 버튼
        ImageButton logout = findViewById(R.id.imageButton03);
        logout.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View view) {
                LogoutDialog dlg = new LogoutDialog(Act_DashBoard.this, new LogoutDialog.LogoutDialogListener(){
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
        // 일간 근무 현황 리프레시 버튼
        ImageButton ibtnRefresh = findViewById(R.id.imgbtn_dash_dailywork_refresh);
        ibtnRefresh.setOnClickListener(new View.OnClickListener() {
            @Override
            // 클릭
            public void onClick(View view) {
                try {
                    userWorkInfo = new UserWorkInfo();
                    index = userWorkInfo.m_StateInfoVec.size()-1;
                    TextInsert();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
        // 일간 근무 현황 좌 이동 버튼
        ImageButton ibtnGoLeft = findViewById(R.id.imgbtn_dash_dailywork_goleft);
        ibtnGoLeft.setOnClickListener(new View.OnClickListener() {
            @Override
            // 클릭
            public void onClick(View view) {
                index = index - 1;
                if(index < 0){
                    index = 0;
                    Toast.makeText(Act_DashBoard.this, "조회된 데이터가 없습니다.", Toast.LENGTH_SHORT).show();
                }else {
                    try {
                        TextInsert();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        });
        // 일간 근무 현황 우측 이동 버튼
        ImageButton ibtnGoRight = findViewById(R.id.imgbtn_dash_dailywork_goright);
        ibtnGoRight.setOnClickListener(new View.OnClickListener() {
            @Override
            // 클릭
            public void onClick(View view) {
                index = index + 1;
                if(index >= userWorkInfo.m_StateInfoVec.size()){
                    index = userWorkInfo.m_StateInfoVec.size()-1;
                    Toast.makeText(Act_DashBoard.this, "현재 날짜입니다.", Toast.LENGTH_SHORT).show();
                }else {
                    try {
                        TextInsert();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        });
        
        // 기간 근무 현황 리프레시 버튼
        ImageButton whole_btnRefresh = findViewById(R.id.imageButton04);
        whole_btnRefresh.setOnClickListener(new View.OnClickListener() {
            @Override
            // 클릭
            public void onClick(View view) {
                try {
                    now = LocalDate.now();
                    DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
                    String getDate = formatter.format(now);
                    Vector<String> start_end_date = MakeStateInfo(getDate);
                    weekTextInsert(start_end_date);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
        // 기간 근무 현황 좌 이동 버튼
        ImageButton whole_btnGoLeft = findViewById(R.id.imageButton05);
        whole_btnGoLeft.setOnClickListener(new View.OnClickListener() {
            @Override
            // 클릭
            public void onClick(View view) {
                try {
                    String tmp_date = userWorkInfo.m_StateInfoVec.get(0).m_Date;
                    LocalDate comp = LocalDate.parse(tmp_date);
                    now = now.minusDays(7);
                    if(now.isBefore(comp)){
                        Toast.makeText(Act_DashBoard.this, "조회된 데이터가 없습니다.", Toast.LENGTH_SHORT).show();
                        now = comp;
                    }
                    DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
                    String getDate = formatter.format(now);
                    Vector<String> start_end_date = MakeStateInfo(getDate);
                    weekTextInsert(start_end_date);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
        // 기간 근무 현황 우측 이동 버튼
        ImageButton whole_btnGoRight = findViewById(R.id.imageButton06);
        whole_btnGoRight.setOnClickListener(new View.OnClickListener() {
            @Override
            // 클릭
            public void onClick(View view) {
                try {
                    LocalDate comp = LocalDate.now().plusDays(1);
                    now = now.plusDays(7);
                    if(now.isAfter(comp)){
                        Toast.makeText(Act_DashBoard.this, "현재 날짜입니다.", Toast.LENGTH_SHORT).show();
                        now = LocalDate.now();
                    };
                    DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
                    String getDate = formatter.format(now);
                    Vector<String> start_end_date = MakeStateInfo(getDate);
                    weekTextInsert(start_end_date);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
    }

    // 일간 근무 현황 데이터 리스트에서 가져오기
    @SuppressLint("SetTextI18n")
    void TextInsert() {
        // 상태
        TextView status_current_main = (TextView) findViewById(R.id.text_status_current_main);
        // 날짜 및 요일
        TextView date_main = (TextView) findViewById(R.id.text_date_main);
        String date = getDate(userWorkInfo.m_StateInfoVec.get(index).m_Date, userWorkInfo.m_StateInfoVec.get(index).m_Day);
        date_main.setText(date);
        // 상태
        TextView status_current = (TextView) findViewById(R.id.text_status_current);
        // 출근 시각
        TextView attendTime = (TextView) findViewById(R.id.text_arrivetime_time);
        // 설정한 출근 시각
        TextView settedAttendTime = (TextView) findViewById(R.id.text_arrivetime_settingtime);
        // 퇴근 시각
        TextView offTime = (TextView) findViewById(R.id.text_offtime_time);
        // 설정한 퇴근 시각
        TextView settedOffTime = (TextView) findViewById(R.id.text_offtime_settingtime);
        // 연장 근로 상태
        TextView extend_current = (TextView) findViewById(R.id.text_extend_current);
        // 소정 근로
        TextView originwork_current = (TextView) findViewById(R.id.text_originwork_current);
        // 소정 근로 퍼센트
        TextView originwork_percentage = (TextView) findViewById(R.id.text_originwork_percentage);
        ProgressBar originwork_bar = (ProgressBar) findViewById(R.id.progress_originwork);
        // 연장 근로
        TextView extendwork_current = (TextView) findViewById(R.id.text_extendwork_current);
        // 연장 근로 퍼센트
        TextView extendwork_percentage = (TextView) findViewById(R.id.text_extendwork_percentage);
        ProgressBar extendwork_bar = (ProgressBar) findViewById(R.id.progress_extendwork);

        ImageView imageView = findViewById(R.id.img_attentimg);
        imageView.setImageResource(R.drawable.status_attend_img);

        if(userWorkInfo.m_StateInfoVec.get(index).m_DayState == UserState.DayType.Normal){
            status_current_main.setText("정상근무");
            status_current.setText("정상근무");
        }else if(userWorkInfo.m_StateInfoVec.get(index).m_DayState == UserState.DayType.Trip){
            status_current_main.setText("출장");
            status_current.setText("출장");
        }else if(userWorkInfo.m_StateInfoVec.get(index).m_DayState == UserState.DayType.Vacation){
            status_current_main.setText("휴가");
            status_current.setText("휴가");
        }else if(userWorkInfo.m_StateInfoVec.get(index).m_DayState == UserState.DayType.Outside){
            status_current_main.setText("외근");
            status_current.setText("외근");
        }else if(userWorkInfo.m_StateInfoVec.get(index).m_DayState == UserState.DayType.Education){
            status_current_main.setText("교육");
            status_current.setText("교육");
        }else if(userWorkInfo.m_StateInfoVec.get(index).m_DayState == UserState.DayType.WeekEnd){
            status_current_main.setText("주말");
            status_current.setText("주말");
        }else if(userWorkInfo.m_StateInfoVec.get(index).m_DayState == UserState.DayType.Holiday){
            status_current_main.setText("공휴일");
            status_current.setText("공휴일");
        }else if(userWorkInfo.m_StateInfoVec.get(index).m_DayState == UserState.DayType.Sick){
            status_current_main.setText("병가");
            status_current.setText("병가");
        }if(userWorkInfo.m_StateInfoVec.get(index).m_DayState == UserState.DayType.EndType){
            status_current_main.setText("미출근");
            status_current.setText("미출근");
            imageView.setImageResource(R.drawable.status_absent_img);
        }
        UserState.DayType dt = userWorkInfo.m_StateInfoVec.get(index).m_DayState;
        if(!((dt == UserState.DayType.EndType) || (dt == UserState.DayType.Sick) || (dt == UserState.DayType.Vacation) || (dt == UserState.DayType.Holiday)
        || (dt == UserState.DayType.WeekEnd)  || (userWorkInfo.m_StateInfoVec.get(index).m_AttendTime == null))) {
            attendTime.setText(userWorkInfo.m_StateInfoVec.get(index).m_AttendTime);
            settedAttendTime.setText("근무시작시각: " + userWorkInfo.m_StateInfoVec.get(index).m_SettedAttendTime);
            extend_current.setText(userWorkInfo.m_StateInfoVec.get(index).m_over_type);
            if(userWorkInfo.m_StateInfoVec.get(index).m_OffTime == null){
                offTime.setText("-");
                settedOffTime.setText("근무종료시각: -");
                originwork_current.setText("-");
            }else {
                offTime.setText(userWorkInfo.m_StateInfoVec.get(index).m_OffTime);
                settedOffTime.setText("근무종료시각: " + userWorkInfo.m_StateInfoVec.get(index).m_SettedOffTime);
                originwork_current.setText(userWorkInfo.m_StateInfoVec.get(index).m_WorkTime + "시간");
            }
            if (userWorkInfo.m_StateInfoVec.get(index).m_OverTime == 0) {
                extendwork_current.setText("-");
            } else {
                extendwork_current.setText(userWorkInfo.m_StateInfoVec.get(index).m_OverTime + "시간");
            }
        }else{
            attendTime.setText("-");
            settedAttendTime.setText("근무시작시각: -");
            offTime.setText("-");
            settedOffTime.setText("근무종료시각: -");
            extend_current.setText("-");
            extendwork_current.setText("-");
            if((dt == UserState.DayType.Vacation) || (dt == UserState.DayType.Holiday))
                originwork_current.setText(userWorkInfo.m_StateInfoVec.get(index).m_WorkTime + "시간");
            else
                originwork_current.setText("-");
        }
        originwork_percentage.setText(userWorkInfo.m_StateInfoVec.get(index).m_WorkPercent + "%");
        int percent = Math.round(userWorkInfo.m_StateInfoVec.get(index).m_WorkPercent);
        originwork_bar.setProgress(percent);
        extendwork_percentage.setText(userWorkInfo.m_StateInfoVec.get(index).m_OverPercent+ "%");
        percent = Math.round(userWorkInfo.m_StateInfoVec.get(index).m_OverPercent);
        extendwork_bar.setProgress(percent);
    }
    @SuppressLint("SetTextI18n")
    void weekTextInsert(Vector<String> start_end_date){
        tableMonth.ClearTable();
        Vector<String> Row = new Vector<>(Arrays.asList("날짜", "요일", "출근", "퇴근", "근무시작", "근무종료", "소정근로", "연장근로"));
        tableMonth.AddRow(Row,70,40,15,R.drawable.table_title_cell,"#020202", Typeface.BOLD);

        // 전체 근무 상세 현황 테이블
        int tableCnt = userWorkInfo.m_StateWeekVec.size();
        for(int i=0; i<tableCnt; i++) {
            Row = GetWorkStatusDetail(i);
            tableMonth.AddRow(Row, 70, -1, 15, R.drawable.table_normal_cell, "#848484", Typeface.NORMAL);
        }

        TextView work_date = (TextView) findViewById(R.id.text_wholework_date);
        work_date.setText("("+start_end_date.get(0) + " ~ " + start_end_date.get(start_end_date.size()-1) + ")");

        int standard_WorkTime = userWorkInfo.m_MustWorkTime;

        TextView work_progress = (TextView) findViewById(R.id.text_wholework_progressunit);
        work_progress.setText(standard_WorkTime + "시간");

        if(userWorkInfo.m_TotalWorkTime > standard_WorkTime)
            standard_WorkTime = userWorkInfo.m_TotalWorkTime;
        float week_percent = (float)userWorkInfo.m_TotalWorkTime / (float)standard_WorkTime * 100;
        TextView work_percentage = (TextView) findViewById(R.id.text_wholework_progresspercentage);
        work_percentage.setText("- ( " + week_percent + "% ) "+ userWorkInfo.m_TotalWorkTime  +"시간");
        ProgressBar progress_origin_work = (ProgressBar) findViewById(R.id.progress_bigger_originwork);
        int percent = Math.round(week_percent);
        progress_origin_work.setProgress(percent);
    }

    //year month week
    // 특정 날짜를 지정하면 해당 날짜에 대한 주차 정보를 m_StateWeekVec 삽입
    Vector<String> MakeStateInfo(String date) throws Exception {
        Vector<String> start_end_date = weekDate(date);
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
        String getDate = formatter.format(LocalDate.now());
        int status = 0;

        userWorkInfo.m_TotalWorkTime = 0;
        userWorkInfo.m_StateWeekVec.clear();

        for (int i=0; i<start_end_date.size(); i++) {
            if(userWorkInfo.m_StateInfo.containsKey(start_end_date.get(i))) {
                UserState uw = userWorkInfo.m_StateInfo.get(start_end_date.get(i));
                userWorkInfo.m_StateWeekVec.add(uw);
                assert uw != null;
                if(!(uw.m_DayState.equals(UserState.DayType.EndType) || uw.m_DayState.equals(UserState.DayType.Sick))){
                    userWorkInfo.m_TotalWorkTime += Objects.requireNonNull(userWorkInfo.m_StateInfo.get(start_end_date.get(i))).m_WorkTime;
                }
            }
        }
        userWorkInfo.m_MustWorkTime = userWorkInfo.m_WorkTimePerDay * 5;

        return start_end_date;
    }
    // 현재 사용 유저의 근무 상세 현황 배열로 전달
    public Vector<String> GetWorkStatusDetail(int i) {
        // 날짜, 요일, 출근, 퇴근, 근무시각, 근무종료, 소정근로, 연장근로
        Vector<String> userDetailWork = new Vector<>();
        UserState.DayType dt = userWorkInfo.m_StateWeekVec.get(i).m_DayState;
        if(!(dt.equals(UserState.DayType.EndType) || dt.equals(UserState.DayType.Vacation) || dt.equals(UserState.DayType.Sick))) {
            userDetailWork.add(userWorkInfo.m_StateWeekVec.get(i).m_Date);
            userDetailWork.add(userWorkInfo.m_StateWeekVec.get(i).m_Day);
            userDetailWork.add(userWorkInfo.m_StateWeekVec.get(i).m_SettedAttendTime);
            userDetailWork.add(userWorkInfo.m_StateWeekVec.get(i).m_SettedOffTime);
            userDetailWork.add(userWorkInfo.m_StateWeekVec.get(i).m_AttendTime);
            userDetailWork.add(userWorkInfo.m_StateWeekVec.get(i).m_OffTime);
            userDetailWork.add(userWorkInfo.m_StateWeekVec.get(i).m_WorkTime + "시간");
            userDetailWork.add(userWorkInfo.m_StateWeekVec.get(i).m_OverTime + "시간");
        }
        return userDetailWork;
    }
}