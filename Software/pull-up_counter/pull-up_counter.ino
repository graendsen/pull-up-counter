#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>

#include "LedMatrix.h"
#include "Adafruit_VCNL4010.h"
#include "LedControl.h"

#define DOWN_BUTTON 4
#define RESET_BUTTON 3
#define UP_BUTTON 2
#define LM_CS_PIN 9
#define SEG_CS_PIN 10

LedMatrix leds(LM_CS_PIN, false);                   // Success LEDs
LedControl segment=LedControl(11,13,SEG_CS_PIN,1);  // 7-segment display
Adafruit_VCNL4010 vcnl;                             // Proximity sensor

int delayLeftRight = 60;        // Delay between lighting up each column of success LEDs
int detectedDistance = 2350;    // Raw distance number from proximity sensor. Numbers lower than this will result in detection.
int delayAfterPullup = 500;     // Delay (in milliseconds) after pullup

int detectedState = 0;
int lastDetectedState = 0;

uint16_t pullupCount = (uint16_t)EEPROM.read(0) |
                       ((uint16_t)EEPROM.read(1) << 8);


void setup() {

    Serial.begin(9600);
    Serial.println("Pull-up counter start.");
    
    pinMode(DOWN_BUTTON,INPUT);
    pinMode(RESET_BUTTON,INPUT);
    pinMode(UP_BUTTON,INPUT);
    
    segInit();
    segNum(pullupCount);

    if (! vcnl.begin()){
        Serial.println("Proximity sensor not found :(");
        while (1);
    }

    Serial.println("Found proximity sensor.");
}

void loop() {

    buttonCheck();

    if (vcnl.readProximity() < detectedDistance){
        detectedState = false;
    }else{
        detectedState = true;
    }

    if (detectedState != lastDetectedState){
        if (detectedState == true) {
            pullupCount++;
  
            segInit();
            segNum(pullupCount);
            segClose();
        
            ledsInit();
            ledsLeftRight(30);
            ledsClose();
            
            EEPROM.put(0,pullupCount);
            
        }
    delay(delayAfterPullup);
    
    }

    lastDetectedState = detectedState;
  
}

void buttonCheck(){
    int downState = digitalRead(DOWN_BUTTON);
    int upState = digitalRead(UP_BUTTON);
    int resetState = digitalRead(RESET_BUTTON);
    
    if (downState == HIGH){
        pullupCount--;
        segInit();
        segNum(pullupCount);
        segClose();
        delay(150);
    }
    if (upState == HIGH){
        pullupCount++;
        segInit();
        segNum(pullupCount);
        segClose();
        delay(150);
    }
    if (resetState == HIGH){
        delay(2000);
        if (resetState == HIGH){
            pullupCount = 0;
            segInit();
            segNum(pullupCount);
            segClose();
            delay(150);
        }
    }
}

void segNum(int number){
    segment.shutdown(0,false);   // MAX7221 is in power saving mode, so we have to wake it up
    segment.setIntensity(0,15);  // Setting brightness, max 15
    segment.clearDisplay(0);     // clearing display

    segment.setDigit(0,3,number % 10, false);
    number /=10;
    segment.setDigit(0,2,number % 10, false);
    number /=10;
    segment.setDigit(0,1,number % 10, false);
    number /=10;
    segment.setDigit(0,0,number % 10, false);
    number /=10;
    return 0;
}

void segInit(){
    segment.shutdown(0,false);   // MAX7221 is in power saving mode, so we have to wake it up
    segment.setIntensity(0,15);  // Setting brightness, max 15
    segment.clearDisplay(0);     // clearing display
}

void segClose(){
    SPI.end();
}

void ledsInit(){
    leds.begin();
    leds.setIntensity(3);
}

void ledsClose(){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){    
            leds.clearBufferPixel(i, j);
        }
    }
    leds.sendBuffer();  
    SPI.end();
}

