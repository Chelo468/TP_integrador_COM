#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>


#define LED_PIN1                15

//Sensor Proximidad
#define PIN_PROXIMIDAD          19

//Sensor de humedad
#define DHTPIN                  21
#define DHTTYPE                 DHT11

//Sensor de sonido HW-484 JY-037
#define PINSONIDO_DIGITAL       25
#define PINSONIDO_ANALOGICO     26

//Sensor tacometro
#define PIN_SENSOR_GOLPE        27

//BUZZER
#define PIN_BUZZER              3

#define LEDC_CHANNEL_0          0
#define LEDC_TIMER_8_BIT        8
#define LEDC_BASE_FREQ          5000

//DHT - Temperatura
DHT dht(DHTPIN, DHTTYPE);
int contCiclo = 0;
int CICLO_MOSTRAR_TEMPERATURA_CONSOLA = 300;
int cicloRefrescoTemperatura = 0;
float temperatura_alarma_minima = 15;
float temperatura_alarma_maxima = 30;

//Configuracion
int DELAY_LOOP = 100;


//SONIDO
int valor_sonido;
int aviso_sonido = 0;

//Sensor Golpe
int valor_sensor_golpe;

//PROXIMIDAD
int valor_proximidad;
int aviso_proximidad = 0;


//Activacion de alarmas
int alarma_sonido_activada = 1;
int alarma_temperatura_activada = 1;
int alarma_proximidad_activada = 1;
int aviso_temperatura = 0;
int TIEMPO_REFRESCO = 50;//5 segundos por los 100 de delay
int DELAY_MENSAJES = 10;//Milisegundos
int alarmaActivada = 0;
int contAlarma = 0;


//WIFI
const char* SSID = "wifi casa";
const char* PWD = "MglMcmqybT";
int DELAY_TRY_RECONNECT = 500;//Milisegundos

// MQTT client
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient); 
char* mqttServer = "broker.hivemq.com";
int mqttPort = 1883;

//Topicos
const char* TOPIC_CONSOLA = "/com/consola";
String TOPIC_CONSOLA_STRING;
const char* TOPIC_REFRESCO_TEMPERATURA = "/swa/temperatura";
String TOPIC_REFRESCO_TEMPERATURA_STRING;
const char* TOPIC_ALARMA = "/swa/alarma";
String TOPIC_ALARMA_STRING;
const char* TOPIC_CONFIG_TEMPERATURA_MINIMA = "/com/cuna/configTemperaturaMinima";
String TOPIC_CONFIG_TEMPERATURA_MINIMA_STRING;
const char* TOPIC_CONFIG_TEMPERATURA_MAXIMA = "/com/cuna/configTemperaturaMaxima";
String TOPIC_CONFIG_TEMPERATURA_MAXIMA_STRING;
const char* TOPIC_SWITCH_ALARMA_SONIDO = "/com/cuna/switchAlarmaSonido";
String TOPIC_SWITCH_ALARMA_SONIDO_STRING;
const char* TOPIC_SWITCH_ALARMA_TEMPERATURA = "/com/cuna/switchAlarmaTemperatura";
String TOPIC_SWITCH_ALARMA_TEMPERATURA_STRING;
const char* TOPIC_SWITCH_ALARMA_PROXIMIDAD = "/com/cuna/switchAlarmaProximidad";
String TOPIC_SWITCH_ALARMA_PROXIMIDAD_STRING;
const char* TOPIC_GET_CONFIGURACION = "/com/cuna/getConfiguration";
String TOPIC_GET_CONFIGURACION_STRING;
const char* TOPIC_ENVIAR_CONFIGURACION = "/com/cuna/myConfiguration";
String TOPIC_ENVIAR_CONFIGURACION_STRING;


void setup() {
  //Activamos la salida por consola
  Serial.begin(115200);

  //Seteo de pines del ESP32
  configurarPines();

  //Inicializamos modulo dht para lectura de humedad y temperatura del ambiente
  dht.begin();

  //WIFI
  configurarYConectarWifi();

  //MQTT
  configurarMqtt();

  //Obtenemos las constantes de los topicos en String
  getTopicStrings();
}

