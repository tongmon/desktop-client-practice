package com.example.attendance_app;

import android.app.Activity;
import android.content.Context;
import android.graphics.Color;
import android.util.TypedValue;
import android.view.Gravity;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.LinearLayout;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;

import androidx.core.content.ContextCompat;

import java.util.Vector;

// 동적 테이블
class DynamicTable {
    TableLayout m_Table;
    Context m_context;
    boolean m_AllChecked;

    DynamicTable() {}

    DynamicTable(int id, Activity act)
    {
        m_AllChecked = false;
        m_context = act.getBaseContext();
        m_Table= (TableLayout) act.findViewById(id);
        m_Table.setStretchAllColumns(true);
        m_Table.removeAllViews();
    }

    void InitTable(int id, Activity act)
    {
        m_context = act.getBaseContext();
        m_Table= (TableLayout) act.findViewById(id);
        m_Table.setStretchAllColumns(true);
        m_Table.removeAllViews();
    }

    int DPtoPX(float dp) {
        return ((int)((dp * m_context.getResources().getDisplayMetrics().density) + 0.5));
    }

    void AddRow(Vector<String> Row, float CellWidth, float CellHeight, float RowTextSize, int BackGroundID, String TextColor, int TextStyle)
    {
        if(Row.isEmpty()) return;
        final TableRow tableRow = new TableRow(m_context);
        TableLayout.LayoutParams trParams = new
                TableLayout.LayoutParams(TableLayout.LayoutParams.WRAP_CONTENT, TableLayout.LayoutParams.WRAP_CONTENT);
        trParams.setMargins(0, 0, 0, 0);
        tableRow.setLayoutParams(trParams);
        tableRow.setGravity(Gravity.CENTER);
        tableRow.setBackground(ContextCompat.getDrawable(m_context, BackGroundID));

        for (int i = 0;i<Row.size();i++) {
            final TextView tv = new TextView(m_context);
            tv.setLayoutParams(new
                    TableRow.LayoutParams((CellWidth > 0 ? DPtoPX(CellWidth) : TableRow.LayoutParams.WRAP_CONTENT),
                    CellHeight > 0 ? DPtoPX(CellHeight) : TableRow.LayoutParams.WRAP_CONTENT));
            tv.setText(Row.get(i));
            tv.setTextSize(TypedValue.COMPLEX_UNIT_PX, DPtoPX(RowTextSize));
            tv.setTypeface(tv.getTypeface(), TextStyle);
            tv.setGravity(Gravity.CENTER);
            tv.setTextColor(Color.parseColor(TextColor));
            tv.setPadding(DPtoPX(5), DPtoPX(5), DPtoPX(5), DPtoPX(5));
            tableRow.addView(tv);
        }
        m_Table.addView(tableRow);
    }

    // 체크 박스 삽입용, 무조건 초반 셀에 체크 박스가 들어가니 타이틀 셀보다 개수가 1개 적은 Row를 넣어야함
    void AddRowWithCheckBox(Vector<String> Row, float CellWidth, float CellHeight, float RowTextSize, int BackGroundID, String TextColor, int TextStyle)
    {
        if(Row.isEmpty()) return;
        final TableRow tableRow = new TableRow(m_context);
        TableLayout.LayoutParams trParams = new
                TableLayout.LayoutParams(TableLayout.LayoutParams.WRAP_CONTENT, TableLayout.LayoutParams.WRAP_CONTENT);
        trParams.setMargins(0, 0, 0, 0);
        tableRow.setLayoutParams(trParams);
        tableRow.setGravity(Gravity.CENTER);
        tableRow.setBackground(ContextCompat.getDrawable(m_context, BackGroundID));

        final LinearLayout layoutForChBox = new LinearLayout(m_context);
        LinearLayout.LayoutParams llParam = new
                TableRow.LayoutParams(TableRow.LayoutParams.MATCH_PARENT, TableRow.LayoutParams.MATCH_PARENT);
        layoutForChBox.setLayoutParams(llParam);
        layoutForChBox.setOrientation(LinearLayout.VERTICAL);
        layoutForChBox.setGravity(Gravity.CENTER);

        CheckBox box = new CheckBox(m_context);
        box.setChecked(false);
        box.setLayoutParams(new
                LinearLayout.LayoutParams((LinearLayout.LayoutParams.WRAP_CONTENT), LinearLayout.LayoutParams.WRAP_CONTENT));
        box.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener(){
            public void onCheckedChanged(CompoundButton btnView, boolean isChecked) {
                // 콤보박스 수행될 때 넣어야 함
                IsAllChecked();
            }
        });

