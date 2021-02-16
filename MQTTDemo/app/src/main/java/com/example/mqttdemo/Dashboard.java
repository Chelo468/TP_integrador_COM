package com.example.mqttdemo;

import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Build;
import android.os.Bundle;

import androidx.annotation.ColorInt;
import androidx.annotation.Nullable;
import androidx.annotation.RequiresApi;
import androidx.cardview.widget.CardView;
import androidx.fragment.app.Fragment;
import androidx.preference.PreferenceManager;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import static com.example.mqttdemo.Constants.TOPIC_PROXIMIDAD;
import static com.example.mqttdemo.Constants.TOPIC_TEMPERATURA_ACTUAL;

import java.nio.charset.StandardCharsets;

public class Dashboard extends Fragment implements MqttCallback, IMqttActionListener {

    private View mRootView;
    private SharedPreferences sp;
    private int green = Color.argb(255,60,124,60);
    private int red = Color.argb(255, 204, 0, 0);





    private static final String PERSISTENT_VARIABLE_BUNDLE_KEY = "persistentVariable";

    private TextView persistentVariableEdit;


    TextView txtTemperaturaActual;
    CardView cardSonido;
    CardView cardProximidad;
    CardView cardTemperatura;


    public Dashboard() {
        // Required empty public constructor
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        MQTTConn.getInstance(getActivity().getApplicationContext(), this);

        if (savedInstanceState != null) {
            txtTemperaturaActual.setText(
                    savedInstanceState.getString("temperaturaActual", "")
            );
        }

    }

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        if (mRootView==null) {
            mRootView = inflater.inflate(R.layout.fragment_dashboard, container, false);
        }

        return mRootView;
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        Bundle mySavedInstanceState = getArguments();

        sp = PreferenceManager.getDefaultSharedPreferences(getContext());

        txtTemperaturaActual = (TextView) getView().findViewById(R.id.txtTemperaturaActual);
        cardSonido = (CardView) getView().findViewById(R.id.card_sonido);
        cardProximidad = (CardView) getView().findViewById(R.id.card_proximidad);
        cardTemperatura = (CardView) getView().findViewById(R.id.card_temperatura);

        cardProximidad.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                cardProximidad.setCardBackgroundColor(green);
            }
        });

        cardSonido.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                cardSonido.setBackgroundColor(green);
            }
        });


        if (savedInstanceState != null) {
            txtTemperaturaActual.setText(mySavedInstanceState.getString(PERSISTENT_VARIABLE_BUNDLE_KEY));
        }

    }

    @Override
    public void onSuccess(IMqttToken asyncActionToken) {

    }

    @Override
    public void onFailure(IMqttToken asyncActionToken, Throwable exception) {

    }

    @Override
    public void connectionLost(Throwable cause) {

    }

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    @Override
    public void messageArrived(String topic, MqttMessage message) throws Exception {
        String msg = new String(message.getPayload(), StandardCharsets.UTF_8);

        switch (topic) {
            case TOPIC_TEMPERATURA_ACTUAL:
                txtTemperaturaActual.setText(msg);

                String tempValue = msg.substring(13, 18);

                if (Double.parseDouble(tempValue) < sp.getInt("temp_minima",20)
                        || Double.parseDouble(tempValue) > sp.getInt("temp_maxima",40)) {
                    cardTemperatura.setBackgroundColor(red);
                } else {
                    cardTemperatura.setBackgroundColor(green);
                }

                break;
            case "/swa/alarma":
                if (msg.equals("Sonido")) {
                    cardSonido.setCardBackgroundColor(red);
                } else {
                    cardProximidad.setCardBackgroundColor(red);
                }
                break;

            default:
                break;
        }
    }

    @Override
    public void deliveryComplete(IMqttDeliveryToken token) {

    }

}