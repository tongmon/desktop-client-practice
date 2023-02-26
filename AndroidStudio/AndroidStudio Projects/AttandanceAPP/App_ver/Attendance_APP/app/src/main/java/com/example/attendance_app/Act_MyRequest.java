package com.example.attendance_app;

import android.content.Intent;
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

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Vector;
import java.util.concurrent.ExecutionException;

// 문서유형, 신청자, 날짜, 기안일시, 승인자, 승인일시, 승인상태, 근무지, 내용

public class Act_MyRequest extends AppCompatActivity {

    DynamicTable tableMyRequest;
    RequestData requestData;
    String select_type = "휴가";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_act_my_request);
        requestData = new RequestData();
        // 메뉴 이정표 텍스트
        TextView menuText = (TextView) findViewById(R.id.text_myrequest_menu_milestone);
        menuText.setText("홈 >> 근로관리 >> 내 승인요청관리");

        // 승인 유형 선택 스피너
        Spinner spi_annualSelector = findViewById(R.id.spinner_annual_year_selector);
        Vector<String> itemsInSpinner = new Vector<>(Arrays.asList("휴가", "연장 근무", "외근", "출장"));
        spi_annualSelector.setSelection(0);

        ArrayAdapter<String> spi_Adapter = new ArrayAdapter<String>(this, R.layout.spinner_title_text, itemsInSpinner.toArray(new String[itemsInSpinner.size()]));
        spi_Adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        // 스피너 객체에다가 어댑터를 넣어줌
        spi_annualSelector.setAdapter(spi_Adapter);
        spi_annualSelector.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            // 선택
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                select_type = itemsInSpinner.get(position);
                try {
                    switch (select_type) {
                        case "휴가":
                            vacationRequest();
                            break;
                        case "연장 근무":
                            overWorkRequest();
                            break;
                        case "외근":
                            outsideRequest();
                            break;
                        default:
                            businessRequest();
                            break;
                    }
                } catch (ExecutionException | InterruptedException e) {
                    e.printStackTrace();
                }
            }

            // 아무것도 선택 안됨
            @Override
            public void onNothingSelected(AdapterView<?> parent) {

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
                Dialog_Alert dlg = new Dialog_Alert(Act_MyRequest.this);
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
                CustomDialog dlg = new CustomDialog(Act_MyRequest.this);
                dlg.show();
            }
        });

        // 근로 신청 버튼
        TextView requestBtn = findViewById(R.id.btn_myRequest_request);
        requestBtn.setOnClickListener(new Button.OnClickListener() {
            @Override
            // 클릭
            public void onClick(View view) {
                Intent intent = new Intent(getApplicationContext(), Act_RequestInfo.class);
                startActivity(intent);
            }
        });

        // 로그아웃 버튼
        ImageButton logout = findViewById(R.id.imageButton03);
        logout.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View view) {
                LogoutDialog dlg = new LogoutDialog(Act_MyRequest.this, new LogoutDialog.LogoutDialogListener(){
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

        // 승인요청관리 테이블
        tableMyRequest = new DynamicTable(R.id.table_vacationUsage, Act_MyRequest.this);
    }

    public void vacationRequest() throws ExecutionException, InterruptedException {
        Vector<Vacation> list = requestData.vacationList();
        tableMyRequest.ClearTable();
        if(list.size() > 0) {
            Vector<String> Row = new Vector<>(Arrays.asList("유형", "휴가 시작 날짜", "휴가 종료 날짜", "휴가 기간"
                    , "휴가 사유", "상태", "휴가 승인 날짜", "승인자"));
            tableMyRequest.AddRow(Row, 70, 55, 15, R.drawable.table_title_cell, "#020202", Typeface.BOLD);
            for (int i = 0; i < list.size(); i++) {
                Vector<String> row = new Vector<>(Arrays.asList(list.get(i).name, list.get(i).vacation_start_date, list.get(i).vacation_end_date,
                        list.get(i).vacation_period + "일", list.get(i).vacation_reason, list.get(i).permit_name, list.get(i).permit_date, list.get(i).user_nm));
                tableMyRequest.AddRow(row, 70, -1, 15, R.drawable.table_normal_cell, "#848484", Typeface.NORMAL);
            }
        }else{
            tableMyRequest.ClearTable();
            Vector<String> Row = new Vector<>(Arrays.asList("유형", "근무 날짜"));
            tableMyRequest.AddRow(Row,70,-1,15,R.drawable.table_title_cell,"#020202", Typeface.BOLD);
            Row = new Vector<>();
            Row.add("조회된 데이터가 없습니다.");
            tableMyRequest.AddRow(Row, 70, -1, 15, R.drawable.table_normal_cell, "#848484", Typeface.NORMAL);
        }
    }

    public void overWorkRequest() throws ExecutionException, InterruptedException {
        Vector<OverWork> list = requestData.overWorkList();
        tableMyRequest.ClearTable();
        if(list.size() > 0) {
            Vector<String> Row = new Vector<>(Arrays.asList("유형", "근무 날짜", "근무 시작 시간", "근무 종료 시간"
                    , "근무 사유", "상태", "근무 승인 날짜", "승인자"));
            tableMyRequest.AddRow(Row, 70, 55, 15, R.drawable.table_title_cell, "#020202", Typeface.BOLD);
            for (int i = 0; i < list.size(); i++) {
                Vector<String> row = new Vector<>(Arrays.asList(list.get(i).name, list.get(i).attend_date, list.get(i).over_start_time, list.get(i).over_end_time,
                        list.get(i).over_reason, list.get(i).permit_name, list.get(i).permit_date, list.get(i).user_nm));
                tableMyRequest.AddRow(row, 70, -1, 15, R.drawable.table_normal_cell, "#848484", Typeface.NORMAL);
            }
        }else{
            tableMyRequest.ClearTable();
            Vector<String> Row = new Vector<>(Arrays.asList("유형", "근무 날짜"));
            tableMyRequest.AddRow(Row,70,-1,15,R.drawable.table_title_cell,"#020202", Typeface.BOLD);
            Row = new Vector<>();
            Row.add("조회된 데이터가 없습니다.");
            tableMyRequest.AddRow(Row, 70, -1, 15, R.drawable.table_normal_cell, "#848484", Typeface.NORMAL);
        }
    }

    public void outsideRequest() throws ExecutionException, InterruptedException {
        Vector<Outside> list = requestData.outsideList();
        tableMyRequest.ClearTable();
        if(list.size() > 0) {
            Vector<String> Row = new Vector<>(Arrays.asList("유형", "근무 날짜", "근무 시작 시간", "근무 종료 시간"
                    , "근무 사유", "상태", "근무 승인 날짜", "승인자"));
            tableMyRequest.AddRow(Row, 70, 55, 15, R.drawable.table_title_cell, "#020202", Typeface.BOLD);
            for (int i = 0; i < list.size(); i++) {
                Vector<String> row = new Vector<>(Arrays.asList(list.get(i).name, list.get(i).outside_date, list.get(i).outside_start_time, list.get(i).outside_end_time,
                        list.get(i).outside_reason, list.get(i).permit_name, list.get(i).permit_date, list.get(i).user_nm));
                tableMyRequest.AddRow(row, 70, -1, 15, R.drawable.table_normal_cell, "#848484", Typeface.NORMAL);
            }
        } else{
            tableMyRequest.ClearTable();
            Vector<String> Row = new Vector<>(Arrays.asList("유형", "근무 날짜"));
            tableMyRequest.AddRow(Row,70,-1,15,R.drawable.table_title_cell,"#020202", Typeface.BOLD);
            Row = new Vector<>();
            Row.add("조회된 데이터가 없습니다.");
            tableMyRequest.AddRow(Row, 70, -1, 15, R.drawable.table_normal_cell, "#848484", Typeface.NORMAL);
        }
    }

    public void businessRequest() throws ExecutionException, InterruptedException {
        Vector<Business> list = requestData.businessList();
        tableMyRequest.ClearTable();
        if(list != null || list.size() > 0) {
            Vector<String> Row = new Vector<>(Arrays.asList("출장 시작 날짜", "출장 종료 날짜", "출장 기간"
                    , "출장 사유", "상태", "출장 승인 날짜", "승인자"));
            tableMyRequest.AddRow(Row, 70, 55, 15, R.drawable.table_title_cell, "#020202", Typeface.BOLD);
            for (int i = 0; i < list.size(); i++) {
                Vector<String> row = new Vector<>(Arrays.asList(list.get(i).business_start_date, list.get(i).business_end_date, list.get(i).business_period + "일", list.get(i).business_reason,
                        list.get(i).permit_name, list.get(i).permit_date, list.get(i).user_nm));
                tableMyRequest.AddRow(row, 70, -1, 15, R.drawable.table_normal_cell, "#848484", Typeface.NORMAL);
            }
        } else{
            tableMyRequest.ClearTable();
            Vector<String> Row = new Vector<>(Arrays.asList("유형", "근무 날짜"));
            tableMyRequest.AddRow(Row,70,-1,15,R.drawable.table_title_cell,"#020202", Typeface.BOLD);
            Row = new Vector<>();
            Row.add("조회된 데이터가 없습니다.");
            tableMyRequest.AddRow(Row, 70, -1, 15, R.drawable.table_normal_cell, "#848484", Typeface.NORMAL);
        }
    }

    protected void onStart() {
        super.onStart();
        try {
            switch (select_type) {
                case "휴가":
                    vacationRequest();
                    break;
                case "연장 근무":
                    overWorkRequest();
                    break;
                case "외근":
                    outsideRequest();
                    break;
                default:
                    businessRequest();
                    break;
            }
        } catch (ExecutionException | InterruptedException e) {
            e.printStackTrace();
        }
    }
}