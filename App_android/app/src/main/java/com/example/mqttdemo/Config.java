package com.example.mqttdemo;

public class Config {

    public String temperatura_minima;
    public String temperatura_maxima;
    public String alarma_temperatura;
    public String alarma_sonido;
    public String alarma_proximidad;

    public Config(String alarma_temperatura, String temperatura_minima, String temperatura_maxima, String alarma_sonido, String alarma_proximidad) {
        this.alarma_temperatura = alarma_temperatura;
        this.temperatura_minima = temperatura_minima;
        this.temperatura_maxima = temperatura_maxima;
        this.alarma_sonido = alarma_sonido;
        this.alarma_proximidad = alarma_proximidad;
    }

    public String getAlarma_temperatura() {
        return alarma_temperatura;
    }

    public void setAlarma_temperatura(String alarma_temperatura) {
        this.alarma_temperatura = alarma_temperatura;
    }

    public String getTemperatura_minima() {
        return temperatura_minima;
    }

    public void setTemperatura_minima(String temperatura_minima) {
        this.temperatura_minima = temperatura_minima;
    }

    public String getTemperatura_maxima() {
        return temperatura_maxima;
    }

    public void setTemperatura_maxima(String temperatura_maxima) {
        this.temperatura_maxima = temperatura_maxima;
    }

    public String getAlarma_sonido() {
        return alarma_sonido;
    }

    public void setAlarma_sonido(String alarma_sonido) {
        this.alarma_sonido = alarma_sonido;
    }

    public String getAlarma_proximidad() {
        return alarma_proximidad;
    }

    public void setAlarma_proximidad(String alarma_proximidad) {
        this.alarma_proximidad = alarma_proximidad;
    }


    public boolean esAlarmaTemperaturaActivada()
    {
        return alarma_temperatura.equals("1");
    }

    public boolean esAlarmaSonidoActivada()
    {
        return alarma_sonido.equals("1");
    }

    public boolean esAlarmaProximidadActivada()
    {
        return alarma_proximidad.equals("1");
    }
}
