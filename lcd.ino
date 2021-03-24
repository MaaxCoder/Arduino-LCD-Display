#include <LiquidCrystal.h>
#include <Servo.h>

LiquidCrystal lcd(12, 11,5, 4, 3, 2);
Servo monServo;

int etatPotentiometre, mapEtatPotentiometre;
int mode = 0;
int btPlus = 7;
int btSet = 8;
int btMoins = 9;
int btTest = 13;
int angleServo;
int pourcAngleServo;
int vitesse;
int potentio = A0;

boolean etatbtSet;
boolean etatBtTest;
boolean etatBtMoins;
boolean etatBtPlus;
boolean calib = 0;

byte Coeur[8] = {
    B00000,
    B01010,
    B11111,
    B11111,
    B11111,
    B01110,
    B00100,
    B00000
};

byte Cadre[8] = {
    B11111,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B11111
};

byte Rempli[8] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111
};

byte Degre[8] = {
    B00000,
    B11000,
    B11000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000
};

void setup()
{
    lcd.begin(16, 2);
    pinMode(btSet, INPUT_PULLUP);
    pinMode(btTest, INPUT_PULLUP);
    pinMode(btPlus, INPUT_PULLUP);
    pinMode(btMoins, INPUT_PULLUP);

    lcd.createChar(0, Coeur);
    lcd.createChar(1, Cadre);
    lcd.createChar(2, Rempli);
    lcd.createChar(3, Degre);

    Serial.begin(9600);

    monServo.attach(10);
}

void loop()
{
    etatbtSet = digitalRead(btSet);

    while (mode == 0 && etatbtSet == 1)
    {
        lcd.setCursor(0, 0);
        lcd.print("0 : Accueil");
        lcd.setCursor(0, 1);
        lcd.print("Les 3 boutons ");
        lcd.write(byte(0));
        verifbtSet(1);
    }
    
    while (mode == 1 && etatbtSet == 1)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("1: Potentiometre");
        lcd.setCursor(0, 1);
        etatPotentiometre = analogRead(potentio);
        lcd.print(etatPotentiometre);
        lcd.print("/1023 ");
        lcd.setCursor(12,1);
        mapEtatPotentiometre = map(etatPotentiometre, 0, 1023, 0, 100);
        lcd.print(mapEtatPotentiometre);
        lcd.print("%");

        verifbtSet(2);

        delay(100);
    }

    while (mode == 2 && etatbtSet == 1)
    {
        lcd.clear();
        etatBtMoins = digitalRead(btMoins);
        etatBtPlus = digitalRead(btPlus);
        lcd.setCursor(0, 0);
        lcd.print("2: Le bouton");
        lcd.setCursor(0, 1);
        if (etatBtPlus == 0 || etatBtMoins == 0)
        {
            lcd.print("appuye");
            lcd.setCursor(8, 2);
            lcd.write(byte(2));
        }
        else
        {
            lcd.print("relache");
            lcd.setCursor(8, 2);
            lcd.write(byte(1));
        }

        verifbtSet(3);

        delay(100); 
    }

    while (mode == 3 && etatbtSet == 1)
    {
        angleServo = monServo.read();
        pourcAngleServo = map(angleServo, 0, 180, 0, 100);
        etatBtMoins = digitalRead(btMoins);
        etatBtPlus = digitalRead(btPlus);
        etatPotentiometre = analogRead(potentio);
        vitesse = map(etatPotentiometre, 0, 1020, 5, 0);

        if (etatBtPlus == 0 && angleServo != 180)
        {
            monServo.write(angleServo + vitesse);
        }
        else if (etatBtMoins == 0 && angleServo > 0)
        {
            monServo.write(angleServo - vitesse);
        }
        
        

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("3 : Servomoteur");
        lcd.setCursor(0, 1);
        lcd.leftToRight();
        lcd.print(angleServo);
        lcd.write(byte(3));
        lcd.setCursor(5, 1);
        lcd.print(pourcAngleServo);
        lcd.print("%");
        lcd.setCursor(11, 1);
        lcd.print("V:");
        lcd.print(vitesse);
        lcd.print	("/5");

        verifbtSet(0);
        delay(100);

        
    }
    
    
    
}

void verifbtSet(int nb)
{
    etatbtSet = digitalRead(btSet);
    
    if (etatbtSet == 0)
    {
        mode = nb;
        lcd.clear();
    }
}