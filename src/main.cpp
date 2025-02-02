#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>

#define SS_PIN D4
#define RST_PIN D3

const char* ssid = "TT_IOT";
const char* password = "ttiot2019";

MFRC522 mfrc522(SS_PIN, RST_PIN);
int statuss = 0;
int out = 0;
WiFiServer server(80);
String ID_TAG;

void readRFID(byte *buffer, byte bufferSize) {
  ID_TAG = "";
  for(byte i = 0;i<bufferSize; i++) {
    ID_TAG=ID_TAG+String(buffer[i], HEX);
  }
  
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Connecting to ID");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi is Connected");

  Serial.println("Server started");
  Serial.println(WiFi.localIP());
  delay(3000);
  Serial.println("Stick the RFID Card");
  server.begin();
}

void loop() {
  if(!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {

  }
  readRFID(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.print("UID RFID :");
  Serial.println(ID_TAG);

  WiFiClient client = server.available();
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection :close");
  client.println("Refresh : 10");
  client.println();
  client.println("<!DOCTYPE HMTL>");
  client.println("<p style='text-align: center;'><span style='font-size: x-large;'><strong>RFID Reader</strong></span></p>");
  client.println("<hr size='5px' color='red' width='30%' align='centre'>");
  client.print("<p style='text-align: center;'><span style='font-size: x-large;'>https://github.com/luckyseptian17</span></p>");
  client.print("<p style='text-align: center;'><span style='color:rgb(6, 6, 131);'><strong style='font-size: large;'>RFID= ");
  client.println(ID_TAG); 
  client.print("</p>");
  client.println("</html>");
  delay(2000); 
}