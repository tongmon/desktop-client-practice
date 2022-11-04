package com.example.coronasearch;

import android.app.Activity;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.TextView;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.HashMap;
import java.util.Vector;
import java.util.concurrent.ExecutionException;
import java.util.regex.Pattern;

class Datast {
    public String date, time, x, y;
}

class GetInfoToURL extends AsyncTask<String,String,String> {
    HashMap local = new HashMap<String, Integer>();
    String test = "";
    public String save;
    TextView textview, t2;
    public String iD;
    public String json;
    String st = "false";
    public Vector<Datast> Info = new Vector<Datast>();

    GetInfoToURL() {
        local.put("seoul", 100);
        local.put("anyang", 101);
        local.put("suwon", 102);
    }

    protected String doInBackground(String... params) {
        try {
            String uri = params[0];
            URL url = new URL(uri);
            HttpURLConnection con = (HttpURLConnection) url.openConnection();
            StringBuilder sb = new StringBuilder();

            BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(con.getInputStream()));
            String j;
            while ((j = bufferedReader.readLine()) != null) {
                sb.append(j).append("\n");
            }
            save = sb.toString().trim();
            return sb.toString().trim();
        } catch (Exception e) {
            e.printStackTrace();
            return "false";
        }
    }

    protected void onPostExecute(String result) {
        json = result;
    }
}

public class SearchInput extends Activity {

    private EditText InputBox1;
    private TextView textView4;
    static GetInfoToURL InfoPatient = new GetInfoToURL();
    static boolean is_On = false;

    private ImageButton confirm;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.search_activity);

        InputBox1 = findViewById(R.id.InputBox1);
        textView4 = findViewById(R.id.textView4);
        final Intent intent = new Intent(getApplicationContext(), DongsunActivity.class);

        confirm = findViewById(R.id.confirm);

        View.OnClickListener clickListener = new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (v.getId() == R.id.confirm) {
                    boolean Valid = true;
                    String Data = InputBox1.getText().toString();
                    if (Data.length() < 5)
                        Valid = false;
                    if (Pattern.matches("\\d{3}\\#\\d+", Data) == false)
                        Valid = false;
                    if (Valid) // 올바른 입력일 시
                    {
                        // 여기서 넘겨줌
                        // 확인도 필요, Data가 검색할 환자 ID
                        InfoPatient.iD = Data;
                        if (!is_On) {
                            try {
                                InfoPatient.execute("http://18.217.217.150:3000/geo").get();
                                is_On = true;
                            } catch (ExecutionException e) {
                                e.printStackTrace();
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }
                        }
                        try {
                            JSONArray jsonObject = new JSONArray(InfoPatient.save);
                            for (int i = 0; i < jsonObject.length(); i++) {
                                JSONObject usersObject = jsonObject.getJSONObject(i);
                                if (usersObject.getString("id").equals(InfoPatient.iD)) {
                                    Datast buffer = new Datast();
                                    buffer.date = usersObject.getString("geodate");
                                    buffer.time = usersObject.getString("geotime");
                                    buffer.x = usersObject.getString("x_cord");
                                    buffer.y = usersObject.getString("y_cord");
                                    InfoPatient.Info.add(buffer);
                                }
                            }
                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                        if (!InfoPatient.Info.isEmpty())
                            startActivity(intent);
                        else {
                            textView4.setText("매칭되는 ID가 없습니다.");
                        }
                    } else // 잘못된 입력일 시
                    {
                        textView4.setText("잘못된 입력입니다.\n000#000 형식을 맞춰주세요.");
                    }
                }
            }
        };
        confirm.setOnClickListener(clickListener);
    }
}
