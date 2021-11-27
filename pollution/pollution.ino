#include "DHT.h"
#include "LiquidCrystal.h"

#define DHTPIN 12
#define DHTTYPE DHT11    // DHT11 sinon

#define MQ135_0_A A0
#define MQ135_1_A A1

#define MQ135_0_D 8
#define MQ135_1_D 9

#define buzzer 7


byte const rs = 11;
byte const enable = 10;

int niv_pol_MQ135_0;
int niv_pol_MQ135_1;

float tempsActuel;


LiquidCrystal lcd(rs, enable, 5, 4, 3, 2);
DHT dht(DHTPIN, DHTTYPE);


void setup() {
 Serial.begin(9600); 
 
 pinMode(MQ135_0_D, INPUT);
 pinMode(MQ135_1_D, INPUT);
 pinMode(buzzer, OUTPUT);

 digitalWrite(buzzer, LOW);
 lcd.begin(16, 2);

 tempsActuel = millis();
}





void tempHum() {
  // On récupère la température de l'air
  double dTempExt = dht.readTemperature();

  // On imprime la température de l'air
  Serial.print("Température de l'air: ");
  Serial.print(dTempExt);
  Serial.println("°C");

  // On récupère la température de l'air
  double dHumidExt = dht.readHumidity();
  
  // On imprime l'humidité de l'air
  Serial.print("Humidité de l'air: ");
  Serial.print(dHumidExt);
  Serial.println("%");
  tempsActuel = millis();
}



void gaz() {
  niv_pol_MQ135_0 = map(analogRead(MQ135_0_A), 0, 1023, 10, 1000);
  niv_pol_MQ135_1 = map(analogRead(MQ135_1_A), 0, 1023, 10, 1000);
  
  lcd.clear();
  lcd.print("Qlt air: ");
  if (niv_pol_MQ135_0 >= niv_pol_MQ135_1) {
    lcd.print(niv_pol_MQ135_0);
  }  
  else {
    lcd.print(niv_pol_MQ135_1);
  }
  lcd.print(" ppm");

  lcd.setCursor(0,1);
  if (niv_pol_MQ135_0 <= 170 && niv_pol_MQ135_1 <= 170) {
    lcd.print("Air clean");
    digitalWrite(buzzer, LOW);
  }
  else if (niv_pol_MQ135_0 <= 600 || niv_pol_MQ135_1 <= 600) {
    lcd.print("Air risque");
    digitalWrite(buzzer, LOW);
  }
  else if(niv_pol_MQ135_0 > 600 || niv_pol_MQ135_1 > 600) {
    lcd.print("Air pollue");
    digitalWrite(buzzer, HIGH);
  }
  
  
  if (digitalRead(MQ135_0_D) == 0 || digitalRead(MQ135_1_D) == 0) {
    digitalWrite(buzzer, HIGH);
  }
}



void loop() {

  if ( millis() - tempsActuel >= 30000) {
    tempHum();
  }
  
  gaz();
  
  delay(1000);
  
  
}
