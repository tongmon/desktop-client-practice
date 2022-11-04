package com.example.attendance_app;

import static com.example.attendance_app.Act_LoginView.user_type;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import java.util.concurrent.ExecutionException;

public class Act_SubWorkManager extends AppCompatActivity {
    static String select = "state";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_act_sub_work_manager);

        // 일정 관리 버튼
        Button btn_Schedule = (Button) findViewById(R.id.WorkSchedule_Button);
        btn_Schedule.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick (View view){
                // TODO : click event
                Intent intent = new Intent(getApplicationContext(), Act_WorkScheduler.class);
                startActivity(intent);
            }
        });

        // 내 승인요청관리 버튼
        Button btn_MyRequestList = (Button) findViewById(R.id.MyRequest_Button);
        btn_MyRequestList.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick (View view){
                // TODO : click event
                Intent intent = new Intent(getApplicationContext(), Act_MyRequest.class);
                startActivity(intent);
            }
        });
        // 받은 승인요청관리
        Button btn_AcceptRequestList = (Button) findViewById(R.id.AcceptRequest_Button);
        btn_AcceptRequestList.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(user_type == 1) {
                    Intent intent = new Intent(getApplicationContext(), Act_AcceptRequest.class);
                    startActivity(intent);
                }else{
                    Toast.makeText(Act_SubWorkManager.this, "관리자만 사용할 수 있습니다.", Toast.LENGTH_LONG).show();
                }
            }
        });

        // 근로실적조회 버튼
        Button btn_Performance = (Button) findViewById(R.id.WorkPerformance_Button);
        btn_Performance.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick (View view){
                // TODO : click event
                Intent intent = new Intent(getApplicationContext(), Act_WorkPerformance.class);
                startActivity(intent);
            }
        });

        // 연차현황 버튼
        Button btn_Annual = (Button) findViewById(R.id.Annual_Button);
        btn_Annual.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick (View view){
                // TODO : click event
                Intent intent = new Intent(getApplicationContext(), Act_Annual.class);
                startActivity(intent);
            }
        });
    }

    @RequiresApi(api = Build.VERSION_CODES.O)
    @Override
    protected void onStart() {
        super.onStart();

        if(Act_MainMenu.isLogoutState)
            finish();
    }
}