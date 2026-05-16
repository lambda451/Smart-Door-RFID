# RFID-Based Smart Access Control System

An Arduino-based smart access control system that uses RFID authentication to control door access through a servo motor locking mechanism. The project integrates multiple embedded peripherals including an RC522 RFID reader, DS3231 RTC module, I2C LCD display, LEDs, buzzer, and a manual override button.

## Features

- RFID card authentication
- Servo motor door lock control
- Real-time clock (RTC) timestamp display
- LCD status messages
- Audio and visual feedback
- Unauthorized access detection
- Manual override button
- Integration of SPI, I2C, PWM, and GPIO communication methods

## Hardware Components

- Arduino Uno
- RC522 RFID Module
- DS3231 RTC Module
- 16x2 I2C LCD Display
- SG90 Servo Motor
- LEDs
- Active Buzzer
- Push Button
- Breadboard and jumper wires

## Pin Mapping

| Component | Arduino Pin |
|---|---|
| Push Button | D2 |
| Green LED | D6 |
| Red LED | D7 |
| Buzzer | D8 |
| Servo Motor | D9 |
| RC522 SDA/SS | D10 |
| RC522 MOSI | D11 |
| RC522 MISO | D12 |
| RC522 SCK | D13 |
| RC522 RST | D5 |
| LCD SDA | A4 |
| LCD SCL | A5 |
| RTC SDA | A4 |
| RTC SCL | A5 |

## Required Libraries

Install the following libraries through the Arduino IDE Library Manager:

- MFRC522
- LiquidCrystal_I2C
- RTClib
- Servo

## System Operation

1. The system waits for an RFID card scan.
2. The RC522 reads the RFID card UID.
3. The Arduino compares the UID with the authorized UID.
4. If authorized:
   - Access is granted
   - Servo unlocks the system
   - Green LED and buzzer activate
   - LCD displays confirmation message
   - RTC displays timestamp
5. If unauthorized:
   - Access is denied
   - Red LED and buzzer alert the user
   - LCD displays warning message

## Future Improvements

- Cloud-based logging
- Mobile application integration
- Ultrasonic or PIR sensor integration
- EEPROM/database user storage
- Wireless connectivity

## Author

Bachir Dahouas
