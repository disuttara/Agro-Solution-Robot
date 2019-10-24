#include<Servo.h>
#include <Wire.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht;

int RM1 = 1 ;
int RM2 = 2 ;
int LM1 = 3 ;
int LM2 = 4 ;
int pump = 5 ;
int switch1 = 6;

Servo servo1 ;
Servo servo2 ;

const int trigPin = 21;
const int echoPin = 22;

int moisture_sensor = A0;



float humidity;
float temperature;
int moisture_output;
long duration;
int distance;

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);

  dht.setup(10);

  servo1.attach(11);
  servo2.attach(12);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  lcd.init();
  lcd.init();
  lcd.backlight();

}

void dht_sensor()
{
  delay(dht.getMinimumSamplingPeriod());

  humidity = dht.getHumidity();
  temperature = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.println(dht.toFahrenheit(temperature), 1);
}

void moisture_sensorf() {
  moisture_output = analogRead(moisture_sensor);
  moisture_output = map(moisture_output, 550, 0, 0, 100);
  Serial.print("Mositure : ");
  Serial.print(moisture_output);
  Serial.println("%");
  delay(100);
}

void sonar() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);
}

void forward() {
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
}
void backward() {
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
}
void turn_right() {
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
}
void turn_left() {
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
}
void bot_stop() {
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
}
void loop() {
  dht_sensor();

  ////temperature
  lcd.setCursor(0, 1);
  lcd.print("Temp:");
  lcd.setCursor(5, 1);
  lcd.print(temperature);

  ////temperature
  lcd.setCursor(8, 1);
  lcd.print("Hum:");
  lcd.setCursor(12, 1);
  lcd.print(humidity);


  //// seeding
  servo1.write(0);
  delay(500);
  servo1.write(90);
  delay(500);

  ////moisture
  servo2.write(0);
  moisture_sensorf();
  lcd.setCursor(0, 0);
  lcd.print("Mos :");
  lcd.setCursor(6, 0);
  lcd.print(moisture_output);
  delay(2000);
  servo2.write(90);
  delay(500);

   ////pumping
    if (moisture_output <= 30) {
      digitalWrite(pump, HIGH);
      delay(3000);
    }
    if (moisture_output > 30) {
      digitalWrite(pump, LOW);
    }

  ///direction
  forward();
  delay(3000);
  bot_stop();
}
