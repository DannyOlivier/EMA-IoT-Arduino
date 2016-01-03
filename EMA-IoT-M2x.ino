#include <jsonlite.h>
#include <Console.h>
#include <Process.h>
#include <YunClient.h>
#include "M2XStreamClient.h"

// M2X Device Config Variables
char deviceId[] = "282a67bdca7eacce13c5c59dfede9ab6"; // Device you want to push to
char bodyTemp[] = "BodyTemp";
char hartPulseRate[] = "PulseRate";
char bloodPreasure[] = "BloodPreasure";
char m2xKey[] = "252f2167f6130d1ae560eb30d57f0ba8"; // Your M2X access key
int  m2xResponse = 0;
YunClient client;
M2XStreamClient m2xClient(&client, m2xKey);
char fromTime[] = "2016-01-01T00:00:00.00"; // yyyy-mm-ddTHH:MM:SS.SSSZ
char endTime[] = "2017-01-04T00:00:00.00"; // yyyy-mm-ddTHH:MM:SS.SSSZ

// Sensors Analog Port Config
const int bodyTempPin = A0;
const int hartPulseRatePin = A1;
const int bloodPreasurePin = A2;
      int sensorVal = 0;
unsigned long timeLong = 0;
      char firstTimeFlag = '1';
      char fiveMinFlag = '1';

void setup() {

  // Bridge Initaliazing Block
  Bridge.begin();   // Initialize Bridge

  // Arduino YUN delay to ensure correct Initialization
  delay(2000);

  //Delete previous data
  m2xClient.deleteValues(deviceId,bodyTemp,fromTime,endTime);
  m2xClient.deleteValues(deviceId,hartPulseRate,fromTime,endTime);
  m2xClient.deleteValues(deviceId,bloodPreasure,fromTime,endTime);
}

void loop() {
  
      if(fiveMinFlag == '0'){
           unsigned long tempTime = millis();
           if(tempTime >= timeLong){
            fiveMinFlag = '1';
           }
            
      }else{
          timeLong = millis()+600000;
          fiveMinFlag = '0';
          m2xClient.deleteValues(deviceId,bodyTemp,fromTime,endTime);
          m2xClient.deleteValues(deviceId,hartPulseRate,fromTime,endTime);
          m2xClient.deleteValues(deviceId,bloodPreasure,fromTime,endTime);
      }
      

      // Measuring & Reporting Body Temperature
      sensorVal = analogRead(bodyTempPin);
      m2xResponse = m2xClient.updateStreamValue(deviceId, bodyTemp, ((sensorVal+20)/15));

      // Measuring & Reporting Hart Pulse Rate
      sensorVal = analogRead(hartPulseRatePin);
      m2xResponse = m2xClient.updateStreamValue(deviceId, hartPulseRate, ((sensorVal*2)/8));
      

      // Measuring & Reporting Blood Preasure
      sensorVal = analogRead(bloodPreasurePin);
      m2xResponse = m2xClient.updateStreamValue(deviceId, bloodPreasure, sensorVal/2); 

      //Reporting GPS Location   
      double latitude = 20.653065047944413;
      double longitude = -103.39148998304154;
      double elevation = 0.0;
      m2xResponse = m2xClient.updateLocation(deviceId,"EMA-IoT-GDL" , latitude, longitude, elevation);
      Serial.println("M2x-DEviceLocation:");
      Serial.println(m2xResponse);

  }