void loop() {
    
 if (!mqttClient.connected())
 {
    reconnect();
 }  
  
  mqttClient.loop();
  
  ledcWrite(LEDC_CHANNEL_0, 0);

  digitalWrite(PIN_BUZZER, LOW);
  
  alarmaActivada = 0;
  
  //DHT
  leerHumedad();

  //SONIDO
  leerSensorSonido();

  //TACOMETRO
  leerSensorMovimiento();

  //PROXIMIDAD
  leerSensorProximidad(); 

  if(alarmaActivada == 1)
  {
    contAlarma++;
  }
  else
  {
    contAlarma = 0;
  }

  delay(DELAY_LOOP);
}

void refrescarTemperatura(String mensaje){

  int n = mensaje.length();

  //Conversion de String a char[]
  char char_array[n + 1];
  
  strcpy(char_array, mensaje.c_str());

  for (int i = 0; i < n; i++)
      char_array[i] = mensaje[i];

  //Publicacion de mensaje
  if (mqttClient.connected()){
      mqttClient.publish(TOPIC_REFRESCO_TEMPERATURA, char_array);
      delay(DELAY_MENSAJES);
    }
}

void leerHumedad(){

  contCiclo++;

  cicloRefrescoTemperatura++;
  
  // Leemos la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();

  // Comprobamos si ha habido algún error en la lectura
  if (isnan(t)) {
    if(cicloRefrescoTemperatura == 0 || cicloRefrescoTemperatura == TIEMPO_REFRESCO)
     {
        if(cicloRefrescoTemperatura > 0)
        {
          cicloRefrescoTemperatura = 0;
        }
        mostrarEnConsola("Error obteniendo los datos del sensor DHT11");
     }
  }
  else  {
  
      //Cada 10 tenemos 1 segundo por el delay de 100 milisegundos
     if(contCiclo == CICLO_MOSTRAR_TEMPERATURA_CONSOLA)
     {
        contCiclo = 0;

        mostrarEnConsola("Temperatura: " + String(t) + " ºC");
     }

     if(cicloRefrescoTemperatura == 0 || cicloRefrescoTemperatura == TIEMPO_REFRESCO)
     {
      if(cicloRefrescoTemperatura > 0)
      {
        cicloRefrescoTemperatura = 0;
      }
      
      refrescarTemperatura("Temperatura: " + String(t) + " ºC");
     }
     else
     {
        //mostrarEnConsola(String(cicloRefrescoTemperatura));
     }
     
    if(alarma_temperatura_activada == 1)
     {
        if(t > temperatura_alarma_maxima || t < temperatura_alarma_minima)
        {
          

          if(aviso_temperatura == 0 || aviso_temperatura == TIEMPO_REFRESCO)
          {
            if(aviso_temperatura > 0)
            {
              aviso_temperatura = 0;  
            }
            
            alarma("Temperatura " + String(t) + " ºC");    
          }

          aviso_temperatura++;
          
        }        
        else
        {
          aviso_temperatura = 0;
        }
     }
  }
}

void leerSensorSonido(){
  
  valor_sonido = digitalRead(PINSONIDO_DIGITAL);

  if(valor_sonido == HIGH)
  {
    if(alarma_sonido_activada == 1)
    {
      if(aviso_sonido == 0 || aviso_sonido == TIEMPO_REFRESCO)
      {
        if(aviso_sonido > 0)
        {
          aviso_sonido = 0;  
        }
        
        alarma("Sonido");    
      }

      aviso_sonido++;
      
    }
    

    mostrarEnConsola("Microfono detectado.");
  }
  else
  {
    aviso_sonido = 0;  
  }
}

void leerSensorMovimiento(){
  valor_sensor_golpe = digitalRead(PIN_SENSOR_GOLPE);

  if(valor_sensor_golpe == LOW)
  {
    alarma("Movimiento");

    mostrarEnConsola("Movimiento detectado.");
  }
}

