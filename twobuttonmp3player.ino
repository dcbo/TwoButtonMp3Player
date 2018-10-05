/*
  Zahnputzlied
  - copyright: Dario Carluccio 
               https://www.carluccio.de
  - history:   
    - 05.10.2018 initial release
  - Hardware: 
    - Arduino Nano v3 
    - DF-Player  
  - Purpose:
    - Dead Simple MP3-Player with only one Button    
  - Connection of GPIO:
     2 - Green Button (Play Song 1)
     2 - Red Button   (Pause / Resume)
     9 - LED Program Start
    10 - DF-Player TxD (SoftwareSerial RxD)
    11 - DF-Player RxD (SoftwareSerial TxD)
  - Connection of DF-Player:                       +------  ------+
    VCC  - Arduino VCC - VCC                 VCC  o|     +--+     |o Busy
    RX   - Arduino 11                         RX  o|              |o USB-
    TX   - Arduino 10                         TX  o|  ##########  |o USB+
    SPK1 - Speaker-                         DACR  o|  #        #  |o ADKEY2
    GND  - Arduino GND                      DACI  o|  #   SD   #  |o ADKEY1
    SPK2 - Speaker+                         SPK1  o|  #  Slot  #  |o IO2
                                             GND  o|  #        #  |o GND
                                            SPK2  o|  ##########  |o IO1
                                                   +--------------+
*/

/***************************
 * Include Libs
 **************************/
#include "SoftwareSerial.h"
#include <DFRobotDFPlayerMini.h>
#include <Bounce2.h>          // from: https://github.com/thomasfredericks/Bounce2

/***************************
 * GPIO Defines
 **************************/
// SoftwareSerial
#define SoftSerial_RxD       10
#define SoftSerial_TxD       11
// Buttons
#define BUTTON_GREEN          2
#define BUTTON_RED            3
#define BUTTON_AUX_1          4
#define BUTTON_AUX_2          5
#define BUTTON_AUX_3          6
#define NUM_BUTTONS           5

/***************************
 * global vars 
 **************************/
SoftwareSerial mySerial(SoftSerial_RxD, SoftSerial_TxD);
DFRobotDFPlayerMini myDFPlayer;
Bounce       * gButton     = new Bounce[NUM_BUTTONS]; // Buttons
boolean        g_isPlaying = false;

/*************************** 
 *  Initialize GPIO Ports
 **************************/
void initPorts() {
  // init Inputs
  pinMode(BUTTON_GREEN,  INPUT_PULLUP);
  pinMode(BUTTON_RED,    INPUT_PULLUP);
  pinMode(BUTTON_AUX_1,  INPUT_PULLUP);
  pinMode(BUTTON_AUX_2,  INPUT_PULLUP);
  pinMode(BUTTON_AUX_3,  INPUT_PULLUP);
}

/*************************** 
 *  Initialize Buttons
 **************************/
void initButtons() {
  gButton[0].attach(BUTTON_GREEN, INPUT_PULLUP);
  gButton[1].attach(BUTTON_RED,   INPUT_PULLUP);
  gButton[2].attach(BUTTON_AUX_1, INPUT_PULLUP);
  gButton[3].attach(BUTTON_AUX_2, INPUT_PULLUP);
  gButton[4].attach(BUTTON_AUX_3, INPUT_PULLUP);  
  for (int i = 0; i < NUM_BUTTONS; i++) {
    gButton[i].interval(100);              
  }
}


/*************************** 
 *  Setup
 **************************/
void setup() {
  // init Serial-Port
  Serial.begin(115200);  
  Serial.println(F("Init..."));

  // Init GPIO
  Serial.print(F(" GPIO... "));
  initPorts();
  Serial.println(F("done."));
  
    // Init Buttons
  Serial.print(F(" Buttons... "));  
  initButtons();
  Serial.println(F("done."));
  
  // Init Softwareserial
  Serial.print(F(" Softwareserial... "));  
  mySerial.begin (9600);
  Serial.println("done.");
  
  // Init DFPlayer
  Serial.print(F(" DF-Player... "));  
  if (!myDFPlayer.begin(mySerial)) {  
    Serial.println(F(" *** Unable to begin:"));
    Serial.println(F(" *** 1.Please recheck the connection!"));
    Serial.println(F(" *** 2.Please insert the SD card!"));
  }  
  myDFPlayer.volume(20);
  Serial.println(F("DFPlayer online."));
  
  
  // Init done.
  Serial.println("Init done.");
  Serial.println("---------------------");
  Serial.println("Starting Main Loop...");
}


/*************************** 
 *  Main Loop
 **************************/
void loop () { 
  // get button states and handle changes
  for (int i = 0; i < NUM_BUTTONS; i++)  {
    gButton[i].update();    
    if (gButton[i].fell() ) {            
        switch (i) {
        case 0:    // BUTTON_GREEN
          Serial.println(F("Green: Play 1"));
          myDFPlayer.play(1);  
          g_isPlaying = true;
          break;
        case 1:    // BUTTON_RED
          if (g_isPlaying) {
            Serial.println(F("Red: Pause"));
            myDFPlayer.pause();  
            g_isPlaying = false;
          } else {
            Serial.println(F("Red: Resume"));
            myDFPlayer.play();  
            g_isPlaying = true;
          }
          break;
        case 2:    // BUTTON_AUX2
          Serial.println(F("AUX 2"));
          break;
        case 3:    // BUTTON_AUX3
          Serial.println(F("AUX 3"));
          break;
        case 4:    // BUTTON_AUX4
          Serial.println(F("AUX 4"));
          break;
      }
    }
  }
}
