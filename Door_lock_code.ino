#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <IRremote.h>

int IRpin = 9;   // IR receiver input signal pin connected to pin 9
IRrecv irrecv(IRpin);
decode_results results; 

SoftwareSerial mySerial(2,3);  //Serial Communications

//Created instances
LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int relayPin = 12;

void setup()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  Serial.begin(9600);
  
  irrecv.enableIRIn();
  
  while (!Serial);
  delay(100);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Fingerprint Door");
  lcd.setCursor(0, 1);
  lcd.print("lock ");
  delay(5000);
  lcd.clear();

  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    lcd.setCursor(0, 0);
    lcd.print("  FingerPrint ");
    lcd.setCursor(0, 1);
    lcd.print("Sensor Connected");
  }

  else  {
    lcd.setCursor(0, 0);
    lcd.print("Unable to found");
    lcd.setCursor(0, 1);
    lcd.print("Sensor");
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Check Connections");

    while (1) {
      delay(1);
    }
  }
  lcd.clear();
}

void loop()                     // run over and over again
{
  
  getFingerprintIDez();
  delay(1000);            //don't need to run this at full speed.

  remoteControl();
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  {
    lcd.setCursor(0, 0);
    lcd.print("  Waiting For");
    lcd.setCursor(0, 1);
    lcd.print("  Valid Finger");
    return -1;
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  Messy Image");
    lcd.setCursor(0, 1);
    lcd.print("  Try Again");
    delay(3000);
    lcd.clear();
    return -1;
  }

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Not Valid Finger");
    delay(3000);
    lcd.clear();
    return -1;
  }

  // found a match!
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Door Unlocked");
  lcd.setCursor(0, 1);
  lcd.print("    Welcome");
  digitalWrite(relayPin, LOW);
  delay(3000);
  digitalWrite(relayPin, HIGH);
  lcd.clear();
  return finger.fingerID;
}

//function for remote control
int remoteControl(){
  
if (irrecv.decode(&results)) 
    {   
     irrecv.resume();   // Receive the next value
     digitalWrite(relayPin,LOW);
     delay(3000);
     digitalWrite(relayPin,HIGH);
     return 1;
     
    }
}
