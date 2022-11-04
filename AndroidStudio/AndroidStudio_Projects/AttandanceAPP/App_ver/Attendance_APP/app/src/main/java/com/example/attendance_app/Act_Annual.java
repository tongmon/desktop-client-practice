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
import android.widget.TextView;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import java.util.Arrays;
import java.util.Vector;
import java.util.concurrent.ExecutionException;

public class Act_Annual extends AppCompatActivity {
    DynamicTable tableVacationStatus;
    DynamicTable tableVacationUsage;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_act_annual);

        // 메뉴 이정표 텍스트
        TextView menuText = (TextView) findViewById(R.id.text_annual_menu_milestone);
        menuText.setText("홈 >> 근로관리 >> 연차현황");
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
                Dialog_Alert dlg = new Dialog_Alert(Act_Annual.this);
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
                CustomDialog dlg = new CustomDialog(Act_Annual.this);
                dlg.show();
            }
        });

        // 로그아웃 버튼
        ImageButton logout = findViewById(R.id.imageButton03);
        logout.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View view) {
                LogoutDialog dlg = new LogoutDialog(Act_Annual.this, new LogoutDialog.LogoutDialogListener(){
                    @RequiresApi(api = Build.VERSION_CODES.O)
                    @Override
                    public void clickBtn() {
                        Act_MainMenu.isLogoutState = true;
                        finish();
                    }
                });
                dlg.show();
            }
        });

        // 년도 선택 스피너
        Spinner spi_YearSelector = findViewById(R.id.spinner_annual_year_selector);

        // DB 연동
        // 스피너에 들어갈 년도 itemsInSpinner에 삽입해야 함
        Vector<String> itemsInSpinner = new Vector<>();
        Util_Vacation uv;
        try {
            uv = new Util_Vacation();
            for (VacationInfo vi : Util_Vacation.vacation_info) {
                if (!itemsInSpinner.contains((vi.state_select_year) + "년")) {
                    itemsInSpinner.add(vi.state_select_year + "년");
                }
            }
        } catch (ExecutionException | InterruptedException e) {
            e.printStackTrace();
        }
        spi_YearSelector.setSelection(0);
        // DB 연동 끝

        ArrayAdapter<String> spi_Adapter = new ArrayAdapter<String>(this, R.layout.spinner_title_text, itemsInSpinner.toArray(new String[itemsInSpinner.size()]));
        spi_Adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        // 스피너 객체에다가 어댑터를 넣어줌
        spi_YearSelector.setAdapter(spi_Adapter);
        spi_YearSelector.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            // 선택
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                String select_year = itemsInSpinner.get(position);
                select_year = select_year.substring(0,4);
                selectYear(select_year);
            }

            // 아무것도 선택 안됨
            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
        // 휴가 현황 테이블
        tableVacationStatus = new DynamicTable(R.id.table_vacationState, Act_Annual.this);
        // 휴가 사용 내역 테이블
        tableVacationUsage = new DynamicTable(R.id.table_vacationUsage, Act_Annual.this);
    }
    void selectYear(String select_year){
        tableVacationStatus.ClearTable();
        Vector<String> Row = new Vector<>(Arrays.asList("사원명", "직급", "부서", "입사일"
                , "연차", "사용연차", "월차", "사용월차", "대체", "사용대체", "잔여일수"));
        tableVacationStatus.AddRow(Row,70,40,15,R.drawable.table_title_cell,"#020202", Typeface.BOLD);
        for(VacationInfo vi : Util_Vacation.vacation_info){
            if(select_year.equals(vi.state_select_year)){
                Row = statusInsert(vi);
                tableVacationStatus.AddRow(Row, 70, -1, 15, R.drawable.table_normal_cell, "#848484", Typeface.NORMAL);
            }
        }
        tableVacationUsage.ClearTable();
        Row = new Vector<>(Arrays.asList("번호", "구분", "적용일", "시작일", "종료일", "연차", "사용연차", "월차", "사용월차", "대체", "사용대체", "내용"));
        tableVacationUsage.AddRow(Row,70,40,15,R.drawable.table_title_cell,"#020202", Typeface.BOLD);
        for(VacationStateList vs : Util_Vacation.vacation_state_list){
            if(select_year.equals(vs.use_select_year)){
                Row = usageInsert(vs);
                tableVacationUsage.AddRow(Row, 70, -1, 15, R.drawable.table_normal_cell, "#848484", Typeface.NORMAL);
            }
        }
    }
    Vector<String> statusInsert(VacationInfo vi){
        Vector<String> list = new Vector<>();
        list.add(vi.name);
        list.add(vi.position);
        list.add(vi.department);
        list.add(vi.join_date);
        list.add(Integer.toString(vi.vacation_year));
        list.add(Integer.toString(vi.vacation_use_year));
        list.add(Integer.toString(vi.vacation_month));
        list.add(Integer.toString(vi.vacation_use_month));
        list.add(Integer.toString(vi.vacation_replace));
        list.add(Integer.toString(vi.vacation_use_replace));
        list.add(Integer.toString(vi.remaining_day));
        return list;
    }
    Vector<String> usageInsert(VacationStateList vs){
        Vector<String> list = new Vector<>();
        list.add(Integer.toString(vs.row));
        list.add(vs.section);
        list.add(vs.application_date);
        list.add(vs.start_date);
        list.add(vs.end_date);
        list.add(Integer.toString(vs.vacation_year));
        list.add(Integer.toString(vs.vacation_use_year));
        list.add(Integer.toString(vs.vacation_month));
        list.add(Integer.toString(vs.vacation_use_month));
        list.add(Integer.toString(vs.vacation_replace));
        list.add(Integer.toString(vs.vacation_use_replace));
        list.add(vs.memo);
        return list;
    }
}