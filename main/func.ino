
void SendConnectPacket(void) {
  GPRS.print("\r\nAT+CIPSEND\r\n");
  delay(3000);
  GPRS.write(0x10);
  MQTTProtocolNameLength = strlen(MQTTProtocolName);
  MQTTClientIDLength = strlen(MQTTClientID);
  MQTTUsernameLength = strlen(MQTTUsername);
  MQTTPasswordLength = strlen(MQTTPassword);
  datalength = MQTTProtocolNameLength + 2 + 4 + MQTTClientIDLength + 2 + MQTTUsernameLength + 2 + MQTTPasswordLength + 2;
  X = datalength;
  do {
    encodedByte = X % 128;
    X = X / 128;
    if ( X > 0 ) {
      encodedByte |= 128;
    }
    GPRS.write(encodedByte);
  }
  while ( X > 0 );
  GPRS.write(MQTTProtocolNameLength >> 8);
  GPRS.write(MQTTProtocolNameLength & 0xFF);
  GPRS.print(MQTTProtocolName);
  GPRS.write(MQTTLVL); // LVL
  GPRS.write(MQTTFlags); // Flags
  GPRS.write(MQTTKeepAlive >> 8);
  GPRS.write(MQTTKeepAlive & 0xFF);
  GPRS.write(MQTTClientIDLength >> 8);
  GPRS.write(MQTTClientIDLength & 0xFF);
  GPRS.print(MQTTClientID);
  GPRS.write(MQTTUsernameLength >> 8);
  GPRS.write(MQTTUsernameLength & 0xFF);
  GPRS.print(MQTTUsername);
  GPRS.write(MQTTPasswordLength >> 8);
  GPRS.write(MQTTPasswordLength & 0xFF);
  GPRS.print(MQTTPassword);
  GPRS.write(0x1A);

  Serial.println("connected");
}

void SendPublishPacket(void) {
  GPRS.print("\r\nAT+CIPSEND\r\n");
  delay(3000);
  memset(str, 0, 250);
  topiclength = sprintf((char*)topic, MQTTTopic);
  datalength = sprintf((char*)str, "%s%s", topic, Message);
  delay(1000);
  GPRS.write(0x30);
  X = datalength + 2;

  do {
    encodedByte = X % 128;
    X = X / 128;
    if ( X > 0 ) {
      encodedByte |= 128;
    }
    GPRS.write(encodedByte);
  }
  while ( X > 0 );
  GPRS.write(topiclength >> 8);
  GPRS.write(topiclength & 0xFF);
  GPRS.write(str);
  GPRS.write(0x1A);
  Serial.write(str);
  Serial.println("published");
}

void SendSubscribePacket(void) {
  GPRS.print("\r\nAT+CIPSEND\r\n");
  delay(3000);
  memset(str, 0, 250);
  topiclength = strlen(MQTTTopic);
  datalength = 2 + 2 + topiclength + 1;
  delay(1000);
  GPRS.write(0x82);
  X = datalength;
  do {
    encodedByte = X % 128;
    X = X / 128;
    if ( X > 0 ) {
      encodedByte |= 128;
    }
    GPRS.write(encodedByte);
  }
  while ( X > 0 );
  GPRS.write(MQTTPacketID >> 8);
  GPRS.write(MQTTPacketID & 0xFF);
  GPRS.write(topiclength >> 8);
  GPRS.write(topiclength & 0xFF);
  GPRS.print(MQTTTopic);
  GPRS.write(MQTTQOS);
  GPRS.write(0x1A);
}
