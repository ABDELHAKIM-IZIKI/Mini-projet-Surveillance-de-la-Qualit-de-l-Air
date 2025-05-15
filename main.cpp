#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Définition des constantes
#define CAPTEUR_GAZ A0
#define DHT_PIN 10 
#define BUZZER 8
#define LED_ROUGE 4
#define LED_JAUNE 5
#define LED_VERTE 7
#define DHT_TYPE DHT22

// Initialisation des capteurs et afficheur
DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4);


// Configuration initiale
void setup() {
  // Initialisation du capteur DHT
  dht.begin();
  
  // Configuration de l'écran LCD
  lcd.init();
  lcd.backlight();  
  lcd.setCursor(0, 0);
  
  // Configuration des broches
  pinMode(LED_ROUGE, OUTPUT);
  pinMode(LED_JAUNE, OUTPUT);
  pinMode(LED_VERTE, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(CAPTEUR_GAZ, INPUT);

  Serial.begin(9600);
}

String getNiveauQualiteAir(int valeur) {
  if (valeur < 30) return "Pur";
  else if (valeur < 60) return "Faible Pollution";
  else if (valeur < 80) return "Forte Pollution";
  else return "DANGER !";
}

void afficherDonnees(float temp, float hum, int pollution, String qualite) {
  lcd.clear();
  
  // Ligne 1: Température
  lcd.setCursor(0, 0);
  lcd.print("Temp : ");
  lcd.print(temp);
  lcd.print(" C");
  
  // Ligne 2: Humidité
  lcd.setCursor(0, 1);
  lcd.print("Hum : ");
  lcd.print(hum);
  lcd.print(" %");
  
  // Ligne 3:  Pourcentage qualité air
  lcd.setCursor(0, 2);
  lcd.print("P Q Air : ");
  lcd.print(pollution);
  lcd.print(" %");
  
  // Ligne 4: Qualité air
  lcd.setCursor(0, 3);
  lcd.print("Q Air : ");
  lcd.print(qualite);
}

void gererAlertes(int pollution) {
  // Niveau 1: Air pur
  if (pollution < 30) {
    noTone(BUZZER);
    digitalWrite(LED_ROUGE, LOW);
    digitalWrite(LED_JAUNE, LOW);
    digitalWrite(LED_VERTE, HIGH);
  } 
  // Niveau 2: Faible pollution
  else if (pollution < 60) {
    noTone(BUZZER);
    digitalWrite(LED_ROUGE, LOW);
    digitalWrite(LED_VERTE, LOW);
    digitalWrite(LED_JAUNE, HIGH);
  } 
  // Niveau 3: Pollution dangereuse
  else {
    digitalWrite(LED_JAUNE, LOW);
    digitalWrite(LED_VERTE, LOW);
    digitalWrite(LED_ROUGE, HIGH);
    tone(BUZZER, 1000); 
    delay(500);
  }
}


// Boucle principale
void loop() {
  // Lecture des capteurs
  float temperature = dht.readTemperature();
  float humidite = dht.readHumidity();
  int valeurGaz = analogRead(CAPTEUR_GAZ);
  int pourcentageGaz = map(valeurGaz, 0, 1023, 0, 100);
  String qualiteAir = getNiveauQualiteAir(pourcentageGaz);

  // Affichage sur LCD
  afficherDonnees(temperature, humidite, pourcentageGaz, qualiteAir);
  
  // Gestion des alertes
  gererAlertes(pourcentageGaz);

  delay(1000); 
}