void leerSensorProximidad(){
  
  valor_proximidad = digitalRead(PIN_PROXIMIDAD);

  if(valor_proximidad == LOW)
  {
    if(alarma_proximidad_activada == 1)
    {
      if(aviso_proximidad == 0 || aviso_proximidad == TIEMPO_REFRESCO)
      {
        if(aviso_proximidad > 0)
        {
          aviso_proximidad = 0;  
        }

        alarma("Proximidad"); 
      }

      aviso_proximidad++;
       
    }
    
    mostrarEnConsola("Objeto aproximado.");
  }
  else
  {
    aviso_proximidad = 0;
  }
  
}

void configurarPines(){
  
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_8_BIT);
  ledcAttachPin(LED_PIN1, LEDC_CHANNEL_0);

  pinMode(LED_PIN1, OUTPUT);

  pinMode(PIN_PROXIMIDAD, INPUT);

  pinMode(PINSONIDO_DIGITAL, INPUT);
  
  pinMode(PIN_SENSOR_GOLPE, INPUT);

  pinMode(PIN_BUZZER, OUTPUT);
}

void configurarYConectarWifi(){
  WiFi.begin(SSID, PWD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(DELAY_TRY_RECONNECT);
  } 

  String nombre_red;

  for (int i = 0; i < strlen(SSID); i++) {
    
    nombre_red = nombre_red + (char)SSID[i];
  }
  
  mostrarEnConsola("Conectado a " + nombre_red);
}

void configurarMqtt(){
  mqttClient.setServer(mqttServer, mqttPort);
  
  //Configuramos el método donde el dispositivo realizará la lectura de los mensajes a los que está suscripto
  mqttClient.setCallback(callback);
}

void alarma(String tipo){

    alarmaActivada = 1;
    
    ledcWrite(LEDC_CHANNEL_0, 200);
    digitalWrite(PIN_BUZZER, HIGH);

  //Conversion de String a char[]
    int n = tipo.length();
   
    char char_array[n + 1];
 
    strcpy(char_array, tipo.c_str());
 
    for (int i = 0; i < n; i++)
    {
       char_array[i] = tipo[i];
    }
      
  
    if (mqttClient.connected()){
      mqttClient.publish(TOPIC_ALARMA, char_array);

      delay(DELAY_MENSAJES);
    }

    mostrarEnConsola("Alarma informada: " + tipo);
 }

void reconnect() {
  
  mostrarEnConsola("Conectando a Broker MQTT...");
  
  while (!mqttClient.connected()) {
    
      mostrarEnConsola("Reonectando a Broker MQTT...");

      //Generacion de clientID
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);
      
      if (mqttClient.connect(clientId.c_str())) {
        
        mostrarEnConsola("Conectado a Broker MQTT.");
        
        //suscripcion a topicos
        mqttClient.subscribe(TOPIC_CONFIG_TEMPERATURA_MINIMA);
        mqttClient.subscribe(TOPIC_CONFIG_TEMPERATURA_MAXIMA);
        mqttClient.subscribe(TOPIC_SWITCH_ALARMA_SONIDO);
        mqttClient.subscribe(TOPIC_SWITCH_ALARMA_TEMPERATURA);
        mqttClient.subscribe(TOPIC_SWITCH_ALARMA_PROXIMIDAD);
        mqttClient.subscribe(TOPIC_GET_CONFIGURACION);
        mqttClient.subscribe(TOPIC_ENVIAR_CONFIGURACION);
      }
      
  }
}

