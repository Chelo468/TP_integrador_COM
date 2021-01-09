#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>


#define LED_PIN1                15
//#define HCSR04_TRIGGER        18
//#define HCSR04_ECHO           19

//Sensor Proximidad
#define PIN_PROXIMIDAD          19

//Sensor de humedad
#define DHTPIN                  21
#define DHTTYPE                 DHT11

//Sensor de sonido HW-484 JY-037
#define PINSONIDO_DIGITAL       25
#define PINSONIDO_ANALOGICO     26

//Sensor tacometro
#define PIN_TACOMETRO_GIRO      27

//BUZZER
#define PIN_BUZZER              3

#define LEDC_CHANNEL_0          0
#define LEDC_TIMER_8_BIT        8
#define LEDC_BASE_FREQ          5000

int brillo = 0;
int accion = 0;//0 subir - 1 bajar

int duracion = 0;
int distancia = 0;
int contCiclo = 0;
int contErrores = 0;
int contReiniciadas = 1;

int apagarLed = 1;

//Temperatura
int mostrar = 0;

//DHT
DHT dht(DHTPIN, DHTTYPE);


//SONIDO
int valor_sonido;

//TACOMETRO
int valor_tacometro;

//PROXIMIDAD
int valor_proximidad;


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
}

void loop() {
  // put your main code here, to run repeatedly:

  /*if(accion == 0)
  {
    if(brillo < 255)
    {
      brillo = brillo + 1;  
    }
    else
    {
      accion = 1;
      brillo = brillo - 1;      
    }
  }
  else
  {
    if(brillo > 0)
    {
      brillo = brillo - 1;
    }
    else 
    {
      accion = 0;
      brillo = brillo +1;
    }
  }*/
  
 if (!mqttClient.connected())
 {
    reconnect();
 }
  
  
  mqttClient.loop();
  
  ledcWrite(LEDC_CHANNEL_0, 0);

  digitalWrite(PIN_BUZZER, LOW);
  
  //Serial.println("Accion: " + String(accion) + "; Brillo; " + String(brillo));

  //Enviamos un pulso de 1 milisegundo en nivel alto
  //digitalWrite(HCSR04_TRIGGER, HIGH);
  
  //delayMicroseconds(10);
  
  //Volvemos el pulso de nivel bajo
  //digitalWrite(HCSR04_TRIGGER, LOW);

  

  //pulseIn devuelve un valor de tiempo en microsegundos
  //duracion = pulseIn(HCSR04_ECHO, HIGH);

  //contCiclo = 0;
  /*while(duracion == 0 && contCiclo < 5)
  {
    duracion = pulseIn(HCSR04_ECHO, HIGH);
    contCiclo++;

    delay(10);
  }*/

/*
  if(duracion > 0)
  {
      Serial.println("Duracion: " + String(duracion));
  
      //Convertimos el tiempo en distancia
      distancia = duracion / 58.2;
    
      Serial.println("Distancia: " + String(distancia));
  }
  else
  {
    contErrores++;

    if(contErrores == 5)
    {
      contErrores = 0;
      Serial.println("Reiniciado de contador " + String(contReiniciadas));

      contReiniciadas++;
    }
  }*/

  //DHT
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
    
  }
else
{
    // Calcular el índice de calor en Fahrenheit
    float hif = dht.computeHeatIndex(f, h);
    // Calcular el índice de calor en grados centígrados
    float hic = dht.computeHeatIndex(t, h, false);

    //Cada 10 tenemos 1 segundo por el delay de 100
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
   }

  if(t > 28)
  {
    alarma("Temperatura " + String(t) + " ºC");
  }
}


 

  //SONIDO

  valor_sonido = digitalRead(PINSONIDO_DIGITAL);

  if(valor_sonido == HIGH)
  {
    Serial.println("Microfono detectado.");
    alarma("Sonido");
  }

  //TACOMETRO
  valor_tacometro = digitalRead(PIN_TACOMETRO_GIRO);

  if(valor_tacometro == LOW)
  {
    Serial.println("Movimiento detectado.");
    alarma("Movimiento");
  }

  //PROXIMIDAD

  valor_proximidad = digitalRead(PIN_PROXIMIDAD);

  if(valor_proximidad == LOW)
  {
    Serial.println("Objeto aproximado.");
    alarma("Proximidad");
  }

 

  delay(100);
}

configurarPines(){
  
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_8_BIT);
  ledcAttachPin(LED_PIN1, LEDC_CHANNEL_0);

  pinMode(LED_PIN1, OUTPUT);
  //pinMode(HCSR04_TRIGGER, OUTPUT);
  //pinMode(HCSR04_ECHO, INPUT);

  pinMode(PIN_PROXIMIDAD, INPUT);

  pinMode(PINSONIDO_DIGITAL, INPUT);
  
  pinMode(PIN_TACOMETRO_GIRO, INPUT);

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

  if (mqttClient.connected())
    mqttClient.publish("/swa/alarma", char_array);
}

void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
      Serial.println("Reconnecting to MQTT Broker..");
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);
      
      if (mqttClient.connect(clientId.c_str())) {
        Serial.println("Connected.");
        // subscribe to topic
        mqttClient.subscribe("/swa/commands");
      }
      
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Callback - Topic: ");

  for (int i = 0; i < strlen(topic); i++) {
    Serial.print((char)topic[i]);
  }

String mensaje;
  
  Serial.print(" ; Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    mensaje = mensaje + (char)payload[i];
    
  }

  Serial.println(mensaje);

  if(mensaje == "alarma")
  {
      ledcWrite(LEDC_CHANNEL_0, 200);
      digitalWrite(PIN_BUZZER, HIGH);

      delay(100);
  }

  
}