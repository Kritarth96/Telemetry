
#define BLYNK_TEMPLATE_ID "TMPL3ogn8AYXi"
#define BLYNK_TEMPLATE_NAME "Telemetry for drone"
#define BLYNK_AUTH_TOKEN "sN9_Cm8zsRhSEGo1x2JLAImVR_v2x27p"
#include <Adafruit_BMP280.h> 
#include<Wire.h>
#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include<MPU6050.h>
#include<I2Cdev.h>
#include <Adafruit_Sensor.h>


#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


BlynkTimer timer;
char auth[] = "sN9_Cm8zsRhSEGo1x2JLAImVR_v2x27p";       // You should get Auth Token in the Blynk App.
char ssid[] = "TU";                       // Your WiFi credentials.
char pass[] = "tu@inet1";

const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
 
int minVal=265;
int maxVal=402;
 
double x;
double y;
double z;
//Create three variables for pressure
double  P;
char status;
Adafruit_BMP280 bmp; 
 
void setup(){
Wire.begin();
Wire.beginTransmission(MPU_addr);
Wire.write(0x6B);
Wire.write(0);
Wire.endTransmission(true);
Serial.begin(9600);
Blynk.begin(auth, ssid, pass);
bmp.begin();
if (!bmp.begin(0x76)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
dht.begin();

 }
 
void pressure() {
  float P = bmp.readPressure()/3386.39;// 0 to 3
    Serial.println(P);
  float alt= bmp.readAltitude(1019.66);
    Serial.println(alt);
  
      Blynk.virtualWrite(V6, P);
      Blynk.virtualWrite(V10,alt);
       

  }
  

void loop(){
Blynk.run();
pressure();
float humidity = dht.readHumidity();
float temperature = dht.readTemperature();
Serial.println(humidity);
Serial.print(temperature);

if (!isnan(humidity) && !isnan(temperature)) {
    Blynk.virtualWrite(V1, temperature);
    Blynk.virtualWrite(V2, humidity);
}
delay(2000);
Wire.beginTransmission(MPU_addr);
Wire.write(0x3B);
Wire.endTransmission(false);
Wire.requestFrom(MPU_addr,14,true);
AcX=Wire.read()<<8|Wire.read();
AcY=Wire.read()<<8|Wire.read();
AcZ=Wire.read()<<8|Wire.read();
int xAng = map(AcX,minVal,maxVal,-90,90);
int yAng = map(AcY,minVal,maxVal,-90,90);
int zAng = map(AcZ,minVal,maxVal,-90,90);
 
x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
 
Serial.print("AngleX= ");
Serial.println(x);
 
Serial.print("AngleY= ");
Serial.println(y);
 
Serial.print("AngleZ= ");
Serial.println(z);
Serial.println("-----------------------------------------");
 
Blynk.virtualWrite(V3, x);
Blynk.virtualWrite(V4, y);
Blynk.virtualWrite(V5, z);
 
delay(1000);
}