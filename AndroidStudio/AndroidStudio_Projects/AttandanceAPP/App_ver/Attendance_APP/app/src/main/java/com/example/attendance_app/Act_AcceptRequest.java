package com.example.attendance_app;

import android.graphics.Typeface;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.Spinner;
import android.widget.TableRow;
import android.widget.TextView;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import java.util.Arrays;
import java.util.Vector;
import java.util.concurrent.ExecutionException;

// 문서유형, 신청자, 날짜, 기안일시, 승인자, 승인일시, 승인상태, 근무지, 내용
@RequiresApi(api = Build.VERSION_CODES.O)
public class Act_AcceptRequest extends AppCompatActivity {

    DynamicTable tableAcceptRequest;
    AcceptData acceptData;
    Vector<Accept> accepts;
    String spinner_type;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_act_accept_request);

        TextView btn_Accept = (TextView) findViewById(R.id.btn_accept_accept);
        btn_Accept.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick (View view) {
                Vector<Integer> getId = tableAcceptRequest.CheckedGetIdRow(true);
                int type = 0;
                try {
                    for (int i = 0; i < getId.size(); i++) {
                        int num = accepts.get(getId.get(i)).num;
                        switch (spinner_type) {
                            case "휴가":
                                type = 1;
                                acceptData.vacationAcceptUpdate(num, 1, accepts.get(getId.get(i)).period,
                                        accepts.get(getId.get(i)).user_id, accepts.get(getId.get(i)).type);
                                break;
                            case "연장 근무":
                                type = 2;
                                acceptData.overAcceptUpdate(num, 1);
                                break;
                            case "외근":
                                type = 3;
                                acceptData.outAcceptUpdate(num, 1);
                                break;
                            default:
                                type = 4;
                                acceptData.businessAcceptUpdate(num, 1);
                                break;
                        }
                        new AlertData().insertAlert(accepts.get(getId.get(i)).user_id,
                                accepts.get(getId.get(i)).type, accepts.get(getId.get(i)).start_date,
                                accepts.get(getId.get(i)).period, "허용");
                    }
                    select(type);
                } catch (ExecutionException | InterruptedException e) {
                    e.printStackTrace();
                }
            }

        });

        TextView btn_Reject = (TextView) findViewById(R.id.btn_accept_reject);
        btn_Reject.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick (View view){
                Vector<Integer> getId = tableAcceptRequest.CheckedGetIdRow(true);
                int type = 0;
                try {
                    for (int i = 0; i < getId.size(); i++) {
                        int num = accepts.get(getId.get(i)).num;
                        switch (spinner_type) {
                            case "휴가":
                                type = 1;
                                acceptData.vacationAcceptUpdate(num, 2, accepts.get(getId.get(i)).period,
                                        accepts.get(getId.get(i)).user_id, accepts.get(getId.get(i)).type);
                                break;
                            case "연장 근무":
                                type = 2;
                                acceptData.overAcceptUpdate(num, 2);
                                break;
                            case "외근":
                                type = 3;
                                acceptData.outAcceptUpdate(num, 2);
                                break;
                            default:
                                type = 4;
                                acceptData.businessAcceptUpdate(num, 2);
                                break;
                        }
                        new AlertData().insertAlert(accepts.get(getId.get(i)).user_id,
                                accepts.get(getId.get(i)).type, accepts.get(getId.get(i)).start_date,
                                accepts.get(getId.get(i)).period, "반려");
                    }
                    select(type);
                } catch (ExecutionException | InterruptedException e) {
                    e.printStackTrace();
                }
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
                Dialog_Alert dlg = new Dialog_Alert(Act_AcceptRequest.this);
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
                CustomDialog dlg = new CustomDialog(Act_AcceptRequest.this);
                dlg.show();
            }
        });

        // 로그아웃 버튼
        ImageButton logout = findViewById(R.id.imageButton03);
        logout.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View view) {
                LogoutDialog dlg = new LogoutDialog(Act_AcceptRequest.this, new LogoutDialog.LogoutDialogListener(){
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

        // 메뉴 이정표 텍스트
        TextView menuText = (TextView) findViewById(R.id.text_acceptrequest_menu_milestone);
        menuText.setText("홈 >> 근로관리 >> 받은 승인요청관리");

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
                spinner_type = itemsInSpinner.get(position);
                try {
                    switch (spinner_type) {
                        case "휴가":
                            select(1);
                            break;
                        case "연장 근무":
                            select(2);
                            break;
                        case "외근":
                            select(3);
                            break;
                        default:
                            select(4);
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
        // 승인요청관리 테이블
        tableAcceptRequest = new DynamicTable(R.id.table_accept_request, Act_AcceptRequest.this);
    }

    void select(int type) throws ExecutionException, InterruptedException {
        acceptData = new AcceptData();
        tableAcceptRequest.ClearTable();
        Vector<String> Row = new Vector<>(Arrays.asList("문서유형", "신청자", "날짜", "기안일시", "승인자", "승인일시", "승인상태", "근무지", "내용"));
        tableAcceptRequest.AddRowWithCheckBox(Row,70,-1,15,R.drawable.table_title_cell,"#020202", Typeface.BOLD);

        if(type == 1) {
            accepts = acceptData.vacationAcceptList();
        }else if(type == 2){
            accepts = acceptData.overAcceptList();
        }else if(type == 3){
            accepts = acceptData.outAcceptList();
        }else{
            accepts = acceptData.businessAcceptList();
        }
        for(int i=0; i<accepts.size(); i++){
            Vector<String> row = new Vector<>(Arrays.asList(accepts.get(i).type, accepts.get(i).user_nm,
                    accepts.get(i).start_date, accepts.get(i).draft_date, accepts.get(i).permit_nm, accepts.get(i).permit_date,
                    accepts.get(i).permit, accepts.get(i).address, accepts.get(i).reason));
            tableAcceptRequest.AddRowWithCheckBox(row, 70, -1, 15, R.drawable.table_normal_cell, "#848484", Typeface.NORMAL);
        }
        if(accepts.size() == 0) {
            tableAcceptRequest.ClearTable();
            Row = new Vector<>(Arrays.asList("문서유형", "신청자"));
            tableAcceptRequest.AddRow(Row,70,-1,15,R.drawable.table_title_cell,"#020202", Typeface.BOLD);
            Row = new Vector<>();
            Row.add("조회된 데이터가 없습니다.");
            tableAcceptRequest.AddRow(Row, 70, -1, 15, R.drawable.table_normal_cell, "#848484", Typeface.NORMAL);
        }
    }
}