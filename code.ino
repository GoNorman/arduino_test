#include <SoftwareSerial.h>
#include <MHZ.h>
#include <DHT.h>
#include <Wire.h>
#include <RTC.h>
static DS3231 RTC;

#define STATUS "status"

#define pump 4 //насос
//свет
#define led_1 5
#define led_2 6
#define led_3 7

#define CO2 13 // датчик CO2
#define DHTPIN 12// датчика влажности и температуры(к какому сигналу подключено)
#define DHTTYPE DHT22//тип датчика влажности и температуры
//датчики воды (номера сигналов)
#define check_water_1 10
#define check_water_2 11

//включение и выключение датчиков
#define led_1_ON "ON1" // включение первой лампы по фразе ON1
#define led_2_ON "ON2" // включение второй лампы по фразе ON2
#define led_3_ON "ON3" // включение третьей лампы по фразе ON3

#define led_1_OFF "OFF1" // выключение первой лампы по фразе ON1
#define led_2_OFF "OFF2" // выключение второй лампы по фразе ON2
#define led_3_OFF "OFF3" // выключение третьей лампы по фразе ON3

#define pump_on "pump_on" // включение насоса по фразе pump_on
#define pump_off "pump_off" // выключение насоса по фразе pump_off

#define delimeter "================================================="

DHT dht(DHTPIN, DHTTYPE); // это копируем из примера DHT Tester
MHZ co2(CO2, MHZ19B); // это копируем из примера MHZ- CO2 Sensors

//  объявляем функцию и весь код функции копируется из примера RTC DS3231
void check_time() {
  Serial.print("Is Clock Running: ");
  if (RTC.isRunning())
  {
    Serial.println("Yes");
    Serial.print(RTC.getDay());
    Serial.print("-");
    Serial.print(RTC.getMonth());
    Serial.print("-");
    Serial.print(RTC.getYear());
    Serial.print(" ");
    Serial.print(RTC.getHours());
    Serial.print(":");
    Serial.print(RTC.getMinutes());
    Serial.print(":");
    Serial.print(RTC.getSeconds());
    Serial.print("");
    if (RTC.getHourMode() == CLOCK_H12)
    {
      switch (RTC.getMeridiem()) {
        case HOUR_AM:
          Serial.print(" AM");
          break;
        case HOUR_PM:
          Serial.print(" PM");
          break;
      }
    }
    Serial.println("");
    delay(1000);
  }
  else
  {
    delay(1500);

    Serial.println("No");
    Serial.println("Setting Time");
    //RTC.setHourMode(CLOCK_H12);
    RTC.setHourMode(CLOCK_H24);
    RTC.setDateTime(__DATE__, __TIME__);
    Serial.println("New Time Set");
    Serial.print(__DATE__);
    Serial.print(" ");
    Serial.println(__TIME__);
    RTC.startClock();
  }
}

//Функция проверки температуры и влажности,тело которой берётся из примера DHT tester
void GET_HT(){
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
}

void Water_level_1() {
  int i = digitalRead(check_water_1);
  if (i == 0) {
    Serial.println("Tank is empty");
  }
  else {
    Serial.println("Tank is full");
  }
}

void Water_level_2() {
  int j = digitalRead(check_water_2);
  if (j == 0) {
    Serial.println("Tank is empty");
  }
  else {
    Serial.println("Tank is full");
  }
}
// берётся из примера MHZ- CO2 Sensors
void check_CO2() {
  Serial.println("Данные датчика CO2");
  int ppm_pwm = co2.readCO2PWM();
  Serial.print(", PPMpwm: ");
  Serial.print(ppm_pwm);

  int temperature = co2.getLastTemperature();
  Serial.print(", Temperature: ");

  if (temperature > 0) {
    Serial.println(temperature);
  } else {
    Serial.println("n/a");
  }
}

void check_user_command() {
  String strBuffer = Serial.readString();
  if (Serial.available()) {
    if (strBuffer == STATUS) {
      Serial.println(delimeter);
      check_time();
      GET_HT();
      Water_level_1();
      Water_level_2();
      check_CO2();
    }
  }
  else if(strBuffer == pump_on){
    digitalWrite(pump,HIGH);
  }

   else if(strBuffer == pump_off){
    digitalWrite(pump,LOW);
  }

  
  else if(strBuffer == led_1_ON){
    digitalWrite(led_1,HIGH);
  }

   else if(strBuffer == led_1_OFF){
    digitalWrite(led_1,LOW);
  }

   else if(strBuffer == led_2_ON){
    digitalWrite(led_2,HIGH);
  }

   else if(strBuffer == led_2_OFF){
    digitalWrite(led_2,LOW);
  }

   else if(strBuffer == led_3_ON){
    digitalWrite(led_3,HIGH);
  }

   else if(strBuffer == led_3_OFF){
    digitalWrite(led_3,LOW);
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  RTC.begin();
  dht.begin();

  pinMode(delimeter, INPUT);
  pinMode(12, INPUT); //датчик влажности и температуры(к какому сигналу подключено) смотреть в начале, где объявляются переменные
  pinMode(10, INPUT); //датчик воды 1 (номер сигнала)
  pinMode(11, INPUT); //датчик воды 2 (номер сигнала)
  pinMode(CO2, INPUT);
  pinMode(check_time, INPUT);
  pinMode(pump, INPUT);
  pinMode(pump, OUTPUT);

  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  check_user_command();
}
