TTTOTP
======

Tiny Trinket Time-Based One-Time Password

Purpose:
This project uses an Adafruit Trinket Pro, LED backpack and a DS1307 RTC breakout board to generate a TOTP hash. 

This project uses the following libraries
- Adafruit LED Backpack Library ( https://github.com/adafruit/Adafruit-LED-Backpack-Library )
- Adafruit RTC Library ( https://github.com/adafruit/RTClib )
- Cathedrow's Cryptosuit ( https://github.com/Cathedrow/Cryptosuite )
- Luca Dentella's TOTP library ( http://www.lucadentella.it/en/totp-libreria-per-arduino/ and https://github.com/lucadentella/ArduinoLib_TOTP )

Hardware:
- A Sparkfun Redboard was used for development and testing. It is red and has serial. (https://www.sparkfun.com/products/12757)
- Adafruit Trinket Pro (https://www.adafruit.com/product/2000)
- Adafruit .56" LED Backpack (https://learn.adafruit.com/adafruit-led-backpack/0-dot-56-seven-segment-backpack)
- Adafruit DS1307 Breakout Board (https://www.adafruit.com/product/264)
- breadboard, 6mm button and jumpers

Challenges:
- Learning the Arduino programming environment
- Interfacing with hardware
- Displaying a 6 digit number on a 4 digit display is a challenge. 

Notes: 
- The return value of Sha1Class::write() and Sha256Class::write() needed to be updated to return size_t. 
- ifdefs need to be worked out as the test code was developed on a Sparkfun Redboard but used on an Adafruit Trinket Pro

