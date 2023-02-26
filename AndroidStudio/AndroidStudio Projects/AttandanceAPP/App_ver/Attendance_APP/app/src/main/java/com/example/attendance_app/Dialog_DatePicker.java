package com.example.attendance_app;

import android.app.Dialog;
import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.annotation.NonNull;

import java.util.Calendar;
import java.util.GregorianCalendar;
import java.util.Objects;
import java.util.concurrent.ExecutionException;

public class Dialog_DatePicker extends Dialog {

    private Context mContext;
    private DialogListener mDialogListener;
    public int mYear, mMonth, mDay;

    DatePicker.OnDateChangedListener mOnDateChangedListener = new DatePicker.OnDateChangedListener(){
        @Override
        public void onDateChanged(DatePicker datePicker, int yy, int mm, int dd) {
            Act_RequestInfo.y = yy;
            Act_RequestInfo.m = mm;
            Act_RequestInfo.d = dd;
            Act_RequestInfo.m++;
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.datepicker_dialog);

        // 다이얼로그의 배경을 투명으로 만든다.
        Objects.requireNonNull(getWindow()).setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));

        // 커스텀 다이얼로그의 각 위젯들을 정의한다.
        Button okButton = findViewById(R.id.btn_date_Ok);

        // 버튼 리스너 설정
        okButton.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View view) {
                mDialogListener.clickBtn();
                dismiss();
            }
        });

        if(Act_RequestInfo.dateDialState == 1) {
            if(Act_RequestInfo.stYear == 0) {
                Calendar calendar = new GregorianCalendar();
                mYear = calendar.get(Calendar.YEAR);
                mMonth = calendar.get(Calendar.MONTH);
                mDay = calendar.get(Calendar.DAY_OF_MONTH);
            }
            else {
                mYear = Act_RequestInfo.stYear;
                mMonth = Act_RequestInfo.stMonth - 1;
                mDay = Act_RequestInfo.stDay;
            }
        }
        else {
            if(Act_RequestInfo.endYear == 0) {
                Calendar calendar = new GregorianCalendar();
                mYear = calendar.get(Calendar.YEAR);
                mMonth = calendar.get(Calendar.MONTH);
                mDay = calendar.get(Calendar.DAY_OF_MONTH);
            }
            else {
                mYear = Act_RequestInfo.endYear;
                mMonth = Act_RequestInfo.endMonth - 1;
                mDay = Act_RequestInfo.endDay;
            }
        }
        Act_RequestInfo.y = mYear;
        Act_RequestInfo.m = mMonth;
        Act_RequestInfo.d = mDay;
        Act_RequestInfo.m++;

        DatePicker datePicker = findViewById(R.id.date_picker);
        datePicker.init(mYear, mMonth, mDay, mOnDateChangedListener);
    }

    public Dialog_DatePicker(@NonNull Context context, DialogListener dialogListener) {
        super(context);
        this.mContext = context;
        this.mDialogListener = dialogListener;
    }

    public interface DialogListener{
        void clickBtn();
    }

    public int GetYear(){return 1;}
}
