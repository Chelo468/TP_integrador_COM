package com.example.mqttdemo;

import android.content.SharedPreferences;
import android.view.View;

import androidx.preference.PreferenceManager;

public class Preferences {

    private static Preferences preferences;
    public SharedPreferences sp;

    private Preferences (View vista) {
        sp = PreferenceManager.getDefaultSharedPreferences(vista.getContext());
    }

    public static Preferences getInstance(View vista) {
        if (preferences == null) {
            preferences = new Preferences(vista);
        }
        return preferences;
    }
}
