package com.example.attendance_app;


import android.app.Dialog;
import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import java.util.Objects;

public class LogoutDialog extends Dialog {

    private LogoutDialogListener logoutDialogListener;
    public LogoutDialog(Context context, LogoutDialogListener logoutDialogListener) {
        super(context);
        this.logoutDialogListener = logoutDialogListener;
    }
    public interface LogoutDialogListener{
        void clickBtn();
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.logout_dialog);

        // 다이얼로그의 배경을 투명으로 만든다.
        Objects.requireNonNull(getWindow()).setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));

        // 커스텀 다이얼로그의 각 위젯들을 정의한다.
        Button okButton = findViewById(R.id.btn_Ok);
        // 버튼 리스너 설정
        okButton.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View view) {
                logoutDialogListener.clickBtn();
                dismiss();
            }
        });
        Button cancelButton = findViewById(R.id.btn_cancel);
        cancelButton.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View view) {
                dismiss();
            }
        });
    }
}
