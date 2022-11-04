package com.example.coronasearch;

import android.app.Activity;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;

import java.io.BufferedInputStream;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;


public class SendFile extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        new DownloadFileFromURL().execute("18.217.217.150:3000");
    }

    public void sendFiles(){
        try {
            URL u = new URL("http://18.217.217.150:3000/");
            InputStream is = u.openStream();
            DataInputStream dis = new DataInputStream(is);

            byte[] buffer = new byte[1024];
            int length;

            FileOutputStream fos = new FileOutputStream(new File(Environment.getExternalStorageDirectory() + "/" + "data/test"));
            while ((length = dis.read(buffer)) > 0) {
                fos.write(buffer, 0, length);
            }

        } catch (MalformedURLException mue) {
            Log.e("ㅎㅇ", "비었음", mue);
        } catch (IOException ioe) {
            Log.e("ㅎㅇ2", "여기오면안됨", ioe);
        } catch (SecurityException se) {
            Log.e("ㅎㅇ3", "1212", se);
        }
    }
    class DownloadFileFromURL extends AsyncTask<String, String, String> {

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            Log.d("여기까지", "와야함");
        }
        // 업로드 역순
        @Override
        protected String doInBackground(String... f_url) {
            int count;
            try {
                URL url = new URL(f_url[0]);
                URLConnection conection = url.openConnection();
                conection.connect();

                int lenghtOfFile = conection.getContentLength();

                InputStream input = new BufferedInputStream(url.openStream(),
                        8192);

                OutputStream output = new FileOutputStream(Environment
                        .getExternalStorageDirectory().toString()
                        + "/data/downloadedfile");

                byte data[] = new byte[1024];

                long total = 0;

                while ((count = input.read(data)) != -1) {
                    total += count;
                    // 일단 쓰니까
                    publishProgress("" + (int) ((total * 100) / lenghtOfFile));

                    // 데이터 쓰기
                    output.write(data, 0, count);
                }

                // flushing output
                output.flush();

                // closing streams
                output.close();
                input.close();
            } catch (Exception e) {
                Log.e("에러", e.getMessage());
            }

            return null;
        }


        protected void onProgressUpdate(String... progress) {
        }


        @Override
        protected void onPostExecute(String file_url) {
        }
    }
}
