#ifndef awsQims_h
#define awsQims_h

//#include "Arduino.h"
#include "stdint.h"
#include "stdbool.h"
//#include <pgmspace.h>

//some additional libraries need for this lib to work properly
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include "WiFi.h"

#define READ_BUFFER_SIZE            256

typedef enum
{
   WIFI_CONNECTED = 0,
   WIFI_CONNECTION_LOST, 
   WIFI_DISCONNECTED, 
   WIFI_FAILED,
}AWSQIms_Conn_Status;

typedef enum
{
    CLIENT_CONNECTED = 0,
    CLIENT_DISCONNECTED,
}AWSQims_ClientConn_Status;


typedef enum
{
    PUBLISHED_SUCCESSFULLY = 0,
    PUBLISHED_FAILED,
    UNDEFINED_TOPIC,
    DATA_TOPIC_IS_NULL,
}AWSQims_Publish_Status;


class awsQims{
    public:
        awsQims(WiFiClass *_wifi_hw);

        bool setup(const char *_ssid = nullptr, const char *_pass = nullptr, uint8_t timeTryingToConnect = 0);

        AWSQIms_Conn_Status tryingWifiConnection();
        AWSQIms_Conn_Status connectionStatus();
        AWSQims_ClientConn_Status tryingClientConnection();
        AWSQims_Publish_Status publishMsg(String _userTopic, char _userPayload[]);
        AWSQIms_Conn_Status keepConnecting();

    private:    
        WiFiClientSecure net;    //= WiFiClientSecure();
        MQTTClient mqttClient;   //= MQTTClient(READ_BUFFER_SIZE);
        WiFiClass *wifiUser;

        const char *ssidUser;
        const char *passUser = nullptr;
        uint8_t tryToConnect;
};

#endif //awsQims_h
