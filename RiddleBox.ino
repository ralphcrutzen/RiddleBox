#include <LiquidCrystal.h>
#include <Servo.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

Servo slot;
int servoPin = 2;
int openPos = 45;
int dichtPos = 135;
int huidigPos = dichtPos;


int redPin = 13;
int greenPin = 12;
int bluePin = 11;

int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRight  0
#define btnUp     1
#define btnDown   2
#define btnLeft   3
#define btnSelect 4
#define btnNone   5

#define aantalVragen 3

String vragen[aantalVragen] = {
  "geel paars bruin",
  "1 11 21 1211 x",
  "x92458"
};

int antwoorden[aantalVragen] = {
  470,
  3112, 
  2997
};

int cheatcode = -11;

int vraag;
int getal;
int delta;

void setup() {
  randomSeed(analogRead(A1));
  
  Serial.begin(115200);
  
  lcd.begin(16, 2);
  
  slot.attach(servoPin);
  slot.write(dichtPos);
  
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  lichtUit();
  licht(redPin);
  delay(500);
  licht(greenPin);
  delay(500);
  licht(bluePin);
  delay(500);
  lichtUit();
  
  getal = 0;
  delta = 1;
  vraag = 0;
}

void loop() { 
  while (vraag == 3); // doe dan niks meer 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(vragen[vraag]);
  lcd.setCursor(0,1);
  lcd.print(getal);
  Serial.println(vragen[vraag]);

  lcd_key = read_LCD_buttons();
  while (lcd_key == btnNone) {
    lcd_key = read_LCD_buttons();
  }

  if (lcd_key == btnUp) {
    getal += delta;
  }


  if (lcd_key == btnDown) {
    getal -= delta;
  }

  if (lcd_key == btnRight) {
    if (delta < 10000) delta *= 10;
  }

  if (lcd_key == btnLeft) {
    if (delta > 1) delta /= 10;
  }  

  if (lcd_key == btnSelect) {
    if (getal == cheatcode) {
      vraag = 3;
      openSlot();
    }
    else {
      if (getal == antwoorden[vraag]) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Goed!");
        knipper(greenPin, 100, 10);
        vraag++;
        getal = 0;
        delta = 1;
        if (vraag == 3) {
          openSlot();
        }
        else {
          //
        }
      }
      else {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Fout.");
        knipper(redPin, 250, 4);
      }
    }
  }

  delay(300);
  
}

void lichtUit() {
  for (int i = 11; i <= 13; i++)
    digitalWrite(i, LOW);
}

void licht(int kleur) {
  for (int i = 11; i <= 13; i++)
    if (kleur == i)
      digitalWrite(i, HIGH);
    else
      digitalWrite(i, LOW);
}

void knipper(int kleur, int duur, int aantal) {
  lichtUit();
  for (int i = 0; i < aantal; i++) {
    licht(kleur);
    delay(duur);
    lichtUit();
    delay(duur);
  }
}

void openSlot() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Open!!!");
  if (openPos > huidigPos) {
    for (int pos = huidigPos; pos <= openPos; pos++) {
      slot.write(pos);
      delay(15);
    }
  }
  else {
    for (int pos = huidigPos; pos >= openPos; pos--) {
      slot.write(pos);
      delay(15);
    }    
  }
}


void sluitSlot() {
  if (dichtPos > huidigPos) {
    for (int pos = huidigPos; pos <= dichtPos; pos++) {
      slot.write(pos);
      delay(15);
    }
  }
  else {
    for (int pos = huidigPos; pos >= dichtPos; pos--) {
      slot.write(pos);
      delay(15);
    }    
  }
}

int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNone; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRight;  
 if (adc_key_in < 250)  return btnUp; 
 if (adc_key_in < 450)  return btnDown; 
 if (adc_key_in < 650)  return btnLeft; 
 if (adc_key_in < 850)  return btnSelect;  

 // For V1.0 comment the other threshold and use the one below:
/*
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   
*/


 return btnNone;  // when all others fail, return this...
}
