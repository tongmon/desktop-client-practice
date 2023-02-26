package com.example.attendance_app;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Arrays;
import java.util.Calendar;
import java.util.GregorianCalendar;
import java.util.Vector;
import java.util.concurrent.ExecutionException;

public class Act_RequestInfo extends AppCompatActivity {

    static int y, m ,d, dateDialState, stYear = 0, stMonth, stDay, endYear = 0, endMonth, endDay;
    static double mapLatitude, mapLongitude;
    static String locName;
    String stDate, endDate, select_type;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_act_request_info);

        Calendar calendar = new GregorianCalendar();
        y = calendar.get(Calendar.YEAR);
        m = calendar.get(Calendar.MONTH);
        d = calendar.get(Calendar.DAY_OF_MONTH);
        m++;

        stDate = Integer.toString(y) + '-' + Integer.toString(m) + '-' + Integer.toString(d);
        dateDialState = 0;
        endDate = select_type = "";
        locName = "-";

        // 승인 유형 선택 스피너
        Spinner spi_annualSelector = findViewById(R.id.spinner_request_type_selector);
        Vector<String> itemsInSpinner = new Vector<>(Arrays.asList("연차", "월차", "출장"));
        spi_annualSelector.setSelection(0);

        ArrayAdapter<String> spi_Adapter = new ArrayAdapter<String>(this, R.layout.spinner_title_text_request, itemsInSpinner.toArray(new String[itemsInSpinner.size()]));
        spi_Adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        // 스피너 객체에다가 어댑터를 넣어줌
        spi_annualSelector.setAdapter(spi_Adapter);
        spi_annualSelector.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            // 선택
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                select_type = itemsInSpinner.get(position);
                TextView textView = findViewById(R.id.text_request_location);
                if(select_type.compareTo("연차") == 0) {
                    textView.setText("위치: -");
                }
                else if(select_type.compareTo("월차") == 0) {
                    textView.setText("위치: -");
                }
                else{
                    Intent intent = new Intent(getApplicationContext(), Act_Map.class);
                    startActivity(intent);
                }
            }

            // 아무것도 선택 안됨
            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });

        // 시작 날짜 버튼
        Button stDayBtn = findViewById(R.id.DayStart_Button);
        stDayBtn.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View view) {
                dateDialState = 1;
                Dialog_DatePicker dlg = new Dialog_DatePicker(Act_RequestInfo.this, new Dialog_DatePicker.DialogListener(){
                    @Override
                    public void clickBtn() {
                        stYear = y; stMonth = m; stDay = d;
                        String M = Integer.toString(m), D = Integer.toString(d);
                        String tempDay = Integer.toString(y) + '-' +
                                (M.length() == 1 ? '0' + M : M) + '-' + (D.length() == 1 ? '0' + D : D);
                        if(tempDay.compareTo(stDate) < 0){
                            Toast.makeText(Act_RequestInfo.this, "현재 날짜보다 큰 날짜를 선택해주세요.", Toast.LENGTH_LONG).show();
                            return;
                        }
                        stDate = tempDay;
                        stDayBtn.setText("시작 날짜: " + stDate);
                    }
                });
                dlg.show();
            }
        });

        // 종료 날짜 버튼
        Button endDayBtn = findViewById(R.id.DayEnd_Button);
        endDayBtn.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View view) {
                dateDialState = 2;
                Dialog_DatePicker dlg = new Dialog_DatePicker(Act_RequestInfo.this, new Dialog_DatePicker.DialogListener(){
                    @Override
                    public void clickBtn() {
                        endYear = y; endMonth = m; endDay = d;
                        String M = Integer.toString(m), D = Integer.toString(d);
                        String tempDay = Integer.toString(y) + '-' +
                                (M.length() == 1 ? '0' + M : M) + '-' + (D.length() == 1 ? '0' + D : D);
                        if(tempDay.compareTo(stDate) < 0) {
                            Toast.makeText(Act_RequestInfo.this, "현재 날짜, 시작 날짜보다 큰 날짜를 선택해주세요.", Toast.LENGTH_LONG).show();
                            return;
                        }
                        endDate = tempDay;
                        endDayBtn.setText("종료 날짜: " + endDate);
                    }
                });
                dlg.show();
            }
        });

        // 신청 버튼
        Button requestBtn = findViewById(R.id.request_Button);
        requestBtn.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View view) {
                EditText editText = findViewById(R.id.etext_expression_Input);
                String eText = editText.getText().toString();
                if (stDate.isEmpty() || endDate.isEmpty() || locName.isEmpty() || eText.isEmpty()) {
                    Toast.makeText(Act_RequestInfo.this, "모든 내용을 입력해주세요.", Toast.LENGTH_LONG).show();
                    return;
                }
                RequstInfoData reqData = new RequstInfoData();
                if(select_type.compareTo("월차") == 0 || select_type.compareTo("연차") == 0) {
                    try {
                        reqData.insertVacatonStatus((select_type.compareTo("월차") == 0 ? 1 : 0), stDate, endDate, eText);
                    } catch (ExecutionException e) {
                        e.printStackTrace();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                else if(select_type.compareTo("출장") == 0) {
                    try {
                        reqData.insertBusinessStatus(stDate, endDate, eText, mapLongitude, mapLatitude, locName);
                    } catch (ExecutionException e) {
                        e.printStackTrace();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }

                y = m  = d = dateDialState = stYear = stMonth = stDay = endYear = endMonth = endDay = 0;
                mapLatitude = mapLongitude = 0.0;

                Calendar calendar = new GregorianCalendar();
                y = calendar.get(Calendar.YEAR);
                m = calendar.get(Calendar.MONTH);
                d = calendar.get(Calendar.DAY_OF_MONTH);
                m++;

                stDate = Integer.toString(y) + '-' + Integer.toString(m) + '-' + Integer.toString(d);
                dateDialState = 0;
                endDate = select_type = "";
                locName = "-";

                finish();
            }
        });
    }

    protected void onStart() {
        super.onStart();

        TextView textView = findViewById(R.id.text_request_location);
        if(select_type.compareTo("출장") == 0){
            textView.setText("위치: " + locName);
        }
        else{
            textView.setText("위치: -");
        }
    }
}