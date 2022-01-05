#include <ThingerESP8266.h>
#include <ArduinoJson.h>

#define USERNAME "turbo"
#define DEVICE_ID "ridon"
#define DEVICE_CREDENTIAL "myZa6Yv7ngiE"

#define SSID "MERCUSYS_5A5A"
#define SSID_PASSWORD "rna12345"
#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);
int Temp;
int Flow;
int Turb;


ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void setup() {
Serial.begin(9600);
s.begin(9600);
while (!Serial) continue;
thing.add_wifi(SSID, SSID_PASSWORD);
pinMode(D0,OUTPUT);
thing["Temperature"] >> outputValue(Temp);
thing["Flow Speed"] >> outputValue(Flow);
thing["Turbidity"] >> outputValue(Turb);
thing["Valve"] << digitalPin(D0);

}

void loop() 
{
 
 StaticJsonBuffer<1000> jsonBuffer;
 JsonObject& root = jsonBuffer.parseObject(s); 
 if (root == JsonObject::invalid())
  {
    return;
  }
  Serial.println("JSON received and parsed");
  root.prettyPrintTo(Serial);
  Serial.println("");
  Serial.print("Temperature ");
  Temp=root["temp"];
  Serial.println(Temp);
  Serial.print("Flow Speed    ");
  Flow=root["flow"];
 Serial.println(Flow);
 Serial.print("Turbidity    ");
  Turb=root["turb"];
 Serial.println(Turb);
  Serial.println("");
  Serial.println("---------------------xxxxx--------------------");
 Serial.println("");
  thing.handle();
    

  
}
