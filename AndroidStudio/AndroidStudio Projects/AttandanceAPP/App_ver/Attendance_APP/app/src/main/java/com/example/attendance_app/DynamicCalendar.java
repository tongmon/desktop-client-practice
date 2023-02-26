package com.example.attendance_app;

import android.app.Activity;
import android.graphics.Color;
import android.graphics.Typeface;
import android.os.Build;
import android.util.TypedValue;
import android.view.Gravity;
import android.widget.LinearLayout;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;

import androidx.annotation.RequiresApi;
import androidx.core.content.ContextCompat;

import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Calendar;
import java.util.Date;
import java.util.Vector;

// 셀 하나 당 정보
class CellInfo {
    Vector<String> m_Content; // 컨텐츠
    Vector<Integer> m_BGcolors; // 텍스트 뷰의 배경색, 흰, 빨, 파 3가지 색상
    Vector<String> m_StrColor; // 텍스트 색상
    Vector<Integer> m_Style; // 텍스트가 Bold, Normal 인지 속성
    CellInfo()
    {
        m_Content = new Vector<>();
        m_BGcolors = new Vector<>();
        m_Style = new Vector<>();
        m_StrColor= new Vector<>();
    }
    void Add(String content, Integer bgColor, String strColor, int style)
    {
        m_Content.add(content);
        m_BGcolors.add(bgColor);
        if(strColor.isEmpty())
            strColor = "#020202";
        m_StrColor.add(strColor);
        m_Style.add(style);
    }
    void Insert(String content, Integer bgColor, String strColor, int style, int Index)
    {
        m_Content.insertElementAt(content, Index);
        m_BGcolors.insertElementAt(bgColor, Index);
        if(strColor.isEmpty())
            strColor = "#020202";
        m_StrColor.insertElementAt(strColor, Index);
        m_Style.insertElementAt(style, Index);
    }
    void Clear()
    {
        m_Content.clear();
        m_BGcolors.clear();
        m_StrColor.clear();
        m_Style.clear();
    }
}

public class DynamicCalendar extends DynamicTable {
    int m_Year;
    int m_Month;
    int m_LastDay;
    int m_StartWeek;
    int m_dateFontSize;
    int m_strFontSize;
    int m_titleFoneSize;

    DynamicCalendar() {}

    DynamicCalendar(int id, Activity act)
    {
        super(id, act);
        m_dateFontSize = m_titleFoneSize = 15;
        m_strFontSize = 10;
    }

    DynamicCalendar(int id, Activity act, int year, int month)
    {
        super(id, act);
        m_Year = year;
        m_Month = month;
    }

    // 인자 Vector<Vector<CellInfo>> content
    // 추가해야 되는데 테스트 땜시 뺌
    @RequiresApi(api = Build.VERSION_CODES.O)
    void Display(int year, int month, Vector<Vector<CellInfo>> content)
    {
        ClearTable();

        m_Year = year;
        m_Month = month;

        if(m_Month == 2){
            int k =0;
        }

        Calendar Cur = Calendar.getInstance();
        // 저번 달의 최고 끝 날짜를 정함
        int backMonth = m_Month - 2, backYear = m_Year, backDay;
        if(backMonth < 0) {
            backYear--;
            backMonth = 12;
        }
        Cur.set(backYear, backMonth, 1);
        backDay = Cur.getActualMaximum(Cur.DAY_OF_MONTH);

        Cur.set(m_Year, m_Month - 1,1);
        m_LastDay = Cur.getActualMaximum(Cur.DAY_OF_MONTH);
        m_StartWeek = Cur.get(Cur.DAY_OF_WEEK);

        // 달에 해당하는 모든 날짜들 삽입
        Vector<Integer> Days = new Vector<Integer>();
        for (int i = m_StartWeek;i>1;i--) {
            Days.add(backDay - i + 2);
        }
        for (int i = 1;i<=m_LastDay;i++) {
            Days.add(i);
        }
        int i = 1;
        while(Days.size() < 42) {
            Days.add(i++);
        }

        Vector<String> Row = new Vector<>(Arrays.asList("일", "월", "화", "수", "목", "금", "토")), Temp;
        AddDateTitle(Row,30,-1,m_titleFoneSize,R.drawable.table_calender_normal_cell,"#020202", Typeface.BOLD);

        Date now = new Date();
        SimpleDateFormat date = new SimpleDateFormat("yyyy/M/d");
        String dayNow = date.format(now);
        Boolean dayCheck = false;

        // content.size()는 6이여야함
        int k = 0;
        for (i = 0;i < content.size();i++){
            Vector<CellInfo> row = content.get(i);

            // row.size()는 7이여야함
            for (int j = 0;j < row.size();j++) {
                if(Days.get(k) == 1) dayCheck = true;
                // 날짜 담아줌
                String strColor = "", dayCalculate = Integer.toString(m_Year) + "/" + Integer.toString(m_Month) + "/" + Days.get(k).toString();
                if(j == 0) strColor = "#fa3832";
                if(j == 6) strColor = "#237af4";
                row.get(j).Insert(Days.get(k++).toString(), R.drawable.text_normal_cell, strColor,
                        (dayCheck && dayCalculate == dayNow) ? Typeface.BOLD : Typeface.NORMAL, 0);
            }
            AddDateRow(row,30,-1,m_strFontSize);
        }
    }

