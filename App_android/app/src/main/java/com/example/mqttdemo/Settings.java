package com.example.mqttdemo;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.text.InputType;
import android.widget.BaseAdapter;
import android.widget.EditText;

import androidx.annotation.NonNull;
import androidx.preference.EditTextPreference;
import androidx.preference.Preference;
import androidx.preference.PreferenceFragmentCompat;
import androidx.preference.PreferenceGroup;
import androidx.preference.PreferenceManager;
import androidx.preference.PreferenceScreen;
import androidx.preference.SwitchPreferenceCompat;

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

        getConfiguration();
        sp = Preferences.getInstance(this.getView()).sp;

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
                    SwitchPreferenceCompat temp = (SwitchPreferenceCompat) findPreference("switch_temp");
                    temp.setChecked(value);
                    publishAlarma(topicAlarmaTemperatura, value);
                } else if (key.equals("switch_sonido")) {
                    boolean value = sharedPreferences.getBoolean("switch_sonido", true);
                    SwitchPreferenceCompat sonido = (SwitchPreferenceCompat) findPreference("switch_sonido");
                    sonido.setChecked(value);
                    publishAlarma(topicAlarmaSonido, value);
                } else if (key.equals("switch_proximidad")) {
                    boolean value = sharedPreferences.getBoolean("switch_proximidad", true);
                    SwitchPreferenceCompat proximidad = (SwitchPreferenceCompat) findPreference("switch_sonido");
                    proximidad.setChecked(value);
                    publishAlarma(topicAlarmaProximidad, value);
                } else if (key.equals("temp_minima")) {
                    String value = sharedPreferences.getString("temp_minima", "");
                    EditTextPreference temp_min = (EditTextPreference) findPreference("temp_minima");
                    temp_min.setText(value);
                    publishTemperatura(topicTemperaturaMinima, value);
                } else if (key.equals("temp_maxima")) {
                    String value = sharedPreferences.getString("temp_maxima", "");
                    EditTextPreference temp_max = (EditTextPreference) findPreference("temp_maxima");
                    temp_max.setText(value);
                    publishTemperatura(topicTemperaturaMaxima, value);
                }

                getConfiguration();
            }
        };

        sp.registerOnSharedPreferenceChangeListener(preferenceChangeListener);

    }

    private void configuracionInicial() {
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

    public void getConfiguration() {
        try {
            client.publish("/com/cuna/getConfiguration", "".getBytes(), 0, false);
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

}