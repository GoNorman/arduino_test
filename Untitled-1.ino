#include "DHT.h" pidor
#include <MHZ.h>
#include <Wire.h>
#include <RTC.h>
#define CO2_IN 13
#define DHTPIN 12
#define DHTTYPE DHT22
#define WAT1 10
#define WAT2 11
#define MHZPIN 8
#define STATUS "status"
#define deliter "++++++++++++++++++++++++"
#define LIGHT1 "1on"
#define LIGHT1OFF "1off"
#define LIGHT2 "2on"
#define LIGHT2OFF "2off"
#define LIGHT3 "3on"
#define LIGHT3OFF "3off"
#define LIGHTON "allon"
#define LIGHTOFF "alloff"
#define POMPON "pompon"
#define POMPOFF "pompoff"
#define SVET1 5
#define SVET2 6
#define SVET3 7
#define NASOS 4

MHZ co2(CO2_IN, MHZ19B);
DHT dht(DHTPIN, DHTTYPE);
static DS3231 RTC;

void check_ho2() {
float h = dht.readHumidity();
float t = dht.readTemperature();
if (isnan(h) || isnan(t)) {
Serial.println(F("Failed to read from DHT sensor!"));
return;
}
Serial.print(F("Humidity: "));
Serial.print(h);
Serial.print(F("% Temperature: "));
Serial.print(t);
Serial.println(F("°C "));
}

void print_date() {
LCD.setCursor(0,0);
lcd.print("Time: ");
lcd.print(rtc.getTimeStr());

lcd.setCursor(0,1);
lcd.print("Date: ");
lcd.print(rtc.getDateStr());
}

void check_water_level() {
int i = digitalRead(WAT1);
int j = digitalRead(WAT2);
if (i == 0 && j == 0) {
Serial.println("Tank Full");
} else if ((i == 0 && j == 1) || (i == 1 && j == 0)) {
Serial.println("Tank 1/2");
} else if (i == 1 && j == 1) {
Serial.println("Tank empty");
}
}
void on_svet1() {
digitalWrite(SVET1, HIGH);
}

void on_svet2() {
digitalWrite(SVET2, HIGH);
}

void on_svet3() {
digitalWrite(SVET3, HIGH);
}

void off_svet1() {
digitalWrite(SVET1, LOW);
}

void off_svet2() {
digitalWrite(SVET2, LOW);
}

void off_svet3() {
digitalWrite(SVET3, LOW);
}

void water_pomp_on() {
digitalWrite(NASOS, HIGH);
}

void water_pomp_off() {
digitalWrite(NASOS, LOW);
}

void check_user_command() {
if (Serial.available()) {
String strbuffer = Serial.readString();
if (strbuffer == STATUS) {
Serial.println(deliter);
print_date();
check_water_level();
check_ho2();
check_co2();
} else if (strbuffer == LIGHT1) {
on_svet1();
} else if (strbuffer == LIGHT1OFF) {
off_svet1();
} else if (strbuffer == LIGHT2) {
on_svet2();
} else if (strbuffer == LIGHT2OFF) {
off_svet2();
} else if (strbuffer == LIGHT3) {
on_svet3();
} else if (strbuffer == LIGHT3OFF) {
off_svet3();
} else if (strbuffer == LIGHTON) {
on_svet1();
on_svet2();
on_svet3();
} else if (strbuffer == LIGHTOFF) {
off_svet1();
off_svet2();
off_svet3();
} else if (strbuffer == POMPON) {
water_pomp_on();
} else if (strbuffer == POMPOFF) {
water_pomp_off();
}
}
}

void check_co2() {
int ppm_pwm = co2.readCO2PWM();
Serial.print(" CO2: ");
Serial.println(ppm_pwm);
}



void setup() {
dht.begin();
Serial.begin(9600);
RTC.begin(DS3231);
pinMode(DHTPIN, INPUT);
pinMode (WAT1, INPUT);
pinMode(WAT2, INPUT);
pinMode (CO2_IN, INPUT);
pinMode(SVET1, OUTPUT);
pinMode(SVET2, OUTPUT);
pinMode(SVET3, OUTPUT);
pinMode(NASOS, OUTPUT);
RTC.setHourMode(CLOCK_H24);
RTC.setDateTime(__DATE__, __TIME__);
}

void loop() {
check_user_command();
}