    /*
    // 테스트 용
    void Display(int year, int month)
    {
        m_Year = year;
        m_Month = month;

        Calendar Cur = Calendar.getInstance();
        // 저번 달의 최고 끝 날짜를 정함
        int backMonth = m_Month - 2, backYear = m_Year, backDay;
        if(backMonth < 0) {
            backYear--;
            backMonth = 12;
        }
        Cur.set(backYear, backMonth, 1);
        backDay = Cur.getActualMaximum(Cur.DAY_OF_MONTH);

        //사용자 지정요일 값을 입력함
        Cur.set(m_Year, m_Month - 1,1);
        //입력받은 달의 마지막 일 수 계산(30일, 31일 등)
        m_LastDay = Cur.getActualMaximum(Cur.DAY_OF_MONTH);
        //1이면 일요일, 2이면 월요일 ...순으로 입력 월의 시작 요일값 저장
        m_StartWeek = Cur.get(Cur.DAY_OF_WEEK);

        // 달에 해당하는 모든 날짜들 삽입
        Vector<Integer> Days = new Vector<Integer>();
        for (int i = m_StartWeek;i>1;i--) {
            Days.add(backDay - i + 2);
        }
        for (int i = 1;i<=m_LastDay;i++) {
            Days.add(i);
        }
        int i = 1;
        while(Days.size() < 42) {
            Days.add(i++);
        }

        Vector<String> Row = new Vector<>(Arrays.asList("일", "월", "화", "수", "목", "금", "토")), Temp;
        AddDateTitle(Row,30,-1,15,R.drawable.table_calender_normal_cell,"#020202", Typeface.BOLD);

        int k = 0;
        for (i = 0;i < 6;i++){
            Vector<Vector<String>> rowContent = new Vector<>();
            Vector<Vector<String>> rowColors = new Vector<>();

            for (int j = 0;j < 7;j++) {
                Vector<String> cell = new Vector<>();
                cell.add(Days.get(k++).toString());
                rowContent.add(cell);

                Vector<String> cellColor = new Vector<>();
                cellColor.add("#020202");
                rowColors.add(cellColor);
            }
            AddDateRow(rowContent,30,-1,15,R.drawable.table_calender_normal_cell, rowColors, Typeface.BOLD);
        }
    }
    */

