#include "DHT.h"
#include "LiquidCrystal.h"

#define DHTPIN 12 // Pin relié au capteur DHT 11
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


LiquidCrystal lcd(rs, enable, 5, 4, 3, 2); // Initialisation de l'écran LCD
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




// Fonction qui affiche sur le moniteur série les informations concernant la température et l'humidité
void tempHum() {
  // Récupération de la température de l'air
  double dTempExt = dht.readTemperature();
  
  // Affichage de la température de l'air
  Serial.print("Température de l'air: ");
  Serial.print(dTempExt);
  Serial.println("°C");
  
  // Récupération de l'humidité de l'air
  double dHumidExt = dht.readHumidity();
  
  // Affichage de l'humidité de l'air
  Serial.print("Humidité de l'air: ");
  Serial.print(dHumidExt);
  Serial.println("%");
  tempsActuel = millis();
}



void gaz() {

  // Récupération des données provenant des capteurs de gaz et conversion
  niv_pol_MQ135_0 = map(analogRead(MQ135_0_A), 0, 1023, 10, 1000); // Conversion des données de [0-1023] à [10-1000]
  niv_pol_MQ135_1 = map(analogRead(MQ135_1_A), 0, 1023, 10, 1000);

  // Actualisation des informations à l'écran LCD
  lcd.clear();
  lcd.print("Qlt air: ");
  if (niv_pol_MQ135_0 >= niv_pol_MQ135_1) {
    lcd.print(niv_pol_MQ135_0);
  }  
  else {
    lcd.print(niv_pol_MQ135_1);
  }
  lcd.print(" ppm");

  lcd.setCursor(0,1); // Permet d'écrire sur la deuxième ligne de l'écran LCD
  if (niv_pol_MQ135_0 <= 170 && niv_pol_MQ135_1 <= 170) {
    lcd.print("Air clean");
    digitalWrite(buzzer, LOW); // On éteint l'alarme si la qualité descend en-dessous de 600 ppm
  }
  else if (niv_pol_MQ135_0 <= 600 || niv_pol_MQ135_1 <= 600) {
    lcd.print("Air risque");
    digitalWrite(buzzer, LOW);
  }
  else if(niv_pol_MQ135_0 > 600 || niv_pol_MQ135_1 > 600) {
    lcd.print("Air pollue");
    digitalWrite(buzzer, HIGH); // On sonne l'alarme si la qualité de l'air dépasse 600 ppm
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
