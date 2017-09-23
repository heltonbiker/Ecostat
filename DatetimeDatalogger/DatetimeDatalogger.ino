#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;
const int chipSelect = 4;
File dataFile;

const int LM35 = A0;

const int ONE_SECOND_INTERVAL = 1000;
long lastMillis = 0;

typedef union {
  uint32_t val;
  byte buf[4];
} bufferedUint;

bufferedUint timestamp;


typedef union {
 float val;
 byte buf[4];
} bufferedFloat;

bufferedFloat temperatura;


float voltage = 1.1;
int range = 1023;
float factor = 0.01;


void setup () {
    
    Wire.begin();
    rtc.begin();
    Serial.begin(9600);
    analogReference(INTERNAL);
    
    if (! rtc.isrunning()) {
        Serial.println("Adjusting RTC");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // date and time THIS script has been compiled
    }
    
    pinMode(10, OUTPUT); // necessary for SD library
    pinMode(LED_BUILTIN, OUTPUT);
       
    SdInit();
}

void SdInit()
{
    if (!SD.begin(chipSelect)) {
      Serial.println("Card failed, or not present");
      return;
    }

    dataFile = SD.open("log.dat", FILE_WRITE);
    Serial.println("card initialized.");
}

void loop () 
{
    if (millis() - lastMillis > ONE_SECOND_INTERVAL)
    {
        log();
        lastMillis += ONE_SECOND_INTERVAL;
    }
    delay(20);
}

void log()
{
    if (dataFile)
    {
        DateTime now = rtc.now();    
        timestamp.val = now.unixtime();
    
        temperatura.val = (float(analogRead(LM35))*voltage/(range))/factor;

        dataFile.write(timestamp.buf, sizeof(timestamp.buf));
        dataFile.write(temperatura.buf, sizeof(temperatura.buf));
        dataFile.flush();
        blinkNotify();
        Serial.println(String(timestamp.val));
        Serial.println(String(temperatura.val));
    }
    else
    {
      SdInit();
    }

}

void blinkNotify()
{
    int duration = 1;
    blink(LED_BUILTIN, duration);
    delay(duration);
    blink(LED_BUILTIN, duration);
}

void blink(int pin, int duration)
{
    digitalWrite(pin, HIGH);
    delay(duration);
    digitalWrite(pin, LOW);  
}