    // 달력 제목 넣기
    void AddDateTitle(Vector<String> Row, float CellWidth, float CellHeight, float RowTextSize, int BackGroundID, String TextColor, int TextStyle)
    {
        final TableRow tableRow = new TableRow(m_context);
        TableLayout.LayoutParams trParams = new
                TableLayout.LayoutParams(TableLayout.LayoutParams.WRAP_CONTENT, TableLayout.LayoutParams.WRAP_CONTENT);
        trParams.setMargins(0, 0, 0, 0);
        tableRow.setLayoutParams(trParams);
        tableRow.setGravity(Gravity.CENTER);

        for (int i = 0;i<Row.size();i++) {
            // 선형 레이아웃 선언
            final LinearLayout layoutForText = new LinearLayout(m_context);
            LinearLayout.LayoutParams llParam = new
                    TableRow.LayoutParams(CellWidth > 0 ? DPtoPX(CellWidth) : TableRow.LayoutParams.WRAP_CONTENT,
                    CellHeight > 0 ? DPtoPX(CellHeight) : TableRow.LayoutParams.WRAP_CONTENT);
            layoutForText.setLayoutParams(llParam);
            layoutForText.setOrientation(LinearLayout.VERTICAL);
            layoutForText.setBackground(ContextCompat.getDrawable(m_context, BackGroundID));

            final TextView tv = new TextView(m_context);
            tv.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT));
            tv.setText(Row.get(i));
            tv.setTextSize(TypedValue.COMPLEX_UNIT_PX, DPtoPX(RowTextSize));
            tv.setTypeface(tv.getTypeface(), TextStyle);
            tv.setGravity(Gravity.CENTER);
            tv.setTextColor(Color.parseColor(TextColor));
            tv.setPadding(DPtoPX(5), DPtoPX(5), DPtoPX(5), DPtoPX(5));
            layoutForText.addView(tv);
            tableRow.addView(layoutForText);
        }
        m_Table.addView(tableRow);
    }

    // 달력 실제 내용 넣기
    void AddDateRow(Vector<CellInfo> Row, float CellWidth, float CellHeight, float RowTextSize)
    {
        final TableRow tableRow = new TableRow(m_context);
        TableLayout.LayoutParams trParams = new
                TableLayout.LayoutParams(TableLayout.LayoutParams.WRAP_CONTENT, TableLayout.LayoutParams.WRAP_CONTENT);
        trParams.setMargins(0, 0, 0, 0);
        tableRow.setLayoutParams(trParams);
        tableRow.setGravity(Gravity.CENTER);

        for (int i = 0;i<Row.size();i++) {
            // 선형 레이아웃 선언
            final LinearLayout layoutForText = new LinearLayout(m_context);
            LinearLayout.LayoutParams llParam = new
                    TableRow.LayoutParams(CellWidth > 0 ? DPtoPX(CellWidth) : TableRow.LayoutParams.WRAP_CONTENT,
                    CellHeight > 0 ? DPtoPX(CellHeight) : TableRow.LayoutParams.MATCH_PARENT);
            layoutForText.setLayoutParams(llParam);
            layoutForText.setOrientation(LinearLayout.VERTICAL);
            layoutForText.setBackground(ContextCompat.getDrawable(m_context, R.drawable.table_calender_normal_cell));

            Vector<String> rowTexts = Row.get(i).m_Content;
            Vector<Integer> bgColor = Row.get(i).m_BGcolors;
            Vector<String> strColor = Row.get(i).m_StrColor;
            Vector<Integer> textStyle = Row.get(i).m_Style;

            for (int j = 0;j < rowTexts.size();j++) {
                final TextView tv = new TextView(m_context);
                tv.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT));
                tv.setText(rowTexts.get(j));
                tv.setTextSize(TypedValue.COMPLEX_UNIT_PX, j == 0 ? DPtoPX(m_dateFontSize) : DPtoPX(RowTextSize));
                tv.setTypeface(tv.getTypeface(), textStyle.get(j));
                tv.setGravity(Gravity.LEFT);
                tv.setTextColor(Color.parseColor(strColor.get(j)));
                tv.setPadding(DPtoPX(5), DPtoPX(5), DPtoPX(5), DPtoPX(5));
                tv.setBackgroundResource(bgColor.get(j));
                layoutForText.addView(tv);
            }
            tableRow.addView(layoutForText);
        }
        m_Table.addView(tableRow);
    }
}
