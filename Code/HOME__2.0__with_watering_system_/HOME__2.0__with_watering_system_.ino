/*
Project name:- HOME+ 2.0
Creator: Revanth Sai Nandamuri
Date of last update:- 25/02/2021
*/






#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
char auth[] = "";     //CREATED IN THE APPLICATION
char ssid[] = "";
char pass[] = "";
#define DHTPIN 2          // Digital pin 4

//Uncomment the type of sensor which of your choice and coment the rest
//#define DHTTYPE DHT11     // DHT 11
#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301
int water=0;
DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;
void tempdht()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);  //V5 is for Humidity
  Blynk.virtualWrite(V6, t);  //V6 is for Temperature
}
void moisture()
{
 int sense_Pin = 0;//A0
 int value= analogRead(sense_Pin);
 value= value/10; 
 Serial.println(value);
 Blynk.virtualWrite(V7,value);
 if(value>50)
 {
  Serial.println("Plants need water");
  Blynk.notify("Please water the plants"); 
  //digitalWrite(water, HIGH);
 }
//else
// {
//digitalWrite(water, LOW);
//}
}

int flag=0;
void notification()
{
  int isButtonPressed = digitalRead(D5);
  if (isButtonPressed==1 && flag==0) 
  {
    Serial.println("Fire in the House");
    Blynk.notify("Alert : Fire in the House");
    
    flag=1;
  }
  else if (isButtonPressed==0)
  {
    flag=0;
  }
  
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  dht.begin();
  timer.setInterval(1000L, tempdht);
  
  pinMode(D5,INPUT_PULLUP);
  timer.setInterval(1000L,notification);

  pinMode(A0,INPUT_PULLUP);
  timer.setInterval(1000L,moisture);
  
  pinMode(water, OUTPUT);
}

void loop()
{
  Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer
}
