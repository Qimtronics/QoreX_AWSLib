/*
  Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
  Permission is hereby granted, free of charge, to any person obtaining a copy of this
  software and associated documentation files (the "Software"), to deal in the Software
  without restriction, including without limitation the rights to use, copy, modify,
  merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "WiFi.h"
#include <ArduinoJson.h>
#include "awsQims.h"

const char *WIFI_SSID = "RUT200_F1B0";
const char *WIFI_PASSWORD = "w2BRb6a4";

#define USER_PUBLISH_TOPIC      "myesp32/sensordummy"

awsQims awsQimsProtocol(&WiFi);

AWSQIms_Conn_Status connStatus;
AWSQims_ClientConn_Status clientStatus;
AWSQims_Publish_Status publishedStatus;

bool setupChecked = false;

void connectAWS()
{
  Serial.begin(115200);
  
  setupChecked = awsQimsProtocol.setup(WIFI_SSID, WIFI_PASSWORD, 20);
  if(setupChecked)
  {
    Serial.println("Setup OK");
  }
  else
  {
    Serial.println("Setup FAILED");
  }

  connStatus = awsQimsProtocol.tryingWifiConnection();
  if(connStatus == WIFI_CONNECTED)
  {
     Serial.println("MY IP Address:");
     Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("DEV FAILED TO CONNECT TO...");
  }

  clientStatus = awsQimsProtocol.tryingClientConnection();
  
  if(clientStatus == CLIENT_CONNECTED)
  {
    Serial.println("AWS IoT Connected!");
  }
  else
  {
    Serial.println("FAIL TO CONNECT WITH AWS");
  }

}

void publishMessage()
{
  bool statusPublish = false;
  
  StaticJsonDocument<200> doc;
  doc["time"] = millis();
  doc["sensor_a0"] = analogRead(0);
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client

  publishedStatus = awsQimsProtocol.publishMsg(USER_PUBLISH_TOPIC, jsonBuffer);

  if(publishedStatus == PUBLISHED_SUCCESSFULLY)
  {
    Serial.println("MESSAGE SUBCRIBED SUCCESSFULLY");
  }
  else
  {
    Serial.println("MESSAGE FAILED");
  }
}


void setup() {
  //Serial.begin(9600);
  connectAWS();
}

void loop() {
  publishMessage();
  connStatus = awsQimsProtocol.keepConnecting();

  if(connStatus == WIFI_CONNECTED)
  {
    Serial.println("Wifi Still Connected");
  }
  else
  {
    Serial.println("Wifi Fail to Connect");
  }  
  delay(2000);
}
