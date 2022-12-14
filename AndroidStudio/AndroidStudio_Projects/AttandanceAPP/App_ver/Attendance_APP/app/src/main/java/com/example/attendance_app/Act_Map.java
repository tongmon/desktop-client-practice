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
                    Toast.makeText(Act_Map.this, "????????? ????????? ???????????? ????????? ???????????????", Toast.LENGTH_LONG).show();
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
                    // editText??? ????????? ?????????(??????, ??????, ?????? ???)??? ?????? ????????? ????????? ??????
                    addressList = geocoder.getFromLocationName(
                            str, // ??????
                            10); // ?????? ?????? ?????? ??????
                }
                catch (IOException e) {
                    e.printStackTrace();
                }

                if(addressList == null || addressList.isEmpty()){
                    Toast.makeText(Act_Map.this, "?????? ??? ?????? ???????????????.", Toast.LENGTH_LONG).show();
                    return;
                }

                System.out.println(addressList.get(0).toString());
                // ????????? ???????????? split
                String []splitStr = addressList.get(0).toString().split(",");
                String address = splitStr[0].substring(splitStr[0].indexOf("\"") + 1,splitStr[0].length() - 2); // ??????
                System.out.println(address);

                String latitude = splitStr[10].substring(splitStr[10].indexOf("=") + 1); // ??????
                String longitude = splitStr[12].substring(splitStr[12].indexOf("=") + 1); // ??????
                System.out.println(latitude);
                System.out.println(longitude);

                curCoord = new LatLng(Double.parseDouble(latitude), Double.parseDouble(longitude));

                // ??????(??????, ??????) ??????
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

        // request code??? ???????????? ?????? ??????
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

        // NaverMap ?????? ????????? NaverMap ????????? ?????? ?????? ??????
        mNaverMap = naverMap;
        mNaverMap.setLocationSource(mLocationSource);

        // UI ????????? ?????????
        UiSettings uiSettings = mNaverMap.getUiSettings();
        uiSettings.setCompassEnabled(true); // ????????? : true
        uiSettings.setScaleBarEnabled(true); // ????????? : true
        uiSettings.setZoomControlEnabled(true); // ????????? : true
        uiSettings.setLocationButtonEnabled(true); // ????????? : false
        uiSettings.setLogoGravity(Gravity.RIGHT | Gravity.BOTTOM);

        // ????????????. ????????? onRequestPermissionsResult ?????? ????????? ??????
        ActivityCompat.requestPermissions(this, PERMISSIONS, PERMISSION_REQUEST_CODE);

        curCoord = new LatLng(Act_MainMenu.curLatitude, Act_MainMenu.curLongtitude);

        /*
        // ????????? ?????? ?????? (?????? 1km ?????? ???????????? ???????????? ?????????, ??? 4??????)
        //1km ???????????? ????????? ???????????? ????????? ?????????.
        //?????? ????????? ???????????? ?????? ??? ????????? ????????? ???????????? ?????????
        //????????? ????????? ????????? ?????? ?????? ??? ????????? ????????? ????????? ???????????????
        //?????? for?????? ?????? ?????? ???????????? ???????????? ?????? ????????? ?????? ??????????
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
                // ????????? ?????????????????? ???????????? ????????????????????? ?????? ??????
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
        // ????????? ????????? ??????...
        return false;
    }

    // ?????? ?????? ???????????? ????????? ??????
    private Vector<LatLng> markersPosition;
    private Vector<Marker> activeMarkers;

    // ?????? ???????????? ???????????? ??????
    public LatLng getCurrentPosition(NaverMap naverMap) {
        CameraPosition cameraPosition = naverMap.getCameraPosition();
        return new LatLng(cameraPosition.target.latitude, cameraPosition.target.longitude);
    }

    // ????????? ????????? ????????? ????????????(???????????? ???????????? ?????? ?????? 3km ???)??? ????????? ??????
    public final static double REFERANCE_LAT = 1 / 109.958489129649955;
    public final static double REFERANCE_LNG = 1 / 88.74;
    public final static double REFERANCE_LAT_X3 = 3 / 109.958489129649955;
    public final static double REFERANCE_LNG_X3 = 3 / 88.74;
    public boolean withinSightMarker(LatLng currentPosition, LatLng markerPosition) {
        boolean withinSightMarkerLat = Math.abs(currentPosition.latitude - markerPosition.latitude) <= REFERANCE_LAT_X3;
        boolean withinSightMarkerLng = Math.abs(currentPosition.longitude - markerPosition.longitude) <= REFERANCE_LNG_X3;
        return withinSightMarkerLat && withinSightMarkerLng;
    }

    // ???????????? ?????????????????? ????????? ???????????? ??????
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