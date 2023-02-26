package com.example.attendance_app;

import android.annotation.SuppressLint;
import android.os.AsyncTask;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class GetDataJson {
    @SuppressLint("StaticFieldLeak")
    static class GetDataJSON extends AsyncTask<String, Void, String> {
        private String json;

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
                }

                return sb.toString().trim();
            } catch (Exception e) {
                System.out.println(e);
                return null;
            }
        }

        protected void onPostExecute(String result) {
            json = result;
        }
    }
}