        layoutForChBox.addView(box);
        tableRow.addView(layoutForChBox);
        
        for (int i = 0;i<Row.size();i++) {
            final TextView tv = new TextView(m_context);
            tv.setLayoutParams(new
                    TableRow.LayoutParams((CellWidth > 0 ? DPtoPX(CellWidth) : TableRow.LayoutParams.WRAP_CONTENT),
                    CellHeight > 0 ? DPtoPX(CellHeight) : TableRow.LayoutParams.WRAP_CONTENT));
            tv.setText(Row.get(i));
            tv.setTextSize(TypedValue.COMPLEX_UNIT_PX, DPtoPX(RowTextSize));
            tv.setTypeface(tv.getTypeface(), TextStyle);
            tv.setGravity(Gravity.CENTER);
            tv.setTextColor(Color.parseColor(TextColor));
            tv.setPadding(DPtoPX(5), DPtoPX(5), DPtoPX(5), DPtoPX(5));
            tableRow.addView(tv);
        }
        m_Table.addView(tableRow);
    }

    void IsAllChecked() {
        TableRow tableRow;
        LinearLayout linearLayout;
        CheckBox chBox;

        tableRow = (TableRow) m_Table.getChildAt(0);
        linearLayout = (LinearLayout) tableRow.getChildAt(0);
        chBox = (CheckBox) linearLayout.getChildAt(0);

        boolean Val = true;
        if (m_AllChecked == false && chBox.isChecked()) Val = true;
        else if (m_AllChecked == true && !chBox.isChecked()) Val = false;
        else return;

        m_AllChecked = Val;
        for (int i = 1; i < m_Table.getChildCount(); i++) {
            tableRow = (TableRow) m_Table.getChildAt(i);
            linearLayout = (LinearLayout) tableRow.getChildAt(0);
            chBox = (CheckBox) linearLayout.getChildAt(0);
            chBox.setChecked(Val);
        }
    }

    void DeleteCheckedRow(boolean isAccepted)
    {
        TableRow tableRow;
        LinearLayout linearLayout;
        CheckBox chBox;
        boolean isCheckedLeft = false;

        do {
            isCheckedLeft = false;
            for (int i = 1; i < m_Table.getChildCount(); i++) {
                tableRow = (TableRow) m_Table.getChildAt(i);
                linearLayout = (LinearLayout) tableRow.getChildAt(0);
                chBox = (CheckBox) linearLayout.getChildAt(0);
                isCheckedLeft ^= chBox.isChecked();
                if (chBox.isChecked()) {
                    m_Table.removeView(tableRow);
                    isCheckedLeft = true;
                    break;
                }
            }
        } while(isCheckedLeft);
    }

    Vector<Integer> CheckedGetIdRow(boolean isAccepted)
    {
        Vector<Integer> getId = new Vector<>();
        TableRow tableRow;
        LinearLayout linearLayout;
        CheckBox chBox;

        for (int i = 1; i < m_Table.getChildCount(); i++) {
            tableRow = (TableRow) m_Table.getChildAt(i);
            linearLayout = (LinearLayout) tableRow.getChildAt(0);
            chBox = (CheckBox) linearLayout.getChildAt(0);
            if (chBox.isChecked()) {
                getId.add(i-1);
            }
        }
        return getId;
    }

    void ClearTable()
    {
        m_Table.removeAllViews();
    }
}
