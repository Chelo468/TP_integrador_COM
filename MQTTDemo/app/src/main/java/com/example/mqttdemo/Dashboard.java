package com.example.mqttdemo;

import android.os.Build;
import android.os.Bundle;

import androidx.annotation.Nullable;
import androidx.annotation.RequiresApi;
import androidx.fragment.app.Fragment;

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
import static com.example.mqttdemo.Constants.TOPIC_TEMPERATURA_ACTUAL;

import java.nio.charset.StandardCharsets;

public class Dashboard extends Fragment implements MqttCallback, IMqttActionListener {

    private View mRootView;

    String topicStr = "/com/cuna/configTemperaturaMinima";
    String topicAlarmaSonido = "/com/cuna/switchAlarmaSonido";
    String topicAlarmaTemperatura = "/com/cuna/switchAlarmaTemperatura";
    String topicAlarmaProximidad = "/com/cuna/switchAlarmaProximidad";


    private static final String PERSISTENT_VARIABLE_BUNDLE_KEY = "persistentVariable";

    private TextView persistentVariableEdit;


    TextView txtTemperaturaActual;



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

        txtTemperaturaActual = (TextView) getView().findViewById(R.id.txtTemperaturaActual);

        if (savedInstanceState != null) {
            txtTemperaturaActual.setText(mySavedInstanceState.getString(PERSISTENT_VARIABLE_BUNDLE_KEY));
        }

    }

    /*
    public void publish (View v) {
        String topic = topicStr;
        String message = txtTemp.getText().toString();
        try {
            client.publish(topic, message.getBytes(), 0, false);
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    public void publishAlarmaTemp (View v) {
        String topic = topicAlarmaTemperatura;
        boolean message = swithTemp.isChecked();
        try {
            client.publish(topic, message ? "1".getBytes() : "0".getBytes(), 0, false);
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }
    */

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
                txtTemperaturaActual.setText("TEMPERATURA ACTUAL: " + msg + "°C");
                break;
            default:
                break;
        }
    }

    @Override
    public void deliveryComplete(IMqttDeliveryToken token) {

    }

}