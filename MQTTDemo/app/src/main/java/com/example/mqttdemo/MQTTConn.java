package com.example.mqttdemo;

import android.content.Context;
import android.os.Build;

import androidx.annotation.RequiresApi;

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

import static com.example.mqttdemo.Constants.MQTTHOST;
import static com.example.mqttdemo.Constants.TOPIC_PROXIMIDAD;
import static com.example.mqttdemo.Constants.TOPIC_SONIDO;
import static com.example.mqttdemo.Constants.TOPIC_TEMPERATURA_ACTUAL;
import static com.example.mqttdemo.Constants.USERNAME;
import static com.example.mqttdemo.Constants.PASSWORD;

public class MQTTConn implements MqttCallback, IMqttActionListener{

    private static Dashboard dashboard;
    private static Context contextLose;

    static MqttAndroidClient client;


    String[] topicsSub = {
            "/swa/alarma",
            TOPIC_TEMPERATURA_ACTUAL,
            TOPIC_SONIDO,
            TOPIC_PROXIMIDAD
    };

    private static MQTTConn mqttConn = null;

    private MQTTConn (Context context) {
        connect(context);
    }


    public static MQTTConn getInstance(Context context, Dashboard dashboardClass) {

        if (mqttConn == null) {
            mqttConn = new MQTTConn(context);
        }
        contextLose = context;
        client.setCallback(dashboardClass);

        return mqttConn;
    }

    private void connect(Context context) {

        String clientId = MqttClient.generateClientId();
        client = new MqttAndroidClient(context, MQTTHOST, clientId);

        MqttConnectOptions options = new MqttConnectOptions();
        options.setUserName(USERNAME);
        options.setPassword(PASSWORD.toCharArray());

        client.setCallback(dashboard);

        try {
            IMqttToken token = client.connect(options);
            token.setActionCallback(new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    subscribe();
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {

                }
            });
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    private void subscribe () {
        IMqttToken subToken = new MqttDeliveryToken();
        int qos = 0;
        try {
            for (String topic: topicsSub) {
                subToken = client.subscribe(topic, qos);
            }

            subToken.setActionCallback(new IMqttActionListener() {
                @RequiresApi(api = Build.VERSION_CODES.KITKAT)
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {

                }

                @Override
                public void onFailure(IMqttToken asyncActionToken,
                                      Throwable exception) {

                }
            });
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void connectionLost(Throwable cause) {
        connect(contextLose);
    }

    @Override
    public void messageArrived(String topic, MqttMessage message) throws Exception {

    }

    @Override
    public void deliveryComplete(IMqttDeliveryToken token) {

    }

    @Override
    public void onSuccess(IMqttToken asyncActionToken) {

    }

    @Override
    public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
        connect(contextLose);
    }
}
