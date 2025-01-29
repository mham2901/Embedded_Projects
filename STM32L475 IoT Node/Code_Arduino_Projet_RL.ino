//-------SENSORS INCLUDES----

#include <stm32l475e_iot01.h>

#include <stm32l475e_iot01_accelero.h>
#include <stm32l475e_iot01_gyro.h>
#include <stm32l475e_iot01_hsensor.h>
#include <stm32l475e_iot01_magneto.h>
#include <stm32l475e_iot01_psensor.h>
#include <stm32l475e_iot01_qspi.h>
#include <stm32l475e_iot01_tsensor.h>



#include <Wire.h>
#include <SPI.h>
#include <WiFiST.h>



//-------WIFI INCLUDES-------

#include <SPI.h>

//-------MQTT INCLUDES-------

#include <MQTT.h>
#include <MQTTClient.h>

//-------GLOBAL VARIABLES----

//*********Connection********

WiFiClient net;
MQTTClient client;


SPIClass SPI_3(PC12, PC11, PC10);

WiFiClass WiFi(&SPI_3, PE0, PE1, PE8, PB13);


char ssid[] = "Anis";    //  network name
char pass[] = "12345678";           //  network password



char serverIPAddress[] = "broker.hivemq.com";//  IP address of MQTT broker(a.k.a server)
char Port[]="1883";                       //  Port
char login[] = "chaima";         //  Login of server




char clientName[] = "DIS_L4IOT";         //  Name of board(a.k.a client)
int status = WL_IDLE_STATUS;             



//**********Sensors**********


float fTemperatureValue,fHumidityValue,fPressureValue;  //Sensor float variables
char data[1000];


//-------SETUP---------------

void setup() {
  
  initSerial();     // initialize serial communication and wait for serial monitor to open
  
  
  initWifi();   // initialize the WiFi module

  
  initSensors();    // initialize sensors

  
  connectToBroker();// connect to MQTT broker
}

//-------LOOP----------------

void loop() {
  reconnect();
  publishTemperature();
  publishHumidity();
  publishPressure();


}

//-------FUNCTION DEFINITIONS----
//---------------INIT------------
void initSerial(){
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
}

void initWifi(){
  Serial.println("OK");
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi module not detected");
    // don't continue:
    while (true);
  }

  // Check firmware version:
  String fv = WiFi.firmwareVersion();
  if (fv != "C3.5.2.5.STM") {
    Serial.println("Please upgrade the firmware");
  }

  // Attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA/WPA2 network: ");
    Serial.println(ssid);
    // Connect to WPA2 network:
    status = WiFi.begin(ssid, pass);
  
    if (status != WL_CONNECTED) {
      // Connect to WPA (TKIP) network:
//      status = WiFi.begin(ssid, pass, ES_WIFI_SEC_WPA);
status = WiFi.begin(ssid, pass);
    }

    // wait 10 seconds for connection:
    delay(1000);
  }

  // you're connected now, so print out the connection info:
  Serial.println("Connected.");
}

void initSensors(){
  BSP_TSENSOR_Init();
  BSP_HSENSOR_Init();
  BSP_PSENSOR_Init();
  BSP_MAGNETO_Init();
  BSP_GYRO_Init();
  BSP_ACCELERO_Init(); 
}

void connectToBroker() {
  Serial.print("Checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  client.begin(serverIPAddress, net);
  Serial.print("\nConnecting to MQTT Broker...");
  while (!client.connect(clientName,login))
  {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");



}

//---------------LOOP------------

void publishTemperature(){
 fTemperatureValue = BSP_TSENSOR_ReadTemp();
 Serial.print("\n ******Temperature****** \n Temperature = ");
 Serial.print(fTemperatureValue);
 sTemperatureValue = String(fTemperatureValue, 1);
 delay(10000);
 client.publish("station/temperature", sTemperatureValue);
}


void publishHumidity(){
 fHumidityValue = BSP_HSENSOR_ReadHumidity();
 Serial.print("\n ******Humidity****** \n Humidity = ");
 Serial.print(fHumidityValue);
 sHumidityValue = String(fHumidityValue, 1);
 delay(10000);
 client.publish("station/humidite", sHumidityValue);
}

void publishPressure(){
 fPressureValue = BSP_PSENSOR_ReadPressure();
 Serial.print("\n ******Pressure****** \n Pressure = ");
 Serial.print(fPressureValue);
 sPressureValue = String(fPressureValue, 1);
 delay(10000);
 client.publish("station/pression", sPressureValue);
}




void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
//    Serial.print("Connection lost,Attempting to reconnect...");
    if (client.connect(clientName, login)) {
//      Serial.println("Connected");
      } else {
      Serial.print("Failed ,will try again in 5 seconds");
      // Wait 5 seconds before retrying 
      delay(5000);
    }
  }
  
  snprintf (data, 100, "{\"T\":%u,\"H\":%u,\"P\":%u}",
  (unsigned int)fTemperatureValue,(unsigned int)fHumidityValue,(unsigned int)fPressureValue);
  client.publish("/Sensors", data);
}
