package com.example.coronasearch;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.Spinner;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import java.util.concurrent.ExecutionException;

public class RegisterPatient extends AppCompatActivity {

    private EditText edit1;
    private TextView textView1;
    private ImageButton click;
    private Spinner spinner;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_register_patient);

        this.getViewObject();

        View.OnClickListener clickListener = new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                if (v.getId() == R.id.click) {
                    String locCode = "1", Buf = "";
                    boolean Valid = false;
                    String Data = edit1.getText().toString();
                    if(Data.equals("123")) // 인증코드
                        Valid = true;
                    if (Valid) // 올바른 입력일 시
                    {
                        // 지역번호는 locCode에 저장되고 보낼 메소드 필요
                        Buf = Integer.toString(spinner.getSelectedItemPosition());

                        if(Buf.length() == 2){
                            locCode = locCode + Buf;
                        }
                        else{
                            locCode = locCode + "0" + Buf;
                        }
                        textView1.setText("인증 되었습니다.\n지역 번호: " + locCode);
                        String RealData = "ab1218913c" + ',' + locCode;
                        PatientRegister register = new PatientRegister();
                        try {
                            boolean registerResult = register.registerFunc(RealData);
                            if(registerResult == true){
                                textView1.setText("환자 번호 : " + register.patientID);
                            }
                        } catch (ExecutionException e) {
                            e.printStackTrace();
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                        Handler handler = new Handler();
                        handler.postDelayed(new Runnable(){
                            @Override
                            public void run() {
                                Intent intent = new Intent (getApplicationContext(), CompleteActivity.class);
                                startActivity(intent); //다음화면으로 넘어감
                                finish();
                            }
                        },2000); //3초
                    }
                    else // 잘못된 입력일 시
                    {
                        textView1.setText("잘못된 인증 코드 입니다.");
                    }
                }
            }
        };
        click.setOnClickListener(clickListener);
    }

    private void getViewObject()
    {
        edit1 = findViewById(R.id.edit1);
        textView1 = findViewById(R.id.textView1);
        click = findViewById(R.id.click);
        spinner = findViewById(R.id.spinner);
    }

}