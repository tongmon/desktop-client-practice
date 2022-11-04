package com.example.coronasearch;

import android.graphics.Color;
import android.os.Bundle;
import android.widget.SeekBar;

import androidx.fragment.app.FragmentActivity;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.Polyline;
import com.google.android.gms.maps.model.PolylineOptions;

import static com.example.coronasearch.SearchInput.InfoPatient;

public class DongsunActivity extends FragmentActivity implements OnMapReadyCallback {

    private GoogleMap mMap;
    private LatLng startLatLng = new LatLng(0, 0);
    private LatLng endLatLng = new LatLng(0, 0);
    private Polyline line;
    private SeekBar seekBar;
    private int Limit = 12;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_dongsun);

        seekBar  = (SeekBar) findViewById(R.id.seekBar);

        SupportMapFragment mapFragment = (SupportMapFragment) getSupportFragmentManager()
                .findFragmentById(R.id.map);
        mapFragment.getMapAsync(this);
        seekBar.setProgress(Limit);
        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            public void onStopTrackingTouch(SeekBar seekBar) {
                mMap.clear();
                Limit = seekBar.getProgress();
                endLatLng = new LatLng(Double.parseDouble(InfoPatient.Info.get(0).x), Double.parseDouble(InfoPatient.Info.get(0).y));
                for (int i = 1; i < InfoPatient.Info.size() * Limit / 25; i++) {
                    startLatLng = new LatLng(endLatLng.latitude, endLatLng.longitude);
                    endLatLng = new LatLng(Double.parseDouble(InfoPatient.Info.get(i).x), Double.parseDouble(InfoPatient.Info.get(i).y));
                    drawPath();
                }
            }

            public void onStartTrackingTouch(SeekBar seekBar) {
                //tv.setText("onStart TrackingTouch");
            }

            public void onProgressChanged(SeekBar seekBar, int progress,
                                          boolean fromUser) {
                mMap.clear();
                Limit = seekBar.getProgress();
                endLatLng = new LatLng(Double.parseDouble(InfoPatient.Info.get(0).x), Double.parseDouble(InfoPatient.Info.get(0).y));
                for (int i = 1; i < InfoPatient.Info.size() * Limit / 25; i++) {
                    startLatLng = new LatLng(endLatLng.latitude, endLatLng.longitude);
                    endLatLng = new LatLng(Double.parseDouble(InfoPatient.Info.get(i).x), Double.parseDouble(InfoPatient.Info.get(i).y));
                    drawPath();
                }
            }
        });
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();
        InfoPatient.Info.clear();
        line.remove();
        //finish();
    }

    @Override
    public void onMapReady(GoogleMap googleMap) {
        mMap = googleMap;
        endLatLng = new LatLng(Double.parseDouble(InfoPatient.Info.get(0).x), Double.parseDouble(InfoPatient.Info.get(0).y));
        mMap.moveCamera(CameraUpdateFactory.newLatLng(endLatLng));
        mMap.moveCamera(CameraUpdateFactory.newLatLngZoom(endLatLng,13));
        for (int i = 1; i < InfoPatient.Info.size() * Limit / 25; i++){
            startLatLng = new LatLng(endLatLng.latitude, endLatLng.longitude);
            endLatLng = new LatLng(Double.parseDouble(InfoPatient.Info.get(i).x), Double.parseDouble(InfoPatient.Info.get(i).y));
            drawPath();
        }
    }

    private void drawPath()
    {
        PolylineOptions options = new PolylineOptions().add(startLatLng).add(endLatLng).width(15).color(Color.RED).geodesic(true);
        line = mMap.addPolyline(options);
    }
}