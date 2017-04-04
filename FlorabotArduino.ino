#include <ArduinoJson.h>
#include <NewPing.h>
#include <Servo.h>
Servo door;
int estado_puerta = 90;
int pin=4;
#include <dht.h>
dht DHT;
#define DHT11_PIN 4

#define TRIGGER_PIN  3  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     2  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
#define SensorPin 0          //pH meter Analog output to Arduino Analog Input 0
unsigned long int avgValue;  //Store the average value of the sensor feedback
boolean Orden_Puerta = false; 
float b;
int buf[10],temp;
//
int sensor_door = 2;
int led_crecimiento = 7;
int led_floracion = 8;

int intractor_derecho = 5;
int intractor_izquierdo = 6;
byte number = 0;
//LDR
int LDR_1 = A1;            // select the input pin for the ldr
int LDR_2 = A2;            // select the input pin for the ldr
unsigned int LDR_1_value, LDR_2_value = 0;  // variable to store the value coming from the ldr
//hum
int err;
float temperatura, humedad;
float phValue;
/////variables para lectura serial
String Msj[6];
int mensaje = 0;

boolean stringComplete, ventilador_derecho, ventilador_izquierdo = false;  // whether the string is complete

float tiempo_anterior, tiempo_apertura_puerta, tiempo_cierre_puerta = 0;
int estado_sensor_puerta;
void setup () {
  Serial.begin(9600); // Establece la velocidad de datos del puerto serie
  pinMode(led_crecimiento, OUTPUT);
  pinMode(led_floracion, OUTPUT);
  pinMode(sensor_door, INPUT);
  pinMode(intractor_derecho, OUTPUT);
  pinMode(intractor_izquierdo, OUTPUT);
  attachInterrupt(0, Puerta, FALLING);
  // Inside the brackets, 200 is the size of the pool in bytes.
  // If the JSON object is more complex, you need to increase that value.
  door.attach(9);
  door.write(estado_puerta);
  while (! Serial);
   if(digitalRead(sensor_door) == 0){
    estado_puerta = 35;
    door.write(estado_puerta);
  }else{
    Orden_Puerta = true;
    tiempo_apertura_puerta = millis();
  }
}
void Puerta(){
  if((millis() - tiempo_apertura_puerta) >= 1000){
    estado_puerta = 35;
    door.write(estado_puerta);
  }
  tiempo_cierre_puerta = millis();
  
}
void loop(){
    
  serialEvent();
  estado_sensor_puerta = digitalRead(sensor_door);
  if(estado_sensor_puerta == 1 && (Orden_Puerta == true || (millis() - tiempo_cierre_puerta) < 3000 )){
    estado_puerta = 90;
    tiempo_apertura_puerta = millis();
    door.write(estado_puerta);
  }else if((millis() - tiempo_cierre_puerta) >= 3000 ){
    Orden_Puerta = false;
  }
  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
    //Serial.print("OK,\t"); 
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.print("Checksum error,\t"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.print("Time out error,\t"); 
    break;
    default: 
    Serial.print("Unknown error,\t"); 
    break;
  }
  estado_sensor_puerta = digitalRead(sensor_door);
  LDR_1_value = analogRead(LDR_1);    
  LDR_2_value = analogRead(LDR_2);   
  lectura_ph();
  String json = "";
  json = json + DHT.temperature;
  json = json + "-";
  json = json + DHT.humidity;
  json = json + "-";
  json = json + phValue;
  json = json + "-";
  json = json + LDR_1_value;
  json = json + "-";
  json = json + LDR_2_value;
  json = json + "-";
  json = json + ventilador_derecho;
  json = json + "-";
  json = json + ventilador_izquierdo;
  json = json + "-";
  json = json + estado_sensor_puerta;
  json = json + "*";
  if((millis() - tiempo_anterior) > 1000){
    Serial.println(json); 
    tiempo_anterior = millis();
  }
  delay(500); //delay for reread
}

void sendJson(String accion){
  if(accion == "sensores"){
    

    /*json = "<{\"temperatura\":";
    json = json + temperatura;
    json = json + ",\"humedad\":";
    json = json + humedad;
    json = json + ",\"pH\":";
    json = json + phValue;
    json = json + ",\"LDR\":{\"inferior\":";
    json = json + LDR_1_value;
    json = json + ",\"superior\":";
    json = json + LDR_2_value;
    json = json + "}}>";*/
    
  }
    
}

void lectura_ph(){
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  
  phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue; 

}

