package com.example.attendance_app;

import android.app.Dialog;
import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TableRow;
import android.widget.TextView;

import androidx.annotation.NonNull;

import java.util.Objects;
import java.util.Vector;
import java.util.concurrent.ExecutionException;

public class Dialog_Alert extends Dialog {

    private Context mContext;
    private LinearLayout mLinearLayout;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.alert_dialog);

        // 다이얼로그의 배경을 투명으로 만든다.
        Objects.requireNonNull(getWindow()).setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));
        try {
            userInfoDialog();
        } catch (ExecutionException | InterruptedException e) {
            e.printStackTrace();
        }

        // 커스텀 다이얼로그의 각 위젯들을 정의한다.
        Button saveButton = findViewById(R.id.btn_alert_Ok);
        // 버튼 리스너 설정
        saveButton.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    new AlertData().deleteAlert();
                } catch (ExecutionException | InterruptedException e) {
                    e.printStackTrace();
                }
                // Custom Dialog 종료
                dismiss();
            }
        });
    }

    int DPtoPX(float dp) {
        return ((int)((dp * mContext.getResources().getDisplayMetrics().density) + 0.5));
    }

    public Dialog_Alert(@NonNull Context context) {
        super(context);
        this.mContext = context;
    }

    public void userInfoDialog() throws ExecutionException, InterruptedException {
        mLinearLayout = findViewById(R.id.alert_Layout);
        Vector<AlertStatus> status = new AlertData().selectAlert();
        // 알림이 없으면? 없다고 말해주는 텍스트 하나만 넣음
        if(status.size() == 0) {
            AddInfo("알림이 없습니다.");
        }
        else {
            for(int i=0; i < status.size(); i++) {
                String date = status.get(i).start_date.substring(0, 10);
                AddInfo(status.get(i).type + "(" + date + ", " + status.get(i).period + "일)이 " +
                        status.get(i).status + "되었습니다.");
            }
        }
    }

    public void AddInfo(String content){
        TextView textView = new TextView(mContext);
        textView.setText(content);
        textView.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT));
        textView.setPadding(DPtoPX(2), DPtoPX(2), DPtoPX(2), DPtoPX(2));
        textView.setTextSize(DPtoPX(7));
        textView.setGravity(Gravity.CENTER);
        mLinearLayout.addView(textView);
    }
}