void callback(char* topic, byte* payload, unsigned int length) {

  String topico;
  String mensaje;
  
  for (int i = 0; i < strlen(topic); i++) {
    
    topico = topico + (char)topic[i];
  }
  
  for (int i = 0; i < length; i++) {
    
    mensaje = mensaje + (char)payload[i];
    
  }

  mostrarEnConsola("Topico Recibido: " + topico + "; Mesaje Recibido:" + mensaje);
  
  bool igual = topico.equalsIgnoreCase(TOPIC_CONFIG_TEMPERATURA_MINIMA_STRING);
  
  if(igual)
  {
    //Configuramos la temperatura minima obtenida del mensaje
    temperatura_alarma_minima = mensaje.toFloat(); 
  }

  igual = topico.equalsIgnoreCase(TOPIC_CONFIG_TEMPERATURA_MAXIMA_STRING);
  
  if(igual)
  {
    //Configuramos la temperatura maxima obtenida del mensaje
    temperatura_alarma_maxima = mensaje.toFloat();
  }

  igual = topico.equalsIgnoreCase(TOPIC_SWITCH_ALARMA_TEMPERATURA_STRING);
  
  if(igual)  
  {
    if(mensaje == "1"){
      alarma_temperatura_activada = 1;
    }
    else{
      alarma_temperatura_activada = 0;
    }
  }

  igual = topico.equalsIgnoreCase(TOPIC_SWITCH_ALARMA_SONIDO_STRING);
  
  if(igual)  
  {
    if(mensaje == "1"){
      alarma_sonido_activada = 1;
    }
    else{
      alarma_sonido_activada = 0;
    }
  }

  igual = topico.equalsIgnoreCase(TOPIC_SWITCH_ALARMA_PROXIMIDAD_STRING);

  if(igual)  
  {
    if(mensaje == "1"){
      alarma_proximidad_activada = 1;
    }
    else{
      alarma_proximidad_activada = 0;
    }
  }  

  igual = topico.equalsIgnoreCase(TOPIC_GET_CONFIGURACION);

  if(igual)  
  {
    enviarConfiguracion();
  }  
}

void enviarConfiguracion(){

  String configuracion = "{";
  configuracion = configuracion + "\"alarma_temperatura\" : \"" + String(alarma_temperatura_activada) + "\",";
  configuracion = configuracion + "\"temperatura_minima\" : \"" + String(temperatura_alarma_minima) + "\",";
  configuracion = configuracion + "\"temperatura_maxima\" : \"" + String(temperatura_alarma_maxima) + "\",";
  configuracion = configuracion + "\"alarma_sonido\" : \"" + String(alarma_sonido_activada) + "\",";
  configuracion = configuracion + "\"alarma_proximidad\" : \"" + String(alarma_proximidad_activada) + "\"";
  configuracion = configuracion + "}";
  
  //
  int n = configuracion.length();

  //Conversion de String a char[]
  char char_array[n + 1];
  
  strcpy(char_array, configuracion.c_str());

  for (int i = 0; i < n; i++)
      char_array[i] = configuracion[i];

  //Publicacion de mensaje
  if (mqttClient.connected()){
      mqttClient.publish(TOPIC_ENVIAR_CONFIGURACION, char_array);
      delay(DELAY_MENSAJES);
    }
}

void mostrarEnConsola(String mensaje){

    Serial.println(mensaje);

    //Conversion de String a char[]
    int n = mensaje.length();
 
    char char_array[n + 1];
 
    strcpy(char_array, mensaje.c_str());
 
    for (int i = 0; i < n; i++)
        char_array[i] = mensaje[i];


    //Publicacion de mensaje en consola de broker
    if (mqttClient.connected())
    {
      mqttClient.publish(TOPIC_CONSOLA, char_array);
      delay(DELAY_MENSAJES);
    }
      
}

void getTopicStrings(){
 TOPIC_CONSOLA_STRING = String(TOPIC_CONSOLA);
 TOPIC_REFRESCO_TEMPERATURA_STRING = String(TOPIC_REFRESCO_TEMPERATURA);
 TOPIC_ALARMA_STRING = String(TOPIC_ALARMA);
 TOPIC_CONFIG_TEMPERATURA_MINIMA_STRING = String(TOPIC_CONFIG_TEMPERATURA_MINIMA);
 TOPIC_CONFIG_TEMPERATURA_MAXIMA_STRING = String(TOPIC_CONFIG_TEMPERATURA_MAXIMA);
 TOPIC_SWITCH_ALARMA_SONIDO_STRING = String(TOPIC_SWITCH_ALARMA_SONIDO);
 TOPIC_SWITCH_ALARMA_TEMPERATURA_STRING = String(TOPIC_SWITCH_ALARMA_TEMPERATURA);
 TOPIC_SWITCH_ALARMA_PROXIMIDAD_STRING = String(TOPIC_SWITCH_ALARMA_PROXIMIDAD);
 TOPIC_GET_CONFIGURACION_STRING = String(TOPIC_GET_CONFIGURACION);
 TOPIC_ENVIAR_CONFIGURACION_STRING = String(TOPIC_ENVIAR_CONFIGURACION);
 
}
