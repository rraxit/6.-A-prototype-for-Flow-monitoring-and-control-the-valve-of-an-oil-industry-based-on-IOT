#define SENSOR       A2

float voltage,turbidity;

void setup()
{
  Serial.begin(9600);
  pinMode(SENSOR,INPUT);
}

void loop()
{
  voltage=0.004888*analogRead(SENSOR);  //in V
  turbidity=-1120.4*voltage*voltage+5742.3*voltage-4352.9;  //in NTU
  if((voltage>=2.5)&(turbidity>=0))
  {
    Serial.println("Voltage="+String(voltage)+" V Turbidity="+String(turbidity)+" NTU");  
    delay(500);
  }
}
