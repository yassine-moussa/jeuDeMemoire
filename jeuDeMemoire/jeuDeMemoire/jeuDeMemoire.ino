#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Définition des broches pour les LEDs et les boutons
const int ledPins[] = {2, 3, 4, 5};
const int buttonPins[] = {6, 7, 8, 9};
const int buzzerPin = 10;
const int numberOfButtons = 4;

LiquidCrystal_I2C lcd(0x27, 16, 2);

int sequence[100];
int userInput[100];
int level = 1;
int score = 0;
int difficulty = 1; // 1 = facile, 2 = moyen, 3 = difficile
int delays[] = {500, 300, 150}; // Durées d'affichage et de pause pour chaque niveau de difficulté

void setup() {
  for (int i = 0; i < numberOfButtons; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(buzzerPin, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  randomSeed(analogRead(0));
  selectDifficulty();
}

void generateSequence() {
  for (int i = 0; i < level; i++) {
    sequence[i] = random(0, numberOfButtons);
  }
}

void playSequence() {
  int displayDelay = delays[difficulty - 1];
  for (int i = 0; i < level; i++) {
    digitalWrite(ledPins[sequence[i]], HIGH);
    delay(displayDelay);
    digitalWrite(ledPins[sequence[i]], LOW);
    delay(displayDelay);
  }
}

bool checkUserInput() {
  for (int i = 0; i < level; i++) {
    while (true) {
      for (int j = 0; j < numberOfButtons; j++) {
        if (digitalRead(buttonPins[j]) == LOW) {
          userInput[i] = j;
          digitalWrite(ledPins[j], HIGH);
          delay(300);
          digitalWrite(ledPins[j], LOW);
          delay(300);
          break;
        }
      }
      if (userInput[i] != -1) break;
    }
    if (userInput[i] != sequence[i]) return false;
  }
  return true;
}

void playBuzzer() {
  tone(buzzerPin, 1000, 200);
  delay(500);
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Niveau: ");
  lcd.print(level);
  lcd.setCursor(0, 1);
  lcd.print("Points: ");
  lcd.print(score);
}

void selectDifficulty() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Difficulte:");
  lcd.setCursor(0, 1);
  lcd.print("1:Facile 2:Moyen 3:Dur");

  while (true) {
    for (int i = 0; i < numberOfButtons; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {
        difficulty = i + 1;
        delay(300);
        return;
      }
    }
  }
}

void loop() {
  generateSequence();
  playSequence();
  if (checkUserInput()) {
    score += level;
    level++;
    delay(1000);
  } else {
    playBuzzer();
    level = 1;
    score = 0;
    delay(2000);
}
updateLCD();
}
