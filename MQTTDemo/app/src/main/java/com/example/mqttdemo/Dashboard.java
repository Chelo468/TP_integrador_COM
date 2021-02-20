package com.example.mqttdemo;

import android.app.AlertDialog;
import android.content.DialogInterface;
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

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;

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
import org.eclipse.paho.client.mqttv3.logging.LoggerFactory;

import static com.example.mqttdemo.Constants.TOPIC_PROXIMIDAD;
import static com.example.mqttdemo.Constants.TOPIC_TEMPERATURA_ACTUAL;

import java.lang.reflect.Type;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Logger;

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

        sp = Preferences.getInstance(this.getView()).sp;

        txtTemperaturaActual = (TextView) getView().findViewById(R.id.txtTemperaturaActual);
        cardSonido = (CardView) getView().findViewById(R.id.card_sonido);
        cardProximidad = (CardView) getView().findViewById(R.id.card_proximidad);
        cardTemperatura = (CardView) getView().findViewById(R.id.card_temperatura);


        cardProximidad.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
                builder.setMessage("Desea desactivar la alarma?")
                        .setPositiveButton("Si", new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                cardProximidad.setCardBackgroundColor(green);
                            }
                        })
                        .setNegativeButton("No", new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                // User cancelled the dialog
                            }
                        });
                // Create the AlertDialog object and return it
                AlertDialog dialog = builder.create();
                dialog.show();
            }
        });

        cardSonido.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
                builder.setMessage("Desea desactivar la alarma?")
                        .setPositiveButton("Si", new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                cardSonido.setBackgroundColor(green);
                            }
                        })
                        .setNegativeButton("No", new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                // User cancelled the dialog
                            }
                        });
                // Create the AlertDialog object and return it
                AlertDialog dialog = builder.create();
                dialog.show();
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
        MQTTConn.getInstance(getContext(), this).connectionLost(cause);
    }

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    @Override
    public void messageArrived(String topic, MqttMessage message) throws Exception {
        String msg = new String(message.getPayload(), StandardCharsets.UTF_8);

        switch (topic) {
            case TOPIC_TEMPERATURA_ACTUAL:
                txtTemperaturaActual.setText(msg);

                Double tempValue = Double.parseDouble(msg.substring(13, 18));
                Double tempMinima = Double.parseDouble(sp.getString("temp_minima", "0"));
                Double tempMaxima = Double.parseDouble(sp.getString("temp_maxima", "0"));

                if ( tempValue < tempMinima
                        || tempValue > tempMaxima) {
                    cardTemperatura.setBackgroundColor(red);
                } else {
                    cardTemperatura.setBackgroundColor(green);
                }

                break;
            case "/swa/alarma":
                if (msg.equals("Sonido")) {
                    cardSonido.setCardBackgroundColor(red);
                } else if (msg.equals("Proximidad")) {
                    cardProximidad.setCardBackgroundColor(red);
                } else if (msg.contains("Temperatura")){
                    cardTemperatura.setBackgroundColor(red);
                }
                break;
            case "/com/cuna/myConfiguration":
                try {
                    Config configs = new Gson().fromJson(msg, Config.class);
                    SharedPreferences.Editor editor = sp.edit();
                    editor.putString("temp_minima", configs.getTemperatura_minima());
                    editor.putString("temp_maxima", configs.getTemperatura_maxima());
                    editor.putBoolean("switch_temp", configs.esAlarmaTemperaturaActivada());
                    editor.putBoolean("switch_sonido", configs.esAlarmaSonidoActivada());
                    editor.putBoolean("switch_proximidad", configs.esAlarmaProximidadActivada());
                    editor.commit();
                } catch (Exception e) {
                    e.printStackTrace();
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