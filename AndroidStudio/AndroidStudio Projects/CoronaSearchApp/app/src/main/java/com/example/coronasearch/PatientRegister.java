package com.example.coronasearch;

import android.annotation.SuppressLint;
import android.os.AsyncTask;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.concurrent.ExecutionException;

public class PatientRegister {
    public String patientID; // 확진자 ID
    public boolean userStatus = true; // 접근허용상태
    String json;             // 서버에서 텍스트 가져오는 변수

    public boolean registerFunc(String authenticationCode) throws ExecutionException, InterruptedException {
        boolean result = userInputCheck(authenticationCode); // 유효성검사(공인인증코드, 지역번호)
        if (!userStatus || patientID == null || result == false) { // 값이 유효하지 않을 경우 종료
            System.out.println("Invalid Input Data!");
            return false;
        }
        result = patientInfoFinder(authenticationCode);
        return result;
    }

    // 유효성 검사 함수(codeArea(공인인증코드+지역번호))
    public boolean userInputCheck(String codeArea) throws ExecutionException, InterruptedException {
        boolean aCode = true, localCode = true;  // 공인인증코드 유효성, 지역번호 유효성
        int patientNum = 0;     // 환자번호
        String[] codeArray;    //공인인증코드, 지역번호
        String url = ""; // 지역을 정하기 위한 url

        if (codeArea == null) {     // 공인인증코드+지역번호 없을 경우 종료
            System.out.println("Wrong input!");
            return false;
        }
        codeArray = codeArea.split(",");

        if (aCode) {      // 지역번호 비교 후 해당 url 저장
            switch (codeArray[1]) {
                case "100":
                    url = "http://18.217.217.150:3000/seoul";
                    break;
                case "101":
                    url = "http://18.217.217.150:3000/anyang";
                    break;
                case "102":
                    url = "http://18.217.217.150:3000/suwon";
                    break;
                default:
                    localCode = false;
                    break;
            }
            if (localCode) {
                json = new GetDataJSON().execute(url).get();
                patientNum = patientCntJsonParsing(json);
                patientID = codeArray[1] + "#" + patientNum;
                return true;
            }
        }
        return false;
    }

    public boolean patientInfoFinder(String codeArea) throws ExecutionException, InterruptedException {
        String[] codeArray;    //공인인증코드, 지역번호
        String url = "";
        boolean localCode = true;

        codeArray = codeArea.split(",");
        switch (codeArray[1]) {
            case "100":
                url = "http://18.217.217.150:3000/seoul/new";
                break;
            case "101":
                url = "http://18.217.217.150:3000/anyang/new";
                break;
            case "102":
                url = "http://18.217.217.150:3000/suwon/new";
                break;
            default:
                localCode = false;
                break;
        }
        if (localCode) {
            String postResult2 = new Post().execute(url, codeArray[1]).get();
            if(postResult2.equals("true"))
                return true;
        }
        return false;
    }

    // 테이블 값을 서버로부터 가져오는 클래스
    @SuppressLint("StaticFieldLeak")
    class GetDataJSON extends AsyncTask<String, Void, String> {
        protected String doInBackground(String... params) {
            String uri = params[0];

            BufferedReader bufferedReader = null;
            try {
                URL url = new URL(uri);
                HttpURLConnection con = (HttpURLConnection) url.openConnection();
                StringBuilder sb = new StringBuilder();

                bufferedReader = new BufferedReader(new InputStreamReader(con.getInputStream()));

                String j;
                while ((j = bufferedReader.readLine()) != null) {
                    sb.append(j).append("\n");
                    System.out.println("sb: " + sb);        // 테스트
                }

                return sb.toString().trim();
            } catch (Exception e) {
                return null;
            }
        }
        protected void onPostExecute(String result) {
            json = result;
        }
    }

    // patient 테이블을 데이터로 갖고 온 후 확진자 수 구하는 함수
    protected int patientCntJsonParsing(String json) {
        int num = 0;        // 지역 확진자 수
        try {
            if(json == null){
                return num+1;
            }
            JSONArray jsonObject = new JSONArray(json);

            for (int i = 0; i < jsonObject.length(); i++) {
                JSONObject usersObject = jsonObject.getJSONObject(i);
                num = usersObject.getInt("no");
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
        num = num + 1;
        return num;
    }

    // insert json 파일로 만드는 함수
    JSONObject insert(String number) throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.accumulate("choice_local", number);
        jsonObject.accumulate("choice_name", patientID);
        return jsonObject;
    }

    // 데이터베이스에 POST 하는 클래스
    class Post extends AsyncTask<String, String, String> {
        @Override
        protected String doInBackground(String... params) {
            try {
                JSONObject jsonObject = insert(params[1]);
                HttpURLConnection con = null;
                BufferedReader reader = null;
                try {
                    URL url = new URL(params[0]);
                    con = (HttpURLConnection) url.openConnection();
                    con.setRequestMethod("POST");
                    con.setRequestProperty("Cache-Control", "no-cache");    //캐시 설정
                    con.setRequestProperty("Content-Type", "application/json");
                    con.setDoOutput(true);
                    con.setDoInput(true);
                    con.connect();
                    OutputStream outStream = con.getOutputStream();
                    BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(outStream));
                    writer.write(jsonObject.toString());
                    writer.flush();
                    writer.close();
                    InputStream stream = con.getInputStream();
                    reader = new BufferedReader(new InputStreamReader(stream));
                    StringBuffer buffer = new StringBuffer();
                    String line = "";
                    while((line = reader.readLine()) != null){
                        buffer.append(line);
                    }
                    return "true";
                } catch (IOException e) {
                    e.printStackTrace();
                } finally {
                    if (con != null) {
                        con.disconnect();
                    }
                    try {
                        if (reader != null) {
                            reader.close();
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
            return "false";
        }
        @Override
        protected void onPostExecute(String result) {
            super.onPostExecute(result);
        }
    }
}
