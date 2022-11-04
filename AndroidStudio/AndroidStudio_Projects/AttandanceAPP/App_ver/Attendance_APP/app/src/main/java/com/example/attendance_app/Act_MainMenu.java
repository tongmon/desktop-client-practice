package com.example.attendance_app;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Build;
import android.os.Bundle;
import android.os.RemoteException;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import org.altbeacon.beacon.Beacon;
import org.altbeacon.beacon.BeaconConsumer;
import org.altbeacon.beacon.BeaconManager;
import org.altbeacon.beacon.BeaconParser;
import org.altbeacon.beacon.MonitorNotifier;
import org.altbeacon.beacon.RangeNotifier;
import org.altbeacon.beacon.Region;

import java.util.Collection;
import java.util.concurrent.ExecutionException;

// 메인 화면
@SuppressLint("SetTextI18n")
@RequiresApi(api = Build.VERSION_CODES.O)
public class Act_MainMenu extends AppCompatActivity  implements BeaconConsumer  {

    private BeaconManager beaconManager;
    protected static final String TAG1 = "mainmenu : ";
    AttendOff attend_off;
    String device_id = "-99";
    static boolean isLogoutState;
    Button btn_Off;
    Button btn_Attendance;
    protected LocationManager locationManager;
    static double curLongtitude, curLatitude;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_act_mainmenu);
        try {
            attend_off = new AttendOff();
        } catch (Exception e) {
            e.printStackTrace();
        }

        isLogoutState = false;
        locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);

        // 네트워크랑 GPS 킬지 MainMenu에서 물어봄
        int permissionCheck = ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION);
        if (permissionCheck == PackageManager.PERMISSION_DENIED)
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, 0);
        permissionCheck = ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION);
        if (permissionCheck == PackageManager.PERMISSION_DENIED)
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.ACCESS_COARSE_LOCATION}, 0);
        permissionCheck = ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION);
        if (permissionCheck == PackageManager.PERMISSION_DENIED)
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.BLUETOOTH}, 0);
        GetLocation();

        // AutoPermissions.Companion.loadAllPermissions(this,101);
        beaconManager = BeaconManager.getInstanceForApplication(this);
        beaconManager.getBeaconParsers().add(new BeaconParser().setBeaconLayout("m:2-3=0215,i:4-19,i:20-21,i:22-23,p:24-24,d:25-25"));
        beaconManager.bind(this);
        // 대시 보드 버튼
        Button btn_DashBoard = (Button) findViewById(R.id.DashBoard_Button);
        btn_DashBoard.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                // TODO : click event
                Intent intent = new Intent(getApplicationContext(), Act_DashBoard.class);
                startActivity(intent);
                // button1.setVisibility(View.INVISIBLE);
                // button1.setVisibility(View.VISIBLE);
                // button1.setVisibility(View.GONE);
                // button1.getVisibility() == View.INVISIBLE
                // button1.setEnabled(false);
                // button1.setEnabled(true);
            }
        });

        // 근로 관리 버튼
        Button btn_WorkManagement = (Button) findViewById(R.id.WorkManagement_Button);
        btn_WorkManagement.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                // TODO : click event
                Intent intent = new Intent(getApplicationContext(), Act_SubWorkManager.class);
                startActivity(intent);
            }
        });

        // 출근 버튼
        btn_Attendance = (Button) findViewById(R.id.Attendance_Button);
        btn_Attendance.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    Boolean result;
                    String message = "출근 정보가 일치하지 않습니다.";
                    if((attend_off.workType == 1) || (attend_off.workType == 6)){
                        if(!device_id.equals("-99")){
                            result = attend_off.attendClick(true, attend_off.userNow.gps_x,attend_off.userNow.gps_y, device_id);
                            btn_Attendance.setEnabled(false);
                            btn_Attendance.setText("출근 : " + attend_off.userNow.attend_time);
                        }else{
                            result = false;
                        }
                        if(!result){
                            Toast.makeText(Act_MainMenu.this, message, Toast.LENGTH_LONG).show();
                        }
                    }else if((attend_off.workType == 2) || (attend_off.workType == 4) || (attend_off.workType == 5)){   // GPS 위치로 출퇴근 확인
                        Boolean ret = IsRightPoint(attend_off.userNow.gps_y, attend_off.userNow.gps_x, 1.5);
                        result = attend_off.attendClick(ret, curLongtitude,curLatitude, device_id);
                        System.out.println(result);
                        if(!result){
                            Toast.makeText(Act_MainMenu.this, message, Toast.LENGTH_LONG).show();
                        }else{
                            btn_Attendance.setEnabled(false);
                            btn_Attendance.setText("출근 : " + attend_off.userNow.attend_time);
                        }
                    }
                } catch (ExecutionException | InterruptedException e) {
                    e.printStackTrace();
                }

            }
        });

        // 퇴근 버튼
        btn_Off = (Button) findViewById(R.id.Off_Button);
        btn_Off.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick (View view){
                try {
                    Boolean result;
                    String message = "퇴근 정보가 일치하지 않습니다.";
                    System.out.println();
                    if((attend_off.workType == 1) || (attend_off.workType == 6)){
                        if(!device_id.equals("-99")){
                            result = attend_off.offClick(true, attend_off.userNow.gps_x,attend_off.userNow.gps_y, device_id);
                            System.out.println(result);
                            btn_Off.setEnabled(false);
                            btn_Off.setText("퇴근 : " + attend_off.userNow.off_time);
                        } else{
                            result = false;
                        }
                        if(!result){
                            Toast.makeText(Act_MainMenu.this, message, Toast.LENGTH_LONG).show();
                        }
                    }else if((attend_off.workType == 2) || (attend_off.workType == 4) || (attend_off.workType == 5)){   // GPS 위치로 출퇴근 확인
                        Boolean ret = IsRightPoint(attend_off.userNow.gps_y, attend_off.userNow.gps_x,1.5);
                        result = attend_off.offClick(ret, curLongtitude,curLatitude, device_id);
                        if(!result){
                            Toast.makeText(Act_MainMenu.this, message, Toast.LENGTH_LONG).show();
                        }else{
                            btn_Off.setEnabled(false);
                            btn_Off.setText("퇴근 : " + attend_off.userNow.off_time);
                        }
                    }
                } catch (ExecutionException | InterruptedException e) {
                    e.printStackTrace();
                }

            }
        });
        buttonIf();

    }
    public void buttonIf(){
        if(attend_off == null){                          // 로딩 중
            btn_Attendance.setEnabled(true); // f
            btn_Off.setEnabled(true); // f
        }else if(attend_off.userNow.attend_state == 0){  // 출근 버튼 클릭하지 않아도 되는 경우
            btn_Attendance.setEnabled(true); // f
            btn_Off.setEnabled(true);
            btn_Attendance.setText("출근 : " + attend_off.userNow.attend_time);
            btn_Off.setText("퇴근 : 미출근");
        }else if(attend_off.userNow.attend_state == 1 || attend_off.userNow.weekend_type == 1 || attend_off.userNow.vacation_type == 1){ // 출근, 퇴근 버튼 클릭하지 않아도 되는 경우
            System.out.println("Attend and Off Block");
            btn_Attendance.setEnabled(true); // f
            btn_Off.setEnabled(true); // f
            if(attend_off.userNow.attend_state == 1){
                btn_Attendance.setText("출근 : " + attend_off.userNow.attend_time);
                btn_Off.setText("퇴근 : " + attend_off.userNow.off_time);
            }else{
                btn_Attendance.setText("출근 : -");
                btn_Off.setText("퇴근 : -");
            }
        }else{  // 출근 버튼 클릭해야 되는 경우(퇴근 버튼 클릭  X)
            btn_Attendance.setText("출근 : 미출근");
            btn_Off.setText("퇴근 : 미출근");
            btn_Attendance.setEnabled(true);
            btn_Off.setEnabled(true); // f
        }
    }

    @Override
    protected void onStart() {
        super.onStart();
        try {
            attend_off = new AttendOff();
        } catch (Exception e) {
            e.printStackTrace();
        }
        if(isLogoutState)
            finish();
        device_id = "-99";
        GetLocation();
        beaconManager.bind( Act_MainMenu.this);
        buttonIf();
    }

    @Override
    protected void onStop() {
        super.onStop();
        locationManager.removeUpdates(mLocationListener);
        beaconManager.unbind( Act_MainMenu.this);
    }


    @Override
    public void onBeaconServiceConnect() {
        beaconManager.removeAllMonitorNotifiers();
        beaconManager.addMonitorNotifier(new MonitorNotifier() {
            @Override
            public void didEnterRegion(Region region) {
                device_id = "-1";
            }

            @Override
            public void didExitRegion(Region region) {
                device_id = "-99";
            }

            @Override
            public void didDetermineStateForRegion(int state, Region region) {
                if(state == 0){
                    device_id = "-1";
                    System.out.println(device_id);
                }else{
                    device_id = "-99";
                }
            }
        });

        beaconManager.addRangeNotifier(new RangeNotifier() {
            @Override
            public void didRangeBeaconsInRegion(Collection<Beacon> beacons, Region region) {
                if(beacons.size() > 0){
                    if(beacons.iterator().next().getDistance() < 1.5){
                        device_id = beacons.iterator().next().getId1().toString();
                    }
                }
            }
        });
        try {
            beaconManager.startRangingBeaconsInRegion(new Region("c2:02:DD:00:13:DD", null, null, null));
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }


    // 현재 앱 사용자가 위치해 있는 좌표와 거리 비교를 위한 출근, 퇴근해야 하는 장소인 x, y 위치를 받는다.
    // range(중심에서 반경을 의미) 안되면 컷
    public Boolean IsRightPoint(double latitude, double longitude, double range) {
        double curRange = Math.pow((latitude - curLatitude),2) + Math.pow((longitude - curLongtitude),2);
        System.out.println(latitude + ": " + longitude);
        System.out.println(curLatitude + ": " + curLongtitude);
        System.out.println(curRange);
        // 범위보다 밖에 있으면 출퇴근 기록 안되게 false 반환
        return (range * range) >= curRange ? true : false;
    }

    private final LocationListener mLocationListener = new LocationListener() {
        public void onLocationChanged(Location location) {
            //여기서 위치값이 갱신되면 이벤트가 발생한다.
            //값은 Location 형태로 리턴되며 좌표 출력 방법은 다음과 같다.

            Log.d("test", "onLocationChanged, location:" + location);
            curLongtitude = location.getLongitude(); //경도
            curLatitude = location.getLatitude();   //위도
        }
        public void onProviderDisabled(String provider) {
            // Disabled시
        }

        public void onProviderEnabled(String provider) {
            // Enabled시
        }

        public void onStatusChanged(String provider, int status, Bundle extras) {
            // 변경시
        }
    };

    // 위치 매니저로 위치 수정
    public void GetLocation()
    {
        boolean isGPSEnabled = locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER);
        boolean isNetworkEnabled = locationManager.isProviderEnabled(LocationManager.NETWORK_PROVIDER);

        int fineAccepted = ContextCompat.checkSelfPermission(Act_MainMenu.this.getBaseContext(),
                Manifest.permission.ACCESS_FINE_LOCATION);
        int coarseAccepted = ContextCompat.checkSelfPermission(Act_MainMenu.this.getBaseContext(),
                Manifest.permission.ACCESS_COARSE_LOCATION);

        if(fineAccepted == PackageManager.PERMISSION_GRANTED && coarseAccepted == PackageManager.PERMISSION_GRANTED) {
            if(isNetworkEnabled) {
                locationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER, // 등록할 위치제공자
                        100, // 통지사이의 최소 시간간격 (miliSecond)
                        1, // 통지사이의 최소 변경거리 (m)
                        mLocationListener);
            }
            if(isGPSEnabled) {
                locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, // 등록할 위치제공자
                        100, // 통지사이의 최소 시간간격 (miliSecond)
                        1, // 통지사이의 최소 변경거리 (m)
                        mLocationListener);
            }
        }
    }
}