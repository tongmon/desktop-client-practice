package com.example.attendance_app;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.fragment.app.FragmentManager;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.location.Address;
import android.location.Geocoder;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.naver.maps.geometry.LatLng;
import com.naver.maps.map.CameraAnimation;
import com.naver.maps.map.CameraPosition;
import com.naver.maps.map.CameraUpdate;
import com.naver.maps.map.LocationTrackingMode;
import com.naver.maps.map.MapView;
import com.naver.maps.map.NaverMap;
import com.naver.maps.map.OnMapReadyCallback;
import com.naver.maps.map.UiSettings;
import com.naver.maps.map.overlay.Marker;
import com.naver.maps.map.overlay.Overlay;
import com.naver.maps.map.overlay.OverlayImage;
import com.naver.maps.map.util.FusedLocationSource;
import com.naver.maps.map.widget.CompassView;
import com.naver.maps.map.widget.LocationButtonView;
import com.naver.maps.map.widget.ScaleBarView;
import com.naver.maps.map.widget.ZoomControlView;

import org.json.JSONException;

import java.io.IOException;
import java.util.List;
import java.util.Vector;
import java.util.concurrent.ExecutionException;

public class Act_Map extends AppCompatActivity implements OnMapReadyCallback, Overlay.OnClickListener {
    private MapView mapView;

    private static final int PERMISSION_REQUEST_CODE = 100;
    private static final String[] PERMISSIONS = {
            Manifest.permission.ACCESS_FINE_LOCATION,
            Manifest.permission.ACCESS_COARSE_LOCATION
    };

