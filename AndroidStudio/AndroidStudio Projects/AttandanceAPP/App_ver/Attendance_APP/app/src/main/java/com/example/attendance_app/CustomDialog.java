package com.example.attendance_app;

import android.app.Dialog;
import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import androidx.annotation.NonNull;

import java.util.Objects;
import java.util.concurrent.ExecutionException;

public class CustomDialog extends Dialog {

    private Context mContext;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.custom_dialog);

        // 다이얼로그의 배경을 투명으로 만든다.
        Objects.requireNonNull(getWindow()).setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));
        try {
            userInfoDialog();
        } catch (ExecutionException | InterruptedException e) {
            e.printStackTrace();
        }
        // 커스텀 다이얼로그의 각 위젯들을 정의한다.
        Button saveButton = findViewById(R.id.btnSave);
        // 버튼 리스너 설정
        saveButton.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View view) {
                // Custom Dialog 종료
                dismiss();
            }
        });
    }
    public CustomDialog(@NonNull Context context) {
        super(context);
        this.mContext = context;
    }

    public void userInfoDialog() throws ExecutionException, InterruptedException {
        UserInfo us = new UserInfo();

        TextView name_dialog = (TextView) findViewById(R.id.text_user_name);
        name_dialog.setText(us.name);
        TextView department_dialog = (TextView) findViewById(R.id.text_department);
        department_dialog.setText(us.department);
        TextView position_dialog = (TextView) findViewById(R.id.text_position);
        position_dialog.setText(us.position);
        TextView join_date_dialog = (TextView) findViewById(R.id.text_join_date);
        join_date_dialog.setText(us.join_date);
    }
}
