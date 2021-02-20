package com.example.mqttdemo;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.text.InputType;
import android.widget.EditText;

import androidx.annotation.NonNull;
import androidx.preference.EditTextPreference;
import androidx.preference.PreferenceFragmentCompat;
import androidx.preference.PreferenceManager;

import org.eclipse.paho.client.mqttv3.MqttException;

import static com.example.mqttdemo.Constants.TOPIC_PROXIMIDAD;
import static com.example.mqttdemo.Constants.TOPIC_SONIDO;
import static com.example.mqttdemo.Constants.topicAlarmaProximidad;
import static com.example.mqttdemo.Constants.topicAlarmaSonido;
import static com.example.mqttdemo.Constants.topicAlarmaTemperatura;
import static com.example.mqttdemo.Constants.topicTemperaturaMaxima;
import static com.example.mqttdemo.Constants.topicTemperaturaMinima;
import static com.example.mqttdemo.MQTTConn.client;

public class Settings extends PreferenceFragmentCompat {

    private SharedPreferences sp;

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        setPreferencesFromResource(R.xml.root_preferences, rootKey);

        configuracionInicial();

        EditTextPreference tempMinima = findPreference("temp_minima");
        EditTextPreference tempMaxima = findPreference("temp_maxima");

        if (tempMinima != null) {
            tempMinima.setOnBindEditTextListener(new EditTextPreference.OnBindEditTextListener() {
                @Override
                public void onBindEditText(@NonNull EditText editText) {
                    editText.setInputType(InputType.TYPE_CLASS_NUMBER);
                }
            });
        }

        if (tempMaxima != null) {
            tempMaxima.setOnBindEditTextListener(new EditTextPreference.OnBindEditTextListener() {
                @Override
                public void onBindEditText(@NonNull EditText editText) {
                    editText.setInputType(InputType.TYPE_CLASS_NUMBER);
                }
            });
        }

        SharedPreferences.OnSharedPreferenceChangeListener preferenceChangeListener = new SharedPreferences.OnSharedPreferenceChangeListener() {
            @Override
            public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {
                if (key.equals("switch_temp")) {
                    boolean value = sharedPreferences.getBoolean("switch_temp", true);
                    publishAlarma(topicAlarmaTemperatura, value);
                } else if (key.equals("switch_sonido")) {
                    boolean value = sharedPreferences.getBoolean("switch_sonido", true);
                    publishAlarma(topicAlarmaSonido, value);
                } else if (key.equals("switch_proximidad")) {
                    boolean value = sharedPreferences.getBoolean("switch_proximidad", true);
                    publishAlarma(topicAlarmaProximidad, value);
                } else if (key.equals("temp_minima")) {
                    String value = sharedPreferences.getString("temp_minima", "");
                    publishTemperatura(topicTemperaturaMinima, value);
                } else if (key.equals("temp_maxima")) {
                    String value = sharedPreferences.getString("temp_maxima", "");
                    publishTemperatura(topicTemperaturaMaxima, value);
                }
            }
        };

        SharedPreferences sp = PreferenceManager.getDefaultSharedPreferences(getContext());

        sp.registerOnSharedPreferenceChangeListener(preferenceChangeListener);
    }

    private void configuracionInicial() {
        sp = PreferenceManager.getDefaultSharedPreferences(getContext());
        publishAlarma(topicAlarmaSonido, sp.getBoolean("switch_sonido", true));
        publishAlarma(topicAlarmaProximidad, sp.getBoolean("switch_proximidad", true));
        publishAlarma(topicAlarmaTemperatura, sp.getBoolean("switch_temp", true));
        publishTemperatura(topicTemperaturaMinima, sp.getString("temp_minima", ""));
        publishTemperatura(topicTemperaturaMaxima, sp.getString("temp_maxima", ""));
    }

    public void publishAlarma (String topic, boolean value) {
        try {
            client.publish(topic, value ? "1".getBytes() : "0".getBytes(), 0, false);
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    public void publishTemperatura (String topic, String value) {
        try {
            client.publish(topic, value.getBytes(), 0, false);
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

}