    private Geocoder geocoder;
    private FusedLocationSource mLocationSource;
    private NaverMap mNaverMap;
    private LatLng curCoord;
    Marker curMaker;
    EditText etext_Location;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_act_map);

        mapView = findViewById(R.id.map_view);
        mapView.onCreate(savedInstanceState);
        mapView.getMapAsync(this);

        geocoder = new Geocoder(this.getBaseContext());
        curCoord = new LatLng(0, 0);
        curMaker = new Marker();

        etext_Location = (EditText) findViewById(R.id.etext_location_Input); // etext_ID.getText().toString()

        Button btn_ok = (Button) findViewById(R.id.button_ok);
        btn_ok.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick (View view) {
                List<Address> citylist = null;
                try {
                    citylist = geocoder.getFromLocation(curCoord.latitude,curCoord.longitude,10);
                } catch (IOException e) {
                    e.printStackTrace();
                }
                if(citylist != null && !citylist.isEmpty()) {
                    String locality = citylist.get(0).getLocality();
                    String featureName = citylist.get(0).getFeatureName();
                    String thoroughfare = citylist.get(0).getThoroughfare();
                    Act_RequestInfo.locName = (locality == null ? "" : locality) + " " +
                            (thoroughfare == null ? "" : thoroughfare) + " " + (featureName == null ? "" : featureName);
                    Act_RequestInfo.mapLongitude = curCoord.longitude;
                    Act_RequestInfo.mapLatitude = curCoord.latitude;
                    finish();
                }
                else{
                    Toast.makeText(Act_Map.this, "올바른 지역을 선택하고 확인을 눌러주세요", Toast.LENGTH_LONG).show();
                }
            }
        });

        Button btn_cancel = (Button) findViewById(R.id.button_cancel);
        btn_cancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick (View view) {
                finish();
            }
        });

        Button btn_search = (Button) findViewById(R.id.button_search);
        btn_search.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick (View view) {
                String str = etext_Location.getText().toString();
                List<Address> addressList = null;
                try {
                    // editText에 입력한 텍스트(주소, 지역, 장소 등)을 지오 코딩을 이용해 변환
                    addressList = geocoder.getFromLocationName(
                            str, // 주소
                            10); // 최대 검색 결과 개수
                }
                catch (IOException e) {
                    e.printStackTrace();
                }

                if(addressList == null || addressList.isEmpty()){
                    Toast.makeText(Act_Map.this, "찾을 수 없는 지역입니다.", Toast.LENGTH_LONG).show();
                    return;
                }

                System.out.println(addressList.get(0).toString());
                // 콤마를 기준으로 split
                String []splitStr = addressList.get(0).toString().split(",");
                String address = splitStr[0].substring(splitStr[0].indexOf("\"") + 1,splitStr[0].length() - 2); // 주소
                System.out.println(address);

                String latitude = splitStr[10].substring(splitStr[10].indexOf("=") + 1); // 위도
                String longitude = splitStr[12].substring(splitStr[12].indexOf("=") + 1); // 경도
                System.out.println(latitude);
                System.out.println(longitude);

                curCoord = new LatLng(Double.parseDouble(latitude), Double.parseDouble(longitude));

                // 좌표(위도, 경도) 생성
                LatLng point = new LatLng(curCoord.latitude, curCoord.longitude);
                curMaker.setMap(null);
                curMaker.setPosition(point);
                curMaker.setMap(mNaverMap);

                mNaverMap.moveCamera(CameraUpdate.scrollTo(point).animate(CameraAnimation.Easing, 1000));
            }
        });
    }

    @Override
    protected void onStart() {
        super.onStart();
        mapView.onStart();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mapView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        mapView.onPause();
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        mapView.onSaveInstanceState(outState);
    }

    @Override
    protected void onStop() {
        super.onStop();
        mapView.onStop();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mapView.onDestroy();
    }

    @Override
    public void onLowMemory() {
        super.onLowMemory();
        mapView.onLowMemory();
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);

        // request code와 권한획득 여부 확인
        if (requestCode == PERMISSION_REQUEST_CODE) {
            if (grantResults.length > 0
                    && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                mNaverMap.setLocationTrackingMode(LocationTrackingMode.Follow);
            }
        }
    }

    @Override
    public void onMapReady(@NonNull NaverMap naverMap) {

        curMaker.setPosition(new LatLng(37.5670135, 126.9783740));
        curMaker.setMap(naverMap);
        // geocoder = new Geocoder(this);
        // marker.setWidth(100);
        // marker.setHeight(100);
        // marker.setIcon(OverlayImage.fromResource(R.drawable.btn_profile_img));
        // marker.setOnClickListener(this);

        // NaverMap 객체 받아서 NaverMap 객체에 위치 소스 지정
        mNaverMap = naverMap;
        mNaverMap.setLocationSource(mLocationSource);

        // UI 컨트롤 재배치
        UiSettings uiSettings = mNaverMap.getUiSettings();
        uiSettings.setCompassEnabled(true); // 기본값 : true
        uiSettings.setScaleBarEnabled(true); // 기본값 : true
        uiSettings.setZoomControlEnabled(true); // 기본값 : true
        uiSettings.setLocationButtonEnabled(true); // 기본값 : false
        uiSettings.setLogoGravity(Gravity.RIGHT | Gravity.BOTTOM);

        // 권한확인. 결과는 onRequestPermissionsResult 콜백 매서드 호출
        ActivityCompat.requestPermissions(this, PERMISSIONS, PERMISSION_REQUEST_CODE);

        curCoord = new LatLng(Act_MainMenu.curLatitude, Act_MainMenu.curLongtitude);

        /*
        // 마커들 위치 정의 (대충 1km 간격 동서남북 방향으로 만개씩, 총 4만개)
        //1km 간격으로 바둑판 형식으로 마커가 생긴다.
        //그럼 여기서 데이터를 받아 온 다음에 마커를 생성하면 되겠네
        //스팟의 정보를 여기서 전부 받아 온 다음에 한번에 마커로 생성해주자
        //밑에 for문에 전체 스팟 리스트로 사이즈를 주고 만들면 될거 같은데?
        LatLng initialPosition = new LatLng(Act_MainMenu.curLatitude, Act_MainMenu.curLongtitude);
        markersPosition = new Vector<LatLng>();
        for (int x = 0; x < 100; ++x) {
            for (int y = 0; y < 100; ++y) {
                markersPosition.add(new LatLng(
                        initialPosition.latitude - (REFERANCE_LAT * x),
                        initialPosition.longitude + (REFERANCE_LNG * y)
                ));
                markersPosition.add(new LatLng(
                        initialPosition.latitude + (REFERANCE_LAT * x),
                        initialPosition.longitude - (REFERANCE_LNG * y)
                ));
                markersPosition.add(new LatLng(
                        initialPosition.latitude + (REFERANCE_LAT * x),
                        initialPosition.longitude + (REFERANCE_LNG * y)
                ));
                markersPosition.add(new LatLng(
                        initialPosition.latitude - (REFERANCE_LAT * x),
                        initialPosition.longitude - (REFERANCE_LNG * y)
                ));
            }
        }
        */

        naverMap.setOnMapClickListener((point, coord) ->
                {
                    curCoord = coord;
                    curMaker.setMap(null);
                    curMaker.setPosition(new LatLng(coord.latitude, coord.longitude));
                    curMaker.setMap(mNaverMap);
                    mNaverMap.moveCamera(CameraUpdate.scrollTo(coord).animate(CameraAnimation.Easing, 1000));
                }
        );

        naverMap.addOnCameraChangeListener(new NaverMap.OnCameraChangeListener() {
            @Override
            public void onCameraChange(int reason, boolean animated) {
                /*
                freeActiveMarkers();
                // 정의된 마커위치들중 가시거리 내에있는것들만 마커 생성
                LatLng currentPosition = getCurrentPosition(naverMap);
                for (LatLng markerPosition: markersPosition) {
                    if (!withinSightMarker(currentPosition, markerPosition))
                        continue;
                    Marker marker = new Marker();
                    marker.setPosition(markerPosition);
                    marker.setMap(naverMap);
                    activeMarkers.add(marker);
                }
                */
            }
        });
    }

    @Override
    public boolean onClick(@NonNull Overlay overlay) {
        // 마커가 클릭된 경우...
        return false;
    }

    // 마커 정보 저장시킬 변수들 선언
    private Vector<LatLng> markersPosition;
    private Vector<Marker> activeMarkers;

    // 현재 카메라가 보고있는 위치
    public LatLng getCurrentPosition(NaverMap naverMap) {
        CameraPosition cameraPosition = naverMap.getCameraPosition();
        return new LatLng(cameraPosition.target.latitude, cameraPosition.target.longitude);
    }

    // 선택한 마커의 위치가 가시거리(카메라가 보고있는 위치 반경 3km 내)에 있는지 확인
    public final static double REFERANCE_LAT = 1 / 109.958489129649955;
    public final static double REFERANCE_LNG = 1 / 88.74;
    public final static double REFERANCE_LAT_X3 = 3 / 109.958489129649955;
    public final static double REFERANCE_LNG_X3 = 3 / 88.74;
    public boolean withinSightMarker(LatLng currentPosition, LatLng markerPosition) {
        boolean withinSightMarkerLat = Math.abs(currentPosition.latitude - markerPosition.latitude) <= REFERANCE_LAT_X3;
        boolean withinSightMarkerLng = Math.abs(currentPosition.longitude - markerPosition.longitude) <= REFERANCE_LNG_X3;
        return withinSightMarkerLat && withinSightMarkerLng;
    }

    // 지도상에 표시되고있는 마커들 지도에서 삭제
    private void freeActiveMarkers() {
        if (activeMarkers == null) {
            activeMarkers = new Vector<Marker>();
            return;
        }
        for (Marker activeMarker: activeMarkers) {
            activeMarker.setMap(null);
        }
        activeMarkers = new Vector<Marker>();
    }
}