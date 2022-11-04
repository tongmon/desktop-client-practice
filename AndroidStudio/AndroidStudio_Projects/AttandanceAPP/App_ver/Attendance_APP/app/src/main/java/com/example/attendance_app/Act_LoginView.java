package com.example.attendance_app;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.concurrent.ExecutionException;

public class Act_LoginView extends AppCompatActivity {

    EditText etext_ID, etext_PW;
    static CheckBox chbox_loginMem;
    Boolean is_Logined = false;
    static String user_id = "";
    static int user_type = 0;
    static SharedPreferences.Editor g_PrefEditor;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_act_login_view);

        SharedPreferences g_Pref = getSharedPreferences("g_Pref", Act_LoginView.MODE_PRIVATE); // PreferenceManager.getDefaultSharedPreferences(this);
        g_PrefEditor = g_Pref.edit();

        etext_ID = (EditText) findViewById(R.id.etext_id_Input);
        etext_PW = (EditText) findViewById(R.id.etext_pw_Input);
        chbox_loginMem = (CheckBox) findViewById(R.id.checkbox_rememberlogin);

        // 로그인 기억이 되어있다면
        if (g_Pref.getBoolean("autoLogin", false)) {
            etext_ID.setText(g_Pref.getString("id", ""));
            etext_PW.setText(g_Pref.getString("pw", ""));
            chbox_loginMem.setChecked(true);

            // 비밀번호가 일치한다면 메뉴로 이동
            try {
                if(GetValidation(etext_ID.getText().toString(), etext_PW.getText().toString())) {
                    Intent intent = new Intent(getApplicationContext(), Act_MainMenu.class);
                    startActivity(intent);
                }
            } catch (ExecutionException | InterruptedException | JSONException e) {
                e.printStackTrace();
            }
        }

        // 로그인 버튼
        Button btn_login = (Button) findViewById(R.id.button_login);
        btn_login.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick (View view){
                String id = etext_ID.getText().toString();
                String password = etext_PW.getText().toString();

                try {
                    if(GetValidation(id, password)) {
                        // 로그인 기억 상태면 로그인 아이디, 비번을 Pref에 저장
                        if(is_Logined) {
                            g_PrefEditor.putString("id", id);
                            g_PrefEditor.putString("pw", password);
                            g_PrefEditor.putBoolean("autoLogin", true);
                            g_PrefEditor.apply();
                        }
                        System.out.println(user_id);
                        // 원 메뉴로 이동
                        Intent intent = new Intent(getApplicationContext(), Act_MainMenu.class);
                        startActivity(intent);

                    }
                } catch (ExecutionException | InterruptedException | JSONException e) {
                    e.printStackTrace();
                }
            }
        });

        chbox_loginMem.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked) {
                    is_Logined = true;
                }
                else {
                    // 체크 안되어 있다면 다 예전에 저장했던 비밀번호 캐시 데이터 지워버림
                    is_Logined = false;
                    g_PrefEditor.clear();
                    g_PrefEditor.apply();
                }
            }
        });
    }

    // 로그인 검사 함수
    private boolean GetValidation(String id, String password) throws ExecutionException, InterruptedException, JSONException {
        String message = "Enter the ID";
        /*
        if (g_Pref.getString("id", "").equals(id) && g_Pref.getString("pw", "").equals(password)) {
            // 로그인 성공
            Toast.makeText(MainActivity.this, "Login Success", Toast.LENGTH_LONG).show();
            return true;
        }*/
        // DB 유저와 연동, 조건 심으셈(id: 2021090101 password: 1234)
        if (!id.equals("") && !password.equals("")) {
            String url = BuildConfig.ec_ip + "/userinfo/:" + id;
            String json = new GetDataJson.GetDataJSON().execute(url).get();
            if (json != null) {
                JSONArray jsonObject = new JSONArray(json);
                if (jsonObject.length() == 1) {
                    JSONObject usersObject = jsonObject.getJSONObject(0);
                    user_id = usersObject.getString("user_id");
                    String comp_passwd = usersObject.getString("passwd");
                    user_type = usersObject.getInt("user_type");
                    if (comp_passwd.equals(password)) {
                        return true;
                    } else {
                        message = "Login Failed";
                    }
                } else {
                    message = "Please Sign in first";
                }
            } else {
                message = "Server error";
            }
        }else{
            if(!id.equals("")) message = "Enter the password";
        }
        Toast.makeText(Act_LoginView.this, message, Toast.LENGTH_LONG).show();
        return false;
    }
}