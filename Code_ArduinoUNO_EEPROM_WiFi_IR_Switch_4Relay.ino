/**********************************************************************************
 *  TITLE: WiFi (SinricPro) + IR + Manual Switch (latched) control 4 Relays using Arduino EEPROM
 *  Click on the following links to learn more. 
 *  YouTube Video: https://www.youtube.com/watch?v=1UPYd1EZyW4&ab_channel=FutureEES
 *
 *  Download the libraries 
 *  IRremote 3.5.2 Library:  https://github.com/Arduino-IRremote/Arduino-IRremote
 *  Arduino-timer 2.3.1 Library: https://github.com/contrem/arduino-timer
 **********************************************************************************/

#include <EEPROM.h>
#include <IRremote.h>
#include <SoftwareSerial.h>
#include <arduino-timer.h>

auto timer = timer_create_default(); // create a timer with default settings

SoftwareSerial mySerial(2, 3); // RX, TX D2, D3

// define the GPIO connected with Relays and switches
#define RelayPin1 4  //D4

#define WIFI_LED A3  //A3

String bt_data; // variable for storing bluetooth data
String pinStatus = "0000";

void relayOnOff(int relay){
 switch(relay){
      case 1:
            digitalWrite(RelayPin1, !digitalRead(RelayPin1)); // change state for relay-1
            EEPROM.update(0,digitalRead(RelayPin1));
            delay(100);
      default : break;      
      }  
}

void eepromState()
{
  digitalWrite(RelayPin1, EEPROM.read(0)); delay(200);
}  

void wifi_control()
{
  if(mySerial.available()) {
    bt_data = mySerial.readString();
    Serial.println(bt_data);
        if(bt_data == "A1") { digitalWrite(RelayPin1, LOW);  EEPROM.update(0,LOW); } // if 'A1' received Turn on Relay1
        if(bt_data == "A0") { digitalWrite(RelayPin1, HIGH); EEPROM.update(0,HIGH); } // if 'A0' received Turn off Relay1
        if(bt_data == "W1") { digitalWrite(WIFI_LED, HIGH); } // if 'W1' Turn on WiFi LED
        if(bt_data == "W0") { digitalWrite(WIFI_LED, LOW); } // if 'W0' Turn off WiFi LED
        delay(20);
  }
}

void sendStatus(){  
  pinStatus = String(!digitalRead(RelayPin1));
  mySerial.print(pinStatus);
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(RelayPin1, OUTPUT);
  pinMode(WIFI_LED, OUTPUT);

  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, HIGH);
  digitalWrite(WIFI_LED, LOW);
  
  delay(500);
  eepromState();
  // call the toggle_led function every 2000 millis (2 seconds)
  timer.every(2000, sendStatus);  
}

void loop() {

  wifi_control();

  timer.tick(); // tick the timer
}
