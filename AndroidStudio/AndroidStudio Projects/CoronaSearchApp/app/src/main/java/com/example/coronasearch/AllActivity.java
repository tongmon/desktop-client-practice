package com.example.coronasearch;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.select.Elements;

import java.io.IOException;


public class AllActivity extends AppCompatActivity {
    String test = "";
    String save;
    TextView t1, t2, t3;
    Document doc;
    Elements contents;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.all_activity);
        t1 = (TextView) findViewById(R.id.pa);
        t2 = (TextView) findViewById(R.id.die);
        t3 = (TextView) findViewById(R.id.heal);

        final Bundle bundle = new Bundle();

        new Thread() {
            @Override
            public void run() {
                doc = null;
                try {
                    doc = Jsoup.connect("https://www.kobic.re.kr/covid19/").timeout(3000).get();
                    contents = doc.select("em");
                    save = contents.toString();
                    save = save.replaceAll("<em>", "");
                    save = save.replaceAll("</em>", "");
                    String data[] = save.split("\\n");
                    for (int i = 1; i < 5; i++) {
                        test += data[i] + ':';
                    }
                    /* data[i], i=0~3 확진자수, 완치, 치료중, 사망*/

                    bundle.putString("numbers", test);
                    Message msg = handler.obtainMessage();
                    msg.setData(bundle);
                    handler.sendMessage(msg);
                } catch (IOException e) {
                    e.printStackTrace();
                    ;
                }
            }
        }.start();
    }

    Handler handler = new Handler(){
        @Override
        public void handleMessage(@org.jetbrains.annotations.NotNull Message msg) {
            Bundle bundle = msg.getData();
            String Data = bundle.getString("numbers");
            String Arr[] = new String[3];
            Arr = Data.split("[:]");
            t1.setText(Arr[0]);
            t2.setText(Arr[1]);
            t3.setText(Arr[2]);
        }
    };
}