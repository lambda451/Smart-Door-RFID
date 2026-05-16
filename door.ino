#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <RTClib.h>

// ---------- Pin Mapping ----------
const int buttonPin = 2;
const int rfidRST   = 5;
const int greenLED  = 6;
const int redLED    = 7;
const int buzzerPin = 8;
const int servoPin  = 9;
const int rfidSS    = 10;

// ---------- Objects ----------
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;
MFRC522 rfid(rfidSS, rfidRST);
RTC_DS3231 rtc;

// ---------- Authorized Card UID ----------
byte authorizedUID[] = {0x37, 0x87, 0x14, 0x06};
byte authorizedUIDSize = 4;

bool lastButtonState = HIGH;

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  myServo.attach(servoPin);
  myServo.write(0);

  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, HIGH);

  lcd.init();
  lcd.backlight();

  SPI.begin();
  rfid.PCD_Init();

  if (!rtc.begin()) {
    lcd.clear();
    lcd.print("RTC Error");
    Serial.println("RTC not found!");
    while (1);
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan Card...");

  Serial.println("System Ready");
}

void loop() {
  checkButton();
  checkRFID();
}

void checkButton() {
  bool currentButtonState = digitalRead(buttonPin);

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    Serial.println("Manual override used");
    printTime();
    grantAccess("Manual Override");
  }

  lastButtonState = currentButtonState;
}

void checkRFID() {
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  Serial.print("Card UID: ");
  printUID();

  if (isAuthorized()) {
    Serial.println("Access Granted");
    printTime();
    grantAccess("Access Granted");
  } else {
    Serial.println("Access Denied");
    printTime();
    denyAccess();
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

bool isAuthorized() {
  if (rfid.uid.size != authorizedUIDSize) return false;

  for (byte i = 0; i < authorizedUIDSize; i++) {
    if (rfid.uid.uidByte[i] != authorizedUID[i]) {
      return false;
    }
  }

  return true;
}

void grantAccess(String message) {
  digitalWrite(greenLED, HIGH);
  digitalWrite(redLED, LOW);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);

  tone(buzzerPin, 1000);
  delay(150);
  noTone(buzzerPin);

  myServo.write(90);
  delay(3000);

  myServo.write(0);

  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Locked");
  delay(500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan Card...");
}

void denyAccess() {
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Access Denied");

  tone(buzzerPin, 500);
  delay(500);
  noTone(buzzerPin);

  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan Card...");
}

void printUID() {
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print("0x");
    if (rfid.uid.uidByte[i] < 0x10) Serial.print("0");
    Serial.print(rfid.uid.uidByte[i], HEX);

    if (i < rfid.uid.size - 1) Serial.print(", ");
  }
  Serial.println();
}

void printTime() {
  DateTime now = rtc.now();

  Serial.print("Time: ");
  Serial.print(now.day());
  Serial.print("/");
  Serial.print(now.month());
  Serial.print("/");
  Serial.print(now.year());
  Serial.print(" ");

  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.println(now.second());
}