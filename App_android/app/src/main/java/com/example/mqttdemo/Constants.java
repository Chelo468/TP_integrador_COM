package com.example.mqttdemo;

public class Constants {

    static final String TOPIC_TEMPERATURA_ACTUAL = "/swa/temperatura";
    static final String TOPIC_SONIDO = "/swa/sonido";
    static final String TOPIC_PROXIMIDAD = "/swa/proximidad";
    static final String MQTTHOST = "tcp://broker.hivemq.com:1883";
    static final String USERNAME = "miguel-test";
    static final String PASSWORD = "test";
    static final String topicTemperaturaMinima = "/com/cuna/configTemperaturaMinima";
    static final String topicTemperaturaMaxima = "/com/cuna/configTemperaturaMaxima";
    static final String topicAlarmaSonido = "/com/cuna/switchAlarmaSonido";
    static final String topicAlarmaTemperatura = "/com/cuna/switchAlarmaTemperatura";
    static final String topicAlarmaProximidad = "/com/cuna/switchAlarmaProximidad";

}
