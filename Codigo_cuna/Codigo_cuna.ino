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

int alarmaActivada = 0;
int contAlarma = 0;
float temperatura_alarma = 28;


//SONIDO
int valor_sonido;

//Sensor Golpe
int valor_sensor_golpe;

//PROXIMIDAD
int valor_proximidad;


//Activacion de alarmas
int alarma_sonido_activada = 1;
int alarma_temperatura_activada;
int alarma_proximidad_activada = 1;


//WIFI
const char* SSID = "wifi casa";
const char* PWD = "MglMcmqybT";

// MQTT client
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient); 

char* mqttServer = "broker.hivemq.com";
int mqttPort = 1883;


void setup() {
  //Activamos la salida por consola
  Serial.begin(115200);

  configurarPines();

  //Inicializamos modulo dht
  dht.begin();

  //WIFI
  configurarYConectarWifi();

  //MQTT
  configurarMqtt();

  alarma_temperatura_activada = 1;
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

  delay(100);
}

void leerHumedad(){

  contCiclo++;
  
  // Leemos la humedad relativa
  float h = dht.readHumidity();
  // Leemos la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();
  // Leemos la temperatura en grados Fahrenheit
  float f = dht.readTemperature(true);

  // Comprobamos si ha habido algún error en la lectura
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    mostrarEnConsola("Error obteniendo los datos del sensor DHT11");
  }
  else  {
      // Calcular el índice de calor en Fahrenheit
      float hif = dht.computeHeatIndex(f, h);
      // Calcular el índice de calor en grados centígrados
      float hic = dht.computeHeatIndex(t, h, false);
  
      //Cada 10 tenemos 1 segundo por el delay de 100 milisegundos
     if(contCiclo == 300)
     {
        contCiclo = 0;
        Serial.print("Humedad: ");
        Serial.print(h);
        Serial.print(" %\t");
        Serial.print("Temperatura: ");
        Serial.print(t);
        Serial.print(" *C ");
        Serial.print(f);
        Serial.print(" *F\t");
        Serial.print("Índice de calor: ");
        Serial.print(hic);
        Serial.print(" *C ");
        Serial.print(hif);
        Serial.println(" *F");

        mostrarEnConsola("Humedad: " + String(h) + " Temperatura: " + String(t) + " ºC");
     }
    
    if(t > temperatura_alarma)
    {
      if(alarma_temperatura_activada == 1)
      {
        
        alarma("Temperatura " + String(t) + " ºC");  
      }        
    }
  }
}

void leerSensorSonido(){
  
  valor_sonido = digitalRead(PINSONIDO_DIGITAL);

  if(valor_sonido == HIGH)
  {
    Serial.println("Microfono detectado.");

    if(alarma_sonido_activada == 1)
    {
      alarma("Sonido");  
    }
    

    mostrarEnConsola("Microfono detectado.");
  }
}

void leerSensorMovimiento(){
  valor_sensor_golpe = digitalRead(PIN_SENSOR_GOLPE);

  if(valor_sensor_golpe == LOW)
  {
    Serial.println("Movimiento detectado.");
    alarma("Movimiento");

    mostrarEnConsola("Movimiento detectado.");
  }
}

void leerSensorProximidad(){
  
  valor_proximidad = digitalRead(PIN_PROXIMIDAD);

  if(valor_proximidad == LOW)
  {
    Serial.println("Objeto aproximado.");

    if(alarma_proximidad_activada == 1)
    {
      alarma("Proximidad");  
    }
    
    mostrarEnConsola("Objeto aproximado.");
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
    delay(500);
  }
  
  Serial.print("Connected.");
}

void configurarMqtt(){
  mqttClient.setServer(mqttServer, mqttPort);
  // set the callback function
  mqttClient.setCallback(callback);
}

void alarma(String tipo){

  alarmaActivada = 1;

  /*if(contAlarma == 0 || contAlarma == 100)
  {
    if(contAlarma == 100)
    {
      contAlarma = 0;
    }*/
    
    Serial.println("Alarma detectada: " + tipo);
  
    ledcWrite(LEDC_CHANNEL_0, 200);
    digitalWrite(PIN_BUZZER, HIGH);
  
    int n = tipo.length();
   
      // declaring character array
      char char_array[n + 1];
   
      // copying the contents of the
      // string to char array
      strcpy(char_array, tipo.c_str());
   
      for (int i = 0; i < n; i++)
          char_array[i] = tipo[i];
  
    if (mqttClient.connected()){
      mqttClient.publish("/swa/alarma", char_array);
    }
      

      
    //}
  
}

void reconnect() {
  
  Serial.println("Conectando a Broker MQTT...");

  mostrarEnConsola("Conectando a Broker MQTT...");
  
  while (!mqttClient.connected()) {
      Serial.println("Reonectando a Broker MQTT...");

      mostrarEnConsola("Reonectando a Broker MQTT...");
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);
      
      if (mqttClient.connect(clientId.c_str())) {
        Serial.println("Connected.");
        // subscribe to topic
        mqttClient.subscribe("/swa/commands");
        mqttClient.subscribe("/com/cuna/configTemperaturaMinima");
        mqttClient.subscribe("/com/cuna/switchAlarmaSonido");
        mqttClient.subscribe("/com/cuna/switchAlarmaTemperatura");
        mqttClient.subscribe("/com/cuna/switchAlarmaProximidad");
      }
      
  }
}

void callback(char* topic, byte* payload, unsigned int length) {

  String topico;
  String mensaje;
  
  Serial.print("Callback - Topic: ");

  for (int i = 0; i < strlen(topic); i++) {
    Serial.print((char)topic[i]);
    topico = topico + (char)payload[i];
  }
  
  Serial.print(" ; Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    mensaje = mensaje + (char)payload[i];
    
  }

  Serial.println("Mesaje:" + mensaje);

  bool es1 = mensaje == "1";
  
  
  if (strcmp(topic,"/com/cuna/configTemperaturaMinima")==0){
    // whatever you want for this topic
    temperatura_alarma = mensaje.toFloat(); 
  }

  if(strcmp(topic,"/com/cuna/switchAlarmaTemperatura")==0){
    if(es1){
      Serial.println("entro al true");
      alarma_temperatura_activada = 1;
    }
    else{
      Serial.println("entro al false");
      alarma_temperatura_activada = 0;
    }
  }

  
  if(strcmp(topic,"/com/cuna/switchAlarmaSonido")==0){
    if(es1 == 1){
      alarma_sonido_activada = 1;
    }
    else{
      alarma_sonido_activada = 0;
    }
  }

  if(strcmp(topic,"/com/cuna/switchAlarmaProximidad")==0){
    if(es1 == 1){
      alarma_proximidad_activada = 1;
    }
    else{
      alarma_proximidad_activada = 0;
    }
  }

  if(mensaje == "alarma")
  {
      ledcWrite(LEDC_CHANNEL_0, 200);
      digitalWrite(PIN_BUZZER, HIGH);

      delay(100);
  }

  
}

void mostrarEnConsola(String mensaje){
    int n = mensaje.length();
 
    // declaring character array
    char char_array[n + 1];
 
    // copying the contents of the
    // string to char array
    strcpy(char_array, mensaje.c_str());

    // copying the contents of the
    // string to char array
    strcpy(char_array, mensaje.c_str());
 
    for (int i = 0; i < n; i++)
        char_array[i] = mensaje[i];

    if (mqttClient.connected())
      mqttClient.publish("/com/consola", char_array);
}
