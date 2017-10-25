#include <ESP8266WiFi.h>


const char* ssid     = "NOVOHORIZONTE";
const char* password = "caico2015";


const char* host = "www.dweet.io";
//const char* thing  = "Sensor_NodeMCU";
//const char* thing_content = "Temperatura &Nome=Clara";


unsigned long previousMillis = 0; 
 
const long interval = 500;
 
int ledVerde = D7;
int ledVermelho = D6;
const int lm35 = A0;
double lm35Value;

const int selectB = D2;

const int noise = A0;
unsigned int noiseValue;
const int sampleWindow = 50;


void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(selectB, OUTPUT);
  pinMode(D1, OUTPUT);
  Serial.println();
  Serial.println();
  /*Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());*/
}


int value = 0;


void loop() {
  delay(1000);
/*
  Serial.print("connecting to ");
  Serial.println(host);

  // WIFI CONNECT 
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
*/
  ////////////////////////////////////////////////////////////
  digitalWrite(D2,HIGH);  //Noise
  unsigned long currentMillis = millis();
 
  if (currentMillis - previousMillis >= interval)
  {
     previousMillis = currentMillis;
     
     lm35Value = analogRead(lm35);
     lm35Value = 1000.0*lm35Value/1023.0;
     lm35Value = lm35Value/0.3125;
     lm35Value = lm35Value/10.0;
     
     Serial.println((int)lm35Value);
    // Serial.print((char)176);
    /*if(lm35Value <= 35.0)
    {
        analogWrite(ledVerde, 77);
        delay(100);
        analogWrite(ledVerde, 0);
        delay(100);
    }
    else if(lm35Value >=37.5)
    {
        analogWrite(ledVermelho, 77);
        delay(100);
        analogWrite(ledVermelho, 0);
        delay(100);
    }*/
  }
  ////////////////////////////////////////////////////////////////////////////
  digitalWrite(D1, HIGH);
  digitalWrite(D1, LOW);
  noiseValue=readNoise();
  digitalWrite(D1, HIGH);
  digitalWrite(D1, LOW);
  
  String url = "/dweet/for/";
  url += "Sensor_NodeMCU";
  url += "?";
  url += "Temperatura";
  url += "=";
  url += lm35Value;
  url += "&Ruido";
  url += "=";
  url += noiseValue;  
  
 
  Serial.print("Requesting URL: ");
  Serial.println(url);
 
  // This will send the request to the server
  /*client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  int timeout = millis() + 5000;
  while (client.available() == 0) {
    if (timeout - millis() < 0) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
 
  Serial.println();
  Serial.println("closing connection");*/
}


unsigned int readNoise(){
  digitalWrite(D2,LOW);  //Noise
  unsigned long startMillis= millis();  // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level
  unsigned int sample;
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
  
  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(noise);
    if (sample < 1024)  // toss out spurious readings
    {
       if (sample > signalMax)
       {
          signalMax = sample;  // save just the max levels
       }
       else if (sample < signalMin)
       {
          signalMin = sample;  // save just the min levels
       }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  return (peakToPeak);
}
