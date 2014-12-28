//import sha1 and TOTP support
#include "sha1.h"
#include "TOTP.h"

//setup a single secret
// The shared secret is MyLegoDoor
uint8_t hmacKey[] = {
  0x4d, 0x79, 0x4c, 0x65, 0x67, 0x6f, 0x44, 0x6f, 0x6f, 0x72};

//create TOTP object
TOTP totp = TOTP(hmacKey, 10);
char code[7];

//#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
//int buttonInput = 3; //my button
////#include <TinyWireM.h> // Enable this line if using Adafruit Trinket, Gemma, etc.
//#include <Wire.h>
//
//#else
////#define SERIAL_OK
////#define DEBUG 
//int buttonInput = 2; //my button
#include <Wire.h>
//#endif

int buttonInput = 3; //my button

//import to to use RTC
#include "RTClib.h"

RTC_DS1307 rtc;

//setup the lcd
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_7segment matrix = Adafruit_7segment();

int lightOutput = 13; // the red sik led
int buttonState = 0;
boolean pressed = false ;
String toPrint = "" ;
//uint16_t counter = 0000 ;
DateTime now ; 
long releaseTime ;
boolean cleared ;
#define CLEAR_TIME 5

void setup() {

#ifdef SERIAL_OK
  //setup serial  
  Serial.begin(9600);
#endif

  //setup i2c
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif

  //setup the led
  pinMode(lightOutput, OUTPUT);
  //setup the button
  pinMode(buttonInput, INPUT);
  digitalWrite(buttonInput, HIGH);

  //setup the led defaults to 0x70
  matrix.begin(0x70);

  matrix.println(10000);
  matrix.writeDisplay();
  delay(500);

  matrix.println(1111);
  matrix.writeDisplay();
  delay(500);

  matrix.println(10000);
  matrix.writeDisplay();
  delay(500);

  //i need to assign a junk value for some reason  
  toPrint = "5" ;

  pressed = false ;

  rtc.begin();

#if 0
  if (! rtc.isrunning()) 
  {
    Serial.println("RTC is NOT running!");
  }
#endif
  while( ! rtc.isrunning() ) {} 

}

void loop() {

  buttonState = digitalRead(buttonInput);

  if ( (buttonState == LOW)  && ( pressed == false ) )
  {
    
    pressed = true ;
    cleared = false ;
    //light the led
    digitalWrite(lightOutput, HIGH);

    now = rtc.now();

    long GMT = now.unixtime() ;
    char* newCode = totp.getCode(GMT);
    strcpy(code, newCode);
#ifdef SERIAL_OK
    Serial.println(code);
#endif
 
    toPrint.setCharAt(0, code[0] ) ;
#ifdef DEBUG
    Serial.print( "First digit: " ) ;
    Serial.println( toPrint ) ;
#endif
    matrix.writeDigitNum( 0, toPrint.toInt(), false ) ;
    
    toPrint.setCharAt(0, code[1] ) ;
#ifdef DEBUG
    Serial.print( "Second digit: " ) ;
    Serial.println( toPrint ) ;
#endif
    matrix.writeDigitNum( 1, toPrint.toInt(), false ) ;

    toPrint.setCharAt(0, code[2] ) ;
#ifdef DEBUG
    Serial.print( "Third digit: " ) ;
    Serial.println( toPrint ) ;
#endif
    matrix.writeDigitNum( 3, toPrint.toInt(), false ) ;
    
    matrix.writeDigitRaw( 4, 0 ) ;

    matrix.drawColon( true ) ;

    matrix.writeDisplay();

  }
  else if ( ( buttonState == HIGH ) && (pressed == true ))
  {
    pressed = false ;
//    Serial.println("Released: " ) ;
    digitalWrite(lightOutput, LOW);

    matrix.writeDigitRaw( 0, 0 ) ;

    toPrint.setCharAt(0, code[3] ) ;
#ifdef DEBUG
    Serial.print( "Fourth digit: " ) ;
    Serial.println( toPrint ) ;
#endif
    matrix.writeDigitNum( 1, toPrint.toInt(), false ) ;
    
    toPrint.setCharAt(0, code[4] ) ;
#ifdef DEBUG
    Serial.print( "Fifth digit: " ) ;
    Serial.println( toPrint ) ;
#endif
    matrix.writeDigitNum( 3, toPrint.toInt(), false ) ;
    
    toPrint.setCharAt(0, code[5] ) ;
#ifdef DEBUG
    Serial.print( "Sixth digit: " ) ;
    Serial.println( toPrint ) ;
#endif
    matrix.writeDigitNum( 4, toPrint.toInt(), false ) ;
    
    matrix.drawColon( false ) ;

    matrix.writeDisplay();
    
    now = rtc.now();

    releaseTime = now.unixtime() ;
//    Serial.print( "Relase time: " ) ;
//    Serial.println( releaseTime ) ;
      
  } 
  else if ( ( buttonState == HIGH ) && (pressed == false ))
  {
    now = rtc.now();
    if ( ( now.unixtime() > releaseTime + CLEAR_TIME ) && ! cleared ) 
    {
//      Serial.print( "Clear time: " ) ;
//      Serial.println( now.unixtime() ) ;
      
      cleared = true ;
      matrix.clear() ;
      matrix.writeDisplay() ;
    }
  }
}

