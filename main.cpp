/**
 * ========================================
 * CIFRU ELECTRONIC SEIF (CU EEPROM)
 * ========================================
 */

#include <Arduino.h>
#include <Keypad.h>
#include <EEPROM.h> // Biblioteca pentru memoria permanenta

// ========== CONFIGURARE TASTATURA ==========

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','4','7','*'},
  {'2','5','8','0'},
  {'3','6','9','#'},
  {'A','B','C','D'}
};

byte rowPins[ROWS] = {5, 4, 3, 2};  
byte colPins[COLS] = {9, 8, 7, 6};  

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ========== CONFIGURARE LED-URI ==========

#define LED_VERDE 11
#define LED_ROSU 10

// ========== VARIABILE ==========

String codCorect = "1234"; // Codul default
String codIntrodus = "";
int incercariGresite = 0;
const int MAX_INCERCARI = 3;
bool blocat = false;
bool modSchimbareParola = false;

// ========== DECLARARE FUNCTII ==========
void proceseazaTasta(char tasta);
void verificaCod();
void accesPermis();
void accesRefuzat();
void blocare();
void deblocare();
void afiseazaPrompt();
void citesteCodDinMemorie();

// ========== SETUP ==========

void setup() {
  Serial.begin(9600);
  
  citesteCodDinMemorie();
  
  Serial.println(F("\n========================================"));
  Serial.println(F("     CIFRU ELECTRONIC SEIF"));
  Serial.println(F("========================================"));
  Serial.print(F("Cod actual: ")); Serial.println(codCorect);
  Serial.println(F("* = sterge cod | A = schimba codul"));
  Serial.println(F("========================================\n"));
  
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROSU, OUTPUT);
  
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_ROSU, LOW);
  
  afiseazaPrompt();
}

// ========== LOOP ==========

void loop() {
  char tasta = keypad.getKey();
  if (tasta) {
    proceseazaTasta(tasta);
  }
}

// ========== IMPLEMENTARE FUNCTII ==========

void proceseazaTasta(char tasta) {
  Serial.print(F("Tasta: "));
  Serial.println(tasta);
  
  if (tasta == 'A') {
    if (codIntrodus == codCorect) {
      modSchimbareParola = true;
      codIntrodus = "";
      Serial.println(F("\n[ADMIN] Introdu noul cod (minim 4 cifre) si apasa #"));
      digitalWrite(LED_VERDE, HIGH);
    } else {
      Serial.println(F("[EROARE] Trebuie sa introduci codul ACTUAL corect, apoi apasa A pentru a-l schimba."));
      codIntrodus = "";
    }
  }
  else if (tasta == '#') {
    if (modSchimbareParola) {
      if (codIntrodus.length() >= 4) {
        codCorect = codIntrodus;
        
        for (unsigned int i = 0; i < codCorect.length(); i++) {
          EEPROM.write(i, codCorect[i]);
        }
        EEPROM.write(codCorect.length(), '\0');
        
        modSchimbareParola = false;
        codIntrodus = "";
        digitalWrite(LED_VERDE, LOW);
        
        Serial.println(F("\n[SUCCES] Noul cod a fost SALVAT permanent!"));
        afiseazaPrompt();
      } else {
        Serial.println(F("[EROARE] Noul cod trebuie sa aiba minim 4 cifre! Mai incearca."));
        codIntrodus = "";
      }
    } else {
      verificaCod();
    }
  }
  else if (tasta == '*') {
    codIntrodus = "";
    modSchimbareParola = false;
    digitalWrite(LED_VERDE, LOW);
    Serial.println(F("[STERS] Cod resetat. Operatiune anulata."));
    afiseazaPrompt();
  }
  else if (tasta >= '0' && tasta <= '9') {
    if (codIntrodus.length() < 10) {
      codIntrodus += tasta;
      Serial.print(F("Cod: "));
      for (unsigned int i = 0; i < codIntrodus.length(); i++) {
        Serial.print('*');
      }
      Serial.println();
    }
  }
}

void verificaCod() {
  Serial.println(F("\n--- VERIFICARE ---"));
  
  if (codIntrodus.length() == 0) {
    Serial.println(F("[EROARE] Cod gol!"));
    afiseazaPrompt();
    return;
  }
  
  if (blocat) {
    if (codIntrodus == codCorect) {
      deblocare();
    } else {
      Serial.println(F("[BLOCAT] Cod gresit!"));
      for (int i = 0; i < 3; i++) {
        digitalWrite(LED_ROSU, LOW);
        delay(100);
        digitalWrite(LED_ROSU, HIGH);
        delay(100);
      }
    }
  }
  else {
    if (codIntrodus == codCorect) {
      accesPermis();
    } else {
      accesRefuzat();
    }
  }
  
  codIntrodus = "";
}

void accesPermis() {
  Serial.println(F("\n========================================"));
  Serial.println(F("      ACCES PERMIS!"));
  Serial.println(F("========================================\n"));
  
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_ROSU, LOW);
  delay(3000);
  digitalWrite(LED_VERDE, LOW);
  
  incercariGresite = 0;
  afiseazaPrompt();
}

void accesRefuzat() {
  incercariGresite++;
  
  Serial.println(F("\n========================================"));
  Serial.print(F("      COD GRESIT! ("));
  Serial.print(incercariGresite);
  Serial.print(F("/"));
  Serial.print(MAX_INCERCARI);
  Serial.println(F(")"));
  Serial.println(F("========================================"));
  
  if (incercariGresite >= MAX_INCERCARI) {
    blocare();
  } else {
    int ramase = MAX_INCERCARI - incercariGresite;
    Serial.print(F("Mai ai "));
    Serial.print(ramase);
    Serial.println(F(" incercari!\n"));
    
    for (int i = 0; i < 2; i++) {
      digitalWrite(LED_ROSU, HIGH);
      delay(250);
      digitalWrite(LED_ROSU, LOW);
      delay(250);
    }
    
    afiseazaPrompt();
  }
}

void blocare() {
  blocat = true;
  
  Serial.println(F("\n========================================"));
  Serial.println(F("      SISTEM BLOCAT!"));
  Serial.println(F("========================================"));
  Serial.println(F("Prea multe incercari gresite!"));
  Serial.println(F("========================================\n"));
  
  digitalWrite(LED_ROSU, HIGH);
  digitalWrite(LED_VERDE, LOW);
}

void deblocare() {
  blocat = false;
  incercariGresite = 0;
  
  Serial.println(F("\n========================================"));
  Serial.println(F("      DEBLOCAT!"));
  Serial.println(F("========================================\n"));
  
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_ROSU, LOW);
    delay(200);
    digitalWrite(LED_VERDE, LOW);
    delay(200);
  }
  
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_ROSU, LOW);
  
  afiseazaPrompt();
}

void afiseazaPrompt() {
  if (blocat) {
    Serial.println(F("BLOCAT >>> Deschide cu codul: ____#"));
  } else {
    Serial.println(F("Introdu cod: ____# (sau cod_actual+A pt schimbare)"));
  }
}

void citesteCodDinMemorie() {
  char primulCaracter = EEPROM.read(0);
  if (primulCaracter >= '0' && primulCaracter <= '9') {
    codCorect = ""; // Stergem codul "1234"
    int adresa = 0;
    char c = EEPROM.read(adresa);
    while (c != '\0' && adresa < 10) {
      codCorect += c;
      adresa++;
      c = EEPROM.read(adresa);
    }
  }
}