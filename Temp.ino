#include <SoftwareSerial.h>
#define SENSOR  A2
#define ONE_WIRE_BUS 4
#include <ArduinoJson.h>
#include <dht11.h>
#define DHT11PIN 10
dht11 DHT11;
SoftwareSerial s(5,6);
int Temp;


byte sensorInterrupt = 0;  
byte sensorPin       = 02;
float calibrationFactor = 4.5;
volatile byte pulseCount;  
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long oldTime;
int frac;
float voltage;
int turbidity;
void setup(void) 
{ 
 s.begin(9600);
 Serial.begin(9600); 
// Serial.println("Dallas Temperature IC Control Library Demo"); 
// sensors.begin(); 
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);
  pinMode(SENSOR,INPUT);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);

} 

void loop(void) 
{
  StaticJsonBuffer<600> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();
int chk = DHT11.read(DHT11PIN);
Temp=DHT11.temperature;



if((millis() - oldTime) > 1000)    
  { 
    detachInterrupt(sensorInterrupt);
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    oldTime = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
    unsigned int frac;
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate));  // Print the integer part of the variable
    Serial.print(".");             // Print the decimal point
    frac = (flowRate - int(flowRate)) * 10;
    Serial.print(frac, DEC) ;      // Print the fractional part of the variable
    Serial.print("L/min");
    // Print the number of litres flowed in this second
    Serial.print("  Current Liquid Flowing: ");             // Output separator
    Serial.print(flowMilliLitres);
    Serial.print("mL/Sec");
    Serial.print("  Output Liquid Quantity: ");             // Output separator
    Serial.print(totalMilliLitres);
    Serial.println("mL"); 
     pulseCount = 0;
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }
  voltage=0.004888*analogRead(SENSOR);  //in V
  turbidity=-1120.4*voltage*voltage+5742.3*voltage-4352.9; 
Serial.print("  turbidity: ");             // Output separator
Serial.print(turbidity);//in NTU
Serial.print(Temp); 
  if((voltage>=2.5)&(turbidity>=0))
  {
    Serial.println("Voltage="+String(voltage)+" V Turbidity="+String(turbidity)+" NTU");  
    delay(500);
  }
  root["temp"] = Temp;
  root["turb"] = turbidity;
  root["flow"] = flowRate;
 
if(s.available()>0)
{
 root.printTo(s);
}
delay(100);
}
void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}
