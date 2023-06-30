#include <LiquidCrystal.h> //Biblioteka za LCD
#include <OneWire.h> //Biblioteka za OneWire komunikaciju
#include <DallasTemperature.h> //Biblioteka za DS18B20 senzor

const int kulerPin = 6;
const int tempPin = 5;
const int led = 7;

const float tempTar = 27, tempMax=30;

LiquidCrystal lcd(13,12,11,10,9,8); //Pinovi u upotrebi za LCD displej
OneWire oneWire(tempPin); //Inicijalizacija komunikacije
DallasTemperature senzor(&oneWire); //Propustanje reference za OneWire

float temp;
int kuler;
int kulerSpeed;

void setup() {
  Serial.begin(9600);
  pinMode(kulerPin, OUTPUT);
  pinMode(led, OUTPUT);
  lcd.begin(16, 2);
  lcd.print("TEMP :");
  lcd.setCursor(0,1);
  lcd.print("KULER:");
  senzor.begin();
}

void loop() {
  
  senzor.requestTemperatures(); //Zahtevanje temperature od senzora
  temp = senzor.getTempCByIndex(0); //Koristimo ByIndex u slucaju da zelimo vise senzora

  if(temp<=tempTar) {
    kulerSpeed = 0;
    analogWrite(kulerPin, kulerSpeed);
    digitalWrite(kulerPin, LOW);
  }
  if((temp > tempTar) && (temp <= tempMax)) {
    kulerSpeed = int(abs(tempTar-temp)*33);
    
    if(kulerSpeed<20) {
      kulerSpeed = 0;
      analogWrite(kulerPin, kulerSpeed);
      digitalWrite(kulerPin, LOW);
    } else {
      analogWrite(kulerPin, kulerSpeed);
      Serial.println(kulerSpeed);
    }
  }
  if(temp >= tempMax) {
    digitalWrite(led, HIGH);
    kulerSpeed = 100;
  } else {
    digitalWrite(led, LOW);
  }
  
  ispis(temp, kulerSpeed);
  delay(250);
}

void ispis(float temp, int kul) {
  lcd.setCursor(6,0);
  lcd.print(temp, 1);
  lcd.print("C ");
  lcd.setCursor(6,1);
  lcd.print(kul);
  lcd.print("% ");
}