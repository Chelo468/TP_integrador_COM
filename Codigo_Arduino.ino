#include <NewPing.h>

#define PIN_SENSOR_MOVIMIENTO        15
#define PIN_SENSOR_PROXIMIDAD        17
#define PIN_SENSOR_SONIDO_ENTRADA    16
#define PIN_SENSOR_SONIDO_SALIDA     14
#define PIN_SENSOR_TEMPERATURA       13
#define MAX_DISTANCE                 200
#define MIN_TEMPERATURA              36
#define MAX_TEMPERATURA              38

float tempC;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
    
  pinMode(PIN_SENSOR_SONIDO_ENTRADA, INPUT);
  pinMode(PIN_SENSOR_SONIDO_SALIDA, OUTPUT);
  pinMode(PIN_SENSOR_PROXIMIDAD, INPUT);
  pinMode(PIN_SENSOR_MOVIMIENTO, INPUT);
  pinMode(PIN_SENSOR_TEMPERATURA, INPUT);
}

void loop() {

  //Se obtiene el valor de la distancia de objetos
  unsigned int uS = sonar.ping(); // Envía el ping y recibe el tiempo de eco in microseconds (uS)
 
  uS = uS / US_ROUNDTRIP_CM; //Convierte el tiempo del ping a distancia

  //Si existe un objeto más cerca de lo permitido envía una alerta
  if(uS < MAX_DISTANCE)
  {
    enviarAlerta(uS, 2);
  }
  
  //Se obtiene el valor de la temperatura
  tempC = analogRead(PIN_SENSOR_TEMPERATURA);
  tempC = (5.0 * tempC * 100.0) / 1024.0; 

  //Si está por debajo del valor minimo o por encima del valor máximo envía una alerta
  if(Temperatura < MIN_TEMPERATURA || Temperatura > MAX_TEMPERATURA)
  {
    enviarAlerta(Temperatura, 4);
  }  
  
  delay(1000);
}

//int tipoAlerta = 0;//1 - Movimiento; 2 - Proximidad; 3 - Sonido; 4 - Temperatura
enviarAlerta(float valor,int tipoAlerta){
  //En ésta función realizaremos el envío de una alerta de acuerdo al tipo
}