void ledsLeftRight(int dly){
    leds.setBufferPixel(2, 0);
    leds.setBufferPixel(7, 0);
    leds.setBufferPixel(7, 1);     
    leds.setBufferPixel(7, 2);
    leds.setBufferPixel(7, 3);
    leds.setBufferPixel(2, 3);   
    leds.setBufferPixel(2, 4);
    leds.setBufferPixel(7, 4);
    leds.setBufferPixel(7, 5);   
    leds.setBufferPixel(7, 6);
    leds.setBufferPixel(7, 7);
    leds.setBufferPixel(2, 7);
    leds.sendBuffer();    
    delay(dly);
    leds.setBufferPixel(1, 0);
    leds.setBufferPixel(6, 0);
    leds.setBufferPixel(6, 1);
    leds.setBufferPixel(6, 2);
    leds.setBufferPixel(6, 3);
    leds.setBufferPixel(1, 3);
    leds.setBufferPixel(1, 4);
    leds.setBufferPixel(6, 4);
    leds.setBufferPixel(6, 5); 
    leds.setBufferPixel(6, 6); 
    leds.setBufferPixel(1, 7);
    leds.setBufferPixel(6, 7);
    leds.sendBuffer();  
    delay(dly);
    leds.setBufferPixel(0, 0);
    leds.setBufferPixel(5, 0); 
    leds.setBufferPixel(5, 1);
    leds.setBufferPixel(5, 2);
    leds.setBufferPixel(5, 3);
    leds.setBufferPixel(0, 3);
    leds.setBufferPixel(0, 4); 
    leds.setBufferPixel(5, 4);
    leds.setBufferPixel(5, 5);
    leds.setBufferPixel(5, 6);
    leds.setBufferPixel(5, 7);
    leds.setBufferPixel(0, 7);
    leds.sendBuffer();  
    delay(dly);
    leds.setBufferPixel(4, 0);
    leds.setBufferPixel(4, 1);
    leds.setBufferPixel(4, 2);
    leds.setBufferPixel(4, 3);
    leds.setBufferPixel(4, 4);
    leds.setBufferPixel(4, 5);
    leds.setBufferPixel(4, 6);
    leds.setBufferPixel(4, 7);
    leds.sendBuffer();  
    delay(dly);
    leds.setBufferPixel(3, 0);
    leds.setBufferPixel(3, 1);
    leds.setBufferPixel(3, 2);
    leds.setBufferPixel(3, 3);
    leds.setBufferPixel(3, 4);
    leds.setBufferPixel(3, 5);  
    leds.setBufferPixel(3, 6);
    leds.setBufferPixel(3, 7);
    leds.sendBuffer();  
    delay(dly);
    leds.setBufferPixel(2, 1);
    leds.setBufferPixel(2, 2);
    leds.setBufferPixel(2, 5);
    leds.setBufferPixel(2, 6);
    leds.sendBuffer();  
    delay(dly);
    leds.setBufferPixel(1, 1);
    leds.setBufferPixel(1, 2);
    leds.setBufferPixel(1, 5);
    leds.setBufferPixel(1, 6);
    leds.sendBuffer();  
    delay(dly);
    leds.setBufferPixel(0, 1);
    leds.setBufferPixel(0, 2);
    leds.setBufferPixel(0, 5);
    leds.setBufferPixel(0, 6);
    leds.sendBuffer();  
    dly = dly/2;
    delay(dly);
    leds.clearBufferPixel(2, 0);
    leds.clearBufferPixel(7, 0);
    leds.clearBufferPixel(7, 1);     
    leds.clearBufferPixel(7, 2);
    leds.clearBufferPixel(7, 3);
    leds.clearBufferPixel(2, 3);   
    leds.clearBufferPixel(2, 4);
    leds.clearBufferPixel(7, 4);
    leds.clearBufferPixel(7, 5);   
    leds.clearBufferPixel(7, 6);
    leds.clearBufferPixel(7, 7);
    leds.clearBufferPixel(2, 7);
    leds.sendBuffer();    
    delay(dly);
    leds.clearBufferPixel(1, 0);
    leds.clearBufferPixel(6, 0);
    leds.clearBufferPixel(6, 1);
    leds.clearBufferPixel(6, 2);
    leds.clearBufferPixel(6, 3);
    leds.clearBufferPixel(1, 3);
    leds.clearBufferPixel(1, 4);
    leds.clearBufferPixel(6, 4);
    leds.clearBufferPixel(6, 5); 
    leds.clearBufferPixel(6, 6); 
    leds.clearBufferPixel(1, 7);
    leds.clearBufferPixel(6, 7);
    leds.sendBuffer();  
    delay(dly);
    leds.clearBufferPixel(0, 0);
    leds.clearBufferPixel(5, 0); 
    leds.clearBufferPixel(5, 1);
    leds.clearBufferPixel(5, 2);
    leds.clearBufferPixel(5, 3);
    leds.clearBufferPixel(0, 3);
    leds.clearBufferPixel(0, 4); 
    leds.clearBufferPixel(5, 4);
    leds.clearBufferPixel(5, 5);
    leds.clearBufferPixel(5, 6);
    leds.clearBufferPixel(5, 7);
    leds.clearBufferPixel(0, 7);
    leds.sendBuffer();  
    delay(dly);
    leds.clearBufferPixel(4, 0);
    leds.clearBufferPixel(4, 1);
    leds.clearBufferPixel(4, 2);
    leds.clearBufferPixel(4, 3);
    leds.clearBufferPixel(4, 4);
    leds.clearBufferPixel(4, 5);
    leds.clearBufferPixel(4, 6);
    leds.clearBufferPixel(4, 7);
    leds.sendBuffer();  
    delay(dly);
    leds.clearBufferPixel(3, 0);
    leds.clearBufferPixel(3, 1);
    leds.clearBufferPixel(3, 2);
    leds.clearBufferPixel(3, 3);
    leds.clearBufferPixel(3, 4);
    leds.clearBufferPixel(3, 5);  
    leds.clearBufferPixel(3, 6);
    leds.clearBufferPixel(3, 7);
    leds.sendBuffer();  
    delay(dly);
    leds.clearBufferPixel(2, 1);
    leds.clearBufferPixel(2, 2);
    leds.clearBufferPixel(2, 5);
    leds.clearBufferPixel(2, 6);
    leds.sendBuffer();  
    delay(dly);
    leds.clearBufferPixel(1, 1);
    leds.clearBufferPixel(1, 2);
    leds.clearBufferPixel(1, 5);
    leds.clearBufferPixel(1, 6);
    leds.sendBuffer();  
    delay(dly);
    leds.clearBufferPixel(0, 1);
    leds.clearBufferPixel(0, 2);
    leds.clearBufferPixel(0, 5);
    leds.clearBufferPixel(0, 6);
    leds.sendBuffer();  
}
