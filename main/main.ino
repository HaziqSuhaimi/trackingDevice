#include <SoftwareSerial.h>
#include <TinyGPS++.h>

SoftwareSerial GPRS(10, 11); // RX, TX
SoftwareSerial SerialGPS(2, 3); //rx,tx
TinyGPSPlus gps;

unsigned char Message[30]; //your massage to send
unsigned long datalength, CheckSum, RLength;
unsigned short topiclength;
unsigned char topic[30];
char str[250];
unsigned char encodedByte;
int X;
unsigned short MQTTProtocolNameLength;
unsigned short MQTTClientIDLength;
unsigned short MQTTUsernameLength;
unsigned short MQTTPasswordLength;
const char MQTTHost[30] = "farmer.cloudmqtt.com"; // server name
const char MQTTPort[10] = "14433"; // port number
const char MQTTClientID[20] = "adi"; // anything that unique to the device
const char MQTTTopic[30] = "device1"; // unique topic name
const char MQTTProtocolName[10] = "MQIsdp"; //dont change if u r using cloudMQTT
const char MQTTLVL = 0x03;
const char MQTTFlags = 0xC2;
unsigned short MQTTKeepAlive = 60;
const char MQTTUsername[30] = ""; // instance username.. check in instance's details
const char MQTTPassword[35] = ""; // instance password
const char MQTTQOS = 0x00;
char MQTTPacketID = 0x0001;

float flon, flat;

String gprsStr = "";
int index = 0;
byte data1;
boolean gprsReady = false;

void setup() {
  GPRS.begin(9600);
  Serial.begin(9600);
  SerialGPS.begin(9600);
  delay(3000);
  Serial.println("Starting GSM and GPS...");
}

void loop() {
  GPRS.print("AT+CSTT=\"diginet\",\"\",\"\"\r\n"); //"AT+CSTT=\"<YOUR TELCO APN>\",\"\",\"\"\r\n"
  delay(1000);
  GPRS.print("AT+CIPMODE=0\r\n");
  delay(1000);
  GPRS.print("AT+CIICR\r\n");
  delay(5000);
  GPRS.print("AT+CIPSTART=\"TCP\",\"farmer.cloudmqtt.com\",\"14433\"\r\n"); //"AT+CIPSTART=\"TCP\",\"<SERVER NAME>\",\"<SERVER PORT>\"\r\n"
  delay(4000);
  SendConnectPacket();

  gprsReady = isGPRSReady();
  while (gprsReady) {
    while (SerialGPS.available()) {
      gps.encode(SerialGPS.read());
    }
    if (gps.location.isUpdated()) {
      flon = gps.location.lng();
      flat = gps.location.lat();
//      Serial.println(flon,6); //check long,lat value
//      Serial.println(flat,6);
      String data = String(flon, 6) + "," + String(flat, 6); //longlat
      data.toCharArray(Message, 30);
      SendPublishPacket();
    }
  }
}

boolean isGPRSReady() {
  GPRS.println("AT");
  GPRS.println("AT");
  GPRS.println("AT+CGATT?");
  index = 0;
  while (GPRS.available()) {
    data1 = (char)GPRS.read();
    Serial.write(data1);
    gprsStr[index++] = data1;
  }
  Serial.println("Check OK");
  Serial.print("gprs str = ");
  Serial.println(data1);
  if (data1 > -1) {
    Serial.println("GPRS OK");
    return true;
  }
  else {
    Serial.println("GPRS NOT OK");
    return false;
  }
}
